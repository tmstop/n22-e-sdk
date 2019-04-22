/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

/* TODO Add any manufacture supplied header files can be included
here. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 #include<math.h> 
#include "n22/drivers/n22_func.h"
#include "soc/drivers/soc.h"
#include "soc/drivers/board.h"
#include "n22/drivers/riscv_encoding.h"
#include "n22/drivers/n22_tmr.h"

#include "portmacro.h"

#include "lcd/oled.h"
#include "lcd/bmp.h"
#include "bldc/bldc.h"
#include "mpu6050/ioi2c.h"
#include "mpu6050/MPU6050.h"
#include "mpu6050/filter.h"
/* Priorities at which the tasks are created.  The event semaphore task is
given the maximum priority of ( configMAX_PRIORITIES - 1 ) to ensure it runs as
soon as the semaphore is given. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )
#define mainEVENT_SEMAPHORE_TASK_PRIORITY   ( configMAX_PRIORITIES - 1 )


/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS        pdMS_TO_TICKS( 10 )


#define mainQUEUE_LENGTH                    ( 1 )
static void prvSetupHardware( void );
extern void idle_task(void);

u8 ref=0;
u16 vx=15542,vy=11165; 
u16 chx=140,chy=146;
#define PI 3.14159265
float Angle_Balance; 

uint8_t adc;
/* The queue used by the queue send and queue receive tasks. */
static QueueHandle_t xQueue = NULL;


static void vExampleTimerCallback( TimerHandle_t xTimer );

void config_eclic_irqs (){
  //time_init   in port.c
    eclic_disable_interrupt(ECLIC_INT_DEVICE_zeroA);
    eclic_disable_interrupt(ECLIC_INT_DEVICE_zeroB);
    eclic_disable_interrupt(ECLIC_INT_DEVICE_zeroC);//eclic_disable_interrupt
    eclic_set_nlbits(4);
  //  The button have higher level
    eclic_set_int_level(CLIC_INT_TMR, 1 << 4);
    eclic_set_int_level(ECLIC_INT_DEVICE_zeroA, 12 << 4);
    eclic_set_int_level(ECLIC_INT_DEVICE_zeroB, 12<< 4);
    eclic_set_int_level(ECLIC_INT_DEVICE_zeroC, 12 << 4);
 } 

void prvSetupHardware( void )
{
    IIC_Init();
    Adc_Init();
    MPU6050_initialize(); 
    DS18B20_Init();
    Lcd_Init(); 
    LCD_Clear(0xFCFCFC);
    showimage(); 
    config_eclic_irqs();
    pwm_init();
    ZERO_init();
    Anwerfen(100);
}

