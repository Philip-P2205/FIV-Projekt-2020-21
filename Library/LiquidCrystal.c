/*
 * LiquidCrystal.c
 *
 *  Created on: 26.06.2021
 *      Author: Philip Prohaska, Jakob Posselt
 */
#include "LiquidCrystal.h"
// private:
void lcdInit_priv();
void lcdSend(uint8_t, uint8_t);
void lcdWrite4bits(uint8_t);
void lcdExpanderWrite(uint8_t);
void lcdPulseEnable(uint8_t);

uint8_t _Addr;
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _numlines;
uint8_t _cols;
uint8_t _rows;
uint8_t _backlightval;
uint8_t _data_i2c, _has_data_to_send;

void lcdWrite(uint8_t value)
{
    lcdSend(value, Rs);
}

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

/**
 * LiquidCrystal_I2C constructor
 */
void lcdConstructor(uint8_t addr, uint8_t cols, uint8_t rows)
{
    _Addr = addr;
    _cols = cols;
    _rows = rows;
    _backlightval = LCD_NOBACKLIGHT;
}

/**
 * Only initialize the LCD if interrupts are enabled!
 */
void lcdInit()
{
    lcdInit_priv();
}

void lcdInit_priv()
{
    // TODO: Wire.begin(); // Initializes the TWI (I2C) code, change to configI2C
    configLCD();
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    lcdBegin(_cols, _rows, LCD_5x8DOTS);
}

void lcdBegin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1)
        _displayfunction |= LCD_2LINE;
    _numlines = lines;

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != 0) && (lines == 1))
    {
        _displayfunction |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
//    delayMs(50);

    __delay_cycles(400000 / 32768); // at 8 MHz
    // Now we pull both RS and R/W low to begin commands
    lcdExpanderWrite(_backlightval); // reset expander and turn backlight off (Bit 8 =1)
    // delayMs(1000);

    __delay_cycles(8000000 / 32768); // at 8 MHz

    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    lcdWrite4bits(0x03 << 4);
//    delayMs(5); // wait min 4.1ms

    __delay_cycles(40000 / 32768); // at 8 MHz
    // second try
    lcdWrite4bits(0x03 << 4);
//    delayMs(5); // wait min 4.1ms

    __delay_cycles(40000 / 32768); // at 8 MHz

    // third go!
    lcdWrite4bits(0x03 << 4);
    //delayMs(1); //150us

    __delay_cycles(1); // at 8 MHz

    // finally, set to 4-bit interface
    lcdWrite4bits(0x02 << 4);

    // set # lines, font size, etc.
    lcdCommand(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcdDisplay();

    // clear it off
    lcdClear();

    // Initialize to default text direction (for roman languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    lcdCommand(LCD_ENTRYMODESET | _displaymode);

    lcdHome();
}

/********** high level commands, for the user! */
void lcdClear()
{
    lcdCommand(LCD_CLEARDISPLAY);
//    delayMs(2); // this command takes a long time !

    __delay_cycles(16000 / 32768); // at 8 MHz
}

void lcdHome()
{
    lcdCommand(LCD_RETURNHOME);
//    delayMs(2); // this command takes a long time !

    __delay_cycles(16000 / 32768); // at 8 MHz
}

void lcdSetCursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row > _numlines)
    {
        row = _numlines - 1; // we count rows starting w/0
    }
    lcdCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void lcdNoDisplay()
{
    _displaycontrol &= ~ LCD_DISPLAYON;
    lcdCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}
