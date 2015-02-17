#ifndef _CLOCK_H_
#define _CLOCK_H_
#include <stdint.h>
#define DIGITAL 0
#define MILITARY 1
#define ANALOG 2
extern volatile uint16_t time_seconds, time_minutes, time_hours, analog_display_hours;
extern volatile uint8_t display_mode, seconds_since_button_press;
#define CYCLES_PER_SECOND 80000000			// number of cycles per second @ 80MHz
#define SECONDS_PER_MINUTE 60
#define MINUTES_PER_HOUR 60
#define HOURS_PER_DAY 24
#define SECONDS_UNTIL_DISPLAY_RESET 10

void Clock_Init();
void Clock_setDisplayFunction(void(*task)(void));


#endif