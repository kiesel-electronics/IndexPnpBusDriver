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


#ifndef HAL_UART_SAMD21G_H_
#define HAL_UART_SAMD21G_H_

#include "HAL_uart_Interface.h"

class HAL_uart_SAMD21G : public HAL_uart_Interface {
  public:
    HAL_uart_SAMD21G(Sercom *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinDir);
    void Init(HAL_uart_cbk_Interface* _nextLayer, uint32_t gclkFrequency, uint32_t _baudrate);
    void setDirectionToTx(void);
    void setDirectionToRx(void);
    bool writeByte(uint8_t data);
    void enableTxInterrupt(void);
    void disableTxInterrupt(void);
    uint32_t getBaudrate(void);
    uint32_t getTime_us(void);

    void IrqHandler(void);
  protected:
  private:
    HAL_uart_cbk_Interface* nextLayer;
    Sercom *sercom;
    uint32_t baudrate;
    uint8_t uc_pinRX;
    uint8_t uc_pinTX;
    SercomRXPad uc_padRX;
    SercomUartTXPad uc_padTX;
    uint8_t uc_pinDir;

    SercomNumberStopBit extractNbStopBit(uint16_t config);
    SercomUartCharSize extractCharSize(uint16_t config);
    SercomParityMode extractParity(uint16_t config);

    /* ========== UART ========== */
    void initUart(SercomUartMode mode, uint32_t gclkFrequency, SercomUartSampleRate sampleRate, uint32_t baudrate=0) ;
    void initFrame(SercomUartCharSize charSize, SercomDataOrder dataOrder, SercomParityMode parityMode, SercomNumberStopBit nbStopBits) ;
    void initPads(SercomUartTXPad txPad, SercomRXPad rxPad) ;

    void enableUART(void);
    uint8_t getUartData(void);
    bool getUartDataAvailableFlg(void);
    bool getUartDataRegEmptyFlg(void);
    bool getUartFramErrorFlg(void);
    void clearUartFrameError(void);
    bool getUartErrorFlg();
    void clearUartError();

    void initClockNVIC(void);
};



#endif /* HAL_UART_SAMD21G_H_ */