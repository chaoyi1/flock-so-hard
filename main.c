#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Wrapping error code/pointer checking functions compresses code length greatly
int sdl_check_code(int code) {
    if (code < 0) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }
    return code;
}

void *sdl_check_pointer(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}

int main(int argc, char const *argv[])
{
    
    sdl_check_code(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *const window = sdl_check_pointer(SDL_CreateWindow("flock so hard", 
                                            0, 0,
                                            SCREEN_WIDTH, SCREEN_HEIGHT,
                                            SDL_WINDOW_RESIZABLE));

    SDL_Renderer *const renderer = sdl_check_pointer(SDL_CreateRenderer(window, 
                                        -1, SDL_RENDERER_ACCELERATED));

    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = 1;
                } break;
            }
        }

        // These set of funcs makes a window
        sdl_check_code(SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255));
        sdl_check_code(SDL_RenderClear(renderer));
        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return 0;
}