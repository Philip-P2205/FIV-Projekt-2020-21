/*
 * ps2.c
 *
 *  Created on: 23.06.2021
 *      Author: Philip Prohaska
 */
#include "ps2.h"

static const unsigned char *keymap =
        "\0\0\0\0\0\0\0\0\0\0\0\0\0 `\0\0\0\0\0\0q1\0\0\0ysaw2\0\0cxde43\0\0 vftr5\0\0nbhgz6\0\0\0mju78\0\0,kio09\0\0.-l\xF6p\xDF-\0\0\0'\xE4[\xFC\xD4\0\0\0\",+0#,\"]<\\\0\0\0\0\0\0\0\0\0\0\01\047\0\0\00.2568\",$1b,\"\0\0+3-*9\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
static const unsigned char *keymap_shifted =
        "\0\0\0\0\0\0\0\0\0\0\0\0\0 ~\0\0\0\0\0\0Q!\0\0\0YSAW\"\0\0CXDE$§\0\0 VFTR%\0\0NBHGZ&\0\0\0MJU/(\0\0;KIO=)\0\0:_L:P?\0\0\0\"\0{+\0\0\0\0\0;*=';\0\0>\0\0\0\0\0\01\047\0\0\00.2568\0\0\0+3-*9\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

unsigned char buffer[64]; // 64 character buffer
unsigned char character;
uint8_t keyboard_status = 0;

void configPS2()
{
    DIR_INPUT(PS2_SCL);
    DIR_INPUT(PS2_SDA);
    IES_FALLING(PS2_SCL);
    PS2_SCL_IFG = 0;
    IE_ENABLE(PS2_SCL);
}

inline void isrPS2()
{
    static uint8_t mask = 0x03; // Initial state for 'skipping' start condition
    static uint8_t data;
//    static uint8_t parity = 0;
    // start condition, init mask
    if (mask == 0x03)
        mask = 0x01;
    else
    {
        // data read-in
        // D+ high
        if (BIT_ENABLED(PS2_SDA_IN, PS2_SDA))
            data |= mask;
        // D+ low
        else
            data &= ~mask;

        mask <<= 1; // Shift mask (LSB-first)

        // data has been received
        if (mask == 0)
        {
            mask = 0x03; // reset mask

            //TODO: parity check

//            if (BIT_ENABLED(PS2_SDA_IN, PS2_SDA)) // parity bit is 1
//            {
//                if (BIT_DISABLED(parity, BIT0)) // odd parity ?
//                {
//                    data = 0; // discard data
//                    return;
//                }
//            }
//            else if (BIT_ENABLED(parity, BIT0)) // even parity ?
//            {
//                data = 0; // discard data
//                return;
//            }

//            if ((BIT_ENABLED(PS2_SDA_IN, PS2_SDA) && BIT_DISABLED(parity, BIT0))
//                    || (BIT_DISABLED(PS2_SDA_IN, PS2_SDA)
//                            && BIT_ENABLED(parity, BIT0)))
//            {
//                data = 0; // discard data
//                return;
//            }

            switch (data)
            {
            case 0xF0: // Key release code
                BIT_ENABLE(keyboard_status, KB_RELEASED); // Activate release status
                break;

            case 0x12: // Shift L
            case 0x59: // Shift R
//                if (BIT_ENABLED(keyboard_status, KB_RELEASED))
//                    BIT_DISABLE(keyboard_status, KB_SHIFT);
//                else
//                    BIT_DISABLE(keyboard_status, KB_SHIFT);
                BIT_TOGGLE(keyboard_status, KB_SHIFT); // Toggle shift for now, as holding is not working
                __delay_cycles(50000);
                break;

                // Discard the following scan codes
            case 0xE0:
            case 0xE3:
            case 0x49:
            case 0x04:
            case 0x1F:
            case 0xC6:
            case 0x8F:
                break;

            default:
                if (BIT_DISABLED(keyboard_status, KB_RELEASED)) // No key release
                {
                    if (BIT_ENABLED(keyboard_status, KB_SHIFT))
                        character = keymap_shifted[data];
                    else
                        character = keymap[data];

                    // Shift buffer
                    uint8_t i;
                    for (i = 63; i > 0; --i)
                        buffer[i] = buffer[(uint8_t) (i - 1)];

                    buffer[0] = character;
                    buffer[32] = data;
                    data = 0;
                    BIT_DISABLE(keyboard_status, KB_SHIFT);

                    user_interrupts |= KBINT;
                    SFRIFG1 |= NMIIFG; // Cause software interrupt
                }
                else // key released
                {
                    BIT_DISABLE(keyboard_status, KB_RELEASED);
                }
                break;
            }
        }
    }
}

