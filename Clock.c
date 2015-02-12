#include "Clock.h"
#include "inc/tm4c123gh6pm.h"
#include "Timer0A.h"
#define CYCLES_PER_SECOND 80000000			// number of cycles per second @ 80MHz
#define SECONDS_PER_MINUTE 60
#define MINUTES_PER_HOUR 60
#define HOURS_DISPLAYED 12
#define LED (*((volatile uint32_t *)0x40025038)) //Uses PF3-1. Looks like mainly for colors (1=red,2=blue,3=green)

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
	LED ^= 0x08;
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
	GPIO_PORTF_DIR_R |= 0x0E;        // make PF3-1 output (PF3-1 built-in LEDs)
  GPIO_PORTF_AFSEL_R &= ~0x0E;     // disable alt funct on PF3-1
  GPIO_PORTF_DEN_R |= 0x0E;        // enable digital I/O on PF3-1
                                   // configure PF3-1 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;
	
	LED = 0x00;
	
}