#include "spriteStruct.h"
#include "charizard_back_frames.h"
#include "../graphics.h"
#include "../map.h"
#include "../predefined_graphics.h"
#include "../tiles.h"

Sprite playerSprite;

static inline int is_magenta_key(unsigned short c) {
    const int r = (c >> 11) & 31;
    const int g = (c >> 5) & 63;
    const int b = c & 31;
    return (r >= 28 && b >= 28 && g <= 3);
}

void initCharizardBackSprite(void) {
    playerSprite.frames     = charizardFrames;
    playerSprite.frameCount = CHARIZARD_FRAME_COUNT;
    playerSprite.tileSize   = CHARIZARD_TILE_SIZE;
    playerSprite.x          = 40;   
    playerSprite.y          = 120;
    playerSprite.frameIndex = 0;
    playerSprite.frameTimer = 0;
    playerSprite.frameDelay = 2; 
}

void drawSpriteAnimation(void) {
    Sprite* sprite = &playerSprite;

    const unsigned short* frame = sprite->frames[sprite->frameIndex];
    for (int y = 0; y < sprite->tileSize; y++) {
        for (int x = 0; x < sprite->tileSize; x++) {
            unsigned short color = frame[y * sprite->tileSize + x];
            if (is_magenta_key(color)) {
                continue; // transparent
            }
            draw_pixel(sprite->x + x, sprite->y + y, color);
        }
    }

    sprite->frameTimer++;
    if (sprite->frameTimer >= sprite->frameDelay) {
        sprite->frameTimer = 0;
        sprite->frameIndex = (sprite->frameIndex + 1) % sprite->frameCount; //ensure that you loop back to 1 at the end of array
    }
}

void drawSpriteCurrentFrameOnly(void) {
    Sprite* sprite = &playerSprite;
    const unsigned short* frame = sprite->frames[sprite->frameIndex];
    for (int y = 0; y < sprite->tileSize; y++) {
        for (int x = 0; x < sprite->tileSize; x++) {
            unsigned short color = frame[y * sprite->tileSize + x];
            if (is_magenta_key(color)) continue;
            draw_pixel(sprite->x + x, sprite->y + y, color);
        }
    }
}

// restore map under
void drawSpriteAnimationWithMap(void) {
    Sprite* sprite = &playerSprite;

    int leftTile   = sprite->x / TILE_SIZE;
    int rightTile  = (sprite->x + sprite->tileSize - 1) / TILE_SIZE;
    int topTile    = sprite->y / TILE_SIZE;
    int bottomTile = (sprite->y + sprite->tileSize - 1) / TILE_SIZE;

    for (int tile_y = topTile; tile_y <= bottomTile; tile_y++) {
        for (int tile_x = leftTile; tile_x <= rightTile; tile_x++) {
            drawTile(tile_x, tile_y, map[tile_y][tile_x]);
        }
    }

    const unsigned short* frame = sprite->frames[sprite->frameIndex];
    for (int y = 0; y < sprite->tileSize; y++) {
        for (int x = 0; x < sprite->tileSize; x++) {
            unsigned short color = frame[y * sprite->tileSize + x];
            if (is_magenta_key(color)) {
                continue; // transparent
            }
            draw_pixel(sprite->x + x, sprite->y + y, color);
        }
    }

    sprite->frameTimer++;
    if (sprite->frameTimer >= sprite->frameDelay) {
        sprite->frameTimer = 0;
        sprite->frameIndex = (sprite->frameIndex + 1) % sprite->frameCount;
    }
}