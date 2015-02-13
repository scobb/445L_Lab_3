#include "Alarm.h"
#include "Timer1A.h"
#include "Timer0A.h"
#include "Clock.h"
#include "inc/tm4c123gh6pm.h"
#define E1 (*((volatile uint32_t *)0x40024008))

volatile uint16_t alarm_hours, alarm_minutes;
volatile uint8_t alarm_armed;
#define A_440 181818

void Alarm_Init(){
	// initialize a declawed timer
	Timer1A_Init(0, A_440);
	
	// initialize GPIO for speaker output
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x00000010;  // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTE_CR_R |= 0x02;           // allow changes to PE1
	GPIO_PORTE_DIR_R |= 0x02;        // make PE1 op
  GPIO_PORTE_AFSEL_R &= ~0x02;     // disable alt funct on PE1
  GPIO_PORTE_DEN_R |= 0x02;        // enable digital I/O on PE1
                                   // configure PE1 as GPIO
  GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFFFFF0F);
  GPIO_PORTE_AMSEL_R &= ~0x02;
	
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
	// toggle GPIO for speaker output
	E1 ^= 0x02;
	
}