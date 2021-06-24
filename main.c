#include "main.h"
#include "Library/lcd.h"
#include "Library/binconst.h"
/**
 * main.c
 */

uint16_t todo_tasks;
uint8_t data;

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

    configLCD();

    data = 'H';
//    initLCD();

//    lcd_buffer = B16(0000000, 11000000);
    lcd_buffer = 0xaaaa;
    __sendBuffer();
    while (1)
    {
//        sendChar(data);
//        delayMs(500);
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

