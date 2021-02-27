/*
 * IndexPnpBusSlave.cpp
 *
 * Created: 21.02.2021 19:18:58
 *  Author: f.kiesel
 */ 

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


void IndexPnpBusClient::txFrameComplete(void){

}


void IndexPnpBusClient::Init(IndexPnpBusClient_cbk_Interface* _appModule){
  this->appModule = _appModule;
}

