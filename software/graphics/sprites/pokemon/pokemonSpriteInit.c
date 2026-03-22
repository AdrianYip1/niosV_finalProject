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

static bool getPixelsById(int pokemon_id, bool want_front,
                          const unsigned short** out_pixels,
                          int* out_w,
                          int* out_h)
{
    if (!out_pixels || !out_w || !out_h) return false;

    const unsigned short* pixels = 0;
    int w = 0;
    int h = 0;

    switch (pokemon_id) {
        case POKEMON_ID_CHARMANDER:
            if (want_front) { pixels = charmanderFront; w = CHARMANDER_FRONT_WIDTH; h = CHARMANDER_FRONT_HEIGHT; }
            else            { pixels = charmanderBack;  w = CHARMANDER_BACK_WIDTH;  h = CHARMANDER_BACK_HEIGHT;  }
            break;
        case POKEMON_ID_CHARMELEON:
            if (want_front) { pixels = charmeleonFront; w = CHARMELEON_FRONT_WIDTH; h = CHARMELEON_FRONT_HEIGHT; }
            else            { pixels = charmeleonBack;  w = CHARMELEON_BACK_WIDTH;  h = CHARMELEON_BACK_HEIGHT;  }
            break;
        case POKEMON_ID_CHARIZARD:
            if (want_front) { pixels = charizardFront;  w = CHARIZARD_FRONT_WIDTH;  h = CHARIZARD_FRONT_HEIGHT;  }
            else            { pixels = charizardBack;   w = CHARIZARD_BACK_WIDTH;   h = CHARIZARD_BACK_HEIGHT;   }
            break;
        default:
            return false;
    }

    *out_pixels = pixels;
    *out_w = w;
    *out_h = h;
    return true;
}

static bool initPokemonBattleSprite(StaticSprite* out, int pokemon_id, bool want_front, int x, int y) {
    const unsigned short* pixels = 0;
    int w = 0;
    int h = 0;

    const bool ok = getPixelsById(pokemon_id, want_front, &pixels, &w, &h);
    initFromPixels(out, ok ? pixels : 0, ok ? w : 0, ok ? h : 0, x, y);
    return ok;
}

bool initPokemonBackBattleSprite(StaticSprite* out, int pokemon_id, int x, int y) {
    return initPokemonBattleSprite(out, pokemon_id, false, x, y);
}

bool initPokemonFrontBattleSprite(StaticSprite* out, int pokemon_id, int x, int y) {
    return initPokemonBattleSprite(out, pokemon_id, true, x, y);
}
