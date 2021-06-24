#include "main.h"

/**
 * main.c
 */

uint16_t todo_tasks;
uint8_t user_interrupts;

void configIO();

void init();

inline void keyboardISR();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    configTimer();
    configDebounce();
    configIO();
    configPS2();

    init();

    ENABLE_SWINT(); // Enable software interrupts
    __enable_interrupt();

    while (1)
    {
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

inline void keyboardISR()
{
    __no_operation(); // Just for breakpoint
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
        keyboardISR(); // Call keyboard interrupt service routine
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1()
{
    __bic_SR_register_on_exit(CPUOFF);
    DEBOUNCE_ISR_PORT1();
}

#pragma vector = PORT2_VECTOR
__interrupt void Port2()
{
    __bic_SR_register_on_exit(CPUOFF);

    if (BIT_ENABLED(PS2_SCL_IFG, PS2_SCL)) // SCL interrupt
    {
        isrPS2();
        CLR_IFG(PS2_SCL);
    }
}
