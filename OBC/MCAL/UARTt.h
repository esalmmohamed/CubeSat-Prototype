/*
 * UARTt.h
 *
 * Created: 2/3/2020 11:42:34 PM
 *  Author: Ahmed
 */ 


#ifndef UARTT_H_
#define UARTT_H_

#include "Data_Types.h"
#define FCPU 16000000UL
#define BAUDRATE 38400
#define BAUD_PRESCALLER (((FCPU / (BAUDRATE * 16UL))) - 1)
//Declaration of our functions


void USART_init(void);
unsigned char USART_receive(void);
u8 UART_RxString(char *ptr_string);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
void SerialReceive(unsigned char *buffer, unsigned int max_size);
void SerialTransmit(unsigned char *buffer,unsigned int size);
void UART_transmitString(unsigned char * u8pStringCpy);


#endif /* UARTT_H_ */