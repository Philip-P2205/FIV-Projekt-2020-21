/*
 * ps2.h
 *
 *  Created on: 27.05.2021
 *      Author: jakob
 */

#ifndef LIBRARY_PS2_H_
#define LIBRARY_PS2_H_

#include <stdint.h>

#define PS2_SHIFT_PRESSED_STATE 0x01

extern uint8_t ps2_states;
extern uint8_t *ps2_map;
extern uint8_t *ps2_map_shifted;
extern uint8_t ps2_buffer[256];

extern uint8_t ps2KeyboardChar(uint8_t scancode);


#endif /* LIBRARY_PS2_H_ */
