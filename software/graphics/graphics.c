#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PIXEL_BASE 0xFF203020
#define pixel_buffer_start ((short int *)PIXEL_BASE)

void draw_pixel(int x, int y, short int colour) {
    volatile short int *one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
    *one_pixel_address = colour;
}

void clear_screen() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            draw_pixel(x, y, 0); //draws black pixel at every position
        }
    }
}

void wait_for_vsync() {
    volatile int* pixel_ctrl_ptr = (int*)0xFF203020;
    register int status;
    *pixel_ctrl_ptr = 1; //start synchronization

    status = *(pixel_ctrl_ptr + 3); //read status register
    while ((status & 0x01) != 0) { //polling loop for S bit to go to 0
        status = *(pixel_ctrl_ptr + 3);
    } //wait for status to be ready
}

void draw_hline(int x, int y, int width, short int colour){
    for (int position = x; position < x + widthl position++) {
        draw_pixel(position, y, colour); //draws horizontal line at y position
    }
}

void draw_vline(int x, int y, int height, short int colour) {
    for (int position = y; position < y + height; position++) {
        draw_pixel(x, position, colour); //draws vertical line at x position
    }
}

void draw_rect(int x, int y, int width, int height, short int colour) {
    for (int position = x; position < x + width; position++) {
        draw_pixel(position, y, colour); //draws top horizontal line
        draw_pixel(position, y + height - 1, colour); //draws bottom horizontal line
    }
    for (int position = y; position < y + height; position++) {
        draw_pixel(x, position, colour); //draws left vertical line
        draw_pixel(x + width - 1, position, colour); //draws right vertical line
    }
}

void draw_rect_outline(int x, int y, int width, int height, short int colour) {
    draw_hline(x, y, width, colour); //top line
    draw_hline(x, y + height - 1, width, colour); //bottom line
    draw_vline(x, y, height, colour); //left line
    draw_vline(x + width - 1, y, height, colour); //right line
}

void draw_circle(int cx, int cy, int radius, short int colour);
void draw_circle_filled(int cx, int cy, int radius, short int colour);
void draw_triangle(int x0,int y0,int x1,int y1,int x2,int y2, short int colour);









