/*
 * I2C_Slave_H_File.h
 *
 * Created: 3/13/2020 10:28:03 PM
 *  Author: Ahmed
 */ 


#ifndef I2C_H_
#define I2C_H_
#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>		/* Include inbuilt defined Delay header file */
#include <stdio.h>
#include "Data_Types.h"
#include "main.h"

void I2C_Master_Init(void);
u8 I2C_Master_T (u8 Sla_Addr , u8 data);
u8 I2C_Master_R (u8 Sla_Addr , u8* Data_R);
u8 I2C_Start(u8 Sla_Addr);
u8 I2C_Rep_Start(u8 Sla_Addr_R);
void I2C_Stop(void);
u8 I2C_Write(u8 data);
u8 I2C_Read(u8 v_ackOption_u8);
char I2C_Read_Nack();
char I2C_Read_Ack();
void I2C_Slave_Init(uint8 slave_address);
int8_t I2C_Slave_Listen();
int8_t I2C_Slave_Transmit(char data);
char I2C_Slave_Receive();
char getCRC(u8 *message,  u8 length);



#endif /* I2C_SLAVE_H_FILE_H_ */