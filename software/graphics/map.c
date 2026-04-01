#include "map.h"
#include "graphics.h"
#include "tiles.h"
#include "backdrops/backdrop1_tiles.h"
#include "backdrops/ground_tiles.h"
#include "backdrops/pokemon_center_interior_tiles.h"
#include "backdrops/poke_mart_interior_tiles.h"
#include "backdrops/house_interior_tiles.h"
#include "backdrops/gym_interior_tiles.h"
#include "sprites/pokemonCenter/pokemonCenterClerkSprite.h"
#include "sprites/pokemonCenter/pokemonCenterDeskSprite.h"
#include "sprites/pokemonCenter/pokemonCenterNurseSprite.h"
#include "sprites/pokemonCenter/pokemonCenterNurseLeftSprite.h"
#include "sprites/pokeballSprites/smallPokeballSprite.h"
#include "sprites/trainerSprites/cynthiaFrontMapSprite.h"
#include "sprites/trainerSprites/npc1BackMapSprite.h"
#include "sprites/trainerSprites/npc1FrontMapSprite.h"
#include "sprites/trainerSprites/npc1LeftMapSprite.h"
#include "sprites/trainerSprites/npc1RightMapSprite.h"
#include "sprites/trainerSprites/trainerLeftMapSprite.h"
#include "sprites/trainerSprites/trainerWalking_frames.h"
#include <stddef.h>

TileId map[MAP_HEIGHT][MAP_WIDTH];
int map_overlay[MAP_HEIGHT][MAP_WIDTH];

typedef struct {
    TileId tile;
    int x;
    int y;
} DecorEntry;

typedef struct {
    const MapTilePosition *grass_patch_positions;
    unsigned int grass_patch_count;
    const MapTilePosition *tree_positions;
    unsigned int tree_count;
} MapDecorDefinition;

typedef struct {
    int x;
    int y;
    int length;
} CliffPlacement;

typedef struct {
    int x;
    int y;
    int length;
} VerticalCliffPlacement;

static DecorEntry g_decor_entries[MAP_WIDTH * MAP_HEIGHT * 2];
static int g_decor_entry_count = 0;
static MapPresetId g_current_preset = MAP_PRESET_ROUTE;
static MapDecorLayout g_current_decor_layout = MAP_DECOR_ROUTE_A;
static bool g_nurse_facing_left = false;
typedef enum {
    NPC1_FACING_FRONT = 0,
    NPC1_FACING_BACK,
    NPC1_FACING_LEFT,
    NPC1_FACING_RIGHT,
} Npc1Facing;

static Npc1Facing g_npc1_facing = NPC1_FACING_FRONT;
typedef enum {
    ROUTE_B_TRAINER_IDLE = 0,
    ROUTE_B_TRAINER_WALKING,
    ROUTE_B_TRAINER_ARRIVED,
} RouteBTrainerState;

static RouteBTrainerState g_route_b_trainer_state = ROUTE_B_TRAINER_IDLE;
static int g_route_b_trainer_x = 130;
static int g_route_b_trainer_y = 146;
static int g_route_b_trainer_frame = 0;
static int g_route_b_trainer_frame_timer = 0;
static bool g_route_b_trainer_arrival_pending = false;
static bool g_route_b_pokeball_collected = false;
static bool g_route_b_trainer_defeated = false;

#define ROUTE_B_TRAINER_START_X 130
#define ROUTE_B_TRAINER_START_Y 146
#define ROUTE_B_TRAINER_BBOX_INSET_X 3
#define ROUTE_B_TRAINER_BBOX_INSET_Y 8
#define ROUTE_B_TRAINER_BBOX_W (TRAINER_LEFT_MAP_WIDTH - 6)
#define ROUTE_B_TRAINER_BBOX_H (TRAINER_LEFT_MAP_HEIGHT - 8)
#define ROUTE_B_TRAINER_STOP_GAP 4
#define ROUTE_B_TRAINER_FRAME_DELAY 4
#define ROUTE_B_TRAINER_MOVE_SPEED 2
#define ROUTE_B_POKEBALL_TILE_X 8
#define ROUTE_B_POKEBALL_TILE_Y 12
#define ROUTE_B_POKEBALL_DRAW_X ((ROUTE_B_POKEBALL_TILE_X * TILE_SIZE) + ((TILE_SIZE - SMALL_POKEBALL_SPRITE_WIDTH) / 2))
#define ROUTE_B_POKEBALL_DRAW_Y ((ROUTE_B_POKEBALL_TILE_Y * TILE_SIZE) + ((TILE_SIZE - SMALL_POKEBALL_SPRITE_HEIGHT) / 2))
#define GYM_CYNTHIA_DRAW_X ((MAP_WIDTH * TILE_SIZE - CYNTHIA_FRONT_MAP_WIDTH) / 2)
#define GYM_CYNTHIA_DRAW_Y ((MAP_HEIGHT * TILE_SIZE - CYNTHIA_FRONT_MAP_HEIGHT) / 2)
#define GYM_CYNTHIA_BBOX_INSET_X 4
#define GYM_CYNTHIA_BBOX_INSET_TOP 16
#define GYM_CYNTHIA_BBOX_INSET_BOTTOM 2
#define ROUTE_A_POKEMON_CENTER_TILE_X 8
#define ROUTE_A_POKEMON_CENTER_TILE_Y 1
#define ROUTE_A_POKE_MART_TILE_X 12
#define ROUTE_A_POKE_MART_TILE_Y 1
#define ROUTE_A_HOUSE_TILE_X 12
#define ROUTE_A_HOUSE_TILE_Y 8
#define ROUTE_B_GYM_TILE_X 7
#define ROUTE_B_GYM_TILE_Y 1
#define POKEMON_CENTER_NPC1_X 48
#define POKEMON_CENTER_NPC1_Y 88

static bool map_place_cliff_xy(int x, int y, int length);
static bool map_place_cliff_vertical_xy(int x, int y, int length);
static bool is_cliff_tile(TileId tile);
static bool is_vertical_cliff_tile(TileId tile);
static bool rects_overlap(int ax0, int ay0, int ax1, int ay1,
                          int bx0, int by0, int bx1, int by1);
static void get_gym_cynthia_bbox(int *x0, int *y0, int *x1, int *y1);
static void get_pokemon_center_npc1_bbox(int *x0, int *y0, int *x1, int *y1);
static void get_route_b_trainer_bbox_at(int draw_x, int draw_y,
                                        int *x0, int *y0, int *x1, int *y1);
static void set_npc1_facing_toward_bounds(const McBounds *bounds);

void map_set_nurse_facing_left(bool left) {
    g_nurse_facing_left = left;
}

static void get_route_b_trainer_bbox_at(int draw_x, int draw_y,
                                        int *x0, int *y0, int *x1, int *y1) {
    if (x0 != NULL) {
        *x0 = draw_x + ROUTE_B_TRAINER_BBOX_INSET_X;
    }
    if (y0 != NULL) {
        *y0 = draw_y + ROUTE_B_TRAINER_BBOX_INSET_Y;
    }
    if (x1 != NULL) {
        *x1 = draw_x + ROUTE_B_TRAINER_BBOX_INSET_X + ROUTE_B_TRAINER_BBOX_W - 1;
    }
    if (y1 != NULL) {
        *y1 = draw_y + ROUTE_B_TRAINER_BBOX_INSET_Y + ROUTE_B_TRAINER_BBOX_H - 1;
    }
}

