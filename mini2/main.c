
#include "lpc_types.h"

#include "main.h"
#include "led.h"
#include "serialIO.h"
#include "sysTick.h"
#include "i2cIO.h"

void stage1()
{
	listAllDevices();
}

void stage2()
{
	printOnLCD("A");
}

void stage3()
{
	registerKeyboardInterrupt();
}

void stage4()
{

}

int main()
{   
    setupLEDs();
    i2c_init();
	serial_init();

    //stage1();
    //stage2();
    stage3();
    //stage4();
    
    return 0;
}
