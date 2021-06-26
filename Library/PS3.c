/*
 * PS3.c
 *
 *  Created on: 23.05.2021
 *      Author: Philip Prohaska, Jakob Posselt
 */
#include "PS3.h"

// private:
uint8_t keyboard_status = 0;
struct PS2
{
    uint8_t status = 0;
    uint8_t mask;
    uint8_t data;
    uint8_t character;
} ps2;

void configPS2()
{
    DIR_INPUT(PS2_SCL);
    DIR_OUTPUT(PS2_SDA);
    IES_FALLING(PS2_SCL);
    PS2_SCL_IFG = 0;
    IE_ENABLE(PS2_SCL);
}

void ps2ISR()
{
    if (BIT_ENABLED(ps2.status, PS2_START))
    {
        // TODO: read in PS/2 scan code
        if (BIT_ENABLED(PS2_SDA_IN, PS2_SDA))
        {
            data |= mask;
            BIT_TOGGLE(ps2.status, PS2_PARITY_BIT); // Toggle odd parity bit
        }
        else
            data &= ~mask;
    }
    else if (BIT_ENABLED(ps2.status, PS2_PARITY_CHECK))
    {
        // TODO: check parity
        // The following state indicate corrupt data:
        if ((BIT_ENABLED(PS2_SDA_IN, PS2_SDA)
                && BIT_ENABLED(ps2.status, PS2_PARITY_BIT))
                || (BIT_DISABLED(PS2_SDA_IN, PS2_SDA)
                        && BIT_DISABLED(ps2.statix, PS2_PARITY_BIT)))
        {
        }
        else // The data was received correctly
        {
            getChar(ps2.data);
        }

        // TODO: if correct, get character
        // TODO: move character into buffer
    }
    else
    {
        BIT_DISABLE(ps2.status, PS2_PARITY_BIT); // Reset parity bit before receiving data.
        BIT_ENABLE(ps2.status, PS2_START); // This is the start bit. enable ps2 data read in now.
    }
}