static void get_route_b_trainer_bbox(int *x0, int *y0, int *x1, int *y1) {
    get_route_b_trainer_bbox_at(g_route_b_trainer_x, g_route_b_trainer_y, x0, y0, x1, y1);
}

static void get_gym_cynthia_bbox(int *x0, int *y0, int *x1, int *y1) {
    if (x0 != NULL) {
        *x0 = GYM_CYNTHIA_DRAW_X + GYM_CYNTHIA_BBOX_INSET_X;
    }
    if (y0 != NULL) {
        *y0 = GYM_CYNTHIA_DRAW_Y + GYM_CYNTHIA_BBOX_INSET_TOP;
    }
    if (x1 != NULL) {
        *x1 = GYM_CYNTHIA_DRAW_X + CYNTHIA_FRONT_MAP_WIDTH - 1 - GYM_CYNTHIA_BBOX_INSET_X;
    }
    if (y1 != NULL) {
        *y1 = GYM_CYNTHIA_DRAW_Y + CYNTHIA_FRONT_MAP_HEIGHT - 1 - GYM_CYNTHIA_BBOX_INSET_BOTTOM;
    }
}

static void get_pokemon_center_npc1_bbox(int *x0, int *y0, int *x1, int *y1) {
    if (x0 != NULL) {
        *x0 = POKEMON_CENTER_NPC1_X;
    }
    if (y0 != NULL) {
        *y0 = POKEMON_CENTER_NPC1_Y;
    }
    if (x1 != NULL) {
        *x1 = POKEMON_CENTER_NPC1_X + NPC1_FRONT_MAP_WIDTH - 1;
    }
    if (y1 != NULL) {
        *y1 = POKEMON_CENTER_NPC1_Y + NPC1_FRONT_MAP_HEIGHT - 1;
    }
}

static void set_npc1_facing_toward_bounds(const McBounds *bounds) {
    const int npc_center_x = POKEMON_CENTER_NPC1_X + (NPC1_FRONT_MAP_WIDTH / 2);
    const int npc_center_y = POKEMON_CENTER_NPC1_Y + (NPC1_FRONT_MAP_HEIGHT / 2);
    int mc_center_x;
    int mc_center_y;
    int dx;
    int dy;
    int abs_dx;
    int abs_dy;

    if (bounds == NULL || !bounds->valid) {
        return;
    }

    mc_center_x = (bounds->x0 + bounds->x1) / 2;
    mc_center_y = (bounds->y0 + bounds->y1) / 2;
    dx = mc_center_x - npc_center_x;
    dy = mc_center_y - npc_center_y;
    abs_dx = dx < 0 ? -dx : dx;
    abs_dy = dy < 0 ? -dy : dy;

    if (abs_dx >= abs_dy) {
        g_npc1_facing = (dx < 0) ? NPC1_FACING_LEFT : NPC1_FACING_RIGHT;
    } else {
        g_npc1_facing = (dy < 0) ? NPC1_FACING_BACK : NPC1_FACING_FRONT;
    }
}

void map_reset_route_b_trainer(void) {
    g_route_b_trainer_state = ROUTE_B_TRAINER_IDLE;
    g_route_b_trainer_x = ROUTE_B_TRAINER_START_X;
    g_route_b_trainer_y = ROUTE_B_TRAINER_START_Y;
    g_route_b_trainer_frame = 0;
    g_route_b_trainer_frame_timer = 0;
    g_route_b_trainer_arrival_pending = false;
}

bool map_consume_route_b_trainer_arrival(void) {
    const bool pending = g_route_b_trainer_arrival_pending;
    g_route_b_trainer_arrival_pending = false;
    return pending;
}

void map_set_route_b_trainer_defeated(bool defeated) {
    g_route_b_trainer_defeated = defeated;
    if (defeated) {
        g_route_b_trainer_arrival_pending = false;
    }
}

bool map_is_route_b_trainer_defeated(void) {
    return g_route_b_trainer_defeated;
}

bool map_tick_route_b_trainer_event(const McBounds *bounds) {
    int trainer_x0;
    int trainer_y0;
    int trainer_x1;
    int trainer_y1;

    if (g_current_preset != MAP_PRESET_GROUND || g_current_decor_layout != MAP_DECOR_ROUTE_B) {
        return false;
    }
    if (g_route_b_trainer_defeated) {
        return false;
    }
    if (bounds == NULL || !bounds->valid) {
        return g_route_b_trainer_state == ROUTE_B_TRAINER_WALKING;
    }

    get_route_b_trainer_bbox(&trainer_x0, &trainer_y0, &trainer_x1, &trainer_y1);

    if (g_route_b_trainer_state == ROUTE_B_TRAINER_IDLE) {
        const bool same_y_band = !(bounds->y1 < trainer_y0 || trainer_y1 < bounds->y0);
        const bool mc_is_to_the_left = bounds->x1 < trainer_x0 - 8;
        if (same_y_band && mc_is_to_the_left) {
            g_route_b_trainer_state = ROUTE_B_TRAINER_WALKING;
            g_route_b_trainer_frame = 0;
            g_route_b_trainer_frame_timer = 0;
        }
    }

    if (g_route_b_trainer_state != ROUTE_B_TRAINER_WALKING) {
        return false;
    }

    g_route_b_trainer_frame_timer++;
    if (g_route_b_trainer_frame_timer >= ROUTE_B_TRAINER_FRAME_DELAY) {
        g_route_b_trainer_frame_timer = 0;
        g_route_b_trainer_frame = (g_route_b_trainer_frame + 1) % TRAINER_WALKING_FRAME_COUNT;
    }

    {
        int next_trainer_x0;
        int next_trainer_y0;
        int next_trainer_x1;
        int next_trainer_y1;

        get_route_b_trainer_bbox_at(g_route_b_trainer_x - ROUTE_B_TRAINER_MOVE_SPEED, g_route_b_trainer_y,
                                    &next_trainer_x0, &next_trainer_y0,
                                    &next_trainer_x1, &next_trainer_y1);

        if (next_trainer_x0 <= bounds->x1 + ROUTE_B_TRAINER_STOP_GAP &&
            rects_overlap(next_trainer_x0, next_trainer_y0, next_trainer_x1, next_trainer_y1,
                          bounds->x0, bounds->y0, bounds->x1, bounds->y1)) {
            g_route_b_trainer_state = ROUTE_B_TRAINER_ARRIVED;
            g_route_b_trainer_frame = 0;
            g_route_b_trainer_frame_timer = 0;
            g_route_b_trainer_arrival_pending = true;
            return true;
        }

        g_route_b_trainer_x -= ROUTE_B_TRAINER_MOVE_SPEED;
    }

    return true;
}

