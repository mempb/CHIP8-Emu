#include <stdio.h>
#include <SDL2/SDL.h>
#include <dirent.h>     // opendir, readdir, closedir

#include "chip8.h"

#define ROM_DIR "../ROMs"
#define MAX_ROMS 32     // Max roms read, used for array size
#define MAX_PATH 256    // Max rom path, used for array size

SDL_Renderer *initializeSDL();
void renderSDL(SDL_Renderer *renderer, Chip8 *c);
int pickRom(char outPath[MAX_PATH]);

int main(int argc, char* argv[]) {
    SDL_Renderer *renderer = initializeSDL();
    if (renderer == NULL) return 1;

    Chip8 c;
    chip8_init(&c);
    printf("CHIP-8 Initialized\n");


    char romPath[MAX_PATH];
    int romSelected = pickRom(romPath);
    if (romSelected != 0) {
        printf("No ROM selected.\n");
        return 1;
    }

    printf("ROM Path: %s\n", romPath);
    chip8_load_rom(&c, romPath);
    printf("ROM Loaded\n");

    while (1) {
        renderSDL(renderer, &c);
    }
}

SDL_Renderer *initializeSDL() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return NULL;
    }
    printf("SDL Initialized\n");

    // Create and render SDL window
    SDL_Window *window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    printf("SDL Window Rendered\n");
    return renderer;
}

void renderSDL(SDL_Renderer *renderer, Chip8 *c) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
    }
    chip8_cycle(c);

    if (c->draw_flag) {
        // clear screen to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw pixels in white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < 64 * 32; i++) {
            if (c->display[i]) {
                int x = i % 64;
                int y = i / 64;
                SDL_Rect rect = { x * 10, y * 10, 10, 10 };
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        SDL_RenderPresent(renderer);
        c->draw_flag = 0;
    }
}

int pickRom(char outPath[MAX_PATH]) {
    DIR *dir = opendir(ROM_DIR);

    if (dir == NULL) {
        printf("Error opening ROM path!\n");
        perror("opendir");
        return -1;
    }

    char roms[MAX_ROMS][MAX_PATH];
    int romCount = 0;
    struct dirent *entry;

    // Read entire ROM directory, skip non .ch8 files
    while ((entry = readdir(dir)) != NULL && romCount < MAX_ROMS) {
        char *ext = strrchr(entry->d_name, '.');
        if (!ext || strcmp(ext, ".ch8") != 0) continue;
        snprintf(roms[romCount++], MAX_PATH, "%s/%s", ROM_DIR, entry->d_name);
    }
    closedir(dir);

    // Fail if no ROMs read
    if (romCount == 0) {
        printf("No ROMs found\n");
        return -1;
    }

    // Print each ROM name
    for (int i = 0; i < romCount; i++) {
        const char *name = strrchr(roms[i], '/');
        printf("[%d] %s\n", i + 1, name ? name + 1 : roms[i]);
    }

    // Allow user to choose ROM
    printf("Choose: ");
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > romCount) return -1;   // Fail if choice is invalid

    // Copy chosen ROM string to outPath, return
    strncpy(outPath, roms[choice - 1], MAX_PATH);
    return 0;
}