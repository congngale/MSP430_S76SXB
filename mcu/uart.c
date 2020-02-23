/*
 * uart.c
 *
 *  Created on: Apr 11, 2019
 *      Author: Cong Nga Le
 */


#include "i2c.h"
#include "uart.h"
#include "gpio.h"
#include <msp430fr5994.h>

//***** Global Variables ******************************************************
#ifdef DEBUG
static char bch_tx_buffer[MAX_BUFFER];
static unsigned int bch_tx_data_len = 0;
static char bch_queue_tx_buffer[MAX_BUFFER];
static unsigned int bch_queue_tx_data_len = 0;
#endif

static char lora_tx_buffer[MAX_BUFFER];
static unsigned int lora_tx_data_len = 0;


#ifdef DEBUG
static char lora_rx_buffer[MAX_BUFFER];
static unsigned int lora_rx_data_len = 0;
#endif

// The baud rate values were calculated at:
// http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
void init_uart() {
#ifdef DEBUG
  // Configure USCI_A0 for UART mode
  UCA0CTLW0 = UCSWRST;          // enable software reset
  UCA0CTLW0 |= UCSSEL__SMCLK;   // CLK = SMCLK

  // Baud Rate calculation
  // 8000000/(16*9600) = 52.083
  // Fractional portion = 0.083
  // UCBRFx = int ( (52.083-52)*16) = 1
  UCA0BR0 = 52;                 // 8000000/16/9600
  UCA0BR1 = 0x00;
  UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;

  UCA0CTLW0 &= ~UCSWRST;        // Initialize eUSCI, disable software reset
  UCA0IE |= UCRXIE;             // Enable USCI_A0 RX interrupt
  UCA0IE |= UCTXIE;             // Enable USCI_A0 TX interrupt
#endif

  // Configure USCI_A3 for UART mode
  UCA3CTLW0 = UCSWRST;          // enable software reset
  UCA3CTLW0 |= UCSSEL__SMCLK;   // CLK = SMCLK

  // Baud Rate calculation
  // 8000000/(16*115200) = 4.340
  // Fractional portion = 0.340
  // UCBRFx = int ((4.340-4)*16) = 5
  UCA3BR0 = 4;                 // 8000000/16/115200
  UCA3BR1 = 0x00;
  UCA3MCTLW |= UCOS16 | UCBRF_5 | 0x5500;

  UCA3CTLW0 &= ~UCSWRST;        // Initialize eUSCI, disable software reset
  UCA3IE |= UCRXIE;             // Enable USCI_A3 RX interrupt
  UCA3IE |= UCTXIE;             // Enable USCI_A3 TX interrupt
}

#ifdef DEBUG
int back_channel_write(const char *buffer, unsigned int buffer_len) {
  //initialize
  volatile unsigned int i;

  //check tx ready
  if (bch_tx_data_len == 0) {
    //set tx data len
    bch_tx_data_len = buffer_len + 1;

    //copy all data to buffer
    for (i = buffer_len; i > 0; i--) {
      //copy data to buffer
      bch_tx_buffer[buffer_len - i + 2] = buffer[i-1];
    }

    //append enter and line feed
    bch_tx_buffer[1] = ASCII_ENTER;
    bch_tx_buffer[0] = ASCII_LINEFEED;

    //write first byte
    UCA0TXBUF = buffer[0];
  } else { //tx is busy, queue 1 message
    //set data len
    bch_queue_tx_data_len = buffer_len + 2;

    //copy all data to buffer
    for (i = buffer_len; i > 0; i--) {
      //copy data to buffer
      bch_queue_tx_buffer[buffer_len - i + 2] = buffer[i-1];
    }

    //append enter and line feed
    bch_queue_tx_buffer[1] = ASCII_ENTER;
    bch_queue_tx_buffer[0] = ASCII_LINEFEED;
  }

  return 0;
}
#endif

