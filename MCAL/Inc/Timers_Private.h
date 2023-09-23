/*
 * Timers_Private.h
 *
 * Created: 31/08/2023 09:02:34 PM
 *  Author: Amir
 */ 

#ifndef TIMERS_PRIVATE_H_
#define TIMERS_PRIVATE_H_

//******************************************************************
//*********************       MACROS         ***********************
//******************************************************************

//----------------
//TIMER0 Registers
//----------------
//TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
#define TCCR0	*((volatile u8 *)0x53)
//TCNT0: Timer/Counter0 (8 Bits)
#define TCNT0	*((volatile u8 *)0x52)
//OCR0: Timer/Counter0 Output Compare Register
#define OCR0	*((volatile u8 *)0x5C)

//----------------
//TIMER1 Registers
//----------------
//TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10
#define TCCR1A	*((volatile u8 *)0x4F)
//TCCR1B: ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
#define TCCR1B	*((volatile u8 *)0x4E)

#define TCNT1	*((volatile u16 *)0x4C)
//TCNT1L: Timer/Counter1 – Counter Register Low Byte
#define TCNT1L	*((volatile u8 *)0x4C)
//TCNT1H: Timer/Counter1 – Counter Register High Byte
#define TCNT1H	*((volatile u8 *)0x4D)


#define OCR1A	*((volatile u16 *)0x4A)
//OCR1AL: Timer/Counter1 – Output Compare Register A Low Byte
#define OCR1AL	*((volatile u8 *)0x4A)
//OCR1AH: Timer/Counter1 – Output Compare Register A High Byte
#define OCR1AH	*((volatile u8 *)0x4B)


#define OCR1B	*((volatile u16 *)0x48)
//OCR1BL: Timer/Counter1 – Output Compare Register B Low Byte
#define OCR1BL	*((volatile u8 *)0x48)
//OCR1BH: Timer/Counter1 – Output Compare Register B High Byte
#define OCR1BH	*((volatile u8 *)0x49)


#define ICR1	*((volatile u16 *)0x46)
//ICR1L: Timer/Counter1 – Input Capture Register Low Byte
#define ICR1L	*((volatile u8 *)0x46)
//ICR1H: Timer/Counter1 – Input Capture Register High Byte
#define ICR1H	*((volatile u8 *)0x47)


//-----------------------
//COMMON TIMERS Registers
//-----------------------
//TIMSK: OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 OCIE0 TOIE0
#define TIMSK	*((volatile u8 *)0x59)
//TIFR: OCF2 TOV2 ICF1 OCF1A OCF1B TOV1 OCF0 TOV0
#define TIFR	*((volatile u8 *)0x58)
//----------------------------

//*****************************************************************
/*************************       ENUMS        *********************
*******************************************************************/

//---------------------------------
//BIT Positions in TIMER0 Registers
//---------------------------------
// TCCR0 (Timer/Counter Control Register)
typedef enum{
	CS00 = 0,
	CS01,
	CS02,
	WGM01,   //Bit 3 – WGM01:0: Waveform Generation Mode
	COM00,   //Bit 4 – COM00: Compare Match Output Mode
	COM01,   //Bit 5 – COM01: Compare Match Output Mode
	WGM00,   //Bit 6 – WGM01:0: Waveform Generation Mode
	FOC0 = 7 // Bit 7 – FOC0: Force Output Compare
}TIMER0_TCCR0_PINs_E;


//---------------------------------
//BIT Positions in TIMER1 Registers
//---------------------------------
//TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10
typedef enum{
	WGM10 = 0,	// Bit 0 – WGM10: Waveform Generation Mode
	WGM11 ,		// Bit 1 – WGM11: Waveform Generation Mode
	FOC1B,		// Bit 2 – FOC1B: Force Output Compare for Compare unit B
	FOC1A,		// Bit 3 – FOC1A: Force Output Compare for Compare unit A
	COM1B0,		// Bit 4 – COM1B0: Compare Output Mode for Compare unit B
	COM1B1,		// Bit 5 – COM1B1: Compare Output Mode for Compare unit B
	COM1A0,		// Bit 6 – COM1A0: Compare Output Mode for Compare unit A
	COM1A1 = 7	// Bit 7 – COM1A1: Compare Output Mode for Compare unit A
}TIMER1_TCCR1A_PINs_E;

//TCCR1B: ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
typedef enum{
	CS10 = 0,	// Bit 0 – CS10: Clock Select
	CS11 ,		// Bit 1 – CS11: Clock Select
	CS12,		// Bit 2 – CS12: Clock Select
	WGM12,		// Bit 3 – WGM12: Waveform Generation Mode
	WGM13,		// Bit 4 – WGM13: Waveform Generation Mode
	ICES1 = 6,	// Bit 6 – ICES1: Input Capture Edge Select
	ICNC1 = 7	// Bit 7 – ICNC1: Input Capture Noise Canceler
}TIMER1_TCCR1B_PINs_E;

//-------------------------------------------------------
// COMMON BIT Positions in TIMER0/TIMER1/TIMER2 Registers
//-------------------------------------------------------
//TIMSK: (Timer/Counter Interrupt Mask Register)
typedef enum{
	TOIE0 = 0,// Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable
	OCIE0,	  // Bit 1 – OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable
	TOIE1,	// Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable
	OCIE1B,	// Bit 3 – OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
	OCIE1A, // Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	TICIE1, // Bit 5 – TICIE1: Timer/Counter1, Input Capture Interrupt Enable
	TOIE2,
	OCIE2 = 7
}TIMER0_1_2_TIMSK_PINs_E;

//TIFR:  Timer/Counter Interrupt Flag Register
typedef enum{
	TOV0 = 0,//• Bit 0 – TOV0: Timer/Counter0 Overflow Flag
	OCF0 ,  //• Bit 1 – OCF0: Output Compare Flag 0
	TOV1,
	OCF1B,
	OCF1A,
	ICF1,
	TOV2,
	OCF2 = 7
}TIMER0_1_2_TIFR_PINs_E;


#endif /* TIMERS_PRIVATE_H_ */