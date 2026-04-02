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
        case 1: return LEARNSET_CHARMANDER;
        case 2: return LEARNSET_CHARMELEON;
        case 3: return LEARNSET_CHARIZARD;
        case 4: return LEARNSET_RAYQUAZA;
        case 13: return LEARNSET_GIBLE;
        case 14: return LEARNSET_GABITE;
        case 5: return LEARNSET_GARCHOMP;
        case 15: return LEARNSET_RIOLU;
        case 6: return LEARNSET_LUCARIO;
        case 12: return LEARNSET_FEEBAS;
        case 7: return LEARNSET_MILOTIC;
        case 11: return LEARNSET_BUDEW;
        case 8: return LEARNSET_ROSERADE;
        case 9: return LEARNSET_SPIRITOMB;
        case 16: return LEARNSET_TOGEPI;
        case 17: return LEARNSET_TOGETIC;
        case 10: return LEARNSET_TOGEKISS;
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
