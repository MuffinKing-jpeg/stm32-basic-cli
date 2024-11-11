#ifndef __TOGGLE_PIN_H__
#define __TOGGLE_PIN_H__

#include "cli.h"

void cmd_led(Tokens args);
void led_toggle(GPIO_PinState state);

#endif