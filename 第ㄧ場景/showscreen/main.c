// main.c
#include "common.h"
#include "defs.h"
#include "init.h"
#include "draw.h"
#include "input.h"
#include "main.h"
#include "structs.h"
App app; // 定義全域變數

int main(int argc, char *argv[]) {
    memset(&app, 0, sizeof(App));

    initSDL();

    atexit(cleanup);

    while (1) {
        prepareScene();
        doInput();
        presentScene();
        SDL_Delay(16); // 延遲 16 毫秒，約等於 60FPS
    }

    return 0;
}
