#include "Alarm.h"
#include "Timer1A.h"
#include "Timer0A.h"
#include "Clock.h"

volatile uint16_t alarm_hours, alarm_minutes;
volatile uint8_t alarm_armed;
#define A_440 181818

void Alarm_Init(){
	// initialize a declawed timer
	Timer1A_Init(0, A_440);
	
	// TODO - initialize GPIO for speaker output
	
	// initialize alarm variables
	alarm_hours = 0;
	alarm_minutes = 0;
}
void soundAlarm(){
	// Has our 440Hz timer use this handler
	setTimer1AHandler(&alarmHandler);
}

void armAlarm() {
	// change shared variable
	alarm_armed = TRUE;
	
	// update Timer to use our check
	setAlarmCheck(&checkAlarm);
}
void disarmAlarm(){
	// this function should work for both stopping an alarm and preventing an alarm
	// changed shared variable
	alarm_armed = FALSE;
	
	// disable checking
	setAlarmCheck(0);
	
	// disable sounding
	setTimer1AHandler(0);
}
void checkAlarm(){
	// Check if alarm time is NOW
	if (time_hours == alarm_hours && time_minutes == alarm_minutes && time_seconds == 0){
		soundAlarm();
	}
}
void alarmHandler(){
	// TODO - toggle GPIO for speaker output
	
}