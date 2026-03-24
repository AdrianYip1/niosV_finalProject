#pragma once

#include <stdbool.h>
#include "../staticSprite.h"

#define POKEMON_ID_CHARMANDER 1
#define POKEMON_ID_CHARMELEON 2
#define POKEMON_ID_CHARIZARD  3


#define POKEMON_BATTLE_BACK_X  32
#define POKEMON_BATTLE_BACK_Y  77

#define POKEMON_BATTLE_FRONT_X 190
#define POKEMON_BATTLE_FRONT_Y 17

// Battle sprite initializers (front/back only). Returns false if pokemon_id is unknown.
bool initPokemonBackBattleSprite(StaticSprite* out, int pokemon_id, int x, int y);
bool initPokemonFrontBattleSprite(StaticSprite* out, int pokemon_id, int x, int y);

bool setPokemonBattleSpriteId(StaticSprite* sprite, int pokemon_id, bool want_front);

static inline bool setPokemonBackBattleSpriteId(StaticSprite* sprite, int pokemon_id) {
    return setPokemonBattleSpriteId(sprite, pokemon_id, false);
}
static inline bool setPokemonFrontBattleSpriteId(StaticSprite* sprite, int pokemon_id) {
    return setPokemonBattleSpriteId(sprite, pokemon_id, true);
}


static inline bool initPokemonBackBattleSpriteDefault(StaticSprite* out, int pokemon_id) {
    return initPokemonBackBattleSprite(out, pokemon_id, POKEMON_BATTLE_BACK_X, POKEMON_BATTLE_BACK_Y);
}
static inline bool initPokemonFrontBattleSpriteDefault(StaticSprite* out, int pokemon_id) {
    return initPokemonFrontBattleSprite(out, pokemon_id, POKEMON_BATTLE_FRONT_X, POKEMON_BATTLE_FRONT_Y);
}

static inline bool initCharmanderBackBattleSprite(StaticSprite* out, int x, int y) {
    return initPokemonBackBattleSprite(out, POKEMON_ID_CHARMANDER, x, y);
}
static inline bool initCharmanderFrontBattleSprite(StaticSprite* out, int x, int y) {
    return initPokemonFrontBattleSprite(out, POKEMON_ID_CHARMANDER, x, y);
}
static inline bool initCharmeleonBackBattleSprite(StaticSprite* out, int x, int y) {
    return initPokemonBackBattleSprite(out, POKEMON_ID_CHARMELEON, x, y);
}
static inline bool initCharmeleonFrontBattleSprite(StaticSprite* out, int x, int y) {
    return initPokemonFrontBattleSprite(out, POKEMON_ID_CHARMELEON, x, y);
}
static inline bool initCharizardBackBattleSprite(StaticSprite* out, int x, int y) {
    return initPokemonBackBattleSprite(out, POKEMON_ID_CHARIZARD, x, y);
}
static inline bool initCharizardFrontBattleSprite(StaticSprite* out, int x, int y) {
    return initPokemonFrontBattleSprite(out, POKEMON_ID_CHARIZARD, x, y);
}
