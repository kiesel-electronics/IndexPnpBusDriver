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

IndexPnpBusClient::IndexPnpBusClient() {
  
};


void IndexPnpBusClient::receivePdu(IndexPnpBusPdu &rxPdu) {
  IndexPnpBusPdu txPdu;
  IndexPnpBusResponseCode res;
  uint8_t uuid[12];
  uint8_t version[4];

  switch((IndexPnpBusFunctionCode)(rxPdu.payload[0])) {
    case IndexPnpBusFunctionCode::getFeederId:
      res = appModule->getFeederId(uuid);
      txPdu.buildResponse(res, 12, uuid);
      transmitPdu(txPdu);
    break;

    case IndexPnpBusFunctionCode::initializeFeeder:
      // copy uuid from payload
      memcpy(uuid, &rxPdu.payload[1], 12);
      res = appModule->initializeFeeder(uuid);
      txPdu.buildResponse(res, 0, uuid);
      transmitPdu(txPdu);
    break;

    case IndexPnpBusFunctionCode::getVersion:
      res = appModule->getFeederVersion(version);
      txPdu.buildResponse(res, 4, version);
      transmitPdu(txPdu);
    break;

    case IndexPnpBusFunctionCode::moveFeedForward:
      res = appModule->moveFeederForward(rxPdu.payload[1]);
      txPdu.buildResponse(res, 0, uuid);
      transmitPdu(txPdu);
    break;

    case IndexPnpBusFunctionCode::moveFeedBackward:
      res = appModule->moveFeederBackward(rxPdu.payload[1]);
      txPdu.buildResponse(res, 0, uuid);
      transmitPdu(txPdu);
    break;

    case IndexPnpBusFunctionCode::getFeederAddress:
      // copy uuid from payload
      memcpy(uuid, &rxPdu.payload[1], 12);
      res = appModule->getFeederAddress(uuid);
      txPdu.buildResponse(res, 0, uuid);
      if (res == IndexPnpBusResponseCode::ok) {
        transmitPdu(txPdu);
      }
    break;
  }

   return;
}


void IndexPnpBusClient::txFrameComplete(void) {

}


void IndexPnpBusClient::Init(IndexPnpBusClient_cbk_Interface* _appModule) {
  this->appModule = _appModule;
}


void IndexPnpBusClient::SendTestFrm() {
  uint8_t pay[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
      txPdu.buildResponse(IndexPnpBusResponseCode::ok, 8, pay);
      transmitPdu(txPdu);
}
