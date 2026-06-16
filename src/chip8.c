#include <stdio.h>      // standard input/output library
#include <string.h>     // string and memory util
#include <stdlib.h>     // standard library
#include "chip8.h"      // chip8 header file

// Font data — 16 characters, 5 bytes each
// Each byte is a row of 8 pixels, only upper 4 bits used
static const uint8_t font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// zero everything out, set PC to 0x200, copy font data into RAM at 0x050
void chip8_init(Chip8 *c) {
    // 1. zero everything
    memset(c, 0, sizeof(*c)); // memset(ptr, value, size);

    // 2. set PC
    c->pc = 0x200; // set PC to 0x200 (where ROM loads)
    //printf("PC: %x", c->pc); 

    // 3. copy font into ram
    memcpy(c->ram + 0x050, font, sizeof(font)); // memcpy(dest, src, size);
}

// open the file, read bytes into RAM starting at 0x200
void chip8_load_rom(Chip8 *c, const char *romPath) {
    FILE *f = fopen(romPath, "rb"); // open rom, "rb" = read binary

    if (f == NULL) { // handle failure
        printf("Failed to open ROM\n");
        exit(1);
    }

    fseek(f, 0, SEEK_END);                  // seek end of file
    long size = ftell(f);                   // get size before rewind
    rewind(f);                              // seek back to beginning
    fread(c->ram + 0x200, 1, size, f);      // read into ram 
    fclose(f);                              // close the file
}

// fetch-decode-execute loop
void chip8_cycle(Chip8 *c) {
    // ---- Fetch ----
    // read 2 bytes from RAM at PC
    uint8_t opHigh = c->ram[c->pc];       // read first byte
    uint8_t opLow = c->ram[c->pc + 1];    // read second byte

    // combine into 16-bit opcode
    uint16_t opcode = ((uint16_t)opHigh << 8) | opLow;  // shift opHigh left by 8 bits (XXXX0000), then combine (OR) with opLow

    // advance PC by 2
    c->pc += 2;

    // ---- Decode ----
    // extract nibbles from opcode
    // mask needed byte with &, shift left to isolate it
    // ex: (0x1234) 0001 0010 0011 0100 & (0xF000) = 1111 0000 0000 000 >> 12 = 0001 0000 0000 0000
    uint8_t     type =  (opcode & 0xF000) >> 12;    // bits 0-3 indicate opcode type
    uint8_t     X =     (opcode & 0x0F00) >> 8;     // 2nd nibble, looks up Vx
    uint8_t     Y =     (opcode & 0x00F0) >> 4;     // 3rd nibble, looks up Vy
    uint8_t     N =     (opcode & 0x000F);          // 4th nibble, 4-bit number
    uint8_t     NN =    (opcode & 0x00FF);          // 3rd and 4th nibbles, 8-bit
    uint16_t    NNN =   (opcode & 0x0FFF);          // 2nd, 3rd, and 4th nibbles, 12-bit

    // ---- Execute ----
    // switch on first nibble (type)
    switch (type) 
    {
        case 0x0:
            switch (NN) 
            {
                case 0xE0: 
                    // 00E0 - clear display
                    memset(c->display, 0, sizeof(c->display));
                    break;
                
                case 0xEE:
                    // 00EE - return from subroutine 
                    c->sp--;   //Decrement sp 
                    c->pc = c->stack[c->sp]; // Pop PC at stack location sp
                break;
            }
            break;

        case 0x1:
            // 1NNN - jump
            c->pc = NNN; 
            break;

        case 0x2:
            // 2NNN - call subroutine
            c->stack[c->sp] = c->pc; // Store value of PC to address of stack at SP
            c->sp++; // Increment sp
            c->pc = NNN; // Jump to NNN (subroutine)
            break;

        case 0x3:
            // 3XNN - skips if VX == NN
            if (c->V[X] == NN)
            {
                c->pc += 2;
            }
            break;

        case 0x4:
            // 4XNN - skips if VX != NN
            if (c->V[X] != NN)
            {
                c->pc += 2;
            }
            break;

        case 0x5:
            // 5XY0 - skips if VX == VY
            if (c->V[X] == c->V[Y])
            {
                c->pc += 2;
            }
            break;

        case 0x6:
            // 6XNN - set VX to NN
            c->V[X] = NN;
            break;

        case 0x7:
            // 7XNN — add NN to register VX
            c->V[X] += NN;
            break;

        case 0x8:
        // 8XYN - ALU: performs bitwise operations and arithmetic
            switch (N)
            {
                case 0x0:
                    // VX = VY
                    c->V[X] = c->V[Y];
                    break;

                case 0x1:
                    // VX OR VY
                    c->V[X] |= c->V[Y];
                    break;

                case 0x2:
                    // VX AND VY
                    c->V[X] &= c->V[Y];
                    break;

                case 0x3:
                    // VX XOR VY
                    c->V[X] ^= c->V[Y];
                    break;

                case 0x4:
                    // VX + VY, VF = 1 if overflow
                    uint16_t sum = c->V[X] + c->V[Y];   // 16 bit, holds up to 510
                    c->V[X] = sum & 0xFF;   // Put lower byte in V[X]
                    c->V[0xF] = (sum > 255) ? 1 : 0;    // carry flag, set 1 if sum > 255, otherwise 0
                    break;

            //     case 0x5:
            //         // VX - VY, VF = 1 if VX >= VY
            //         uint8_t diff = c->V[X] - c->V[Y]; 
            //         c->V[0xF] = (c->V[X] >= c->V[Y]) ? 1 : 0; 
            //         break;

            //     case 0x6:s
            //         // VF = (VX AND 1), then VX right shift 1

            //         break;

            //     case 0x7:
            //         // VX = VY - VX, VF = 1 if VY >= VX

            //         break;

            //     case 0xE:
            //         // VF = ((VX right shift 7) AND 1), then VX left shit 1

            //         break;
            // }
            // break;

        case 0x9:
            // 9XY0 - skips if VX != VY
            if (c->V[X] != c->V[Y])
            {
                c->pc += 2;
            }
            break;

        case 0xA:
            // ANNN — set index register I to NNN
            c->I = NNN;
            break;

        case 0xD:
            // DXYN - draw sprite
            // draws an N-pixel-tall sprite at screen position (VX, VY), 
            // reading sprite data from RAM at address I
            uint8_t xPos = c->V[X] % 64;
            uint8_t yPos = c->V[Y] % 32;
            c->V[0xF] = 0;

            for (int row = 0; row < N; row++) 
            {
                uint16_t sprite = c->ram[c->I + row];

                for (int col = 0; col < 8; col++) 
                {
                    if (sprite & (0x80 >> col)) 
                    {
                        int idx = (yPos + row) * 64 + (xPos + col);
                        if (c->display[idx]) c->V[0xF] = 1;
                        c->display[idx] ^= 1;
                    }
                }
            }

            c->draw_flag = 1;
            break;

        default:
            printf("Unknown opcode: 0x%04x\n", opcode);
            break;
    }
}