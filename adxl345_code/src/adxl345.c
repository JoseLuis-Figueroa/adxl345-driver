/**
 * @file adxl345.c
 * @author Jose Luis Figueroa 
 * @brief This module contains the implementation for the ADXL345 digital 
 * accelerometer.
 * @version 1.1
 * @date 2025-03-19
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License
 * 
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "adxl345.h"

/*****************************************************************************
* Module Variable Definitions
*****************************************************************************/
char data;

/*****************************************************************************
* Function Prototypes
*****************************************************************************/
static void ADXL345_write(const Adxl345Config_t * const Config, 
uint8_t address, uint8_t value);

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: ADXL345_init()
*//**
*\b Description:
 * This function is used to initialize the ADXL345 digital accelerometer. 
 * 
 * PRE-CONDITION: The MCU clocks must be configured and enabled. <br>
 * PRE-CONDITION: SPI pins should be configured using GPIO driver. <br>
 * PRE-CONDITION: SPI peripheral should be configured. <br>
 * PRE-CONDITION: Adxl345Config_t needs to be populated. <br>
 * PRE-CONDITION: The Channel is within the maximum SpiChannel_t. <br>
 * PRE-CONDITION: The Port is within the maximum DioPort_t. <br>
 * PRE-CONDITION: The Pin is within the maximum DioPin_t. <br>
 *
 * POST-CONDITION: The ADXL345 is set up with the configuration settings.
 * 
 * @param[in]   Config A pointer to a structure containing the channel, port, 
 *              and pin of the SPI.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 * const Dio_ConfigType_t * const DioConfig = DIO_configGet();
 * size_t configSize = DIO_configSizeGet();
 * DIO_Init(DioConfig, configSize);
 * 
 * const SpiConfig_t * const SpiConfig = SPI_configGet();
 * size_t configSize = SPI_configSizeGet();
 * SPI_Init(DioConfig, configSize);
 *     
 * Adxl345Config_t Adxl345Config =
 * {
 *    .Channel = SPI_CHANNEL1,
 *    .Port = DIO_PA, 
 *    .Pin = DIO_PA4
 * };
 * 
 * ADXL345(&Adxl345Config);
 * @endcode
 * 
 * @see DIO_configGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see SPI_configGet
 * @see SPI_configSizeGet
 * @see SPI_init
 * @see ADXL345_init
 * @see ADXL345_read
 * @see ADXL345_write
 * 
*****************************************************************************/
void ADXL345_init(const Adxl345Config_t * const Config)
{
    /*Set data format range to +-4g*/
    ADXL345_write(Config, DATA_FORMAT_R, FOUR_G);
    /*Reset all bits*/
    ADXL345_write(Config, POWER_CTL_R, RESET);
    /*Configure power control measure bit*/
    ADXL345_write(Config, POWER_CTL_R, SET_MEASURE);
}

