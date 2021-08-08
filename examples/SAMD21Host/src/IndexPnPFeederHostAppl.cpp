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

#include "IndexPnPFeederHostAppl.h"
#include "arduinoHelpers.h"
#include "project_config.h"


 void IndexPnPFeederHostAppl::Init(uint8_t _tx_led_pin, uint8_t _rx_led_pin) {
  txLedTimer = millis();
  rxLedTimer = txLedTimer;
  tx_led_pin = _tx_led_pin;
  rx_led_pin = _rx_led_pin;
  pinMode(tx_led_pin, OUTPUT);
  pinMode(rx_led_pin, OUTPUT);
  digitalWrite(tx_led_pin, HIGH);
  digitalWrite(rx_led_pin, HIGH);
}


void IndexPnPFeederHostAppl::Handler() {
  noInterrupts();
  unsigned long timer = millis();
  if (timer - txLedTimer >= RS485_LED_PULSE_MS) {
    digitalWrite(tx_led_pin, HIGH);
  }
  if (timer - rxLedTimer >= RS485_LED_PULSE_MS) {
    digitalWrite(rx_led_pin, HIGH);
  }
  interrupts();
}


// led triggers
void IndexPnPFeederHostAppl::txLedTrigger() {
  txLedTimer = millis();
  digitalWrite(tx_led_pin, LOW);
}


void IndexPnPFeederHostAppl::rxLedTrigger() {
  rxLedTimer = millis();
  digitalWrite(rx_led_pin, LOW);
}


// command responses
void IndexPnPFeederHostAppl::responseGetFeederId(IndexPnpBusResponseCode responseCode, uint8_t *uuid_in) {
  uint8_t uuid_conv[12];
  for (int i=0; i<12; i++) {
    uuid_conv[i] = uuid_in[11-i];
  }
  sendResponse(responseCode, false);
  PrintHex16(uuid_conv, 12, &SerialUSB, false);
}


void IndexPnPFeederHostAppl::responseInitializeFeeder(IndexPnpBusResponseCode responseCode) {
   sendResponse(responseCode, true);
}


void IndexPnPFeederHostAppl::responseGetFeederVersion(IndexPnpBusResponseCode responseCode, uint8_t *version_in) {
  sendResponse(responseCode, false);
  PrintHex16(version_in, 4, &SerialUSB, true);
}


void IndexPnPFeederHostAppl::responseMoveFeederForward(IndexPnpBusResponseCode responseCode) {
   sendResponse(responseCode, true);
}


void IndexPnPFeederHostAppl::responseMoveFeederBackward(IndexPnpBusResponseCode responseCode) {
   sendResponse(responseCode, true);
}


void IndexPnPFeederHostAppl::responseSetUuid(IndexPnpBusResponseCode responseCode) {
   sendResponse(responseCode, true);
}


void IndexPnPFeederHostAppl::responseSetParam(IndexPnpBusResponseCode responseCode) {
   sendResponse(responseCode, true);
}


void IndexPnPFeederHostAppl::responseGetParam(IndexPnpBusResponseCode responseCode, uint8_t value) {
   sendResponse(responseCode, false);
   SerialUSB.println(value);
 }


// broadcast commands
void IndexPnPFeederHostAppl::responseGetFeederAddress(IndexPnpBusResponseCode responseCode, uint8_t feederAddress) {
  sendResponse(responseCode, false);
  SerialUSB.println(feederAddress);
}


void IndexPnPFeederHostAppl::sendResponse(IndexPnpBusResponseCode responseCode, bool addprintln) {
  if (responseCode == IndexPnpBusResponseCode::ok) {
    if (addprintln) {
      SerialUSB.println("ok");
    } else {
      SerialUSB.print("ok ");
    }
  } else {
    SerialUSB.print("error ");
    SerialUSB.println((uint8_t)responseCode);
  }
}
