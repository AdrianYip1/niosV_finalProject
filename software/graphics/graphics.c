#include "font8x8_basic.h"
#include "font5x9_pokemon.h"
#include "../../address_map.h"
#include "textbox/textBoxSprite.h"
#include "map.h"
#include "tiles.h"
#include <string.h>

#define TILE_SIZE 16
#define TRANSPARENT_COLOUR 0xF81F //anyy pink pixels will be transparent
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define FRAMEBUFFER_0 0x02000000
#define FRAMEBUFFER_1 0x02040000

//fonts
typedef enum {
    FONT_8X8 = 0,  // font8x8_basic
    FONT_5X9 = 1,  // font5x9_pokemon
} FontId;

static FontId current_font = FONT_8X8;

void set_font(FontId font) { current_font = font; }
FontId get_font(void)      { return current_font; }


static volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
static unsigned int pixel_buffer_start;   // address drawing into (back buffer)
static unsigned int front_buffer_start;   // address currently being displayed
static unsigned int back_buffer_start;    // address draw the next frame into

void init_graphics() {
    pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;

    front_buffer_start = FRAMEBUFFER_0;
    back_buffer_start = FRAMEBUFFER_1;

    // configure the back buffer for drawing.
    *(pixel_ctrl_ptr + 1) = back_buffer_start;
    pixel_buffer_start = back_buffer_start;

    clear_screen();
    wait_for_vsync();

    // Clear the other buffer
    pixel_buffer_start = (back_buffer_start == FRAMEBUFFER_0) ? FRAMEBUFFER_1 : FRAMEBUFFER_0;
    clear_screen();
    pixel_buffer_start = back_buffer_start;
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

    //old front becomes  back.
    unsigned int tmp = front_buffer_start;
    front_buffer_start = back_buffer_start;
    back_buffer_start = tmp;

    // get nextback buffer and update the draw pointer.
    *(pixel_ctrl_ptr + 1) = back_buffer_start;
    pixel_buffer_start = back_buffer_start;
}

void set_pixel_buffer(unsigned int addr) {
    // Treat addr as the new back buffer
    back_buffer_start = addr;
    *(pixel_ctrl_ptr + 1) = back_buffer_start;
    pixel_buffer_start = back_buffer_start;
}

unsigned int get_other_buffer(void) {
    // With double buffering, other is the currently displayed front buffer.
    return front_buffer_start;
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
            unsigned short colour = sprite[sy * width + sx];
            if (colour == (unsigned short)transparent) {
                continue;
            }
            {
                draw_pixel(x + sx, y + sy, colour);
            }
        }
    }
}

void draw_sprite_any_shake(const unsigned short *sprite,
                           int width, int height,
                           int x, int y,
                           short transparent,
                           int shake_frame)
{

    static const signed char dx_pattern[SHAKE_SPRITE_FRAME_COUNT] = {0, -4, 4, -4, 4, -2, 2, -2, 2, -1, 1, 0};

    int dx = 0;
    if (shake_frame >= 0 && shake_frame < SHAKE_SPRITE_FRAME_COUNT) {
        dx = (int)dx_pattern[shake_frame];
    }

    draw_sprite_any(sprite, width, height, x + dx, y, transparent);
}

static unsigned short shade_565(unsigned short colour, int delta);

void draw_sprite_any_flash(const unsigned short *sprite,
                           int width, int height,
                           int x, int y,
                           short transparent,
                           int flash_frame)
{

    if (flash_frame < 0 || flash_frame >= FLASH_SPRITE_FRAME_COUNT) {
        draw_sprite_any(sprite, width, height, x, y, transparent);
        return;
    }

    const int on = ((flash_frame / 2) % 2) == 0;
    if (on) {
        draw_sprite_any(sprite, width, height, x, y, transparent);
        return;
    }

    for (int sy = 0; sy < height; sy++) {
        for (int sx = 0; sx < width; sx++) {
            unsigned short colour = sprite[sy * width + sx];
            if (colour == (unsigned short)transparent) {
                continue;
            }
            draw_pixel(x + sx, y + sy, shade_565(colour, 8));
        }
    }
}

//up and down motion
void draw_sprite_any_bob(const unsigned short *sprite,
                         int width, int height,
                         int x, int y,
                         short transparent,
                         int bob_frame)
{

    static const signed char dy_pattern[BOB_SPRITE_FRAME_COUNT] = {
        0, -1, -1, 0, 0, 1, 1, 0,
        0, -1, -1, 0, 0, 1, 1, 0
    };

    int dy = 0;
    if (bob_frame >= 0 && bob_frame < BOB_SPRITE_FRAME_COUNT) {
        dy = (int)dy_pattern[bob_frame];
    }

    draw_sprite_any(sprite, width, height, x, y + dy, transparent);
}

