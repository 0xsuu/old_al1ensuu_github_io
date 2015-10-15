
#include "lpc17xx_systick.h"

#include "main.h"
#include "led.h"
#include "serialIO.h"
#include "sysTick.h"

void delay() 
{
   unsigned int i;
 
   for (i = 0; i < 0x800000; i++);
}

void stage1()
{
    //Stage 1
    int i;
    for (i = 0; i < 5; i++)
    {
        LEDdebug(0b1111);
        delay();
        LEDoff();
        delay();
    }
}

void stage2()
{
    int i;
    for (i = 0; i < 16; i++)
    {
        LEDdebug(i);
        delay();
        LEDoff();
    }
}

void stage3()
{
    LEDdebugR(0b1001);
    delayMS(1000);
    LEDoff();
}

void stage4()
{
	LEDdebugR(0b1001);
	serial_init();
	printWithInt("Hello World! %\n\r", 0x12345678);
}

int main()
{   
    setupLEDs();
    //LEDdebug(0b1010);
    //stage1();
    //stage2();
    stage3();
    //stage4();
    
    return 0;
}
