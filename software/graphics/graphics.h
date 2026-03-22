#ifndef GRAPHICS_H
#define GRAPHICS_H

#define TILE_SIZE 16
#define TRANSPARENT_COLOUR 0xF81F //anyy pink pixels will be transparent
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Base address for the pixel buffer
#define PIXEL_BASE 0xFF203020

void init_graphics(void);

void draw_pixel(int x, int y, short int colour);
void clear_screen(void);
void wait_for_vsync(void);
void set_pixel_buffer(unsigned int addr);
unsigned int get_other_buffer(void);

void draw_hline(int x, int y, int width, short int colour);
void draw_vline(int x, int y, int height, short int colour);

void draw_rect(int x, int y, int width, int height, short int colour);
void draw_rect_outline(int x, int y, int width, int height, short int colour);
void draw_window(int x, int y, int w, int h, short int border_colour, short int fill_colour);


void draw_tiles(const short *tile, int x, int y);
void draw_sprite_transparent(const short *sprite, int x, int y, short transparent);

// Generic sprite drawer
void draw_sprite_any(const unsigned short *sprite,
                     int width, int height,
                     int x, int y,
                     short transparent);


#define SHAKE_SPRITE_FRAME_COUNT 12
void draw_sprite_any_shake(const unsigned short *sprite,
                           int width, int height,
                           int x, int y,
                           short transparent,
                           int shake_frame);


#define FLASH_SPRITE_FRAME_COUNT 16
void draw_sprite_any_flash(const unsigned short *sprite,
                           int width, int height,
                           int x, int y,
                           short transparent,
                           int flash_frame);

#define BOB_SPRITE_FRAME_COUNT 16
void draw_sprite_any_bob(const unsigned short *sprite,
                         int width, int height,
                         int x, int y,
                         short transparent,
                         int bob_frame);

#define SILHOUETTE_SPRITE_FRAME_COUNT 16
void draw_sprite_any_silhouette(const unsigned short *sprite,
                                int width, int height,
                                int x, int y,
                                short transparent,
                                short silhouette_colour,
                                int silhouette_frame);

void draw_circle(int cx, int cy, int radius, short int colour);
void draw_circle_filled(int cx, int cy, int radius, short int colour);
void draw_triangle(int x0,int y0,int x1,int y1,int x2,int y2, short int colour);
void draw_textbox_instant_text(const unsigned short *textBoxSprite, int x, int y, const char *string, short colour);
int draw_textbox_animated_text(const unsigned short *textBoxSprite, int x, int y, const char *string, short colour);
void hide_textbox(int x, int y);
void draw_char(int x, int y, char c, short int colour);
void draw_string(int x, int y, const char *string, short colour);
#endif
