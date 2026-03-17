#include <stdbool.h>

#include "mcDirectionChecker.h"
#include "mcMoving.h"
#include "../../../hardware/keyboard.h"
#include "../../../graphics/mcWalkingDraw.h"


void mcMovingInit(int startX, int startY, McFacing facing) {
    initMCWalkingSprite(startX, startY, facing);
    drawMCWalkingAnimation(); // show MC on frame 0
}

void mcMovingTick(bool up, bool down, bool left, bool right) {
    static McDirection lastDir = MC_DIR_NONE;

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
            default: break; 
        }
    }

    switch (dir) {
        case MC_DIR_N:  goUp(); break;
        case MC_DIR_S:  goDown(); break;
        case MC_DIR_W:  goLeft(); break;
        case MC_DIR_E:  goRight(); break;
        case MC_DIR_NE: goUp(); goRight(); break;
        case MC_DIR_NW: goUp(); goLeft(); break;
        case MC_DIR_SE: goDown(); goRight(); break;
        case MC_DIR_SW: goDown(); goLeft(); break;
        default: break; 
    }

    if (dir != MC_DIR_NONE && dir != MC_DIR_INVALID) {
        drawMCWalkingAnimation();
    }
}