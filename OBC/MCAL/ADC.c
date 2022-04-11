/*
 * ADC.c
 *
 * Created: 8/18/2020 12:11:55 PM
 *  Author: Ahmed
 */ 

#include "ADC.h"
#include <avr/io.h>

void ADC_VoidInit(void)
{
	ADCSRA |=(1<< ADPS2)|(1<< ADPS1)|(1<< ADPS0);    // Setting 16 Prescalar
	ADMUX |=(1<<REFS0);      // Setting AVcc as reference voltage
	ADCSRA |=(1<< ADIE);     // ADC interrupt enable
	ADCSRA |=(1<<ADEN);      // ADC Enable
}

void ADC_VoidRead(void)
{
	ADCSRA |=(1<< ADSC);
}
