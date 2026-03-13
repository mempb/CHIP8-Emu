#include <stdio.h>
#include "chip8.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    Chip8 c;
    chip8_init(&c);

    char path[] = "C://Users/mburroughs/Desktop/Chip-8/ROMs/IBM_logo.ch8";
    chip8_load_rom(&c, path);

    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
        }
        chip8_cycle(&c);
    }
}

// TODOS:
// - Setup environment on PC
// - Understand and rewrite SDL2 code, read docs
// - Understand and rewrite CMakeLists.txt file
// - Make SDL folder, give its own CMakeLists.txt file
// - Implement draw opcode, get IBM logo running
// - Implement remaining opcode