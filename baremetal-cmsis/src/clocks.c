#include "stm32f303x8.h"
#include "stm32f3xx.h"
#include "clocks.h"

// Configure STM32 clocks
void init_clocks(void) {
    // enable high-speed internal clock
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    // set PLL multiplier to x8 and to source from HSI/2
    RCC->CFGR &= ~(1 << 16); // RCC_CFGR_PLLSRC_HSI_DIV2;
    RCC->CFGR &= ~RCC_CFGR_PLLMUL; // clear bits
    RCC->CFGR |= RCC_CFGR_PLLMUL8; // 8Mhz/2 * 8 = 32MHz

    // enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // set the system clock to use the PLL clock
    RCC->CFGR &= ~RCC_CFGR_SW; // clear bits
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
