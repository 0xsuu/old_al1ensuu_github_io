
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_gpio.h"

#include "i2cIO.h"
#include "serialIO.h"
#include "led.h"
#include "lcd.h"
#include "stringProcess.h"
#include "sysTick.h"

void i2c_init()
{
	PINSEL_CFG_Type PinCfg;

	PinCfg.OpenDrain  = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Funcnum = 3;
	
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	
	I2C_Init((LPC_I2C_TypeDef *)LPC_I2C1, 100000);
	//I2C_DeInit((LPC_I2C_TypeDef *);
	I2C_Cmd((LPC_I2C_TypeDef *)LPC_I2C1, ENABLE);
}

int i2cTransferM(int address, uint8_t *msgSend, int lengthSend, uint8_t *msgReceive, int lengthReceive, int type)
{
	I2C_M_SETUP_Type transferCfg;
	
	transferCfg.sl_addr7bit = address;
	transferCfg.tx_data = msgSend;		
	transferCfg.tx_length = lengthSend;
	transferCfg.rx_data = msgReceive;
    transferCfg.rx_length = lengthReceive;
	transferCfg.retransmissions_max = 3;
	
	return I2C_MasterTransferData(LPC_I2C1, &transferCfg, type);
}

void listAllDevices()
{
	int addr;
	uint8_t data = 0;
	int count_devices =0;
	
	serialPrint("All Devices List:\n\r");
	
	for (addr=0; addr<= 127; addr++)
	{
		if (i2cTransferM(addr, &data, 1, NULL, 0, I2C_TRANSFER_POLLING))
		{
			count_devices ++;
			serialPrintWithInt("Device NO.%\n\r", count_devices);
			serialPrintWithInt("\tAddr: %\n\r", addr);
		}
	}
	serialPrintWithInt("\n\rCount: %\n\r", count_devices);
}

uint8_t result[16], inputString[16];
void calc()
{
    int operation = 0;
    int position = -1;
    
    //Get operation type and position
    int i;
    for (i = 0; i < stringLength(inputString); i++)
    {
        if (inputString[i] > '9' || inputString[i] < '0')
        {
            if (i == 0)
            {
                //result[0] = (uint8_t *)"Error!";
                return;
            }
            else
            {
                operation = inputString[i];
                position = i;
                break;
            }
        }
    }
    int num1 = 0;
    int num2 = 0;
    for (i = 0; i < position; i ++)
    {
        num1 += (inputString[i]-'0');
        num1 *= 10;
    }
    num1/=10;
    
    for (i = position+1; i < stringLength(inputString); i ++)
    {
        num2 += (inputString[i]-'0');
        num2 *= 10;
    }
    num2/=10;
    
    int resultNumber = operation=='+'?num1+num2:(operation=='-'?num1-num2:(operation=='*'?num1*num2:(operation=='/'?num1/num2:0)));
    
    int counter = 0;
    while (resultNumber != 0)
    {
        result[counter] = resultNumber - resultNumber/10 * 10 + '0';
        resultNumber /= 10;
        counter ++;
    }
    
    for (i = 0; i < stringLength(result)/2; i++)
    {
        uint8_t tmp = 0;
        tmp = result[i];
        result[i] = result[stringLength(result)-i-1];
        result[stringLength(result)-i-1] = tmp;
    }
}

//Global Variable for interrupt
uint8_t masterBuffer;
uint8_t slaveBuffer;

uint8_t readKeyPressed()
{
	//Keypad mapping
	const uint8_t row1[4] = {'1', '2', '3', '+'};
	const uint8_t row2[4] = {'4', '5', '6', '-'};
	const uint8_t row3[4] = {'7', '8', '9', '*'};
	const uint8_t row4[4] = {'C', '0', '=', '/'};
	
	const uint8_t *keypad[4] = {row1, row2, row3, row4};
	
	int valid = 1;
	
	//Read Row(lower bits)
    uint8_t highBits = 0xf0;
    i2cTransferM(0x21, &highBits, 1, NULL, 0, I2C_TRANSFER_POLLING);
    i2cTransferM(0x21, NULL, 0, &highBits, 1, I2C_TRANSFER_POLLING);
    if (highBits == 0xf0) valid = 0;
    serialPrintWithInt("Low % ",highBits);
    int y = 0;
    int i;
    highBits = highBits >> 4;
    for (i = 0; i < 4; i ++)
    {
    	if (((highBits >> i) & 1) == 0)
    		y = 3-i;
    }
    
	//Read Column(higher bits)
	highBits = 0x0f;
    i2cTransferM(0x21, &highBits, 1, NULL, 0, I2C_TRANSFER_POLLING);
    i2cTransferM(0x21, NULL, 0, &highBits, 1, I2C_TRANSFER_POLLING);
    if (highBits == 0x0f) valid = 0;
    serialPrintWithInt("High % ",highBits);

    int x = 0;
    for (i = 0; i < 4; i ++)
    {
    	if (((highBits >> i) & 1) == 0)
    		x = 3-i;
    }
    
    if (!valid) return 0;
    else return keypad[x][y];
}

