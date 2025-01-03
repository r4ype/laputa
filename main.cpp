#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <algorithm>
#include <iostream>

#define windowSizeWidth 640
#define windowSizeHeight 480

const int gridSize = 15;
const int width = windowSizeWidth - (windowSizeWidth % gridSize);
const int height = windowSizeHeight - (windowSizeHeight % gridSize);
const int CELL_WIDTH = width / gridSize;
const int CELL_HEIGHT = height / gridSize;
int board[gridSize][gridSize];

struct position {
    int x, y, xvel, yvel;
};

enum Keys { LEFT = 0, RIGHT, UP, DOWN };

/////////////////////////////////////////////////////////
class Character {
   private:
    position p1 = {0};
    int movementDelay = 5;  // Number of frames to wait between movements
    int frameCounter = 0;   // Counter to track frames

   public:
    void init(int startX, int startY, int velocityX, int velocityY) {
        board[startX][startY] = 1;
        p1.x = startX;
        p1.y = startY;
        p1.xvel = velocityX;
        p1.yvel = velocityY;
    }
    void UpdatePosition(bool keys[4]);
    position GetCharacterPosition();
};

void Character::UpdatePosition(bool keys[4]) {
    frameCounter++;
    if (frameCounter < movementDelay) {
        return;  // Skip update if the delay hasn't passed
    }
    frameCounter = 0;  // Reset the counter after movement

    if (keys[LEFT] && p1.x - 1 >= 0 && board[p1.x - 1][p1.y] == 0) {
        std::swap(board[p1.x][p1.y], board[p1.x - 1][p1.y]);
        p1.x -= 1;
    }
    if (keys[RIGHT] && p1.x + 1 < gridSize && board[p1.x + 1][p1.y] == 0) {
        std::swap(board[p1.x][p1.y], board[p1.x + 1][p1.y]);
        p1.x += 1;
    }
    if (keys[UP] && p1.y - 1 >= 0 && board[p1.x][p1.y - 1] == 0) {
        std::swap(board[p1.x][p1.y], board[p1.x][p1.y - 1]);
        p1.y -= 1;
    }
    if (keys[DOWN] && p1.y + 1 < gridSize && board[p1.x][p1.y + 1] == 0) {
        std::swap(board[p1.x][p1.y], board[p1.x][p1.y + 1]);
        p1.y += 1;
    }
}

position Character::GetCharacterPosition() {
    return {p1.x * CELL_WIDTH, p1.y * CELL_HEIGHT, p1.xvel, p1.yvel};
}

/////////////////////////////////////////////////////////
class Window {
   private:
    int width, height;

   public:
    void init(int widthScreen, int heightScreen) {
        width = widthScreen;
        height = heightScreen;
    }
    void DrawGrid(SDL_Renderer* renderer);
};

void Window::DrawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 251, 173, 1);
    for (int i = 0; i < width; i += CELL_WIDTH) {
        SDL_RenderDrawLine(renderer, i, 0, i, height);
    }
    for (int i = 0; i < height; i += CELL_HEIGHT) {
        SDL_RenderDrawLine(renderer, 0, i, width, i);
    }
    SDL_RenderDrawLine(renderer, 0, height-1, width-1, height-1);
    SDL_RenderDrawLine(renderer, width-1, 0, width-1, height-1);
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            if (board[x][y] == 1) {  // Draw active cells
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect cell = {x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH,
                                 CELL_HEIGHT};
                SDL_RenderFillRect(renderer, &cell);
            }
            if (board[x][y] == 2) {
                SDL_SetRenderDrawColor(renderer, 250, 94, 173, 1);
                SDL_Rect cell = {x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH,
                                 CELL_HEIGHT};
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }
}

/////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    std::fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(int), 0);

    // draw wall for now should be in window class !!!
    for (int i = 1; i < gridSize - 2; i += 2)
        for (int j = 1; j < gridSize - 1; j += 2) board[i][j] = 2;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError()
                  << std::endl;
        return -1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError()
                  << std::endl;
        SDL_Quit();
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
    if (!playerTexture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    Character pazu;
    pazu.init(0, 0, CELL_WIDTH, CELL_HEIGHT);
    position Now;

    Window MainWindow;
    MainWindow.init(width, height);

    bool gameIsRunning = true;
    SDL_Event event;
    bool keys[4] = {false, false, false, false};

    Uint32 frameStart, frameTime;

    while (gameIsRunning) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameIsRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            keys[LEFT] = true;
                            break;
                        case SDLK_d:
                            keys[RIGHT] = true;
                            break;
                        case SDLK_w:
                            keys[UP] = true;
                            break;
                        case SDLK_s:
                            keys[DOWN] = true;
                            break;
                        case SDLK_q:
                            gameIsRunning = false;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            keys[LEFT] = false;
                            break;
                        case SDLK_d:
                            keys[RIGHT] = false;
                            break;
                        case SDLK_w:
                            keys[UP] = false;
                            break;
                        case SDLK_s:
                            keys[DOWN] = false;
                            break;
                    }
                    break;
            }
        }

        pazu.UpdatePosition(keys);

        SDL_SetRenderDrawColor(renderer, 30, 30, 46, 1);
        SDL_RenderClear(renderer);

        Now = pazu.GetCharacterPosition();
        SDL_Rect destRect = {Now.x, Now.y, CELL_WIDTH, CELL_HEIGHT};
        MainWindow.DrawGrid(renderer);
        SDL_RenderCopy(renderer, playerTexture, NULL, &destRect);

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16) SDL_Delay(16 - frameTime);  // Cap at ~60 FPS
    }

    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
