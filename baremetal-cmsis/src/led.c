#include "stm32f303x8.h"
#include "led.h"
#include "board.h"

void init_onboard_led() {
    // Enable clock corresponding to LED
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // Set pin mode as a general purpose output
    // No AFR configuration needed since not in AF mode
    LED3_PORT->MODER &= ~(0x3 << (LED3_PIN * 2));
    LED3_PORT->MODER |= 0x1 << (LED3_PIN * 2);

    // Set output type do be push-pull as we drive it
    LED3_PORT->OTYPER &= ~(0x1 << LED3_PIN);

    // Set speed to low
    LED3_PORT->OSPEEDR &= ~(0x3 << (LED3_PIN * 2));
}

void write_onboard_led(bool state) {
    if (state) { LED3_PORT->BSRR = 1 << LED3_PIN; }
    else { LED3_PORT->BRR = 1 << LED3_PIN; }
}

void init_ws2812b() {
    // PWM + DMA
    // Use a 1.25us PWM period
    //
}