/*
 * IndexPnpBus_Interface.h
 *
 * Created: 21.02.2021 19:04:14
 *  Author: f.kiesel
 */ 


#ifndef INDEXPNPBUS_INTERFACE_H_
#define INDEXPNPBUS_INTERFACE_H_

#include "IndexPnpBusPdu.h"

class IndexPnpBusClient_cbk_Interface {
public:
  virtual IndexPnpBusResponseCode getFeederId(uint8_t (&uuid_out)[12]);
  virtual IndexPnpBusResponseCode initializeFeeder(uint8_t (&uuid_in)[12]);
  virtual IndexPnpBusResponseCode getFeederVersion(uint8_t (&version_in)[4]);
  virtual IndexPnpBusResponseCode moveFeederForward(uint8_t distance);
  virtual IndexPnpBusResponseCode moveFeederBackward(uint8_t distance);

  // broadcast commands
  virtual IndexPnpBusResponseCode getFeederAddress(uint8_t (&uuid_in)[12]);
};

class IndexPnpBusHost_cbk_Interface {
  public:
  virtual void getFeederIdResponse(IndexPnpBusResponseCode responseCode, uint8_t *uuid_in);
  virtual void initializeFeederResponse(IndexPnpBusResponseCode responseCode);
  virtual void getFeederVersionResponse(IndexPnpBusResponseCode responseCode, uint8_t *version_in);
  virtual void moveFeederForwardResponse(IndexPnpBusResponseCode responseCode);
  virtual void moveFeederBackwardResponse(IndexPnpBusResponseCode responseCode);

  // broadcast commands
  virtual void getFeederAddressResponse(IndexPnpBusResponseCode responseCode, uint8_t feederAddress);
};


#endif /* INDEXPNPBUS_INTERFACE_H_ */