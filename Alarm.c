#include "Alarm.h"
#include "Timer0B.h"
volatile uint16_t alarm_hours, alarm_minutes;

void Alarm_Init(){
	Timer0B_Init();
	alarm_hours = 0;
	alarm_minutes = 0;
}
void soundAlarm(){
	setTimerBHandler(&alarmHandler);
}

void armAlarm() {
	
}
void disarmAlarm(){
	// disable checking
	
	
	// disable sounding - this function will work for both stopping an alarm and preventing an alarm
	setTimerBHandler(0);
}
void checkAlarm(){
	// TODO - this will be a method that TimerA0 will call 
}
void alarmHandler(){
	// TODO - interface with speaker
	// timer
}