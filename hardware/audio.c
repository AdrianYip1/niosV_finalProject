#include "audio.h"
#include "../address_map.h"
#include <stdlib.h>
#include <stdint.h>

static int noise_remaining = 0;
static const uint8_t *bgm_ptr = NULL;
static int bgm_len = 0;
static int bgm_pos = 0;

void play_step_sound(void) {
    noise_remaining = 500; // ~60ms @ 48kHz
}

void play_bgm(const uint8_t *data, int length) {
    bgm_ptr = data;
    bgm_len = length;
    bgm_pos = 0;
}

void stop_bgm(void) {
    bgm_ptr = NULL;
}

void audio_update(void) {
    volatile int * audio_ptr = (int *) AUDIO_BASE;
    int fifospace = *(audio_ptr + 1);
    
    // Extract available space for Left (wslc) and Right (wsrc)
    int wsrc = (fifospace >> 16) & 0xFF;
    int wslc = (fifospace >> 24) & 0xFF;
    int space = (wslc < wsrc) ? wslc : wsrc;
    
    for (int i = 0; i < space; i++) {
        int final_sample = 0;

        // Footstep logic (White noise)
        if (noise_remaining > 0) {
            int max_amp = 0x00FFFFFF; 
            int noise = (rand() % max_amp) - (max_amp / 2);
            final_sample += (int)((long long)noise * noise_remaining / 3000);
            noise_remaining--;
        }

        // BGM logic (8-bit Mono @ 48000Hz)
        if (bgm_ptr != NULL && bgm_pos < bgm_len) {
            // Convert 8-bit unsigned (0-255) to signed and scale up to 32-bit range
            int sample8 = bgm_ptr[bgm_pos];
            int sample_signed = (sample8 - 128) << 20; 
            final_sample += sample_signed;
            
            // Audio core is 48kHz, BGM is now 48kHz. Increment each sample.
            bgm_pos++;
            if (bgm_pos >= bgm_len) bgm_pos = 0; // Loop BGM
        }
        
        *(audio_ptr + 2) = final_sample; // Left
        *(audio_ptr + 3) = final_sample; // Right
    }
}