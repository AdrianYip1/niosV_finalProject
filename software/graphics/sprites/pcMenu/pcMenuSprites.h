#pragma once

#include <stdint.h>

#define PC_MENU_SPRITE_COUNT  7

typedef enum {
    PC_MENU_SPRITE_LEFT_ARROW = 0,
    PC_MENU_SPRITE_PARTY_BOX = 1,
    PC_MENU_SPRITE_PC_BOX_BACKGROUND = 2,
    PC_MENU_SPRITE_PC_BOX_BLUE = 3,
    PC_MENU_SPRITE_PC_LABEL = 4,
    PC_MENU_SPRITE_RIGHT_ARROW = 5,
    PC_MENU_SPRITE_SELECT_CURSOR = 6,
} PcMenuSpriteId;

#define PC_MENU_LEFT_ARROW_WIDTH  16
#define PC_MENU_LEFT_ARROW_HEIGHT 32
extern const unsigned short leftArrowSprite[PC_MENU_LEFT_ARROW_WIDTH * PC_MENU_LEFT_ARROW_HEIGHT];

#define PC_MENU_PARTY_BOX_WIDTH  105
#define PC_MENU_PARTY_BOX_HEIGHT 152
extern const unsigned short partyBoxSprite[PC_MENU_PARTY_BOX_WIDTH * PC_MENU_PARTY_BOX_HEIGHT];

#define PC_MENU_PC_BOX_BACKGROUND_WIDTH  220
#define PC_MENU_PC_BOX_BACKGROUND_HEIGHT 210
extern const unsigned short pcBoxBackgroundSprite[PC_MENU_PC_BOX_BACKGROUND_WIDTH * PC_MENU_PC_BOX_BACKGROUND_HEIGHT];

#define PC_MENU_PC_BOX_BLUE_WIDTH  189
#define PC_MENU_PC_BOX_BLUE_HEIGHT 149
extern const unsigned short pcBoxBlueSprite[PC_MENU_PC_BOX_BLUE_WIDTH * PC_MENU_PC_BOX_BLUE_HEIGHT];

#define PC_MENU_PC_LABEL_WIDTH  178
#define PC_MENU_PC_LABEL_HEIGHT 24
extern const unsigned short pcLabelSprite[PC_MENU_PC_LABEL_WIDTH * PC_MENU_PC_LABEL_HEIGHT];

#define PC_MENU_RIGHT_ARROW_WIDTH  16
#define PC_MENU_RIGHT_ARROW_HEIGHT 32
extern const unsigned short rightArrowSprite[PC_MENU_RIGHT_ARROW_WIDTH * PC_MENU_RIGHT_ARROW_HEIGHT];

#define PC_MENU_SELECT_CURSOR_WIDTH  32
#define PC_MENU_SELECT_CURSOR_HEIGHT 32
extern const unsigned short selectCursorSprite[PC_MENU_SELECT_CURSOR_WIDTH * PC_MENU_SELECT_CURSOR_HEIGHT];

extern const unsigned short* const pcMenuSprites[PC_MENU_SPRITE_COUNT];
extern const uint16_t pcMenuSpriteWidths[PC_MENU_SPRITE_COUNT];
extern const uint16_t pcMenuSpriteHeights[PC_MENU_SPRITE_COUNT];
