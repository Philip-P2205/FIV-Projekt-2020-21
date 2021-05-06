#include "main.h"
#include "Library/lcd.h"
#include "Library/binconst.h"
/**
 * main.c
 */

uint16_t todo_tasks;

void configIO();

void init();

void debounceTaskEnable();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    configTimer();
    configDebounce();
    configIO();

    init();

    __enable_interrupt();

    lcdConfig();
    lcdInit();

    lcdSendChar(B8(01001000));

    while (1)
    {
//        if (TASK_ENABLED(TASK_DEBOUNCE))
//        {
//            debounce();
//            TASK_DISABLE(TASK_DEBOUNCE);
//        }

//        TIE_TO_STATE(P1OUT, BIT0, BIT_ENABLED(debouncerPort1.toggles, BIT1));
        BIT_TOGGLE(P1OUT, BIT0);
        delayMs(500);
    }
}

void configIO()
{
    P1DIR |= BIT0;
}

void init()
{
//    timerA0.ccr0 = &debounceTaskEnable;
}

void debounceTaskEnable()
{
    TASK_ENABLE(TASK_DEBOUNCE);
}

/* Interrupts */

#pragma vector = PORT1_VECTOR
__interrupt void Port_1()
{
    __bic_SR_register_on_exit(CPUOFF);
    DEBOUNCE_ISR_PORT1();
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2()
{
    __bic_SR_register_on_exit(CPUOFF);
    DEBOUNCE_ISR_PORT2();
}

