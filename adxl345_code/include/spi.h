/**
 * @file spi.h
 * @author Jose Luis Figueroa 
 * @brief The interface definition for the spi. This is the header file for 
 * the definition of the interface for a Serial Peripheral Serial (SPI) on 
 * a standard microcontroller.
 * @version 1.0
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. All rights reserved.
 * 
 */
#ifndef SPI_H_
#define SPI_H_

/*****************************************************************************
* Includes
*****************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "spi_cfg.h"
#include "stm32f4xx.h"   

/*****************************************************************************
* Function Prototypes
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void SPI_init(const SpiConfig_t * const Config);
void SPI_transfer(SpiChannel_t Channel, uint16_t *data, uint16_t size);
void SPI_receive(SpiChannel_t Channel, uint16_t *data, uint16_t size);
void SPI_registerWrite(uint32_t address, uint32_t value);
uint16_t SPI_registerRead(uint32_t address);

#ifdef __cplusplus
} // extern C
#endif

#endif /*SPI_H_*/