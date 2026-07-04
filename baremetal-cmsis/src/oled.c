#include "stm32f303x8.h"
#include "oled.h"

// CS   = PA4 (GPIO toggled manually)
// SCK  = PA5
// MISO = PA6
// MOSI = PA7
void init_oled() {
    // Enable clock corresponding to GPIO A and SPI peripheral
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Configure CS (PA4)
    // Set mode to General Purpose Output (01)
    GPIOA->MODER &= ~(0x3 << (4*2));
    GPIOA->MODER |= (0x1 << (4*2));
    GPIOA->OTYPER &= ~(0x1 << 4); // push-pull
    GPIOA->OSPEEDR |= (0x3 << (4*2));
    GPIOA->BSRR = (1 << 4); // drive high

    // Set mode to AF (10) for PA5, PA6, PA7
    GPIOA->MODER &= ~((0x3 << (5*2)) | (0x3 << (6*2)) | (0x3 << (7*2)));
    GPIOA->MODER |= (0x2 << (5*2)) | (0x2 << (6*2)) | (0x2 << (7*2));

    // Set alternate functions
    GPIOA->AFR[0] &= ~((0xf << (5 * 4)) | (0xf << (6 * 4)) | (0xf << (7 * 4)));
    GPIOA->AFR[0] |= ((0x5 << (5 * 4)) | (0x5 << (6 * 4)) | (0x5 << (7 * 4)));

    // Configure SPI
    SPI1->CR1 = 0; // clear reg
    SPI1->CR1 |= SPI_CR1_SSM; // enable slave-select via software
    SPI1->CR1 |= SPI_CR1_SSI; // slave-select internal (sets NSS high) so it doesn't fault
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1; // f_pclk / 16
    SPI1->CR1 |= SPI_CR1_MSTR; // enable MCU as master

    SPI1->CR2 = 0; // clear reg
    // data size = 10-bit (1001)
    // since it defaults to 8-bit (0111) after settings to 0, have to clear the middle bits
    SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_3;
    SPI1->CR2 &= ~(SPI_CR2_DS_1 | SPI_CR2_DS_2);

    SPI1->CR1 |= SPI_CR1_SPE; // enable SPI
}

// Send a single command to the OLED
void oled_send_cmd(uint32_t cmd) {
    // For a command: RS = 0 (bit 9), R/W = 0 (bit 8)
    // Masking with 0xFF guarantees the top bits are 0
    uint16_t tx_data = cmd & 0xFF;

    // Pull CS LOW to select the device
    GPIOA->BRR = (1 << 4);

    // Wait until the Transmit Buffer Empty (TXE) flag is set
    while (!(SPI1->SR & SPI_SR_TXE));

    // Write to Data Register
    SPI1->DR = tx_data;

    // Wait for the SPI peripheral
    while (SPI1->SR & SPI_SR_BSY);

    // Pull CS HIGH to deselect the device
    GPIOA->BSRR = (1 << 4);
}

// Send a string of characters to a specific row
void oled_send_str(const char *str, uint8_t len, uint8_t row) {
    // truncate string if it is too long
    if (len > OLED_MAX_CHARS_PER_LINE) {
        len = OLED_MAX_CHARS_PER_LINE;
    }

    // Move the cursor to the correct row in DDRAM
    // Row 0 begins at address 0x00
    // Row 1 begins at address 0x40
    uint8_t address = (row == 0) ? 0x00 : 0x40;

    // The "Set DDRAM address" command requires DB7 to be 1 (0x80)
    oled_send_cmd(0x80 | address);

    // Pull CS LOW to begin the data burst
    GPIOA->BRR = (1 << 4);

    // Loop through the string and blast characters into the FIFO
    for (int i = 0; i < len; i++) {
        // For data: RS = 1 (bit 9), R/W = 0 (bit 8)
        uint16_t tx_data = (1 << 9) | str[i];

        // Wait until the TX FIFO has room for at least one more frame
        while (!(SPI1->SR & SPI_SR_TXE));

        // Write the packed character to the Data Register
        SPI1->DR = tx_data;
    }

    // Wait for the very last character to finish transmitting
    while (SPI1->SR & SPI_SR_BSY);

    // Pull CS HIGH
    GPIOA->BSRR = (1 << 4);
}