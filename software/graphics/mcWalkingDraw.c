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
static int g_mc_scale_num = 1;
static int g_mc_scale_den = 1;
static int g_mc_jump_frame = 0;
static const int g_mc_jump_offsets[] = { 0, 3, 6, 9, 12, 9, 6, 3, 0 };
static const int g_mc_jump_x_offsets[] = { 0, 1, 2, 3, 2, 1, 0, -1, 0 };

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

static void updateMcBoundsFromFrameAt(const unsigned short* frame, int tileSize,
                                      int drawX, int drawY) {
    const SpriteBounds b = computeSpriteBoundsNxN(frame, tileSize);
    if (b.valid) {
        const int scaled_min_x = drawX + (b.minX * g_mc_scale_num) / g_mc_scale_den;
        const int scaled_min_y = drawY + (b.minY * g_mc_scale_num) / g_mc_scale_den;
        const int scaled_max_x = drawX + (((b.maxX + 1) * g_mc_scale_num + g_mc_scale_den - 1) / g_mc_scale_den) - 1;
        const int scaled_max_y = drawY + (((b.maxY + 1) * g_mc_scale_num + g_mc_scale_den - 1) / g_mc_scale_den) - 1;
        const int bbox_height = scaled_max_y - scaled_min_y + 1;
        const int lower_body_start = scaled_min_y + (bbox_height / 2);
        g_mcBounds.x0 = scaled_min_x;
        g_mcBounds.y0 = lower_body_start;
        g_mcBounds.x1 = scaled_max_x;
        g_mcBounds.y1 = scaled_max_y;
        g_mcBounds.valid = 1;
    } else {
        g_mcBounds.x0 = drawX;
        g_mcBounds.y0 = drawY;
        g_mcBounds.x1 = drawX - 1;
        g_mcBounds.y1 = drawY - 1;
        g_mcBounds.valid = 0;
    }
}

