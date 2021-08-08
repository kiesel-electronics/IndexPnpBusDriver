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


#ifndef INDEXPNPFEEDERAPPL_H_
#define INDEXPNPFEEDERAPPL_H_

#include <IndexPnpBusInterface.h>

class IndexPnPFeederHostAppl : public IndexPnpBusHost_cbk_Interface {
public:
  void Init(uint8_t _tx_led_pin, uint8_t _rx_led_pin);
  void Handler();

  void responseGetFeederId(IndexPnpBusResponseCode responseCode, uint8_t *uuid_in);
  void responseInitializeFeeder(IndexPnpBusResponseCode responseCode);
  void responseGetFeederVersion(IndexPnpBusResponseCode responseCode, uint8_t *version_in);
  void responseMoveFeederForward(IndexPnpBusResponseCode responseCode);
  void responseMoveFeederBackward(IndexPnpBusResponseCode responseCode);
  void responseSetUuid(IndexPnpBusResponseCode responseCode);
  void responseSetParam(IndexPnpBusResponseCode responseCode);
  void responseGetParam(IndexPnpBusResponseCode responseCode, uint8_t value);
  // broadcast commands
  void responseGetFeederAddress(IndexPnpBusResponseCode responseCode, uint8_t feederAddress);
  // led triggers
  void txLedTrigger();
  void rxLedTrigger();
protected:
private:
  volatile unsigned long txLedTimer;
  volatile unsigned long rxLedTimer;
  uint8_t tx_led_pin;
  uint8_t rx_led_pin;
  void sendResponse(IndexPnpBusResponseCode responseCode, bool addprintln);
};



#endif /* INDEXPNPFEEDERAPPL_H_ */