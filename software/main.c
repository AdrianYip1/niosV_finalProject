#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/playerSprite.h"
#include "graphics/tiles.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define NUM_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)


int main(void)
{
    // Init VGA and tiles
    init_graphics();
    init_predefined_graphics();

    clear_screen();

    draw_sprite_transparent(player_up_1_sprite, 100, 100, PINK_TRANSPARENT);

    for (int tile_y = 0; tile_y < NUM_TILES_Y; tile_y++) {
        for (int tile_x = 0; tile_x < NUM_TILES_X; tile_x++) {
            drawTile(tile_x, tile_y, TILE_GRASS);
        }
    }

    initCharizardBackSprite();
    

    // Draw some text over the tiles
    draw_string(10, 10, "HELLO WORLD", GREEN);

    while (1) {
        drawSpriteAnimation();

        wait_for_vsync();
    }

    return 0;
}