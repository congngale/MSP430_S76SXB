/*
 * uart.h
 *
 *  Created on: Apr 11, 2019
 *      Author: Cong Nga Le
 */

#ifndef MCU_UART_H_
#define MCU_UART_H_

#include "define.h"

//***** Function Prototypes ***************************************************
void init_uart(void);
int s76sxb_write(const char *buffer, unsigned int buffer_len);

#ifdef DEBUG
int back_channel_write(const char *buffer, unsigned int buffer_len);
#endif

#endif /* MCU_UART_H_ */
