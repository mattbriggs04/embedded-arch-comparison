#include "stm32f303x8.h"

#include "systick.h"
// Configure Cortex-M4 system timer
// See the programming manual (PM0214)
void init_systick(uint32_t ticks) {
    // the preffered sequence is
    // 1. Program reload value
    // 2. Clear current value
    // 3. Program control and status register

    // reload value to generate 1ms ticks = 8000 (8MHz / (7999 + 1) = 1Khz)
    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0;

    SysTick->CTRL |= 1 << 2; // CLKSOURCE = AHB
    SysTick->CTRL |= 1 << 1; // TICKINT = assert SysTick exception
    SysTick->CTRL |= 1 << 0; // enable clock
}
