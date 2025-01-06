#include "window.hpp"
#include "board.hpp"

void Window::init(int widthScreen, int heightScreen) {
    width = widthScreen;
    height = heightScreen;
}

double Window::Distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void Window::DrawGrid(SDL_Renderer* renderer, position player, int VisionRadios) {
    const double DEG_TO_RAD = M_PI / 180.0;
    const double RAD_TO_DEG = 180.0 / M_PI;
    int playerGridX = player.x / CELL_WIDTH;
    int playerGridY = player.y / CELL_HEIGHT;
    int playerCenterX = player.x + CELL_WIDTH / 2;
    int playerCenterY = player.y + CELL_HEIGHT / 2;
    int startX = std::max(0, playerGridX - VisionRadios);
    int endX = std::min(gridSize, playerGridX + VisionRadios + 1);
    int startY = std::max(0, playerGridY - VisionRadios);
    int endY = std::min(gridSize, playerGridY + VisionRadios + 1);

    struct ShadowRange {
        double minAngle, maxAngle;
    };
    const int maxShadows = gridSize * gridSize;
    ShadowRange shadowRanges[maxShadows];
    int shadowCount = 0;

    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {
            if (board[j][i] == 2) { // Wall cell
                double angles[4] = { atan2(i * CELL_HEIGHT - playerCenterY, j * CELL_WIDTH - playerCenterX) * RAD_TO_DEG,
                                     atan2((i + 1) * CELL_HEIGHT - playerCenterY, j * CELL_WIDTH - playerCenterX) * RAD_TO_DEG,
                                     atan2(i * CELL_HEIGHT - playerCenterY, (j + 1) * CELL_WIDTH - playerCenterX) * RAD_TO_DEG,
                                     atan2((i + 1) * CELL_HEIGHT - playerCenterY, (j + 1) * CELL_WIDTH - playerCenterX) * RAD_TO_DEG };
                for (int k = 0; k < 4; k++) {
                    if (angles[k] < 0) angles[k] += 360;
                }
                std::sort(angles, angles + 4);
                double minAngle = angles[0];
                double maxAngle = angles[3];
                if (maxAngle - minAngle > 180) {
                    minAngle = angles[1];
                    maxAngle = angles[2];
                }
                if (shadowCount < maxShadows) {
                    shadowRanges[shadowCount++] = {minAngle, maxAngle};
                }
            }
        }
    }

    for (int y = startY * CELL_HEIGHT; y < endY * CELL_HEIGHT; y++) {
        for (int x = startX * CELL_WIDTH; x < endX * CELL_WIDTH; x++) {
            double dist = Distance(playerCenterX, playerCenterY, x, y);
            if (dist > VisionRadios * CELL_WIDTH) continue;
            double angle = atan2(y - playerCenterY, x - playerCenterX) * RAD_TO_DEG;
            if (angle < 0) angle += 360;
            bool inShadow = false;
            for (int i = 0; i < shadowCount; i++) {
                const ShadowRange& range = shadowRanges[i];
                if (range.maxAngle - range.minAngle < 180) {
                    if (angle >= range.minAngle && angle <= range.maxAngle) {
                        inShadow = true;
                        break;
                    }
                } else {
                    if (angle >= range.maxAngle || angle <= range.minAngle) {
                        inShadow = true;
                        break;
                    }
                }
            }
            if (!inShadow && (x % CELL_WIDTH == 0 || y % CELL_HEIGHT == 0 || board[x / CELL_WIDTH][y / CELL_HEIGHT] == 2)) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}
