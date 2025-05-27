# ADXL345 Driver

This repository contains the implementation of the **ADXL345 3-axis accelerometer** driver. The ADXL345 is a compact, low-power, three-axis accelerometer designed for precise acceleration measurement along the X, Y, and Z axes, making it suitable for various applications.

This project integrates reusable SPI and DIO drivers to configure and interface with the ADXL345. Developed in **embedded C (bare-metal)**, it directly interacts with the microcontroller's (MCU) register map, adhering to **industry-standard coding practices** such as **MISRA-C**.

---

## Table of Contents

1. [About the Project](#about-the-project)  
2. [Project Status](#project-status)  
3. [Getting Started](#getting-started)  
   1. [Dependencies](#dependencies)  
   2. [Getting the Source](#getting-the-source)  
   3. [Building](#building)  
   4. [Running Tests](#running-tests)  
   5. [Installation](#installation)  
4. [Usage](#usage)  
5. [Release Process](#release-process)  
6. [How to Get Help](#how-to-get-help)  
7. [Contributing](#contributing)  
8. [Further Reading](#further-reading)  
9. [License](#license)  
10. [Authors](#authors)  
11. [Acknowledgments](#acknowledgments)  

---

## About the Project

The ADXL345 is a small, thin, low-power 3-axis accelerometer with high resolution (13-bit) measurement at up to ±16g. Communication with the ADXL345 is via SPI or I2C. This project focuses on SPI.

This driver is part of a reusable embedded driver suite, and is built from scratch in **Embedded C** with a layered structure (driver ↔ HAL ↔ application), designed for portability and real-world applications.

### Key Features

- Full-duplex SPI communication at 4 kHz.
- Configurable ±4g range and 10-bit full-resolution mode.
- Continuous data acquisition of X, Y, Z axes.
- Data scaled to g-force.
- Integration with logic analyzer for SPI waveform verification.

### Technical Details

- **Microcontroller Platform:** STM32F401RE (ARM Cortex-M4).
- **MCU Board:** Nucleo-F401RE.
- **Language:** Embedded C  (Bare-Metal).
- **Coding Standard:** MISRA-C.
- **Tools:**
  - **IDE:** VS Code with PlatformIO.
  - **Toolchain:** GNU ARM Embedded.
  - **Sensor:** ADXL345.     
  - **Debug/Verification:** KY-57 Logic Analyzer.
  - **Documentation:** Doxygen.

**[Back to top](#table-of-contents)**

---

## Project Status

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)

Current Version: `v1.1.`  
Tested On: `Nucleo-F401RE.`  
- ✅ SPI Driver Integration.  
- ✅ DIO Driver Integration.  
- ✅ ADXL345 Initialization.  
- ✅ Real-Time Data Reading.  

**[Back to top](#table-of-contents)**

---

## Getting Started

This guide helps you build and test the firmware on supported microcontrollers.

### Dependencies

Install the following tools:
- [Visual Studio Code](https://code.visualstudio.com/) with [PlatformIO extension.](https://platformio.org/install)
- [GNU ARM Embedded Toolchain.](https://developer.arm.com/downloads/-/gnu-rm)
- [STM32Cube Drivers](https://www.st.com/en/ecosystems/stm32cube.html) (via PlatformIO).
- [Doxygen](https://doxygen.nl/manual/install.html) (for documentation).

### Getting the Source

Clone the GitHub repository:

```bash
git clone https://github.com/JoseLuis-Figueroa/adxl345-driver
cd adxl345-driver
```
### Building

You can build the project using either the **PlatformIO extension in Visual Studio Code** or the **PlatformIO CLI**.

- **Build using the PlatformIO Toolbar (GUI):** If you're using VS Code with the PlatformIO extension, simply click the "_Build_" button in the PlatformIO toolbar. This will compile your project using the selected environment in your `platformio.ini`.

- **Build using the PlatformIO CLI:** From the project's root directory, run the following command to build all environments defined in `platformio.ini`:

```
pio run
```

- **Build specific environments in CLI:** run the following command to build a specific environment:

```
pio run -e nucleo_f401re
```

### Running Tests

You can test on actual hardware by uploading the code using the **PlatformIO extension in Visual Studio Code** or the **PlatformIO CLI**:

- **Run and debug using the PlatformIO Toolbar (GUI):** If you're using VS Code with the PlatformIO extension, simply click the "_Run and Debug_" button in the PlatformIO toolbar. This will run and debug your project using the selected environment in your `platformio.ini`.

- **Build using the PlatformIO CLI:** From the project's root directory, run the following command to upload all environments defined in `platformio.ini`:

```
pio run --target upload
```

#### Other Tests

- Manual debugging via SWD.  
- Analyze SPI communication using the **KY-57 logic analyzer**.
- Static analysis (planned).

### Installation

No additional installation required. Flash the firmware directly via ST-Link.

**[Back to top](#table-of-contents)**

---

## Usage 

The implementation configures SPI1 as a master to communicate with the ADXL345 accelerometer. The SPI settings include:

- **Baud rate:** 4 kHz.
- **Comunication mode:** Full-duplex communication.
- **Data frame format:** 8-bit.
- **Bit transmission order:** Most significant bit (MSB) first.

A KY-57 logic analyzer is connected between the MCU and the ADXL345 for data reception and analysis. The following table outlines the SPI pin connections:

### **SPI Pin Connections**

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

<p align="center">Table 1. SPI Pin Connections.</p>

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

## Reading Acceleration

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

---

## Release Process

### Versioning

We use [Semantic Versioning](http://semver.org/).  
Latest release: **v1.1.**

### Payload

- `v1.0`: Initial ADXL345 SPI Driver release.
- `v1.1`: Updated documentation, structural improvements, and design by contract enhancements.

**[Back to top](#table-of-contents)**

---

## How to Get Help

- [Open an issue](https://github.com/JoseLuis-Figueroa/adxl345-driver/issues)
- Message me via my GitHub profile.

---

## Contributing

Contributions are welcome!  
Please refer to [CONTRIBUTING.md](Documentation/CONTRIBUTING.md) for details.

**[Back to top](#table-of-contents)**

---

## Further Reading

- [ADXL345 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/adxl345.pdf)
- [STM32F4 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
- [MISRA-C Guidelines](https://www.misra.org.uk/)
- [Jacob Beningo: Reusable Firmware Development](https://www.beningo.com/)

---

## License

Licensed under the MIT License – see [LICENSE.md](LICENSE) for details.

---

## Authors

**[JoseLuis-Figueroa](https://github.com/JoseLuis-Figueroa)** – Developer & Maintainer

---

## Acknowledgments

- [Analog Devices](https://www.analog.com/)
- [Beningo Embedded Group](https://www.beningo.com/)
- [PlatformIO](https://platformio.org/)

**[Back to top](#table-of-contents)**
