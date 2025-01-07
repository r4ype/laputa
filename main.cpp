
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>

#define windowSizeWidth 640
#define windowSizeHeight 480

const int gridSize = 15;
const int width = windowSizeWidth - (windowSizeWidth % gridSize);
const int height = windowSizeHeight - (windowSizeHeight % gridSize);
const int CELL_WIDTH = width / gridSize;
const int CELL_HEIGHT = height / gridSize;
const int VisionRadios = 3;
int board[gridSize][gridSize];

struct position {
    int x, y;
};

class Character {
   private:
    position p1 = {0};

   public:
    void init(int startX, int startY) {
        board[startX][startY] = 1;
        p1.x = startX;
        p1.y = startY;
    }
    position GetCharacterPosition() {
        return {p1.x * CELL_WIDTH, p1.y * CELL_HEIGHT};
    }
};

class Window {
   private:
    int width, height;
    double Distance(int x1, int y1, int x2, int y2) {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

   public:
    void init(int widthScreen, int heightScreen) {
        width = widthScreen;
        height = heightScreen;
    }
    void DrawGrid(SDL_Renderer* renderer, position player, int VisionRadios);
};

void Window::DrawGrid(SDL_Renderer* renderer, position player,
                      int VisionRadios) {
    int playerGridX = player.x / CELL_WIDTH;
    int playerGridY = player.y / CELL_HEIGHT;
    int playerCenterX = player.x + CELL_WIDTH / 2;
    int playerCenterY = player.y + CELL_HEIGHT / 2;

    int startX = std::max(0, playerGridX - VisionRadios);
    int endX = std::min(gridSize, playerGridX + VisionRadios + 1);
    int startY = std::max(0, playerGridY - VisionRadios);
    int endY = std::min(gridSize, playerGridY + VisionRadios + 1);

    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {
            if (board[j][i] == 2) {
                SDL_Rect rect = {j * CELL_WIDTH, i * CELL_HEIGHT, CELL_WIDTH,
                                 CELL_HEIGHT};
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    std::fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(int), 0);

    board[3][6] = 2;
    board[4][7] = 2;
    board[5][8] = 2;

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

    Character player;
    player.init(0, 0);

    Window gameWindow;
    gameWindow.init(width, height);

    bool gameIsRunning = true;
    SDL_Event event;

    // start of modification
    std::queue<std::string> actionQueue;
    std::string currentInput;
    auto isValidAction = [](const std::string& action) {
        if (action.empty()) return false;
        char dir = action.back();
        if (dir != 'h' && dir != 'j' && dir != 'k' && dir != 'l') return false;
        std::string numPart = action.substr(0, action.size() - 1);
        if (numPart.empty()) return true;
        for (char c : numPart) {
            if (!isdigit(c)) return false;
        }
        int num = std::stoi(numPart);
        return num >= 1 && num <= 8;
    };
    // end of modification

    while (gameIsRunning) {
        SDL_SetRenderDrawColor(renderer, 30, 30, 46, 255);
        SDL_RenderClear(renderer);

        position playerPos = player.GetCharacterPosition();
        SDL_Rect destRect = {playerPos.x, playerPos.y, CELL_WIDTH, CELL_HEIGHT};
        gameWindow.DrawGrid(renderer, playerPos, VisionRadios);
        SDL_RenderCopy(renderer, playerTexture, NULL, &destRect);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameIsRunning = false;
            }
            // start of modification
            else if (event.type == SDL_KEYDOWN) {
                char key = static_cast<char>(event.key.keysym.sym);
                if (isdigit(key)) {
                    currentInput += key;
                } else if (key == 'h' || key == 'j' || key == 'k' || key == 'l') {
                    currentInput += key;
                    if (isValidAction(currentInput)) {
                        actionQueue.push(currentInput);
                    }
                    currentInput.clear();
                }
            }
            // end of modification
        }

        // start of modification
        if (!actionQueue.empty()) {
            std::string currentAction = actionQueue.front();
            actionQueue.pop();
            std::cout << currentAction << std::endl;
        }
        // end of modification
    }

    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
