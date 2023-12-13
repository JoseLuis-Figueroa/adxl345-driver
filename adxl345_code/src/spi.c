/**
 * @file spi.c
 * @author Jose Luis Figueroa 
 * @brief The implementation for the SPI.
 * @version 1.0
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
 */
/*****************************************************************************
* Includes
*****************************************************************************/
#include "spi.h"

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
 * This function is used to initialize the spi based on the configuration  
 * table defined in spi_cfg module.
 * 
 * PRE-CONDITION: The MCU clocks must be configured and enabled.
 * PRE-CONDITION: SPI pins should be configured using GPIO driver.
 * PRE-CONDITION: Configuration table needs to be populated (sizeof > 0) <br>
 *
 * POST-CONDITION: The peripheral is set up with the configuration settings.
 * 
 * @param[in]   Config is a pointer to the configuration table that contains 
 *               the initialization for the peripheral.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 *  const SpiConfig_t * const SpiConfig = SPI_configGet();
 *  SPI_init(DioConfig);
 * @endcode
 * 
 * @see SPI_configGet
 * @see SPI_init
 * @see SPI_transfer
 * @see SPI_registerWrite
 * @see SPI_registerRead
 * @see SPI_callbackRegister
 * 
*****************************************************************************/
void SPI_init(const SpiConfig_t * const Config)
{
    /**Loop through all the elements of the configuration table.*/
    for(uint8_t i=0; i<SPI_CHANNELS_NUMBER; i++)
    {
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
            printf("This mode does not exist\n");
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
            printf("This hierarchy does not exist\n");
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
            printf("This baud rate does not exist\n");
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
            printf("This slave select pin option does not exist\n");
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
            printf("This frame format does not exist\n");
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
            printf("This data transfer type does not exist");
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
            printf("This data size does not exist\n");
        }

        /**Enable the SPI module*/
        *controlRegister1[Config[i].Channel] |= SPI_CR1_SPE;
    }

}

/**********************************************************************
 * Function: SPI_transfer()
*//**
 *\b Description:
 * This function is used to initialize a data transfer on the SPI bus. 
 * 
 * PRE-CONDITION: SPI_Init must be called with valid configuration data.
 * PRE-CONDITION: SpiTransfer_t needs to be populated.
 * PRE-CONDITION: The MCU clocks must be configured and enabled.
 * 
 * POST-CONDITION: Data transferred based on configuration.
 * 
 * @param[in]   Channel is the SPI from SpiChannel_t used to transfer data.
 * @param[in]   data is the information to be sent.
 * @param[in]   size is data size.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 *  SPI_transfer(*data, size);
 * @endcode
 * 
 * @see SPI_configGet
 * @see SPI_init
 * @see SPI_transfer
 * @see SPI_registerWrite
 * @see SPI_registerRead
 * @see SPI_callbackRegister
 * 
 **********************************************************************/
void SPI_transfer(SpiChannel_t Channel, uint16_t *data, uint16_t size)
{
    for(uint16_t i=0; i<size; i++)
    {
        /* Wait until TXE is set (buffer empty)*/
        while(!(*statusRegister[Channel] & SPI_SR_TXE))
        {
            asm("nop");
        }
        *dataRegister[Channel] = data[i];
    }

    /* Wait until TXE is set to ensure the bus is empty*/
    while(!(*statusRegister[Channel] & SPI_SR_TXE))
    {
        asm("nop");
    }

    /* Wait until bus is not busy to reset*/
    while(*statusRegister[Channel] & SPI_SR_BSY)
    {
        asm("nop");
    }

    /* Clear OVR bit (Overrun flag) in case of error*/
    uint16_t clearingFlag;
    clearingFlag = *dataRegister[Channel];
    clearingFlag = *statusRegister[Channel];
}

/**********************************************************************
 * Function: SPI_receive()
*//**
 *\b Description:
 * This function is used to initialize a data reception on the SPI bus. 
 * 
 * PRE-CONDITION: SPI_Init must be called with valid configuration data.
 * PRE-CONDITION: SpiTransfer_t needs to be populated.
 * PRE-CONDITION: The MCU clocks must be configured and enabled.
 * 
 * POST-CONDITION: Data transferred based on configuration.
 * 
 * @param[in]   Channel is the SPI from SpiChannel_t used to transfer data.
 * @param[in]   data is the information to be sent.
 * @param[in]   size is data size.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 *  SPI_receive(*data, size);
 * @endcode
 * 
 * @see SPI_configGet
 * @see SPI_init
 * @see SPI_transfer
 * @see SPI_registerWrite
 * @see SPI_registerRead
 * @see SPI_callbackRegister
 * 
 **********************************************************************/
void SPI_receive(SpiChannel_t Channel, uint16_t *data, uint16_t size)
{
    while(size)
    {
        /* Send dummy data (Recommended).*/
        *dataRegister[Channel] = 0;
        /* Wait for RXEN flag to be sent*/
        while(!(*statusRegister[Channel] & SPI_SR_RXNE))
        {
            asm("nop");
        }
        /* Read the data*/
        *data++ = *dataRegister[Channel];
        size--;
    }
}

/**********************************************************************
 * Function: SPI_registerWrite()
*//**
 *\b Description:
 * This function is used to directly address and modify a SPI register.
 * The function should be used to access specialized functionality in 
 * the SPI peripheral that is not exposed by any other function of the
 * interface.
 * 
 * PRE-CONDITION: Address is within the boundaries of the SPI register
 * address space.
 * 
 * POST-CONDITION: The register located at address is updated with
 * value.
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
 * @see SPI_init
 * @see SPI_transfer
 * @see SPI_registerWrite
 * @see SPI_registerRead
 * @see SPI_callbackRegister
 * 
**********************************************************************/  
void SPI_registerWrite(uint32_t address, uint32_t value)
{
    volatile uint32_t * const registerPointer = (uint32_t*)address;
    *registerPointer = value;
}

/**********************************************************************
 * Function: SPI_registerRead()
*//**
 *\b Description:
 * This function is used to directly address a SPI register. The 
 * function should be used to access specialized functionality in the 
 * SPI peripheral that is not exposed by any other function of the 
 * interface.
 * 
 * PRE-CONDITION: Address is within the boundaries of the SPI register 
 * address space.
 * 
 * POST-CONDITION: The value stored in the register is returned to the 
 * caller.
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
 * @see SPI_init
 * @see SPI_transfer
 * @see SPI_registerWrite
 * @see SPI_registerRead
 * @see SPI_callbackRegister
 *
 **********************************************************************/
uint16_t SPI_registerRead(uint32_t address)
{
    volatile uint16_t * const registerPointer = (uint16_t *)address;

    return *registerPointer;
}