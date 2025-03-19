/**
 * @file dio.h
 * @author Jose Luis Figueroa
 * @brief The interface definition for the DIO. This is the header file for 
 * the definition of the interface for a digital input/output peripheral on 
 * a standard microcontroller.
 * @version 1.1
 * @date 2025-03-19
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
 * 
 */
#ifndef DIO_H_
#define DIO_H_

/*****************************************************************************
* Includes
*****************************************************************************/
#include <stdint.h>
#include <stdio.h>
//#define NDEBUG          /*To disable assert function*/  
#include <assert.h>
#include "dio_cfg.h"    /*For dio configuration*/
#include "stm32f4xx.h"  /*Microcontroller family header*/  

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
    DioPort_t Port;             /**< The I/O port */
    DioPin_t Pin;               /**< The I/O pin */
}DioPinConfig_t;

/*****************************************************************************
* Variables
*****************************************************************************/

/*****************************************************************************
* Function Prototypes
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void DIO_init(const DioConfig_t * const Config, size_t configSize);
DioPinState_t DIO_pinRead(const DioPinConfig_t * const PinConfig);
void DIO_pinWrite(const DioPinConfig_t * const PinConfig, DioPinState_t State);
void DIO_pinToggle(const DioPinConfig_t * const PinConfig);
void DIO_registerWrite(uint32_t address, uint32_t value);
uint32_t DIO_registerRead(uint32_t address);

#ifdef __cplusplus
} // extern C
#endif

#endif /*DIO_H_*/