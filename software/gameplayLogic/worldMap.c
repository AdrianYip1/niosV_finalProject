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

static const MapTransition kRouteATransitions[] = {
    { MAP_TRANSITION_EDGE, MAP_TRANSITION_INPUT_LEFT, 0, 0, MC_MOVE_EXIT_LEFT, WORLD_MAP_ROUTE_B, (MAP_WIDTH * TILE_SIZE) - 24, 112, MC_FACING_W },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_UP, 9, 3, MC_MOVE_OK, WORLD_MAP_POKEMON_CENTER_1, 152, 192, MC_FACING_N },
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_UP, 13, 3, MC_MOVE_OK, WORLD_MAP_POKE_MART_1, 144, 192, MC_FACING_N },
};

static const MapTransition kRouteBTransitions[] = {
    { MAP_TRANSITION_EDGE, MAP_TRANSITION_INPUT_RIGHT, 0, 0, MC_MOVE_EXIT_RIGHT, WORLD_MAP_ROUTE_A, 8, 112, MC_FACING_E },
};

static const MapTransition kPokemonCenter1Transitions[] = {
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 9, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 152, 64, MC_FACING_S },
};

static const MapTransition kPokeMart1Transitions[] = {
    { MAP_TRANSITION_TILE, MAP_TRANSITION_INPUT_DOWN, 9, 13, MC_MOVE_OK, WORLD_MAP_ROUTE_A, 216, 64, MC_FACING_S },
};

static const WorldMapDefinition kWorldMaps[WORLD_MAP_COUNT] = {
    { MAP_PRESET_GROUND, MAP_DECOR_ROUTE_A, kRouteATransitions, (int)(sizeof(kRouteATransitions) / sizeof(kRouteATransitions[0])) },
    { MAP_PRESET_GROUND, MAP_DECOR_ROUTE_B, kRouteBTransitions, (int)(sizeof(kRouteBTransitions) / sizeof(kRouteBTransitions[0])) },
    { MAP_PRESET_POKEMON_CENTER_INTERIOR, MAP_DECOR_NONE, kPokemonCenter1Transitions, (int)(sizeof(kPokemonCenter1Transitions) / sizeof(kPokemonCenter1Transitions[0])) },
    { MAP_PRESET_POKE_MART_INTERIOR, MAP_DECOR_NONE, kPokeMart1Transitions, (int)(sizeof(kPokeMart1Transitions) / sizeof(kPokeMart1Transitions[0])) },
};

void load_world_map(WorldMapId map_id, int start_x, int start_y, McFacing facing) {
    const WorldMapDefinition *def = &kWorldMaps[(int)map_id];
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

bool resolve_map_transition(WorldMapId current_map,
                            McMoveResult move_result,
                            bool upPressed, bool downPressed,
                            bool leftPressed, bool rightPressed,
                            WorldMapId *out_target_map,
                            int *out_spawn_x, int *out_spawn_y,
                            McFacing *out_spawn_facing) {
    const WorldMapDefinition *def = &kWorldMaps[(int)current_map];
    const McBounds bounds = getMCBounds();
    const int foot_tile_x = bounds.valid ? ((bounds.x0 + bounds.x1) / 2) / TILE_SIZE : -1;
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

    return false;
}
