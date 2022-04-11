/*
 * I2C.c
 *
 * Created: 05-Oct-18 11:13:50 AM
 *  Author: MO_Ali
 */ 

#include "I2C.h"
/***************************************************************************************************
                         void I2C_Init()
 ****************************************************************************************************
 * I/P Arguments: none.
 * Return value	: none

 * description :This function is used to initialize the I2C module
------------------------------------------------------------------------------------*/
void I2C_Master_Init(void)
{
	
		TWSR=0x00; //set presca1er bits to zero
		TWBR=0x46; //SCL frequency is 50K for 16Mhz  // DEC= 70
		TWCR=0x04; //enab1e TWI module
}
void I2C_Slave_Init(u8 Slave_Addr)
{
	TWAR=Slave_Addr;					/* Assign Address in TWI address register */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);/* Enable TWI, Enable Ack generation */
}
/************** Master Functions *****************/
/************************************************/
//Master Transmitter Mode
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

//Master Receiver Mode

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

/************************************************/

/************* SLave Functions *******************/
u8 I2C_Slave_Listen(void)
{
	
		u8 Ret_Val = 0xff ;
		u8 status;			        /* Declare variable */
		while(!(TWCR&(1<<TWINT)));	    /* Wait to be addressed */
		status = TWSR & (0xF8);		    /* Read TWI status register */
		
		if(status==0x60||status==0x68)			/* Own SLA+W received & Ack returned */
			Ret_Val = (0);						/* Return 0 to indicate Ack returned */
		 if(status==0xA8||status==0xB0)		/* Own SLA+R received & Ack returned */
			Ret_Val =  (1);						/* Return 1 to indicate Ack returned */
		if(status==0x70||status==0x78)		/* General call received & Ack returned */
			Ret_Val =  (2);						/* Return 2 to indicate Ack returned */
		
		return Ret_Val;
	
	
}
u8 I2C_Slave_Transmit(u8 data)
{
	u8 Ret_Val =0 ;
	uint8_t status;
	
	TWDR=data;			/* Write data to TWDR to be transmitted */
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);/* Enable TWI & clear interrupt flag */
	
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status = TWSR & (0xF8);		/* Read TWI status register */
	
	if(status==0xB8)		/* Check for data transmitted & Ack received */
		Ret_Val = 1;
	
	else if(status==0xC0)		/* Check for data transmitted & Not_Ack received */
	{
		TWCR|=(1<<TWINT);
		Ret_Val = 2;
	}
	
	 else if(status==0xC8)		/* Last byte transmitted with Ack received */
		Ret_Val = 3;
		
	else if(status==0xA0)		/* Check for STOP/REPEATED START received */
	{
		TWCR|=(1<<TWINT);	/* Clear interrupt flag & return -1 */
		Ret_Val =  4;
	}
	
			
	return Ret_Val;
}

u8 I2C_Slave_Receive(u8* Data)
{
	u8 Ret_Val =0 ;
	u8 status;						/* Declare variable */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);/* Enable TWI & generation of Ack */
	
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status = TWSR & (0xF8);		/* Read TWI status register */
	*Data = TWDR;
	if(status==0x60||status==0x70)/* Own SLA+W has been received; ACK has been returned */
	{
		Ret_Val = 1;
	}
	
	 else if(status==0x68||status==0x78)/* Arbitration lost in SLA+R/W asmaster; own SLA+W has been received; ACK has been returned */
	{
		Ret_Val = 2;
	}
	
	 else if(status==0x80||status==0x90)/* Check for data received &Ack returned */
	{
		Ret_Val = 1;	
		
	}
	/* Check for data received, Not_Ack returned & switched to not addressed slave mode */
	 else if(status==0x88||status==0x98)
	{
		//Do Something
		Ret_Val = 4;
	}
	 else if(status==0xA0)		/* Check whether STOP/REPEATED START */
	{
		TWCR|=(1<<TWINT);		/* Clear interrupt flag */
		Ret_Val = 5;
	}
	
	return Ret_Val;
}
/*********************************************/


