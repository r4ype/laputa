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

  // changed SDL_WINDOW_SHOWN to SDL_WINDOW_FULLSCREEN_DESKTOP
  window = SDL_CreateWindow("an SDL2 window", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, 640, 480,
                            SDL_WINDOW_FULLSCREEN_DESKTOP);
  if (window == NULL) {
    std ::cout << "could not create window";
  }
  // created a rendered for drawing a square (arguments " window - index -
  // acceleration method") window | -1 for best driver |
  // SDL_RENDERER_ACCELERATED for gpu/hardware acceleration
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RendererInfo info;
  if (SDL_GetRendererInfo(renderer, &info) != 0) {
    std::cerr << "SDL_GetRendererInfo Error: " << SDL_GetError() << std::endl;
  } else {
    std::cout << "Current GPU: " << info.name << std::endl;
  }

  class Character {
  public:
    int x, y;
    int xvel, yvel;
    int speed;
  };

  Character pazu;
  pazu.x = 0;
  pazu.y = 0;
  pazu.xvel = 0;
  pazu.yvel = 0;
  pazu.speed = 3;
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
          pazu.xvel = -pazu.speed;
          break;
        case SDLK_d:
          pazu.xvel = pazu.speed;
          break;
        case SDLK_w:
          pazu.yvel = -pazu.speed;
          break;
        case SDLK_s:
          pazu.yvel = pazu.speed;
          break;
        case SDLK_q:
          GameIsRunning = false;
          break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_a:
          if (pazu.xvel < 0)
            pazu.xvel = 0;
          break;
        case SDLK_d:
          if (pazu.xvel > 0)
            pazu.xvel = 0;
          break;
        case SDLK_w:
          if (pazu.yvel < 0)
            pazu.yvel = 0;
          break;
        case SDLK_s:
          if (pazu.yvel > 0)
            pazu.yvel = 0;
          break;
        }
        break;
      }
    }
    // update players possition
    pazu.x += pazu.xvel;
    pazu.y += pazu.yvel;
    // draw the player
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect shape = {pazu.x, pazu.y, 50, 50};
    SDL_RenderFillRect(renderer, &shape);

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
