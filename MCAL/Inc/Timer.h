
#ifndef TIMER_H_
#define TIMER_H_


//****************************************************************
/***********************       Includes        *******************
******************************************************************/
#include "Timers_Private.h"
#include "STDTYPE.h"
#include "UTILS.h"

//******************************************************************
//******************   Extern Generic Variables		****************
//******************************************************************
//-------
//TIMER0
//-------
// Callback function pointers
PTR_ToVoidFun_t TIMER0_CaLLBack_OVF;
PTR_ToVoidFun_t TIMER0_CaLLBack_CM;
// Generic Variables
extern u16  TIMER0_u8_OVF_Number;
extern u16  TIMER0_u8_CTC_Number;

//-------
//TIMER1
//-------
// Callback function pointers
PTR_ToVoidFun_t TIMER1_CaLLBack_OVF;
PTR_ToVoidFun_t TIMER1_CaLLBack_CM[2];
// Generic Variables
extern u16 TIMER1_u16_OVF_Number;
extern u16 TIMER1_u16_CTC_Number[2];
//******************************************************************
//******************************************************************
/*************************       STRUCTS        ******************
******************************************************************/

typedef struct 
{
	u8 MODE;           // Timer mode, @ref TIMER1_MODE_Define
	u8 SELECT_CHANNEL;	   // Select between CHANNEL(A,B)	@ref TIMER1_SELECT_CHANNEL_Define
	u8 PRESCALER_CLK;  // Prescaler clock, @ref TIMER0_1_PRESCALER_CLK_Define
	u8 OCAM_Interrupt;  // Output Compare A Match interrupt, @ref TIMER1_OCAMIE_Define
	u8 OCBM_Interrupt;  // Output Compare B Match interrupt, @ref TIMER1_OCMBIE_Define
	u8 OVF_Interrupt;  // Overflow interrupt, @ref TIMER1_TOVIE1_Define
	u8 PWM1_STATE;     // PWM0 state, @ref PWM1_STATE_Define
	
} TIMER1_Config_t;

//------------------------------
// Structure to configure Timer0
//------------------------------
typedef struct 
{
    u8 MODE;           // Timer mode, @ref TIMER0_MODE_Define
    u8 PRESCALER_CLK;  // Prescaler clock, @ref TIMER0_1_PRESCALER_CLK_Define
    u8 OCM_Interrupt;  // Output Compare Match interrupt, @ref TIMER0_OCMIE_Define
    u8 OVF_Interrupt;  // Overflow interrupt, @ref TIMER0_TOVIE_Define
    u8 PWM0_STATE;     // PWM0 state, @ref PWM0_STATE_Define
	
} TIMER0_Config_t;

//******************************************************************
//*********************       MACROS         ***********************
//******************************************************************	
	
//*****************************************************************
//********************  TIMER0 MACROS References Define  **********
//*****************************************************************

//--------------------------
//@ref TIMER0_MODE_Define
//--------------------------
#define TIMER0_NORMAL_MODE              (u8)(0)
#define TIMER0_CTC_MODE                 (u8)(1<<WGM01)
#define TIMER0_Phase_Correct_PWM        (u8)(1<<WGM00)  //Update of OCR0 is TOP ()
#define TIMER0_FAST_PWM_MODE            (u8)((1<<WGM00) | (1<<WGM01)) //Update of OCR0 is BOTTOM

//---------------------------------------
//@ref TIMER0_1_PRESCALER_CLK_Define
//---------------------------------------
//TCCR0: Bit 2:0  – CS02 CS01 CS00  (Clock Select)
//TCCR1B: Bit 2:0 – CS12 CS11 CS10 (Clock Select)
// Prescaler clock settings
#define NO_CLK_SRC		                ~((u8)((1<<CS02) | (1<<CS01) | (1<<CS00)))//No clock source (Timer/Counter stopped).
#define NO_PRESCALING                   (u8)(1<<CS00)                 //clk		  (No prescaling)
#define PRESCALING_CLK_8                (u8)(1<<CS01)                 //clk/8     (From prescaler)
#define PRESCALING_CLK_64               (u8)((1<<CS01) | (1<<CS00))   //clk/64    (From prescaler)
#define PRESCALING_CLK_256              (u8) (1<<CS02)                //clk/256   (From prescaler)
#define PRESCALING_CLK_1024             (u8)((1<<CS02) | (1<<CS00))   //clk/1024     (From prescaler)
#define EXTERNAL_CLK_FALLING_EDG        (u8)((1<<CS02) | (1<<CS01))   //External clock source on T0/T1 pin. Clock on falling edge.
#define EXTERNAL_CLK_RISING_EDG         (u8)((1<<CS02) | (1<<CS01) | (1<<CS00))//External clock source on T0/T1 pin. Clock on rising edge.

//---------------------------------------
//@ref TIMER0_OCMIE_Define
//---------------------------------------
//TIMSK: Bit 1 – OCIE0:(Timer/Counter0) Output Compare Match Interrupt Enable
#define OCMIE0_Disable                   ~((u8)(1<<OCIE0))
#define OCMIE0_Enable                    (u8)(1<<OCIE0)

