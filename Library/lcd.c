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
    OUT_HIGH(LCD_D_RCLK)
    OUT_HIGH(LCD_D_SRCLK);
    OUT_HIGH(LCD_D_SER);
}

void sendData(uint8_t data)
{
    uint8_t i;
    OUT_LOW(LCD_D_RCLK);
    for (i = 8; i > 0; --i)
    {
        OUT_LOW(LCD_D_SRCLK);
        if ((data & (1 << i)) != 0)
            OUT_HIGH(LCD_D_SER);
        else
            OUT_LOW(LCD_D_SER);
        OUT_HIGH(LCD_D_SRCLK);
    }
    OUT_HIGH(LCD_D_RCLK);
}

