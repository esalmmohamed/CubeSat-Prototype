/*
 * RTC.h
 *
 * Created: 3/5/2020 2:07:35 AM
 *  Author: Ahmed
 */ 


#ifndef RTC_H_
#define RTC_H_

#include<avr/io.h>
#include "delay.h"
#include "I2C.h"

typedef struct{
	u8 sec;
	u8 min;
	u8 hr;
	u8 day;
	u8 dt;
	u8 mnth;
	u8 yr;
	u8 am_pm;
}rtc_t;

void ds3231_init(void);
void ds3231_set(rtc_t *rtc);
void ds3231_get(rtc_t *rtc);
u8 BCDtoDEC(u8 bcd_val);


#endif




