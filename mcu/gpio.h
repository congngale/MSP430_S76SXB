/*
 * gpio.h
 *
 *  Created on: Apr 11, 2019
 *      Author: Cong Nga Le
 */

#ifndef MCU_GPIO_H_
#define MCU_GPIO_H_

#include "define.h"

#define ON  1   // LED mode on
#define OFF 0   // LED mode off

void init_gpio(void);

#ifdef DEBUG
void set_red_led(int mode);
void set_green_led(int mode);
void blink_red_led(unsigned int count);
void blink_green_led(unsigned int count);
#endif

#endif /* MCU_GPIO_H_ */
