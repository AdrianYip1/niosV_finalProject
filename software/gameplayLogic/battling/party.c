#include "party.h"
 
void initParty(Party *party) {
    party->count = 0;
    party->activeIndex = 0;
    for (int i = 0; i < 6; i++) {
        party->slots[i].alive = false;
    }
}
 
bool addPokemonToParty(Party *party, const PokemonData *species, int level) {
    if (party->count >= 6) return false;
    initPokemonInBattle(&party->slots[party->count], species, level);
    party->count++;
    return true;
}
 
pokemonInBattle *getActivePokemon(Party *party) {
    return &party->slots[party->activeIndex];
}
 
bool hasAlivePokemon(Party *party) {
    for (int i = 0; i < party->count; i++) {
        if (party->slots[i].alive) return true;
    }
    return false;
}
 
bool switchPokemon(Party *party, int slot) {
    if (slot < 0 || slot >= party->count) return false;
    if (!party->slots[slot].alive) return false;
    if (slot == party->activeIndex) return false;
    party->activeIndex = slot;
    return true;
}
 
int getFirstAlivePokemon(Party *party) {
    for (int i = 0; i < party->count; i++) {
        if (party->slots[i].alive) return i;
    }
    return -1;
}
 