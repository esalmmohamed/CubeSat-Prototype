/*
 * command.h
 *
 * Created: 2/28/2020 3:25:09 AM
 *  Author: Ahmed
 */ 


#ifndef COMMAND_H_
#define COMMAND_H_

#include "Data_Types.h"
#include "I2C.h"
#include "ADC.h"
#include "LCD.h"
#include "RTC.h"

typedef union 
{
	float telemetry;
	u8 telemetry_byte[4];
	
}temp_data;

char buffer[10];
char getCRC(u8 *message,  u8 length);
u8 Rotate_right(u8 *command,u8 *ADCS_Comm);
u8 Rotate_left(u8 *command,u8 *ADCS_Comm);
u8 get_angularvelocity(u8 *command,u8 *ADCS_Comm);
void get_temp(temp_data *temp,char *Temperature,u8 *_data) ;
void reset_buffer(u8 *command);
void Emergency_mode(void);
void ADCS_command(u8 *command,u8* ADCS_comm,u8 * telemetry);
void Read_telemetry(u8 *telemetry);
void RTC_SetDisplay(u8 *command_rec,rtc_t *rtc,rtc_t *rtc_read);
#endif /* COMMAND_H_ */