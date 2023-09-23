/*
 * Timers_APIs.h
 *
 * Created: 06/09/2023 11:40:07 م
 *  Author: Amir
 */ 


#ifndef TIMERS_APP_H_
#define TIMERS_APP_H_

#include "Timer.h"
#include "DIO_interface.h"
#include "DIO_private.h"

//-------------
//TIMER0 MACROS
//-------------
#define F_CPU	8000000UL
// Define Pulse Output Pins
#define PULSE_OC0_PIN			 Pin_3
#define TMR0_OC0_Interrupt_LED   Pin_2   // TMR0_CH_Interrupt_LED  (PORTB.PIN_2)

//-------------
//TIMER0 MACROS
//-------------
// Motor control pins
#define MOTOR1_PWM_PIN			Pin_5	// Motor 1 PWM pin (OC1A)
#define MOTOR2_PWM_PIN			Pin_4	// Motor 2 PWM pin (OC1B)
#define TMR1_CHA_Interrupt_LED	Pin_0   // TMR1_CHA_Interrupt_LED (PORTD.PIN_0)
#define TMR1_CHB_Interrupt_LED  Pin_0   // TMR1_CHB_Interrupt_LED (PORTA.PIN_0)

//===============================================
//========== TIMER0 Generic Functions ===========
//===============================================

extern u8 CntL;
extern u8 CntH ;
extern u8 flag;

void TIMER0_CM_CALLBack_Handler(void);
void TIMER0_OVFI_With_CMI_CALLBack_Handler();
void TIMER0_Generate_PWM_Using_OVFI_OMI(u8 DutyCycle);
//-----------------------------------------------------------------
void TIMER0_OVF_CALLBack_Handler(void);
void TIMER0_Generate_PWM_Using_OVFI(u8 DutyCycle);
//-----------------------------------------------------------------
void PWM0_Init_Handle();
//-----------------------------------------------------------------
void CustomPWM0_Without_Interrupts(u8 counter,u8 pin);
//-----------------------------------------------------------------
// Make a delay with milliseconds by using (TIMER0)
void TIMER0_Delay_ms_Init();
void delay_ms(u16 milliseconds);
// Make a delay with microseconds by using (TIMER0)
void TIMER0_Delay_us_Init();
void delay_us(u32 microseconds);
//===============================================
//========== TIMER1 Generic Functions ===========
//===============================================
extern u16 TOP;

void TIMER1_CM_CHA_Handler(void);
void TIMER1_OVF_CHA_Handler(void);

void TIMER1_CM_CHB_Handler(void);
void PWM1_Init_Handle();
#endif /* TIMERS_APP_H_ */