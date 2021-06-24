/*
 * ps2.h
 *
 *  Created on: 23.06.2021
 *      Author: Philip Prohaska
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

#define KB_RELEASED    BIT0
#define KB_SHIFT             BIT1
#define KB_CTRL             BIT2
#define KB_ALT                BIT3
#define KB_WIN                BIT4

extern unsigned char character;
extern uint8_t keyboard_status;

extern void configPS2();
extern inline void isrPS2();

#endif /* LIBRARY_PS2_H_ */
