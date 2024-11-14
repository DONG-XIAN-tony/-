#include "common.h"
#include "defs.h"
#include "init.h"
#include "main.h"

extern SDL_Texture *playerTexture;  // 外部宣告 playerTexture，便於清理

void initSDL(void) {
    // 初始化 SDL
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
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_SOFTWARE);
    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    // 設置渲染縮放品質
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    // 初始化 SDL_image 庫
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }
}


int isCleanedUp = 0;

void cleanup(void) {
    if (isCleanedUp) return;
    isCleanedUp = 1;

    if (playerTexture) {
        SDL_DestroyTexture(playerTexture);
        playerTexture = NULL;
    }
    if (app.renderer) {
        SDL_DestroyRenderer(app.renderer);
        app.renderer = NULL;
    }
    if (app.window) {
        SDL_DestroyWindow(app.window);
        app.window = NULL;
    }

    IMG_Quit();
    SDL_Quit();
}
