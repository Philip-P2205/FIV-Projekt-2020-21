/*
 * PS2.h
 *
 *  Created on: 23.05.2021
 *      Author: Philip Prohaska, Jakob Posselt
 *
 *      This library reads in PS/2 keyboard signals and converts them into ASCII encoded characters.
 *      It uses a 64-byte keyboard buffer to store previous keystrokes. The size of the buffer may be altered by
 *      changing the PS2_BUFFER_SIZE value.
 *      The used ports are fully configurable, however the port for the SCL line has to be capable of using interrupts.
 *
 *      Usage:
 *      - The configPS2() function has to be called before usage in order for the ports to be initialized correctly.
 *      - The ps2ISR() function has to be called in the interrupt service routine of the SCL line.
 *        Making use of my util.h library this can be done in the following way:
 *          if (BIT_ENABLED(PS2_SCL_IFG, PS2_SCL)) // SCL interrupt
 *          {
 *              ps2ISR();
 *              CLR_IFG(PS2_SCL);
 *          }
 *      - The registered keystrokes will be stored in the globally accessible ps2.buffer keyboard buffer.
 *        The most recent keystrokes will always be first in the buffer as it is shifted back with every key pressed.
 *
 */

#ifndef LIBRARY_PS2_H_
#define LIBRARY_PS2_H_
#include <msp430.h>
#include <stdint.h>
#include "util.h"
#include "../main.h"

#define PS2_SDA_PORT    P2
#define PS2_SDA_PIN        BIT6
#define PS2_SCL_PORT    P2
#define PS2_SCL_PIN        BIT3

#define PS2_BUFFER_SIZE 64

// DO NOT TOUCH THE STUFF BELOW !

#define PS2_SCL_DIR       GET_DIR(PS2_SCL_PORT)
#define PS2_SDA_DIR      GET_DIR(PS2_SDA_PORT)
#define PS2_SCL_IN         GET_IN(PS2_SCL_PORT)
#define PS2_SDA_IN         GET_IN(PS2_SDA_PORT)

#define PS2_SCL_IES       GET_IES(PS2_SCL_PORT)
#define PS2_SDA_IES       GET_IES(PS2_SDA_PORT)
#define PS2_SCL_IE         GET_IE(PS2_SCL_PORT)
#define PS2_SDA_IE         GET_IE(PS2_SDA_PORT)
#define PS2_SCL_IFG       GET_IFG(PS2_SCL_PORT)
#define PS2_SDA_IFG      GET_IFG(PS2_SDA_PORT)

#define PS2_SDA              PS2_SDA_PIN
#define PS2_SCL              PS2_SCL_PIN

#define PS2_MASK                       0x01 // LSB first

// Keyboard status bits
#define KB_RELEASED    BIT0     // Pending key release
#define KB_SHIFT             BIT1     // Shift pressed
#define KB_CTRL             BIT2      // Control pressed
#define KB_ALT                BIT3      // Alt pressed
#define KB_WIN                BIT4      // Windows pressed
#define KB_CAPS             BIT5      // Caps active
#define KB_SPECIAL        BIT6       // Pending "special" scan code

// public
extern void configPS2();
extern void ps2ISR();
extern unsigned char getChar(uint8_t code);

typedef struct
{
    uint16_t mask;
    uint16_t scancode;
    unsigned char character;
    unsigned char buffer[PS2_BUFFER_SIZE]; // 64-byte keyboard buffer
} PS2;
extern PS2 ps2;

#endif /* LIBRARY_PS2_H_ */
