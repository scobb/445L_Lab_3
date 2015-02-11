#include "DigitalDisplay.h"
#include "Clock.h"
#include "ST7735_SevenSeg.h"
#include "ST7735.h"

// define the pixels we'll use to display
#define HOUR_LEFT_X 13
#define HOUR_RIGHT_X 45
#define MINUTE_LEFT_X 85
#define MINUTE_RIGHT_X 113
#define ALL_Y 50
typedef struct{
	void(*left)(uint16_t, uint16_t);
	void(*right)(uint16_t, uint16_t);
} displayCell;

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

void enableDigitalDisplay(){
	Clock_setDisplayFunction(&displayDigital);
	drawColon(50, 64);
	(*minuteDisplayMap[time_minutes].left)(MINUTE_LEFT_X, ALL_Y);
	(*minuteDisplayMap[time_minutes].right)(MINUTE_RIGHT_X, ALL_Y);
	(*hourDisplayMap[time_hours].left)(HOUR_LEFT_X, ALL_Y);
	(*hourDisplayMap[time_hours].right)(HOUR_RIGHT_X, ALL_Y);
}
void displayDigital(){
	// draw spot 1
	static uint16_t my_minutes = -1;
	static uint16_t my_hours = -1;
		
	if (my_minutes != time_minutes){
		if (time_minutes % 10 == 0){
			(*minuteDisplayMap[time_minutes].left)(MINUTE_LEFT_X, ALL_Y);
		}
		(*minuteDisplayMap[time_minutes].right)(MINUTE_RIGHT_X, ALL_Y);
		my_minutes = time_minutes;
	}
	if (my_hours != time_hours){
		if (time_hours == 1 || time_hours == 10){
			if (hourDisplayMap[time_hours].left)
				(*hourDisplayMap[time_hours].left)(HOUR_LEFT_X, ALL_Y);
		}
		(*hourDisplayMap[time_hours].right)(HOUR_RIGHT_X, ALL_Y);
		my_hours = time_hours;
		
	}
	/*draw1(13, 50);
	draw2(45, 50);
	draw3(85, 50)
	draw4(113, 50);
	drawColon(50, 64);*/
}