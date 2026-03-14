#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/playerSprite.h"
#include "graphics/tiles.h"
#include "graphics/map.h"

#define TITLE_TEXT_X 10
#define TITLE_TEXT_Y 10
#define TITLE_TEXT_STRING "Text for testing 123"

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

    init_map();                 // load route preset into map[][]
    initCharizardBackSprite();

    // Draw the full map once as background
    draw_map();
    wait_for_vsync();

    while (1) {
        drawSpriteAnimationWithMap();
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, BLACK);
        wait_for_vsync();
    }

    return 0;
}
