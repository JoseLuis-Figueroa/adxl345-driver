/**
 * @file dio.c
 * @author Jose Luis Figueroa
 * @brief The implementation for the DIO driver.
 * @version 1.1
 * @date 2025-03-19
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
 * 
 */
/*****************************************************************************
* Module Includes
*****************************************************************************/
#include "dio.h"        /*For this modules definitions*/                                        

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
/* Defines a array of pointers to the GPIO port mode register */
static uint32_t volatile * const moderRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->MODER, (uint32_t*)&GPIOB->MODER, 
    (uint32_t*)&GPIOC->MODER, (uint32_t*)&GPIOD->MODER,
    (uint32_t*)&GPIOH->MODER
};

/* Defines a array of pointers to the GPIO port output type register. */
static uint32_t volatile * const otyperRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->OTYPER, (uint32_t*)&GPIOB->OTYPER,
    (uint32_t*)&GPIOC->OTYPER, (uint32_t*)&GPIOD->OTYPER, 
    (uint32_t*)&GPIOH->OTYPER
};

/* Define a array of pointers to the GPIO port output speed register. */
static uint32_t volatile * const ospeedrRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->OSPEEDR, (uint32_t*)&GPIOB->OSPEEDR,
    (uint32_t*)&GPIOC->OSPEEDR, (uint32_t*)&GPIOD->OSPEEDR, 
    (uint32_t*)&GPIOH->OSPEEDR
};

/* Defines a array of pointers to the GPIO port pull-up/pull-down register.*/
static uint32_t volatile * const pupdrRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->PUPDR, (uint32_t*)&GPIOB->PUPDR,
    (uint32_t*)&GPIOC->PUPDR, (uint32_t*)&GPIOD->PUPDR, 
    (uint32_t*)&GPIOH->PUPDR
};

/* Defines a array of pointers to the GPIO port input data register. */
static uint32_t volatile * const idrRegister[NUMBER_OF_PORTS] =  
{
    (uint32_t*)&GPIOA->IDR, (uint32_t*)&GPIOB->IDR, (uint32_t*)&GPIOC->IDR,
    (uint32_t*)&GPIOD->IDR, (uint32_t*)&GPIOH->IDR
};

/* Defines a array of pointers to the GPIO port output data register. */
static uint32_t volatile * const odrRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->ODR, (uint32_t*)&GPIOB->ODR, (uint32_t*)&GPIOC->ODR, 
    (uint32_t*)&GPIOD->ODR, (uint32_t*)&GPIOH->ODR
};

/* Defines a array of pointers to the GPIO alternate function low register.
 * This is compound for two 32 bits registers.
*/
static uint32_t volatile * const afrRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->AFR[0], (uint32_t*)&GPIOB->AFR[0], 
    (uint32_t*)&GPIOC->AFR[0], (uint32_t*)&GPIOD->AFR[0], 
    (uint32_t*)&GPIOH->AFR[0]
};

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: DIO_init()
*//**
*\b Description:
 * This function is used to initialize the DIO based on the configuration  
 * table defined in dio_cfg module.
 * 
 * PRE-CONDITION: The MCU clocks must be configured and enabled. <br>
 * PRE-CONDITION: Configuration table needs to be populated (sizeof > 0) <br>
 * PRE-CONDITION: NUMBER_OF_PORTS > 0 <br>
 * PRE-CONDITION: The setting is within the maximum values (DIO_MAX). <br>
 * 
 * POST-CONDITION: The DIO peripheral is set up with the configuration 
 * settings.
 * 
 * @param[in]   Config is a pointer to the configuration table that contains 
 *               the initialization for the peripheral.
 * @param[in]   configSize is the size of the configuration table.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 * const Dio_ConfigType_t * const DioConfig = DIO_configGet();
 * size_t configSize = DIO_configSizeGet();
 * 
 * DIO_Init(DioConfig, configSize);
 * @endcode
 * 
 * @see DIO_configGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * 
