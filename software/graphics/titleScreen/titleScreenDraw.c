#include "../graphics.h"
#include "titleScreen_frames.h"
#include "titleScreenDraw.h"

static int frameIndex;
static int frameTimer;

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

void initTitleScreen(void) {
    frameIndex = 0;
    frameTimer = 0;
}

void drawTitleScreen(void) {
    frameTimer++;
    if (frameTimer >= GLOBAL_ANIM_FRAME_DELAY) {
        frameTimer = 0;
        frameIndex = (frameIndex + 1) % TITLE_SCREEN_FRAME_COUNT;
    }

    drawTitleScreenFrame(frameIndex);
}

void drawTitleScreenCurrentFrame(void) {
    drawTitleScreenFrame(frameIndex);
}

int getTitleFrameIndex(void) {
    return frameIndex;
}
