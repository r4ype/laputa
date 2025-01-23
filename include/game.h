#pragma once
#include <SDL2/SDL.h>
#include "player.h"

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void cleanup();

private:
    void handleEvents();
    void update();
    void render();

    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player* player;
};

