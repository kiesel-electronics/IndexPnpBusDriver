/*
 * IndexPnpBusLinkLayer.h
 *
 * Created: 21.02.2021 14:16:14
 *  Author: f.kiesel
 */ 


#ifndef IndexPnpBusLinkLayer_H_
#define IndexPnpBusLinkLayer_H_

#include "IndexPnpBus_config.h"
#include "HAL_uart_Interface.h"
#include "IndexPnpBusInterface.h"
#include "IndexPnpBusPdu.h"






class IndexPnpBusLinkLayer : public HAL_uart_cbk_Interface {
  public:
    void InitLl(HAL_uart_Interface* _uart, uint8_t _deviceAddress);

	  void rxComplete(uint8_t byte);
	  void txComplete(void);
    uint32_t getLastBusActivity_us(void);  

    virtual void receivePdu(IndexPnpBusPdu &pdu) = 0;
    virtual void txFrameComplete(void) = 0;
    
  protected:
    int transmitPdu(IndexPnpBusPdu &pdu);
    uint8_t deviceAddress;

  private:
    HAL_uart_Interface* uart;
    uint32_t lastBusActivityTimestamp_us;
        
    enum class rxFrameStatusType {
      idle,
      deviceAdderss,
      payloadLength,
      payload,
      checksumLow,
      checksumHigh,
      ignoreRestOfFrame,
      };

    enum class txFrameStatusType {
      idle,
      dummy_1,
      dummy_2,
      dummy_3,
      dummy_4,
      startFrame,
      deviceAdderss,
      payloadLength,
      payload,
      checksumLow,
      checksumHigh,
    };
      
    IndexPnpBusPdu rxPdu;
    rxFrameStatusType rxStatus;
    uint16_t rxErrorCnt;
    uint16_t rxSuccessCnt;
        
    IndexPnpBusPdu txPdu;
    txFrameStatusType txStatus;
};



#endif /* IndexPnpBusLinkLayer_H_ */