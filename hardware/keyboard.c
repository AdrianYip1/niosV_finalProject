#include "keyboard.h"
#include "../address_map.h"

// State of tracked keys
static bool w_pressed = false;
static bool a_pressed = false;
static bool s_pressed = false;
static bool d_pressed = false;
static bool space_pressed = false;
static bool shift_pressed = false;

static bool up_pressed = false;
static bool down_pressed = false;
static bool left_pressed = false;
static bool right_pressed = false;

// Internal state machine trackers
static bool is_break = false;
static bool is_extended = false;

void update_keyboard(void) {
    volatile int *PS2_ptr = (int *)PS2_BASE;
    int PS2_data, RVALID;
    
    // Read all available bytes in the PS/2 FIFO
    while (1) {
        PS2_data = *(PS2_ptr);
        RVALID = PS2_data & 0x8000;
        
        if (RVALID) {
            unsigned char code = PS2_data & 0xFF;
            
            if (code == 0xE0) {
                is_extended = true;
            } else if (code == 0xF0) {
                is_break = true;
            } else {
                bool state = !is_break; // true if make code, false if break code
                
                if (is_extended) {
                    switch (code) {
                        case 0x75: up_pressed = state; break;
                        case 0x72: down_pressed = state; break;
                        case 0x6B: left_pressed = state; break;
                        case 0x74: right_pressed = state; break;
                        default: break;
                    }
                    is_extended = false;
                } else {
                    switch (code) {
                        case 0x1D: w_pressed = state; break;
                        case 0x1C: a_pressed = state; break;
                        case 0x1B: s_pressed = state; break;
                        case 0x23: d_pressed = state; break;
                        case 0x12:
                        case 0x59: shift_pressed = state; break;
                        case 0x29: 
                            if (state) space_pressed = true; // Latch Make Code only
                            break;
                        default: break;
                    }
                }
                is_break = false;
            }
        } else {
            break; // No more data to read
        }
    }
}

// Getters
bool is_key_w_pressed(void) { return w_pressed; }
bool is_key_a_pressed(void) { return a_pressed; }
bool is_key_s_pressed(void) { return s_pressed; }
bool is_key_d_pressed(void) { return d_pressed; }
bool is_key_space_pressed(void) { 
    bool temp = space_pressed;
    space_pressed = false; // クリアして「読んだ」ことにする（Fast-Tap対策）
    return temp; 
}

bool is_key_shift_pressed(void) { return shift_pressed; }

bool is_key_up_pressed(void) { return up_pressed; }
bool is_key_down_pressed(void) { return down_pressed; }
bool is_key_left_pressed(void) { return left_pressed; }
bool is_key_right_pressed(void) { return right_pressed; }
