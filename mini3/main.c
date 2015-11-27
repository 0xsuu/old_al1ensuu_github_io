
#include "lpc_types.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pwm.h"
#include "string.h"

#include "main.h"
#include "ad.h"
#include "led.h"
#include "serialIO.h"
#include "sysTick.h"
#include "i2cIO.h"

void stage1()
{
	while(1)
	{
		int data = 0;
		data = readChannel(1);
		if (data)
		{
		char dataString[20];
		sprintf(dataString, "\n\rVoltage: %lf v\n\r", 3.3*data/4095.0);
		
		serialPrint(dataString);
		//serialPrintWithInt("\n\rVoltage: %\n\r", data);
		break;}
	}
}
double amp=0.65,freq=40;
void stage2()
{
	amp=0.65;
	freq=100;
	while(1)
	DAC_outputSineWave(amp, freq);
}

void stage3()
{
	int data = 0;
	while(1)
	{
		data = 0;
		data = readChannel(1);

		if (data)
		{	
			//serialPrintWithInt("\n\rVoltage: %\n\r", data);
			DAC_output((int)(0x3ff*(data*1.0)/0xfff));
		}
		//delayMS(1000);
	}
}


void TIMER1_IRQHandler(void)
{	
	PWM_MATCHCFG_Type PWMMatchCfgDat;
        
	if (TIM_GetIntStatus(LPC_TIM1, TIM_MR1_INT)== SET)
        {
		amp += 0.5;
		freq += 100;
		
	}
        TIM_ClearIntPending(LPC_TIM1, TIM_MR1_INT);
}

void Timer10Sec()
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
	// Initialize timer 0, prescale count time of 100uS
        TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
        TIM_ConfigStruct.PrescaleValue  = 1;

        // use channel 0, MR0
        TIM_MatchConfigStruct.MatchChannel = 1;
        // Enable interrupt when MR0 matches the value in TC register
        TIM_MatchConfigStruct.IntOnMatch   = TRUE;
        //Enable reset on MR0: TIMER will reset if MR0 matches it
        TIM_MatchConfigStruct.ResetOnMatch = TRUE;
        //Stop on MR0 if MR0 matches it
        TIM_MatchConfigStruct.StopOnMatch  = FALSE;
        //Toggle MR0.0 pin if MR0 matches it
        TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
        // Set Match value, count value of 10000000 
        TIM_MatchConfigStruct.MatchValue   = 10000000;

        // Set configuration for Tim_config and Tim_MatchConfig
        TIM_Init(LPC_TIM1, TIM_TIMER_MODE,&TIM_ConfigStruct);
        TIM_ConfigMatch(LPC_TIM1,&TIM_MatchConfigStruct);

        /* preemption = 1, sub-priority = 1 */
        //NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
        /* Enable interrupt for timer 0 */
        NVIC_EnableIRQ(TIMER1_IRQn);
        // To start timer 0
        TIM_Cmd(LPC_TIM1,ENABLE);

	//serialPrint("Int confg\n\r");
	
	PWM_MATCHCFG_Type PWMMatchCfgDat;
}

void stage4()
{
	PWM_init();
}

int main()
{   
	serial_init();
	AI_init();
	DAC_init();
	Timer10Sec();
	
	//p.16
    //stage1();
    //p.18
    //stage2();
    //p.16 & p.18
    //stage3();
    //p.25
    stage4();
    
    return 0;
}
