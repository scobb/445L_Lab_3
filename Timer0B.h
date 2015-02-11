#ifdef _TIMER_0B_
#define _TIMER_0B_

void Timer0B_Init(void(*task)(void), uint32_t period);
void setTimerBHandler(void(*task)(void));
#endif