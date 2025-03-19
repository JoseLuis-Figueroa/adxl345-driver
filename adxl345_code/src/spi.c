/**
 * @file spi.c
 * @author Jose Luis Figueroa 
 * @brief The implementation for the SPI Driver.
 * @version 1.1
 * @date 2025-03-11
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
 * 
 */
/*****************************************************************************
* Includes
*****************************************************************************/
#include "spi.h"

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
/** Defines an array of pointers to the SPI control register 1*/
static uint16_t volatile * const controlRegister1[SPI_PORTS_NUMBER] = 
{
    (uint16_t*)&SPI1->CR1, (uint16_t*)&SPI2->CR1, (uint16_t*)&SPI3->CR1,
    (uint16_t*)&SPI4->CR1
};

/** Define an array of pointers to the SPI control register 2*/
static uint16_t volatile * const controlRegister2[SPI_PORTS_NUMBER] =
{
    (uint16_t*)&SPI1->CR2, (uint16_t*)&SPI2->CR2, (uint16_t*)&SPI3->CR2,
    (uint16_t*)&SPI4->CR2 
};

/** Define an array of pointers to the SPI status register*/
static uint16_t volatile * const statusRegister[SPI_PORTS_NUMBER] =
{
    (uint16_t*)&SPI1->SR, (uint16_t*)&SPI2->SR, (uint16_t*)&SPI3->SR,
    (uint16_t*)&SPI4->SR
};

/** Define an array of pointers to the SPI data register*/
static uint16_t volatile * const dataRegister[SPI_PORTS_NUMBER] =
{
    (uint16_t*)&SPI1->DR, (uint16_t*)&SPI2->DR, (uint16_t*)&SPI3->DR,
    (uint16_t*)&SPI4->DR
};

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: SPI_init()
*//**
*\b Description:
 * This function is used to initialize the SPI based on the configuration  
 * table defined in spi_cfg module.
 * 
 * PRE-CONDITION: The MCU clocks must be configured and enabled. <br>
 * PRE-CONDITION: SPI pins should be configured using GPIO driver. <br>
 * PRE-CONDITION: Configuration table needs to be populated (sizeof > 0) <br>
 * PRE-CONDITION: The setting is within the maximum values (SPI_MAX). <br>
 *
 * POST-CONDITION: The peripheral is set up with the configuration settings. <br>
 * 
 * @param[in]   Config is a pointer to the configuration table that contains 
 *               the initialization for the peripheral. 
 * @param[in]   configSize is the size of the configuration table. 
 * 
 * @return  void 
 *  
 * \b Example:
 * @code
 *  const SpiConfig_t * const SpiConfig = SPI_configGet();
 *  size_t configSize = SPI_configSizeGet();
 * 
 *  SPI_Init(DioConfig, configSize);
 * @endcode
 * 
 * @see SPI_configGet
 * @see SPI_getConfigSize
 * @see SPI_Init
 * @see SPI_Transfer
 * @see SPI_RegisterWrite
 * @see SPI_RegisterRead
 * @see SPI_CallbackRegister
 * 
