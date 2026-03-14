#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/playerSprite.h"
#include "graphics/tiles.h"
#include "graphics/map.h"
#include "graphics/titleScreen/titleScreenDraw.h"

#define TITLE_TEXT_X 10
#define TITLE_TEXT_Y 10
#define TITLE_TEXT_STRING "Text for testing 123"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define NUM_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

int main(void)
{
    unsigned int frame_count = 0;
    int current_phase = 0;      // 0 = route, 1 = black
    short colour = BLACK;       // text colour: BLACK on route, WHITE on black

    // Init VGA and tiles
    init_graphics();
    init_predefined_graphics();
    clear_screen();

    initTitleScreen();
    unsigned int titleFrames = 0;
    while (titleFrames < 100) {
        clear_screen();   /* known back buffer state so title + text don't jitter */
        drawTitleScreen();
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, colour);
        wait_for_vsync();
        titleFrames++;
    }

    
    init_map();                 // start on route preset
    initCharizardBackSprite();

    load_map_preset(MAP_PRESET_ROUTE);
    draw_map();                 // initial background

    while (1) {
        int phase = (frame_count / 300) & 1; // swap every 5 seconds
        if (phase != current_phase) {
            current_phase = phase;
            if (current_phase == 0) {
                load_map_preset(MAP_PRESET_ROUTE);
                colour = BLACK;
            } else {
                load_map_preset(MAP_PRESET_BLACK);
                colour = WHITE;
            }
            draw_map(); // redraw full map with new preset
        }
        frame_count++;

        drawSpriteAnimationWithMap();
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, colour);
        wait_for_vsync();
    }

    return 0;
}
