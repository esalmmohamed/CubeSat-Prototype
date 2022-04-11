/*
 * slave.c
 *
 * Created: 3/13/2020 10:24:33 PM
 * Author : Ahmed
 */ 

#define F_CPU 8000000UL		/* Define CPU clock Frequency 8MHz */
	
#include <string.h>		/* Include string header file */
#include "LCD.h"	/* Include LCD header file */
#include "I2C.h"	/* Include I2C slave header file */
#include "motor.h"
#include "delay.h"
#include "motor.h"

#define Slave_Address		0x40
#define ACK                 0xAC
#define NACK                0xA0


int main(void)
{
	DDRD|=0x08;
	char arr[7]={0};
	int8_t replay=0,y=0,x=0,count = 0;
	LCD_lcd_init();
	motor_init();
	I2C_Slave_Init(Slave_Address);
	DELAY_MS(500);
	PORTD|=0x08;
	char telemetry_data[28];
	char send_telemetry=0;
	Telemetry Xg,Yg,Zg,t,Xa,Ya,Za;
	volatile int8_t Ack_status=0,telemetry=0,ack_ok=0;
	
	
	while (1)
	{
		if(I2C_Slave_Listen()==0)	/* Check for SLA+W or SLA+R */
		{
			do
			{
				if (y!=0)
				{
					arr[x]=count;
					x++;
					if (getCRC(arr,6)==arr[6])
					{
						replay=ACK;
						if(arr[2]==0x03)
						{
							set_speed_left(arr[5]);
							move_backward();
						}
						else if(arr[2]==0x04)
						{
							set_speed_left(arr[5]);
							move_forward();
						}
						else if(arr[2]==0x05)
						{
	                       telemetry=1;
						  
						}
					}
					else
					    replay=NACK;
				}
				y++;
				count = I2C_Slave_Receive();/* Receive data byte*/
				} while (count != -1);/* Receive until STOP/REPEATED START */
				count = 0;
				x=0;
				y=0;		
	    }
	 else if (I2C_Slave_Listen()==1)
	 {
		 
		 do
		 {
			 if (send_telemetry==1)
			 {
				static char i=0;
				 Ack_status = I2C_Slave_Transmit(telemetry_data[i]);
				 i++;
				// _delay_us(2);
				 if (i==28)
				 {
					 send_telemetry=0;
					 i=0;
				 }
			 }
			 
			 else 
			 {
				 Ack_status = I2C_Slave_Transmit(replay);/* Send data byte */
			 }
			 
		 } while (Ack_status == 0);/* Send until Ack is receive */
		     
			 if ( (telemetry==1))
			 {
		       ack_ok=1;
			 }
 
	 }
	 
     if ( (telemetry==1) && (ack_ok==1))
      {
			
			  I2C_Master_Init();		/* Initialize I2C */
			  telemetry=0,ack_ok=0;
			  Gyro_Init();		/* Initialize Gyro */
			  Read_RawValue();

			 /* Divide raw value by sensitivity scale factor */
			  Xa.mpu_data = Acc_x/16384.0;
			  Ya.mpu_data = Acc_y/16384.0;
			  Za.mpu_data = Acc_z/16384.0;
			
			  Xg.mpu_data = Gyro_x/16.4;
			  Yg.mpu_data = Gyro_y/16.4;
			  Zg.mpu_data = Gyro_z/16.4;

			  /* Convert temperature in /c using formula */
			  t.mpu_data = (Temperature/340.00)+36.53;
	
			 char i,j=0;
			 for (i=0;i<28;i++)
			 {			
					 if ((0<= i)&&(i <4))
					 {
						 telemetry_data[i]=Zg.mpu_data_byte[j];
					 }
					 
					 else if ((4<= i) &&(i<8))
					{
					    telemetry_data[i]=Xg.mpu_data_byte[j];
					 }
					 
					 else if ((8<= i)&&(i <12))
					 {
						 telemetry_data[i]=Yg.mpu_data_byte[j];
					 }
					 
					 else if ((12 <= i)&&(i<16) )
					 {
						 telemetry_data[i]=t.mpu_data_byte[j];
					 }
					 
					 else if ((16 <= i)&&(i<20) )
					 {
						 telemetry_data[i]=Xa.mpu_data_byte[j];
					 }
					 else if ((20 <= i)&&(i<24) )
					 {
						 telemetry_data[i]=Ya.mpu_data_byte[j];
					 }
					 else if ((24 <= i)&&(i<28) )
					 {
						 telemetry_data[i]=Za.mpu_data_byte[j];
					 }
				 j++;
				     if (j==4)
				    {
						j=0; 
				     }
			 }
			  send_telemetry=1;
			  I2C_Slave_Init(Slave_Address);
              
      }
    
	 
	}
 }