*****************************************************************************/
void DIO_init(const DioConfig_t * const Config, size_t configSize)
{
    /* Loop through all the elements of the configuration table. */
    for(uint8_t i=0; i<configSize; i++)
    {
        /* Prevent to assign a value out of the range of the port and pin.
         * The registers arrays are limited to the NUMBER_OF_PORTS, higher 
         * value can cause a memory violation.
        */
        assert(Config[i].Port < DIO_MAX_PORT);
        assert(Config[i].Pin < DIO_MAX_PIN);

        /* 
         * Set the mode of the Dio pin on the GPIO port mode register. 
         * Multiply the pin number (Config[i].Pin) by two as MODER uses two 
         * bits to configure one pin.
        */
        if(Config[i].Mode == DIO_INPUT)
        {
            *moderRegister[Config[i].Port] &=~ (1UL<<(Config[i].Pin*2));
            *moderRegister[Config[i].Port] &=~ (2UL<<(Config[i].Pin*2));

        }
        else if (Config[i].Mode == DIO_OUTPUT)
        {
            *moderRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *moderRegister[Config[i].Port] &=~ (2UL<<(Config[i].Pin*2));
        }
        else if (Config[i].Mode == DIO_FUNCTION)
        {
            *moderRegister[Config[i].Port] &=~ (1UL<<(Config[i].Pin*2));
            *moderRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
        }
        else if (Config[i].Mode == DIO_ANALOG)
        {
            *moderRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *moderRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
        }
        else
        {
            assert(Config[i].Mode < DIO_MAX_MODE);
        }

        /*
         * Set the output type of the Dio pin on the GPIO port output type 
         * register.
         */
        if(Config[i].Type == DIO_PUSH_PULL)
        {
            *otyperRegister[Config[i].Port] &= ~(1UL<<Config[i].Pin);
        }
        else if (Config[i].Type == DIO_OPEN_DRAIN)
        {
            *otyperRegister[Config[i].Port] |= (1UL<<Config[i].Pin);
        }
        else
        {
            assert(Config[i].Type < DIO_MAX_TYPE);
        }

        /*
         * Set the speed of the Dio pin on the GPIO port output speed 
         * register. Multiply the pin number (Config[i].Pin) by two as 
         * OSPEEDR uses two bits to configure one pin. 
         */
        if(Config[i].Speed == DIO_LOW_SPEED)
        {
            *ospeedrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*2));
            *ospeedrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*2));
        }
        else if (Config[i].Speed == DIO_MEDIUM_SPEED)
        {
            *ospeedrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *ospeedrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*2));
        }
        else if (Config[i].Speed == DIO_HIGH_SPEED)
        {
            *ospeedrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*2));
            *ospeedrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
        }
        else if(Config[i].Speed == DIO_VERY_SPEED)
        {
            *ospeedrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *ospeedrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
        }
        else
        {
            assert(Config[i].Speed < DIO_MAX_SPEED);
        }

        /*
         * Set the internal resistor of the Dio pin on the GPIO port 
         * pull-up/pull-down register. Multiply the pin number 
         * (Config[i].Pin) by two as PUPDR uses two bits to configure 
         * one pin. 
        */
       if(Config[i].Resistor == DIO_NO_RESISTOR)
       {
            *pupdrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*2));
            *pupdrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*2));
       }
       else if (Config[i].Resistor == DIO_PULLUP)
       {
            *pupdrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *pupdrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*2));
       }
       else if (Config[i].Resistor == DIO_PULLDOWN)
       {
            *pupdrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*2));
            *pupdrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
       }
       else
       {
            assert(Config[i].Resistor < DIO_MAX_RESISTOR);
       }

        /*
         * Set the alternate function of the Dio pin on the GPIO alternate 
         * function. Multiply the pin number (Config[i].Pin) by four as AFR 
         * uses four bits to configure one pin. 
        */
       if(Config[i].Function == DIO_AF0)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF1)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF2)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF3)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF4)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF5)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF6)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF7)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF8)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF9)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF10)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF11)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF12)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF13)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF14)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF15)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else
       {
            assert(Config[i].Function < DIO_MAX_FUNCTION);
       }

    }
}

