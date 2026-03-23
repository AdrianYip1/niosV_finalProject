#pragma once

#define BATTLE_PARTY_WIDTH  160
#define BATTLE_PARTY_HEIGHT 91

extern const unsigned short battlePartySpriteDefault[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];
extern const unsigned short battlePartySlot1Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];
extern const unsigned short battlePartySlot2Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];
extern const unsigned short battlePartySlot3Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];
extern const unsigned short battlePartySlot4Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];
extern const unsigned short battlePartySlot5Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];
extern const unsigned short battlePartySlot6Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];

extern const unsigned short* const battlePartySlotSprites[7];


#define battlePartySprite battlePartySlot1Sprite
