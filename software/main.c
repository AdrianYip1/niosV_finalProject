#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/playerSprite.h"
#include "gameplayLogic/map_movement/mcMoving.h"
#include "graphics/tiles.h"
#include "graphics/map.h"
#include "graphics/titleScreen/titleScreenDraw.h"
#include "graphics/textbox/textBoxSprite.h"
#include <stdbool.h>

#define TITLE_TEXT_X 10
#define TITLE_TEXT_Y 10
#define TITLE_TEXT_STRING "Text for testing 123"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define NUM_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

#define TEXTBOX_X 0
#define TEXTBOX_Y (SCREEN_HEIGHT - TEXT_BOX_HEIGHT)

int main(void)
{
    unsigned int frame_count = 0;
    int current_phase = 0;      // 0 = route, 1 = black
    short colour = BLACK;    

    // Init VGA and tiles
    init_graphics();
    init_predefined_graphics();
    clear_screen();

    initTitleScreen();
    unsigned int titleFrames = 0;
    while (titleFrames < 100) {
        drawTitleScreen();
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, WHITE);
        wait_for_vsync();
        titleFrames++;
        
        drawTitleScreen();
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, WHITE);
        wait_for_vsync();
        titleFrames++;
    }
    
    init_map();
    load_map_preset(MAP_PRESET_ROUTE);
    initCharizardBackSprite();
    mcMovingInit(80, 112, MC_FACING_S);
    

    draw_map();
    draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, BLACK);
    draw_textbox_animated_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, TITLE_TEXT_STRING, BLACK);
    wait_for_vsync();

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

        

            draw_map();
            draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, colour);

        }
        frame_count++;

        bool up = false, down = false, left = false, right = false;
        int t = (int)(frame_count % 540);   // 9 phases of 60 frames
        int movePhase = t / 60;
        if (movePhase == 0) {
            right = true;                   // E
        } else if (movePhase == 1) {
            down = true;                    // S
        } else if (movePhase == 2) {
            left = true;                    // W
        } else if (movePhase == 3) {
            up = true;                      // N
        } else if (movePhase == 4) {
            up = true; right = true;        // NE
        } else if (movePhase == 5) {
            up = true; left = true;         // NW
        } else if (movePhase == 6) {
            down = true; right = true;      // SE
        } else if (movePhase == 7) {
            down = true; left = true;       // SW
        } else {
            // movePhase == 8 -> idle (all false)
        }
//full map redraw 
        draw_map();
        drawSpriteAnimation();
        mcMovingTick(up, down, left, right);
        
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, colour);
        draw_textbox_animated_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, TITLE_TEXT_STRING, BLACK);
        wait_for_vsync();
    }

    return 0;
}
