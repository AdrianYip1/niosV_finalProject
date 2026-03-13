#include "graphics.h"
#include "predefined_graphics.h"
#include "predefined_colours.h"

int main(void)
{
    // Init VGA and tiles
    init_graphics();
    init_predefined_graphics();

    clear_screen();

    // Draw one grass tile at (0,0)
    draw_tiles(tiles[TILE_GRASS], 0, 0);

    // Draw a 5x3 block of grass tiles starting at (32, 32)
    for (int ty = 0; ty < 3; ty++) {
        for (int tx = 0; tx < 5; tx++) {
            int x = 32 + tx * TILE_SIZE;
            int y = 32 + ty * TILE_SIZE;
            draw_tiles(tiles[TILE_GRASS], x, y);
        }
    }

    // Draw some text over the tiles
    draw_string(10, 10, "HELLO WORLD", GREEN);

    // Main loop: just keep the frame on screen
    while (1) {
        wait_for_vsync();
    }

    return 0;
}