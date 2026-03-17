#include "sprites/mainCharacter/walking/mcWalkingEast_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingNorth_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingNorthEast_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingNorthWest_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingSouth_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingSouthEast_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingSouthWest_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingWest_frames.h"
#include "graphics.h"
#include "sprites/spriteStruct.h"
#include "sprites/playerSprites.h"
#include "mcWalkingDraw.h"

Sprite mcWalkingSprite;

void initMCWalkingSprite(int startX, int startY, McFacing facing) {
    mcWalkingSprite.x = startX;
    mcWalkingSprite.y = startY;
    mcWalkingSprite.frameIndex = 0;
    mcWalkingSprite.frameTimer = 0;

    switch (facing) {
        case MC_FACING_N:  initMCWalkingNorth(); break;
        case MC_FACING_S:  initMCWalkingSouth(); break;
        case MC_FACING_W:  initMCWalkingWest(); break;
        case MC_FACING_E:  initMCWalkingEast(); break;
        case MC_FACING_NE: initMCWalkingNorthEast(); break;
        case MC_FACING_NW: initMCWalkingNorthWest(); break;
        case MC_FACING_SE: initMCWalkingSouthEast(); break;
        case MC_FACING_SW: initMCWalkingSouthWest(); break;
        default: initMCWalkingSouth(); break;
    }
}

void drawMCWalkingAnimation(void) {
    Sprite* MCsprite = &mcWalkingSprite;

    const unsigned short* frame = MCsprite->frames[MCsprite->frameIndex];
    for (int y = 0; y < MCsprite->tileSize; y++) {
        for (int x = 0; x < MCsprite->tileSize; x++) {
            unsigned short color = frame[y * MCsprite->tileSize + x];
            if (color == 0xF81F) {
                continue; // transparent
            }
            draw_pixel(MCsprite->x + x, MCsprite->y + y, color);
        }
    }

    MCsprite->frameTimer++;
    if (MCsprite->frameTimer >= MCsprite->frameDelay) {
        MCsprite->frameTimer = 0;
        MCsprite->frameIndex = (MCsprite->frameIndex + 1) % MCsprite->frameCount; //ensure that you loop back to 1 at the end of array
    }
}

void goUp(void) {
    mcWalkingSprite.y -= 1;
}

void goDown(void) {
    mcWalkingSprite.y += 1;
}

void goLeft(void) {
    mcWalkingSprite.x -= 1;
}

void goRight(void) {
    mcWalkingSprite.x += 1;
}

void initMCWalkingEast(void) {
    mcWalkingSprite.frames = mcWalkingEastFrames;
    mcWalkingSprite.frameCount = MC_WALKING_EAST_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_WALKING_EAST_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 2;
}

void initMCWalkingNorth(void) {
    mcWalkingSprite.frames = mcWalkingNorthFrames;
    mcWalkingSprite.frameCount = MC_WALKING_NORTH_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_WALKING_NORTH_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 2;
}

void initMCWalkingNorthEast(void) {
    mcWalkingSprite.frames = mcWalkingNorthEastFrames;
    mcWalkingSprite.frameCount = MC_WALKING_NORTH_EAST_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_WALKING_NORTH_EAST_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 2;
}

void initMCWalkingNorthWest(void) {
    mcWalkingSprite.frames = mcWalkingNorthWestFrames;
    mcWalkingSprite.frameCount = MC_WALKING_NORTH_WEST_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_WALKING_NORTH_WEST_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 2;
}

void initMCWalkingSouth(void) {
    mcWalkingSprite.frames = mcWalkingSouthFrames;
    mcWalkingSprite.frameCount = MC_WALKING_SOUTH_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_WALKING_SOUTH_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 2;
}

void initMCWalkingSouthEast(void) {
    mcWalkingSprite.frames = mcWalkingSouthEastFrames;
    mcWalkingSprite.frameCount = MC_WALKING_SOUTH_EAST_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_WALKING_SOUTH_EAST_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 2;
}

void initMCWalkingSouthWest(void) {
    mcWalkingSprite.frames = mcWalkingSouthWestFrames;
    mcWalkingSprite.frameCount = MC_WALKING_SOUTH_WEST_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_WALKING_SOUTH_WEST_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 2;
}

void initMCWalkingWest(void) {
    mcWalkingSprite.frames = mcWalkingWestFrames;
    mcWalkingSprite.frameCount = MC_WALKING_WEST_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_WALKING_WEST_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 2;
}
