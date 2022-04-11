/*
 * I2C.c
 *
 * Created: 3/13/2020 10:28:27 PM
 *  Author: Ahmed
 */ 

#include "I2C.h"

void I2C_Master_Init(void)
{
	
	TWSR=0x00; //set presca1er bits to zero
	TWBR=0x46; //SCL frequency is 50K for 16Mhz  // DEC= 70
	TWCR=0x04; //enab1e TWI module
}

u8 I2C_Master_T (u8 Sla_Addr , u8 data)
{
	u8 Ret_Val = 0 ;
	if (I2C_Start(Sla_Addr) == 1)			// SLA+W
	{
		if (I2C_Write(data) == 1)
		{
			Ret_Val = 1 ;
		}
		else if (I2C_Write(data) == 0)
		{
			Ret_Val = 0 ;
		}
		I2C_Stop();
	}
	return Ret_Val ;
}

u8 I2C_Master_R (u8 Sla_Addr , u8* Data_R)
{
	u8 Ret_Val = 0 ;

	if (I2C_Rep_Start(Sla_Addr +1) == 1)
	{
		I2C_Read(Data_R);
		I2C_Stop();
		Ret_Val = 1 ;
	}
	return Ret_Val ;
}

u8 I2C_Start(u8 Sla_Addr)
{
	u8 status = 0 ;
	u8 Ret_Val = 0;
	
	TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN)); /* Enable TWI, generate START */
	while (!(TWCR & (1<<TWINT))); /* Wait until TWI finish its current job */
	
	status = TWSR & (0xF8);		/* Read TWI status register */
	if(status==0x08)			/* Check weather START transmitted or not? */
	{
		TWDR = Sla_Addr;		/* Write SLA+W in TWI data register */
		TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI & clear interrupt flag */
		while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
		status = TWSR & (0xF8);		/* Read TWI status register */
		
		if(status==0x18)		/* Check for SLA+W transmitted & Ack received */
		Ret_Val = 1;			/* Return 1 to indicate Ack received */
		else if(status==0x20)		/* Check for SLA+W transmitted & Not_Aack received */
		Ret_Val = 0;			/* Return 2 to indicate Not_Aack received */
	}
	
	else
	Ret_Val = 0 ;/* Return 0 to indicate start condition fail */
	
	
	return Ret_Val;
}

u8 I2C_Rep_Start(u8 Sla_Addr_R) /* I2C repeated start function */
{
	
	uint8_t status;
	u8 Ret_Val = 0;
	
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);/* Enable TWI, generate start */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */

	if(status==(0x10))		/* Check for repeated start transmitted */
	{
		
		TWDR = Sla_Addr_R;		/* Write SLA+R in TWI data register */
		TWCR = (1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
		while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
		status = TWSR & (0xF8);		/* Read TWI status register */
		if(status==0x40)		/* Check for SLA+R transmitted & Ack received */
		Ret_Val =  1;			/* Return 1 to indicate Ack received */
		else if(status==0x48)		/* Check for SLA+R transmitted & Not_Ack received */
		Ret_Val = 2;			/* Return 2 to indicate Not_Ack received */
	}
	
	else
	Ret_Val = 0 ;/* Return 0 for repeated start condition fail */
	
	return Ret_Val ;
}

void I2C_Stop(void)
{
	TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWSTO));
	while(TWCR&(1<<TWSTO));	/* Wait until stop condition execution */
	_delay_ms(5) ; //wait for a short time
}

