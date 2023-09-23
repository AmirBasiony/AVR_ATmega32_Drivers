/*
 * UART.c
 *
 * Created: 25/07/2023 02:21:02 ص
 *  Author: Amir
 */ 
#include "UART.h"

u8* TX_str;
u8 flag_send = 1;

void UART_Init(void)
{
	// Baud_Rate
	UBRRL = 51; // for baud rate = 9600
	// Clear U2X for normal Mode
	CLR_BIT(UCSRA,U2X); 
	
	// Frame 	
	// No Parity is initial value by default
	CLR_BIT(UCSRC,UPM0);
	CLR_BIT(UCSRC,UPM1);
	// 8-bit Data is initial value by default
	SET_BIT(UCSRC,UCSZ0);
	SET_BIT(UCSRC,UCSZ1);
	CLR_BIT(UCSRB,UCSZ2);
	
	// Enable UART
	//(Enabling UART Should Done After Handling the Configuration) 
	SET_BIT(UCSRB,TXEN); // Set Pin TXEN To Enable UART Transmit
	SET_BIT(UCSRB,RXEN); // Set Pin RXEN To Enable UART Receive
}
void UART_Send_Char(u8 data)
{
	while(!GET_BIT(UCSRA,UDRE)); // Check Receive Data Register Empty 
	UDR  = data ;
}
u8 UART_Receive_Char(void)
{
	while(!GET_BIT(UCSRA,RXC)); // Check USART Receive Complete (if RXC = 1 So Receive Buffer is Complete,if RXC=0 So Receive  Buffer is not Complete)
	return UDR ;
}
void UART_Send_String(u8 *string)
{
/*	while(*string != '\0')
	{	
		UART_Send_Char(*string++);
	}
*/
	u8 i=0; 
	for(i=0 ;i<string[i];i++)  
	{
		UART_Send_Char(string[i]);
	}	
	UART_Send_Char(DefaultStop);
}
void UART_Receive_String(u8 *string)
{
	*string = UART_Receive_Char();
	while(*string != DefaultStop)
	{
		string++;
		*string = UART_Receive_Char();				
	}
	*string = '\0';
}
void UART_Send_NumberU32(u32 num)
{
	UART_Send_Char((u8)(num));
	UART_Send_Char((u8)(num>>8));
	UART_Send_Char((u8)(num>>16));
	UART_Send_Char((u8)(num>>24));
	/*
	u8* ptr = &num;
	UART_Send_Char(ptr[0]);
	UART_Send_Char(ptr[1]);
	UART_Send_Char(ptr[2]);
	UART_Send_Char(ptr[3]);	
	*/
}
u32 UART_Receive_NumberU32(void)
{
	/*
	u32 num;
	u8 b1,b2,b3,b4;
	b1 = UART_Receive_Char();
	b2 = UART_Receive_Char();
	b3 = UART_Receive_Char();
	b4 = UART_Receive_Char();
	num = b1 | (b2<<8) | (b3<<16) | (b4<<24);
	*/
	u32 num;
	u8* ptr = (u8*)&num;
	ptr[0] = UART_Receive_Char();
	ptr[1] = UART_Receive_Char();
	ptr[2] = UART_Receive_Char();
	ptr[3] = UART_Receive_Char();
	return num;
}

u8 UART_Receive_PeriodicCheck(u8 *pdata)
{
	if(GET_BIT(UCSRA,RXC)) // Check USART Receive Complete (if RXC = 1 So Receive Buffer is Complete,if RXC=0 So Receive  Buffer is not Complete)
	{
		*pdata = UDR;
		return 1;
	}
	return 0;
}
void UART_Send_NoBlock(u8 data)
{
	UDR = data;
}
u8  UART_Receive_NoBlock(void)
{
	return UDR;
}
void UART_RX_InterruptEnable(void)
{
	SET_BIT(UCSRB,RXCIE);
}
void UART_RX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,RXCIE);
}
void UART_TX_InterruptEnable(void)
{
	SET_BIT(UCSRB,TXCIE);
}
void UART_TX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,TXCIE);
}
// Send String Using Interrupt

void UART_Send_Asynch(u8* str)
{
	if(flag_send == 1)
	{
		UART_TX_InterruptEnable();
		UART_Send_NoBlock(str[0]);
		TX_str = str;
		flag_send = 0;
	}
}
/*
ISR(USART_TXC_vect)
{
	static u8 i = 1;
	if(TX_str[i] != 0)
	{
		UART_Send_NoBlock(TX_str[i]);
		i++;
	}
	else
	{
		flag_send = 1;
		i = 1;
	}
}
*/

// Interrupt Vectors Handling
void (* UART_TX_vec) (void) = NULL ;
void (* UART_RX_vec) (void) = NULL ;

// For Receive Interrupt
void USART_RX_CallBack(void (* func)(void))
{
	if(func != NULL)
	{
		UART_RX_vec = func;
	}
}
void __vector_13 (void)__attribute__((signal));
void __vector_13 (void)
{
	UART_RX_vec();
}

// For Transmit Interrupt
void USART_TX_CallBack(void (* func)(void))
{
	if(func != NULL)
	{
		UART_TX_vec = func;
	}
}

void __vector_15 (void)__attribute__((signal));
void __vector_15 (void)
{
	UART_TX_vec();
}
