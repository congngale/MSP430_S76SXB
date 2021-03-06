//***************************************************************************************
//  MSP430 LoRa - Connect With S76SXB
//
//
//                   MSP430FR5994
//                 -----------------
//            /|\ |             P6.1|<-- Receive Data (UCA3RXD)
//             |  |                 |
//             ---|RST          P6.0|--> Transmit Data (UCA3TXD)
//                |                 |
//                |             PJ.5|--- LFXOUT
//                |                 |   |
//                |                 |  32kHz
//                |                 |   |
//                |             PJ.4|--- LFXIN
//
//  Texas Instruments, Inc
//  July 2013
//***************************************************************************************

#include "i2c.h"
#include "gpio.h"
#include "uart.h"
#include "lora.h"
#include "timer.h"
#include "clocks.h"
#include <string.h>
#include <msp430fr5994.h>

void main(void) {
#if defined(DEBUG) || defined(S76SXB)
  //init message
  char *message;
#endif

  // Stop watch dog timer
  WDTCTL = WDTPW | WDTHOLD;

  //init gpio
  init_gpio();

  //init clocks
  init_clocks();

  //init uart
  init_uart();

  //init i2c
  init_i2c();

  //init sleep interval
  init_sleep_interval(30);       // Interval sleep is 30 seconds

  //need delay to make sensor available
  __delay_cycles(1000000);      // SW Delay

  //init hm3301 sensor
  init_hm3301_sensor();

#ifdef DEBUG
  //init message
  message = "Initialize UARTs success!";

  //write message
  back_channel_write(message, strlen(message));

  message = "sip get_ver";
  s76sxb_write(message, strlen(message));
#endif

  // Enter LPM1, interrupts enabled
  __bis_SR_register(LPM3_bits | GIE);
}
