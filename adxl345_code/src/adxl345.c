/**
 * @file adxl345.c
 * @author Jose Luis Figueroa 
 * @brief This module contains the implementation for the ADXL345 digital 
 * accelerometer.
 * @version 1.0
 * @date 2023-10-5
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License
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
static void ADXL345_write( SpiChannel_t Channel, DioPort_t Port, DioPin_t Pin,
uint8_t address, char value);

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: ADXL345_init()
*//**
*\b Description:
 * This function is used to initialize the ADXL345 digital accelerometer.
 * 
 * PRE-CONDITION: The MCU clocks must be configured and enabled.
 * PRE-CONDITION: SPI pins should be configured using GPIO driver.
 * PRE-CONDITION: SPI peripheral should be configured.
 *
 * POST-CONDITION: The ADXL345 is set up with the configuration settings.
 * 
 * @param[in]   Channel is the SPI from SpiChannel_t used to transfer data.
 * @param[in]   Port is the GPIO from DioPort_t to be modified.
 * @param[in]   Pin is the pin from DioPin_t to be modified used to configure
 * Slave-Selector.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 *  const DioConfig_t * const DioConfig = DIO_configGet();
 *  DIO_init(DioConfig);
 *  const SpiConfig_t * const SpiConfig = SPI_configGet();
 *  SPI_Init(SpiConfig);
 *  ADXL345(Channel, Port, Pin);
 * @endcode
 * 
 * @see DIO_configGet
 * @see DIO_init
 * @see SPI_configGet
 * @see SPI_init
 * @see ADXL345_init
 * @see ADXL345_read
 * @see ADXL345_write
 * 
*****************************************************************************/
void ADXL345_init(SpiChannel_t Channel, DioPort_t Port, DioPin_t Pin)
{
    /*Set data format range to +-4g*/
    ADXL345_write(Channel, Port, Pin, DATA_FORMAT_R, FOUR_G);
    /*Reset all bits*/
    ADXL345_write(Channel, Port, Pin, POWER_CTL_R, RESET);
    /*Configure power control measure bit*/
    ADXL345_write(Channel, Port, Pin, POWER_CTL_R, SET_MEASURE);
}

/*****************************************************************************
 * Function: ADXL345_write()
*//**
*\b Description:
 * This function is used to directly address and modify a ADXL345 register.
 * 
 * PRE-CONDITION: SPI_init must be called with valid configuration data.
 * PRE-CONDITION: Address is within the boundaries of the ADXL345 register
 * address space.
 *
 * POST-CONDITION: The register located at address is updated with value.
 * 
 * @param[in]   Channel is the SPI from SpiChannel_t used to transfer data.
 * @param[in]   Port is the GPIO from DioPort_t to be modified.
 * @param[in]   Pin is the pin from DioPin_t to be modified used to configure
 * Slave-Selector.
 * @param[in]   address is a register address within the ADXL345 register map.
 * @param[in]   value is the data to set the ADXL345 register.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 * ADXL345_write(Channel, Port, Pin, DATA_FORMAT_R, FOUR_G);
 * @endcode
 * 
 * @see DIO_configGet
 * @see DIO_init
 * @see SPI_configGet
 * @see SPI_init
 * @see ADXL345_init
 * @see ADXL345_read
 * @see ADXL345_write
 * 
*****************************************************************************/
static void ADXL345_write(SpiChannel_t Channel, DioPort_t Port, DioPin_t Pin, 
 uint8_t address, char value)
{
    uint8_t data[2];
    /*Enable multi-byte, place address into buffer*/
    data[0] = address | MULTI_BYTE_EN;
    /*Place the data into buffer*/
    data[1] = value;
    /*Pull cs line low to enable slave*/
    DIO_pinWrite(DIO_PA, DIO_PA4, DIO_HIGH);
    /*Transmit data and address*/
    SPI_transfer(Channel, data, 2);
    /*Pull cs line high to disable slave*/
    DIO_pinWrite(DIO_PA, DIO_PA4, DIO_LOW);
}

/*****************************************************************************
* Function: ADXL345_read()
*//**
*\b Description:
 * This function is used to directly read data from ADXL345 register.
 * 
 * PRE-CONDITION: SPI_init must be called with valid configuration data.
 * PRE-CONDITION: Address is within the boundaries of the ADXL345 register
 * address space.
 *
 * POST-CONDITION: The register located at address is read it.
 * 
 * @param[in]   Channel is the SPI from SpiChannel_t used to receive data.
 * @param[in]   Port is the GPIO from DioPort_t to be modified.
 * @param[in]   Pin is the pin from DioPin_t to be modified used to configure
 * Slave-Selector.
 * @param[in]   address is a register address within the ADXL345 register map.
 * @param[in]   value is the data to set the ADXL345 register.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 * ADXL345_read(Channel, Port, Pin, DATA_FORMAT_R, FOUR_G);
 * @endcode
 * 
 * @see DIO_configGet
 * @see DIO_init
 * @see SPI_configGet
 * @see SPI_init
 * @see ADXL345_init
 * @see ADXL345_read
 * @see ADXL345_write
 * 
*****************************************************************************/
