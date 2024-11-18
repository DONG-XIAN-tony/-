#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 初始化 SDL_image
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 創建一個窗口
    const int WINDOW_WIDTH = 840;
    const int WINDOW_HEIGHT = 480;
    SDL_Window* window = SDL_CreateWindow("Smooth Moving Background",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 渲染器設置
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 加載圖片
    SDL_Texture* texture = IMG_LoadTexture(renderer, "/Users/tonykk90918/Documents/程式/C++/程式設計/期末遊戲專案/期末專案程式碼/第二場景/圖片資料庫/背景圖片.png"); // 替換為你圖片的路徑
    if (!texture) {
        std::cerr << "Failed to load texture! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 背景初始位置
    float bgX1 = 0;
    float bgX2 = WINDOW_WIDTH; // 第二張背景緊接著第一張

    // 設定移動速度（每秒移動的像素）
    const float SPEED = 60.0f; // 每秒移動 60 像素

    // 計時變量
    Uint32 lastTime = SDL_GetTicks(); // 上一次更新的時間

    // 主循環標誌
    bool quit = false;
    SDL_Event e;

    // 主事件循環
    while (!quit) {
        // 處理事件
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) { // 點擊關閉窗口
                quit = true;
            }
        }

        // 計算時間差
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // 以秒為單位
        lastTime = currentTime;

        // 更新背景位置
        bgX1 -= SPEED * deltaTime; // 按時間更新位置
        bgX2 -= SPEED * deltaTime;

        // 如果背景移出螢幕，重置位置
        if (bgX1 + WINDOW_WIDTH <= 0) {
            bgX1 = bgX2 + WINDOW_WIDTH;
        }
        if (bgX2 + WINDOW_WIDTH <= 0) {
            bgX2 = bgX1 + WINDOW_WIDTH;
        }

        // 清空畫面
        SDL_RenderClear(renderer);

        // 渲染背景圖片（縮放到視窗大小）
        SDL_Rect bgRect1 = {static_cast<int>(bgX1), 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_Rect bgRect2 = {static_cast<int>(bgX2), 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, texture, NULL, &bgRect1);
        SDL_RenderCopy(renderer, texture, NULL, &bgRect2);

        // 顯示內容
        SDL_RenderPresent(renderer);
    }

    // 清理
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}