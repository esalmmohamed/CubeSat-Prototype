/*
 * STD_Fun.h
 *
 * Created: 27-Sep-18 08:01:54 AM
 *  Author: MO_Ali
 */ 


#ifndef _STD_FUN_H_
#define _STD_FUN_H_

#include "main.h"
#include <time.h>
#include <stdlib.h>

extern void ftoa(float n, char *res, int afterpoint) ;
extern uint16 Create_Random (void);
uint16 intToStr(uint16 x, uint8 str[], uint16 d);

#endif /* _STD_FUN_H_ */