static unsigned short shade_565(unsigned short colour, int delta) {
    // delta in roughly [-8..+8]. Green has twice the steps.
    int red = (colour >> 11) & 31;
    int green = (colour >> 5) & 63;
    int blue = colour & 31;

    red += delta;
    green += (delta * 2);
    blue += delta;

    if (red < 0) red = 0; else if (red > 31) red = 31;
    if (green < 0) green = 0; else if (green > 63) green = 63;
    if (blue < 0) blue = 0; else if (blue > 31) blue = 31;

    return (unsigned short)((red << 11) | (green << 5) | blue);
}

void draw_sprite_any_shade_pulse(const unsigned short *sprite,
                                 int width, int height,
                                 int x, int y,
                                 short transparent,
                                 int pulse_frame)
{
    if (!sprite || width <= 0 || height <= 0) return;

    const int frame = (pulse_frame < 0) ? 0 : (pulse_frame % SHADE_PULSE_FRAME_COUNT);
    static const signed char shade_pattern[SHADE_PULSE_FRAME_COUNT] = {
        -6, -3, 0, 3, 6, 3, 0, -3,
        -6, -3, 0, 3, 6, 3, 0, -3
    };

    const int delta = (int)shade_pattern[frame];

    for (int sy = 0; sy < height; sy++) {
        for (int sx = 0; sx < width; sx++) {
            unsigned short colour = sprite[sy * width + sx];
            if (colour == (unsigned short)transparent) {
                continue;
            }
            draw_pixel(x + sx, y + sy, shade_565(colour, delta));
        }
    }
}

void draw_sprite_any_silhouette(const unsigned short *sprite,
                                int width, int height,
                                int x, int y,
                                short transparent,
                                short silhouette_colour,
                                int silhouette_frame)
{
    if (silhouette_frame < 0 || silhouette_frame >= SILHOUETTE_SPRITE_FRAME_COUNT) {
        draw_sprite_any(sprite, width, height, x, y, transparent);
        return;
    }

    // Animated silhouette: slight jitter + blink + pulsing brightness.
    static const signed char dx_pattern[SILHOUETTE_SPRITE_FRAME_COUNT] = {
        0, 0, -1, 0, 1, 0, -1, 0,
        1, 0, 0, 0, -1, 0, 1, 0
    };
    static const signed char shade_pattern[SILHOUETTE_SPRITE_FRAME_COUNT] = {
        -6, -3, 0, 3, 6, 3, 0, -3,
        -6, -3, 0, 3, 6, 3, 0, -3
    };

    const int visible = ((silhouette_frame / 2) % 2) == 0;
    if (!visible) return;

    const int dx = (int)dx_pattern[silhouette_frame];
    const unsigned short draw_colour = shade_565((unsigned short)silhouette_colour, (int)shade_pattern[silhouette_frame]);

    for (int sy = 0; sy < height; sy++) {
        for (int sx = 0; sx < width; sx++) {
            unsigned short colour = sprite[sy * width + sx];
            if (colour == (unsigned short)transparent) {
                continue;
            }

            draw_pixel(x + dx + sx, y + sy, draw_colour);
        }
    }
}

void draw_sprite_any_region(const unsigned short *sprite,
                            int sprite_width, int sprite_height,
                            int source_x, int source_y,
                            int region_width, int region_height,
                            int dst_x, int dst_y,
                            short transparent)
{
    if (!sprite || sprite_width <= 0 || sprite_height <= 0) return;
    if (region_width <= 0 || region_height <= 0) return;

    if (source_x < 0) { dst_x -= source_x; region_width += source_x; source_x = 0; }
    if (source_y < 0) { dst_y -= source_y; region_height += source_y; source_y = 0; }
    if (source_x + region_width > sprite_width) region_width = sprite_width - source_x;
    if (source_y + region_height > sprite_height) region_height = sprite_height - source_y;
    if (region_width <= 0 || region_height <= 0) return;

    for (int sy = 0; sy < region_height; sy++) {
        const int source_row = (source_y + sy) * sprite_width;
        for (int sx = 0; sx < region_width; sx++) {
            unsigned short colour = sprite[source_row + (source_x + sx)];
            if (colour == (unsigned short)transparent) {
                continue;
            }
            draw_pixel(dst_x + sx, dst_y + sy, colour);
        }
    }
}

