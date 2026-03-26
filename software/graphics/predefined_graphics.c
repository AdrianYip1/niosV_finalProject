#include "graphics.h"
#include "predefined_graphics.h"
#include "predefined_colours.h"
#include "backdrops/backdrop1_tiles.h"
#include "backdrops/decor_tiles.h"
#include "backdrops/ground_tiles.h"

#include <string.h>

short tiles[TILE_COUNT][TILE_SIZE * TILE_SIZE] = {0};

void init_predefined_graphics(void)
{
   //precompute graphics for tiles maybe
    for (int i = 0; i < TILE_SIZE * TILE_SIZE; i++) {
        tiles[TILE_GRASS][i] = GREEN;
        tiles[TILE_BLACK][i] = BLACK;
    }

    memcpy(tiles[TILE_TREE_TOP], tree_top_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_TREE_BOTTOM], tree_bottom_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GRASS_PATCH], grass_patch_tile, sizeof(tiles[0]));

    for (int i = 0; i < BACKDROP1_TILE_COUNT; i++) {
        memcpy(tiles[TILE_BACKDROP1_START + i], backdrop1_tiles[i], sizeof(tiles[0]));
    }

    for (int i = 0; i < GROUND_TILE_COUNT; i++) {
        memcpy(tiles[TILE_GROUND_START + i], ground_tiles[i], sizeof(tiles[0]));
    }

}
