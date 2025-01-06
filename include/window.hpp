#pragma once

#include <SDL2/SDL.h>
#include "utils.hpp"
#include "position.hpp"

class Window {
private:
    int width, height;
    double Distance(int x1, int y1, int x2, int y2);
public:
    void init(int widthScreen, int heightScreen);
    void DrawGrid(SDL_Renderer* renderer, position player, int VisionRadios);
};
