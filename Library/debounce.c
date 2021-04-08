/*
 * debouncer.c
 *
 *  Created on: 31.03.2021
 *      Author: Philip Prohaska
 */

#include <Library/debounce.h>

#if (DEBOUNCE_P1)
Debouncer debouncerPort1;
#endif
#if (DEBOUNCE_P2)
Debouncer debouncerPort2;
#endif

#define _debounce(d, p, n) __debounce(d, p, BIT##n, n)
static inline void __debounce(Debouncer *d, volatile uint8_t port, uint8_t bit,
                              uint8_t index)
{
    if (BIT_DISABLED(d->debounce, bit))
        return;
    if (BIT_DISABLED(port, bit))
    {
        if (++d->counters[index] >= __DEBOUCE_COUNTER_MAX)
        {
            d->counters[index] = __DEBOUCE_COUNTER_MAX;
            if (BIT_DISABLED(d->states, bit))
                BIT_TOGGLE(d->toggles, bit);
            BIT_ENABLE(d->states, bit);
        }
    }
    else if (d->counters[index]-- <= __DEBOUNCE_COUNTER_MIN)
    {
        d->counters[index] = 0;
        BIT_DISABLE(d->states, bit);
        BIT_DISABLE(d->debounce, bit);
    }
}

void debounce()
{
// Port 1
#if (DEBOUNCE_P1) & BIT0
    _debounce(&debouncerPort1, P1IN, 0);
#endif
#if (DEBOUNCE_P1) & BIT1
    _debounce(&debouncerPort1, P1IN, 1);
#endif
#if (DEBOUNCE_P1) & BIT2
    _debounce(&debouncerPort1, P1IN, 2);
#endif
#if (DEBOUNCE_P1) & BIT3
    _debounce(&debouncerPort1, P1IN, 3);
#endif
#if (DEBOUNCE_P1) & BIT4
    _debounce(&debouncerPort1, P1IN, 4);
#endif
#if (DEBOUNCE_P1) & BIT5
    _debounce(&debouncerPort1, P1IN, 5);
#endif
#if (DEBOUNCE_P1) & BIT6
    _debounce(&debouncerPort1, P1IN, 6);
#endif
#if (DEBOUNCE_P1) & BIT7
    _debounce(&debouncerPort1, P1IN, 7);
#endif

// Port 2
#if (DEBOUNCE_P2) & BIT0
    _debounce(&debouncerPort2, P2IN, 0);
#endif
#if (DEBOUNCE_P2) & BIT1
    _debounce(&debouncerPort2, P2IN, 1);
#endif
#if (DEBOUNCE_P2) & BIT2
    _debounce(&debouncerPort2, P2IN, 2);
#endif
#if (DEBOUNCE_P2) & BIT3
    _debounce(&debouncerPort2, P2IN, 3);
#endif
#if (DEBOUNCE_P2) & BIT4
    _debounce(&debouncerPort2, P2IN, 4);
#endif
#if (DEBOUNCE_P2) & BIT5
    _debounce(&debouncerPort2, P2IN, 5);
#endif
#if (DEBOUNCE_P2) & BIT6
    _debounce(&debouncerPort2, P2IN, 6);
#endif
#if (DEBOUNCE_P2) & BIT7
    _debounce(&debouncerPort2, P2IN, 7);
#endif
}

void configDebounce()
{
    //Port 1
    BIT_DISABLE(P1DIR, DEBOUNCE_P1);
    BIT_ENABLE(P1REN, DEBOUNCE_P1);
    BIT_ENABLE(P1OUT, DEBOUNCE_P1);
    BIT_ENABLE(P1IES, DEBOUNCE_P1);
    P1IFG = 0;
    BIT_ENABLE(P1IE, DEBOUNCE_P1);

    //Port 2
    BIT_DISABLE(P2DIR, DEBOUNCE_P2);
    BIT_ENABLE(P2REN, DEBOUNCE_P2);
    BIT_ENABLE(P2OUT, DEBOUNCE_P2);
    BIT_ENABLE(P2IES, DEBOUNCE_P2);
    P1IFG = 0;
    BIT_ENABLE(P2IE, DEBOUNCE_P2);

}
