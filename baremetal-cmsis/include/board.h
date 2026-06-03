#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f303x8.h"

#define BUTTON_PORT GPIOA
#define BUTTON_PIN  9

#define BOARD_UART         USART1
#define BOARD_UART_TX_PORT GPIOB
#define BOARD_UART_TX_PIN  6
#define BOARD_UART_RX_PORT GPIOB
#define BOARD_UART_RX_PIN  7

void board_init();

#endif /* __BOARD_H__ */