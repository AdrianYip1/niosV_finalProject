#include "../graphics.h"
#include "titleScreen_frames.h"

#define TITLE_SCREEN_FRAME_DELAY 2  // waits 2 vsyncs before going to the next frame

static int frameIndex;
static int frameTimer;

void initTitleScreen(void) {
    frameIndex = 0;
    frameTimer = 0;
}

static void drawTitleScreenFrame(int index) {
    if (index < 0 || index >= TITLE_SCREEN_FRAME_COUNT) {
        return;
    }

    const unsigned short* frame = titleScreenFrames[index];
    for (int y = 0; y < TITLE_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < TITLE_SCREEN_WIDTH; x++) {
            unsigned short colour = frame[y * TITLE_SCREEN_WIDTH + x];
            draw_pixel(x, y, colour);
        }
    }
}

void drawTitleScreen(void) {
    drawTitleScreenFrame(frameIndex);

    frameTimer++;
    if (frameTimer >= TITLE_SCREEN_FRAME_DELAY) {
        frameTimer = 0;
        frameIndex = (frameIndex + 1) % TITLE_SCREEN_FRAME_COUNT; //makes sure it loops back to 0 at the end of the array
    }
}
