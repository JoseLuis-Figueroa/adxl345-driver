/**
 * @file adxl345.h
 * @author Jose Luis Figueroa 
 * @brief The interface definition for the adxl345. This
 * is the header file for the definition of the interface for adxl345
 * digital accelerometer. 
 * @version 1.0
 * @date 2023-10-5
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License
 * 
 */
#ifndef ADXL345_H_
#define ADXL345_H_
/*****************************************************************************
* Includes
*****************************************************************************/
#include <stdio.h>
#include "spi.h"
#include "dio.h"

/*****************************************************************************
* Configuration Constants
*****************************************************************************/
/*adxl345 registers*/
#define DEVID_R             (0x00)
#define POWER_CTL_R         (0x2D)
#define DATA_FORMAT_R       (0x31)
#define DATA_START_R        (0x32)

/*Constants*/
#define RESET               (0x00)
#define FOUR_G              (0x01)
#define SET_MEASURE         (0x08)
#define MULTI_BYTE_EN       (0x40)
#define DEVICE_ADDR         (0x53)
#define READ_OPERATION      (0x80)
#define FOUR_G_SCALE_FACTOR (0.0078)

/*****************************************************************************
* Function Prototypes
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void ADXL345_init(SpiChannel_t Channel, DioPort_t Port, DioPin_t Pin);
void ADXL345_read(SpiChannel_t Channel, DioPort_t Port, DioPin_t Pin, 
uint8_t address, uint8_t *data);

#ifdef __cplusplus
}   /*Extern C*/
#endif

#endif /*ADXL345_H_*/