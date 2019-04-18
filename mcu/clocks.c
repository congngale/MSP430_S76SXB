/*
 * clocks.c
 *
 *  Created on: Apr 12, 2019
 *      Author: Cong Nga Le
 */

#include "clocks.h"
#include <msp430fr5994.h>

void init_clocks() {
  // Startup clock system with max DCO setting ~8MHz
  CSCTL0_H = CSKEY >> 8;                // Unlock clock registers
  CSCTL1 = DCOFSEL_3 | DCORSEL;         // Set DCO to 8MHz
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1; // Set all dividers
  CSCTL0_H = 0;                         // Lock CS registers
}
