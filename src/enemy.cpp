#include <iostream>
#include <cmath>
#include <algorithm> // Include this for std::swap

int board[10][10] = {0};

void draw() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

double Distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

struct mapBlock {
    int x, y;
    float h;
    int k;
};

void toMapBlock(mapBlock *a, int x, int y, int tx, int ty, int k) {
    a->x = x;
    a->y = y;
    a->h = Distance(x, y, tx, ty);
    a->k = k + 1;
}

bool find(mapBlock now, int targetX, int targetY) {
    if (board[now.x][now.y] == 1) {
        return true;
    } else {
        mapBlock around[4];
        toMapBlock(&around[0], std::max(0, now.x - 1), now.y, targetX, targetY, now.k);
        toMapBlock(&around[1], std::min(9, now.x + 1), now.y, targetX, targetY, now.k); 
        toMapBlock(&around[2], now.x, std::max(0, now.y - 1), targetX, targetY, now.k);
        toMapBlock(&around[3], now.x, std::min(9, now.y + 1), targetX, targetY, now.k); 

        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 4; j++) {
                if (around[i].k + around[i].h > around[j].k + around[j].h) { // Sorting condition fixed
                    std::swap(around[i], around[j]);
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            if ( find(around[i], targetX, targetY)) {
                board[now.x][now.y] = 2;
                return true;
            }
        }
    }
    return false;
}

int main() {
    board[7][9] = 1;
    board[1][1] = 3;
    mapBlock enemy;
    toMapBlock(&enemy, 1, 1, 7, 9, 0);

    if (find(enemy, 7, 9)) {
        draw();
    } else {
        std::cout << "can't reach there" << std::endl;
    }
}

