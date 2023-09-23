#ifndef __DIO_INTERFACE_H__
#define __DIO_INTERFACE_H__

#include "STDTYPE.h"


// PORTS
typedef enum{
	Port_A,
	Port_B,
	Port_C,
	Port_D,
	PORT_LOW = 0X00,
	PORT_HIGH = 0Xff ,
	PORT_INPUT = 0x00,
	PORT_OUTPUT = 0xff
}PORTS_ENUM;
// PINS
typedef enum{
	Pin_0 = 0,
	Pin_1,
	Pin_2,
	Pin_3,
	Pin_4,
	Pin_5,
	Pin_6,
	Pin_7,
	LOW = 0,
	HIGH,
	INPUT = 0,
	OUTPUT	
}PINS_ENUM;

void DIO_SetPinValue(u8 port , u8 pin_num , u8 Value);
void DIO_SetPinDirection(u8 port , u8 pin_num , u8 Direction);
u8 DIO_GetPinValue(u8 port , u8 pin_num);

void DIO_SetPortValue(u8 port , u8 value);
void DIO_SetPortDirection(u8 port , u8 Direction);
void DIO_GetPortValue(u8 port , u8 *value);

#endif 