//---------------------------------------
//@ref TIMER0_TOVIE_Define
//---------------------------------------
//TIMSK: Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable
#define TOVIE0_Disable                    ~((u8)(1<<TOIE0))
#define TOVIE0_Enable                    (u8)(1<<TOIE0)

//---------------------------------------
//@ref PWM0_STATE_Define
//---------------------------------------
#define PWM0_STATE_Disable					(u8)(0)
#define PWM0_STATE_INVERTING				(u8)(1<<COM01 | 1<< COM00)
#define PWM0_STATE_NON_INVERTING			(u8)(1<<COM01)

//*****************************************************************
//****************  TIMER1 MACROS References Define  **************
//*****************************************************************

//----------------------
//@ref PWM1_STATE_Define
//----------------------
#define PWM1_STATE_CHA_Disable						~(u8)((1<<COM1A1) | (1<< COM1A0)) //Normal port operation, OC1A disconnected.
#define PWM1_STATE_CHB_Disable						~(u8)((1<<COM1B1) | (1<< COM1B0)) //Normal port operation, OC1B disconnected.
#define PWM1_STATE_CHA_CHB_Disable					~(u8)((1<<COM1A1) | (1<< COM1A0))|(1<<COM1B1) | (1<< COM1B0)) //Normal port operation,OC1A and OC1B disconnected.

#define PWM1_STATE_CHA_INVERTING					(u8)(1<<COM1A1 | 1<< COM1A0)		//(CHA is inverting mode)
#define PWM1_STATE_CHB_INVERTING					(u8)(1<<COM1B1 | 1<< COM1B0)		//(CHB is inverting mode)

#define PWM1_STATE_CHA_NON_INVERTING				(u8)(1<<COM1A1)						//(CHA is non-inverting mode)
#define PWM1_STATE_CHB_NON_INVERTING				(u8)(1<<COM1B1)						//(CHB is non-inverting mode)

#define PWM1_STATE_CHA_CHB_INVERTING				(u8)((1<<COM1A1)|(1<< COM1A0)|(1<<COM1B1)|(1<< COM1B0))//(CHA and CHB are inverting mode)
#define PWM1_STATE_CHA_CHB_NON_INVERTING			(u8)((1<<COM1B1)|(1<<COM1A1))			//(CHA and CHB are non-inverting mode)
#define PWM1_STATE_CHA_INVERTING_CHB_NON_INVERTING	(u8)((1<<COM1A1)|(1<< COM1A0)|(1<<COM1B1))//(CHA>>inverting mode,CHB>>non-inverting mode)
#define PWM1_STATE_CHB_INVERTING_CHA_NON_INVERTING	(u8)((1<<COM1B1)|(1<< COM1B0)|(1<<COM1A1))//(CHB>>inverting mode,CHA>>non-inverting mode)

#define PWM1_Toggle_OC1A_On_Compare_Match			(u8)(1<< COM1A0)		//WGM13:0 = 15: Toggle OC1A on Compare Match,
																			//OC1B disconnected (normal port operation).
//--------------------------------------------------------------------------------------

//--------------------------
//@ref TIMER1_OCMIE1A_Define
//--------------------------
//TIMSK: Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
#define OCMIE1A_Disable                   ~((u8)(1<<OCIE1A))
#define OCMIE1A_Enable                    (u8)(1<<OCIE1A)

//--------------------------
//@ref TIMER1_OCMIE1B_Define
//--------------------------
//TIMSK: Bit 3 – OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
#define OCMIE1B_Disable                   ~((u8)(1<<OCIE1B))
#define OCMIE1B_Enable                    (u8)(1<<OCIE1B)

//-------------------------
//@ref TIMER1_TOVIE1_Define
//-------------------------
//TIMSK:  Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable
#define TOVIE1_Disable                   ~((u8)(1<<TOIE1))
#define TOVIE1_Enable                    (u8)(1<<TOIE1)

//-------------------------
//@ref TIMER1_TICIE1_Define
//-------------------------
// NOT SUPPORTED!!!!!
// Bit 5 – TICIE1: Timer/Counter1, Input Capture Interrupt Enable
#define TICIE1_Disable                   ~((u8)(1<<TICIE1))	// NOT SUPPORTED!!!!!
#define TICIE1_Enable                    (u8)(1<<TICIE1)	// NOT SUPPORTED!!!!!

//******************************************************************
/*************************       ENUMS        *********************
*******************************************************************/

//*****************************************************************
//********************  TIMER1 ENUMS References Define  ***********
//*****************************************************************

