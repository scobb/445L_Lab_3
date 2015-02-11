#ifndef _CLOCK_H_
#define _CLOCK_H_
#include <stdint.h>
extern volatile uint16_t time_seconds, time_minutes, time_hours;

void Clock_Init();


#endif