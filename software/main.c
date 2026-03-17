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

// draws only the map tiles covered by the textbox region.

static void redraw_tiles_under_textbox(void)
{
    int leftTile = 0;
    int rightTile = (SCREEN_WIDTH - 1) / TILE_SIZE;

    int topTile = TEXTBOX_Y / TILE_SIZE;
    int bottomTile = (TEXTBOX_Y + TEXT_BOX_HEIGHT - 1) / TILE_SIZE;

    // Clamp to map bounds just in case.
    if (topTile < 0) topTile = 0;
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
        drawTitleScreen(); // full-screen title frame

        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y,
                    TITLE_TEXT_STRING, WHITE);
        wait_for_vsync();
        titleFrames++;
    }

    
    init_map();                 // start on route preset

    load_map_preset(MAP_PRESET_ROUTE);
    draw_map();                 // initial background (back buffer)

    initCharizardBackSprite();

    mcMovingInit(80, 112, MC_FACING_S);

    draw_sprite_any(textBoxSprite,
                     TEXT_BOX_WIDTH,
                     TEXT_BOX_HEIGHT,
                     TEXTBOX_X,
                     TEXTBOX_Y,
                     TRANSPARENT_COLOUR);
    draw_string(TEXTBOX_X + 30, TEXTBOX_Y + 30, TITLE_TEXT_STRING, BLACK);

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
            draw_map(); // redraw full map on preset switch
        }
        frame_count++;


        redraw_tiles_under_textbox();

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

        mcMovingTick(up, down, left, right);

        draw_sprite_any(textBoxSprite,
                        TEXT_BOX_WIDTH,
                        TEXT_BOX_HEIGHT,
                        TEXTBOX_X,
                        TEXTBOX_Y,
                        TRANSPARENT_COLOUR);

        // Draw text on top of the textbox
        draw_string(TEXTBOX_X + 30, TEXTBOX_Y + 30, TITLE_TEXT_STRING, BLACK);
        wait_for_vsync();
    }

    return 0;
}
