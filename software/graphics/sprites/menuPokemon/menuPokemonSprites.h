#pragma once

#include "../pokemon/pokemonSpriteInit.h"

#define MENU_POKEMON_SPRITE_WIDTH  40
#define MENU_POKEMON_SPRITE_HEIGHT 30
#define MENU_POKEMON_SPRITE_COUNT  10

extern const unsigned short menuCharmanderSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuCharmeleonSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuCharizardSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuRayquazaSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuGarchompSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuLucarioSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuMiloticSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuRoseradeSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuSpiritombSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];
extern const unsigned short menuTogekissSprite[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];

extern const unsigned short* const menuPokemonSpriteById[POKEMON_ID_TOGEKISS + 1];

static inline const unsigned short* menuPokemonSpriteForId(int pokemon_id) {
    return (pokemon_id >= 0 && pokemon_id <= POKEMON_ID_TOGEKISS) ? menuPokemonSpriteById[pokemon_id] : 0;
}
