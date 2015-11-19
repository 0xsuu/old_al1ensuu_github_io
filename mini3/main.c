
#include "lpc_types.h"

#include "main.h"
#include "ad.h"
#include "led.h"
#include "serialIO.h"
#include "sysTick.h"
#include "i2cIO.h"

void stage1()
{
	AI_init();
	while(1)
	{
		int data = readChannel(1);
		if (data)
		serialPrintWithInt("\n\rVoltage: %\n\r", data);
	}
}

void stage2()
{
	DAC_init();

	DAC_outputSineWave(3.3, 1);
}

void stage3()
{

}

int main()
{   
	serial_init();
	
    //stage1();
    stage2();
    //stage3();
    
    return 0;
}
