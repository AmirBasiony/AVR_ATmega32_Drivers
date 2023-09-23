/*
 * WDT.h
 *
 * Created: 19/08/2023 08:43:41 م
 *  Author: Amir
 */ 


#ifndef WDT_H_
#define WDT_H_

#include "STDTYPE.h"
#include "UTILS.h"

#define WDTCR	*((volatile u8 *)0x41)

typedef enum
{
	//Bits 2..0 – WDP2, WDP1, WDP0: Watchdog Timer Prescaler 2, 1, and 0
	WDP0 = 0,
	WDP1,
	WDP2,
	//Bit 3 – WDE: Watchdog Enable
	WDE,
	//Bit 4 – WDTOE: Watchdog Turn-off Enable
	WDTOE = 4 //
}WDT_PinREG_t;

// @ref	WDT_PRESCALER_TimeOut_Define		
#define WDT_PRESCALER_TimeOut_16ms			(u8)((0b000<<WDP0)& 0b111)	     // Time Out = 16.3 ms
#define WDT_PRESCALER_TimeOut_32ms		 	(u8)((0b001<<WDP0)& 0b111)		 // Time Out = 32.5 ms
#define WDT_PRESCALER_TimeOut_65ms			(u8)((0b010<<WDP0)& 0b111)		 // Time Out = 65 ms
#define WDT_PRESCALER_TimeOut_130ms			(u8)((0b011<<WDP0)& 0b111)		 // Time Out = 0.13 s = 130 ms
#define WDT_PRESCALER_TimeOut_260ms			(u8)((0b100<<WDP0)& 0b111)		 // Time Out = 0.26 s = 260 ms
#define WDT_PRESCALER_TimeOut_520ms			(u8)((0b101<<WDP0)& 0b111)		 // Time Out =  0.52 s = 520 ms
#define WDT_PRESCALER_TimeOut_1s			(u8)((0b110<<WDP0)& 0b111)		 // Time Out =  1.0 s 
#define WDT_PRESCALER_TimeOut_2s			(u8)((0b111<<WDP0)& 0b111)		 // Time Out =  2.1 s 

//---------------------------------------------------
// --------------------APIs of WDT-------------------
//---------------------------------------------------
void WDT_ON(u8 TimeOut_Prescaler);
void WDT_OFF(void);

#endif /* WDT_H_ */