#include "main.h"

/**
 * main.c
 */

uint16_t todo_tasks;
uint8_t user_interrupts;
uint8_t col = 0;
uint8_t row = 0;
unsigned char character;

unsigned char *text = " ";

void configIO();

void init();

inline void keyboardISR();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
//
//    configTimer();
////    configDebounce();
////    configIO();
//    configPS2();
//
//    init();
//
//    ENABLE_SWINT(); // Enable software interrupts
//    __enable_interrupt();
//
////    lcdConstructor(LCD_I2C_ADDRESS, 16, 2); // initialize LCD library for LCD1602 with driver at
////    lcdInit();
////    lcdBacklight();

    while (1)
    {
//        if (TASK_ENABLED(TASK_PS2))
//        {
////            text[0] = character;
////            lcdSetCursor(col, row);
////            lcdPrint(text);
////            if (col == 15)
////            {
////                col = 0;
////                row ^= 1;
////            }
////            else
////                col++;
////            __no_operation(); // Just for breakpoint
//            TASK_DISABLE(TASK_PS2);
//    }
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

// User interrupts
#pragma vector = UNMI_VECTOR
__interrupt void unmi()
{
    if ((user_interrupts & KBINT) != 0)
    {
        BIT_DISABLE(SFRIFG1, NMIIFG);
        BIT_DISABLE(user_interrupts, KBINT);
        TASK_ENABLE(TASK_PS2);
    }
}

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
//    isrPS2();
//    CLR_IFG(PS2_SCL);
    }
}
