#include <SDL2/SDL.h>
#include <iostream>
int main(int argc, char *argv[]) {
  // create a window data type
  SDL_Window *window = nullptr;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std ::cout << "could not be initialize !" << SDL_GetError();
  } else {
    std::cout << "SDL is working\n";
  }

  window =
      SDL_CreateWindow("an SDL2 window", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    std ::cout << "could not create window";
  }

  // event loop
  bool GameIsRunning = 1;
  while (GameIsRunning) {
    SDL_Event event;
    // start our event loop

    while (SDL_PollEvent(&event)) {
      // handel each specific event
      if (event.type == SDL_QUIT) {
        GameIsRunning = 0;
      }
    }
  }

  SDL_Delay(3000);
  SDL_DestroyWindow(window);
  return 0;
}
