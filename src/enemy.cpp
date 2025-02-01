#include "enemy.h"

Enemy::Enemy() {
}

void Enemy::draw(int board[][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

double Enemy::Distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void Enemy::toMapBlock(mapBlock *a, int x, int y, int tx, int ty, int k) {
    a->x = x;
    a->y = y;
    a->h = Distance(x, y, tx, ty);
    a->k = k + 1;
}

bool Enemy::find(int board[][BOARD_SIZE], mapBlock now, int targetX, int targetY) {
    if (board[now.x][now.y] == 1) {
        return true;
    } else {
        mapBlock around[4];
        toMapBlock(&around[0], std::max(0, now.x - 1), now.y, targetX, targetY, now.k);
        toMapBlock(&around[1], std::min(BOARD_SIZE - 1, now.x + 1), now.y, targetX, targetY, now.k); 
        toMapBlock(&around[2], now.x, std::max(0, now.y - 1), targetX, targetY, now.k);
        toMapBlock(&around[3], now.x, std::min(BOARD_SIZE - 1, now.y + 1), targetX, targetY, now.k); 

        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 4; j++) {
                if (around[i].k + around[i].h > around[j].k + around[j].h) { // Sorting condition fixed
                    std::swap(around[i], around[j]);
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            if (board[around[i].x][around[i].y] != 4 && find(board, around[i], targetX, targetY)) {
                board[now.x][now.y] = 2;
                return true;
            }
        }
    }
    return false;
}

bool Enemy::findPath(int mainBoard[][BOARD_SIZE], int startX, int startY, int targetX, int targetY) {
    mapBlock enemy;
    toMapBlock(&enemy, startX, startY, targetX, targetY, 0);
    if (find(mainBoard, enemy, targetX, targetY)) {
        board[startX][startY] = 3;
        draw(mainBoard);
        return true;
    } else {
        std::cout << "can't reach there" << std::endl;
        return false;
    }
}
