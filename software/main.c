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
#include "../hardware/audio.h"
#include "../software/se/opening_audio.h"
#include "../software/se/introduction_audio.h"
#include "../software/se/battle_audio.h"
#include "../software/se/map_audio.h"
#include "../software/se/plink_audio.h"
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
#include "graphics/sprites/boxSprites/pokemonBoxSpriteInit.h"
#include "graphics/sprites/battleIcons/attackTypes/attackTypeSprites.h"
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

#define STATUS_EFFECT_X
#define STATUS_EFFECT_Y

#define CAUGHT_ICON_X
#define CAUGHT_ICON_Y

//location for drawing names, hp, level

#define myName_X
#define myName_Y

#define oppName_X
#define oppName_Y
 
 //y1 is 20 below 
 //x1 is 62 right 
 //y2 24 below
 //x2 is 48+ 62
#define HP_WIDTH 48
#define HP_HEIGHT 4

#define myHP_X MY_HP_EMPTY_X + 62
#define myHP_Y MY_HP_EMPTY_Y + 20 

#define myHP_SHOWN_X MY_HP_EMPTY_X + 62
#define myHP_SHOWN_Y MY_HP_EMPTY_Y + 20 

#define myLVL_X MY_HP_EMPTY_X + 98
#define myLVL_Y MY_HP_EMPTY_Y + 7

#define oppLVL_X OPP_HP_EMPTY_X + 85
#define oppLVL_Y OPP_HP_EMPTY_Y + 7

#define CAUGHT_X OPP_HP_EMPTY_X + 5
#define CAUGHT_Y OPP_HP_EMPTY_Y + 18

#define STATUS_X CAUGHT_X + 9
#define STATUS_Y CAUGHT_Y

#define MYSTATUS_X myHP_X - 10
#define MYSTATUS_Y myHP_SHOWN_Y - 2

#define MYNAME_X MY_HP_EMPTY_X + 15
#define MYNAME_Y myLVL_Y 

#define OPPNAME_X OPP_HP_EMPTY_X + 5
#define OPPNAME_Y oppLVL_Y

#define EXP_X MY_HP_EMPTY_X + 30
#define EXP_Y MY_HP_EMPTY_Y + 38

#define EXP_WIDTH 88
#define EXP_HEIGHT 2

#define TOTAL_HPNUM3_X MY_HP_EMPTY_X + 65
#define TOTAL_HPNUM2_X MY_HP_EMPTY_X + 71
#define TOTAL_HPNUM1_X MY_HP_EMPTY_X + 77

#define REMAINING_HP_X MY_HP_EMPTY_X + 97

#define HPNUM_Y MY_HP_EMPTY_Y + 27


//location for attacks and pp

#define MOVE_

