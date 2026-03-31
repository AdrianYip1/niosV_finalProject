#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "predefined_graphics.h"
#include "mcWalkingDraw.h"
#include <stddef.h>

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
    MAP_PRESET_HOUSE_INTERIOR,
    MAP_PRESET_GYM_INTERIOR,
    MAP_PRESET_COUNT,
} MapPresetId;

// Load a preset into map. 
void load_map_preset(MapPresetId preset);

bool map_set_tile_xy(int x, int y, TileId tile);
bool map_place_tree_xy(int x, int y);
bool map_place_pokemon_center_xy(int x, int y);
bool map_place_poke_mart_xy(int x, int y);
bool map_place_house_xy(int x, int y);
bool map_place_gym_xy(int x, int y);
bool map_set_overlay_tile_xy(int x, int y, TileId tile);
bool map_is_walkable_tile(TileId tile);
bool map_is_walkable_at_xy(int x, int y);
bool map_bounds_are_walkable(int x0, int y0, int x1, int y1, int dx, int dy);
void apply_map_decor(void);
void apply_map_decor_layout(MapDecorLayout layout);
void draw_map_cell(int x, int y);
bool map_can_talk_to_route_b_cynthia(const McBounds *bounds);
bool map_can_talk_to_gym_cynthia(const McBounds *bounds);
bool map_can_talk_to_pokemon_center_nurse(const McBounds *bounds);
bool map_can_talk_to_pokemon_center_npc1(const McBounds *bounds);
void map_face_pokemon_center_npc1_toward(const McBounds *bounds);
bool map_can_talk_to_poke_mart_clerk(const McBounds *bounds);
bool map_is_mc_on_grass_patch(const McBounds *bounds);
bool map_can_use_pokemon_center_pc(const McBounds *bounds);
bool map_get_route_a_pokemon_center_position(MapTilePosition *out_position);
bool map_get_route_a_poke_mart_position(MapTilePosition *out_position);
bool map_get_route_a_house_position(MapTilePosition *out_position);
bool map_get_route_b_gym_position(MapTilePosition *out_position);
void map_set_route_b_trainer_defeated(bool defeated);
bool map_is_route_b_trainer_defeated(void);
void map_reset_route_b_trainer(void);
bool map_tick_route_b_trainer_event(const McBounds *bounds);
bool map_consume_route_b_trainer_arrival(void);
bool map_try_collect_route_b_pokeball(const McBounds *bounds);

void init_map(void); 
void draw_map(void);
void map_set_nurse_facing_left(bool left);


#endif
