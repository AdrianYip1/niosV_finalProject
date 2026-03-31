#include "graphics.h"
#include "predefined_graphics.h"
#include "predefined_colours.h"
#include "backdrops/backdrop1_tiles.h"
#include "backdrops/decor_tiles.h"
#include "backdrops/ground_tiles.h"
#include "backdrops/pokemon_center_interior_tiles.h"
#include "backdrops/poke_mart_interior_tiles.h"
#include "backdrops/house_interior_tiles.h"
#include "backdrops/gym_interior_tiles.h"

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
    memcpy(tiles[TILE_POKEMON_CENTER_TOP_LEFT], pokemon_center_top_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKEMON_CENTER_TOP_MIDDLE], pokemon_center_top_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKEMON_CENTER_TOP_RIGHT], pokemon_center_top_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKEMON_CENTER_MIDDLE_LEFT], pokemon_center_middle_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKEMON_CENTER_MIDDLE_MIDDLE], pokemon_center_middle_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKEMON_CENTER_MIDDLE_RIGHT], pokemon_center_middle_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKEMON_CENTER_BOTTOM_LEFT], pokemon_center_bottom_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKEMON_CENTER_BOTTOM_MIDDLE], pokemon_center_bottom_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKEMON_CENTER_BOTTOM_RIGHT], pokemon_center_bottom_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_TOP_LEFT], poke_mart_top_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_TOP_MIDDLE], poke_mart_top_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_TOP_RIGHT], poke_mart_top_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_MIDDLE_LEFT], poke_mart_middle_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_MIDDLE_MIDDLE], poke_mart_middle_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_MIDDLE_RIGHT], poke_mart_middle_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_BOTTOM_LEFT], poke_mart_bottom_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_BOTTOM_MIDDLE], poke_mart_bottom_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_POKE_MART_BOTTOM_RIGHT], poke_mart_bottom_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_TOP_LEFT], house_top_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_TOP_MIDDLE], house_top_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_TOP_RIGHT], house_top_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_MIDDLE_LEFT], house_middle_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_MIDDLE_MIDDLE], house_middle_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_MIDDLE_RIGHT], house_middle_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_BOTTOM_LEFT], house_bottom_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_BOTTOM_MIDDLE], house_bottom_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_HOUSE_BOTTOM_RIGHT], house_bottom_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_TOP_LEFT], gym_top_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_TOP_MIDDLE], gym_top_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_TOP_RIGHT], gym_top_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_MIDDLE_LEFT], gym_middle_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_MIDDLE_MIDDLE], gym_middle_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_MIDDLE_RIGHT], gym_middle_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_BOTTOM_LEFT], gym_bottom_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_BOTTOM_MIDDLE], gym_bottom_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_GYM_BOTTOM_RIGHT], gym_bottom_right_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_CLIFF_LEFT], cliff_left_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_CLIFF_MIDDLE], cliff_middle_tile, sizeof(tiles[0]));
    memcpy(tiles[TILE_CLIFF_RIGHT], cliff_right_tile, sizeof(tiles[0]));

    for (int i = 0; i < BACKDROP1_TILE_COUNT; i++) {
        memcpy(tiles[TILE_BACKDROP1_START + i], backdrop1_tiles[i], sizeof(tiles[0]));
    }

    for (int i = 0; i < GROUND_TILE_COUNT; i++) {
        memcpy(tiles[TILE_GROUND_START + i], ground_tiles[i], sizeof(tiles[0]));
    }

    for (int i = 0; i < POKEMON_CENTER_INTERIOR_TILE_COUNT; i++) {
        memcpy(tiles[TILE_POKEMON_CENTER_INTERIOR_START + i], pokemon_center_interior_tiles[i], sizeof(tiles[0]));
    }
    for (int i = 0; i < POKE_MART_INTERIOR_TILE_COUNT; i++) {
        memcpy(tiles[TILE_POKE_MART_INTERIOR_START + i], poke_mart_interior_tiles[i], sizeof(tiles[0]));
    }
    for (int i = 0; i < HOUSE_INTERIOR_TILE_COUNT; i++) {
        memcpy(tiles[TILE_HOUSE_INTERIOR_START + i], house_interior_tiles[i], sizeof(tiles[0]));
    }
    for (int i = 0; i < GYM_INTERIOR_TILE_COUNT; i++) {
        memcpy(tiles[TILE_GYM_INTERIOR_START + i], gym_interior_tiles[i], sizeof(tiles[0]));
    }

}
