#ifndef _CLOCK_H_
#define _CLOCK_H_
#include <stdint.h>
extern volatile uint16_t time_seconds, time_minutes, time_hours;

void Clock_Init();
void Clock_setDisplayFunction(void(*task)(void));


#endif