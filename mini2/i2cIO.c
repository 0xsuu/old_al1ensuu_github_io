
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"

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

//Global Variable for interrupt
uint8_t masterBuffer;
uint8_t slaveBuffer;

int interruptStatus = RESET;

void I2C1_IRQHandler(void)
{
    // just call std int handler
	I2C_MasterHandler(LPC_I2C1);
	if (I2C_MasterTransferComplete(LPC_I2C1))
	{
		serialPrintWithInt("Received: %\n\r", slaveBuffer);
		serialPrint("INTERRUPTED\n\r");
        interruptStatus = COMPLETED;
    }
    
    // just call std int handler
	I2C_SlaveHandler(LPC_I2C1);
	if (I2C_SlaveTransferComplete(LPC_I2C1))
	{
		serialPrint("INTERRUPTED\n\r");
        interruptStatus = COMPLETED;
    }
}

void registerKeyboardInterrupt()
{
	uint8_t data = 0;
	uint32_t dataReceive = 0;
	
	I2C_M_SETUP_Type transferMCfg;
    I2C_S_SETUP_Type transferSCfg;

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
	
	/* Then start I2C master device */
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
	
    highBits = 0x0f;
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
	
	while (interruptStatus == RESET)
	{
		//serialPrintWithInt("Received: %\n\r", slaveBuffer);
		delayS(1);
	}
}


