#include <stdint.h> // for uint

typedef struct {
    uint8_t     ram[4096];          // 4KB memory
    uint8_t     display[64 * 32];   // pixel buffer, 1 byte per pixel
    uint16_t    stack[16];          // call stack
    uint8_t     sp;                 // stack pointer
    uint16_t    pc;                 // program counter
    uint16_t    I;                  // index register
    uint8_t     V[16];              // general purpose registers V0-VF
    uint8_t     delay_timer;
    uint8_t     sound_timer;
    uint8_t     keypad[16];         // key states
    uint8_t     draw_flag;          // set to 1 when display needs redrawing
} Chip8;

void chip8_init(Chip8 *c);
void chip8_load_rom(Chip8 *c, const char *romPath);
void chip8_cycle(Chip8 *c);