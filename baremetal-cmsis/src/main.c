#include "stm32f3xx.h"
#include "systick.h"
#include "clocks.h"
// #include "uart.h"
// #include "spi.h"

int main(void) {

    // Setup
    init_clocks();
    init_systick(SYSCLK_FREQ / 8000); // 8Mhz / 8000 = 1ms
    // init_uart();
    // init_spi(); // these will deal with enabling pin specific clocks and prescalers etc.
    // Baremetal: superloop method
    // since no RTOS or Async framework
    while (1) {
        // wait for interrupt
        __WFI();
    }
}
