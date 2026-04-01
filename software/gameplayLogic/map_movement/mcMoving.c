#include <stdbool.h>

#include "mcDirectionChecker.h"
#include "mcMoving.h"
#include "../../../hardware/keyboard.h"
#include "../../graphics/mcWalkingDraw.h"
#include "../../graphics/map.h"
#include "../../../hardware/audio.h"
#include "../../se/wallbump_audio.h"

static bool mcCanMoveBy(int dx, int dy);
static bool tryMoveUp(void);
static bool tryMoveDown(void);
static bool tryMoveLeft(void);
static bool tryMoveRight(void);
static McMoveResult mcEdgeExitResult(McDirection dir);

// Tick state is kept across frames; reset on map load/teleport so held directions work correctly.
static McDirection g_lastDir = MC_DIR_INVALID;
static unsigned int g_stepCounter = 0;
static bool g_bumpLatch = false;

void mcMovingInit(int startX, int startY, McFacing facing) {
    initMCWalkingSprite(startX, startY, facing);
    // Default to idle on spawn/teleport
    initMCIdle();
    drawMCIdleAnimation();

    // Force the first tick on the new map to re-initialize the walking animation if a direction is held.
    g_lastDir = MC_DIR_INVALID;
    g_stepCounter = 0;
    g_bumpLatch = false;
}

static bool moveDiagonal(McDirection dir, unsigned int stepCounter) {
    const bool even = ((stepCounter & 1u) == 0u);

    switch (dir) {
        case MC_DIR_NE:
            if (even) {
                return tryMoveRight();
            } else {
                return tryMoveUp();
            }
        case MC_DIR_NW:
            if (even) {
                return tryMoveLeft();
            } else {
                return tryMoveUp();
            }
        case MC_DIR_SE:
            if (even) {
                return tryMoveRight();
            } else {
                return tryMoveDown();
            }
        case MC_DIR_SW:
            if (even) {
                return tryMoveLeft();
            } else {
                return tryMoveDown();
            }
        default:
            return false;
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
                                   bounds.y1 + dy,
                                   dx,
                                   dy);
}

static bool tryMoveUp(void) {
    if (mcCanMoveBy(0, -1)) {
        goUp();
        return true;
    }
    return false;
}

static bool tryMoveDown(void) {
    if (mcCanMoveBy(0, 1)) {
        goDown();
        return true;
    }
    return false;
}

static bool tryMoveLeft(void) {
    if (mcCanMoveBy(-1, 0)) {
        goLeft();
        return true;
    }
    return false;
}

static bool tryMoveRight(void) {
    if (mcCanMoveBy(1, 0)) {
        goRight();
        return true;
    }
    return false;
}

//connect to keyboard polling
McMoveResult mcMovingTick(bool up, bool down, bool left, bool right, bool shift) {
    const McDirection dir = mcPickDirection(up, down, left, right);

    if (dir != g_lastDir) {
        g_lastDir = dir;
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

    int speed_multiplier = shift ? 6 : 2;
    bool movedThisTick = false;
    for (int i = 0; i < speed_multiplier; i++) {
        g_stepCounter++;
        // only when mc is walking
        if (dir !=MC_DIR_NONE && dir !=MC_DIR_INVALID){
            if (g_stepCounter % 10 == 0) play_step_sound();
        }
        switch (dir) {
            case MC_DIR_N:
                movedThisTick = tryMoveUp() || movedThisTick;
                break;
            case MC_DIR_S:
                movedThisTick = tryMoveDown() || movedThisTick;
                break;
            case MC_DIR_W:
                movedThisTick = tryMoveLeft() || movedThisTick;
                break;
            case MC_DIR_E:
                movedThisTick = tryMoveRight() || movedThisTick;
                break;
            case MC_DIR_NE:
            case MC_DIR_NW:
            case MC_DIR_SE:
            case MC_DIR_SW:
                movedThisTick = moveDiagonal(dir, g_stepCounter) || movedThisTick;
                break;
            default:
                break;
        }
    }

    if (dir == MC_DIR_NONE || dir == MC_DIR_INVALID) {
        g_bumpLatch = false;
    } else if (movedThisTick) {
        g_bumpLatch = false;
    } else if (!g_bumpLatch) {
        play_sfx(wallbump_audio, wallbump_audio_len);
        g_bumpLatch = true;
    }

    if (dir == MC_DIR_NONE) {
        drawMCIdleAnimation();
    } else if (dir != MC_DIR_INVALID) {
        drawMCWalkingAnimation();
    }

    if (!movedThisTick) {
        return mcEdgeExitResult(dir);
    }
    return MC_MOVE_OK;
}

static McMoveResult mcEdgeExitResult(McDirection dir) {
    const McBounds bounds = getMCBounds();
    if (!bounds.valid) {
        return MC_MOVE_OK;
    }

    if ((dir == MC_DIR_W || dir == MC_DIR_NW || dir == MC_DIR_SW) && bounds.x0 <= 0) {
        return MC_MOVE_EXIT_LEFT;
    }

    if ((dir == MC_DIR_E || dir == MC_DIR_NE || dir == MC_DIR_SE) &&
        bounds.x1 >= (MAP_WIDTH * TILE_SIZE) - 1) {
        return MC_MOVE_EXIT_RIGHT;
    }

    if ((dir == MC_DIR_N || dir == MC_DIR_NE || dir == MC_DIR_NW) && bounds.y0 <= 0) {
        return MC_MOVE_EXIT_UP;
    }

    if ((dir == MC_DIR_S || dir == MC_DIR_SE || dir == MC_DIR_SW) &&
        bounds.y1 >= (MAP_HEIGHT * TILE_SIZE) - 1) {
        return MC_MOVE_EXIT_DOWN;
    }

    return MC_MOVE_OK;
}
