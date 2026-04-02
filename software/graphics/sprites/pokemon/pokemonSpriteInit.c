#include "pokemonSpriteInit.h"

#include "pokemonSprites.h"

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
        case POKEMON_ID_GIBLE:
            if (want_front) { pixels = gibleFront;      w = GIBLE_FRONT_WIDTH;      h = GIBLE_FRONT_HEIGHT;      }
            else            { pixels = gibleBack;       w = GIBLE_BACK_WIDTH;       h = GIBLE_BACK_HEIGHT;       }
            break;
        case POKEMON_ID_GABITE:
            if (want_front) { pixels = gabiteFront;     w = GABITE_FRONT_WIDTH;     h = GABITE_FRONT_HEIGHT;     }
            else            { pixels = gabiteBack;      w = GABITE_BACK_WIDTH;      h = GABITE_BACK_HEIGHT;      }
            break;
        case POKEMON_ID_GARCHOMP:
            if (want_front) { pixels = garchompFront;   w = GARCHOMP_FRONT_WIDTH;   h = GARCHOMP_FRONT_HEIGHT;   }
            else            { pixels = garchompBack;    w = GARCHOMP_BACK_WIDTH;    h = GARCHOMP_BACK_HEIGHT;    }
            break;
        case POKEMON_ID_FEEBAS:
            if (want_front) { pixels = feebasFront;     w = FEEBAS_FRONT_WIDTH;     h = FEEBAS_FRONT_HEIGHT;     }
            else            { pixels = feebasBack;      w = FEEBAS_BACK_WIDTH;      h = FEEBAS_BACK_HEIGHT;      }
            break;
        case POKEMON_ID_MILOTIC:
            if (want_front) { pixels = miloticFront;    w = MILOTIC_FRONT_WIDTH;    h = MILOTIC_FRONT_HEIGHT;    }
            else            { pixels = miloticBack;     w = MILOTIC_BACK_WIDTH;     h = MILOTIC_BACK_HEIGHT;     }
            break;
        case POKEMON_ID_RIOLU:
            if (want_front) { pixels = rioluFront;      w = RIOLU_FRONT_WIDTH;      h = RIOLU_FRONT_HEIGHT;      }
            else            { pixels = rioluBack;       w = RIOLU_BACK_WIDTH;       h = RIOLU_BACK_HEIGHT;       }
            break;
        case POKEMON_ID_LUCARIO:
            if (want_front) { pixels = lucarioFront;    w = LUCARIO_FRONT_WIDTH;    h = LUCARIO_FRONT_HEIGHT;    }
            else            { pixels = lucarioBack;     w = LUCARIO_BACK_WIDTH;     h = LUCARIO_BACK_HEIGHT;     }
            break;
        case POKEMON_ID_BUDEW:
            if (want_front) { pixels = budewFront;      w = BUDEW_FRONT_WIDTH;      h = BUDEW_FRONT_HEIGHT;      }
            else            { pixels = budewBack;       w = BUDEW_BACK_WIDTH;       h = BUDEW_BACK_HEIGHT;       }
            break;
        case POKEMON_ID_ROSERADE:
            if (want_front) { pixels = roseradeFront;   w = ROSERADE_FRONT_WIDTH;   h = ROSERADE_FRONT_HEIGHT;   }
            else            { pixels = roseradeBack;    w = ROSERADE_BACK_WIDTH;    h = ROSERADE_BACK_HEIGHT;    }
            break;
        case POKEMON_ID_TOGEPI:
            if (want_front) { pixels = togepiFront;     w = TOGEPI_FRONT_WIDTH;     h = TOGEPI_FRONT_HEIGHT;     }
            else            { pixels = togepiBack;      w = TOGEPI_BACK_WIDTH;      h = TOGEPI_BACK_HEIGHT;      }
            break;
        case POKEMON_ID_TOGETIC:
            if (want_front) { pixels = togeticFront;    w = TOGETIC_FRONT_WIDTH;    h = TOGETIC_FRONT_HEIGHT;    }
            else            { pixels = togeticBack;     w = TOGETIC_BACK_WIDTH;     h = TOGETIC_BACK_HEIGHT;     }
            break;
        case POKEMON_ID_TOGEKISS:
            if (want_front) { pixels = togekissFront;   w = TOGEKISS_FRONT_WIDTH;   h = TOGEKISS_FRONT_HEIGHT;   }
            else            { pixels = togekissBack;    w = TOGEKISS_BACK_WIDTH;    h = TOGEKISS_BACK_HEIGHT;    }
            break;
        case POKEMON_ID_SPIRITOMB:
            if (want_front) { pixels = spiritombFront;  w = SPIRITOMB_FRONT_WIDTH;  h = SPIRITOMB_FRONT_HEIGHT;  }
            else            { pixels = spiritombBack;   w = SPIRITOMB_BACK_WIDTH;   h = SPIRITOMB_BACK_HEIGHT;   }
            break;
        case POKEMON_ID_RAYQUAZA:
            if (want_front) { pixels = rayquazaFront;   w = RAYQUAZA_FRONT_WIDTH;   h = RAYQUAZA_FRONT_HEIGHT;   }
            else            { pixels = rayquazaBack;    w = RAYQUAZA_BACK_WIDTH;    h = RAYQUAZA_BACK_HEIGHT;    }
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

bool setPokemonBattleSpriteId(StaticSprite* sprite, int pokemon_id, bool want_front) {
    if (!sprite) return false;

    const unsigned short* pixels = 0;
    int w = 0;
    int h = 0;

    const bool ok = getPixelsById(pokemon_id, want_front, &pixels, &w, &h);
    sprite->pixels = ok ? pixels : 0;
    sprite->width  = ok ? w : 0;
    sprite->height = ok ? h : 0;
    return ok;
}
