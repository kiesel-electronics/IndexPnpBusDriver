/*
 * IndexPnpBus.h
 *
 * Created: 21.02.2021 19:01:54
 *  Author: f.kiesel
 */ 


#ifndef INDEXPNPBUS_H_
#define INDEXPNPBUS_H_

#include "IndexPnpBusLinkLayer.h"

class IndexPnpBusClient : public IndexPnpBusLinkLayer {
public:
  IndexPnpBusClient();
  void Init(IndexPnpBusSlave_cbk_Interface* _appModule);
  void Handler();

protected:
  void receivePdu(IndexPnpBusPdu &pdu);
  void txFrameComplete(void);

private:
  IndexPnpBusSlave_cbk_Interface* appModule;
};



#endif /* INDEXPNPBUS_H_ */