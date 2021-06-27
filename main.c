#include "main.h"

/**
 * main.c
 */

uint16_t todo_tasks;
unsigned char text[16];

void configIO();

void init();

inline void keyboardISR();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
                              //
    configTimer();
    ////    configDebounce();
    ////    configIO();
    //
    //    init();
    //
    //    ENABLE_SWINT(); // Enable software interrupts
    __enable_interrupt();
    //
    lcdConstructor(LCD_I2C_ADDRESS, 16, 2); // initialize LCD library for LCD1602 with I2C driver
    lcdInit();
    lcdBacklight();

    configPS2();

    TASK_ENABLE(TASK_PS2);
    while (1)
    {
        if (TASK_ENABLED(TASK_PS2))
        {
            lcdSetCursor(0, 0);
            lcdPrint("Text from kb:");
            lcdSetCursor(0, 1);

            // Reverse String
            uint8_t length = strlen(ps2.buffer) - 1;
            uint8_t i = length + 1;
            while (i--)
            {
                text[i] = ps2.buffer[(uint8_t) (length - i)];
            }

            lcdPrint(text);

            __no_operation(); // Just for breakpoint
            TASK_DISABLE(TASK_PS2);
        }
        __bic_SR_register(LPM0_bits);
    }
}

void configIO()
{
    // P1DIR |= BIT0;
}

void init()
{
    //    timerA0.ccr0 = &debounceTaskEnable;
}

/* Interrupts */

//// User interrupts
//#pragma vector = UNMI_VECTOR
//__interrupt void unmi()
//{
//    if ((user_interrupts & KBINT) != 0)
//    {
//        BIT_DISABLE(SFRIFG1, NMIIFG);
//        BIT_DISABLE(user_interrupts, KBINT);
//        TASK_ENABLE(TASK_PS2);
//    }
//}
#pragma vector = PORT1_VECTOR
__interrupt void Port_1()
{
    __bic_SR_register_on_exit(CPUOFF);
}

#pragma vector = PORT2_VECTOR
__interrupt void Port2()
{
    __bic_SR_register_on_exit(CPUOFF);

    if (BIT_ENABLED(PS2_SCL_IFG, PS2_SCL)) // SCL interrupt
    {
        ps2ISR();
        CLR_IFG(PS2_SCL);
    }
}