int resultCount = 0;
void EINT3_IRQHandler(void)
{
	if (GPIO_GetIntStatus(0, 23, 1))
    {
        GPIO_ClearInt(0,(1<<23));
        serialPrint("Before Falling\n\r");	
		int i;

		uint8_t prt1, prt2;

		prt1 = readKeyPressed();
		for (i = 0; i < 0x200000; i++);
		prt2 = readKeyPressed();
		//}
		if (prt1 && !prt2)
		{
			if (prt1 == '=')
			{
				calc();
				lcdPrint(" \n= ");
				
				lcdPrint(result);
			}
			else if (prt1 == 'C')
			{
				resultCount = 0;
				int j;
				for (j = 0; j < 16; j++)
				{
					result[j] = 0;
					inputString[j] = 0;
				}
				lcdClearScreen();
			}
			else
			{
				lcdPut(prt1);
				inputString[resultCount] = prt1;
				resultCount++;
			}
		}
		
		
		serialPrint("Falling\n\r");
		//LEDdebug(0b1010);
    }
    /*else if (GPIO_GetIntStatus(0, 23, 0))
    {
    	GPIO_ClearInt(0,(1<<23));

    	lcdClearScreen();
    	serialPrint("Rising\n\r");
    	LEDdebug(0b0101);
    }*/
	
}

void registerKeyboardInterrupt()
{
	lcdClearScreen();
/*NVIC_EnableIRQ(EINT3_IRQn);
	uint8_t data = 0;
	uint32_t dataReceive = 0;
	
	I2C_M_SETUP_Type transferMCfg;
    I2C_S_SETUP_Type transferSCfg;
*/
	/* Start I2C slave device first 

    transferSCfg.tx_data = NULL;
    transferSCfg.tx_length = 0;
    transferSCfg.rx_data = &slaveBuffer;
    transferSCfg.rx_length = sizeof(slaveBuffer);
    if (I2C_SlaveTransferData(LPC_I2C1, &transferSCfg, I2C_TRANSFER_POLLING))
    {
    	serialPrint("Success\n\r");
    }
    else
    {
    	serialPrint("Failed\n\r");
    }
    serialPrintWithInt("Received: %\n\r", slaveBuffer);*/
	
	/* Then start I2C master device 
    transferMCfg.sl_addr7bit = 0x21;
    transferMCfg.retransmissions_max = 3;
    
    uint8_t highBits = 0xf0;
    transferMCfg.tx_data = &highBits;
    transferMCfg.tx_length = 1;
    transferMCfg.rx_data = 0;
    transferMCfg.rx_length = 0;
    I2C_MasterTransferData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_POLLING);
    
    transferMCfg.tx_data = NULL;
    transferMCfg.tx_length = 0;
    transferMCfg.rx_data = &masterBuffer;
    transferMCfg.rx_length = 1;
    
	serialPrint("Sending...\n\r");
	if (I2C_MasterTransferData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_POLLING))
	{
		serialPrint("Successful sending data through I2C\n\r");
		
		serialPrintWithInt("Received: %\n\r", masterBuffer);
	}
	else
	{
		serialPrint("I2C send data Failed\n\r");
		LEDdebug(0b1111);
		delayS(1);
		LEDoff();
	}
	*/
    //uint8_t highBits = 0x0f;
    //i2cTransferM(0x21, &highBits, 1, NULL, 0, I2C_TRANSFER_POLLING);
    /*
    transferMCfg.tx_data = NULL;
    transferMCfg.tx_length = 0;
    transferMCfg.rx_data = &masterBuffer;
    transferMCfg.rx_length = 1;
    
	serialPrint("Sending...\n\r");
	if (I2C_MasterTransferData(LPC_I2C1, &transferMCfg, I2C_TRANSFER_POLLING))
	{
		serialPrint("Successful sending data through I2C\n\r");
		
		serialPrintWithInt("Received: %\n\r", masterBuffer);
	}
	else
	{
		serialPrint("I2C send data Failed\n\r");
		LEDdebug(0b1111);
		delayS(1);
		LEDoff();
	}*/
	
	GPIO_SetDir(0, 1<<23, 0);
	GPIO_SetValue(0, 1<<23);
	GPIO_ClearInt(0, 1<<23);
	GPIO_IntCmd(0,(1<<23),1);
	NVIC_EnableIRQ(EINT3_IRQn);
	
	while (1);
}


