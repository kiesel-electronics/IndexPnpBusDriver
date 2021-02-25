/*
 * HAL_serial_Interface.h
 *
 * Created: 21.02.2021 13:06:35
 *  Author: f.kiesel
 */ 

#include "compiler.h"

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
		virtual bool writeByte(uint8_t byte) = 0;
		
	};
	

#endif /* HAL_UART_H_ */