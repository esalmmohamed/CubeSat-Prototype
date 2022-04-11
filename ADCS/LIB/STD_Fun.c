/*
 * STD_Fun.c
 *
 * Created: 25-Sep-18 11:45:39 PM
 *  Author: MO_Ali
 */ 
// reverses a string 'str' of length 'len'
#include "STD_Fun.h"

void reverse(char *str, int len)
{
	int i=0, j=len-1, temp;
	while (i<j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.

uint16 intToStr(uint16 x, uint8 str[], uint16 d)
{
	uint8 i = 0;
	while (x)
	{
		str[i++] = (x%10) + '0';
		x = x/10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
	// Extract integer part
	uint16 ipart = (uint16)n;
	
	// Extract floating part
	f32 fpart = n - (f32)ipart;
	
	// convert integer part to string
	uint16 i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0)
	{
		res[i] = '.';  // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter is needed
		// to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}
/*
uint16 Create_Random (void)
{
	/ *time_t t ;* /
	rtc_t rtc;
	RTC_GetDateTime(&rtc);	
	/ * Intializes random number generator * /
	uint16 Time = ((uint16)(rtc.sec) + (uint16)(rtc.min) + (uint16)(rtc.hour) 
				+ (uint16)(rtc.date) + (uint16)(rtc.month) + (uint16)(rtc.year) );
	srand(Time);
	return (rand()+1);
}*/