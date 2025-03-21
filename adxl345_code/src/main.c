/**
 * @file main.c
 * @author Jose Luis Figueroa
 * @brief Implement the adxl345 accelerometer using Nucleo-F401RE. 
 * @version 1.1
 * @date 2025-03-21
 * @note The debug mode is used to test and review the behavior of the
 * ADXL345 accelerometer.
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
 * 
*/

/*****************************************************************************
* Includes
*****************************************************************************/
#include <adxl345.h>

/*****************************************************************************
* Variable Definitions
*****************************************************************************/
int16_t x, y, z;
float xg, yg, zg;
uint16_t dataAxis[6];

int main (void)
{
    /*Enable clock access to GPIOA and SPI1*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    /*Get the address of the configuration table for DIO*/
    const DioConfig_t * const DioConfig = DIO_configGet();
    /*Get the size of the configuration table*/
    size_t configSizeDio = DIO_configSizeGet();
    /* Initialize the DIO pins according to the configuration table*/
    DIO_init(DioConfig, configSizeDio);

    /*Get the address of the configuration table for SPI*/
    const SpiConfig_t * const SpiConfig = SPI_ConfigGet();
    /*Get the size of the configuration table*/
    size_t configSizeSpi = SPI_configSizeGet();
    /*Initialize the SPI channel according to the configuration table*/
    SPI_init(SpiConfig, configSizeSpi);


    /*ADXL345 configuration data*/
    Adxl345Config_t Adxl345Config =
    {
        .Channel = SPI_CHANNEL1,
        .Port = DIO_PA,
        
        .Pin = DIO_PA4
    };

    /*Initialize accelerometer*/
    ADXL345_init(&Adxl345Config);

    while(1)
    {
        /*Read the axis values*/
        ADXL345_read(&Adxl345Config, DATA_START_R, 
        sizeof(dataAxis)/sizeof(dataAxis[0]), &dataAxis[0]);

        /*Get x, y, z. Order the bytes (x0 and x1) on one 16 bits variable*/
        x = ((dataAxis[1]<<8) | dataAxis[0]);
        y = ((dataAxis[3]<<8) | dataAxis[2]);
        z = ((dataAxis[5]<<8) | dataAxis[4]);

        /*Multiply for four g scale factor*/
        xg = (x * 0.0078);
        yg = (y * 0.0078);
        zg = (z * 0.0078);
    }

}