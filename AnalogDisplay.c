#include "Clock.h"
#include "ST7735.h"
#include "ST7735_Constants.h"
#include "ST7735_Line.h"
#include "inc/tm4c123gh6pm.h"
#include "AnalogDisplay.h"
#include "Alarm.h"

#define CENTER_X 64
#define CENTER_Y 64
#define MINUTES_PER_HOUR 60

//drawDashes - 1 means draw them, 0 means don't
//numToDraw - 0 means draw all numbers, 1-12 means draw that number
void drawClock(uint8_t drawDashes, uint8_t numToDraw) {
	ST7735_SetCursor(0, 0);
	//This will draw all of the clock dashes
	if (drawDashes == 1){
		ST7735_FillRect(0, 0, 128, 128, 0);
		for (int i = 0; i < 24; i+=2){
			ST7735_Line(analogDashX[i], analogDashY[i], analogDashX[i+1], analogDashY[i+1], ST7735_Color565(255, 255, 255));
		}
	}
	
	if (numToDraw == 0){
		for (int j = 1; j <= 12; j++){
			ST7735_SetCursor(analogNumX[j-1], analogNumY[j-1]);
			printf("%d", j);
		}
	}
	else {
		ST7735_SetCursor(analogNumX[numToDraw-1], analogNumY[numToDraw-1]);
		printf("%d", numToDraw);
	}
	
	//Need to draw hour/minute hands immediately upon drawing
	ST7735_Line(CENTER_X, CENTER_Y, xVals[time_minutes], yVals[time_minutes], ST7735_Color565(0, 255, 0));
	ST7735_Line(CENTER_X, CENTER_Y, xValsH[(analog_display_hours*5)+(time_minutes/12)], yValsH[(analog_display_hours*5)+(time_minutes/12)], ST7735_Color565(0, 0, 255));
}

void displayCurrentAlarmTimeAnalog() {
	//We are going to use drawClock, but use alarm time instead of time
	ST7735_SetCursor(0, 0);
	ST7735_FillRect(0, 0, 128, 128, 0);
	//This will draw all of the clock dashes
	for (int i = 0; i < 24; i+=2){
		ST7735_Line(analogDashX[i], analogDashY[i], analogDashX[i+1], analogDashY[i+1], ST7735_Color565(255, 255, 255));
	}
	
	for (int j = 1; j <= 12; j++){
		ST7735_SetCursor(analogNumX[j-1], analogNumY[j-1]);
		printf("%d", j);
	}
	
	//Need to draw hour/minute hands immediately upon drawing
	ST7735_Line(CENTER_X, CENTER_Y, xVals[alarm_minutes], yVals[alarm_minutes], ST7735_Color565(0, 255, 0));
	ST7735_Line(CENTER_X, CENTER_Y, xValsH[(alarm_hours*5)+(alarm_minutes/12)], yValsH[(alarm_hours*5)+(alarm_minutes/12)], ST7735_Color565(0, 0, 255));

}

void analogTime() {
	static uint16_t my_minutes = 0;
	static uint16_t my_hourIncrementer = 0;
	
	//Before we do anything, let's check to see if the hour/minute hands are currently overlapping
	if (my_hourIncrementer == my_minutes){
		ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer], yValsH[my_hourIncrementer], ST7735_Color565(0, 0, 0));
		ST7735_Line(CENTER_X, CENTER_Y, xVals[time_minutes], yVals[time_minutes], ST7735_Color565(0, 255, 0));
	}
	if (my_minutes != time_minutes){
		//Draws a black line over the previous value and a green line on the next minute
		if (time_minutes > my_minutes+1){
			//Just in case minutes is updated by more than 1
			ST7735_Line(CENTER_X, CENTER_Y, xVals[my_minutes], yVals[my_minutes], ST7735_Color565(0, 0, 0));
		}
		//We will also erase right behind us, to eliminate any possibility of multiple minute hands
		if (time_minutes == 0){
			ST7735_Line(CENTER_X, CENTER_Y, xVals[59], yVals[59], ST7735_Color565(0, 0, 0));
		}
		else {
			ST7735_Line(CENTER_X, CENTER_Y, xVals[time_minutes-1], yVals[time_minutes-1], ST7735_Color565(0, 0, 0));
		}
		ST7735_Line(CENTER_X, CENTER_Y, xVals[time_minutes], yVals[time_minutes], ST7735_Color565(0, 255, 0));
		my_minutes = time_minutes;
		
		if (time_minutes % 5 != 3){
			drawClock(0, (time_minutes+2)/5);//Should redraw the right number for 4 out of 5 minutes; changed for padding
		}
	}
	
	//Now dealing with hours
	//Will erase previous hour hand in case of an update
	ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer], yValsH[my_hourIncrementer], ST7735_Color565(0, 0, 0));
	my_hourIncrementer = (analog_display_hours*5)+(time_minutes/12);
	
	ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer], yValsH[my_hourIncrementer], ST7735_Color565(0, 0, 255));
	if (my_hourIncrementer >= MINUTES_PER_HOUR){
		my_hourIncrementer = 0;
	}
}

