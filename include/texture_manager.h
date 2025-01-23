#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class TextureManager {
public:
    static SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* renderer);
};