bool map_try_collect_route_b_pokeball(const McBounds *bounds) {
    const int foot_tile_x = bounds != NULL && bounds->valid ? (((bounds->x0 + bounds->x1) / 2) / TILE_SIZE) : -1;
    const int foot_tile_y = bounds != NULL && bounds->valid ? (bounds->y1 / TILE_SIZE) : -1;

    if (g_route_b_pokeball_collected) {
        return false;
    }
    if (g_current_preset != MAP_PRESET_GROUND || g_current_decor_layout != MAP_DECOR_ROUTE_B) {
        return false;
    }
    if (foot_tile_x == ROUTE_B_POKEBALL_TILE_X && foot_tile_y == ROUTE_B_POKEBALL_TILE_Y) {
        g_route_b_pokeball_collected = true;
        return true;
    }
    return false;
}

bool map_get_route_a_house_position(MapTilePosition *out_position) {
    if (out_position == NULL) {
        return false;
    }

    out_position->x = ROUTE_A_HOUSE_TILE_X;
    out_position->y = ROUTE_A_HOUSE_TILE_Y;
    return true;
}

bool map_get_route_a_pokemon_center_position(MapTilePosition *out_position) {
    if (out_position == NULL) {
        return false;
    }

    out_position->x = ROUTE_A_POKEMON_CENTER_TILE_X;
    out_position->y = ROUTE_A_POKEMON_CENTER_TILE_Y;
    return true;
}

bool map_get_route_a_poke_mart_position(MapTilePosition *out_position) {
    if (out_position == NULL) {
        return false;
    }

    out_position->x = ROUTE_A_POKE_MART_TILE_X;
    out_position->y = ROUTE_A_POKE_MART_TILE_Y;
    return true;
}

bool map_get_route_b_gym_position(MapTilePosition *out_position) {
    if (out_position == NULL) {
        return false;
    }

    out_position->x = ROUTE_B_GYM_TILE_X;
    out_position->y = ROUTE_B_GYM_TILE_Y;
    return true;
}

#define POKEMON_CENTER_NURSE_X 150
#define POKEMON_CENTER_NURSE_Y 58
#define POKEMON_CENTER_NURSE_BBOX_X POKEMON_CENTER_NURSE_X
#define POKEMON_CENTER_NURSE_BBOX_Y POKEMON_CENTER_NURSE_Y
#define POKEMON_CENTER_NURSE_BBOX_W POKEMON_CENTER_NURSE_WIDTH
#define POKEMON_CENTER_NURSE_BBOX_H POKEMON_CENTER_NURSE_HEIGHT
#define POKEMON_CENTER_NPC1_BBOX_X POKEMON_CENTER_NPC1_X
#define POKEMON_CENTER_NPC1_BBOX_Y POKEMON_CENTER_NPC1_Y
#define POKEMON_CENTER_NPC1_BBOX_W NPC1_FRONT_MAP_WIDTH
#define POKEMON_CENTER_NPC1_BBOX_H NPC1_FRONT_MAP_HEIGHT
#define POKE_MART_CLERK_X 44
#define POKE_MART_CLERK_Y 87
#define POKE_MART_CLERK_BBOX_X POKE_MART_CLERK_X
#define POKE_MART_CLERK_BBOX_Y POKE_MART_CLERK_Y
#define POKE_MART_CLERK_BBOX_W POKEMON_CENTER_CLERK_WIDTH
#define POKE_MART_CLERK_BBOX_H POKEMON_CENTER_CLERK_HEIGHT
#define POKEMON_CENTER_DESK_BBOX_X 95
#define POKEMON_CENTER_DESK_BBOX_Y 47
#define POKEMON_CENTER_DESK_BBOX_W POKEMON_CENTER_DESK_WIDTH
#define POKEMON_CENTER_DESK_BBOX_H POKEMON_CENTER_DESK_HEIGHT
#define POKEMON_CENTER_PC_BBOX_X 230
#define POKEMON_CENTER_PC_BBOX_Y 70
#define POKEMON_CENTER_PC_BBOX_W 30
#define POKEMON_CENTER_PC_BBOX_H 10

static bool is_within_interior_walkable_rect(int x0, int y0, int x1, int y1) {
    const int min_x = 10;
    const int max_x = 310;
    const int min_y = 80;
    const int max_y = 220;
    const int doorway_min_x = 129;
    const int doorway_max_x = 191;
    const int doorway_max_y = (MAP_HEIGHT * TILE_SIZE) - 1;

    if (x0 < min_x || x1 > max_x || y0 < min_y) {
        return false;
    }
    if (y1 <= max_y) {
        if (!(x1 < POKEMON_CENTER_DESK_BBOX_X ||
              (POKEMON_CENTER_DESK_BBOX_X + POKEMON_CENTER_DESK_BBOX_W - 1) < x0 ||
              y1 < POKEMON_CENTER_DESK_BBOX_Y ||
              (POKEMON_CENTER_DESK_BBOX_Y + POKEMON_CENTER_DESK_BBOX_H - 1) < y0)) {
            return false;
        }
        return true;
    }

    return x0 >= doorway_min_x && x1 <= doorway_max_x && y1 <= doorway_max_y;
}

static bool is_within_poke_mart_walkable_area(int x0, int y0, int x1, int y1) {
    const int min_x = 12;
    const int max_x = 280;
    const int min_y = 80;
    const int max_y = 230;

    if (x0 < min_x || x1 > max_x || y0 < min_y || y1 > max_y) {
        return false;
    }

    if (!(x1 < 0 || 80 < x0 || y1 < 0 || 150 < y0)) {
        return false;
    }

    if (!(x1 < 172 || (172 + 54 - 1) < x0 || y1 < 120 || (120 + 80 - 1) < y0)) {
        return false;
    }

    return true;
}

static bool is_within_house_walkable_area(int x0, int y0, int x1, int y1) {
    static const bool house_walkable_mask[MAP_HEIGHT][MAP_WIDTH] = {
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,  false, false, false, false, false },
        { false, false, false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false },
        { false, false,  true,  true,  true,  true, true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false },
        { false, false,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true, false,  false },
        { false, false,  true,  true,  true,  true,  true,  true,  true,  true,  true, true, true,  true,  true,  true,  true,  true,   false,  false },
        { false, false,  true,  true,  true,  false, false, false, false, false, true, true, true,  true,  true,  true,  true,  true,  false,  false },
        { false, false,  true,  true,  true,  false, false, false, false, false, true, true, true,  true,  true,  true,  true,  true,  false,  false },
        { false, false,  true,  true,  true,  false, false, false, false, false, true, true, true,  true,  true,  true,  true,  true,  false,  false },
        { false, false,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false,  false },
        { false, false,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false },
        { false, false,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false },
        { false, false,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false },
        { false, false,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false },
    };
    const int left_tile = x0 / TILE_SIZE;
    const int right_tile = x1 / TILE_SIZE;
    const int top_tile = y0 / TILE_SIZE;
    const int bottom_tile = y1 / TILE_SIZE;

    if (x0 < 0 || y0 < 0 || x1 >= MAP_WIDTH * TILE_SIZE || y1 >= MAP_HEIGHT * TILE_SIZE) {
        return false;
    }

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            if (!house_walkable_mask[y][x]) {
                return false;
            }
        }
    }

    return true;
}

