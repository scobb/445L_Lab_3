#include "Timer0B.h"
#include "inc/tm4c123gh6pm.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void (*TimerBFunction)(void);   // user function
void Timer0B_Init(void(*task)(void), uint32_t period){long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;      // 0) activate timer0
  TIMER0_CTL_R &= ~0x00000010;     // 1) disable timer0A during setup
  TIMER0_CFG_R = 0x00000000;       // 2) configure for 32-bit timer mode
  TIMER0_TBMR_R = 0x00000002;      // 3) configure for periodic mode, default down-count settings
  TIMER0_TBILR_R = period-1;       // 4) reload value
  TIMER0_TBPR_R = 0;               // 5) 12.5ns timer0B
  TIMER0_ICR_R = 0x00000010;       // 6) clear timer0B timeout flag
  TIMER0_IMR_R |= 0x00000010;      // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // 8) priority 2
  NVIC_EN0_R = NVIC_EN0_INT20;     // 9) enable interrupt 20 in NVIC
  TIMER0_CTL_R |= 0x00000010;      // 10) enable timer0A
  EndCritical(sr);
}
void setTimerBHandler(void(*task)(void)){
	TimerBFunction = task;
}


void Timer0B_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge timer0A timeout
	if (TimerBFunction){
		(*TimerBFunction)();
	}
}