/*
 * IndexPnpBus.h
 *
 * Created: 21.02.2021 19:01:54
 *  Author: f.kiesel
 */ 


#ifndef INDEXPNPBUSHOST_H_
#define INDEXPNPBUSHOST_H_

#include "IndexPnpBusLinkLayer.h"

class IndexPnpBusHost : public IndexPnpBusLinkLayer {
public:
  IndexPnpBusHost();
  void Init(IndexPnpBusHost_cbk_Interface* _appModule);

  int getFeederId(uint8_t feederAddress);
  int initializeFeeder(uint8_t feederAddress, uint8_t (&uuid_in)[12]);
  int getFeederVersion(uint8_t feederAddress);
  int moveFeederForward(uint8_t feederAddress, uint8_t distance);
  int moveFeederBackward(uint8_t feederAddress, uint8_t distance);
  // broadcast commands
  int getFeederAddress(uint8_t (&uuid_in)[12]);

protected:
  void receivePdu(IndexPnpBusPdu &pdu);
  void txFrameComplete(void);

private:
  IndexPnpBusHost_cbk_Interface* appModule;
};



#endif /* INDEXPNPBUSHOST_H_ */