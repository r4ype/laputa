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
    const double DEG_TO_RAD = M_PI / 180.0;
    const double RAD_TO_DEG = 180.0 / M_PI;

    int playerGridX = player.x / CELL_WIDTH;
    int playerGridY = player.y / CELL_HEIGHT;
    int playerCenterX = player.x + CELL_WIDTH / 2;
    int playerCenterY = player.y + CELL_HEIGHT / 2;

    int startX = std::max(0, playerGridX - VisionRadios);
    int endX = std::min(gridSize, playerGridX + VisionRadios + 1);
    int startY = std::max(0, playerGridY - VisionRadios);
    int endY = std::min(gridSize, playerGridY + VisionRadios + 1);

    struct ShadowRange {
        double minAngle, maxAngle;
        int expStartX,expStartY,expEndX,expEndY;
    };

    const int maxShadows =
        gridSize * gridSize;  // Maximum possible shadows in the grid
    ShadowRange shadowRanges[maxShadows];
    int shadowCount = 0;

    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {
            if (board[j][i] == 2) {  // Wall cell
                double angles[4] = {
                    atan2(i * CELL_HEIGHT - playerCenterY,
                          j * CELL_WIDTH - playerCenterX) *
                        RAD_TO_DEG,
                    atan2((i + 1) * CELL_HEIGHT - playerCenterY,
                          j * CELL_WIDTH - playerCenterX) *
                        RAD_TO_DEG,
                    atan2(i * CELL_HEIGHT - playerCenterY,
                          (j + 1) * CELL_WIDTH - playerCenterX) *
                        RAD_TO_DEG,
                    atan2((i + 1) * CELL_HEIGHT - playerCenterY,
                          (j + 1) * CELL_WIDTH - playerCenterX) *
                        RAD_TO_DEG};
                    double side[4][3] = {
                    {Distance(playerCenterX, playerCenterY, j * CELL_WIDTH, i * CELL_HEIGHT), (double)j * CELL_WIDTH, (double)i * CELL_HEIGHT},
                    {Distance(playerCenterX, playerCenterY, j * CELL_WIDTH, (i + 1) * CELL_HEIGHT), (double)j * CELL_WIDTH, (double)(i + 1) * CELL_HEIGHT},
                    {Distance(playerCenterX, playerCenterY, (j + 1) * CELL_WIDTH, i * CELL_HEIGHT), (double)(j + 1) * CELL_WIDTH, (double)i * CELL_HEIGHT},
                    {Distance(playerCenterX, playerCenterY, (j + 1) * CELL_WIDTH, (i + 1) * CELL_HEIGHT), (double)(j + 1) * CELL_WIDTH, (double)(i + 1) * CELL_HEIGHT}
                                };
                for(int i = 0;i < 3;i++){
                    for(int j = i+1 ;j < 4;j++){
                        if(side[i][0] > side[j][0]){
                            std::swap(side[i][0],side[j][0]);
                        }
                    }
                }
                
                for (int k = 0; k < 4; k++) {
                    if (angles[k] < 0) angles[k] += 360;
                }
                std::sort(angles, angles + 4);

                double minAngle = angles[0];
                double maxAngle = angles[3];

                if (maxAngle - minAngle > 180) {
                    minAngle = angles[1];
                    maxAngle = angles[2];
                }

                if (shadowCount < maxShadows) {
                    shadowRanges[shadowCount++] = {minAngle, maxAngle,(int)side[0][1],(int)side[0][2],(int)side[3][1],(int)side[3][2]};
                }
            }
        }
    }

    for (int y = startY * CELL_HEIGHT; y < endY * CELL_HEIGHT; y++) {
        for (int x = startX * CELL_WIDTH; x < endX * CELL_WIDTH; x++) {
            double dist = Distance(playerCenterX, playerCenterY, x, y);
            if (dist > VisionRadios * CELL_WIDTH) continue;

            double angle =
                atan2(y - playerCenterY, x - playerCenterX) * RAD_TO_DEG;
            if (angle < 0) angle += 360;

            bool inShadow = false;
            for (int i = 0; i < shadowCount; i++) {
                const ShadowRange& range = shadowRanges[i];
                if (playerCenterX <= x && playerCenterY <= y && !(x <= std::max(range.expEndX,range.expStartX) && y <= std::max(range.expEndY,range.expStartX))){
                    if (range.maxAngle - range.minAngle < 180 ) {
                        if (angle >= range.minAngle && angle <= range.maxAngle) {
                            inShadow = true;
                            break;
                        }
                    } else {
                        if (angle >= range.maxAngle || angle <= range.minAngle) {
                            inShadow = true;
                            break;
                        }
                    }
                }
                if (playerCenterX <= x && playerCenterY >= y && !(x <= std::max(range.expEndX,range.expStartX) && y >= std::min(range.expEndY,range.expStartX))){
                    if (range.maxAngle - range.minAngle < 180 ) {
                        if (angle >= range.minAngle && angle <= range.maxAngle) {
                            inShadow = true;
                            break;
                        }
                    } else {
                        if (angle >= range.maxAngle || angle <= range.minAngle) {
                            inShadow = true;
                            break;
                        }
                    }
                }
                if (playerCenterX >= x && playerCenterY >= y && !(x >= std::min(range.expEndX,range.expStartX) && y >= std::min(range.expEndY,range.expStartX))){
                    if (range.maxAngle - range.minAngle < 180 ) {
                        if (angle >= range.minAngle && angle <= range.maxAngle) {
                            inShadow = true;
                            break;
                        }
                    } else {
                        if (angle >= range.maxAngle || angle <= range.minAngle) {
                            inShadow = true;
                            break;
                        }
                    }
                }
                if (playerCenterX >= x && playerCenterY <= y && !(x >= std::min(range.expEndX,range.expStartX) && y <= std::max(range.expEndY,range.expStartX))){
                    if (range.maxAngle - range.minAngle < 180 ) {
                        if (angle >= range.minAngle && angle <= range.maxAngle) {
                            inShadow = true;
                            break;
                        }
                    } else {
                        if (angle >= range.maxAngle || angle <= range.minAngle) {
                            inShadow = true;
                            break;
                        }
                    }
                }
            }

            if (!inShadow && (x % CELL_WIDTH == 0 || y % CELL_HEIGHT == 0 ||
                              board[x / CELL_WIDTH][y / CELL_HEIGHT] == 2)) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

/////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    std::fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(int), 0);

    board[5][6] = 2;
    board[6][8] = 2;

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

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError()
                  << std::endl;
        return -1;
    }

    // Initialize SDL_image with PNG support
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError()
                  << std::endl;
        SDL_Quit();
        return -1;
    }

    // Your code to create a window and renderer...
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "../assets/images/player.png");
    if (!playerTexture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Your code to handle texture and game loop...

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
