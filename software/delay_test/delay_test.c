// See LICENSE for license details.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "stdatomic.h"
#include "n22/drivers/n22_func.h"
#include "soc/drivers/soc.h"
#include "soc/drivers/i2c.h"
#include "soc/drivers/board.h"
#include "n22/drivers/riscv_encoding.h"
#include "n22/drivers/n22_tmr.h"
#include "n22/drivers/n22_clic.h"

#include "oled.h"
#include "bmp.h"
#include "bldc.h"
#include "ioi2c.h"
#include "MPU6050.h"
#include "filter.h"
#include "DataScope_DP.h"
#define ECLIC_INT_DEVICE_zeroA 27
#define ECLIC_INT_DEVICE_zeroB 35
#define ECLIC_INT_DEVICE_zeroC 34

#define PI 3.14159265
float Angle_Balance; 
__uint8_t adc,bldc_mode=0;
//extern unsigned int  PWM=0;
void sen_dis()
{
  short temperature; 
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
  temperature=DS18B20_Get_Temp();
  //adc= Get_Adc();
 if(temperature)
 {
	LCD_ShowNum(106,100,temperature/10,2);
	LCD_ShowString(122,100,".");
	LCD_ShowNum(130,100,temperature%10,1);
}
	LCD_ShowNum(106,132,adc,3);
  if(angle_X<0)
  {
    LCD_ShowString(98,164,"-");
    LCD_ShowNum(106,164,abs(angle_X),3);
  }
  else
  {
    LCD_ShowString(98,164," ");
    LCD_ShowNum(106,164,angle_X,3);
  }
  if(angle_Y<0)
  {
    LCD_ShowString(98,198,"-");
    LCD_ShowNum(106,198,abs(angle_Y),3);
  }
  else
  {
    LCD_ShowString(98,198," ");
    LCD_ShowNum(106,198,angle_Y,3);
  }
}
void config_eclic_irqs()
{

  // Have to enable the interrupt both at the GPIO level,
  // and at the CLIC level.
 // eclic_enable_interrupt (CLIC_INT_TMR);
 

  eclic_disable_interrupt(ECLIC_INT_DEVICE_zeroA);
 eclic_disable_interrupt(ECLIC_INT_DEVICE_zeroB);
  eclic_disable_interrupt(ECLIC_INT_DEVICE_zeroC);//eclic_disable_interrupt
  eclic_set_nlbits(4);
  //  The button have higher level
 // eclic_set_int_level(CLIC_INT_TMR, 1 << 4);
  eclic_set_int_level(ECLIC_INT_DEVICE_zeroA, 9<< 4);
  eclic_set_int_level(ECLIC_INT_DEVICE_zeroB, 9<< 4);
  eclic_set_int_level(ECLIC_INT_DEVICE_zeroC, 9 << 4);
}

void setup_mtime (){

    // Set the machine timer to go off in 2 seconds.
    volatile uint64_t * mtime    = (uint64_t*) (TMR_CTRL_ADDR + TMR_MTIME);
    volatile uint64_t * mtimecmp = (uint64_t*) (TMR_CTRL_ADDR + TMR_MTIMECMP);
    uint64_t now = *mtime;
    uint64_t then = now + 0.02* TMR_FREQ;
    *mtimecmp = then;
   // Get_Angle();
   
}

void string_dis()
{
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
  LCD_ShowString(10,100,"temperature:");
  LCD_ShowString(10,132,"ADC:");
  LCD_ShowString(10,164,"Angle_X:"); 
  LCD_ShowString(10,196,"Angle_Y:"); 
  LCD_ShowString(100,16,"BLDC_CTRL_MODE:"); 
  if(bldc_mode==1)
  {
    LCD_ShowString(260,16,"angleY");
  }
  else
  {
    LCD_ShowString(260,16,"ADC");
  }
  
}

