#include "ST7735_Line.h"
#include "ST7735.h"
#include <math.h>
#include "inc/tm4c123gh6pm.h"
//*************?  ? ST7735_Line********************************************  
//  Draws one line on the ST7735 color LCD  
//  Inputs: (x1,y1) is the start point  
//          (x2,y2) is the end point  
// x1,x2 are horizontal positions, columns from the left edge  
//               must be less than 128  
//               0 is on the left, 126 is near the right  
// y1,y2 are vertical positions, rows from the top edge  
//               must be less than 160  
//               159 is near the wires, 0 is the side opposite the wires  
//        color 16­bit color, which can be produced by ST7735_Color565()   
// Output: none  
#define POSITIVE 1
#define NEGATIVE -1

void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	// TODO - validate input
	int16_t num, denom;
	uint16_t current_x, current_y;
	int8_t incrementor = 0;
	num = y2 - y1;
	denom = x2 - x1;
	current_x = x1;
	current_y = y1;
	// case 1: x2 - x1 > y2 - y1
	// increment across x
	
	if ( abs(denom) > abs(num) ){
		// denom > num => denom != 0
		int numSteps = 1;
		if (denom > 0){
			// we'll add +1
			incrementor = POSITIVE;
		} else {
			// we'll add -1
			incrementor = NEGATIVE;
		}
		int16_t base_y = y1 * denom;
		while (current_x != x2){
			ST7735_DrawPixel(current_x, current_y, color);
			// sample: (5, 4) => (10, 8)
			// num = 4; den = 8
			// drawn: (5, 4); (6, [(20 + 1 * 4)/5] = 4); (7, [(20 + 2 * 4)/5] = 5); (8, [(20 + 3*4)/5] = 6);
			// (9, [(20 + 4*4) / 5] = 7); (10, [(20 + 5*4)/5] = 8)
			// simplified: (5, 4); (6, 4); (7, 5); (8, 6); (9, 7); (10, 8)
			// Assuming you only used an integral slope, the slope 4/5 would evaluate to 0.
			
			current_x += incrementor;
			current_y = (base_y + incrementor * numSteps * num) / (denom); 
			++numSteps;
		}
	}
	
	// case 2: y2 - y1 > x2 - x1
	// increment across y
	else {
		// num >= denom
		// if num == denom == 0, current_y == y2 and the loop won't be executed
		// sample: (4,5) => (8, 10)
		// num = 5, den = 4
		// drawn: (4, 5); ([(4*5) + 1 * 4)/5] = 4, 6); ([(20 + 2*4)/5 = 5, 7); ([(20+3*4)/5] = 6, 8);
		// ([(20 + 4*4)/5] = 7, 9); ([(20 + 5*4)/5] = 8, 10);
		// simplified: (4,5); (4,6); (5,7); (6,8); (7,9); (8,10)
	
		int numSteps = 1;
		if (num > 0){
			incrementor = POSITIVE;
		} else {
			incrementor = NEGATIVE;
		}
		int16_t base_x = x1 * num;
		
		while (current_y != y2){
			ST7735_DrawPixel(current_x, current_y, color);
			current_y += incrementor;
			current_x = (base_x + incrementor * numSteps * denom) / (num);
			++numSteps;
			
		}
	}
	
	
	
	
}