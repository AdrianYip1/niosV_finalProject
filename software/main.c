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


    
    initTitleScreen();
    initCharizardBackSprite();


    while (1) {
        drawTitleScreen();                               // full background
        drawSpriteAnimation();                           // Charizard
        draw_string(10, 10, "Text for testing 123", BLACK); // text
        wait_for_vsync();                                // swap front/back
    }

    return 0;
}