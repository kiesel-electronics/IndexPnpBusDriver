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
#include <IndexPnpBusHost.h>

#include "HAL_uart_SAMD21G.h"
#include "IndexPnpBusClient.h"
#include "IndexPnPFeederHostAppl.h"

#define RS485_RX_PIN    ((uint8_t)12)
#define RS485_TX_PIN    ((uint8_t)10)
#define RS485_DIR_PIN   ((uint8_t)2)
#define GCLK_FREQUENCY  (SystemCoreClock)


HAL_uart_SAMD21G IndexPnpUart_0(SERCOM1, RS485_RX_PIN, RS485_TX_PIN, SERCOM_RX_PAD_3, UART_TX_PAD_2, RS485_DIR_PIN);
IndexPnpBusHost IndexPnpBusHost_0;
IndexPnPFeederHostAppl HostApplication;

void SERCOM1_Handler() {
  IndexPnpUart_0.IrqHandler();
}


uint32_t timer500ms;

void setup() {
  // put your setup code here, to run once:

  Serial.println();
  SerialUSB.begin(9600);

  IndexPnpUart_0.Init(&IndexPnpBusHost_0, GCLK_FREQUENCY, 19200);
  IndexPnpBusHost_0.InitLl(&IndexPnpUart_0, 0x00);
  IndexPnpBusHost_0.Init(&HostApplication);
  timer500ms = millis();

}

void loop() {
  //uint8_t uuid_in[12] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78};
  static uint8_t led_cnt = 0;
  static uint8_t frm_cnt = 0;
  // put your main code here, to run repeatedly:
  //delay(1000);
  if(SerialUSB.available()) {
    int read = SerialUSB.read();
    SerialUSB.println(frm_cnt);
    IndexPnpBusHost_0.getFeederId(0x02);
    frm_cnt++;
  }
  
  if (timer500ms < millis()){
    timer500ms += 500;
    digitalWrite(LED_BUILTIN, led_cnt&0x01);
    led_cnt++;
  }


  //IndexPnpBusHost_0.initializeFeeder(0x04, uuid_in);

  
} 