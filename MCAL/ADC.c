/*
 * ADC.c
 *
 * Created: 26/08/2023 08:39:37 PM
 *  Author: Amir
 */ 

//******************************************************************
/***********************       Includes        *********************
 *******************************************************************/
#include "ADC.h"

//================================================================
//				Generic Variables
//================================================================
ADC_Config_t Global_ADC_CFG;

//================================================================
//				APIs Functions Definition
//================================================================
void ADC_Init(ADC_Config_t *ADC_CFG)
{
	Global_ADC_CFG = *ADC_CFG;
	u8 ADCSRA_Temp = 0 , ADMUX_Temp = 0;	
	
	//1.Determines The ADC Prescaler Clock 
	ADCSRA_Temp |= (Global_ADC_CFG.PRESCALER_CLK << ADPS0);
	
	//2.Determines the ADC Voltage Reference
	ADMUX_Temp |= (Global_ADC_CFG.Volt_Ref << REFS0);
	
	//3.Enable ADC
	SET_BIT(ADCSRA_Temp,ADEN);
	
	//4.Determines The The ADC Result will Read From ADCL Register or ADCH Register
	#ifdef  ADC_LOW_BYTE_REGISTER
	CLR_BIT(ADMUX_Temp,ADLAR);
	#elif  ADC_HIGH_BYTE_REGISTER
	SET_BIT(ADMUX_Temp,ADLAR);
	#endif
	
	ADMUX  = ADMUX_Temp;
	ADCSRA = ADCSRA_Temp;
}
u16 ADC_Read(ADC_CHANNEL_NUMBER_E Channel_No)
{
	//Determines The Channel Number
	ADMUX |= (Channel_No << MUX0);
	
	//Start Conversion
	SET_BIT(ADCSRA,ADSC);
	
	//Wait Until the Conversion Complete 
	while(GET_BIT(ADCSRA,ADSC));
	
	//return the ADC Result
	#ifdef  ADC_LOW_BYTE_REGISTER
	return ADC; //Case ADCL  
	#elif  ADC_HIGH_BYTE_REGISTER
	return (ADC >> 6); //Case ADCH  
	#endif
	
}