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
#include "gcodeParser.h"
#include "IndexPnpBusHost.h"

#include "HAL_uart_SAMD21G.h"
#include "IndexPnPFeederHostAppl.h"
#include "arduinoHelpers.h"
#include "project_config.h"



HAL_uart_SAMD21G IndexPnpUart_0(SERCOM0, RS485_1_RX_PIN, RS485_1_TX_PIN, SERCOM_RX_PAD_3, UART_TX_PAD_2, RS485_1_DIR_PIN);
IndexPnpBusHost IndexPnpBusHost_0;
IndexPnPFeederHostAppl HostApplication;

gcodeParser parser(&SerialUSB, '\n');
void cmd(uint8_t num_params, char** params);

void getFeederId(uint8_t num_params, char** params);
void getFeederVersion(uint8_t num_params, char** params);
void getFeederAddress(uint8_t num_params, char** params);
void initFeeder(uint8_t num_params, char** params);
void moveFeederForward(uint8_t num_params, char** params);
void moveFeederBackward(uint8_t num_params, char** params);
void setUuid(uint8_t num_params, char** params);
void setParam(uint8_t num_params, char** params);
void getParam(uint8_t num_params, char** params);

unsigned long TaskTimer;
unsigned long Task100msCnt;
unsigned long Task1000msCnt;


void setup() {
  // write your initialization code here
  SerialUSB.begin(9600);
  //while(!SerialUSB.available()){}

  parser.RegisterCommand("M134", &cmd);

  parser.RegisterCommand("M200", &getFeederId);
  parser.RegisterCommand("M201", &getFeederVersion);
  parser.RegisterCommand("M202", &getFeederAddress);
  parser.RegisterCommand("M203", &initFeeder);
  parser.RegisterCommand("M204", &moveFeederForward);
  parser.RegisterCommand("M205", &moveFeederBackward);
  parser.RegisterCommand("M206", &setUuid);
  parser.RegisterCommand("M207", &setParam);
  parser.RegisterCommand("M208", &getParam);

  // Init
  IndexPnpUart_0.Init(&IndexPnpBusHost_0, GCLK_FREQUENCY, 19200);
  IndexPnpBusHost_0.InitLl(&IndexPnpUart_0, 0x00);
  IndexPnpBusHost_0.Init(&HostApplication);

  HostApplication.Init(RS485_1_TX_LED, RS485_1_RX_LED);

  //

  // Init Task timer variables
  TaskTimer = millis();
  Task100msCnt = TaskTimer;
  Task1000msCnt = TaskTimer;
}

void loop() {
  // get time
  TaskTimer = millis();

  // 100ms Task
  if (TaskTimer - Task100msCnt >= 100) {
    Task100msCnt += 100;
  }

  // 1000ms Task
  if (TaskTimer - Task1000msCnt >= 1000) {
    Task1000msCnt += 1000;
  }

  // write your code here
  parser.Handler();
  HostApplication.Handler();
}


void SERCOM0_Handler() {
  IndexPnpUart_0.IrqHandler();
}


void getFeederId(uint8_t num_params, char** params) {
  if (num_params >= 1){
    //SerialUSB.println(params[0]);
    int feederId = atoi(params[0]);
    IndexPnpBusHost_0.getFeederId(feederId);
    //SerialUSB.println("Get feeder ID!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void getFeederVersion(uint8_t num_params, char** params) {
  if (num_params >= 1){
    //SerialUSB.println(params[0]);
    int feederId = atoi(params[0]);
    IndexPnpBusHost_0.getFeederVersion(feederId);
    //SerialUSB.println("Get feeder Version!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void getFeederAddress(uint8_t num_params, char** params) {
  if (num_params >= 1){
    char uuid_hex[24];
    uint8_t uuid_bytes[12];
    // convert uuid
    for (uint8_t  i=0; i<24; i++){
      uuid_hex[i] = params[1][23-i];
    }
    conv_hex_to_uint8(uuid_hex, uuid_bytes, 12);
    IndexPnpBusHost_0.getFeederAddress(uuid_bytes);
    //SerialUSB.println("Get feeder Address!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void initFeeder(uint8_t num_params, char** params) {
  if (num_params >= 2){
    //SerialUSB.println(params[0]);
    int feederId = atoi(params[0]);
    uint8_t uuid_in[12];
    memcpy(uuid_in, params[1], 12);
    IndexPnpBusHost_0.initializeFeeder(feederId, uuid_in);
    //SerialUSB.println("Init feeder!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void moveFeederForward(uint8_t num_params, char** params) {
  if (num_params >= 2){
    //SerialUSB.println(params[0]);
    int feederId = atoi(params[0]);
    int distance = atoi(params[1]);
    IndexPnpBusHost_0.moveFeederForward(feederId, distance);
    //SerialUSB.println("Move feeder forward!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void moveFeederBackward(uint8_t num_params, char** params) {
  if (num_params >= 2){
    //SerialUSB.println(params[0]);
    int feederId = atoi(params[0]);
    int distance = atoi(params[1]);
    IndexPnpBusHost_0.moveFeederBackward(feederId, distance);
    //SerialUSB.println("Move feeder backward!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void setUuid(uint8_t num_params, char** params) {
  if (num_params >= 2){
    //SerialUSB.println(params[0]);
    int feederId = atoi(params[0]);
    char uuid_hex[24];
    uint8_t uuid_bytes[12];
    // convert uuid
    for (uint8_t  i=0; i<24; i++){
      uuid_hex[i] = params[1][23-i];
    }
    conv_hex_to_uint8(uuid_hex, uuid_bytes, 12);
    IndexPnpBusHost_0.setUuid(feederId, uuid_bytes);
    //SerialUSB.println("Set uuid!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void setParam(uint8_t num_params, char** params) {
  if (num_params >= 3){
    //SerialUSB.println(params[0]);
    int feederId = atoi(params[0]);
    int paramAddr = atoi(params[1]);
    int value = atoi(params[2]);
    IndexPnpBusHost_0.setParam(feederId, paramAddr, value);
    //SerialUSB.println("Set parameter!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void getParam(uint8_t num_params, char** params) {
  if (num_params >= 1){
    //SerialUSB.println(params[0]);
    int feederId = atoi(params[0]);
    int paramAddr = atoi(params[1]);
    IndexPnpBusHost_0.getParam(feederId, paramAddr);
    //SerialUSB.println("Set parameter!");
  } else {
    SerialUSB.println("Parameter missing!");
  }
}


void cmd(uint8_t num_params, char** params){

  for (int i = 0; i < num_params; i++) {
    SerialUSB.println(params[i]);
  }
}