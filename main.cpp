// g++ main.cpp -o prog -lSDL2
#include <SDL2/SDL.h>
#include <iostream>
int main(int argc, char *argv[]) {
  // create a window data type
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

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
  // created a rendered for drawing a square (arguments " window - index -
  // acceleration method") window | -1 for best driver |
  // SDL_RENDERER_ACCELERATED for gpu/hardware acceleration
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int player_x = 0, player_y = 0;
  int player_xvel = 0, player_yvel = 0;
  // event loop
  bool GameIsRunning = true;
  while (GameIsRunning) {
    SDL_Event event;
    // start our event loop

    // handel kardan event ha ba switch kase bejaye if-else
    while (SDL_PollEvent(&event)) {
      // handel each specific even
      switch (event.type) {
      case SDL_QUIT:
        GameIsRunning = false;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_a:
          player_xvel = -1;
          break;
        case SDLK_d:
          player_xvel = 1;
          break;
        case SDLK_w:
          player_yvel = -1;
          break;
        case SDLK_s:
          player_yvel = 1;
          break;
        case SDLK_q:
          GameIsRunning = false;
          break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_a:
          if (player_xvel < 0)
            player_xvel = 0;
          break;
        case SDLK_d:
          if (player_xvel > 0)
            player_xvel = 0;
          break;
        case SDLK_w:
          if (player_yvel < 0)
            player_yvel = 0;
          break;
        case SDLK_s:
          if (player_yvel > 0)
            player_yvel = 0;
          break;
        }
        break;
      }
    }
    // draw the player
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect player = {player_x, player_y, 50, 50};
    SDL_RenderFillRect(renderer, &player);

    SDL_RenderPresent(renderer);

    SDL_Delay(16);
  }
  // destroy everything and quit
  // SDL_Delay(3000);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return 0;
}
