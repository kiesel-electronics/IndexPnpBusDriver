/*
  wiring_private.h - Internal header file.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#ifndef WiringPrivate_h
#define WiringPrivate_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdarg.h>
#include "clock_config.h"
#include "atomic.h"


#ifdef __cplusplus
extern "C"{
#endif


#define TIME_TRACKING_TIMER_PERIOD    0xFF
#define TIME_TRACKING_TICKS_PER_OVF   (TIME_TRACKING_TIMER_PERIOD + 1)  // Timer ticks per overflow of TCB3
#define TIME_TRACKING_TIMER_DIVIDER   64    // Clock divider for TCB0
#define TIME_TRACKING_CYCLES_PER_OVF  (TIME_TRACKING_TICKS_PER_OVF * TIME_TRACKING_TIMER_DIVIDER)

#define PWM_TIMER_PERIOD   0xFF  // For frequency
#define PWM_TIMER_COMPARE  0x80  // For duty cycle



// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

void init(void);


/* Allows performing a correction on the CPU value using the signature row 
	values indicating oscillator error provided from the device manufacturer */
#define PERFORM_SIGROW_CORRECTION_F_CPU 0

/* Variable containing corrected F_CPU value, after checks for safe operating
	frequency vs supply voltage, oscillator fuse setting and MCLK divider.
	Also includes the correction from signature row values if above #define
	PERFORM_SIGROW_CORRECTION_F_CPU = 1 */
extern uint32_t F_CPU_CORRECTED;

uint16_t clockCyclesPerMicrosecondComp(uint32_t clk);
uint16_t clockCyclesPerMicrosecond();
unsigned long clockCyclesToMicroseconds(unsigned long cycles);
unsigned long microsecondsToClockCycles(unsigned long microseconds);

#define NOT_ON_TIMER 0
#define TIMERA0 1
#define TIMERB0 2
#define TIMERB1 3
#define TIMERB2 4
#define TIMERB3 5

void setup_timers();

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
