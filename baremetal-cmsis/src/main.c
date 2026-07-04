#include "stm32f3xx.h"
#include "systick.h"
#include "clocks.h"
#include "uart.h"
#include "led.h"
#include "oled.h"

int main(void) {

    // Setup
    init_clocks();
    init_systick(SYSCLK_FREQ / 1000); // gives 1ms
    init_uart();
    init_onboard_led();
    init_oled();
    // init_spi(); // these will deal with enabling pin specific clocks and prescalers etc.
    // Baremetal: superloop method
    // since no RTOS or Async framework
    uint32_t led_ms = get_ms();
    bool led_state = false;

    while (1) {
        uint32_t curr_ms = get_ms();
        if ((curr_ms - led_ms) >= 1000) {
            write_onboard_led(led_state);
            led_state = !led_state;
            led_ms = curr_ms;
        }
    }
}