/*****************************************************************************
 * Function: DIO_pinRead()
*//**
 *\b Description:
 * This function is used to reads the state of a specified pin.
 * This function reads the state of a digital input/output pin specified by
 * the DioPinConfig_t structure, which contains the port and pin information.
 * 
 * PRE-CONDITION: The pin is configured as INPUT <br>
 * PRE-CONDITION: The pin is configured as GPIO <br>
 * PRE-CONDITION: DioPinConfig_t needs to be populated (sizeof > 0) <br>
 * PRE-CONDITION: The Port is within the maximum DioPort_t. <br>
 * PRE-CONDITION: The Pin is within the maximum DioPin_t. 
 * definition. <br>
 * 
 * POST-CONDITION: The channel state is returned. <br>
 * 
 * @param[in] PinConfig A pointer to a structure containing the port and pin 
 * to be read.
 * 
 * @return    DioPinState_t The state of the pin (high or low).
 * 
 * \b Example:
 * @code
 * const DioPinConfig_t  UserButton1= 
 * {
 *      .Port = DIO_PC, 
 *      .Pin = DIO_PC13
 * };
 *  bool pin = DIO_pinRead(&UserButton1);
 * @endcode
 * 
 * @see DIO_ConfigGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * 
**********************************************************************/
DioPinState_t DIO_pinRead(const DioPinConfig_t * const PinConfig)
{
    /* Prevent to assign a value out of the range of the port and pin.
     * The registers arrays are limited to the NUMBER_OF_PORTS, higher 
     * value can cause a memory violation.
    */
    assert(PinConfig->Port < DIO_MAX_PORT);
    assert(PinConfig->Pin < DIO_MAX_PIN);

    /* Read the port associated with the desired pin */
    uint16_t portState = *idrRegister[PinConfig->Port];
    /* Determinate the Port bit associated with this pin*/
    uint16_t pinMask = (1UL<<(PinConfig->Pin));

    return ((portState & pinMask) ? DIO_HIGH : DIO_LOW); 
}

/**********************************************************************
 * Function: DIO_pinWrite()
*//**
 *\b Description:
 * This function is used to write the state of a pin as either logic 
 * high or low. it reads the state of a digital input/output pin 
 * specified by the DioPinConfig_t structure and the DioPinState_t to 
 * define the desired state, which contains the port and pin 
 * information.
 * 
 * PRE-CONDITION: The pin is configured as OUTPUT <br>
 * PRE-CONDITION: The pin is configured as GPIO <br>
 * PRE-CONDITION: DioPinConfig_t needs to be populated (sizeof > 0) <br>
 * PRE-CONDITION: The Port is within the maximum DioPort_t. <br>
 * PRE-CONDITION: The Pin is within the maximum DioPin_t. <br>
 * PRE-CONDITION: The State is within the maximum DioPinState_t. <br>
 * 
 * POST-CONDITION: The channel state is Stated. <br>
 * 
 * @param[in]   pinConfig A pointer to a structure containing the port 
 *              and pin to be written.
 * @param[in]   State is HIGH or LOW as defined in the DioPinState_t 
 *              enum. 
 * 
 * @return      void
 * 
 * \b Example:
 * @code
 * const DioPinConfig_t  UserLED1= 
 * {
 *      .Port = DIO_PA, 
 *      .Pin = DIO_PA5
 * };
 * const DioPinConfig_t  UserLED2= 
 * {
 *      .Port = DIO_PA, 
 *      .Pin = DIO_PA6
 * };
 * DIO_pinWrite(&UserLED1, LOW);    //Set the pin low
 * DIO_pinWrite(&UserLED2, HIGH);   //Set the pin high
 * @endcode
 * 
 * @see DIO_ConfigGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * 
 **********************************************************************/
