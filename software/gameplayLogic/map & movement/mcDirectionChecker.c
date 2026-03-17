#include <stdbool.h>


typedef enum McDirection {
    MC_DIR_NONE = 0,
    MC_DIR_N,
    MC_DIR_S,
    MC_DIR_W,
    MC_DIR_E,
    MC_DIR_NE,
    MC_DIR_NW,
    MC_DIR_SE,
    MC_DIR_SW,
    MC_DIR_INVALID // 3 or more WASD keys pressed at once
} McDirection;

static int countTrue4(bool W, bool A, bool S, bool D) {
    return (W ? 1 : 0) + (A ? 1 : 0) + (S ? 1 : 0) + (D ? 1 : 0);
}


McDirection mcPickDirection(bool up, bool down, bool left, bool right) {
    if (countTrue4(up, left, down, right) >= 3) {
        return MC_DIR_INVALID;
    }

    if (up && down) {
        up = false;
        down = false;
    }
    if (left && right) {
        left = false;
        right = false;
    }

    if (up && right) return MC_DIR_NE;
    if (up && left) return MC_DIR_NW;
    if (down && right) return MC_DIR_SE;
    if (down && left) return MC_DIR_SW;

    if (up) return MC_DIR_N;
    if (down) return MC_DIR_S;
    if (left) return MC_DIR_W;
    if (right) return MC_DIR_E;

    return MC_DIR_NONE;
}