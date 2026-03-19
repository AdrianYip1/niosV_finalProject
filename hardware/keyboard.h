#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

void update_keyboard(void);

bool is_key_w_pressed(void);
bool is_key_a_pressed(void);
bool is_key_s_pressed(void);
bool is_key_d_pressed(void);
bool is_key_space_pressed(void);
bool is_key_shift_pressed(void);

bool is_key_up_pressed(void);
bool is_key_down_pressed(void);
bool is_key_left_pressed(void);
bool is_key_right_pressed(void);

#endif 