static bool is_within_gym_walkable_area(int x0, int y0, int x1, int y1) {
    static const bool gym_walkable_mask[MAP_HEIGHT][MAP_WIDTH] = {
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false, false },
        { false, false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false },
        { false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false },
        { false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false },
        { false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false },
        { false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false },
        { false, false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false },
        { false, false, false, false, false, false, false, true,  true,  true,  true,  true,  true,  false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, true,  true,  true,  true,  true,  true,  false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, true,  true,  true,  true,  true,  true,  false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, true,  true,  true,  true,  true,  true,  false, false, false, false, false, false, false },
    };
    const int left_tile = x0 / TILE_SIZE;
    const int right_tile = x1 / TILE_SIZE;
    const int top_tile = y0 / TILE_SIZE;
    const int bottom_tile = y1 / TILE_SIZE;

    if (x0 < 0 || y0 < 0 || x1 >= MAP_WIDTH * TILE_SIZE || y1 >= MAP_HEIGHT * TILE_SIZE) {
        return false;
    }

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            if (!gym_walkable_mask[y][x]) {
                return false;
            }
        }
    }

    return true;
}

static bool is_decor_transparent(unsigned short colour) {
    const int r5 = (colour >> 11) & 31;
    const int g6 = (colour >> 5) & 63;
    const int b5 = colour & 31;

    if (colour == (unsigned short)TRANSPARENT_COLOUR) {
        return true;
    }

    return r5 >= 27 && b5 >= 27 && g6 <= 20;
}

typedef struct {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    bool valid;
} TileBounds;

static TileBounds compute_tile_bounds(const short *tile) {
    TileBounds bounds;
    bounds.min_x = TILE_SIZE;
    bounds.min_y = TILE_SIZE;
    bounds.max_x = -1;
    bounds.max_y = -1;
    bounds.valid = false;

    for (int y = 0; y < TILE_SIZE; y++) {
        for (int x = 0; x < TILE_SIZE; x++) {
            const unsigned short colour = tile[y * TILE_SIZE + x];
            if (is_decor_transparent(colour)) {
                continue;
            }

            if (x < bounds.min_x) bounds.min_x = x;
            if (y < bounds.min_y) bounds.min_y = y;
            if (x > bounds.max_x) bounds.max_x = x;
            if (y > bounds.max_y) bounds.max_y = y;
            bounds.valid = true;
        }
    }

    return bounds;
}

static bool rects_overlap(int ax0, int ay0, int ax1, int ay1,
                          int bx0, int by0, int bx1, int by1) {
    return !(ax1 < bx0 || bx1 < ax0 || ay1 < by0 || by1 < ay0);
}

static bool tile_collision_rect(TileId tile, int tile_x, int tile_y,
                                int *x0, int *y0, int *x1, int *y1) {
    const int px = tile_x * TILE_SIZE;
    const int py = tile_y * TILE_SIZE;

    if (!map_is_walkable_tile(tile)) {
        if (is_cliff_tile(tile)) {
            *x0 = px;
            *y0 = py;
            *x1 = px + TILE_SIZE - 1;
            *y1 = py + TILE_SIZE - 1;
            return true;
        }

        TileBounds bounds = compute_tile_bounds(tiles[tile]);
        if (!bounds.valid) {
            return false;
        }

        *x0 = px + bounds.min_x;
        *y0 = py + bounds.min_y;
        *x1 = px + bounds.max_x;
        *y1 = py + bounds.max_y;
        return true;
    }

    return false;
}

static bool is_cliff_tile(TileId tile) {
    return tile == TILE_CLIFF_LEFT ||
           tile == TILE_CLIFF_MIDDLE ||
           tile == TILE_CLIFF_RIGHT ||
           tile == TILE_CLIFF_TOP ||
           tile == TILE_CLIFF_MIDDLE_VERTICAL ||
           tile == TILE_CLIFF_BOTTOM;
}

static bool is_vertical_cliff_tile(TileId tile) {
    return tile == TILE_CLIFF_TOP ||
           tile == TILE_CLIFF_MIDDLE_VERTICAL ||
           tile == TILE_CLIFF_BOTTOM;
}

static void clear_decor_entries(void) {
    g_decor_entry_count = 0;
}

static void add_decor_entry(TileId tile, int x, int y) {
    if (g_decor_entry_count >= (int)(sizeof(g_decor_entries) / sizeof(g_decor_entries[0]))) {
        return;
    }
    g_decor_entries[g_decor_entry_count].tile = tile;
    g_decor_entries[g_decor_entry_count].x = x;
    g_decor_entries[g_decor_entry_count].y = y;
    g_decor_entry_count++;
}

static bool map_place_overlay_rect(int x, int y,
                                   int width, int height,
                                   const TileId *tiles_to_place) {
    if (x < 0 || y < 0 || x + width > MAP_WIDTH || y + height > MAP_HEIGHT) {
        return false;
    }

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            const TileId tile = tiles_to_place[row * width + col];
            map_overlay[y + row][x + col] = tile;
            add_decor_entry(tile, x + col, y + row);
        }
    }

    return true;
}

static void draw_overlay_tile(TileId tile, int x, int y) {
    const short *overlay_tile = tiles[tile];
    const int screen_x = x * TILE_SIZE;
    const int screen_y = y * TILE_SIZE;

    for (int tile_y = 0; tile_y < TILE_SIZE; tile_y++) {
        for (int tile_x = 0; tile_x < TILE_SIZE; tile_x++) {
            const unsigned short colour = overlay_tile[tile_y * TILE_SIZE + tile_x];
            if (is_decor_transparent(colour)) {
                continue;
            }
            draw_pixel(screen_x + tile_x, screen_y + tile_y, colour);
        }
    }
}

static void draw_all_decor(void) {
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int i = 0; i < g_decor_entry_count; i++) {
            if (g_decor_entries[i].y != row) {
                continue;
            }
            draw_overlay_tile(g_decor_entries[i].tile, g_decor_entries[i].x, g_decor_entries[i].y);
        }
    }
}