void Get_Angle()
{ 
	    double  Accel_Y,Accel_X=0,Accel_Z=0,Gyro_Y,Gyro_X;
      unsigned char Send_Count; //串口需要发送的数据个数

			float  Accel_X_atan,Accel_Y_atan;
      Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪
			Gyro_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L);
      Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L);    //读取Z轴陀螺仪
		  Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度记
	 		Accel_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L);
      if(Gyro_Y>32768)  Gyro_Y-=65536;     //数据类型转换  也可通过short强制类型转换
      if(Gyro_X>32768)  Gyro_X-=65536;     //数据类型转换  也可通过short强制类型转换
		  if(Accel_Z>32768)  Accel_Z-=65536;     //数据类型转换
	  	if(Accel_X>32768) Accel_X-=65536;    //数据类型转换
      if(Accel_Y>32768) Accel_Y-=65536;

	   	Accel_Y_atan=atan2(Accel_X,Accel_Z)*180/PI;                 //计算与地面的夹角	
	   	Accel_X_atan=atan2(Accel_Y,Accel_Z)*180/PI;                 //计算与地面的夹角	
       
		  Gyro_Y=Gyro_Y/16.4;                                    //陀螺仪量程转换	
      Gyro_X=Gyro_X/16.4; 
		  Gyro_Y=Gyro_Y/16.4;                                    //陀螺仪量程转换	
      Yijielvbo_X(Accel_X_atan,-Gyro_X);	
			Yijielvbo_Y(Accel_Y_atan,-Gyro_Y);
	    Angle_Balance=angle_Y;                                  //更新平衡倾角
      
	      while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
          UART0_REG(UART_REG_TXFIFO) =0x03 ;
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
        UART0_REG(UART_REG_TXFIFO) =0xfc ;
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
        UART0_REG(UART_REG_TXFIFO) =Accel_X_atan+100;
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
        UART0_REG(UART_REG_TXFIFO) =angle_Y+100;
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
        UART0_REG(UART_REG_TXFIFO) =angle_X+100;
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
        UART0_REG(UART_REG_TXFIFO) =0xfc ;
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
          UART0_REG(UART_REG_TXFIFO) =0x03 ;
}

void MTIME_HANDLER(){


  volatile uint64_t * mtime       = (uint64_t*) (TMR_CTRL_ADDR + TMR_MTIME);
  volatile uint64_t * mtimecmp    = (uint64_t*) (TMR_CTRL_ADDR + TMR_MTIMECMP);
  uint64_t now = *mtime;
  uint64_t then = now + 0.01 * TMR_FREQ;// Here we set 1 second, but we need to wait 5 cycles to get out from this handler, so the blink will not toggle as 1 cycle
  *mtimecmp = then;
 // Get_Angle();
}


__uint8_t Get_Adc(void)
{
  __uint8_t value;
  SPI1_REG(SPI_REG_TXFIFO)=0x00;
  value=SPI1_REG(SPI_REG_RXFIFO);
  return value;
}

void main(void)
{

  short temperature;
  unsigned char x,y;
  unsigned char adc_value;
   GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1 << 30)|(0x1 << 26));
	  GPIO_REG(GPIO_OUTPUT_EN) |= ((0x1 << 30)|(0x1 << 26)); 
  Lcd_Init(); 
  LCD_Clear(0xFCFCFC);
  DS18B20_Init();
  pwm_init();
  ZERO_init();
  IIC_Init();
  Adc_Init();
  MPU6050_initialize(); 
  config_eclic_irqs();
  setup_mtime ();
  
 // showimage(); 
  string_dis();
  Anwerfen(100);
  
  set_csr(mstatus, MSTATUS_MIE);

  while (1)
  {
    /* code */
     sen_dis();
  temperature=DS18B20_Get_Temp();
//  
  
   adc=Get_Adc();
   
      PWM=adc/2+50;
  
    if(PWM>100) PWM=100; 
    
   // printf("PWM=  %d\n  ",PWM);
 //   temperature = DS18B20_Get_Temp();
  //  printf("temperature=%d.%d\n",temperature/10,temperature%10);
  //  Get_Angle();
//  printf("Pitch:  %f\r\n  ",Angle_Balance);
 
 //adc_value=Get_Adc();
 //   PWM=adc_value+50;
 printf("adc=  %d\n  ",adc);
  }
}


void zeroA_HANDLER()
{
GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 26);

  SIGNAL();
//  printf("a\n");
 
  //  if(GPIO_REG(GPIO_RISE_IE) &(0x1 << zeroA__GPIO_OFFSET))
    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroA__GPIO_OFFSET);
  //  else if(GPIO_REG(GPIO_FALL_IE) &(0x1 << zeroA__GPIO_OFFSET))
    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroA__GPIO_OFFSET);
}
void zeroB_HANDLER()
{
GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 26);
   
 
  SIGNAL();
  
//  printf("b\n");

  //  if(GPIO_REG(GPIO_RISE_IE) &(0x1 << zeroB__GPIO_OFFSET))
    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroB__GPIO_OFFSET);
  // else if(GPIO_REG(GPIO_FALL_IE) &(0x1 << zeroA__GPIO_OFFSET))
    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroA__GPIO_OFFSET);

}
void zeroC_HANDLER()
{
 GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 26);
 
  SIGNAL();
//  printf("c\n");
  
  // if(GPIO_REG(GPIO_RISE_IE) &(0x1 << zeroC__GPIO_OFFSET))
    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
 //   else if(GPIO_REG(GPIO_FALL_IE) &(0x1 << zeroC__GPIO_OFFSET))
    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET);
}
