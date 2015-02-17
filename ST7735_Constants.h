#ifndef _ST7735_CONSTANTS_H_
#define _ST7735_CONSTANTS_H_
#include <stdint.h>

static unsigned short speaker[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x52AA, 0x4A49, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xCE79, 0xFFFF, 0x632C, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x630C, 0xFFFF, 0xFFFF, 0xFFFF, 0x6B4D, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x2965, 0xFFFF, 0xFFFF, 0xCE79, 0x0841, 0xDEDB, 0xFFFF, 0xFFFF, 0xFFDF, 0xFFFF, 0xB5B6, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x39E7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDF, 0xF79E, 0xF79E, 0xF79E, 0xFFDF, 0xBDD7, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x39E7, 0xFFFF, 0xFFDF, 0xF7BE, 0xEF9D, 0xDEFB, 0xDEFB, 0xDEFB, 0xDEFB, 0xE73C, 0xA534, 0x0861, 0x0000, 0x0000,
 0x0000, 0x0000, 0x39C7, 0xF79E, 0xDEFB, 0xDF1C, 0xDEDB, 0xBDD7, 0xBDF7, 0xBDF7, 0xBDF7, 0xC618, 0x94B2, 0x1082, 0x0000, 0x0000,
 0x0000, 0x0000, 0x31A6, 0xA555, 0x9CD3, 0xA514, 0x94B2, 0x7BEF, 0x7BF0, 0x7BF0, 0x7BF0, 0x7C10, 0x7BEF, 0x1082, 0x0000, 0x0000,
 0x0000, 0x0000, 0x2965, 0x6B8E, 0x630C, 0x7BEF, 0x8C51, 0x52AB, 0x5ACB, 0x5ACB, 0x5ACB, 0x5ACB, 0x6B4D, 0x0861, 0x0000, 0x0000,
 0x0000, 0x0000, 0x39C7, 0xDEFB, 0xCE59, 0xDEDB, 0xEF7D, 0x94B2, 0x94B3, 0x94B3, 0x94B3, 0x9CF3, 0x73AE, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x39C7, 0xFFFF, 0xFFFF, 0xEF5D, 0x39C7, 0xCE59, 0xD69A, 0xC618, 0xC618, 0xCE79, 0x7BEF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8C51, 0xFFFF, 0xE73C, 0xF79E, 0x630C, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18C3, 0xEF5D, 0xFFFF, 0x632C, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x9492, 0x630C, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,


};

static int xVals[61] = {64,70,76,81,87,
93,97,102,107,111,
115,116,118,119,120,
122,120,119,118,116,
115,111,107,102,97,
93,87,81,76,70,
64,58,52,47,41,
35,31,26,22,17,
13,11,10,8,6,
5,6,8,10,11,
13,17,22,26,31,
35,41,47,52,58,64};
static int yVals[61] = {5,7,8,10,12,
13, 17, 21, 26, 30,
35, 41, 46, 52, 58,
64, 70, 75, 81, 87,
93, 97, 101, 106, 110,
115, 117, 118, 120, 122,
123, 122, 120, 119, 117,
116, 111, 107, 102, 98,
93, 87, 81, 76, 70,
64, 58, 52, 47, 41,
35, 30, 26, 21, 17, 
13, 12, 10, 8, 7, 5};
//Hours
static int xValsH[61] = {64,67,70,72,76,
78,81,83,86,87,
89,90,91,91,92,
93,92,91,91,90,
89,87,85,83,81,
78,76,72,70,67,
64,61,58,55,53,
49,48,45,43,41,
39,38,38,37,36,
35,36,37,38,38,
39,41,43,45,48,
49,53,55,58,61,64};
static int yValsH[61] = {35,36,37,38,38,
39,41,43,45,48,
49,53,55,58,61,
64,67,70,72,76,
78,81,83,86,87,
89,90,91,91,92,
93,92,91,91,90,
89,87,85,83,81,
78,76,72,70,67,
64,61,58,55,53,
49,48,45,43,41, 
39,38,38,37,36,35};
	//These next two arrays go in this order: clockwise from 0 to 12, and dimensions from inside to outside
static int analogDashX[24] = {64, 64, 93, 96, 115, 119, 122, 
127, 115, 119, 93, 96, 64, 64, 35, 32, 13, 9, 
5, 0, 13, 9, 35, 32};
static int analogDashY[24] = {5, 0, 13, 9, 35, 32, 64, 64, 
93, 96, 115, 119, 123, 126, 116, 119, 93, 
96, 64, 64, 35, 32, 13, 9};
static int analogNumX[12] = {14, 17, 18, 17, 14, 10, 6, 3, 2, 3, 6, 10};
static int analogNumY[12] = {2, 4, 6, 8, 10, 11, 10, 8, 6, 4, 2, 1};

#endif