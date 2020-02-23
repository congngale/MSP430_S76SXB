/*
 * lora.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Cong Nga Le
 */

#include "lora.h"
#include "uart.h"
#include "define.h"
#include <string.h>

#define OTAA_JOIN_MSG "mac join otaa"
#define SEND_DATA_CMD "mac tx ucnf 15 "

char hex_to_char(unsigned int hex);

void send_join_request() {
  //send join request to s76sxb
  s76sxb_write(OTAA_JOIN_MSG, strlen(OTAA_JOIN_MSG));
}

void send_data(char data[], unsigned int data_len) {
  //init
  char buffer;
  unsigned int i;
  unsigned int message_len = 0;

  //init message
  char message[MAX_BUFFER];

  //get format len
  unsigned int prefix_len = strlen(SEND_DATA_CMD);

  //set first data
  message[message_len] = SEND_DATA_CMD[message_len];

  //loop all format
  for (i = (prefix_len - 1); i > 0; i--) {
    //append data
    message[++message_len] = SEND_DATA_CMD[prefix_len - i];
  }

  // loop all data length
  for (i = data_len; i > 0; i--) {
    //get data
    buffer = data[data_len - i];

    //append data
    message[++message_len] = hex_to_char(buffer / 16);

    //append data
    message[++message_len] = hex_to_char(buffer % 16);
  }

  //increase len
  message_len += 1;

#ifdef DEBUG
  //write debug
  back_channel_write(message, message_len);
#endif

  //send message
  s76sxb_write(message, message_len);
}

char hex_to_char(unsigned int hex) {
  //init result
  char result;

  //check value
  if (hex < 10) {
    //convert to char
    result = '0' + hex;
  } else {
    //convert to char
    result = 'a' + (hex - 10);
  }

  return result;
}
