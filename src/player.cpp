#include "player.h"
#include "texture_manager.h"

Player::Player(SDL_Renderer* renderer)
    : renderer(renderer), x(0), y(0), speed(5), keys{0, 0, 0, 0} {
    texture = TextureManager::loadTexture("assets/images/player.png", renderer);
    destRect = {x, y, 32, 32};
}

Player::~Player() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Player::handleInput(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w: keys[0] = 1; break;
            case SDLK_s: keys[1] = 1; break;
            case SDLK_a: keys[2] = 1; break;
            case SDLK_d: keys[3] = 1; break;
        }
    }
    else if(event.type == SDL_KEYUP){
        switch (event.key.keysym.sym) {
            case SDLK_w: keys[0] = 0; break;
            case SDLK_s: keys[1] = 0; break;
            case SDLK_a: keys[2] = 0; break;
            case SDLK_d: keys[3] = 0; break;
        }
    }
}

void Player::update() {
    if (keys[0] && y - speed >= 0) { y -= speed; }  // Move up
    if (keys[1] && y + 32 + speed <= 480) { y += speed; }  // Move down
    if (keys[2] && x - speed >= 0) { x -= speed; }  // Move left
    if (keys[3] && x + 32 + speed <= 640) { x += speed; }  // Move right
    destRect.x = x;
    destRect.y = y;
}

void Player::render() {
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}
