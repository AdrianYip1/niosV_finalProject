#pragma once

#include <stdbool.h>

#include "../graphics/mcWalkingDraw.h"
#include "map_movement/mcMoving.h"

typedef enum {
    WORLD_MAP_ROUTE_A = 0,
    WORLD_MAP_ROUTE_B = 1,
    WORLD_MAP_POKEMON_CENTER_1 = 2,
    WORLD_MAP_COUNT,
} WorldMapId;

void load_world_map(WorldMapId map_id, int start_x, int start_y, McFacing facing);
bool resolve_map_transition(WorldMapId current_map,
                            McMoveResult move_result,
                            bool upPressed, bool downPressed,
                            bool leftPressed, bool rightPressed,
                            WorldMapId *out_target_map,
                            int *out_spawn_x, int *out_spawn_y,
                            McFacing *out_spawn_facing);
