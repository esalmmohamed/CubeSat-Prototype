/*
 * main.h
 *
 * Created: 09-Aug-18 06:21:26 AM
 *  Author: Mohamed Ali
 */ 


#ifndef _MAIN_H_
#define _MAIN_H_

#include "Std_Types.h"
#include "Std_Macros.h"
#include "Amit_Kit.h"
#include "Dio.h"
#include "Dio_Cfg.h"
#include "Data_Types.h"
#include "GPIO.h"
#include "MPU.h"

#include "Controller.h"
#include "LCD.h"
#include "STD_Fun.h"
#include "I2C.h"

typedef union
{
	float mpu_data;
	char mpu_data_byte[4];
} Telemetry;
	
#endif /* _MAIN_H_ */