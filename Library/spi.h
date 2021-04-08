/*
 * spi.h
 *
 *  Created on: 08.04.2021
 *      Author: Philip Prohaska
 */

#ifndef LIBRARY_SPI_H_
#define LIBRARY_SPI_H_

#include <stdint.h>
#include <msp430.h>
#include "util.h"

#define SPI_INTERRUPT_ENABLE
#define SPI_CE_ENABLE 0         //0: LOW, 1: HIGH

#define SPI_CE_PORT P3
#define SPI_CE_PIN  BIT2

#define SPI_CE      SPI_CE_PIN
#define SPI_CE_DIR  GET_DIR(SPI_CE_PORT)
#define SPI_CE_OUT  GET_OUT(SPI_CE_PORT)

#define SPI_POWER_MODE LPM0_bits

extern void configSPI();
extern void sendSPI8Bit(uint8_t);
extern void sendSPI16Bit(uint16_t);
extern void sendSPI32Bit(uint32_t);
extern void sendSPI64Bit(uint64_t);

#endif /* LIBRARY_SPI_H_ */
