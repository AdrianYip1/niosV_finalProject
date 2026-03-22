#pragma once

#include <stdbool.h>
#include "../staticSprite.h"

// PokemonData.id values (see `software/gameplayLogic/entities/pokemonDataBase.c`)
#define POKEMON_ID_CHARMANDER 1
#define POKEMON_ID_CHARMELEON 2
#define POKEMON_ID_CHARIZARD  3

// Battle sprite initializers (front/back only). Returns false if pokemon_id is unknown.
bool initPokemonBackBattleSprite(StaticSprite* out, int pokemon_id, int x, int y);
bool initPokemonFrontBattleSprite(StaticSprite* out, int pokemon_id, int x, int y);

// Convenience wrappers for the currently supported Pokémon.
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
