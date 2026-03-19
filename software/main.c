#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/playerSprite.h"
#include "gameplayLogic/map_movement/mcMoving.h"
#include "graphics/tiles.h"
#include "graphics/map.h"
#include "graphics/titleScreen/titleScreenDraw.h"
#include "graphics/textbox/textBoxSprite.h"
#include "graphics/textbox/textMessages.h"
#include "graphics/sprites/spacebar/spacebar_frames.h"
#include "../hardware/keyboard.h"
#include <stdbool.h>

#define TITLE_TEXT_X 10
#define TITLE_TEXT_Y 10


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define NUM_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

#define TEXTBOX_X 0
#define TEXTBOX_Y (SCREEN_HEIGHT - TEXT_BOX_HEIGHT)

#define SPACEBAR_X ((SCREEN_WIDTH - SPACEBAR_WIDTH) / 2)
#define SPACEBAR_Y (TEXTBOX_Y - SPACEBAR_HEIGHT - 4)
#define SPACEBAR_SPEED_FRAMES 8
#define SPACEBAR_TITLE_Y (SCREEN_HEIGHT - SPACEBAR_HEIGHT - 12)


#define TITLE_TEXT_PIXEL_WIDTH ((int)((sizeof(TEXT_TITLE) - 1) * 8))
#define TITLE_TEXT_X_CENTERED   ((SCREEN_WIDTH - TITLE_TEXT_PIXEL_WIDTH) / 2)
#define TITLE_TEXT_Y_ABOVE_BAR  (SPACEBAR_TITLE_Y - 8)

int main(void)
{

    int textboxMsgIndex = TEXTMSG_TITLE;
    const char *textboxMsg = TEXT_MESSAGES[textboxMsgIndex];
    int textboxPrevDone = 0; 
    int spacebarFrame = 0;
    int spacebarTimer = 0;

    unsigned int frame_count = 0;
    int current_phase = 0;      // 0 = route, 1 = black
    short colour = BLACK;    

    // Init VGA and tiles
    init_graphics();
    init_predefined_graphics();
    clear_screen();

    initTitleScreen();
    bool title_done = false;
    while (!title_done) { // waiting for the space key
        update_keyboard();
        
        if (is_key_space_pressed()) { 
            title_done = true;
        }

        drawTitleScreen();

        spacebarTimer++;
        if (spacebarTimer >= SPACEBAR_SPEED_FRAMES) {
            spacebarTimer = 0;
            spacebarFrame = (spacebarFrame + 1) % SPACEBAR_FRAME_COUNT;
        }
        draw_sprite_any(spacebarFrames[spacebarFrame],
                        SPACEBAR_WIDTH,
                        SPACEBAR_HEIGHT,
                        SPACEBAR_X,
                        SPACEBAR_TITLE_Y,
                        TRANSPARENT_COLOUR);

        draw_string(TITLE_TEXT_X_CENTERED, TITLE_TEXT_Y_ABOVE_BAR, TEXT_TITLE, WHITE);

        wait_for_vsync();
    }
    
    init_map();
    load_map_preset(MAP_PRESET_ROUTE);
    initCharizardBackSprite();
    mcMovingInit(80, 112, MC_FACING_S);
    

    draw_map();
    draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TEXT_TITLE, BLACK);
    textboxPrevDone = draw_textbox_animated_text(textBoxSprite,
                                                   TEXTBOX_X,
                                                   TEXTBOX_Y,
                                                   textboxMsg,
                                                   BLACK);
    wait_for_vsync();

    bool prev_space = false;
    while (1) {
        update_keyboard();
        
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
            draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TEXT_TITLE, colour);

        }
        frame_count++;

        bool up = is_key_w_pressed();
        bool down = is_key_s_pressed();
        bool left = is_key_a_pressed();
        bool right = is_key_d_pressed();
//full map redraw 
        draw_map();
        drawSpriteAnimation();
        mcMovingTick(up, down, left, right);
        
        draw_string(TITLE_TEXT_X, TITLE_TEXT_Y, TEXT_TITLE, colour);

        {
            int textboxDone = draw_textbox_animated_text(textBoxSprite,
                                                          TEXTBOX_X,
                                                          TEXTBOX_Y,
                                                          textboxMsg,
                                                          BLACK);

            bool curr_space = is_key_space_pressed();
            if (textboxDone && curr_space && !prev_space) {
                textboxMsgIndex = (textboxMsgIndex == TEXTMSG_TITLE) ? TEXTMSG_SECOND : TEXTMSG_TITLE;
                textboxMsg = TEXT_MESSAGES[textboxMsgIndex];
            }
            prev_space = curr_space;
        }
        wait_for_vsync();
    }

    return 0;
}
