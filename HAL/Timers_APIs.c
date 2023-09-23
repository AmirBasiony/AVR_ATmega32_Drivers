/*
 * Timers_APP.c
 *
 * Created: 06/09/2023 11:39:52 م
 *  Author: Amir
 */ 

#include "Timers_APIs.h"

//===============================================
//========== TIMER0 Generic Functions ===========
//===============================================
u8 CntL = 0 , CntH = 0 ,flag = 0;
// Callback function to handle the pulse generation
void TIMER0_CM_CALLBack_Handler(void)
{
	TOGGLE_BIT(PORTD,0);
}
void TIMER0_OVFI_With_CMI_CALLBack_Handler()
{
	TOGGLE_BIT(PORTD,0);
}
void TIMER0_Generate_PWM_Using_OVFI_OMI(u8 DutyCycle)
{
	TIMER0_Config_t timer0Config = {TIMER0_NORMAL_MODE ,PRESCALING_CLK_64 ,OCMIE0_Enable ,TOVIE0_Enable,PWM0_STATE_Disable};
	TIMER0_SetCallBack_CompareMatch_Interrupt(TIMER0_CM_CALLBack_Handler);
	TIMER0_SetCallBack_Overflow_Interrupt(TIMER0_OVFI_With_CMI_CALLBack_Handler);

	TIMER0_Init(&timer0Config);
	// Set the compare value for a 50% duty cycle
	PWM0_SetDutyCycleValue(DutyCycle);
	SET_BIT(DDRD,0);
	//Non-Inverting
	SET_BIT(PORTD,0);
	//Inverting
	//CLR_BIT(PORTD,0);
	GIE_enable();
}
//-----------------------------------------------------------------
void TIMER0_OVF_CALLBack_Handler(void)
{
	if(flag == 0)
	{
		//TCNT0 = CntH;
		//TIMER0_SetCounterValue(CntH);
		flag = 1;
	}
	else if(flag == 1)
	{
		//TCNT0 = CntL;
		TIMER0_SetCounterValue(CntL);
		flag = 0;
	}
	TOGGLE_BIT(PORTD,1);
}
void TIMER0_Generate_PWM_Using_OVFI(u8 DutyCycle)
{
	TIMER0_Config_t timer0Config = {TIMER0_NORMAL_MODE ,PRESCALING_CLK_8 ,OCMIE0_Disable ,TOVIE0_Enable,PWM0_STATE_Disable};
	
	TIMER0_SetCallBack_Overflow_Interrupt(TIMER0_OVF_CALLBack_Handler);
	
	CntH = (DutyCycle / 100.0)*256;// (75/100.0)*256 = 192 Ticks (TH)
	CntH = 255 - CntH;  // 63 Ticks (TH)
	
	CntL = ((100-DutyCycle) / 100.0)*255;// (25/100.0)*255 = 63 Ticks (TL)
	CntL = 255 - CntL;  // 192 Ticks (TL)
	
	SET_BIT(DDRD,1);
	//NON_Inverting
	CLR_BIT(PORTD,1);
	//Inverting
	//	SET_BIT(PORTD,1);
	
	TIMER0_Init(&timer0Config);
	TCNT0 = CntL;
	
	GIE_enable();
}
//-----------------------------------------------------------------

