# Low-power LoRaWAN Transmitter for MSP430FR5994

This code implements the functions needed for a class A compatible LoRaWAN device. 
Compatible to TTN, using module S76SXB for LoRaWAN transmission.


## Getting Started

After downloading the code you have to change the configuration in the define.h. Especially:
- Networkkey, App_key, device_adress to get TTN working

Flashing the code:
Open in CodeComposer and flash it.
The code will send join otaa to TTN Gateway after startup.

### Prerequisites

You are going to need Code Composer to compile, flash the code.
The code is tested on the MSP430FR5994, but should also work on other MSP430 with modifing pins.

Default Pins used for the S76SXB LoRa Transmitter


| MSP430FR5994 | S76SXB | Description |
| --- | --- | --- |
| UCA3RXD/P6.1 | U_TX | UART |
| UCA3TXD/P6.0 | U_RX | UART |


## Authors
Cong Nga Le

## License

This project is licensed under the MIT License

