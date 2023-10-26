# ADXL345 Driver

**Introduction**

Contains the ADXL345 accelerometer driver implementation. The ADXL345 is a compact, low-power, three-axis accelerometer sensor, designed for precise measurement of acceleration along the X, Y, and Z axes. It finds application in a diverse range of fields.

This project incorporates reusable SPI and DIO drivers to configure the ADXL345 accelerometer driver. The development is rooted in embedded C (Bare-metal) code, which directly interacts with the microcontroller's (MCU) register map, adhering to industry-standard coding practices such as MISRA-C.

**Project Overview.**

The repository consists of creating reusable drivers, and HALs for MCU. The current tooling being used for this project includes:
* Development Board: _Nucleo-F401RE._
* IDE/Debugger: _Visual Studio Code (PlatformIO extension)._
* Compiler Toolchain: _GNU ARM Embedded Toolchain._

**ADXL345 Driver Implementation**

The code involves the configuration of the SPI1 as a master. The SPI driver is used to configure the ADXL345 accelerometer. The setting includes a baud rate = 4KHz, full duplex communication, an 8-bit data frame format, and most significant bit transmission.
A KY-57 logic analyzer is connected between the MCU and the ADXL345 accelerometer for data reception and analysis. To establish the physical connections, the GPIO driver configures the SPI1 pins. The following table illustrates the pin connections among the Nucleo board, and KY-57 logic analyzer:

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

The KY-57 logic analyzer is used to show the ADXL345 accelerometer behavior. The ADXL345_init function configures the ADXL345 accelerometer with 10 bits full resolution at +/-4g and sets the device into measurement mode. Here is the implementation of the ADXL345_init function:

<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/5a186351-2ece-447d-b7da-bc868912dbb3" alt="ADXL345_init">
</p>

The following images show each data transmission between the MCU and the ADXL345 within the ADXL345 function.

Set the data range to 10 bits full resolution at +/-4g.
<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/2fc53852-9000-497e-adeb-a27cda8ed1a8" alt="First Write">
</p>

Reset the accelerometer.
<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/d2bc4880-6d65-4371-8289-6dfaf345e821" alt="Second Write">
</p>

Sets the device into measurement mode.
<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/c92d7f24-4294-45a9-9f21-be3770457d51" alt="Second Write">
</p>

The ADXL345_read function serves to retrieve data from the accelerometer's x, y, and z axes, subsequently processing this data to obtain the g-scale factor. The code implementation for this function is provided below:

<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/49f96ceb-3706-4913-a5d8-6a441e52012e" alt="Main code">
</p>

The following image shows the data reception of the axes values. 

<p align="center">
    <img src="https://github.com/JoseLuis-Figueroa/adxl345-driver/assets/113542682/c8f30ba3-87d8-4476-9ba4-755f3359aa86" alt="Data Reception">
</p>

