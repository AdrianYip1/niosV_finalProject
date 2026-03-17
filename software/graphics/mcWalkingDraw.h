#ifndef MC_WALKING_DRAW_H
#define MC_WALKING_DRAW_H

typedef enum McFacing {
    MC_FACING_N = 0,
    MC_FACING_S,
    MC_FACING_W,
    MC_FACING_E,
    MC_FACING_NE,
    MC_FACING_NW,
    MC_FACING_SE,
    MC_FACING_SW,
} McFacing;

typedef struct McBounds {
    int x0;
    int y0;
    int x1;
    int y1;
    int valid;
} McBounds;

void drawMCWalkingAnimation(void);
void drawMCIdleAnimation(void);
void initMCIdle(void);
void drawMCAnimation(void);
McBounds getMCBounds(void);
void goUp(void);
void goDown(void);
void goLeft(void);
void goRight(void);
void initMCWalkingSprite(int startX, int startY, McFacing facing);
void initMCIdle(void);
void initMCWalkingEast(void);
void initMCWalkingNorth(void);
void initMCWalkingNorthEast(void);
void initMCWalkingNorthWest(void);
void initMCWalkingSouth(void);
void initMCWalkingSouthEast(void);
void initMCWalkingSouthWest(void);
void initMCWalkingWest(void);
#endif