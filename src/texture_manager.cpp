#include "texture_manager.h"

SDL_Texture* TextureManager::loadTexture(const std::string& file, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(file.c_str());
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
