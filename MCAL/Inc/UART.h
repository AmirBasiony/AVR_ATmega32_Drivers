/*
 * UART.h
 *
 * Created: 25/07/2023 02:20:50 ص
 *  Author: Amir
 */ 


#ifndef UART_H_
#define UART_H_

#include "STDTYPE.h"
#include "DIO_interface.h"
#include "UTILS.h"

typedef enum{
	//PINS of UCSRA 
	MPCM,//• Bit 0 – MPCM: Multi-processor Communication Mode
	U2X,// • Bit 1 – U2X: Double the USART Transmission Speed
	PE,//  • Bit 2 – PE: Parity Error
	DOR,// • Bit 3 – DOR: Data OverRun
	FE,//  • Bit 4 – FE: Frame Error
	UDRE,//• Bit 5 – UDRE: USART Data Register Empty
	TXC,// • Bit 6 – TXC: USART Transmit Complete
	RXC,// • Bit 7 – RXC: USART Receive Complete
	//PINS of UCSRB
	TXB8 = 0,//• Bit 0 – TXB8: Transmit Data Bit 8
	RXB8,// • Bit 1 – RXB8: Receive Data Bit 8
	UCSZ2,//• Bit 2 – UCSZ2: Character Size
	TXEN,// • Bit 3 – TXEN: Transmitter Enable
	RXEN,// • Bit 4 – RXEN: Receiver Enable
	UDRIE,//• Bit 5 – UDRIE: USART Data Register Empty Interrupt Enable
	TXCIE,//• Bit 6 – TXCIE: TX Complete Interrupt Enable
	RXCIE,//• Bit 7 – RXCIE: RX Complete Interrupt Enable
	//PINS of UCSRC
	UCPOL = 0,//• Bit 0 – UCPOL: Clock Polarity
	UCSZ0,//• Bit 1 – UCSZ0: Character Size
	UCSZ1,//• Bit 2 – UCSZ1: Character Size
	USBS,// • Bit 3 – USBS: Stop Bit Select
	UPM0, //• Bit 4 – UPM0: Parity Mode
	UPM1, //• Bit 5 – UPM1: Parity Mode
	UMSEL,//• Bit 6 – UMSEL: USART Mode Select
	URSEL,//• Bit 7 – URSEL: Register Select
}UART_REG_PIN_t;

#define UDR     *((volatile u8 *)0x2C)
#define UCSRA   *((volatile u8 *)0x2B)
#define UCSRB   *((volatile u8 *)0x2A)
#define UBRRL   *((volatile u8 *)0x29)
#define UCSRC   *((volatile u8 *)0x40)
#define UBRRH   *((volatile u8 *)0x40)
//Status Register For Interrupt
#define SREG    *((volatile u8 *)0x5F)

#define DefaultStop		'\r'

void UART_Init(void);
void UART_Send_Char(u8 data);
u8 UART_Receive_Char(void);
void UART_Send_NumberU32(u32 num);
u32 UART_Receive_NumberU32(void);
void UART_Send_String(u8 *string);
void UART_Receive_String(u8 *string);
u8 UART_Receive_PeriodicCheck(u8 *pdata);

void UART_Send_NoBlock(u8 data);
u8  UART_Receive_NoBlock(void);
void UART_Send_Asynch(u8* str);
void UART_RX_InterruptEnable(void);
void UART_RX_InterruptDisable(void);
void UART_TX_InterruptEnable(void);
void UART_TX_InterruptDisable(void);


void UART_TX_ISR();
void UART_RX_ISR();

#endif /* UART_H_ */