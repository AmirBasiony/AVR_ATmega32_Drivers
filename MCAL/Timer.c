/*
 * Timer.c
 *
 * Created: 20/08/2023 03:38:30 AM
 *  Author: Amir
 */ 
//******************************************************************
/***********************       Includes        *********************
 *******************************************************************/

#include "interrupt_interface.h"
#include "Timer.h"
#include "Timers_Private.h"
#include "STDTYPE.h"
#include "UTILS.h"
//================================================================
//				Generic Variables
//================================================================
//-------
//TIMER0 
//-------
// Callback function pointers
PTR_ToVoidFun_t TIMER0_CaLLBack_OVF = NULL;
PTR_ToVoidFun_t TIMER0_CaLLBack_CM = NULL;
// Generic Variables
TIMER0_Config_t G_TIMER0_Config;
u16 TIMER0_u8_OVF_Number;
u16 TIMER0_u8_CTC_Number;

//-------
//TIMER1
//-------
// Callback function pointers
PTR_ToVoidFun_t TIMER1_CaLLBack_OVF = NULL;
PTR_ToVoidFun_t TIMER1_CaLLBack_CM[2] = {NULL, NULL};

// Generic Variables
TIMER1_Config_t G_TIMER1_Config;
u16 TIMER1_u16_OVF_Number;
u16 TIMER1_u16_CTC_Number[2];


//================================================================
//				APIs Functions Definition
//================================================================


//---------------------------------------------------------------
//---------------------     TIMER1 APIs   -----------------------
//---------------------------------------------------------------
// Initialize Timer0 based on the provided configuration
E_STATUS_t TIMER0_Init(TIMER0_Config_t *TIM0_Config) {
	E_STATUS_t RetValue = E_NOK;
	
	// Copy the configuration
	G_TIMER0_Config = *TIM0_Config;
	
	// Resetting Timer0 Counter Value
	TCNT0 = 0;
		
		
	// Configure Timer mode
	if ((G_TIMER0_Config.MODE == TIMER0_NORMAL_MODE) ||
	(G_TIMER0_Config.MODE == TIMER0_CTC_MODE) ||
	(G_TIMER0_Config.MODE == TIMER0_Phase_Correct_PWM) ||
	(G_TIMER0_Config.MODE == TIMER0_FAST_PWM_MODE)) 
	{
		TCCR0 |= G_TIMER0_Config.MODE;
	} 
	else
	{
		RetValue = E_NOK;
	}
	
	// Configure Prescaler
	if ((G_TIMER0_Config.PRESCALER_CLK >= NO_CLK_SRC) && (G_TIMER0_Config.PRESCALER_CLK <= EXTERNAL_CLK_RISING_EDG)) 
	{
		TCCR0 |= G_TIMER0_Config.PRESCALER_CLK;
	} 
	else
	{
		RetValue = E_NOK;
	}

	// Configure Overflow Interrupt
	if (G_TIMER0_Config.OVF_Interrupt == TOVIE0_Enable)
	{
		TIMSK |= G_TIMER0_Config.OVF_Interrupt;
	} 
	else if (G_TIMER0_Config.OVF_Interrupt == TOVIE0_Disable) 
	{
		TIMSK &= TOVIE0_Disable;
	} 
	else
	{
		RetValue = E_NOK;
	}

	// Configure Compare Match Interrupt
	if ((G_TIMER0_Config.OCM_Interrupt == OCMIE0_Enable)) 
	{
		TIMSK |= G_TIMER0_Config.OCM_Interrupt;
	}
	else if(G_TIMER0_Config.OCM_Interrupt == OCMIE0_Disable)
	{
		TIMSK &= G_TIMER0_Config.OCM_Interrupt;
	}
	else
	{
		RetValue = E_NOK;
	}

	// Initialize Timer0 PWM settings if applicable
	if ((G_TIMER0_Config.MODE == TIMER0_Phase_Correct_PWM) || (G_TIMER0_Config.MODE == TIMER0_FAST_PWM_MODE)) 
	{
		TCCR0 |= TIM0_Config->PWM0_STATE; // Inverting or non-inverting
	}
	

	return RetValue;
}

void PWM0_SetDutyCycleValue(u8 DutyCycle)
{
	u8 Compare_Value = (DutyCycle / 100.0) * 255;
	TIMER0_SetCompareValue(Compare_Value);
}

