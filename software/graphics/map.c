#include "map.h"
#include "tiles.h"
#include "backdrops/backdrop1_tiles.h"
#include "backdrops/ground_tiles.h"
#include "backdrops/pokemon_center_interior_tiles.h"

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

static DecorEntry g_decor_entries[MAP_WIDTH * MAP_HEIGHT * 2];
static int g_decor_entry_count = 0;

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
    static const MapTilePosition route_a_grass_patch_positions[] = {
        {7,4}, {8,4}, {9,4}, {10,4}, {11,4}, {12,4},
        {7,5}, {8,5}, {9,5}, {10,5}, {11,5}, {12,5},
        {7,6}, {8,6}, {9,6}, {10,6}, {11,6}, {12,6},
        {7,7}, {8,7}, {9,7}, {10,7}, {11,7}, {12,7},
        {7,8}, {8,8}, {9,8}, {10,8}, {11,8}, {12,8},
        {7,9}, {8,9}, {9,9}, {10,9}, {11,9}, {12,9},
    };
    static const MapTilePosition route_a_tree_positions[] = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
        {0, 10}, {0, 11}, {0, 12}, {0, 13},
        {19, 0}, {19, 1}, {19, 2}, {19, 3}, {19, 4}, {19, 5}, {19, 6},
        {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12}, {19, 13}
    };
    static const MapTilePosition route_a_pokemon_centers[] = {
        {8, 1},
    };

    static const MapTilePosition route_b_grass_patch_positions[] = {
        {7,4}, {8,4}, {9,4}, {10,4}, {11,4}, {12,4},
        {7,5}, {8,5}, {9,5}, {10,5}, {11,5}, {12,5},
        {7,6}, {8,6}, {9,6}, {10,6}, {11,6}, {12,6},
        {7,7}, {8,7}, {9,7}, {10,7}, {11,7}, {12,7},
        {7,8}, {8,8}, {9,8}, {10,8}, {11,8}, {12,8},
        {7,9}, {8,9}, {9,9}, {10,9}, {11,9}, {12,9},
    };
    static const MapTilePosition route_b_tree_positions[] = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6},
        {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13},
        {19, 0}, {19, 1}, {19, 2}, {19, 3}, {19, 4},
        {19, 10}, {19, 11}, {19, 12}, {19, 13},
    };
    static const MapDecorDefinition route_a_decor = {
        route_a_grass_patch_positions,
        sizeof(route_a_grass_patch_positions) / sizeof(route_a_grass_patch_positions[0]),
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
};

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
_Static_assert((sizeof(preset_ptrs) / sizeof(preset_ptrs[0])) == MAP_PRESET_COUNT,
               "preset_ptrs out of sync with MapPresetId");
#endif

void load_map_preset(MapPresetId preset) {
    if (preset >= MAP_PRESET_COUNT) return;
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
           tile != TILE_POKEMON_CENTER_BOTTOM_RIGHT;
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

bool map_bounds_are_walkable(int x0, int y0, int x1, int y1) {
    if (x0 < 0 || y0 < 0 || x1 >= MAP_WIDTH * TILE_SIZE || y1 >= MAP_HEIGHT * TILE_SIZE) {
        return false;
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
                return false;
            }

            if (tile_collision_rect(map[y][x], x, y, &tx0, &ty0, &tx1, &ty1) &&
                rects_overlap(x0, y0, x1, y1, tx0, ty0, tx1, ty1)) {
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
}
