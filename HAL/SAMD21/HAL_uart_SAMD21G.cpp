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

#include "HAL_uart_SAMD21G.h"
#include "Arduino.h"
#include "wiring_private.h"


  HAL_uart_SAMD21G::HAL_uart_SAMD21G(Sercom *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinDir) {
    sercom = _s;
    uc_pinRX = _pinRX;
    uc_pinTX = _pinTX;
    uc_padRX = _padRX ;
    uc_padTX = _padTX;
    uc_pinDir = _pinDir;
  }


void HAL_uart_SAMD21G::Init(HAL_uart_cbk_Interface* _nextLayer, uint32_t gclkFrequency, uint32_t _baudrate) {
  uint16_t config = SERIAL_8N1;
  this->nextLayer = _nextLayer;
  this->baudrate = _baudrate;

  // initialize hardware
  pinMode(uc_pinDir, OUTPUT);
  digitalWrite(uc_pinDir, LOW);
  pinPeripheral(uc_pinRX, PIO_SERCOM);
  pinPeripheral(uc_pinTX, PIO_SERCOM);

  initUart(UART_INT_CLOCK, gclkFrequency, SAMPLE_RATE_x16, baudrate);
  initFrame(extractCharSize(config), LSB_FIRST, extractParity(config), extractNbStopBit(config));
  initPads(uc_padTX, uc_padRX);

  enableUART();
}


void HAL_uart_SAMD21G::setDirectionToTx(void) {
  digitalWrite(uc_pinDir, HIGH);
}


void HAL_uart_SAMD21G::setDirectionToRx(void) {
  digitalWrite(uc_pinDir, LOW);
}


bool HAL_uart_SAMD21G::writeByte(uint8_t data) {
  // send byte
  if (getUartDataRegEmptyFlg()) {
    //Put data into DATA register
    sercom->USART.DATA.reg = (uint16_t)data;
    return true;
  } else {
    return false;
  }
}


void HAL_uart_SAMD21G::enableTxInterrupt(void) {
  sercom->USART.INTENSET.reg = SERCOM_USART_INTENSET_TXC;
}


void HAL_uart_SAMD21G::disableTxInterrupt(void) {
  sercom->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_TXC;
}


uint32_t HAL_uart_SAMD21G::getBaudrate(void) {
  return baudrate;
}


uint32_t HAL_uart_SAMD21G::getTime_us(void) {
  return micros();
}

void HAL_uart_SAMD21G::IrqHandler() {
  uint8_t rxByte;
  if (getUartFramErrorFlg()) {
    // frame error, next byte is invalid so read and discard it
    getUartData();
    clearUartFrameError();
  }

  if (getUartDataAvailableFlg()) {
    rxByte = getUartData();
    nextLayer->rxComplete(rxByte);
  }

  if (getUartDataRegEmptyFlg()) {
    nextLayer->txComplete();
  }

  if (getUartErrorFlg()) {
    clearUartError();
    // TODO: if (sercom->isBufferOverflowErrorUART()) ....
    // TODO: if (sercom->isParityErrorUART()) ....

    // clear Uart Status
    sercom->USART.STATUS.reg = SERCOM_USART_STATUS_RESETVALUE;
  }
}


void HAL_uart_SAMD21G::initUart(SercomUartMode mode, uint32_t gclkFrequency, SercomUartSampleRate sampleRate, uint32_t baudrate) {
  initClockNVIC();

   // Set uart reset
  sercom->USART.CTRLA.bit.SWRST = 1 ;
  while ( sercom->USART.CTRLA.bit.SWRST || sercom->USART.SYNCBUSY.bit.SWRST ) {
    // Wait until software reset and SYNCBUSY are 0
  }

  //Set CTRLA register
  sercom->USART.CTRLA.reg = SERCOM_USART_CTRLA_MODE(mode) |
                            SERCOM_USART_CTRLA_SAMPR(sampleRate);

  //Set Interrupt register
  sercom->USART.INTENSET.reg =  SERCOM_USART_INTENSET_RXC |  //Rx complete ISR
                                SERCOM_USART_INTENSET_ERROR; //All errors ISR

  if ( mode == UART_INT_CLOCK ) {
    uint16_t sampleRateVal;
    if (sampleRate == SAMPLE_RATE_x16) {
      sampleRateVal = 16;
    } else {
      sampleRateVal = 8;
    }

    // Async fractional mode (Table 24-2 in datasheet)
    // BAUD = fref / (sampleRateVal * fbaud)
    // (multiply by 8, to calculate fractional piece)
    uint32_t baudTimes8 = (gclkFrequency * 8) / (sampleRateVal * baudrate);

    sercom->USART.BAUD.FRAC.FP   = (baudTimes8 % 8);
    sercom->USART.BAUD.FRAC.BAUD = (baudTimes8 / 8);
  }
}


void HAL_uart_SAMD21G::initFrame(SercomUartCharSize charSize, SercomDataOrder dataOrder, SercomParityMode parityMode, SercomNumberStopBit nbStopBits) {
  //Set the CTRLA register
  sercom->USART.CTRLA.reg |= SERCOM_USART_CTRLA_FORM( (parityMode == SERCOM_NO_PARITY ? 0 : 1) )
                | dataOrder << SERCOM_USART_CTRLA_DORD_Pos;

  //Set the CTRLB register
  sercom->USART.CTRLB.reg |= SERCOM_USART_CTRLB_CHSIZE(charSize)
                | nbStopBits << SERCOM_USART_CTRLB_SBMODE_Pos
                | (parityMode == SERCOM_NO_PARITY ? 0 : parityMode) << SERCOM_USART_CTRLB_PMODE_Pos; //use default value if there is no parity 
}


