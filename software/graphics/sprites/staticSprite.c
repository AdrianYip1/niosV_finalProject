#include "staticSprite.h"

#include "../graphics.h"

void drawStaticSprite(const StaticSprite* sprite) {
    if (!sprite || !sprite->pixels) return;
    draw_sprite_any(sprite->pixels, sprite->width, sprite->height, sprite->x, sprite->y, TRANSPARENT_COLOUR);
}

