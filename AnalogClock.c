#include "Clock.h"
#include "ST7735.h"
#include "ST7735_Constants.h"
#include "St7735_Line.h"
#include "inc/tm4c123gh6pm.h"

void drawClock() {
	//This will draw all of the clock dashes
	for (int i = 0; i < 24; i+=2){
		ST7735_line(analogDashX[i], analogDashY[i], analogDashX[i+1], analogDashY[i+1], ST7735_Color565(255, 255, 255));
	}
	
	//Testing setCursor to see how it works
	for (int j = 0; j < 16; j++){
		ST7735_SetCursor(64, 10*j);
		printf("Test");
	}
}
