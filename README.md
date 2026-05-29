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

### Phase 3: Embassy Rust (Async/Await Cooperative)
A modern, `no_std` Rust implementation utilizing the Embassy executor for cooperative multitasking, yielding the safety guarantees of the Rust compiler.
* **Architecture:** Async tasks running on a cooperative executor, utilizing `.await` points to yield control during hardware delays (e.g., waiting for a DMA Transfer Complete interrupt).
* **Core Challenge:** Satisfying the Rust borrow checker in an embedded environment. Safely sharing state and peripherals between async tasks using Embassy's `Channel` and `Signal` primitives.
* **Objective:** Evaluate how zero-cost async abstractions compare to a preemptive RTOS in both developer ergonomics and runtime overhead, serving as a masterclass in modern systems programming.

## Implementation Goals & Driver Specs

Regardless of the paradigm, the system must achieve the following functional requirements:

1. **WS2812b Driver (Timer PWM + DMA):** Generate a precise 800kHz ($1.25 \mu s$) waveform. The CPU must only compute the color data; the actual bit-banging must be offloaded to the DMA to ensure zero jitter.
2. **Matrix Keypad Scanner:**
   Implement non-blocking debounce logic and matrix scanning. A key press should trigger an action (e.g., change the LED animation or update the OLED).
3. **OLED Display Interface (SPI + DMA):**
   Render the current state of the macro-pad (e.g., active LED mode, last key pressed, UART status). Screen updates must happen asynchronously via DMA to avoid blocking the CPU.
4. **PC Communication (UART + DMA):**
   Receive configuration commands from the host PC (e.g., "Set LED color to red") and transmit status logs back, ensuring no bytes are dropped during heavy rendering tasks.
5. **Hardware Interrupt (EXTI):**
   The external button must immediately preempt the current execution context to cycle the active WS2812b color palette.

## Success Criteria & Evaluation
The project is considered complete when all three implementations are functionally identical. A final comparison will be documented, evaluating:
* **Binary Size & Memory Footprint:** Flash and RAM usage across all three.
* **Code Complexity & Readability:** Lines of code, modularity, and the mental overhead required to add a new feature.
* **Concurrency Robustness:** How effectively each paradigm handles simultaneous events (e.g., mashing the keypad while receiving a UART burst and driving the LEDs).