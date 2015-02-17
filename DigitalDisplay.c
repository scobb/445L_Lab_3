#include "DigitalDisplay.h"
#include "Clock.h"
#include "ST7735_SevenSeg.h"
#include "ST7735.h"
#include "Alarm.h"

// define the pixels we'll use to display
#define HOUR_LEFT_X 13
#define HOUR_RIGHT_X 45
#define MINUTE_LEFT_X 85
#define MINUTE_RIGHT_X 113
#define COLON_X 50
#define COLON_Y 64
#define MILITARY_HOUR_LEFT_X 32
#define MILITARY_HOUR_RIGHT_X 62
#define MILITARY_MINUTE_LEFT_X 92
#define MILITARY_MINUTE_RIGHT_X 122
#define ALL_Y 50
typedef struct{
	void(*left)(uint16_t, uint16_t);
	void(*right)(uint16_t, uint16_t);
} displayCell;

displayCell militaryHourDisplayMap[] = {
	{&draw0, &draw0},
	{&draw0, &draw1},
	{&draw0, &draw2},
	{&draw0, &draw3},
	{&draw0, &draw4},
	{&draw0, &draw5},
	{&draw0, &draw6},
	{&draw0, &draw7},
	{&draw0, &draw8},
	{&draw0, &draw9},
	{&draw1, &draw0},
	{&draw1, &draw1},
	{&draw1, &draw2},
	{&draw1, &draw3},
	{&draw1, &draw4},
	{&draw1, &draw5},
	{&draw1, &draw6},
	{&draw1, &draw7},
	{&draw1, &draw8},
	{&draw1, &draw9},
	{&draw2, &draw0},
	{&draw2, &draw1},
	{&draw2, &draw2},
	{&draw2, &draw3}
};
displayCell hourDisplayMap[] = {
	{&draw1, &draw2},
	{&drawBlank, &draw1},
	{&drawBlank, &draw2},
	{&drawBlank, &draw3},
	{&drawBlank, &draw4},
	{&drawBlank, &draw5},
	{&drawBlank, &draw6},
	{&drawBlank, &draw7},
	{&drawBlank, &draw8},
	{&drawBlank, &draw9},
	{&draw1, &draw0},
	{&draw1, &draw1},
};
displayCell minuteDisplayMap[] = {
	{&draw0, &draw0}, {&draw0, &draw1}, {&draw0, &draw2}, {&draw0, &draw3}, {&draw0, &draw4},
	{&draw0, &draw5}, {&draw0, &draw6}, {&draw0, &draw7}, {&draw0, &draw8}, {&draw0, &draw9},
	{&draw1, &draw0}, {&draw1, &draw1}, {&draw1, &draw2}, {&draw1, &draw3}, {&draw1, &draw4},
	{&draw1, &draw5}, {&draw1, &draw6}, {&draw1, &draw7}, {&draw1, &draw8}, {&draw1, &draw9},
	{&draw2, &draw0}, {&draw2, &draw1}, {&draw2, &draw2}, {&draw2, &draw3}, {&draw2, &draw4},
	{&draw2, &draw5}, {&draw2, &draw6}, {&draw2, &draw7}, {&draw2, &draw8}, {&draw2, &draw9},
	{&draw3, &draw0}, {&draw3, &draw1}, {&draw3, &draw2}, {&draw3, &draw3}, {&draw3, &draw4},
	{&draw3, &draw5}, {&draw3, &draw6}, {&draw3, &draw7}, {&draw3, &draw8}, {&draw3, &draw9},
	{&draw4, &draw0}, {&draw4, &draw1}, {&draw4, &draw2}, {&draw4, &draw3}, {&draw4, &draw4},
	{&draw4, &draw5}, {&draw4, &draw6}, {&draw4, &draw7}, {&draw4, &draw8}, {&draw4, &draw9},
	{&draw5, &draw0}, {&draw5, &draw1}, {&draw5, &draw2}, {&draw5, &draw3}, {&draw5, &draw4},
	{&draw5, &draw5}, {&draw5, &draw6}, {&draw5, &draw7}, {&draw5, &draw8}, {&draw5, &draw9},
};

void displayCurrentTimeDigital(){
	// draws the full time regardless of previous time-- for use initializations
	uint8_t display_hours = time_hours;
	(*minuteDisplayMap[time_minutes].left)(MINUTE_LEFT_X, ALL_Y);
	(*minuteDisplayMap[time_minutes].right)(MINUTE_RIGHT_X, ALL_Y);
	if (display_hours >= 12)
		display_hours -= 12;
	(*hourDisplayMap[display_hours].left)(HOUR_LEFT_X, ALL_Y);
	(*hourDisplayMap[display_hours].right)(HOUR_RIGHT_X, ALL_Y);
	drawColon(COLON_X, COLON_Y);
}
void displayCurrentTimeMilitary(){
	(*minuteDisplayMap[time_minutes].left)(MILITARY_MINUTE_LEFT_X, ALL_Y);
	(*minuteDisplayMap[time_minutes].right)(MILITARY_MINUTE_RIGHT_X, ALL_Y);
	(*militaryHourDisplayMap[time_hours].left)(MILITARY_HOUR_LEFT_X, ALL_Y);
	(*militaryHourDisplayMap[time_hours].right)(MILITARY_HOUR_RIGHT_X, ALL_Y);
}
void displayCurrentAlarmTimeDigital(){
	// draws the full time regardless of previous time-- for use initializations
	uint8_t display_hours = alarm_hours;
	(*minuteDisplayMap[alarm_minutes].left)(MINUTE_LEFT_X, ALL_Y);
	(*minuteDisplayMap[alarm_minutes].right)(MINUTE_RIGHT_X, ALL_Y);
	if (display_hours >= 12)
		display_hours -= 12;
	(*hourDisplayMap[display_hours].left)(HOUR_LEFT_X, ALL_Y);
	(*hourDisplayMap[display_hours].right)(HOUR_RIGHT_X, ALL_Y);
	drawColon(COLON_X, COLON_Y);
}
void displayCurrentAlarmTimeMilitary(){
	(*minuteDisplayMap[alarm_minutes].left)(MILITARY_MINUTE_LEFT_X, ALL_Y);
	(*minuteDisplayMap[alarm_minutes].right)(MILITARY_MINUTE_RIGHT_X, ALL_Y);
	(*militaryHourDisplayMap[alarm_hours].left)(MILITARY_HOUR_LEFT_X, ALL_Y);
	(*militaryHourDisplayMap[alarm_hours].right)(MILITARY_HOUR_RIGHT_X, ALL_Y);
}
void enableMilitaryDisplay(){
	display_mode = MILITARY;
	Clock_setDisplayFunction(&displayMilitary);
	displayCurrentTimeMilitary();
}

