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

#include "IndexPnpBusHost.h"

IndexPnpBusHost::IndexPnpBusHost() {

};

void IndexPnpBusHost::receivePdu() {

  switch((IndexPnpBusFunctionCode)(txPdu.payload[0])) {
    case IndexPnpBusFunctionCode::getFeederId:
      appModule->responseGetFeederId((IndexPnpBusResponseCode)rxPdu.payload[0], &rxPdu.payload[2]);
    break;

    case IndexPnpBusFunctionCode::initializeFeeder:
      appModule->responseInitializeFeeder((IndexPnpBusResponseCode)rxPdu.payload[0]);
    break;

    case IndexPnpBusFunctionCode::getVersion:
      appModule->responseGetFeederVersion((IndexPnpBusResponseCode)rxPdu.payload[0], &rxPdu.payload[2]);
    break;

    case IndexPnpBusFunctionCode::moveFeedForward:
      appModule->responseMoveFeederForward((IndexPnpBusResponseCode)rxPdu.payload[0]);
    break;

    case IndexPnpBusFunctionCode::moveFeedBackward:
      appModule->responseMoveFeederBackward((IndexPnpBusResponseCode)rxPdu.payload[0]);
    break;

    case IndexPnpBusFunctionCode::getFeederAddress:
      appModule->responseGetFeederAddress((IndexPnpBusResponseCode)rxPdu.payload[0], rxPdu.payload[2]);
    break;
  }
   return;
}


int IndexPnpBusHost::getFeederId(uint8_t feederAddress) {
  txPdu.deviceAddress = feederAddress;
  txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::getFeederId;
  txPdu.payloadLength = 1;
  return transmitPdu();
}

int IndexPnpBusHost::initializeFeeder(uint8_t feederAddress, uint8_t (&uuid_in)[12]) {
  txPdu.deviceAddress = feederAddress;
  txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::initializeFeeder;
  memcpy(txPdu.payload+1, uuid_in, 12);
  txPdu.payloadLength = 13;
  return transmitPdu();
}

int IndexPnpBusHost::getFeederVersion(uint8_t feederAddress) {
  txPdu.deviceAddress = feederAddress;
  txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::getVersion;
  txPdu.payloadLength = 1;
  return transmitPdu();
}

int IndexPnpBusHost::moveFeederForward(uint8_t feederAddress, uint8_t distance) {
  txPdu.deviceAddress = feederAddress;
  txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::moveFeedForward;
  txPdu.payload[1] = distance; 
  txPdu.payloadLength = 2;
  return transmitPdu();
}

int IndexPnpBusHost::moveFeederBackward(uint8_t feederAddress, uint8_t distance) {
  txPdu.deviceAddress = feederAddress;
  txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::moveFeedBackward;
  txPdu.payload[1] = distance;
  txPdu.payloadLength = 2;
  return transmitPdu();
}

// broadcast commands
int IndexPnpBusHost::getFeederAddress(uint8_t (&uuid_in)[12]) {
  txPdu.deviceAddress = 0xFF;
  txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::getFeederAddress;
  memcpy(txPdu.payload+1, uuid_in, 12);
  txPdu.payloadLength = 13;
  return transmitPdu();
}


void IndexPnpBusHost::txFrameComplete(void){
}

void IndexPnpBusHost::Init(IndexPnpBusHost_cbk_Interface* _appModule){
  this->appModule = _appModule;
}

