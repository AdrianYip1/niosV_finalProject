#include "graphics.h"
#include "sprites/mainCharacter/walking/mcWalkingEast_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingNorth_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingNorthEast_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingNorthWest_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingSouth_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingSouthEast_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingSouthWest_frames.h"
#include "sprites/mainCharacter/walking/mcWalkingWest_frames.h"
#include "sprites/spriteStruct.h"
#include "mcWalkingDraw.h"
#include "sprites/mainCharacter/idle/mcIdle_frames.h"

Sprite mcWalkingSprite;
static McBounds g_mcBounds = {0, 0, -1, -1, 0}; //the stored bounding box

typedef struct {
    int minX;
    int minY;
    int maxX;
    int maxY;
    int valid;
} SpriteBounds;

static SpriteBounds computeSpriteBoundsNxN(const unsigned short* frame, int n) {
    SpriteBounds b;
    b.minX = n;
    b.minY = n;
    b.maxX = -1;
    b.maxY = -1;
    b.valid = 0;

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            const unsigned short c = frame[y * n + x];
            if (c == TRANSPARENT_COLOUR) continue;

            if (x < b.minX) b.minX = x;
            if (y < b.minY) b.minY = y;
            if (x > b.maxX) b.maxX = x;
            if (y > b.maxY) b.maxY = y;
            b.valid = 1;
        }
    }

    return b;
}

static void updateMcBoundsFromFrameAt(const unsigned short* frame, int tileSize, int spriteX, int spriteY) {
    const SpriteBounds b = computeSpriteBoundsNxN(frame, tileSize);
    if (b.valid) {
        g_mcBounds.x0 = spriteX + b.minX;
        g_mcBounds.y0 = spriteY + b.minY;
        g_mcBounds.x1 = spriteX + b.maxX;
        g_mcBounds.y1 = spriteY + b.maxY;
        g_mcBounds.valid = 1;
    } else {
        g_mcBounds.x0 = spriteX;
        g_mcBounds.y0 = spriteY;
        g_mcBounds.x1 = spriteX - 1;
        g_mcBounds.y1 = spriteY - 1;
        g_mcBounds.valid = 0;
    }
}

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

void initMCIdle(void) {
    mcWalkingSprite.frames = mcIdleFrames;
    mcWalkingSprite.frameCount = MC_IDLE_FRAME_COUNT;
    mcWalkingSprite.tileSize = MC_IDLE_TILE_SIZE;
    if (mcWalkingSprite.frameCount > 0) {
        mcWalkingSprite.frameIndex %= mcWalkingSprite.frameCount;
        if (mcWalkingSprite.frameIndex < 0) mcWalkingSprite.frameIndex += mcWalkingSprite.frameCount;
    } else {
        mcWalkingSprite.frameIndex = 0;
    }
    mcWalkingSprite.frameTimer = 0;
    mcWalkingSprite.frameDelay = 6; 
}

void drawMCIdleAnimation(void) {
    Sprite* MCsprite = &mcWalkingSprite;
    const unsigned short* frame = MCsprite->frames[MCsprite->frameIndex];

    for (int y = 0; y < MCsprite->tileSize; y++) {
        for (int x = 0; x < MCsprite->tileSize; x++) {
            unsigned short color = frame[y * MCsprite->tileSize + x];
            if (color == TRANSPARENT_COLOUR) continue;
            draw_pixel(MCsprite->x + x, MCsprite->y + y, color);
        }
    }

    updateMcBoundsFromFrameAt(frame, MCsprite->tileSize, MCsprite->x, MCsprite->y);

    MCsprite->frameTimer++;
    if (MCsprite->frameTimer >= MCsprite->frameDelay) {
        MCsprite->frameTimer = 0;
        if (MCsprite->frameCount > 0) {
            MCsprite->frameIndex = (MCsprite->frameIndex + 1) % MCsprite->frameCount;
        }
    }
}

void drawMCWalkingAnimation(void) {
    Sprite* MCsprite = &mcWalkingSprite;

    const unsigned short* frame = MCsprite->frames[MCsprite->frameIndex];
    for (int y = 0; y < MCsprite->tileSize; y++) {
        for (int x = 0; x < MCsprite->tileSize; x++) {
            unsigned short color = frame[y * MCsprite->tileSize + x];
            if (color == TRANSPARENT_COLOUR) {
                continue; // transparent
            }
            draw_pixel(MCsprite->x + x, MCsprite->y + y, color);
        }
    }

    updateMcBoundsFromFrameAt(frame, MCsprite->tileSize, MCsprite->x, MCsprite->y);

    MCsprite->frameTimer++;
    if (MCsprite->frameTimer >= MCsprite->frameDelay) {
        MCsprite->frameTimer = 0;
        MCsprite->frameIndex = (MCsprite->frameIndex + 1) % MCsprite->frameCount; //ensure that you loop back to 1 at the end of array
    }
}

McBounds getMCBounds(void) {
    return g_mcBounds;
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
