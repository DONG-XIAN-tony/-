// draw.c
#include "common.h"
#include "draw.h"
#include "main.h"
#include "defs.h"

SDL_Texture *playerTexture;  // 全域變數，用於儲存角色的紋理

void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}

void loadPlayerTexture(void) {
    playerTexture = IMG_LoadTexture(app.renderer, "Jesus.png");

    if (!playerTexture) {
        printf("Failed to load player texture: %s\n", IMG_GetError());
        exit(1);
    } else {
        int w, h;
        SDL_QueryTexture(playerTexture, NULL, NULL, &w, &h);
        printf("Player texture loaded successfully with dimensions: %dx%d\n", w, h);
    }
}

void drawPlayer(int x, int y) {
    SDL_Rect playerRect = {x, y, 100, 100};  // 設定角色的座標和大小
    SDL_RenderCopy(app.renderer, playerTexture, NULL, &playerRect);
}
