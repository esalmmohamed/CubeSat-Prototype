/*
 * command.c
 *
 * Created: 2/28/2020 3:25:24 AM
 *  Author: Ahmed
 */ 

#include "command.h"

char getCRC(u8 *message,  u8 length)
{
	u8 j, crc = 0;
	u8 CRC7_POLY = 0x91;
	long aa = 1, c;
	char i;
	
	for (i = 1; i <  length; i++)
	{
		crc ^= message[i];

		for (j = 0; j < 8; j++)
		{
			c = (long)crc & aa;
			if (c > 0)
			crc ^= CRC7_POLY;
			crc >>= 1;
		}
	}
	return crc;

}


u8 Rotate_right(u8 *command,u8 *ADCS_Comm)
{     	
	u8 i,error_detection=0;
	for(i=0;i<6;i++)
	{
		ADCS_Comm[i]=command[i];
	}
	
	ADCS_Comm[6]=getCRC(ADCS_Comm,6);
	I2C_Start(0x40);
	for(i=0;i<=6;i++)
	{
		I2C_Write(ADCS_Comm[i]);
	}
	I2C_Rep_Start(0x41);
    error_detection=I2C_Read_Nack();
    I2C_Stop();
    return error_detection;
}


u8 Rotate_left(u8 *command,u8 *ADCS_Comm)
{  
	u8 i,error_detection=0;
	for(i=0;i<6;i++){
		ADCS_Comm[i]=command[i];
	}
	
	ADCS_Comm[6]=getCRC(ADCS_Comm,6);
	
	I2C_Start(0x40);
	for(i=0;i<=6;i++)
	{
		I2C_Write(ADCS_Comm[i]);
	}
	I2C_Rep_Start(0x41);
	error_detection=I2C_Read_Nack();
	I2C_Stop();
	return error_detection;
	
}

u8 get_angularvelocity(u8 *command,u8 *ADCS_Comm)
{	
	u8 i,error_detection=0;
	for(i=0;i<6;i++)
	{
		ADCS_Comm[i]=command[i];
	}
	
	ADCS_Comm[6]=getCRC(ADCS_Comm,6);
	
	
	I2C_Start(0x40);
	
	for(i=0;i<=6;i++)
	{
		I2C_Write(ADCS_Comm[i]);
	}
	I2C_Rep_Start(0x41);
	DELAY_MS(200);
	error_detection=I2C_Read_Nack();
	I2C_Stop();
	
	
	return error_detection;
	
}

/*
void get_temp(temp_data *temp,char *Temperature,u8 *_data) 
{
	u8 i;
	temp->telemetry =ADC_Read()	;
	sprintf(Temperature,"%d%cC  ",(int)temp->telemetry,0xdf); //convert integer value to ASCII string */
	/*LCD_Clear();
	LCD_String(Temperature);
	for(i=5;i<7;i++)
	{
	   _data[i]=temp->telemetry_byte[i];
		
	}
	SerialTransmit(_data,10); 
}

*/
void reset_buffer(u8 *command) {
	u8 i;
	for( i=0; i< 13; i++){
		
		command[i] =0;
    }
}
void ADCS_command(u8 *command,u8* ADCS_comm,u8 * telemetry)
{
	
	if(command[1]==0x0B)
	{
		if(command[2]==0x03)
		{
			// Rotate motor RIGHT
			LCD_Clear();
			sprintf(buffer, "%d    ",Rotate_right(command,ADCS_comm));/* Read & Ack of data */
			LCD_String_xy(1, 5, buffer);
		}
		
		else if(command[2]==0x04)
		{
			// Rotate motor Left
			LCD_Clear();
			sprintf(buffer, "%d    ",Rotate_left(command,ADCS_comm));/* Read & Ack of data */
			LCD_String_xy(0, 1, buffer);
		}
	}
	
	else if (command[1]==0x0c)
	{
		//get angular velocity
		LCD_Clear();
		sprintf(buffer, "%d    ",get_angularvelocity(command,ADCS_comm));
		LCD_String_xy(0, 5, buffer);
		DELAY_MS(2000);
		Read_telemetry(telemetry);
		SerialTransmit(telemetry,33);
/*		temp_data tel,x,y,z;
		z.telemetry_byte[0]=telemetry[4];
		z.telemetry_byte[1]=telemetry[5];
		z.telemetry_byte[2]=telemetry[6];
		z.telemetry_byte[3]=telemetry[7];
		x.telemetry_byte[0]=telemetry[8];
		x.telemetry_byte[1]=telemetry[9];
		x.telemetry_byte[2]=telemetry[10];
		x.telemetry_byte[3]=telemetry[11];
		y.telemetry_byte[0]=telemetry[12];
		y.telemetry_byte[1]=telemetry[13];
		y.telemetry_byte[2]=telemetry[14];
		y.telemetry_byte[3]=telemetry[15];
		tel.telemetry_byte[0]=telemetry[16];
		tel.telemetry_byte[1]=telemetry[17];
		tel.telemetry_byte[2]=telemetry[18];
		tel.telemetry_byte[3]=telemetry[19];
		LCD_String_xy(0,0,"z=");
		LCD_display_real_number(z.telemetry);
		LCD_String_xy(0,8,"y=");
		LCD_display_real_number(y.telemetry);
		LCD_String_xy(1,0,"x=");
		LCD_display_real_number(x.telemetry);
		LCD_String_xy(1,8,"T=");
		LCD_display_real_number(tel.telemetry);*/
	}
	
}

