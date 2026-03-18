#include "font8x8_basic.h"
#include "../../address_map.h"
#include <string.h>

#define TILE_SIZE 16
#define TRANSPARENT_COLOUR 0xF81F //anyy pink pixels will be transparent
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define FRAMEBUFFER_0 0x02000000
#define FRAMEBUFFER_1 0x02040000

#ifndef GRAPHICS_USE_DOUBLE_BUFFER
#define GRAPHICS_USE_DOUBLE_BUFFER 1
#endif

static volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
static unsigned int pixel_buffer_start;   // address drawing into (back buffer)
#if GRAPHICS_USE_DOUBLE_BUFFER
static unsigned int front_buffer_start;   // address currently being displayed
#endif
static unsigned int back_buffer_start;    // address draw the next frame into

void init_graphics() {
    pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;

#if GRAPHICS_USE_DOUBLE_BUFFER
    front_buffer_start = FRAMEBUFFER_0;
    back_buffer_start = FRAMEBUFFER_1;
#else
    back_buffer_start = FRAMEBUFFER_0;
#endif

    // configure the back buffer for drawing.
    *(pixel_ctrl_ptr + 1) = back_buffer_start;
    pixel_buffer_start = back_buffer_start;

    clear_screen();
    wait_for_vsync();

#if GRAPHICS_USE_DOUBLE_BUFFER
    // Clear the other buffer
    pixel_buffer_start = (back_buffer_start == FRAMEBUFFER_0) ? FRAMEBUFFER_1 : FRAMEBUFFER_0;
    clear_screen();
    pixel_buffer_start = back_buffer_start;
#endif
}

void draw_pixel(int x, int y, short int colour) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    volatile short int *one_pixel_address;

    one_pixel_address = (volatile short int *)(pixel_buffer_start + (y << 10) + (x << 1));
    *one_pixel_address = colour; //write the colour to the pixel buffer
}

void clear_screen() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            draw_pixel(x, y, 0); //draws black pixel at every position
        }
    }
}

void wait_for_vsync() {
    *(pixel_ctrl_ptr + 1) = back_buffer_start;

    // init synchronization.
    *pixel_ctrl_ptr = 1;
    while ((*(pixel_ctrl_ptr + 3) & 0x01) != 0) { }

#if GRAPHICS_USE_DOUBLE_BUFFER
    //old front becomes  back.
    unsigned int tmp = front_buffer_start;
    front_buffer_start = back_buffer_start;
    back_buffer_start = tmp;

    // get nextback buffer and update the draw pointer.
    *(pixel_ctrl_ptr + 1) = back_buffer_start;
    pixel_buffer_start = back_buffer_start;
#else
    pixel_buffer_start = back_buffer_start;
#endif
}

void set_pixel_buffer(unsigned int addr) {
    // Treat addr as the new back buffer
    back_buffer_start = addr;
    *(pixel_ctrl_ptr + 1) = back_buffer_start;
    pixel_buffer_start = back_buffer_start;
}

unsigned int get_other_buffer(void) {
    // Returns the buffer that is not currently being drawn into.
    return (pixel_buffer_start == FRAMEBUFFER_0) ? FRAMEBUFFER_1 : FRAMEBUFFER_0;
}


void draw_hline(int x, int y, int width, short int colour){
    for (int position = x; position < x + width; position++) {
        draw_pixel(position, y, colour); //draws horizontal line at y position
    }
}

void draw_vline(int x, int y, int height, short int colour) {
    for (int position = y; position < y + height; position++) {
        draw_pixel(x, position, colour); //draws vertical line at x position
    }
}

void draw_rect(int x, int y, int width, int height, short int colour) {
    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            draw_pixel(x + dx, y + dy, colour);
        }
    }
}

void draw_rect_outline(int x, int y, int width, int height, short int colour) {
    draw_hline(x, y, width, colour); //top line
    draw_hline(x, y + height - 1, width, colour); //bottom line
    draw_vline(x, y, height, colour); //left line
    draw_vline(x + width - 1, y, height, colour); //right line
}

void draw_window(int x, int y, int w, int h, short int border_colour, short int fill_colour) {
    draw_rect(x, y, w, h, fill_colour);
    draw_rect_outline(x, y, w, h, border_colour);
}

// Draw a 16x16 tile; tile is a 16*16 array of colours (pre defined).
void draw_tiles(const short *tile, int x, int y) {
    for (int tile_y = 0; tile_y < TILE_SIZE; tile_y++) {
        for (int tile_x = 0; tile_x < TILE_SIZE; tile_x++) {
            draw_pixel(x + tile_x, y + tile_y, tile[tile_y * TILE_SIZE + tile_x]);
        }
    }
}

// Draw a 16x16 sprite with transparency; any pixel equal to 'transparent'
// is skipped so the background shows through.
void draw_sprite_transparent(const short *sprite, int x, int y, short transparent) {
    for (int sprite_y = 0; sprite_y < TILE_SIZE; sprite_y++) {
        for (int sprite_x = 0; sprite_x < TILE_SIZE; sprite_x++) {
            short colour = sprite[sprite_y * TILE_SIZE + sprite_x];
            if (colour != transparent) { //if the pixel is not transparent, draw it
                draw_pixel(x + sprite_x, y + sprite_y, colour);
            }
        }
    }
}

void draw_sprite_any(const unsigned short *sprite,
                     int width, int height,
                     int x, int y,
                     short transparent)
{
    for (int sy = 0; sy < height; sy++) {
        for (int sx = 0; sx < width; sx++) {
            short colour = sprite[sy * width + sx];
            if (colour != transparent) {
                draw_pixel(x + sx, y + sy, colour);
            }
        }
    }
}

void draw_textbox_instant_text(const unsigned short *textBoxSprite, int x, int y, const char *string, short colour) {
    draw_sprite_any(textBoxSprite, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT, x, y, TRANSPARENT_COLOUR);
    draw_string(x + 30, y + 30, string, colour);
}

void draw_textbox_animated_text(const unsigned short *textBoxSprite, int x, int y, const char *string, short colour) {
    // text cursor variables
    static int cursor = 0;
    static int frameTimer = 0;
    const int speedFrames = 4; // frames per character

    int len = (int)strlen(string);
    if (len <= 0) return;

    // increment cursor location
    if (cursor < len) {
        frameTimer++;
        if (frameTimer >= speedFrames) {
            frameTimer = 0;
            cursor++;
        }
    }

    // Draw textbox
    draw_sprite_any(textBoxSprite, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT, x, y, TRANSPARENT_COLOUR);

    char buffer[128];
    int n = cursor;
    if (n > len) n = len;
    if (n > (int)sizeof(buffer) - 1) n = (int)sizeof(buffer) - 1;

    memcpy(buffer, string, n);
    buffer[n] = '\0';

    draw_string(x + 30, y + 30, buffer, colour);
}

void draw_char(int x, int y, char c, short int colour) {
    unsigned char character = (unsigned char)c;
    if (character >= 128) return; //out of bounds

    const unsigned char *font_data = font8x8_basic[character];

    for (int row = 0; row < 8; row++) {
        unsigned char bits = font_data[row];
        for (int col = 0; col < 8; col++) {
            if (bits & (1 << col)) { //if the bit is 1, draw the pixel. 1 << col masks the bit to col
                draw_pixel(x + col, y + row, colour);
            }
        }
    }
}

void draw_string(int x, int y, const char *string, short colour)
{
    int cursor_x = x;
    while (*string) {
        draw_char(cursor_x, y, *string, colour);
        cursor_x += 8; // next character cell
        string++;
    }
}








