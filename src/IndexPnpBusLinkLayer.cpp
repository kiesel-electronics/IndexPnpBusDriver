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

#include "IndexPnpBusLinkLayer.h"

void IndexPnpBusLinkLayer::InitLl(HAL_uart_Interface* _uart, uint8_t _deviceAddress) {
  uart = _uart;
  lastBusActivityTimestamp_us = uart->getTime_us();
  deviceAddress = _deviceAddress;
  rxStatus = rxFrameStatusType::idle;
  txStatus = txFrameStatusType::idle;
  rxPdu.Init();
  txPdu.Init();
  rxErrorCnt = 0;
  rxSuccessCnt = 0;
  // timeout in us, 10Bit per byte, 3.5 Byte timeout
  frame_timeout = (uint32_t)1000000/uart->getBaudrate()*35;
}


uint32_t IndexPnpBusLinkLayer::getLastBusActivity_us() {
  return lastBusActivityTimestamp_us;
}


uint16_t IndexPnpBusLinkLayer::getErrorCnt() {
  return rxErrorCnt;
}


void IndexPnpBusLinkLayer::rxComplete(uint8_t rxByte) {
  uint32_t current_time_us = uart->getTime_us();
  // check for new frame condition
  if ((current_time_us - lastBusActivityTimestamp_us) > frame_timeout) {
    // new frame started due to timeout, check if a new frame is expected
    if ((rxStatus != rxFrameStatusType::idle) && (rxStatus != rxFrameStatusType::ignoreRestOfFrame)) {
      // report error TODO
      rxErrorCnt++;
      // discard current frame and start a new one
    } 
    // ready to process device address
    rxStatus = rxFrameStatusType::deviceAdderss;
    rxPdu.Init();
  } 
  else if (rxStatus == rxFrameStatusType::idle) {
    // no data expected until frame timeout
    rxErrorCnt++;
    // handle error TODO
    rxStatus = rxFrameStatusType::ignoreRestOfFrame;
  } 
  lastBusActivityTimestamp_us = current_time_us;

  // process data
  switch (rxStatus) {
    case rxFrameStatusType::idle:
      // unexpected state, this is an implementation error!
      // TODO
    break;

    case rxFrameStatusType::ignoreRestOfFrame:
    // do nothing here
    break;
    
    case rxFrameStatusType::deviceAdderss:
      // received device address
      rxPdu.deviceAddress = rxByte;
      rxStatus = rxFrameStatusType::payloadLength;
    break;
    
    case rxFrameStatusType::payloadLength:
      // received payload size
      if (rxPdu.payloadLength > INDEX_PNP_BUS_PAYLOAD_MAX_SIZE) {
        // payload too big for buffer
        rxErrorCnt++;
        // TODO handle error
        rxStatus = rxFrameStatusType::ignoreRestOfFrame;
        break;
      }
      rxPdu.payloadLength = rxByte;
      rxStatus = rxFrameStatusType::payload;
    break;
    
    case rxFrameStatusType::payload:
      //receive payload
      rxPdu.payload[rxPdu.currentPosition] = rxByte;
      rxPdu.currentPosition++;
      if (rxPdu.currentPosition == rxPdu.payloadLength) {
        // payload finished
        rxStatus = rxFrameStatusType::checksumLow;
      }
    break;
    
    case rxFrameStatusType::checksumLow:
      // received low part of CRC16
      rxPdu.crc = rxByte;
      rxStatus = rxFrameStatusType::checksumHigh;
    break;

    case rxFrameStatusType::checksumHigh:
      // this is the laxt byte of the frame
      rxStatus = rxFrameStatusType::idle;
      // received high part of CRC16, the frame is now complete
      rxPdu.crc |= ((uint16_t)rxByte<< 8);
      // check CRC
      if (rxPdu.checkCrc()) {
        // frame is valid
        rxSuccessCnt++;
        if ((rxSuccessCnt%128) == 0) {
          // decrease error counter by 1 every 128 successful received frames
          if (rxErrorCnt > 0) {
            rxErrorCnt--;
          }
        }
        // forward only frames for this device
        if (rxPdu.deviceAddress == deviceAddress) {
          receivePdu(rxPdu);
        }
      } else {
        // CRC error, discard frame
        rxErrorCnt++;
        // TODO handle error
      }
    break;
  }
}



