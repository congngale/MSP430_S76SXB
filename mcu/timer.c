/*
 * timer.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Cong Nga Le
 */

#include "i2c.h"
#include "gpio.h"
#include "timer.h"
#include <msp430fr5994.h>

#define TIMER_INTERVAL 160

//***** Global Variables ******************************************************
static unsigned int count = 0;
static unsigned int interval = 0;

void init_timer(unsigned int second) {
  TA0CCTL0 = CCIE;                        // TACCR0 interrupt enabled

  interval = second * TIMER_INTERVAL;     // Set timer interval by seconds

  TA0CCR0 = 50000;
  TA0CTL = TASSEL__SMCLK | MC__UP;        // SMCLK, UP mode
}

// Timer0_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
#ifdef DEBUG
    //check time
    if (count == interval) {
      count = 0;                // Update counter
//      blink_green_led(2);

      //read sensor value
      read_sensor_hm3301_value();
    }
#endif

    count++;
}
