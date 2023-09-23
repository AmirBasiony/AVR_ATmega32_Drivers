/*
 * interrupt_interface.h
 *
 * Created: 02/03/2023 07:50:29 م
 *  Author: Amir
 */ 
#include "STDTYPE.h"
#include "UTILS.h"

#ifndef INTERRUPT_INTERFACE_H_
#define INTERRUPT_INTERFACE_H_


#define SREG	*((volatile u8 *)0x5F)
#define SREG_BIT7 7

#define MCUCR	*((volatile u8 *)0x55)
#define MCUCR_ISC00 0
#define MCUCR_ISC01 1
#define MCUCR_ISC10 2
#define MCUCR_ISC11 3

#define GICR	*((volatile u8 *)0x5B)
#define GICR_INT2_PIN 5
#define GICR_INT0_PIN 6
#define GICR_INT1_PIN 7

// interrupt 2
#define MCUCSR	*((volatile u8 *)0x54 )
#define MCUCSR_ISC2_PIN 6

#define LOW_LEVEL 0
#define on_change 1 
#define falling_edge 2
#define rising_edge 3

#define Sense_Control falling_edge 

void GIE_enable(void);
void GIE_disable(void);
void INT_0_init(void);
void INT_1_init(void);
void INT_2_init(void);

void INT0_CallBack(void (* func)(void));
void INT1_CallBack(void (* func)(void));
void INT2_CallBack(void (* func)(void));



#endif /* INTERRUPT_INTERFACE_H_ */