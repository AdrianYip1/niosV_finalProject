#include "party.h"
 
void initParty(Party *party) {
    party->count = 0;
    party->activeIndex = 0;
    for (int i = 0; i < 6; i++) {
        party->slots[i] = NULL;
    }
}
 
bool addPokemonToParty(Party *party, pokemonInBattle *pokemon) {
    if (party == NULL || pokemon == NULL) return false;
    if (party->count >= 6) return false;
    party->slots[party->count] = pokemon;
    party->count++;
    return true;
}
 
pokemonInBattle *getActivePokemon(Party *party) {
    if (party == NULL) return NULL;
    if (party->activeIndex < 0 || party->activeIndex >= party->count) return NULL;
    return party->slots[party->activeIndex];
}
 
bool hasAlivePokemon(Party *party) {
    if (party == NULL) return false;
    for (int i = 0; i < party->count; i++) {
        if (party->slots[i] != NULL && party->slots[i]->alive) return true;
    }
    return false;
}
 
bool switchPokemon(Party *party, int slot) {
    if (party == NULL) return false;
    if (slot < 0 || slot >= party->count) return false;
    if (party->slots[slot] == NULL) return false;
    if (!party->slots[slot]->alive) return false;
    if (slot == party->activeIndex) return false;

    // stat stages reset when switching.
    pokemonInBattle *oldActive = getActivePokemon(party);
    if (oldActive != NULL) resetStatStages(oldActive);
    resetStatStages(party->slots[slot]);

    party->activeIndex = slot;
    return true;
}
 
int getFirstAlivePokemon(Party *party) {
    if (party == NULL) return -1;
    for (int i = 0; i < party->count; i++) {
        if (party->slots[i] != NULL && party->slots[i]->alive) return i;
    }
    return -1;
}
 
void healParty(Party *party) {
    if (party == NULL) return;
    for (int i = 0; i < party->count; i++) {
        pokemonInBattle *mon = party->slots[i];
        if (mon == NULL) continue;
        mon->alive = true;
        fullRestore(mon);
        restoreAllPP(mon);
    }
}