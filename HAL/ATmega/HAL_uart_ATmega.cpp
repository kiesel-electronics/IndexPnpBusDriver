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

#include "HAL_uart_ATmega.h"
#include "HAL_config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <util/atomic.h>

#define SERIAL_8N1 0x06

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

HAL_uart_Arduino::HAL_uart_Arduino(uint8_t _dirPin) {
  this->dirPin = _dirPin;
}


void HAL_uart_Arduino::Init(HAL_uart_cbk_Interface* _nextLayer, uint32_t _baudrate) {
  this->nextLayer = _nextLayer;
  this->baudrate = _baudrate;

  // init direction pin
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, LOW);

  // initialize hardware
  // Try u2x mode first
  uint16_t baud_setting = (F_CPU / 4 / baudrate - 1) / 2;
  UCSR0A = 1 << U2X0;

  // assign the baud_setting
  UBRR0H = baud_setting >> 8;
  UBRR0L = baud_setting;

  //set the data bits, parity, and stop bits
  UCSR0C = SERIAL_8N1;
  
  // enable tx and rx
  sbi(UCSR0B, RXEN0);
  sbi(UCSR0B, TXEN0);
  // set rx irq en
  sbi(UCSR0B, RXCIE0);
  // clear tx complete irq en
  cbi(UCSR0B, TXCIE0);
}


void HAL_uart_Arduino::setDirectionToTx(void) {
  digitalWrite(dirPin, HIGH);
}


void HAL_uart_Arduino::setDirectionToRx(void) {
  digitalWrite(dirPin, LOW);
}


bool HAL_uart_Arduino::writeByte(uint8_t byte) {
  // send byte
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    UDR0 = byte;
    UCSR0A = (UCSR0A & ((1 << U2X0) | (1 << MPCM0))) | (1 << TXC0);
  }
  return true;
}


void HAL_uart_Arduino::enableTxInterrupt(void) {
  sbi(UCSR0B, TXCIE0);
}


void HAL_uart_Arduino::disableTxInterrupt(void) {
  cbi(UCSR0B, TXCIE0);
}


uint32_t HAL_uart_Arduino::getBaudrate(void) {
  return baudrate;
}


uint32_t HAL_uart_Arduino::getTime_us(void) {
  return micros();
}


void HAL_uart_Arduino::rxIrq(void) {
  uint8_t byte;
  byte = UDR0;
  nextLayer->rxComplete(byte);
}


void HAL_uart_Arduino::txIrq(void) {
  nextLayer->txComplete();
}