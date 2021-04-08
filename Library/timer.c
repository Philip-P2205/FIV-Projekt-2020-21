/*
 * timer.c
 *
 *  Created on: 01.04.2021
 *      Author: Philip Prohaska
 */
#include "timer.h"

#define __TIMER_UPDATE(n, c) {          \
    TA##n##CCR##c += TIMER_FREQUENCY(TIMER_FREQUENCY_A##n##_CCR##c); \
    if (TA##n##CCR##c > TA##n##CCR0)    \
        TA##n##CCR##c -= TA##n##CCR0;   \
}
#define __CONFIG_TIMER(n, c) {          \
    TA##n##CCTL##c = CCIE;              \
    TA##n##CCR##c = TIMER_FREQUENCY(TIMER_FREQUENCY_A##n##_CCR##c); \
    timerA##n.ccr##c = &_null;         \
}

static uint16_t __ms = 0;

void _null()
{
}

void setVcore(uint16_t level)
{
    PMMCTL0_H = PMMPW_H;           // Open PMM registers for write

    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level; // Set SVS/SVM high side new level
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level; // Set SVM low side to new level

    while ((PMMIFG & SVSMLDLYIFG) == 0)
        ;  // Wait till SVM is settled

    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);    // Clear already set flags
    PMMCTL0_L = PMMCOREV0 * level;          // Set VCore to new level

    if ((PMMIFG & SVMLIFG))               // Wait till new level reached
        while ((PMMIFG & SVMLVLRIFG) == 0)
            ;

    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level; // Set SVS/SVM low side to new level

    PMMCTL0_H = 0x00;                     // Lock PMM registers for write access
}

void configClock()
{
    //clock config for MSP430F5529

    P5SEL |= BIT4 + BIT5;                         // port select XT1
    UCSCTL6 &= ~(XT1OFF + XCAP_3);         // XT1 On, clear internal load caps
    UCSCTL6 |= XCAP_0; // internal load caps 2pF, 6pF, 9pF or 12pF could be selected, XCAP_0 => 2pF

    do                                   // loop until XT1 fault flag is cleared
    {
        UCSCTL7 &= ~XT1LFOFFG;                      // clear XT1 fault flags
    }
    while (UCSCTL7 & XT1LFOFFG);                   // test XT1 fault flag

    UCSCTL6 &= ~(XT1DRIVE_3);  // reduce XT1driver strength to the lowest level

    __bis_SR_register(SCG0);                     // disable the FLL control loop

#ifdef CLOCK_25MHz
    setVcore(0x01);                      // Increase Vcore setting to level 3...
    setVcore(0x02);                               // ...to support fsystem=25MHz
    setVcore(0x03);             // NOTE: Change core voltage one level at a time

    UCSCTL0 = 0x0000;                          // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_7;                          // select DCO range 50MHz
    UCSCTL2 = FLLD_0 + 762;                       // 32768Hz * (762 + 1) = 25MHz
#endif

#ifdef CLOCK_8MHz
    UCSCTL1 = DCORSEL_3;                       // select DCO range 1MHz to 10MHz
    UCSCTL2 = FLLD_1 + 243;                       // 32768Hz * (243 + 1) = 8MHz
#endif

    UCSCTL3 = SELREF_0 + FLLREFDIV_0; // Set DCO FLL reference = XT1, FLL reference divider 1
    UCSCTL4 = SELA_0 + SELS_3 + SELM_3; // select clock sources for ACLK, MCLK and SMCLK
    UCSCTL5 = DIVPA_0 + DIVA_0 + DIVS_0 + DIVM_0; // select clock dividers

    __bic_SR_register(SCG0);                      // enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.

#ifdef CLOCK_8MHz
    __delay_cycles(250000); // 32 x 32 x 8 MHz / 32768 Hz = 250000 = MCLK cycles for DCO to settle
#endif

#ifdef CLOCK_25MHz
    __delay_cycles(782000); // 32 x 32 x 25 MHz / 32768 Hz = 782000 = MCLK cycles for DCO to settle
#endif
}