u8 I2C_Count = 0 ;
u8 I2C_LastVal[2] ;
u8 I2C_Write(u8 data)
{
	
	/*if (0 == I2C_Count)
	{
		I2C_LastVal[I2C_Count] = v_i2cData_u8 ;
		I2C_Count = 1 ;
	}
	else if (1 == I2C_Count)
	{
		I2C_LastVal[I2C_Count] = v_i2cData_u8 ;
		I2C_Count = 0 ;
	}
	*/
	u8 Ret_Val =0 ;
	u8 status ;
    TWDR = data ;
    TWCR = ((1<< TWINT) | (1<<TWEN));
    while (!(TWCR & (1 <<TWINT)));
	status= TWSR & (0xF8);		/* Read TWI status register */
	if(status==0x28)		/* Check for data transmitted & Ack received */
		Ret_Val = 1;			/* Return 1 to indicate Ack received */
	else if(status==0x30)		/* Check for data transmitted & Ack received */
	Ret_Val = 0;			/* Return 0 to indicate Not_Ack received */
	
	
	//LCD_DisplayNum(Ret_Val);
	return Ret_Val ;
	
	/*if (0x08u != TWSR)
	{
		if (0x20u == TWSR)
		{
			I2C_Stop();
			I2C_Start();
			I2C_Write(v_i2cData_u8);
		}
		if (0x30u == TWSR)
		{
			I2C_Stop();
			I2C_Start();
			I2C_Write(I2C_LastVal[I2C_Count]);
			I2C_Write(I2C_LastVal[I2C_Count]);
		}
		
	}*/
}

u8 I2C_Read(u8 v_ackOption_u8)
{
	u8 Ret_Val = 0 ;
	u8 status ;
	
	TWCR = ((1<< TWINT) | (1<<TWEN) | (v_ackOption_u8<<TWEA));
	while ( !(TWCR & (1 <<TWINT)));
	
	status= TWSR & (0xF8);		/* Read TWI status register */
	if(status==0x50)		/* Data byte has been received ACK has been returned */
	Ret_Val = 1;			/* Return 1 to indicate Ack received */
	else if(status==0x38)		/* Data byte has been received NOT ACK has been returned */
	Ret_Val = 0;			/* Return 0 to indicate Not_Ack received */
	
	return TWDR;
}
void I2C_Slave_Init(uint8_t slave_address)
{
	TWAR=slave_address;		/* Assign Address in TWI address register */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);/* Enable TWI, Enable ack generation */
}

int8_t I2C_Slave_Listen()
{
	while(1)
	{
		uint8_t status;			/* Declare variable */
		while(!(TWCR&(1<<TWINT)));	/* Wait to be addressed */
		status=TWSR&0xF8;		/* Read TWI status register */
		if(status==0x60||status==0x68)	/* Own SLA+W received &ack returned */
		return 0;			/* Return 0 to indicate ack returned */
		if(status==0xA8||status==0xB0)	/* Own SLA+R received &ack returned */
		return 1;			/* Return 0 to indicate ack returned */
		if(status==0x70||status==0x78)	/* General call received &ack returned */
		return 2;			/* Return 1 to indicate ack returned */
		else
		continue;			/* Else continue */
	}
}

int8_t I2C_Slave_Transmit(char data)
{
	uint8_t status;
	TWDR=data;			/* Write data to TWDR to be transmitted */
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);/* Enable TWI & clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	
	if(status==0xA0)		/* Check for STOP/REPEATED START received */
	{
		TWCR|=(1<<TWINT);	/* Clear interrupt flag & return -1 */
		return -1;
	}
	if(status==0xB8)		/* Check for data transmitted &ack received */
	return 0;			/* If yes then return 0 */
	if(status==0xC0)		/* Check for data transmitted &nack received */
	{
		TWCR|=(1<<TWINT);	/* Clear interrupt flag & return -2 */
		return -2;
	}
	if(status==0xC8)		/* Last byte transmitted with ack received */
	return -3;			/* If yes then return -3 */
	else			/* else return -4 */
	return -4;
}


char I2C_Slave_Receive()
{
	uint8_t status;		/* Declare variable */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);/* Enable TWI & generation of ack */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status==0x80||status==0x90)/* Check for data received &ack returned */
	return TWDR;		/* If yes then return received data */

	/* Check for data received, nack returned & switched to not addressed slave mode */
	if(status==0x88||status==0x98)
	return TWDR;		/* If yes then return received data */
	if(status==0xA0 || status==0x10 )		/* Check wether STOP/REPEATED START */
	{
		TWCR|=(1<<TWINT);	/* Clear interrupt flag & return -1 */
		return -1;
	}
	else
	return -2;			/* Else return -2 */
}

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
char I2C_Read_Ack()		/* I2C read ack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA); /* Enable TWI, generation of ack */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;			/* Return received data */
}

char I2C_Read_Nack()		/* I2C read nack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;		/* Return received data */
}