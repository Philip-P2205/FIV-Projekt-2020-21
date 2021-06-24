#include <msp430.h>

//  MSP430F552x Demo - USCI_B0 I2C Master TX single bytes to MSP430 Slave
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave. This is the master code. It continuously
//  transmits 00h, 01h, ..., 0ffh and demonstrates how to implement an I2C
//  master transmitter sending a single byte using the USCI_B0 TX interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
//
// ***to be used with "MSP430F55xx_uscib0_i2c_07.c" ***
//
//                                /|\  /|\
//                MSP430F5529     10k  10k     MSP430F5529
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P3.0/UCB0SDA|<-|----+->|P3.0/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P3.1/UCB0SCL|<-+------>|P3.1/UCB0SCL     |
//            |                 |          |                 |
//
//   Bhargavi Nisarga
//   Texas Instruments Inc.
//   April 2009
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
//The code from TI company, use reference volt to pull-up SDA and SCL.


#define LCD_ADDRESS 0x27
// Byte count for sending values to lcd
#define LCD_TX_BYTE_CNT 1

// Data to send to the lcd
unsigned char TXData[LCD_TX_BYTE_CNT] = { 0x01 };
unsigned char TXByteCtr = LCD_TX_BYTE_CNT;

void configI2C();

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    configI2C();

    while (1)
    {

        // Ensure stop condition got sent
        while (UCB0CTL1 & UCTXSTP)
            ;
        UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

        __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupts
        //__no_operation();                       // Remain in LPM0 until all data is TX'd
    }
}

void configI2C()
{
    P3REN |= 0x03;
    P3OUT |= 0x03;
    P3SEL |= 0x03;                            // Assign I2C pins to USCI_B0
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
    UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0I2CSA = LCD_ADDRESS;                         // Slave Address
    UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
    UCB0IE |= UCTXIE;                         // Enable TX interrupt
}

//------------------------------------------------------------------------------
// The USCIAB0_ISR is structured such that it can be used to transmit any
// number of bytes by pre-loading TXByteCtr with the byte count.
//------------------------------------------------------------------------------

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch (__even_in_range(UCB0IV, 12))
    {
    case 0:
        break;                           // Vector  0: No interrupts
    case 2:
        break;                           // Vector  2: ALIFG
    case 4:
        break;                           // Vector  4: NACKIFG
    case 6:
        break;                           // Vector  6: STTIFG
    case 8:
        break;                           // Vector  8: STPIFG
    case 10:
        break;                           // Vector 10: RXIFG
    case 12:                                  // Vector 12: TXIFG
        if (TXByteCtr)                          // Check TX byte counter
        {
            TXByteCtr--;                          // Decrement TX byte counter
            UCB0TXBUF = TXData[TXByteCtr];                   // Load TX buffer
        }
        else
        {
            UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
            UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
            __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
        }
        break;
    default:
        break;
    }
}