void apply_map_decor_layout(MapDecorLayout layout) {
    static const MapTilePosition route_a_tree_positions[] = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
        {0, 10}, {0, 11}, {0, 12}, {0, 13},
    };
    static const MapTilePosition right_tree_positions[] = {
        {19, 0}, {19, 1}, {19, 2}, {19, 3}, {19, 4}, {19, 5}, {19, 6},
        {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12}, {19, 13}
    };
    static const MapTilePosition route_a_pokemon_centers[] = {
        {ROUTE_A_POKEMON_CENTER_TILE_X, ROUTE_A_POKEMON_CENTER_TILE_Y},
    };
    static const MapTilePosition route_a_poke_marts[] = {
        {ROUTE_A_POKE_MART_TILE_X, ROUTE_A_POKE_MART_TILE_Y},
    };
    static const MapTilePosition route_a_houses[] = {
        {ROUTE_A_HOUSE_TILE_X, ROUTE_A_HOUSE_TILE_Y},
    };

    static const MapTilePosition route_b_grass_patch_positions[] = {
        {4,8}, {5,8}, {6,8}, {7,8},
        {4,9}, {5,9}, {6,9}, {7,9},
        {4,10}, {5,10}, {6,10}, {7,10},
        {4,11}, {5,11}, {6,11}, {7,11},
        {4,12}, {5,12}, {6,12}, {7,12},
        {12,8}, {13,8}, {14,8}, {15,8},
        {12,9}, {13,9}, {14,9}, {15,9},
        {12,10}, {13,10}, {14,10}, {15,10},
        {12,11}, {13,11}, {14,11}, {15,11},
        {12,12}, {13,12}, {14,12}, {15,12},
    };
    static const MapTilePosition route_b_tree_positions[] = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6},
        {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13},
        {10, 7}, {10,8}, {10, 9},{10,10}, {10, 11},
        {19, 0}, {19, 1}, {19, 2}, {19, 3}, {19, 4},
        {19, 10}, {19, 11}, {19, 12}, {19, 13},
    };
    static const MapTilePosition route_b_gyms[] = {
        {ROUTE_B_GYM_TILE_X, ROUTE_B_GYM_TILE_Y},
    };
    static const CliffPlacement route_b_cliffs[] = {
        {4, 5, 15},
    };
    static const VerticalCliffPlacement route_b_vertical_cliffs[] = {
        {10, 13, 2},
    };
    static const MapDecorDefinition route_a_decor = {
        0,
        0,
        route_a_tree_positions,
        sizeof(route_a_tree_positions) / sizeof(route_a_tree_positions[0]),
    };
    static const MapDecorDefinition route_b_decor = {
        route_b_grass_patch_positions,
        sizeof(route_b_grass_patch_positions) / sizeof(route_b_grass_patch_positions[0]),
        route_b_tree_positions,
        sizeof(route_b_tree_positions) / sizeof(route_b_tree_positions[0]),
    };
    const MapDecorDefinition *decor = &route_a_decor;
    g_current_decor_layout = layout;

    switch (layout) {
        case MAP_DECOR_NONE:
            return;
        case MAP_DECOR_ROUTE_B:
            decor = &route_b_decor;
            break;
        case MAP_DECOR_ROUTE_A:
        default:
            break;
    }

    for (unsigned int i = 0; i < decor->grass_patch_count; i++) {
        map_set_overlay_tile_xy(decor->grass_patch_positions[i].x, decor->grass_patch_positions[i].y, TILE_GRASS_PATCH);
    }
    for (unsigned int i = 0; i < decor->tree_count; i++) {
        map_place_tree_xy(decor->tree_positions[i].x, decor->tree_positions[i].y);
    }
    if (layout == MAP_DECOR_ROUTE_A) {
        for (unsigned int i = 0; i < sizeof(route_a_pokemon_centers) / sizeof(route_a_pokemon_centers[0]); i++) {
            map_place_pokemon_center_xy(route_a_pokemon_centers[i].x, route_a_pokemon_centers[i].y);
        }
        for (unsigned int i = 0; i < sizeof(route_a_poke_marts) / sizeof(route_a_poke_marts[0]); i++) {
            map_place_poke_mart_xy(route_a_poke_marts[i].x, route_a_poke_marts[i].y);
        }
        for (unsigned int i = 0; i < sizeof(route_a_houses) / sizeof(route_a_houses[0]); i++) {
            map_place_house_xy(route_a_houses[i].x, route_a_houses[i].y);
        }
    } else if (layout == MAP_DECOR_ROUTE_B) {
        for (unsigned int i = 0; i < sizeof(route_b_gyms) / sizeof(route_b_gyms[0]); i++) {
            map_place_gym_xy(route_b_gyms[i].x, route_b_gyms[i].y);
        }
        for (unsigned int i = 0; i < sizeof(route_b_cliffs) / sizeof(route_b_cliffs[0]); i++) {
            map_place_cliff_xy(route_b_cliffs[i].x, route_b_cliffs[i].y, route_b_cliffs[i].length);
        }
        for (unsigned int i = 0; i < sizeof(route_b_vertical_cliffs) / sizeof(route_b_vertical_cliffs[0]); i++) {
            map_place_cliff_vertical_xy(route_b_vertical_cliffs[i].x,
                                        route_b_vertical_cliffs[i].y,
                                        route_b_vertical_cliffs[i].length);
        }
    }
}

void apply_map_decor(void) {
    apply_map_decor_layout(MAP_DECOR_ROUTE_A);
}

// Predefined layouts.
static const TileId preset_route[MAP_HEIGHT][MAP_WIDTH] = {
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
    { TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS, TILE_GRASS },
};

// All-black preset using TILE_BLACK
static const TileId preset_black[MAP_HEIGHT][MAP_WIDTH] = {
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
    { TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK, TILE_BLACK },
};

static const TileId *const preset_ptrs[] = {
    (const TileId *)preset_route,
    (const TileId *)preset_black,
    (const TileId *)preset_backdrop1,
    (const TileId *)preset_ground,
    (const TileId *)preset_pokemon_center_interior,
    (const TileId *)preset_poke_mart_interior,
    (const TileId *)preset_house_interior,
    (const TileId *)preset_gym_interior,
};

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
_Static_assert((sizeof(preset_ptrs) / sizeof(preset_ptrs[0])) == MAP_PRESET_COUNT,
               "preset_ptrs out of sync with MapPresetId");
#endif

void load_map_preset(MapPresetId preset) {
    if (preset >= MAP_PRESET_COUNT) return;
    g_current_preset = preset;
    g_npc1_facing = NPC1_FACING_FRONT;
    map_reset_route_b_trainer();
    const TileId *preset_layout = preset_ptrs[preset];
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            map[row][col] = preset_layout[row * MAP_WIDTH + col];
            map_overlay[row][col] = -1;
        }
    }
    clear_decor_entries();

    if (preset == MAP_PRESET_ROUTE) {
        apply_map_decor();
    }
}

bool map_set_tile_xy(int x, int y, TileId tile) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return false;
    }
    map[y][x] = tile;
    return true;
}

bool map_place_tree_xy(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y + 1 >= MAP_HEIGHT) {
        return false;
    }

    map_overlay[y][x] = TILE_TREE_TOP;
    map_overlay[y + 1][x] = TILE_TREE_BOTTOM;
    add_decor_entry(TILE_TREE_TOP, x, y);
    add_decor_entry(TILE_TREE_BOTTOM, x, y + 1);
    return true;
}

bool map_place_pokemon_center_xy(int x, int y) {
    static const TileId pokemon_center_tiles[3 * 3] = {
        TILE_POKEMON_CENTER_TOP_LEFT, TILE_POKEMON_CENTER_TOP_MIDDLE, TILE_POKEMON_CENTER_TOP_RIGHT,
        TILE_POKEMON_CENTER_MIDDLE_LEFT, TILE_POKEMON_CENTER_MIDDLE_MIDDLE, TILE_POKEMON_CENTER_MIDDLE_RIGHT,
        TILE_POKEMON_CENTER_BOTTOM_LEFT, TILE_POKEMON_CENTER_BOTTOM_MIDDLE, TILE_POKEMON_CENTER_BOTTOM_RIGHT,
    };
    return map_place_overlay_rect(x, y, 3, 3, pokemon_center_tiles);
}

