#include "worldMap.h"

#include "../graphics/map.h"

typedef enum {
    MAP_TRANSITION_INPUT_UP = 0,
    MAP_TRANSITION_INPUT_DOWN = 1,
    MAP_TRANSITION_INPUT_LEFT = 2,
    MAP_TRANSITION_INPUT_RIGHT = 3,
} MapTransitionInput;

typedef enum {
    MAP_TRANSITION_EDGE = 0,
    MAP_TRANSITION_TILE = 1,
} MapTransitionType;

typedef struct {
    MapTransitionType type;
    MapTransitionInput input;
    int tile_x;
    int tile_y;
    McMoveResult edge_result;
    WorldMapId target_map;
    int spawn_x;
    int spawn_y;
    McFacing spawn_facing;
} MapTransition;

typedef struct {
    MapPresetId preset;
    MapDecorLayout decor;
    const MapTransition *transitions;
    int transition_count;
} WorldMapDefinition;

static bool matches_building_entrance_tile(MapTilePosition building_pos,
                                           int foot_tile_x,
                                           int foot_tile_y);
static void get_building_exterior_spawn(MapTilePosition building_pos,
                                        int *spawn_x,
                                        int *spawn_y);
static void get_building_doorway_spawn(MapTilePosition building_pos,
                                       int *spawn_x,
                                       int *spawn_y);
static void get_interior_entry_spawn(WorldMapId target_map,
                                     int *spawn_x,
                                     int *spawn_y,
                                     McFacing *spawn_facing);

static const MapTransition kRouteATransitions[] = {
    { MAP_TRANSITION_EDGE, MAP_TRANSITION_INPUT_LEFT, 0, 0, MC_MOVE_EXIT_LEFT, WORLD_MAP_ROUTE_B, (MAP_WIDTH * TILE_SIZE) - 24, 112, MC_FACING_W },
};

static const MapTransition kRouteBTransitions[] = {
    { MAP_TRANSITION_EDGE, MAP_TRANSITION_INPUT_RIGHT, 0, 0, MC_MOVE_EXIT_RIGHT, WORLD_MAP_ROUTE_A, 8, 112, MC_FACING_E },
};

static const MapTransition kPokemonCenter1Transitions[] = {
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 8, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 152, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 9, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 152, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 10, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 152, 64, MC_FACING_S },
};

static const MapTransition kPokeMart1Transitions[] = {
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 5, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 6, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 7, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 8, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 5, 14, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 6, 14, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 7, 14, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 8, 14, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
};

static const MapTransition kHouse1Transitions[] = {
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 8, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 280, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 9, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 280, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 10, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 280, 64, MC_FACING_S },
};

static const MapTransition kGym1Transitions[] = {
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 8, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_B, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 9, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_B, 216, 64, MC_FACING_S },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 10, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_B, 216, 64, MC_FACING_S },
};

static const WorldMapDefinition kWorldMaps[WORLD_MAP_COUNT] = {
    { MAP_PRESET_GROUND, MAP_DECOR_ROUTE_A, kRouteATransitions, (int)(sizeof(kRouteATransitions) / sizeof(kRouteATransitions[0])) },
    { MAP_PRESET_GROUND, MAP_DECOR_ROUTE_B, kRouteBTransitions, (int)(sizeof(kRouteBTransitions) / sizeof(kRouteBTransitions[0])) },
    { MAP_PRESET_POKEMON_CENTER_INTERIOR, MAP_DECOR_NONE, kPokemonCenter1Transitions, (int)(sizeof(kPokemonCenter1Transitions) / sizeof(kPokemonCenter1Transitions[0])) },
    { MAP_PRESET_POKE_MART_INTERIOR, MAP_DECOR_NONE, kPokeMart1Transitions, (int)(sizeof(kPokeMart1Transitions) / sizeof(kPokeMart1Transitions[0])) },
    { MAP_PRESET_HOUSE_INTERIOR, MAP_DECOR_NONE, kHouse1Transitions, (int)(sizeof(kHouse1Transitions) / sizeof(kHouse1Transitions[0])) },
    { MAP_PRESET_GYM_INTERIOR, MAP_DECOR_NONE, kGym1Transitions, (int)(sizeof(kGym1Transitions) / sizeof(kGym1Transitions[0])) },
};

