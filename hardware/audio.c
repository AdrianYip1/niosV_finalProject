#include "audio.h"
#include "../address_map.h"
#include <stdlib.h>
#include <stdint.h>

static int noise_remaining = 0;
static const int16_t *bgm_ptr = NULL;
static int bgm_len = 0;
static int bgm_pos = 0;


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

void audio_update_steps(void){
    int * audio_ptr = (int*) AUDIO_BASE;

    int fifospace = *(audio_ptr +1);

    int wsrc = (fifospace >> 16) & 0xff;
    int wslc = (fifospace >> 24) & 0xff;
    // we can only write the min space 
    int space = (wslc < wsrc) ? wslc : wsrc;


    for (int i = 0; i< space; i++){
        if(noise_remaining > 0){

            int max_amp = 0x03FFFFFF; // not too loud
            int noise = (rand() % max_amp) - (max_amp/2);
            long long amp = (long long) noise * noise_remaining/ 3000;
            *(audio_ptr + 2) = amp;
            *(audio_ptr + 3) = amp;      
            noise_remaining--; 
        } else {
            *(audio_ptr + 2) = 0;
            *(audio_ptr + 3) = 0;           
        }
    }
}

void audio_update_bgm(void){
    int * audio_ptr = (int*) AUDIO_BASE;

    int fifospace = *(audio_ptr +1);

    int wsrc = (fifospace >> 16) & 0xff;
    int wslc = (fifospace >> 24) & 0xff;
    // we can only write the min space 
    int space = (wslc < wsrc) ? wslc : wsrc;

    if (bgm_ptr == NULL || bgm_len <= 0) return;

    for (int i = 0; i < space; i++){
        int32_t sample = bgm_ptr[bgm_pos];
        bgm_pos++;
        if (bgm_pos >= bgm_len) {
            bgm_pos = 0; // Loop BGM
        }

        int32_t output = sample << 12; //amp
        *(audio_ptr + 2) = output;
        *(audio_ptr + 3) = output;
    }
}