int IndexPnpBusLinkLayer::transmitPdu(IndexPnpBusPdu &pdu) {
  // the function calling this method has to make sure, that the 3.5 byte long timeout has passed
  uint32_t current_time_us = uart->getTime_us();
  if (txStatus != txFrameStatusType::idle) {
    // other Tx frame still in progress
    // TODO handle error
    return -1;
  }
  
  // copy PDU
  txPdu = pdu;
  // calculate crc, init current position
  txPdu.crc = txPdu.calculateCrc16();
  txPdu.currentPosition = 0;

  if ((current_time_us - lastBusActivityTimestamp_us) < frame_timeout) {
    // timeout for new frame not yet elapsed, send 4 dummy bytes
    if(rxStatus != rxFrameStatusType::idle){
      // rx frame in progress
      // TODO
    }
    txStatus = txFrameStatusType::dummy_1;
    uart->setDirectionToRx();

  } else {
    // timeout already elapsed
    txStatus = txFrameStatusType::startFrame;
    rxStatus = rxFrameStatusType::idle;
  }
  // trigger transmission
  txComplete();
  uart->enableTxInterrupt();
  return 0;
}


void IndexPnpBusLinkLayer::txComplete(void) {
  uint8_t byte;

  switch (txStatus) {
    case txFrameStatusType::idle:
    // unexpected state TODO
    break;

    case txFrameStatusType::dummy_1:
      uart->writeByte(INDEX_PNP_BUS_DUMMY_BYTE);
      txStatus = txFrameStatusType::dummy_2;
    break;

    case txFrameStatusType::dummy_2:
    uart->writeByte(INDEX_PNP_BUS_DUMMY_BYTE);
    txStatus = txFrameStatusType::dummy_3;
    break;

    case txFrameStatusType::dummy_3:
    uart->writeByte(INDEX_PNP_BUS_DUMMY_BYTE);
    txStatus = txFrameStatusType::dummy_4;
    break;

    case txFrameStatusType::dummy_4:
    uart->writeByte(INDEX_PNP_BUS_DUMMY_BYTE);
    txStatus = txFrameStatusType::startFrame;
    break;

    case txFrameStatusType::startFrame:
      uart->setDirectionToTx();
      uart->writeByte(txPdu.deviceAddress);
      txStatus = txFrameStatusType::deviceAdderss;
    break;
        
    case txFrameStatusType::deviceAdderss:
      // deviceAdderss sent, sent payloadLength
      uart->writeByte(txPdu.payloadLength);
      txStatus = txFrameStatusType::payloadLength;
    break;

    case txFrameStatusType::payloadLength:
      // payloadLength sent, sent payload
      uart->writeByte(txPdu.payload[0]);
      txStatus = txFrameStatusType::payload;
    break;

    case txFrameStatusType::payload:
      // payload byte sent
      txPdu.currentPosition++;
      if (txPdu.currentPosition < txPdu.payloadLength) {
        // sent next byte
        uart->writeByte(txPdu.payload[txPdu.currentPosition]);
      } else {
        // payload finished, send crcLow
        byte = txPdu.crc&0x00FF;
        uart->writeByte(byte);
        txStatus = txFrameStatusType::checksumLow;
      }
    break;

    case txFrameStatusType::checksumLow:
      // crcLow sent, send crcHigh
      byte = (txPdu.crc&0xFF00) >> 8;
      uart->writeByte(byte);
      txStatus = txFrameStatusType::checksumHigh;
    break;

    case txFrameStatusType::checksumHigh:
      // crcHigh sent, frame is now finished
      txStatus = txFrameStatusType::idle;
      uart->disableTxInterrupt();
      txFrameComplete();
      uart->setDirectionToRx();
    break;
    }
}


