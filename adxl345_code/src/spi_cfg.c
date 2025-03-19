/**
 * @file spi_cfg.c
 * @author Jose Luis Figueroa.
 * @brief This module contains the implementation for the Serial Peripheral
 * Interface (SPI).
 * @version 1.1
 * @date 2025-03-19
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
 * 
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "spi_cfg.h"

/*****************************************************************************
* Module Variable Definitions
*****************************************************************************/
/**
 * The following array contains the configuration data for each Serial 
 * Peripheral Interface. Each row represent a single SPI configuration.
 * Each column is representing a member of the SpiConfig_t structure. This 
 * table is read in by SPI_Init, where each channel is then set up based on 
 * this table. 
*/
const SpiConfig_t SpiConfig[] = 
{
/*                                                          
 * Channel        Mode       Hierarchy   Baud rate   NSS pin,                   
 * Frame    Type             Size               
*/
   {SPI_CHANNEL1, SPI_MODE3, SPI_MASTER, SPI_FPCLK4, SPI_HARDWARE_NSS_ENABLED, 
   SPI_MSB, SPI_FULL_DUPLEX, SPI_8BITS},
};

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: SPI_ConfigGet()
*/
/**
*\b Description:
 * This function is used to initialize the SPI based on the configuration
 * table defined in spi_cfg module.
 * 
 * PRE-CONDITION: configuration table needs to be populated (sizeof > 0). <br>
 * POST-CONDITION: A constant pointer to the first member of the configuration 
 * table will be returned. <br>
 * 
 * @return A pointer to the configuration table. <br>
 * 
 * \b Example: 
 * @code
 * const SpiConfig_t * const SpiConfig = SPI_ConfigGet();
 * size_t configSize = DIO_configSizeGet();
 * 
 * SPI_Init(SpiConfig, configSize);
 * @endcode
 *
 * @see SPI_configGet
 * @see SPI_configSizeGet
 * @see SPI_Init
 * @see SPI_Transfer
 * @see SPI_RegisterWrite
 * @see SPI_RegisterRead
 * @see SPI_CallbackRegister
 * 
*****************************************************************************/
const SpiConfig_t * const SPI_ConfigGet(void)
{
   /* The cast is performed to ensure that the address of the first element 
    * of configuration table is returned as a constant pointer and not a
    * pointer that can be modified
   */
  return (const SpiConfig_t*)&SpiConfig[0];

}

/*****************************************************************************
 * Function: SPI_configSizeGet()
*/
/**
*\b Description:
 * This function is used to get the size of the configuration table.
 * 
 * PRE-CONDITION: configuration table needs to be populated (sizeof > 0) <br>
 * 
 * POST-CONDITION: The size of the configuration table will be returned. <br>
 * 
 * @return The size of the configuration table. <br>
 * 
 * \b Example: 
 * @code
 * const SpiConfig_t * const SpiConfig = SPI_ConfigGet();
 * size_t configSize = DIO_configSizeGet();
 * 
 * SPI_Init(SpiConfig, configSize);
 * @endcode
 * 
 * @see SPI_configGet
 * @see SPI_configSizeGet
 * @see SPI_Init
 * @see SPI_Transfer
 * @see SPI_RegisterWrite
 * @see SPI_RegisterRead
 * @see SPI_CallbackRegister
 * 
*****************************************************************************/
size_t SPI_configSizeGet(void)
{
   return sizeof(SpiConfig)/sizeof(SpiConfig[0]);
}