 /******************************************************************************
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2021 Friedrich Kiesel
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *******************************************************************************/


#ifndef IndexPnpBusLinkLayer_H_
#define IndexPnpBusLinkLayer_H_

#include "HAL_uart_Interface.h"
#include "IndexPnpBusInterface.h"
#include "IndexPnpBusPdu.h"



#define INDEX_PNP_BUS_DUMMY_BYTE (0x00)


class IndexPnpBusLinkLayer : public HAL_uart_cbk_Interface {
  public:
    void InitLl(HAL_uart_Interface* _uart, uint8_t _deviceAddress);

	  void rxComplete(uint8_t byte);
	  void txComplete(void);
    uint32_t getLastBusActivity_us(void);
    uint16_t getErrorCnt();

    virtual void receivePdu(IndexPnpBusPdu &pdu) = 0;
    virtual void txFrameComplete(void) = 0;
    
  protected:
    int transmitPdu(IndexPnpBusPdu &pdu);
    uint8_t deviceAddress;

    HAL_uart_Interface* uart;
    uint32_t lastBusActivityTimestamp_us;
    uint32_t frame_timeout;
        
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
      
    IndexPnpBusPdu txPdu;
    txFrameStatusType txStatus;

    IndexPnpBusPdu rxPdu;
    rxFrameStatusType rxStatus;
    uint16_t rxErrorCnt;
    uint16_t rxSuccessCnt;
};



#endif /* IndexPnpBusLinkLayer_H_ */