#ifndef MAP_H
#define MAP_H

#include "predefined_graphics.h"

// Map size in tiles (320x240 = 20x15 tiles)
#define MAP_WIDTH  20
#define MAP_HEIGHT 15

extern TileId map[MAP_HEIGHT][MAP_WIDTH];

// active map preset is active. Change with load_map_preset()
#define MAP_PRESET_COUNT 3
typedef enum {
    MAP_PRESET_ROUTE,
    MAP_PRESET_BOSS,
    MAP_PRESET_BLACK,
    MAP_PRESET_BATTLE
} MapPresetId;

// Load a preset into map. 
void load_map_preset(MapPresetId preset);

void init_map(void); 
void draw_map(void);

#endif
