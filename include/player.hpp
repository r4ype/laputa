#pragma once
#include <SDL.h>
#include <SDL_image.h>

struct position {
    int x, y, xvel, yvel;
};

class Character {
   private:
    position p1 = {0};
    int movementDelay = 5;  // Number of frames to wait between movements
    int frameCounter = 0;   // Counter to track frames

   public:
    void init(int, int, int, int);

    void UpdatePosition(bool keys[4]);
    position GetCharacterPosition();
};
