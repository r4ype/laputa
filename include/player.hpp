#pragma once

struct position {
    int x, y;
};

class Character {
private:
    position pos;
    int width, height;
public:
    void init(int startX, int startY, int w, int h);
    void UpdatePosition(bool keys[4]);
    position GetCharacterPosition();
};
