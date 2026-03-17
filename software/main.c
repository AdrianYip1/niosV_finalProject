#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/playerSprite.h"
#include "gameplayLogic/map_movement/mcMoving.h"
#include "graphics/tiles.h"
#include "graphics/map.h"
#include "graphics/titleScreen/titleScreenDraw.h"
#include <stdbool.h>

#define TITLE_TEXT_X 10
#define TITLE_TEXT_Y 10
#define TITLE_TEXT_STRING "Text for testing 123"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define NUM_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

// Redraw only the map tiles under the MC's previous bounding box.
static void redraw_tiles_under_mcbounds(McBounds b)
{
    if (!b.valid) return;

    int leftTile   = b.x0 / TILE_SIZE;
    int rightTile  = b.x1 / TILE_SIZE;
    int topTile    = b.y0 / TILE_SIZE;
    int bottomTile = b.y1 / TILE_SIZE;

    if (leftTile < 0) leftTile = 0;
    if (topTile < 0) topTile = 0;
    if (rightTile >= MAP_WIDTH) rightTile = MAP_WIDTH - 1;
    if (bottomTile >= MAP_HEIGHT) bottomTile = MAP_HEIGHT - 1;

    for (int ty = topTile; ty <= bottomTile; ty++) {
        for (int tx = leftTile; tx <= rightTile; tx++) {
            drawTile(tx, ty, map[ty][tx]);
        }
    }
}

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
        drawTitleScreen();                          // full-screen title frame
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y,    
                    TITLE_TEXT_STRING, WHITE);      // draw on current back buffer
        wait_for_vsync();
 
        drawTitleScreen();                          // redraw frame on new back buffer
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y,
                    TITLE_TEXT_STRING, WHITE);
        titleFrames++;
    }

    
    init_map();                 // start on route preset

    load_map_preset(MAP_PRESET_ROUTE);
    draw_map();                 // initial background (back buffer)

    initCharizardBackSprite();


    mcMovingInit(80, 112, MC_FACING_S);

    wait_for_vsync();           // swap: start drawing into the other buffer
    draw_map();
    drawSpriteAnimationWithMap();
    draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, colour);

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

        redraw_tiles_under_mcbounds(getMCBounds());

        drawSpriteAnimationWithMap();

        
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
            // Diagonals move at half speed: only move on even frames.
            if ((frame_count & 1u) == 0u) {
                up = true; right = true;    // NE
            }
        } else if (movePhase == 5) {
            if ((frame_count & 1u) == 0u) {
                up = true; left = true;     // NW
            }
        } else if (movePhase == 6) {
            if ((frame_count & 1u) == 0u) {
                down = true; right = true;  // SE
            }
        } else if (movePhase == 7) {
            if ((frame_count & 1u) == 0u) {
                down = true; left = true;   // SW
            }
        } else {
            // movePhase == 8 -> idle (all false)
        }

        mcMovingTick(up, down, left, right);

        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TITLE_TEXT_STRING, colour);
        wait_for_vsync();
    }

    return 0;
}