E_STATUS_t TIMER0_Start(void)
{
	E_STATUS_t Value = E_OK;
	TCCR0 |= G_TIMER0_Config.PRESCALER_CLK;
	return Value;
}

E_STATUS_t TIMER0_Stop(void)
{
	E_STATUS_t	RetValue = E_OK;
	TCCR0	&=	NO_CLK_SRC;
	return	RetValue;
}

E_STATUS_t TIMER0_GetCompareValue(u8 *ptr_u8_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	*ptr_u8_ticks	=	OCR0;
	return	RetValue;
}

E_STATUS_t TIMER0_SetCompareValue(u8 u8_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	OCR0 = u8_ticks;
	return	RetValue;
}

u8 TIMER0_GetCounterValue(void)
{
	u8 u8_ticks = TCNT0;
	
	return	u8_ticks;
}

E_STATUS_t TIMER0_SetCounterValue(u8 u8_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	TCNT0 = u8_ticks;
	return	RetValue;
}

E_STATUS_t TIMER0_GetOverflowValue(u8 *ptr_u8_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	*ptr_u8_ticks = TIMER0_u8_OVF_Number;
	return	RetValue;
}

E_STATUS_t TIMER0_SetOverflowValue(u8 u8_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	TIMER0_u8_OVF_Number = u8_ticks;
	return	RetValue;
}

void  TIMER0_SetCallBack_Overflow_Interrupt(PTR_ToVoidFun_t CallBack)
{
	if(CallBack != NULL)
	{
		TIMER0_CaLLBack_OVF = CallBack;
	}
}

void  TIMER0_SetCallBack_CompareMatch_Interrupt(PTR_ToVoidFun_t CallBack)
{
	if(CallBack != NULL)
	{
		TIMER0_CaLLBack_CM = CallBack;
	}
}

//---------------------------------------------------------------
//---------------------     TIMER1 APIs   -----------------------
//---------------------------------------------------------------

