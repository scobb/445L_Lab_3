#ifndef _ALARM_H_
#define _ALARM_H_
#include <stdint.h>
#define TRUE 1
#define FALSE 0
extern volatile uint16_t alarm_hours, alarm_minutes;
extern volatile uint8_t alarm_armed;
// use this to arm the alarm--we begin checking every second to see if alarm time == time
void Alarm_Init();
void soundAlarm();
void armAlarm();
void disarmAlarm();
void checkAlarm();
void alarmHandler();

#endif