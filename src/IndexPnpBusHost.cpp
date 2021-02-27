/*
 * IndexPnpBusSlave.cpp
 *
 * Created: 21.02.2021 19:18:58
 *  Author: f.kiesel
 */ 

#include "IndexPnpBusHost.h"

IndexPnpBusHost::IndexPnpBusHost() {

};

void IndexPnpBusHost::receivePdu(IndexPnpBusPdu &rxPdu) {

  switch((IndexPnpBusFunctionCode)(txPdu.payload[0])) {
    case IndexPnpBusFunctionCode::getFeederId:
      appModule->getFeederIdResponse((IndexPnpBusResponseCode)rxPdu.payload[0], &rxPdu.payload[1]);
    break;

    case IndexPnpBusFunctionCode::initializeFeeder:
      appModule->initializeFeederResponse((IndexPnpBusResponseCode)rxPdu.payload[0]);
    break;

    case IndexPnpBusFunctionCode::getVersion:
      appModule->getFeederVersionResponse((IndexPnpBusResponseCode)rxPdu.payload[0], &rxPdu.payload[1]);      
    break;

    case IndexPnpBusFunctionCode::moveFeedForward:
      appModule->moveFeederForwardResponse((IndexPnpBusResponseCode)rxPdu.payload[0]);
    break;

    case IndexPnpBusFunctionCode::moveFeedBackward:
      appModule->moveFeederBackwardResponse((IndexPnpBusResponseCode)rxPdu.payload[0]);
    break;

    case IndexPnpBusFunctionCode::getFeederAddress:
      appModule->getFeederAddressResponse((IndexPnpBusResponseCode)rxPdu.payload[0], rxPdu.payload[1]);
    break;
  }
   return;
}


  int IndexPnpBusHost::getFeederId(uint8_t feederAddress) {
    txPdu.deviceAddress = feederAddress;
    txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::getFeederId;
    txPdu.payloadLength = 1;
    return transmitPdu(txPdu);
  }
  
  int IndexPnpBusHost::initializeFeeder(uint8_t feederAddress, uint8_t (&uuid_in)[12]) {
    txPdu.deviceAddress = feederAddress;
    txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::initializeFeeder;
    memcpy(txPdu.payload+1, uuid_in, 12);
    txPdu.payloadLength = 13;
    return transmitPdu(txPdu);
  }
  
  int IndexPnpBusHost::getFeederVersion(uint8_t feederAddress) {
    txPdu.deviceAddress = feederAddress;
    txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::getVersion;
    txPdu.payloadLength = 1;
    return transmitPdu(txPdu);
  }
  
  int IndexPnpBusHost::moveFeederForward(uint8_t feederAddress, uint8_t distance) {
    txPdu.deviceAddress = feederAddress;
    txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::moveFeedForward;
    txPdu.payload[1] = distance; 
    txPdu.payloadLength = 2;
    return transmitPdu(txPdu);
  }
  
  int IndexPnpBusHost::moveFeederBackward(uint8_t feederAddress, uint8_t distance) {
    txPdu.deviceAddress = feederAddress;
    txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::moveFeedBackward;
    txPdu.payload[1] = distance;
    txPdu.payloadLength = 2;
    return transmitPdu(txPdu);
  }
  
  // broadcast commands
  int IndexPnpBusHost::getFeederAddress(uint8_t (&uuid_in)[12]) {
    txPdu.deviceAddress = 0xFF;
    txPdu.payload[0] = (uint8_t)IndexPnpBusFunctionCode::getFeederAddress;
    memcpy(txPdu.payload+1, uuid_in, 12);
    txPdu.payloadLength = 13;
    return transmitPdu(txPdu);
  }


void IndexPnpBusHost::txFrameComplete(void){

}

void IndexPnpBusHost::Init(IndexPnpBusHost_cbk_Interface* _appModule){
  this->appModule = _appModule;
}

