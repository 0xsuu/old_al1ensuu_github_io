
#include "lpc17xx_gpio.h" //GPIO header

#include "led.h"

void setupLEDs()
{
    GPIO_SetDir(1, LED1 | LED2 | LED3 | LED4, 1);
    GPIO_ClearValue(1, LED1 | LED2 | LED3 | LED4);
}

void LEDoff()
{
	GPIO_ClearValue(1, LED1 | LED2 | LED3 | LED4);
}

void LEDdebug(uint8_t num)
{
	LEDoff();
    GPIO_SetValue(1, ((num&1) << 23) | (((num>>1) & 1) << 21) | (((num>>2) & 1) << 20) | (((num>>3) & 1) << 18));
}

void LEDdebugN(uint8_t num)
{
	LEDoff();
    GPIO_SetValue(1, ((num&1) << 18) | (((num>>1) & 1) << 20) | (((num>>2) & 1) << 21) | (((num>>3) & 1) << 23));
}

