
#include "stringProcess.h"

int stringLength(char targetStr[])
{
    int i = 0;
    unsigned short character = targetStr[0];
    while (character != '\x00')
    {
        i++;
        character = targetStr[i];
    }
    return i;
}
/*
char * stringCat(char *strA, char *strB)
{

    char retStr[(stringLength(strA)+stringLength(strB))];
    
    int i;
    for (i = 0; strA[i]!='\x00'; i++)
    {
        retStr[i] = strA[i];
    }
    
    int start = i;
    for (i = 0; strB[i]!='\x00'; i++)
    {
        retStr[start+i+1] = strB[i];
    }
    
    return retStr;
}*/

void stringCopy(char dest[], const char source[]) 
{
	int i = 0;
   while (1) 
   {
      dest[i] = source[i];
      if (dest[i] == '\0') break;
      i++;
	} 
}

//Useage: stringWithInt("[String Content]%[String Content]", int)
//Only support one integer
char * stringWithInt(char outStr[], int value)
{
    int length = stringLength(outStr);
    
    int hasInteger = 0;
    int i;
    for (i = 0; i<length; i++)
    {
        if (outStr[i] == '%')
        {
            hasInteger = 1;
            break;
        }
    }
    
    if (hasInteger == 1)
    {
        length += sizeof(int)*2 + 1;
        
        char newOutStr[length];
        int j;
        for (j = 0; j<length; j++)
        {
            if (j < i)
            {
                newOutStr[j] = outStr[j];
            }
            else if (j == i)
            {
                //Add 0x prefix
                newOutStr[j] = '0';
                newOutStr[j+1] = 'x';
                j += 2;
                
                //Convert hex into char, and convert little endian into big endian
                int k;
                
                for (k = sizeof(int); k < sizeof(int)*2; k++)
                {
                    unsigned short addition = 0;
                    if (((value >> ((sizeof(int)-k-1)*4)) & 0xf) > 0x9)
                    {
                        addition = 55;
                    }
                    else
                    {
                        addition = 48;
                    }
                    
                    newOutStr[j+k-sizeof(int)] = ((value >> ((sizeof(int)*3-k-1)*4)) & 0xf) + addition;
                }
                
                for (k = 0; k < sizeof(int); k++)
                {
                    unsigned short addition = 0;
                    if (((value >> ((sizeof(int)-k-1)*4)) & 0xf) > 0x9)
                    {
                        addition = 55;
                    }
                    else
                    {
                        addition = 48;
                    }
                    
                    newOutStr[j+k+sizeof(int)] = ((value >> ((sizeof(int)-k-1)*4)) & 0xf) + addition;
                }
                j += sizeof(int)*2 - 1;
            }
            else
            {
            	
                newOutStr[j] = outStr[j-sizeof(int)*2-1];
            }
        }
        return &newOutStr;
    }
}
