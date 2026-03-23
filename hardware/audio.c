#include "audio.h"
#include "../address_map.h"
#include <stdlib.h>
#include <stdint.h>

static int noise_remaining = 0;
static const uint8_t *bgm_ptr = NULL;
static int bgm_len = 0;
static int bgm_pos = 0;


void play_bgm(const uint8_t *data, int length) {
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