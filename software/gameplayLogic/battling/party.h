#pragma once
#include "../entities/pokemonObject.h"
 
typedef struct {
    pokemonInBattle slots[6];
    int count;
    int activeIndex;
} Party;
 
void initParty(Party *party);
bool addPokemonToParty(Party *party, const PokemonData *species, int level);
pokemonInBattle *getActivePokemon(Party *party);
bool hasAlivePokemon(Party *party);
bool switchPokemon(Party *party, int slot);
int getFirstAlivePokemon(Party *party);
 