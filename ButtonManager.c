#include "ButtonManager.h"
#include "inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "SysTick.h"
#include "Alarm.h"
#include "Clock.h"
#include "DigitalDisplay.h"
#include "AnalogDisplay.h"

#define PF0       (*((volatile uint32_t *)0x40025004))
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF4       (*((volatile uint32_t *)0x40025040))
#define TRUE 1
#define FALSE 0
#define NONE 0
#define ALARM 1
#define TIME 2
#define NUM_SET_MODES 3
#define LABEL_X 16
#define LABEL_Y 126
#define LABEL_WIDTH 80
#define LABEL_HEIGHT 20

void(*incrementHours)(void) = 0;
void(*incrementMinutes)(void) = 0;
uint8_t set_mode = NONE;
typedef struct {
  uint32_t readValue;
	uint8_t isLow;
	void(*handler)(void);
} buttonStatus;


void ButtonManager_Init(){
	volatile uint32_t delay;
	
  SYSCTL_RCGCGPIO_R |= 0x00000020;  // 1) activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R |= 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R &= ~0x1F;        // 3) disable analog on PF
  GPIO_PORTF_DIR_R &= ~0x1F;          // 5) PF4,PF0 out, PF1 out
  GPIO_PORTF_AFSEL_R &= ~0x1F;        // 6) disable alt funct on PF1-0
  GPIO_PORTF_PUR_R |= 0x1F;          // enable pull-up on PF0 and PF1
  GPIO_PORTF_DEN_R |= 0x1F;          // 7) enable digital I/O on PF1-0
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; // configure PF0, 1 as GPIO
	
  GPIO_PORTF_IS_R &= ~0x1F;     // (d) PF0, 1 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x1F;    //     PF0, 1 is not both edges
  GPIO_PORTF_IEV_R &= ~0x1F;    //     PF0, 1 falling edge event
  GPIO_PORTF_ICR_R = 0x1F;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x1F;      // (f) arm interrupt on PF0, 1 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
}
void armDisarmPressed(){
	// Toggles the status of our alarm.
	if (alarm_armed){
		disarmAlarm();
	} else {
		armAlarm();
	}
}
void displayModePressed(){
	// handler for button press to swap between digital and analog
	if (display_mode == DIGITAL){
		ST7735_FillScreen(0);    
		// ST7735_SetCursor(0,0);
		enableAnalogDisplay();
	} else {
		ST7735_FillScreen(0);    
		enableDigitalDisplay();
	}
}
void incrementAlarmHours(){
	// handler for button press to update alarm hours
	++alarm_hours;
	displayAlarmDigital();
}
void incrementAlarmMinutes(){
	// handler for button press to update alarm minutes
	++alarm_minutes;
	displayAlarmDigital();
}
void incrementTimeHours(){
	// handler for button press to update time hours
	++time_hours;
	displayDigital();
}
void incrementTimeMinutes(){
	// handler for button press to update time minutes
	++time_minutes;
	displayDigital();
}
void setModePressed(){
	// changes whether we're setting time, setting alarm, or neither
	set_mode = (set_mode + 1 ) % NUM_SET_MODES;
	if (set_mode == NONE){
		ST7735_FillRect(LABEL_X, LABEL_Y, LABEL_WIDTH, LABEL_HEIGHT, 0);
		// normal operation; disable H/M button presses
		incrementHours = 0;
		incrementMinutes = 0;
	} else if (set_mode == ALARM){
		// set alarm mode
		// disable real time screen updates
		Clock_setDisplayFunction(0);  
		
		// display label
		ST7735_FillRect(LABEL_X, LABEL_Y, LABEL_WIDTH, LABEL_HEIGHT, 0);
		ST7735_SetCursor(3,13);
		printf("set alarm");
		
		// display alarm time
		// if (display_mode == DIGITAL)
		displayCurrentAlarmTimeDigital();
		//else displayAlarmAnalog();
		
		// update H/M button functionality
		incrementHours = &incrementAlarmHours;
		incrementMinutes = &incrementAlarmMinutes;
	} else {
		// set time mode
		
		// clear screen
		// ST7735_FillScreen(0);    
		
		// display label
		ST7735_FillRect(LABEL_X, LABEL_Y, LABEL_WIDTH, LABEL_HEIGHT, 0);
		ST7735_SetCursor(3,13);
		printf("set time");

		if (display_mode == DIGITAL) {
			// show the current time in full
			displayCurrentTimeDigital();
			
			// resume real time updates
			Clock_setDisplayFunction(&displayDigital);
		} else {
			Clock_setDisplayFunction(&analogTime);
		}
		
		// update H/M button functionality
		incrementHours = &incrementTimeHours;
		incrementMinutes = &incrementTimeMinutes;
	}
}

void hoursPressed(){
	// HOURS button
	if (incrementHours)
		(*incrementHours)();
}
void minutesPressed(){
	// MINUTES button
	if (incrementMinutes)
		(*incrementMinutes)();
}
void CheckDebounce(buttonStatus* buttons, uint8_t numPorts){
	// private function to allow us to debounce all buttons
	uint8_t i;
	SysTick_Wait10ms(40);
	for (i=0; i < numPorts; ++i){
		// if a button was low before and is still low, call its handler
		if (buttons[i].isLow && buttons[i].readValue == 0){
			buttons[i].handler();
		}
	}
}
void GPIOPortF_Handler(void){
	// handler for port F -- all 5 buttons
	uint8_t i;
	GPIO_PORTF_ICR_R = 0x1F;      // acknowledge flag 0-4
	buttonStatus ports[5] = {
		{PF0, FALSE, &armDisarmPressed},
		{PF1, FALSE ,&displayModePressed}, 
		{PF2, FALSE, &setModePressed},
		{PF3, FALSE, &hoursPressed},
		{PF4, FALSE, &minutesPressed}
	};
	
	// check all ports to see if any is low
	for (i=0; i < 5; i++){
		if (ports[i].readValue == 0){
			ports[i].isLow = TRUE;
		}
	}
	CheckDebounce(&ports[0], 5);
}
