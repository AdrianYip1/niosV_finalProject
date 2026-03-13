#include "spriteStruct.h"
#include "charizard_back_frames.h"

Sprite playerSprite;

void initCharizardBackSprite(void) {
    playerSprite.frames     = charizardFrames;
    playerSprite.frameCount = CHARIZARD_FRAME_COUNT;
    playerSprite.tileSize   = CHARIZARD_TILE_SIZE;
    playerSprite.x          = 40;   
    playerSprite.y          = 120;
    playerSprite.frameIndex = 0;
    playerSprite.frameTimer = 0;
    playerSprite.frameDelay = 5;
}

void drawSpriteAnimation(void) {
    Sprite* sprite = &playerSprite;

    //get the tiles the sprite is in
    int leftTile = sprite->x / TILE_SIZE;
    int rightTile = (sprite->x + sprite->tileSize - 1) / TILE_SIZE;
    int topTile = sprite->y / TILE_SIZE;
    int bottomTile = (sprite->y + sprite->tileSize - 1) / TILE_SIZE;

    for (int tile_y = topTile; tile_y <= bottomTile; tile_y++) {
        for (int tile_x = leftTile; tile_x <= rightTile; tile_x++) {
            drawTile(tile_x, tile_y, TILE_GRASS); //grass as defaukt for now
        }
    }

    for (int i = 0; i < sprite->tileSize; i++) {
        for (int j = 0; j < sprite->tileSize; j++) {
            draw_pixel(sprite->x + i, sprite->y + j, sprite->frames[sprite->frameIndex][i * sprite->tileSize + j]);
        }
    }

    sprite->frameTimer++;
    if (sprite->frameTimer >= sprite->frameDelay) {
        sprite->frameTimer = 0;
        sprite->frameIndex = (sprite->frameIndex + 1) % sprite->frameCount; //ensure that you loop back to 1 at the end of array
    }
}