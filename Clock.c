#include "Clock.h"
#include "inc/tm4c123gh6pm.h"
#include "Timer0A.h"
#include "ButtonManager.h"

volatile uint16_t time_seconds, time_minutes, time_hours, analog_display_hours;
volatile uint8_t display_mode, seconds_since_button_press;
void check_reset(){
	++seconds_since_button_press;
	if (seconds_since_button_press > SECONDS_UNTIL_DISPLAY_RESET){
		resetToMainScreen();
	}
}

void increment_time(){
	++time_seconds;
	if (time_seconds >= SECONDS_PER_MINUTE){
		time_seconds = 0;
		++time_minutes;
		if (time_minutes >= MINUTES_PER_HOUR){
			time_minutes = 0;
			++time_hours;
			analog_display_hours = time_hours;
			if (time_hours >= HOURS_PER_DAY){
				time_hours=0;
			} else if (time_hours >= 12){
				analog_display_hours = time_hours - 12;
			}
		}
	}
	check_reset();
}
void debug_increment_time(){
	++time_minutes;
	if (time_minutes >= MINUTES_PER_HOUR){
		time_minutes = 0;
		++time_hours;
		analog_display_hours = time_hours;
		if (time_hours >= HOURS_PER_DAY){
			time_hours=0;
		} else if (time_hours >= 12){
			analog_display_hours = time_hours - 12;
		}
	}
	check_reset();
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
	Timer0A_Init(&increment_time, CYCLES_PER_SECOND);
	
}