
#include "player.h"
#include "texture_manager.h"

Player::Player(SDL_Renderer* renderer) : renderer(renderer), x(0), y(0), speed(5) {
    texture = TextureManager::loadTexture("assets/images/player.png", renderer);

    destRect = {x, y, 32, 32};
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleInput(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w: y -= speed; break;
            case SDLK_s: y += speed; break;
            case SDLK_a: x -= speed; break;
            case SDLK_d: x += speed; break;
        }
    }
}

void Player::update() {
    destRect.x = x;
    destRect.y = y;
}

void Player::render() {
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}
