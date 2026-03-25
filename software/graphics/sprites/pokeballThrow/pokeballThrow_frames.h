#pragma once

#define POKEBALLTHROW_WIDTH  16
#define POKEBALLTHROW_HEIGHT 23
#define POKEBALLTHROW_FRAME_COUNT 21

#define POKEBALLTHROW_THROW_FRAME_START 0
#define POKEBALLTHROW_THROW_FRAME_COUNT 8
#define POKEBALLTHROW_LAND_FRAME_START 8
#define POKEBALLTHROW_LAND_FRAME_COUNT 2
#define POKEBALLTHROW_CATCH_FRAME_START 10
#define POKEBALLTHROW_CATCH_FRAME_COUNT (POKEBALLTHROW_FRAME_COUNT - POKEBALLTHROW_CATCH_FRAME_START)

#define POKEBALLTHROW_SHAKE_FRAME_START 16
#define POKEBALLTHROW_SHAKE_FRAME_COUNT 3

#define POKEBALL_UNOPENED_FRAME_COUNT POKEBALLTHROW_THROW_FRAME_COUNT

extern const unsigned short pokeballThrow_frame_0[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_1[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_2[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_3[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_4[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_5[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_6[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_7[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_8[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_9[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_10[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_11[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_12[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_13[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_14[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_15[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_16[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_17[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_18[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_19[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];
extern const unsigned short pokeballThrow_frame_20[POKEBALLTHROW_WIDTH * POKEBALLTHROW_HEIGHT];

extern const unsigned short* const pokeballThrowFrames[POKEBALLTHROW_FRAME_COUNT];