bool map_place_poke_mart_xy(int x, int y) {
    static const TileId poke_mart_tiles[3 * 3] = {
        TILE_POKE_MART_TOP_LEFT, TILE_POKE_MART_TOP_MIDDLE, TILE_POKE_MART_TOP_RIGHT,
        TILE_POKE_MART_MIDDLE_LEFT, TILE_POKE_MART_MIDDLE_MIDDLE, TILE_POKE_MART_MIDDLE_RIGHT,
        TILE_POKE_MART_BOTTOM_LEFT, TILE_POKE_MART_BOTTOM_MIDDLE, TILE_POKE_MART_BOTTOM_RIGHT,
    };
    return map_place_overlay_rect(x, y, 3, 3, poke_mart_tiles);
}

bool map_place_house_xy(int x, int y) {
    static const TileId house_tiles[3 * 3] = {
        TILE_HOUSE_TOP_LEFT, TILE_HOUSE_TOP_MIDDLE, TILE_HOUSE_TOP_RIGHT,
        TILE_HOUSE_MIDDLE_LEFT, TILE_HOUSE_MIDDLE_MIDDLE, TILE_HOUSE_MIDDLE_RIGHT,
        TILE_HOUSE_BOTTOM_LEFT, TILE_HOUSE_BOTTOM_MIDDLE, TILE_HOUSE_BOTTOM_RIGHT,
    };
    return map_place_overlay_rect(x, y, 3, 3, house_tiles);
}

bool map_place_gym_xy(int x, int y) {
    static const TileId gym_tiles[3 * 3] = {
        TILE_GYM_TOP_LEFT, TILE_GYM_TOP_MIDDLE, TILE_GYM_TOP_RIGHT,
        TILE_GYM_MIDDLE_LEFT, TILE_GYM_MIDDLE_MIDDLE, TILE_GYM_MIDDLE_RIGHT,
        TILE_GYM_BOTTOM_LEFT, TILE_GYM_BOTTOM_MIDDLE, TILE_GYM_BOTTOM_RIGHT,
    };
    return map_place_overlay_rect(x, y, 3, 3, gym_tiles);
}

static bool map_place_cliff_xy(int x, int y, int length) {
    if (length < 2 || x < 0 || y < 0 || x + length > MAP_WIDTH || y >= MAP_HEIGHT) {
        return false;
    }

    for (int i = 0; i < length; i++) {
        TileId tile = TILE_CLIFF_MIDDLE;
        if (i == 0) {
            tile = TILE_CLIFF_LEFT;
        } else if (i == length - 1) {
            tile = TILE_CLIFF_RIGHT;
        }
        map_overlay[y][x + i] = tile;
        add_decor_entry(tile, x + i, y);
    }

    return true;
}

static bool map_place_cliff_vertical_xy(int x, int y, int length) {
    if (length < 2 || x < 0 || y < 0 || x >= MAP_WIDTH || y + length > MAP_HEIGHT) {
        return false;
    }

    for (int i = 0; i < length; i++) {
        TileId tile = TILE_CLIFF_MIDDLE_VERTICAL;
        if (i == 0) {
            tile = TILE_CLIFF_TOP;
        } else if (i == length - 1) {
            tile = TILE_CLIFF_BOTTOM;
        }
        map_overlay[y + i][x] = tile;
        add_decor_entry(tile, x, y + i);
    }

    return true;
}

bool map_set_overlay_tile_xy(int x, int y, TileId tile) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return false;
    }

    map_overlay[y][x] = tile;
    add_decor_entry(tile, x, y);
    return true;
}

bool map_is_walkable_tile(TileId tile) {
    return tile != TILE_TREE_TOP &&
           tile != TILE_TREE_BOTTOM &&
           tile != TILE_POKEMON_CENTER_TOP_LEFT &&
           tile != TILE_POKEMON_CENTER_TOP_MIDDLE &&
           tile != TILE_POKEMON_CENTER_TOP_RIGHT &&
           tile != TILE_POKEMON_CENTER_MIDDLE_LEFT &&
           tile != TILE_POKEMON_CENTER_MIDDLE_MIDDLE &&
           tile != TILE_POKEMON_CENTER_MIDDLE_RIGHT &&
           tile != TILE_POKEMON_CENTER_BOTTOM_LEFT &&
           tile != TILE_POKEMON_CENTER_BOTTOM_RIGHT &&
           tile != TILE_POKE_MART_TOP_LEFT &&
           tile != TILE_POKE_MART_TOP_MIDDLE &&
           tile != TILE_POKE_MART_TOP_RIGHT &&
           tile != TILE_POKE_MART_MIDDLE_LEFT &&
           tile != TILE_POKE_MART_MIDDLE_MIDDLE &&
           tile != TILE_POKE_MART_MIDDLE_RIGHT &&
           tile != TILE_POKE_MART_BOTTOM_LEFT &&
           tile != TILE_POKE_MART_BOTTOM_RIGHT &&
           tile != TILE_HOUSE_TOP_LEFT &&
           tile != TILE_HOUSE_TOP_MIDDLE &&
           tile != TILE_HOUSE_TOP_RIGHT &&
           tile != TILE_HOUSE_MIDDLE_LEFT &&
           tile != TILE_HOUSE_MIDDLE_MIDDLE &&
           tile != TILE_HOUSE_MIDDLE_RIGHT &&
           tile != TILE_HOUSE_BOTTOM_LEFT &&
           tile != TILE_HOUSE_BOTTOM_RIGHT &&
           tile != TILE_GYM_TOP_LEFT &&
           tile != TILE_GYM_TOP_MIDDLE &&
           tile != TILE_GYM_TOP_RIGHT &&
           tile != TILE_GYM_MIDDLE_LEFT &&
           tile != TILE_GYM_MIDDLE_MIDDLE &&
           tile != TILE_GYM_MIDDLE_RIGHT &&
           tile != TILE_GYM_BOTTOM_LEFT &&
           tile != TILE_GYM_BOTTOM_RIGHT &&
           tile != TILE_CLIFF_LEFT &&
           tile != TILE_CLIFF_MIDDLE &&
           tile != TILE_CLIFF_RIGHT;
}

bool map_is_walkable_at_xy(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return false;
    }
    if (map_overlay[y][x] >= 0 && !map_is_walkable_tile((TileId)map_overlay[y][x])) {
        return false;
    }
    return map_is_walkable_tile(map[y][x]);
}

