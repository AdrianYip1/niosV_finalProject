#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "predefined_graphics.h"

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
    MAP_PRESET_ROUTE,
    MAP_PRESET_BLACK,
    MAP_PRESET_BACKDROP1,
    MAP_PRESET_GROUND,
    MAP_PRESET_COUNT,
} MapPresetId;

// Load a preset into map. 
void load_map_preset(MapPresetId preset);

bool map_set_tile_xy(int x, int y, TileId tile);
bool map_place_tree_xy(int x, int y);
bool map_set_overlay_tile_xy(int x, int y, TileId tile);
bool map_is_walkable_tile(TileId tile);
bool map_is_walkable_at_xy(int x, int y);
bool map_bounds_are_walkable(int x0, int y0, int x1, int y1);
void apply_map_decor(void);
void draw_map_cell(int x, int y);

void init_map(void); 
void draw_map(void);

#endif
