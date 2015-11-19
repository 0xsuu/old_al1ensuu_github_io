
#ifndef LCD_H
#define LCD_H

void lcdClearScreen();
void lcd_init();
void lcdPrint(char *outStr);
void lcdPut(uint8_t outChar);
//int lcdPrintWithInt(char *outStr, uint32_t value);

#endif
