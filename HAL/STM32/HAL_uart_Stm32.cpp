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

#include "HAL_uart_Stm32.h"


HAL_uart_Stm32::HAL_uart_Stm32(USART_TypeDef* _usart, uint32_t _dePin, uint32_t _rePin, int (*_tx_callback)(serial_t *), void (*_rx_callback)(serial_t *)) {
  _serial.pin_rx = pinmap_pin(_usart, PinMap_UART_RX);
  _serial.pin_tx = pinmap_pin(_usart, PinMap_UART_TX);
  dePin = digitalPinToPinName(_dePin);
  rePin = digitalPinToPinName(_rePin);

  rx_callback   = _rx_callback;
  tx_callback   = _tx_callback;

  _serial.rx_buff = _rx_buffer;
  _serial.rx_head = 0;
  _serial.rx_tail = 0;
  _serial.tx_buff = _tx_buffer;
  _serial.tx_head = 0;
  _serial.tx_tail = 0;
}


void HAL_uart_Stm32::Init(HAL_uart_cbk_Interface* _nextLayer, uint32_t _baudrate) {
  this->nextLayer = _nextLayer;
  this->baudrate = _baudrate;

  pinMode(pinNametoDigitalPin(dePin), OUTPUT);
  pinMode(pinNametoDigitalPin(rePin), OUTPUT);
  setDirectionToRx();

  uart_init(&_serial, (uint32_t)_baudrate, UART_WORDLENGTH_8B, UART_PARITY_NONE, UART_STOPBITS_1);
  uart_attach_rx_callback(&_serial, rx_callback);
}


void HAL_uart_Stm32::setDirectionToTx(void) {
  digitalWriteFast(dePin, HIGH);
  digitalWriteFast(rePin, HIGH);
}


void HAL_uart_Stm32::setDirectionToRx(void) {
  digitalWriteFast(dePin, LOW);
  digitalWriteFast(rePin, LOW);
}


bool HAL_uart_Stm32::writeByte(uint8_t byte) {
  uint8_t c = byte;
  // send byte
  tx_buffer_index_t i = (_serial.tx_head + 1) % SERIAL_TX_BUFFER_SIZE;
  while (i == _serial.tx_tail) {
    // nop, the interrupt handler will free up space for us
  }
  _serial.tx_buff[_serial.tx_head] = c;
  _serial.tx_head = i;

  if (!serial_tx_active(&_serial)) {
    uart_attach_tx_callback(&_serial, tx_callback);
    return true;
  } else {
    return false;
  }
}


void HAL_uart_Stm32::enableTxInterrupt(void) {
  // Nothing to do
}


void HAL_uart_Stm32::disableTxInterrupt(void) {
  // Nothing to do
}


uint32_t HAL_uart_Stm32::getBaudrate(void) {
  return baudrate;
}


uint32_t HAL_uart_Stm32::getTime_us(void) {
  return micros();
}


void HAL_uart_Stm32::_rx_complete_irq(serial_t *obj) {
  // No Parity error, read byte and store it in the buffer if there is room
  unsigned char c;

  if (uart_getc(obj, &c) == 0) {
    nextLayer->rxComplete(c);
  }
}


int HAL_uart_Stm32::_tx_complete_irq(serial_t *obj) {
  // TX complete
  obj->tx_tail = (obj->tx_tail + 1) % SERIAL_TX_BUFFER_SIZE;

  nextLayer->txComplete();

  if (obj->tx_head == obj->tx_tail) {
    return -1;
  }
  return 0;
}

