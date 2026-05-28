# Baremetal + CMSIS

## Overview
This directory contains the Phase 1 implementation of the Smart Macro-Pad. It is a pure C, baremetal application built exclusively using CMSIS headers. It completely discards the ST Hardware Abstraction Layer (HAL) and CubeMX generators to expose the raw memory-mapped registers, clock tree, and interrupt vectors of the STM32F767ZI (ARM Cortex-M7).

## Toolchain Requirements
To build and flash this project, ensure you have the following installed and available in your system's PATH:
* **Compiler:** `arm-none-eabi-gcc` (GNU Arm Embedded Toolchain)
* **Build System:** `make` (or `cmake` + `ninja`)
* **Flasher/Debugger:** `openocd` (Supports the onboard ST-Link V3)

## Directory Structure
This project requires a specific layout to manage the raw hardware definitions and linker scripts properly.

```text
baremetal-cmsis/
├── Makefile                
├── linker/
│   └── STM32F767ZITX_FLASH.ld
├── startup/
│   └── startup_stm32f767xx.s
├── include/                     
│   └── main.h
│  
├── src/                         
│   ├── main.c
│   └── system_stm32f7xx.c
└── lib/                         
    ├── cmsis_core/
    └── cmsis_device/
```

## External Dependencies
To compile, you must populate the `linker/`, `startup/`, `src/`, and `lib/` directories with official ST and ARM files. You can source them directly from their respective GitHub repositories:

1. **Linker Script:** [`STM32F767ZITX_FLASH.ld`](https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Projects/STM32F767ZI-Nucleo/Examples/FLASH/FLASH_EraseProgram/SW4STM32/STM32F767ZI-Nucleo/STM32F767ZITx_FLASH.ld)
2. **Startup Assembly:** [`startup_stm32f767xx.s`](https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Projects/STM32F767ZI-Nucleo/Examples/RCC/RCC_ClockConfig/SW4STM32/startup_stm32f767xx.s)
3. **System Initialization Code:** [`system_stm32f7xx.c`](https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Projects/STM32F767ZI-Nucleo/Templates/Src/system_stm32f7xx.c) (Place this directly in your `src/` directory)
4. **CMSIS Core Headers:** [`cmsis_core/` Directory](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/Core/Include)
   - Do not need any of the `core_cmX.h` but `core_cm7.h`
   - Keep `cmsis_version.h`, `cmsis_compiler.h`, `cmsis_gcc.h`, and `mpu_armv7.h`
5. **CMSIS Device Headers:** [`cmsis_device/` Directory](https://github.com/STMicroelectronics/cmsis_device_f7/tree/master/Include)
   - Only truly need:
   - `stm32f7xx.h`
   - `stm32f767xx.h`
   - `system_stm32f7xx.h`
