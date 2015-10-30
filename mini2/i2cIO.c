
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"

#include "i2cIO.h"
#include "serialIO.h"
#include "led.h"
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

void listAllDevices()
{
	int addr ;
	//uint8_t result = 0;
	uint8_t data = 0;
	int count_devices =0;
	
	serialPrint("All Devices List:\n\r");

	I2C_M_SETUP_Type transferCfg;
		
	//transferCfg.sl_addr7bit =		//slave address in 7bit mode
	transferCfg.tx_data = &data ;		
	transferCfg.tx_length =	sizeof(uint8_t);
	transferCfg.rx_data = NULL;
    transferCfg.rx_length = 0;
	transferCfg.retransmissions_max = 3;	
	//transferCfg.tx_count = 0;	
	//transferCfg.rx_data = NULL;			
	//transferCfg.rx_length = 0;		
	//transferCfg.rx_count = 0;			
	//transferCfg.retransmissions_max = 		
	//transferCfg.retransmissions_count = 	
	//transferCfg.status = 	0;				//current status
		
	//I2C_MasterTransferData((LPC_I2C_TypeDef *) LPC_I2C1, &transferCfg, I2C_TRANSFER_POLLING );
	
	for (addr=0; addr<= 127; addr++)
	{
		transferCfg.sl_addr7bit = addr;
		if (I2C_MasterTransferData(LPC_I2C1, &transferCfg, I2C_TRANSFER_POLLING ))
		{
			count_devices ++;
			transferCfg.sl_addr7bit = addr;
			serialPrintWithInt("Device NO.%\n\r", count_devices);
			serialPrintWithInt("\tAddr: %\n\r", addr);
		}
	}
	serialPrintWithInt("\n\rCount: %\n\r", count_devices);
}

void printOnLCD(char *outStr)
{		
	//Setup string length
	uint32_t length = stringLength(outStr);
	
	//Setup data
	uint8_t data[24];
	data[0] = 0x00;
	data[1] = 0x24;
	data[2] = 0x40;
	data[3] = 0x40;
	data[4] = 0x06;
	data[5] = 0x09;
	data[6] = 0x08;
	data[7] = 0x1e;
	data[8] = 0x1e;
	data[9] = 0x08;
	data[10] = 0x09;
	data[11] = 0x06;
	data[12] = 0x00;
	data[13] = 0x00;
	data[14] = 0x26;
	data[15] = 0x06;
	data[16] = 0x08;
	data[17] = 0x40;
	data[18] = 0xa0;
	data[19] = 0x24;
	data[20] = 0x0c;
	data[21] = 0x06;
	data[22] = 0x02;
	data[23] = 0x01;
	
	
	//Configure i2c transfer
	I2C_M_SETUP_Type transferCfg;
		
	transferCfg.sl_addr7bit = 0x3B;	//address of LCD
			
	transferCfg.tx_length =	24*sizeof(uint8_t);
	transferCfg.rx_data = NULL;
    transferCfg.rx_length = 0;
	transferCfg.retransmissions_max = 3;
	
	transferCfg.tx_data = data;
	serialPrint("Printing...\n\r");
	if (I2C_MasterTransferData(LPC_I2C1, &transferCfg, I2C_TRANSFER_POLLING))
	{
		LEDdebug(0b0010);
		delayS(1);
		LEDoff();
	}
	else
	{
		LEDdebug(0b1111);
		delayS(1);
		LEDoff();
	}	
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


