#include "audio.h"
#include "../address_map.h"
#include <stdlib.h>
#include <stdint.h>

#define AUDIO_CTRL_RE (1u << 0)
#define AUDIO_CTRL_WE (1u << 1)
#define AUDIO_CTRL_CR (1u << 2)
#define AUDIO_CTRL_CW (1u << 3)

#define NIOSV_AUDIO_IRQ_CAUSE 21u
#define MSTATUS_MIE (1u << 3)

static int noise_remaining = 0;
static const int16_t *bgm_ptr = NULL;
static int bgm_len = 0;
static int bgm_pos = 0;
static volatile int *const audio_ptr = (int *)AUDIO_BASE;

// audio interrupt
static inline unsigned int read_mcause(void) {
    unsigned int value;
    __asm__ volatile ("csrr %0, mcause" : "=r"(value));
    return value;
}

static inline void write_mtvec(void *handler) {
    __asm__ volatile ("csrw mtvec, %0" :: "r"(handler));
}

static inline void set_mie_bits(unsigned int mask) {
    __asm__ volatile ("csrs mie, %0" :: "r"(mask));
}

static inline void set_mstatus_bits(unsigned int mask) {
    __asm__ volatile ("csrs mstatus, %0" :: "r"(mask));
}

static inline void audio_write_control(unsigned int value) {
    *audio_ptr = (int)value;
}

static inline unsigned int audio_read_control(void) {
    return (unsigned int)(*audio_ptr);
}

static inline unsigned int audio_read_writable_control(void) {
    return audio_read_control() & 0xFu;
}

static inline int audio_sources_active(void) {
    return ((bgm_ptr != NULL && bgm_len > 0) || (noise_remaining > 0));
}


// audio interrupt controller
static void audio_enable_write_interrupt(void) {
    audio_write_control(audio_read_writable_control() | AUDIO_CTRL_WE);
}

static void audio_disable_write_interrupt(void) {
    audio_write_control(audio_read_writable_control() & ~AUDIO_CTRL_WE);
}

static void audio_kick_output(void) {
    if (!audio_sources_active()) return;
    audio_update();
    audio_enable_write_interrupt();
}

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

void __attribute__((interrupt("machine"))) audio_interrupt_handler(void) {
    const unsigned int mcause = read_mcause();
    const unsigned int is_interrupt = mcause >> 31;
    const unsigned int cause = mcause & 0x7FFFFFFFu;

    if (!is_interrupt || cause != NIOSV_AUDIO_IRQ_CAUSE) {
        return;
    }

    if (audio_sources_active()) {
        audio_update();
    } else {
        audio_disable_write_interrupt();
    }
}

// tap vector, IRQ
void audio_init_interrupts(void) {
    audio_write_control(AUDIO_CTRL_CW);
    audio_write_control(0);

    write_mtvec((void *)audio_interrupt_handler);
    set_mie_bits(1u << NIOSV_AUDIO_IRQ_CAUSE);
    set_mstatus_bits(MSTATUS_MIE);
}


void play_bgm(const int16_t *data, int length) {
    bgm_ptr = data;
    bgm_len = length;
    bgm_pos = 0;
    audio_kick_output();
}

void stop_bgm(void) {
    bgm_ptr = NULL;
}

void play_step_sound(void){
    noise_remaining = 500;
    audio_kick_output();
}

// Mix bgm and other SE
void audio_update(void){
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
