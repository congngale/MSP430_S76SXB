/*
 * gpio.c
 *
 *  Created on: Apr 11, 2019
 *      Author: Cong Nga Le
 */

#include "gpio.h"
#include <msp430.h>

//***** Global Variables ******************************************************
//static unsigned int red_time = 0;
//static unsigned int green_time = 0;

//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void init_gpio() {
  //set LED1 - Red LED
  P1DIR |= BIT0;    //conf LED at P1.0 for debugging
  P1OUT &= ~BIT0;    //LED P1.0 switched off at startup

  //set LED2 - Green LED
  P1DIR |= BIT1;    //conf LED at P1.1 for debugging
  P1OUT &= ~BIT1;    //LED P1.1 switched off at startup

#ifdef DEBUG
  // Set P2.0 and P2.1 for USCI_A0 UART operation
  P2SEL1 |= BIT0 | BIT1;
  P2SEL0 &= ~(BIT0 | BIT1);
#endif

#ifdef S76SXB
  // Set P6.0 and P6.1 for USCI_A3 UART operation
  P6SEL0 |= BIT0 | BIT1;
  P6SEL1 &= ~(BIT0 | BIT1);
#endif

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
}

#ifdef DEBUG
//*****************************************************************************
// Blink RED Led
//*****************************************************************************
void blink_red_led(unsigned int count) {
  //init
  volatile unsigned int i;

  //blink by count
  for (i = count; i > 0; i--) {
    //toggle led
    P1OUT ^= BIT0; // Red LED

    __delay_cycles(1000000);     // SW Delay
  }
}

void blink_green_led(unsigned int count) {
  //init
  volatile unsigned int i;

  //blink by count
  for (i = count; i > 0; i--) {
    //toggle led
    P1OUT ^= BIT1; // Green LED

    __delay_cycles(1000000);     // SW Delay
  }
}
#endif

