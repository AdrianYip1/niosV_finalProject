#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/playerSprite.h"
#include "graphics/tiles.h"
#include "graphics/titleScreen/titleScreenDraw.h"

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

    // Draw grass into both frame buffers so there is no flicker
    for (int tile_y = 0; tile_y < NUM_TILES_Y; tile_y++) {
        for (int tile_x = 0; tile_x < NUM_TILES_X; tile_x++) {
            drawTile(tile_x, tile_y, TILE_GRASS);
        }
    }
    wait_for_vsync();
    for (int tile_y = 0; tile_y < NUM_TILES_Y; tile_y++) {
        for (int tile_x = 0; tile_x < NUM_TILES_X; tile_x++) {
            drawTile(tile_x, tile_y, TILE_GRASS);
        }
    }

    
    initTitleScreen();
    initCharizardBackSprite();

    // Track which animation frame is in each of the 2 double buffers (index 0 or 1)
    int title_frame_in_buffer[2] = { -1, -1 };
    int sprite_frame_in_buffer[2] = { -1, -1 };

    while (1) {
        wait_for_vsync();
        int back = get_back_buffer_index();

        drawTitleScreen();
        drawSpriteAnimation();
        draw_string(10, 10, "Text for testing 123", BLACK);

        title_frame_in_buffer[back] = getTitleFrameIndex();
        sprite_frame_in_buffer[back] = getSpriteFrameIndex();
    }

    return 0;
}