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


#ifndef INDEXPNPBUS_INTERFACE_H_
#define INDEXPNPBUS_INTERFACE_H_

#include "IndexPnpBusPdu.h"

class IndexPnpBusClient_cbk_Interface {
public:
  virtual IndexPnpBusResponseCode initializeFeeder(uint8_t* uuid_in) = 0;
  virtual IndexPnpBusResponseCode moveFeederForward(uint8_t distance) = 0;
  virtual IndexPnpBusResponseCode moveFeederBackward(uint8_t distance) = 0;
  virtual IndexPnpBusResponseCode setUuid(uint8_t* uuid_in) = 0;
  virtual IndexPnpBusResponseCode setParam(uint8_t paramAddr, uint8_t value) = 0;
  virtual IndexPnpBusResponseCode getParam(uint8_t paramAddr, uint8_t *txData) = 0;
  // led triggers
  virtual void txLedTrigger() = 0;
  virtual void rxLedTrigger() = 0;  
};

class IndexPnpBusHost_cbk_Interface {
  public:
  virtual void responseGetFeederId(IndexPnpBusResponseCode responseCode, uint8_t *uuid_in) = 0;
  virtual void responseInitializeFeeder(IndexPnpBusResponseCode responseCode) = 0;
  virtual void responseGetFeederVersion(IndexPnpBusResponseCode responseCode, uint8_t *version_in) = 0;
  virtual void responseMoveFeederForward(IndexPnpBusResponseCode responseCode) = 0;
  virtual void responseMoveFeederBackward(IndexPnpBusResponseCode responseCode) = 0;
  virtual void responseSetUuid(IndexPnpBusResponseCode responseCode) = 0;
  virtual void responseSetParam(IndexPnpBusResponseCode responseCode) = 0;
  virtual void responseGetParam(IndexPnpBusResponseCode responseCode, uint8_t value) = 0;
  // broadcast commands
  virtual void responseGetFeederAddress(IndexPnpBusResponseCode responseCode, uint8_t feederAddress) = 0;
  // led triggers
  virtual void txLedTrigger() = 0;
  virtual void rxLedTrigger() = 0;
};


#endif /* INDEXPNPBUS_INTERFACE_H_ */