#include "tiles.h"
#include "predefined_graphics.h"

void drawTile(int x, int y, TileId tileId) {
    short* tile = tiles[tileId];
    int screen_x = x * TILE_SIZE;
    int screen_y = y * TILE_SIZE;//getting tile coordinates on screen (since tiles are 16x16)
    //so 0,0 is 0,0 and 1,1 is 16,16
    
    for (int tile_y = 0; tile_y < TILE_SIZE; tile_y++) {
        for (int tile_x = 0; tile_x < TILE_SIZE; tile_x++) {
            draw_pixel(screen_x + tile_x, screen_y + tile_y, tile[tile_y * TILE_SIZE + tile_x]);
        } //flatten the 2d tile array into 1d array to draw
    }

}