void enableDigitalDisplay(){
	display_mode = DIGITAL;
	Clock_setDisplayFunction(&displayDigital);
	displayCurrentTimeDigital();
}

uint16_t redraw_alarm_minutes(uint8_t left, uint8_t right, uint16_t my_minutes){
	if (my_minutes != alarm_minutes){
		if (alarm_minutes % 10 == 0 || alarm_minutes % 10 == 9)
			(*minuteDisplayMap[alarm_minutes].left)(left, ALL_Y);
		(*minuteDisplayMap[alarm_minutes].right)(right, ALL_Y);
		return alarm_minutes;
	}
	return my_minutes;
}
uint16_t redraw_alarm_hours(uint8_t left, uint8_t right, uint16_t my_hours, uint8_t is_military){
	// are we in 12 hour time?
	uint8_t drawn_hours = alarm_hours;
	displayCell* myMap;
	if (!is_military) {
		if (alarm_hours >= 12){
			drawn_hours = alarm_hours - 12;
		}
		myMap = hourDisplayMap;
	} else {
		myMap = militaryHourDisplayMap;
	}
	if (my_hours != drawn_hours){
		(*myMap[drawn_hours].left)(left, ALL_Y);
		(*myMap[drawn_hours].right)(right, ALL_Y);
		return drawn_hours;
	}
	return my_hours;
}
void displayAlarmMilitary(){
	// draw spot 1
	static uint16_t my_minutes = -1;
	static uint16_t my_hours = -1;
	my_minutes = redraw_alarm_minutes(MILITARY_MINUTE_LEFT_X, MILITARY_MINUTE_RIGHT_X, my_minutes);
	my_hours = redraw_alarm_hours(MILITARY_HOUR_LEFT_X, MILITARY_HOUR_RIGHT_X, my_hours, TRUE);
}
void displayAlarmDigital(){
	// draw spot 1
	static uint16_t my_minutes = -1;
	static uint16_t my_hours = -1;
	
	my_minutes = redraw_alarm_minutes(MINUTE_LEFT_X, MINUTE_RIGHT_X, my_minutes);
  my_hours = redraw_alarm_hours(HOUR_LEFT_X, HOUR_RIGHT_X, my_hours, FALSE);
	
}

uint16_t redraw_minutes(uint8_t left, uint8_t right, uint16_t my_minutes){
	if (my_minutes != time_minutes){
		if (time_minutes % 10 == 0 || time_minutes % 10 == 9){
			(*minuteDisplayMap[time_minutes].left)(left, ALL_Y);
		}
		(*minuteDisplayMap[time_minutes].right)(right, ALL_Y);
		return time_minutes;
	}
	return my_minutes;
}

uint16_t redraw_hours(uint8_t left, uint8_t right, uint16_t my_hours, uint8_t is_military){
	// are we in 12 hour time?
	uint8_t drawn_hours = time_hours;
	displayCell* myMap;
	if (!is_military) {
		if (time_hours >= 12){
			drawn_hours = time_hours - 12;
		}
		myMap = hourDisplayMap;
	} else {
		myMap = militaryHourDisplayMap;
	}
	if (my_hours != drawn_hours){
		(*myMap[drawn_hours].left)(left, ALL_Y);
		(*myMap[drawn_hours].right)(right, ALL_Y);
		return drawn_hours;
	}
	return my_hours;
}

void displayMilitary(){
	static uint16_t my_minutes = -1;
	static uint16_t my_hours = -1;
	
	my_minutes = redraw_minutes(MILITARY_MINUTE_LEFT_X, MILITARY_MINUTE_RIGHT_X, my_minutes);
	my_hours = redraw_hours(MILITARY_HOUR_LEFT_X, MILITARY_HOUR_RIGHT_X, my_hours, TRUE);
}
	
void displayDigital(){
	static uint16_t my_minutes = -1;
	static uint16_t my_hours = -1;
	
	my_minutes = redraw_minutes(MINUTE_LEFT_X, MINUTE_RIGHT_X, my_minutes);
	my_hours = redraw_hours(HOUR_LEFT_X, HOUR_RIGHT_X, my_hours, FALSE);
}