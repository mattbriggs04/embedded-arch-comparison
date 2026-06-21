#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f3xx.h"

#define SPI_CS_PORT   GPIOA
#define SPI_CS_PIN    11
#define SPI_MOSI_PORT GPIOB
#define SPI_MOSI_PIN  5
#define SPI_MISO_PORT GPIOB
#define SPI_MISO_PIN  4
#define SPI_SCK_PORT  GPIOB
#define SPI_SCK_PIN   3

#define BOARD_UART         USART1
#define BOARD_UART_TX_PORT GPIOA
#define BOARD_UART_TX_PIN  9
#define BOARD_UART_RX_PORT GPIOA
#define BOARD_UART_RX_PIN  10

void board_init();

#endif /* __BOARD_H__ */
