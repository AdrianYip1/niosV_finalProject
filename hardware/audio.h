#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

void play_step_sound(void);
void play_bgm(const int16_t *data, int length);
void stop_bgm(void);

void audio_update_bgm(void);
void audio_update_steps(void);

#endif