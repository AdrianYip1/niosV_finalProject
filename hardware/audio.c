#include "audio.h"
#include "../address_map.h"
#include <stdlib.h>
#include <stdint.h>

static int noise_remaining = 0;
static const int16_t *bgm_ptr = NULL;
static int bgm_len = 0;
static int bgm_pos = 0;

static int32_t clamp_audio_sample(int64_t sample) {
    const int32_t max_amp = 0x7FFFFFF;
    const int32_t min_amp = -0x8000000;

    if (sample > max_amp) return max_amp;
    if (sample < min_amp) return min_amp;
    return (int32_t)sample;
}

static int32_t next_bgm_sample(void) {
    if (bgm_ptr == NULL || bgm_len <= 0) {
        return 0;
    }

    {
        const int32_t sample = ((int32_t)bgm_ptr[bgm_pos]) << 12;
        bgm_pos++;
        if (bgm_pos >= bgm_len) {
            bgm_pos = 0;
        }
        return sample;
    }
}

static int32_t next_step_sample(void) {
    if (noise_remaining <= 0) {
        return 0;
    }

    {
        const int32_t max_amp = 0x03FFFFFF;
        const int32_t noise = (rand() % max_amp) - (max_amp / 2);
        const int32_t sample = (int32_t)(((int64_t)noise * noise_remaining) / 3000);
        noise_remaining--;
        return sample;
    }
}


void play_bgm(const int16_t *data, int length) {
    bgm_ptr = data;
    bgm_len = length;
    bgm_pos = 0;
}

void stop_bgm(void) {
    bgm_ptr = NULL;
}

void play_step_sound(void){
    noise_remaining = 500;
}

void audio_update(void){
    int * audio_ptr = (int*) AUDIO_BASE;
    const int fifospace = *(audio_ptr + 1);
    const int wsrc = (fifospace >> 16) & 0xff;
    const int wslc = (fifospace >> 24) & 0xff;
    const int space = (wslc < wsrc) ? wslc : wsrc;

    for (int i = 0; i < space; i++) {
        const int64_t mixed = (int64_t)next_bgm_sample() + (int64_t)next_step_sample();
        const int32_t output = clamp_audio_sample(mixed);
        *(audio_ptr + 2) = output;
        *(audio_ptr + 3) = output;
    }
}
