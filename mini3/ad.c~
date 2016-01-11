
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pwm.h"
#include "math.h"

#include "i2cIO.h"
#include "serialIO.h"
#include "ad.h"
#include "led.h"
#include "lcd.h"
#include "stringProcess.h"
#include "sysTick.h"

#define PI (3.141592653589793)

void AI_init()
{
	//AD0.1 at P0.24
	PINSEL_CFG_Type PinCfg;
	PinCfg.OpenDrain  = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Funcnum = 1;
	
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 24;
	PINSEL_ConfigPin(&PinCfg);

	ADC_Init(LPC_ADC , 200000);
	ADC_ChannelCmd(LPC_ADC, 1, ENABLE);
	ADC_BurstCmd(LPC_ADC,ENABLE);
	ADC_StartCmd(LPC_ADC, ADC_START_NOW);
}

void DAC_init()
{
	//Analog Out at P0.26, Function 2 (PIN 18)
	PINSEL_CFG_Type PinCfg;
	PinCfg.OpenDrain  = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Funcnum = 2;
	
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 26;
	PINSEL_ConfigPin(&PinCfg);

	DAC_Init(LPC_DAC);
}

int readChannel(uint8_t channel)
{
	if (ADC_ChannelGetStatus(LPC_ADC, channel, 1))
	{
		return ADC_ChannelGetData(LPC_ADC, channel);
	}
	else
	{
		return 0;
	}
}	

void DAC_output(int value)
{
	DAC_UpdateValue(LPC_DAC, value);
}

void DAC_outputSineWave(double amplitude, double frequency)
{
	if (amplitude > 1.65) amplitude = 1.65;
	
	int i = 0;
	for (i=0; i<20000/frequency; i++)
	{
		DAC_output((int)((sin(frequency*i/20000*2*PI)*amplitude+amplitude)*(1023/3.3)));
	}
}

int PWM_ON_TIME = 1450;

void TIMER0_IRQHandler(void)
{	
	PWM_MATCHCFG_Type PWMMatchCfgDat;
        
	if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)== SET)
        {
		if (PWM_ON_TIME > 1900)
		{ PWM_ON_TIME = 600;}
		else
		{
		PWM_ON_TIME += 10;}
		PWM_MatchUpdate(LPC_PWM1, 2, PWM_ON_TIME, PWM_MATCH_UPDATE_NOW);
		PWMMatchCfgDat.IntOnMatch = DISABLE;
		PWMMatchCfgDat.MatchChannel = 2;
		PWMMatchCfgDat.ResetOnMatch = DISABLE;
		PWMMatchCfgDat.StopOnMatch = DISABLE;
		PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);
	
		PWM_ChannelCmd(LPC_PWM1, 2, ENABLE);
		
		PWM_ResetCounter(LPC_PWM1);
		PWM_CounterCmd(LPC_PWM1, ENABLE);
	
		 /* Start PWM now */
		PWM_Cmd(LPC_PWM1, ENABLE);
		
	
        }
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}

void timer0_init()

{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
	// Initialize timer 0, prescale count time of 100uS
        TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
        TIM_ConfigStruct.PrescaleValue  = 1; // 1 us 

        // use channel 0, MR0
        TIM_MatchConfigStruct.MatchChannel = 0;
        // Enable interrupt when MR0 matches the value in TC register
        TIM_MatchConfigStruct.IntOnMatch   = TRUE;
        //Enable reset on MR0: TIMER will reset if MR0 matches it
        TIM_MatchConfigStruct.ResetOnMatch = TRUE;
        //Stop on MR0 if MR0 matches it
        TIM_MatchConfigStruct.StopOnMatch  = FALSE;
        //Toggle MR0.0 pin if MR0 matches it
        TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
        // Set Match value, count value of 50000 (50000 * 1uS = 50000us)
        TIM_MatchConfigStruct.MatchValue   = 50000;

        // Set configuration for Tim_config and Tim_MatchConfig
        TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);
        TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);

        /* preemption = 1, sub-priority = 1 */
        //NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
        /* Enable interrupt for timer 0 */
        NVIC_EnableIRQ(TIMER0_IRQn);
        // To start timer 0
        TIM_Cmd(LPC_TIM0,ENABLE);

	//serialPrint("Int confg\n\r");
	
	PWM_MATCHCFG_Type PWMMatchCfgDat;
	
	
	
	
}
void PWM_init()
{
	//timer0_init();
	PWM_TIMERCFG_Type PWMCfgDat;
	PWM_MATCHCFG_Type PWMMatchCfgDat;
	PINSEL_CFG_Type PinCfg;

	//p25 as PWM1.2 output. P2.1 in embed

	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	
	PinCfg.Funcnum = 1;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	// initialize PWM timer mode
	PWMCfgDat.PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
	PWMCfgDat.PrescaleValue = 1;
	PWM_Init(LPC_PWM1, PWM_MODE_TIMER, &PWMCfgDat);
	
	/* Set match value for PWM match channel 0 = 5000000uS, update immediately */
	PWM_MatchUpdate(LPC_PWM1, 0, 20000, PWM_MATCH_UPDATE_NOW);
	/* PWM Timer/Counter will be reset when channel 0 matching
	* no interrupt when match
	 * no stop when match 
	*  PWM Pulse Width is set by the PWMMRO */
	PWMMatchCfgDat.IntOnMatch = DISABLE;
	PWMMatchCfgDat.MatchChannel = 0;
	PWMMatchCfgDat.ResetOnMatch = ENABLE;
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);
	
	//PWM channel edge option
	PWM_ChannelConfig(LPC_PWM1, 2, PWM_CHANNEL_SINGLE_EDGE);
	
	//Configure match value for match registers
	PWM_MatchUpdate(LPC_PWM1, 2, PWM_ON_TIME, PWM_MATCH_UPDATE_NOW);
	PWMMatchCfgDat.IntOnMatch = DISABLE;
	PWMMatchCfgDat.MatchChannel = 2;
	PWMMatchCfgDat.ResetOnMatch = DISABLE;
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);
	
	PWM_ChannelCmd(LPC_PWM1, 2, ENABLE);
	
	PWM_ResetCounter(LPC_PWM1);
	PWM_CounterCmd(LPC_PWM1, ENABLE);
	
	 /* Start PWM now */
	PWM_Cmd(LPC_PWM1, ENABLE);
	
	
}


