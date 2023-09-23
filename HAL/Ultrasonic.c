#include "Ultrasonic.h"
#include "Timers_Private.h"
#include "Timer.h"
#include "UTILS.h"
#include "DIO_interface.h"
#include "Timers_APIs.h"
// Function to calculate the total counter value including overflow counts
uint32_t Get_counter_value()
{
	return (TCNT0 + (TIMER0_u8_OVF_Number * 256)); // Calculate total counts
}

// Initialize the ultrasonic sensor
void Ultra_Init(uint8_t portName, uint8_t trig_pin, uint8_t echo_pin)
{
	// Configure the trigger and echo pins as output and input respectively
	DIO_SetPinDirection(portName,trig_pin,OUTPUT);
	DIO_SetPinDirection(portName,echo_pin,INPUT);
	DIO_SetPinValue(portName,echo_pin,LOW);
}

// Measure and return the distance using the ultrasonic sensor
uint16_t GetUltrasonicDistance(uint8_t portName, uint8_t trig_pin, uint8_t echo_pin)
{
	uint16_t distance;
	TIMER0_Config_t timer0Config = {TIMER0_NORMAL_MODE ,PRESCALING_CLK_64 ,OCMIE0_Disable ,TOVIE0_Enable,PWM0_STATE_Disable};
	GIE_enable();
	
	// Trigger the ultrasonic sensor
	DIO_SetPinValue(portName,trig_pin,HIGH);
	delay_us(10);
	DIO_SetPinValue(portName,trig_pin,LOW);
	TIMER0_Init(&timer0Config);
	// Measure the time for the echo to return
	while (!(DIO_GetPinValue(portName, echo_pin)));
	
	TIMER0_u8_OVF_Number = 0; // Clear overflow count
	TIMER0_SetCallBack_Overflow_Interrupt(NULL);
	
	TIMER0_SetCounterValue(0);
	/*TIMER0_Start();*/

	while (DIO_GetPinValue(portName, echo_pin));

	TIMER0_Stop();
	uint32_t Total_ticks = Get_counter_value();

	uint32_t elapsedTime = Total_ticks * (PRESCALER / CPU_F);			 // 64 stands for the preScaler
																		 // 8000000.0 is the CPU freq
	distance = (elapsedTime * SPEED_OF_SOUND_CM_PER_US * MM_PER_CM) / 2; // Distance in millimeters

	return distance;
}
