typedef struct {
    const unsigned short* const* frames; 
    int frameCount;
    int tileSize;

    int x;
    int y;

    int frameIndex;
    int frameTimer;
    int frameDelay;
} Sprite;