#ifndef PREDEFINED_GRAPHICS_H
#define PREDEFINED_GRAPHICS_H

#include "graphics.h"

// Tile IDs for map
typedef enum {
    TILE_GRASS = 0,
    TILE_BLACK,
    TILE_COUNT
} TileId;

// 16x16 tiles 
extern short tiles[TILE_COUNT][TILE_SIZE * TILE_SIZE];


void init_predefined_graphics(void);


#endif