//--------------------------
//@ref TIMER1_MODE_Define
//--------------------------
//enum for bits(WGM13,WGM12,WGM11,WGM10)
//WGM13,WGM12 @ TCCR1B bit(4,3)
//WGM11,WGM10 @ TCCR1A bit(1,0)
typedef enum
{
	TIMER1_NORMAL_MODE	= 0,	//TOP = (0xFFFF) ,Update of OCR1x = (Immediate) ,TOV1 Flag Set on (MAX)
	PWM1_Phase_Correct_8B_MODE,	
	PWM1_Phase_Correct_9B_MODE,	
	PWM1_Phase_Correct_10B_MODE,  
	TIMER1_CTC_OCR1_MODE,		//TOP = (OCR1) ,Update of OCR1x = (Immediate) ,TOV1 Flag Set on (MAX)		
	PWM1_Fast_8B_MODE,			
	PWM1_Fast_9B_MODE,			
	PWM1_Fast_10B_MODE,			
	PWM1_PhaseAndFreqCorrect_ICR1_MODE,  
	PWM1_PhaseAndFreqCorrect_OCR1_MODE,
	PWM1_Phase_Correct_ICR1_MODE,	   
	PWM1_Phase_Correct_OCR1_MODE,	   // TOP = (OCR1) ,Update of OCR1x = (TOP) ,TOV1 Flag Set on (BOTTOM)
	TIMER1_CTC_ICR1_MODE,			    
	PWM1_Fast_ICR1_MODE = 14,		   // TOP = (ICR1) ,Update of OCR1x = (BOTTOM) ,TOV1 Flag Set on (TOP)
	PWM1_Fast_OCR1A_MODE,			   // TOP = (OCR1A) ,Update of OCR1x = (BOTTOM) ,TOV1 Flag Set on (TOP)
	
}TIMER1_MODE_Define_E;

//-----------------------------------
//@ref TIMER1_SELECT_CHANNEL_Define
//-----------------------------------
typedef enum
{
	TIMER1_SELECT_CHANNEL_A,
	TIMER1_SELECT_CHANNEL_B,
	TIMER1_SELECT_CHANNEL_A_B,
	TIMER1_SELECT_NO_CHANNEL
}TIMER1_SELECT_UNIT_Define_E;
	
//****************************************************************
/*******************       APIs Functions        *****************
******************************************************************/

//-----------
//TIMER0 APIs
//-----------
// Function to initialize Timer0
E_STATUS_t TIMER0_Init(TIMER0_Config_t *TIM0_Config);
// Function to Start Timer0
E_STATUS_t TIMER0_Start(void);
// Function to stop Timer0
E_STATUS_t TIMER0_Stop(void);
// Function to set the compare value of Timer0
E_STATUS_t TIMER0_SetCompareValue(u8 u8_ticks);
// Function to get the current counter value of Timer0
E_STATUS_t TIMER0_GetCounterValue(u8 *ptr_u8_ticks);
// Function to set the counter value of Timer0
E_STATUS_t TIMER0_SetCounterValue(u8 u8_ticks);
// Function to get the overflow counter value of Timer0
E_STATUS_t TIMER0_GetOverflowValue(u8 *ptr_u8_ticks);
// Function to set the overflow counter value of Timer0
E_STATUS_t TIMER0_SetOverflowValue(u8 u8_ticks);
// Function to set the overflow interrupt callback function for Timer0
void TIMER0_SetCallBack_Overflow_Interrupt(PTR_ToVoidFun_t CallBack);
// Function to set the compare match interrupt callback function for Timer0
void TIMER0_SetCallBack_CompareMatch_Interrupt(PTR_ToVoidFun_t CallBack);

// Function to initialize PWM settings for Timer0 (internal use)
//void TIMER0_InitPWM(TIMER0_Config_t *TIM0_Config);
//calculate OCR0 from the Duty Cycle
void PWM0_SetDutyCycleValue(u8 DutyCycle);

//-----------
//TIMER1 APIs
//-----------
E_STATUS_t TIMER1_Init(TIMER1_Config_t *TIM1_Config);
// Function to Start Timer1
E_STATUS_t TIMER1_Start(void);
// Function to stop Timer1
E_STATUS_t TIMER1_Stop(void);
// Function to set the compare value of Timer1
E_STATUS_t TIMER1_SetICR1Value(u16 u16_ticks);
E_STATUS_t TIMER1_SetCompareValue(u16 u16_ticks,u8 CHANNEL);
// Function to get the current counter value of Timer1
E_STATUS_t TIMER1_GetCounterValue(u16 *ptr_u16_ticks);
// Function to set the counter value of Timer1
E_STATUS_t TIMER1_SetCounterValue(u16 u16_ticks);
// Function to get the overflow counter value of Timer1
E_STATUS_t TIMER1_GetOverflowValue(u16 *ptr_u16_ticks);
// Function to set the overflow counter value of Timer1
E_STATUS_t TIMER1_SetOverflowValue(u16 u16_ticks);
// Function to set the overflow interrupt callback function for Timer1 
void TIMER1_SetCallBack_Overflow_Interrupt(PTR_ToVoidFun_t CallBack);
// Function to set the compare match interrupt callback function for Timer1
E_STATUS_t TIMER1_SetCallBack_CompareMatch_Interrupt(PTR_ToVoidFun_t CallBack,u8 CHANNEL);

//calculate OCR1A/B from the Duty Cycle
E_STATUS_t PWM1_SetDutyCycleAndICR1Value(u8 DutyCycle,u8 CHANNEL,u16 TOP);

// Function to initialize PWM settings for Timer1 (internal use)
void TIMER1_InitPWM();
#endif /* TIMER_H_ */