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

#include "gpio.h"
#include "uart.h"
#include "clocks.h"
#include <msp430.h>
#include <string.h>

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

#ifdef DEBUG
  //init message
  message = "Initialize back channel UART success!";

  //write message
  back_channel_write(message, strlen(message));

  //blink red led
  blink_red_led(2);

  //blink green led
  blink_green_led(2);

  //set message
  message = "Blink LED!";

  //write message
  back_channel_write(message, strlen(message));
#endif

#ifdef S76SXB
  message = "sip get_ver";
  s76sxb_write(message, strlen(message));
#endif

  // Enter LPM0, interrupts enabled
  __bis_SR_register(LPM1_bits | GIE);
}
