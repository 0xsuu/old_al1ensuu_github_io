
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"

#include "i2cIO.h"
#include "serialIO.h"

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
	serial_init();
	serialPrint("All Devices List:\n\r");

	i2c_init();
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
	/*int addr;
	//uint8_t result = 0;
	uint8_t data = 0;
	int count_devices =0;
	serial_init();
	
	i2c_init();
	I2C_M_SETUP_Type *transferCfg;
	
	//printWithInt("I2C Loaded\n\r", 1);
		
	transferCfg->tx_data = &data;		
	transferCfg->tx_length = 1;	
	
	for (addr=1; addr<= 127; addr++)
	{
		//printWithInt("Looping %\n\r", transferCfg);
		transferCfg->sl_addr7bit = addr;
		
		if (I2C_MasterTransferData((LPC_I2C_TypeDef *)LPC_I2C1, transferCfg, I2C_TRANSFER_POLLING))
		{
			count_devices ++;
			transferCfg->sl_addr7bit = addr;
			printWithInt("Device NO.%\n\r", count_devices);
			printWithInt("\tAddr: %\n\r", addr);
		}
	}
	printWithInt("Count: %\n\r", count_devices);*/
}
