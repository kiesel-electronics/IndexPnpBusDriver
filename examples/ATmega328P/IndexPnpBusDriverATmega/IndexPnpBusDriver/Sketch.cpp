/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

#include "project_config.h"
#include "feeder_types.h"

#include "HAL_uart_ATmega.h"
#include "IndexPnpBusClient.h"
#include "IndexPnPFeederClientAppl.h"

feederSettings_struct feederSettings;
uint8_t version_in[4] = {0x01, 0x00, 0x00, 0x00};
  
HAL_uart_Arduino IndexPnpUart_0( 2);
IndexPnpBusClient IndexPnpBusClient_0(feederSettings.uuid, version_in);
IndexPnPFeederClientAppl IndexPnpBusAppl;

uint8_t position;

unsigned long TaskTimer;
unsigned long Task100msCnt;
unsigned long Task1000msCnt;

uint8_t led_cnt;


void setup() {
  position = 100;
  led_cnt = 0;
  
  // init RS485
  // DIR: PD2, RX: PD0, TX: PD1
  IndexPnpBusAppl.Init();
  IndexPnpUart_0.Init(&IndexPnpBusClient_0, 19200);
  IndexPnpBusClient_0.InitLl(&IndexPnpUart_0, position);
  IndexPnpBusClient_0.Init(&IndexPnpBusAppl);
  

  // Init Task timer variables
  TaskTimer = millis();
  Task100msCnt = TaskTimer;
  Task1000msCnt = TaskTimer;
}

// the loop function runs over and over again forever
void loop() {
  // get time
  TaskTimer = millis();  
 
  // 100ms Task
  if (TaskTimer - Task100msCnt >= 100) {
    Task100msCnt += 100;
  }

   // 1000ms Task
  if (TaskTimer - Task1000msCnt >= 1000) {
    Task1000msCnt += 1000; 
  }
  
  IndexPnpBusClient_0.handler();
  IndexPnpBusAppl.Handler();
}


/* Interrupt service routine for RX complete */
ISR(USART_RX_vect) {
  IndexPnpUart_0.rxIrq();
}


/* Interrupt service routine for Tx complete */
ISR(USART_TX_vect) {
  IndexPnpUart_0.txIrq();
}
