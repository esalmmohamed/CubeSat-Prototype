/*
 * I2C.h
 *
 * Created: 05-Oct-18 11:14:04 AM
 *  Author: MO_Ali
 */ 


#ifndef _I2C_H_
#define _I2C_H_

#include <avr/io.h>
#include "Data_Types.h"
#include "delay.h"

typedef enum {Master , Slave}I2C_Typ ;
/***************************************************************************************************
                             Function Prototypes
***************************************************************************************************/
void I2C_Master_Init(void);
void I2C_Slave_Init(u8 Slave_Addr);

u8 I2C_Master_T (u8 Sla_Addr , u8 data);
u8 I2C_Master_R (u8 Sla_Addr , u8* Data_R);

u8 I2C_Slave_Listen(void);
u8 I2C_Slave_Transmit(u8 data);
u8 I2C_Slave_Receive(u8* Data);

u8 I2C_Start(u8 Sla_Addr);
u8 I2C_Rep_Start(u8 Sla_Addr_R);
void I2C_Stop(void);
u8 I2C_Write(u8 data);
u8 I2C_Read(u8 v_ackOption_u8);
char I2C_Read_Nack();
char I2C_Read_Ack();
u8 I2C_Repeated_Start(char read_address);
void twi_send(u8 data);
u8 twi_receive(u8 ack_val);
void twi_stop(void);
void twi_start(void);
void twi_init(void);
/**************************************************************************************************/



#endif /* _I2C_H_ */