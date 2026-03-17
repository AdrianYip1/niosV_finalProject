#pragma once
#include <stdbool.h>

#include "../../../graphics/mcWalkingDraw.h"

void mcMovingInit(int startX, int startY, McFacing facing);
void mcMovingTick(bool up, bool down, bool left, bool right);

