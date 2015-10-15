#include "lpc17xx_gpio.h"		
#include "lpc17xx_systick.h"

void SysTick_Handler();
void SETUP_Systick(uint8_t);

int flag = 0;
static unsigned current_time =0;

int stopCount = 0;

void delayS(uint8_t time)
{
	stopCount = time*1000/80;
	SETUP_Systick(80);
	while(!flag);
	SYSTICK_Cmd(DISABLE);
	flag = 0;
}

void delayMS(uint32_t time)
{
	stopCount = time/80;
	SETUP_Systick(80);
	while(!flag);
	SYSTICK_Cmd(DISABLE);
	flag = 0;
}

void SysTick_Handler()
{
	current_time ++;
	if(current_time == stopCount) 
	{
		flag = 1;
		current_time = 0;
	}
	
}

void SETUP_Systick(uint8_t msec)
{
	SYSTICK_InternalInit(msec);
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}
		
