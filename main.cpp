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
    void SetCharacterSpeed(position player);
    position GetCharacterPosition();
    void UpdatePosition(bool keys[4][2]);
};
void Character::SetCharacterSpeed(position player) {
    p1.xvel = player.xvel;
    p1.yvel = player.yvel;
}


position Character::GetCharacterPosition() {
    return p1;
}

void Character::UpdatePosition(bool keys[4][2]) {
    if (keys[0][0] && keys[0][1]) p1.x -= p1.xvel; // left
    if (keys[1][0] && keys[1][1]) p1.x += p1.xvel; // right
    if (keys[2][0] && keys[2][1]) p1.y -= p1.yvel; // up
    if (keys[3][0] && keys[3][1]) p1.y += p1.yvel; // down
}
//////////////////////////////////////////////////////////////

class Window {

private : 
    int width ,height;

public :
    void init(int widthScreen, int heightScreen){
        width = widthScreen;height = heightScreen;
    }
    void DrawGrid(SDL_Renderer*);
};
void Window::DrawGrid(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    for(int i = 0;i < width;i += (width/10)){
        SDL_RenderDrawLine(renderer,i,0,i,height);
    }
    for(int i = 0;i < height;i += (height/10)){
        SDL_RenderDrawLine(renderer,0,i,width,i);
    }
}











////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    const int width = 640, height= 480;
    Window MainWindow;
    MainWindow.init(width,height);





    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
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
    pazu.init(0, 0, 2, 2);
    position Now;

    bool gameIsRunning = true;
    SDL_Event event;
    bool keys[4][2] = {{false, true},{false, true},{false, true},{false, true}}; // left, right, up, down

    while (gameIsRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameIsRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            keys[1][1] = false;
                            keys[0][0] = true;
                            break;
                        case SDLK_d:
                            keys[0][1] = false;
                            keys[1][0] = true;
                            break;
                        case SDLK_w:
                            keys[3][1] = false;
                            keys[2][0] = true;
                            break;
                        case SDLK_s:
                            keys[2][1] = false;
                            keys[3][0] = true;
                            break;
                        case SDLK_q:
                            gameIsRunning = false;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            keys[0][0] = false;
                            keys[1][1] = true;
                            break;
                        case SDLK_d:
                            keys[1][0] = false;
                            keys[0][1] = true;
                            break;
                        case SDLK_w:
                            keys[2][0] = false;
                            keys[3][1] = true;
                            break;
                        case SDLK_s:
                            keys[3][0] = false;
                            keys[2][1] = true;
                            break;
                    }
                    break;
            }
        }

        pazu.UpdatePosition(keys);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        Now = pazu.GetCharacterPosition();
        SDL_Rect shape = {Now.x, Now.y, width/10, height/10};
        MainWindow.DrawGrid(renderer);
        SDL_RenderFillRect(renderer, &shape);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