void load_world_map(WorldMapId map_id, int start_x, int start_y, McFacing facing) {
    const WorldMapDefinition *def = &kWorldMaps[(int)map_id];
    if (def->preset == MAP_PRESET_POKEMON_CENTER_INTERIOR ||
        def->preset == MAP_PRESET_POKE_MART_INTERIOR ||
        def->preset == MAP_PRESET_HOUSE_INTERIOR ||
        def->preset == MAP_PRESET_GYM_INTERIOR) {
        setMCScale(3, 2);
    } else {
        setMCScale(1, 1);
    }
    init_map();
    load_map_preset(def->preset);
    apply_map_decor_layout(def->decor);
    mcMovingInit(start_x, start_y, facing);
}

static bool is_transition_input_pressed(MapTransitionInput input,
                                        bool upPressed, bool downPressed,
                                        bool leftPressed, bool rightPressed) {
    switch (input) {
        case MAP_TRANSITION_INPUT_UP: return upPressed;
        case MAP_TRANSITION_INPUT_DOWN: return downPressed;
        case MAP_TRANSITION_INPUT_LEFT: return leftPressed;
        case MAP_TRANSITION_INPUT_RIGHT: return rightPressed;
        default: return false;
    }
}

static bool matches_building_entrance_tile(MapTilePosition building_pos,
                                           int foot_tile_x,
                                           int foot_tile_y) {
    return (foot_tile_x == building_pos.x && foot_tile_y == building_pos.y + 3) ||
           (foot_tile_x == building_pos.x + 1 && foot_tile_y == building_pos.y + 3) ||
           (foot_tile_x == building_pos.x + 1 && foot_tile_y == building_pos.y + 2) ||
           (foot_tile_x == building_pos.x + 2 && foot_tile_y == building_pos.y + 3);
}

static void get_building_exterior_spawn(MapTilePosition building_pos,
                                        int *spawn_x,
                                        int *spawn_y) {
    if (spawn_x != NULL) {
        *spawn_x = (building_pos.x * TILE_SIZE) + ((3 * TILE_SIZE) / 2);
    }
    if (spawn_y != NULL) {
        *spawn_y = (building_pos.y + 3) * TILE_SIZE;
    }
}

static void get_building_doorway_spawn(MapTilePosition building_pos,
                                       int *spawn_x,
                                       int *spawn_y) {
    if (spawn_x != NULL) {
        *spawn_x = (building_pos.x * TILE_SIZE) + ((3 * TILE_SIZE) / 2);
    }
    if (spawn_y != NULL) {
        *spawn_y = (building_pos.y + 2) * TILE_SIZE;
    }
}

static void get_interior_entry_spawn(WorldMapId target_map,
                                     int *spawn_x,
                                     int *spawn_y,
                                     McFacing *spawn_facing) {
    if (spawn_x == NULL || spawn_y == NULL || spawn_facing == NULL) {
        return;
    }

    *spawn_facing = MC_FACING_N;

    switch (target_map) {
        case WORLD_MAP_POKEMON_CENTER_1:
        case WORLD_MAP_HOUSE_1:
        case WORLD_MAP_GYM_1:
            *spawn_x = 152;
            *spawn_y = 192;
            break;
        case WORLD_MAP_POKE_MART_1:
            *spawn_x = 144;
            *spawn_y = 192;
            break;
        default:
            *spawn_x = 152;
            *spawn_y = 192;
            break;
    }
}

