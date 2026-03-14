#include "graphics.h"
#include "predefined_graphics.h"
#include "predefined_colours.h"

short tiles[TILE_COUNT][TILE_SIZE * TILE_SIZE] = {0};

void init_predefined_graphics(void)
{
   //precompute graphics for tiles maybe
    for (int i = 0; i < TILE_SIZE * TILE_SIZE; i++) {
        tiles[TILE_GRASS][i] = GREEN;
        tiles[TILE_BLACK][i] = BLACK;
    }

}