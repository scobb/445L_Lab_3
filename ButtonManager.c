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
void handlePF0(){
	// PF0 will be our arm/disarm button. Toggles the status of our alarm.
	if (alarm_armed){
		disarmAlarm();
	} else {
		armAlarm();
	}
}
void handlePF1(){
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
	++alarm_hours;
}
void incrementAlarmMinutes(){
	++alarm_minutes;
}
void incrementTimeHours(){
	++time_hours;
}
void incrementTimeMinutes(){
	++time_minutes;
}
void handlePF2(){
	set_mode = (set_mode + 1 ) % NUM_SET_MODES;
	if (set_mode == NONE){
		incrementHours = 0;
		incrementMinutes = 0;
	} else if (set_mode == ALARM){
		incrementHours = &incrementAlarmHours;
		incrementMinutes = &incrementAlarmMinutes;
	}
}
void handlePF3(){
	printf("3\n");
}
void handlePF4(){
	printf("4\n");
}
void CheckDebounce(buttonStatus* buttons, uint8_t numPorts){
	uint8_t i;
	SysTick_Wait10ms(40);
	for (i=0; i < numPorts; ++i){
		if (buttons[i].isLow && buttons[i].readValue == 0){
			buttons[i].handler();
		}
	}
}
void GPIOPortF_Handler(void){
	uint8_t i;
	GPIO_PORTF_ICR_R = 0x1F;      // acknowledge flag 0, 1
	// TODO - add a blind cycle wait to get rid of bounce
	buttonStatus ports[5] = {
		{PF0, FALSE, &handlePF0},
		{PF1, FALSE ,&handlePF1}, 
		{PF2, FALSE, &handlePF2},
		{PF3, FALSE, &handlePF3},
		{PF4, FALSE, &handlePF4}
	};
	for (i=0; i < 5; i++){
		if (ports[i].readValue == 0){
			ports[i].isLow = TRUE;
		}
	}
	CheckDebounce(&ports[0], 5);
}
