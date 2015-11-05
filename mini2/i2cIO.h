
#include "lcd.h"

#define RESET 10
#define COMPLETED 20

void i2c_init();
void listAllDevices();
void printOnLCD(char *outStr);
void registerKeyboardInterrupt();
int i2cTransferM(int address, uint8_t *msgSend, int lengthSend, uint8_t *msgReceive, int lengthReceive, int type);
