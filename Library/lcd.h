/*
 * lcd.h
 *
 *  Created on: 08.04.2021
 *      Author: Philip Prohaska
 */

#ifndef LIBRARY_LCD_H_
#define LIBRARY_LCD_H_

#include "util.h"
#include <msp430.h>
#include <stdint.h>

#define LCD_RS_PORT         P8
#define LCD_RS_PIN          BIT1
#define LCD_RW_PORT         P2
#define LCD_RW_PIN          BIT3
#define LCD_E_PORT          P2
#define LCD_E_PIN           BIT6

#define LCD_D_SER_PORT      P3
#define LCD_D_SER_PIN       BIT1
#define LCD_D_SRCLK_PORT    P7
#define LCD_D_SRCLK_PIN     BIT4
#define LCD_D_RCLK_PORT     P3
#define LCD_D_RCLK_PIN      BIT0

#define LCD_RS_DIR          GET_DIR(LCD_RS_PORT)
#define LCD_RW_DIR          GET_DIR(LCD_RW_PORT)
#define LCD_E_DIR           GET_DIR(LCD_E_PORT)
#define LCD_D_SER_DIR       GET_DIR(LCD_D_SER_PORT)
#define LCD_D_SRCLK_DIR     GET_DIR(LCD_D_SRCLK_PORT)
#define LCD_D_RCLK_DIR      GET_DIR(LCD_D_RCLK_PORT)

#define LCD_RS_OUT          GET_OUT(LCD_RS_PORT)
#define LCD_RW_OUT          GET_OUT(LCD_RW_PORT)
#define LCD_E_OUT           GET_OUT(LCD_E_PORT)
#define LCD_D_SER_OUT       GET_OUT(LCD_D_SER_PORT)
#define LCD_D_SRCLK_OUT     GET_OUT(LCD_D_SRCLK_PORT)
#define LCD_D_RCLK_OUT      GET_OUT(LCD_D_RCLK_PORT)

#define LCD_RS              LCD_RS_PIN
#define LCD_RW              LCD_RW_PIN
#define LCD_E               LCD_E
#define LCD_D_SER           LCD_D_SER_PIN
#define LCD_D_SRCLK         LCD_D_SRCLK_PIN
#define LCD_D_RCLK          LCD_D_RCLK_PIN

extern void configLCD();
extern void initLCD();

extern void sendData(uint8_t data);
extern void sendCmd();

#endif /* LIBRARY_LCD_H_ */
