/*
 * i2c.h
 *
 *  Created on: Apr 16, 2019
 *      Author: Cong Nga Le
 */

#ifndef MCU_I2C_H_
#define MCU_I2C_H_

#define HM3301_ADDRESS 0x40
#define HM3301_DATA_LENGTH 29
#define HM3301_SELECT_COMM_CMD 0x88

void init_i2c(void);

void init_hm3301_sensor(void);
void read_sensor_hm3301_value(void);
int validate_hm3301_data(char data[], unsigned int data_len);

#endif /* MCU_I2C_H_ */
