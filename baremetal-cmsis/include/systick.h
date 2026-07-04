#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>

void init_systick(uint32_t ticks);
uint32_t get_ms();
#endif // __SYSTICK_H__