void PWM0_Init_Handle()
{
	//TIMER0_Config_t timer0Config = {TIMER0_NORMAL_MODE ,PRESCALING_CLK_8 ,OCMIE0_Disable ,TOVIE0_Disable,PWM0_STATE_Disable};
	
	TIMER0_Config_t timer0Config = {TIMER0_FAST_PWM_MODE ,PRESCALING_CLK_8 ,OCMIE0_Enable ,TOVIE0_Disable,PWM0_STATE_NON_INVERTING};
	//TIMER0_Config_t timer0Config = {TIMER0_FAST_PWM_MODE ,PRESCALING_CLK_8 ,OCMIE0_Enable ,TOVIE0_Disable,PWM0_STATE_INVERTING};
	//TIMER0_Config_t timer0Config = {TIMER0_Phase_Correct_PWM ,PRESCALING_CLK_8 ,OCMIE0_Enable ,TOVIE0_Disable,PWM0_STATE_NON_INVERTING};
	//TIMER0_Config_t timer0Config = {TIMER0_Phase_Correct_PWM ,PRESCALING_CLK_8 ,OCMIE0_Enable ,TOVIE0_Disable,PWM0_STATE_INVERTING};
	// Initialize TIMER0
	
	//TIMER0_SetCallBack_CompareMatch_Interrupt(TIMER0_CM_CHA_Handler);
	//	TIMER0_SetCallBack_Overflow_Interrupt(TIMER0_OVF_CHA_Handler);
	// Set the compare value for a 50% duty cycle
	PWM0_SetDutyCycleValue(50);
	SET_BIT(DDRB,PULSE_OC0_PIN);
	TIMER0_Init(&timer0Config);
	// Set pulse output pin as output
	//DDRB |= (1<<PULSE_OC0_PIN) | (1<<TMR0_OC0_Interrupt_LED);
	
	
	
	// Set the compare value for a 75% duty cycle
	//	 PWM0_SetDutyCycleValue(50);

	// Set the compare match interrupt callback function
	/*	 TIMER0_SetCallBack_CompareMatch_Interrupt(PWM0_PulseCallback);	 */
}
//-----------------------------------------------------------------
void CustomPWM0_Without_Interrupts(u8 counter,u8 pin)
{
	TCNT0=0;
	while(TCNT0 < 0xFF)
	{
		if (TCNT0<counter)
		{
			SET_BIT(PORTB,pin);
		}
		else
		CLR_BIT(PORTB,pin);
	}
	TOGGLE_BIT(PORTB,pin);
}
//-----------------------------------------------------------------
// Make a delay with milliseconds by using (TIMER0)
void TIMER0_Delay_ms_Init()
{
	TIMER0_Config_t TIM0_Config = {TIMER0_NORMAL_MODE,PRESCALING_CLK_64 ,OCMIE0_Disable ,TOVIE0_Enable,PWM0_STATE_Disable};
	TIMER0_Init(&TIM0_Config);
	GIE_enable();
}
void TIMER0_Delay_us_Init()
{
	TIMER0_Config_t TIM0_Config = {TIMER0_NORMAL_MODE,NO_PRESCALING,OCMIE0_Disable ,TOVIE0_Enable,PWM0_STATE_Disable};
	TIMER0_Init(&TIM0_Config);
	GIE_enable();
}
void delay_ms(u16 milliseconds)
{
	 // Calculate the total number of timer ticks required for the delay
	 double total_ticks =(milliseconds * (double)F_CPU) / (1000.0 * 64); // Assuming prescaler = 64
	 
	 // Calculate the number of timer overflows required for the delay
	 u16 required_overflows = (u16)(total_ticks / 256.0);
	 
	 // Calculate the remaining ticks within the current overflow
//	 u8 remaining_ticks = (u8)(total_ticks - (required_overflows * 256.0));
	 
	 u8 remaining_ticks = (u8)((u32)total_ticks % 256UL);
	 
	 // Reset the overflow count for the current delay
	 TIMER0_u8_OVF_Number = 0;
	 
	 TIMER0_Delay_ms_Init();

	 // Wait for the required number of timer overflows
	 while (TIMER0_u8_OVF_Number < required_overflows);
	 
// 	 u8 *pCnt_Val = NULL;
// 	 TIMER0_GetCounterValue(pCnt_Val);
	 // Wait for the remaining ticks
	 while (TCNT0 < remaining_ticks);
	 
	 // Reset the overflow count for the next delay
	 TIMER0_u8_OVF_Number = 0;
	 TIMER0_Stop();
}

void delay_us(u32 microseconds) 
{
	// Calculate the number of timer ticks required for the delay
	double ticks_required = (microseconds * (double)F_CPU) / (1000000.0 * 1); // Assuming a No prescaler

	// Calculate the number of timer overflows required for the delay
	u16 required_overflows = (u16)(ticks_required / 256.0);
	
	// Calculate the remainder ticks
// 	u8 remainder_ticks = (u8)(ticks_required - (required_overflows * 256.0));

	// Calculate the remaining ticks within the current overflow
	u8 remainder_ticks = (u8)((u32)ticks_required % 256UL);
	
	// Reset the overflow count for the current delay
	TIMER0_u8_OVF_Number = 0;
	
	TIMER0_Delay_us_Init();

	// Wait for the required number of overflows
	while (TIMER0_u8_OVF_Number < required_overflows);
	
	// 	u8 *pCnt_Val = NULL;	
	// 	TIMER0_GetCounterValue(pCnt_Val);
	// Wait for the remaining ticks
	while (TCNT0 < remainder_ticks);

	// Reset the overflow count for the next delay
	TIMER0_u8_OVF_Number = 0;
	TIMER0_Stop();
}
//===============================================
//========== TIMER1 Generic Functions ===========
//===============================================
u16 TOP=0;

void TIMER1_CM_CHA_Handler(void)
{
	TOGGLE_BIT(PORTD,TMR1_CHA_Interrupt_LED);
}
void TIMER1_OVF_CHA_Handler(void)
{
	TOGGLE_BIT(PORTD,TMR1_CHA_Interrupt_LED);
}

void TIMER1_CM_CHB_Handler(void)
{
	TOGGLE_BIT(PORTA,TMR1_CHB_Interrupt_LED);
}
void PWM1_Init_Handle()
{
	TIMER1_Config_t timer1Config = {
		PWM1_Fast_ICR1_MODE,
		TIMER1_SELECT_CHANNEL_A_B,
		PRESCALING_CLK_8,
		OCMIE1A_Enable,
		OCMIE1B_Enable,
		TOVIE1_Disable,
		PWM1_STATE_CHA_CHB_INVERTING
	};
	//TIMER1_SetCallBack_Overflow_Interrupt(TIMER1_OVF_CHA_Handler);
	TIMER1_SetCallBack_CompareMatch_Interrupt(TIMER1_CM_CHA_Handler,TIMER1_SELECT_CHANNEL_A);
	TIMER1_SetCallBack_CompareMatch_Interrupt(TIMER1_CM_CHB_Handler,TIMER1_SELECT_CHANNEL_B);

	// Initialize Timer1 for PWM in Fast PWM ICR1 mode
	TOP = 0x00FF;
	TIMER1_SetICR1Value(TOP);
	//CLR_BIT(PORTD,TMR1_CHA_Interrupt_LED);
	TIMER1_Init(&timer1Config);
	

}