void configTimer()
{
    configClock();
// Timer A0
#ifdef TIMER_ENABLE_TA0
#if TIMER_FREQUENCY_A0_CCR0
    TA0CTL = TASSEL_1 + MC_1;
//    TA0CCTL0 = CCIE;
//    TA0CCR0 = TIMER_FREQUENCY(TIMER_FREQUENCY_A0_CCR0);
    __CONFIG_TIMER(0, 0);
#endif
#if TIMER_FREQUENCY_A0_CCR1
    __CONFIG_TIMER(0, 1)
#endif
#if TIMER_FREQUENCY_A0_CCR2
    __CONFIG_TIMER(0, 2)
#endif
#if TIMER_FREQUENCY_A0_CCR3
    __CONFIG_TIMER(0, 3)
#endif
#if TIMER_FREQUENCY_A0_CCR4
    __CONFIG_TIMER(0, 4)
#endif
#endif /* TIMER_ENABLE_TA0 */
// Timer A1
#ifdef TIMER_ENABLE_TA1
#if TIMER_FREQUENCY_A1_CCR0
    TA1CTL = TASSEL_1 + MC_1;
    __CONFIG_TIMER(1, 0)
#endif
#if TIMER_FREQUENCY_A1_CCR1
    __CONFIG_TIMER(1, 1)
#endif
#if TIMER_FREQUENCY_A1_CCR2
    __CONFIG_TIMER(1, 2)
#endif
#endif /* TIMER_ENABLE_TA1 */
}

inline void delayMs(uint16_t delay)
{
    delay += __ms;
    while (delay != __ms)
        __bic_SR_register(TIMER_POWER_MODE);
}

/* Timer ISR */
#ifdef TIMER_ENABLE_TA0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer0A0()
{
    __bic_SR_register_on_exit(TIMER_POWER_MODE);
#if TIMER_FREQUENCY_A0_CCR0
    timerA0.ccr0();
#endif
#if TIMER_FREQUENCY_A0_CCR0 == 1000
    __ms++;
#endif
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void timer0A1()
{
    __bic_SR_register_on_exit(TIMER_POWER_MODE);

    switch (__even_in_range(TA0IV, 14))
    {
    case TA0IV_TA0CCR1:
#if TIMER_FREQUENCY_A0_CCR1
#ifndef TIMER_UPDATE_DISABLE_TA0CCR1
        __TIMER_UPDATE(0, 1)
#endif
#if TIMER_FREQUENCY_A0_CCR1 == 1000
        __ms++;
#endif
        timerA0.ccr1();
#endif
        break;

    case TA0IV_TA0CCR2:
#if TIMER_FREQUENCY_A0_CCR2
#ifndef TIMER_UPDATE_DISABLE_TA0CCR2
        __TIMER_UPDATE(0, 2)
#endif
#if TIMER_FREQUENCY_A0_CCR2 == 1000
        __ms++;
#endif
        timerA0.ccr2();
#endif
        break;

    case TA0IV_TA0CCR3:
#if TIMER_FREQUENCY_A0_CCR3
#ifndef TIMER_UPDATE_DISABLE_TA0CCR3
        __TIMER_UPDATE(0, 3)
#endif
#if TIMER_FREQUENCY_A0_CCR3 == 1000
        __ms++;
#endif
        timerA0.ccr3();
#endif
        break;

    case TA0IV_TA0CCR4:
#if TIMER_FREQUENCY_A0_CCR2
#ifndef TIMER_UPDATE_DISABLE_TA0CCR4
        __TIMER_UPDATE(0, 4)
#endif
#if TIMER_FREQUENCY_A0_CCR4 == 1000
        __ms++;
#endif
        timerA0.ccr4();
#endif
        break;

    default:
        break;
    }
}
#endif /* TIMER_ENABLE_TA0 */

#ifdef TIMER_ENABLE_TA1
#pragma vector = TIMER1_A0_VECTOR
__interrupt void timer1A0()
{
    __bic_SR_register_on_exit(TIMER_POWER_MODE);
#if TIMER_FREQUENCY_A1_CCR0
    timerA1.ccr0();
#endif
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void timer1A1()
{
    __bic_SR_register_on_exit(TIMER_POWER_MODE);
    switch (__even_in_range(TA1IV, 14))
    {
    case TA1IV_TA1CCR1:
#if TIMER_FREQUENCY_A1_CCR1
#ifndef TIMER_UPDATE_DISABLE_TA1CCR1
        __TIMER_UPDATE(1, 1)
#endif
        timerA1.ccr1();
#endif
        break;
    case TA1IV_TA1CCR2:
#if TIMER_FREQUENCY_A1_CCR2
#ifndef TIMER_UPDATE_DISABLE_TA1CCR2
        __TIMER_UPDATE(1, 2)
#endif
        timerA1.ccr2();
#endif
        break;
    }
}
#endif /* TIMER_ENABLE_TA1 */
