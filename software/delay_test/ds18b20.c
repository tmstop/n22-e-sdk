// See LICENSE for license details.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "stdatomic.h"

#include "n22/drivers/n22_func.h"
#include "soc/drivers/soc.h"
#include "soc/drivers/board.h"
#include "n22/drivers/riscv_encoding.h"
#include "n22/drivers/n22_tmr.h"
#include "n22/drivers/n22_clic.h"
#define ds18b20__GPIO_OFFSET 15


#define DS18B20_IO_IN()  {GPIO_REG(GPIO_OUTPUT_EN)  &=~(0x1 << ds18b20__GPIO_OFFSET );GPIO_REG(GPIO_INPUT_EN)   |=  (0x1 << ds18b20__GPIO_OFFSET); }
#define DS18B20_IO_OUT() {GPIO_REG(GPIO_OUTPUT_EN)  |= (0x1 << ds18b20__GPIO_OFFSET );GPIO_REG(GPIO_INPUT_EN)   &= ~ (0x1 << ds18b20__GPIO_OFFSET);}

#define TMR_FREQ   32768
void delay_ms ( unsigned int t) //延时1MS {
{
	volatile unsigned int  repeatCount = 1000;
     repeatCount=t;
  while (repeatCount--) ;

}
void delayus (unsigned  int t) //延时1uS {
{
	volatile unsigned int  repeatCount = 1000;
     repeatCount=t;
  while (repeatCount--) ;

}


void DS18B20_DQ_OUT_H(void)	   
{      
  GPIO_REG(GPIO_OUTPUT_VAL)  |=  (0x1 << ds18b20__GPIO_OFFSET) ;
}    


void DS18B20_DQ_OUT_L(void)	   
{      
  GPIO_REG(GPIO_OUTPUT_VAL)  &= ~(0x1 << ds18b20__GPIO_OFFSET) ;
}     


unsigned char DS18B20_DQ_IN(void)	   
{      
  unsigned char x;
  x=(GPIO_REG(GPIO_INPUT_VAL) & (0x1 << ds18b20__GPIO_OFFSET))&&1;
  return x;
}  

void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
  DS18B20_DQ_OUT_L();
  delayus(750*3.19); 	 //750us
  DS18B20_DQ_OUT_H(); //DQ=1 
	delayus(15*3.19);     //15US
}

unsigned char DS18B20_Check(void) 	   
{   
	unsigned char retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN()&&retry<200)
	{
		retry++;
		delayus(3);
	};	 
	if(retry>=200)return 1;

	else retry=0;
    while (!DS18B20_DQ_IN()&&retry<240)
	{
		retry++;
		delayus(3);
	};
	if(retry>=240)return 1;	    
	return 0;
}
unsigned char DS18B20_Read_Bit(void) 			 // read one bit
{
  unsigned char data;
	DS18B20_IO_OUT();//SET PA0 OUTPUT
  DS18B20_DQ_OUT_L(); 
	delayus(2*3.19);
  DS18B20_DQ_OUT_H();
	DS18B20_IO_IN();//SET PA0 INPUT
	delayus(12*3.19);
	if(DS18B20_DQ_IN())
    data=1;
  else
    data=0;	 
  delayus(50*3.19);           
  return data;
}
unsigned char  DS18B20_Read_Byte(void)    // read one byte
{        
  unsigned char  i,j,dat;
  dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
void DS18B20_Write_Byte(unsigned char dat)     
{             
  unsigned char j;
  unsigned  char testb;
	DS18B20_IO_OUT();//SET  OUTPUT;
  for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT_L();// Write 1
            delayus(2*3);                            
            DS18B20_DQ_OUT_H();
            delayus(60*3.19);             
        }
        else 
        {
            DS18B20_DQ_OUT_L();// Write 0
            delayus(60*3.19);              
            DS18B20_DQ_OUT_H();
             delayus(2*3);                          
        }
    }
}
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
  	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 
unsigned char DS18B20_Init(void)
{
  GPIO_REG(GPIO_OUTPUT_EN)  |= (0x1 << ds18b20__GPIO_OFFSET );
  GPIO_REG(GPIO_INPUT_EN)   &= ~ (0x1 << ds18b20__GPIO_OFFSET);
  GPIO_REG(GPIO_OUTPUT_VAL)  |=  (0x1 << ds18b20__GPIO_OFFSET) ;
	DS18B20_Rst();
	return DS18B20_Check();
}  
short DS18B20_Get_Temp(void)
{
    unsigned char temp;
    unsigned char TL,TH;
	  short tem;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;
    }
    else temp=1;  	  
    
    tem=TH; 
    tem<<=8;    
    tem+=TL;
    tem=(float)tem*0.625;   
	if(temp)return tem; 
	else return -tem;    
} 