static void offsetMcBounds(int dx, int dy) {
    if (!g_mcBounds.valid) {
        return;
    }
    g_mcBounds.x0 += dx;
    g_mcBounds.y0 += dy;
    g_mcBounds.x1 += dx;
    g_mcBounds.y1 += dy;
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
        default: initMCWalkingNorth(); break;
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

void drawMCAnimation(void) {
    Sprite* MCsprite = &mcWalkingSprite;
    const unsigned short* frame = MCsprite->frames[MCsprite->frameIndex];
    const int scaled_width = (MCsprite->tileSize * g_mc_scale_num) / g_mc_scale_den;
    const int scaled_height = (MCsprite->tileSize * g_mc_scale_num) / g_mc_scale_den;
    const int logical_draw_x = MCsprite->x - ((scaled_width - MCsprite->tileSize) / 2);
    const int logical_draw_y = MCsprite->y - (scaled_height - MCsprite->tileSize);
    const int jump_offset = (g_mc_jump_frame > 0 && g_mc_jump_frame <= (int)(sizeof(g_mc_jump_offsets) / sizeof(g_mc_jump_offsets[0])))
                                ? g_mc_jump_offsets[g_mc_jump_frame - 1]
                                : 0;
    const int jump_x_offset = (g_mc_jump_frame > 0 && g_mc_jump_frame <= (int)(sizeof(g_mc_jump_x_offsets) / sizeof(g_mc_jump_x_offsets[0])))
                                  ? g_mc_jump_x_offsets[g_mc_jump_frame - 1]
                                  : 0;
    const int draw_x = logical_draw_x + jump_x_offset;
    const int draw_y = logical_draw_y - jump_offset;

    for (int y = 0; y < scaled_height; y++) {
        const int src_y = (y * g_mc_scale_den) / g_mc_scale_num;
        for (int x = 0; x < scaled_width; x++) {
            const int src_x = (x * g_mc_scale_den) / g_mc_scale_num;
            unsigned short color = frame[src_y * MCsprite->tileSize + src_x];
            if (color == TRANSPARENT_COLOUR) continue;
            draw_pixel(draw_x + x, draw_y + y, color);
        }
    }

    updateMcBoundsFromFrameAt(frame, MCsprite->tileSize, logical_draw_x, logical_draw_y);

    MCsprite->frameTimer++;
    if (MCsprite->frameTimer >= MCsprite->frameDelay) {
        MCsprite->frameTimer = 0;
        if (MCsprite->frameCount > 0) {
            MCsprite->frameIndex = (MCsprite->frameIndex + 1) % MCsprite->frameCount;
        }
    }
    if (g_mc_jump_frame > 0) {
        g_mc_jump_frame++;
        if (g_mc_jump_frame > (int)(sizeof(g_mc_jump_offsets) / sizeof(g_mc_jump_offsets[0]))) {
            g_mc_jump_frame = 0;
        }
    }
}

void drawMCAnimationPaused(void) {
    Sprite* MCsprite = &mcWalkingSprite;
    const unsigned short* frame = MCsprite->frames[MCsprite->frameIndex];
    const int scaled_width = (MCsprite->tileSize * g_mc_scale_num) / g_mc_scale_den;
    const int scaled_height = (MCsprite->tileSize * g_mc_scale_num) / g_mc_scale_den;
    const int logical_draw_x = MCsprite->x - ((scaled_width - MCsprite->tileSize) / 2);
    const int logical_draw_y = MCsprite->y - (scaled_height - MCsprite->tileSize);
    const int jump_offset = (g_mc_jump_frame > 0 && g_mc_jump_frame <= (int)(sizeof(g_mc_jump_offsets) / sizeof(g_mc_jump_offsets[0])))
                                ? g_mc_jump_offsets[g_mc_jump_frame - 1]
                                : 0;
    const int jump_x_offset = (g_mc_jump_frame > 0 && g_mc_jump_frame <= (int)(sizeof(g_mc_jump_x_offsets) / sizeof(g_mc_jump_x_offsets[0])))
                                  ? g_mc_jump_x_offsets[g_mc_jump_frame - 1]
                                  : 0;
    const int draw_x = logical_draw_x + jump_x_offset;
    const int draw_y = logical_draw_y - jump_offset;

    for (int y = 0; y < scaled_height; y++) {
        const int src_y = (y * g_mc_scale_den) / g_mc_scale_num;
        for (int x = 0; x < scaled_width; x++) {
            const int src_x = (x * g_mc_scale_den) / g_mc_scale_num;
            unsigned short color = frame[src_y * MCsprite->tileSize + src_x];
            if (color == TRANSPARENT_COLOUR) continue;
            draw_pixel(draw_x + x, draw_y + y, color);
        }
    }

    updateMcBoundsFromFrameAt(frame, MCsprite->tileSize, logical_draw_x, logical_draw_y);

    if (g_mc_jump_frame > 0) {
        g_mc_jump_frame++;
        if (g_mc_jump_frame > (int)(sizeof(g_mc_jump_offsets) / sizeof(g_mc_jump_offsets[0]))) {
            g_mc_jump_frame = 0;
        }
    }
}

void drawMCIdleAnimation(void) { drawMCAnimation(); }
void drawMCWalkingAnimation(void) { drawMCAnimation(); }

McBounds getMCBounds(void) {
    return g_mcBounds;
}

void getMCPosition(int *x, int *y) {
    if (x != 0) {
        *x = mcWalkingSprite.x;
    }
    if (y != 0) {
        *y = mcWalkingSprite.y;
    }
}

void setMCScale(int numerator, int denominator) {
    if (numerator <= 0 || denominator <= 0) {
        g_mc_scale_num = 1;
        g_mc_scale_den = 1;
        return;
    }
    g_mc_scale_num = numerator;
    g_mc_scale_den = denominator;
}

void startMCJumpEffect(void) {
    if (g_mc_jump_frame == 0) {
        g_mc_jump_frame = 1;
    }
}

void goUp(void) {
    mcWalkingSprite.y -= 1;
    offsetMcBounds(0, -1);
}

void goDown(void) {
    mcWalkingSprite.y += 1;
    offsetMcBounds(0, 1);
}

void goLeft(void) {
    mcWalkingSprite.x -= 1;
    offsetMcBounds(-1, 0);
}

void goRight(void) {
    mcWalkingSprite.x += 1;
    offsetMcBounds(1, 0);
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
