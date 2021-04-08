/*
 * debounce.h
 *
 *  Created on: 31.03.2021
 *      Author: Philip Prohaska
 */

#include <msp430.h>
#include <stdint.h>
#include "util.h"

#ifndef DEBOUNCE_DEBOUNCE_H_
#define DEBOUNCE_DEBOUNCE_H_

#define DEBOUNCE_P1 BIT1
#define DEBOUNCE_P2 BIT1

typedef struct Debouncer
{
    uint8_t states;
    uint8_t toggles;
    uint8_t counters[8];
    uint8_t debounce;
} Debouncer;

extern Debouncer debouncerPort1;
extern Debouncer debouncerPort2;
extern void debounce();
extern void configDebounce();

#define __DEBOUCE_COUNTER_MAX 5
#define __DEBOUNCE_COUNTER_MIN 0
#define __DEBOUNCE_ISR(port, bit) {               \
    if (BIT_ENABLED(P##port##IFG, BIT##bit))       \
    {                                               \
        debouncerPort##port.debounce |= BIT##bit;   \
        BIT_DISABLE(P##port##IFG, BIT##bit);        \
    }                                                \
}

// ISR Macros

#if (DEBOUNCE_P1) & BIT0
#define DEBOUNCE_ISR_PORT1_0() __DEBOUNCE_ISR(1, 0)
#else
#define DEBOUNCE_ISR_PORT1_0()
#endif
#if (DEBOUNCE_P1) & BIT1
#define DEBOUNCE_ISR_PORT1_1() __DEBOUNCE_ISR(1, 1)
#else
#define DEBOUNCE_ISR_PORT1_1()
#endif
#if (DEBOUNCE_P1) & BIT2
#define DEBOUNCE_ISR_PORT1_2() __DEBOUNCE_ISR(1, 2)
#else
#define DEBOUNCE_ISR_PORT1_2()
#endif
#if (DEBOUNCE_P1) & BIT3
#define DEBOUNCE_ISR_PORT1_3() __DEBOUNCE_ISR(1, 3)
#else
#define DEBOUNCE_ISR_PORT1_3()
#endif
#if (DEBOUNCE_P1) & BIT4
#define DEBOUNCE_ISR_PORT1_4() __DEBOUNCE_ISR(1, 4)
#else
#define DEBOUNCE_ISR_PORT1_4()
#endif
#if (DEBOUNCE_P1) & BIT5
#define DEBOUNCE_ISR_PORT1_5() __DEBOUNCE_ISR(1, 5)
#else
#define DEBOUNCE_ISR_PORT1_5()
#endif
#if (DEBOUNCE_P1) & BIT6
#define DEBOUNCE_ISR_PORT1_6() __DEBOUNCE_ISR(1, 6)
#else
#define DEBOUNCE_ISR_PORT1_6()
#endif
#if (DEBOUNCE_P1) & BIT7
#define DEBOUNCE_ISR_PORT1_7() __DEBOUNCE_ISR(1, 7)
#else
#define DEBOUNCE_ISR_PORT1_7()
#endif

#define DEBOUNCE_ISR_PORT1() {  \
    DEBOUNCE_ISR_PORT1_0()      \
    DEBOUNCE_ISR_PORT1_1()      \
    DEBOUNCE_ISR_PORT1_2()      \
    DEBOUNCE_ISR_PORT1_3()      \
    DEBOUNCE_ISR_PORT1_4()      \
    DEBOUNCE_ISR_PORT1_5()      \
    DEBOUNCE_ISR_PORT1_6()      \
    DEBOUNCE_ISR_PORT1_7()      \
}

#if (DEBOUNCE_P2) & BIT0
#define DEBOUNCE_ISR_PORT2_0() __DEBOUNCE_ISR(2, 0)
#else
#define DEBOUNCE_ISR_PORT2_0()
#endif
#if (DEBOUNCE_P2) & BIT1
#define DEBOUNCE_ISR_PORT2_1() __DEBOUNCE_ISR(2, 1)
#else
#define DEBOUNCE_ISR_PORT2_1()
#endif
#if (DEBOUNCE_P2) & BIT2
#define DEBOUNCE_ISR_PORT2_2() __DEBOUNCE_ISR(2, 2)
#else
#define DEBOUNCE_ISR_PORT2_2()
#endif
#if (DEBOUNCE_P2) & BIT3
#define DEBOUNCE_ISR_PORT2_3() __DEBOUNCE_ISR(2, 3)
#else
#define DEBOUNCE_ISR_PORT2_3()
#endif
#if (DEBOUNCE_P2) & BIT4
#define DEBOUNCE_ISR_PORT2_4() __DEBOUNCE_ISR(2, 4)
#else
#define DEBOUNCE_ISR_PORT2_4()
#endif
#if (DEBOUNCE_P2) & BIT5
#define DEBOUNCE_ISR_PORT2_5() __DEBOUNCE_ISR(2, 5)
#else
#define DEBOUNCE_ISR_PORT2_5()
#endif
#if (DEBOUNCE_P2) & BIT6
#define DEBOUNCE_ISR_PORT2_6() __DEBOUNCE_ISR(2, 6)
#else
#define DEBOUNCE_ISR_PORT2_6()
#endif
#if (DEBOUNCE_P2) & BIT7
#define DEBOUNCE_ISR_PORT2_7() __DEBOUNCE_ISR(2, 7)
#else
#define DEBOUNCE_ISR_PORT2_7()
#endif

#define DEBOUNCE_ISR_PORT2() {  \
    DEBOUNCE_ISR_PORT2_0()      \
    DEBOUNCE_ISR_PORT2_1()      \
    DEBOUNCE_ISR_PORT2_2()      \
    DEBOUNCE_ISR_PORT2_3()      \
    DEBOUNCE_ISR_PORT2_4()      \
    DEBOUNCE_ISR_PORT2_5()      \
    DEBOUNCE_ISR_PORT2_6()      \
    DEBOUNCE_ISR_PORT2_7()      \
}

#endif /* DEBOUNCE_DEBOUNCE_H_ */
