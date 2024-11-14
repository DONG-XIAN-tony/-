#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // 引入 SDL_image 頭檔
#include <iostream>
using namespace std;

const int SCREEN_WIDTH = 980;
const int SCREEN_HEIGHT = 320;

class App {
public:
    //初始化window初始化renderer 並調用SDL
    App();

    //程式結束後釋放記憶體
    ~App();

    // 如果用戶關閉窗口，就結束程式
    void doInput();

    // 設定渲染器的渲染顏色
    void prepareScene();

    //將渲染好的畫面顯示出來
    void presentScene();

    //檢查應用程式是否仍在運行（只讀）
    bool isRunning() const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running = true; //表示程式是否在運行

    // 加入圖片的相關變數
    SDL_Texture* backgroundTexture; // 背景圖片的SDL_Texture

    //負責初始化 SDL 庫、創建應用程式窗口以及渲染器。
    void initSDL();
    
    //來銷毀渲染器和視窗，並且在最後退出 SDL 系統
    void cleanup();
    
    // 載入背景圖片
    void loadBackground(const char* filePath);
};

//初始化window初始化renderer 並調用SDL
App::App(): window(nullptr), renderer(nullptr), backgroundTexture(nullptr)   
{
    initSDL();      //調用SDL函式庫
    loadBackground("第二場景/圖片資料庫/背景圖片.png"); // 圖片路徑 如果跑不出來 記得要修改路徑
}

//程式結束後釋放記憶體
App::~App() 
{
    cleanup();     //程式結束後 釋放記憶體
}

//如果用戶關閉視窗，就結束程式
void App::doInput() 
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT) //點擊窗口的關閉按鈕
        {
            running = false;
        }
    }
}  // End of doInput function

//設定渲染器的渲染顏色
void App::prepareScene()
{
    // 清除畫面之前先繪製背景圖片
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // 繪製背景
    }
}

//將渲染好的畫面顯示出來
void App::presentScene() 
{
    SDL_RenderPresent(renderer); // 更新顯示
}

//檢查應用程式是否仍在運行（只讀）
bool App::isRunning() const
{
    return running;
}

// initSDL() 方法的實現，負責初始化 SDL 庫、創建應用程式窗口以及渲染器。
void App::initSDL() 
{
    int rendererFlags = SDL_RENDERER_ACCELERATED; // 使用硬體加速
    int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)  // 初始化SDL視頻子系統
    {
        cout << "Couldn't initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    window = SDL_CreateWindow("小恐龍奔跑吧", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    if (!window)    
    {
        cout << "Failed to open " << SCREEN_WIDTH << " x " << SCREEN_HEIGHT << " window: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); 

    renderer = SDL_CreateRenderer(window, -1, rendererFlags); 
    if (!renderer)   
    {
        cout<< "Failed to create renderer: " << SDL_GetError() << endl;
        exit(1);
    }
}

// 清除資源
void App::cleanup() 
{
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture); // 清除背景圖片的資源
    }
    if (renderer)  
    {
        SDL_DestroyRenderer(renderer); // 銷毀渲染器
    }
    if (window)   
    {
        SDL_DestroyWindow(window); // 銷毀視窗
    }
    SDL_Quit(); // 退出SDL系統
}

// 載入背景圖片
void App::loadBackground(const char* filePath) 
{
    SDL_Surface* loadedSurface = IMG_Load(filePath);  // 使用 IMG_Load 載入圖片

    if (!loadedSurface) {
        cout << "Unable to load image " << filePath << " SDL_Error: " << SDL_GetError() << endl;
        exit(1);
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);  // 釋放 surface

    if (!backgroundTexture) {
        cout << "Unable to create texture from surface " << SDL_GetError() << endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) 
{
    // Create an instance of App, which initializes SDL
    App app;  

    // Main loop of the application
    while (app.isRunning()) 
    {
        app.prepareScene();  // 繪製背景
        app.doInput();       // 處理來自用戶的輸入
        app.presentScene();  // 顯示畫面

        SDL_Delay(16);  // 延遲16ms，控制畫面更新速率
    }

    return 0;
}

