/**
 * @file spi_cfg.h
 * @author Jose Luis Figueroa 
 * @brief This module contains interface definitions for the SPI
 * configuration. This is the header file for the definition of the
 * interface for retrieving the Serial Peripheral interface
 * configuration table.
 * @version 1.0
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
 */
#ifndef SPI_CFG_H_
#define SPI_CFG_H_

/**********************************************************************
* Preprocessor Constants
**********************************************************************/
/** 
 * Defines the number of ports on the processor.
 */
#define SPI_PORTS_NUMBER 4U

/** 
 * Set the value according with the number of Serial Peripheral 
 * interface channels to be used.
*/
#define SPI_CHANNELS_NUMBER 1

/**********************************************************************
* Typedefs
**********************************************************************/
/**
 * Define the SPI channels on the MCU device. It is used to specify
 * SPI channel to configure the register map.
*/
typedef enum
{
    SPI_CHANNEL1,   /**< SPI Channel 1*/
    SPI_CHANNEL2,   /**< SPI Channel 2*/
    SPI_CHANNEL3,   /**< SPI Channel 3*/
    SPI_CHANNEL4,   /**< SPI Channel 4*/
    SPI_MAX_CHANNEL /**< Maximum SPI Channel*/
}SpiChannel_t;

/**
 * Define the bus mode according to clock polarity and clock phase.
 */
typedef enum
{
    SPI_MODE0,      /**< Mode 0 (CPOL=0 and CPHA=0)*/      
    SPI_MODE1,      /**< Mode 1 (CPOL=0 and CPHA=1)*/ 
    SPI_MODE2,      /**< Mode 2 (CPOL=1 and CPHA=0)*/ 
    SPI_MODE3,      /**< Mode 3 (CPOL=1 and CPHA=1)*/ 
    SPI_MAX_MODE    /**< Maximum mode */
}SpiMode_t;

/**
 * Define the hierarchy of the device. Set the device as slave or 
 * master.
 */
typedef enum
{
    SPI_SLAVE,          /**< Slave configuration*/
    SPI_MASTER,         /**< Master configuration*/
    SPI_MAX_HIERARCHY   /**< Maximum hierarchy*/
}SpiHierarchy_t;

/**
 * Define the baud rate control.
*/
typedef enum
{
    SPI_FPCLK2,     /**< Baud rate divided by 2*/
    SPI_FPCLK4,     /**< Baud rate divided by 4*/
    SPI_FPCLK8,     /**< Baud rate divided by 8*/
    SPI_FPCLK16,    /**< Baud rate divided by 16*/
    SPI_FPCLK32,    /**< Baud rate divided by 32*/
    SPI_FPCLK64,    /**< Baud rate divided by 64*/
    SPI_FPCLK128,   /**< Baud rate divided by 128*/
    SPI_FPCLK256,   /**< Baud rate divided by 256*/
    SPI_MAX_FPCLK   /**< Maximum baud rate*/
}SpiBaudRate_t;

/**
 * Define the slave select pin management (NSS input). The slave  
 * selector could be configured by software or hardware.
 * SW_NSS: Slave select is driven internally. NSS pin remains free.
 * HW_NSS_ENABLED: Device operates in master mode (NSS pin is used).
 * HW_NSS_DISABLED: Device operates in slave mode (NSS pin is used).
 */
typedef enum
{
    SPI_SOFTWARE_NSS,           /**< Software NSS pin management*/
    SPI_HARDWARE_NSS_ENABLED,   /**< Hardware NSS pin management (Master)*/
    SPI_HARDWARE_NSS_DISABLED,  /**< Hardware NSS pin management (slave)*/
    SPI_MAX_NSS             /**< Maximum NSS input*/
}SpiSlaveSelect_t;

/**
 * Define the frame format. Choose the direction of the data frame 
 * to be sent.
 */
typedef enum
{
    SPI_MSB,    /**< Most significant bit transmitted first*/
    SPI_LSB,    /**< Less significant bit transmitted first*/
    SPI_MAX_FF  /**< Maximum frame format*/
}SpiFrameFormat_t;

/**
 * Define the type of data transfer.  
 */
typedef enum
{
    SPI_FULL_DUPLEX,    /**< Set full duplex communication*/
    SPI_RECEIVE_MODE,   /**< Set Receive only communication*/
    SPI_MAX_DF          /**< Set the maximum data transfer*/
}SpiTypeTransfer_t;

/**
 * Define the data frame format. Set bits number of data.
 */
typedef enum
{
    SPI_8BITS,      /**< 8 bits data is selected for communication*/
    SPI_16BITS,     /**< 16 bits data is selected for communication*/
    SPI_MAX_BITS    /**< Maximum number of bits*/
}SpiDataSize_t;

/**
 * Defines the Serial Peripheral Interface configuration table's 
 * elements that are used by Spi_Init to configure the SPI peripheral.
 */
typedef struct
{
    SpiChannel_t Channel;           /**< The SPI channel */
    SpiMode_t Mode;                 /**< Mode 0,1,2, and 3 */
    SpiHierarchy_t Hierarchy;       /**< Slave and Master */
    SpiBaudRate_t BaudRate;         /**< FPCLK2 - Max FPCLK */
    SpiSlaveSelect_t SlaveSelect;   /**< NSS pin management */
    SpiFrameFormat_t FrameFormat;   /**< MSB and LSB */
    SpiTypeTransfer_t TypeTransfer; /**< Full duplex and Receive mode*/
    SpiDataSize_t DataSize;         /**< 8 bits and 16 bits*/
}SpiConfig_t;


/**********************************************************************
* Function Prototypes
**********************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

const SpiConfig_t * const SPI_ConfigGet(void);

#ifdef __cplusplus
} //extern "C"
#endif

#endif  /*SPI_H_*/
