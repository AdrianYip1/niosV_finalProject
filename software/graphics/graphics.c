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










