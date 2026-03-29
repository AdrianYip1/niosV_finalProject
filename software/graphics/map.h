#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "predefined_graphics.h"
#include "mcWalkingDraw.h"

// Map size in tiles (320x240 = 20x15 tiles)
#define MAP_WIDTH  20
#define MAP_HEIGHT 15

extern TileId map[MAP_HEIGHT][MAP_WIDTH];
extern int map_overlay[MAP_HEIGHT][MAP_WIDTH];

typedef struct {
    int x;
    int y;
} MapTilePosition;

typedef enum {
    MAP_DECOR_ROUTE_A = 0,
    MAP_DECOR_ROUTE_B = 1,
    MAP_DECOR_NONE = 2,
} MapDecorLayout;

typedef enum {
    MAP_PRESET_ROUTE,
    MAP_PRESET_BLACK,
    MAP_PRESET_BACKDROP1,
    MAP_PRESET_GROUND,
    MAP_PRESET_POKEMON_CENTER_INTERIOR,
    MAP_PRESET_POKE_MART_INTERIOR,
    MAP_PRESET_COUNT,
} MapPresetId;

// Load a preset into map. 
void load_map_preset(MapPresetId preset);

bool map_set_tile_xy(int x, int y, TileId tile);
bool map_place_tree_xy(int x, int y);
bool map_place_pokemon_center_xy(int x, int y);
bool map_place_poke_mart_xy(int x, int y);
bool map_set_overlay_tile_xy(int x, int y, TileId tile);
bool map_is_walkable_tile(TileId tile);
bool map_is_walkable_at_xy(int x, int y);
bool map_bounds_are_walkable(int x0, int y0, int x1, int y1);
void apply_map_decor(void);
void apply_map_decor_layout(MapDecorLayout layout);
void draw_map_cell(int x, int y);
bool map_can_talk_to_route_b_cynthia(const McBounds *bounds);
bool map_can_talk_to_pokemon_center_nurse(const McBounds *bounds);
bool map_can_talk_to_poke_mart_clerk(const McBounds *bounds);
bool map_is_mc_on_grass_patch(const McBounds *bounds);
bool map_can_use_pokemon_center_pc(const McBounds *bounds);

void init_map(void); 
void draw_map(void);

#endif
