#include "ButtonManager.h"
#include "inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "SysTick.h"
#include "Alarm.h"
#include "Clock.h"
#include "DigitalDisplay.h"
#include "AnalogDisplay.h"
#include "ST7735_Constants.h"

#define PF0       (*((volatile uint32_t *)0x40025004))
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF4       (*((volatile uint32_t *)0x40025040))
#define PD0       (*((volatile uint32_t *)0x40007004))
#define PD1       (*((volatile uint32_t *)0x40007008))
#define PD2       (*((volatile uint32_t *)0x40007010))
#define PD3       (*((volatile uint32_t *)0x40007020))
#define PD6       (*((volatile uint32_t *)0x40007100))
#define TRUE 1
#define FALSE 0
#define NONE 0
#define ALARM 1
#define TIME 2
#define NUM_SET_MODES 3
#define SPEAKER_X 80
#define SPEAKER_Y 144
#define SPEAKER_WIDTH 16
#define SPEAKER_HEIGHT 15
#define LABEL_X 16
#define LABEL_Y 126
#define LABEL_WIDTH 60
#define LABEL_HEIGHT 20
#define TOGGLE 1
#define INCREMENT 1
#define DECREMENT 0
#define PLUS_X 110
#define PLUS_Y 0
#define PLUS_WIDTH 9
#define PLUS_HEIGHT 18
void(*incrementHours)(void) = 0;
void(*incrementMinutes)(void) = 0;
uint8_t set_mode = NONE;
uint8_t increment_mode = INCREMENT;
uint8_t wasAnalog = FALSE;
typedef struct {
  uint32_t readValue;
	uint8_t isLow;
	void(*handler)(void);
} buttonStatus;

