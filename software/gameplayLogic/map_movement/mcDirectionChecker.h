#pragma once
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
    MC_DIR_INVALID // 3 or more direction keys pressed at once
} McDirection;

McDirection mcPickDirection(bool up, bool down, bool left, bool right);

