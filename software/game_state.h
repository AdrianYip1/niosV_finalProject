#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

// Game state machine
typedef enum {
    GAME_STATE_MAP = 0,
    GAME_STATE_WILD_BATTLE,
    GAME_STATE_WILD_BATTLE_TRANSITION,
    GAME_STATE_WILD_BATTLE_INTRO_TEXT,

    GAME_STATE_TRAINER_BATTLE,
    GAME_STATE_TRAINER_BATTLE_TRANSITION,
    GAME_STATE_TRAINER_BATTLE_INTRO_TEXT,

    GAME_STATE_BATTLE_ACTION_TEXT,
    GAME_STATE_BATTLE_LEVELUP_STATS,
    GAME_STATE_POKEBALL_THROW,
    GAME_STATE_POKEBALL_CATCH,
    GAME_STATE_BATTLE_WIN,
    GAME_STATE_BATTLE_LOSE,
    GAME_STATE_MENU,
    GAME_STATE_SUMMARY,

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

typedef enum {
    BAG_FOCUS_LIST = 0,
    BAG_FOCUS_RIGHT_ARROW = 1,
    BAG_FOCUS_LEFT_ARROW = 2,
    BAG_FOCUS_CANCEL = 3,
} BagMenuFocus;

// State category queries
static inline bool isBattleMenuState(GameState state) {
    return state == GAME_STATE_WILD_BATTLE || state == GAME_STATE_TRAINER_BATTLE;
}

static inline bool isOverworldState(GameState state) {
    return state == GAME_STATE_MAP ||
           state == GAME_STATE_PC_MENU ||
           state == GAME_STATE_SUMMARY ||
           state == GAME_STATE_PC_MENU ||
           state == GAME_STATE_POKEDEX_MENU ||
           state == GAME_STATE_POKEDEX_INFO ||
           state == GAME_STATE_DIALOGUE ||
           state == GAME_STATE_POKEMON_CENTER_HEAL ||
           state == GAME_STATE_BAG_MENU_ITEMS ||
           state == GAME_STATE_BAG_MENU_POKEBALLS ||
           state == GAME_STATE_BAG_MENU_TMS ||
           state == GAME_STATE_BAG_MENU_BERRIES ||
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

// Bag menu page transitions
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

#endif
