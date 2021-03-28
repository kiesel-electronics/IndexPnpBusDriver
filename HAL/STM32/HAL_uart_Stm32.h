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

#ifndef HAL_UART_STM32_H_
#define HAL_UART_STM32_H_

#include "stm32_def.h"
#include <Arduino.h>
#include "PinNames.h"
#include "HAL_uart_Interface.h"


class HAL_uart_Stm32 : public HAL_uart_Interface {
  public:
    HAL_uart_Stm32(USART_TypeDef* _usart, uint32_t _dePin, uint32_t _rePin, int (*_tx_callback)(serial_t *), void (*_rx_callback)(serial_t *));
    void Init(HAL_uart_cbk_Interface* _nextLayer, uint32_t _baudrate);
    void setDirectionToTx(void);
    void setDirectionToRx(void);
    bool writeByte(uint8_t data);
    void enableTxInterrupt(void);
    void disableTxInterrupt(void);
    uint32_t getBaudrate(void);
    uint32_t getTime_us(void);

    // Interrupt handlers
    void _rx_complete_irq(serial_t *obj);
    int _tx_complete_irq(serial_t *obj);

  protected:
  private:
    HAL_uart_cbk_Interface* nextLayer;
  
    void (*rx_callback)(serial_t *);
    int (*tx_callback)(serial_t *);

    serial_t _serial;
    unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
    unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];

    uint32_t baudrate;
    PinName dePin;
    PinName rePin;
    PinName txPin;
    PinName rxPin;

    IRQn_Type irq;
};



#endif /* HAL_UART_STM32_H_ */