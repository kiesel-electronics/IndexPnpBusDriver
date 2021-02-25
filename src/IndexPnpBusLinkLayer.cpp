/*
 * IndexPnpBusLinkLayer.cpp
 *
 * Created: 21.02.2021 14:31:53
 *  Author: f.kiesel
 */ 

#include "IndexPnpBusLinkLayer.h"

void IndexPnpBusLinkLayer::InitLl(HAL_uart_Interface* _uart, uint8_t _deviceAddress) {
  lastBusActivityTimestamp_us = INDEX_PNP_TIME_US;
  uart = _uart;
  deviceAddress = _deviceAddress;
  rxStatus = rxFrameStatusType::idle;
  txStatus = txFrameStatusType::idle;
  rxPdu.Init();
  txPdu.Init();
  rxErrorCnt = 0;
  rxSuccessCnt = 0;
}


uint32_t IndexPnpBusLinkLayer::getLastBusActivity_us(){
  return lastBusActivityTimestamp_us;
}


void IndexPnpBusLinkLayer::rxComplete(uint8_t rxByte) {
  uint32_t current_time_us = INDEX_PNP_TIME_US;
  // check for new frame condition
  if ((current_time_us - lastBusActivityTimestamp_us) > INDEX_PNP_BUS_FRAME_TIMOEUT) {
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
      rxStatus = rxFrameStatusType::idle;
    break;
  }
}



int IndexPnpBusLinkLayer::transmitPdu(IndexPnpBusPdu &pdu) {
  // the function calling this method has to make sure, that the 3.5 byte long timeout has passed
  uint32_t current_time_us = INDEX_PNP_TIME_US;
  if ((txStatus != txFrameStatusType::idle) || (rxStatus != rxFrameStatusType::idle)) {
    // other frame still in progress
    // TODO handle error
    return -1;
  }
  
  // copy PDU
  txPdu = pdu;
  // calculate crc, init current position
  txPdu.crc = txPdu.calculateCrc16();
  txPdu.currentPosition = 0;

  if ((current_time_us - lastBusActivityTimestamp_us) < INDEX_PNP_BUS_FRAME_TIMOEUT) {
    // timeout for new frame not yet elapsed, send 4 dummy bytes
    txStatus = txFrameStatusType::dummy_1;
    uart->setDirectionToRx();
  } else {
    // timeout already elapsed
    txStatus = txFrameStatusType::startFrame;
  }
  // trigger transmission
  txComplete();
  return 0;
}


void IndexPnpBusLinkLayer::txComplete(void) {
  uint8_t byte;

  switch (txStatus) {
    case txFrameStatusType::idle:
    // unexpected state TODO
    break;

    case txFrameStatusType::dummy_1:
      uart->writeByte(0x00);
      txStatus = txFrameStatusType::dummy_2;
    break;

    case txFrameStatusType::dummy_2:
    uart->writeByte(0x00);
    txStatus = txFrameStatusType::dummy_3;
    break;

    case txFrameStatusType::dummy_3:
    uart->writeByte(0x00);
    txStatus = txFrameStatusType::dummy_4;
    break;

    case txFrameStatusType::dummy_4:
    uart->writeByte(0x00);
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
      txFrameComplete();
      uart->setDirectionToRx();
    break;
    }
}


