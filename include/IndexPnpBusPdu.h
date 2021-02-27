/*
 * IndexPnpBusPdu.h
 *
 * Created: 22.02.2021 01:32:51
 *  Author: f.kiesel
 */ 


#ifndef INDEXPNPBUSPDU_H_
#define INDEXPNPBUSPDU_H_

#include "IndexPnpBus_config.h"
#include "IndexPnPBusEnums.h"
#include "string.h"


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