#include <SDL2/SDL.h>
#include <iostream>

class Character {
   public:
    int x = 0, y = 0, xvel = 0, yvel = 0, speed = 3;

    void init(int startX, int startY, int velocityX, int velocityY) {
        x = startX;
        y = startY;
        xvel = velocityX;
        yvel = velocityY;
    }

    void moveLeft() { xvel = -speed; }
    void moveRight() { xvel = speed; }
    void moveUp() { yvel = -speed; }
    void moveDown() { yvel = speed; }
    void stopHorizontalMovement() { xvel = 0; }
    void stopVerticalMovement() { yvel = 0; }

    void updatePosition() {
        x += xvel;
        y += yvel;
        if (x < 0) x = 0;
        if (x > 640 - 50) x = 640 - 50;
        if (y < 0) y = 0;
        if (y > 480 - 50) y = 480 - 50;
    }
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Character pazu;
    pazu.init(0, 0, 0, 0);

    bool gameIsRunning = true;
    SDL_Event event;

    while (gameIsRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameIsRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            pazu.moveLeft();
                            break;
                        case SDLK_d:
                            pazu.moveRight();
                            break;
                        case SDLK_w:
                            pazu.moveUp();
                            break;
                        case SDLK_s:
                            pazu.moveDown();
                            break;
                        case SDLK_q:
                            gameIsRunning = false;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                        case SDLK_d:
                            pazu.stopHorizontalMovement();
                            break;
                        case SDLK_w:
                        case SDLK_s:
                            pazu.stopVerticalMovement();
                            break;
                    }
                    break;
            }
        }

        pazu.updatePosition();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect shape = {pazu.x, pazu.y, 50, 50};
        SDL_RenderFillRect(renderer, &shape);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
