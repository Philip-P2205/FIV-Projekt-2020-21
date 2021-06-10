/*
 * lcd.c
 *
 *  Created on: 08.04.2021
 *      Author: Philip Prohaska
 */
#include "lcd.h"

void configLCD()
{
    DIR_OUTPUT(LCD_RS);
    DIR_OUTPUT(LCD_RW);
    DIR_OUTPUT(LCD_E);
    DIR_OUTPUT(LCD_D_SER);
    DIR_OUTPUT(LCD_D_SRCLK);
    DIR_OUTPUT(LCD_D_RCLK);

    OUT_HIGH(LCD_RS);
    OUT_HIGH(LCD_RW);
    OUT_HIGH(LCD_E);
    OUT_HIGH(LCD_D_RCLK);
    OUT_HIGH(LCD_D_SRCLK);
    OUT_HIGH(LCD_D_SER);
}

void initLCD()
{
    sendCmd(0x30); //B8(00110000) // 8-bit mode, 1-line display, 5x8 dot font
    sendCmd(0x0E); //B8(00001110)); // Display and cursor on
    sendCmd(0x06); //B8(00000110)); // mode to increment, display not shifted
}

inline void toggleEnable()
{
    OUT_HIGH(LCD_E);
    OUT_LOW(LCD_E);
    delayMs(1);
    OUT_HIGH(LCD_E);
}

void sendCmd(uint8_t data)
{
    OUT_LOW(LCD_RS);
    OUT_LOW(LCD_RW);
    sendData(data);
    toggleEnable();
}

/**
 * Sends data to the 74hc595n shift register,
 * that is connected to the data pins D0...D7
 */
void sendData(uint8_t data)
{
    uint8_t i;
    OUT_LOW(LCD_D_RCLK);
    for (i = 8; i > 0; i--)
    {
        OUT_LOW(LCD_D_SRCLK);
        if ((data & (1 << (i - 1))) != 0)
            OUT_HIGH(LCD_D_SER);
        else
            OUT_LOW(LCD_D_SER);
        OUT_HIGH(LCD_D_SRCLK);
    }
    OUT_HIGH(LCD_D_RCLK);
}

void sendChar(uint8_t data)
{
    OUT_HIGH(LCD_RS);
    OUT_LOW(LCD_RW);
    sendData(data);
    toggleEnable();
}

