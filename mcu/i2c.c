/*
 * i2c.c
 *
 *  Created on: Apr 16, 2019
 *      Author: Cong Nga Le
 */

#include "i2c.h"
#include "uart.h"
#include "gpio.h"
#include "define.h"
#include <msp430fr5994.h>

//***** Global Variables ******************************************************
static unsigned int rx_len = 0;
static char rx_buffer[MAX_BUFFER];

static unsigned int tx_len = 0;
static char tx_buffer[MAX_BUFFER];

void init_i2c() {
  UCB2CTLW0 = UCSWRST;                      // Enable SW reset
  UCB2CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC; // I2C master mode, SMCLK
  UCB2BRW = 80;                             // fSCL = SMCLK/80 = ~100kHz
  UCB2I2CSA = HM3301_ADDRESS;               // Slave Address
  UCB2CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
  UCB2IE |= UCNACKIE;                       // Enable ACK interrupt
}

void init_hm3301_sensor() {
  /* Initialize slave address and interrupts */
  UCB2I2CSA = HM3301_ADDRESS;
  UCB2IFG &= ~(UCTXIFG + UCRXIFG);         // Clear any pending interrupts
  UCB2IE &= ~UCRXIE;                       // Disable RX interrupt
  UCB2IE |= UCTXIE;                        // Enable TX interrupt

  tx_len = 1;                              // Set TX length
  tx_buffer[0] = HM3301_SELECT_COMM_CMD;   // Set TX data

  UCB2CTLW0 |= UCTR | UCTXSTT;             // I2C TX, start condition
  __bis_SR_register(LPM0_bits | GIE);      // Enter LPM0 w/ interrupts
}

void read_sensor_hm3301_value() {
  /* Initialize slave address and interrupts */
  UCB2I2CSA = HM3301_ADDRESS;

  UCB2IFG &= ~(UCTXIFG + UCRXIFG);         // Clear any pending interrupts
  UCB2IE |= UCRXIE;                        // Enable RX interrupt
  UCB2IE &= ~UCTXIE;                       // Disable TX interrupt

  rx_len = 0;                              // Reset buffer length

  UCB2CTLW0 &= ~UCTR;                      // Switch to receiver
  UCB2CTLW0 |= UCTXSTT;                    // Send repeated start
  __bis_SR_register(LPM0_bits | GIE);      // Enter LPM0 w/ interrupts
}

int validate_hm3301_data(char data[], unsigned int data_len) {
  //init result
  char sum = 0;
  unsigned int i;
  int result = -1;

  //check data len
  if(data_len > 0) {
    //sum data from data_len - 2 to 1
    for (i = data_len - 2; i > 0 ; i--) {
      sum += data[i];
    }

    //addition
    sum += data[0];

    //check sum
    if (sum == data[data_len - 1]) {
      //correct
      result = 0;
    }
  }

  return result;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = EUSCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(EUSCI_B2_VECTOR))) USCI_B2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCB2IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_NONE:          break;     // Vector 0: No interrupts
        case USCI_I2C_UCALIFG:   break;     // Vector 2: ALIFG
        case USCI_I2C_UCNACKIFG: break;     // Vector 4: NACKIFG
        case USCI_I2C_UCSTTIFG:  break;     // Vector 6: STTIFG
        case USCI_I2C_UCSTPIFG:  break;     // Vector 8: STPIFG
        case USCI_I2C_UCRXIFG3:  break;     // Vector 10: RXIFG3
        case USCI_I2C_UCTXIFG3:  break;     // Vector 12: TXIFG3
        case USCI_I2C_UCRXIFG2:  break;     // Vector 14: RXIFG2
        case USCI_I2C_UCTXIFG2:  break;     // Vector 16: TXIFG2
        case USCI_I2C_UCRXIFG1:  break;     // Vector 18: RXIFG1
        case USCI_I2C_UCTXIFG1:  break;     // Vector 20: TXIFG1
        case USCI_I2C_UCRXIFG0:  {          // Vector 22: RXIFG0
            // Get RX data
            rx_buffer[rx_len] = UCB2RXBUF;

            // Increase buffer length
            rx_len++;

            //check buffer length
            if (rx_len == HM3301_DATA_LENGTH) {
              //validate sensor data
              int ret = validate_hm3301_data(rx_buffer, rx_len);

#ifdef DEBUG
              //check result
              if (ret == 0) {
                set_green_led(ON);
              } else {
                set_red_led(ON);
              }

              back_channel_write(rx_buffer, rx_len);
#endif

              UCB2CTLW0 |= UCTXSTP;                 // Send stop condition
//              UCB2IE &= ~UCRXIE;                    // Disable RX interrupt
            }
            __bic_SR_register_on_exit(LPM1_bits);   // Exit LPM1
            break;
        }
        case USCI_I2C_UCTXIFG0:             // Vector 24: TXIFG0
          //check TX length
          if (tx_len > 0) {
            tx_len--;                       // Decrease counter
            UCB2TXBUF = tx_buffer[tx_len];  // Write data
          } else {
            UCB2CTLW0 |= UCTXSTP;           // Send stop condition
            UCB2IE &= ~UCTXIE;              // Disable TX interrupt
          }
          __bic_SR_register_on_exit(LPM1_bits);   // Exit LPM1
          break;
        case USCI_I2C_UCBCNTIFG: break;     // Vector 26: BCNTIFG
        case USCI_I2C_UCCLTOIFG: break;     // Vector 28: clock low timeout
        case USCI_I2C_UCBIT9IFG: break;     // Vector 30: 9th bit
        default: break;
    }
}
