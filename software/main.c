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
#include "gameplayLogic/battling/battleLoop.h"
#include "gameplayLogic/entities/pokemonDataBase.h"
#include "gameplayLogic/storage/pc.h"
#include "graphics/sprites/pokeballThrow/pokeballThrow_frames.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

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
#define PARTY_4_Y 240 - 91 + 24

#define PARTY_5_X 213 + 1
#define PARTY_5_Y 240 - 91 + 24

#define PARTY_6_X 253 + 3
#define PARTY_6_Y 240 - 91 + 24

#define battleBackdropY 240 - 91


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

#define MYSTATUS_X myHP_X - 37
#define MYSTATUS_Y myHP_SHOWN_Y - 2

#define MYNAME_X MY_HP_EMPTY_X + 15
#define MYNAME_Y myLVL_Y 

#define OPPNAME_X OPP_HP_EMPTY_X + 5
#define OPPNAME_Y oppLVL_Y

#define EXP_X MY_HP_EMPTY_X + 30
#define EXP_Y MY_HP_EMPTY_Y + 38

#define EXP_WIDTH 88
#define EXP_HEIGHT 2

#define TOTAL_HPNUM_X MY_HP_EMPTY_X + 65

#define REMAINING_HP_X MY_HP_EMPTY_X + 97

#define HPNUM_Y MY_HP_EMPTY_Y + 27

#define CURRENT_PP_X_FROM_ATTACK_SPRITE 73 //73 right 
#define CURRENT_PP_Y_FROM_ATTACK_SPRITE 24 //24 down

#define TOTAL_PP_X_FROM_CURRENT_PP 22 //22 right
#define TOTAL_PP_Y_FROM_CURRENT_PP 0 //same y

//location for attacks and pp

#define MOVE_

// Game States
typedef enum {
    GAME_STATE_MAP = 0,
    GAME_STATE_BATTLE = 1,
    GAME_STATE_BATTLE_TRANSITION = 2,
    GAME_STATE_BATTLE_INTRO_TEXT = 3,
    GAME_STATE_BATTLE_ACTION_TEXT = 4,
    GAME_STATE_POKEBALL_THROW = 5,
} GameState;

typedef enum {
    ARROW_CTX_NONE = 0,
    ARROW_CTX_BATTLE_MENU = 1,
    ARROW_CTX_BATTLE_ATTACK = 2,
    ARROW_CTX_BATTLE_BAG = 3,
} ArrowContext;

typedef enum {
    BATTLE_UI_MENU = 0,
    BATTLE_UI_ATTACK_MENU = 1,
    BATTLE_UI_BAG_MENU = 2,
} BattleUiState;

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
        /*0 top left*/ {0, 0, 2, 1},
        /*1 top right*/ {1, 0, 3, 1},
        /*2 bottom left*/ {0, 2, 2, 3},
        /*3 bottom right*/ {1, 2, 3, 3},
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

//returns the address of the global pokemon structs for the pokemon
static const PokemonData *speciesFromPokemonSpriteId(int pokemonId) {
    switch (pokemonId) {
        case POKEMON_ID_CHARMANDER: return &CHARMANDER;
        case POKEMON_ID_CHARMELEON: return &CHARMELEON;
        case POKEMON_ID_CHARIZARD:  return &CHARIZARD;
        default: return NULL;
    }
}

static void syncBattleSprites(const BattleState *state, StaticSprite *playerBackSprite, StaticSprite *enemyFrontSprite) {
    if (state == NULL || playerBackSprite == NULL || enemyFrontSprite == NULL) return;
    if (state->playerParty != NULL) {
        const int i = state->playerParty->activeIndex;
        if (i >= 0 && i < state->playerParty->count) {
            if (state->playerParty->slots[i] != NULL) {
                setPokemonBackBattleSpriteId(playerBackSprite, state->playerParty->slots[i]->id.backFrame_ID);
            }
        }
    }
    if (state->enemyParty != NULL) {
        const int i = state->enemyParty->activeIndex;
        if (i >= 0 && i < state->enemyParty->count) {
            if (state->enemyParty->slots[i] != NULL) {
                setPokemonFrontBattleSpriteId(enemyFrontSprite, state->enemyParty->slots[i]->id.frontFrame_ID);
            }
        }
    }
}

static void battleUiSetSingleMessage(BattleState *state, const char *msg) {
    if (state == NULL || msg == NULL) return;
    state->messageCount = 0;
    state->messageReadIndex = 0;
    snprintf(state->messages[0], sizeof(state->messages[0]), "%s", msg);
    state->messageCount = 1;
}

static int stringPixelWidth(FontId font, const char *s, int maxChars) {
    if (s == NULL) return 0;
    if (maxChars < 0) maxChars = 0;

    const int advance = (font == FONT_5X9) ? 6 : 8; //5x9 is 5 pixels + 1 gap
    const int glyphW = (font == FONT_5X9) ? 5 : 8; //5x9 is 5 pixels

    int len = 0;
    while (s[len] && (maxChars == 0 || len < maxChars)) len++;
    if (len == 0) return 0;
    return (len - 1) * advance + glyphW;
}

static void drawCenteredStringInBox(int boxX, int boxY, int boxW, int boxH,
                                    const char *s, short colour, FontId font) {
    if (s == NULL) return;
    const int advance = (font == FONT_5X9) ? 6 : 8;

    // Clip long strings to fit the box 
    int maxChars = 0;
    if (boxW > 0) {
        const int glyphW = (font == FONT_5X9) ? 5 : 8;
        maxChars = (boxW - glyphW) / advance + 1;
        if (maxChars < 0) maxChars = 0;
    }

    const int textW = stringPixelWidth(font, s, maxChars);
    const int fontH = (font == FONT_5X9) ? 9 : 8;
    const int x = boxX + (boxW - textW) / 2;
    const int y = boxY + (boxH - fontH) / 2;

    // Draw at most maxChars by copying into a small buffer.
    if (maxChars > 0) {
        char buf[32];
        int i = 0;
        for (; i < (int)sizeof(buf) - 1 && i < maxChars && s[i]; i++) buf[i] = s[i];
        buf[i] = '\0';
        draw_string_f(x, y, buf, colour, font);
    }
}

