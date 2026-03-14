#ifndef GRAPHICS_H
#define GRAPHICS_H

#define TILE_SIZE 16
#define TRANSPARENT_COLOUR 0xF81F //anyy pink pixels will be transparent
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Base address for the pixel buffer
#define PIXEL_BASE 0xFF203020

void init_graphics();

void draw_pixel(int x, int y, short int colour);
void clear_screen(void);
void wait_for_vsync(void);
void set_pixel_buffer(unsigned int addr);
unsigned int get_other_buffer(void);
int get_back_buffer_index(void);
unsigned int debug_get_pixel_buffer_start(void);
void agent_log_state(const char *location, const char *message, const char *hypothesisId,
                     int value1, int value2, unsigned int bufAddr);

void draw_hline(int x, int y, int width, short int colour);
void draw_vline(int x, int y, int height, short int colour);

void draw_rect(int x, int y, int width, int height, short int colour);
void draw_rect_outline(int x, int y, int width, int height, short int colour);
void draw_window(int x, int y, int w, int h, short int border_colour, short int fill_colour);


void draw_tiles(const short *tile, int x, int y);
void draw_sprite_transparent(const short *sprite, int x, int y, short transparent);

void draw_circle(int cx, int cy, int radius, short int colour);
void draw_circle_filled(int cx, int cy, int radius, short int colour);
void draw_triangle(int x0,int y0,int x1,int y1,int x2,int y2, short int colour);

void draw_char(int x, int y, char c, short int colour);
void draw_string(int x, int y, const char *string, short colour);


#endif