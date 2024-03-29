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

#include "HAL_platform.h"

#ifndef HAL_UART_H_
#define HAL_UART_H_

/**
 * \interface HAL_serial_cbk_Interace
 * \brief This is the interface, that are needed by the serial HAL
 *
 * This interface defines all callbacks, that the serial Hardware Abstraction Layer uses. 
 *
 */
class HAL_uart_cbk_Interface {
  public:
  virtual void rxComplete(uint8_t byte);
  virtual void txComplete(void);
};

class HAL_uart_Interface {
  public:
    virtual void setDirectionToTx(void) = 0;
    virtual void setDirectionToRx(void) = 0;
    virtual bool writeByte(uint8_t data) = 0;
    virtual void enableTxInterrupt(void) = 0;
    virtual void disableTxInterrupt(void) = 0;
    virtual uint32_t getBaudrate(void) = 0;
    virtual uint32_t getTime_us(void) = 0;
  };


#endif /* HAL_UART_H_ */