#pragma once
#include "spriteStruct.h"

extern Sprite playerSprite;

void initCharizardBackSprite(void);
void drawSpriteAnimation(void);
void drawSpriteCurrentFrameOnly(void);
int getSpriteFrameIndex(void);