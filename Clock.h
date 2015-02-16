#ifndef _CLOCK_H_
#define _CLOCK_H_
#include <stdint.h>
#define DIGITAL 0
#define MILITARY 1
#define ANALOG 2
extern volatile uint16_t time_seconds, time_minutes, time_hours;
extern volatile uint8_t display_mode;

void Clock_Init();
void Clock_setDisplayFunction(void(*task)(void));


#endif