#include "pokemonSpriteInit.h"

#include "charizardSprite.h"
#include "charmanderSprite.h"
#include "charmeleonSprite.h"

static void initFromPixels(StaticSprite* out, const unsigned short* pixels, int w, int h, int x, int y) {
    if (!out) return;
    out->pixels = pixels;
    out->width = w;
    out->height = h;
    out->x = x;
    out->y = y;
}

void initCharizardBackBattleSprite(StaticSprite* out, int x, int y) {
    initFromPixels(out, charizardBack, CHARIZARD_BACK_WIDTH, CHARIZARD_BACK_HEIGHT, x, y);
}

void initCharizardFrontBattleSprite(StaticSprite* out, int x, int y) {
    initFromPixels(out, charizardFront, CHARIZARD_FRONT_WIDTH, CHARIZARD_FRONT_HEIGHT, x, y);
}

void initCharmanderBackBattleSprite(StaticSprite* out, int x, int y) {
    initFromPixels(out, charmanderBack, CHARMANDER_BACK_WIDTH, CHARMANDER_BACK_HEIGHT, x, y);
}

void initCharmanderFrontBattleSprite(StaticSprite* out, int x, int y) {
    initFromPixels(out, charmanderFront, CHARMANDER_FRONT_WIDTH, CHARMANDER_FRONT_HEIGHT, x, y);
}

void initCharmeleonBackBattleSprite(StaticSprite* out, int x, int y) {
    initFromPixels(out, charmeleonBack, CHARMELEON_BACK_WIDTH, CHARMELEON_BACK_HEIGHT, x, y);
}

void initCharmeleonFrontBattleSprite(StaticSprite* out, int x, int y) {
    initFromPixels(out, charmeleonFront, CHARMELEON_FRONT_WIDTH, CHARMELEON_FRONT_HEIGHT, x, y);
}