bool map_bounds_are_walkable(int x0, int y0, int x1, int y1, int dx, int dy) {
    if (x0 < 0 || y0 < 0 || x1 >= MAP_WIDTH * TILE_SIZE || y1 >= MAP_HEIGHT * TILE_SIZE) {
        return false;
    }
    if (g_current_preset == MAP_PRESET_POKEMON_CENTER_INTERIOR &&
        !is_within_interior_walkable_rect(x0, y0, x1, y1)) {
        return false;
    }
    if (g_current_preset == MAP_PRESET_POKE_MART_INTERIOR &&
        !is_within_poke_mart_walkable_area(x0, y0, x1, y1)) {
        return false;
    }
    if (g_current_preset == MAP_PRESET_HOUSE_INTERIOR &&
        !is_within_house_walkable_area(x0, y0, x1, y1)) {
        return false;
    }
    if (g_current_preset == MAP_PRESET_GYM_INTERIOR &&
        !is_within_gym_walkable_area(x0, y0, x1, y1)) {
        return false;
    }
    if (g_current_preset == MAP_PRESET_POKEMON_CENTER_INTERIOR) {
        int npc1_x0;
        int npc1_y0;
        int npc1_x1;
        int npc1_y1;

        get_pokemon_center_npc1_bbox(&npc1_x0, &npc1_y0, &npc1_x1, &npc1_y1);
        if (rects_overlap(x0, y0, x1, y1, npc1_x0, npc1_y0, npc1_x1, npc1_y1)) {
            return false;
        }
    }
    if (g_current_preset == MAP_PRESET_GYM_INTERIOR) {
        int cynthia_x0;
        int cynthia_y0;
        int cynthia_x1;
        int cynthia_y1;

        get_gym_cynthia_bbox(&cynthia_x0, &cynthia_y0, &cynthia_x1, &cynthia_y1);
        if (rects_overlap(x0, y0, x1, y1, cynthia_x0, cynthia_y0, cynthia_x1, cynthia_y1)) {
            return false;
        }
    }
    if (g_current_preset == MAP_PRESET_GROUND &&
        g_current_decor_layout == MAP_DECOR_ROUTE_B) {
        if (!g_route_b_trainer_defeated) {
            int trainer_x0;
            int trainer_y0;
            int trainer_x1;
            int trainer_y1;

            get_route_b_trainer_bbox(&trainer_x0, &trainer_y0, &trainer_x1, &trainer_y1);
            if (!(x1 < trainer_x0 ||
                  trainer_x1 < x0 ||
                  y1 < trainer_y0 ||
                  trainer_y1 < y0)) {
                return false;
            }
        }
    }

    const int left_tile = x0 / TILE_SIZE;
    const int right_tile = x1 / TILE_SIZE;
    const int top_tile = y0 / TILE_SIZE;
    const int bottom_tile = y1 / TILE_SIZE;

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            int tx0;
            int ty0;
            int tx1;
            int ty1;

            if (map_overlay[y][x] >= 0 &&
                tile_collision_rect((TileId)map_overlay[y][x], x, y, &tx0, &ty0, &tx1, &ty1) &&
                rects_overlap(x0, y0, x1, y1, tx0, ty0, tx1, ty1)) {
                if (dy > 0 && is_cliff_tile((TileId)map_overlay[y][x])) {
                    startMCJumpEffect();
                    continue;
                }
                if (dx < 0 && is_vertical_cliff_tile((TileId)map_overlay[y][x])) {
                    startMCJumpEffect();
                    continue;
                }
                return false;
            }

            if (tile_collision_rect(map[y][x], x, y, &tx0, &ty0, &tx1, &ty1) &&
                rects_overlap(x0, y0, x1, y1, tx0, ty0, tx1, ty1)) {
                if (dy > 0 && is_cliff_tile(map[y][x])) {
                    startMCJumpEffect();
                    continue;
                }
                if (dx < 0 && is_vertical_cliff_tile(map[y][x])) {
                    startMCJumpEffect();
                    continue;
                }
                return false;
            }
        }
    }

    return true;
}

void draw_map_cell(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return;
    }

    drawTile(x, y, map[y][x]);
    if (map_overlay[y][x] >= 0) {
        draw_overlay_tile((TileId)map_overlay[y][x], x, y);
    }
}

void init_map(void) {
    load_map_preset(MAP_PRESET_ROUTE);
}

void draw_map(void) {
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            drawTile(col, row, map[row][col]);
        }
    }
    draw_all_decor();
    if (g_current_preset == MAP_PRESET_GROUND && g_current_decor_layout == MAP_DECOR_ROUTE_B) {
        if (!g_route_b_pokeball_collected) {
            draw_sprite_any(smallPokeballSprite,
                            SMALL_POKEBALL_SPRITE_WIDTH, SMALL_POKEBALL_SPRITE_HEIGHT,
                            ROUTE_B_POKEBALL_DRAW_X, ROUTE_B_POKEBALL_DRAW_Y,
                            TRANSPARENT_COLOUR);
        }
        if (g_route_b_trainer_state == ROUTE_B_TRAINER_WALKING) {
            draw_sprite_any(trainerWalkingFrames[g_route_b_trainer_frame],
                            TRAINER_WALKING_WIDTH, TRAINER_WALKING_HEIGHT,
                            g_route_b_trainer_x, g_route_b_trainer_y,
                            TRANSPARENT_COLOUR);
        } else {
            draw_sprite_any(trainerLeftMapSprite,
                            TRAINER_LEFT_MAP_WIDTH, TRAINER_LEFT_MAP_HEIGHT,
                            g_route_b_trainer_x, g_route_b_trainer_y,
                            TRANSPARENT_COLOUR);
        }
    }
    if (g_current_preset == MAP_PRESET_POKEMON_CENTER_INTERIOR) {
        if (g_nurse_facing_left) {
            draw_sprite_any(pokemonCenterNurseLeftSprite,
                            POKEMON_CENTER_NURSE_LEFT_WIDTH, POKEMON_CENTER_NURSE_LEFT_HEIGHT,
                            POKEMON_CENTER_NURSE_X, POKEMON_CENTER_NURSE_Y,
                            TRANSPARENT_COLOUR);
        } else {
            draw_sprite_any(pokemonCenterNurseSprite,
                            POKEMON_CENTER_NURSE_WIDTH, POKEMON_CENTER_NURSE_HEIGHT,
                            POKEMON_CENTER_NURSE_X, POKEMON_CENTER_NURSE_Y,
                            TRANSPARENT_COLOUR);
        }
        draw_sprite_any(pokemonCenterDeskSprite,
                        POKEMON_CENTER_DESK_WIDTH, POKEMON_CENTER_DESK_HEIGHT,
                        95, 47,
                        TRANSPARENT_COLOUR);
        switch (g_npc1_facing) {
            case NPC1_FACING_BACK:
                draw_sprite_any(npc1BackMapSprite,
                                NPC1_BACK_MAP_WIDTH, NPC1_BACK_MAP_HEIGHT,
                                POKEMON_CENTER_NPC1_X, POKEMON_CENTER_NPC1_Y,
                                TRANSPARENT_COLOUR);
                break;
            case NPC1_FACING_LEFT:
                draw_sprite_any(npc1LeftMapSprite,
                                NPC1_LEFT_MAP_WIDTH, NPC1_LEFT_MAP_HEIGHT,
                                POKEMON_CENTER_NPC1_X, POKEMON_CENTER_NPC1_Y,
                                TRANSPARENT_COLOUR);
                break;
            case NPC1_FACING_RIGHT:
                draw_sprite_any(npc1RightMapSprite,
                                NPC1_RIGHT_MAP_WIDTH, NPC1_RIGHT_MAP_HEIGHT,
                                POKEMON_CENTER_NPC1_X, POKEMON_CENTER_NPC1_Y,
                                TRANSPARENT_COLOUR);
                break;
            case NPC1_FACING_FRONT:
            default:
                draw_sprite_any(npc1FrontMapSprite,
                                NPC1_FRONT_MAP_WIDTH, NPC1_FRONT_MAP_HEIGHT,
                                POKEMON_CENTER_NPC1_X, POKEMON_CENTER_NPC1_Y,
                                TRANSPARENT_COLOUR);
                break;
        }
    }
    if (g_current_preset == MAP_PRESET_POKE_MART_INTERIOR) {
        draw_sprite_any(pokemonCenterClerkSprite,
                        POKEMON_CENTER_CLERK_WIDTH, POKEMON_CENTER_CLERK_HEIGHT,
                        POKE_MART_CLERK_X, POKE_MART_CLERK_Y,
                        TRANSPARENT_COLOUR);
    }
    if (g_current_preset == MAP_PRESET_GYM_INTERIOR) {
        draw_sprite_any(cynthiaFrontMapSprite,
                        CYNTHIA_FRONT_MAP_WIDTH, CYNTHIA_FRONT_MAP_HEIGHT,
                        GYM_CYNTHIA_DRAW_X, GYM_CYNTHIA_DRAW_Y,
                        TRANSPARENT_COLOUR);
    }
}

