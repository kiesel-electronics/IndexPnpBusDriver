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


#ifndef INDEXPNPBUSPDU_H_
#define INDEXPNPBUSPDU_H_

#include "HAL_platform.h"
#include "IndexPnPBusEnums.h"
#include "string.h"

#define INDEX_PNP_BUS_PAYLOAD_MAX_SIZE       (32)
#define INDEX_PNP_BUS_HOST_ADDRESS            (0x00u)

class IndexPnpBusPdu {
  public:
    uint8_t deviceAddress;
    uint8_t payloadLength;
    uint8_t payload[INDEX_PNP_BUS_PAYLOAD_MAX_SIZE];
    uint16_t crc;
    uint8_t currentPosition;

    IndexPnpBusPdu();
    void Init(void);
    uint16_t calculateCrc16(void);
    bool checkCrc(void);
    void buildRequest(IndexPnpBusFunctionCode functionCode, uint8_t targetAddress, uint8_t _payloadLength, uint8_t* _payload);
    void buildResponse(IndexPnpBusResponseCode responseCode, uint8_t _payloadLength, uint8_t* _payload);
       
  private:
    static const uint8_t crc16LookupHigh[256];
    static const uint8_t crc16LookupLow[256];
};

#endif /* INDEXPNPBUSPDU_H_ */