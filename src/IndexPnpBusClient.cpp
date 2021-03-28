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


#include "IndexPnpBusClient.h"

IndexPnpBusClient::IndexPnpBusClient(uint8_t* _uuid, uint8_t* _appVersion) {
  uuid = _uuid;
  appVersion = _appVersion;
  initialized_flg = false;
};


void IndexPnpBusClient::Init(IndexPnpBusClient_cbk_Interface* _appModule) {
  this->appModule = _appModule;
}


void IndexPnpBusClient::receivePdu() {
  IndexPnpBusFunctionCode command = (IndexPnpBusFunctionCode)(rxPdu.payload[0]);

  if (!initialized_flg) {
    // feeder not initialized
    if ( (command != IndexPnpBusFunctionCode::getFeederId)
      && (command != IndexPnpBusFunctionCode::initializeFeeder)
      && (command != IndexPnpBusFunctionCode::getVersion)
      && (command != IndexPnpBusFunctionCode::getFeederAddress)
    ) { // send feeder uninitialized answer
      txPdu.buildResponse(IndexPnpBusResponseCode::uninitializedFeeder, deviceAddress, 0, uuid);
      transmitPdu();
      return;
    }
  }
  // either execute the command or set the status to do it in the handler
  state = clientStateType::idle;
  switch(command) {
    case IndexPnpBusFunctionCode::getFeederId:
      getFeederId();
    break;

    case IndexPnpBusFunctionCode::initializeFeeder:
      state = clientStateType::initFeeder;
    break;

    case IndexPnpBusFunctionCode::getVersion:
      getFeederVersion();
    break;

    case IndexPnpBusFunctionCode::moveFeedForward:
      state = clientStateType::moveForward;
    break;

    case IndexPnpBusFunctionCode::moveFeedBackward:
      state = clientStateType::moveBackward;
    break;

    case IndexPnpBusFunctionCode::getFeederAddress:
      getFeederAddress();
    break;
  }
  return;
}


void IndexPnpBusClient::txFrameComplete(void) {
  // Nothing to do for the client
}


void IndexPnpBusClient::SendTestFrm() {
  uint8_t pay[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
  txPdu.buildResponse(IndexPnpBusResponseCode::ok, deviceAddress, 8, pay);
  transmitPdu();
}


/*******************************************************************************
 * Handle commands
 *******************************************************************************/
void IndexPnpBusClient::getFeederId() {
  // this is called from the interrupt, leave as fast as possibel
  // get uuid and send it back
  txPdu.buildResponse(IndexPnpBusResponseCode::ok, deviceAddress, 12, (uint8_t*)uuid);
  transmitPdu();
}

void IndexPnpBusClient::getFeederVersion() {
  // this is called from the interrupt, leave as fast as possibel
  txPdu.buildResponse(IndexPnpBusResponseCode::ok, deviceAddress, 4, appVersion);
  transmitPdu();
}

void IndexPnpBusClient::getFeederAddress() {
  // this is called from the interrupt, leave as fast as possibel
  // compare uuids
  int res = memcmp(&rxPdu.payload[1], (uint8_t*)uuid, 12);
  if (res == 0) {
    txPdu.buildResponse(IndexPnpBusResponseCode::ok, deviceAddress, 12, (uint8_t*)uuid);
    transmitPdu();
  }
}


/*******************************************************************************
 * Handler function
 *******************************************************************************/
void IndexPnpBusClient::handler() {
  static uint8_t txData[30];
  uint8_t txDataLength;
  IndexPnpBusResponseCode res = IndexPnpBusResponseCode::processing;
  // this is called in the while 1 loop
  if (state == clientStateType::idle)
    return;

  switch (state) {
    case clientStateType::initFeeder:
      res = appModule->initializeFeeder(&rxPdu.payload[1]);
      txDataLength = 0;
      if (res == IndexPnpBusResponseCode::ok) {
        initialized_flg = true;
      }
      break;

    case clientStateType::moveForward:
      res = appModule->moveFeederForward(rxPdu.payload[1]);
      txDataLength = 0;
      break;

    case clientStateType::moveBackward:
      res = appModule->moveFeederBackward(rxPdu.payload[1]);
      txDataLength = 0;
      break;

    default:
        state = clientStateType::idle;
      break;
  }
  if (res != IndexPnpBusResponseCode::processing) {
    state = clientStateType::idle;
    txPdu.buildResponse(res, deviceAddress, txDataLength, txData);
    transmitPdu();
  }
}
