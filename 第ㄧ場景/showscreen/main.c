// main.c
#include "common.h"
#include "defs.h"
#include "init.h"
#include "draw.h"
#include "input.h"
#include "main.h"
#include "structs.h"

App app; // 定義全域變數

// 新增角色位置變數
int playerX = PLAYER_START_X;
int playerY = PLAYER_START_Y;

int main(int argc, char *argv[]) {
    memset(&app, 0, sizeof(App));

    initSDL();
    loadPlayerTexture();

    atexit(cleanup);

    while (1) {
        prepareScene();  // 清除畫面
        drawPlayer(playerX, playerY);    // 繪製角色
        doInput(&playerX, &playerY);     // 處理輸入並更新位置
        presentScene();  // 呈現畫面
        SDL_Delay(16);   // 控制幀率
    }

    return 0;
}
