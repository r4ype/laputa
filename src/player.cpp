#include "player.hpp"

void Character::init(int startX, int startY, int w, int h) {
    pos.x = startX;
    pos.y = startY;
    width = w;
    height = h;
}

void Character::UpdatePosition(bool keys[4]) {
    if (keys[0]) pos.x -= width;
    if (keys[1]) pos.x += width;
    if (keys[2]) pos.y -= height;
    if (keys[3]) pos.y += height;
}

position Character::GetCharacterPosition() {
    return pos;
}
