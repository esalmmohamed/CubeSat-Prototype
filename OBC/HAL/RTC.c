/*
 * RTC.c
 *
 * Created: 3/5/2020 2:07:17 AM
 *  Author: Ahmed
 */ 

#include "RTC.h"

void ds3231_init(void)
{
	I2C_Master_Init();
	I2C_Start(0xD0);
	I2C_Write(0x0E);//initial address of ds3231+0 to write next byte
	I2C_Stop();
	
}

void ds3231_set(rtc_t *rtc)
{
	rtc->hr &=0b00011111;//clear first three bits.
	rtc->hr |=0b01000000;//make D7=0,D6=1 for am/pm view,D5=0 for AM/1 for PM.
	rtc->hr |=(rtc->am_pm<<5);
	
	I2C_Start(0xD0);//initial address of ds3231+0 to write next byte
	twi_send(rtc->sec);//update second
	twi_send(rtc->min);//write location of control resistor
	twi_send(rtc->hr);//write 0 to the content of the resistor
	twi_send(rtc->day);//update day of the week
	twi_send(rtc->dt);//update date
	twi_send(rtc->mnth);//update month
	twi_send(rtc->yr);//update year
	I2C_Stop();
}




void ds3231_get(rtc_t *rtc)
{
	I2C_Start(0xD0);
	I2C_Write(0x00);
	I2C_Start(0xD1);
	
	rtc->sec=twi_receive(1);//1 to send ACK
	rtc->min=twi_receive(1);//1 to send ACK
	rtc->hr=twi_receive(1);//0 to send NACK
	rtc->day=twi_receive(1);//1 to send ACK,read day
	rtc->dt=twi_receive(1);//1 to send ACK,read date
    rtc->mnth=twi_receive(1);//1 to send ACK,read month
	rtc->yr=twi_receive(0);//0 to send NACK,read year

	I2C_Stop();
}

u8 BCDtoDEC(u8 bcd_val)
{
	return (((bcd_val >> 4) * 10) + (bcd_val & 0x0F));
}