/*****************************************************************************
 * Function: ADXL345_write()
*//**
*\b Description:
 * This function is used to directly address and modify a ADXL345 register.
 * 
 * PRE-CONDITION: ADXL345_init must be called with valid configuration data.<br>
 * PRE-CONDITION: Adxl345Config_t needs to be populated. <br>
 * PRE-CONDITION: The Channel is within the maximum SpiChannel_t. <br>
 * PRE-CONDITION: The Port is within the maximum DioPort_t. <br>
 * PRE-CONDITION: The Pin is within the maximum DioPin_t. <br>
 * PRE-CONDITION: It is within the boundaries of the ADXL345 register address. <br>
 *
 * POST-CONDITION: The register located at address is updated with value.<br>
 *  
 * @param[in]   Config A pointer to a structure containing the channel, port, 
 *              and pin of the SPI.
 * @param[in]   address is a register address within the ADXL345 register map.
 * @param[in]   value is the data to set the ADXL345 register.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 * void ADXL345_init(const Adxl345Config_t * const Config)
 * {
 *    ADXL345_write(Config, DATA_FORMAT_R, FOUR_G);
 * }
 * @endcode
 * 
 * @see DIO_configGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see SPI_configGet
 * @see SPI_configSizeGet
 * @see SPI_init
 * @see ADXL345_init
 * @see ADXL345_read
 * @see ADXL345_write
 * 
*****************************************************************************/
static void ADXL345_write(const Adxl345Config_t * const Config, 
uint8_t address, uint8_t value)
{
    uint16_t data[2];
    /*Enable multi-byte, place address into buffer*/
    data[0] = address | MULTI_BYTE_EN;
    /* Place the data into buffer*/
    data[1] = value;

     /*Define the pin configuration for PA4 (CS line)*/
     const DioPinConfig_t CSLine = 
     {
         .Port = Config->Port,
         .Pin = Config->Pin
     };

    /*SPI transfer configuration*/
    SpiTransferConfig_t TransferConfig =
    {
        .Channel = Config->Channel,
        .size = sizeof(data)/sizeof(data[0]),
        .data = data
    };

    /*Pull cs line low to enable slave*/
    DIO_pinWrite(&CSLine, DIO_LOW);
    /*Transmit data and address*/
    SPI_transfer(&TransferConfig);
    /*Pull cs line high to disable slave*/
    DIO_pinWrite(&CSLine, DIO_HIGH);
}

/*****************************************************************************
* Function: ADXL345_read()
*//**
*\b Description:
 * This function is used to directly read data from ADXL345 register.
 * 
 * PRE-CONDITION: ADXL345_init must be called with valid configuration data.<br>
 * PRE-CONDITION: Adxl345Config_t needs to be populated. <br>
 * PRE-CONDITION: The Channel is within the maximum SpiChannel_t. <br>
 * PRE-CONDITION: The Port is within the maximum DioPort_t. <br>
 * PRE-CONDITION: The Pin is within the maximum DioPin_t. <br>
 * PRE-CONDITION: It is within the boundaries of the ADXL345 register address. <br>
 *
 * POST-CONDITION: The data is stored in the data buffer. <br>
 * 
 * @param[in]   Config A pointer to a structure containing the channel, port, 
 *              and pin of the SPI.
 * @param[in]   address is a register address within the ADXL345 register map.
 * @param[in]   value is the data to set the ADXL345 register.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 * Adxl345Config_t Adxl345Config =
 * {
 *    .Channel = SPI_CHANNEL1,
 *    .Port = DIO_PA, 
 *    .Pin = DIO_PA4
 * };
 * 
 * ADXL345_read(&Adxl345Config, DATA_FORMAT_R, FOUR_G);
 * @endcode
 * 
 * @see DIO_configGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see SPI_configGet
 * @see SPI_configSizeGet
 * @see SPI_init
 * @see ADXL345_init
 * @see ADXL345_read
 * @see ADXL345_write
 * 
*****************************************************************************/
void ADXL345_read(const Adxl345Config_t * const Config, uint16_t address,
uint16_t size, uint16_t *data)
{
    /*Set read operation*/
    address |= READ_OPERATION;
    /*Enable multi-byte*/
    address |= MULTI_BYTE_EN;

    /*Define the pin configuration for PA4 (CS line)*/
     const DioPinConfig_t CSLine = 
     {
         .Port = Config->Port,
         .Pin = Config->Pin
     };

    /* SPI transfer configuration*/
    SpiTransferConfig_t TransferConfig =
    {
        .Channel = Config->Channel,
        .size = 1,
        .data = &address
    };

    /* SPI transfer configuration*/
    SpiTransferConfig_t receiverConfig =
    {
        .Channel = Config->Channel,
        .size = size,
        .data = data
    };

    /*Pull cs line low to enable slave*/
    DIO_pinWrite(&CSLine, DIO_LOW);
    /*Transmit the address*/
    SPI_transfer(&TransferConfig);
    /*Read 6 bytes*/
    SPI_receive(&receiverConfig);
    /*Pull cs line high to disable slave*/
    DIO_pinWrite(&CSLine, DIO_HIGH);
}