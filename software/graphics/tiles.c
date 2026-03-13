#include "tiles.h"
#include "graphics.h"

void drawTile(int x, int y, TileId tileId) {
    short* tile = tiles[tileId];
    int screen_x = x * TILE_SIZE;
    int screen_y = y * TILE_SIZE; // tile coords to pixel coords

    for (int tile_y = 0; tile_y < TILE_SIZE; tile_y++) {
        for (int tile_x = 0; tile_x < TILE_SIZE; tile_x++) {
            draw_pixel(screen_x + tile_x,
                       screen_y + tile_y,
                       tile[tile_y * TILE_SIZE + tile_x]);
        }
    }
}