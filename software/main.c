#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/pokemon/charizardSprite.h"
#include "graphics/sprites/pokemon/pokemonSpriteInit.h"
#include "graphics/sprites/staticSprite.h"
#include "gameplayLogic/map_movement/mcMoving.h"
#include "graphics/tiles.h"
#include "graphics/map.h"
#include "graphics/titleScreen/titleScreenDraw.h"
#include "graphics/textbox/textBoxSprite.h"
#include "graphics/textbox/textMessages.h"
#include "graphics/sprites/spacebar/spacebar_frames.h"
#include "graphics/sprites/arrowGif/arrowGif_frames.h"
#include "../hardware/keyboard.h"
#include "textinput/getTextFromUser.h"
#include "graphics/textbox/small_spacebar.h"
#include "graphics/sprites/battleicons/battle_icons.h"
#include "graphics/sprites/battleIcons/battleHp/myHpEmpty.h"
#include "graphics/sprites/battleIcons/battleHp/oppHpEmpty.h"
#include "graphics/sprites/battleIcons/battleHp/burned.h"
#include "graphics/sprites/battleIcons/battleHp/caught.h"
#include "graphics/sprites/battleIcons/battleHp/frozen.h"
#include "graphics/sprites/battleIcons/battleHp/para.h"
#include "graphics/sprites/battleIcons/battleHp/poison.h"
#include "graphics/sprites/battleIcons/battleHp/sleep.h"
#include "graphics/sprites/battleParty/battlePartySprite.h"
#include "graphics/sprites/boxSprites/charizardBoxSprite.h"
#include "graphics/sprites/boxSprites/charmeleonBoxSprite.h"
#include "graphics/sprites/boxSprites/charmanderBoxSprite.h"
#include "graphics/sprites/battleUIBackground/battleUIBackgroundSprite.h"
#include <stdbool.h>

#define TITLE_TEXT_X 10
#define TITLE_TEXT_Y 10

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define NUM_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define NUM_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

#define TEXTBOX_X 0
#define TEXTBOX_Y (SCREEN_HEIGHT - TEXT_BOX_HEIGHT)


#define BATTLE_ICON_FIGHT_X 0
#define BATTLE_ICON_FIGHT_Y (240 - 3 - BATTLE_ICON_FIGHT_HEIGHT)

#define BATTLE_ICON_BAG_X (BATTLE_ICON_FIGHT_X + BATTLE_ICON_FIGHT_WIDTH - 3)
#define BATTLE_ICON_BAG_Y (BATTLE_ICON_FIGHT_Y)

#define BATTLE_ICON_RUN_X (BATTLE_ICON_BAG_X)
#define BATTLE_ICON_RUN_Y (230 - BATTLE_ICON_SMALL_HEIGHT + 12)

#define BATTLE_PARTY_X 320 / 2 + 14
#define BATTLE_PARTY_Y 240 - 91 + 1

#define OPP_HP_EMPTY_X 0
#define OPP_HP_EMPTY_Y 15
#define MY_HP_EMPTY_X  340 - MY_HP_EMPTY_WIDTH - 20
#define MY_HP_EMPTY_Y  (TEXTBOX_Y - MY_HP_EMPTY_HEIGHT - 20)

#define SPACEBAR_X ((SCREEN_WIDTH - SPACEBAR_WIDTH) / 2)
#define SPACEBAR_Y (TEXTBOX_Y - SPACEBAR_HEIGHT - 6)
#define SPACEBAR_SPEED_FRAMES 8
#define SPACEBAR_TITLE_Y (SCREEN_HEIGHT - SPACEBAR_HEIGHT - 5)

#define SMALL_SPACEBAR_X SPACEBAR_X + SPACEBAR_WIDTH + 75
#define SMALL_SPACEBAR_Y SPACEBAR_Y + SPACEBAR_HEIGHT + 50


#define TITLE_TEXT_PIXEL_WIDTH ((int)((sizeof(TEXT_TITLE) - 1) * 8))
#define TITLE_TEXT_X_CENTERED   ((SCREEN_WIDTH - TITLE_TEXT_PIXEL_WIDTH) / 2)
#define TITLE_TEXT_Y_ABOVE_BAR  (SPACEBAR_TITLE_Y - 12)

//location for battle switch party
#define PARTY_1_X 170 - 2
#define PARTY_1_Y 240 - 91 - 18

#define PARTY_2_X 210 + 1
#define PARTY_2_Y 240 - 91 - 18

#define PARTY_3_X 250 + 3
#define PARTY_3_Y 240 - 91 - 18

