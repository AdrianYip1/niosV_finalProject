#pragma once
#include <stdbool.h>

#include "../../graphics/mcWalkingDraw.h"

typedef enum {
    MC_MOVE_OK = 0,
    MC_MOVE_EXIT_LEFT,
    MC_MOVE_EXIT_RIGHT,
    MC_MOVE_EXIT_UP,
    MC_MOVE_EXIT_DOWN,
} McMoveResult;

void mcMovingInit(int startX, int startY, McFacing facing);
McMoveResult mcMovingTick(bool up, bool down, bool left, bool right, bool shift);
