/*
 * IndexPnpBus_Interface.h
 *
 * Created: 21.02.2021 19:04:14
 *  Author: f.kiesel
 */ 


#ifndef INDEXPNPBUS_INTERFACE_H_
#define INDEXPNPBUS_INTERFACE_H_

#include "IndexPnpBusPdu.h"

class IndexPnpBusSlave_cbk_Interface {
public:
  virtual IndexPnpBusResponseCode getFeederId(uint8_t (&uuid_out)[12]);
  virtual IndexPnpBusResponseCode initializeFeeder(uint8_t (&uuid_in)[12]);
  virtual IndexPnpBusResponseCode getFeederVersion(uint8_t (&version_in)[4]);
  virtual IndexPnpBusResponseCode moveFeederForward(uint8_t distance);
  virtual IndexPnpBusResponseCode moveFeederBackward(uint8_t distance);

  // broadcast commands
  virtual IndexPnpBusResponseCode getFeederAddress(uint8_t (&uuid_in)[12]);
};



#endif /* INDEXPNPBUS_INTERFACE_H_ */