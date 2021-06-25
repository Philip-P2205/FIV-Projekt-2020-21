/*
 * LiquidCrystal.h
 *
 *  Created on: 24.06.2021
 *      Author: Philip Prohaska, Jakob Posselt
 *
 *      This library is an adaptation for the MSP430F5529 of the LiquidCrystal_I2C library
 *      for the Arduino microcontroller (https://github.com/marcoschwartz/LiquidCrystal_I2C).
 *      It uses pins P3.0 and P3.1 in the I2C mode for communicating with the LCD.
 *
 *      It works with the PCF8574T I2C IO expander on a break-out-board developed for the LCD1602 family.
 *      The expander break-out-board is directly attached to the LCD.
 *      The boards pins are connected in the following way:
 *
 *      - GND to GND
 *      - VCC to +5V
 *      - SDA to P3.0
 *      - SCL to P3.1
 *
 *      DISCLAIMER: This library ONLY works with the 8MHz clock setting!
 *                             This is due to the fact that the LCD is not fast enough to react to a speed beyond that.
 */

#ifndef LIQUIDCRYSTAL_H_
#define LIQUIDCRYSTAL_H_

#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "binconst.h"

#define LCD_HAS_DATA_TO_SEND 0x01
#define LCD_HAS_SENT_DATA 0x02

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B8(00000100)  // Enable bit
#define Rw B8(00000010)  // Read/Write bit
#define Rs B8(00000001)  // Register select bit

// public:

extern void lcdConstructor(uint8_t addr, uint8_t cols, uint8_t rows);
// charsize = LCD_5x8DOTS
extern void lcdBegin(uint8_t cols, uint8_t rows, uint8_t charsize);
extern void lcdClear();
extern void lcdHome();
extern void lcdNoDisplay();
extern void lcdDisplay();
extern void lcdNoBlink();
extern void lcdBlink();
extern void lcdNoCursor();
extern void lcdCursor();
extern void lcdScrollDisplayLeft();
extern void lcdScrollDisplayRight();
extern void lcdPrintLeft();
extern void lcdPrintRight();
extern void lcdLeftToRight();
extern void lcdRightToLeft();
extern void lcdShiftIncrement();
extern void lcdShiftDecrement();
extern void lcdNoBacklight();
extern void lcdBacklight();
extern void lcdAutoscroll();
extern void lcdNoAutoscroll();
extern void lcdCreateChar(uint8_t, uint8_t[]);
extern void lcdSetCursor(uint8_t, uint8_t);

extern void lcdWrite(uint8_t);
extern void lcdCommand(uint8_t);
extern void lcdInit();
extern void lcdPrint(const unsigned char[]);

extern void configI2C();

////compatibility API function aliases
extern void lcdBlinkOn();                                  // alias for blink()
extern void lcdBlinkOff();                               // alias for noBlink()
extern void lcdCursorOn();                                // alias for cursor()
extern void lcdCursorOff();                             // alias for noCursor()
extern void lcdSetBacklight(uint8_t newVal); // alias for backlight() and nobacklight()
extern void lcdLoadCustomCharacter(uint8_t charNum, uint8_t *rows); // alias for createChar()
extern void lcdPrintstr(const unsigned char[]);
extern void configLCD();        // alias for configI2C()

#endif /* LIQUIDCRYSTAL_H_ */
