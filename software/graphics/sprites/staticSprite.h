#pragma once

typedef struct {
    const unsigned short* pixels;
    int width;
    int height;
    int x;
    int y;
} StaticSprite;

void drawStaticSprite(const StaticSprite* sprite);

