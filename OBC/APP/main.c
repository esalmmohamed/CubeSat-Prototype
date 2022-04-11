
#include "GPIO.h"
#include "UARTt.h"
#include "command.h"
#include "ADC.h"
#include <avr/interrupt.h>
#include <stdio.h>

#ifndef F_CPU
#define F_CPU    (16000000UL)
#endif  // F_CPU


u8 command_rec[14];
u8 telemetry[33]={0x00,0x01,0x00,0x10};
u8 ADCS_comm[7];
u8 ack[3]={0x00,0XAC,0x00};
u8 Nack[3]={0x00,0x00,0xA0};
volatile u8 x;

ISR(USART_RXC_vect)
{
	SerialReceive(command_rec,14);
	x++;
}


int main(void)
{
	 GPIO_voidInit();
	 GPIO_u8_WritePinDir(0,0, 'A'); // PA0 ---> input  (ADC0)
     GPIO_u8_WritePinDir(3,1,'d') ;   // relay 1 (Power All Subsystems)
     GPIO_u8_WritePinDir(4,1,'d') ;   // relay 2 (Battery To Charger)
	 I2C_Master_Init();
	 USART_init();
	 LCD_Init();
	 ds3231_init();
	
	 
	 sei();
	 
	 rtc_t rtc_read,rtc;
	 temp_data *temp;
	 u8 y=0,true_comm =0,false_comm=0;
	 x=0;
	 DELAY_MS(500);
	 GPIO_u8_WritePinVal(3,1,'d') ;   // Start All Subsystems

	while(1)
	{
	    Low = ADCL;
		TenBitResult = ADCH<<8 | Low ;
		ADC_float =TenBitResult;
		Vin = ADC_float*5/1024 ;		
		if(Vin>=4.5)
			GPIO_u8_WritePinVal(4,0,'c');
		else
		    GPIO_u8_WritePinVal(4,1,'c');
		
		ADC_VoidRead();
		
		if (x==1)
		{		
			if(getCRC(command_rec,13)==command_rec[13])
			{
				/*The number of commands that delivered true*/
		         true_comm++;
				/*i received true command */
				SerialTransmit(ack,3);
				
				/* command for ADCS  : ADCS ID = 0x0A */
              if(command_rec[0]==0x0A)
              {
	              ADCS_command(command_rec,ADCS_comm,telemetry);
              }
			    /* commands for OBC : OBC ID = 0x2A */
			  if ( (command_rec[0]==0x2A) && (command_rec[1]==0x0F) )
              {
				 RTC_SetDisplay(command_rec,&rtc,&rtc_read);	 
			  }
		   }
		   
		   else
		   {
			   //ERROR
			   
			   SerialTransmit(Nack,3);
			   false_comm++;
			   if (false_comm==3)
			   {
				   Emergency_mode();
			   }
		   }
		    x=0;     
       }

	}
}
