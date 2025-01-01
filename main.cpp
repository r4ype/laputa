#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#define windowSizeWidth 640
#define windowSizeHeight 480

const int gridSize = 15;
const int width = windowSizeWidth - (windowSizeWidth % gridSize);
const int height = windowSizeHeight - (windowSizeHeight % gridSize);
const int CELL_WIDTH = width / gridSize;
const int CELL_HEIGHT = height / gridSize;

struct position {
    int x, y, xvel, yvel;
};
/////////////////////////////////////////////////////////
class Character {
   private:
    position p1 = {0};

   public:
    void init(int startX, int startY, int velocityX, int velocityY) {
        p1.x = startX;
        p1.y = startY;
        p1.xvel = velocityX;
        p1.yvel = velocityY;
    }
    void SetCharacterSpeed(position player);
    position GetCharacterPosition();
    void UpdatePosition(bool keys[4]);
};

void Character::SetCharacterSpeed(position player) {
    p1.xvel = player.xvel;
    p1.yvel = player.yvel;
}

position Character::GetCharacterPosition() { return p1; }

void Character::UpdatePosition(bool keys[4]) {
    if (keys[0] && p1.x - CELL_WIDTH >= 0) p1.x -= CELL_WIDTH;        // left
    if (keys[1] && p1.x + CELL_WIDTH < width) p1.x += CELL_WIDTH;     // right
    if (keys[2] && p1.y - CELL_HEIGHT >= 0) p1.y -= CELL_HEIGHT;      // up
    if (keys[3] && p1.y + CELL_HEIGHT < height) p1.y += CELL_HEIGHT;  // down
}
//////////////////////////////////////////////////
class Window {
   private:
    int width, height;

   public:
    void init(int widthScreen, int heightScreen) {
        width = widthScreen;
        height = heightScreen;
    }
    void DrawGrid(SDL_Renderer*);
};

void Window::DrawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < width; i += CELL_WIDTH) {
        SDL_RenderDrawLine(renderer, i, 0, i, height);
    }
    for (int i = 0; i < height; i += CELL_HEIGHT) {
        SDL_RenderDrawLine(renderer, 0, i, width, i);
    }
    SDL_RenderDrawLine(renderer, width - 1, 0, width - 1, height);
    SDL_RenderDrawLine(renderer, 0, height - 1, width, height - 1);
}
/////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    Window MainWindow;
    MainWindow.init(width, height);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError()
                  << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width,
                                          height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "assets/player.png");

    Character pazu;
    pazu.init(0, 0, CELL_WIDTH, CELL_HEIGHT);
    position Now;

    bool gameIsRunning = true;
    SDL_Event event;
    bool keys[4] = {false, false, false, false};  // left, right, up, down

    while (gameIsRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameIsRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            keys[0] = true;
                            break;
                        case SDLK_d:
                            keys[1] = true;
                            break;
                        case SDLK_w:
                            keys[2] = true;
                            break;
                        case SDLK_s:
                            keys[3] = true;
                            break;
                        case SDLK_q:
                            gameIsRunning = false;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            keys[0] = false;
                            break;
                        case SDLK_d:
                            keys[1] = false;
                            break;
                        case SDLK_w:
                            keys[2] = false;
                            break;
                        case SDLK_s:
                            keys[3] = false;
                            break;
                    }
                    break;
            }
        }

        pazu.UpdatePosition(keys);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Now = pazu.GetCharacterPosition();
        SDL_Rect destRect = {Now.x, Now.y, CELL_WIDTH, CELL_HEIGHT};
        MainWindow.DrawGrid(renderer);
        SDL_RenderCopy(renderer, playerTexture, NULL, &destRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
