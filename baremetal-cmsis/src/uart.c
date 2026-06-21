#include "board.h"
#include "uart.h"

#include "stm32f3xx.h"

void init_uart() {
    // Enable RCC for UART GPIO
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk;
}
