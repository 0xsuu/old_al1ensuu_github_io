
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_adc.h"
#include "math.h"

#include "i2cIO.h"
#include "serialIO.h"
#include "ad.h"
#include "led.h"
#include "lcd.h"
#include "stringProcess.h"
#include "sysTick.h"

#define NUM_SINE_SAMPLE 200
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
	int i = 0;
	for (i=0; i<NUM_SINE_SAMPLE; i++)
	{
		DAC_output((int)((sin(i*2.0*PI/NUM_SINE_SAMPLE)+amplitude/2.0)*128));
	}
}

