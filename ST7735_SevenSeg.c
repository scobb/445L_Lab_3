#include "ST7735_SevenSeg.h"
#include "ST7735.h"
// 128 x 160
#define DELTA_X 24
#define DELTA_Y 30
void drawHorizontalSegment(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of any segment
	*/
	uint16_t x2;
	x2 = x + DELTA_X;
  ST7735_Line(x, y, x2, y, ST7735_WHITE);
}

void drawVerticalSegment(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of any segment
	*/
	uint16_t y2 = y + DELTA_Y;
  ST7735_Line(x, y, x, y2, ST7735_WHITE);
}

void drawSeg1(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawHorizontalSegment(x, y);
}
void drawSeg4(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawHorizontalSegment(x, y+2*DELTA_Y);
}
void drawSeg7(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawHorizontalSegment(x, y+DELTA_Y);
}
void drawSeg2(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawVerticalSegment(x+DELTA_X, y);
}
void drawSeg3(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawVerticalSegment(x+DELTA_X, y+DELTA_Y);
}
void drawSeg5(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawVerticalSegment(x, y+DELTA_Y);
}
void drawSeg6(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawVerticalSegment(x, y);
}

void draw0(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg1(x, y);
	drawSeg2(x, y);
	drawSeg3(x, y);
	drawSeg4(x, y);
	drawSeg5(x, y);
	drawSeg6(x, y);
}

void draw1(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg2(x, y);
	drawSeg3(x, y);
}

void draw2(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg1(x, y);
	drawSeg2(x, y);
	drawSeg7(x, y);
	drawSeg5(x, y);
	drawSeg4(x, y);
}
void draw3(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg1(x, y);
	drawSeg2(x, y);
	drawSeg3(x, y);
	drawSeg4(x, y);
	drawSeg7(x, y);
}
void draw4(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg2(x, y);
	drawSeg3(x, y);
	drawSeg6(x, y);
	drawSeg7(x, y);
}
void draw5(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg1(x, y);
	drawSeg4(x, y);
	drawSeg3(x, y);
	drawSeg6(x, y);
	drawSeg7(x, y);
}
void draw6(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg1(x, y);
	drawSeg3(x, y);
	drawSeg4(x, y);
	drawSeg5(x, y);
	drawSeg6(x, y);
	drawSeg7(x, y);
}
void draw7(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg1(x, y);
	drawSeg2(x, y);
	drawSeg3(x, y);
}
void draw8(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg1(x, y);
	drawSeg2(x, y);
	drawSeg3(x, y);
	drawSeg4(x, y);
	drawSeg5(x, y);
	drawSeg6(x, y);
	drawSeg7(x, y);
}
void draw9(uint16_t x, uint16_t y){
	/*
	args:
	(x, y) describes the top left point of the seven segment display
	*/
	drawSeg1(x, y);
	drawSeg2(x, y);
	drawSeg3(x, y);
	drawSeg4(x, y);
	drawSeg6(x, y);
	drawSeg7(x, y);
}