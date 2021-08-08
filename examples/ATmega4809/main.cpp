 /******************************************************************************
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2021 Friedrich Kiesel
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *******************************************************************************/
 
#include <atmel_start.h>
#include "HAL_uart_Atmega0.h"
#include "IndexPnpBusClient.h"
#include <IndexPnPFeederClientAppl.h>
#include "wiring.h"

extern "C" void __cxa_pure_virtual() { while (1); }

uint8_t version_in[4] = {0x00, 0x00, 0x00, 0x01};
uint8_t uuid[12] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB};

HAL_uart_Atmega0 IndexPnpUart_0(&USART0);
IndexPnpBusClient IndexPnpBusClient_0(uuid, version_in);
IndexPnPFeederClientAppl IndexPnpBusAppl;

uint8_t position = 0x02;

uint32_t Timer500ms;

int main(void)
{
  	/* Initializes MCU, drivers and middleware */
  	atmel_start_init();
    init();

    IndexPnpUart_0.Init(&IndexPnpBusClient_0, 19200);
    IndexPnpBusClient_0.InitLl(&IndexPnpUart_0, position);
    IndexPnpBusClient_0.Init(&IndexPnpBusAppl);

    Timer500ms = IndexPnpUart_0.getTime_us();

    // Init LED pin
    PORTF_set_pin_dir(5, PORT_DIR_OUT);
    PORTF_set_pin_pull_mode(5, PORT_PULL_OFF);
    PORTF_set_pin_level(5, false);

    while (1) {
      IndexPnpBusClient_0.handler();
      uint32_t current_time_us = IndexPnpUart_0.getTime_us();
      if (current_time_us > Timer500ms ) {
        Timer500ms += 500000;
        PORTF_toggle_pin_level(5);
      }
    }
}


/* Interrupt service routine for RX complete */
ISR(USART0_RXC_vect)
{
  IndexPnpUart_0.rxIrq();
}

/* Interrupt service routine for Tx complete */
ISR(USART0_TXC_vect)
{
  IndexPnpUart_0.txIrq();
}