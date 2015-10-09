
#include "lpc17xx_systick.h"

#include "main.h"
#include "led.h"
#include "serialIO.h"

void SysTick_Handler(void)
{
    LEDdebug(0b1111);
    //Clear System Tick counter flag
    SYSTICK_ClearCounterFlag();

    if (Cur_State == ENABLE)
    {
       //GPIO_SetValue(1, LED1 | LED2 | LED3 | LED4);
       LEDdebug(0b0110);
       Cur_State = DISABLE;
    }
    else
    {
       //GPIO_ClearValue(1, LED1 | LED2 | LED3 | LED4);
       LEDoff();
       Cur_State = ENABLE;
    }
}

void delay() 
{
   unsigned int i;
 
   for (i = 0; i < 0x800000; i++);
}

void delayS(uint32_t time)
{
	LEDdebug(0b1000);
	delay();
    //Initialize System Tick with some time interval
    SYSTICK_InternalInit(200);
    //Enable System Tick interrupt
    //SYSTICK_IntCmd(ENABLE);
    //Enable System Tick Counter
    LEDdebug(0b1100);
    delay();
    SYSTICK_Cmd(ENABLE);
    LEDdebug(0b1110);
    delay();
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
    //LEDdebugR(0b1001);
    delayS(100);
    LEDoff();
}

void stage4()
{
	serial_init();
	write_usb_serial_blocking("Hello World!\n\r",16);
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
