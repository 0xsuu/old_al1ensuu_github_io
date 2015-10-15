
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

}

void stage3()
{

}

void stage4()
{

}

int main()
{   
    setupLEDs();
    //LEDdebug(0b1010);
    stage1();
    //stage2();
    //stage3();
    //stage4();
    
    return 0;
}
