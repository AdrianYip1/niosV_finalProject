#pragma once

#include <stdbool.h>

#include "../entities/pokemonObject.h"

//max is 30 pokmeon in pc
#ifndef PC_MAX
#define PC_MAX 25
#endif

typedef struct {
    pokemonInBattle mons[PC_MAX];
    int count;
} PC;

void pcInit(PC *pc);

// Adds a new owned Pokemon instance (dupes allowed).

bool pcAdd(PC *pc, const PokemonData *species, int level, int *outIndex);

pokemonInBattle *pcGet(PC *pc, int index);

