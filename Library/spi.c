/*
 * spi.c
 *
 *  Created on: 08.04.2021
 *      Author: Philip Prohaska
 */
#include "spi.h"

void configSPI()
{
    P3SEL |= BIT3 + BIT4;                     // P3.3,4 option select
    P2SEL |= BIT7;                            // P2.7 option select

    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL0 |= UCMST + UCSYNC + UCCKPL + UCMSB;    // 3-pin, 8-bit SPI master
                                                    // Clock polarity high, MSB

    UCA0CTL1 |= UCSSEL_2;                     // SMCLK

    UCA0BR0 = 0x02;           // / 2 ==> 13Mhz, / 1 ==> 25Mhz
    UCA0BR1 = 0;     // The clock frequency is scaled by UCA0BR0 + UCA0BR1 * 256
    UCA0MCTL = 0;             // No modulation
    UCA0CTL1 &= ~UCSWRST;     // Initialize USCI state machine(Enable SPI)
#ifdef SPI_INTERRUPT_ENABLE
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
#endif
}

void senSPI8Bit(uint8_t data)
{
#ifdef SPI_CE_ENABLE
#if SPI_CE_ENABLE == 0
    OUT_LOW(SPI_CE);
#else
    OUT_HIGH(SPI_CE);
#endif
#endif
    while (!(UCA0IFG & UCTXIFG))
        __bic_SR_register(SPI_POWER_MODE); // Wait until SPI is ready
    UCA0TXBUF = data;

#ifdef SPI_CE_ENABLE
#if SPI_CE_ENABLE == 0
    OUT_HIGH(SPI_CE);
#else
    OUT_LOW(SPI_CE);
#endif
#endif
}

void sendSPI16Bit(uint16_t data)
{
    uint8_t i = 2;
    uint8_t *chunk = (uint8_t*) &data;

#ifdef SPI_CE_ENABLE
#if SPI_CE_ENABLE == 0
    OUT_LOW(SPI_CE);
#else
    OUT_HIGH(SPI_CE);
#endif
#endif

    for (; i > 0; --i)
    {
        while (!(UCA0IFG & UCTXIFG))
            __bic_SR_register(SPI_POWER_MODE); // Wait until SPI is ready

        UCA0TXBUF = chunk[i];
    }

#ifdef SPI_CE_ENABLE
#if SPI_CE_ENABLE == 0
    OUT_HIGH(SPI_CE);
#else
    OUT_LOW(SPI_CE);
#endif
#endif
}

void sendSPI32Bit(uint32_t data)
{
    uint8_t i = 4;
    uint8_t *chunk = (uint8_t*) &data;

#ifdef SPI_CE_ENABLE
#if SPI_CE_ENABLE == 0
    OUT_LOW(SPI_CE);
#else
    OUT_HIGH(SPI_CE);
#endif
#endif

    for (; i > 0; --i)
    {
        while (!(UCA0IFG & UCTXIFG))
            __bic_SR_register(SPI_POWER_MODE); // Wait until SPI is ready

        UCA0TXBUF = chunk[i];
    }

#ifdef SPI_CE_ENABLE
#if SPI_CE_ENABLE == 0
    OUT_HIGH(SPI_CE);
#else
    OUT_LOW(SPI_CE);
#endif
#endif
}

void sendSPI64Bit(uint64_t data)
{
    uint8_t i = 8;
    uint8_t *chunk = (uint8_t*) &data;

#ifdef SPI_CE_ENABLE
#if SPI_CE_ENABLE == 0
    OUT_LOW(SPI_CE);
#else
    OUT_HIGH(SPI_CE);
#endif
#endif

    for (; i > 0; --i)
    {
        while (!(UCA0IFG & UCTXIFG))
            __bic_SR_register(SPI_POWER_MODE); // Wait until SPI is ready

        UCA0TXBUF = chunk[i];
    }

#ifdef SPI_CE_ENABLE
#if SPI_CE_ENABLE == 0
    OUT_HIGH(SPI_CE);
#else
    OUT_LOW(SPI_CE);
#endif
#endif
}
