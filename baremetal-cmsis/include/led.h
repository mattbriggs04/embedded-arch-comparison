#ifndef __LED_H__
#define __LED_H__

#include <stdbool.h>

void init_onboard_led();
void init_ws2812b();
void write_onboard_led(bool state);

#endif // __LED_H__