#pragma once
#include "pokemonDataBase.h"
#include "pokemonObject.h"
#include "../../graphics/sprites/pokemon/pokemonSpriteInit.h"

typedef struct {
    int level;
    const AttackData *move;
} LearnsetEntry;

#define LEARNSET_END {0, NULL}

extern const LearnsetEntry LEARNSET_CHARMANDER[];
extern const LearnsetEntry LEARNSET_CHARMELEON[];
extern const LearnsetEntry LEARNSET_CHARIZARD[];
extern const LearnsetEntry LEARNSET_RAYQUAZA[];
extern const LearnsetEntry LEARNSET_GIBLE[];
extern const LearnsetEntry LEARNSET_GABITE[];
extern const LearnsetEntry LEARNSET_GARCHOMP[];
extern const LearnsetEntry LEARNSET_RIOLU[];
extern const LearnsetEntry LEARNSET_LUCARIO[];
extern const LearnsetEntry LEARNSET_FEEBAS[];
extern const LearnsetEntry LEARNSET_MILOTIC[];
extern const LearnsetEntry LEARNSET_BUDEW[];
extern const LearnsetEntry LEARNSET_ROSERADE[];
extern const LearnsetEntry LEARNSET_SPIRITOMB[];
extern const LearnsetEntry LEARNSET_TOGEPI[];
extern const LearnsetEntry LEARNSET_TOGETIC[];
extern const LearnsetEntry LEARNSET_TOGEKISS[];


static inline const LearnsetEntry *getLearnset(int speciesId) {
    switch (speciesId) {
        case POKEMON_ID_CHARMANDER: return LEARNSET_CHARMANDER;
        case POKEMON_ID_CHARMELEON: return LEARNSET_CHARMELEON;
        case POKEMON_ID_CHARIZARD:  return LEARNSET_CHARIZARD;
        case POKEMON_ID_GIBLE:      return LEARNSET_GIBLE;
        case POKEMON_ID_GABITE:     return LEARNSET_GABITE;
        case POKEMON_ID_GARCHOMP:   return LEARNSET_GARCHOMP;
        case POKEMON_ID_FEEBAS:     return LEARNSET_FEEBAS;
        case POKEMON_ID_MILOTIC:    return LEARNSET_MILOTIC;
        case POKEMON_ID_RIOLU:      return LEARNSET_RIOLU;
        case POKEMON_ID_LUCARIO:    return LEARNSET_LUCARIO;
        case POKEMON_ID_BUDEW:      return LEARNSET_BUDEW;
        case POKEMON_ID_ROSERADE:   return LEARNSET_ROSERADE;
        case POKEMON_ID_TOGEPI:     return LEARNSET_TOGEPI;
        case POKEMON_ID_TOGETIC:    return LEARNSET_TOGETIC;
        case POKEMON_ID_TOGEKISS:   return LEARNSET_TOGEKISS;
        case POKEMON_ID_SPIRITOMB:  return LEARNSET_SPIRITOMB;
        case POKEMON_ID_RAYQUAZA:   return LEARNSET_RAYQUAZA;
        default: return NULL;
    }
}

static inline int learnMove(pokemonInBattle *pokemon, const AttackData *move, int slotToReplace) {
    for (int i = 0; i < 4; i++) {
        if (pokemon->attacks[i] == NULL) {
            pokemon->attacks[i] = move;
            pokemon->currentPP[i] = move->maxPP;
            return i;
        }
    }
    if (slotToReplace >= 0 && slotToReplace < 4) {
        pokemon->attacks[slotToReplace] = move;
        pokemon->currentPP[slotToReplace] = move->maxPP;
        return slotToReplace;
    }
    return -1;
}

static inline void initMovesFromLearnset(pokemonInBattle *pokemon) {
    for (int i = 0; i < 4; i++) {
        pokemon->attacks[i] = NULL;
        pokemon->currentPP[i] = 0;
    }
    const LearnsetEntry *learnset = getLearnset(pokemon->id.data->id);
    if (learnset == NULL) return;
    int count = 0;
    for (int e = 0; learnset[e].move != NULL; e++) {
        if (learnset[e].level <= pokemon->level) count++;
    }
    int skip = (count > 4) ? count - 4 : 0;
    int seen = 0;
    for (int e = 0; learnset[e].move != NULL; e++) {
        if (learnset[e].level <= pokemon->level) {
            if (seen >= skip) learnMove(pokemon, learnset[e].move, -1);
            seen++;
        }
    }
}

static inline void checkLevelUpMoves(pokemonInBattle *pokemon, void (*onLearnMove)(pokemonInBattle *, const AttackData *)) {
    const LearnsetEntry *learnset = getLearnset(pokemon->id.data->id);
    if (learnset == NULL || onLearnMove == NULL) return;
    for (int e = 0; learnset[e].move != NULL; e++) {
        if (learnset[e].level == pokemon->level) onLearnMove(pokemon, learnset[e].move);
    }
}
