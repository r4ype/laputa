#include <SDL2/SDL.h>

#include <iostream>

struct position {
    int x, y, xvel, yvel;
};

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
    void MoveCharacter(char direction);
    void SetCharacterPosition(position player);
    void SetCharacterSpeed(position player);
    position GetCharacterPosition();
};

void Character::MoveCharacter(char direction) {
    switch (direction) {
        case 'l':
            p1.x -= p1.xvel;
            break;
        case 'r':
            p1.x += p1.xvel;
            break;
        case 'u':
            p1.y -= p1.yvel;
            break;
        case 'd':
            p1.y += p1.yvel;
            break;
    }
}

void Character::SetCharacterSpeed(position player) {
    p1.xvel = player.xvel;
    p1.yvel = player.yvel;
}

void Character::SetCharacterPosition(position player) {
    if (p1.x + player.x > 0 && p1.x + player.x < 640) {
        p1.x += player.x;
    }
    if (p1.y + player.y > 0 && p1.y + player.y < 480) {
        p1.y += player.y;
    }
}

position Character::GetCharacterPosition() { return p1; }

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError()
                  << std::endl;
        return -1;
    }

    SDL_Window* window =
        SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Character pazu;
    pazu.init(0, 0, 2, 2);  // Changed initial velocities to 2 for movement
    position Now;

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
                            pazu.MoveCharacter('l');
                            break;
                        case SDLK_d:
                            pazu.MoveCharacter('r');
                            break;
                        case SDLK_w:
                            pazu.MoveCharacter('u');
                            break;
                        case SDLK_s:
                            pazu.MoveCharacter('d');
                            break;
                        case SDLK_q:
                            gameIsRunning = false;
                            break;
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        Now = pazu.GetCharacterPosition();
        SDL_Rect shape = {Now.x, Now.y, 50, 50};
        SDL_RenderFillRect(renderer, &shape);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
// lazygit test
