#include "common.h"   // 包含 SDL 和其他共用標頭
#include "init.h"     // 包含 init 函數的宣告
#include "main.h"     // 包含 app 變數的 extern 宣告
#include "defs.h"   // 包含 SCREEN_WIDTH 和 SCREEN_HEIGHT 的定義

void initSDL(void) {
    // 初始化 SDL 子系統
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // 創建 SDL 視窗
    app.window = SDL_CreateWindow(
        "Running",                  // 視窗標題
        SDL_WINDOWPOS_UNDEFINED,     // 視窗 X 位置
        SDL_WINDOWPOS_UNDEFINED,     // 視窗 Y 位置
        SCREEN_WIDTH,                // 視窗寬度
        SCREEN_HEIGHT,               // 視窗高度
        0                            // 視窗旗標
    );

    if (!app.window) {
        printf("Failed to open window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // 創建 SDL 渲染器
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    // 設定渲染縮放品質
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

void cleanup(void) {
    // 釋放渲染器和視窗
    if (app.renderer) {
        SDL_DestroyRenderer(app.renderer);
    }

    if (app.window) {
        SDL_DestroyWindow(app.window);
    }

    // 關閉 SDL
    SDL_Quit();
}