void Read_telemetry(u8 *telemetry)
{
	u8 i;
	I2C_Start(0x41);
	for (i=4;i<32;i++)
	{
		
		if (i<31)
		{
			telemetry[i]=I2C_Read_Ack();
		}
		else
		telemetry[i]=I2C_Read_Nack();
	}
	
	I2C_Stop();
	telemetry[32]=getCRC(telemetry,32);
	
}


void Emergency_mode(void)
{
  /*Restart all subsystems*/
   GPIO_u8_WritePinVal(1,0,'d');
   GPIO_u8_WritePinVal(2,0,'d');
   DELAY_MS(500);
   
   GPIO_u8_WritePinVal(1,1,'d');
   GPIO_u8_WritePinVal(2,1,'d');
   DELAY_MS(500);
}

void RTC_SetDisplay(u8 *command_rec,rtc_t *rtc,rtc_t *rtc_read)
{
	ds3231_init();
	LCD_Init();
	/* Replace with your application code */
	rtc->hr    =  command_rec[5] ;
	rtc->min   =  command_rec[6] ;
	rtc->sec   =  command_rec[7] ;
	rtc->am_pm =  command_rec[8] ;
	
	rtc->dt    =  command_rec[9] ;
	rtc->mnth  =  command_rec[10] ;
	rtc->yr    =  command_rec[11] ;
	rtc->day   =  command_rec[12] ;
	DELAY_MS(500) ;
	ds3231_set(&rtc);
	DELAY_MS(500);
	ds3231_get(&rtc_read);
	u8 hr_dec,min_dec,sec_dec,i;
	//fetch am/pm data into variable i
	i=rtc_read->hr;
	i&=0b00100000;
	i=(i>>5);
	rtc_read->hr&=0b00011111;

	//Convert BCD to Decimal
	
	hr_dec=BCDtoDEC (rtc_read->hr);
	min_dec=BCDtoDEC (rtc_read->min);
	sec_dec=BCDtoDEC (rtc_read->sec);
	LCD_Clear();
	LCD_display_number(BCDtoDEC(hr_dec));
	LCD_String(":");
	LCD_display_number(BCDtoDEC(min_dec));
	LCD_String(":");
	LCD_display_number(BCDtoDEC(sec_dec));
	//Print AM/PM
	
	
	if(i==0)
	{
		LCD_String(" AM");
	}
	else if(i==1)
	{
		LCD_String(" PM");
	}


	//Print Day

	
	switch(rtc_read->day)
	{
		case 1:
		LCD_String_xy(1,0,"Sun ");
		break;

		case 2:
		LCD_String_xy(1,0,"Mon ");
		break;

		case 3:
		LCD_String_xy(1,0,"Tue ");
		break;

		case 4:
		LCD_String_xy(1,0,"Wed ");
		break;

		case 5:
		LCD_String_xy(1,0,"Thu ");
		break;

		case 6:
		LCD_String_xy(1,0,"Fri ");
		break;

		case 7:
		LCD_String_xy(1,0,"Sat ");
		break;
	}

	//Print date,month,year

	
	LCD_display_number(BCDtoDEC(rtc_read->dt));
	LCD_String("/");
	LCD_display_number(BCDtoDEC(rtc_read->mnth));
	LCD_String("/");
	LCD_display_number(BCDtoDEC(rtc_read->yr));
	DELAY_MS(500);
}