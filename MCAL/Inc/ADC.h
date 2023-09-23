/*
 * ADC.h
 *
 * Created: 26/08/2023 08:39:23 PM
 *  Author: Amir
 */ 


#ifndef ADC_H_
#define ADC_H_

//******************************************************************
/***********************       Includes        *******************
******************************************************************/

#include "STDTYPE.h"
#include "UTILS.h"


//******************************************************************
/*************************       MACROS        *******************
******************************************************************/
#define VoltRef		5000 //milliseconds
#define Resolution  1024 //because ADC Resolution Equal 10Bits  that mean (2^10 = 1024)

#define ADC_LOW_BYTE_REGISTER		1
//#define ADC_HIGH_BYTE_REGISTER		0

#ifdef ADC_LOW_BYTE_REGISTER
//ADCL: ADC Data Register Low Byte
#define ADC	*((volatile u8 *)0x24)
#elif  ADC_HIGH_BYTE_REGISTER
//ADCH: ADC Data Register High Byte
#define ADC	*((volatile u8 *)0x25) 
#endif

//ADMUX: REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0
#define ADMUX	*((volatile u8 *)0x27)
//ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
#define ADCSRA	*((volatile u8 *)0x26)


//*****************************************************************
/*************************       ENUMS        *********************
*******************************************************************/

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
// Enums for bit positions in registers
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

//ADMUX: ADC Multiplexer Selection Register 
typedef enum{
	MUX0 = 0, // Bit 0 – MUX0: Analog Channel and Gain Selection Bit
	MUX1,	  // Bit 1 – MUX1: Analog Channel and Gain Selection Bit
	MUX2,     // Bit 2 – MUX2: Analog Channel and Gain Selection Bit
	MUX3,     // Bit 3 – MUX3: Analog Channel and Gain Selection Bit
	MUX4,     // Bit 4 – MUX4: Analog Channel and Gain Selection Bit
	ADLAR,    // Bit 5 – ADLAR: ADC Left Adjust Result
	REFS0,	  // Bit 6 – REFS0: Reference Selection Bit
	REFS1 = 7 // Bit 7 – REFS1: Reference Selection Bit
}ADC_ADMUX_PINs_E;

//ADCSRA: ADC Control and Status Register A
typedef enum{
	ADPS0 = 0,//Bit  0 – ADPS0: ADC Prescaler Select Bits
	ADPS1,	  //Bit  1 – ADPS1: ADC Prescaler Select Bits
	ADPS2,    //Bit  2 – ADPS2: ADC Prescaler Select Bits
	ADIE,	  //Bit  3 – ADIE: ADC Interrupt Enable
	ADIF,     //Bit  4 – ADIF: ADC Interrupt Flag
	ADATE,    //Bit  5 – ADATE: ADC Auto Trigger Enable
	ADSC,     //Bit  6 – ADSC: ADC Start Conversion
	ADEN = 7  //Bit  7 – ADEN: ADC Enable
}ADC_ADCSRA_PINs_E;


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//ENUM Configuration References
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

//---------------------------------------
// @ref ADC_Channel_Number_Define 
//---------------------------------------
//ADMUX: Bits 4:0 – MUX4:0: Analog Channel and Gain Selection Bits
typedef enum{
	CH_0 = 0,
	CH_1,
	CH_2,
	CH_3,
	CH_4,
	CH_5,
	CH_6,
	CH_7 = 7
} ADC_CHANNEL_NUMBER_E;

//---------------------------------------
//@ref ADC_PRESCALER_SELECT_Define
//---------------------------------------
//ADCSRA: Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits
typedef enum {
	ADC_PRESCALER_2 = 0,
	ADC_PRESCALER_4 = 2,
	ADC_PRESCALER_8,
	ADC_PRESCALER_16,
	ADC_PRESCALER_32,
	ADC_PRESCALER_64,
	ADC_PRESCALER_128 = 7
} ADC_PRESCALER_SELECT_E;

//---------------------------------------
//@ref ADC_VOLT_REF_Define
//---------------------------------------
//ADMUX: Bit 7:6 – REFS1:0: Reference Selection Bits
typedef enum {
	VREF_AREF = 0,//(5V) Voltage Reference ,(Internal) Vref 
	VREF_AVCC,	  //(5V) Voltage Reference (with external) capacitor at AREF pin
	VREF_256 = 3  //Internal (2.56V) Voltage Reference (with external) capacitor at AREF pin
} ADC_VOLT_REF_E;


//******************************************************************
/*************************       STRUCTS        ******************
******************************************************************/
// Structure to configure ADC
typedef struct {
	ADC_PRESCALER_SELECT_E PRESCALER_CLK;// ADC Prescaler clock, @ref ADC_PRESCALER_SELECT_Define
	ADC_VOLT_REF_E Volt_Ref;			 // Voltage Reference Selections for ADC , @ref ADC_VOLT_REF_Define
} ADC_Config_t;


//****************************************************************
/*******************       APIs Functions        *****************
******************************************************************/

void ADC_Init(ADC_Config_t *ADC_CFG);
u16 ADC_Read(ADC_CHANNEL_NUMBER_E Channel_No);



#endif /* ADC_H_ */