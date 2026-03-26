#include "map.h"
#include "tiles.h"
#include "backdrops/backdrop1_tiles.h"
#include "backdrops/ground_tiles.h"

TileId map[MAP_HEIGHT][MAP_WIDTH];
int map_overlay[MAP_HEIGHT][MAP_WIDTH];

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

void apply_map_decor(void) {
    static const MapTilePosition grass_patch_positions[] = {
        {6, 2}, {7, 2}, {6, 3}, {7, 3},
        {1, 6}, {2, 6}, {3, 6},
        {1, 7}, {2, 7},
        {12, 9}, {13, 9}, {12, 10},
        {16, 12}, {17, 12}, {16, 13}
    };
    static const MapTilePosition tree_positions[] = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
        {0, 10}, {0, 11}, {0, 12}, {0, 13},
        {19, 0}, {19, 1}, {19, 2}, {19, 3}, {19, 4}, {19, 5}, {19, 6},
        {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12}, {19, 13}
    };

    for (unsigned int i = 0; i < sizeof(grass_patch_positions) / sizeof(grass_patch_positions[0]); i++) {
        map_set_overlay_tile_xy(grass_patch_positions[i].x, grass_patch_positions[i].y, TILE_GRASS_PATCH);
    }

    for (unsigned int i = 0; i < sizeof(tree_positions) / sizeof(tree_positions[0]); i++) {
        map_place_tree_xy(tree_positions[i].x, tree_positions[i].y);
    }
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
    return true;
}

bool map_set_overlay_tile_xy(int x, int y, TileId tile) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return false;
    }

    map_overlay[y][x] = tile;
    return true;
}

bool map_is_walkable_tile(TileId tile) {
    return tile != TILE_TREE_TOP && tile != TILE_TREE_BOTTOM;
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
        const short *overlay_tile = tiles[map_overlay[y][x]];
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
}

void init_map(void) {
    load_map_preset(MAP_PRESET_ROUTE);
}

void draw_map(void) {
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            draw_map_cell(col, row);
        }
    }
}
