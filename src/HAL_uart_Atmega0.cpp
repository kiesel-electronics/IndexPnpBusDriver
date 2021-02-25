/*
 * HAL_uart_Atmega0.cpp
 *
 * Created: 21.02.2021 13:32:47
 *  Author: Friedrich
 */ 

#include "HAL_uart_Atmega0.h"


void HAL_uart_Atmega0::Init(USART_t* _uartCh, HAL_uart_cbk_Interface* _nextLayer) {
  this->uartCh = _uartCh;
  this->nextLayer = _nextLayer;

  // initialize hardware
   
  uartCh->BAUD = (uint16_t)INDEX_PNP_USART_BAUD_RATE(INDEX_PNP_BUS_BAUDRATE_BPS); /* set baud rate register */

  uartCh->CTRLA = 0 << USART_ABEIE_bp  /* Auto-baud Error Interrupt Enable: disabled */
                | 0 << USART_DREIE_bp  /* Data Register Empty Interrupt Enable: disabled */
                | 0 << USART_LBME_bp   /* Loop-back Mode Enable: disabled */
                | USART_RS485_EXT_gc   /* RS485 Mode External drive */
                | 1 << USART_RXCIE_bp  /* Receive Complete Interrupt Enable: enabled */
                | 0 << USART_RXSIE_bp  /* Receiver Start Frame Interrupt Enable: disabled */
                | 1 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: enabled */

  uartCh->CTRLB = 0 << USART_MPCM_bp     /* Multi-processor Communication Mode: disabled */
                | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
                | 1 << USART_RXEN_bp     /* Receiver Enable: enabled */
                | USART_RXMODE_NORMAL_gc /* Normal mode */
                | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
                | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

}


void HAL_uart_Atmega0::setDirectionToTx(void) {
  // nothing to do here, because the usart is in RS485 mode and does this automatically
}


void HAL_uart_Atmega0::setDirectionToRx(void) {
  // nothing to do here, because the usart is in RS485 mode and does this automatically
}

bool HAL_uart_Atmega0::writeByte(uint8_t byte) {
  // send byte
	uartCh->TXDATAL = byte;
  return true;
}


void HAL_uart_Atmega0::rxIrq(void) {
  uint8_t byte;
  byte = uartCh->RXDATAL;
  nextLayer->rxComplete(byte);
}


void HAL_uart_Atmega0::dreIrq(void) {
  nextLayer->txComplete();
}