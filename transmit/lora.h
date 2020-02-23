/*
 * lora.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Cong Nga Le
 */

#ifndef MCU_LORA_H_
#define MCU_LORA_H_

//Over-the-Air Activation
void send_join_request(void);
void send_data(char data[], unsigned int data_len);

#endif /* MCU_LORA_H_ */
