/*
 * gpio.h
 *
 *  Created on: Apr 11, 2019
 *      Author: Cong Nga Le
 */

#ifndef MCU_GPIO_H_
#define MCU_GPIO_H_

#include "define.h"

void init_gpio(void);

#ifdef DEBUG
void blink_red_led(unsigned int count);
void blink_green_led(unsigned int count);
#endif

#endif /* MCU_GPIO_H_ */