unsigned char getChar(uint8_t code)
{
    switch (code)
    {
// --------- ROW0 ----------
    case 0x0E: // ^
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '^';
        else
            return '°';

    case 0x16: // 1
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '1';
        else
            return '!';

    case 0x1E: // 2
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '2';
        else
            return '"';

    case 0x26: // 3
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '3';
        else
            return '§';

    case 0x25: // 4
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '4';
        else
            return '$';

    case 0x2E: // 5
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '5';
        else
            return '%';

    case 0x36: // 6
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '6';
        else
            return '&';

    case 0x3D: // 7
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '7';
        else
            return '/';

    case 0x3E: // 8
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '8';
        else
            return '(';

    case 0x46: // 9
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '9';
        else
            return ')';

    case 0x45: // 0
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '0';
        else
            return '=';

    case 0x4E: // ß
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'ß';
        else
            return '?';

    case 0x55: // ´
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '´';
        else
            return '`';

        // ---------- ROW 1 ----------
    case 0x0D: // TAB
        return 0;

    case 0x15: // Q
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'Q';
        else
            return 'q';

    case 0x1D: // W
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'W';
        else
            return 'w';

    case 0x24: // E
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'E';
        else
            return 'e';

    case 0x2D: // R
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'R';
        else
            return 'r';

    case 0x2C: // T
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'T';
        else
            return 't';

    case 0x35: // Z
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'Z';
        else
            return 'z';

    case 0x3C: // U
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'U';
        else
            return 'u';

    case 0x43: // I
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'I';
        else
            return 'i';

    case 0x44: // O
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'O';
        else
            return 'o';

    case 0x4D: // P
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'P';
        else
            return 'p';

    case 0x54: // Ü
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'Ü';
        else
            return 'ü';

    case 0x5B: // +
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '*';
        else
            return '+';

    case 0x5A: // ENTER
        return 0;

        // ---------- ROW 2 ----------

    case 0x58: // CAPS
        if (BIT_ENABLED(keyboard_status, KB_RELEASED))
            BIT_DISABLE(keyboard_status, KB_RELEASED);
        else
            BIT_TOGGLE(keyboard_status, KB_CAPS);
        return 0;

    case 0x1C: // A
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'A';
        else
            return 'a';

    case 0x1B: // S
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'S';
        else
            return 's';

    case 0x23: // D
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'D';
        else
            return 'd';

    case 0x2B: // F
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'F';
        else
            return 'f';

    case 0x34: // G
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'G';
        else
            return 'g';

    case 0x33: // H
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'H';
        else
            return 'h';

    case 0x3B: // J
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'J';
        else
            return 'j';

    case 0x42: // K
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'K';
        else
            return 'k';

    case 0x4B: // L
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'L';
        else
            return 'l';

    case 0x4C: // Ö
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'Ö';
        else
            return 'ö';

    case 0x52: // Ä
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'Ä';
        else
            return 'ä';

    case 0x5D: // #
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '#';
        else
            return '\'';

        // --------- ROW 3 ----------

    case 0x12: // LSHIFT
        if (BIT_ENABLED(keyboard_status, KB_RELEASED))
            BIT_DISABLE(keyboard_status, KB_SHIFT | KB_RELEASED);
        else
            BIT_ENABLE(keyboard_status, KB_SHIFT);
        return 0;

    case 0x61: // <
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '<';
        else
            return '>';

    case 0x1A: // Y
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'Y';
        else
            return 'y';

    case 0x22: // X
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'X';
        else
            return 'x';

    case 0x21: // C
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'C';
        else
            return 'c';

    case 0x2A: // V
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'V';
        else
            return 'v';

    case 0x32: // B
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'B';
        else
            return 'b';

    case 0x31: // N
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'N';
        else
            return 'n';

    case 0x3A: // M
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return 'M';
        else
            return 'm';

    case 0x41: // ,
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return ',';
        else
            return ';';

    case 0x49: // .
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '.';
        else
            return ':';

    case 0x4A: // -
        if (BIT_ENABLED(keyboard_status, KB_SHIFT))
            return '-';
        else
            return '_';

    case 0x59: // RSHIFT
        if (BIT_ENABLED(keyboard_status, KB_RELEASED))
            BIT_DISABLE(keyboard_status, KB_SHIFT | KB_RELEASED);
        else
            BIT_ENABLE(keyboard_status, KB_SHIFT);
        return 0;

        // ---------- ROW 4 ----------

    case 0x14: // LCTRL | RCTRL
        if (BIT_ENABLED(keyboard_status, KB_RELEASED))
            BIT_DISABLE(keyboard_status, KB_CTRL | KB_RELEASED);
        else
            BIT_ENABLE(keyboard_status, KB_CTRL);
        return 0;

    case 0x1F: // LWIN
        if (BIT_ENABLED(
                keyboard_status,
                KB_SPECIAL) && BIT_ENABLED(keyboard_status, KB_RELEASED))
            BIT_DISABLE(keyboard_status, KB_WIN | KB_SPECIAL | KB_RELEASED);
        else if (BIT_ENABLED(keyboard_status, KB_SPECIAL))
            BIT_ENABLE(keyboard_status, KB_WIN);
        return 0;

    case 0x11: // ALT | ALTGR
        if (BIT_ENABLED(
                keyboard_status,
                KB_RELEASED) && BIT_ENABLED(keyboard_status, KB_SPECIAL))
        {
            BIT_DISABLE(keyboard_status,
                        KB_ALT | KB_CTRL | KB_SPECIAL | KB_RELEASED);
        }
        else if (BIT_ENABLED(keyboard_status, KB_SPECIAL))
        {
            BIT_ENABLE(keyboard_status, KB_ALT | KB_CTRL);
            BIT_DISABLE(keyboard_status, KB_SPECIAL);
        }
        else if (BIT_ENABLED(keyboard_status, KB_RELEASED))
        {
            BIT_DISABLE(keyboard_status, KB_ALT | KB_RELEASED);
        }
        else
        {
            BIT_ENABLE(keyboard_status, KB_ALT);
        }
        return 0;

    case 0x29: // SPACE
        return ' ';

    case 0x27: // RWIN
        if (BIT_ENABLED(
                keyboard_status,
                KB_SPECIAL) && BIT_ENABLED(keyboard_status, KB_RELEASED))
        {
            BIT_DISABLE(keyboard_status, KB_WIN | KB_SPECIAL);
        }
        else if (BIT_ENABLED(keyboard_status, KB_SPECIAL))
        {
            BIT_ENABLE(keyboard_status, KB_WIN);
            BIT_DISABLE(keyboard_status, KB_SPECIAL);
        }
        return 0;

    case 0x2F: // RIGHT CLICK
        return 0; // ignore this key

    default:
        return 0;
    }
}

