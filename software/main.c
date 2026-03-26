#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/pokemon/charizardSprite.h"
#include "graphics/sprites/pokemon/rayquazaSprite.h"
#include "graphics/sprites/pokemon/pokemonSpriteInit.h"
#include "graphics/sprites/staticSprite.h"
#include "gameplayLogic/map_movement/mcMoving.h"
#include "gameplayLogic/worldMap.h"
#include "graphics/tiles.h"
#include "graphics/map.h"
#include "graphics/sprites/vsCynthia/vsCynthiaSprite.h"
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
#include "../software/se/pokeball_audio.h"
#include "../software/se/pokeball_shaking_audio.h"
#include "../software/se/caught_pokemon_audio.h"
#include "../software/se/hit_normal_audio.h"
#include "../software/se/recover_audio.h"
#include "../software/graphics/sprites/pokemonAreas/pokemonAreaBack.h"
#include "../software/graphics/sprites/pokemonAreas/pokemonAreaFront.h"
#include "textinput/getTextFromUser.h"
#include "graphics/textbox/small_spacebar.h"
#include "graphics/sprites/battleicons/battle_icons.h"
#include "graphics/sprites/battleIcons/battleHp/myHpEmpty.h"
#include "graphics/sprites/battleIcons/battleHp/oppHpEmpty.h"
#include "graphics/sprites/battleItemsUI/hp.h"
#include "graphics/sprites/battleItemsUI/pokeballs.h"
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
#include "graphics/sprites/boxSprites/rayquazaBoxSprite.h"
#include "graphics/sprites/boxSprites/pokemonBoxSpriteInit.h"
#include "graphics/sprites/battleIcons/attackTypes/attackTypeSprites.h"
#include "graphics/sprites/battleUIBackground/battleUIBackgroundSprite.h"
#include "gameplayLogic/battling/battleLoop.h"
#include "gameplayLogic/entities/pokemonDataBase.h"
#include "gameplayLogic/bag.h"
#include "gameplayLogic/itemDatabase.h"
#include "gameplayLogic/storage/pc.h"
#include "graphics/sprites/battleItemsUI/useLastItem.h"
#include "graphics/sprites/battleItemsUI/useButton.h"
#include "graphics/sprites/battleItemsUI/itemSlot.h"
#include "graphics/sprites/battleItemsUI/itemDescription.h"
#include "graphics/sprites/battleItemsUI/pokeballIcons.h"
#include "graphics/sprites/battleItemsUI/healingItemIcons.h"
#include "graphics/sprites/menu/menuSprites.h"
#include "graphics/sprites/pokeballThrow/pokeballThrow_frames.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
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

#define USE_LAST_ICON_X 60
#define USE_LAST_ICON_Y 240 - BATTLE_BAG_USE_LAST_ITEM_HEIGHT - 1

//location from top left of itemslot
#define ITEM_X 20 //20 right
#define ITEM_Y 24 //24 down

#define ITEM_NAME_X 20 //20 right
#define ITEM_NAME_Y 8 //8 down

#define LAST_USED_ITEM_X 20 //20 right
#define LAST_USED_ITEM_Y 10 //10 down

#define DEFINITION_X 20
#define DEFINITION_Y 10
#define DEFINITION_X_NAME 52
#define DEFINITION_Y_NAME 15
#define DEFINITION_X_TEXT 21
#define DEFINITION_Y_TEXT 45

#define ITEM_AMOUNT_X 54
#define ITEM_AMOUNT_Y 31

//restore items are a bit differnt in dimensions compared to pokeballs (like a bit higher and to the left)
#define RESTORE_ITEM_X ITEM_X - 6
#define RESTORE_ITEM_Y ITEM_Y - 5

#define AREA_FRONT_X 160
#define AREA_FRONT_Y 70
#define AREA_BACK_X 0
#define AREA_BACK_Y 120

// Game States
typedef enum {
    GAME_STATE_MAP = 0,
    GAME_STATE_WILD_BATTLE = 1,
    GAME_STATE_WILD_BATTLE_TRANSITION = 2,
    GAME_STATE_WILD_BATTLE_INTRO_TEXT = 3,

    GAME_STATE_TRAINER_BATTLE = 4,
    GAME_STATE_TRAINER_BATTLE_TRANSITION = 5,
    GAME_STATE_TRAINER_BATTLE_INTRO_TEXT = 6,

    GAME_STATE_BATTLE_ACTION_TEXT = 7,
    GAME_STATE_POKEBALL_THROW = 8,
    GAME_STATE_POKEBALL_CATCH = 9,
    GAME_STATE_BATTLE_WIN = 10,
    GAME_STATE_BATTLE_LOSE = 11,
    GAME_STATE_MENU = 12,
} GameState;

typedef enum {
    ARROW_CTX_NONE = 0,
    ARROW_CTX_BATTLE_MENU = 1,
    ARROW_CTX_BATTLE_ATTACK = 2,
    ARROW_CTX_BATTLE_BAG_MENU = 3,
    ARROW_CTX_BATTLE_BAG_LIST = 4,
} ArrowContext;

typedef enum {
    BATTLE_UI_MENU = 0,
    BATTLE_UI_ATTACK_MENU = 1,
    BATTLE_UI_BAG_MENU = 2,
    BATTLE_UI_BAG_HP_LIST = 3,
    BATTLE_UI_BAG_BALL_LIST = 4,
    BATTLE_UI_BAG_ITEM_DESC = 5,
} BattleUiState;

static inline bool isBattleMenuState(GameState state) {
    return state == GAME_STATE_WILD_BATTLE || state == GAME_STATE_TRAINER_BATTLE;
}

static inline bool isOverworldState(GameState state) {
    return state == GAME_STATE_MAP || state == GAME_STATE_MENU;
}

static ArrowContext getArrowContext(GameState state, BattleUiState battleUi) {
    if (!isBattleMenuState(state)) return ARROW_CTX_NONE;
    if (battleUi == BATTLE_UI_ATTACK_MENU) return ARROW_CTX_BATTLE_ATTACK;
    if (battleUi == BATTLE_UI_BAG_MENU) return ARROW_CTX_BATTLE_BAG_MENU;
    if (battleUi == BATTLE_UI_BAG_HP_LIST || battleUi == BATTLE_UI_BAG_BALL_LIST) return ARROW_CTX_BATTLE_BAG_LIST;
    if (battleUi == BATTLE_UI_BAG_ITEM_DESC) return ARROW_CTX_NONE;
    return ARROW_CTX_BATTLE_MENU;
}

static int arrowCursorCount(ArrowContext ctx) {
    switch (ctx) {
        case ARROW_CTX_BATTLE_MENU: return 9;   // Fight/Bag/Run/party members
        case ARROW_CTX_BATTLE_ATTACK: return 4; 
        case ARROW_CTX_BATTLE_BAG_MENU: return 3;
        case ARROW_CTX_BATTLE_BAG_LIST: return 4;
        default: return 0;
    }
}

static bool isManualBattleMessage(const char *msg) {
    if (msg == NULL) return false;
    // Type effectiveness messages get a longer on-screen delay.
    return (strstr(msg, "super effective") != NULL) ||
           (strstr(msg, "not very effective") != NULL) ||
           (strstr(msg, "no effect") != NULL);
}

typedef enum { DIR_UP = 0, DIR_LEFT = 1, DIR_DOWN = 2, DIR_RIGHT = 3 } NavDir;
static int navBattleBag3(int index, NavDir dir) {
    if (index < 0) index = 0;
    if (index > 2) index = 2;

    if (index == 2) {
        if (dir == DIR_UP) return 0;
        return 2;
    }
    if (dir == DIR_DOWN) return 2;
    if (index == 0) {
        if (dir == DIR_RIGHT) return 1;
        return 0;
    }

    if (dir == DIR_LEFT) return 0;
    return 1;
}

