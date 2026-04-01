#include "graphics/graphics.h"
#include "graphics/predefined_graphics.h"
#include "graphics/sprites/bagMenu/bagMenuSprites.h"
#include "graphics/predefined_colours.h"
#include "graphics/sprites/pokemon/charizardSprite.h"
#include "graphics/sprites/pokemon/rayquazaSprite.h"
#include "graphics/sprites/pokemon/pokemonSpriteInit.h"
#include "graphics/sprites/staticSprite.h"
#include "gameplayLogic/map_movement/mcMoving.h"
#include "gameplayLogic/worldMap.h"
#include "graphics/tiles.h"
#include "graphics/map.h"
#include "graphics/sprites/pokedexMenu/pokedexMenuSprites.h"
#include "graphics/sprites/pokedexTypes/pokedexTypesSprites.h"
#include "graphics/sprites/pcMenu/pcMenuSprites.h"
#include "graphics/sprites/vsCynthia/vsCynthiaSprite.h"
#include "graphics/titleScreen/titleScreenDraw.h"
#include "graphics/textbox/textBoxSprite.h"
#include "graphics/textbox/textMessages.h"
#include "graphics/sprites/spacebar/spacebar_frames.h"
#include "graphics/sprites/arrowGif/arrowGif_frames.h"
#include "graphics/sprites/pokeballSprites/smallPokeballSprite.h"
#include "../hardware/keyboard.h"
#include "../hardware/audio.h"
#include "../software/se/opening_audio.h"
#include "../software/se/introduction_audio.h"
#include "../software/se/battle_audio.h"
#include "../software/se/cynthia_audio.h"
#include "../software/se/map_audio.h"
#include "../software/se/poke_mart_audio.h"
#include "../software/se/pokemon_center_audio.h"
#include "../software/se/plink_audio.h"
#include "../software/se/pokeball_audio.h"
#include "../software/se/pokeball_shaking_audio.h"  
#include "../software/se/caught_pokemon_audio.h"
#include "../software/se/super_effective_audio.h"
#include "../software/se/not_effective_audio.h"
#include "../software/se/stats_up_audio.h"
#include "../software/se/stats_fall_audio.h"
#include "../software/se/pc_se_audio.h"
#include "../software/se/recover_audio.h"
#include "../software/se/pokemon_healed_audio.h"
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
#include "gameplayLogic/entities/pokedexDatabase.h"
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
#include "graphics/sprites/menu/cancelSprite.h"
#include "graphics/sprites/menuPokemon/menuPokemonSprites.h"
  #include "graphics/sprites/mainMenuUi/mainMenuUiSprites.h"
  #include "graphics/sprites/newPokemartIdea/newPokemartIdeaSprites.h"
#include "graphics/sprites/trainerCard/trainerCardSprite.h"
#include "graphics/sprites/trainerCard/trainerCardBackSprite.h"
#include "graphics/sprites/itemShopUI/pokemartBuyScreenSprite.h"
#include "graphics/sprites/evolutionBackdrop/evolutionBackdrop_frames.h"
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

// PC menu cursor
#define PC_MENU_ARROW_Y 20 - 9
#define PC_MENU_LEFT_ARROW_X 3
#define PC_MENU_RIGHT_ARROW_X ((340 / 2) + 20) + 20

#define PC_MENU_GRID_COLS 5
#define PC_MENU_GRID_ROWS 5
#define PC_MENU_GRID_X0 10 + 2
#define PC_MENU_GRID_Y0 40 +4 - 5
#define PC_MENU_GRID_STEP_X 42
#define PC_MENU_GRID_STEP_Y 40 - 10

#define PC_MENU_PARTY_BOX_X 200 
#define PC_MENU_PARTY_BOX_Y (120 - (PC_MENU_PARTY_BOX_HEIGHT / 2))
#define PC_MENU_PARTY_COLS 2
#define PC_MENU_PARTY_ROWS 3
#define PC_MENU_PARTY_X0 (PC_MENU_PARTY_BOX_X + 10) + 20
#define PC_MENU_PARTY_Y0 (PC_MENU_PARTY_BOX_Y + 10) - 2
#define PC_MENU_PARTY_STEP_X 53 - 3
#define PC_MENU_PARTY_STEP_Y 50 - 10 - 5 - 5

#define PC_MENU_PARTY_RIGHT_COL_DY (25)

#define PC_MENU_PARTY_ROW1_DY (-7)
#define PC_MENU_PARTY_ROW2_DY (-20)

#define PC_MENU_PARTY_RIGHT_COL_DX (-3)


//location for drawing names, hp, level

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

//non selected pokemon location 
#define NON_SELECTED_POKEMON_X 5
#define NON_SELECTED_POKEMON_Y 8
#define NON_SELECTED_POKEMON_HP_X 63
#define NON_SELECTED_POKEMON_HP_Y 24
#define NON_SELECTED_POKEMON_HP_WIDTH 48
#define NON_SELECTED_POKEMON_HP_HEIGHT 4
#define NON_SELECTED_POKEMON_CURRENT_HP_TEXT_X 67
#define NON_SELECTED_POKEMON_CURRENT_HP_TEXT_Y 39
#define NON_SELECTED_POKEMON_MAX_HP_TEXT_X 93
#define NON_SELECTED_POKEMON_MAX_HP_TEXT_Y 39
#define NON_SELECTED_POKEMON_LEVEL_TEXT_X 23
#define NON_SELECTED_POKEMON_LEVEL_TEXT_Y 39
#define POKEMON_TEXT_MENU_X 42
#define POKEMON_TEXT_MENU_Y 16

//location from partyMenu
#define continue_x 201
#define continue_y 165
// Cancel button (replaces ESC-to-exit in the menu)
#define CANCEL_X (continue_x - 1)
#define CANCEL_Y (continue_y - 1)

#define NON_SELECTED_POKEMON_STATUS_X 32
#define NON_SELECTED_POKEMON_STATUS_Y 23

// Selected slot art is drawn slightly up compared to unselected art.
#define SELECTED_POKEMON_DIFFERENCE_Y (-3)
#define SELECTED_POKEMON_DIFFERENCE_X (-1)

static const char CYNTHIA_GREETING_TEXT[] = "Cynthia: Good to see you again!";
static const char TRAINER_A_GREETING_TEXT[] = "Trainer A: Hey! I've been waiting for you.";
static const char TRAINER_A_BATTLE_TEXT[] = "When two Trainers' eyes meet,\n it's battle time!";
static const char NURSE_GREETING_TEXT[] = "Nurse: Welcome to the Pokemon Center!\nShall I heal your Pokemon?";
static const char NPC1_GREETING_TEXT[] = "ECE243 is the best!";
static const char NURSE_HEALED_TEXT[] = "Nurse: We hope to see you again!";
static const char CLERK_GREETING_TEXT[] = "Clerk: Welcome to the Poke Mart!\nWhat would you like to buy today?";
static const char FOUND_POKEBALL_TEXT[] = "You found a Poke Ball!";

static void play_world_map_bgm(WorldMapId map_id);
static const char *world_map_display_name(WorldMapId map_id);


// Game States
typedef enum {
    GAME_STATE_MAP = 0,
    GAME_STATE_WILD_BATTLE,
    GAME_STATE_WILD_BATTLE_TRANSITION,
    GAME_STATE_WILD_BATTLE_INTRO_TEXT,

    GAME_STATE_TRAINER_BATTLE,
    GAME_STATE_TRAINER_BATTLE_TRANSITION,
    GAME_STATE_TRAINER_BATTLE_INTRO_TEXT,

    GAME_STATE_BATTLE_ACTION_TEXT,
    GAME_STATE_POKEBALL_THROW,
    GAME_STATE_POKEBALL_CATCH,
    GAME_STATE_BATTLE_WIN,
    GAME_STATE_BATTLE_LOSE,
    GAME_STATE_MENU,

    // Learn-move
    GAME_STATE_LEARN_MOVE_PROMPT,
    GAME_STATE_LEARN_MOVE_YESNO,
    GAME_STATE_LEARN_MOVE_FORGET,
    GAME_STATE_LEARN_MOVE_MESSAGE,

    // Forced switch after player's Pokemon faints.
    GAME_STATE_BATTLE_FORCE_SWITCH,
    // Target select for Revive/Max Revive in battle.
    GAME_STATE_BATTLE_ITEM_TARGET,

    GAME_STATE_PC_MENU,
    GAME_STATE_POKEDEX_MENU,
    GAME_STATE_POKEDEX_INFO,
    GAME_STATE_DIALOGUE,
    GAME_STATE_POKEMON_CENTER_HEAL,
    GAME_STATE_BAG_MENU_ITEMS,
    GAME_STATE_BAG_MENU_POKEBALLS,
    GAME_STATE_BAG_MENU_TMS,
    GAME_STATE_BAG_MENU_BERRIES,
    GAME_STATE_BAG_MENU_KEY_ITEMS,
    GAME_STATE_MAIN_MENU_UI,
    GAME_STATE_TRAINER_CARD_FRONT,
    GAME_STATE_TRAINER_CARD_BACK,
    GAME_STATE_EVOLUTION,
    GAME_STATE_SHOP_UI,
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
    return state == GAME_STATE_MAP ||
           state == GAME_STATE_MENU ||
           state == GAME_STATE_PC_MENU ||
           state == GAME_STATE_POKEDEX_MENU ||
           state == GAME_STATE_POKEDEX_INFO ||
           state == GAME_STATE_DIALOGUE || 
           state == GAME_STATE_POKEMON_CENTER_HEAL || 
           state == GAME_STATE_BAG_MENU_ITEMS || 
           state == GAME_STATE_BAG_MENU_POKEBALLS|| 
           state == GAME_STATE_BAG_MENU_TMS|| 
           state == GAME_STATE_BAG_MENU_BERRIES|| 
           state == GAME_STATE_BAG_MENU_KEY_ITEMS ||
           state == GAME_STATE_MAIN_MENU_UI ||
           state == GAME_STATE_TRAINER_CARD_FRONT ||
           state == GAME_STATE_TRAINER_CARD_BACK ||
           state == GAME_STATE_SHOP_UI ||
           state == GAME_STATE_EVOLUTION;
}

static inline bool isBagMenuState(GameState state) {
    return state == GAME_STATE_BAG_MENU_ITEMS ||
           state == GAME_STATE_BAG_MENU_POKEBALLS ||
           state == GAME_STATE_BAG_MENU_TMS ||
           state == GAME_STATE_BAG_MENU_BERRIES ||
           state == GAME_STATE_BAG_MENU_KEY_ITEMS;
}

typedef enum {
    BAG_FOCUS_LIST = 0,
    BAG_FOCUS_RIGHT_ARROW = 1,
    BAG_FOCUS_LEFT_ARROW = 2,
    BAG_FOCUS_CANCEL = 3,
} BagMenuFocus;

static inline int bagMenuVisibleCountForState(GameState state, const Bag *bag) {
    if (bag == NULL) return 0;
    if (state == GAME_STATE_BAG_MENU_ITEMS) return bagHpVisibleCount(bag);
    if (state == GAME_STATE_BAG_MENU_POKEBALLS) return bagBallVisibleCount(bag);
    return 0;
}

static inline ItemId bagMenuVisibleAtForState(GameState state, const Bag *bag, int visibleIndex) {
    if (bag == NULL) return ITEM_NONE;
    if (state == GAME_STATE_BAG_MENU_ITEMS) return bagHpVisibleAt(bag, visibleIndex);
    if (state == GAME_STATE_BAG_MENU_POKEBALLS) return bagBallVisibleAt(bag, visibleIndex);
    return ITEM_NONE;
}

static inline GameState bagMenuNextPageState(GameState state) {
    switch (state) {
        case GAME_STATE_BAG_MENU_ITEMS: return GAME_STATE_BAG_MENU_POKEBALLS;
        case GAME_STATE_BAG_MENU_POKEBALLS: return GAME_STATE_BAG_MENU_TMS;
        case GAME_STATE_BAG_MENU_TMS: return GAME_STATE_BAG_MENU_BERRIES;
        case GAME_STATE_BAG_MENU_BERRIES: return GAME_STATE_BAG_MENU_KEY_ITEMS;
        case GAME_STATE_BAG_MENU_KEY_ITEMS: return GAME_STATE_BAG_MENU_ITEMS;
        default: return GAME_STATE_BAG_MENU_ITEMS;
    }
}

static inline GameState bagMenuPrevPageState(GameState state) {
    switch (state) {
        case GAME_STATE_BAG_MENU_ITEMS: return GAME_STATE_BAG_MENU_KEY_ITEMS;
        case GAME_STATE_BAG_MENU_POKEBALLS: return GAME_STATE_BAG_MENU_ITEMS;
        case GAME_STATE_BAG_MENU_TMS: return GAME_STATE_BAG_MENU_POKEBALLS;
        case GAME_STATE_BAG_MENU_BERRIES: return GAME_STATE_BAG_MENU_TMS;
        case GAME_STATE_BAG_MENU_KEY_ITEMS: return GAME_STATE_BAG_MENU_BERRIES;
        default: return GAME_STATE_BAG_MENU_ITEMS;
    }
}

static void draw_wrapped_string_fixed_width_f(int x, int y, const char *text, short colour, FontId font, int maxCharsPerLine, int maxLines) {
    if (text == NULL || text[0] == '\0' || maxCharsPerLine <= 0 || maxLines <= 0) return;

    // Build a small wrapped buffer with '\n' inserted.
    char buf[256];
    int out = 0;

    const char *p = text;
    for (int line = 0; line < maxLines && *p; line++) {
        // Skip leading spaces on each line.
        while (*p == ' ') p++;
        if (!*p) break;

        int count = 0;
        int lastSpaceOut = -1;
        int lineStartOut = out;

        while (*p && *p != '\n' && count < maxCharsPerLine && out < (int)sizeof(buf) - 2) {
            buf[out] = *p;
            if (*p == ' ') lastSpaceOut = out;
            out++;
            p++;
            count++;
        }

        // If we hit max width in the middle of a word, wrap back to the last space.
        if (count >= maxCharsPerLine && lastSpaceOut >= 0) {
            // rewind input pointer to after that space
            const int rewind = out - (lastSpaceOut + 1);
            p -= rewind;
            out = lastSpaceOut; // drop trailing part incl space
        }

        // Consume explicit newline.
        if (*p == '\n') p++;

        // If we wrote nothing on this line (very long word), force a break to avoid infinite loop.
        if (out == lineStartOut) {
            while (*p && *p != '\n') p++;
            if (*p == '\n') p++;
        }

        if (line != maxLines - 1 && *p && out < (int)sizeof(buf) - 2) {
            buf[out++] = '\n';
        }
    }

    buf[out] = '\0';
    draw_string_f(x, y, buf, colour, font);
}

static int findNextPendingEvolutionIndex(const Party *party, int afterIndex) {
    if (party == NULL) return -1;
    for (int i = afterIndex + 1; i < party->count; i++) {
        pokemonInBattle *mon = party->slots[i];
        if (mon == NULL) continue;
        if (!mon->alive) continue;
        if (mon->pendingEvolutionInto == NULL) {
            const PokemonData *into = (mon->id.data != NULL) ? checkEvolution(mon->id.data, mon->level) : NULL;
            mon->pendingEvolutionInto = into;
        }
        if (mon->pendingEvolutionInto == NULL) continue;
        return i;
    }
    return -1;
}



static inline int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static inline int expBarWidthFor(const pokemonInBattle *pokemon) {
    if (pokemon == NULL) return 0;
    const int needed = expRequiredAtLevel(pokemon->level);
    if (needed <= 0) return 0;
    if (pokemon->exp <= 0) return 0;

    // Use ceil so small EXP gains still show at least 1 pixel at high levels.
    const int numer = (EXP_WIDTH * pokemon->exp);
    const int w = (numer + needed - 1) / needed;
    return clamp_int(w, 1, EXP_WIDTH);
}

static inline int expBarWidthForValues(int level, int exp) {
    if (level < 1) level = 1;
    const int needed = expRequiredAtLevel(level);
    if (needed <= 0) return 0;
    if (exp <= 0) return 0;
    const int numer = (EXP_WIDTH * exp);
    const int w = (numer + needed - 1) / needed;
    return clamp_int(w, 1, EXP_WIDTH);
}

static inline bool popNextPendingLearnMove(Party *party, pokemonInBattle **outPokemon, const AttackData **outMove) {
    if (outPokemon != NULL) *outPokemon = NULL;
    if (outMove != NULL) *outMove = NULL;
    if (party == NULL || outPokemon == NULL || outMove == NULL) return false;

    for (int i = 0; i < party->count; i++) {
        pokemonInBattle *p = party->slots[i];
        if (p == NULL) continue;
        if (p->pendingLearnMoveCount <= 0) continue;

        const AttackData *m = p->pendingLearnMoves[0];
        // shift down
        for (int k = 1; k < p->pendingLearnMoveCount; k++) {
            p->pendingLearnMoves[k - 1] = p->pendingLearnMoves[k];
        }
        p->pendingLearnMoveCount--;
        if (p->pendingLearnMoveCount < 0) p->pendingLearnMoveCount = 0;
        if (p->pendingLearnMoveCount < 4) p->pendingLearnMoves[p->pendingLearnMoveCount] = NULL;

        *outPokemon = p;
        *outMove = m;
        return (m != NULL);
    }

    return false;
}

static inline void queueLearnedMoveMsg(pokemonInBattle *pokemon, const AttackData *learned, const AttackData *forgotten) {
    if (pokemon == NULL || learned == NULL) return;
    if (pokemon->pendingLearnedMoveCount < 0) pokemon->pendingLearnedMoveCount = 0;
    if (pokemon->pendingLearnedMoveCount >= 4) return;
    const int idx = pokemon->pendingLearnedMoveCount;
    pokemon->pendingLearnedMoves[idx] = learned;
    pokemon->pendingForgottenMoves[idx] = forgotten;
    pokemon->pendingLearnedMoveCount++;
}

static inline bool popNextLearnedMoveMsg(Party *party, pokemonInBattle **outPokemon, const AttackData **outLearned, const AttackData **outForgotten) {
    if (outPokemon != NULL) *outPokemon = NULL;
    if (outLearned != NULL) *outLearned = NULL;
    if (outForgotten != NULL) *outForgotten = NULL;
    if (party == NULL || outPokemon == NULL || outLearned == NULL || outForgotten == NULL) return false;

    for (int i = 0; i < party->count; i++) {
        pokemonInBattle *p = party->slots[i];
        if (p == NULL) continue;
        if (p->pendingLearnedMoveCount <= 0) continue;
        const AttackData *learned = p->pendingLearnedMoves[0];
        const AttackData *forgot = p->pendingForgottenMoves[0];

        for (int k = 1; k < p->pendingLearnedMoveCount; k++) {
            p->pendingLearnedMoves[k - 1] = p->pendingLearnedMoves[k];
            p->pendingForgottenMoves[k - 1] = p->pendingForgottenMoves[k];
        }
        p->pendingLearnedMoveCount--;
        if (p->pendingLearnedMoveCount < 0) p->pendingLearnedMoveCount = 0;
        if (p->pendingLearnedMoveCount < 4) {
            p->pendingLearnedMoves[p->pendingLearnedMoveCount] = NULL;
            p->pendingForgottenMoves[p->pendingLearnedMoveCount] = NULL;
        }

        *outPokemon = p;
        *outLearned = learned;
        *outForgotten = forgot;
        return (learned != NULL);
    }

    return false;
}

//Money, perssistant for player
#define STARTING_MONEY 3000
#define TRAINER_WIN_MONEY_BASE 200
#define TRAINER_WIN_MONEY_PER_LEVEL 25
#define BATTLE_LOSS_PENALTY_MIN 50
#define BATTLE_LOSS_PENALTY_MAX 500
#define BATTLE_LOSS_PENALTY_DIV 10  // lose ~10% (clamped)

static inline int computeTrainerPayout(const Party *enemyParty) {
    if (enemyParty == NULL) return TRAINER_WIN_MONEY_BASE;
    int maxLevel = 1;
    for (int i = 0; i < enemyParty->count; i++) {
        const pokemonInBattle *p = enemyParty->slots[i];
        if (p != NULL && p->level > maxLevel) maxLevel = p->level;
    }
    return TRAINER_WIN_MONEY_BASE + (maxLevel * TRAINER_WIN_MONEY_PER_LEVEL);
}

