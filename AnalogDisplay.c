#include "Clock.h"
#include "ST7735.h"
#include "ST7735_Constants.h"
#include "ST7735_Line.h"
#include "inc/tm4c123gh6pm.h"
#include "AnalogDisplay.h"

#define CENTER_X 64
#define CENTER_Y 64
#define MINUTES_PER_HOUR 60

//drawDashes - 1 means draw them, 0 means don't
//numToDraw - 0 means draw all numbers, 1-12 means draw that number
void drawClock(uint8_t drawDashes, uint8_t numToDraw) {
	ST7735_SetCursor(0, 0);
	//This will draw all of the clock dashes
	if (drawDashes == 1){
		for (int i = 0; i < 24; i+=2){
			ST7735_Line(analogDashX[i], analogDashY[i], analogDashX[i+1], analogDashY[i+1], ST7735_Color565(255, 255, 255));
		}
	}
	
	if (numToDraw == 0){
		for (int j = 1; j <= 12; j++){
			ST7735_SetCursor(analogNumX[j], analogNumY[j]);
			printf("%d", j);
		}
	}
	else {
		ST7735_SetCursor(analogNumX[numToDraw-1], analogNumY[numToDraw-1]);
		printf("%d", numToDraw);
	}
}

void analogTime() {
	static uint16_t my_minutes = -1;
	static uint16_t my_hours = -1;
	static uint16_t my_hourIncrementer = 0;
	
	//Before we do anything, let's check to see if the hour/minute hands are currently overlapping
	if (my_hourIncrementer == my_minutes){
		ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer], yValsH[my_hourIncrementer], ST7735_Color565(0, 0, 0));
	}
	if (my_minutes != time_minutes){
		if (time_minutes % 5 == 0){
			drawClock(0, time_minutes/5);
		}
		//Draws a black line over the previous value and a green line on the next minute
		ST7735_Line(CENTER_X, CENTER_Y, xVals[time_minutes-1], yVals[time_minutes-1], ST7735_Color565(0, 0, 0));
		ST7735_Line(CENTER_X, CENTER_Y, xVals[time_minutes], yVals[time_minutes], ST7735_Color565(0, 255, 0));
		my_minutes = time_minutes;
	}
	
	if (my_hours != time_hours){
		if (time_minutes % 12 == 0){
			my_hourIncrementer = my_hourIncrementer + 1;
			ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer-1], yValsH[my_hourIncrementer-1], ST7735_Color565(0, 0, 0));
		}
		
		ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer], yValsH[my_hourIncrementer], ST7735_Color565(0, 0, 255));
		if (my_hourIncrementer >= MINUTES_PER_HOUR){
			my_hourIncrementer = 0;
		}
		
		my_hours = time_hours;
	}
}

void enableAnalogDisplay() {
	drawClock(1, 0);
	Clock_setDisplayFunction(&analogTime);
}