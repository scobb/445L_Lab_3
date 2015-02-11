#include "Clock.h"
#include "inc/tm4c123gh6pm.h"
#include "Timer0A.h"
#define CYCLES_PER_SECOND 80000000			// number of cycles per second @ 80MHz
#define SECONDS_PER_MINUTE 60
#define MINUTES_PER_HOUR 60
#define HOURS_DISPLAYED 12
volatile uint16_t time_seconds, time_minutes, time_hours;
volatile uint8_t display_mode;
void increment_time(){
	++time_seconds;
	if (time_seconds >= SECONDS_PER_MINUTE){
		time_seconds = 0;
		++time_minutes;
		if (time_minutes >= MINUTES_PER_HOUR){
			time_minutes = 0;
			++time_hours;
			if (time_hours >= HOURS_DISPLAYED){
				time_hours=0;
			}
		}
	}
}
void debug_increment_time(){
	++time_minutes;
	if (time_minutes >= MINUTES_PER_HOUR){
		time_minutes = 0;
		++time_hours;
		if (time_hours >= HOURS_DISPLAYED){
			time_hours=0;
		}
	}
}
void Clock_setDisplayFunction(void(*task)(void)){
	setHandler(task);
}
void Clock_Init(){
	
	// initialize time-keeping variables
	time_seconds = 0;
	time_minutes = 0;
	time_hours = 0;
	
	// set up timer 0A
  // Timer0A_Init(&increment_time, CYCLES_PER_SECOND);
	Timer0A_Init(&debug_increment_time, CYCLES_PER_SECOND);
	
}