void DIO_pinWrite(const DioPinConfig_t * const PinConfig, DioPinState_t State)
{
    /* Prevent to assign a value out of the range of the port and pin.
     * The registers arrays are limited to the NUMBER_OF_PORTS, higher 
     * value can cause a memory violation.
    */
    assert(PinConfig->Port < DIO_MAX_PORT);
    assert(PinConfig->Pin < DIO_MAX_PIN);

    if(State == DIO_HIGH)
    {
        *odrRegister[PinConfig->Port] |= (1UL<<(PinConfig->Pin));
    }
    else if (State == DIO_LOW)
    {
        *odrRegister[PinConfig->Port] &= ~(1UL<<(PinConfig->Pin));
    }
    else
    {
        assert(State < DIO_PIN_STATE_MAX);
    }
}

/**********************************************************************
 * Function: DIO_pinToggle()
*//**
 *\b Description:
 * This function is used to toggle the current state of a pin. 
 * This function reads the state of a digital input/output pin 
 * specified by the DioPinConfig_t structure, which contains the port 
 * and pin information.
 * 
 * PRE-CONDITION: The channel is configured as output <br>
 * PRE-CONDITION: The channel is configured as GPIO <br>
 * PRE-CONDITION: DioPinConfig_t needs to be populated (sizeof > 0) <br>
 * PRE-CONDITION: The Port is within the maximum DioPort_t. <br>
 * PRE-CONDITION: The Pin is within the maximum DioPin_t. <br>
 *
 * POST-CONDITION: The channel state is toggled. <br>
 * 
 * @param[in]   pinConfig A pointer to a structure containing the port 
 *              and pin to be toggled.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 * const DioPinConfig_t  UserLED1= 
 * {
 *      .Port = DIO_PA, 
 *      .Pin = DIO_PA5
 * };
 * DIO_pinToggle(&UserLED1);
 * @endcode
 * 
 * @see DIO_ConfigGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * 
 **********************************************************************/
void DIO_pinToggle(const DioPinConfig_t * const PinConfig)
{
    /* Prevent to assign a value out of the range of the port and pin.
     * The registers arrays are limited to the NUMBER_OF_PORTS, higher 
     * value can cause a memory violation.
    */
    assert(PinConfig->Port < DIO_MAX_PORT);
    assert(PinConfig->Pin < DIO_MAX_PIN);

    *odrRegister[PinConfig->Port] ^= (1UL<<(PinConfig->Pin));
}

/**********************************************************************
 * Function: DIO_registerWrite()
*//**
 *\b Description:
 * This function is used to directly address and modify a GPIO register.
 * The function should be used to access specialized functionality in 
 * the DIO peripheral that is not exposed by any other function of the
 * interface.
 * 
 * PRE-CONDITION: Address is within the boundaries of the DIO register
 * address space. <br>
 * 
 * POST-CONDITION: The register located at address with be updated with
 * value. <br>
 * 
 * @param[in]   address is a register address within the DIO peripheral
 *              map.
 * @param[in]   value is the value to set the DIO register. 
 * 
 * @return void
 * 
 * \b Example
 * @code
 *  DIO_registerWrite(0x1000, 0x15);
 * @endcode
 * 
 * @see DIO_ConfigGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * 
**********************************************************************/ 
void DIO_registerWrite(uint32_t address, uint32_t value)
{
    volatile uint32_t * const registerPointer = (uint32_t*)address;
    *registerPointer = value;
}

/**********************************************************************
 * Function: DIO_registerRead()
*//**
 *\b Description:
 * This function is used to directly address a Dio register. The 
 * function should be used to access specialized functionality in the 
 * Dio peripheral that is not exposed by any other function of the 
 * interface.
 * 
 * PRE-CONDITION: Address is within the boundaries of the Dio register 
 * address space. <br>
 * 
 * POST-CONDITION: The value stored in the register is returned to the 
 * caller. <br>
 * 
 * @param[in]   address is the address of the Dio register to read.
 * 
 * @return  The current value of the Dio register.
 * 
 * \b Example:
 * @code
 * type dioValue = DIO_registerRead(0x1000);
 * @endcode
 * 
 * @see DIO_ConfigGet
 * @see DIO_configSizeGet
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 *
 **********************************************************************/ 
uint32_t DIO_registerRead(uint32_t address)
{
    volatile uint32_t * const registerPointer = (uint32_t*)address;

    return *registerPointer;
}