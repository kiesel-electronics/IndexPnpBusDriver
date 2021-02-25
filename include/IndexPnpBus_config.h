/*
 * IndexPnpBus_config.h
 *
 * Created: 21.02.2021 14:21:46
 *  Author: f.kiesel
 */ 


#ifndef INDEXPNPBUS_CONFIG_H_
#define INDEXPNPBUS_CONFIG_H_

#include "compiler.h"

#define INDEX_PNP_BUS_PAYLOAD_MAX_SIZE       32


#define INDEX_PNP_TIME_US                     ((volatile uint32_t)TCA0.SINGLE.CNT)
#define INDEX_PNP_BUS_BAUDRATE_BPS            (19200)
// calculate value for BAUD register
#define INDEX_PNP_USART_BAUD_RATE(BAUD_RATE)  ((float)(16000000 * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#define INDEX_PNP_BUS_BIT_TIME_US             ((double)1000000/INDEX_PNP_BUS_BAUDRATE_BPS)
#define INDEX_PNP_BUS_BYTE_TIME_US            (INDEX_PNP_BUS_BIT_TIME_US*10)
#define INDEX_PNP_BUS_FRAME_TIMOEUT           (INDEX_PNP_BUS_BIT_TIME_US*35)

#define INDEX_PNP_BUS_HOST_ADDRESS            (0x00u)

#endif /* INDEXPNPBUS_CONFIG_H_ */