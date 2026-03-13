#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Base address for the pixel buffer
#define PIXEL_BASE 0xFF203020
#define pixel_buffer_start ((short int *)PIXEL_BASE)

void draw_pixel(int x, int y, short int colour);
void clear_screen(void);
void wait_for_vsync(void);

#endif