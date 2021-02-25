/*
 * HAL_uart_Atmega0.h
 *
 * Created: 21.02.2021 13:31:01
 *  Author: Friedrich
 */ 


#ifndef HAL_UART_ATMEGA0_H_
#define HAL_UART_ATMEGA0_H_

#include "IndexPnpBus_config.h"
#include "HAL_uart_Interface.h"

class HAL_uart_Atmega0 : public HAL_uart_Interface {
  public:

    void Init(USART_t* _uartCh, HAL_uart_cbk_Interface* _nextLayer);

    void setDirectionToTx(void);    
    void setDirectionToRx(void);


    bool writeByte(uint8_t byte);
    /**
     * Transmit Complete Interrupt function
     */
    void rxIrq(void);
    /**
     * Data Register Empty interrupt function
     * 
     */
    void dreIrq(void);
  protected:
  private:
    USART_t* uartCh;
    HAL_uart_cbk_Interface* nextLayer;
};



#endif /* HAL_UART_ATMEGA0_H_ */