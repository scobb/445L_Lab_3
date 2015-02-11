#ifndef _ALARM_H_
#define _ALARM_H_
#include <stdint.h>
extern volatile uint16_t alarm_hours, alarm_minutes;
// use this to arm the alarm--we begin checking every second to see if alarm time == time
void Alarm_Init();
void soundAlarm();
void armAlarm();
void disarmAlarm();
void checkAlarm();
void alarmHandler();

#endif