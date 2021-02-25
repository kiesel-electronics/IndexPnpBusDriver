/*
 * IndexPnPBusEnums.h
 *
 * Created: 25.02.2021 00:43:57
 *  Author: Friedrich
 */ 


#ifndef INDEXPNPBUSENUMS_H_
#define INDEXPNPBUSENUMS_H_

enum class IndexPnpBusResponseCode : uint8_t {
  ok = 0x00,
  wrongUuid = 0x01,
  motorFault = 0x02,
  uninitializedFeeder = 0x03,
  unexpectedPayloadSize = 0x04,
};

enum class IndexPnpBusFunctionCode : uint8_t {
  getFeederId = 0x01,
  initializeFeeder = 0x02,
  getVersion = 0x03,
  moveFeedForward = 0x04,
  moveFeedBackward = 0x05,
  getFeederAddress = 0xA0,
};

#endif /* INDEXPNPBUSENUMS_H_ */