/*
 * WDT.c
 *
 * Created: 19/08/2023 08:43:57 م
 *  Author: Amir
 */ 
#include "WDT.h"
#include "DIO_interface.h"

void WDT_OFF(void)
{
	//write a logic one to WDTOE and WDE To Clear those Pins
	SET_BIT(WDTCR,WDTOE);
	SET_BIT(WDTCR,WDE);
	
	// Clear WDTCR Register (WDE) after 4 clock cycle
	WDTCR = 0x00; 

	// Now Watchdog Timer is disabled
}

void WDT_ON(u8 TimeOut_Prescaler)
{
	// Set WDT bit to be Enabled
	SET_BIT(WDTCR,WDE);
	
	// Set The Timeout value(Period) of watchdog timer
	WDTCR |= TimeOut_Prescaler;
}