typedef struct {
    const unsigned short *pixels;
    int width;
    int height;
} AttackTypeSpriteRef;

static AttackTypeSpriteRef attackTypeSpriteFor(PokemonType type) {
    switch (type) {
        case TYPE_BUG: return (AttackTypeSpriteRef){bugTypeSprite, BUG_TYPE_WIDTH, BUG_TYPE_HEIGHT };
        case TYPE_DARK: return (AttackTypeSpriteRef){darkTypeSprite, DARK_TYPE_WIDTH, DARK_TYPE_HEIGHT };
        case TYPE_DRAGON: return (AttackTypeSpriteRef){dragonTypeSprite, DRAGON_TYPE_WIDTH, DRAGON_TYPE_HEIGHT };
        case TYPE_ELECTRIC: return (AttackTypeSpriteRef){electricTypeSprite, ELECTRIC_TYPE_WIDTH, ELECTRIC_TYPE_HEIGHT };
        case TYPE_FAIRY: return (AttackTypeSpriteRef){fairyTypeSprite, FAIRY_TYPE_WIDTH, FAIRY_TYPE_HEIGHT };
        case TYPE_FIGHTING: return (AttackTypeSpriteRef){fightingTypeSprite, FIGHTING_TYPE_WIDTH, FIGHTING_TYPE_HEIGHT };
        case TYPE_FIRE: return (AttackTypeSpriteRef){fireTypeSprite, FIRE_TYPE_WIDTH, FIRE_TYPE_HEIGHT };
        case TYPE_FLYING: return (AttackTypeSpriteRef){flyingTypeSprite, FLYING_TYPE_WIDTH, FLYING_TYPE_HEIGHT };
        case TYPE_GHOST: return (AttackTypeSpriteRef){ghostTypeSprite, GHOST_TYPE_WIDTH, GHOST_TYPE_HEIGHT };
        case TYPE_GRASS: return (AttackTypeSpriteRef){grassTypeSprite, GRASS_TYPE_WIDTH, GRASS_TYPE_HEIGHT };
        case TYPE_GROUND: return (AttackTypeSpriteRef){groundTypeSprite, GROUND_TYPE_WIDTH, GROUND_TYPE_HEIGHT };
        case TYPE_ICE: return (AttackTypeSpriteRef){iceTypeSprite, ICE_TYPE_WIDTH, ICE_TYPE_HEIGHT };
        case TYPE_NORMAL: return (AttackTypeSpriteRef){normalTypeSprite, NORMAL_TYPE_WIDTH, NORMAL_TYPE_HEIGHT };
        case TYPE_POISON: return (AttackTypeSpriteRef){poisonTypeSprite, POISON_TYPE_WIDTH, POISON_TYPE_HEIGHT };
        case TYPE_PSYCHIC: return (AttackTypeSpriteRef){psyTypeSprite, PSY_TYPE_WIDTH, PSY_TYPE_HEIGHT };
        case TYPE_ROCK: return (AttackTypeSpriteRef){rockTypeSprite, ROCK_TYPE_WIDTH, ROCK_TYPE_HEIGHT };
        case TYPE_STEEL: return (AttackTypeSpriteRef){steelTypeSprite, STEEL_TYPE_WIDTH, STEEL_TYPE_HEIGHT };
        case TYPE_WATER: return (AttackTypeSpriteRef){waterTypeSprite, WATER_TYPE_WIDTH, WATER_TYPE_HEIGHT };
        default: return (AttackTypeSpriteRef){normalTypeSprite, NORMAL_TYPE_WIDTH, NORMAL_TYPE_HEIGHT };
    }
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

    // Pokeball throw animation state.
    bool pokeballThrown = false;
    bool pokeballThrowInit = false;
    bool pokeballLanding = false;
    int pokeballThrowT = 0;
    int pokeballThrownFrame = 0;
    int pokeballThrownTimer = 0;
    int pokeballLandingTimer = 0;
    float pokeballX0 = 0.0f, pokeballY0 = 0.0f;
    float pokeballVx = 0.0f, pokeballVy = 0.0f;
    const float pokeballG = 0.35f;
    const int pokeballTPeak = 18;
    float pokeballLandX = 0.0f, pokeballLandY = 0.0f;
    int pokeballLandingFrame = 8;

    unsigned int frame_count = 0;
    int current_phase = 0;    
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

    char battleIntroText[96];
    bool battleIntroTextReady = false;

    char battleThrowPokeballText[96];
    bool battleThrowPokeballTextReady = false;

    GameState currentGameState = GAME_STATE_BATTLE;
    BattleUiState battleUi = BATTLE_UI_MENU;
    GameState previousGameState = currentGameState;
    int battleCursor = 0;
    int arrowAnimFrame = 0;
    int arrowAnimTimer = 0;
    const int arrowAnimSpeedFrames = 8;
    bool prevUp = false, prevLeft = false, prevDown = false, prevRight = false;
    bool prevEsc = false;
    bool actionTextAwaitSpaceRelease = false;
    BattleUiState actionTextReturnUi = BATTLE_UI_MENU;
    int actionTextReturnCursor = 0;

    // Battle logic (stub for now).
    BattleState battleState;
    Party playerParty;
    Party enemyParty;
    PC playerPc;
    pokemonInBattle wildEnemy;

    StaticSprite playerBackSprite;
    StaticSprite enemyFrontSprite;

    // Temporary team 
    const int playerTeamSpriteIds[6] = {
        POKEMON_ID_CHARIZARD,
        POKEMON_ID_CHARMANDER,
        POKEMON_ID_CHARMELEON,
        POKEMON_ID_CHARIZARD,
        POKEMON_ID_CHARMANDER,
        POKEMON_ID_CHARMELEON,
    };
    StaticSprite partyBoxSprites[6];
    initPokemonBoxSprite(&partyBoxSprites[0], playerTeamSpriteIds[0], PARTY_1_X, PARTY_1_Y);
    initPokemonBoxSprite(&partyBoxSprites[1], playerTeamSpriteIds[1], PARTY_2_X, PARTY_2_Y);
    initPokemonBoxSprite(&partyBoxSprites[2], playerTeamSpriteIds[2], PARTY_3_X, PARTY_3_Y);
    initPokemonBoxSprite(&partyBoxSprites[3], playerTeamSpriteIds[3], PARTY_4_X, PARTY_4_Y);
    initPokemonBoxSprite(&partyBoxSprites[4], playerTeamSpriteIds[4], PARTY_5_X, PARTY_5_Y);
    initPokemonBoxSprite(&partyBoxSprites[5], playerTeamSpriteIds[5], PARTY_6_X, PARTY_6_Y);

    // init the roster and party (persistent).
    pcInit(&playerPc);
    initParty(&playerParty);
    for (int i = 0; i < 6; i++) {
        const PokemonData *species = speciesFromPokemonSpriteId(playerTeamSpriteIds[i]);
        int ownedIndex = -1;
        if (species != NULL && pcAdd(&playerPc, species, 20 + i, &ownedIndex)) {
            addPokemonToParty(&playerParty, pcGet(&playerPc, ownedIndex));
        }
    }

    initParty(&enemyParty);
    initPokemonInBattle(&wildEnemy, &CHARMANDER, 18);
    addPokemonToParty(&enemyParty, &wildEnemy);
    initBattleState(&battleState, &playerParty, &enemyParty, BATTLE_WILD);

    initPokemonBackBattleSpriteDefault(&playerBackSprite, playerParty.slots[playerParty.activeIndex]->id.backFrame_ID);
    initPokemonFrontBattleSpriteDefault(&enemyFrontSprite, enemyParty.slots[enemyParty.activeIndex]->id.frontFrame_ID);

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

    // Initial state setup 1 for battle, 2 for map
    init_map();
    load_map_preset(MAP_PRESET_BACKDROP1);
    if (currentGameState == GAME_STATE_BATTLE) {
        play_bgm(battle_audio, battle_audio_len);
    } else {
        play_bgm(map_audio, map_audio_len);
    }
    wait_for_vsync();

    textboxDone = 0;
    prevSpaceDown = false;

    while (1) {
        update_keyboard();
        {
            char ch = 0;
            while (keyboard_pop_char(&ch)) {
                if (ch == '1') {
                    currentGameState = GAME_STATE_BATTLE;
                } else if (ch == '2') {
                    currentGameState = GAME_STATE_MAP;
                }
            }
        }

        // Re-init on state change.
        if (currentGameState != previousGameState) {
            if (currentGameState == GAME_STATE_MAP) {
                play_bgm(map_audio, map_audio_len);
                init_map();
                load_map_preset(MAP_PRESET_ROUTE);
                mcMovingInit(80, 112, MC_FACING_S);
            } else {
                play_bgm(battle_audio, battle_audio_len);
                init_map();
                load_map_preset(MAP_PRESET_BACKDROP1);
                battleUi = BATTLE_UI_MENU;
                battleCursor = 0;

                // Reset battle state whenever battle starts.
                // Player party is persistent for this program run
                initParty(&enemyParty);
                initPokemonInBattle(&wildEnemy, &CHARMANDER, 18);
                addPokemonToParty(&enemyParty, &wildEnemy);
                initBattleState(&battleState, &playerParty, &enemyParty, BATTLE_WILD);

                syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);

                currentGameState = GAME_STATE_BATTLE_TRANSITION;
                transitionFrame = 0;
                transitionTimer = 0;
            }
            previousGameState = currentGameState;
        }
        const bool spaceDown = is_key_space_pressed();
        const bool spacePressed = spaceDown && !prevSpaceDown;
        prevSpaceDown = spaceDown;

        const bool escDown = is_key_escape_pressed();
        const bool escPressed = escDown && !prevEsc;
        prevEsc = escDown;

        if (escPressed && battleUi == BATTLE_UI_ATTACK_MENU) {
            battleUi = BATTLE_UI_MENU;
            battleCursor = 0;
        }

        //WASD and arrow keys
        const bool upDown = is_key_w_pressed() || is_key_up_pressed();
        const bool leftDown = is_key_a_pressed() || is_key_left_pressed();
        const bool downDown = is_key_s_pressed() || is_key_down_pressed();
        const bool rightDown = is_key_d_pressed() || is_key_right_pressed();

        const bool upPressed = upDown && !prevUp;
        const bool leftPressed = leftDown && !prevLeft;
        const bool downPressed = downDown && !prevDown;
        const bool rightPressed = rightDown && !prevRight;

        prevUp = upDown;
        prevLeft = leftDown;
        prevDown = downDown;
        prevRight = rightDown;

        switch (currentGameState) {
        case GAME_STATE_BATTLE: {
            bool didMoveBattleCursor = false;

            {
                const ArrowContext arrowCtx = getArrowContext(currentGameState, battleUi);
                const int cursorCount = arrowCursorCount(arrowCtx);

                if (cursorCount > 0) {
                    if (battleCursor < 0 || battleCursor >= cursorCount) {
                        battleCursor = 0;
                    }
                    if (arrowCtx == ARROW_CTX_BATTLE_ATTACK) {
                        const int oldIndex = battleCursor;
                        if (upPressed) battleCursor = navBattleAttack4(battleCursor, DIR_UP);
                        if (leftPressed) battleCursor = navBattleAttack4(battleCursor, DIR_LEFT);
                        if (downPressed) battleCursor = navBattleAttack4(battleCursor, DIR_DOWN);
                        if (rightPressed) battleCursor = navBattleAttack4(battleCursor, DIR_RIGHT);
                        didMoveBattleCursor = (battleCursor != oldIndex);
                    } else if (arrowCtx == ARROW_CTX_BATTLE_MENU) {
                        const int oldIndex = battleCursor;
                        if (upPressed) battleCursor = navBattleMenu9(battleCursor, DIR_UP);
                        if (leftPressed) battleCursor = navBattleMenu9(battleCursor, DIR_LEFT);
                        if (downPressed) battleCursor = navBattleMenu9(battleCursor, DIR_DOWN);
                        if (rightPressed) battleCursor = navBattleMenu9(battleCursor, DIR_RIGHT);
                        didMoveBattleCursor = (battleCursor != oldIndex);
                    } else {
                        battleCursor = 0;
                    }
                }
            }

            if (spacePressed) {
                if (battleUi == BATTLE_UI_MENU) {
                    if (battleCursor == 0) {
                        battleUi = BATTLE_UI_ATTACK_MENU;
                        battleCursor = 0;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (battleCursor == 1) {
                        battleApplyPlayerAction(&battleState, ACTION_ITEM, 0);
                        play_sfx(plink_audio, plink_audio_len);
                        syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                    } else if (battleCursor == 2) {
                        battleApplyPlayerAction(&battleState, ACTION_RUN, 0);
                        play_sfx(plink_audio, plink_audio_len);
                        syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                        if (battleState.result == BATTLE_RESULT_FLED) {
                            currentGameState = GAME_STATE_MAP;
                        }
                    } else if (battleCursor >= 3 && battleCursor <= 8) {
                        // Don't consume a turn for invalid switches
                        Party *p = battleState.playerParty;
                        const int slot = battleCursor - 3;
                        bool canSwitch = false;
                        if (p != NULL && slot >= 0 && slot < p->count) {
                            if (slot != p->activeIndex && p->slots[slot] != NULL && p->slots[slot]->alive) {
                                canSwitch = true;
                            }
                        }

                        if (!canSwitch) {
                            battleUiSetSingleMessage(&battleState, "Can't switch to that Pokemon!");
                            actionTextReturnUi = battleUi;
                            actionTextReturnCursor = battleCursor;
                            currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            actionTextAwaitSpaceRelease = true;
                            play_sfx(plink_audio, plink_audio_len);
                        } else {
                            battleApplyPlayerAction(&battleState, ACTION_SWITCH, slot);
                            play_sfx(plink_audio, plink_audio_len);
                            syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                        }
                    }
                } else if (battleUi == BATTLE_UI_ATTACK_MENU) {
                    // Don't consume a turn if the selected move has no PP.
                    pokemonInBattle *playerActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;
                    const int moveIndex = battleCursor;
                    const AttackData *move = (playerActive != NULL && moveIndex >= 0 && moveIndex < 4) ? playerActive->attacks[moveIndex] : NULL;
                    const int pp = (playerActive != NULL && moveIndex >= 0 && moveIndex < 4) ? playerActive->currentPP[moveIndex] : 0;

                    if (move == NULL) {
                        battleUiSetSingleMessage(&battleState, "No move selected!");
                        actionTextReturnUi = battleUi;
                        actionTextReturnCursor = battleCursor;
                        currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                        previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                        actionTextAwaitSpaceRelease = true;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (pp <= 0) {
                        battleUiSetSingleMessage(&battleState, "No PP left!");
                        actionTextReturnUi = battleUi;
                        actionTextReturnCursor = battleCursor;
                        currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                        previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                        actionTextAwaitSpaceRelease = true;
                        play_sfx(plink_audio, plink_audio_len);
                    } else {
                        battleApplyPlayerAction(&battleState, ACTION_ATTACK, moveIndex);
                        play_sfx(plink_audio, plink_audio_len);
                        syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                        battleUi = BATTLE_UI_MENU;
                        battleCursor = 0;
                    }
                }

                if (battleState.messageCount > 0) {
                    actionTextReturnUi = battleUi;
                    actionTextReturnCursor = battleCursor;
                    currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    actionTextAwaitSpaceRelease = true;
                } else if (battleState.result != BATTLE_RESULT_ONGOING) {
                    currentGameState = GAME_STATE_MAP;
                }
            }

            if (didMoveBattleCursor) {
                play_sfx(plink_audio, plink_audio_len);
            }

            if (escPressed && battleUi == BATTLE_UI_ATTACK_MENU) {
                play_sfx(plink_audio, plink_audio_len);
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
 
            draw_sprite_any_bob(playerBackSprite.pixels,
                                playerBackSprite.width, playerBackSprite.height,
                                playerBackSprite.x, playerBackSprite.y,
                                TRANSPARENT_COLOUR,
                                bobFrame);
            draw_sprite_any(enemyFrontSprite.pixels,
                            enemyFrontSprite.width, enemyFrontSprite.height,
                            enemyFrontSprite.x, enemyFrontSprite.y,
                            TRANSPARENT_COLOUR);
            // Draw the battle UI background before HUD elements so it doesn't cover HP text/bars.
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);
            draw_sprite_any_bob(myHpEmpty,
                                MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                TRANSPARENT_COLOUR,
                                bobFrame);
            draw_sprite_any(oppHpEmpty, OPP_HP_EMPTY_WIDTH, OPP_HP_EMPTY_HEIGHT, OPP_HP_EMPTY_X, OPP_HP_EMPTY_Y, TRANSPARENT_COLOUR);
            //todo: make the top and bottom lines of hp a darker shade to look better
            
            pokemonInBattle *enemyActive = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
            pokemonInBattle *playerActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;

            const int enemyHp = (enemyActive != NULL) ? enemyActive->scaledStatsWithLevel[0] : 0;
            const int enemyMaxHp = (enemyActive != NULL) ? enemyActive->maxHp : 1;
            const int playerHp = (playerActive != NULL) ? playerActive->scaledStatsWithLevel[0] : 0;
            const int playerMaxHp = (playerActive != NULL) ? playerActive->maxHp : 1;

            //hp bars with code that scales
            const int enemyHpBarWidth = (enemyMaxHp > 0) ? (HP_WIDTH * enemyHp) / enemyMaxHp : 0;
            const int playerHpBarWidth = (playerMaxHp > 0) ? (HP_WIDTH * playerHp) / playerMaxHp : 0;

            const int enemyHpPct = (enemyMaxHp > 0) ? (enemyHp * 100) / enemyMaxHp : 0;
            const int playerHpPct = (playerMaxHp > 0) ? (playerHp * 100) / playerMaxHp : 0;

            const short enemyHpBarColour = (enemyHpPct < 15) ? RED : ((enemyHpPct < 50) ? ORANGE : GREEN);
            const short playerHpBarColour = (playerHpPct < 15) ? RED : ((playerHpPct < 50) ? ORANGE : GREEN);

            char oppLvlBuf[8];
            char myLvlBuf[8];
            char myHpCurBuf[8];
            char myHpMaxBuf[8];
            snprintf(oppLvlBuf, sizeof(oppLvlBuf), "%d", (enemyActive != NULL) ? enemyActive->level : 0);
            snprintf(myLvlBuf, sizeof(myLvlBuf), "%d", (playerActive != NULL) ? playerActive->level : 0);
            snprintf(myHpCurBuf, sizeof(myHpCurBuf), "%d", playerHp);
            snprintf(myHpMaxBuf, sizeof(myHpMaxBuf), "%d", playerMaxHp);

            //opponent hp bar
            draw_rect(OPP_HP_EMPTY_X + 50, OPP_HP_EMPTY_Y + 20, (enemyHpBarWidth < 0) ? 0 : ((enemyHpBarWidth > HP_WIDTH) ? HP_WIDTH : enemyHpBarWidth), HP_HEIGHT, enemyHpBarColour);
            draw_string_f(oppLVL_X, oppLVL_Y, oppLvlBuf, BLACK, 1);
            draw_sprite_any(burned, BURNED_WIDTH, BURNED_HEIGHT, STATUS_X, STATUS_Y, TRANSPARENT_COLOUR);
            draw_sprite_any(caught, CAUGHT_WIDTH, CAUGHT_HEIGHT, CAUGHT_X, CAUGHT_Y, TRANSPARENT_COLOUR);
            draw_string_f(OPPNAME_X, OPPNAME_Y, (enemyActive != NULL && enemyActive->id.data != NULL) ? enemyActive->id.data->name : "???", BLACK, 1);

            //got the bobbing dy pattern array to move hp bar, name, level, etc from myHP bar 
            static const signed char dy_pattern[BOB_SPRITE_FRAME_COUNT] = {
            0, -1, -1, 0, 0, 1, 1, 0,
            0, -1, -1, 0, 0, 1, 1, 0 };
            int offsetY = dy_pattern[bobFrame];
            //my hp bar
            draw_rect(myHP_X, myHP_Y + offsetY, (playerHpBarWidth < 0) ? 0 : ((playerHpBarWidth > HP_WIDTH) ? HP_WIDTH : playerHpBarWidth), HP_HEIGHT, playerHpBarColour);
            draw_rect(EXP_X, EXP_Y + offsetY, EXP_WIDTH, EXP_HEIGHT, TURQ);
            draw_string_f(myLVL_X, myLVL_Y + offsetY, myLvlBuf, BLACK, 1);
            draw_string_f(MYNAME_X, MYNAME_Y + offsetY, (playerActive != NULL && playerActive->id.data != NULL) ? playerActive->id.data->name : "???", BLACK, 1);

            draw_string_f(TOTAL_HPNUM_X, HPNUM_Y + offsetY, myHpCurBuf, BLACK, 1);
            draw_string_f(REMAINING_HP_X, HPNUM_Y + offsetY, myHpMaxBuf, BLACK, 1);
            draw_sprite_any(poison, POISON_WIDTH, POISON_HEIGHT, MYSTATUS_X, MYSTATUS_Y + offsetY, TRANSPARENT_COLOUR);
           

            // Battle UI States
            if (battleUi == BATTLE_UI_MENU) {
                if (battleCursor == 0) {
                    draw_sprite_any_shade_pulse(battleIconFight, BATTLE_ICON_FIGHT_WIDTH, BATTLE_ICON_FIGHT_HEIGHT,
                                                BATTLE_ICON_FIGHT_X, BATTLE_ICON_FIGHT_Y, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(battleIconFight, BATTLE_ICON_FIGHT_WIDTH, BATTLE_ICON_FIGHT_HEIGHT,
                                    BATTLE_ICON_FIGHT_X, BATTLE_ICON_FIGHT_Y, TRANSPARENT_COLOUR);
                }
                if (battleCursor == 1) {
                    draw_sprite_any_shade_pulse(battleIconBag, BATTLE_ICON_SMALL_WIDTH, BATTLE_ICON_SMALL_HEIGHT,
                                                BATTLE_ICON_BAG_X, BATTLE_ICON_BAG_Y, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(battleIconBag, BATTLE_ICON_SMALL_WIDTH, BATTLE_ICON_SMALL_HEIGHT,
                                    BATTLE_ICON_BAG_X, BATTLE_ICON_BAG_Y, TRANSPARENT_COLOUR);
                }
                if (battleCursor == 2) {
                    draw_sprite_any_shade_pulse(battleIconRun, BATTLE_ICON_SMALL_WIDTH, BATTLE_ICON_SMALL_HEIGHT,
                                                BATTLE_ICON_RUN_X, BATTLE_ICON_RUN_Y, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(battleIconRun, BATTLE_ICON_SMALL_WIDTH, BATTLE_ICON_SMALL_HEIGHT,
                                    BATTLE_ICON_RUN_X, BATTLE_ICON_RUN_Y, TRANSPARENT_COLOUR);
                }

                const unsigned short* partyBg;
                if (battleCursor <= 2) {
                    partyBg = battlePartySlotSprites[0];
                }
                else {
                    partyBg = battlePartySlotSprites[battleCursor - 2];
                }

                int selectedPartyIndex = -1;

                if (battleCursor >= 3 && battleCursor <= 8) {
                    selectedPartyIndex = battleCursor - 3; 
                
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

            if (battleUi == BATTLE_UI_ATTACK_MENU) {
                const int mxLeft = 18;
                const int mxRight = 160 + 18;
                const int myTop = 240 - 89;
                const int myBottom = 240 - 45;
                const int mxs[4] = { mxLeft, mxRight, mxLeft, mxRight };
                const int mys[4] = { myTop,  myTop,   myBottom, myBottom };

                pokemonInBattle *playerActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;

                // Text box inside each move sprite
                // 97x9px tall, positioned 10px left and 11px down from the move sprite's top-left.
                const int nameBoxW = 97;
                const int nameBoxH = 9;
                const int nameBoxDx = -5;
                const int nameBoxDy = 13;

                for (int i = 0; i < 4; i++) {
                    const AttackData *move = (playerActive != NULL) ? playerActive->attacks[i] : NULL;
                    const char *moveName = (move != NULL && move->name != NULL) ? move->name : "";

                    AttackTypeSpriteRef moveTypeSprite = (move != NULL) ? attackTypeSpriteFor(move->type)
                                                                       : (AttackTypeSpriteRef){normalTypeSprite, NORMAL_TYPE_WIDTH, NORMAL_TYPE_HEIGHT };

                    if (battleCursor == i) {
                        draw_sprite_any_shade_pulse(moveTypeSprite.pixels, moveTypeSprite.width, moveTypeSprite.height,
                                                    mxs[i], mys[i], TRANSPARENT_COLOUR, shadePulseFrame);
                    } else {
                        draw_sprite_any(moveTypeSprite.pixels, moveTypeSprite.width, moveTypeSprite.height,
                                        mxs[i], mys[i], TRANSPARENT_COLOUR);
                    }

                    drawCenteredStringInBox(mxs[i] + nameBoxDx, mys[i] + nameBoxDy, nameBoxW, nameBoxH,
                                            moveName, BLACK, FONT_5X9);

                    //draws pp levels
                    if (playerActive != NULL && move != NULL) {
                        const int ppX = mxs[i] + CURRENT_PP_X_FROM_ATTACK_SPRITE;
                        const int ppY = mys[i] + CURRENT_PP_Y_FROM_ATTACK_SPRITE;

                        const int ppTotalX = mxs[i] + CURRENT_PP_X_FROM_ATTACK_SPRITE + TOTAL_PP_X_FROM_CURRENT_PP;
                        const int ppTotalY = mys[i] + CURRENT_PP_Y_FROM_ATTACK_SPRITE + TOTAL_PP_Y_FROM_CURRENT_PP;

                        const int ppCur = playerActive->currentPP[i];
                        const int ppMax = move->maxPP;
                        char ppBufCurrent[16];
                        char ppBufTotal[16];

                        snprintf(ppBufCurrent, sizeof(ppBufCurrent), "%d", ppCur);
                        snprintf(ppBufTotal, sizeof(ppBufTotal), "%d",ppMax);
                        draw_string_f(ppX, ppY, ppBufCurrent, BLACK, FONT_5X9);
                        draw_string_f(ppTotalX, ppTotalY, ppBufTotal, BLACK, FONT_5X9);
                    }
                }
            }
    
            break;
        }

        case GAME_STATE_BATTLE_ACTION_TEXT: {
            // Hide the menus, keep the battle UI background, and show queued messages.

            if (actionTextAwaitSpaceRelease) {
                if (!spaceDown) actionTextAwaitSpaceRelease = false;
            }

            // Animations (battle only).
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

            // Draw the normal battle base.
            draw_map();

            draw_sprite_any_bob(playerBackSprite.pixels,
                                playerBackSprite.width, playerBackSprite.height,
                                playerBackSprite.x, playerBackSprite.y,
                                TRANSPARENT_COLOUR,
                                bobFrame);
            draw_sprite_any(enemyFrontSprite.pixels,
                            enemyFrontSprite.width, enemyFrontSprite.height,
                            enemyFrontSprite.x, enemyFrontSprite.y,
                            TRANSPARENT_COLOUR);
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);
            draw_sprite_any_bob(myHpEmpty,
                                MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                TRANSPARENT_COLOUR,
                                bobFrame);
            draw_sprite_any(oppHpEmpty, OPP_HP_EMPTY_WIDTH, OPP_HP_EMPTY_HEIGHT, OPP_HP_EMPTY_X, OPP_HP_EMPTY_Y, TRANSPARENT_COLOUR);

            pokemonInBattle *enemyActive = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
            pokemonInBattle *playerActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;

            const int enemyHp = (enemyActive != NULL) ? enemyActive->scaledStatsWithLevel[0] : 0;
            const int enemyMaxHp = (enemyActive != NULL) ? enemyActive->maxHp : 1;
            const int playerHp = (playerActive != NULL) ? playerActive->scaledStatsWithLevel[0] : 0;
            const int playerMaxHp = (playerActive != NULL) ? playerActive->maxHp : 1;

            const int enemyHpBarWidth = (enemyMaxHp > 0) ? (HP_WIDTH * enemyHp) / enemyMaxHp : 0;
            const int playerHpBarWidth = (playerMaxHp > 0) ? (HP_WIDTH * playerHp) / playerMaxHp : 0;

            const int enemyHpPct = (enemyMaxHp > 0) ? (enemyHp * 100) / enemyMaxHp : 0;
            const int playerHpPct = (playerMaxHp > 0) ? (playerHp * 100) / playerMaxHp : 0;

            const short enemyHpBarColour = (enemyHpPct < 15) ? RED : ((enemyHpPct < 50) ? ORANGE : GREEN);
            const short playerHpBarColour = (playerHpPct < 15) ? RED : ((playerHpPct < 50) ? ORANGE : GREEN);

            char oppLvlBuf[8];
            char myLvlBuf[8];
            char myHpCurBuf[8];
            char myHpMaxBuf[8];
            snprintf(oppLvlBuf, sizeof(oppLvlBuf), "%d", (enemyActive != NULL) ? enemyActive->level : 0);
            snprintf(myLvlBuf, sizeof(myLvlBuf), "%d", (playerActive != NULL) ? playerActive->level : 0);
            snprintf(myHpCurBuf, sizeof(myHpCurBuf), "%d", playerHp);
            snprintf(myHpMaxBuf, sizeof(myHpMaxBuf), "%d", playerMaxHp);

            draw_rect(OPP_HP_EMPTY_X + 50, OPP_HP_EMPTY_Y + 20, (enemyHpBarWidth < 0) ? 0 : ((enemyHpBarWidth > HP_WIDTH) ? HP_WIDTH : enemyHpBarWidth), HP_HEIGHT, enemyHpBarColour);
            draw_string_f(oppLVL_X, oppLVL_Y, oppLvlBuf, BLACK, 1);
            draw_sprite_any(burned, BURNED_WIDTH, BURNED_HEIGHT, STATUS_X, STATUS_Y, TRANSPARENT_COLOUR);
            draw_sprite_any(caught, CAUGHT_WIDTH, CAUGHT_HEIGHT, CAUGHT_X, CAUGHT_Y, TRANSPARENT_COLOUR);
            draw_string_f(OPPNAME_X, OPPNAME_Y, (enemyActive != NULL && enemyActive->id.data != NULL) ? enemyActive->id.data->name : "???", BLACK, 1);

            static const signed char dy_pattern[BOB_SPRITE_FRAME_COUNT] = {
            0, -1, -1, 0, 0, 1, 1, 0,
            0, -1, -1, 0, 0, 1, 1, 0 };
            int offsetY = dy_pattern[bobFrame];
            draw_rect(myHP_X, myHP_Y + offsetY, (playerHpBarWidth < 0) ? 0 : ((playerHpBarWidth > HP_WIDTH) ? HP_WIDTH : playerHpBarWidth), HP_HEIGHT, playerHpBarColour);
            draw_rect(EXP_X, EXP_Y + offsetY, EXP_WIDTH, EXP_HEIGHT, TURQ);
            draw_string_f(myLVL_X, myLVL_Y + offsetY, myLvlBuf, BLACK, 1);
            draw_string_f(MYNAME_X, MYNAME_Y + offsetY, (playerActive != NULL && playerActive->id.data != NULL) ? playerActive->id.data->name : "???", BLACK, 1);
            draw_string_f(TOTAL_HPNUM_X, HPNUM_Y + offsetY, myHpCurBuf, BLACK, 1);
            draw_string_f(REMAINING_HP_X, HPNUM_Y + offsetY, myHpMaxBuf, BLACK, 1);
            draw_sprite_any(poison, POISON_WIDTH, POISON_HEIGHT, MYSTATUS_X, MYSTATUS_Y + offsetY, TRANSPARENT_COLOUR);

            // Textbox message.
            const char *msg = (battleState.messageCount > 0 && battleState.messageReadIndex >= 0 && battleState.messageReadIndex < battleState.messageCount)
                                  ? battleState.messages[battleState.messageReadIndex]
                                  : "";
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, msg, BLACK);

            if (!actionTextAwaitSpaceRelease && spacePressed) {
                battleState.messageReadIndex++;
                if (battleState.messageReadIndex >= battleState.messageCount) {
                    battleState.messageReadIndex = 0;
                    battleState.messageCount = 0;

                    if (battleState.result != BATTLE_RESULT_ONGOING) {
                        currentGameState = GAME_STATE_MAP;
                    } else {
                        battleUi = actionTextReturnUi;
                        battleCursor = actionTextReturnCursor;
                        currentGameState = GAME_STATE_BATTLE;
                        previousGameState = GAME_STATE_BATTLE;
                    }
                }
            }

            break;
        }

        case GAME_STATE_BATTLE_TRANSITION: {
            if (battleState.type == BATTLE_WILD) {
                // Wild battle transition

                // Precompute center
                const int centerX = SCREEN_WIDTH / 2;
                const int centerY = SCREEN_HEIGHT / 2;

                // Draw battle scene underneath
                draw_map();
                draw_sprite_any(battleUIBackgroundSprite,
                                BATTLE_UI_BACKGROUND_WIDTH,
                                BATTLE_UI_BACKGROUND_HEIGHT,
                                0, battleBackdropY,
                                TRANSPARENT_COLOUR);

                // Draw the opponent sprite underneath 
                if (enemyFrontSprite.pixels != NULL) {
                    draw_sprite_any(enemyFrontSprite.pixels,
                                    enemyFrontSprite.width, enemyFrontSprite.height,
                                    enemyFrontSprite.x, enemyFrontSprite.y,
                                    TRANSPARENT_COLOUR);
                }

                // Expanding rectangle
                int halfW = transitionFrame * 12;  // speed (increase for faster)
                int halfH = transitionFrame * 8;

                int left = centerX - halfW;
                int right = centerX + halfW;
                int top = centerY - halfH;
                int bottom = centerY + halfH;

                // Clamp to screen
                if (left < 0) left = 0;
                if (right > SCREEN_WIDTH) right = SCREEN_WIDTH;
                if (top < 0) top = 0;
                if (bottom > SCREEN_HEIGHT) bottom = SCREEN_HEIGHT;

                // Draw black borders
                if (top > 0)
                    draw_rect(0, 0, SCREEN_WIDTH, top, BLACK);
                if (bottom < SCREEN_HEIGHT)
                    draw_rect(0, bottom, SCREEN_WIDTH, SCREEN_HEIGHT - bottom, BLACK);
                if (left > 0)
                    draw_rect(0, top, left, bottom - top, BLACK);
                if (right < SCREEN_WIDTH)
                    draw_rect(right, top, SCREEN_WIDTH - right, bottom - top, BLACK);

                transitionTimer++;
                if (transitionTimer >= transitionSpeedFrames) {
                    transitionTimer = 0;
                    transitionFrame++;
                }

                if (left == 0 && right == SCREEN_WIDTH &&
                    top == 0 && bottom == SCREEN_HEIGHT) {

                    currentGameState = GAME_STATE_BATTLE_INTRO_TEXT;
                    previousGameState = GAME_STATE_BATTLE_INTRO_TEXT;
                    battleIntroTextReady = false;
                    transitionFrame = 0;
                }
            } else {
                // Trainer battle transition (placeholder)
                draw_map();
                draw_sprite_any(battleUIBackgroundSprite,
                                BATTLE_UI_BACKGROUND_WIDTH,
                                BATTLE_UI_BACKGROUND_HEIGHT,
                                0, battleBackdropY,
                                TRANSPARENT_COLOUR);

                draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
                draw_string_f(92, 112, "TRAINER BATTLE", WHITE, FONT_5X9);

                transitionTimer++;
                if (transitionTimer >= transitionSpeedFrames) {
                    transitionTimer = 0;
                    transitionFrame++;
                }

                if (transitionFrame >= 20) {
                    currentGameState = GAME_STATE_BATTLE_INTRO_TEXT;
                    previousGameState = GAME_STATE_BATTLE_INTRO_TEXT;
                    battleIntroTextReady = false;
                    transitionFrame = 0;
                }
            }
        
            break;
        }
        
        case GAME_STATE_BATTLE_INTRO_TEXT: {
            draw_map();
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);

            if (enemyFrontSprite.pixels != NULL) {
                draw_sprite_any(enemyFrontSprite.pixels,
                                enemyFrontSprite.width, enemyFrontSprite.height,
                                enemyFrontSprite.x, enemyFrontSprite.y,
                                TRANSPARENT_COLOUR);
            }

            if (!battleIntroTextReady) {
                const pokemonInBattle *enemyActive = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
                const char *enemyName = (enemyActive != NULL && enemyActive->id.data != NULL && enemyActive->id.data->name != NULL)
                                            ? enemyActive->id.data->name
                                            : "???";
                // Wild battle intro text 
                snprintf(battleIntroText, sizeof(battleIntroText), "%s wants to fight!", enemyName);
                battleIntroTextReady = true;
            }

            int done = draw_textbox_animated_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, battleIntroText, BLACK);
        
            if (done && spacePressed) {
                play_sfx(plink_audio, plink_audio_len);
                currentGameState = GAME_STATE_POKEBALL_THROW;
                previousGameState = GAME_STATE_POKEBALL_THROW;
                battleThrowPokeballTextReady = false;
            }
        
            break;
        }

        case GAME_STATE_POKEBALL_THROW: {
            //  projectile motion variables (for arc throwing)
            if (!pokeballThrowInit) {
                pokeballThrowInit = true;
                pokeballThrown = false;
                pokeballLanding = false;
                pokeballThrowT = 0;
                pokeballThrownFrame = 0;
                pokeballThrownTimer = 0;
                pokeballLandingTimer = 0;
                pokeballLandingFrame = 8;

                // Start off-screen (left + below). Only becomes visible at the peak.
                pokeballX0 = -(float)POKEBALLTHROW_WIDTH;

                const float landX = (float)(playerBackSprite.x + playerBackSprite.width / 2 - POKEBALLTHROW_WIDTH / 2);
                const float landY = (float)(playerBackSprite.y + playerBackSprite.height - (POKEBALLTHROW_HEIGHT / 2)) - 40;
                pokeballLandX = landX;
                pokeballLandY = landY;

                // Peak above the player's side.
                float yPeak = (float)(playerBackSprite.y - 30);
                if (yPeak < 5.0f) yPeak = 5.0f;

                pokeballVy = -(pokeballG * (float)pokeballTPeak);
                pokeballY0 = yPeak + 0.5f * pokeballG * (float)(pokeballTPeak * pokeballTPeak);

                float dy = pokeballLandY - yPeak;
                if (dy < 0.0f) dy = 0.0f;
                const float tLand = (float)pokeballTPeak + sqrtf((2.0f * dy) / pokeballG);
                pokeballVx = (pokeballLandX - pokeballX0) / ((tLand > 1.0f) ? tLand : 1.0f);
            }

            if (!battleThrowPokeballTextReady) {
                const pokemonInBattle *myActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;
                const char *myPokemonName = (myActive != NULL && myActive->id.data != NULL && myActive->id.data->name != NULL)
                                                ? myActive->id.data->name
                                                : "???";
                snprintf(battleThrowPokeballText, sizeof(battleThrowPokeballText), "Go! %s!", myPokemonName);
                battleThrowPokeballTextReady = true;
            }

            // Draw battle background + sprites (hide your Pokémon until the throw finishes).
            draw_map();
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);
            draw_sprite_any(enemyFrontSprite.pixels,
                            enemyFrontSprite.width, enemyFrontSprite.height,
                            enemyFrontSprite.x, enemyFrontSprite.y,
                            TRANSPARENT_COLOUR);

            // Update/draw projectile.
            if (!pokeballThrown) {
                // Advance the simulated time (frames).
                pokeballThrowT++;

                const float t = (float)pokeballThrowT;
                const float x = pokeballX0 + pokeballVx * t;
                const float y = pokeballY0 + pokeballVy * t + 0.5f * pokeballG * t * t;

                if (!pokeballLanding && pokeballThrowT >= pokeballTPeak) {
                    // Animate frames 0..7 only (pokeball circling around)
                    const int inflightFrames = 8;
                    const int inflightSpeed = 3;
                    pokeballThrownTimer++;
                    if (pokeballThrownTimer >= inflightSpeed) {
                        pokeballThrownTimer = 0;
                        pokeballThrownFrame = (pokeballThrownFrame + 1) % inflightFrames;
                    }

                    const int dx = (int)(x + 0.5f);
                    const int dy = (int)(y + 0.5f);
                    if (dy >= -POKEBALLTHROW_HEIGHT && dy < SCREEN_HEIGHT) {
                        draw_sprite_any(pokeballThrowFrames[pokeballThrownFrame],
                                        POKEBALLTHROW_WIDTH,
                                        POKEBALLTHROW_HEIGHT,
                                        dx,
                                        dy,
                                        TRANSPARENT_COLOUR);
                    }
                }

                // frames 8-11  when pokeball lands
                if (!pokeballLanding && pokeballThrowT >= pokeballTPeak && y >= pokeballLandY) {
                    pokeballLanding = true;
                    pokeballLandingFrame = 8;
                    pokeballLandingTimer = 0;
                }

                if (pokeballLanding) {
                    const int landSpeed = 6;
                    pokeballLandingTimer++;
                    if (pokeballLandingTimer >= landSpeed) {
                        pokeballLandingTimer = 0;
                        pokeballLandingFrame++;
                        if (pokeballLandingFrame > 10) {
                            pokeballThrown = true;
                        }
                    }

                    if (!pokeballThrown) {
                        const int dx = (int)(pokeballLandX + 0.5f);
                        const int dy = (int)(pokeballLandY + 0.5f);
                        draw_sprite_any(pokeballThrowFrames[pokeballLandingFrame],
                                        POKEBALLTHROW_WIDTH,
                                        POKEBALLTHROW_HEIGHT,
                                        dx,
                                        dy,
                                        TRANSPARENT_COLOUR);
                    }
                }
            }

            if (pokeballThrown) {
                            draw_sprite_any(playerBackSprite.pixels,
                                playerBackSprite.width, playerBackSprite.height,
                                playerBackSprite.x, playerBackSprite.y,
                                TRANSPARENT_COLOUR);
            }

            // Textbox (instant) over the throw animation.
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, battleThrowPokeballText, BLACK);

            if (pokeballThrown && spacePressed) {
                play_sfx(plink_audio, plink_audio_len);
                currentGameState = GAME_STATE_BATTLE;
                previousGameState = GAME_STATE_BATTLE;
                battleThrowPokeballTextReady = false;
                pokeballThrowInit = false;
            }
            break;
        }
      
        case GAME_STATE_MAP:
        default:

            draw_map();
            mcMovingTick(upDown, downDown, leftDown, rightDown, is_key_shift_pressed());
            break;
        }

        wait_for_vsync();
    }

    return 0;
}
