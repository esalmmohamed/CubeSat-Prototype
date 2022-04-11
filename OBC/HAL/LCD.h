/*
 * LCD.h
 *
 * Created: 2/5/2020 4:22:21 PM
 *  Author: Ahmed
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>			/* Include AVR std. library file */
#include "delay.h"
#include <stdio.h>
#define LCD_Dir  DDRA			/* Define LCD data port direction */
#define LCD_Port PORTA			/* Define LCD data port */
#define RS PA1				/* Define Register Select pin */
#define EN PA2 				/* Define Enable signal pin */

void LCD_Command( unsigned char cmnd );
void LCD_Char( unsigned char data );
void LCD_Init (void);
void LCD_String (char *str);
void LCD_String_xy (char row, char pos, char *str);
void LCD_Clear();
void LCD_display_number (int Number );
void LCD_display_real_number (double  Number );


#endif /* LCD_H_ */