#include "staticSprite.h"
#include "pokemon/charizardSprite.h"
#include "../graphics.h"
#include "../map.h"
#include "../predefined_graphics.h"
#include "../tiles.h"

static StaticSprite g_playerPokemon;

void initCharizardBackSprite(void) {
    g_playerPokemon.pixels = charizardBack;
    g_playerPokemon.width  = CHARIZARD_BACK_WIDTH;
    g_playerPokemon.height = CHARIZARD_BACK_HEIGHT;
    g_playerPokemon.x      = 40;
    g_playerPokemon.y      = 120;
}

void drawSpriteAnimation(void) {
    draw_sprite_any(g_playerPokemon.pixels, g_playerPokemon.width, g_playerPokemon.height,
                    g_playerPokemon.x, g_playerPokemon.y, TRANSPARENT_COLOUR);
}

void drawSpriteCurrentFrameOnly(void) {
    drawSpriteAnimation();
}

// restore map under
void drawSpriteAnimationWithMap(void) {
    const StaticSprite* sprite = &g_playerPokemon;

    int leftTile   = sprite->x / TILE_SIZE;
    int rightTile  = (sprite->x + sprite->width - 1) / TILE_SIZE;
    int topTile    = sprite->y / TILE_SIZE;
    int bottomTile = (sprite->y + sprite->height - 1) / TILE_SIZE;

    for (int tile_y = topTile; tile_y <= bottomTile; tile_y++) {
        for (int tile_x = leftTile; tile_x <= rightTile; tile_x++) {
            drawTile(tile_x, tile_y, map[tile_y][tile_x]);
        }
    }

    drawSpriteAnimation();
}