int s76sxb_write(const char *buffer, unsigned int buffer_len) {
  //initialize
  volatile unsigned int i;

  //check tx ready
  if (lora_tx_data_len == 0) {
    //set tx data len
    lora_tx_data_len = buffer_len - 1;

    //copy all data to buffer
    for (i = buffer_len; i > 0; i--) {
      //copy data to buffer
      lora_tx_buffer[buffer_len - i] = buffer[i-1];
    }

    //write first byte
    UCA3TXBUF = buffer[0];
  }

  return 0;
}

#ifdef DEBUG
//*****************************************************************************
// USCI_A0 Interrupt Service Routine
//*****************************************************************************
#pragma vector = USCI_A0_VECTOR
__interrupt void uart_0_isr(void)
{
    switch ( __even_in_range( UCA0IV, USCI_UART_UCTXCPTIFG ))
    {
        case USCI_NONE:
            __no_operation();
            break;

        // UART receive interrupt
        case USCI_UART_UCRXIFG:
#ifdef DEBUG
            if (UCA0RXBUF == 'a') {
              //write to s76sxb
              s76sxb_write("sip get_ver", 11);
            } else if (UCA0RXBUF == 'b') {
              //write to s76sxb
              s76sxb_write("mac tx ucnf 15 1234", 19);
            } else if (UCA0RXBUF == 'c') {
              //init sensor
              init_hm3301_sensor();
            } else if (UCA0RXBUF == 'd') {
              //off led
              set_red_led(OFF);
              set_green_led(OFF);

              //read sensor value
              read_sensor_hm3301_value();
            }
#endif
            __no_operation();
            break;

        // UART transmit interrupt
        case USCI_UART_UCTXIFG:
            if ( bch_tx_data_len > 0 ) {
              //sent success
              bch_tx_data_len--;

              //send next data
              UCA0TXBUF = bch_tx_buffer[bch_tx_data_len];
            } else if (bch_queue_tx_data_len > 0) { //check queue
              //copy data len
              bch_tx_data_len = bch_queue_tx_data_len;

              //reset queue
              bch_queue_tx_data_len = 0;

              //copy data
              memcpy(bch_tx_buffer, bch_queue_tx_buffer, bch_tx_data_len);

              //decrease
              bch_tx_data_len--;

              //send first data
              UCA0TXBUF = bch_tx_buffer[bch_tx_data_len];
            }
            break;

        case USCI_UART_UCSTTIFG:
            __no_operation();
            break;

        case USCI_UART_UCTXCPTIFG:
            __no_operation();
            break;
    }
}
#endif

//*****************************************************************************
// USCI_A3 Interrupt Service Routine
//*****************************************************************************
#pragma vector = USCI_A3_VECTOR
__interrupt void uart_3_isr(void)
{
  switch ( __even_in_range( UCA3IV, USCI_UART_UCTXCPTIFG ))
  {
      case USCI_NONE:
        break;

      // UART receive interrupt
      case USCI_UART_UCRXIFG:
#ifdef DEBUG
          //check data
          if (UCA3RXBUF == ASCII_LINEFEED) {
            //write back channel
            back_channel_write(lora_rx_buffer, lora_rx_data_len);

            //reset
            lora_rx_data_len = 0;
            //notify data received
            __no_operation();
          } else {
            //get buffer
            lora_rx_buffer[lora_rx_data_len] = UCA3RXBUF;

            //append 1 data
            lora_rx_data_len++;
          }
#endif
          break;

      // UART transmit interrupt
      case USCI_UART_UCTXIFG:
          //toggle led
          if ( lora_tx_data_len > 0 ) {
            //sent success
            lora_tx_data_len--;

            //send next data
            UCA3TXBUF = lora_tx_buffer[lora_tx_data_len];
          } else {
            __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM1
          }
          break;

      case USCI_UART_UCSTTIFG:
          __no_operation();
          break;

      case USCI_UART_UCTXCPTIFG:
          __no_operation();
          break;
  }
}
