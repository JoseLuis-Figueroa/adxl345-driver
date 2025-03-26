# ADXL345 Driver

## Introduction

This repository contains the implementation of the ADXL345 accelerometer driver. The ADXL345 is a compact, low-power, three-axis accelerometer designed for precise acceleration measurement along the X, Y, and Z axes, making it suitable for various applications.

This project integrates reusable SPI and DIO drivers to configure and interface with the ADXL345. Developed in **embedded C (bare-metal)**, it directly interacts with the microcontroller's (MCU) register map, adhering to **industry-standard coding practices** such as **MISRA-C**.

## Project Overview

This repository focuses on creating **reusable drivers** and **Hardware Abstraction Layers (HALs)** for microcontrollers. The tools used in this project include:

- **Development Board:** _Nucleo-F401RE._
- **Logic Analyzer:** _KY-57._
- **IDE/Debugger:** _Visual Studio Code (PlatformIO extension)._
- **Compiler Toolchain:** _GNU ARM Embedded Toolchain._

## ADXL345 Driver Implementation

The implementation configures SPI1 as a master to communicate with the ADXL345 accelerometer. The SPI settings include:

- **Baud rate:** 4 kHz.
- **Comunication mode:** Full-duplex communication.
- **Data frame format:** 8-bit.
- **Bit transmission order:** Most significant bit (MSB) first.

A KY-57 logic analyzer is connected between the MCU and the ADXL345 for data reception and analysis. The following table outlines the SPI pin connections:

### **Pin Connections**

<div align="center">
<table>
  <tr>
    <th>SPI Connection</th>
    <th>Nucleo F401RE</th>
    <th>ADXL345</th>
    <th>Logic Analyzer</th>
  </tr>
  <tr>
    <td>NSS</td>
    <td>PA4</td>
    <td>CS</td>
    <td>CH4</td>
  </tr>
  <tr>
    <td>SCK</td>
    <td>PA5</td>
    <td>SCL</td>
    <td>CH1</td>
  </tr>
  <tr>
    <td>MISO</td>
    <td>PA6</td>
    <td>SDO</td>
    <td>CH2</td>
  </tr>
  <tr>
    <td>MOSI</td>
    <td>PA7</td>
    <td>SDA</td>
    <td>CH3</td>
  </tr>
</table>
</div>

## ADXL345 Initialization

The `ADXL345_init` function configures the ADXL345 with a **10-bit full resolution at ±4g** and sets the device into **measurement mode**.

```c
void ADXL345_init(const Adxl345Config_t * const Config)
{
    /*Set data format range to +-4g*/
    ADXL345_write(Config, DATA_FORMAT_R, FOUR_G);
    /*Reset all bits*/
    ADXL345_write(Config, POWER_CTL_R, RESET);
    /*Configure power control measure bit*/
    ADXL345_write(Config, POWER_CTL_R, SET_MEASURE);
}
```

### Configuration Steps

1. **Set the data range** to 10-bit full resolution at ±4g.
<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/2fc53852-9000-497e-adeb-a27cda8ed1a8" alt="First Write">
</p>

2. **Reset the accelerometer.**
<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/d2bc4880-6d65-4371-8289-6dfaf345e821" alt="Second Write">
</p>

3. **Set the device into measurement mode.**
<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/c92d7f24-4294-45a9-9f21-be3770457d51" alt="Second Write">
</p>

## Data Retrieval

The `ADXL345_read` function retrieves acceleration data from the X, Y, and Z axes and processes it to obtain the g-scale factor.
```c
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
```

### Data Reception

The image below displays the acceleration data received from the ADXL345 during movement.
<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/c8f30ba3-87d8-4476-9ba4-755f3359aa86" alt="Data Reception">
</p>
