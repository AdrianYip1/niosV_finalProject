#pragma once

#include <stdint.h>

typedef struct {
    int pokemon_id;               
    const char *category;           
    uint16_t height_cm;            
    uint16_t width_kg;             
    const char *entry;            
} PokedexEntry;

const PokedexEntry *getPokedexEntry(int pokemon_id);
