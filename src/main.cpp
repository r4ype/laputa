
#include "game.h"

int main() {
    Game game;
    if (game.init("Simple SDL2 Game", 800, 600)) {
        game.run();
    }
    game.cleanup();
    return 0;
}
