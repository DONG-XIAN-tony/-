// draw.c
#include "common.h"
#include "draw.h"
#include "main.h"
void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}