*****************************************************************************/
void SPI_init(const SpiConfig_t * const Config, size_t configSize)
{
    /**Loop through all the elements of the configuration table.*/
    for(uint8_t i=0; i<configSize; i++)
    {
        /* Prevent to assign a value out of the range of the channels.
         * The registers arrays are limited to the SPI_PORTS_NUMBER, higher 
         * value can cause a memory violation.
        */
       assert(Config[i].Channel < SPI_MAX_CHANNEL);

        /**Set the configuration of the SPI on the control register 1*/
        /**Set the Clock phase and polarity modes*/
        if(Config[i].Mode == SPI_MODE0)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_CPHA;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_CPOL;
        } 
        else if(Config[i].Mode == SPI_MODE1)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_CPHA;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_CPOL;
        }
        else if(Config[i].Mode == SPI_MODE2)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_CPHA;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_CPOL;
        }
        else if(Config[i].Mode == SPI_MODE3)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_CPHA;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_CPOL;
        }
        else
        {
            assert(Config[i].Mode < SPI_MAX_MODE);
        }

        /**Set the hierarchy of the device*/
        if(Config[i].Hierarchy == SPI_MASTER)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_MSTR;
        }
        else if(Config[i].Hierarchy == SPI_SLAVE)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_MSTR;
        } 
        else
        {
            assert(Config[i].Hierarchy < SPI_MAX_HIERARCHY);
        }

        /**Set the baud rate of the device*/
        if(Config[i].BaudRate == SPI_FPCLK2)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_0;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_1;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_2;
        }
        else if(Config[i].BaudRate == SPI_FPCLK4)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_0;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_1;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_2;
        }
        else if(Config[i].BaudRate == SPI_FPCLK8)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_0;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_1;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_2;
        }
        else if(Config[i].BaudRate == SPI_FPCLK16)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_0;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_1;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_2;
        }
        else if(Config[i].BaudRate == SPI_FPCLK32)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_0;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_1;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_2;
        }
        else if(Config[i].BaudRate == SPI_FPCLK64)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_0;
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_1;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_2;
        }
        else if(Config[i].BaudRate == SPI_FPCLK128)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_BR_0;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_1;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_2;
        }
        else if(Config[i].BaudRate == SPI_FPCLK256)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_0;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_1;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_BR_2;
        }
        else
        {
            assert(Config[i].BaudRate < SPI_MAX_FPCLK);
        }

        /**Set the slave select pin management for the device*/
        if(Config[i].SlaveSelect == SPI_SOFTWARE_NSS)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_SSM;
            *controlRegister1[Config[i].Channel] |= SPI_CR1_SSI;
        }
        else if(Config[i].SlaveSelect == SPI_HARDWARE_NSS_ENABLED)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_SSM;
            *controlRegister2[Config[i].Channel] |= SPI_CR2_SSOE;
        }
        else if(Config[i].SlaveSelect == SPI_HARDWARE_NSS_DISABLED)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_SSM;
            *controlRegister2[Config[i].Channel] &=~ SPI_CR2_SSOE;
        }
        else
        {
            assert(Config[i].SlaveSelect < SPI_MAX_NSS);
        }

        /**Set the frame format of the device*/
        if(Config[i].FrameFormat == SPI_MSB)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_LSBFIRST;
        }
        else if(Config[i].FrameFormat == SPI_LSB)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_LSBFIRST;
        }
        else
        {
            assert(Config[i].FrameFormat < SPI_MAX_FF);
        }

        /**Set the data transfer type of the device*/
        if(Config[i].TypeTransfer == SPI_RECEIVE_MODE)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_RXONLY;
        }
        else if(Config[i].TypeTransfer == SPI_FULL_DUPLEX)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_RXONLY;
        }
        else
        {
            assert(Config[i].TypeTransfer < SPI_MAX_DF);
        }

        /**Set the data frame format (size) of the device*/
        if(Config[i].DataSize == SPI_8BITS)
        {
            *controlRegister1[Config[i].Channel] &=~ SPI_CR1_DFF;
        }
        else if(Config[i].DataSize == SPI_16BITS)
        {
            *controlRegister1[Config[i].Channel] |= SPI_CR1_DFF;
        }
        else
        {
            assert(Config[i].DataSize < SPI_MAX_BITS);
        }

        /**Enable the SPI module*/
        *controlRegister1[Config[i].Channel] |= SPI_CR1_SPE;
    }

}

/*****************************************************************************
 * Function: SPI_Transfer()
*//**
 *\b Description:
 * This function is used to initialize a data transfer on the SPI bus. This 
 * function is used to send data to a slave device specified by the 
 * SpiTransferConfig_t structure, which contains the channel, size, and data.
 * 
 * PRE-CONDITION: The MCU clocks must be configured and enabled. <br>
 * PRE-CONDITION: SPI_Init must be called with valid configuration data. <br>
 * PRE-CONDITION: SpiTransferConfig_t needs to be populated. <br>
 * PRE-CONDITION: The Channel is within the maximum SpiChannel_t. <br>
 * PRE-CONDITION: The size is greater than 0. <br>
 * PRE-CONDITION: The data is not NULL. <br>
 * 
 * POST-CONDITION: Data transferred based on configuration. <br>
 * 
 * @param[in] SpiTransferConfig A pointer to a structure containing the
 * channel, size, and data to be read.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 * uint16_t data[] = {0x56};
 * SpiTransferConfig_t TransferConfig =
 * {
 *     .Channel = SPI_CHANNEL1,
 *     .size = sizeof(data)/sizeof(data[0]),
 *     .data = data
 * };
 * SPI_Transfer(&TransferConfig);
 * @endcode
 * 
 * @see SPI_configGet
 * @see SPI_getConfigSize
 * @see SPI_Init
 * @see SPI_Transfer
 * @see SPI_RegisterWrite
 * @see SPI_RegisterRead
 * @see SPI_CallbackRegister
 * 
 ****************************************************************************/
void SPI_transfer(const SpiTransferConfig_t * const TransferConfig)
{
    /* Prevent to assign a value out of the range of the channel*/
    assert(TransferConfig->Channel < SPI_MAX_CHANNEL);
    /* Prevent to use an empty data size*/
    assert(TransferConfig->size > 0);
    /* Prevent to use an empty data transfer*/
    assert(TransferConfig->data != NULL);

    for (uint16_t i = 0; i < TransferConfig->size; i++)
    {
        /* Wait until TXE is set (buffer empty)*/
        while(!(*statusRegister[TransferConfig->Channel] & SPI_SR_TXE))
        {
            asm("nop");
        }
        *dataRegister[TransferConfig->Channel] = TransferConfig->data[i];
    }

    /* Wait until TXE is set to ensure the bus is empty*/
    while(!(*statusRegister[TransferConfig->Channel] & SPI_SR_TXE))
    {
        asm("nop");
    }

    /* Wait until bus is not busy to reset*/
    while(*statusRegister[TransferConfig->Channel] & SPI_SR_BSY)
    {
        asm("nop");
    }

    /* Clear OVR bit (Overrun flag) in case of error*/
    uint16_t clearingFlag;
    clearingFlag = *dataRegister[TransferConfig->Channel];
    clearingFlag = *statusRegister[TransferConfig->Channel];
}

