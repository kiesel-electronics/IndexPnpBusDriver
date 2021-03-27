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

#include <Arduino.h>
#include "stm32yyxx_ll.h"
#include "HAL_uart_Stm32.h"
#include "IndexPnpBusClient.h"
#include "IndexPnPFeederClientAppl.h"


#define RS485_DE_PIN   (PA15)
#define RS485_RE_PIN   (PA8)

int uart1_tx_cbk(serial_t *obj);
void uart1_rx_cbk(serial_t *obj);

HAL_uart_Stm32 IndexPnpUart_1(USART1, RS485_DE_PIN, RS485_RE_PIN, uart1_tx_cbk, uart1_rx_cbk);
IndexPnpBusClient IndexPnpBusClient_1;
IndexPnPFeederClientAppl IndexPnpBusAppl((uint8_t *)UID_BASE_ADDRESS);
uint32_t Timer500ms;

int uart1_tx_cbk(serial_t *obj) {
  return IndexPnpUart_1._tx_complete_irq(obj);
}

void uart1_rx_cbk(serial_t *obj) {
  IndexPnpUart_1._rx_complete_irq(obj);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  SerialUSB.begin();
  Serial1.begin(9600);

  IndexPnpUart_1.Init(&IndexPnpBusClient_1, 19200);
  IndexPnpBusClient_1.InitLl(&IndexPnpUart_1, 0x02);
  IndexPnpBusClient_1.Init(&IndexPnpBusAppl);

  Timer500ms = IndexPnpUart_1.getTime_us();
}

void loop() {
  static uint8_t led_cnt = 0;

  if (Timer500ms < IndexPnpUart_1.getTime_us()){
    Timer500ms += 500000;
    digitalWrite(LED_BUILTIN, led_cnt&0x01);
    led_cnt++;
  }
}