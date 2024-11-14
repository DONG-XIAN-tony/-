// input.c
#include "common.h"
#include "input.h"
#include "main.h"

void doInput(int *x, int *y) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:    // 上移
                        *y -= 10;
                        break;
                    case SDLK_DOWN:  // 下移
                        *y += 10;
                        break;
                    case SDLK_LEFT:  // 左移
                        *x -= 10;
                        break;
                    case SDLK_RIGHT: // 右移
                        *x += 10;
                        break;
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
}
