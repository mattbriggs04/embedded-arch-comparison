#include "board.h"
#include "uart.h"

#include "stm32f303x8.h"

void init_uart() {
    // See global clock tree, enable
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

}
