#include <stdbool.h>

#include "mcDirectionChecker.h"
#include "mcMoving.h"
#include "../../../hardware/keyboard.h"
#include "../../graphics/mcWalkingDraw.h"
#include "../../graphics/map.h"
#include "../../../hardware/audio.h"

static bool mcCanMoveBy(int dx, int dy);
static void tryMoveUp(void);
static void tryMoveDown(void);
static void tryMoveLeft(void);
static void tryMoveRight(void);

void mcMovingInit(int startX, int startY, McFacing facing) {
    initMCWalkingSprite(startX, startY, facing);
    drawMCWalkingAnimation(); // show MC on frame 0
}

static void moveDiagonal(McDirection dir, unsigned int stepCounter) {
    const bool even = ((stepCounter & 1u) == 0u);

    switch (dir) {
        case MC_DIR_NE:
            if (even) {
                tryMoveRight();
            } else {
                tryMoveUp();
            }
            break;
        case MC_DIR_NW:
            if (even) {
                tryMoveLeft();
            } else {
                tryMoveUp();
            }
            break;
        case MC_DIR_SE:
            if (even) {
                tryMoveRight();
            } else {
                tryMoveDown();
            }
            break;
        case MC_DIR_SW:
            if (even) {
                tryMoveLeft();
            } else {
                tryMoveDown();
            }
            break;
        default:
            break;
    }
}

static bool mcCanMoveBy(int dx, int dy) {
    const McBounds bounds = getMCBounds();
    if (!bounds.valid) {
        return true;
    }

    return map_bounds_are_walkable(bounds.x0 + dx,
                                   bounds.y0 + dy,
                                   bounds.x1 + dx,
                                   bounds.y1 + dy);
}

static void tryMoveUp(void) {
    if (mcCanMoveBy(0, -1)) {
        goUp();
    }
}

static void tryMoveDown(void) {
    if (mcCanMoveBy(0, 1)) {
        goDown();
    }
}

static void tryMoveLeft(void) {
    if (mcCanMoveBy(-1, 0)) {
        goLeft();
    }
}

static void tryMoveRight(void) {
    if (mcCanMoveBy(1, 0)) {
        goRight();
    }
}

//connect to keyboard polling
void mcMovingTick(bool up, bool down, bool left, bool right, bool shift) {
    static McDirection lastDir = MC_DIR_NONE;
    static unsigned int stepCounter = 0;

    const McDirection dir = mcPickDirection(up, down, left, right);

    if (dir != lastDir) {
        lastDir = dir;
        switch (dir) {
            case MC_DIR_N:  initMCWalkingNorth(); break;
            case MC_DIR_S:  initMCWalkingSouth(); break;
            case MC_DIR_W:  initMCWalkingWest(); break;
            case MC_DIR_E:  initMCWalkingEast(); break;
            case MC_DIR_NE: initMCWalkingNorthEast(); break;
            case MC_DIR_NW: initMCWalkingNorthWest(); break;
            case MC_DIR_SE: initMCWalkingSouthEast(); break;
            case MC_DIR_SW: initMCWalkingSouthWest(); break;
            case MC_DIR_NONE: initMCIdle(); break;
            default: break; // MC_DIR_INVALID
        }
    }

    int speed_multiplier = shift ? 2 : 1;
    for (int i = 0; i < speed_multiplier; i++) {
        stepCounter++;
        // only when mc is walking
        if (dir !=MC_DIR_NONE && dir !=MC_DIR_INVALID){
            if (stepCounter % 10 == 0) play_step_sound();
        }
        switch (dir) {
            case MC_DIR_N:
                tryMoveUp();
                break;
            case MC_DIR_S:
                tryMoveDown();
                break;
            case MC_DIR_W:
                tryMoveLeft();
                break;
            case MC_DIR_E:
                tryMoveRight();
                break;
            case MC_DIR_NE:
            case MC_DIR_NW:
            case MC_DIR_SE:
            case MC_DIR_SW:
                moveDiagonal(dir, stepCounter);
                break;
            default:
                break;
        }
    }

    if (dir == MC_DIR_NONE) {
        drawMCIdleAnimation();
    } else if (dir != MC_DIR_INVALID) {
        drawMCWalkingAnimation();
    }
}
