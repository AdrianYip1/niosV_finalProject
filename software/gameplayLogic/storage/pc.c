#include "pc.h"

void pcInit(PC *pc) {
    if (pc == NULL) return;
    pc->count = 0;
}

bool pcAdd(PC *pc, const PokemonData *species, int level, int *outIndex) {
    if (outIndex) *outIndex = -1;
    if (pc == NULL || species == NULL) return false;
    if (pc->count < 0) pc->count = 0;
    if (pc->count >= PC_MAX) return false;

    const int idx = pc->count;
    initPokemonInBattle(&pc->mons[idx], species, level);
    pc->count++;
    if (outIndex) *outIndex = idx;
    return true;
}

pokemonInBattle *pcGet(PC *pc, int index) {
    if (pc == NULL) return NULL;
    if (index < 0 || index >= pc->count) return NULL;
    return &pc->mons[index];
}

