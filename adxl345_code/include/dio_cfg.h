/**
 * @file dio_cfg.h
 * @author Jose Luis Figueroa
 * @brief This module contains interface definitions for the Dio 
 * configuration. This is the header file for the definition of the
 * interface for retrieving the digital input/output configuration table.
 * @version 1.0
 * @date 2023-03-16
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
 */
#ifndef DIO_CFG_H_
#define DIO_CFG_H_

/*****************************************************************************
* Includes
*****************************************************************************/

/*****************************************************************************
* Preprocessor Constants
*****************************************************************************/
/**
 * Defines the number of ports on the processor.
 */
#define NUMBER_OF_PORTS 5U

/** Set the value according with the number of digital input/output peripheral
 * channel (pins) used.
*/
#define NUMBER_DIGITAL_PINS 4

/*****************************************************************************
* Typedefs
*****************************************************************************/
/**
 * Defines the possible states for a digital output pin.
*/
typedef enum
{
    DIO_LOW,            /**< Defines digital state ground*/
    DIO_HIGH,           /**< Defines digital state power*/
    DIO_PIN_STATE_MAX   /**< Defines the maximum digital state*/
}DioPinState_t;

/**
 * Define the ports contained on the MCU device. It is used to identify the
 * specific port GPIO to configure the register map.
 */
typedef enum
{
    DIO_PA,         /**< Port A*/
    DIO_PB,         /**< Port B*/
    DIO_PC,         /**< Port C*/
    DIO_PD,         /**< Port D*/
    DIO_PH,         /**< Port H*/
    DIO_MAX_PORT    /**< Defines the maximum Port*/
}DioPort_t;


/**
 * Defines all the pins contained on the MCU device. It is used to set a 
 * specific bit on the ports.
 */
typedef enum
{
    DIO_PA0,        /**< PA0 */
    DIO_PA1,        /**< PA1 */
    DIO_PA2,        /**< PA2 */
    DIO_PA3,        /**< PA3 */
    DIO_PA4,        /**< PA4 */
    DIO_PA5,        /**< PA5 */
    DIO_PA6,        /**< PA6 */
    DIO_PA7,        /**< PA7 */
    DIO_PA8,        /**< PA8 */
    DIO_PA9,        /**< PA9 */
    DIO_PA10,       /**< PA10 */
    DIO_PA11,       /**< PA11 */
    DIO_PA12,       /**< PA12 */
    DIO_PA13,       /**< PA13 SWD (NC)*/
    DIO_PA14,       /**< PA14 SWD (NC)*/
    DIO_PA15,       /**< PA15 */
    DIO_PB0 = 0,    /**< PB0 */
    DIO_PB1,        /**< PB1 */
    DIO_PB2,        /**< PB2 */
    DIO_PB3,        /**< PB3 */
    DIO_PB4,        /**< PB4 */
    DIO_PB5,        /**< PB5 */
    DIO_PB6,        /**< PB6 */
    DIO_PB7,        /**< PB7 */
    DIO_PB8,        /**< PB8 */
    DIO_PB9,        /**< PB9 */
    DIO_PB10,       /**< PB10*/
    DIO_PB12 = 12,   /**< PB12 */
    DIO_PB13,       /**< PB13 */
    DIO_PB14,       /**< PB14 */
    DIO_PB15,       /**< PB15 */
    DIO_PC0 = 0,    /**< PC0 */
    DIO_PC1,        /**< PC1 */
    DIO_PC2,        /**< PC2 */
    DIO_PC3,        /**< PC3 */
    DIO_PC4,        /**< PC4 */
    DIO_PC5,        /**< PC5 */
    DIO_PC6,        /**< PC6 */
    DIO_PC7,        /**< PC7 */
    DIO_PC8,        /**< PC8 */
    DIO_PC9,        /**< PC9 */
    DIO_PC10,       /**< PC10*/
    DIO_PC11,       /**< PC11 */
    DIO_PC12,       /**< PC12 */
    DIO_PC13,       /**< PC13 Push button*/
    DIO_PC14,       /**< PC14 OSC32_IN*/
    DIO_PC15,       /**< PC15 OSC32_OUT*/
    DIO_PD2 = 2,    /**< PD2 */
    DIO_PH0 = 0,    /**< PH0 OSC_IN*/
    DIO_PH1,        /**< PH1 OSC_OUT*/
    DIO_MAX_PIN = 16/**< Defines the maximum pin value*/
}DioPin_t;