/***************************************************************************************************
                         void I2C_Start()
 ****************************************************************************************************
 
 * description  :This Function Makes Start bit & make sure that the Slave address is Exist

***************************************************************************************************/
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


/***************************************************************************************************
                         void I2C_RStart()
 ****************************************************************************************************
 
 * description  :This Function Makes Start bit (for reading) & make sure that the Slave address is Exist

***************************************************************************************************/

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




/***************************************************************************************************
                         void I2C_Stop()
 ****************************************************************************************************

 * description  :This function is used to generate I2C Stop Condition.
                 Stop Condition: SDA goes High when SCL is High.
 ***************************************************************************************************/

void I2C_Stop(void)
{
    TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWSTO));
	 while(TWCR&(1<<TWSTO));	/* Wait until stop condition execution */
    DELAY_MS(5) ; //wait for a short time
}




/***************************************************************************************************
                         void I2C_Write(uint8_t v_i2cData_u8)
 ****************************************************************************************************
 * I/P Arguments: uint8-->8bit data to be sent.
 * Return value  : none

 * description  :This function is used to send a byte on SDA line using I2C protocol
                 8bit data is sent bit-by-bit on each clock cycle.
                 MSB(bit) is sent first and LSB(bit) is sent at last.
                 Data is sent when SCL is low.

 ***************************************************************************************************/
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






/***************************************************************************************************
                         uint8_t I2C_Read(uint8_t v_ackOption_u8)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value  : uint8_t(received byte)

 * description :This fun is used to receive a byte on SDA line using I2C protocol.
               8bit data is received bit-by-bit each clock and finally packed into Byte.
               MSB(bit) is received first and LSB(bit) is received at last.
			   
***************************************************************************************************/
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

char I2C_Read_Nack()		/* I2C read nack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;		/* Return received data */
}

char I2C_Read_Ack()		/* I2C read ack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA); /* Enable TWI, generation of ack */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;			/* Return received data */
}

u8 I2C_Repeated_Start(char read_address) /* I2C repeated start function */
{
	uint8_t status;		/* Declare variable */
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);/* Enable TWI, generate start */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status!=0x10)		/* Check for repeated start transmitted */
	return 0;			/* Return 0 for repeated start condition fail */
	TWDR=read_address;		/* Write SLA+R in TWI data register */
	TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status==0x40)		/* Check for SLA+R transmitted &ack received */
	return 1;			/* Return 1 to indicate ack received */
	if(status==0x48)		/* Check for SLA+R transmitted &nack received */
	return 2;			/* Return 2 to indicate nack received */
	else
	return 3;			/* Else return 3 to indicate SLA+W failed */
}

void twi_init(void)
{
	TWSR=0x00;
	TWBR=0x0F;//see how to configure clock freq of twi.here the freq for 16mhz xtal is nearabout 347khz
	TWCR =(1<<TWEN);//enable TWEN bit
}

void twi_start(void)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//TWINT=0 by writting one to it,TWSTA bit=1,TWEN bit=1 to transmit start condition when the bus is free.
	while((TWCR & (1<<TWINT))==0);//stay here and poll until TWINT becomes 1 at the end of transmit.
}

void twi_stop(void)
{
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//TWINT=0 by writting one to it,TWSTO bit=1,TWEN bit=1 to transmit stOP condition.
}

void twi_send(u8 data)
{
	TWDR=data;//send the data to TWDR resistor
	TWCR=(1<<TWINT)|(1<<TWEN);//TWINT=0 by writting one to it,TWEN bit=1 to enable twi module
	while((TWCR & (1<<TWINT))==0);//stay here and poll until TWINT becomes 1 at the end of transmit.
}

u8 twi_receive(u8 ack_val)
{
	TWCR=(1<<TWINT)|(1<<TWEN)|(ack_val<<TWEA);//if we want to receive more than one byte,we will send 1 as ack_val to send an acknowledge.At the time of last by te,we will send 0 as ack_val to send NACK.
	while((TWCR & (1<<TWINT))==0);//stay here and poll until TWINT becomes 1 at the end of transmit.
	return TWDR;
}


