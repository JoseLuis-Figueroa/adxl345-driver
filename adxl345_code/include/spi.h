/**
 * @file spi.h
 * @author Jose Luis Figueroa 
 * @brief The interface definition for the SPI. This is the header file for 
 * the definition of the interface for a Serial Peripheral Serial (SPI) on 
 * a standard microcontroller.
 * @version 1.1
 * @date 2025-03-11
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. All rights reserved.
 * 
 */
#ifndef SPI_H_
#define SPI_H_

/*****************************************************************************
* Includes
*****************************************************************************/
#include <stdint.h>
#include <stdio.h>
//#define NDEBUG          /*To disable assert function*/  
#include <assert.h>
#include "spi_cfg.h"
#include "stm32f4xx.h"   

/*****************************************************************************
* Preprocessor Constants
*****************************************************************************/

/*****************************************************************************
* Configuration Constants
*****************************************************************************/

/*****************************************************************************
* Macros
*****************************************************************************/

/*****************************************************************************
* Typedefs
*****************************************************************************/
typedef struct
{
    SpiChannel_t Channel;           /**< The SPI channel */
    uint16_t size;                  /**< The size of the data */
    uint16_t *data;                 /**< The data to be sent */
}SpiTransferConfig_t;

/*****************************************************************************
* Variables
*****************************************************************************/

/*****************************************************************************
* Function Prototypes
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void SPI_init(const SpiConfig_t * const Config, size_t configSize);
void SPI_transfer(const SpiTransferConfig_t * const TransferConfig);
void SPI_receive(const SpiTransferConfig_t * const TransferConfig);
void SPI_registerWrite(uint32_t address, uint32_t value);
uint16_t SPI_registerRead(uint32_t address);

#ifdef __cplusplus
} // extern C
#endif

#endif /*SPI_H_*/