// Game States
typedef enum {
    GAME_STATE_MAP = 0,
    GAME_STATE_BATTLE = 1,
    GAME_STATE_BATTLE_TRANSITION = 2,
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


// Navigation map for the 9 "battle menu" cursor targets (0..8).
// Index meanings:
// 0=Fight, 1=Bag, 2=Run, 3..8=Party slots 1..6 (left->right, top row then bottom row).
typedef enum { DIR_UP = 0, DIR_LEFT = 1, DIR_DOWN = 2, DIR_RIGHT = 3 } NavDir;
static int navBattleMenu9(int index, NavDir dir) {
    static const signed char nav[9][4] = {
        /*0 Fight*/ {0, 0, 0, 1},
        /*1 Bag */ {1, 0, 2, 3},
        /*2 Run */ {1, 0, 2, 6},
        /*3 P1  */ {3, 1, 6, 4},
        /*4 P2  */ {4, 3, 7, 5},
        /*5 P3  */ {5, 4, 8, 5},
        /*6 P4  */ {3, 2, 6, 7},
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

static int navBattleAttack4(int index, NavDir dir) {
    static const signed char nav[4][4] = {
        /* up left down right */
        /*0 bug     */ {0, 0, 2, 1},
        /*1 dragon  */ {1, 0, 3, 1},
        /*2 dark    */ {0, 2, 2, 3},
        /*3 electric*/ {1, 2, 3, 3},
    };
    if (index < 0) index = 0;
    if (index > 3) index = 3;
    int d = (int)dir;
    if (d < 0) d = 0;
    if (d > 3) d = 3;
    const int next = (int)nav[index][d];
    if (next < 0 || next > 3) return index;
    return next;
}

int main(void)
{

    //transition into battle
    int transitionFrame = 0;
    int transitionTimer = 0;
    const int transitionSpeedFrames = 2; 


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
    int bobPartyFrame = 0;
    int bobPartyTimer = 0;
    const int shakeSpeedFrames = 3;

    GameState gameState = GAME_STATE_BATTLE;
    BattleUiState battleUiState = BATTLE_UI_MENU;
    GameState prevGameState = gameState;
    int cursorIndex = 0;
    int arrowAnimFrame = 0;
    int arrowAnimTimer = 0;
    const int arrowAnimSpeedFrames = 8;
    bool prevW = false, prevA = false, prevS = false, prevD = false;
    bool prevEsc = false;

    StaticSprite charizardBackSprite;
    initPokemonBackBattleSpriteDefault(&charizardBackSprite, POKEMON_ID_CHARIZARD);
    StaticSprite charmanderFrontSprite;
    initPokemonFrontBattleSpriteDefault(&charmanderFrontSprite, POKEMON_ID_CHARMANDER);

    // Temporary team 
    const int playerTeamIds[6] = {
        POKEMON_ID_CHARIZARD,
        POKEMON_ID_CHARMANDER,
        POKEMON_ID_CHARMELEON,
        POKEMON_ID_CHARIZARD,
        POKEMON_ID_CHARMANDER,
        POKEMON_ID_CHARMELEON,
    };
    StaticSprite partyBoxSprites[6];
    initPokemonBoxSprite(&partyBoxSprites[0], playerTeamIds[0], PARTY_1_X, PARTY_1_Y);
    initPokemonBoxSprite(&partyBoxSprites[1], playerTeamIds[1], PARTY_2_X, PARTY_2_Y);
    initPokemonBoxSprite(&partyBoxSprites[2], playerTeamIds[2], PARTY_3_X, PARTY_3_Y);
    initPokemonBoxSprite(&partyBoxSprites[3], playerTeamIds[3], PARTY_4_X, PARTY_4_Y);
    initPokemonBoxSprite(&partyBoxSprites[4], playerTeamIds[4], PARTY_5_X, PARTY_5_Y);
    initPokemonBoxSprite(&partyBoxSprites[5], playerTeamIds[5], PARTY_6_X, PARTY_6_Y);

    // Init VGA and tiles
    init_graphics();
    audio_init_interrupts();
    init_predefined_graphics();
    clear_screen();

    initTitleScreen();
    bool title_done = false;
    play_bgm(opening_audio, opening_audio_length);

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

    stop_bgm();

    textboxMsgIndex++;
    textboxMsg = TEXT_MESSAGES[textboxMsgIndex];

    init_map();
    load_map_preset(MAP_PRESET_BLACK);
    draw_map();

    // Ensure both buffers have the same background before typing.
    wait_for_vsync();
    draw_map();
    wait_for_vsync();
    play_bgm(introduction_audio, introduction_audio_len);
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
    stop_bgm();

    // Initial state setup (press '1' for battle, '2' for map).
    init_map();
    load_map_preset(MAP_PRESET_BACKDROP1);
    if (gameState == GAME_STATE_BATTLE) {
        play_bgm(battle_audio, battle_audio_len);
    } else {
        play_bgm(map_audio, map_audio_len);
    }
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
                play_bgm(map_audio, map_audio_len);
                init_map();
                load_map_preset(MAP_PRESET_ROUTE);
                mcMovingInit(80, 112, MC_FACING_S);
            } else {
                play_bgm(battle_audio, battle_audio_len);
                init_map();
                load_map_preset(MAP_PRESET_BACKDROP1);
                battleUiState = BATTLE_UI_MENU;
                cursorIndex = 0;
                gameState = GAME_STATE_BATTLE_TRANSITION;
                transitionFrame = 0;
                transitionTimer = 0;
            }
            prevGameState = gameState;
        }
        const bool spaceDown = is_key_space_pressed();
        const bool spacePressed = spaceDown && !prevSpaceDown;
        prevSpaceDown = spaceDown;

        const bool escDown = is_key_escape_pressed();
        const bool escPressed = escDown && !prevEsc;
        prevEsc = escDown;

        if (escPressed && battleUiState == BATTLE_UI_ATTACK_MENU) {
            battleUiState = BATTLE_UI_MENU;
            cursorIndex = 0;
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
            bool movedBattleCursor = false;

            {
                const ArrowContext arrowCtx = getArrowContext(gameState, battleUiState);
                const int cursorCount = arrowCursorCount(arrowCtx);

                if (cursorCount > 0) {
                    if (cursorIndex < 0 || cursorIndex >= cursorCount) {
                        cursorIndex = 0;
                    }
                    if (arrowCtx == ARROW_CTX_BATTLE_ATTACK) {
                        const int oldIndex = cursorIndex;
                        if (wPressed) cursorIndex = navBattleAttack4(cursorIndex, DIR_UP);
                        if (aPressed) cursorIndex = navBattleAttack4(cursorIndex, DIR_LEFT);
                        if (sPressed) cursorIndex = navBattleAttack4(cursorIndex, DIR_DOWN);
                        if (dPressed) cursorIndex = navBattleAttack4(cursorIndex, DIR_RIGHT);
                        movedBattleCursor = (cursorIndex != oldIndex);
                    } else if (arrowCtx == ARROW_CTX_BATTLE_MENU) {
                        const int oldIndex = cursorIndex;
                        if (wPressed) cursorIndex = navBattleMenu9(cursorIndex, DIR_UP);
                        if (aPressed) cursorIndex = navBattleMenu9(cursorIndex, DIR_LEFT);
                        if (sPressed) cursorIndex = navBattleMenu9(cursorIndex, DIR_DOWN);
                        if (dPressed) cursorIndex = navBattleMenu9(cursorIndex, DIR_RIGHT);
                        movedBattleCursor = (cursorIndex != oldIndex);
                    } else {
                        cursorIndex = 0;
                    }
                }
            }

            if (spacePressed && battleUiState == BATTLE_UI_MENU && cursorIndex == 0) {
                battleUiState = BATTLE_UI_ATTACK_MENU;
                cursorIndex = 0;
            }

            if (movedBattleCursor) {
                play_sfx(plink_audio, plink_audio_len);
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

            //for party members to show that they are being selected
            bobPartyTimer++;
            if (bobPartyTimer >= bobPartyFrame) {
                bobPartyTimer = 0;
                bobPartyFrame = (bobPartyFrame + 1) % BOB_SPRITE_FRAME_COUNT;
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
            //todo: make the top and bottom lines of hp a darker shade to look better
            
            //opponent hp bar
            draw_rect(OPP_HP_EMPTY_X + 50, OPP_HP_EMPTY_Y + 20, HP_WIDTH, HP_HEIGHT, GREEN);
            draw_string_f(oppLVL_X, oppLVL_Y, "67", BLACK, 1);
            draw_sprite_any(burned, BURNED_WIDTH, BURNED_HEIGHT, STATUS_X, STATUS_Y, TRANSPARENT_COLOUR);
            draw_sprite_any(caught, CAUGHT_WIDTH, CAUGHT_HEIGHT, CAUGHT_X, CAUGHT_Y, TRANSPARENT_COLOUR);
            draw_string_f(OPPNAME_X, OPPNAME_Y, "CHARMANDER", BLACK, 1);

            //got the bobbing dy pattern array to move hp bar, name, level, etc from myHP bar 
            static const signed char dy_pattern[BOB_SPRITE_FRAME_COUNT] = {
            0, -1, -1, 0, 0, 1, 1, 0,
            0, -1, -1, 0, 0, 1, 1, 0 };
            int offsetY = dy_pattern[bobFrame];
            //my hp bar
            draw_rect(myHP_X, myHP_Y + offsetY, HP_WIDTH, HP_HEIGHT, GREEN);
            draw_rect(EXP_X, EXP_Y + offsetY, EXP_WIDTH, EXP_HEIGHT, BLUE);
            draw_string_f(myLVL_X, myLVL_Y + offsetY, "1", BLACK, 1);
            draw_string_f(MYNAME_X, MYNAME_Y + offsetY, "CHARIZARD", BLACK, 1);
            draw_string_f(TOTAL_HPNUM3_X, HPNUM_Y + offsetY, "123", BLACK, 1);
            draw_string_f(REMAINING_HP_X, HPNUM_Y + offsetY, "456", BLACK, 1);
            draw_sprite_any(poison, POISON_WIDTH, POISON_HEIGHT, MYSTATUS_X, MYSTATUS_Y + offsetY, TRANSPARENT_COLOUR);
           

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

                const unsigned short* partyBg;
                if (cursorIndex <= 2) {
                    partyBg = battlePartySlotSprites[0];
                }
                else {
                    partyBg = battlePartySlotSprites[cursorIndex - 2];
                }

                int selectedPartyIndex = -1;

                if (cursorIndex >= 3 && cursorIndex <= 8) {
                    selectedPartyIndex = cursorIndex - 3; 
                
                }
                draw_sprite_any(partyBg, BATTLE_PARTY_WIDTH, BATTLE_PARTY_HEIGHT, BATTLE_PARTY_X, BATTLE_PARTY_Y, TRANSPARENT_COLOUR);
                //box sprites
                

                for (int i = 0; i < 6; i++) {
                    if (i == selectedPartyIndex) {
                        draw_sprite_any_bob_party(
                            partyBoxSprites[i].pixels,
                            partyBoxSprites[i].width,
                            partyBoxSprites[i].height,
                            partyBoxSprites[i].x,
                            partyBoxSprites[i].y,
                            TRANSPARENT_COLOUR,
                            bobPartyFrame
                        );
                    } else {
                        drawStaticSprite(&partyBoxSprites[i]);
                    }
                }
            }

            if (battleUiState == BATTLE_UI_ATTACK_MENU) {
                draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);
            
                const unsigned short* moveSprites[4] = { bugTypeSprite, dragonTypeSprite, darkTypeSprite, electricTypeSprite };
                //location of moves
                const int mxs[4] = { 0, 100, 150, 200 };
                const int mys[4] = { 0, 100, 150, 200 };
            
                if (cursorIndex == 0) {
                    draw_sprite_any_shade_pulse(moveSprites[0], BUG_TYPE_WIDTH, BUG_TYPE_HEIGHT, 0, battleBackdropY + 10, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(moveSprites[0], BUG_TYPE_WIDTH, BUG_TYPE_HEIGHT, 0, battleBackdropY + 10, TRANSPARENT_COLOUR);
                }
                if (cursorIndex == 1) {
                    draw_sprite_any_shade_pulse(moveSprites[1], DRAGON_TYPE_WIDTH, DRAGON_TYPE_HEIGHT, 170, battleBackdropY + 10, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(moveSprites[1], DRAGON_TYPE_WIDTH, DRAGON_TYPE_HEIGHT, 170, battleBackdropY + 10, TRANSPARENT_COLOUR);
                }
                if (cursorIndex == 2) {
                    draw_sprite_any_shade_pulse(moveSprites[2], DARK_TYPE_WIDTH, DARK_TYPE_HEIGHT, 0, battleBackdropY + 50, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any( moveSprites[2], DARK_TYPE_WIDTH, DARK_TYPE_HEIGHT, 0, battleBackdropY + 50, TRANSPARENT_COLOUR);
                }
                if (cursorIndex == 3) {
                    draw_sprite_any_shade_pulse(moveSprites[3], ELECTRIC_TYPE_WIDTH, ELECTRIC_TYPE_HEIGHT, 170, battleBackdropY + 50, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any( moveSprites[3], ELECTRIC_TYPE_WIDTH, ELECTRIC_TYPE_HEIGHT, 170, battleBackdropY + 50, TRANSPARENT_COLOUR);
                }
            }
    
            break;
        }

        case GAME_STATE_BATTLE_TRANSITION: {
            // Draw the battle scene underneath first
            draw_map();
        
            const int GRID_SPACING = 40;
            const int maxRadius = (GRID_SPACING) - transitionFrame;
        
            for (int cy = 0; cy <= SCREEN_HEIGHT + GRID_SPACING; cy += GRID_SPACING) {
                for (int cx = 0; cx <= SCREEN_WIDTH + GRID_SPACING; cx += GRID_SPACING) {
                    for (int dy = -maxRadius; dy <= maxRadius; dy++) {
                        int rowWidth = maxRadius - (dy < 0 ? -dy : dy);
                        if (rowWidth < 0) rowWidth = 0;
                        int px = cx - rowWidth;
                        int py = cy + dy;
                        if (py < 0 || py >= SCREEN_HEIGHT) continue;
                        if (px < 0) px = 0;
                        int endx = cx + rowWidth;
                        if (endx > SCREEN_WIDTH) endx = SCREEN_WIDTH;
                        if (endx > px)
                            draw_rect(px, py, endx - px, 1, BLACK);
                    }
                }
            }
        
            transitionTimer++;
            if (transitionTimer >= transitionSpeedFrames) {
                transitionTimer = 0;
                transitionFrame++;
            }
        
            if (transitionFrame >= GRID_SPACING) {
                gameState = GAME_STATE_BATTLE;
                prevGameState = GAME_STATE_BATTLE;
                transitionFrame = 0;
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