bool resolve_map_transition(WorldMapId current_map,
                            McMoveResult move_result,
                            bool upPressed, bool downPressed,
                            bool leftPressed, bool rightPressed,
                            WorldMapId *out_target_map,
                            int *out_spawn_x, int *out_spawn_y,
                            McFacing *out_spawn_facing) {
    const WorldMapDefinition *def = &kWorldMaps[(int)current_map];
    const McBounds bounds = getMCBounds();
    const int foot_tile_x = bounds.valid ? (((bounds.x0 + bounds.x1) / 2) / TILE_SIZE) : -1;
    const int foot_tile_y = bounds.valid ? (bounds.y1 / TILE_SIZE) : -1;

    for (int i = 0; i < def->transition_count; i++) {
        const MapTransition *transition = &def->transitions[i];
        if (!is_transition_input_pressed(transition->input, upPressed, downPressed, leftPressed, rightPressed)) {
            continue;
        }

        if (transition->type == MAP_TRANSITION_EDGE) {
            if (transition->edge_result != move_result) {
                continue;
            }
        } else if (transition->tile_x != foot_tile_x || transition->tile_y != foot_tile_y) {
            continue;
        }

        *out_target_map = transition->target_map;
        *out_spawn_x = transition->spawn_x;
        *out_spawn_y = transition->spawn_y;
        *out_spawn_facing = transition->spawn_facing;
        return true;
    }

    if (upPressed && current_map == WORLD_MAP_ROUTE_A) {
        MapTilePosition pokemon_center_pos;
        MapTilePosition poke_mart_pos;
        MapTilePosition house_pos;

        if (map_get_route_a_pokemon_center_position(&pokemon_center_pos) &&
            matches_building_entrance_tile(pokemon_center_pos, foot_tile_x, foot_tile_y)) {
            *out_target_map = WORLD_MAP_POKEMON_CENTER_1;
            get_interior_entry_spawn(*out_target_map, out_spawn_x, out_spawn_y, out_spawn_facing);
            return true;
        }

        if (map_get_route_a_poke_mart_position(&poke_mart_pos) &&
            matches_building_entrance_tile(poke_mart_pos, foot_tile_x, foot_tile_y)) {
            *out_target_map = WORLD_MAP_POKE_MART_1;
            get_interior_entry_spawn(*out_target_map, out_spawn_x, out_spawn_y, out_spawn_facing);
            return true;
        }

        if (map_get_route_a_house_position(&house_pos) &&
            matches_building_entrance_tile(house_pos, foot_tile_x, foot_tile_y)) {
            *out_target_map = WORLD_MAP_HOUSE_1;
            get_interior_entry_spawn(*out_target_map, out_spawn_x, out_spawn_y, out_spawn_facing);
            return true;
        }
    }

    if (upPressed && current_map == WORLD_MAP_ROUTE_B) {
        MapTilePosition gym_pos;

        if (map_get_route_b_gym_position(&gym_pos) &&
            matches_building_entrance_tile(gym_pos, foot_tile_x, foot_tile_y)) {
            *out_target_map = WORLD_MAP_GYM_1;
            get_interior_entry_spawn(*out_target_map, out_spawn_x, out_spawn_y, out_spawn_facing);
            return true;
        }
    }

    if (downPressed && current_map == WORLD_MAP_POKEMON_CENTER_1) {
        MapTilePosition pokemon_center_pos;

        if (map_get_route_a_pokemon_center_position(&pokemon_center_pos) &&
            foot_tile_x >= 8 && foot_tile_x <= 10 && foot_tile_y == 13) {
            *out_target_map = WORLD_MAP_ROUTE_A;
            get_building_exterior_spawn(pokemon_center_pos, out_spawn_x, out_spawn_y);
            *out_spawn_facing = MC_FACING_S;
            return true;
        }
    }

    if (downPressed && current_map == WORLD_MAP_POKE_MART_1) {
        MapTilePosition poke_mart_pos;

        if (map_get_route_a_poke_mart_position(&poke_mart_pos) &&
            foot_tile_x >= 5 && foot_tile_x <= 8 && foot_tile_y >= 13 && foot_tile_y <= 14) {
            *out_target_map = WORLD_MAP_ROUTE_A;
            get_building_exterior_spawn(poke_mart_pos, out_spawn_x, out_spawn_y);
            *out_spawn_facing = MC_FACING_S;
            return true;
        }
    }

    if (downPressed && current_map == WORLD_MAP_HOUSE_1) {
        MapTilePosition house_pos;

        if (map_get_route_a_house_position(&house_pos) &&
            foot_tile_x >= 8 && foot_tile_x <= 10 && foot_tile_y == 13) {
            *out_target_map = WORLD_MAP_ROUTE_A;
            get_building_exterior_spawn(house_pos, out_spawn_x, out_spawn_y);
            *out_spawn_facing = MC_FACING_S;
            return true;
        }
    }

    if (downPressed && current_map == WORLD_MAP_GYM_1) {
        MapTilePosition gym_pos;

        if (map_get_route_b_gym_position(&gym_pos) &&
            foot_tile_x >= 8 && foot_tile_x <= 10 && foot_tile_y == 13) {
            *out_target_map = WORLD_MAP_ROUTE_B;
            get_building_doorway_spawn(gym_pos, out_spawn_x, out_spawn_y);
            *out_spawn_facing = MC_FACING_S;
            return true;
        }
    }

    return false;
}
