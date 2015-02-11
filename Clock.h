#ifndef _CLOCK_H_
#define _CLOCK_H_
#include <stdint.h>
#define DIGITAL 0
#define ANALOG 1
extern volatile uint16_t time_seconds, time_minutes, time_hours;
extern volatile uint8_t display_mode;

void Clock_Init();
void Clock_setDisplayFunction(void(*task)(void));


#endif