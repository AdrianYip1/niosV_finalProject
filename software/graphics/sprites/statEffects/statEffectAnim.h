#pragma once

// Moves the statUp/statDown sprite vertically while keeping X aligned .

typedef enum {
    STAT_EFFECT_UP = 0,
    STAT_EFFECT_DOWN = 1,
} StatEffectType;

typedef struct {
    int active;
    StatEffectType type;

    // Center position the effect should stay aligned to on X.
    int x_center;

    // Base Y (the effect moves up or down from this).
    int y_base;

    int frame;
} StatEffectAnim;

// Starts a new stat effect animation at the given anchor.
void statEffectStart(StatEffectAnim* anim, StatEffectType type, int x_center, int y_base);

// Advances the animation by 1 frame and draws it if active.
void statEffectTickDraw(StatEffectAnim* anim);

