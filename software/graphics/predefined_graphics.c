#include "graphics.h"
#include "predefined_graphics.h"
#include "predefined_colours.h"

short tiles[TILE_COUNT][TILE_SIZE * TILE_SIZE] = {0};
short player_sprite[TILE_SIZE * TILE_SIZE] = {0};

void init_predefined_graphics(void)
{
   //example with grass
    for (int i = 0; i < TILE_SIZE * TILE_SIZE; i++) {
        tiles[TILE_GRASS][i] = GREEN;
    }

}