/**
 * Defines the mode of the Dio pin as an input, output, alternate function 
 * and analog.
 */
typedef enum
{
    DIO_INPUT,      /**< Input mode */
    DIO_OUTPUT,     /**< General purpose Output mode */
    DIO_FUNCTION,   /**< Alternate function mode */
    DIO_ANALOG,     /**< Analog mode */
    DIO_MAX_MODE    /**< Defines the maximum mode */
}DioMode_t;

/**
 * Define the output type of the Input/output port. 
 */
typedef enum
{
    DIO_PUSH_PULL,      /**< Enable output push-pull */        
    DIO_OPEN_DRAIN,     /**< Enable output open-drain */
    DIO_MAX_TYPE        /**< Defines the maximum output type */
}DioType_t;

/**
 * Defines the output speed settings available
 */
typedef enum
{
    DIO_LOW_SPEED,      /**< Low speed is configured on the pin */
    DIO_MEDIUM_SPEED,   /**< Medium speed is configured on the pin */
    DIO_HIGH_SPEED,     /**< High speed is configured on the pin */
    DIO_VERY_SPEED,     /**< Very speed is configured on the pin */
    DIO_MAX_SPEED       /**< Defines the maximum output speed */
}DioSpeed_t;

/**
 * Defines the possible states of the channel pull-ups.
 */
typedef enum
{
    DIO_NO_RESISTOR,    /**< Used to disable the internal resistor*/
    DIO_PULLUP,         /**< Used to enable the internal pull-up*/
    DIO_PULLDOWN,       /**< Used to enable the internal pull-down*/
    DIO_MAX_RESISTOR    /**< Defines the maximum resistor value */
}DioResistor_t;

/**
 * Defines the possible DIO alternate function. A multiplexer is used to 
 * select the alternate function
 */
typedef enum
{
    DIO_AF0,        /**< Alternate function 0 */
    DIO_AF1,        /**< Alternate function 1 */
    DIO_AF2,        /**< Alternate function 2 */
    DIO_AF3,        /**< Alternate function 3 */
    DIO_AF4,        /**< Alternate function 4 */
    DIO_AF5,        /**< Alternate function 5 */
    DIO_AF6,        /**< Alternate function 6 */
    DIO_AF7,        /**< Alternate function 7 */
    DIO_AF8,        /**< Alternate function 8 */
    DIO_AF9,        /**< Alternate function 9 */
    DIO_AF10,       /**< Alternate function 10 */
    DIO_AF11,       /**< Alternate function 11 */
    DIO_AF12,       /**< Alternate function 12 */
    DIO_AF13,       /**< Alternate function 13 */
    DIO_AF14,       /**< Alternate function 14 */
    DIO_AF15,       /**< Alternate function 15 */
    DIO_MAX_FUNCTION/**< Defines the maximum function value */
}DioFunction_t;

/**
 * Defines the digital input/output configuration table's elements that are 
 * used by Dio_Init to configure the Dio peripheral.
 */
typedef struct 
{
    DioPort_t Port;             /**< The I/O port */
    DioPin_t Pin;               /**< The I/O pin */
    DioMode_t Mode;             /**< Input, Output, Function, or Analog */
    DioType_t Type;             /**< Push-pull or Open-drain */
    DioSpeed_t Speed;           /**< Low, Medium, High, very */
    DioResistor_t Resistor;     /**< Enabled or Disabled */
    DioFunction_t Function;     /**< Mux Function - Dio_Peri_Select */
}DioConfig_t;


/*****************************************************************************
* Function Prototypes
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

const DioConfig_t * const DIO_configGet(void);

#ifdef __cplusplus
} //extern "C"
#endif

#endif /*DIO_H_*/