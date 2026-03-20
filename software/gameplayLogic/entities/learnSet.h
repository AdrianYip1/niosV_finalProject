#pragma once
#include "pokemonDataBase.h"
#include "pokemonObject.h"

typedef struct {
    int level;
    const AttackData *move;
} LearnsetEntry;

#define LEARNSET_END {0, NULL}

extern const LearnsetEntry LEARNSET_CHARMANDER[];
extern const LearnsetEntry LEARNSET_CHARMELEON[];
extern const LearnsetEntry LEARNSET_CHARIZARD[];

static inline const LearnsetEntry *getLearnset(int speciesId) {
    switch (speciesId) {
        case 1: return LEARNSET_CHARMANDER;
        case 2: return LEARNSET_CHARMELEON;
        case 3: return LEARNSET_CHARIZARD;
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