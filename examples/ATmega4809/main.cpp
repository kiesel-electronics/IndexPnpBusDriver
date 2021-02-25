/*
 * IndexPnpBusDriver.cpp
 *
 * Created: 21.02.2021 19:33:41
 * Author : f.kiesel
 */ 
 
#include <atmel_start.h>
#include "HAL_uart_Atmega0.h"
#include "IndexPnpBusClient.h"
#include <IndexPnPFeederAppl.h>

extern "C" void __cxa_pure_virtual() { while (1); }


HAL_uart_Atmega0 IndexPnpUart_0;
IndexPnpBusClient IndexPnpBusClient_0;
IndexPnPFeederAppl Application;


int main(void)
{
  	/* Initializes MCU, drivers and middleware */
  	atmel_start_init();

    IndexPnpUart_0.Init(&USART0, &IndexPnpBusClient_0);
    IndexPnpBusClient_0.InitLl(&IndexPnpUart_0, 0x10);
    IndexPnpBusClient_0.Init(&Application);
    

    while (1) 
    {
    }
}


/* Interrupt service routine for RX complete */
ISR(USART0_RXC_vect)
{
  IndexPnpUart_0.rxIrq();
}

/* Interrupt service routine for Data Register Empty */
ISR(USART0_DRE_vect)
{
  IndexPnpUart_0.dreIrq();
}