static int navBattleMenu9(int index, NavDir dir) {
    static const signed char nav[9][4] = {
        /*0 Fight*/ {0, 0, 0, 1},
        /*1 Bag */ {1, 0, 2, 3},
        /*2 Run */ {1, 0, 2, 6},
        /*3 P1 */ {3, 1, 6, 4},
        /*4 P2 */ {4, 3, 7, 5},
        /*5 P3 */ {5, 4, 8, 5},
        /*6 P4 */ {3, 2, 6, 7},
        /*7 P5 */ {4, 6, 7, 8},
        /*8 P6 */ {5, 7, 8, 8},
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

static bool is_mc_on_grass_patch(void) {
    const McBounds bounds = getMCBounds();
    if (!bounds.valid) return false;

    const int foot_tile_x = ((bounds.x0 + bounds.x1) / 2) / TILE_SIZE;
    const int foot_tile_y = bounds.y1 / TILE_SIZE;
    if (foot_tile_x < 0 || foot_tile_x >= MAP_WIDTH || foot_tile_y < 0 || foot_tile_y >= MAP_HEIGHT) {
        return false;
    }

    return map_overlay[foot_tile_y][foot_tile_x] == TILE_GRASS_PATCH;
}

static bool should_trigger_grass_battle(bool upPressed, bool downPressed,
                                        bool leftPressed, bool rightPressed) {
    static unsigned int grassEncounterRng = 0x2432026u;
    const bool movedInputPressed = upPressed || downPressed || leftPressed || rightPressed;
    if (!movedInputPressed || !is_mc_on_grass_patch()) {
        return false;
    }

    grassEncounterRng = grassEncounterRng * 1664525u + 1013904223u;
    return (grassEncounterRng % 10u) == 0u;
}

//returns the address of the global pokemon structs for the pokemon
static const PokemonData *speciesFromPokemonSpriteId(int pokemonId) {
    switch (pokemonId) {
        case POKEMON_ID_CHARMANDER: return &CHARMANDER;
        case POKEMON_ID_CHARMELEON: return &CHARMELEON;
        case POKEMON_ID_CHARIZARD:  return &CHARIZARD;
        case POKEMON_ID_RAYQUAZA:  return &RAYQUAZA;
        case POKEMON_ID_GARCHOMP:  return &GARCHOMP;
        case POKEMON_ID_LUCARIO:   return &LUCARIO;
        case POKEMON_ID_MILOTIC:   return &MILOTIC;
        case POKEMON_ID_ROSERADE:  return &ROSERADE;
        case POKEMON_ID_SPIRITOMB: return &SPIRITOMB;
        case POKEMON_ID_TOGEKISS:  return &TOGEKISS;
        default: return NULL;
    }
}

static void setupCynthiaTrainerParty(Party *enemyParty, pokemonInBattle team[6]) {
    if (enemyParty == NULL || team == NULL) return;

    initParty(enemyParty);

    // Cynthia's party
    initPokemonInBattle(&team[0], &SPIRITOMB, 61);
    initPokemonInBattle(&team[1], &ROSERADE, 60);
    initPokemonInBattle(&team[2], &TOGEKISS, 60);
    initPokemonInBattle(&team[3], &LUCARIO, 63);
    initPokemonInBattle(&team[4], &MILOTIC, 63);
    initPokemonInBattle(&team[5], &GARCHOMP, 66);

    for (int i = 0; i < 6; i++) {
        (void)addPokemonToParty(enemyParty, &team[i]);
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

static void draw_wrapped_5x9(int x, int y, int maxW, int maxLines, const char *text, short colour) {
    if (text == NULL) return;
    if (maxW <= 0 || maxLines <= 0) return;

    const int advance = 6; // 5px width + 1px gap
    const int glyphW = 5;
    int maxChars = (maxW - glyphW) / advance + 1;
    if (maxChars < 1) maxChars = 1;
    if (maxChars > 30) maxChars = 30;

    int pos = 0;
    for (int line = 0; line < maxLines && text[pos]; line++) {
        while (text[pos] == ' ') pos++;

        char buf[32];
        int bi = 0;
        int lastSpace = -1;
        int consumed = 0;

        while (text[pos] && bi < maxChars) {
            char c = text[pos];
            if (c == '\n') break;
            if (c == ' ') lastSpace = bi;
            buf[bi++] = c;
            pos++;
            consumed++;
        }

        // If we cut a word, backtrack to last space.
        if (text[pos] && text[pos] != '\n' && lastSpace > 0 && bi == maxChars) {
            int back = bi - lastSpace - 1;
            pos -= back;
            bi = lastSpace;
        }

        // Trim trailing spaces.
        while (bi > 0 && buf[bi - 1] == ' ') bi--;
        buf[bi] = '\0';

        if (bi > 0) draw_string_f(x, y + line * 10, buf, colour, FONT_5X9);

        if (text[pos] == '\n') pos++;
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
    float pokeballPeakX = 0.0f;
    float pokeballTLand = 0.0f;
    float pokeballLandX = 0.0f, pokeballLandY = 0.0f;
    int pokeballLandingFrame = POKEBALLTHROW_LAND_FRAME_START;

    // Pokeball catch animation state (throw to enemy + catch/shake).
    bool pokeballCatchInit = false;
    bool pokeballCatchLanding = false;
    bool pokeballCatchEscape = false;
    bool pokeballCatchSuccess = false;
    ItemId pokeballCatchItem = ITEM_NONE;
    int pokeballCatchT = 0;
    int pokeballCatchThrownTimer = 0;
    int pokeballCatchLandingTimer = 0;
    int pokeballCatchSeqTimer = 0;
    int pokeballCatchFrame = 0;
    int pokeballCatchLandingFrame = POKEBALLTHROW_LAND_FRAME_START;
    int pokeballCatchSeqFrame = POKEBALLTHROW_CATCH_FRAME_START;
    float pokeballCatchStartX = 0.0f, pokeballCatchStartY = 0.0f;
    float pokeballCatchEndX = 0.0f, pokeballCatchEndY = 0.0f;
    float pokeballCatchCtrlX = 0.0f, pokeballCatchCtrlY = 0.0f;

    unsigned int frame_count = 0;
    int current_phase = 0;    
    short colour = BLACK;    
    int bobFrame = 0;
    int bobTimer = 0;
    const int bobSpeedFrames = 3;
    int shadePulseFrame = 0;
    int shakeFrame = 0;
    int shakeTimer = 0;
    int playerHitShakeFrame = -1;
    int enemyHitShakeFrame = -1;
    int bobPartyFrame = 0;
    int bobPartyTimer = 0;
    const int shakeSpeedFrames = 3;

    char battleIntroText[96];
    bool battleIntroTextReady = false;

    char battleThrowPokeballText[96];
    bool battleThrowPokeballTextReady = false;
    bool pokeballThrowShowText = true;
    bool pokeballThrowAutoAdvance = false;
    GameState pokeballThrowReturnState = GAME_STATE_WILD_BATTLE;

    GameState currentGameState = GAME_STATE_MAP;
    BattleUiState battleUi = BATTLE_UI_MENU;
    GameState previousGameState = currentGameState;
    WorldMapId currentMapId = WORLD_MAP_ROUTE_A;
    GameState activeBattleMenuState = GAME_STATE_WILD_BATTLE;
    int battleCursor = 0;

    // Bag UI state.
    int bagHpPage = 0;
    int bagBallPage = 0;
    BattleUiState bagDescReturnUi = BATTLE_UI_BAG_HP_LIST;
    int bagDescReturnCursor = 0;
    int bagDescReturnPage = 0;
    ItemId bagDescItem = ITEM_NONE;

    int arrowAnimFrame = 0;
    int arrowAnimTimer = 0;
    const int arrowAnimSpeedFrames = 8;
    bool prevUp = false, prevLeft = false, prevDown = false, prevRight = false;
    bool prevEsc = false;
    bool actionTextAwaitSpaceRelease = false;
    BattleUiState actionTextReturnUi = BATTLE_UI_MENU;
    int actionTextReturnCursor = 0;
    GameState actionTextReturnGameState = GAME_STATE_WILD_BATTLE;
    int actionTextAutoTimer = 0;
    int actionTextInterAttackPauseTimer = 5;
    int actionTextLastMsgIndex = -1;

    // Battle logic 
    BattleState battleState;
    Party playerParty;
    Party enemyParty;
    PC playerPc;
    Bag playerBag;
    pokemonInBattle wildEnemy;

    pokemonInBattle cynthiaTeam[6];
    BattleType nextBattleType = BATTLE_WILD;

    StaticSprite playerBackSprite;
    StaticSprite enemyFrontSprite;

    // Temporary team 
    const int playerTeamSpriteIds[6] = {
        POKEMON_ID_RAYQUAZA,
        POKEMON_ID_CHARIZARD,
        POKEMON_ID_GARCHOMP,
        POKEMON_ID_CHARIZARD,
        POKEMON_ID_LUCARIO,
        POKEMON_ID_MILOTIC,
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

    bagInit(&playerBag);
    // Starter bag 
    bagAdd(&playerBag, ITEM_POTION, 99);
    bagAdd(&playerBag, ITEM_SUPER_POTION, 3);
    bagAdd(&playerBag, ITEM_HYPER_POTION, 3);
    bagAdd(&playerBag, ITEM_FULL_RESTORE, 3);

    bagAdd(&playerBag, ITEM_POKEBALL, 5);
    bagAdd(&playerBag, ITEM_GREAT_BALL, 2);
    bagAdd(&playerBag, ITEM_ULTRA_BALL, 1);
    bagAdd(&playerBag, ITEM_PREMIER_BALL, 1);
    bagAdd(&playerBag, ITEM_MASTER_BALL, 99);

    for (int i = 0; i < 6; i++) {
        const PokemonData *species = speciesFromPokemonSpriteId(playerTeamSpriteIds[i]);
        int ownedIndex = -1;
        if (species != NULL && pcAdd(&playerPc, species, 60 + i, &ownedIndex)) {
            addPokemonToParty(&playerParty, pcGet(&playerPc, ownedIndex));
        }
    }

    initParty(&enemyParty);
    initPokemonInBattle(&wildEnemy, &CHARMANDER, 18);
    addPokemonToParty(&enemyParty, &wildEnemy);
    initBattleState(&battleState, &playerParty, &enemyParty, &playerBag, BATTLE_WILD);

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

        if (textboxDone && spacePressed) {
            play_sfx(plink_audio, plink_audio_len);
            break;
        }
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

        if (textboxDone && spacePressed) {
            play_sfx(plink_audio, plink_audio_len);
            break;
        }
        wait_for_vsync();
    }
    stop_bgm();

    // Initial state setup: start in the walkable map.
    currentGameState = GAME_STATE_MAP;
    previousGameState = GAME_STATE_MAP;
    currentMapId = WORLD_MAP_ROUTE_A;
    load_world_map(currentMapId, 80, 112, MC_FACING_S);
    play_bgm(map_audio, map_audio_len);
    wait_for_vsync();

    textboxDone = 0;
    prevSpaceDown = false;
    int menuCursor = 0; // 0..5 (2 columns x 3 rows)

    while (1) {
        update_keyboard();
        {
            char ch = 0;
            while (keyboard_pop_char(&ch)) {
                if (ch == '2') {
                    currentGameState = GAME_STATE_MAP;
                    nextBattleType = BATTLE_WILD;
                } else if (ch == '1' && currentGameState == GAME_STATE_MAP) {
                    nextBattleType = BATTLE_WILD;
                    currentGameState = GAME_STATE_WILD_BATTLE;
                } else if (ch == '3' && currentGameState == GAME_STATE_MAP) {
                    nextBattleType = BATTLE_TRAINER;
                    currentGameState = GAME_STATE_TRAINER_BATTLE;
                } else if (ch == '4' && currentGameState == GAME_STATE_MAP) {
                    currentGameState = GAME_STATE_MENU;
                    menuCursor = 0;
                } else if (ch == '4' && currentGameState == GAME_STATE_MENU) {
                    currentGameState = GAME_STATE_MAP;
                }
            }
        }

        const bool wasOverworld = isOverworldState(previousGameState);
        const bool isOverworld = isOverworldState(currentGameState);
        if (wasOverworld != isOverworld) {
            if (isOverworld) {
                play_bgm(map_audio, map_audio_len);
                load_world_map(currentMapId, 80, 112, MC_FACING_S);
            } else {
                play_bgm(battle_audio, battle_audio_len);
                init_map();
                load_map_preset(MAP_PRESET_BACKDROP1);
                battleUi = BATTLE_UI_MENU;
                battleCursor = 0;

 
                // Player party is persistent for this program run
                if (nextBattleType == BATTLE_WILD) {
                    initParty(&enemyParty);
                    initPokemonInBattle(&wildEnemy, &CHARMANDER, 18);
                    addPokemonToParty(&enemyParty, &wildEnemy);
                } else {
                    //battle type is BATTLE_TRAINER
                    setupCynthiaTrainerParty(&enemyParty, cynthiaTeam);
                }
                initBattleState(&battleState, &playerParty, &enemyParty, &playerBag, nextBattleType);

                syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);

                activeBattleMenuState = (nextBattleType == BATTLE_WILD) ? GAME_STATE_WILD_BATTLE : GAME_STATE_TRAINER_BATTLE;
                currentGameState = (nextBattleType == BATTLE_WILD) ? GAME_STATE_WILD_BATTLE_TRANSITION : GAME_STATE_TRAINER_BATTLE_TRANSITION;
                transitionFrame = 0;
                transitionTimer = 0;
                battleIntroTextReady = false;
            }
            previousGameState = currentGameState;
        }
        const bool spaceDown = is_key_space_pressed();
        const bool spacePressed = spaceDown && !prevSpaceDown;
        prevSpaceDown = spaceDown;

        const bool escDown = is_key_escape_pressed();
        const bool escPressed = escDown && !prevEsc;
        prevEsc = escDown;

        if (currentGameState == GAME_STATE_MENU && escPressed) {
            currentGameState = GAME_STATE_MAP;
        }

        if (isBattleMenuState(currentGameState) && escPressed) {
            if (battleUi == BATTLE_UI_ATTACK_MENU) {
                battleUi = BATTLE_UI_MENU;
                battleCursor = 0;
                play_sfx(plink_audio, plink_audio_len);
            } else if (battleUi == BATTLE_UI_BAG_MENU) {
                battleUi = BATTLE_UI_MENU;
                battleCursor = 1;
                play_sfx(plink_audio, plink_audio_len);
            } else if (battleUi == BATTLE_UI_BAG_HP_LIST) {
                battleUi = BATTLE_UI_BAG_MENU;
                battleCursor = 0;
                play_sfx(plink_audio, plink_audio_len);
            } else if (battleUi == BATTLE_UI_BAG_BALL_LIST) {
                battleUi = BATTLE_UI_BAG_MENU;
                battleCursor = 1;
                play_sfx(plink_audio, plink_audio_len);
            } else if (battleUi == BATTLE_UI_BAG_ITEM_DESC) {
                battleUi = bagDescReturnUi;
                battleCursor = bagDescReturnCursor;
                if (bagDescReturnUi == BATTLE_UI_BAG_HP_LIST) {
                    bagHpPage = bagDescReturnPage;
                } else if (bagDescReturnUi == BATTLE_UI_BAG_BALL_LIST) {
                    bagBallPage = bagDescReturnPage;
                }
                play_sfx(plink_audio, plink_audio_len);
            }
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
        case GAME_STATE_MENU: {
            
            const int menuX = (SCREEN_WIDTH - MENU_PARTY_MENU_WIDTH) / 2;
            const int menuY = (SCREEN_HEIGHT - MENU_PARTY_MENU_HEIGHT) / 2;
            const int menuY_right = menuY + 8;

            if (menuCursor < 0) menuCursor = 0;
            if (menuCursor > 5) menuCursor = 5;

            const int row = menuCursor / 2;
            const int col = menuCursor % 2;
            int nextRow = row;
            int nextCol = col;

            if (upPressed && row > 0) nextRow--;
            if (downPressed && row < 2) nextRow++;
            if (leftPressed && col > 0) nextCol--;
            if (rightPressed && col < 1) nextCol++;

            const int nextCursor = nextRow * 2 + nextCol;
            if (nextCursor != menuCursor) {
                menuCursor = nextCursor;
                play_sfx(plink_audio, plink_audio_len);
            }

            clear_screen();
            draw_sprite_any(partyMenuSprite,
                            MENU_PARTY_MENU_WIDTH, MENU_PARTY_MENU_HEIGHT,
                            menuX, menuY,
                            TRANSPARENT_COLOUR);

            const int slotW = 128;
            const int slotH = 49;
            const int unselectedDx = (slotW - MENU_POKEMON_UNSELECTED_WIDTH) / 2;
            const int unselectedDy = (slotH - MENU_POKEMON_UNSELECTED_HEIGHT) / 2;

            for (int i = 0; i < 6; i++) {
                const int slotX = menuX + (i % 2) * slotW;
                const int slotY = menuY + (i / 2) * slotH;
                if (i == menuCursor) {
                    draw_sprite_any(pokemonSelectedSprite,
                                    MENU_POKEMON_SELECTED_WIDTH, MENU_POKEMON_SELECTED_HEIGHT,
                                    slotX, slotY,
                                    TRANSPARENT_COLOUR);
                } else {
                    draw_sprite_any(pokemonUnselectedSprite,
                                    MENU_POKEMON_UNSELECTED_WIDTH, MENU_POKEMON_UNSELECTED_HEIGHT,
                                    slotX + unselectedDx, slotY + unselectedDy,
                                    TRANSPARENT_COLOUR);
                }
            }
            break;
        }
        case GAME_STATE_WILD_BATTLE:
        case GAME_STATE_TRAINER_BATTLE: {
            activeBattleMenuState = currentGameState;
            actionTextReturnGameState = currentGameState;
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
                    } else if (arrowCtx == ARROW_CTX_BATTLE_BAG_MENU) {
                        const int oldIndex = battleCursor;
                        if (upPressed) battleCursor = navBattleBag3(battleCursor, DIR_UP);
                        if (leftPressed) battleCursor = navBattleBag3(battleCursor, DIR_LEFT);
                        if (downPressed) battleCursor = navBattleBag3(battleCursor, DIR_DOWN);
                        if (rightPressed) battleCursor = navBattleBag3(battleCursor, DIR_RIGHT);
                        didMoveBattleCursor = (battleCursor != oldIndex);
                    } else if (arrowCtx == ARROW_CTX_BATTLE_BAG_LIST) {
                        const int oldIndex = battleCursor;
                        int *pagePtr = (battleUi == BATTLE_UI_BAG_HP_LIST) ? &bagHpPage : &bagBallPage;
                        const int itemCount = (battleUi == BATTLE_UI_BAG_HP_LIST) ? bagHpVisibleCount(&playerBag) : bagBallVisibleCount(&playerBag);
                        int pageCount = (itemCount + 3) / 4;
                        if (pageCount < 1) pageCount = 1;
                        if (*pagePtr < 0) *pagePtr = 0;
                        if (*pagePtr >= pageCount) *pagePtr = pageCount - 1;
                        const int oldPage = *pagePtr;

                        if (upPressed) battleCursor = navBattleAttack4(battleCursor, DIR_UP);
                        if (downPressed) battleCursor = navBattleAttack4(battleCursor, DIR_DOWN);

                        if (leftPressed) {
                            const int next = navBattleAttack4(battleCursor, DIR_LEFT);
                            const bool atLeftEdge = (battleCursor == 0 || battleCursor == 2) && (next == battleCursor);
                            if (atLeftEdge && *pagePtr > 0) {
                                (*pagePtr)--;
                            } else {
                                battleCursor = next;
                            }
                        }
                        if (rightPressed) {
                            const int next = navBattleAttack4(battleCursor, DIR_RIGHT);
                            const bool atRightEdge = (battleCursor == 1 || battleCursor == 3) && (next == battleCursor);
                            if (atRightEdge && *pagePtr < pageCount - 1) {
                                (*pagePtr)++;
                            } else {
                                battleCursor = next;
                            }
                        }

                        didMoveBattleCursor = (battleCursor != oldIndex) || (*pagePtr != oldPage);
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
                        battleUi = BATTLE_UI_BAG_MENU;
                        battleCursor = 0;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (battleCursor == 2) {
                        if (battleState.type != BATTLE_WILD) {
                            battleUiSetSingleMessage(&battleState, "You can't run from a trainer battle!");
                            actionTextReturnUi = battleUi;
                            actionTextReturnCursor = battleCursor;
                            actionTextReturnGameState = activeBattleMenuState;
                            currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            actionTextAwaitSpaceRelease = true;
                            play_sfx(plink_audio, plink_audio_len);
                        } else {
                            battleApplyPlayerAction(&battleState, ACTION_RUN, 0);
                            play_sfx(plink_audio, plink_audio_len);
                            syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                            if (battleState.result == BATTLE_RESULT_FLED) {
                                currentGameState = GAME_STATE_MAP;
                            }
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

                            if (battleState.messageCount <= 1) {
                                battleState.messageCount = 0;
                                battleState.messageReadIndex = 0;
                            } else {
                                battleState.messageReadIndex = 1;
                            }

                            pokeballThrowShowText = true;
                            pokeballThrowAutoAdvance = true;
                            pokeballThrowReturnState = GAME_STATE_BATTLE_ACTION_TEXT;
                            pokeballThrowInit = false;
                            battleThrowPokeballTextReady = false;

                            actionTextReturnUi = BATTLE_UI_MENU;
                            actionTextReturnCursor = 0;
                            actionTextAwaitSpaceRelease = true;

                            battleUi = BATTLE_UI_MENU;
                            battleCursor = 0;
                            currentGameState = GAME_STATE_POKEBALL_THROW;
                            previousGameState = GAME_STATE_POKEBALL_THROW;
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
                        if (battleState.result == BATTLE_RESULT_PLAYER_WIN) {
                            currentGameState = GAME_STATE_BATTLE_WIN;
                        }
                        if (battleState.result == BATTLE_RESULT_PLAYER_LOSE) {
                            currentGameState = GAME_STATE_BATTLE_LOSE;
                        }
                        syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                        battleUi = BATTLE_UI_MENU;
                        battleCursor = 0;
                    }
                } else if (battleUi == BATTLE_UI_BAG_MENU) {
                    play_sfx(plink_audio, plink_audio_len);
                    if (battleCursor == 0) {
                        if (bagHpVisibleCount(&playerBag) <= 0) {
                            battleUiSetSingleMessage(&battleState, "No HP items!");
                            actionTextReturnUi = battleUi;
                            actionTextReturnCursor = battleCursor;
                            currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            actionTextAwaitSpaceRelease = true;
                        } else {
                            battleUi = BATTLE_UI_BAG_HP_LIST;
                            battleCursor = 0;
                            bagHpPage = 0;
                        }
                    } else if (battleCursor == 1) {
                        if (bagBallVisibleCount(&playerBag) <= 0) {
                            battleUiSetSingleMessage(&battleState, "No Poke Balls!");
                            actionTextReturnUi = battleUi;
                            actionTextReturnCursor = battleCursor;
                            currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            actionTextAwaitSpaceRelease = true;
                        } else {
                            battleUi = BATTLE_UI_BAG_BALL_LIST;
                            battleCursor = 0;
                            bagBallPage = 0;
                        }
                    } else {
                        if (playerBag.lastUsedItem != ITEM_NONE) {
                            const ItemId last = playerBag.lastUsedItem;
                            if (itemIsBall(last)) {
                                if (battleState.type != BATTLE_WILD) {
                                    battleUiSetSingleMessage(&battleState, "You can't catch a trainer's Pokemon!");
                                    actionTextReturnUi = battleUi;
                                    actionTextReturnCursor = battleCursor;
                                    actionTextReturnGameState = activeBattleMenuState;
                                    currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                    previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                    actionTextAwaitSpaceRelease = true;
                                    break;
                                }
                                if (bagCount(&playerBag, last) <= 0) {
                                    char buf[96];
                                    snprintf(buf, sizeof(buf), "You're out of %s!", itemName(last));
                                    battleUiSetSingleMessage(&battleState, buf);
                                    actionTextReturnUi = battleUi;
                                    actionTextReturnCursor = battleCursor;
                                    actionTextReturnGameState = activeBattleMenuState;
                                    currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                    previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                    actionTextAwaitSpaceRelease = true;
                                } else {
                                    (void)bagRemove(&playerBag, last, 1);
                                    pokeballCatchItem = last;
                                    pokeballCatchInit = false;
                                    currentGameState = GAME_STATE_POKEBALL_CATCH;
                                    previousGameState = GAME_STATE_POKEBALL_CATCH;
                                    battleUi = BATTLE_UI_MENU;
                                    battleCursor = 0;
                                }
                            } else {
                                battleApplyPlayerAction(&battleState, ACTION_ITEM, (int)last);
                                syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                                battleUi = BATTLE_UI_MENU;
                                battleCursor = 0;
                            }
                        } else {
                            battleUiSetSingleMessage(&battleState, "No last item yet!");
                            actionTextReturnUi = battleUi;
                            actionTextReturnCursor = battleCursor;
                            currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                            actionTextAwaitSpaceRelease = true;
                        }
                    }
                } else if (battleUi == BATTLE_UI_BAG_HP_LIST || battleUi == BATTLE_UI_BAG_BALL_LIST) {
                    const int *pagePtr = (battleUi == BATTLE_UI_BAG_HP_LIST) ? &bagHpPage : &bagBallPage;
                    const int itemCount = (battleUi == BATTLE_UI_BAG_HP_LIST) ? bagHpVisibleCount(&playerBag) : bagBallVisibleCount(&playerBag);
                    const int itemIndex = (*pagePtr) * 4 + battleCursor;

                    play_sfx(plink_audio, plink_audio_len);

                    if (itemIndex < 0 || itemIndex >= itemCount) {
                        battleUiSetSingleMessage(&battleState, "Empty slot!");
                        actionTextReturnUi = battleUi;
                        actionTextReturnCursor = battleCursor;
                        currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                        previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                        actionTextAwaitSpaceRelease = true;
                    } else {
                        bagDescReturnUi = battleUi;
                        bagDescReturnCursor = battleCursor;
                        bagDescReturnPage = *pagePtr;
                        bagDescItem = (battleUi == BATTLE_UI_BAG_HP_LIST) ? bagHpVisibleAt(&playerBag, itemIndex) : bagBallVisibleAt(&playerBag, itemIndex);
                        battleUi = BATTLE_UI_BAG_ITEM_DESC;
                        battleCursor = 0;
                    }
                } else if (battleUi == BATTLE_UI_BAG_ITEM_DESC) {
                    play_sfx(plink_audio, plink_audio_len);

                    if (bagDescItem != ITEM_NONE) {
                        const ItemId item = bagDescItem;
                        if (itemIsBall(item)) {
                            if (battleState.type != BATTLE_WILD) {
                                battleUiSetSingleMessage(&battleState, "You can't catch a trainer's Pokemon!");
                                actionTextReturnUi = bagDescReturnUi;
                                actionTextReturnCursor = bagDescReturnCursor;
                                actionTextReturnGameState = activeBattleMenuState;
                                currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                actionTextAwaitSpaceRelease = true;
                            } else
                            if (bagCount(&playerBag, item) <= 0) {
                                char buf[96];
                                snprintf(buf, sizeof(buf), "You're out of %s!", itemName(item));
                                battleUiSetSingleMessage(&battleState, buf);
                                actionTextReturnUi = bagDescReturnUi;
                                actionTextReturnCursor = bagDescReturnCursor;
                                actionTextReturnGameState = activeBattleMenuState;
                                currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                actionTextAwaitSpaceRelease = true;
                            } else {
                                (void)bagRemove(&playerBag, item, 1);
                                pokeballCatchItem = item;
                                pokeballCatchInit = false;
                                currentGameState = GAME_STATE_POKEBALL_CATCH;
                                previousGameState = GAME_STATE_POKEBALL_CATCH;
                                battleUi = BATTLE_UI_MENU;
                                battleCursor = 0;
                            }
                        } else {
                            battleApplyPlayerAction(&battleState, ACTION_ITEM, (int)item);
                            syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                            battleUi = BATTLE_UI_MENU;
                            battleCursor = 0;
                        }
                    } else {
                        battleUi = bagDescReturnUi;
                        battleCursor = bagDescReturnCursor;
                        if (bagDescReturnUi == BATTLE_UI_BAG_HP_LIST) {
                            bagHpPage = bagDescReturnPage;
                        } else if (bagDescReturnUi == BATTLE_UI_BAG_BALL_LIST) {
                            bagBallPage = bagDescReturnPage;
                        }
                    }
                }

                if (isBattleMenuState(currentGameState) && battleState.messageCount > 0) {
                    actionTextReturnUi = battleUi;
                    actionTextReturnCursor = battleCursor;
                    actionTextReturnGameState = activeBattleMenuState;
                    currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    actionTextAwaitSpaceRelease = true;
                } else if (isBattleMenuState(currentGameState) && battleState.result != BATTLE_RESULT_ONGOING) {
                    currentGameState = GAME_STATE_MAP;
                }
            }

            if (didMoveBattleCursor) {
                play_sfx(plink_audio, plink_audio_len);
            }

            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            shakeTimer++;
            if (shakeTimer >= shakeSpeedFrames) {
                shakeTimer = 0;
                shakeFrame = (shakeFrame + 1) % SHAKE_SPRITE_FRAME_COUNT;
            }
            if (playerHitShakeFrame >= 0) {
                playerHitShakeFrame++;
                if (playerHitShakeFrame >= SHAKE_SPRITE_FRAME_COUNT) playerHitShakeFrame = -1;
            }
            if (enemyHitShakeFrame >= 0) {
                enemyHitShakeFrame++;
                if (enemyHitShakeFrame >= SHAKE_SPRITE_FRAME_COUNT) enemyHitShakeFrame = -1;
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


            // Battle base layer
            draw_map();
 
            //shake effct for getting hit for your pokemon or opponents pokemon
            if (playerHitShakeFrame >= 0) {
                draw_sprite_any(pokemonAreaBack,
                                POKEMON_AREA_BACK_WIDTH,
                                POKEMON_AREA_BACK_HEIGHT,
                                AREA_BACK_X, AREA_BACK_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any_shake(playerBackSprite.pixels,
                                      playerBackSprite.width, playerBackSprite.height,
                                      playerBackSprite.x, playerBackSprite.y,
                                      TRANSPARENT_COLOUR,
                                      playerHitShakeFrame);
            } else {
                draw_sprite_any(pokemonAreaBack,
                                POKEMON_AREA_BACK_WIDTH,
                                POKEMON_AREA_BACK_HEIGHT,
                                AREA_BACK_X, AREA_BACK_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any_bob(playerBackSprite.pixels,
                                    playerBackSprite.width, playerBackSprite.height,
                                    playerBackSprite.x, playerBackSprite.y,
                                    TRANSPARENT_COLOUR,
                                    bobFrame);
            }

            if (enemyHitShakeFrame >= 0) {
                draw_sprite_any(pokemonAreaFront,
                                POKEMON_AREA_FRONT_WIDTH,
                                POKEMON_AREA_FRONT_HEIGHT,
                                AREA_FRONT_X, AREA_FRONT_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any_shake(enemyFrontSprite.pixels,
                                      enemyFrontSprite.width, enemyFrontSprite.height,
                                      enemyFrontSprite.x, enemyFrontSprite.y,
                                      TRANSPARENT_COLOUR,
                                      enemyHitShakeFrame);
            } else {
                draw_sprite_any(pokemonAreaFront,
                                POKEMON_AREA_FRONT_WIDTH,
                                POKEMON_AREA_FRONT_HEIGHT,
                                AREA_FRONT_X, AREA_FRONT_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any(enemyFrontSprite.pixels,
                                enemyFrontSprite.width, enemyFrontSprite.height,
                                enemyFrontSprite.x, enemyFrontSprite.y,
                                TRANSPARENT_COLOUR);
            }

            // Draw the battle UI background before do it wont cover HP .
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);

            pokemonInBattle *enemyActive = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
            pokemonInBattle *playerActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;

            const int enemyHp = (enemyActive != NULL) ? enemyActive->scaledStatsWithLevel[0] : 0;
            const int enemyMaxHp = (enemyActive != NULL) ? enemyActive->maxHp : 1;
            const int playerHp = (playerActive != NULL) ? playerActive->scaledStatsWithLevel[0] : 0;
            const int playerMaxHp = (playerActive != NULL) ? playerActive->maxHp : 1;
            const bool playerFainted = (playerActive != NULL) && (!playerActive->alive || playerHp <= 0);

            if (playerFainted) {
                draw_sprite_any_bob_greyscale(myHpEmpty,
                                              MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                              MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                              TRANSPARENT_COLOUR,
                                              bobFrame);
            } else {
                draw_sprite_any_bob(myHpEmpty,
                                    MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                    MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                    TRANSPARENT_COLOUR,
                                    bobFrame);
            }

            draw_sprite_any(oppHpEmpty, OPP_HP_EMPTY_WIDTH, OPP_HP_EMPTY_HEIGHT, OPP_HP_EMPTY_X, OPP_HP_EMPTY_Y, TRANSPARENT_COLOUR);
            //todo: make the top and bottom lines of hp a darker shade to look better

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
            //draw_sprite_any(poison, POISON_WIDTH, POISON_HEIGHT, MYSTATUS_X, MYSTATUS_Y + offsetY, TRANSPARENT_COLOUR);
           

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
                    const pokemonInBattle *slotPokemon = (battleState.playerParty != NULL) ? battleState.playerParty->slots[i] : NULL;
                    const int slotHp = (slotPokemon != NULL) ? slotPokemon->scaledStatsWithLevel[0] : 0;
                    const bool slotFainted = (slotPokemon != NULL) && (!slotPokemon->alive || slotHp <= 0);

                    if (i == selectedPartyIndex) {
                        if (slotFainted) {
                            draw_sprite_any_bob_party_greyscale(
                                partyBoxSprites[i].pixels,
                                partyBoxSprites[i].width,
                                partyBoxSprites[i].height,
                                partyBoxSprites[i].x,
                                partyBoxSprites[i].y,
                                TRANSPARENT_COLOUR,
                                bobPartyFrame
                            );
                        } else {
                            draw_sprite_any_bob_party(
                                partyBoxSprites[i].pixels,
                                partyBoxSprites[i].width,
                                partyBoxSprites[i].height,
                                partyBoxSprites[i].x,
                                partyBoxSprites[i].y,
                                TRANSPARENT_COLOUR,
                                bobPartyFrame
                            );
                        }
                    } else {
                        if (slotFainted) {
                            draw_sprite_any_greyscale(
                                partyBoxSprites[i].pixels,
                                partyBoxSprites[i].width,
                                partyBoxSprites[i].height,
                                partyBoxSprites[i].x,
                                partyBoxSprites[i].y,
                                TRANSPARENT_COLOUR
                            );
                        } else {
                            drawStaticSprite(&partyBoxSprites[i]);
                        }
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
                const int nameBoxDx = 10;
                const int nameBoxDy = 11;

                for (int i = 0; i < 4; i++) {
                    const AttackData *move = (playerActive != NULL) ? playerActive->attacks[i] : NULL;
                    const char *moveName = (move != NULL && move->name != NULL) ? move->name : "";

                    AttackTypeSpriteRef moveTypeSprite = (move != NULL) ? attackTypeSpriteFor(move->type)
                                                                       : (AttackTypeSpriteRef){normalTypeSprite, NORMAL_TYPE_WIDTH, NORMAL_TYPE_HEIGHT };

                    //draw_rect(mxs[i], mys[i], moveTypeSprite.width, moveTypeSprite.height, WHITE);

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

            if (battleUi == BATTLE_UI_BAG_MENU) {
                const int itemY = battleBackdropY + 2;
                const int itemXLeft = 12;
                const int itemXRight = SCREEN_WIDTH - POKEBALLS_WIDTH - 12;

                if (battleCursor == 0) {
                    draw_sprite_any_shade_pulse(hp, BATTLE_ITEM_HP_WIDTH, BATTLE_ITEM_HP_HEIGHT, itemXLeft, itemY, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(hp, BATTLE_ITEM_HP_WIDTH, BATTLE_ITEM_HP_HEIGHT, itemXLeft, itemY, TRANSPARENT_COLOUR);
                }

                if (battleCursor == 1) {
                    draw_sprite_any_shade_pulse(pokeballs, POKEBALLS_WIDTH, POKEBALLS_HEIGHT, itemXRight, itemY, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(pokeballs, POKEBALLS_WIDTH, POKEBALLS_HEIGHT, itemXRight, itemY, TRANSPARENT_COLOUR);
                }
                if (battleCursor == 2) {
                    draw_sprite_any_shade_pulse(useLastItem, BATTLE_BAG_USE_LAST_ITEM_WIDTH, BATTLE_BAG_USE_LAST_ITEM_HEIGHT,
                                                USE_LAST_ICON_X, USE_LAST_ICON_Y, TRANSPARENT_COLOUR, shadePulseFrame);
                } else {
                    draw_sprite_any(useLastItem, BATTLE_BAG_USE_LAST_ITEM_WIDTH, BATTLE_BAG_USE_LAST_ITEM_HEIGHT,
                                            USE_LAST_ICON_X, USE_LAST_ICON_Y, TRANSPARENT_COLOUR);
                }

                if (playerBag.lastUsedItem != ITEM_NONE) {
                    const ItemId item = playerBag.lastUsedItem;
                    const unsigned short *icon = NULL;
                    int iconW = 0;
                    int iconH = 0;

                    if (item == ITEM_POKEBALL) { icon = pokeballIcon_poke; iconW = POKEBALL_ICON_WIDTH; iconH = POKEBALL_ICON_HEIGHT; }
                    else if (item == ITEM_GREAT_BALL) { icon = pokeballIcon_great; iconW = POKEBALL_ICON_WIDTH; iconH = POKEBALL_ICON_HEIGHT; }
                    else if (item == ITEM_ULTRA_BALL) { icon = pokeballIcon_ultra; iconW = POKEBALL_ICON_WIDTH; iconH = POKEBALL_ICON_HEIGHT; }
                    else if (item == ITEM_PREMIER_BALL) { icon = pokeballIcon_premier; iconW = POKEBALL_ICON_WIDTH; iconH = POKEBALL_ICON_HEIGHT; }
                    else if (item == ITEM_MASTER_BALL) { icon = pokeballIcon_master; iconW = POKEBALL_ICON_WIDTH; iconH = POKEBALL_ICON_HEIGHT; }
                    else if (item == ITEM_POTION) { icon = healingItemIcon_potion; iconW = HEALING_ITEM_ICON_WIDTH; iconH = HEALING_ITEM_ICON_HEIGHT; }
                    else if (item == ITEM_SUPER_POTION) { icon = healingItemIcon_superPotion; iconW = HEALING_ITEM_ICON_WIDTH; iconH = HEALING_ITEM_ICON_HEIGHT; }
                    else if (item == ITEM_HYPER_POTION) { icon = healingItemIcon_hyperPotion; iconW = HEALING_ITEM_ICON_WIDTH; iconH = HEALING_ITEM_ICON_HEIGHT; }
                    else if (item == ITEM_FULL_RESTORE) { icon = healingItemIcon_fullRestore; iconW = HEALING_ITEM_ICON_WIDTH; iconH = HEALING_ITEM_ICON_HEIGHT; }

                    if (icon != NULL) {
                        draw_sprite_any(icon,
                                        iconW,
                                        iconH,
                                        USE_LAST_ICON_X + LAST_USED_ITEM_X,
                                        USE_LAST_ICON_Y + LAST_USED_ITEM_Y,
                                        TRANSPARENT_COLOUR);
                    }
                }
            }

            if (battleUi == BATTLE_UI_BAG_HP_LIST || battleUi == BATTLE_UI_BAG_BALL_LIST) {
                const int mxLeft = 18;
                const int mxRight = 160 + 18;
                const int myTop = 240 - 90;
                const int myBottom = 240 - 45;
                const int mxs[4] = { mxLeft, mxRight, mxLeft, mxRight };
                const int mys[4] = { myTop,  myTop,   myBottom, myBottom };

                const int itemCount = (battleUi == BATTLE_UI_BAG_HP_LIST) ? bagHpVisibleCount(&playerBag) : bagBallVisibleCount(&playerBag);
                const int page = (battleUi == BATTLE_UI_BAG_HP_LIST) ? bagHpPage : bagBallPage;
                int pageCount = (itemCount + 3) / 4;
                if (pageCount < 1) pageCount = 1;

                // Draw 4 slots (like the attack menu grid).
                for (int i = 0; i < 4; i++) {
                    if (battleCursor == i) {
                        draw_sprite_any_shade_pulse(itemSlot, BATTLE_BAG_ITEM_SLOT_WIDTH, BATTLE_BAG_ITEM_SLOT_HEIGHT,
                                                    mxs[i], mys[i], TRANSPARENT_COLOUR, shadePulseFrame);
                    } else {
                        draw_sprite_any(itemSlot, BATTLE_BAG_ITEM_SLOT_WIDTH, BATTLE_BAG_ITEM_SLOT_HEIGHT,
                                        mxs[i], mys[i], TRANSPARENT_COLOUR);
                    }

                    const int visibleIndex = page * 4 + i;
                    if (visibleIndex >= 0 && visibleIndex < itemCount) {
                        const ItemId item = (battleUi == BATTLE_UI_BAG_HP_LIST)
                                                ? bagHpVisibleAt(&playerBag, visibleIndex)
                                                : bagBallVisibleAt(&playerBag, visibleIndex);
                        if (item != ITEM_NONE) {
                            draw_string_f(mxs[i] + ITEM_NAME_X, mys[i] + ITEM_NAME_Y, itemName(item), WHITE, FONT_5X9);

                            const int count = bagCount(&playerBag, item);
                            char amountBuf[8];
                            snprintf(amountBuf, sizeof(amountBuf), "x%d", (count < 0) ? 0 : count);
                            draw_string_f(mxs[i] + ITEM_AMOUNT_X, mys[i] + ITEM_AMOUNT_Y, amountBuf, WHITE, FONT_5X9);

                            if (battleUi == BATTLE_UI_BAG_BALL_LIST) {
                                const unsigned short *icon = NULL;
                                if (item == ITEM_POKEBALL) icon = pokeballIcon_poke;
                                else if (item == ITEM_GREAT_BALL) icon = pokeballIcon_great;
                                else if (item == ITEM_ULTRA_BALL) icon = pokeballIcon_ultra;
                                else if (item == ITEM_PREMIER_BALL) icon = pokeballIcon_premier;
                                else if (item == ITEM_MASTER_BALL) icon = pokeballIcon_master;

                                if (icon != NULL) {
                                    draw_sprite_any(icon,
                                                    POKEBALL_ICON_WIDTH,
                                                    POKEBALL_ICON_HEIGHT,
                                                    mxs[i] + ITEM_X,
                                                    mys[i] + ITEM_Y,
                                                    TRANSPARENT_COLOUR);
                                }
                            } else if (battleUi == BATTLE_UI_BAG_HP_LIST) {
                                const unsigned short *icon = NULL;
                                if (item == ITEM_POTION) icon = healingItemIcon_potion;
                                else if (item == ITEM_SUPER_POTION) icon = healingItemIcon_superPotion;
                                else if (item == ITEM_HYPER_POTION) icon = healingItemIcon_hyperPotion;
                                else if (item == ITEM_FULL_RESTORE) icon = healingItemIcon_fullRestore;

                                if (icon != NULL) {
                                    draw_sprite_any(icon,
                                                    HEALING_ITEM_ICON_WIDTH,
                                                    HEALING_ITEM_ICON_HEIGHT,
                                                    mxs[i] + RESTORE_ITEM_X,
                                                    mys[i] + RESTORE_ITEM_Y,
                                                    TRANSPARENT_COLOUR);
                                }
                            }
                        }
                    }
                }

                char pageBuf[24];
                snprintf(pageBuf, sizeof(pageBuf), "Page %d/%d", page + 1, pageCount);
                draw_string_f(8, myTop - 10, pageBuf, BLACK, FONT_5X9);
            }

            if (battleUi == BATTLE_UI_BAG_ITEM_DESC) {
                const int descX = (SCREEN_WIDTH - BATTLE_BAG_ITEM_DESCRIPTION_WIDTH) / 2;
                const int descY = battleBackdropY + (BATTLE_UI_BACKGROUND_HEIGHT - BATTLE_BAG_ITEM_DESCRIPTION_HEIGHT) / 2;
                draw_sprite_any(itemDescription, BATTLE_BAG_ITEM_DESCRIPTION_WIDTH, BATTLE_BAG_ITEM_DESCRIPTION_HEIGHT,
                                descX, descY, TRANSPARENT_COLOUR);

                if (bagDescItem != ITEM_NONE) {
                    const unsigned short *itemIcon = NULL;
                    int itemIconW = 0;
                    int itemIconH = 0;
                    if (bagDescItem == ITEM_POKEBALL) itemIcon = pokeballIcon_poke;
                    else if (bagDescItem == ITEM_GREAT_BALL) itemIcon = pokeballIcon_great;
                    else if (bagDescItem == ITEM_ULTRA_BALL) itemIcon = pokeballIcon_ultra;
                    else if (bagDescItem == ITEM_PREMIER_BALL) itemIcon = pokeballIcon_premier;
                    else if (bagDescItem == ITEM_MASTER_BALL) itemIcon = pokeballIcon_master;
                    if (itemIcon != NULL) {
                        itemIconW = POKEBALL_ICON_WIDTH;
                        itemIconH = POKEBALL_ICON_HEIGHT;
                    } else {
                        if (bagDescItem == ITEM_POTION) itemIcon = healingItemIcon_potion;
                        else if (bagDescItem == ITEM_SUPER_POTION) itemIcon = healingItemIcon_superPotion;
                        else if (bagDescItem == ITEM_HYPER_POTION) itemIcon = healingItemIcon_hyperPotion;
                        else if (bagDescItem == ITEM_FULL_RESTORE) itemIcon = healingItemIcon_fullRestore;
                        if (itemIcon != NULL) {
                            itemIconW = HEALING_ITEM_ICON_WIDTH;
                            itemIconH = HEALING_ITEM_ICON_HEIGHT;
                        }
                    }

                    if (itemIcon != NULL) {
                        draw_sprite_any(itemIcon,
                                        itemIconW,
                                        itemIconH,
                                        descX + DEFINITION_X,
                                        descY + DEFINITION_Y,
                                        TRANSPARENT_COLOUR);
                    }
                    draw_string_f(descX + DEFINITION_X_NAME, descY + DEFINITION_Y_NAME, itemName(bagDescItem), WHITE, FONT_5X9);
                    draw_wrapped_5x9(descX + DEFINITION_X_TEXT, descY + DEFINITION_Y_TEXT, 112, 3, getItemDescription(bagDescItem), WHITE);
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
            if (playerHitShakeFrame >= 0) {
                playerHitShakeFrame++;
                if (playerHitShakeFrame >= SHAKE_SPRITE_FRAME_COUNT) playerHitShakeFrame = -1;
            }
            if (enemyHitShakeFrame >= 0) {
                enemyHitShakeFrame++;
                if (enemyHitShakeFrame >= SHAKE_SPRITE_FRAME_COUNT) enemyHitShakeFrame = -1;
            }
            bobTimer++;
            if (bobTimer >= bobSpeedFrames) {
                bobTimer = 0;
                bobFrame = (bobFrame + 1) % BOB_SPRITE_FRAME_COUNT;
            }

            // between attacks pause
            if (actionTextInterAttackPauseTimer > 0) {
                actionTextInterAttackPauseTimer--;

                draw_map();
                draw_sprite_any(pokemonAreaBack,
                                POKEMON_AREA_BACK_WIDTH,
                                POKEMON_AREA_BACK_HEIGHT,
                                AREA_BACK_X, AREA_BACK_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any(pokemonAreaFront,
                                POKEMON_AREA_FRONT_WIDTH,
                                POKEMON_AREA_FRONT_HEIGHT,
                                AREA_FRONT_X, AREA_FRONT_Y,
                                TRANSPARENT_COLOUR);

                if (playerHitShakeFrame >= 0) {
                    draw_sprite_any_shake(playerBackSprite.pixels,
                                          playerBackSprite.width, playerBackSprite.height,
                                          playerBackSprite.x, playerBackSprite.y,
                                          TRANSPARENT_COLOUR,
                                          playerHitShakeFrame);
                } else {
                    draw_sprite_any_bob(playerBackSprite.pixels,
                                        playerBackSprite.width, playerBackSprite.height,
                                        playerBackSprite.x, playerBackSprite.y,
                                        TRANSPARENT_COLOUR,
                                        bobFrame);
                }

                if (enemyHitShakeFrame >= 0) {
                    draw_sprite_any_shake(enemyFrontSprite.pixels,
                                          enemyFrontSprite.width, enemyFrontSprite.height,
                                          enemyFrontSprite.x, enemyFrontSprite.y,
                                          TRANSPARENT_COLOUR,
                                          enemyHitShakeFrame);
                } else {
                    draw_sprite_any(enemyFrontSprite.pixels,
                                    enemyFrontSprite.width, enemyFrontSprite.height,
                                    enemyFrontSprite.x, enemyFrontSprite.y,
                                    TRANSPARENT_COLOUR);
                }

                draw_sprite_any(battleUIBackgroundSprite,
                                BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT,
                                0, battleBackdropY,
                                TRANSPARENT_COLOUR);

                // Keep HP UI visible during the pause
                {
                    pokemonInBattle *enemyActive = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
                    pokemonInBattle *playerActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;

                    const int enemyHp = (enemyActive != NULL) ? enemyActive->scaledStatsWithLevel[0] : 0;
                    const int enemyMaxHp = (enemyActive != NULL) ? enemyActive->maxHp : 1;
                    const int playerHp = (playerActive != NULL) ? playerActive->scaledStatsWithLevel[0] : 0;
                    const int playerMaxHp = (playerActive != NULL) ? playerActive->maxHp : 1;
                    const bool playerFainted = (playerActive != NULL) && (!playerActive->alive || playerHp <= 0);

                    if (playerFainted) {
                        draw_sprite_any_bob_greyscale(myHpEmpty,
                                                      MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                                      MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                                      TRANSPARENT_COLOUR,
                                                      bobFrame);
                    } else {
                        draw_sprite_any_bob(myHpEmpty,
                                            MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                            MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                            TRANSPARENT_COLOUR,
                                            bobFrame);
                    }
                    draw_sprite_any(oppHpEmpty, OPP_HP_EMPTY_WIDTH, OPP_HP_EMPTY_HEIGHT, OPP_HP_EMPTY_X, OPP_HP_EMPTY_Y, TRANSPARENT_COLOUR);

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

                    draw_rect(OPP_HP_EMPTY_X + 50, OPP_HP_EMPTY_Y + 20,
                              (enemyHpBarWidth < 0) ? 0 : ((enemyHpBarWidth > HP_WIDTH) ? HP_WIDTH : enemyHpBarWidth),
                              HP_HEIGHT,
                              enemyHpBarColour);
                    draw_string_f(oppLVL_X, oppLVL_Y, oppLvlBuf, BLACK, 1);
                    //draw_sprite_any(burned, BURNED_WIDTH, BURNED_HEIGHT, STATUS_X, STATUS_Y, TRANSPARENT_COLOUR);
                    //draw_sprite_any(caught, CAUGHT_WIDTH, CAUGHT_HEIGHT, CAUGHT_X, CAUGHT_Y, TRANSPARENT_COLOUR);
                    draw_string_f(OPPNAME_X, OPPNAME_Y, (enemyActive != NULL && enemyActive->id.data != NULL) ? enemyActive->id.data->name : "???", BLACK, 1);

                    static const signed char dy_pattern[BOB_SPRITE_FRAME_COUNT] = {
                    0, -1, -1, 0, 0, 1, 1, 0,
                    0, -1, -1, 0, 0, 1, 1, 0 };
                    int offsetY = dy_pattern[bobFrame];
                    draw_rect(myHP_X, myHP_Y + offsetY,
                              (playerHpBarWidth < 0) ? 0 : ((playerHpBarWidth > HP_WIDTH) ? HP_WIDTH : playerHpBarWidth),
                              HP_HEIGHT,
                              playerHpBarColour);
                    draw_rect(EXP_X, EXP_Y + offsetY, EXP_WIDTH, EXP_HEIGHT, TURQ);
                    draw_string_f(myLVL_X, myLVL_Y + offsetY, myLvlBuf, BLACK, 1);
                    draw_string_f(MYNAME_X, MYNAME_Y + offsetY, (playerActive != NULL && playerActive->id.data != NULL) ? playerActive->id.data->name : "???", BLACK, 1);
                    draw_string_f(TOTAL_HPNUM_X, HPNUM_Y + offsetY, myHpCurBuf, BLACK, 1);
                    draw_string_f(REMAINING_HP_X, HPNUM_Y + offsetY, myHpMaxBuf, BLACK, 1);
                    draw_sprite_any(poison, POISON_WIDTH, POISON_HEIGHT, MYSTATUS_X, MYSTATUS_Y + offsetY, TRANSPARENT_COLOUR);
                }
                break;
            }

            // Draw the normal battle base.
            draw_map();

            if (playerHitShakeFrame >= 0) {
                draw_sprite_any(pokemonAreaBack,
                                POKEMON_AREA_BACK_WIDTH,
                                POKEMON_AREA_BACK_HEIGHT,
                                AREA_BACK_X, AREA_BACK_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any_shake(playerBackSprite.pixels,
                                      playerBackSprite.width, playerBackSprite.height,
                                      playerBackSprite.x, playerBackSprite.y,
                                      TRANSPARENT_COLOUR,
                                      playerHitShakeFrame);
            } else {
                draw_sprite_any(pokemonAreaBack,
                                POKEMON_AREA_BACK_WIDTH,
                                POKEMON_AREA_BACK_HEIGHT,
                                AREA_BACK_X, AREA_BACK_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any_bob(playerBackSprite.pixels,
                                    playerBackSprite.width, playerBackSprite.height,
                                    playerBackSprite.x, playerBackSprite.y,
                                    TRANSPARENT_COLOUR,
                                    bobFrame);
            }

            if (enemyHitShakeFrame >= 0) {
                draw_sprite_any(pokemonAreaFront,
                                POKEMON_AREA_FRONT_WIDTH,
                                POKEMON_AREA_FRONT_HEIGHT,
                                AREA_FRONT_X, AREA_FRONT_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any_shake(enemyFrontSprite.pixels,
                                      enemyFrontSprite.width, enemyFrontSprite.height,
                                      enemyFrontSprite.x, enemyFrontSprite.y,
                                      TRANSPARENT_COLOUR,
                                      enemyHitShakeFrame);
            } else {
                draw_sprite_any(pokemonAreaFront,
                                POKEMON_AREA_FRONT_WIDTH,
                                POKEMON_AREA_FRONT_HEIGHT,
                                AREA_FRONT_X, AREA_FRONT_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any(enemyFrontSprite.pixels,
                                enemyFrontSprite.width, enemyFrontSprite.height,
                                enemyFrontSprite.x, enemyFrontSprite.y,
                                TRANSPARENT_COLOUR);
            }
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);

            pokemonInBattle *enemyActive = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
            pokemonInBattle *playerActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;

            const int enemyHp = (enemyActive != NULL) ? enemyActive->scaledStatsWithLevel[0] : 0;
            const int enemyMaxHp = (enemyActive != NULL) ? enemyActive->maxHp : 1;
            const int playerHp = (playerActive != NULL) ? playerActive->scaledStatsWithLevel[0] : 0;
            const int playerMaxHp = (playerActive != NULL) ? playerActive->maxHp : 1;
            const bool playerFainted = (playerActive != NULL) && (!playerActive->alive || playerHp <= 0);
            const int playerCurrentExp = (playerActive != NULL) ? playerActive->exp : 0;
            const int playerNeededExp = (playerActive != NULL) ? expRequiredAtLevel(playerActive->level) : 0;

            
            if (playerFainted) {
                draw_sprite_any_bob_greyscale(myHpEmpty,
                                              MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                              MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                              TRANSPARENT_COLOUR,
                                              bobFrame);
            } else {
                draw_sprite_any_bob(myHpEmpty,
                                    MY_HP_EMPTY_WIDTH, MY_HP_EMPTY_HEIGHT,
                                    MY_HP_EMPTY_X, MY_HP_EMPTY_Y,
                                    TRANSPARENT_COLOUR,
                                    bobFrame);
            }
            draw_sprite_any(oppHpEmpty, OPP_HP_EMPTY_WIDTH, OPP_HP_EMPTY_HEIGHT, OPP_HP_EMPTY_X, OPP_HP_EMPTY_Y, TRANSPARENT_COLOUR);

            const int enemyHpBarWidth = (enemyMaxHp > 0) ? (HP_WIDTH * enemyHp) / enemyMaxHp : 0;
            const int playerHpBarWidth = (playerMaxHp > 0) ? (HP_WIDTH * playerHp) / playerMaxHp : 0;

            const int playerExpBarWidth = (playerNeededExp > 0) ? (EXP_WIDTH * playerCurrentExp) / playerNeededExp : 0;

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
            draw_rect(EXP_X, EXP_Y + offsetY, (playerExpBarWidth < 0) ? 0 : ((playerExpBarWidth > HP_WIDTH) ? EXP_WIDTH : playerExpBarWidth), EXP_HEIGHT, TURQ);
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

            // Attack messages auto advance
            if (battleState.messageCount <= 0) {
                actionTextAutoTimer = 0;
                actionTextLastMsgIndex = -1;
            } else if (battleState.messageReadIndex != actionTextLastMsgIndex) {
                actionTextAutoTimer = 0;
                actionTextLastMsgIndex = battleState.messageReadIndex;

                if (msg != NULL && strstr(msg, " used ") != NULL) {
                    const int idx = battleState.messageReadIndex;
                    //check flag for damage dealt
                    const bool didDamage = (idx >= 0 && idx < BATTLE_MSG_MAX) ? (battleState.messageFlags[idx] != 0) : false;

                    if (didDamage) {
                        play_sfx(hit_normal_audio, hit_normal_audio_len);
                        // checks string to see who attacked
                        if (strncmp(msg, "Opposing ", 9) == 0) {
                            playerHitShakeFrame = 0;
                        } else {
                            enemyHitShakeFrame = 0;
                        }
                    }
                }
            }

            const bool manual = isManualBattleMessage(msg);
            bool shouldAdvance = false;

        
            if (battleState.messageCount > 0) {
                const int delayFrames = manual ? 30 : 15;
                actionTextAutoTimer++;
                if (!actionTextAwaitSpaceRelease && (spacePressed || actionTextAutoTimer >= delayFrames)) {
                    shouldAdvance = true;
                }
            }

            if (shouldAdvance) {
                actionTextAutoTimer = 0;
                battleState.messageReadIndex++;
                // treat the new index as new by forcing the nextb frame so SFX/shake can trigger for the 2nd pokemon
                actionTextLastMsgIndex = -1;

                //short pause before showing the next message
                if (battleState.messageReadIndex >= 0 && battleState.messageReadIndex < battleState.messageCount) {
                    const char *nextMsg = battleState.messages[battleState.messageReadIndex];
                    if (nextMsg != NULL && strstr(nextMsg, " used ") != NULL) {
                        actionTextInterAttackPauseTimer = 10;
                    }
                }

                if (battleState.messageReadIndex >= battleState.messageCount) {
                    battleState.messageReadIndex = 0;
                    battleState.messageCount = 0;
                    actionTextLastMsgIndex = -1;

                    if (battleState.result != BATTLE_RESULT_ONGOING) {
                        currentGameState = GAME_STATE_MAP;
                    } else {
                        battleUi = actionTextReturnUi;
                        battleCursor = actionTextReturnCursor;
                        currentGameState = actionTextReturnGameState;
                        previousGameState = actionTextReturnGameState;
                    }
                }
            }

            break;
        }

        case GAME_STATE_WILD_BATTLE_TRANSITION: {
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
                // Slide area and  opponent sprite in from off-screen left.
                const int growWPerFrame = 12;
                const int growHPerFrame = 8;
                const int framesToFullW = ((SCREEN_WIDTH / 2) + growWPerFrame - 1) / growWPerFrame;
                const int framesToFullH = ((SCREEN_HEIGHT / 2) + growHPerFrame - 1) / growHPerFrame;
                const int slideFrames = (framesToFullW > framesToFullH) ? framesToFullW : framesToFullH;

                int t = transitionFrame;
                if (t < 0) t = 0;
                if (t > slideFrames) t = slideFrames;

                const int areaStartX = -POKEMON_AREA_FRONT_WIDTH;
                const int areaEndX = AREA_FRONT_X;
                const int areaX = areaStartX + ((areaEndX - areaStartX) * t) / slideFrames;

                const int enemyStartX = -enemyFrontSprite.width;
                const int enemyEndX = enemyFrontSprite.x;
                const int enemyX = enemyStartX + ((enemyEndX - enemyStartX) * t) / slideFrames;

                draw_sprite_any(pokemonAreaFront,
                                POKEMON_AREA_FRONT_WIDTH,
                                POKEMON_AREA_FRONT_HEIGHT,
                                areaX, AREA_FRONT_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any(enemyFrontSprite.pixels,
                                enemyFrontSprite.width, enemyFrontSprite.height,
                                enemyX, enemyFrontSprite.y,
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

                currentGameState = GAME_STATE_WILD_BATTLE_INTRO_TEXT;
                previousGameState = GAME_STATE_WILD_BATTLE_INTRO_TEXT;
                battleIntroTextReady = false;
                transitionFrame = 0;
            }

            break;
        }

        case GAME_STATE_TRAINER_BATTLE_TRANSITION: {
  
            draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

            draw_sprite_any(vsCynthiaSprite,
                            VS_CYNTHIA_WIDTH,
                            VS_CYNTHIA_HEIGHT,
                            (SCREEN_WIDTH - VS_CYNTHIA_WIDTH) / 2,
                            0,
                            TRANSPARENT_COLOUR);
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, "Cynthia wants to battle!", BLACK);

            if (spacePressed) {
                play_sfx(plink_audio, plink_audio_len);
                pokeballThrowShowText = true;
                pokeballThrowAutoAdvance = false;
                pokeballThrowReturnState = activeBattleMenuState;
                currentGameState = GAME_STATE_POKEBALL_THROW;
                previousGameState = GAME_STATE_POKEBALL_THROW;
                battleThrowPokeballTextReady = false;
                pokeballThrowInit = false;
                transitionFrame = 0;
                transitionTimer = 0;
            }

            break;
        }
        
        case GAME_STATE_WILD_BATTLE_INTRO_TEXT: {
            draw_map();
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);

            if (enemyFrontSprite.pixels != NULL) {
                draw_sprite_any(pokemonAreaFront, 
                                POKEMON_AREA_FRONT_WIDTH, 
                                POKEMON_AREA_FRONT_HEIGHT, 
                                AREA_FRONT_X, AREA_FRONT_Y, 
                                TRANSPARENT_COLOUR);
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

            // Instant intro text.
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, battleIntroText, BLACK);
         
            if (spacePressed) {
                play_sfx(plink_audio, plink_audio_len);
                pokeballThrowShowText = true;
                pokeballThrowAutoAdvance = false;
                pokeballThrowReturnState = activeBattleMenuState;
                currentGameState = GAME_STATE_POKEBALL_THROW;
                previousGameState = GAME_STATE_POKEBALL_THROW;
                battleThrowPokeballTextReady = false;
            }
         
            break;
        }

        case GAME_STATE_TRAINER_BATTLE_INTRO_TEXT: {
            // Placeholder trainer intro: black screen + textbox prompt.
            draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            draw_sprite_any(vsCynthiaSprite,
                            VS_CYNTHIA_WIDTH,
                            VS_CYNTHIA_HEIGHT,
                            (SCREEN_WIDTH - VS_CYNTHIA_WIDTH) / 2,
                            0,
                            TRANSPARENT_COLOUR);
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, "Cynthia wants to battle!", BLACK);

            if (spacePressed) {
                play_sfx(plink_audio, plink_audio_len);
                pokeballThrowShowText = true;
                pokeballThrowAutoAdvance = false;
                pokeballThrowReturnState = activeBattleMenuState;
                currentGameState = GAME_STATE_POKEBALL_THROW;
                previousGameState = GAME_STATE_POKEBALL_THROW;
                battleThrowPokeballTextReady = false;
            }

            break;
        }

        case GAME_STATE_POKEBALL_THROW: {
            //  projectile motion variables (for arc throwing)
            if (!pokeballThrowInit) {
                play_sfx(pokeball_audio, pokeball_audio_len);
                syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                pokeballThrowInit = true;
                pokeballThrown = false;
                pokeballLanding = false;
                pokeballThrowT = 0;
                pokeballThrownFrame = 0;
                pokeballThrownTimer = 0;
                pokeballLandingTimer = 0;
                pokeballLandingFrame = POKEBALLTHROW_LAND_FRAME_START;

                // Start off-screen (left + below). We only draw it at/after the peak.
                pokeballX0 = -(float)POKEBALLTHROW_WIDTH;

                const float landX = (float)(playerBackSprite.x + playerBackSprite.width / 2 - POKEBALLTHROW_WIDTH / 2);
                const float landY = (float)(playerBackSprite.y + playerBackSprite.height - (POKEBALLTHROW_HEIGHT / 2)) - 40;
                pokeballLandX = landX;
                pokeballLandY = landY;

                // Peak above the player's side.
                float yPeak = (float)(playerBackSprite.y - 30);
                if (yPeak < 5.0f) yPeak = 5.0f;

                // Ensure the ball is visible on-screen at the peak (we only draw it at/after the peak).
                pokeballPeakX = pokeballLandX - 60.0f;
                if (pokeballPeakX < 0.0f) pokeballPeakX = 0.0f;
                if (pokeballPeakX > (float)(SCREEN_WIDTH - POKEBALLTHROW_WIDTH)) {
                    pokeballPeakX = (float)(SCREEN_WIDTH - POKEBALLTHROW_WIDTH);
                }

                pokeballVy = -(pokeballG * (float)pokeballTPeak);
                pokeballY0 = yPeak + 0.5f * pokeballG * (float)(pokeballTPeak * pokeballTPeak);

                float dy = pokeballLandY - yPeak;
                if (dy < 0.0f) dy = 0.0f;
                pokeballTLand = (float)pokeballTPeak + sqrtf((2.0f * dy) / pokeballG);

                const float tHoriz = (pokeballTLand - (float)pokeballTPeak);
                pokeballVx = (pokeballLandX - pokeballPeakX) / ((tHoriz > 1.0f) ? tHoriz : 1.0f);
                pokeballX0 = pokeballPeakX - pokeballVx * (float)pokeballTPeak;
            }

            if (pokeballThrowShowText && !battleThrowPokeballTextReady) {
                const pokemonInBattle *myActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;
                const char *myPokemonName = (myActive != NULL && myActive->id.data != NULL && myActive->id.data->name != NULL)
                                                ? myActive->id.data->name
                                                : "???";
                snprintf(battleThrowPokeballText, sizeof(battleThrowPokeballText), "Go! %s!", myPokemonName);
                battleThrowPokeballTextReady = true;
            }

            draw_map();

            // Draw sprites under the battle UI background 
            if (pokeballThrown && playerBackSprite.pixels != NULL) {
                draw_sprite_any(pokemonAreaBack,
                                POKEMON_AREA_BACK_WIDTH,
                                POKEMON_AREA_BACK_HEIGHT,
                                AREA_BACK_X, AREA_BACK_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any(playerBackSprite.pixels,
                                playerBackSprite.width, playerBackSprite.height,
                                playerBackSprite.x, playerBackSprite.y,
                                TRANSPARENT_COLOUR);
            }
            if (enemyFrontSprite.pixels != NULL) {
                draw_sprite_any(pokemonAreaFront,
                                POKEMON_AREA_FRONT_WIDTH,
                                POKEMON_AREA_FRONT_HEIGHT,
                                AREA_FRONT_X, AREA_FRONT_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any(enemyFrontSprite.pixels,
                                enemyFrontSprite.width, enemyFrontSprite.height,
                                enemyFrontSprite.x, enemyFrontSprite.y,
                                TRANSPARENT_COLOUR);
            }

            // Update/draw projectile.
            if (!pokeballThrown) {
                // advance time
                const int timeStep = 2; // increasing speed since this uses the same projectile motion formula but further distance
                pokeballThrowT += timeStep;

                const float t = (float)pokeballThrowT;
                const float x = pokeballX0 + pokeballVx * t;
                const float y = pokeballY0 + pokeballVy * t + 0.5f * pokeballG * t * t;

                // Start the landing/open animation once the ball hits the target y.
                if (!pokeballLanding && pokeballThrowT >= pokeballTPeak && y >= pokeballLandY) {
                    pokeballLanding = true;
                    pokeballLandingFrame = POKEBALLTHROW_LAND_FRAME_START;
                    pokeballLandingTimer = 0;
                }

                if (!pokeballLanding) {
                    // frames 0-7 while in flight.
                    const int inflightFrames = POKEBALLTHROW_THROW_FRAME_COUNT;
                    const int inflightSpeed = 1;

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

                    // Advance after drawing so the first visible frame is frame 0.
                    pokeballThrownTimer++;
                    if (pokeballThrownTimer >= inflightSpeed) {
                        pokeballThrownTimer = 0;
                        pokeballThrownFrame = (pokeballThrownFrame + 1) % inflightFrames;
                    }
                }

                if (pokeballLanding) {
                    const int landSpeed = 3;
                    if (pokeballLandingTimer >= landSpeed) {
                        pokeballLandingTimer = 0;
                        pokeballLandingFrame++;
                        if (pokeballLandingFrame >= (POKEBALLTHROW_LAND_FRAME_START + POKEBALLTHROW_LAND_FRAME_COUNT)) {
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

                    pokeballLandingTimer++;
                }
            }

            // UI background covers the sprites
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);

            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, pokeballThrowShowText ? battleThrowPokeballText : "", BLACK);

            if (pokeballThrown) {
                if (pokeballThrowAutoAdvance) {
                    currentGameState = pokeballThrowReturnState;
                    previousGameState = pokeballThrowReturnState;
                    battleThrowPokeballTextReady = false;
                    pokeballThrowInit = false;
                } else if (spacePressed) {
                    play_sfx(plink_audio, plink_audio_len);
                    currentGameState = pokeballThrowReturnState;
                    previousGameState = pokeballThrowReturnState;
                    battleThrowPokeballTextReady = false;
                    pokeballThrowInit = false;
                }
            }
            break;
        }

        case GAME_STATE_POKEBALL_CATCH: {
            // Throw a ball at the opponent and play the cshake frames sequence.
            //use Bézier curve for the ball's path
            if (!pokeballCatchInit) {
                play_sfx(pokeball_audio, pokeball_audio_len);
                pokeballCatchInit = true;
                pokeballCatchLanding = false;
                pokeballCatchEscape = false;
                pokeballCatchT = 0;
                pokeballCatchThrownTimer = 0;
                pokeballCatchLandingTimer = 0;
                pokeballCatchSeqTimer = 0;
                pokeballCatchFrame = 0;
                pokeballCatchLandingFrame = POKEBALLTHROW_LAND_FRAME_START;
                pokeballCatchSeqFrame = POKEBALLTHROW_CATCH_FRAME_START;

                pokemonInBattle *enemyActive = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
                PokeballType ball = POKEBALL_POKE;
                if (pokeballCatchItem == ITEM_GREAT_BALL) ball = POKEBALL_GREAT;
                else if (pokeballCatchItem == ITEM_ULTRA_BALL) ball = POKEBALL_ULTRA;
                else if (pokeballCatchItem == ITEM_PREMIER_BALL) ball = POKEBALL_PREMIER;
                else if (pokeballCatchItem == ITEM_MASTER_BALL) ball = POKEBALL_MASTER;
                else ball = POKEBALL_POKE;

                pokeballCatchSuccess = (enemyActive != NULL) ? attemptCatchWithBall(enemyActive, ball) : false;

                pokeballCatchStartX = (float)(playerBackSprite.x + playerBackSprite.width / 2 - POKEBALLTHROW_WIDTH / 2);
                pokeballCatchStartY = (float)(playerBackSprite.y + playerBackSprite.height / 2 - POKEBALLTHROW_HEIGHT / 2);
                pokeballCatchEndX = (float)(enemyFrontSprite.x + enemyFrontSprite.width / 2 - POKEBALLTHROW_WIDTH / 2);
                pokeballCatchEndY = (float)(enemyFrontSprite.y + enemyFrontSprite.height / 2 - POKEBALLTHROW_HEIGHT / 2 + 8);

                pokeballCatchCtrlX = 0.5f * (pokeballCatchStartX + pokeballCatchEndX);
                pokeballCatchCtrlY = ((pokeballCatchStartY < pokeballCatchEndY) ? pokeballCatchStartY : pokeballCatchEndY) - 70.0f;
                if (pokeballCatchCtrlY < 0.0f) pokeballCatchCtrlY = 0.0f;
            }

            draw_map();

            // Draw base battle sprites.
            if (playerBackSprite.pixels != NULL) {
                draw_sprite_any(pokemonAreaBack,
                                POKEMON_AREA_BACK_WIDTH,
                                POKEMON_AREA_BACK_HEIGHT,
                                AREA_BACK_X, AREA_BACK_Y,
                                TRANSPARENT_COLOUR);
                draw_sprite_any(playerBackSprite.pixels,
                                playerBackSprite.width, playerBackSprite.height,
                                playerBackSprite.x, playerBackSprite.y,
                                TRANSPARENT_COLOUR);
            }
            if (enemyFrontSprite.pixels != NULL) {
                draw_sprite_any(pokemonAreaFront, 
                                POKEMON_AREA_FRONT_WIDTH, 
                                POKEMON_AREA_FRONT_HEIGHT, 
                                AREA_FRONT_X, AREA_FRONT_Y, 
                                TRANSPARENT_COLOUR);
                draw_sprite_any(enemyFrontSprite.pixels,
                                enemyFrontSprite.width, enemyFrontSprite.height,
                                enemyFrontSprite.x, enemyFrontSprite.y,
                                TRANSPARENT_COLOUR);
            }

            // Animate the ball.
            const int tMax = 36;
            const int timeStep = 2;

            if (!pokeballCatchLanding) {
                pokeballCatchT += timeStep;
                if (pokeballCatchT > tMax) pokeballCatchT = tMax;

                const float u = (float)pokeballCatchT / (float)tMax;
                const float inv = 1.0f - u;
                const float x = (inv * inv) * pokeballCatchStartX + (2.0f * inv * u) * pokeballCatchCtrlX + (u * u) * pokeballCatchEndX;
                const float y = (inv * inv) * pokeballCatchStartY + (2.0f * inv * u) * pokeballCatchCtrlY + (u * u) * pokeballCatchEndY;

                const int dx = (int)(x + 0.5f);
                const int dy = (int)(y + 0.5f);
                if (dy >= -POKEBALLTHROW_HEIGHT && dy < SCREEN_HEIGHT) {
                    draw_sprite_any(pokeballThrowFrames[pokeballCatchFrame],
                                    POKEBALLTHROW_WIDTH,
                                    POKEBALLTHROW_HEIGHT,
                                    dx,
                                    dy,
                                    TRANSPARENT_COLOUR);
                }

                pokeballCatchThrownTimer++;
                if (pokeballCatchThrownTimer >= 1) {
                    pokeballCatchThrownTimer = 0;
                    pokeballCatchFrame = (pokeballCatchFrame + 1) % POKEBALLTHROW_THROW_FRAME_COUNT;
                }

                if (pokeballCatchT >= tMax) {
                    pokeballCatchLanding = true;
                    pokeballCatchLandingFrame = POKEBALLTHROW_LAND_FRAME_START;
                    pokeballCatchLandingTimer = 0;
                    pokeballCatchSeqFrame = POKEBALLTHROW_CATCH_FRAME_START;
                    pokeballCatchSeqTimer = 0;
                }
            } else {
                const int landEnd = POKEBALLTHROW_LAND_FRAME_START + POKEBALLTHROW_LAND_FRAME_COUNT;
                if (pokeballCatchLandingFrame < landEnd) {
                    const int dx = (int)(pokeballCatchEndX + 0.5f);
                    const int dy = (int)(pokeballCatchEndY + 0.5f);
                    draw_sprite_any(pokeballThrowFrames[pokeballCatchLandingFrame],
                                    POKEBALLTHROW_WIDTH,
                                    POKEBALLTHROW_HEIGHT,
                                    dx,
                                    dy,
                                    TRANSPARENT_COLOUR);

                    if (pokeballCatchLandingTimer >= 3) {
                        pokeballCatchLandingTimer = 0;
                        pokeballCatchLandingFrame++;
                    }
                    pokeballCatchLandingTimer++;
                } else {
                    int seqSpeed = (pokeballCatchSeqFrame >= POKEBALLTHROW_SHAKE_FRAME_START && 
                                    pokeballCatchSeqFrame < POKEBALLTHROW_SHAKE_FRAME_START + POKEBALLTHROW_SHAKE_FRAME_COUNT) 
                                    ? 14 : 3;
                    const int dx = (int)(pokeballCatchEndX + 0.5f);
                    const int dy = (int)(pokeballCatchEndY + 0.5f);

                    draw_sprite_any(pokeballThrowFrames[pokeballCatchSeqFrame],
                                    POKEBALLTHROW_WIDTH,
                                    POKEBALLTHROW_HEIGHT,
                                    dx,
                                    dy,
                                    TRANSPARENT_COLOUR);

                    if (pokeballCatchSeqTimer >= seqSpeed) {
                        pokeballCatchSeqTimer = 0;

                        const int escapeStart = POKEBALLTHROW_THROW_FRAME_START + (POKEBALLTHROW_THROW_FRAME_COUNT - 1); // frame 8 
                        const int escapeEnd = POKEBALLTHROW_LAND_FRAME_START + POKEBALLTHROW_LAND_FRAME_COUNT;          // up to frame 10 

                        if (!pokeballCatchEscape) {
                            pokeballCatchSeqFrame++;
                            if (pokeballCatchSeqFrame >= POKEBALLTHROW_SHAKE_FRAME_START && 
                                pokeballCatchSeqFrame < POKEBALLTHROW_SHAKE_FRAME_START + POKEBALLTHROW_SHAKE_FRAME_COUNT) {
                                play_sfx(pokeball_shaking_audio, pokeball_shaking_audio_len);
                            }
                            if (!pokeballCatchSuccess &&
                                pokeballCatchSeqFrame >= (POKEBALLTHROW_SHAKE_FRAME_START + POKEBALLTHROW_SHAKE_FRAME_COUNT)) {
                                // Escaped right after the shake frames.
                                pokeballCatchEscape = true;
                                pokeballCatchSeqFrame = escapeStart;
                            }
                        } else {
                            pokeballCatchSeqFrame++;
                        }

                        if (!pokeballCatchEscape) {
                            if (pokeballCatchSeqFrame >= POKEBALLTHROW_FRAME_COUNT) {
                                // Done (caught).
                                stop_bgm();
                                play_sfx(caught_pokemon_audio, caught_pokemon_audio_len);
                                battleUiSetSingleMessage(&battleState, "Gotcha!");
                                battleState.result = BATTLE_RESULT_CAUGHT;
                                actionTextReturnUi = BATTLE_UI_MENU;
                                actionTextReturnCursor = 0;
                                actionTextReturnGameState = activeBattleMenuState;
                                currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                actionTextAwaitSpaceRelease = true;
                                pokeballCatchInit = false;
                            }
                        } else {
                            if (pokeballCatchSeqFrame >= escapeEnd) {
                                // escaped
                                battleUiSetSingleMessage(&battleState, "Oh no! It broke free!");
                                battleState.result = BATTLE_RESULT_ONGOING;
                                actionTextReturnUi = BATTLE_UI_MENU;
                                actionTextReturnCursor = 0;
                                actionTextReturnGameState = activeBattleMenuState;
                                currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                actionTextAwaitSpaceRelease = true;
                                pokeballCatchInit = false;
                            }
                        }
                    }
                    pokeballCatchSeqTimer++;
                }
            }

            // UI background covers the sprites
            draw_sprite_any(battleUIBackgroundSprite,
                            BATTLE_UI_BACKGROUND_WIDTH,
                            BATTLE_UI_BACKGROUND_HEIGHT,
                            0,
                            battleBackdropY,
                            TRANSPARENT_COLOUR);


            if (pokeballCatchItem != ITEM_NONE) {
                char usedBuf[96];
                snprintf(usedBuf, sizeof(usedBuf), "You used a %s!", itemName(pokeballCatchItem));
                draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, usedBuf, BLACK);
            } else {
                draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, "You used a Poke Ball!", BLACK);
            }

            break;
        }
        case GAME_STATE_BATTLE_WIN:
        //check win/lose
        //get money from trainer battle, exp from wild battle if win
        //exp calculations, levelup, evolution, learn moves
            draw_map();
            pokemonInBattle *playerActive = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;
            pokemonInBattle *opponentActive = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
            gainExp(playerActive, opponentActive);
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, "WIN", BLACK);
            if (spacePressed) currentGameState = GAME_STATE_MAP;

            break;

        case GAME_STATE_BATTLE_LOSE:

            draw_map();
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, "LOSE", BLACK);
             if (spacePressed) currentGameState = GAME_STATE_MAP;
            currentGameState = GAME_STATE_MAP;
            
            break;

        case GAME_STATE_MAP:
        default:
            {
                draw_map();
                const McMoveResult moveResult = mcMovingTick(upDown, downDown, leftDown, rightDown, is_key_shift_pressed());
                WorldMapId targetMap;
                int spawnX;
                int spawnY;
                McFacing spawnFacing;
                if (resolve_map_transition(currentMapId, moveResult,
                                           upDown, downDown, leftDown, rightDown,
                                           &targetMap, &spawnX, &spawnY, &spawnFacing)) {
                    currentMapId = targetMap;
                    load_world_map(currentMapId, spawnX, spawnY, spawnFacing);
                    draw_map();
                } else if (moveResult == MC_MOVE_OK && should_trigger_grass_battle(upPressed, downPressed, leftPressed, rightPressed)) {
                    nextBattleType = BATTLE_WILD;
                    currentGameState = GAME_STATE_WILD_BATTLE;
                }
            }
            break;
        }

        wait_for_vsync();
    }

    return 0;
}
