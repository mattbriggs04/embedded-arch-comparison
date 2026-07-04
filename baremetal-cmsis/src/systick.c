#include "stm32f303x8.h"

#include "systick.h"

static volatile uint32_t systick_ms;

// Configure Cortex-M4 system timer
// See the programming manual (PM0214)
// ticks parameter should be the value that results in a 1ms tick
void init_systick(uint32_t ticks) {
    // the preffered sequence is
    // 1. Program reload value
    // 2. Clear current value
    // 3. Program control and status register

    // ticks should force a value that generates 1ms ticks
    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0;

    SysTick->CTRL |= 1 << 2; // CLKSOURCE = AHB
    SysTick->CTRL |= 1 << 1; // TICKINT = assert SysTick exception
    SysTick->CTRL |= 1 << 0; // enable clock
}

void SysTick_Handler(void) {
    systick_ms++;
}

uint32_t get_ms() {
    return systick_ms;
}