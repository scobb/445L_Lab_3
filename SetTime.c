#include "SetTime.h"
#include "inc/tm4c123gh6pm.h"
#include "ST7735.h"
#define PF0       (*((volatile uint32_t *)0x40025004))
#define PF1       (*((volatile uint32_t *)0x40025008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void SetTime_Init(){
	volatile uint32_t delay;
	
  SYSCTL_RCGCGPIO_R |= 0x00000020;  // 1) activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R |= 0x03;           // allow changes to PF1-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R &= ~0x03;        // 3) disable analog on PF
  GPIO_PORTF_DIR_R &= ~0x03;          // 5) PF4,PF0 out, PF1 out
  GPIO_PORTF_AFSEL_R &= ~0x03;        // 6) disable alt funct on PF1-0
  GPIO_PORTF_PUR_R |= 0x03;          // enable pull-up on PF0 and PF1
  GPIO_PORTF_DEN_R |= 0x03;          // 7) enable digital I/O on PF1-0
  GPIO_PORTF_PCTL_R &= ~0x000000FF; // configure PF0, 1 as GPIO
	
  GPIO_PORTF_IS_R &= ~0x03;     // (d) PF0, 1 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x03;    //     PF0, 1 is not both edges
  GPIO_PORTF_IEV_R &= ~0x03;    //     PF0, 1 falling edge event
  GPIO_PORTF_ICR_R = 0x03;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x03;      // (f) arm interrupt on PF0, 1 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
  EnableInterrupts();           // (i) Clears the I bit
}
void GPIOPortF_Handler(void){
	GPIO_PORTF_ICR_R = 0x03;      // acknowledge flag 0, 1
	// TODO - add a blind cycle wait to get rid of bounce
	if (PF0 == 0){
		printf("0\n");
	} else if (PF1 == 0){
		printf("1\n");
	} else {
		printf("?\n");
	}
}
