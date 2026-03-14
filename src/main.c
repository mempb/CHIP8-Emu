#include <stdio.h>
#include "chip8.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    printf("SDL Initialized\n");

    SDL_Window *window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    printf("SDL Window Rendered\n");

    Chip8 c;
    chip8_init(&c);
    printf("CHIP-8 Initialized\n");

    char path[] = "C:/Dev/Code/CHIP-8/ROMs/IBM_logo.ch8";
    printf("ROM Path: %s\n", path);
    chip8_load_rom(&c, path);
    printf("ROM Loaded\n");

    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
        }
        chip8_cycle(&c);

        if (c.draw_flag) {
            // clear screen to black
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // draw pixels in white
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (int i = 0; i < 64 * 32; i++) {
                if (c.display[i]) {
                    int x = i % 64;
                    int y = i / 64;
                    SDL_Rect rect = { x * 10, y * 10, 10, 10 };
                    SDL_RenderFillRect(renderer, &rect);
                }
            }

            SDL_RenderPresent(renderer);
            c.draw_flag = 0;
        }
    }
}

// TODOS:
// - Understand and rewrite SDL2 code, read docs
// - Understand and rewrite CMakeLists.txt file
// - Make SDL folder, give its own CMakeLists.txt file
// - Create bash script to build
// - Implement remaining opcodes