/*****************************************************************************
 * Function: SPI_Receive()
*//**
 *\b Description:
 * This function is used to initialize a data reception on the SPI bus. This 
 * function is used to receive data specified by the  SpiTransferConfig_t 
 * structure, which contains the channel, size, and data.
 * 
 * PRE-CONDITION: The MCU clocks must be configured and enabled. <br>
 * PRE-CONDITION: SPI_Init must be called with valid configuration data. <br>
 * PRE-CONDITION: SpiTransferConfig_t needs to be populated. <br>
 * PRE-CONDITION: The Channel is within the maximum SpiChannel_t. <br>
 * PRE-CONDITION: The size is greater than 0. <br>
 * PRE-CONDITION: The data is not NULL. <br>
 * 
 * POST-CONDITION: Data transferred based on configuration.
 * 
 * @param[in] SpiTransferConfig A pointer to a structure containing the 
 * channel, size, and data to be read.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
* uint16_t rxdata[1];
 * SpiTransferConfig_t ReceiveConfig =
 * {
 *     .Channel = SPI_CHANNEL1,
 *     .size = sizeof(rxdata)/sizeof(rxdata[0]),
 *     .data = rxdata
 * };
 * SPI_receive(&ReceiveConfig);
 * @endcode
 * 
 * @see SPI_configGet
 * @see SPI_getConfigSize
 * @see SPI_Init
 * @see SPI_Transfer
 * @see SPI_RegisterWrite
 * @see SPI_RegisterRead
 * @see SPI_CallbackRegister
 * 
 ****************************************************************************/
void SPI_receive(const SpiTransferConfig_t * const TransferConfig)
{
    /* Prevent to assign a value out of the range of the channel*/
    assert(TransferConfig->Channel < SPI_MAX_CHANNEL);
    /* Prevent to use an empty data size*/
    assert(TransferConfig->size > 0);
    /* Prevent to use an empty data transfer*/
    assert(TransferConfig != NULL);

    for (uint8_t i = 0; i < TransferConfig->size; i++)
    {
        /* Send dummy data (Recommended).*/
        *dataRegister[TransferConfig->Channel] = 0;
        /* Wait for RXEN flag to be sent*/
        while(!(*statusRegister[TransferConfig->Channel] & SPI_SR_RXNE))
        {
            asm("nop");
        }
        /* Read the data*/
        TransferConfig->data[i] = *dataRegister[TransferConfig->Channel];
    }
}

/*****************************************************************************
 * Function: SPI_registerWrite()
*//**
 *\b Description:
 * This function is used to directly address and modify a SPI register.
 * The function should be used to access specialized functionality in 
 * the SPI peripheral that is not exposed by any other function of the
 * interface.
 * 
 * PRE-CONDITION: Address is within the boundaries of the SPI register
 * address space. <br>
 * 
 * POST-CONDITION: The register located at address with be updated with
 * value. <br>
 * 
 * @param[in]   address is a register address within the SPI peripheral
 *              map.
 * @param[in]   value is the value to set the SPI register. 
 * 
 * @return void
 * 
 * \b Example
 * @code
 *  SPI_registerWrite(0x1000, 0x15);
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
****************************************************************************/  
void SPI_registerWrite(uint32_t address, uint32_t value)
{
    volatile uint32_t * const registerPointer = (uint32_t*)address;
    *registerPointer = value;
}

/*****************************************************************************
 * Function: SPI_registerRead()
*//**
 *\b Description:
 * This function is used to directly address a SPI register. The 
 * function should be used to access specialized functionality in the 
 * SPI peripheral that is not exposed by any other function of the 
 * interface.
 * 
 * PRE-CONDITION: Address is within the boundaries of the SPI register 
 * address space. <br>
 * 
 * POST-CONDITION: The value stored in the register is returned to the 
 * caller. <br>
 * 
 * @param[in]   address is the address of the SPI register to read.
 * 
 * @return  The current value of the SPI register.
 * 
 * \b Example:
 * @code
 * type spiValue = SPI_registerRead(0x1000);
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
 ****************************************************************************/
uint16_t SPI_registerRead(uint32_t address)
{
    volatile uint16_t * const registerPointer = (uint16_t *)address;

    return *registerPointer;
}