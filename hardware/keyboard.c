#include "keyboard.h"
#include "../address_map.h"

// State of tracked keys
static bool w_pressed = false;
static bool a_pressed = false;
static bool s_pressed = false;
static bool d_pressed = false;
static bool space_pressed = false;

//for uppercase letters
static bool shift_pressed = false;

static bool up_pressed = false;
static bool down_pressed = false;
static bool left_pressed = false;
static bool right_pressed = false;

// Internal state machine trackers
static bool is_break = false;
static bool is_extended = false;


#define CHAR_QUEUE_SIZE 64
static char char_queue[CHAR_QUEUE_SIZE];
static int char_head = 0;
static int char_tail = 0;

static void push_char(char c) {
    int next_tail = (char_tail + 1) % CHAR_QUEUE_SIZE;
    if (next_tail == char_head) return;
    char_queue[char_tail] = c;
    char_tail = next_tail;
}

bool keyboard_pop_char(char *out) {
    if (char_head == char_tail) return false;
    *out = char_queue[char_head];
    char_head = (char_head + 1) % CHAR_QUEUE_SIZE;
    return true;
}

bool is_key_enter_pressed(void) {
    char c;

    if (!keyboard_pop_char(&c)) return false;
    if (c == '\n') return true;

    return false;
}

static char scan_to_char(unsigned char code) {
    const char base_upper = 'A';
    const char base_lower = 'a';

    bool upper = shift_pressed;

    switch (code) {
        case 0x1C: return upper ? 'A' : 'a'; // A
        case 0x32: return upper ? 'B' : 'b'; // B
        case 0x21: return upper ? 'C' : 'c'; // C
        case 0x23: return upper ? 'D' : 'd'; // D
        case 0x24: return upper ? 'E' : 'e'; // E
        case 0x2B: return upper ? 'F' : 'f'; // F
        case 0x34: return upper ? 'G' : 'g'; // G
        case 0x33: return upper ? 'H' : 'h'; // H
        case 0x43: return upper ? 'I' : 'i'; // I
        case 0x3B: return upper ? 'J' : 'j'; // J
        case 0x42: return upper ? 'K' : 'k'; // K
        case 0x4B: return upper ? 'L' : 'l'; // L
        case 0x3A: return upper ? 'M' : 'm'; // M
        case 0x31: return upper ? 'N' : 'n'; // N
        case 0x44: return upper ? 'O' : 'o'; // O
        case 0x4D: return upper ? 'P' : 'p'; // P
        case 0x15: return upper ? 'Q' : 'q'; // Q
        case 0x2D: return upper ? 'R' : 'r'; // R
        case 0x1B: return upper ? 'S' : 's'; // S
        case 0x2C: return upper ? 'T' : 't'; // T
        case 0x3C: return upper ? 'U' : 'u'; // U
        case 0x2A: return upper ? 'V' : 'v'; // V
        case 0x1D: return upper ? 'W' : 'w'; // W
        case 0x22: return upper ? 'X' : 'x'; // X
        case 0x35: return upper ? 'Y' : 'y'; // Y
        case 0x1A: return upper ? 'Z' : 'z'; // Z

        // digits
        case 0x16: return '1';
        case 0x1E: return '2';
        case 0x26: return '3';
        case 0x25: return '4';
        case 0x2E: return '5';
        case 0x36: return '6';
        case 0x3D: return '7';
        case 0x3E: return '8';
        case 0x46: return '9';
        case 0x45: return '0';

        // space
        case 0x29: return ' ';

        // backspace (use '\b' as a marker)
        case 0x66: return '\b';

        // enter
        case 0x5A: return '\n';

        default: return '\0';
    }
}

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
                        case 0x59: shift_pressed = state; break; // right shift
                        default: break;
                    }
                    is_extended = false;
                } else {
                    switch (code) {
                        case 0x1D: w_pressed = state; break;
                        case 0x1C: a_pressed = state; break;
                        case 0x1B: s_pressed = state; break;
                        case 0x23: d_pressed = state; break;
                        case 0x29: 
                            if (state) space_pressed = true; // Latch Make Code only
                            break;
                        case 0x12: // Left Shift
                            shift_pressed = state;
                            break;
                        default: break;
                    }

                    // If character key and a MAKE code, enqueue it.
                    if (state) {
                        char c = scan_to_char(code);
                        if (c != '\0') push_char(c);
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

bool is_key_up_pressed(void) { return up_pressed; }
bool is_key_down_pressed(void) { return down_pressed; }
bool is_key_left_pressed(void) { return left_pressed; }
bool is_key_right_pressed(void) { return right_pressed; }