#define PARTY_4_X 173 - 2
#define PARTY_4_Y 240 - 91 + 23

#define PARTY_5_X 213 + 1
#define PARTY_5_Y 240 - 91 + 23

#define PARTY_6_X 253 + 3
#define PARTY_6_Y 240 - 91 + 23

#define battleBackdropY 240 - 91

//location for status effects for opponent hp

//location for drawing names, hp, level


// Game States
typedef enum {
    GAME_STATE_MAP = 0,
    GAME_STATE_BATTLE = 1,
} GameState;


//states for battle
typedef enum {
    BATTLE_UI_MENU = 0,
    BATTLE_UI_ATTACK_MENU = 1,
    BATTLE_UI_BAG_MENU = 2,
} BattleUiState;

typedef enum {
    ARROW_CTX_NONE = 0,
    ARROW_CTX_BATTLE_MENU = 1,
    ARROW_CTX_BATTLE_ATTACK = 2,
    ARROW_CTX_BATTLE_BAG = 3,
} ArrowContext;

static ArrowContext getArrowContext(GameState state, BattleUiState battleUi) {
    if (state != GAME_STATE_BATTLE) return ARROW_CTX_NONE;
    if (battleUi == BATTLE_UI_ATTACK_MENU) return ARROW_CTX_BATTLE_ATTACK;
    if (battleUi == BATTLE_UI_BAG_MENU) return ARROW_CTX_BATTLE_BAG;
    return ARROW_CTX_BATTLE_MENU;
}

static int arrowCursorCount(ArrowContext ctx) {
    switch (ctx) {
        case ARROW_CTX_BATTLE_MENU: return 9;   // Fight/Bag/Run/party members
        case ARROW_CTX_BATTLE_ATTACK: return 4; 
        case ARROW_CTX_BATTLE_BAG: return 1;   
        default: return 0;
    }
}