E_STATUS_t TIMER1_Init(TIMER1_Config_t *TIM1_Config)
{
	//Variables
	E_STATUS_t RetValue = E_OK;
	u8 Temp_TCCR1A = 0 ,Temp_TCCR1B = 0;

	// Copy the configuration
	G_TIMER1_Config = *TIM1_Config;

	// Initialize the counter (TCNT1)
	TCNT1 = 0;
	
	// Configure Timer mode
	//for bits(WGM13,WGM12,WGM11,WGM10)
	//WGM11,WGM10 @ TCCR1A bit(1,0)
	Temp_TCCR1A |= ((G_TIMER1_Config.MODE) & 0x03);
	//WGM13,WGM12 @ TCCR1B bit(4,3)
	Temp_TCCR1B |= ((((G_TIMER1_Config.MODE) & 0x0C) >> 2) << 3);
	
	// Configure Prescaler
	if ((G_TIMER1_Config.PRESCALER_CLK >= NO_CLK_SRC) && (G_TIMER1_Config.PRESCALER_CLK <= EXTERNAL_CLK_RISING_EDG))
	{
		Temp_TCCR1B |= G_TIMER1_Config.PRESCALER_CLK;
	}
	else
	{
		RetValue = E_NOK;
	}
	

	//Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable
	if(TOVIE1_Disable == G_TIMER1_Config.OVF_Interrupt)
	{
		TIMSK &= TOVIE1_Disable;		
	}								
	else if(TOVIE1_Enable == G_TIMER1_Config.OVF_Interrupt)
	{
		TIMSK |= TOVIE1_Enable;
	}
	else
	{
		RetValue = E_NOK;
	}

	if(TIMER1_SELECT_CHANNEL_A == G_TIMER1_Config.SELECT_CHANNEL)
	{		
		// Configure Output Compare A Match Interrupt Enable
		//TIMSK: Bit 4 – OCIE1A: Timer/Counter1
		if ((G_TIMER1_Config.OCAM_Interrupt == OCMIE1A_Enable))
		{
			TIMSK |= G_TIMER1_Config.OCAM_Interrupt;
		
		}
		else if(G_TIMER1_Config.OCAM_Interrupt == OCMIE1A_Disable)
		{
			TIMSK &= G_TIMER1_Config.OCAM_Interrupt;
		}
		else
		{
			RetValue = E_NOK;
		}	
	}
	else if(TIMER1_SELECT_CHANNEL_B == G_TIMER1_Config.SELECT_CHANNEL)
	{
		// Configure  Output Compare B Match Interrupt Enable
		//TIMSK: Bit 3 – OCIE1B: Timer/Counter1
		if ((G_TIMER1_Config.OCBM_Interrupt == OCMIE1B_Enable))
		{
			TIMSK |= G_TIMER1_Config.OCBM_Interrupt;
			
		}
		else if(G_TIMER1_Config.OCBM_Interrupt == OCMIE1B_Disable)
		{
			TIMSK &= G_TIMER1_Config.OCBM_Interrupt;
		}
		else
		{
			RetValue = E_NOK;
		}
	}
	else if(TIMER1_SELECT_CHANNEL_A_B == G_TIMER1_Config.SELECT_CHANNEL)
	{		
		// Configure Output Compare A Match Interrupt Enable
		//TIMSK: Bit 4 – OCIE1A: Timer/Counter1
		if ((G_TIMER1_Config.OCAM_Interrupt == OCMIE1A_Enable))
		{
			TIMSK |= G_TIMER1_Config.OCAM_Interrupt;
			
		}
		else if(G_TIMER1_Config.OCAM_Interrupt == OCMIE1A_Disable)
		{
			TIMSK &= G_TIMER1_Config.OCAM_Interrupt;
		}
		else
		{
			RetValue = E_NOK;
		}
		
		// Configure  Output Compare B Match Interrupt Enable
		//TIMSK: Bit 3 – OCIE1B: Timer/Counter1
		if ((G_TIMER1_Config.OCBM_Interrupt == OCMIE1B_Enable))
		{
			TIMSK |= G_TIMER1_Config.OCBM_Interrupt;
			
		}
		else if(G_TIMER1_Config.OCBM_Interrupt == OCMIE1B_Disable)
		{
			TIMSK &= G_TIMER1_Config.OCBM_Interrupt;
		}
		else
		{
			RetValue = E_NOK;
		}		
	}
	else
	{
		RetValue = E_NOK;
	}
	
	// Function to initialize PWM settings for Timer1 (internal use) if applicable
	if((G_TIMER1_Config.MODE != TIMER1_NORMAL_MODE) &&
	(G_TIMER1_Config.MODE != TIMER1_CTC_OCR1_MODE)&&
	(G_TIMER1_Config.MODE != TIMER1_CTC_ICR1_MODE))
	{	
		Temp_TCCR1A |= G_TIMER1_Config.PWM1_STATE;
	}

    // Copy Temp_TCCR1A value to TCCR1A
    TCCR1A |= Temp_TCCR1A;
    // Copy Temp_TCCR1B value to TCCR1B
    TCCR1B |= Temp_TCCR1B; 

	return RetValue;
}

// Function to Start Timer1
E_STATUS_t TIMER1_Start(void)
{
	E_STATUS_t	RetValue = E_OK;
	TCCR1B	&=	G_TIMER1_Config.PRESCALER_CLK;
	return	RetValue;
}
// Function to stop Timer1
E_STATUS_t TIMER1_Stop(void)
{
	E_STATUS_t	RetValue = E_OK;
	TCCR1B	&=	NO_CLK_SRC;
	return	RetValue;	
}
// Function to set the Input capture value of Timer1
E_STATUS_t TIMER1_SetICR1Value(u16 u16_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	ICR1 = u16_ticks;
	return	RetValue;
}
// Function to set the compare value of Timer1
E_STATUS_t TIMER1_SetCompareValue(u16 u16_ticks,u8 CHANNEL)
{
	E_STATUS_t	RetValue = E_OK;
	if(CHANNEL == TIMER1_SELECT_CHANNEL_A_B)
	{
		OCR1A = u16_ticks;
		OCR1B = u16_ticks;
	}
	else if(CHANNEL == TIMER1_SELECT_CHANNEL_A)
	{
		OCR1A = u16_ticks;
		//OCR1AH = (u8)((u16_ticks & 0xFF00) >> 8);
		//OCR1AL = (u8)(u16_ticks  & 0x00FF);
	}
	else if(CHANNEL == TIMER1_SELECT_CHANNEL_B)
	{
		OCR1B = u16_ticks;
		//OCR1BH = (u8)((u16_ticks & 0xFF00) >> 8);
		//OCR1BL = (u8)(u16_ticks  & 0x00FF);
	}	
	else
	{
		RetValue = E_NOK;
	}
	
	return	RetValue;	
}

