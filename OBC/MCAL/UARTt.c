/*
 * uart.c
 *
 * Created: 2/3/2020 11:41:49 PM
 *  Author: Ahmed
 */ 


/* Initialize USART */
#include "UARTt.h"
#include <avr/io.h>


void USART_init(void){
	UBRRH = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRRL = (uint8_t)(BAUD_PRESCALLER);
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	UCSRC = (1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL);
}

/* Function to receive byte/char */

unsigned char USART_receive(void){
	while(!(UCSRA & (1<<RXC)));
	return UDR;
}

/* Function to send byte/char */
void USART_send( unsigned char data){
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
}


/* Send string */




void USART_putstring(char* StringPtr){
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
	
}

u8 UART_RxString(char *ptr_string)
{
	char ch;
	u8 len = 0;
	while(1)
	{
		ch=USART_receive();    //Receive a char
		//UART_TxChar(ch);     //Echo back the received char

		if((ch=='\r') || (ch=='\n')) //read till enter key is pressed
		{						     //once enter key is pressed null terminate the string
			ptr_string[len]=0;           //and break the loop
			break;
		}
		else if((ch=='\b') && (len!=0))
		{
			len--;    //If backspace is pressed then decrement the index to remove the old char
		}
		else
		{
			ptr_string[len]=ch; //copy the char into string and increment the index
			len++;
		}
	}
	return len;
}

void SerialReceive(unsigned char *buffer, unsigned int max_size)
{
	unsigned int i;
	for(i=0; i< max_size;i++)
	{
		while (!(UCSRA& (1<<RXC))); // wait until data available in RX buffer
		buffer[i] = UDR;
	}
}

void SerialTransmit(unsigned char *buffer,unsigned int size)
{
	unsigned int i;
	for(i=0; i<size; i++)
	{
		while(!( UCSRA & (1 << UDRE))); // wait while TX is busy
		UDR = buffer[i]; // send single character to transmit buffer
	}
}