void string_dis()
{

	
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
  LCD_ShowString(10,100,"temperature:");
  LCD_ShowString(10,132,"ADC:");
  LCD_ShowString(10,164,"Angle_X:"); 
  LCD_ShowString(10,196,"Angle_Y:"); 
}
void sen_dis()
{
  short temperature; 
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
  temperature=DS18B20_Get_Temp();
  adc= Get_Adc();

	LCD_ShowNum(106,100,temperature/10,2);
	LCD_ShowString(122,100,".");
	LCD_ShowNum(130,100,temperature%10,1);

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
void showimage() 
{
  	int i,j,k;
	  Address_set(1,1,80,200);
		for(i=530;i<3000;i++)
		{
      LCD_WR_DATA8(image[i*6+2]);
			LCD_WR_DATA8(image[i*6+1]);
			LCD_WR_DATA8(image[i*6]);
      LCD_WR_DATA8(image[i*6+3]);
			LCD_WR_DATA8(image[i*6+4]);
			LCD_WR_DATA8(image[i*6+5]);
		
		}
	ref=0;
}
void Get_Angle()
{ 
	    float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_X;
	    
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
       
		  Gyro_Y=Gyro_Y/16.4;                                         //陀螺仪量程转换	
      Gyro_X=Gyro_X/16.4;                                         //陀螺仪量程转换
		                                   	
      Yijielvbo_X(Accel_X_atan,-Gyro_X);	
			Yijielvbo_Y(Accel_Y_atan,-Gyro_Y);
	    Angle_Balance=angle_Y;                                      //更新平衡倾角
      
	  	
}


TaskHandle_t StartTask_Handler;
TaskHandle_t StartTask2_Handler;

void start_task(void *pvParameters);
void start_task2(void *pvParameters);

int main(void)
{

	TimerHandle_t xExampleSoftwareTimer = NULL;
 
    /* Configure the system ready to run the demo.  The clock configuration
    can be done here if it was not done before main() was called. */
    
    
    xQueue = xQueueCreate(     /* The number of items the queue can hold. */
                            mainQUEUE_LENGTH,
                            /* The size of each item the queue holds. */
                            sizeof( uint32_t ) );

    if(xQueue == NULL)	{
    	for(;;);
    }
    xTaskCreate((TaskFunction_t )start_task,            
                (const char*    )"start_task",          
                (uint16_t       )521,        
                (void*          )NULL,                  
                (UBaseType_t    )1,       
                (TaskHandle_t*  )&StartTask_Handler);         


     xTaskCreate((TaskFunction_t )start_task2,            
                (const char*    )"start_task2",          
                (uint16_t       )521,        
                (void*          )NULL,               
                (UBaseType_t    )2,       
                (TaskHandle_t*  )&StartTask2_Handler); 
 

 
   xExampleSoftwareTimer = xTimerCreate(     
                                            ( const char * ) "LEDTimer",
                                            
                                            mainSOFTWARE_TIMER_PERIOD_MS,
                                            
                                            pdTRUE,
                                            
                                            ( void * ) 0,
                                            
                                            vExampleTimerCallback
                                        );
    xTimerStart( xExampleSoftwareTimer, 0 );  
   
    prvSetupHardware();
    vTaskStartScheduler();
   
    printf("post   ok \n");
    
    for( ;; )
    {
     
    };
}

void start_task(void *pvParameters)
{
    
  string_dis();
    while(1)
    {
	    sen_dis();  
      vTaskDelay(10);
    }
}   

void start_task2(void *pvParameters)
{
   
    while(1)
    {
        adc=Get_Adc();
	      PWM=adc+50;
    
        vTaskDelay(20);
    }
}   


static void vExampleTimerCallback( TimerHandle_t xTimer )
{
    /* The timer has expired.  Count the number of times this happens.  The
    timer that calls this function is an auto re-load timer, so it will
    execute periodically. */
   
   Get_Angle();
   
   

}
void zeroA_HANDLER()
{
  SIGNAL();
  printf("a\n");
  GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroA__GPIO_OFFSET);
}
void zeroB_HANDLER()
{
  SIGNAL();
  printf("b\n");
  GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroB__GPIO_OFFSET);
}
void zeroC_HANDLER()
{
  SIGNAL();
  printf("c\n");
  GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
}


void vApplicationTickHook( void )
{
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
static uint32_t ulCount = 0;

    /* The RTOS tick hook function is enabled by setting configUSE_TICK_HOOK to
    1 in FreeRTOSConfig.h.

    "Give" the semaphore on every 500th tick interrupt. */
    // xSemaphoreGiveFromISR( xEventSemaphore, &xHigherPriorityTaskWoken );
    //    ulCount = 0UL;

   // 	GPIO_REG(GPIO_OUTPUT_VAL)  ^=   (0x1 << GREEN_LED_GPIO_OFFSET) ;
    	

    /* If xHigherPriorityTaskWoken is pdTRUE then a context switch should
    normally be performed before leaving the interrupt (because during the
    execution of the interrupt a task of equal or higher priority than the
    running task was unblocked).  The syntax required to context switch from
    an interrupt is port dependent, so check the documentation of the port you
    are using.

    In this case, the function is running in the context of the tick interrupt,
    which will automatically check for the higher priority task to run anyway,
    so no further action is required. */
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* The malloc failed hook is enabled by setting
    configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

    Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	write(1,"malloc failed\n", 14);
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
  //  ( void ) pcTaskName;
   // ( void ) xTask;

    /* Run time stack overflow checking is performed if
    configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected.  pxCurrentTCB can be
    inspected in the debugger if the task name passed into this function is
    corrupt. */
    write(1, "Stack Overflow\n", 15);
    for( ;; );
}
/*-----------------------------------------------------------*/

extern UBaseType_t uxCriticalNesting;
void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;
// wait_seconds(2);
 //printf(", the mstatus is 0x%x \n", read_csr(mstatus));
// printf(", the mepc is 0x%x\n", read_csr(mepc));
   // idle_task();//printf("In trap handler, the mstatus is 0x%x \n", read_csr(mstatus));
    /* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
    FreeRTOSConfig.h.

    This function is called on each cycle of the idle task.  In this case it
    does nothing useful, other than report the amount of FreeRTOS heap that
    remains unallocated. */
  //  xFreeStackSpace = xPortGetFreeHeapSize();
 // printf(" the mie is 0x%x\n",(read_csr(mstatus)&MSTATUS_MIE));
 //s printf( "idle_task\n");
  //      eclic_enable_interrupt (CLIC_INT_TMR);
  //  set_csr(mstatus, MSTATUS_MIE);
   // if( xFreeStackSpace > 100 )
   // {
        /* By now, the kernel has allocated everything it is going to, so
        if there is a lot of heap remaining unallocated then
        the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
        reduced accordingly. */
  //  }
}
