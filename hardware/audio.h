#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

void audio_init_interrupts(void);
void play_step_sound(void);
void play_sfx(const int16_t *data, int length);
void play_bgm(const int16_t *data, int length);
void stop_bgm(void);

void audio_update(void);

#endif
