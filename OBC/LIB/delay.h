/*
 * delay.h
 *
 * Created: 2/5/2020 4:42:55 PM
 *  Author: Ahmed
 */ 


#ifndef DELAY_H_
#define DELAY_H_

#include "Data_Types.h"


// the "NOP" takes 1 Cycle only

#define clock   16000000
#define error_ratio 30
u32 DelayCount;

#define DELAY_MS(x) \
for( DelayCount = ((clock/1000)*x/error_ratio); DelayCount >0; DelayCount--)\
{\
	__asm("NOP");\
}


#define DELAY_uS(x) DELAY_MS((x/1000))




#endif /* DELAY_H_ */