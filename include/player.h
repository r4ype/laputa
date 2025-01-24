
#pragma once
#include <SDL2/SDL.h>
#include <string>

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    void handleInput(const SDL_Event& event);
    void update();
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    bool keys[4];
    int x, y;
    int speed;
};
