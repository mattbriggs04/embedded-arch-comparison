# STM32 Tri-Paradigm Architecture Study: Smart Macro-Pad

## Overview
This project is an architectural exercise designed to implement the exact same embedded system—a Smart Macro-Pad—across three distinct software paradigms. By keeping the hardware and feature set constant, this project isolates and exposes the structural, mental, and performance trade-offs between baremetal C, a preemptive RTOS, and modern async Rust. 

The goal is not just to make the hardware work, but to deeply analyze how different methodologies handle concurrency, resource sharing, and peripheral management on an ARM Cortex-M7.

## Hardware Stack
* **Microcontroller:** STM32 Nucleo F767ZI (ARM Cortex-M7)
* **Host Communication:** PC via UART + DMA
* **Visual Output 1:** WS2812b LED Strip (driven via Timer PWM + DMA)
* **Visual Output 2:** OLED Display (driven via SPI + DMA)
* **Input 1:** 4x4 Matrix Keypad (GPIO matrix scanning)
* **Input 2:** External Hardware Button (GPIO EXTI line)

## The Three Paradigms

This repository is split into three standalone projects, to be completed sequentially:

### Phase 1: Baremetal + CMSIS
A pure C implementation relying exclusively on the CMSIS headers, manually manipulating registers for the RCC, NVIC, and peripherals. 
* **Architecture:** Non-blocking `while(1)` superloop heavily reliant on state machines.
* **Core Challenge:** Managing concurrency without an OS. Polling the keypad or writing to the OLED must not block the CPU long enough to stall the WS2812b DMA buffer updates or drop incoming UART bytes.
* **Objective:** Establish a profound baseline understanding of the STM32F7 memory map and interrupt vector table.

### Phase 2: FreeRTOS in C (Preemptive Multitasking)
A traditional preemptive RTOS architecture that abstracts away the superloop, allowing the scheduler to slice CPU time across dedicated tasks.
* **Architecture:** Isolated threads (`Task_MatrixScan`, `Task_OLED`, `Task_LED`, `Task_UART`).
* **Core Challenge:** Thread safety and inter-process communication (IPC). Transitioning from volatile state flags to Mutexes, Semaphores, and RTOS Queues to safely share peripherals (like the OLED) across concurrent tasks.
* **Objective:** Master task synchronization