void draw_sprite_any_region_silhouette(const unsigned short *sprite,
                                       int sprite_width, int sprite_height,
                                       int source_x, int source_y,
                                       int region_width, int region_height,
                                       int dst_x, int dst_y,
                                       short transparent,
                                       short silhouette_colour)
{
    if (!sprite || region_width <= 0 || region_height <= 0) return;

    if (source_x < 0) { dst_x -= source_x; region_width += source_x; source_x = 0; }
    if (source_y < 0) { dst_y -= source_y; region_height += source_y; source_y = 0; }
    if (source_x + region_width > sprite_width) region_width = sprite_width - source_x;
    if (source_y + region_height > sprite_height) region_height = sprite_height - source_y;
    if (region_width <= 0 || region_height <= 0) return;

    const unsigned short draw_colour = (unsigned short)silhouette_colour;

    for (int sy = 0; sy < region_height; sy++) {
        const int source_row = (source_y + sy) * sprite_width;
        for (int sx = 0; sx < region_width; sx++) {
            unsigned short colour = sprite[source_row + (source_x + sx)];
            if (colour == (unsigned short)transparent) {
                continue;
            }
            draw_pixel(dst_x + sx, dst_y + sy, draw_colour);
        }
    }
}

void draw_textbox_instant_text(const unsigned short *textBoxSprite, int x, int y, const char *string, short colour) {
    draw_sprite_any(textBoxSprite, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT, x, y, TRANSPARENT_COLOUR);
    draw_string(x + 30, y + 30, string, colour);
}

int draw_textbox_animated_text(const unsigned short *textBoxSprite, int x, int y, const char *string, short colour) {
    // text cursor variables
    static int cursor = 0;
    static int frameTimer = 0;
    static const char *lastString = 0;
    const int speedFrames = 4; // frames per character

    int len = (int)strlen(string);
    if (string == 0 || len <= 0) return 1;

    // Reset typing for different strings
    if (string != lastString) {
        cursor = 0;
        frameTimer = 0;
        lastString = string;
    }

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

    return cursor >= len;
}

void hide_textbox(int x, int y)
{
    // Restore the background map tiles underneath the textbox rectangle.
    const int x0 = x;
    const int y0 = y;
    const int x1 = x + TEXT_BOX_WIDTH - 1;
    const int y1 = y + TEXT_BOX_HEIGHT - 1;

    int leftTile   = x0 / TILE_SIZE;
    int rightTile  = x1 / TILE_SIZE;
    int topTile    = y0 / TILE_SIZE;
    int bottomTile = y1 / TILE_SIZE;

    if (leftTile < 0) leftTile = 0;
    if (topTile < 0) topTile = 0;
    if (rightTile >= MAP_WIDTH) rightTile = MAP_WIDTH - 1;
    if (bottomTile >= MAP_HEIGHT) bottomTile = MAP_HEIGHT - 1;

    for (int ty = topTile; ty <= bottomTile; ty++) {
        for (int tx = leftTile; tx <= rightTile; tx++) {
            drawTile(tx, ty, map[ty][tx]);
        }
    }
}

// Draw a single character using the currently active font.
void draw_char_f(int x, int y, char c, short int colour, FontId font) {
    unsigned char ch = (unsigned char)c;
    if (ch >= 128) return;

    if (font == FONT_5X9) {
        const uint8_t *glyph = font5x9_basic[ch];
        for (int row = 0; row < 9; row++) {
            uint8_t bits = glyph[row];
            for (int col = 0; col < 5; col++) {
                if (bits & (1 << (4 - col)))
                    draw_pixel(x + col, y + row, colour);
            }
        }
    } else {
        // FONT_8X8
        const unsigned char *glyph = font8x8_basic[ch];
        for (int row = 0; row < 8; row++) {
            unsigned char bits = glyph[row];
            for (int col = 0; col < 8; col++) {
                if (bits & (1 << col))
                    draw_pixel(x + col, y + row, colour);
            }
        }
    }
}

void draw_char(int x, int y, char c, short int colour) {
    draw_char_f(x, y, c, colour, current_font);
}

// Draw a string using an explicit font.
void draw_string_f(int x, int y, const char *string, short colour, FontId font) {
    int advance = (font == FONT_5X9) ? 6 : 8;  // char width + 1px gap
    int cursor_x = x;
    while (*string) {
        draw_char_f(cursor_x, y, *string, colour, font);
        cursor_x += advance;
        string++;
    }
}

void draw_string(int x, int y, const char *string, short colour) {
    draw_string_f(x, y, string, colour, current_font);
}