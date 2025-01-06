#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "window.hpp"
#include "player.hpp"
#include "board.hpp"

bool gameIsRunning = true;
const int width = 800;
const int height = 600;
const int VisionRadios = 5;

int main(int argc, char* argv[]) {
    std::fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(int), 0);
    board[5][8] = 2;
    board[6][7] = 2;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "../assets/images/player.png");
    if (!playerTexture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    Character pazu;
    pazu.init(0, 0, CELL_WIDTH, CELL_HEIGHT);
    position Now;
    Window MainWindow;
    MainWindow.init(width, height);

    SDL_Event event;
    bool keys[4] = {false, false, false, false};
    Uint32