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

class IndexPnPFeederClientAppl : public IndexPnpBusClient_cbk_Interface {
public:
  IndexPnPFeederClientAppl();
  void Init();
  void Handler();  

  IndexPnpBusResponseCode initializeFeeder(uint8_t* uuid_in);
  IndexPnpBusResponseCode moveFeederForward(uint8_t distance);
  IndexPnpBusResponseCode moveFeederBackward(uint8_t distance);
  IndexPnpBusResponseCode setUuid(uint8_t* uuid_in);
  IndexPnpBusResponseCode setParam(uint8_t paramAddr, uint8_t value);
  IndexPnpBusResponseCode getParam(uint8_t paramAddr, uint8_t *txData);
  // led triggers
  void txLedTrigger();
  void rxLedTrigger();
private:
  volatile unsigned long txLedTimer;
  volatile unsigned long rxLedTimer;
};



#endif /* INDEXPNPFEEDERAPPL_H_ */