void ButtonManager_Init(){
	volatile uint32_t delay;
	
  SYSCTL_RCGCGPIO_R |= 0x00000008;  // 1) activate clock for Port D
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  // GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;   // Not using PD7 so don't need to unlock
  // GPIO_PORTD_CR_R |= 0x4F;           // Don't need to unlock PD7
  GPIO_PORTD_AFSEL_R &= ~0x4F;        // 6) disable alt funct on PF1-0
  GPIO_PORTD_DIR_R &= ~0x4F;          // 5) PD0-3, 6 are in
  GPIO_PORTD_AMSEL_R &= ~0x4F;        // 3) disable analog on PD
  GPIO_PORTD_PUR_R |= 0x4F;          // enable pull-up on PD
  GPIO_PORTD_DEN_R |= 0x4F;          // 7) enable digital I/O on PD
  GPIO_PORTD_PCTL_R &= ~0x0F00FFFF; // configure PD0-3, 6 as GPIO
	
  GPIO_PORTD_IS_R &= ~0x4F;     // (d) PF0, 1 is edge-sensitive
  GPIO_PORTD_IBE_R &= ~0x4F;    //     PF0, 1 is not both edges
  GPIO_PORTD_IEV_R &= ~0x4F;    //     PF0, 1 falling edge event
  GPIO_PORTD_ICR_R = 0x4F;      // (e) clear flag4
  GPIO_PORTD_IM_R |= 0x4F;      // (f) arm interrupt on PD0-3, 6 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&0x0FFFFFFF)|0x40000000; // (g) priority 2
  NVIC_EN0_R |= NVIC_EN0_INT3;      // (h) enable interrupt 19 in NVIC
	
	
}
void armDisarmPressed(){
	// Toggles the status of our alarm.
	if (alarm_armed){
		ST7735_FillRect(SPEAKER_X, SPEAKER_Y-SPEAKER_HEIGHT, SPEAKER_WIDTH+1, SPEAKER_HEIGHT+1, 0);
		disarmAlarm();
	} else {
		ST7735_DrawBitmap(SPEAKER_X, SPEAKER_Y, speaker, SPEAKER_WIDTH, SPEAKER_HEIGHT);
		armAlarm();
	}
}
void hideIncrementMode(){
	ST7735_FillRect(PLUS_X, PLUS_Y, PLUS_WIDTH, PLUS_HEIGHT, 0);
}
void displayIncrementMode(){
	hideIncrementMode();
	ST7735_SetCursor(19,1);
	if (increment_mode == INCREMENT)
		printf("+");
	else printf("-");
}
void toggleIncrementMode(){
	increment_mode ^= TOGGLE;
	displayIncrementMode();
}
void displayModePressed(){
	// handler for button press to swap between digital and analog
	if (set_mode != NONE){
		toggleIncrementMode();
	}
	else if (display_mode == DIGITAL){
		Clock_setDisplayFunction(&displayMilitary);
		ST7735_FillRect(0, 0, 128, 128, 0);  
		enableMilitaryDisplay();
		hideAmPm();
	} else if (display_mode == MILITARY){
		wasAnalog = TRUE;
		Clock_setDisplayFunction(&analogTime);
		ST7735_FillRect(0, 0, 128, 128, 0); 
		enableAnalogDisplay();
		displayAmPm();
	}
	 else {
		wasAnalog = FALSE;
		Clock_setDisplayFunction(&displayDigital);
		ST7735_FillRect(0, 0, 128, 128, 0);  
		enableDigitalDisplay();
	}
}
void incrementAlarmHours(){
	// handler for button press to update alarm hours
	if (increment_mode == INCREMENT) {
		alarm_hours = (alarm_hours + 1) % HOURS_PER_DAY;
	}
	else alarm_hours = (alarm_hours + 23) % HOURS_PER_DAY;
	if (display_mode != MILITARY) {displayAlarmDigital();}
	else {displayAlarmMilitary();}
}
void incrementAlarmMinutes(){
	// handler for button press to update alarm minutes
	if (increment_mode == INCREMENT) {
		alarm_minutes = (alarm_minutes + 1) % MINUTES_PER_HOUR;
	}
	else {alarm_minutes = (alarm_minutes + 59) % MINUTES_PER_HOUR;}
	if (display_mode != MILITARY) {displayAlarmDigital();}
	else {displayAlarmMilitary();}
}
void incrementTimeHours(){
	// handler for button press to update time hours
	if (increment_mode == INCREMENT)
		time_hours = (time_hours + 1) % HOURS_PER_DAY;
	else 
		time_hours = (time_hours + 23) % HOURS_PER_DAY;
	analog_display_hours = time_hours;
	if (time_hours >= 12){
		analog_display_hours = time_hours -12;
	}
	if (display_mode != MILITARY) {displayDigital();}
	else {displayMilitary();}
}
void incrementTimeMinutes(){
	// handler for button press to update time minutes
	if (increment_mode == INCREMENT)
		time_minutes = (time_minutes + 1) % MINUTES_PER_HOUR;
	else 
		time_minutes = (time_minutes + 59) % MINUTES_PER_HOUR;
	if (display_mode != MILITARY) {displayDigital();}
	else {displayMilitary();}
}
void setModePressed(){
	// changes whether we're setting time, setting alarm, or neither
	set_mode = (set_mode + 1 ) % NUM_SET_MODES;
	if (set_mode == NONE){
		hideIncrementMode();
		if (wasAnalog){
			enableAnalogDisplay();
		}
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
		ST7735_FillRect(0, 0, 127, 127, 0);
		if (display_mode != MILITARY) {
			displayCurrentAlarmTimeDigital();
		} else {displayCurrentAlarmTimeMilitary();}
		
		// update H/M button functionality
		displayIncrementMode();
		incrementHours = &incrementAlarmHours;
		incrementMinutes = &incrementAlarmMinutes;
	} else {
		// set time mode
		
		// display label
		ST7735_FillRect(LABEL_X, LABEL_Y, LABEL_WIDTH, LABEL_HEIGHT, 0);
		ST7735_SetCursor(3,13);
		printf("set time");

		if (display_mode != MILITARY){
			displayCurrentTimeDigital();
			
			// resume real time updates
			Clock_setDisplayFunction(&displayDigital);
		} else {
			displayCurrentTimeMilitary();
			Clock_setDisplayFunction(&displayMilitary);
		}
		
		// update H/M button functionality
		incrementHours = &incrementTimeHours;
		incrementMinutes = &incrementTimeMinutes;
	}
}
void resetToMainScreen(){
	seconds_since_button_press = 0;
	if (set_mode == NONE)
		return;
	if (set_mode == ALARM)
		setModePressed();
	setModePressed();
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

void GPIOPortD_Handler(void){
	// handler for port D -- all 5 buttons
	seconds_since_button_press = 0;
	GPIO_PORTD_ICR_R = 0x4F;      // acknowledge flag 0-4
	uint8_t i;
	buttonStatus ports[5] = {
		{PD0, FALSE, &armDisarmPressed},
		{PD1, FALSE ,&displayModePressed}, 
		{PD2, FALSE, &setModePressed},
		{PD3, FALSE, &hoursPressed},
		{PD6, FALSE, &minutesPressed}
	};
	
	// check all ports to see if any is low
	for (i=0; i < 5; i++){
		if (ports[i].readValue == 0){
			ports[i].isLow = TRUE;
		}
	}
	CheckDebounce(&ports[0], 5);
}

// deprecated code -- for use if interfacing buttons on Port F
void ButtonManager_Init_F(){
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
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; // configure PF0-4 as GPIO
	
  GPIO_PORTF_IS_R &= ~0x1F;     // (d) PF0, 1 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x1F;    //     PF0, 1 is not both edges
  GPIO_PORTF_IEV_R &= ~0x1F;    //     PF0, 1 falling edge event
  GPIO_PORTF_ICR_R = 0x1F;      // (e) clear flags
  GPIO_PORTF_IM_R |= 0x1F;      // (f) arm interrupt on PF0, 1 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
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
