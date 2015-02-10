#ifndef _ST7735_CONSTANTS_H_
#define _ST7735_CONSTANTS_H_
#include <stdint.h>

static int xVals[60] = {};
static int yVals[60] = {};
static int analogNumLocs[12] = {};
	//These next two arrays go in this order: clockwise from 0 to 12, and dimensions from inside to outside
static int analogDashX[24] = {64, 64, 93, 96, 115, 119, 122, 127, 115, 119, 93, 96, 64, 64, 35, 32, 13, 9, 5, 0, 13, 9, 35, 32};
static int analogDashY[24] = {21, 16, 29, 25, 51, 48, 80, 80, 109, 112, 131, 135, 139, 144, 132, 135, 109, 112, 80, 80, 51, 48, 29, 25};
static int digitalNumLocs[28] = {};

#endif