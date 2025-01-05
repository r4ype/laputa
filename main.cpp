#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <algorithm>
#include <cmath>
#include <iostream>

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
    void DrawGrid(SDL_Renderer* renderer, position player, int VisionRadios);
};

void Window::DrawGrid(SDL_Renderer* renderer, position player, int VisionRadios) {
    // Convert player's position to grid coordinates
    int playerGridX = player.x / CELL_WIDTH;
    int playerGridY = player.y / CELL_HEIGHT;
    player.x += CELL_WIDTH / 2;
    player.y += CELL_HEIGHT / 2;

    // Define the range of cells to render
    int startX = std::max(0, playerGridX - VisionRadios);
    int endX = std::min(gridSize, playerGridX + VisionRadios + 1);
    int startY = std::max(0, playerGridY - VisionRadios);
    int endY = std::min(gridSize, playerGridY + VisionRadios + 1);

    // Collect shadow ranges
    std::vector<std::pair<double, double>> shadowRanges;
    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {
            if (board[j][i] == 2) { // Wall cell
                double minAngle = std::min({
                    atan2(i * CELL_HEIGHT - player.y, j * CELL_WIDTH - player.x),
                    atan2((i + 1) * CELL_HEIGHT - player.y, j * CELL_WIDTH - player.x),
                    atan2(i * CELL_HEIGHT - player.y, (j + 1) * CELL_WIDTH - player.x),
                    atan2((i + 1) * CELL_HEIGHT - player.y, (j + 1) * CELL_WIDTH - player.x)
                });

                double maxAngle = std::max({
                    atan2(i * CELL_HEIGHT - player.y, j * CELL_WIDTH - player.x),
                    atan2((i + 1) * CELL_HEIGHT - player.y, j * CELL_WIDTH - player.x),
                    atan2(i * CELL_HEIGHT - player.y, (j + 1) * CELL_WIDTH - player.x),
                    atan2((i + 1) * CELL_HEIGHT - player.y, (j + 1) * CELL_WIDTH - player.x)
                });

                shadowRanges.emplace_back(minAngle, maxAngle);
            }
        }
    }

    // Draw the grid
    for (int y = startY * CELL_HEIGHT; y < endY * CELL_HEIGHT; y++) {
        for (int x = startX * CELL_WIDTH; x < endX * CELL_WIDTH; x++) {
            bool inShadow = false;
            double angle = atan2(y - player.y, x - player.x);

            for (const auto& range : shadowRanges) {
                if (angle >= range.first && angle <= range.second) {
                    inShadow = true;
                    break;
                }
            }

            if (!inShadow && (x % CELL_WIDTH == 0 || y % CELL_HEIGHT == 0 || board[x / CELL_WIDTH][y / CELL_HEIGHT] == 2)) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}
/////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    std::fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(int), 0);

    // draw wall for now should be in window class !!!
    board[8][8] = 2;
    board[9][7] = 2;

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

        SDL_SetRenderDrawColor(renderer, 30, 30, 46, 255);
        SDL_RenderClear(renderer);

        Now = pazu.GetCharacterPosition();
        SDL_Rect destRect = {Now.x, Now.y, CELL_WIDTH, CELL_HEIGHT};
        MainWindow.DrawGrid(renderer, Now, VisionRadios);
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