bool map_can_talk_to_route_b_cynthia(const McBounds *bounds) {
    const int interaction_margin = 18;
    int trainer_x0;
    int trainer_y0;
    int trainer_x1;
    int trainer_y1;

    if (bounds == 0 || !bounds->valid) {
        return false;
    }
    if (g_current_preset != MAP_PRESET_GROUND || g_current_decor_layout != MAP_DECOR_ROUTE_B) {
        return false;
    }
    if (g_route_b_trainer_state != ROUTE_B_TRAINER_ARRIVED) {
        return false;
    }
    get_route_b_trainer_bbox(&trainer_x0, &trainer_y0, &trainer_x1, &trainer_y1);

    return !(bounds->x1 < (trainer_x0 - interaction_margin) ||
             (trainer_x1 + interaction_margin) < bounds->x0 ||
             bounds->y1 < (trainer_y0 - interaction_margin) ||
             (trainer_y1 + interaction_margin) < bounds->y0);
}

bool map_can_talk_to_gym_cynthia(const McBounds *bounds) {
    const int horizontal_margin = 20;
    const int vertical_margin = 28;
    int cynthia_x0;
    int cynthia_y0;
    int cynthia_x1;
    int cynthia_y1;

    if (bounds == NULL || !bounds->valid) {
        return false;
    }
    if (g_current_preset != MAP_PRESET_GYM_INTERIOR) {
        return false;
    }

    get_gym_cynthia_bbox(&cynthia_x0, &cynthia_y0, &cynthia_x1, &cynthia_y1);

    return !(bounds->x1 < (cynthia_x0 - horizontal_margin) ||
             (cynthia_x1 + horizontal_margin) < bounds->x0 ||
             bounds->y1 < (cynthia_y0 - vertical_margin) ||
             (cynthia_y1 + vertical_margin) < bounds->y0);
}

bool map_can_talk_to_pokemon_center_nurse(const McBounds *bounds) {
    const int horizontal_margin = 20;
    const int vertical_margin = 28;

    if (bounds == 0 || !bounds->valid) {
        return false;
    }
    if (g_current_preset != MAP_PRESET_POKEMON_CENTER_INTERIOR) {
        return false;
    }

    return !(bounds->x1 < (POKEMON_CENTER_NURSE_BBOX_X - horizontal_margin) ||
             (POKEMON_CENTER_NURSE_BBOX_X + POKEMON_CENTER_NURSE_BBOX_W - 1 + horizontal_margin) < bounds->x0 ||
             bounds->y1 < (POKEMON_CENTER_NURSE_BBOX_Y - vertical_margin) ||
             (POKEMON_CENTER_NURSE_BBOX_Y + POKEMON_CENTER_NURSE_BBOX_H - 1 + vertical_margin) < bounds->y0);
}

bool map_can_talk_to_pokemon_center_npc1(const McBounds *bounds) {
    const int horizontal_margin = 20;
    const int vertical_margin = 28;
    int npc1_x0;
    int npc1_y0;
    int npc1_x1;
    int npc1_y1;

    if (bounds == NULL || !bounds->valid) {
        return false;
    }
    if (g_current_preset != MAP_PRESET_POKEMON_CENTER_INTERIOR) {
        return false;
    }

    get_pokemon_center_npc1_bbox(&npc1_x0, &npc1_y0, &npc1_x1, &npc1_y1);

    return !(bounds->x1 < (npc1_x0 - horizontal_margin) ||
             (npc1_x1 + horizontal_margin) < bounds->x0 ||
             bounds->y1 < (npc1_y0 - vertical_margin) ||
             (npc1_y1 + vertical_margin) < bounds->y0);
}

void map_face_pokemon_center_npc1_toward(const McBounds *bounds) {
    if (g_current_preset != MAP_PRESET_POKEMON_CENTER_INTERIOR) {
        return;
    }
    set_npc1_facing_toward_bounds(bounds);
}

bool map_can_talk_to_poke_mart_clerk(const McBounds *bounds) {
    const int horizontal_margin = 24;
    const int vertical_margin = 28;

    if (bounds == NULL || !bounds->valid) {
        return false;
    }
    if (g_current_preset != MAP_PRESET_POKE_MART_INTERIOR) {
        return false;
    }

    return !(bounds->x1 < (POKE_MART_CLERK_BBOX_X - horizontal_margin) ||
             (POKE_MART_CLERK_BBOX_X + POKE_MART_CLERK_BBOX_W - 1 + horizontal_margin) < bounds->x0 ||
             bounds->y1 < (POKE_MART_CLERK_BBOX_Y - vertical_margin) ||
             (POKE_MART_CLERK_BBOX_Y + POKE_MART_CLERK_BBOX_H - 1 + vertical_margin) < bounds->y0);
}

bool map_is_mc_on_grass_patch(const McBounds *bounds) {
    if (bounds == NULL || !bounds->valid) {
        return false;
    }

    const int foot_tile_x = ((bounds->x0 + bounds->x1) / 2) / TILE_SIZE;
    const int foot_tile_y = bounds->y1 / TILE_SIZE;
    if (foot_tile_x < 0 || foot_tile_x >= MAP_WIDTH || foot_tile_y < 0 || foot_tile_y >= MAP_HEIGHT) {
        return false;
    }

    return map_overlay[foot_tile_y][foot_tile_x] == TILE_GRASS_PATCH;
}

bool map_can_use_pokemon_center_pc(const McBounds *bounds) {
    const int horizontal_margin = 0;
    const int vertical_margin = 0;

    if (g_current_preset != MAP_PRESET_POKEMON_CENTER_INTERIOR || bounds == NULL || !bounds->valid) {
        return false;
    }
    return !(bounds->x1 < (POKEMON_CENTER_PC_BBOX_X - horizontal_margin) ||
             (POKEMON_CENTER_PC_BBOX_X + POKEMON_CENTER_PC_BBOX_W - 1 + horizontal_margin) < bounds->x0 ||
             bounds->y1 < (POKEMON_CENTER_PC_BBOX_Y - vertical_margin) ||
             (POKEMON_CENTER_PC_BBOX_Y + POKEMON_CENTER_PC_BBOX_H - 1 + vertical_margin) < bounds->y0);
}
