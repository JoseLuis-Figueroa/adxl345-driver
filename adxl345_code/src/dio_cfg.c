/**
 * @file dio_cfg.c
 * @author Jose Luis Figueroa
 * @brief This module contains the implementation for the digital 
 * input/output peripheral configuration.
 * @version 1.0
 * @date 2023-03-17
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
 */

/*****************************************************************************
* Module Includes
*****************************************************************************/
#include "dio_cfg.h"
 
/*****************************************************************************
* Module Preprocessor Constants
*****************************************************************************/

/*****************************************************************************
* Module Preprocessor Macros
*****************************************************************************/

/*****************************************************************************
* Module Typedefs
*****************************************************************************/

/*****************************************************************************
* Module Variable Definitions
*****************************************************************************/
/**
 * The following array contains the configuration data for each digital
 * input/output peripheral channel (pin). Each row represent a single pin.
 * Each column is representing a member of the DioConfig_t structure. This 
 * table is read in by Dio_Init, where each channel is then set up based on 
 * this table.
*/
const DioConfig_t DioConfig[] = 
{
/*                                                          
 *  Port    Pin      Mode        Type           Speed          Resistor         Function
 *                
*/ 
   {DIO_PA, DIO_PA4, DIO_OUTPUT,   DIO_PUSH_PULL, DIO_LOW_SPEED, DIO_NO_RESISTOR, DIO_AF5},
   {DIO_PA, DIO_PA5, DIO_FUNCTION, DIO_PUSH_PULL, DIO_LOW_SPEED, DIO_NO_RESISTOR, DIO_AF5},
   {DIO_PA, DIO_PA6, DIO_FUNCTION, DIO_PUSH_PULL, DIO_LOW_SPEED, DIO_NO_RESISTOR, DIO_AF5},
   {DIO_PA, DIO_PA7, DIO_FUNCTION, DIO_PUSH_PULL, DIO_LOW_SPEED, DIO_NO_RESISTOR, DIO_AF5},
};

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: DIO_configGet()
*/
/**
*\b Description:
 * This function is used to initialize the DIO based on the configuration
 * table defined in dio_cfg module.
 * 
 * PRE-CONDITION: configuration table needs to be populated (sizeof > 0)
 * POST-CONDITION: A constant pointer to the first member of the  
 * configuration table will be returned.
 * @return A pointer to the configuration table.
 * 
 * \b Example: 
 * @code
 * const Dio_ConfigType_t * const DioConfig = DIO_configGet();
 * 
 * DIO_Init(DioConfig);
 * @endcode
 * 
 * @see DIO_init
 * @see DIO_channelRead
 * @see DIO_channelWrite
 * @see DIO_channelToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * 
*****************************************************************************/
const DioConfig_t * const DIO_configGet(void)
{
   /* The cast is performed to ensure that the address of the first element 
    * of configuration table is returned as a constant pointer and not a
    * pointer that can be modified
   */
  return (const DioConfig_t*)&DioConfig[0];

}