void analogAlarmTime() {
	static uint16_t my_minutes = 0;
	static uint16_t my_hourIncrementer = 0;
	
	//Before we do anything, let's check to see if the hour/minute hands are currently overlapping
	if (my_hourIncrementer == my_minutes){
		ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer], yValsH[my_hourIncrementer], ST7735_Color565(0, 0, 0));
		ST7735_Line(CENTER_X, CENTER_Y, xVals[alarm_minutes], yVals[alarm_minutes], ST7735_Color565(0, 255, 0));
	}
	
	//Now let's see if we are at 60 minutes. If so, we need to to set it down to 0
	if (alarm_minutes >= 60){
		alarm_minutes = 0;
		alarm_hours++;
		if (alarm_hours == 12){
			alarm_hours = 0;
		}
	}
	
	if (my_minutes != alarm_minutes){
		//Draws a black line over the previous value and a green line on the next minute
		if (alarm_minutes > my_minutes+1){
			//Just in case minutes is updated by more than 1
			ST7735_Line(CENTER_X, CENTER_Y, xVals[my_minutes], yVals[my_minutes], ST7735_Color565(0, 0, 0));
		}
		//We will also erase right behind us, to eliminate any possibility of multiple minute hands
		if (alarm_minutes == 0){
			ST7735_Line(CENTER_X, CENTER_Y, xVals[59], yVals[59], ST7735_Color565(0, 0, 0));
		}
		else {
			ST7735_Line(CENTER_X, CENTER_Y, xVals[alarm_minutes-1], yVals[alarm_minutes-1], ST7735_Color565(0, 0, 0));
		}
		ST7735_Line(CENTER_X, CENTER_Y, xVals[alarm_minutes], yVals[alarm_minutes], ST7735_Color565(0, 255, 0));
		my_minutes = alarm_minutes;
		
		//The greater than 3 condition is so we don't draw a 0 on the screen
		if (alarm_minutes % 5 != 3 && alarm_minutes > 3){
			ST7735_SetCursor(analogNumX[(alarm_minutes+2)/5-1], analogNumY[(alarm_minutes+2)/5-1]);
			printf("%d", (alarm_minutes+2)/5);
		}
	}
	
	//Now dealing with hours
	//Will erase previous hour hand in case of an update
	if (my_hourIncrementer != my_minutes) {
		ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer], yValsH[my_hourIncrementer], ST7735_Color565(0, 0, 0));
	}
	my_hourIncrementer = (alarm_hours*5)+(alarm_minutes/12);
	
	ST7735_Line(CENTER_X, CENTER_Y, xValsH[my_hourIncrementer], yValsH[my_hourIncrementer], ST7735_Color565(0, 0, 255));
	if (my_hourIncrementer >= MINUTES_PER_HOUR){
		my_hourIncrementer = 0;
	}
}

void enableAnalogDisplay() {
	display_mode = ANALOG;
	drawClock(1, 0);
	Clock_setDisplayFunction(&analogTime);
}