void HAL_uart_SAMD21G::initPads(SercomUartTXPad txPad, SercomRXPad rxPad) {
  //Set the CTRLA register
  sercom->USART.CTRLA.reg |= SERCOM_USART_CTRLA_TXPO(txPad)
                | SERCOM_USART_CTRLA_RXPO(rxPad);

  // Enable Tx and Rx
  sercom->USART.CTRLB.reg |= (SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN);
}


inline void HAL_uart_SAMD21G::enableUART() {
  //set the enable bit to 1
  sercom->USART.CTRLA.bit.ENABLE = 0x1u;
  //Busy wait until enable bit is 0
  while(sercom->USART.SYNCBUSY.bit.ENABLE);
}


inline uint8_t HAL_uart_SAMD21G::getUartData() {
  return sercom->USART.DATA.bit.DATA;
}


inline bool HAL_uart_SAMD21G::getUartDataAvailableFlg() {
  // return receive complete flag
  return sercom->USART.INTFLAG.bit.RXC;
}


inline bool HAL_uart_SAMD21G::getUartDataRegEmptyFlg() {
  return sercom->USART.INTFLAG.bit.DRE;
}


inline bool HAL_uart_SAMD21G::getUartFramErrorFlg() {
  return sercom->USART.STATUS.bit.FERR;
}

inline void HAL_uart_SAMD21G::clearUartFrameError() {
  // clear FERR bit by writing 1 to the bit
  sercom->USART.STATUS.bit.FERR = 1;
}


inline bool HAL_uart_SAMD21G::getUartErrorFlg() {
  // return error flag
  return sercom->USART.INTFLAG.bit.ERROR;
}


inline void HAL_uart_SAMD21G::clearUartError() {
  // clear error
  sercom->USART.INTFLAG.bit.ERROR = 1;
}


inline void HAL_uart_SAMD21G::initClockNVIC( void ) {
  uint8_t clkId = 0;
  IRQn_Type nvicId=PendSV_IRQn ; // Set dummy value, to detect later if an new value was set

  if(sercom == SERCOM0) {
    clkId = GCM_SERCOM0_CORE;
    nvicId = SERCOM0_IRQn;
  }
  else if(sercom == SERCOM1) {
    clkId = GCM_SERCOM1_CORE;
    nvicId = SERCOM1_IRQn;
  }
  else if(sercom == SERCOM2) {
    clkId = GCM_SERCOM2_CORE;
    nvicId = SERCOM2_IRQn;
  }
  else if(sercom == SERCOM3) {
    clkId = GCM_SERCOM3_CORE;
    nvicId = SERCOM3_IRQn;
  }
  #if defined(SERCOM4)
  else if(sercom == SERCOM4) {
    clkId = GCM_SERCOM4_CORE;
    nvicId = SERCOM4_IRQn;
  }
  #endif // SERCOM4
  #if defined(SERCOM5)
  else if(sercom == SERCOM5) {
    clkId = GCM_SERCOM5_CORE;
    nvicId = SERCOM5_IRQn;
  }
  #endif // SERCOM5

  if ( nvicId == PendSV_IRQn ) {
    // Something went wrong, no nvicId was set!
    return ;
  }

  // Setting NVIC
  NVIC_EnableIRQ(nvicId);
  NVIC_SetPriority (nvicId, SERCOM_NVIC_PRIORITY);  /* set Priority */

  //Setting clock
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( clkId ) | // Generic Clock 0 (SERCOMx)
                      GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
                      GCLK_CLKCTRL_CLKEN ;

  while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ) {/* Wait for sync */}
}


SercomNumberStopBit HAL_uart_SAMD21G::extractNbStopBit(uint16_t config) {
  switch(config & SERIAL_STOP_BIT_MASK) {
    case SERIAL_STOP_BIT_1:
    default:
      return SERCOM_STOP_BIT_1;
      break;
    case SERIAL_STOP_BIT_2:
      return SERCOM_STOP_BITS_2;
      break;
  }
}


SercomUartCharSize HAL_uart_SAMD21G::extractCharSize(uint16_t config) {
  switch(config & SERIAL_DATA_MASK) {
    case SERIAL_DATA_5:
      return UART_CHAR_SIZE_5_BITS;
      break;
    case SERIAL_DATA_6:
      return UART_CHAR_SIZE_6_BITS;
      break;
    case SERIAL_DATA_7:
      return UART_CHAR_SIZE_7_BITS;
      break;
    case SERIAL_DATA_8:
    default:
      return UART_CHAR_SIZE_8_BITS;
      break;
  }
}


SercomParityMode HAL_uart_SAMD21G::extractParity(uint16_t config) {
  switch(config & SERIAL_PARITY_MASK) {
    case SERIAL_PARITY_NONE:
    default:
      return SERCOM_NO_PARITY;
      break;
    case SERIAL_PARITY_EVEN:
      return SERCOM_EVEN_PARITY;
      break;
    case SERIAL_PARITY_ODD:
      return SERCOM_ODD_PARITY;
      break;
  }
}