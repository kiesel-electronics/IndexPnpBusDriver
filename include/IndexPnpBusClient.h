/*
 * IndexPnpBus.h
 *
 * Created: 21.02.2021 19:01:54
 *  Author: f.kiesel
 */ 


#ifndef INDEXPNPBUSCLIENT_H_
#define INDEXPNPBUSCLIENT_H_

#include "IndexPnpBusLinkLayer.h"

class IndexPnpBusClient : public IndexPnpBusLinkLayer {
public:
  IndexPnpBusClient();
  void Init(IndexPnpBusClient_cbk_Interface* _appModule);

protected:
  void receivePdu(IndexPnpBusPdu &pdu);
  void txFrameComplete(void);

private:
  IndexPnpBusClient_cbk_Interface* appModule;
};



#endif /* INDEXPNPBUSCLIENT_H_ */