static inline int computeLossPenalty(int money) {
    if (money <= 0) return 0;
    int penalty = money / BATTLE_LOSS_PENALTY_DIV;
    if (penalty < BATTLE_LOSS_PENALTY_MIN) penalty = BATTLE_LOSS_PENALTY_MIN;
    if (penalty > BATTLE_LOSS_PENALTY_MAX) penalty = BATTLE_LOSS_PENALTY_MAX;
    if (penalty > money) penalty = money;
    return penalty;
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
    // Type effectiveness messages get a longer on delay.
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

static int navPCMenu33(int index, NavDir dir) {
    static const signed char nav[33][4] = {
         /* up left down right */
        /*0 left arrow*/ {0, 0, 2, 1},
        /*1 right arrow */ {1, 0, 6, 27},
        /*2 top left */ {0, 2, 7, 3},
        /*3 */ {0, 2, 8, 4},
        /*4  */ {1, 3, 9, 5},
        /*5  */ {1, 4, 10, 6},
        /*6 top right */ {1, 5, 11, 27},
        /*7  */ {2, 7, 12, 8},
        /*8  */ {3, 7, 13, 9},
        /*9 */ {4, 8, 14, 10},
        /*10  */ {5, 9, 15, 11},
        /*11  */ {6, 10, 16, 28},
        /*12 */ {7, 12, 17, 13},
        /* 13*/ {8, 12, 18, 14},
        /*14  */ {9, 13, 19, 15},
        /*15  */ {10, 14, 20, 16},
        /*16  */ {11, 15, 21, 29},
        /*17 */ {12, 17, 22, 18},
        /*18*/ {13, 17, 23, 19},
        /*19 */ {14, 18, 24, 20},
        /*20 */ {15, 19, 25, 21},
        /*21 */ {16, 20, 26, 31},
        /*22 bottom left*/ {17, 22, 22, 23},
        /*23 */ {18, 22, 23, 24},
        /*24 */ {19, 23, 24, 25},
        /*25 */ {20, 24, 25, 26},
        /*26 : bottom right*/ {21, 25, 26, 31},
        /*27: p1 */ {27, 6, 29, 28},
        /*28 : p2*/ {28, 27, 30, 28},
        /*29 : p3*/ {27, 11, 31, 30},
        /*30 : p4*/ {28, 29, 32, 30},
        /*31 : p5*/ {29, 16, 31, 32},
        /*32 : p6*/ {30, 31, 32, 32},
    };

    if (index < 0) index = 0;
    if (index > 32) index = 32;

    int d = (int)dir;
    if (d < 0) d = 0;
    if (d > 3) d = 3;

    const int next = (int)nav[index][d];
    if (next < 0 || next > 32) return index;
    return next;
}

static void pcMenuCursorPos(int index, int *outX, int *outY) {
    if (outX) *outX = 0;
    if (outY) *outY = 0;
    if (outX == NULL || outY == NULL) return;

    if (index < 0) index = 0;
    if (index > 32) index = 32;

    const int cursorW = PC_MENU_SELECT_CURSOR_WIDTH;
    const int cursorH = PC_MENU_SELECT_CURSOR_HEIGHT;
    (void)cursorH;

    // Arrow icons
    if (index == 0) {
        // Left arrow sprite is drawn at (0, 20)
        *outX = PC_MENU_LEFT_ARROW_X;
        *outY = PC_MENU_ARROW_Y;
        return;
    }
    if (index == 1) {
        // Right arrow sprite is drawn at ((340 / 2) + 20, 20)
        const int arrowX = PC_MENU_RIGHT_ARROW_X;
        const int arrowW = PC_MENU_RIGHT_ARROW_WIDTH;
        *outX = arrowX - (cursorW - arrowW) / 2;
        *outY = PC_MENU_ARROW_Y;
        return;
    }

    // PC storage grid: indices 2..26 (5 columns x 5 rows)
    if (index >= 2 && index <= 26) {
        const int gridIndex = index - 2;
        const int row = gridIndex / PC_MENU_GRID_COLS;
        const int col = gridIndex % PC_MENU_GRID_COLS;

        *outX = PC_MENU_GRID_X0 + col * PC_MENU_GRID_STEP_X;
        *outY = PC_MENU_GRID_Y0 + row * PC_MENU_GRID_STEP_Y;
        return;
    }

    // Party slots: indices 27..32 (2 columns x 3 rows)
    if (index >= 27 && index <= 32) {
        const int partyIndex = index - 27;
        const int row = partyIndex / PC_MENU_PARTY_COLS;
        const int col = partyIndex % PC_MENU_PARTY_COLS;

        const int rowDy = (row == 1) ? PC_MENU_PARTY_ROW1_DY : ((row == 2) ? PC_MENU_PARTY_ROW2_DY : 0);
        const int colDx = (col == 1) ? PC_MENU_PARTY_RIGHT_COL_DX : 0;
        const int colDy = (col == 1) ? PC_MENU_PARTY_RIGHT_COL_DY : 0;
        *outX = PC_MENU_PARTY_X0 + col * PC_MENU_PARTY_STEP_X + colDx;
        *outY = PC_MENU_PARTY_Y0 + row * PC_MENU_PARTY_STEP_Y + rowDy + colDy;
        return;
    }
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


static bool should_trigger_grass_battle(bool upPressed, bool downPressed,
                                        bool leftPressed, bool rightPressed) {
    static unsigned int grassEncounterRng = 0x2432026u;
    const bool movedInputPressed = upPressed || downPressed || leftPressed || rightPressed;
    const McBounds bounds = getMCBounds();
    if (!movedInputPressed || !map_is_mc_on_grass_patch(&bounds)) {
        return false;
    }

    grassEncounterRng = grassEncounterRng * 1664525u + 1013904223u;
    return (grassEncounterRng % 10u) == 0u;
}

//pokedex state

static bool g_pokedexSeen[POKEMON_ID_TOGEKISS + 1];
static bool g_pokedexCaught[POKEMON_ID_TOGEKISS + 1];

static void make_sprite_black(const unsigned short *sprite,
                              int width, int height,
                              int x, int y,
                              short transparent)
{
    if (sprite == NULL || width <= 0 || height <= 0) return;
    for (int sy = 0; sy < height; sy++) {
        for (int sx = 0; sx < width; sx++) {
            const unsigned short c = sprite[sy * width + sx];
            if (c == (unsigned short)transparent) continue;
            draw_pixel(x + sx, y + sy, BLACK);
        }
    }
}

static void draw_multiline_string_f(int x, int y, const char *text, short colour, FontId font, int line_step) {
    if (text == NULL) return;
    if (line_step <= 0) line_step = 12;

    char buf[96];
    int bi = 0;
    int cy = y;

    for (const char *p = text; ; p++) {
        const char ch = *p;
        const bool at_end = (ch == '\0');
        if (at_end || ch == '\n') {
            buf[bi] = '\0';
            if (bi > 0) {
                draw_string_f(x, cy, buf, colour, font);
            }
            cy += line_step;
            bi = 0;
            if (at_end) break;
            continue;
        }

        if (bi < (int)(sizeof(buf) - 1)) {
            buf[bi++] = ch;
        }
    }
}

static const unsigned short *pokedex_type_sprite_for(PokemonType t, int *outW, int *outH) {
    if (outW) *outW = 0;
    if (outH) *outH = 0;

    PokedexTypeSpriteId sid;
    switch (t) {
        case TYPE_BUG: sid = POKEDEX_TYPE_SPRITE_BUG; break;
        case TYPE_DARK: sid = POKEDEX_TYPE_SPRITE_DARK; break;
        case TYPE_DRAGON: sid = POKEDEX_TYPE_SPRITE_DRAGON; break;
        case TYPE_ELECTRIC: sid = POKEDEX_TYPE_SPRITE_ELECTRIC; break;
        case TYPE_FAIRY: sid = POKEDEX_TYPE_SPRITE_FAIRY; break;
        case TYPE_FIGHTING: sid = POKEDEX_TYPE_SPRITE_FIGHTING; break;
        case TYPE_FIRE: sid = POKEDEX_TYPE_SPRITE_FIRE; break;
        case TYPE_FLYING: sid = POKEDEX_TYPE_SPRITE_FLYING; break;
        case TYPE_GHOST: sid = POKEDEX_TYPE_SPRITE_GHOST; break;
        case TYPE_GRASS: sid = POKEDEX_TYPE_SPRITE_GRASS; break;
        case TYPE_GROUND: sid = POKEDEX_TYPE_SPRITE_GROUND; break;
        case TYPE_ICE: sid = POKEDEX_TYPE_SPRITE_ICE; break;
        case TYPE_NORMAL: sid = POKEDEX_TYPE_SPRITE_NORMAL; break;
        case TYPE_POISON: sid = POKEDEX_TYPE_SPRITE_POISON; break;
        case TYPE_PSYCHIC: sid = POKEDEX_TYPE_SPRITE_PSY; break;
        case TYPE_ROCK: sid = POKEDEX_TYPE_SPRITE_ROCK; break;
        case TYPE_STEEL: sid = POKEDEX_TYPE_SPRITE_STEEL; break;
        case TYPE_WATER: sid = POKEDEX_TYPE_SPRITE_WATER; break;
        case TYPE_NONE:
        default:
            return NULL;
    }

    if ((int)sid < 0 || (int)sid >= POKEDEX_TYPE_SPRITE_COUNT) return NULL;
    if (outW) *outW = (int)pokedexTypesSpritesWidths[sid];
    if (outH) *outH = (int)pokedexTypesSpritesHeights[sid];
    return pokedexTypesSprites[sid];
}

static inline void pokedex_mark_seen(int pokemon_id) {
    if (pokemon_id >= 0 && pokemon_id <= POKEMON_ID_TOGEKISS) g_pokedexSeen[pokemon_id] = true;
}

static inline void pokedex_mark_caught(int pokemon_id) {
    if (pokemon_id >= 0 && pokemon_id <= POKEMON_ID_TOGEKISS) {
        g_pokedexSeen[pokemon_id] = true;
        g_pokedexCaught[pokemon_id] = true;
    }
}

static inline bool pokedex_is_seen(int pokemon_id) {
    return (pokemon_id >= 0 && pokemon_id <= POKEMON_ID_TOGEKISS) ? g_pokedexSeen[pokemon_id] : false;
}

static inline bool pokedex_is_caught(int pokemon_id) {
    return (pokemon_id >= 0 && pokemon_id <= POKEMON_ID_TOGEKISS) ? g_pokedexCaught[pokemon_id] : false;
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

static void setupTrainerAParty(Party *enemyParty, pokemonInBattle team[6]) {
    if (enemyParty == NULL || team == NULL) return;

    initParty(enemyParty);

    initPokemonInBattle(&team[0], &GARCHOMP, 20);

    for (int i = 0; i < 1; i++) {
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

static inline bool partySlotIsAlive(const pokemonInBattle *slotPokemon) {
    if (slotPokemon == NULL) return false;
    const int hp = slotPokemon->scaledStatsWithLevel[0];
    return slotPokemon->alive && (hp > 0);
}

static void syncPartyBoxSpritesToParty(StaticSprite partyBoxSprites[6], const Party *party) {
    if (partyBoxSprites == NULL || party == NULL) return;
    for (int i = 0; i < 6; i++) {
        const pokemonInBattle *slotPokemon = (i >= 0 && i < party->count) ? party->slots[i] : NULL;
        const int spriteId = (slotPokemon != NULL) ? slotPokemon->id.frontFrame_ID : 0;
        setPokemonBoxSpriteId(&partyBoxSprites[i], spriteId);
    }
}

static void autoSwapLeadIfFainted(Party *party, StaticSprite partyBoxSprites[6]) {
    if (party == NULL || party->count <= 0) return;
    if (partySlotIsAlive(party->slots[0])) return;

    int nextAlive = -1;
    for (int i = 1; i < party->count; i++) {
        if (partySlotIsAlive(party->slots[i])) {
            nextAlive = i;
            break;
        }
    }
    if (nextAlive < 0) return;

    pokemonInBattle *tmp = party->slots[0];
    party->slots[0] = party->slots[nextAlive];
    party->slots[nextAlive] = tmp;

    if (party->activeIndex == 0) party->activeIndex = nextAlive;
    else if (party->activeIndex == nextAlive) party->activeIndex = 0;

    syncPartyBoxSpritesToParty(partyBoxSprites, party);
}

static void draw_sprite_any_rot90_cw(const unsigned short *sprite,
                                     int width, int height,
                                     int x, int y,
                                     short transparent)
{
    if (sprite == NULL || width <= 0 || height <= 0) return;

    // 90° clockwise rotation.
    for (int sy = 0; sy < height; sy++) {
        for (int sx = 0; sx < width; sx++) {
            const unsigned short colour = sprite[sy * width + sx];
            if (colour == (unsigned short)transparent) continue;
            const int dx = x + (height - 1 - sy);
            const int dy = y + sx;
            draw_pixel(dx, dy, (short)colour);
        }
    }
}

static void battleUiSetSingleMessage(BattleState *state, const char *msg) {
    if (state == NULL || msg == NULL) return;
    state->messageCount = 0;
    state->messageReadIndex = 0;
    for (int i = 0; i < BATTLE_MSG_MAX; i++) {
        state->messages[i][0] = '\0';
        state->messageFlags[i] = 0;
        state->hitEffects[i] = (signed char)BATTLE_HIT_EFFECT_NORMAL;
        state->hpAfterPlayer[i] = -1;
        state->hpAfterEnemy[i] = -1;
        state->displayPlayerIndex[i] = -1;
        state->displayEnemyIndex[i] = -1;
    }
    snprintf(state->messages[0], sizeof(state->messages[0]), "%s", msg);
    state->displayPlayerIndex[0] = (state->playerParty != NULL) ? (signed char)state->playerParty->activeIndex : (signed char)-1;
    state->displayEnemyIndex[0] = (state->enemyParty != NULL) ? (signed char)state->enemyParty->activeIndex : (signed char)-1;
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

    bool clearShopOpen = false;
    //transition into battle
    int transitionFrame = 0;
    int transitionTimer = 0;
    const int transitionSpeedFrames = 2; 

    int textboxMsgIndex = 0;
    const char *textboxMsg = TEXT_MESSAGES[textboxMsgIndex];

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
    const char *dialogueText = NULL;
    GameState dialogueReturnState = GAME_STATE_MAP;

    // Pokemon Center heal animation state
    int pcHealFrame = 0;
    int pcHealTimer = 0;
    int pcHealPulseFrame = 0;
    bool pcHealDone = false;
    // SHADE_PULSE_FRAME_COUNT = 16 total frames, run ~3 full cycles (~48 ticks)
    static const int PC_HEAL_ANIM_TICKS = 48;
    static const int PC_HEAL_ANIM_SPEED = 2; // ticks per pulse frame advance
    static const int PC_HEAL_POKEBALL_BASE_X = 125;
    static const int PC_HEAL_POKEBALL_BASE_Y = 158;
    static const int PC_HEAL_ROW_H = 20; // vertical spacing between rows
    static const int PC_HEAL_POKEBALL_PX[6] = {
        124,       // pb1: base X
        134,       // pb2: base X + 10
        124,       // pb3: base X as pb1
        134,       // pb4: same X as pb2
        124,       // pb5: same X as pb1
        134,       // pb6: same X as pb2
    };
    static const int PC_HEAL_POKEBALL_PY[6] = {
        54,       // pb1: row 1
        54,       // pb2: row 1
        58,       // pb3: row 2
        58,       // pb4: row 2
        62,       // pb5: row 3
        62,       // pb6: row 3
    };
    // Ticks each pokeball stays before the next appears
    static const int PC_HEAL_TICKS_PER_BALL = 8;
    (void)PC_HEAL_POKEBALL_BASE_X; (void)PC_HEAL_POKEBALL_BASE_Y; (void)PC_HEAL_ROW_H;
    BattleUiState battleUi = BATTLE_UI_MENU;
    GameState previousGameState = currentGameState;
    WorldMapId currentMapId = WORLD_MAP_ROUTE_A;
    int mapReturnX = 80;
    int mapReturnY = 112;
    GameState activeBattleMenuState = GAME_STATE_WILD_BATTLE;
    int battleCursor = 0;

    // Bag UI state.
    int bagHpPage = 0;
    int bagBallPage = 0;
    BattleUiState bagDescReturnUi = BATTLE_UI_BAG_HP_LIST;
    int bagDescReturnCursor = 0;
    int bagDescReturnPage = 0;
    ItemId bagDescItem = ITEM_NONE;

    bool prevUp = false, prevLeft = false, prevDown = false, prevRight = false;
    bool prevEsc = false;
    bool actionTextAwaitSpaceRelease = false;
    BattleUiState actionTextReturnUi = BATTLE_UI_MENU;
    int actionTextReturnCursor = 0;
    GameState actionTextReturnGameState = GAME_STATE_WILD_BATTLE;
    int actionTextAutoTimer = 0;
    int actionTextInterAttackPauseTimer = 5;
    int actionTextLastMsgIndex = -1;
    
    int actionTextShownPlayerHp = -1;
    int actionTextShownEnemyHp = -1;
    int actionTextTargetPlayerHp = -1;
    int actionTextTargetEnemyHp = -1;
    int actionTextDisplayPlayerIndex = -1;
    int actionTextDisplayEnemyIndex = -1;

    // EXP animation in action-text
    int actionTextExpStartLevel = -1;
    int actionTextExpStartExp = -1;
    int actionTextShownLevel = -1;
    int actionTextShownExp = -1;
    int actionTextTargetLevel = -1;
    int actionTextTargetExp = -1;
    bool actionTextExpAnimating = false;

    // Battle logic (static to avoid stack overflow + crash) -> pc has too much data probablt
    static BattleState battleState;
    static Party playerParty;
    static Party enemyParty;
    static PC playerPc;
    static Bag playerBag;
    static pokemonInBattle wildEnemy;

    static pokemonInBattle cynthiaTeam[6];
    static pokemonInBattle trainerATeam[6];
    BattleType nextBattleType = BATTLE_WILD;

    StaticSprite playerBackSprite;
    StaticSprite enemyFrontSprite;

    // Defer battle actions until GAME_STATE_BATTLE_ACTION_TEXT so HP/shake can be timed.
    bool battleActionPending = false;
    BattleAction pendingBattleAction = ACTION_ATTACK;
    int pendingBattleParam = 0;

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

    // init the roster and party
    pcInit(&playerPc);
    initParty(&playerParty);
    int playerMoney = STARTING_MONEY;
    unsigned int playTimeFrames = 0;
    const unsigned int GAME_FPS = 20;

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
    bagAdd(&playerBag, ITEM_REVIVE, 3);
    bagAdd(&playerBag, ITEM_MAX_REVIVE, 3);


    for (int i = 0; i < 6; i++) {
        const PokemonData *species = speciesFromPokemonSpriteId(playerTeamSpriteIds[i]);
        int ownedIndex = -1;
        if (species != NULL && pcAdd(&playerPc, species, 19 + i, &ownedIndex)) {
            addPokemonToParty(&playerParty, pcGet(&playerPc, ownedIndex));
        }
    }

    // Initialize pokedex state from owned Pokemon.
    memset(g_pokedexSeen, 0, sizeof(g_pokedexSeen));
    memset(g_pokedexCaught, 0, sizeof(g_pokedexCaught));
    for (int i = 0; i < playerPc.count; i++) {
        const int id = playerPc.mons[i].id.frontFrame_ID;
        pokedex_mark_caught(id);
    }

    initParty(&enemyParty);
    initPokemonInBattle(&wildEnemy, &CHARMANDER, 15);
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
    play_world_map_bgm(currentMapId);
    wait_for_vsync();

    textboxDone = 0;
    prevSpaceDown = false;
    int menuCursor = 0; // 0..5 (2 columns x 3 rows)
    int menuSwapIndex = -1; // first picked index for swapping in the party menu
    GameState partyMenuReturnState = GAME_STATE_MAP;
    char battleEndMsg[96] = "WIN";

    int pcCursor = 0; // 6 + 2 + max storage in pc
    int pcSwapIndex = -1; //first picked index for swapping in pc
    pokemonInBattle *pcHeldMon = NULL;
    GameState pcMenuReturnState = GAME_STATE_MAP;

    // Pokedex UI state
    int pokedexSelectedId = POKEMON_ID_CHARMANDER;
    int pokedexScrollIndex = POKEMON_ID_CHARMANDER;
    int pokedexInfoCursor = 1; // 0=up arrow, 1=down arrow, 2=X
    GameState pokedexMenuReturnState = GAME_STATE_MAP;

    // Bag menu return state (Esc to close).
    GameState bagMenuReturnState = GAME_STATE_MAP;
    int bagMenuCursor = 0;
    BagMenuFocus bagMenuFocus = BAG_FOCUS_LIST;

    // Evolution UI state (runs after battles).
    int evolutionFrame = 0;
    int evolutionTimer = 0;
    int evolutionPhase = 0; // 0=backdrop, 1=show base, 2=show evolved
    int evolutionPhaseTimer = 0;
    int evolutionPokemonIndex = -1;
    GameState evolutionReturnState = GAME_STATE_MAP;
    char evolutionFromName[64];
    char evolutionIntoName[64];
    evolutionFromName[0] = '\0';
    evolutionIntoName[0] = '\0';

    // Learn-move flow state
    pokemonInBattle *learnMovePokemon = NULL;
    const AttackData *learnMoveMove = NULL;
    const AttackData *learnMoveForgottenMove = NULL;
    int learnMoveForgetIndex = 0; // 0..3
    GameState learnMoveReturnState = GAME_STATE_MAP;
    BattleUiState learnMoveReturnUi = BATTLE_UI_MENU;
    int learnMoveReturnCursor = 0;
    char learnMoveMsgBuf[192];

    // Forced switch UI state.
    int forcedSwitchIndex = 0;

    // Item target select (Revive/Max Revive) state.
    ItemId itemTargetItem = ITEM_NONE;
    int itemTargetIndex = 0;
    BattleUiState itemTargetReturnUi = BATTLE_UI_MENU;
    int itemTargetReturnCursor = 0;
    int itemTargetReturnPage = 0;

    while (1) {
        update_keyboard();
        bool enterPressed = false;
        {
            char ch = 0;
            while (keyboard_pop_char(&ch)) {
                if (ch == '2') {
                    currentMapId = WORLD_MAP_ROUTE_A;
                    mapReturnX = 80;
                    mapReturnY = 112;
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
                    menuSwapIndex = -1;
                    partyMenuReturnState = GAME_STATE_MAP;
                } else if (ch == '4' && currentGameState == GAME_STATE_MENU) {
                    currentGameState = GAME_STATE_MAP;
                    menuSwapIndex = -1;
                } else if (ch == '5' && currentGameState == GAME_STATE_MAP) {
                    currentGameState = GAME_STATE_PC_MENU;
                    pcCursor = 0;
                    pcSwapIndex = -1;
                    pcHeldMon = NULL;
                    pcMenuReturnState = GAME_STATE_MAP;
                    play_sfx(pc_se_audio, pc_se_audio_len);
                    } else if (ch == '6' && currentGameState == GAME_STATE_MAP) {
                        currentGameState = GAME_STATE_POKEDEX_MENU;
                        pokedexScrollIndex = POKEMON_ID_CHARMANDER;
                        pokedexSelectedId = POKEMON_ID_CHARMANDER;
                        pokedexMenuReturnState = GAME_STATE_MAP;
                        play_sfx(pc_se_audio, pc_se_audio_len);
                    } else if (ch == '7') {
                        
                        if (currentGameState == GAME_STATE_MAP) {
                            currentGameState = GAME_STATE_BAG_MENU_ITEMS;
                            bagMenuReturnState = GAME_STATE_MAP;
                            bagMenuCursor = 0;
                            bagMenuFocus = BAG_FOCUS_LIST;
                        } else if (isBagMenuState(currentGameState)) {
                            currentGameState = GAME_STATE_MAP;
                        }
                    } else if (ch == '\t') {

                        if (currentGameState == GAME_STATE_MAP) {
                            currentGameState = GAME_STATE_MAIN_MENU_UI;
                        } else if (currentGameState == GAME_STATE_MAIN_MENU_UI) {
                            currentGameState = GAME_STATE_MAP;
                        }
                    } else if (ch == '\n') {
                        enterPressed = true;
                    }
                }
            }

        const bool wasOverworld = isOverworldState(previousGameState);
        const bool isOverworld = isOverworldState(currentGameState);
        if (wasOverworld != isOverworld) {
            if (isOverworld) {
                load_world_map(currentMapId, mapReturnX, mapReturnY, MC_FACING_S);
                play_world_map_bgm(currentMapId);
            } else {
                getMCPosition(&mapReturnX, &mapReturnY);
                if (nextBattleType == BATTLE_TRAINER) {
                    if (currentMapId == WORLD_MAP_ROUTE_B) {
                        play_bgm(battle_audio, battle_audio_len);
                    } else {
                        play_bgm(cynthia_audio, cynthia_audio_len);
                    }
                } else {
                    play_bgm(battle_audio, battle_audio_len);
                }
                init_map();
                load_map_preset(MAP_PRESET_BACKDROP1);
                battleUi = BATTLE_UI_MENU;
                battleCursor = 0;

 
                // Player party is persistent for this program run
                if (nextBattleType == BATTLE_WILD) {
                    initParty(&enemyParty);
                    initPokemonInBattle(&wildEnemy, &CHARMANDER, 15);
                    addPokemonToParty(&enemyParty, &wildEnemy);
                } else {
                    // battle type is BATTLE_TRAINER
                    if (currentMapId == WORLD_MAP_ROUTE_B) {
                        setupTrainerAParty(&enemyParty, trainerATeam);
                    } else {
                        setupCynthiaTrainerParty(&enemyParty, cynthiaTeam);
                    }
                }

                // Ensure the party leader (slot 0) gets sent out first.
                playerParty.activeIndex = 0;
                if (playerParty.slots[0] == NULL || !playerParty.slots[0]->alive) {
                    const int firstAlive = getFirstAlivePokemon(&playerParty);
                    if (firstAlive >= 0) playerParty.activeIndex = firstAlive;
                }
                initBattleState(&battleState, &playerParty, &enemyParty, &playerBag, nextBattleType);

                // Mark encountered Pokemon as "seen" in the pokedex.
                for (int i = 0; i < enemyParty.count; i++) {
                    const pokemonInBattle *mon = enemyParty.slots[i];
                    if (mon != NULL) pokedex_mark_seen(mon->id.frontFrame_ID);
                }

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
            // Slot content anchor (slot art is at x=32,y=24).
            const int menuX = 32;
            const int menuY = 24;

            if (menuCursor < 0) menuCursor = 0;
            if (menuCursor > 6) menuCursor = 6;

            // Navigation: slots 0..5 plus Cancel (6). Cancel is reached from the bottom-right slot (5).
            const int prevCursor = menuCursor;
            if (menuCursor == 6) {
                if (leftPressed || upPressed) menuCursor = 5;
            } else {
                const int row = menuCursor / 2;
                const int col = menuCursor % 2;
                int nextRow = row;
                int nextCol = col;
                if (upPressed && row > 0) nextRow--;
                if (downPressed && row < 2) nextRow++;
                if (leftPressed && col > 0) nextCol--;
                if (rightPressed && col < 1) nextCol++;

                menuCursor = nextRow * 2 + nextCol;
                if (menuCursor < 0) menuCursor = 0;
                if (menuCursor > 5) menuCursor = 5;

                if (prevCursor == 5 && (rightPressed || downPressed)) {
                    menuCursor = 6;
                }
            }
            if (menuCursor != prevCursor) play_sfx(plink_audio, plink_audio_len);

            // Animate selected pokemon bob in the menu.
            bobTimer++;
            if (bobTimer >= bobSpeedFrames) {
                bobTimer = 0;
                bobFrame = (bobFrame + 1) % BOB_SPRITE_FRAME_COUNT;
            }

            draw_map();
            draw_sprite_any(partyMenuSprite,
                            MENU_PARTY_MENU_WIDTH, MENU_PARTY_MENU_HEIGHT,
                            0, 0,
                            TRANSPARENT_COLOUR);

            const int slotW = 128;
            const int slotH = 48;

            for (int i = 0; i < 6; i++) {
                const int slotX = menuX + (i % 2) * slotW;
                const int slotY = menuY + (i / 2) * slotH;
                const int colYOffset = ((i % 2) == 1) ? 9 : 0;
                const bool isSelected = (i == menuCursor) || (i == menuSwapIndex);
                const bool isLeaderSlot = (i == 0);

                // Slot frame art shifts for unselected slots (x+1, y+3).
                const int slotXOffsetForFrame = isSelected ? 0 : 1;
                const int slotYOffsetForFrame = isSelected ? 0 : 3;

                const int dy = (i / 2) * 48;
                const int dx = ((i % 2) == 1) ? (161 - 33) : 0;

                if (isLeaderSlot) {
                    if (isSelected) {
                        draw_sprite_any(partyLeaderSelectedSprite,
                                        MENU_PARTY_LEADER_SELECTED_WIDTH, MENU_PARTY_LEADER_SELECTED_HEIGHT,
                                        32, 24,
                                        TRANSPARENT_COLOUR);
                    } else {
                        draw_sprite_any(partyLeaderUnselectedSprite,
                                        MENU_PARTY_LEADER_UNSELECTED_WIDTH, MENU_PARTY_LEADER_UNSELECTED_HEIGHT,
                                        33, 27,
                                        TRANSPARENT_COLOUR);
                    }
                } else {
                    if (isSelected) {
                        draw_sprite_any(pokemonSelectedSprite,
                                        MENU_POKEMON_SELECTED_WIDTH, MENU_POKEMON_SELECTED_HEIGHT,
                                        32 + dx, 24 + colYOffset + dy,
                                        TRANSPARENT_COLOUR);
                    } else {
                        draw_sprite_any(pokemonUnselectedSprite,
                                        MENU_POKEMON_UNSELECTED_WIDTH, MENU_POKEMON_UNSELECTED_HEIGHT,
                                        33 + dx, 27 + colYOffset + dy,
                                        TRANSPARENT_COLOUR);
                    }
                }

                pokemonInBattle *p = (i >= 0 && i < playerParty.count) ? playerParty.slots[i] : NULL;
                if (p == NULL) continue;

                // HP bar inside the slot.
                const int hpBarAdjustX = isSelected ? 0 : -1;   // unselected: 1 left
                const int hpBarAdjustY = isSelected ? 1 : -2;   // selected: 1 down, unselected: 2 up
                const int hpBarX = slotX + NON_SELECTED_POKEMON_HP_X + slotXOffsetForFrame + 1 + hpBarAdjustX;
                const int hpBarY = slotY + NON_SELECTED_POKEMON_HP_Y + colYOffset + slotYOffsetForFrame + 1 + hpBarAdjustY;
                const int hpBarW = NON_SELECTED_POKEMON_HP_WIDTH;
                const int hpBarH = NON_SELECTED_POKEMON_HP_HEIGHT;

                const int curHp = p->scaledStatsWithLevel[0];
                const int maxHp = (p->maxHp > 0) ? p->maxHp : 1;
                const int hpWidth = (hpBarW * curHp) / maxHp;
                const int hpPct = (curHp * 100) / maxHp;
                const short hpColour = (hpPct < 15) ? RED : ((hpPct < 50) ? ORANGE : GREEN);
                draw_rect(hpBarX, hpBarY, (hpWidth < 0) ? 0 : ((hpWidth > hpBarW) ? hpBarW : hpWidth), hpBarH, hpColour);

                // Status icon.
                const unsigned short *statusSprite = NULL;
                int statusW = 0, statusH = 0;
                switch (p->status) {
                    case STATUS_BURN: statusSprite = burned; statusW = BURNED_WIDTH; statusH = BURNED_HEIGHT; break;
                    case STATUS_POISON: statusSprite = poison; statusW = POISON_WIDTH; statusH = POISON_HEIGHT; break;
                    case STATUS_PARALYSIS: statusSprite = para; statusW = PARA_WIDTH; statusH = PARA_HEIGHT; break;
                    case STATUS_SLEEP: statusSprite = sleep; statusW = SLEEP_WIDTH; statusH = SLEEP_HEIGHT; break;
                    case STATUS_FREEZE: statusSprite = frozen; statusW = FROZEN_WIDTH; statusH = FROZEN_HEIGHT; break;
                    default: break;
                }
                if (statusSprite != NULL) {
                    draw_sprite_any(statusSprite,
                                    statusW, statusH,
                                    slotX + NON_SELECTED_POKEMON_STATUS_X + slotXOffsetForFrame,
                                    slotY + NON_SELECTED_POKEMON_STATUS_Y + colYOffset + slotYOffsetForFrame,
                                    TRANSPARENT_COLOUR);
                }

                // Pokemon sprite + name/level 
                const unsigned short *pokeSprite = menuPokemonSpriteForId(p->id.frontFrame_ID);
                const int pokeX = slotX + NON_SELECTED_POKEMON_X + 12 - 20 + slotXOffsetForFrame;
                const int pokeY = slotY + NON_SELECTED_POKEMON_Y + 14 + colYOffset + slotYOffsetForFrame - 20;
                if (pokeSprite != NULL) {
                    if (isSelected) {
                        draw_sprite_any_bob(pokeSprite,
                                            MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                            pokeX, pokeY,
                                            TRANSPARENT_COLOUR,
                                            bobFrame);
                    } else {
                        draw_sprite_any(pokeSprite,
                                        MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                        pokeX, pokeY,
                                        TRANSPARENT_COLOUR);
                    }
                }

                char lvlBuf[8];
                char hpCurBuf[8];
                char hpMaxBuf[8];
                snprintf(lvlBuf, sizeof(lvlBuf), "%d", p->level);
                snprintf(hpCurBuf, sizeof(hpCurBuf), "%d", curHp);
                snprintf(hpMaxBuf, sizeof(hpMaxBuf), "%d", maxHp);

                const int slotTextAdjustY = isSelected ? 2 : -1; // selected: 2 down, unselected: 1 up

                draw_string_f(slotX + POKEMON_TEXT_MENU_X + slotXOffsetForFrame,
                              slotY + POKEMON_TEXT_MENU_Y + colYOffset + slotYOffsetForFrame - 5,
                              (p->id.data != NULL && p->id.data->name != NULL) ? p->id.data->name : "???",
                              BLACK,
                              1);

                draw_string_f(slotX + NON_SELECTED_POKEMON_LEVEL_TEXT_X + slotXOffsetForFrame,
                              slotY + NON_SELECTED_POKEMON_LEVEL_TEXT_Y + colYOffset + slotYOffsetForFrame - 6 + slotTextAdjustY,
                              lvlBuf,
                              BLACK,
                              1);

                draw_string_f(slotX + NON_SELECTED_POKEMON_CURRENT_HP_TEXT_X - 2 + slotXOffsetForFrame,
                              slotY + NON_SELECTED_POKEMON_CURRENT_HP_TEXT_Y + colYOffset + slotYOffsetForFrame - 6 + slotTextAdjustY,
                              hpCurBuf,
                              BLACK,
                              1);
                draw_string_f(slotX + NON_SELECTED_POKEMON_MAX_HP_TEXT_X + slotXOffsetForFrame,
                              slotY + NON_SELECTED_POKEMON_MAX_HP_TEXT_Y + colYOffset + slotYOffsetForFrame - 6 + slotTextAdjustY,
                              hpMaxBuf,
                              BLACK,
                              1);
            }

            // Cancel button 
            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            const int cancelX = 232;
            const int cancelY = 188;
            if (menuCursor == 6) {
                draw_sprite_any_shade_pulse(cancelSprite,
                                            CANCEL_SPRITE_WIDTH, CANCEL_SPRITE_HEIGHT,
                                            cancelX, cancelY,
                                            TRANSPARENT_COLOUR,
                                            shadePulseFrame);
            } else {
                draw_sprite_any(cancelSprite,
                                CANCEL_SPRITE_WIDTH, CANCEL_SPRITE_HEIGHT,
                                cancelX, cancelY,
                                TRANSPARENT_COLOUR);
            }

            {
                char moneyBuf[32];
                snprintf(moneyBuf, sizeof(moneyBuf), "$%d", playerMoney);
                draw_string_f(39, 174, moneyBuf, BLACK, 1);
            }

            // Space selects a pokemon to swap; pressing Space on another swaps the two.
            if (escPressed) {
                if (menuSwapIndex >= 0) {
                    menuSwapIndex = -1;
                    play_sfx(plink_audio, plink_audio_len);
                } else {
                    currentGameState = partyMenuReturnState;
                    break;
                }
            }
            if (spacePressed) {
                if (menuCursor == 6) {
                    currentGameState = partyMenuReturnState;
                    menuSwapIndex = -1;
                    break;
                }
                if (menuCursor >= 0 && menuCursor < playerParty.count && playerParty.slots[menuCursor] != NULL) {
                    if (menuSwapIndex < 0) {
                        menuSwapIndex = menuCursor;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (menuSwapIndex == menuCursor) {
                        menuSwapIndex = -1; // cancel selection
                        play_sfx(plink_audio, plink_audio_len);
                    } else {
                        const int a = menuSwapIndex;
                        const int b = menuCursor;

                        pokemonInBattle *tmp = playerParty.slots[a];
                        playerParty.slots[a] = playerParty.slots[b];
                        playerParty.slots[b] = tmp;

                        if (playerParty.activeIndex == a) playerParty.activeIndex = b;
                        else if (playerParty.activeIndex == b) playerParty.activeIndex = a;

                        // Keep battle party box sprites consistent with the party order.
                        for (int k = 0; k < 6; k++) {
                            const pokemonInBattle *slotPokemon = (k >= 0 && k < playerParty.count) ? playerParty.slots[k] : NULL;
                            const int spriteId = (slotPokemon != NULL) ? slotPokemon->id.frontFrame_ID : 0;
                            setPokemonBoxSpriteId(&partyBoxSprites[k], spriteId);
                        }

                        menuSwapIndex = -1;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                }
            }
            break;
        }
        case GAME_STATE_MAIN_MENU_UI: {
            draw_map();
            draw_sprite_any(mainMenuUiBackdropMenuSprite,
                            MAIN_MENU_UI_BACKDROP_MENU_WIDTH, MAIN_MENU_UI_BACKDROP_MENU_HEIGHT,
                            0, 0,
                            TRANSPARENT_COLOUR);
            typedef enum {
                MENU_DIR_NONE = -1,
                MENU_DIR_N = 0,
                MENU_DIR_NE,
                MENU_DIR_E,
                MENU_DIR_SE,
                MENU_DIR_S,
                MENU_DIR_SW,
                MENU_DIR_W,
                MENU_DIR_NW,
            } MenuPulseDir;


            int heldX = 0;
            int heldY = 0;
            if (upDown && !downDown) heldY = -1;
            else if (downDown && !upDown) heldY = 1;
            if (leftDown && !rightDown) heldX = -1;
            else if (rightDown && !leftDown) heldX = 1;

            MenuPulseDir heldDir = MENU_DIR_NONE;
            if (heldX == 0 && heldY == -1) heldDir = MENU_DIR_N;
            else if (heldX == 1 && heldY == -1) heldDir = MENU_DIR_NE;
            else if (heldX == 1 && heldY == 0) heldDir = MENU_DIR_E;
            else if (heldX == 1 && heldY == 1) heldDir = MENU_DIR_SE;
            else if (heldX == 0 && heldY == 1) heldDir = MENU_DIR_S;
            else if (heldX == -1 && heldY == 1) heldDir = MENU_DIR_SW;
            else if (heldX == -1 && heldY == 0) heldDir = MENU_DIR_W;
            else if (heldX == -1 && heldY == -1) heldDir = MENU_DIR_NW;

            static MenuPulseDir lastPulseDir = MENU_DIR_NONE;
            static int lastPulseTimer = 0;
            const MenuPulseDir pulseVisualDir = (heldDir != MENU_DIR_NONE)
                ? heldDir
                : (lastPulseTimer > 0 ? lastPulseDir : MENU_DIR_NONE);

            if (heldDir != MENU_DIR_NONE) {
                lastPulseDir = heldDir;
                lastPulseTimer = 15;
            } else if (lastPulseTimer > 0) {
                lastPulseTimer--;
            }

            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;

            const bool pulseTop = (pulseVisualDir == MENU_DIR_N);
            const bool pulseTopLeft = (pulseVisualDir == MENU_DIR_NW);
            const bool pulseTopRight = (pulseVisualDir == MENU_DIR_NE);
            const bool pulseLeft = (pulseVisualDir == MENU_DIR_W);
            const bool pulseRight = (pulseVisualDir == MENU_DIR_E);
            const bool pulseBottomLeft = (pulseVisualDir == MENU_DIR_SW);
            const bool pulseBottomRight = (pulseVisualDir == MENU_DIR_SE);
            const bool pulseBottom = (pulseVisualDir == MENU_DIR_S);

            if (pulseLeft) {
                draw_sprite_any_shade_pulse(mainMenuUiPokemonLeftSprite, MAIN_MENU_UI_POKEMON_LEFT_WIDTH, MAIN_MENU_UI_POKEMON_LEFT_HEIGHT, 3, 70, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(mainMenuUiPokemonLeftSprite, MAIN_MENU_UI_POKEMON_LEFT_WIDTH, MAIN_MENU_UI_POKEMON_LEFT_HEIGHT, 3, 70, TRANSPARENT_COLOUR);
            }
            if (pulseRight) {
                draw_sprite_any_shade_pulse(mainMenuUiPokemonRightSprite, MAIN_MENU_UI_POKEMON_RIGHT_WIDTH, MAIN_MENU_UI_POKEMON_RIGHT_HEIGHT, 227, 70, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(mainMenuUiPokemonRightSprite, MAIN_MENU_UI_POKEMON_RIGHT_WIDTH, MAIN_MENU_UI_POKEMON_RIGHT_HEIGHT, 227, 70, TRANSPARENT_COLOUR);
            }

            draw_sprite_any(mainMenuUiMenuBorderSprite,
                            MAIN_MENU_UI_MENU_BORDER_WIDTH, MAIN_MENU_UI_MENU_BORDER_HEIGHT,
                            0, 0,
                            TRANSPARENT_COLOUR);

            if (pulseBottomLeft) {
                draw_sprite_any_shade_pulse(mainMenuUiUi3dsMenuSprite, MAIN_MENU_UI_UI3DS_MENU_WIDTH, MAIN_MENU_UI_UI3DS_MENU_HEIGHT, 82, 113, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(mainMenuUiUi3dsMenuSprite, MAIN_MENU_UI_UI3DS_MENU_WIDTH, MAIN_MENU_UI_UI3DS_MENU_HEIGHT, 82, 113, TRANSPARENT_COLOUR);
            }
            if (pulseTopLeft) {
                draw_sprite_any_shade_pulse(mainMenuUiBagMenuSprite, MAIN_MENU_UI_BAG_MENU_WIDTH, MAIN_MENU_UI_BAG_MENU_HEIGHT, 82, 58, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(mainMenuUiBagMenuSprite, MAIN_MENU_UI_BAG_MENU_WIDTH, MAIN_MENU_UI_BAG_MENU_HEIGHT, 82, 58, TRANSPARENT_COLOUR);
            }
            if (pulseTop) {
                draw_sprite_any_shade_pulse(mainMenuUiBallMenuSprite, MAIN_MENU_UI_BALL_MENU_WIDTH, MAIN_MENU_UI_BALL_MENU_HEIGHT, 130, 29, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(mainMenuUiBallMenuSprite, MAIN_MENU_UI_BALL_MENU_WIDTH, MAIN_MENU_UI_BALL_MENU_HEIGHT, 130, 29, TRANSPARENT_COLOUR);
            }
            if (pulseTopRight) {
                draw_sprite_any_shade_pulse(mainMenuUiDexMenuSprite, MAIN_MENU_UI_DEX_MENU_WIDTH, MAIN_MENU_UI_DEX_MENU_HEIGHT, 178, 58, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(mainMenuUiDexMenuSprite, MAIN_MENU_UI_DEX_MENU_WIDTH, MAIN_MENU_UI_DEX_MENU_HEIGHT, 178, 58, TRANSPARENT_COLOUR);
            }
            draw_sprite_any(mainMenuUiFrontFacingMcSprite, MAIN_MENU_UI_FRONT_FACING_MC_WIDTH, MAIN_MENU_UI_FRONT_FACING_MC_HEIGHT, 128, 80, TRANSPARENT_COLOUR);

            // Party sprites beside the left/right (W/E) icons: 3 on the left, 3 on the right.

            //239,97 for 4 in right
            //278,81 for 5 in right 
            //278, 111 for 6 in right

            //-2, 79 in left for 2
            //-2, 110 in left for 3
            //41,94 in left for 1
            {

                static const int partySpriteX[6] = {
                    41,  // 1 (left)
                    -2 + 5,  // 2 (left)
                    -2 + 5,  // 3 (left)
                    239, // 4 (right)
                    278 - 2, // 5 (right)
                    278 - 2, // 6 (right)
                };
                static const int partySpriteY[6] = {
                    94,  // 1 (left)
                    79,  // 2 (left)
                    110, // 3 (left)
                    97,  // 4 (right)
                    81,  // 5 (right)
                    111, // 6 (right)
                };

                for (int i = 0; i < 6; i++) {
                    pokemonInBattle *p = (i >= 0 && i < playerParty.count) ? playerParty.slots[i] : NULL;
                    if (p == NULL) continue;
                    const unsigned short *spr = menuPokemonSpriteForId(p->id.frontFrame_ID);
                    if (spr == NULL) continue;

                    const bool leftSide = (i < 3);
                    const int x = partySpriteX[i];
                    const int y = partySpriteY[i];

                    const bool pulseSide = leftSide ? pulseLeft : pulseRight;
                    if (!p->alive) {
                        draw_sprite_any_greyscale(spr, MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT, x, y, TRANSPARENT_COLOUR);
                    } else if (pulseSide) {
                        draw_sprite_any_shade_pulse(spr, MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT, x, y, TRANSPARENT_COLOUR, shadePulseFrame);
                    } else {
                        draw_sprite_any(spr, MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT, x, y, TRANSPARENT_COLOUR);
                    }
                }
            }

            if (pulseBottom) {
                draw_sprite_any_shade_pulse(mainMenuUiSaveMenuSprite, MAIN_MENU_UI_SAVE_MENU_WIDTH, MAIN_MENU_UI_SAVE_MENU_HEIGHT, 130, 141, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(mainMenuUiSaveMenuSprite, MAIN_MENU_UI_SAVE_MENU_WIDTH, MAIN_MENU_UI_SAVE_MENU_HEIGHT, 130, 141, TRANSPARENT_COLOUR);
            }
            if (pulseBottomRight) {
                draw_sprite_any_shade_pulse(mainMenuUiTrainerCardMenuSprite, MAIN_MENU_UI_TRAINER_CARD_MENU_WIDTH, MAIN_MENU_UI_TRAINER_CARD_MENU_HEIGHT, 178, 113, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(mainMenuUiTrainerCardMenuSprite, MAIN_MENU_UI_TRAINER_CARD_MENU_WIDTH, MAIN_MENU_UI_TRAINER_CARD_MENU_HEIGHT, 178, 113, TRANSPARENT_COLOUR);
            }

            if (escPressed) {
                currentGameState = GAME_STATE_MAP;
            } else if (spacePressed) {
                const MenuPulseDir selectionDir = (heldDir != MENU_DIR_NONE)
                    ? heldDir
                    : (lastPulseTimer > 0 ? lastPulseDir : MENU_DIR_NONE);

                switch (selectionDir) {
                    case MENU_DIR_NW: // Bag
                        play_sfx(plink_audio, plink_audio_len);
                        currentGameState = GAME_STATE_BAG_MENU_ITEMS;
                        bagMenuReturnState = GAME_STATE_MAIN_MENU_UI;
                        bagMenuCursor = 0;
                        bagMenuFocus = BAG_FOCUS_LIST;
                        break;
                    case MENU_DIR_N: // Ball (Party menu)
                        play_sfx(plink_audio, plink_audio_len);
                        currentGameState = GAME_STATE_MENU;
                        menuCursor = 0;
                        menuSwapIndex = -1;
                        partyMenuReturnState = GAME_STATE_MAIN_MENU_UI;
                        break;
                    case MENU_DIR_NE: // Dex
                        currentGameState = GAME_STATE_POKEDEX_MENU;
                        pokedexScrollIndex = POKEMON_ID_CHARMANDER;
                        pokedexSelectedId = POKEMON_ID_CHARMANDER;
                        pokedexMenuReturnState = GAME_STATE_MAIN_MENU_UI;
                        play_sfx(pc_se_audio, pc_se_audio_len);
                        break;
                    case MENU_DIR_W: // PC
                    case MENU_DIR_E: // PC
                        currentGameState = GAME_STATE_PC_MENU;
                        pcCursor = 0;
                        pcSwapIndex = -1;
                        pcHeldMon = NULL;
                        pcMenuReturnState = GAME_STATE_MAIN_MENU_UI;
                        play_sfx(pc_se_audio, pc_se_audio_len);
                        break;
                    case MENU_DIR_SW: //placeholder
                        break;
                    case MENU_DIR_S: //placeholder
                        break;
                    case MENU_DIR_SE: // Trainer card
                        play_sfx(plink_audio, plink_audio_len);
                        currentGameState = GAME_STATE_TRAINER_CARD_FRONT;
                        break;
                    case MENU_DIR_NONE:
                    default:
                        play_sfx(plink_audio, plink_audio_len);
                        currentGameState = GAME_STATE_MAP;
                        break;
                }
            }
            break;
        }
        case GAME_STATE_TRAINER_CARD_FRONT: {
            draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            draw_sprite_any(trainerCardSprite,
                            TRAINER_CARD_SPRITE_WIDTH, TRAINER_CARD_SPRITE_HEIGHT,
                            0, 0,
                            TRANSPARENT_COLOUR);

            draw_string_f(55, 80 -5, "Name:", BLACK, FONT_5X9);
            draw_string_f(55, 96 - 5, "$     ", BLACK, FONT_5X9);
            draw_string_f(55, 112 - 5, "Pokedex: ", BLACK, FONT_5X9);
            draw_string_f(55, 128 - 5, "Time Played: ", BLACK, FONT_5X9);
            draw_string_f(55, 152 - 5, "Location: ", BLACK, FONT_5X9);

            {
                const char *name = getUserText();
                if (name == NULL || name[0] == '\0') name = "???";
                draw_string_f(120, 80 -5, name, BLACK, FONT_5X9);

                char moneyNumBuf[16];
                snprintf(moneyNumBuf, sizeof(moneyNumBuf), "%d", playerMoney);
                draw_string_f(65 + 5, 96 - 5, moneyNumBuf, BLACK, FONT_5X9);

                int seenCount = 0;
                int caughtCount = 0;
                for (int i = 0; i <= POKEMON_ID_TOGEKISS; i++) {
                    if (g_pokedexSeen[i]) seenCount++;
                    if (g_pokedexCaught[i]) caughtCount++;
                }
                char dexBuf[32];
                snprintf(dexBuf, sizeof(dexBuf), "C:%d S:%d", caughtCount, seenCount);
                draw_string_f(120, 112 - 5, dexBuf, BLACK, FONT_5X9);

                const unsigned int totalSeconds = (GAME_FPS > 0) ? (playTimeFrames / GAME_FPS) : 0;
                const unsigned int hours = totalSeconds / 3600U;
                const unsigned int minutes = (totalSeconds % 3600U) / 60U;
                const unsigned int seconds = totalSeconds % 60U;
                char timeBuf[24];
                snprintf(timeBuf, sizeof(timeBuf), "%02u:%02u:%02u", hours, minutes, seconds);
                draw_string_f(125, 152 - 4, timeBuf, BLACK, FONT_5X9);

                const char *location = world_map_display_name(currentMapId);
                if (location == NULL) location = "Unknown";
                draw_string_f(120, 128 - 4, location, BLACK, FONT_5X9);
            }
            if (escPressed) {
                currentGameState = GAME_STATE_MAIN_MENU_UI;
            } else if (spacePressed) {
                currentGameState = GAME_STATE_TRAINER_CARD_BACK;
            }
            break;
        }
        case GAME_STATE_TRAINER_CARD_BACK: {
            draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            draw_sprite_any(trainerCardBackSprite,
                            TRAINER_CARD_BACK_SPRITE_WIDTH, TRAINER_CARD_BACK_SPRITE_HEIGHT,
                            0, 0,
                            TRANSPARENT_COLOUR);

            // Party Pokemon positions on the back of the trainer card.
            static const int partyX[6] = { 67, 127, 187, 86, 146, 206 };
            static const int partyY[6] = { 52,  52,  52,  85,  85,  85 };
            for (int i = 0; i < 6; i++) {
                pokemonInBattle *p = (i >= 0 && i < playerParty.count) ? playerParty.slots[i] : NULL;
                if (p == NULL) continue;

                const unsigned short *spr = menuPokemonSpriteForId(p->id.frontFrame_ID);
                if (spr == NULL) continue;

                if (!p->alive) {
                    draw_sprite_any_greyscale(spr,
                                              MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                              partyX[i], partyY[i],
                                              TRANSPARENT_COLOUR);
                } else {
                    draw_sprite_any(spr,
                                    MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                    partyX[i], partyY[i],
                                    TRANSPARENT_COLOUR);
                }
            }
            if (escPressed) {
                currentGameState = GAME_STATE_MAIN_MENU_UI;
            } else if (spacePressed) {
                currentGameState = GAME_STATE_TRAINER_CARD_FRONT;
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
                            // timing hp and animate hits.
                            pokemonInBattle *p = getActivePokemon(&playerParty);
                            pokemonInBattle *e = getActivePokemon(&enemyParty);
                            actionTextShownPlayerHp = (p != NULL) ? p->scaledStatsWithLevel[0] : -1;
                            actionTextShownEnemyHp = (e != NULL) ? e->scaledStatsWithLevel[0] : -1;
                            actionTextTargetPlayerHp = actionTextShownPlayerHp;
                            actionTextTargetEnemyHp = actionTextShownEnemyHp;

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
                            // Switching should immediately show the new active Pokemon's HUD values.
                            // Reset the action-text snapshots so they re-initialize from the displayed Pokemon.
                            actionTextShownPlayerHp = -1;
                            actionTextShownEnemyHp = -1;
                            actionTextTargetPlayerHp = -1;
                            actionTextTargetEnemyHp = -1;
                            actionTextDisplayPlayerIndex = -1;
                            actionTextDisplayEnemyIndex = -1;
                            actionTextShownLevel = -1;
                            actionTextShownExp = -1;
                            actionTextTargetLevel = -1;
                            actionTextTargetExp = -1;
                            actionTextExpAnimating = false;

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
                        // animate hits.
                        pokemonInBattle *pActive = getActivePokemon(&playerParty);
                        pokemonInBattle *eActive = getActivePokemon(&enemyParty);
                        actionTextShownPlayerHp = (pActive != NULL) ? pActive->scaledStatsWithLevel[0] : -1;
                        actionTextShownEnemyHp = (eActive != NULL) ? eActive->scaledStatsWithLevel[0] : -1;
                        actionTextTargetPlayerHp = actionTextShownPlayerHp;
                        actionTextTargetEnemyHp = actionTextShownEnemyHp;
                        actionTextExpStartLevel = (pActive != NULL) ? pActive->level : -1;
                        actionTextExpStartExp = (pActive != NULL) ? pActive->exp : -1;
                        actionTextShownLevel = actionTextExpStartLevel;
                        actionTextShownExp = actionTextExpStartExp;
                        actionTextTargetLevel = actionTextShownLevel;
                        actionTextTargetExp = actionTextShownExp;
                        actionTextExpAnimating = false;

                        // Defer applying the action until GAME_STATE_BATTLE_ACTION_TEXT (prevents one-frame HP jump).
                        battleActionPending = true;
                        pendingBattleAction = ACTION_ATTACK;
                        pendingBattleParam = moveIndex;
                        play_sfx(plink_audio, plink_audio_len);

                        // Show attack + faint + EXP messages 
                        actionTextReturnUi = BATTLE_UI_MENU;
                        actionTextReturnCursor = 0;
                        actionTextReturnGameState = activeBattleMenuState;
                        currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                        previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                        actionTextAwaitSpaceRelease = true;

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
                                // animate hits.
                                pokemonInBattle *pActive = getActivePokemon(&playerParty);
                                pokemonInBattle *eActive = getActivePokemon(&enemyParty);
                                actionTextShownPlayerHp = (pActive != NULL) ? pActive->scaledStatsWithLevel[0] : -1;
                                actionTextShownEnemyHp = (eActive != NULL) ? eActive->scaledStatsWithLevel[0] : -1;
                                actionTextTargetPlayerHp = actionTextShownPlayerHp;
                                actionTextTargetEnemyHp = actionTextShownEnemyHp;
                                actionTextExpStartLevel = (pActive != NULL) ? pActive->level : -1;
                                actionTextExpStartExp = (pActive != NULL) ? pActive->exp : -1;
                                actionTextShownLevel = actionTextExpStartLevel;
                                actionTextShownExp = actionTextExpStartExp;
                                actionTextTargetLevel = actionTextShownLevel;
                                actionTextTargetExp = actionTextShownExp;
                                actionTextExpAnimating = false;

                                if (itemIsRevive(last)) {
                                    itemTargetItem = last;
                                    itemTargetIndex = 0;
                                    itemTargetReturnUi = battleUi;
                                    itemTargetReturnCursor = battleCursor;
                                    itemTargetReturnPage = 0;
                                    currentGameState = GAME_STATE_BATTLE_ITEM_TARGET;
                                    previousGameState = GAME_STATE_BATTLE_ITEM_TARGET;
                                } else {
                                    battleActionPending = true;
                                    pendingBattleAction = ACTION_ITEM;
                                    pendingBattleParam = (int)last;
                                    battleUi = BATTLE_UI_MENU;
                                    battleCursor = 0;

                                    actionTextReturnUi = BATTLE_UI_MENU;
                                    actionTextReturnCursor = 0;
                                    actionTextReturnGameState = activeBattleMenuState;
                                    currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                    previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                    actionTextAwaitSpaceRelease = true;
                                }
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
                            //  animate hits.
                            pokemonInBattle *p = getActivePokemon(&playerParty);
                            pokemonInBattle *e = getActivePokemon(&enemyParty);
                            actionTextShownPlayerHp = (p != NULL) ? p->scaledStatsWithLevel[0] : -1;
                            actionTextShownEnemyHp = (e != NULL) ? e->scaledStatsWithLevel[0] : -1;
                            actionTextTargetPlayerHp = actionTextShownPlayerHp;
                            actionTextTargetEnemyHp = actionTextShownEnemyHp;
                            actionTextExpStartLevel = (p != NULL) ? p->level : -1;
                            actionTextExpStartExp = (p != NULL) ? p->exp : -1;
                            actionTextShownLevel = actionTextExpStartLevel;
                            actionTextShownExp = actionTextExpStartExp;
                            actionTextTargetLevel = actionTextShownLevel;
                            actionTextTargetExp = actionTextShownExp;
                            actionTextExpAnimating = false;

                            if (itemIsRevive(item)) {
                                itemTargetItem = item;
                                itemTargetIndex = 0;
                                itemTargetReturnUi = bagDescReturnUi;
                                itemTargetReturnCursor = bagDescReturnCursor;
                                itemTargetReturnPage = bagDescReturnPage;
                                currentGameState = GAME_STATE_BATTLE_ITEM_TARGET;
                                previousGameState = GAME_STATE_BATTLE_ITEM_TARGET;
                            } else {
                                battleActionPending = true;
                                pendingBattleAction = ACTION_ITEM;
                                pendingBattleParam = (int)item;
                                battleUi = BATTLE_UI_MENU;
                                battleCursor = 0;

                                actionTextReturnUi = BATTLE_UI_MENU;
                                actionTextReturnCursor = 0;
                                actionTextReturnGameState = activeBattleMenuState;
                                currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                                actionTextAwaitSpaceRelease = true;
                            }
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
                    autoSwapLeadIfFainted(&playerParty, partyBoxSprites);
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
            if (playerHitShakeFrame == -2) {
                playerHitShakeFrame = 0;
            } else if (playerHitShakeFrame >= 0) {
                playerHitShakeFrame++;
                if (playerHitShakeFrame >= SHAKE_SPRITE_FRAME_COUNT) playerHitShakeFrame = -1;
            }
            if (enemyHitShakeFrame == -2) {
                enemyHitShakeFrame = 0;
            } else if (enemyHitShakeFrame >= 0) {
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

            pokemonInBattle *enemyActive = (battleState.enemyParty != NULL && actionTextDisplayEnemyIndex >= 0 && actionTextDisplayEnemyIndex < battleState.enemyParty->count)
                                               ? battleState.enemyParty->slots[actionTextDisplayEnemyIndex]
                                               : ((battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL);
            pokemonInBattle *playerActive = (battleState.playerParty != NULL && actionTextDisplayPlayerIndex >= 0 && actionTextDisplayPlayerIndex < battleState.playerParty->count)
                                                ? battleState.playerParty->slots[actionTextDisplayPlayerIndex]
                                                : ((battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL);

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
            //draw_sprite_any(burned, BURNED_WIDTH, BURNED_HEIGHT, STATUS_X, STATUS_Y, TRANSPARENT_COLOUR);
            //draw_sprite_any(caught, CAUGHT_WIDTH, CAUGHT_HEIGHT, CAUGHT_X, CAUGHT_Y, TRANSPARENT_COLOUR);
            draw_string_f(OPPNAME_X, OPPNAME_Y, (enemyActive != NULL && enemyActive->id.data != NULL) ? enemyActive->id.data->name : "???", BLACK, 1);

            //got the bobbing dy pattern array to move hp bar, name, level, etc from myHP bar 
            static const signed char dy_pattern[BOB_SPRITE_FRAME_COUNT] = {
            0, -1, -1, 0, 0, 1, 1, 0,
            0, -1, -1, 0, 0, 1, 1, 0 };
            int offsetY = dy_pattern[bobFrame];
            //my hp bar
            draw_rect(myHP_X, myHP_Y + offsetY, (playerHpBarWidth < 0) ? 0 : ((playerHpBarWidth > HP_WIDTH) ? HP_WIDTH : playerHpBarWidth), HP_HEIGHT, playerHpBarColour);
            draw_rect(EXP_X, EXP_Y + offsetY, expBarWidthFor(playerActive), EXP_HEIGHT, TURQ);
            draw_string_f(myLVL_X, myLVL_Y + offsetY, myLvlBuf, BLACK, 1);
            draw_string_f(MYNAME_X, MYNAME_Y + offsetY, (playerActive != NULL && playerActive->id.data != NULL) ? playerActive->id.data->name : "???", BLACK, 1);

            draw_string_f(TOTAL_HPNUM_X, HPNUM_Y + offsetY, myHpCurBuf, BLACK, 1);
            draw_string_f(REMAINING_HP_X, HPNUM_Y + offsetY, myHpMaxBuf, BLACK, 1);

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
                    else if (item == ITEM_REVIVE) { icon = healingItemIcon_revive; iconW = HEALING_ITEM_ICON_WIDTH; iconH = HEALING_ITEM_ICON_HEIGHT; }
                    else if (item == ITEM_MAX_REVIVE) { icon = healingItemIcon_maxRevive; iconW = HEALING_ITEM_ICON_WIDTH; iconH = HEALING_ITEM_ICON_HEIGHT; }

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
                                 else if (item == ITEM_REVIVE) icon = healingItemIcon_revive;
                                 else if (item == ITEM_MAX_REVIVE) icon = healingItemIcon_maxRevive;

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
                         else if (bagDescItem == ITEM_REVIVE) itemIcon = healingItemIcon_revive;
                         else if (bagDescItem == ITEM_MAX_REVIVE) itemIcon = healingItemIcon_maxRevive;
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

            // Apply the pending battle action now 
            if (battleActionPending) {
                battleActionPending = false;
                battleApplyPlayerAction(&battleState, pendingBattleAction, pendingBattleParam);
                syncBattleSprites(&battleState, &playerBackSprite, &enemyFrontSprite);
                // Force the action-text message-change logic to run for message 0.
                actionTextLastMsgIndex = -1;
            }

            // Current message 
            const char *msg = (battleState.messageCount > 0 && battleState.messageReadIndex >= 0 && battleState.messageReadIndex < battleState.messageCount)
                                  ? battleState.messages[battleState.messageReadIndex]
                                  : "";

            // Default HP animation 
            pokemonInBattle *enemyActiveForAnim = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
            pokemonInBattle *playerActiveForAnim = (battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL;
            const int actualEnemyHpForAnim = (enemyActiveForAnim != NULL) ? enemyActiveForAnim->scaledStatsWithLevel[0] : 0;
            const int actualPlayerHpForAnim = (playerActiveForAnim != NULL) ? playerActiveForAnim->scaledStatsWithLevel[0] : 0;

            if (actionTextShownEnemyHp < 0) actionTextShownEnemyHp = actualEnemyHpForAnim;
            if (actionTextShownPlayerHp < 0) actionTextShownPlayerHp = actualPlayerHpForAnim;
            if (actionTextTargetEnemyHp < 0) actionTextTargetEnemyHp = actionTextShownEnemyHp;
            if (actionTextTargetPlayerHp < 0) actionTextTargetPlayerHp = actionTextShownPlayerHp;

            if (actionTextShownLevel < 0) actionTextShownLevel = (playerActiveForAnim != NULL) ? playerActiveForAnim->level : -1;
            if (actionTextShownExp < 0) actionTextShownExp = (playerActiveForAnim != NULL) ? playerActiveForAnim->exp : 0;
            if (actionTextTargetLevel < 0) actionTextTargetLevel = actionTextShownLevel;
            if (actionTextTargetExp < 0) actionTextTargetExp = actionTextShownExp;

            // Update HP targets + hit shake exactly when the message changes.
            if (battleState.messageCount <= 0) {
                actionTextAutoTimer = 0;
                actionTextLastMsgIndex = -1;
                actionTextShownEnemyHp = actualEnemyHpForAnim;
                actionTextShownPlayerHp = actualPlayerHpForAnim;
                actionTextTargetEnemyHp = actionTextShownEnemyHp;
                actionTextTargetPlayerHp = actionTextShownPlayerHp;
                actionTextDisplayPlayerIndex = (battleState.playerParty != NULL) ? battleState.playerParty->activeIndex : -1;
                actionTextDisplayEnemyIndex = (battleState.enemyParty != NULL) ? battleState.enemyParty->activeIndex : -1;
            } else if (battleState.messageReadIndex != actionTextLastMsgIndex) {
                actionTextAutoTimer = 0;
                actionTextLastMsgIndex = battleState.messageReadIndex;

                const int idx = battleState.messageReadIndex;
                if (idx >= 0 && idx < BATTLE_MSG_MAX) {
                    const int newDispP = (battleState.displayPlayerIndex[idx] >= 0) ? battleState.displayPlayerIndex[idx] : ((battleState.playerParty != NULL) ? battleState.playerParty->activeIndex : -1);
                    const int newDispE = (battleState.displayEnemyIndex[idx] >= 0) ? battleState.displayEnemyIndex[idx] : ((battleState.enemyParty != NULL) ? battleState.enemyParty->activeIndex : -1);

                    if (newDispP != actionTextDisplayPlayerIndex) {
                        const bool wasUninitialized = (actionTextDisplayPlayerIndex < 0);
                        actionTextDisplayPlayerIndex = newDispP;
                        // Don't clobber the initial pre-hit HP snapshot; only reset HP when a real switch happens.
                        if (!wasUninitialized || actionTextShownPlayerHp < 0) {
                            pokemonInBattle *p = (battleState.playerParty != NULL && newDispP >= 0 && newDispP < battleState.playerParty->count)
                                                     ? battleState.playerParty->slots[newDispP]
                                                     : NULL;
                            const int hp = (p != NULL) ? p->scaledStatsWithLevel[0] : actualPlayerHpForAnim;
                            actionTextShownPlayerHp = hp;
                            actionTextTargetPlayerHp = hp;
                        }
                    }
                    if (newDispE != actionTextDisplayEnemyIndex) {
                        const bool wasUninitialized = (actionTextDisplayEnemyIndex < 0);
                        actionTextDisplayEnemyIndex = newDispE;
                        if (!wasUninitialized || actionTextShownEnemyHp < 0) {
                            pokemonInBattle *e = (battleState.enemyParty != NULL && newDispE >= 0 && newDispE < battleState.enemyParty->count)
                                                     ? battleState.enemyParty->slots[newDispE]
                                                     : NULL;
                            const int hp = (e != NULL) ? e->scaledStatsWithLevel[0] : actualEnemyHpForAnim;
                            actionTextShownEnemyHp = hp;
                            actionTextTargetEnemyHp = hp;
                        }
                    }

                    if (battleState.hpAfterPlayer[idx] >= 0) actionTextTargetPlayerHp = battleState.hpAfterPlayer[idx];
                    if (battleState.hpAfterEnemy[idx] >= 0) actionTextTargetEnemyHp = battleState.hpAfterEnemy[idx];

                    // Start EXP animation on the EXP message (use the pre-action snapshot as the start).
                    if (msg != NULL && strstr(msg, "Gained ") != NULL && strstr(msg, " EXP") != NULL) {
                        const int dispP = (battleState.displayPlayerIndex[idx] >= 0) ? battleState.displayPlayerIndex[idx] : ((battleState.playerParty != NULL) ? battleState.playerParty->activeIndex : -1);
                        pokemonInBattle *p = (battleState.playerParty != NULL && dispP >= 0 && dispP < battleState.playerParty->count) ? battleState.playerParty->slots[dispP] : NULL;
                        const int tgtL = (p != NULL) ? p->level : actionTextShownLevel;
                        const int tgtE = (p != NULL) ? p->exp : actionTextShownExp;

                        actionTextShownLevel = (actionTextExpStartLevel > 0) ? actionTextExpStartLevel : tgtL;
                        actionTextShownExp = (actionTextExpStartExp >= 0) ? actionTextExpStartExp : tgtE;
                        actionTextTargetLevel = tgtL;
                        actionTextTargetExp = tgtE;
                        actionTextExpAnimating = true;
                    }

                    const bool playerHpChanged = (battleState.hpAfterPlayer[idx] >= 0);
                    const bool enemyHpChanged = (battleState.hpAfterEnemy[idx] >= 0);
                    if (battleState.hitEffects[idx] == (signed char)BATTLE_HIT_EFFECT_STATS_UP) {
                        play_sfx(stats_up_audio, stats_up_audio_len);
                    } else if (battleState.hitEffects[idx] == (signed char)BATTLE_HIT_EFFECT_STATS_DOWN) {
                        play_sfx(stats_fall_audio, stats_fall_audio_len);
                    } else if (playerHpChanged || enemyHpChanged) {
                        if (battleState.hitEffects[idx] == (signed char)BATTLE_HIT_EFFECT_SUPER_EFFECTIVE) {
                            play_sfx(super_effective_audio, super_effective_audio_len);
                        } else if (battleState.hitEffects[idx] == (signed char)BATTLE_HIT_EFFECT_NOT_EFFECTIVE) {
                            play_sfx(not_effective_audio, not_effective_audio_len);
                        }
                    }
                    if (playerHpChanged || enemyHpChanged) {
                        if (playerHpChanged) playerHitShakeFrame = -2; // start shake next frame at 0
                        if (enemyHpChanged) enemyHitShakeFrame = -2;
                    }
                }
            }

            // Animate EXP with level-up wrap (Pokémon-like).
            if (actionTextExpAnimating) {
                const bool done = (actionTextShownLevel == actionTextTargetLevel) && (actionTextShownExp == actionTextTargetExp);
                if (!done) {
                    const int needed = expRequiredAtLevel(actionTextShownLevel);
                    int stepExp = (needed > 0) ? (needed / 16) : 1;
                    if (stepExp < 1) stepExp = 1;

                    for (int s = 0; s < stepExp; s++) {
                        if (actionTextShownLevel < actionTextTargetLevel) {
                            const int need = expRequiredAtLevel(actionTextShownLevel);
                            if (need > 0 && actionTextShownExp + 1 >= need) {
                                actionTextShownLevel++;
                                actionTextShownExp = 0;
                            } else {
                                actionTextShownExp++;
                            }
                        } else if (actionTextShownLevel == actionTextTargetLevel) {
                            if (actionTextShownExp < actionTextTargetExp) actionTextShownExp++;
                            else if (actionTextShownExp > actionTextTargetExp) actionTextShownExp--;
                        }

                        if ((actionTextShownLevel == actionTextTargetLevel) && (actionTextShownExp == actionTextTargetExp)) break;
                    }
                } else {
                    actionTextExpAnimating = false;
                }
            }

            // Ensure sprites match the displayed pokemon for the current message (prevents early auto-switch visuals).
            {
                const int idx = battleState.messageReadIndex;
                const int dispP = (idx >= 0 && idx < BATTLE_MSG_MAX && battleState.displayPlayerIndex[idx] >= 0)
                                      ? battleState.displayPlayerIndex[idx]
                                      : actionTextDisplayPlayerIndex;
                const int dispE = (idx >= 0 && idx < BATTLE_MSG_MAX && battleState.displayEnemyIndex[idx] >= 0)
                                      ? battleState.displayEnemyIndex[idx]
                                      : actionTextDisplayEnemyIndex;
                if (battleState.playerParty != NULL && dispP >= 0 && dispP < battleState.playerParty->count && battleState.playerParty->slots[dispP] != NULL) {
                    setPokemonBackBattleSpriteId(&playerBackSprite, battleState.playerParty->slots[dispP]->id.backFrame_ID);
                }
                if (battleState.enemyParty != NULL && dispE >= 0 && dispE < battleState.enemyParty->count && battleState.enemyParty->slots[dispE] != NULL) {
                    setPokemonFrontBattleSpriteId(&enemyFrontSprite, battleState.enemyParty->slots[dispE]->id.frontFrame_ID);
                }
            }

            // Animate HP towards targets so damage doesn't appear  at once
            {
                const int step = 2;
                if (actionTextShownPlayerHp > actionTextTargetPlayerHp) {
                    int delta = actionTextShownPlayerHp - actionTextTargetPlayerHp;
                    if (delta > step) delta = step;
                    actionTextShownPlayerHp -= delta;
                } else if (actionTextShownPlayerHp < actionTextTargetPlayerHp) {
                    int delta = actionTextTargetPlayerHp - actionTextShownPlayerHp;
                    if (delta > step) delta = step;
                    actionTextShownPlayerHp += delta;
                }

                if (actionTextShownEnemyHp > actionTextTargetEnemyHp) {
                    int delta = actionTextShownEnemyHp - actionTextTargetEnemyHp;
                    if (delta > step) delta = step;
                    actionTextShownEnemyHp -= delta;
                } else if (actionTextShownEnemyHp < actionTextTargetEnemyHp) {
                    int delta = actionTextTargetEnemyHp - actionTextShownEnemyHp;
                    if (delta > step) delta = step;
                    actionTextShownEnemyHp += delta;
                }
            }

            // Animations (battle only).
            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            shakeTimer++;
            if (shakeTimer >= shakeSpeedFrames) {
                shakeTimer = 0;
                shakeFrame = (shakeFrame + 1) % SHAKE_SPRITE_FRAME_COUNT;
            }
            if (playerHitShakeFrame == -2) {
                playerHitShakeFrame = 0;
            } else if (playerHitShakeFrame >= 0) {
                playerHitShakeFrame++;
                if (playerHitShakeFrame >= SHAKE_SPRITE_FRAME_COUNT) playerHitShakeFrame = -1;
            }
            if (enemyHitShakeFrame == -2) {
                enemyHitShakeFrame = 0;
            } else if (enemyHitShakeFrame >= 0) {
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
                    pokemonInBattle *enemyActive = (battleState.enemyParty != NULL && actionTextDisplayEnemyIndex >= 0 && actionTextDisplayEnemyIndex < battleState.enemyParty->count)
                                                       ? battleState.enemyParty->slots[actionTextDisplayEnemyIndex]
                                                       : ((battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL);
                    pokemonInBattle *playerActive = (battleState.playerParty != NULL && actionTextDisplayPlayerIndex >= 0 && actionTextDisplayPlayerIndex < battleState.playerParty->count)
                                                        ? battleState.playerParty->slots[actionTextDisplayPlayerIndex]
                                                        : ((battleState.playerParty != NULL) ? getActivePokemon(battleState.playerParty) : NULL);

                    const int enemyHp = (actionTextShownEnemyHp >= 0) ? actionTextShownEnemyHp : ((enemyActive != NULL) ? enemyActive->scaledStatsWithLevel[0] : 0);
                    const int enemyMaxHp = (enemyActive != NULL) ? enemyActive->maxHp : 1;
                    const int playerHp = (actionTextShownPlayerHp >= 0) ? actionTextShownPlayerHp : ((playerActive != NULL) ? playerActive->scaledStatsWithLevel[0] : 0);
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
                    draw_string_f(OPPNAME_X, OPPNAME_Y, (enemyActive != NULL && enemyActive->id.data != NULL) ? enemyActive->id.data->name : "???", BLACK, 1);

                    static const signed char dy_pattern[BOB_SPRITE_FRAME_COUNT] = {
                    0, -1, -1, 0, 0, 1, 1, 0,
                    0, -1, -1, 0, 0, 1, 1, 0 };
                    int offsetY = dy_pattern[bobFrame];
                    draw_rect(myHP_X, myHP_Y + offsetY,
                              (playerHpBarWidth < 0) ? 0 : ((playerHpBarWidth > HP_WIDTH) ? HP_WIDTH : playerHpBarWidth),
                              HP_HEIGHT,
                              playerHpBarColour);
                    draw_rect(EXP_X, EXP_Y + offsetY, expBarWidthFor(playerActive), EXP_HEIGHT, TURQ);
                    draw_string_f(myLVL_X, myLVL_Y + offsetY, myLvlBuf, BLACK, 1);
                    draw_string_f(MYNAME_X, MYNAME_Y + offsetY, (playerActive != NULL && playerActive->id.data != NULL) ? playerActive->id.data->name : "???", BLACK, 1);
                    draw_string_f(TOTAL_HPNUM_X, HPNUM_Y + offsetY, myHpCurBuf, BLACK, 1);
                    draw_string_f(REMAINING_HP_X, HPNUM_Y + offsetY, myHpMaxBuf, BLACK, 1);
                    //draw_sprite_any(poison, POISON_WIDTH, POISON_HEIGHT, MYSTATUS_X, MYSTATUS_Y + offsetY, TRANSPARENT_COLOUR);
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

            // Use per-message display indices for UI values (prevents showing the next Pokemon early).
            if (battleState.enemyParty != NULL && actionTextDisplayEnemyIndex >= 0 && actionTextDisplayEnemyIndex < battleState.enemyParty->count) {
                enemyActive = battleState.enemyParty->slots[actionTextDisplayEnemyIndex];
            }
            if (battleState.playerParty != NULL && actionTextDisplayPlayerIndex >= 0 && actionTextDisplayPlayerIndex < battleState.playerParty->count) {
                playerActive = battleState.playerParty->slots[actionTextDisplayPlayerIndex];
            }

            const int enemyHp = (actionTextShownEnemyHp >= 0) ? actionTextShownEnemyHp : ((enemyActive != NULL) ? enemyActive->scaledStatsWithLevel[0] : 0);
            const int enemyMaxHp = (enemyActive != NULL) ? enemyActive->maxHp : 1;
            const int playerHp = (actionTextShownPlayerHp >= 0) ? actionTextShownPlayerHp : ((playerActive != NULL) ? playerActive->scaledStatsWithLevel[0] : 0);
            const int playerMaxHp = (playerActive != NULL) ? playerActive->maxHp : 1;
            const bool playerFainted = (playerActive != NULL) && (!playerActive->alive || playerHp <= 0);
            const int levelForUi = (actionTextShownLevel > 0) ? actionTextShownLevel : ((playerActive != NULL) ? playerActive->level : 0);
            const int expForUi = (actionTextShownExp >= 0) ? actionTextShownExp : ((playerActive != NULL) ? playerActive->exp : 0);
            const int playerExpBarWidth = expBarWidthForValues(levelForUi, expForUi);

            
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

            draw_rect(OPP_HP_EMPTY_X + 50, OPP_HP_EMPTY_Y + 20, (enemyHpBarWidth < 0) ? 0 : ((enemyHpBarWidth > HP_WIDTH) ? HP_WIDTH : enemyHpBarWidth), HP_HEIGHT, enemyHpBarColour);
            draw_string_f(oppLVL_X, oppLVL_Y, oppLvlBuf, BLACK, 1);
            //draw_sprite_any(burned, BURNED_WIDTH, BURNED_HEIGHT, STATUS_X, STATUS_Y, TRANSPARENT_COLOUR);
            //draw_sprite_any(caught, CAUGHT_WIDTH, CAUGHT_HEIGHT, CAUGHT_X, CAUGHT_Y, TRANSPARENT_COLOUR);
            draw_string_f(OPPNAME_X, OPPNAME_Y, (enemyActive != NULL && enemyActive->id.data != NULL) ? enemyActive->id.data->name : "???", BLACK, 1);

            static const signed char dy_pattern[BOB_SPRITE_FRAME_COUNT] = {
            0, -1, -1, 0, 0, 1, 1, 0,
            0, -1, -1, 0, 0, 1, 1, 0 };
            int offsetY = dy_pattern[bobFrame];
            draw_rect(myHP_X, myHP_Y + offsetY, (playerHpBarWidth < 0) ? 0 : ((playerHpBarWidth > HP_WIDTH) ? HP_WIDTH : playerHpBarWidth), HP_HEIGHT, playerHpBarColour);
            draw_rect(EXP_X, EXP_Y + offsetY, playerExpBarWidth, EXP_HEIGHT, TURQ);
            draw_string_f(myLVL_X, myLVL_Y + offsetY, myLvlBuf, BLACK, 1);
            draw_string_f(MYNAME_X, MYNAME_Y + offsetY, (playerActive != NULL && playerActive->id.data != NULL) ? playerActive->id.data->name : "???", BLACK, 1);
            draw_string_f(TOTAL_HPNUM_X, HPNUM_Y + offsetY, myHpCurBuf, BLACK, 1);
            draw_string_f(REMAINING_HP_X, HPNUM_Y + offsetY, myHpMaxBuf, BLACK, 1);
            //draw_sprite_any(poison, POISON_WIDTH, POISON_HEIGHT, MYSTATUS_X, MYSTATUS_Y + offsetY, TRANSPARENT_COLOUR);

            // Textbox message.
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, msg, BLACK);

            const bool manual = isManualBattleMessage(msg);
            bool shouldAdvance = false;

        
            if (battleState.messageCount > 0) {
                const bool animatingHp = (actionTextShownPlayerHp != actionTextTargetPlayerHp) || (actionTextShownEnemyHp != actionTextTargetEnemyHp);
                const bool animatingExp = actionTextExpAnimating && ((actionTextShownLevel != actionTextTargetLevel) || (actionTextShownExp != actionTextTargetExp));
                const int delayFrames = manual ? 30 : 15;
                actionTextAutoTimer++;
                if (!animatingHp && !animatingExp && !actionTextAwaitSpaceRelease && (spacePressed || actionTextAutoTimer >= delayFrames)) {
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

                    // If a move was learned automatically (empty slot), show that message before returning.
                    pokemonInBattle *learnedP = NULL;
                    const AttackData *learnedM = NULL;
                    const AttackData *forgotM = NULL;
                    if (popNextLearnedMoveMsg(&playerParty, &learnedP, &learnedM, &forgotM)) {
                        learnMovePokemon = learnedP;
                        learnMoveMove = learnedM;
                        learnMoveForgottenMove = forgotM;
                        currentGameState = GAME_STATE_LEARN_MOVE_MESSAGE;
                        previousGameState = GAME_STATE_LEARN_MOVE_MESSAGE;
                        break;
                    }

                    bool startedLearnFlow = false;
                    if (popNextPendingLearnMove(&playerParty, &learnMovePokemon, &learnMoveMove)) {
                        learnMoveForgetIndex = 0;
                        learnMoveReturnUi = actionTextReturnUi;
                        learnMoveReturnCursor = actionTextReturnCursor;
                        startedLearnFlow = true;
                    }

                    if (battleState.result != BATTLE_RESULT_ONGOING) {
                        autoSwapLeadIfFainted(&playerParty, partyBoxSprites);
                        GameState endState = GAME_STATE_MAP;
                        if (battleState.result == BATTLE_RESULT_PLAYER_WIN) {
                            endState = GAME_STATE_BATTLE_WIN;
                            int delta = 0;
                            if (battleState.type == BATTLE_TRAINER) {
                                delta = computeTrainerPayout(&enemyParty);
                                playerMoney += delta;
                                if (currentMapId == WORLD_MAP_ROUTE_B) {
                                    map_set_route_b_trainer_defeated(true);
                                }
                            }
                            if (delta > 0) {
                                snprintf(battleEndMsg, sizeof(battleEndMsg), "WIN! +$%d", delta);
                            } else {
                                snprintf(battleEndMsg, sizeof(battleEndMsg), "WIN");
                            }
                        } else if (battleState.result == BATTLE_RESULT_PLAYER_LOSE) {
                            endState = GAME_STATE_BATTLE_LOSE;
                            const int penalty = computeLossPenalty(playerMoney);
                            playerMoney -= penalty;
                            snprintf(battleEndMsg, sizeof(battleEndMsg), "You blacked out!");
                        } else {
                            // Fled / caught results return to map.
                            endState = GAME_STATE_MAP;
                        }

                        if (startedLearnFlow) {
                            learnMoveReturnState = endState;
                            currentGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                            previousGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                        } else {
                            currentGameState = endState;
                        }
                    } else {
                        if (startedLearnFlow) {
                            learnMoveReturnState = actionTextReturnGameState;
                            currentGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                            previousGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                        } else if (battleState.playerMustSwitch) {
                            forcedSwitchIndex = getFirstAlivePokemon(&playerParty);
                            if (forcedSwitchIndex < 0) forcedSwitchIndex = 0;
                            currentGameState = GAME_STATE_BATTLE_FORCE_SWITCH;
                            previousGameState = GAME_STATE_BATTLE_FORCE_SWITCH;
                        } else {
                            battleUi = actionTextReturnUi;
                            battleCursor = actionTextReturnCursor;
                            currentGameState = actionTextReturnGameState;
                            previousGameState = actionTextReturnGameState;
                        }
                    }
                }
            }

            break;
        }

        case GAME_STATE_LEARN_MOVE_PROMPT: {
            if (learnMoveReturnState == GAME_STATE_EVOLUTION) {
                draw_sprite_any(evolutionBackdropFrames[evolutionFrame],
                                EVOLUTIONBACKDROP_WIDTH, EVOLUTIONBACKDROP_HEIGHT,
                                0, 0,
                                TRANSPARENT_COLOUR);

                const int evolutionSpeedFrames = 10;
                evolutionTimer++;
                if (evolutionTimer >= evolutionSpeedFrames) {
                    evolutionTimer = 0;
                    evolutionFrame = (evolutionFrame + 1) % EVOLUTIONBACKDROP_FRAME_COUNT;
                }

                const int evoAreaH = TEXTBOX_Y;
                const int pokeId = (learnMovePokemon != NULL) ? learnMovePokemon->id.frontFrame_ID : 0;
                StaticSprite front = {0};
                (void)setPokemonFrontBattleSpriteId(&front, pokeId);
                front.x = (front.width > 0) ? (SCREEN_WIDTH - front.width) / 2 : 0;
                front.y = (front.height > 0) ? (evoAreaH - front.height) / 2 : 0;
                if (front.pixels != NULL) {
                    draw_sprite_any(front.pixels, front.width, front.height, front.x, front.y, TRANSPARENT_COLOUR);
                }
            }

            const char *pokeName = (learnMovePokemon != NULL && learnMovePokemon->id.data != NULL && learnMovePokemon->id.data->name != NULL)
                                       ? learnMovePokemon->id.data->name
                                       : "???";
            const char *moveName = (learnMoveMove != NULL && learnMoveMove->name != NULL) ? learnMoveMove->name : "???";
            char buf[192];
            snprintf(buf, sizeof(buf), "%s wants to learn %s!", pokeName, moveName);
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, buf, BLACK);

            if (spacePressed) {
                currentGameState = GAME_STATE_LEARN_MOVE_YESNO;
                previousGameState = GAME_STATE_LEARN_MOVE_YESNO;
            }
            break;
        }

        case GAME_STATE_LEARN_MOVE_YESNO: {
            if (learnMoveReturnState == GAME_STATE_EVOLUTION) {
                draw_sprite_any(evolutionBackdropFrames[evolutionFrame],
                                EVOLUTIONBACKDROP_WIDTH, EVOLUTIONBACKDROP_HEIGHT,
                                0, 0,
                                TRANSPARENT_COLOUR);

                const int evolutionSpeedFrames = 10;
                evolutionTimer++;
                if (evolutionTimer >= evolutionSpeedFrames) {
                    evolutionTimer = 0;
                    evolutionFrame = (evolutionFrame + 1) % EVOLUTIONBACKDROP_FRAME_COUNT;
                }

                const int evoAreaH = TEXTBOX_Y;
                const int pokeId = (learnMovePokemon != NULL) ? learnMovePokemon->id.frontFrame_ID : 0;
                StaticSprite front = {0};
                (void)setPokemonFrontBattleSpriteId(&front, pokeId);
                front.x = (front.width > 0) ? (SCREEN_WIDTH - front.width) / 2 : 0;
                front.y = (front.height > 0) ? (evoAreaH - front.height) / 2 : 0;
                if (front.pixels != NULL) {
                    draw_sprite_any(front.pixels, front.width, front.height, front.x, front.y, TRANSPARENT_COLOUR);
                }
            }

            const char *pokeName = (learnMovePokemon != NULL && learnMovePokemon->id.data != NULL && learnMovePokemon->id.data->name != NULL)
                                       ? learnMovePokemon->id.data->name
                                       : "???";
            const char *moveName = (learnMoveMove != NULL && learnMoveMove->name != NULL) ? learnMoveMove->name : "???";
            char buf[192];
            snprintf(buf, sizeof(buf), "Teach %s %s?\nSPACE=Yes  ESC=No", pokeName, moveName);
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, buf, BLACK);

            if (spacePressed) {
                currentGameState = GAME_STATE_LEARN_MOVE_FORGET;
                previousGameState = GAME_STATE_LEARN_MOVE_FORGET;
            } else if (escPressed) {
                // declined: continue
                learnMovePokemon = NULL;
                learnMoveMove = NULL;
                if (popNextPendingLearnMove(&playerParty, &learnMovePokemon, &learnMoveMove)) {
                    currentGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                    previousGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                } else {
                    battleUi = learnMoveReturnUi;
                    battleCursor = learnMoveReturnCursor;
                    currentGameState = learnMoveReturnState;
                    previousGameState = learnMoveReturnState;
                }
            }
            break;
        }

        case GAME_STATE_LEARN_MOVE_FORGET: {
            if (learnMoveReturnState == GAME_STATE_EVOLUTION) {
                draw_sprite_any(evolutionBackdropFrames[evolutionFrame],
                                EVOLUTIONBACKDROP_WIDTH, EVOLUTIONBACKDROP_HEIGHT,
                                0, 0,
                                TRANSPARENT_COLOUR);

                const int evolutionSpeedFrames = 10;
                evolutionTimer++;
                if (evolutionTimer >= evolutionSpeedFrames) {
                    evolutionTimer = 0;
                    evolutionFrame = (evolutionFrame + 1) % EVOLUTIONBACKDROP_FRAME_COUNT;
                }

                const int evoAreaH = TEXTBOX_Y;
                const int pokeId = (learnMovePokemon != NULL) ? learnMovePokemon->id.frontFrame_ID : 0;
                StaticSprite front = {0};
                (void)setPokemonFrontBattleSpriteId(&front, pokeId);
                front.x = (front.width > 0) ? (SCREEN_WIDTH - front.width) / 2 : 0;
                front.y = (front.height > 0) ? (evoAreaH - front.height) / 2 : 0;
                if (front.pixels != NULL) {
                    draw_sprite_any(front.pixels, front.width, front.height, front.x, front.y, TRANSPARENT_COLOUR);
                }
            }

            // Show current 4 moves (battle attack menu layout) and pick one to replace.
            {
                const int mxLeft = 18;
                const int mxRight = 160 + 18;
                const int myTop = 240 - 89;
                const int myBottom = 240 - 45;
                const int mxs[4] = { mxLeft, mxRight, mxLeft, mxRight };
                const int mys[4] = { myTop,  myTop,   myBottom, myBottom };

                // Text box inside each move sprite
                const int nameBoxW = 97;
                const int nameBoxH = 9;
                const int nameBoxDx = 10;
                const int nameBoxDy = 11;

                const char *newMoveName = (learnMoveMove != NULL && learnMoveMove->name != NULL) ? learnMoveMove->name : "???";
                char titleBuf[96];
                snprintf(titleBuf, sizeof(titleBuf), "Choose a move to forget (learn %s)", newMoveName);
                draw_string_f(8, 8, titleBuf, BLACK, FONT_5X9);

                for (int i = 0; i < 4; i++) {
                    const AttackData *move = (learnMovePokemon != NULL) ? learnMovePokemon->attacks[i] : NULL;
                    const char *moveName = (move != NULL && move->name != NULL) ? move->name : "";

                    AttackTypeSpriteRef moveTypeSprite = (move != NULL) ? attackTypeSpriteFor(move->type)
                                                                       : (AttackTypeSpriteRef){normalTypeSprite, NORMAL_TYPE_WIDTH, NORMAL_TYPE_HEIGHT };

                    if (learnMoveForgetIndex == i) {
                        draw_sprite_any_shade_pulse(moveTypeSprite.pixels, moveTypeSprite.width, moveTypeSprite.height,
                                                    mxs[i], mys[i], TRANSPARENT_COLOUR, shadePulseFrame);
                    } else {
                        draw_sprite_any(moveTypeSprite.pixels, moveTypeSprite.width, moveTypeSprite.height,
                                        mxs[i], mys[i], TRANSPARENT_COLOUR);
                    }

                    drawCenteredStringInBox(mxs[i] + nameBoxDx, mys[i] + nameBoxDy, nameBoxW, nameBoxH,
                                            moveName, BLACK, FONT_5X9);

                    if (learnMovePokemon != NULL && move != NULL) {
                        const int ppX = mxs[i] + CURRENT_PP_X_FROM_ATTACK_SPRITE;
                        const int ppY = mys[i] + CURRENT_PP_Y_FROM_ATTACK_SPRITE;

                        const int ppTotalX = mxs[i] + CURRENT_PP_X_FROM_ATTACK_SPRITE + TOTAL_PP_X_FROM_CURRENT_PP;
                        const int ppTotalY = mys[i] + CURRENT_PP_Y_FROM_ATTACK_SPRITE + TOTAL_PP_Y_FROM_CURRENT_PP;

                        const int ppCur = learnMovePokemon->currentPP[i];
                        const int ppMax = move->maxPP;
                        char ppBufCurrent[16];
                        char ppBufTotal[16];

                        snprintf(ppBufCurrent, sizeof(ppBufCurrent), "%d", ppCur);
                        snprintf(ppBufTotal, sizeof(ppBufTotal), "%d", ppMax);
                        draw_string_f(ppX, ppY, ppBufCurrent, BLACK, FONT_5X9);
                        draw_string_f(ppTotalX, ppTotalY, ppBufTotal, BLACK, FONT_5X9);
                    }
                }
            }

            if (leftPressed && learnMoveForgetIndex > 0) learnMoveForgetIndex--;
            if (rightPressed && learnMoveForgetIndex < 3) learnMoveForgetIndex++;
            if (upPressed && learnMoveForgetIndex >= 2) learnMoveForgetIndex -= 2;
            if (downPressed && learnMoveForgetIndex <= 1) learnMoveForgetIndex += 2;
            if (learnMoveForgetIndex < 0) learnMoveForgetIndex = 0;
            if (learnMoveForgetIndex > 3) learnMoveForgetIndex = 3;

            if (escPressed) {
                currentGameState = GAME_STATE_LEARN_MOVE_YESNO;
                previousGameState = GAME_STATE_LEARN_MOVE_YESNO;
            } else if (spacePressed) {
                if (learnMovePokemon != NULL && learnMoveMove != NULL) {
                    learnMoveForgottenMove = (learnMoveForgetIndex >= 0 && learnMoveForgetIndex < 4) ? learnMovePokemon->attacks[learnMoveForgetIndex] : NULL;
                    (void)learnMove(learnMovePokemon, learnMoveMove, learnMoveForgetIndex);
                    queueLearnedMoveMsg(learnMovePokemon, learnMoveMove, learnMoveForgottenMove);
                }

                currentGameState = GAME_STATE_LEARN_MOVE_MESSAGE;
                previousGameState = GAME_STATE_LEARN_MOVE_MESSAGE;
                break;

            }
            break;
        }

        case GAME_STATE_LEARN_MOVE_MESSAGE: {
            if (learnMoveReturnState == GAME_STATE_EVOLUTION) {
                draw_sprite_any(evolutionBackdropFrames[evolutionFrame],
                                EVOLUTIONBACKDROP_WIDTH, EVOLUTIONBACKDROP_HEIGHT,
                                0, 0,
                                TRANSPARENT_COLOUR);

                const int evolutionSpeedFrames = 10;
                evolutionTimer++;
                if (evolutionTimer >= evolutionSpeedFrames) {
                    evolutionTimer = 0;
                    evolutionFrame = (evolutionFrame + 1) % EVOLUTIONBACKDROP_FRAME_COUNT;
                }

                const int evoAreaH = TEXTBOX_Y;
                const int pokeId = (learnMovePokemon != NULL) ? learnMovePokemon->id.frontFrame_ID : 0;
                StaticSprite front = {0};
                (void)setPokemonFrontBattleSpriteId(&front, pokeId);
                front.x = (front.width > 0) ? (SCREEN_WIDTH - front.width) / 2 : 0;
                front.y = (front.height > 0) ? (evoAreaH - front.height) / 2 : 0;
                if (front.pixels != NULL) {
                    draw_sprite_any(front.pixels, front.width, front.height, front.x, front.y, TRANSPARENT_COLOUR);
                }
            }

            const char *pokeName = (learnMovePokemon != NULL && learnMovePokemon->id.data != NULL && learnMovePokemon->id.data->name != NULL)
                                       ? learnMovePokemon->id.data->name
                                       : "???";
            const char *learnedName = (learnMoveMove != NULL && learnMoveMove->name != NULL) ? learnMoveMove->name : "???";
            const char *forgotName = (learnMoveForgottenMove != NULL && learnMoveForgottenMove->name != NULL) ? learnMoveForgottenMove->name : NULL;

            if (forgotName != NULL) {
                snprintf(learnMoveMsgBuf, sizeof(learnMoveMsgBuf), "%s forgot %s and learned %s!", pokeName, forgotName, learnedName);
            } else {
                snprintf(learnMoveMsgBuf, sizeof(learnMoveMsgBuf), "%s learned %s!", pokeName, learnedName);
            }
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, learnMoveMsgBuf, BLACK);

            if (spacePressed) {
                learnMovePokemon = NULL;
                learnMoveMove = NULL;
                learnMoveForgottenMove = NULL;

                // Show any additional learned messages first.
                pokemonInBattle *p2 = NULL;
                const AttackData *l2 = NULL;
                const AttackData *f2 = NULL;
                if (popNextLearnedMoveMsg(&playerParty, &p2, &l2, &f2)) {
                    learnMovePokemon = p2;
                    learnMoveMove = l2;
                    learnMoveForgottenMove = f2;
                    break;
                }

                if (popNextPendingLearnMove(&playerParty, &learnMovePokemon, &learnMoveMove)) {
                    currentGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                    previousGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                } else {
                    battleUi = learnMoveReturnUi;
                    battleCursor = learnMoveReturnCursor;
                    currentGameState = learnMoveReturnState;
                    previousGameState = learnMoveReturnState;
                }
            }
            break;
        }

        case GAME_STATE_BATTLE_FORCE_SWITCH: {
            // pick a living Pokemon to send out after faint.
            if (!battleState.playerMustSwitch) {
                currentGameState = activeBattleMenuState;
                previousGameState = activeBattleMenuState;
                break;
            }

            Party *p = &playerParty;
            if (p->count <= 0) {
                currentGameState = GAME_STATE_MAP;
                previousGameState = GAME_STATE_MAP;
                break;
            }

            if (forcedSwitchIndex < 0) forcedSwitchIndex = 0;
            if (forcedSwitchIndex > 5) forcedSwitchIndex = 5;

            //  3x2 navigation across slots 0..5 
            const int prev = forcedSwitchIndex;
            int row = forcedSwitchIndex / 3;
            int col = forcedSwitchIndex % 3;
            if (upPressed && row > 0) row--;
            if (downPressed && row < 1) row++;
            if (leftPressed && col > 0) col--;
            if (rightPressed && col < 2) col++;
            int next = row * 3 + col;
            if (next < 0) next = 0;
            if (next > 5) next = 5;
            forcedSwitchIndex = next;
            if (forcedSwitchIndex != prev) play_sfx(plink_audio, plink_audio_len);

            draw_map();
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, "Choose a Pokemon to send out!", BLACK);

            // Party slot background + box sprites (same layout as the battle menu party selector).
            {
                const unsigned short *partyBg = battlePartySlotSprites[forcedSwitchIndex + 1];
                draw_sprite_any(partyBg, BATTLE_PARTY_WIDTH, BATTLE_PARTY_HEIGHT, BATTLE_PARTY_X, BATTLE_PARTY_Y, TRANSPARENT_COLOUR);

                for (int i = 0; i < 6; i++) {
                    const pokemonInBattle *slotPokemon = (battleState.playerParty != NULL && i < battleState.playerParty->count) ? battleState.playerParty->slots[i] : NULL;
                    const int slotHp = (slotPokemon != NULL) ? slotPokemon->scaledStatsWithLevel[0] : 0;
                    const bool slotFainted = (slotPokemon != NULL) && (!slotPokemon->alive || slotHp <= 0);

                    if (i == forcedSwitchIndex) {
                        if (slotFainted || slotPokemon == NULL) {
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
                        if (slotFainted || slotPokemon == NULL) {
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

            if (spacePressed) {
                const bool valid =
                    (battleState.playerParty != NULL) &&
                    (forcedSwitchIndex >= 0) &&
                    (forcedSwitchIndex < battleState.playerParty->count) &&
                    (battleState.playerParty->slots[forcedSwitchIndex] != NULL) &&
                    (battleState.playerParty->slots[forcedSwitchIndex]->alive);

                if (!valid) {
                    battleUiSetSingleMessage(&battleState, "Can't switch to that Pokemon!");
                    actionTextReturnUi = BATTLE_UI_MENU;
                    actionTextReturnCursor = 0;
                    actionTextReturnGameState = GAME_STATE_BATTLE_FORCE_SWITCH;
                    currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    actionTextAwaitSpaceRelease = true;
                    break;
                }

                battleApplyPlayerAction(&battleState, ACTION_SWITCH, forcedSwitchIndex);

                // If it failed, show the message and return here
                if (battleState.playerMustSwitch) {
                    actionTextReturnUi = BATTLE_UI_MENU;
                    actionTextReturnCursor = 0;
                    actionTextReturnGameState = GAME_STATE_BATTLE_FORCE_SWITCH;
                    currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    actionTextAwaitSpaceRelease = true;
                    break;
                }

                battleState.messageCount = 0;
                battleState.messageReadIndex = 0;

                actionTextShownPlayerHp = -1;
                actionTextShownEnemyHp = -1;
                actionTextTargetPlayerHp = -1;
                actionTextTargetEnemyHp = -1;
                actionTextDisplayPlayerIndex = -1;
                actionTextDisplayEnemyIndex = -1;
                actionTextShownLevel = -1;
                actionTextShownExp = -1;
                actionTextTargetLevel = -1;
                actionTextTargetExp = -1;
                actionTextExpAnimating = false;

                pokeballThrowShowText = true;
                pokeballThrowAutoAdvance = true;
                pokeballThrowReturnState = activeBattleMenuState;
                pokeballThrowInit = false;
                battleThrowPokeballTextReady = false;

                battleUi = BATTLE_UI_MENU;
                battleCursor = 0;
                currentGameState = GAME_STATE_POKEBALL_THROW;
                previousGameState = GAME_STATE_POKEBALL_THROW;
            }

            break;
        }

        case GAME_STATE_BATTLE_ITEM_TARGET: {
            if (itemTargetItem == ITEM_NONE || !itemIsRevive(itemTargetItem)) {
                currentGameState = activeBattleMenuState;
                previousGameState = activeBattleMenuState;
                break;
            }

            // Cancel back to where we came from in the bag UI.
            if (escPressed) {
                battleUi = itemTargetReturnUi;
                battleCursor = itemTargetReturnCursor;
                if (itemTargetReturnUi == BATTLE_UI_BAG_HP_LIST) {
                    bagHpPage = itemTargetReturnPage;
                } else if (itemTargetReturnUi == BATTLE_UI_BAG_BALL_LIST) {
                    bagBallPage = itemTargetReturnPage;
                }
                currentGameState = activeBattleMenuState;
                previousGameState = activeBattleMenuState;
                break;
            }

            if (itemTargetIndex < 0) itemTargetIndex = 0;
            if (itemTargetIndex > 5) itemTargetIndex = 5;

            //  3x2 navigation across slots 0..5 
            const int prev = itemTargetIndex;
            int row = itemTargetIndex / 3;
            int col = itemTargetIndex % 3;
            if (upPressed && row > 0) row--;
            if (downPressed && row < 1) row++;
            if (leftPressed && col > 0) col--;
            if (rightPressed && col < 2) col++;
            int next = row * 3 + col;
            if (next < 0) next = 0;
            if (next > 5) next = 5;
            itemTargetIndex = next;
            if (itemTargetIndex != prev) play_sfx(plink_audio, plink_audio_len);

            draw_map();
            draw_sprite_any(battleUIBackgroundSprite, BATTLE_UI_BACKGROUND_WIDTH, BATTLE_UI_BACKGROUND_HEIGHT, 0, battleBackdropY, TRANSPARENT_COLOUR);
            {
                char buf[96];
                snprintf(buf, sizeof(buf), "Use %s on which Pokemon?", itemName(itemTargetItem));
                draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, buf, BLACK);
            }

            // Party slot background + box sprites.
            {
                const unsigned short *partyBg = battlePartySlotSprites[itemTargetIndex + 1];
                draw_sprite_any(partyBg, BATTLE_PARTY_WIDTH, BATTLE_PARTY_HEIGHT, BATTLE_PARTY_X, BATTLE_PARTY_Y, TRANSPARENT_COLOUR);

                for (int i = 0; i < 6; i++) {
                    const pokemonInBattle *slotPokemon = (battleState.playerParty != NULL && i < battleState.playerParty->count) ? battleState.playerParty->slots[i] : NULL;
                    const int slotHp = (slotPokemon != NULL) ? slotPokemon->scaledStatsWithLevel[0] : 0;
                    const bool slotFainted = (slotPokemon != NULL) && (!slotPokemon->alive || slotHp <= 0);
                    const bool slotSelectable = slotFainted;

                    if (i == itemTargetIndex) {
                        if (!slotSelectable || slotPokemon == NULL) {
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
                        if (!slotSelectable || slotPokemon == NULL) {
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

            if (spacePressed) {
                const bool valid =
                    (battleState.playerParty != NULL) &&
                    (itemTargetIndex >= 0) &&
                    (itemTargetIndex < battleState.playerParty->count) &&
                    (battleState.playerParty->slots[itemTargetIndex] != NULL) &&
                    (!battleState.playerParty->slots[itemTargetIndex]->alive);

                if (!valid) {
                    battleUiSetSingleMessage(&battleState, "Choose a fainted Pokemon!");
                    actionTextReturnUi = BATTLE_UI_MENU;
                    actionTextReturnCursor = 0;
                    actionTextReturnGameState = GAME_STATE_BATTLE_ITEM_TARGET;
                    currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                    actionTextAwaitSpaceRelease = true;
                    break;
                }

                pokemonInBattle *p = getActivePokemon(&playerParty);
                pokemonInBattle *e = getActivePokemon(&enemyParty);
                actionTextShownPlayerHp = (p != NULL) ? p->scaledStatsWithLevel[0] : -1;
                actionTextShownEnemyHp = (e != NULL) ? e->scaledStatsWithLevel[0] : -1;
                actionTextTargetPlayerHp = actionTextShownPlayerHp;
                actionTextTargetEnemyHp = actionTextShownEnemyHp;
                actionTextExpStartLevel = (p != NULL) ? p->level : -1;
                actionTextExpStartExp = (p != NULL) ? p->exp : -1;
                actionTextShownLevel = actionTextExpStartLevel;
                actionTextShownExp = actionTextExpStartExp;
                actionTextTargetLevel = actionTextShownLevel;
                actionTextTargetExp = actionTextShownExp;
                actionTextExpAnimating = false;

                battleActionPending = true;
                pendingBattleAction = ACTION_ITEM;
                pendingBattleParam = ((int)itemTargetItem & 0xFF) | ((itemTargetIndex + 1) << 8);
                battleUi = BATTLE_UI_MENU;
                battleCursor = 0;

                actionTextReturnUi = BATTLE_UI_MENU;
                actionTextReturnCursor = 0;
                actionTextReturnGameState = activeBattleMenuState;
                currentGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                previousGameState = GAME_STATE_BATTLE_ACTION_TEXT;
                actionTextAwaitSpaceRelease = true;
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

            if (currentMapId != WORLD_MAP_ROUTE_B) {
                draw_sprite_any(vsCynthiaSprite,
                                VS_CYNTHIA_WIDTH,
                                VS_CYNTHIA_HEIGHT,
                                (SCREEN_WIDTH - VS_CYNTHIA_WIDTH) / 2,
                                0,
                                TRANSPARENT_COLOUR);
            }
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y,
                                      currentMapId == WORLD_MAP_ROUTE_B ? TRAINER_A_BATTLE_TEXT : "Cynthia wants to battle!",
                                      BLACK);

            if (spacePressed) {
                play_sfx(plink_audio, plink_audio_len);
                play_bgm(battle_audio, battle_audio_len);
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
            if (currentMapId != WORLD_MAP_ROUTE_B) {
                draw_sprite_any(vsCynthiaSprite,
                                VS_CYNTHIA_WIDTH,
                                VS_CYNTHIA_HEIGHT,
                                (SCREEN_WIDTH - VS_CYNTHIA_WIDTH) / 2,
                                0,
                                TRANSPARENT_COLOUR);
            }
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y,
                                      currentMapId == WORLD_MAP_ROUTE_B ? TRAINER_A_BATTLE_TEXT : "Cynthia wants to battle!",
                                      BLACK);

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

                // Start off screen (left + below). only draw it at/after the peak.
                pokeballX0 = -(float)POKEBALLTHROW_WIDTH;

                const float landX = (float)(playerBackSprite.x + playerBackSprite.width / 2 - POKEBALLTHROW_WIDTH / 2);
                const float landY = (float)(playerBackSprite.y + playerBackSprite.height - (POKEBALLTHROW_HEIGHT / 2)) - 40;
                pokeballLandX = landX;
                pokeballLandY = landY;

                // Peak above the player's side.
                float yPeak = (float)(playerBackSprite.y - 30);
                if (yPeak < 5.0f) yPeak = 5.0f;

                // Ensure the ball is visible on screen at the peak (only draw it at/after the peak).
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

                                // Add the caught Pokemon to the player's PC (dupes allowed).
                                {
                                    const pokemonInBattle *caughtPokemon = (battleState.enemyParty != NULL) ? getActivePokemon(battleState.enemyParty) : NULL;
                                    if (caughtPokemon != NULL && caughtPokemon->id.data != NULL) {
                                        int pcIndex = -1;
                                        (void)pcAdd(&playerPc, caughtPokemon->id.data, caughtPokemon->level, &pcIndex);
                                        pokedex_mark_caught(caughtPokemon->id.frontFrame_ID);
                                    }
                                }
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

        case GAME_STATE_EVOLUTION: {
            // Evolution screen: backdrop + front sprite + mandatory animation (can't skip).
            draw_sprite_any(evolutionBackdropFrames[evolutionFrame],
                            EVOLUTIONBACKDROP_WIDTH, EVOLUTIONBACKDROP_HEIGHT,
                            0, 0,
                            TRANSPARENT_COLOUR);

            const int evolutionSpeedFrames = 10;
            evolutionTimer++;
            if (evolutionTimer >= evolutionSpeedFrames) {
                evolutionTimer = 0;
                evolutionFrame = (evolutionFrame + 1) % EVOLUTIONBACKDROP_FRAME_COUNT;
            }

            // Find pending evolution target.
            if (evolutionPokemonIndex < 0 || evolutionPokemonIndex >= playerParty.count) {
                evolutionPokemonIndex = findNextPendingEvolutionIndex(&playerParty, -1);
            }
            if (evolutionPokemonIndex < 0) {
                currentGameState = evolutionReturnState;
                break;
            }

            pokemonInBattle *mon = playerParty.slots[evolutionPokemonIndex];
            if (mon == NULL || !mon->alive) {
                const int next = findNextPendingEvolutionIndex(&playerParty, evolutionPokemonIndex);
                if (next >= 0) {
                    evolutionPokemonIndex = next;
                    evolutionPhase = 0;
                    evolutionPhaseTimer = 0;
                    evolutionFromName[0] = '\0';
                    evolutionIntoName[0] = '\0';
                } else {
                    currentGameState = evolutionReturnState;
                }
                break;
            }

            // During phase 2 we intentionally allow pendingEvolutionInto to be NULL (it typically gets cleared by applyPendingEvolution()).
            if (evolutionPhase != 2 && mon->pendingEvolutionInto == NULL) {
                mon->pendingEvolutionInto = (mon->id.data != NULL) ? checkEvolution(mon->id.data, mon->level) : NULL;
            }
            if (evolutionPhase != 2 && mon->pendingEvolutionInto == NULL) {
                const int next = findNextPendingEvolutionIndex(&playerParty, evolutionPokemonIndex);
                if (next >= 0) {
                    evolutionPokemonIndex = next;
                    evolutionPhase = 0;
                    evolutionPhaseTimer = 0;
                    evolutionFromName[0] = '\0';
                    evolutionIntoName[0] = '\0';
                } else {
                    currentGameState = evolutionReturnState;
                }
                break;
            }

            const PokemonData *into = mon->pendingEvolutionInto;

            // Cache names at the start of each evolution so messages stay correct after applyPendingEvolution().
            if (evolutionPhase == 0 && evolutionPhaseTimer == 0) {
                const char *from = (mon->id.data != NULL && mon->id.data->name != NULL) ? mon->id.data->name : "???";
                const char *to = (into != NULL && into->name != NULL) ? into->name : "???";
                snprintf(evolutionFromName, sizeof(evolutionFromName), "%s", from);
                snprintf(evolutionIntoName, sizeof(evolutionIntoName), "%s", to);
            }

            // Build front sprites 
            StaticSprite baseFront = {0};
            StaticSprite evoFront = {0};
            (void)setPokemonFrontBattleSpriteId(&baseFront, mon->id.frontFrame_ID);
            if (into != NULL) (void)setPokemonFrontBattleSpriteId(&evoFront, into->id);

            const int evoAreaH = TEXTBOX_Y; // space above textbox
            baseFront.x = (baseFront.width > 0) ? (SCREEN_WIDTH - baseFront.width) / 2 : 0;
            baseFront.y = (baseFront.height > 0) ? (evoAreaH - baseFront.height) / 2 : 0;
            evoFront.x = (evoFront.width > 0) ? (SCREEN_WIDTH - evoFront.width) / 2 : baseFront.x;
            evoFront.y = (evoFront.height > 0) ? (evoAreaH - evoFront.height) / 2 : baseFront.y;

            char evoMsg[192];
            evoMsg[0] = '\0';

            const int phase0Ticks = 45;
            const int phase1Ticks = 60;

            if (evolutionPhase == 0) {
                if (baseFront.pixels != NULL) {
                    draw_sprite_any(baseFront.pixels, baseFront.width, baseFront.height, baseFront.x, baseFront.y, TRANSPARENT_COLOUR);
                }
                snprintf(evoMsg, sizeof(evoMsg), "What? %s is trying to evolve!", evolutionFromName);

                evolutionPhaseTimer++;
                if (evolutionPhaseTimer >= phase0Ticks) {
                    evolutionPhase = 1;
                    evolutionPhaseTimer = 0;
                }
            } else if (evolutionPhase == 1) {
                const bool showEvolved = ((evolutionPhaseTimer / 6) % 2) == 1;
                const StaticSprite *s = (showEvolved && evoFront.pixels != NULL) ? &evoFront : &baseFront;
                if (s != NULL && s->pixels != NULL) {
                    draw_sprite_any(s->pixels, s->width, s->height, s->x, s->y, TRANSPARENT_COLOUR);
                }
                snprintf(evoMsg, sizeof(evoMsg), "What? %s is trying to evolve!", evolutionFromName);

                evolutionPhaseTimer++;
                if (evolutionPhaseTimer >= phase1Ticks) {
                    (void)applyPendingEvolution(mon);
                    syncPartyBoxSpritesToParty(partyBoxSprites, &playerParty);
                    // register in pokedex
                    if (mon != NULL) pokedex_mark_caught(mon->id.frontFrame_ID);
                    evolutionPhase = 2;
                    evolutionPhaseTimer = 0;
                }
            } else {

                StaticSprite newFront = {0};
                (void)setPokemonFrontBattleSpriteId(&newFront, mon->id.frontFrame_ID);
                newFront.x = (newFront.width > 0) ? (SCREEN_WIDTH - newFront.width) / 2 : 0;
                newFront.y = (newFront.height > 0) ? (evoAreaH - newFront.height) / 2 : 0;
                if (newFront.pixels != NULL) {
                    draw_sprite_any(newFront.pixels, newFront.width, newFront.height, newFront.x, newFront.y, TRANSPARENT_COLOUR);
                }

                snprintf(evoMsg, sizeof(evoMsg), "%s evolved into %s!", evolutionFromName, evolutionIntoName);

                if (spacePressed) {
                    // If a move was learned automatically show that message.
                    pokemonInBattle *learnedP = NULL;
                    const AttackData *learnedM = NULL;
                    const AttackData *forgotM = NULL;
                    if (popNextLearnedMoveMsg(&playerParty, &learnedP, &learnedM, &forgotM)) {
                        learnMovePokemon = learnedP;
                        learnMoveMove = learnedM;
                        learnMoveForgottenMove = forgotM;
                        learnMoveReturnState = GAME_STATE_EVOLUTION;
                        currentGameState = GAME_STATE_LEARN_MOVE_MESSAGE;
                        previousGameState = GAME_STATE_LEARN_MOVE_MESSAGE;
                        break;
                    }

                    if (popNextPendingLearnMove(&playerParty, &learnMovePokemon, &learnMoveMove)) {
                        learnMoveForgetIndex = 0;
                        learnMoveReturnState = GAME_STATE_EVOLUTION;
                        currentGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                        previousGameState = GAME_STATE_LEARN_MOVE_PROMPT;
                        break;
                    }

                    if (mon->pendingEvolutionInto == NULL) {
                        mon->pendingEvolutionInto = (mon->id.data != NULL) ? checkEvolution(mon->id.data, mon->level) : NULL;
                    }
                    if (mon->pendingEvolutionInto != NULL) {
                        // Same Pokemon can evolve again.
                        evolutionPhase = 0;
                        evolutionPhaseTimer = 0;
                        evolutionFromName[0] = '\0';
                        evolutionIntoName[0] = '\0';
                    } else {
                        const int next = findNextPendingEvolutionIndex(&playerParty, evolutionPokemonIndex);
                        if (next >= 0) {
                            evolutionPokemonIndex = next;
                            evolutionPhase = 0;
                            evolutionPhaseTimer = 0;
                            evolutionFromName[0] = '\0';
                            evolutionIntoName[0] = '\0';
                        } else {
                            currentGameState = evolutionReturnState;
                        }
                    }
                }
            }

            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, evoMsg, BLACK);
            break;
        }

        case GAME_STATE_BAG_MENU_ITEMS:
            draw_sprite_any(bagMenuBagMenuSprite,BAG_MENU_BAG_MENU_WIDTH,  BAG_MENU_BAG_MENU_HEIGHT, 0,0,TRANSPARENT_COLOUR);
            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                draw_sprite_any_shade_pulse(bagMenuBackButtonSprite, BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuBackButtonSprite,BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR );
            }
            draw_sprite_any(bagMenuItemsSprite, BAG_MENU_ITEMS_WIDTH,BAG_MENU_ITEMS_HEIGHT, 64, 48, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuBag1Sprite, BAG_MENU_BAG1_WIDTH,BAG_MENU_BAG1_HEIGHT, 83, 67, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuSpin1Sprite,BAG_MENU_SPIN1_WIDTH, BAG_MENU_SPIN1_HEIGHT, 48, 48, TRANSPARENT_COLOUR );
            if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR);
            }
            if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR);
            }
            draw_sprite_any(bagMenuItemsBagSprite,BAG_MENU_ITEMS_BAG_WIDTH,BAG_MENU_ITEMS_BAG_HEIGHT, 85, 91, TRANSPARENT_COLOUR);

            {
                // coords for cursor in bag
                const int cursorX0 = 154;
                const int cursorY0 = 58;
                const int cursorDy = 12;
                const int cursorMaxY = 166;
                const int itemNameX = 145;
                const int itemNameY = 152;
                const int descX = 47;
                const int descY = 169;

                const int listTextX = cursorX0 + 10;
                const int listTextY0 = cursorY0 - 1;

                const int visibleCount = bagMenuVisibleCountForState(currentGameState, &playerBag);

                if (bagMenuFocus == BAG_FOCUS_LIST) {
                    if (visibleCount > 0) {
                        const int oldCursor = bagMenuCursor;
                        if (upPressed) bagMenuCursor--;
                        if (downPressed) bagMenuCursor++;
                        bagMenuCursor = clamp_int(bagMenuCursor, 0, visibleCount - 1);
                        if (bagMenuCursor != oldCursor) play_sfx(plink_audio, plink_audio_len);
                    } else {
                        bagMenuCursor = 0;
                    }
                    if (leftPressed) {
                        bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                } else if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                    if (leftPressed) {
                        bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (rightPressed) {
                        bagMenuFocus = BAG_FOCUS_LIST;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                    if (downPressed) {
                        bagMenuFocus = BAG_FOCUS_CANCEL;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (rightPressed) {
                        bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                } else { // BAG_FOCUS_CANCEL
                    if (rightPressed) {
                        bagMenuFocus = BAG_FOCUS_LIST;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (upPressed) {
                        bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                }

                if (spacePressed) {
                    if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                        currentGameState = bagMenuNextPageState(currentGameState);
                        play_sfx(plink_audio, plink_audio_len);
                        break;
                    } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                        currentGameState = bagMenuPrevPageState(currentGameState);
                        play_sfx(plink_audio, plink_audio_len);
                        break;
                    } else if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                        currentGameState = bagMenuReturnState;
                        play_sfx(plink_audio, plink_audio_len);
                        break;
                    }
                }

                if (visibleCount > 0) bagMenuCursor = clamp_int(bagMenuCursor, 0, visibleCount - 1);
                else bagMenuCursor = 0;

                if (bagMenuFocus == BAG_FOCUS_LIST && visibleCount > 0) {
                    const int cy = cursorY0 + bagMenuCursor * cursorDy;
                    if (cy <= cursorMaxY) {
                        draw_sprite_any(bagMenuCursorIndicatorSprite,
                                        BAG_MENU_CURSOR_INDICATOR_WIDTH, BAG_MENU_CURSOR_INDICATOR_HEIGHT,
                                        cursorX0, cy,
                                        TRANSPARENT_COLOUR);
                    }
                }

                    const int maxRows = ((cursorMaxY - cursorY0) / cursorDy) + 1;
                    const int rows = (visibleCount < maxRows) ? visibleCount : maxRows;
                    for (int i = 0; i < rows; i++) {
                        const ItemId it = bagMenuVisibleAtForState(currentGameState, &playerBag, i);
                        if (it == ITEM_NONE) continue;
                        draw_string_f(listTextX, listTextY0 + i * cursorDy, itemName(it), BLACK, FONT_5X9);
                    }

                    if (visibleCount > 0) {
                        const ItemId selected = bagMenuVisibleAtForState(currentGameState, &playerBag, bagMenuCursor);
                        if (selected != ITEM_NONE) {
                            draw_string_f(itemNameX, itemNameY, itemName(selected), BLACK, FONT_5X9);
                            draw_wrapped_string_fixed_width_f(descX, descY, getItemDescription(selected), BLACK, FONT_5X9, 42, 2);
                        }
                    }
                }
            }

            if (escPressed) { currentGameState = bagMenuReturnState; break; }
        break;

        case GAME_STATE_BAG_MENU_POKEBALLS:
            draw_sprite_any(bagMenuBagMenuSprite,BAG_MENU_BAG_MENU_WIDTH,  BAG_MENU_BAG_MENU_HEIGHT, 0,0,TRANSPARENT_COLOUR);
            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                draw_sprite_any_shade_pulse(bagMenuBackButtonSprite, BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuBackButtonSprite,BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR );
            }
            draw_sprite_any(bagMenuPokeballsSprite, BAG_MENU_POKEBALLS_WIDTH,BAG_MENU_POKEBALLS_HEIGHT, 64, 48, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuBag2Sprite, BAG_MENU_BAG2_WIDTH,BAG_MENU_BAG2_HEIGHT, 83, 67, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuSpin1Sprite,BAG_MENU_SPIN1_WIDTH, BAG_MENU_SPIN1_HEIGHT, 48, 48, TRANSPARENT_COLOUR );
            if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR);
            }
            if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR);
            }
            draw_sprite_any(bagMenuPokeballBagSprite,BAG_MENU_POKEBALL_BAG_WIDTH,BAG_MENU_POKEBALL_BAG_HEIGHT, 85, 91, TRANSPARENT_COLOUR);

            {
                // coords for cursor
                const int cursorX0 = 154;
                const int cursorY0 = 58;
                const int cursorDy = 12;
                const int cursorMaxY = 166;
                const int itemNameX = 145;
                const int itemNameY = 152;
                const int descX = 47;
                const int descY = 169;

                const int listTextX = cursorX0 + 10;
                const int listTextY0 = cursorY0 - 1;

                const int visibleCount = bagMenuVisibleCountForState(currentGameState, &playerBag);

                if (bagMenuFocus == BAG_FOCUS_LIST) {
                    if (visibleCount > 0) {
                        const int oldCursor = bagMenuCursor;
                        if (upPressed) bagMenuCursor--;
                        if (downPressed) bagMenuCursor++;
                        bagMenuCursor = clamp_int(bagMenuCursor, 0, visibleCount - 1);
                        if (bagMenuCursor != oldCursor) play_sfx(plink_audio, plink_audio_len);
                    } else {
                        bagMenuCursor = 0;
                    }
                    if (leftPressed) {
                        bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                } else if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                    if (leftPressed) {
                        bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (rightPressed) {
                        bagMenuFocus = BAG_FOCUS_LIST;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                    if (downPressed) {
                        bagMenuFocus = BAG_FOCUS_CANCEL;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (rightPressed) {
                        bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                } else { // BAG_FOCUS_CANCEL
                    if (rightPressed) {
                        bagMenuFocus = BAG_FOCUS_LIST;
                        play_sfx(plink_audio, plink_audio_len);
                    } else if (upPressed) {
                        bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                }

                if (spacePressed) {
                    if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                        currentGameState = bagMenuNextPageState(currentGameState);
                        play_sfx(plink_audio, plink_audio_len);
                        break;
                    } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                        currentGameState = bagMenuPrevPageState(currentGameState);
                        play_sfx(plink_audio, plink_audio_len);
                        break;
                    } else if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                        currentGameState = bagMenuReturnState;
                        play_sfx(plink_audio, plink_audio_len);
                        break;
                    }
                }

                if (visibleCount > 0) bagMenuCursor = clamp_int(bagMenuCursor, 0, visibleCount - 1);
                else bagMenuCursor = 0;

                if (bagMenuFocus == BAG_FOCUS_LIST && visibleCount > 0) {
                    const int cy = cursorY0 + bagMenuCursor * cursorDy;
                    if (cy <= cursorMaxY) {
                        draw_sprite_any(bagMenuCursorIndicatorSprite,
                                        BAG_MENU_CURSOR_INDICATOR_WIDTH, BAG_MENU_CURSOR_INDICATOR_HEIGHT,
                                        cursorX0, cy,
                                        TRANSPARENT_COLOUR);
                    }
                }

                    const int maxRows = ((cursorMaxY - cursorY0) / cursorDy) + 1;
                    const int rows = (visibleCount < maxRows) ? visibleCount : maxRows;
                    for (int i = 0; i < rows; i++) {
                        const ItemId it = bagMenuVisibleAtForState(currentGameState, &playerBag, i);
                        if (it == ITEM_NONE) continue;
                        draw_string_f(listTextX, listTextY0 + i * cursorDy, itemName(it), BLACK, FONT_5X9);
                    }

                    if (visibleCount > 0) {
                        const ItemId selected = bagMenuVisibleAtForState(currentGameState, &playerBag, bagMenuCursor);
                        if (selected != ITEM_NONE) {
                            draw_string_f(itemNameX, itemNameY, itemName(selected), BLACK, FONT_5X9);
                            draw_wrapped_string_fixed_width_f(descX, descY, getItemDescription(selected), BLACK, FONT_5X9, 42, 2);
                        }
                    }
                }
            }

            if (escPressed) { currentGameState = bagMenuReturnState; break; }
        break;

        case GAME_STATE_BAG_MENU_TMS:
            draw_sprite_any(bagMenuBagMenuSprite,BAG_MENU_BAG_MENU_WIDTH,  BAG_MENU_BAG_MENU_HEIGHT, 0,0,TRANSPARENT_COLOUR);
            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                draw_sprite_any_shade_pulse(bagMenuBackButtonSprite, BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuBackButtonSprite,BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR );
            }
            draw_sprite_any(bagMenuTmsSprite, BAG_MENU_TMS_WIDTH,BAG_MENU_TMS_HEIGHT, 64, 48, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuBag3Sprite, BAG_MENU_BAG3_WIDTH,BAG_MENU_BAG3_HEIGHT, 83, 67, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuSpin1Sprite,BAG_MENU_SPIN1_WIDTH, BAG_MENU_SPIN1_HEIGHT, 48, 48, TRANSPARENT_COLOUR );
            if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR);
            }
            if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR);
            }
            draw_sprite_any(bagMenuTmBagSprite,BAG_MENU_TM_BAG_WIDTH,BAG_MENU_TM_BAG_HEIGHT, 85, 91, TRANSPARENT_COLOUR);

            if (bagMenuFocus == BAG_FOCUS_LIST) {
                if (leftPressed) {
                    bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                if (leftPressed) {
                    bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_LIST;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                if (downPressed) {
                    bagMenuFocus = BAG_FOCUS_CANCEL;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else { // BAG_FOCUS_CANCEL
                if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_LIST;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (upPressed) {
                    bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            }

            if (spacePressed) {
                if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                    currentGameState = bagMenuNextPageState(currentGameState);
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                    currentGameState = bagMenuPrevPageState(currentGameState);
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                } else if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                    currentGameState = bagMenuReturnState;
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                }
            }

            if (escPressed) { currentGameState = bagMenuReturnState; break; }
        break;

        case GAME_STATE_BAG_MENU_BERRIES:
            draw_sprite_any(bagMenuBagMenuSprite,BAG_MENU_BAG_MENU_WIDTH,  BAG_MENU_BAG_MENU_HEIGHT, 0,0,TRANSPARENT_COLOUR);
            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                draw_sprite_any_shade_pulse(bagMenuBackButtonSprite, BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuBackButtonSprite,BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR );
            }
            draw_sprite_any(bagMenuBerriesSprite, BAG_MENU_BERRIES_WIDTH,BAG_MENU_BERRIES_HEIGHT, 64, 48, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuBag4Sprite, BAG_MENU_BAG4_WIDTH,BAG_MENU_BAG4_HEIGHT, 83, 67, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuSpin1Sprite,BAG_MENU_SPIN1_WIDTH, BAG_MENU_SPIN1_HEIGHT, 48, 48, TRANSPARENT_COLOUR );
            if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR);
            }
            if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR);
            }
            draw_sprite_any(bagMenuBerryBagSprite,BAG_MENU_BERRY_BAG_WIDTH,BAG_MENU_BERRY_BAG_HEIGHT, 85, 91, TRANSPARENT_COLOUR);

            if (bagMenuFocus == BAG_FOCUS_LIST) {
                if (leftPressed) {
                    bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                if (leftPressed) {
                    bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_LIST;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                if (downPressed) {
                    bagMenuFocus = BAG_FOCUS_CANCEL;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else { // BAG_FOCUS_CANCEL
                if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_LIST;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (upPressed) {
                    bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            }

            if (spacePressed) {
                if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                    currentGameState = bagMenuNextPageState(currentGameState);
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                    currentGameState = bagMenuPrevPageState(currentGameState);
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                } else if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                    currentGameState = bagMenuReturnState;
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                }
            }

            if (escPressed) { currentGameState = bagMenuReturnState; break; }
        break;

        case GAME_STATE_BAG_MENU_KEY_ITEMS:
            draw_sprite_any(bagMenuBagMenuSprite,BAG_MENU_BAG_MENU_WIDTH,  BAG_MENU_BAG_MENU_HEIGHT, 0,0,TRANSPARENT_COLOUR);
            shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
            if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                draw_sprite_any_shade_pulse(bagMenuBackButtonSprite, BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuBackButtonSprite,BAG_MENU_BACK_BUTTON_WIDTH, BAG_MENU_BACK_BUTTON_HEIGHT, 44, 110, TRANSPARENT_COLOUR );
            }
            draw_sprite_any(bagMenuKeyItemsSprite, BAG_MENU_KEY_ITEMS_WIDTH,BAG_MENU_KEY_ITEMS_HEIGHT, 64, 48, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuBag5Sprite, BAG_MENU_BAG5_WIDTH,BAG_MENU_BAG5_HEIGHT, 83, 67, TRANSPARENT_COLOUR );
            draw_sprite_any(bagMenuSpin1Sprite,BAG_MENU_SPIN1_WIDTH, BAG_MENU_SPIN1_HEIGHT, 48, 48, TRANSPARENT_COLOUR );
            if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuLeftArrowSprite,BAG_MENU_LEFT_ARROW_WIDTH,BAG_MENU_LEFT_ARROW_HEIGHT, 64, 49, TRANSPARENT_COLOUR);
            }
            if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                draw_sprite_any_shade_pulse(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR, shadePulseFrame);
            } else {
                draw_sprite_any(bagMenuRightArrowSprite, BAG_MENU_RIGHT_ARROW_WIDTH,BAG_MENU_RIGHT_ARROW_HEIGHT, 137, 49, TRANSPARENT_COLOUR);
            }
            draw_sprite_any(bagMenuKeyItemBagSprite,BAG_MENU_KEY_ITEM_BAG_WIDTH,BAG_MENU_KEY_ITEM_BAG_HEIGHT, 85, 91, TRANSPARENT_COLOUR);

            if (bagMenuFocus == BAG_FOCUS_LIST) {
                if (leftPressed) {
                    bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                if (leftPressed) {
                    bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_LIST;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                if (downPressed) {
                    bagMenuFocus = BAG_FOCUS_CANCEL;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_RIGHT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            } else { // BAG_FOCUS_CANCEL
                if (rightPressed) {
                    bagMenuFocus = BAG_FOCUS_LIST;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (upPressed) {
                    bagMenuFocus = BAG_FOCUS_LEFT_ARROW;
                    play_sfx(plink_audio, plink_audio_len);
                }
            }

            if (spacePressed) {
                if (bagMenuFocus == BAG_FOCUS_RIGHT_ARROW) {
                    currentGameState = bagMenuNextPageState(currentGameState);
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                } else if (bagMenuFocus == BAG_FOCUS_LEFT_ARROW) {
                    currentGameState = bagMenuPrevPageState(currentGameState);
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                } else if (bagMenuFocus == BAG_FOCUS_CANCEL) {
                    currentGameState = bagMenuReturnState;
                    play_sfx(plink_audio, plink_audio_len);
                    break;
                }
            }

            if (escPressed) { currentGameState = bagMenuReturnState; break; }
        break;

        case GAME_STATE_PC_MENU: {

            const int oldPcIndex = pcCursor;
            if (upPressed) pcCursor = navPCMenu33(pcCursor, DIR_UP);
            if (leftPressed) pcCursor = navPCMenu33(pcCursor, DIR_LEFT);
            if (downPressed) pcCursor = navPCMenu33(pcCursor, DIR_DOWN);
            if (rightPressed) pcCursor = navPCMenu33(pcCursor, DIR_RIGHT);
            const bool didMovePcCursor = (pcCursor != oldPcIndex);
            (void)didMovePcCursor;

            if (escPressed) {
                pcSwapIndex = -1;
                pcHeldMon = NULL;
                currentGameState = pcMenuReturnState;
                break;
            }

            // Base UI
            draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
                  draw_sprite_any_rot90_cw(pcBoxBlueSprite,
                                     PC_MENU_PC_BOX_BLUE_WIDTH, PC_MENU_PC_BOX_BLUE_HEIGHT,
                                     (340 - PC_MENU_PC_BOX_BLUE_WIDTH) + 40, (120 - (PC_MENU_PC_BOX_BLUE_HEIGHT / 2)) - 20 + 3,
                                     TRANSPARENT_COLOUR);
            draw_sprite_any(pcBoxBackgroundSprite,
                            PC_MENU_PC_BOX_BACKGROUND_WIDTH, PC_MENU_PC_BOX_BACKGROUND_HEIGHT,
                            0, (120 - (PC_MENU_PARTY_BOX_HEIGHT / 2)) - 35 + 10 - 5 + 5,
                            TRANSPARENT_COLOUR);



            draw_sprite_any(leftArrowSprite,
                            PC_MENU_LEFT_ARROW_WIDTH, PC_MENU_LEFT_ARROW_HEIGHT,
                            PC_MENU_LEFT_ARROW_X, PC_MENU_ARROW_Y + 7,
                            TRANSPARENT_COLOUR);

            draw_sprite_any(rightArrowSprite,
                            PC_MENU_RIGHT_ARROW_WIDTH, PC_MENU_RIGHT_ARROW_HEIGHT,
                            PC_MENU_RIGHT_ARROW_X - 4, PC_MENU_ARROW_Y + 7,
                            TRANSPARENT_COLOUR);

            draw_sprite_any(pcLabelSprite,
                            PC_MENU_PC_LABEL_WIDTH, PC_MENU_PC_LABEL_HEIGHT,
                            20 + 3, 20 + 3,
                            TRANSPARENT_COLOUR);

            draw_sprite_any(partyBoxSprite,
                            PC_MENU_PARTY_BOX_WIDTH, PC_MENU_PARTY_BOX_HEIGHT,
                            PC_MENU_PARTY_BOX_X + 10, PC_MENU_PARTY_BOX_Y,
                            TRANSPARENT_COLOUR);

            //show owned Pokemon that arent currently in the party.
            pokemonInBattle *pcDisplay[PC_MAX];
            int pcDisplayCount = 0;
            for (int i = 0; i < playerPc.count && pcDisplayCount < PC_MAX; i++) {
                pokemonInBattle *mon = &playerPc.mons[i];
                bool inParty = false;
                for (int j = 0; j < playerParty.count; j++) {
                    if (playerParty.slots[j] == mon) {
                        inParty = true;
                        break;
                    }
                }
                if (!inParty) {
                    pcDisplay[pcDisplayCount++] = mon;
                }
            }

            // Determine what's under the cursor.
            pokemonInBattle *cursorMon = NULL;
            if (pcCursor >= 2 && pcCursor <= 26) {
                const int idx = pcCursor - 2;
                if (idx >= 0 && idx < pcDisplayCount) cursorMon = pcDisplay[idx];
            } else if (pcCursor >= 27 && pcCursor <= 32) {
                const int idx = pcCursor - 27;
                if (idx >= 0 && idx < playerParty.count) cursorMon = playerParty.slots[idx];
            }

            // Space-> pick up / drop / swa pokemon
            if (spacePressed) {
                if (pcSwapIndex < 0) {
                    if (pcCursor >= 2 && pcCursor <= 32 && cursorMon != NULL) {
                        pcSwapIndex = pcCursor;
                        pcHeldMon = cursorMon;
                    }
                } else {
                    const bool targetIsArrow = (pcCursor == 0 || pcCursor == 1);
                    if (!targetIsArrow) {
                        if (pcCursor == pcSwapIndex) {
                            pcSwapIndex = -1;
                            pcHeldMon = NULL;
                        } else if (pcHeldMon != NULL) {
                            bool didSwap = false;

                            // Party <-> Party swap uses pointer swap
                            if (pcSwapIndex >= 27 && pcSwapIndex <= 32 && pcCursor >= 27 && pcCursor <= 32) {
                                const int a = pcSwapIndex - 27;
                                const int b = pcCursor - 27;
                                if (a >= 0 && a < playerParty.count && b >= 0 && b < playerParty.count) {
                                    pokemonInBattle *tmp = playerParty.slots[a];
                                    playerParty.slots[a] = playerParty.slots[b];
                                    playerParty.slots[b] = tmp;

                                    if (playerParty.activeIndex == a) playerParty.activeIndex = b;
                                    else if (playerParty.activeIndex == b) playerParty.activeIndex = a;

                                    didSwap = true;
                                }
                            } else if (cursorMon != NULL) {
                                // PC grid swaps (and party<->PC swaps) are done by swapping the Pokemon data.
                                pokemonInBattle tmp = *pcHeldMon;
                                *pcHeldMon = *cursorMon;
                                *cursorMon = tmp;
                                didSwap = true;
                            } else if (pcCursor >= 27 && pcCursor <= 32) {
                                // Allow placing a PC Pokemon into the next empty party slot (at the end).
                                const int targetSlot = pcCursor - 27;
                                bool heldAlreadyInParty = false;
                                for (int j = 0; j < playerParty.count; j++) {
                                    if (playerParty.slots[j] == pcHeldMon) {
                                        heldAlreadyInParty = true;
                                        break;
                                    }
                                }
                                if (!heldAlreadyInParty && targetSlot == playerParty.count && playerParty.count < 6) {
                                    playerParty.slots[playerParty.count++] = pcHeldMon;
                                    didSwap = true;
                                }
                            }

                            if (didSwap) {
                                //  sync with the party after PC/party edits.
                                syncPartyBoxSpritesToParty(partyBoxSprites, &playerParty);
                                pcSwapIndex = -1;
                                pcHeldMon = NULL;
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < PC_MAX; i++) {
                int x = 0, y = 0;
                pcMenuCursorPos(2 + i, &x, &y);
                pokemonInBattle *mon = (i < pcDisplayCount) ? pcDisplay[i] : NULL;
                if (pcHeldMon != NULL && mon == pcHeldMon) continue;
                const unsigned short *pokeSprite = (mon != NULL) ? menuPokemonSpriteForId(mon->id.frontFrame_ID) : NULL;
                if (pokeSprite != NULL) {
                    draw_sprite_any(pokeSprite,
                                    MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                    x - 10, y + 20,
                                    TRANSPARENT_COLOUR);
                }
            }

            for (int i = 0; i < 6; i++) {
                int x = 0, y = 0;
                pcMenuCursorPos(27 + i, &x, &y);
                const pokemonInBattle *mon = (i >= 0 && i < playerParty.count) ? playerParty.slots[i] : NULL;
                if (pcHeldMon != NULL && mon == pcHeldMon) continue;
                const unsigned short *pokeSprite = (mon != NULL) ? menuPokemonSpriteForId(mon->id.frontFrame_ID) : NULL;
                if (pokeSprite != NULL) {
                    draw_sprite_any(pokeSprite,
                                    MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                    x - 10, y + 20,
                                    TRANSPARENT_COLOUR);
                }
            }

            // Selection cursor
            {
                int cx = 0, cy = 0;
                pcMenuCursorPos(pcCursor, &cx, &cy);
                draw_sprite_any(selectCursorSprite,
                                PC_MENU_SELECT_CURSOR_WIDTH, PC_MENU_SELECT_CURSOR_HEIGHT,
                                cx, cy,
                                TRANSPARENT_COLOUR);
            }

            // Held Pokemon follows the cursor.
            if (pcHeldMon != NULL) {
                const unsigned short *heldSprite = menuPokemonSpriteForId(pcHeldMon->id.frontFrame_ID);
                if (heldSprite != NULL) {
                    int hx = 0, hy = 0;
                    pcMenuCursorPos(pcCursor, &hx, &hy);
                    draw_sprite_any(heldSprite,
                                    MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                    hx, hy - 2,
                                    TRANSPARENT_COLOUR);
                }
            }

            break;
        }
        case GAME_STATE_BATTLE_WIN:
        //check win/lose
        //get money from trainer battle, exp from wild battle if win
        //exp calculations, levelup, evolution, learn moves
            draw_map();
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, battleEndMsg, BLACK);
            if (spacePressed) {
                const int idx = findNextPendingEvolutionIndex(&playerParty, -1);
                if (idx >= 0) {
                    evolutionPokemonIndex = idx;
                    evolutionReturnState = GAME_STATE_MAP;
                    evolutionFrame = 0;
                    evolutionTimer = 0;
                    evolutionPhase = 0;
                    evolutionPhaseTimer = 0;
                    currentGameState = GAME_STATE_EVOLUTION;
                } else {
                    currentGameState = GAME_STATE_MAP;
                }
            }

            break;

        case GAME_STATE_BATTLE_LOSE:

            draw_map();
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, battleEndMsg, BLACK);
            if (spacePressed) {
                healParty(&playerParty);
                currentMapId = WORLD_MAP_HOUSE_1;
                mapReturnX = 152;
                mapReturnY = 192;
                load_world_map(currentMapId, 152, 192, MC_FACING_N);
                play_world_map_bgm(currentMapId);
                currentGameState = GAME_STATE_MAP;
            }
            
            break;

        case GAME_STATE_DIALOGUE:
            draw_map();
            drawMCAnimationPaused();
            draw_textbox_instant_text(textBoxSprite, TEXTBOX_X, TEXTBOX_Y, dialogueText ? dialogueText : "", BLACK);
            if (clearShopOpen) {
                if (spacePressed || enterPressed) {
                    currentGameState =  GAME_STATE_SHOP_UI;
                    clearShopOpen = false;
                }
            }
            else if (spacePressed || enterPressed) {
                currentGameState = dialogueReturnState;
            }
            break;

        case GAME_STATE_POKEMON_CENTER_HEAL: {
            draw_map();
            drawMCAnimationPaused();

            if (!pcHealDone) {
                // How many pokeballs to show (capped at 6)
                const int numBalls = (playerParty.count < 6) ? playerParty.count : 6;
                // Total ticks = one ball per PC_HEAL_TICKS_PER_BALL, then hold
                // Hold long enough for 3 full shade_pulse cycles after all balls appear
                // SHADE_PULSE_FRAME_COUNT=16 frames, each advances every PC_HEAL_ANIM_SPEED ticks
                const int holdTicks = SHADE_PULSE_FRAME_COUNT * PC_HEAL_ANIM_SPEED; // = 96
                const int totalTicks = numBalls * PC_HEAL_TICKS_PER_BALL + holdTicks;

                pcHealTimer++;
                // Trigger SFX once when shining phase starts (all balls placed)
                if (pcHealTimer == 1 && pcHealFrame == numBalls * PC_HEAL_TICKS_PER_BALL) {
                    play_sfx(pokemon_healed_audio, pokemon_healed_audio_len);
                }
                if (pcHealTimer >= PC_HEAL_ANIM_SPEED) {
                    pcHealTimer = 0;
                    pcHealPulseFrame = (pcHealPulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
                    pcHealFrame++;
                }

                // How many pokeballs are visible now
                int visibleBalls = pcHealFrame / PC_HEAL_TICKS_PER_BALL;
                if (visibleBalls > numBalls) visibleBalls = numBalls;

                // Draw each visible pokeball:
                // - placing phase: plain sprite (no glow)
                // - shining phase: shade_pulse glow
                const bool isShiningPhase = (pcHealFrame >= numBalls * PC_HEAL_TICKS_PER_BALL);
                for (int b = 0; b < visibleBalls; b++) {
                    if (isShiningPhase) {
                        draw_sprite_any_shade_pulse(
                            smallPokeballSprite,
                            SMALL_POKEBALL_SPRITE_WIDTH, SMALL_POKEBALL_SPRITE_HEIGHT,
                            PC_HEAL_POKEBALL_PX[b],
                            PC_HEAL_POKEBALL_PY[b],
                            TRANSPARENT_COLOUR,
                            pcHealPulseFrame
                        );
                    } else {
                        draw_sprite_any(
                            smallPokeballSprite,
                            SMALL_POKEBALL_SPRITE_WIDTH, SMALL_POKEBALL_SPRITE_HEIGHT,
                            PC_HEAL_POKEBALL_PX[b],
                            PC_HEAL_POKEBALL_PY[b],
                            TRANSPARENT_COLOUR
                        );
                    }
                }


                if (pcHealFrame >= totalTicks) {
                    // Animation done: heal the party
                    healParty(&playerParty);
                    pcHealDone = true;
                    map_set_nurse_facing_left(false);  // Nurse turns back to face the player
                    dialogueText = NURSE_HEALED_TEXT;
                    dialogueReturnState = GAME_STATE_MAP;
                    currentGameState = GAME_STATE_DIALOGUE;
                }
            }
            break;
        }

        case GAME_STATE_POKEDEX_MENU:
            {
                // Handle scrolling before drawing 
                if (upPressed) {
                    if (pokedexScrollIndex <= POKEMON_ID_CHARMANDER) pokedexScrollIndex = POKEMON_ID_TOGEKISS;
                    else pokedexScrollIndex--;
                    play_sfx(plink_audio, plink_audio_len);
                }
                if (downPressed) {
                    if (pokedexScrollIndex >= POKEMON_ID_TOGEKISS) pokedexScrollIndex = POKEMON_ID_CHARMANDER;
                    else pokedexScrollIndex++;
                    play_sfx(plink_audio, plink_audio_len);
                }

                int pokedexScrollIndexBottom = pokedexScrollIndex + 7;
                int number = 0;

                draw_sprite_any(pokedexPokemonSprite, POKEDEX_MENU_POKEMON_WIDTH, POKEDEX_MENU_POKEMON_HEIGHT, 0, 0, TRANSPARENT_COLOUR);
                draw_sprite_any(pokedexListSprite, POKEDEX_MENU_LIST_WIDTH, POKEDEX_MENU_LIST_HEIGHT, 0, -1, TRANSPARENT_COLOUR);
                draw_sprite_any(pokedexSelectSprite, POKEDEX_MENU_SELECT_WIDTH, POKEDEX_MENU_SELECT_HEIGHT, 3, 3, TRANSPARENT_COLOUR);
                draw_sprite_any(pokedexBottomSprite, POKEDEX_MENU_BOTTOM_WIDTH, POKEDEX_MENU_BOTTOM_HEIGHT, 0, 0, TRANSPARENT_COLOUR);
                draw_sprite_any(scrollMenuSprite, POKEDEX_MENU_SCROLL_MENU_WIDTH, POKEDEX_MENU_SCROLL_MENU_HEIGHT, 320 - POKEDEX_MENU_SCROLL_MENU_WIDTH, 3, TRANSPARENT_COLOUR);

                // For non-selected, they go down by 30 pixels
                // Draw text, pokemon, seen, etc
                for (int i = pokedexScrollIndex; i < pokedexScrollIndexBottom; i++) {
                    const PokemonData *species = speciesFromPokemonSpriteId(i);
                    const char *name = (species != NULL && species->name != NULL) ? species->name : "???";
                    if (number == 0) {
                        pokedexSelectedId = i;
                    }

                    const bool seen = pokedex_is_seen(i);
                    const bool caught = pokedex_is_caught(i);

                    if (seen) {
                        // Small list sprite
                        if (caught) {
                            draw_sprite_any(menuPokemonSpriteForId(i),
                                            MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                            120, -5 + (30 * number),
                                            TRANSPARENT_COLOUR);
                        } else {
                            make_sprite_black(menuPokemonSpriteForId(i),
                                              MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                              120, -5 + (30 * number),
                                              TRANSPARENT_COLOUR);
                        }
                        draw_string_f(197, 8 + (30 * number), name, WHITE, FONT_5X9);


                        if (number == 0) {
                            StaticSprite front = (StaticSprite){0};
                            if (initPokemonFrontBattleSprite(&front, i, 11, 15)) {
                                if (caught) {
                                    drawStaticSprite(&front);
                                } else {
                                    make_sprite_black(front.pixels, front.width, front.height, front.x, front.y, TRANSPARENT_COLOUR);
                                }
                            }
                        }

                        if (caught) {
                            if (number == 0) {
                                draw_sprite_any(pokedexCaughtSelectedSprite,
                                                POKEDEX_MENU_CAUGHT_SELECTED_WIDTH, POKEDEX_MENU_CAUGHT_SELECTED_HEIGHT,
                                                167, 0,
                                                TRANSPARENT_COLOUR);
                            } else {
                                draw_sprite_any(pokedexCaughtSprite,
                                                POKEDEX_MENU_CAUGHT_WIDTH, POKEDEX_MENU_CAUGHT_HEIGHT,
                                                167, (30 * number),
                                                TRANSPARENT_COLOUR);
                            }
                        } else {
                            // Seen but not caught
                            if (number == 0) {
                                draw_sprite_any_greyscale(pokedexCaughtSprite,
                                                          POKEDEX_MENU_CAUGHT_WIDTH, POKEDEX_MENU_CAUGHT_HEIGHT,
                                                          167, 0,
                                                          TRANSPARENT_COLOUR);
                            } else {
                                draw_sprite_any_greyscale(pokedexCaughtSprite,
                                                          POKEDEX_MENU_CAUGHT_WIDTH, POKEDEX_MENU_CAUGHT_HEIGHT,
                                                          167, (30 * number),
                                                          TRANSPARENT_COLOUR);
                            }
                        }
                    } else {
                        // Not seen: show black silhouette sprite + placeholder name.
                        make_sprite_black(menuPokemonSpriteForId(i),
                                          MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                          120, -5 + (30 * number),
                                          TRANSPARENT_COLOUR);
                        if (number == 0) {
                            StaticSprite front = (StaticSprite){0};
                            if (initPokemonFrontBattleSprite(&front, i, 11, 15)) {
                                make_sprite_black(front.pixels, front.width, front.height, front.x, front.y, TRANSPARENT_COLOUR);
                            }
                        }
                        draw_string_f(197, 8 + (30 * number), "???", WHITE, FONT_5X9);
                    }

                    number++;
                }

                if (spacePressed) {
                    pokedexInfoCursor = 1; // default to down arrow
                    currentGameState = GAME_STATE_POKEDEX_INFO;
                }
                if (escPressed) currentGameState = pokedexMenuReturnState;
            }
            break;
    
        case GAME_STATE_POKEDEX_INFO:
            {
                // Cursor navigation across (up arrow, down arrow, X)
                if (leftPressed) {
                    pokedexInfoCursor--;
                    if (pokedexInfoCursor < 0) pokedexInfoCursor = 2;
                    play_sfx(plink_audio, plink_audio_len);
                }
                if (rightPressed) {
                    pokedexInfoCursor++;
                    if (pokedexInfoCursor > 2) pokedexInfoCursor = 0;
                    play_sfx(plink_audio, plink_audio_len);
                }

                // Activate current selection
                if (spacePressed || enterPressed) {
                    if (pokedexInfoCursor == 2) {
                        currentGameState = GAME_STATE_POKEDEX_MENU;
                        play_sfx(plink_audio, plink_audio_len);
                    } else {
                        if (pokedexInfoCursor == 0) {
                            // Up / previous entry
                            if (pokedexSelectedId <= POKEMON_ID_CHARMANDER) pokedexSelectedId = POKEMON_ID_TOGEKISS;
                            else pokedexSelectedId--;
                        } else {
                            // Down / next entry
                            if (pokedexSelectedId >= POKEMON_ID_TOGEKISS) pokedexSelectedId = POKEMON_ID_CHARMANDER;
                            else pokedexSelectedId++;
                        }
                        pokedexScrollIndex = pokedexSelectedId;
                        play_sfx(plink_audio, plink_audio_len);
                    }
                }

                draw_sprite_any(pokemonSelectedPokedexSprite,
                                POKEDEX_MENU_POKEMON_SELECTED_POKEDEX_WIDTH, POKEDEX_MENU_POKEMON_SELECTED_POKEDEX_HEIGHT,
                                0, 0,
                                TRANSPARENT_COLOUR);
                draw_sprite_any(pokedexDescriptionSprite, POKEDEX_MENU_DESCRIPTION_WIDTH, POKEDEX_MENU_DESCRIPTION_HEIGHT, 0, 0, TRANSPARENT_COLOUR);
                draw_sprite_any(pokedexBottomSprite, POKEDEX_MENU_BOTTOM_WIDTH, POKEDEX_MENU_BOTTOM_HEIGHT, 0, 0, TRANSPARENT_COLOUR);


                shadePulseFrame = (shadePulseFrame + 1) % SHADE_PULSE_FRAME_COUNT;
                if (pokedexInfoCursor == 0) {
                    draw_sprite_any_shade_pulse(pokedexUpArrowSprite, POKEDEX_MENU_UP_ARROW_WIDTH, POKEDEX_MENU_UP_ARROW_HEIGHT, 11, 208, TRANSPARENT_COLOUR, shadePulseFrame);
                    draw_sprite_any(pokedexDownArrowSprite, POKEDEX_MENU_DOWN_ARROW_WIDTH, POKEDEX_MENU_DOWN_ARROW_HEIGHT, 59, 208, TRANSPARENT_COLOUR);
                    draw_sprite_any(pokedexXSprite, POKEDEX_MENU_X_WIDTH, POKEDEX_MENU_X_HEIGHT, 288, 210, TRANSPARENT_COLOUR);
                } else if (pokedexInfoCursor == 1) {
                    draw_sprite_any(pokedexUpArrowSprite, POKEDEX_MENU_UP_ARROW_WIDTH, POKEDEX_MENU_UP_ARROW_HEIGHT, 11, 208, TRANSPARENT_COLOUR);
                    draw_sprite_any_shade_pulse(pokedexDownArrowSprite, POKEDEX_MENU_DOWN_ARROW_WIDTH, POKEDEX_MENU_DOWN_ARROW_HEIGHT, 59, 208, TRANSPARENT_COLOUR, shadePulseFrame);
                    draw_sprite_any(pokedexXSprite, POKEDEX_MENU_X_WIDTH, POKEDEX_MENU_X_HEIGHT, 288, 210, TRANSPARENT_COLOUR);
                } else {
                    draw_sprite_any(pokedexUpArrowSprite, POKEDEX_MENU_UP_ARROW_WIDTH, POKEDEX_MENU_UP_ARROW_HEIGHT, 11, 208, TRANSPARENT_COLOUR);
                    draw_sprite_any(pokedexDownArrowSprite, POKEDEX_MENU_DOWN_ARROW_WIDTH, POKEDEX_MENU_DOWN_ARROW_HEIGHT, 59, 208, TRANSPARENT_COLOUR);
                    draw_sprite_any_shade_pulse(pokedexXSprite, POKEDEX_MENU_X_WIDTH, POKEDEX_MENU_X_HEIGHT, 288, 210, TRANSPARENT_COLOUR, shadePulseFrame);
                }

                const int id = pokedexSelectedId;
                const PokemonData *species = speciesFromPokemonSpriteId(id);
                const bool seen = pokedex_is_seen(id);
                const bool caught = pokedex_is_caught(id);

                // Selected pokemon front sprite
                if (seen) {
                    StaticSprite front = (StaticSprite){0};
                    if (initPokemonFrontBattleSprite(&front, id, 17, 12)) {
                        if (caught) {
                            drawStaticSprite(&front);
                            draw_sprite_any(menuPokemonSpriteForId(pokedexSelectedId),
                                            MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                            130, 67,
                                            TRANSPARENT_COLOUR);
                        } else {
                            make_sprite_black(front.pixels, front.width, front.height, front.x, front.y, TRANSPARENT_COLOUR);
                            make_sprite_black(menuPokemonSpriteForId(pokedexSelectedId),
                                            MENU_POKEMON_SPRITE_WIDTH, MENU_POKEMON_SPRITE_HEIGHT,
                                            130, 67,
                                            TRANSPARENT_COLOUR);
                        }
                    }
                }

                // Name + number
                if (species != NULL && seen) {
                    char header[32];
                    snprintf(header, sizeof(header), "#%03d %s", id, species->name ? species->name : "");
                    draw_string_f(171, 22, header, BLACK, FONT_5X9);
                } else {
                    draw_string_f(171, 22, "???", BLACK, FONT_5X9);
                }

                // Types
                if (species != NULL && seen) {
                    int tw = 0, th = 0;
                    const unsigned short *t1 = pokedex_type_sprite_for(species->type1, &tw, &th);
                    if (t1 != NULL) {
                        draw_sprite_any(t1, tw, th, 164, 108, TRANSPARENT_COLOUR);
                    }
                    if (species->type2 != TYPE_NONE) {
                        const unsigned short *t2 = pokedex_type_sprite_for(species->type2, &tw, &th);
                        if (t2 != NULL) {
                            draw_sprite_any(t2, tw, th, 242, 108, TRANSPARENT_COLOUR);
                        }
                    }
                }

                // Pokedex database info
                const PokedexEntry *dex = getPokedexEntry(id);
                if (dex != NULL) {
                    const bool hasDexInfo = seen || caught;

                    if (hasDexInfo && dex->category != NULL) {
                        draw_string_f(174, 43, dex->category, BLACK, FONT_5X9);
                    } else {
                        draw_string_f(174, 43, "???", BLACK, FONT_5X9);
                    }

                    char sizeBufW[48];
                    char sizeBufH[48];
                    if (hasDexInfo) {
                        snprintf(sizeBufW, sizeof(sizeBufW), "W:%ukg", (unsigned)dex->width_kg);
                        snprintf(sizeBufH, sizeof(sizeBufH), "H:%ucm", (unsigned)dex->height_cm);
                    } else {
                        snprintf(sizeBufW, sizeof(sizeBufW), "W:???");
                        snprintf(sizeBufH, sizeof(sizeBufH), "H:???");
                    }
                    draw_string_f(186, 65, sizeBufW, BLACK, FONT_5X9);
                    draw_string_f(186, 85, sizeBufH, BLACK, FONT_5X9);

                    if (hasDexInfo && dex->entry != NULL) {
                        draw_multiline_string_f(16, 147, dex->entry, WHITE, FONT_5X9, 12);
                    } else {
                        draw_multiline_string_f(16, 147, "???", WHITE, FONT_5X9, 12);
                    }
                }

                if (escPressed) currentGameState = pokedexMenuReturnState;
            }
            break;

        case GAME_STATE_SHOP_UI: {
            draw_sprite_any(pokemartBuyScreenSprite,
                            POKEMART_BUY_SCREEN_SPRITE_WIDTH, POKEMART_BUY_SCREEN_SPRITE_HEIGHT,
                            0, 0,
                            TRANSPARENT_COLOUR);
            if (escPressed) currentGameState = GAME_STATE_MAP;
                               
            break;
        }

        case GAME_STATE_MAP:
        default:
            {
                const McBounds mcBoundsBeforeMove = getMCBounds();
                const bool routeBTrainerLock = map_tick_route_b_trainer_event(&mcBoundsBeforeMove);
                draw_map();
                const McMoveResult moveResult = routeBTrainerLock
                    ? mcMovingTick(false, false, false, false, false)
                    : mcMovingTick(upDown, downDown, leftDown, rightDown, is_key_shift_pressed());
                const McBounds mcBounds = getMCBounds();
                WorldMapId targetMap;
                int spawnX;
                int spawnY;
                McFacing spawnFacing;
                if (resolve_map_transition(currentMapId, moveResult,
                                           upDown, downDown, leftDown, rightDown,
                                           &targetMap, &spawnX, &spawnY, &spawnFacing)) {
                    currentMapId = targetMap;
                    load_world_map(currentMapId, spawnX, spawnY, spawnFacing);
                    play_world_map_bgm(currentMapId);
                    draw_map();
                } else if (currentMapId == WORLD_MAP_ROUTE_B &&
                           map_consume_route_b_trainer_arrival()) {
                    dialogueText = TRAINER_A_GREETING_TEXT;
                    dialogueReturnState = GAME_STATE_TRAINER_BATTLE;
                    nextBattleType = BATTLE_TRAINER;
                    currentGameState = GAME_STATE_DIALOGUE;
                } else if (!routeBTrainerLock &&
                           (spacePressed || enterPressed) &&
                           map_try_collect_route_b_pokeball(&mcBounds)) {
                    bagAdd(&playerBag, ITEM_POKEBALL, 1);
                    dialogueText = FOUND_POKEBALL_TEXT;
                    dialogueReturnState = GAME_STATE_MAP;
                    currentGameState = GAME_STATE_DIALOGUE;
                    play_sfx(plink_audio, plink_audio_len);
                } else if (!routeBTrainerLock &&
                           (spacePressed || enterPressed) &&
                           map_can_use_pokemon_center_pc(&mcBounds)) {
                    currentGameState = GAME_STATE_PC_MENU;
                    pcCursor = 0;
                    pcSwapIndex = -1;
                    pcHeldMon = NULL;
                    pcMenuReturnState = GAME_STATE_MAP;
                    play_sfx(pc_se_audio, pc_se_audio_len);
                } else if (!routeBTrainerLock &&
                           (spacePressed || enterPressed) &&
                           map_can_talk_to_pokemon_center_nurse(&mcBounds)) {
                    dialogueText = NURSE_GREETING_TEXT;
                    // After greeting, go to heal animation (not back to map)
                    dialogueReturnState = GAME_STATE_POKEMON_CENTER_HEAL;
                    currentGameState = GAME_STATE_DIALOGUE;
                    pcHealFrame = 0;
                    pcHealTimer = 0;
                    pcHealPulseFrame = 0;
                    pcHealDone = false;
                    map_set_nurse_facing_left(true);  // Nurse turns to face the healing tray
                } else if (!routeBTrainerLock &&
                           (spacePressed || enterPressed) &&
                           map_can_talk_to_poke_mart_clerk(&mcBounds)) {
                    dialogueText = CLERK_GREETING_TEXT;
                    dialogueReturnState = GAME_STATE_MAP;
                    clearShopOpen = true;
                    currentGameState = GAME_STATE_DIALOGUE;
                } else if (!routeBTrainerLock &&
                           (spacePressed || enterPressed) &&
                           map_can_talk_to_pokemon_center_npc1(&mcBounds)) {
                    map_face_pokemon_center_npc1_toward(&mcBounds);
                    dialogueText = NPC1_GREETING_TEXT;
                    dialogueReturnState = GAME_STATE_MAP;
                    currentGameState = GAME_STATE_DIALOGUE;
                } else if (!routeBTrainerLock &&
                           currentMapId == WORLD_MAP_ROUTE_B &&
                           (spacePressed || enterPressed) &&
                           map_can_talk_to_route_b_cynthia(&mcBounds)) {
                    dialogueText = TRAINER_A_GREETING_TEXT;
                    dialogueReturnState = GAME_STATE_TRAINER_BATTLE;
                    nextBattleType = BATTLE_TRAINER;
                    currentGameState = GAME_STATE_DIALOGUE;
                } else if (!routeBTrainerLock &&
                           currentMapId == WORLD_MAP_GYM_1 &&
                           (spacePressed || enterPressed) &&
                           map_can_talk_to_gym_cynthia(&mcBounds)) {
                    dialogueText = CYNTHIA_GREETING_TEXT;
                    dialogueReturnState = GAME_STATE_TRAINER_BATTLE;
                    nextBattleType = BATTLE_TRAINER;
                    play_bgm(cynthia_audio, cynthia_audio_len);
                    currentGameState = GAME_STATE_DIALOGUE;
                } else if (!routeBTrainerLock &&
                           moveResult == MC_MOVE_OK &&
                           should_trigger_grass_battle(upPressed, downPressed, leftPressed, rightPressed)) {
                    nextBattleType = BATTLE_WILD;
                    currentGameState = GAME_STATE_WILD_BATTLE;
                }
            }
            break;
        }

        playTimeFrames++;
        wait_for_vsync();
    }

    return 0;
}

static const char *world_map_display_name(WorldMapId map_id) {
    switch (map_id) {
        case WORLD_MAP_ROUTE_A: return "Route A";
        case WORLD_MAP_ROUTE_B: return "Route B";
        case WORLD_MAP_POKEMON_CENTER_1: return "Pokemon Center";
        case WORLD_MAP_POKE_MART_1: return "Poke Mart";
        case WORLD_MAP_HOUSE_1: return "House";
        case WORLD_MAP_GYM_1: return "Gym";
        case WORLD_MAP_COUNT:
        default:
            return "Unknown";
    }
}

static void play_world_map_bgm(WorldMapId map_id) {
    if (map_id == WORLD_MAP_POKEMON_CENTER_1) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        play_bgm(pokemon_center_audio, pokemon_center_audio_len);
        return;
    }
    if (map_id == WORLD_MAP_POKE_MART_1) {
        play_bgm(poke_mart_audio, poke_mart_audio_len);
        return;
    }

    play_bgm(map_audio, map_audio_len);
}
