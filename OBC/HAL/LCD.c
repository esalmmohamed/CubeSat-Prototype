/*
 * LCD.c
 *
 * Created: 2/5/2020 4:22:41 PM
 *  Author: Ahmed
 */ 


#include "LCD.h"


void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x07) | ((cmnd>>1) & 0x78); /* sending upper nibble */
	LCD_Port &= ~ (1<<RS);		/* RS=0, command reg. */
	LCD_Port |= (1<<EN);		/* Enable pulse */
	DELAY_uS(1);
	LCD_Port &= ~ (1<<EN);

	DELAY_uS(200);

	LCD_Port = (LCD_Port & 0x07) | (cmnd << 3);  /* sending lower nibble */
	LCD_Port |= (1<<EN);
	DELAY_uS(1);
	LCD_Port &= ~ (1<<EN);
	DELAY_MS(2);
}


void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x07) | ((data>>1) & 0x78); /* sending upper nibble */
	LCD_Port |= (1<<RS);		/* RS=1, data reg. */
	LCD_Port|= (1<<EN);
	DELAY_uS(1);
	LCD_Port &= ~ (1<<EN);

	DELAY_uS(200);

	LCD_Port = (LCD_Port & 0x07) | (data << 3); /* sending lower nibble */
	LCD_Port |= (1<<EN);
	DELAY_uS(1);
	LCD_Port &= ~ (1<<EN);
	DELAY_MS(2);
}

void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Dir = 0xFF;			/* Make LCD port direction as o/p */
	DELAY_MS(20);			/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);              /* Display on cursor off*/
	LCD_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);              /* Clear display screen*/
	DELAY_MS(2);
}


void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* Clear display */
	DELAY_MS(2);
	LCD_Command (0x80);		/* Cursor at home position */
}

void LCD_display_number (int Number )
{

	char  str[7];

	sprintf(str,"%d",Number);	// Adjust the formatting to your liking.

	LCD_String (str) ;

}

void LCD_display_real_number (double  Number )
{

	char  str[16];


	char *tmpSign = (Number < 0) ? "-" : "";
	float tmpVal = (Number < 0) ? -Number : Number;

	int tmpInt1 = tmpVal;                  // Get the integer (678).
	float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
	int tmpInt2 = tmpFrac * 10000;  // Turn into integer (123).

	// Print as parts, note that you need 0-padding for fractional bit.

	sprintf (str, "%s%d.%04d", tmpSign, tmpInt1, tmpInt2);



	LCD_String (str) ;


}
