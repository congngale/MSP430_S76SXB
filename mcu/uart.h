/*
 * uart.h
 *
 *  Created on: Apr 11, 2019
 *      Author: Cong Nga Le
 */

#ifndef MCU_UART_H_
#define MCU_UART_H_

#include "define.h"

//***** Defines ***************************************************************
#define ASCII_ENTER       0x0D
#define ASCII_LINEFEED    0x0A

#define MAX_QUEUE         5
#define MAX_BUFFER        512

//***** Function Prototypes ***************************************************
void init_uart(void);

#ifdef S76SXB
int s76sxb_write(const char *buffer, unsigned int buffer_len);
#endif

#ifdef DEBUG
int back_channel_write(const char *buffer, unsigned int buffer_len);
#endif

#endif /* MCU_UART_H_ */