void lcdDisplay()
{
    _displaycontrol |= LCD_DISPLAYON;
    lcdCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void lcdNoCursor()
{
    _displaycontrol &= ~LCD_CURSORON;
    lcdCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}
void lcdCursor()
{
    _displaycontrol |= LCD_DISPLAYON;
    lcdCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void lcdNoBlink()
{
    _displaycontrol &= ~LCD_BLINKON;
    lcdCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}
void lcdBlink()
{
    _displaycontrol |= LCD_BLINKON;
    lcdCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void lcdScrollDisplayLeft()
{
    lcdCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
// This is for text that flows Right to Left
void lcdScrollDisplayRight()
{
    lcdCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcdLeftToRight()
{
    _displaymode |= LCD_ENTRYLEFT;
    lcdCommand(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void lcdRightToLeft()
{
    _displaymode &= ~ LCD_ENTRYLEFT;
    lcdCommand(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void lcdAutoscroll()
{
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    lcdCommand(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void lcdNoAutoscroll()
{
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    lcdCommand(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcdCreateChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x07; // we only have 8 locations (0-7)
    lcdCommand(LCD_SETCGRAMADDR | (location << 3));
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        lcdWrite(charmap[i]);
    }
}

// Turn the (optional) backlight off/on
void lcdNoBacklight()
{
    _backlightval = LCD_NOBACKLIGHT;
    lcdExpanderWrite(0);
}
void lcdBacklight()
{
    _backlightval = LCD_BACKLIGHT;
    lcdExpanderWrite(0);
}

/*********** mid level commands, for sending data/cmds */
inline void lcdCommand(uint8_t value)
{
    lcdSend(value, 0);
}

/************ low level data pushing commands **********/

// write either command or data
void lcdSend(uint8_t value, uint8_t mode)
{
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    lcdWrite4bits((highnib) | mode);
    lcdWrite4bits((lownib) | mode);
}

void lcdWrite4bits(uint8_t value)
{
    lcdExpanderWrite(value);
    lcdPulseEnable(value);
}

void lcdExpanderWrite(uint8_t data)
{
    // --- Wire.beginTransmission(_Addr); // sets the address for transmission
    UCB0I2CSA = _Addr; // Set the I2C address of the LCD

    // --- Wire.send((int)(_data) | _backlightval); // Sends the data
    _data_i2c = data | _backlightval;
    _has_data_to_send = 1;
    // Ensure stop condition got sent
    while (UCB0CTL1 & UCTXSTP)
        ;
    UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition

    // --- Wire.endTransmission(); // Ends the transmission
    // Handled by I2C-ISR (USCI_B0_ISR)

}

void lcdPulseEnable(uint8_t _data)
{
    lcdExpanderWrite(_data | En); // En high

    __delay_cycles(5000); // En pulse must be >450ns
    lcdExpanderWrite(_data & ~En);

    __delay_cycles(50000); // commands need > 37us to settle
}

void lcdPrint(const unsigned char str[])
{
    if (str == NULL)
        return;
    uint8_t size = strlen(str);
    while (size--)
    {
        lcdWrite(*str++);
    }
}

void configI2C()
{
    /*
     * DISCLAIMER:
     * Code (slightly modified) from
     *
     * MSP430F552x Demo - USCI_B0 I2C Master TX single bytes to MSP430 Slave
     * Bhargavi Nisarga
     * Texas Instruments Inc.
     * April 2009
     */
    P3REN |= 0x03;
    P3OUT |= 0x03;
    P3SEL |= 0x03;                            // Assign I2C pins to USCI_B0
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
    UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0I2CSA = _Addr;                         // Slave Address
    UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
    UCB0IE |= UCTXIE;                         // Enable TX interrupt
}

// Alias functions

void lcdCursorOn()
{
    lcdCursor();
}
void lcdCursorOff()
{
    lcdNoCursor();
}

void lcdBlinkOn()
{
    lcdBlink();
}
void lcdBlinkOff()
{
    lcdNoBlink();
}

void lcdLoadCustomCharacter(uint8_t charNum, uint8_t *rows)
{
    lcdCreateChar(charNum, rows);
}

void lcdSetBacklight(uint8_t newVal)
{
    if (newVal)
        lcdBacklight(); // turn backlight on
    else
        lcdNoBacklight(); // turn backlight off
}

void lcdPrintstr(const unsigned char c[])
{
    //This function is not identical to the function used for "real" I2C displays
    //it's here so the user sketch doesn't have to be changed
    lcdPrint(c);
}

void configLCD()
{
    configI2C();
}
/**
 * I2C interrupt service routine
 */
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch (__even_in_range(UCB0IV, 12))
    {
    case 0:    // Vector  0: No interrupts
        break;
    case 2:    // Vector  2: ALIFG
        break;
    case 4:    // Vector  4: NACKIFG
        break;
    case 6:    // Vector  6: STTIFG
        break;
    case 8: // Vector  8: STPIFG
        break;
    case 10: // Vector 10: RXIFG
        break;
    case 12: // Vector 12: TXIFG
        if (_has_data_to_send)
        {
            UCB0TXBUF = _data_i2c;   // Load TX buffer
            _has_data_to_send = 0;
        }
        else
        {
            UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
            UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
            __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
        }
        break;
    default:
        break;
    }
}
