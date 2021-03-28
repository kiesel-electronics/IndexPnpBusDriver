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


#ifndef INDEXPNPBUSCLIENT_H_
#define INDEXPNPBUSCLIENT_H_

#include "IndexPnpBusLinkLayer.h"

class IndexPnpBusClient : public IndexPnpBusLinkLayer {
public:
  IndexPnpBusClient(uint8_t* _uuid, uint8_t* _appVersion);
  void Init(IndexPnpBusClient_cbk_Interface* _appModule);
  void handler(void);
  void SendTestFrm();

protected:
  void receivePdu();
  void txFrameComplete(void);

private:
  uint8_t* uuid;
  uint8_t* appVersion;
  IndexPnpBusClient_cbk_Interface* appModule;
  bool initialized_flg;

  void getFeederId(void);
  void getFeederVersion(void);
  // broadcast commands
  void getFeederAddress(void);

  enum class clientStateType {
    idle,
    initFeeder,
    moveForward,
    moveBackward,
    };

  IndexPnpBusClient::clientStateType state;
};



#endif /* INDEXPNPBUSCLIENT_H_ */