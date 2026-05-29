# Baremetal + CMSIS

## Overview
This directory contains the Phase 1 implementation of the Smart Macro-Pad. It is a pure C, baremetal application built exclusively using CMSIS headers. It completely discards the ST Hardware Abstraction Layer (HAL) and CubeMX generators to expose the raw memory-mapped registers, clock tree, and interrupt vectors of the STM32F303K8T6 on the Nucleo-F303K8 board.

The Nucleo-F303K8 is a breadboard-friendly Nucleo-32 board built around an STM32F303K8T6, an ARM Cortex-M4F microcontroller with 64 KiB of flash, 12 KiB of main SRAM, and 4 KiB of CCM RAM.

## Toolchain Requirements
To build and flash this project, ensure you have the following installed and available in your system's PATH:
* **Compiler:** `arm-none-eabi-gcc` (GNU Arm Embedded Toolchain)
* **Build System:** `make`
* **Flasher/Debugger:** `openocd`

## Directory Structure
This project requires a specific layout to manage the raw hardware definitions and linker scripts properly.

```text
baremetal-cmsis/
├── Makefile                
├── linker/
│   └── STM32F303K8Tx_FLASH.ld
├── startup/
│   └── startup_stm32f303x8.s
├── include/                     
│   └── main.h
│  
├── src/                         
│   ├── main.c
│   └── system_stm32f3xx.c
└── lib/                         
    ├── cmsis_core/
    └── cmsis_device/
```

## External Dependencies
To compile, you must populate the `linker/`, `startup/`, `src/`, and `lib/` directories with official ST and ARM files. You can source them directly from their respective GitHub repositories:

1. **Linker Script:** [`STM32F303K8Tx_FLASH.ld`](https://github.com/STMicroelectronics/STM32CubeF3/blob/master/Projects/STM32F303K8-Nucleo/Templates/SW4STM32/STM32F303K8_NUCLEO/STM32F303K8Tx_FLASH.ld)
   - Place this in `linker/`.
   - The ST script defines `FLASH`, `RAM`, and `CCMRAM` regions for the STM32F303K8.
2. **Startup Assembly:** [`startup_stm32f303x8.s`](https://github.com/STMicroelectronics/STM32CubeF3/blob/master/Projects/STM32F303K8-Nucleo/Templates/SW4STM32/startup_stm32f303x8.s)
   - Place this in `startup/`.
   - Use the GCC/SW4STM32 version, not the EWARM or MDK-ARM versions.
3. **System Initialization Code:** [`system_stm32f3xx.c`](https://github.com/STMicroelectronics/STM32CubeF3/blob/master/Projects/STM32F303K8-Nucleo/Templates/Src/system_stm32f3xx.c)
   - Place this directly in `src/`.
4. **CMSIS Core Headers:** [`CMSIS/Core/Include`](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/Core/Include)
   - For this Cortex-M4F target, keep at least:
   - `core_cm4.h`
   - `cmsis_version.h`
   - `cmsis_compiler.h`
   - `cmsis_gcc.h`
   - `mpu_armv7.h`
5. **CMSIS Device Headers:** [`cmsis-device-f3/Include`](https://github.com/STMicroelectronics/cmsis-device-f3/tree/master/Include)
   - Only truly need:
   - `stm32f3xx.h`
   - `stm32f303x8.h`
   - `system_stm32f3xx.h`

