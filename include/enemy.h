#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <cmath>
#include <algorithm> // Include this for std::swap

class Enemy {
private:
    static const int BOARD_SIZE = 10;
    int board[BOARD_SIZE][BOARD_SIZE];

    struct mapBlock {
        int x, y;
        float h;
        int k;
    };

    double Distance(int x1, int y1, int x2, int y2);
    void toMapBlock(mapBlock *a, int x, int y, int tx, int ty, int k);
    bool find(int mainBoard[][BOARD_SIZE], mapBlock now, int targetX, int targetY);

public:
    Enemy();
    void initializeBoard();
    void draw(int board[][BOARD_SIZE]);
    bool findPath(int mainBoard[][BOARD_SIZE], int startX, int startY, int targetX, int targetY);
};

#endif // ENEMY_iH
