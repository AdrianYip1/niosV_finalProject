#include <stdbool.h>

#include "mcDirectionChecker.h"
#include "mcMoving.h"
#include "../../../hardware/keyboard.h"
#include "../../graphics/mcWalkingDraw.h"
#include "../../../hardware/audio.h"

void mcMovingInit(int startX, int startY, McFacing facing) {
    initMCWalkingSprite(startX, startY, facing);
    drawMCWalkingAnimation(); // show MC on frame 0
}

static void moveDiagonal(McDirection dir, unsigned int stepCounter) {
    const bool even = ((stepCounter & 1u) == 0u);

    switch (dir) {
        case MC_DIR_NE:
            if (even) {
                goRight();
            } else {
                goUp();
            }
            break;
        case MC_DIR_NW:
            if (even) {
                goLeft();
            } else {
                goUp();
            }
            break;
        case MC_DIR_SE:
            if (even) {
                goRight();
            } else {
                goDown();
            }
            break;
        case MC_DIR_SW:
            if (even) {
                goLeft();
            } else {
                goDown();
            }
            break;
        default:
            break;
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
        audio_update();
        switch (dir) {
            case MC_DIR_N:
                goUp();
                break;
            case MC_DIR_S:
                goDown();
                break;
            case MC_DIR_W:
                goLeft();
                break;
            case MC_DIR_E:
                goRight();
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