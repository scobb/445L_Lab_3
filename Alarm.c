#include "Alarm.h"
#include "Timer1A.h"
#include "Timer0A.h"
volatile uint16_t alarm_hours, alarm_minutes;
#define A_440 181818

void Alarm_Init(){
	Timer1A_Init(&alarmHandler, A_440);
	alarm_hours = 0;
	alarm_minutes = 0;
}
void soundAlarm(){
	setTimer1AHandler(&alarmHandler);
}

void armAlarm() {
	
}
void disarmAlarm(){
	// disable checking
	setAlarmCheck(0);
	
	// disable sounding - this function will work for both stopping an alarm and preventing an alarm
	setTimer1AHandler(0);
}
void checkAlarm(){
	// TODO - this will be a method that TimerA0 will call 
}
void alarmHandler(){
	int bob = 5;
	// printf("hi\n");
	// TODO - interface with speaker
	// timer
}