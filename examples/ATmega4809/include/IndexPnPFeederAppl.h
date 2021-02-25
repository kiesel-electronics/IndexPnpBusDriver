/*
 * IndexPnPFeederAppl.h
 *
 * Created: 25.02.2021 01:00:35
 *  Author: Friedrich
 */ 


#ifndef INDEXPNPFEEDERAPPL_H_
#define INDEXPNPFEEDERAPPL_H_

#include <IndexPnpBusInterface.h>

class IndexPnPFeederAppl : public IndexPnpBusSlave_cbk_Interface {
public:
  IndexPnpBusResponseCode getFeederId(uint8_t (&uuid_out)[12]);
  IndexPnpBusResponseCode initializeFeeder(uint8_t (&uuid_in)[12]);
  IndexPnpBusResponseCode getFeederVersion(uint8_t (&version_in)[4]);
  IndexPnpBusResponseCode moveFeederForward(uint8_t distance);
  IndexPnpBusResponseCode moveFeederBackward(uint8_t distance);

  // broadcast commands
  IndexPnpBusResponseCode getFeederAddress(uint8_t (&uuid_in)[12]);
protected:
private:
};



#endif /* INDEXPNPFEEDERAPPL_H_ */