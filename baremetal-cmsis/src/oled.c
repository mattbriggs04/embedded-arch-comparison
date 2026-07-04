#include "stm32f303x8.h"
#include "oled.h"

// CS   = PA4 (GPIO toggled manually)
// MOSI = PA7
// MISO = PA6
// SCK  = PA5
void init_oled() {
    // Enable clock corresponding to GPIO A, SPI peripheral, and DMA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_DMA1EN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Configure CS (PA4)
    // Set mode to General Purpose Output (01)
    GPIOA->MODER &= ~(0x3 << (4*2));
    GPIOA->MODER |= (0x1 << (4*2));
    GPIOA->OTYPER &= (0x1 << 4); // push-pull
    GPIOA->OSPEEDR |= (0x3 << (4*2));
    GPIOA->BSRR = (1 << 4); // drive high

    // Set mode to AF (10) for PA5, PA6, PA7
    GPIOA->MODER &= ~((0x3 << (5*2)) | (0x3 << (6*2)) | (0x3 << (7*2)));
    GPIOA->MODER |= (0x2 << (5*2)) | (0x2 << (6*2)) | (0x2 << (7*2));

    // Set alternate functions
    GPIOA->AFR[0] &= ~((0xf << (5 * 4)) | (0xf << (6 * 4)) | (0xf << (7 * 4)));
    GPIOA->AFR[0] |= ((0x5 << (5 * 4)) | (0x5 << (6 * 4)) | (0x5 << (7 * 4)));

    // Configure DMA channel 3
    DMA1_Channel3->CCR = 0; // clear configuration register
    DMA1_Channel3->CPAR = (volatile uint32_t)&SPI1->DR; // peripheral address
    DMA1_Channel3->CCR |= DMA_CCR_MINC; // auto-increment buffer pointer
    DMA1_Channel3->CCR |= DMA_CCR_DIR;  // read from memory (TX to peripheral)
    DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0; // Mem size = peripheral size = 16-bit

    // Configure SPI
    SPI1->CR1 = 0; // clear reg
    SPI1->CR1 |= SPI_CR1_SSM; // enable slave-select via software
    SPI1->CR1 |= SPI_CR1_SSI; // slave-select internal (sets NSS high) so it doesn't fault
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1; // f_pclk / 16 = ??
    SPI1->CR1 |= SPI_CR1_MSTR; // enable MCU as master

    SPI1->CR2 = 0; // clear reg
    // data size = 10-bit (1001)
    // since it defaults to 8-bit (0111) after settings to 0, have to clear the middle bits
    SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_3;
    SPI1->CR2 &= ~(SPI_CR2_DS_1 | SPI_CR2_DS_2);

    SPI1->CR2 |= SPI_CR2_TXDMAEN; // enable DMA transmit


    SPI1->CR1 |= SPI_CR1_SPE; // enable SPI
}

// row = whether to send to row 0 or row 1 of the oled
void oled_send_cmd(uint32_t cmd) {

}

void oled_send_str(const char *str, uint8_t len, uint8_t row) {
    // truncate string if it is too long
    if (len > OLED_MAX_CHARS_PER_LINE) {
        len = OLED_MAX_CHARS_PER_LINE;
    }


}