static void arrowGetPos(ArrowContext ctx, int index, int* out_x, int* out_y) {
    if (!out_x || !out_y) return;

    if (ctx == ARROW_CTX_BATTLE_BAG) {
        *out_x = BATTLE_ICON_BAG_X + (BATTLE_ICON_SMALL_WIDTH / 2) - (ARROWGIF_WIDTH / 2);
        *out_y = BATTLE_ICON_BAG_Y - ARROWGIF_HEIGHT - 2;
        return;
    }

    if (ctx == ARROW_CTX_BATTLE_ATTACK) {

        const int baseX = 30;
        const int baseY = TEXTBOX_Y + 10;
        const int stepX = 120;
        const int stepY = 18;
        if (index < 0) index = 0;
        if (index > 3) index = 3;
        const int row = index / 2;
        const int col = index % 2;
        *out_x = baseX + (col * stepX);
        *out_y = baseY + (row * stepY);
        return;
    }

    if (ctx == ARROW_CTX_BATTLE_MENU) {
        const int xs[9] = {
            BATTLE_ICON_FIGHT_X + (BATTLE_ICON_FIGHT_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
            BATTLE_ICON_BAG_X + (BATTLE_ICON_SMALL_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
            BATTLE_ICON_RUN_X + (BATTLE_ICON_SMALL_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
            PARTY_1_X + (CHARIZARD_BOX_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
            PARTY_2_X + (CHARIZARD_BOX_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
            PARTY_3_X + (CHARIZARD_BOX_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
            PARTY_4_X + (CHARIZARD_BOX_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
            PARTY_5_X + (CHARIZARD_BOX_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
            PARTY_6_X + (CHARIZARD_BOX_WIDTH / 2) - (ARROWGIF_WIDTH / 2),
        };
        const int ys[9] = {
            BATTLE_ICON_FIGHT_Y - ARROWGIF_HEIGHT - 2,
            BATTLE_ICON_BAG_Y - ARROWGIF_HEIGHT - 2,
            BATTLE_ICON_RUN_Y - ARROWGIF_HEIGHT - 2,
            PARTY_1_Y - ARROWGIF_HEIGHT - 2,
            PARTY_2_Y - ARROWGIF_HEIGHT - 2,
            PARTY_3_Y - ARROWGIF_HEIGHT - 2,
            PARTY_4_Y - ARROWGIF_HEIGHT - 2,
            PARTY_5_Y - ARROWGIF_HEIGHT - 2,
            PARTY_6_Y - ARROWGIF_HEIGHT - 2,
        };
        if (index < 0) index = 0;
        if (index > 8) index = 8;
        *out_x = xs[index];
        *out_y = ys[index];
        return;
    }

    *out_x = 0;
    *out_y = 0;
}

// Navigation map for the 9 "battle menu" cursor targets (0..8).
// Index meanings:
// 0=Fight, 1=Bag, 2=Run, 3..8=Party slots 1..6 (left->right, top row then bottom row).
typedef enum { DIR_UP = 0, DIR_LEFT = 1, DIR_DOWN = 2, DIR_RIGHT = 3 } NavDir;
static int navBattleMenu9(int index, NavDir dir) {
    static const signed char nav[9][4] = {
        /*0 Fight*/ {0, 0, 0, 1},
        /*1 Bag */ {1, 0, 2, 3},
        /*2 Run */ {1, 0, 2, 3},
        /*3 P1  */ {3, 1, 6, 4},
        /*4 P2  */ {4, 3, 7, 5},
        /*5 P3  */ {5, 4, 8, 5},
        /*6 P4  */ {3, 1, 6, 7},
        /*7 P5  */ {4, 6, 7, 8},
        /*8 P6  */ {5, 7, 8, 8},
    };

    if (index < 0) index = 0;
    if (index > 8) index = 8;

    int d = (int)dir;
    if (d < 0) d = 0;
    if (d > 3) d = 3;

    const int next = (int)nav[index][d];
    if (next < 0 || next > 8) return index;
    return next;
}


int main(void)
{

    int textboxMsgIndex = 0;
    const char *textboxMsg = TEXT_MESSAGES[textboxMsgIndex];
    int textboxPreviousDone = 0;

    char msg3Render[96];
    char msg4Render[96];
    int spacebarFrame = 0;
    int spacebarTimer = 0;

    unsigned int frame_count = 0;
    int current_phase = 0;      // 0 = route, 1 = black
    short colour = BLACK;    
    int bobFrame = 0;
    int bobTimer = 0;
    const int bobSpeedFrames = 3;
    int shadePulseFrame = 0;
    int shakeFrame = 0;
    int shakeTimer = 0;
    const int shakeSpeedFrames = 3;

    GameState gameState = GAME_STATE_BATTLE;
    BattleUiState battleUiState = BATTLE_UI_MENU;
    GameState prevGameState = gameState;
    int cursorIndex = 0;
    int arrowAnimFrame = 0;
    int arrowAnimTimer = 0;
    const int arrowAnimSpeedFrames = 8;
    bool prevW = false, prevA = false, prevS = false, prevD = false;

    StaticSprite charizardBackSprite;
    initPokemonBackBattleSpriteDefault(&charizardBackSprite, POKEMON_ID_CHARIZARD);
    StaticSprite charmanderFrontSprite;
    initPokemonFrontBattleSpriteDefault(&charmanderFrontSprite, POKEMON_ID_CHARMANDER);

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

        draw_string(TITLE_TEXT_X_CENTERED, TITLE_TEXT_Y_ABOVE_BAR, textboxMsg, WHITE);

        wait_for_vsync();
    }

    textboxMsgIndex++;
    textboxMsg = TEXT_MESSAGES[textboxMsgIndex];

    init_map();
    load_map_preset(MAP_PRESET_BLACK);
    draw_map();

    // Ensure both buffers have the same background before typing.
    wait_for_vsync();
    draw_map();
    wait_for_vsync();


    getTextFromUserIntoTextbox(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, BLACK, textboxMsg);
    

    textboxMsgIndex++;
    const char *userName = getUserText();
    formatTextWithNameToken(msg3Render, sizeof(msg3Render), TEXT_MESSAGES[textboxMsgIndex], userName);
    textboxMsgIndex++;
    formatTextWithNameToken(msg4Render, sizeof(msg4Render), TEXT_MESSAGES[textboxMsgIndex], userName);

    int textboxDone = 0;
    bool prevSpaceDown = false;

    // Wait for msg3 typing to finish, then require a space press to continue.
    prevSpaceDown = false;
    while (1) {
        update_keyboard();

        textboxDone = draw_textbox_animated_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, msg3Render, BLACK);

        const bool spaceDown = is_key_space_pressed();
        const bool spacePressed = (spaceDown && !prevSpaceDown);
        prevSpaceDown = spaceDown;

        if (textboxDone && spacePressed) break;
        wait_for_vsync();
    }

    // Wait for msg4 typing to finish, then require a space press to continue.
    prevSpaceDown = false;
    while (1) {
        update_keyboard();

        textboxDone = draw_textbox_animated_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, msg4Render, BLACK);

        const bool spaceDown = is_key_space_pressed();
        const bool spacePressed = (spaceDown && !prevSpaceDown);
        prevSpaceDown = spaceDown;

        if (textboxDone && spacePressed) break;
        wait_for_vsync();
    }

    // Initial state setup (press '1' for battle, '2' for map).
    init_map();
    load_map_preset(MAP_PRESET_BACKDROP1);
    wait_for_vsync();

    textboxDone = 0;
    prevSpaceDown = false;

    while (1) {
        update_keyboard();

        // Toggle states with typed characters ('1' = battle, '2' = map).
        {
            char ch = 0;
            while (keyboard_pop_char(&ch)) {
                if (ch == '1') {
                    gameState = GAME_STATE_BATTLE;
                } else if (ch == '2') {
                    gameState = GAME_STATE_MAP;
                }
            }
        }

        // Re-init on state change.
        if (gameState != prevGameState) {
            if (gameState == GAME_STATE_MAP) {
                init_map();
                load_map_preset(MAP_PRESET_ROUTE);
                mcMovingInit(80, 112, MC_FACING_S);
            } else {
                init_map();
                load_map_preset(MAP_PRESET_BACKDROP1);
                battleUiState = BATTLE_UI_MENU;
                cursorIndex = 0;
            }
            prevGameState = gameState;
        }

        const bool wDown = is_key_w_pressed();
        const bool aDown = is_key_a_pressed();
        const bool sDown = is_key_s_pressed();
        const bool dDown = is_key_d_pressed();
        const bool wPressed = wDown && !prevW;
        const bool aPressed = aDown && !prevA;
        const bool sPressed = sDown && !prevS;
        const bool dPressed = dDown && !prevD;
        prevW = wDown; prevA = aDown; prevS = sDown; prevD = dDown;

        switch (gameState) {
        case GAME_STATE_BATTLE: {

            {
                const ArrowContext arrowCtx = getArrowContext(gameState, battleUiState);
                const int cursorCount = arrowCursorCount(arrowCtx);

                if (cursorCount > 0) {
                    if (cursorIndex < 0 || cursorIndex >= cursorCount) {
                        cursorIndex = 0;
                    }
                    if (arrowCtx == ARROW_CTX_BATTLE_ATTACK) {
                        // 2 rows x 2 columns
                        const int row = cursorIndex / 2;
                        const int col = cursorIndex % 2;
                        int newRow = row;
                        int newCol = col;

                        if (aPressed) newCol = (col + 1) % 2;
                        if (dPressed) newCol = (col + 1) % 2;
                        if (wPressed) newRow = (row + 1) % 2;
                        if (sPressed) newRow = (row + 1) % 2;

                        cursorIndex = (newRow * 2) + newCol;
                    } else if (arrowCtx == ARROW_CTX_BATTLE_MENU) {

                        if (wPressed) cursorIndex = navBattleMenu9(cursorIndex, DIR_UP);
                        if (aPressed) cursorIndex = navBattleMenu9(cursorIndex, DIR_LEFT);
                        if (sPressed) cursorIndex = navBattleMenu9(cursorIndex, DIR_DOWN);
                        if (dPressed) cursorIndex = navBattleMenu9(cursorIndex, DIR_RIGHT);
                    } else {
                        cursorIndex = 0;
                    }
                }
            }

            // Animations (battle only).
            arrowAnimTimer++;
            if (arrowAnimTimer >= arrowAnimSpeedFrames) {
                arrowAnimTimer = 0;
                arrowAnimFrame = (arrowAnimFrame + 1) % ARROWGIF_FRAME_COUNT;
            }

            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            shakeTimer++;
            if (shakeTimer >= shakeSpeedFrames) {
                shakeTimer = 0;
                shakeFrame = (shakeFrame + 1) % SHAKE_SPRITE_FRAME_COUNT;
            }
            bobTimer++;
            if (bobTimer >= bobSpeedFrames) {
                bobTimer = 0;
                bobFrame = (bobFrame + 1) % BOB_SPRITE_FRAME_COUNT;
            }

            // Battle base layer (always drawn in battle state).
            draw_map();
            draw_sprite_any_bob(charizardBackSprite.pixels,
                                charizardBackSprite.width, charizardBackSprite.height,
                                charizardBackSprite.x, charizardBackSprite.y,
                                TRANSPARENT_COLOUR,
                                bobFrame);
            draw_sprite_any_shake(charmanderFrontSprite.pixels,
                                  charmanderFrontSprite.width, charmanderFrontSprite.height,
                                  charmanderFrontSprite.x, charmanderFrontSprite.y,
                                  TRANSPARENT_COLOUR,
                                  shakeFrame);
            draw_sprite_any_bob(myHpEmpty,
                                MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                TRANSPARENT_COLOUR,
                                bobFrame);
            draw_sprite_any(oppHpEmpty, OPP_HP_EMPTY_WIDTH, OPP_HP_EMPTY_HEIGHT, OPP_HP_EMPTY_X, OPP_HP_EMPTY_Y, TRANSPARENT_COLOUR);

            // Battle UI States
            if (battleUiState == BATTLE_UI_MENU) {
                // Menu background
                draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);

                // Fight / Bag / Run icons + party display (battle menu only)
                if (cursorIndex == 0) {
                    draw_sprite_any_shade_pulse(battleIconFight, BATTLE_ICON_FIGHT_WIDTH, BATTLE_ICON_FIGHT_HEIGHT,
                                                BATTLE_ICON_FIGHT_X, BATTLE_ICON_FIGHT_Y, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(battleIconFight, BATTLE_ICON_FIGHT_WIDTH, BATTLE_ICON_FIGHT_HEIGHT,
                                    BATTLE_ICON_FIGHT_X, BATTLE_ICON_FIGHT_Y, TRANSPARENT_COLOUR);
                }
                if (cursorIndex == 1) {
                    draw_sprite_any_shade_pulse(battleIconBag, BATTLE_ICON_SMALL_WIDTH, BATTLE_ICON_SMALL_HEIGHT,
                                                BATTLE_ICON_BAG_X, BATTLE_ICON_BAG_Y, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(battleIconBag, BATTLE_ICON_SMALL_WIDTH, BATTLE_ICON_SMALL_HEIGHT,
                                    BATTLE_ICON_BAG_X, BATTLE_ICON_BAG_Y, TRANSPARENT_COLOUR);
                }
                if (cursorIndex == 2) {
                    draw_sprite_any_shade_pulse(battleIconRun, BATTLE_ICON_SMALL_WIDTH, BATTLE_ICON_SMALL_HEIGHT,
                                                BATTLE_ICON_RUN_X, BATTLE_ICON_RUN_Y, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(battleIconRun, BATTLE_ICON_SMALL_WIDTH, BATTLE_ICON_SMALL_HEIGHT,
                                    BATTLE_ICON_RUN_X, BATTLE_ICON_RUN_Y, TRANSPARENT_COLOUR);
                }

                draw_sprite_any(battlePartySprite, BATTLE_PARTY_WIDTH, BATTLE_PARTY_HEIGHT, BATTLE_PARTY_X, BATTLE_PARTY_Y, TRANSPARENT_COLOUR);
                draw_sprite_any(charizardBox, CHARIZARD_BOX_WIDTH, CHARIZARD_BOX_HEIGHT, PARTY_1_X, PARTY_1_Y, TRANSPARENT_COLOUR);
                draw_sprite_any(charmanderBox, CHARIZARD_BOX_WIDTH, CHARIZARD_BOX_HEIGHT, PARTY_2_X, PARTY_2_Y, TRANSPARENT_COLOUR);
                draw_sprite_any(charmeleonBox, CHARIZARD_BOX_WIDTH, CHARIZARD_BOX_HEIGHT, PARTY_3_X, PARTY_3_Y, TRANSPARENT_COLOUR);
                draw_sprite_any(charizardBox, CHARIZARD_BOX_WIDTH, CHARIZARD_BOX_HEIGHT, PARTY_4_X, PARTY_4_Y, TRANSPARENT_COLOUR);
                draw_sprite_any(charmanderBox, CHARIZARD_BOX_WIDTH, CHARIZARD_BOX_HEIGHT, PARTY_5_X, PARTY_5_Y, TRANSPARENT_COLOUR);
                draw_sprite_any(charmeleonBox, CHARIZARD_BOX_WIDTH, CHARIZARD_BOX_HEIGHT, PARTY_6_X, PARTY_6_Y, TRANSPARENT_COLOUR);
            }

            {
                int ax = 0, ay = 0;
                ArrowContext arrowCtx = getArrowContext(gameState, battleUiState);
                if (arrowCtx != ARROW_CTX_NONE) {
                    arrowGetPos(arrowCtx, cursorIndex, &ax, &ay);
                    draw_sprite_any(arrowGifFrames[arrowAnimFrame],
                                    ARROWGIF_WIDTH, ARROWGIF_HEIGHT,
                                    ax, ay,
                                    TRANSPARENT_COLOUR);
                }
            }
            break;
        }
        case GAME_STATE_MAP:
        default:

            draw_map();
            mcMovingTick(wDown, sDown, aDown, dDown, is_key_shift_pressed());
            break;
        }

        wait_for_vsync();
    }

    return 0;
}