// Function to get the current counter value of Timer1
u16 TIMER1_GetCounterValue()
{
	u16 u16_ticks = TCNT1;
// 	*ptr_u16_ticks = ((u16)TCNT1L);	
// 	*ptr_u16_ticks =  (((u16)TCNT1H) << 8);

	return	u16_ticks;	
}
// Function to set the counter value of Timer1
E_STATUS_t TIMER1_SetCounterValue(u16 u16_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	
	TCNT1 = u16_ticks;
// 	TCNT1H = (u8)((u16_ticks & 0xFF00) >> 8);
// 	TCNT1L = (u8)(u16_ticks  & 0x00FF);

	return	RetValue;	
}
// Function to get the overflow counter value of Timer1
E_STATUS_t TIMER1_GetOverflowValue(u16 *ptr_u16_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	
	*ptr_u16_ticks = TIMER1_u16_OVF_Number;
	
	return	RetValue;	
}
// Function to set the overflow counter value of Timer1
E_STATUS_t TIMER1_SetOverflowValue(u16 u16_ticks)
{
	E_STATUS_t	RetValue = E_OK;
	
	TIMER1_u16_OVF_Number = u16_ticks;
	
	return	RetValue;	
}
// Function to set the overflow interrupt callback function for Timer1
void TIMER1_SetCallBack_Overflow_Interrupt(PTR_ToVoidFun_t CallBack)
{
	TIMER1_CaLLBack_OVF = CallBack;
}

// Function to set the compare match interrupt callback function for Timer1
E_STATUS_t TIMER1_SetCallBack_CompareMatch_Interrupt(PTR_ToVoidFun_t CallBack,u8 CHANNEL)
{
	E_STATUS_t	RetValue = E_OK;
	TIMER1_CaLLBack_CM[CHANNEL] = CallBack;
	return	RetValue;
}

//calculate OCR1AL/OCR1AH OR OCR1BL/OCR1BH from the Duty Cycle
E_STATUS_t PWM1_SetDutyCycleAndICR1Value(u8 DutyCycle,u8 CHANNEL,u16 TOP)
{
	E_STATUS_t	RetValue = E_OK;
	
	u16 Compare_Value = (u16)((DutyCycle / 100.0) * TOP);	
	RetValue = TIMER1_SetCompareValue(Compare_Value,CHANNEL);
	
	return	RetValue;
}

//==================================================================
//==================INTERRUPT VECTOR TABLE HANDLER==================
//==================================================================

//-----------
// TIMER1 IVT
//-----------

// (7)TIMER1 COMPA Timer/Counter1 Compare Match A
void __vector_7 (void)__attribute__((signal));
void __vector_7 (void)
{
	TIMER1_u16_CTC_Number[TIMER1_SELECT_CHANNEL_A]++;	
	
	if (TIMER1_CaLLBack_CM[TIMER1_SELECT_CHANNEL_A] != NULL) 
	{
		TIMER1_CaLLBack_CM[TIMER1_SELECT_CHANNEL_A]();
	}
	
}

//	(8)TIMER1 COMPB Timer/Counter1 Compare Match B
void __vector_8 (void)__attribute__((signal));
void __vector_8 (void)
{
	TIMER1_u16_CTC_Number[TIMER1_SELECT_CHANNEL_B]++;
	
	if (TIMER1_CaLLBack_CM[TIMER1_SELECT_CHANNEL_B] != NULL) 
	{
		TIMER1_CaLLBack_CM[TIMER1_SELECT_CHANNEL_B]();
	}
}
// (9)TIMER1 OVF Timer/Counter1 Overflow
void __vector_9 (void)__attribute__((signal));
void __vector_9 (void)
{
	TIMER0_u8_OVF_Number++;
	
	if(TIMER1_CaLLBack_OVF != NULL)
	{
		TIMER1_CaLLBack_OVF();	
	}	
	
}

//-----------
// TIMER0 IVT
//-----------

void __vector_10 (void)__attribute__((signal));
void __vector_10 (void)
{
	TIMER0_u8_CTC_Number++;
	if(TIMER0_CaLLBack_CM != NULL)
	{
		TIMER0_CaLLBack_CM();
	}
	
}

void __vector_11 (void)__attribute__((signal));
void __vector_11 (void)
{
	TIMER0_u8_OVF_Number++;
	if(TIMER0_CaLLBack_OVF != NULL)
	{
		TIMER0_CaLLBack_OVF();
	}	
}



