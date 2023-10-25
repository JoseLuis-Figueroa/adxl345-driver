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



