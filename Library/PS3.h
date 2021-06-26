/*
 * ps3.h
 *
 *  Created on: 23.05.2021
 *      Author: Philip Prohaska, Jakob Posselt
 */

#ifndef LIBRARY_PS3_H_
#define LIBRARY_PS3_H_
#include <msp430.h>
#include <stdint.h>
#include "util.h"

#define PS2_SDA_PORT    P2
#define PS2_SDA_PIN        BIT6
#define PS2_SCL_PORT    P2
#define PS2_SCL_PIN        BIT3

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

// PS2 status bits
#define PS2_START                      BIT0    // Start bit came. Impending data read in.
#define PS2_PARITY_CHECK       BIT1    // Impending parity check with next bit read in
#define PS2_PARITY_BIT              BIT2   // (Odd) Parity bit for checking parity. Set (in PS2-protocol) if number of ones is even.

// Keyboard status bits
#define KB_RELEASED    BIT0     // Pending key release
#define KB_SHIFT             BIT1     // Shift pressed
#define KB_CTRL             BIT2      // Control pressed
#define KB_ALT                BIT3      // Alt pressed
#define KB_WIN                BIT4      // Windows pressed
#define KB_CAPS             BIT5      // Caps active
#define KB_SPECIAL       BIT6       // Pending "special" scancode

// public
extern void configPS2();
extern void ps2ISR();
extern unsigned char getChar(uint8_t code);

#endif /* LIBRARY_PS3_H_ */
