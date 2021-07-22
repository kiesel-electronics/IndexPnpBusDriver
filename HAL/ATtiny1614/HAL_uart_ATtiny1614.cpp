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

#include "HAL_uart_ATtiny1614.h"
#include <port.h>


HAL_uart_ATtiny1614::HAL_uart_ATtiny1614(USART_t* _uartCh) {
  this->uartCh = _uartCh;
}


void HAL_uart_ATtiny1614::Init(HAL_uart_cbk_Interface* _nextLayer, uint32_t _baudrate) {
  this->nextLayer = _nextLayer;
  this->baudrate = _baudrate;

  // init direction pin
  PORTA_set_pin_dir(7, PORT_DIR_OUT);
  PORTA_set_pin_pull_mode(7, PORT_PULL_OFF);
  PORTA_set_pin_level(7, false);

  // initialize hardware
  #define INDEX_PNP_USART_BAUD_RATE(BAUD_RATE)  ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
  uartCh->BAUD = (uint16_t)INDEX_PNP_USART_BAUD_RATE(baudrate); /* set baud rate register */

  uartCh->CTRLA = 0 << USART_ABEIE_bp  /* Auto-baud Error Interrupt Enable: disabled */
                | 0 << USART_DREIE_bp  /* Data Register Empty Interrupt Enable: disabled */
                | 0 << USART_LBME_bp   /* Loop-back Mode Enable: disabled */
                | USART_RS485_OFF_gc   /* RS485 Mode disabled */
                | 1 << USART_RXCIE_bp  /* Receive Complete Interrupt Enable: enabled */
                | 0 << USART_RXSIE_bp  /* Receiver Start Frame Interrupt Enable: disabled */
                | 0 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */

  uartCh->CTRLB = 0 << USART_MPCM_bp     /* Multi-processor Communication Mode: disabled */
                | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
                | 1 << USART_RXEN_bp     /* Receiver Enable: enabled */
                | USART_RXMODE_NORMAL_gc /* Normal mode */
                | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
                | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

}


void HAL_uart_ATtiny1614::setDirectionToTx(void) {
  // PORTC_02
  PORTA_set_pin_level(7, true);
}


void HAL_uart_ATtiny1614::setDirectionToRx(void) {
  // PORTC_02
  PORTA_set_pin_level(7, false);
}


bool HAL_uart_ATtiny1614::writeByte(uint8_t byte) {
  // clear Tx complete ISR
  uartCh->STATUS = USART_TXCIF_bm;
  // send byte
	uartCh->TXDATAL = byte;
  return true;
}


void HAL_uart_ATtiny1614::enableTxInterrupt(void) {
  uartCh->CTRLA |= 1 << USART_TXCIE_bp;
}


void HAL_uart_ATtiny1614::disableTxInterrupt(void) {
  uartCh->CTRLA &= ~(1 << USART_TXCIE_bp);
}


uint32_t HAL_uart_ATtiny1614::getBaudrate(void) {
  return baudrate;
}


uint32_t HAL_uart_ATtiny1614::getTime_us(void) {
  return micros();
}


void HAL_uart_ATtiny1614::rxIrq(void) {
  uint8_t byte;
  byte = uartCh->RXDATAL;
  nextLayer->rxComplete(byte);
}


void HAL_uart_ATtiny1614::txIrq(void) {
  nextLayer->txComplete();
}
