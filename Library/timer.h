/*
 * timer.h
 *
 *  Created on: 01.04.2021
 *      Author: Philip Prohaska
 */
#include <msp430.h>
#include <stdint.h>
#include "util.h"

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

//#define CLOCK_8MHz

#ifndef CLOCK_8MHz
#define CLOCK_25MHz
#endif

#define TIMER_POWER_MODE LPM1_bits

#define TIMER_ENABLE_TA0
#define TIMER_FREQUENCY_A0_CCR0 200
#define TIMER_FREQUENCY_A0_CCR1 1000
//#define TIMER_FREQUENCY_A0_CCR2 4
//#define TIMER_FREQUENCY_A0_CCR3 8
//#define TIMER_FREQUENCY_A0_CCR4 16

//#define TIMER_ENABLE_TA1
#define TIMER_FREQUENCY_A1_CCR0 1
#define TIMER_FREQUENCY_A1_CCR1 2
#define TIMER_FREQUENCY_A1_CCR2 4

//#define TIMER_UPDATE_DISABLE_TA0CCR1
//#define TIMER_UPDATE_DISABLE_TA0CCR2
//#define TIMER_UPDATE_DISABLE_TA0CCR3
//#define TIMER_UPDATE_DISABLE_TA0CCR4

//#define TIMER_UPDATE_DISABLE_TA1CCR1
//#define TIMER_UPDATE_DISABLE_TA1CCR2

#define TIMER_FREQUENCY(f) (32768 / (f))
#define __TIMER_FUNCTION_TYPE(name) void (*name)()

#ifdef TIMER_ENABLE_TA0
struct TimerA0
{
#if TIMER_FREQUENCY_A0_CCR0
    __TIMER_FUNCTION_TYPE(ccr0);
#endif
#if TIMER_FREQUENCY_A0_CCR1
    __TIMER_FUNCTION_TYPE(ccr1);
#endif
#if TIMER_FREQUENCY_A0_CCR2
    __TIMER_FUNCTION_TYPE(ccr2);
#endif
#if TIMER_FREQUENCY_A0_CCR3
    __TIMER_FUNCTION_TYPE(ccr3);
#endif
#if TIMER_FREQUENCY_A0_CCR4
    __TIMER_FUNCTION_TYPE(ccr4);
#endif
} timerA0;
#endif

#ifdef TIMER_ENABLE_TA1
struct TimerA1
{
#if TIMER_FREQUENCY_A1_CCR0
    __TIMER_FUNCTION_TYPE(ccr0)
#endif
#if TIMER_FREQUENCY_A1_CCR1
    __TIMER_FUNCTION_TYPE(ccr1)
#endif
#if TIMER_FREQUENCY_A1_CCR2
    __TIMER_FUNCTION_TYPE(ccr2)
#endif
} timerA1;
#endif

extern void _null();
extern void configClock();
extern void configTimer();
extern inline void delayMs(uint16_t);
extern void setVcore(uint16_t);

#endif /* TIMER_TIMER_H_ */
