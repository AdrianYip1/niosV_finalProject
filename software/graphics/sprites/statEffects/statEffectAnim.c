#include "statEffectAnim.h"

#include "../../graphics.h"
#include "../../predefined_colours.h"
#include "statDown.h"
#include "statUp.h"

#define STAT_EFFECT_FRAMES 16
#define STAT_EFFECT_PX_PER_FRAME 2

void statEffectStart(StatEffectAnim* anim, StatEffectType type, int x_center, int y_base) {
    if (!anim) return;
    anim->active = 1;
    anim->type = type;
    anim->x_center = x_center;
    anim->y_base = y_base;
    anim->frame = 0;
}

void statEffectTickDraw(StatEffectAnim* anim) {
    if (!anim || !anim->active) return;

    const int offset = anim->frame * STAT_EFFECT_PX_PER_FRAME;
    const int dy = (anim->type == STAT_EFFECT_UP) ? -offset : offset;
    const short colour = (anim->type == STAT_EFFECT_UP) ? GREEN : RED;
    const int silhouette_frame = anim->frame % SILHOUETTE_SPRITE_FRAME_COUNT;

    if (anim->type == STAT_EFFECT_UP) {
        const int x = anim->x_center - (STATUP_WIDTH / 2);
        const int y = anim->y_base + dy;
        draw_sprite_any_silhouette(statUp,
                                   STATUP_WIDTH, STATUP_HEIGHT,
                                   x, y,
                                   TRANSPARENT_COLOUR,
                                   colour,
                                   silhouette_frame);
    } else {
        const int x = anim->x_center - (STATDOWN_WIDTH / 2);
        const int y = anim->y_base + dy;
        draw_sprite_any_silhouette(statDown,
                                   STATDOWN_WIDTH, STATDOWN_HEIGHT,
                                   x, y,
                                   TRANSPARENT_COLOUR,
                                   colour,
                                   silhouette_frame);
    }

    anim->frame++;
    if (anim->frame >= STAT_EFFECT_FRAMES) {
        anim->active = 0;
    }
}
