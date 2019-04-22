#ifndef BLDC_H
#define BLDC_H
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

#define MOTO_C_H_GPIO_OFFSET 11     //MOTO_C_H
#define MOTO_C_L_GPIO_OFFSET 10    //MOTO_C_L
#define MOTO_A_L_GPIO_OFFSET 20    //MOTO_A_L
#define MOTO_A_H_GPIO_OFFSET 19    //MOTO_A_H
#define MOTO_B_H_GPIO_OFFSET 21    //MOTO_B_H
#define MOTO_B_L_GPIO_OFFSET 22    //MOTO_B_L

#define zeroA__GPIO_OFFSET 1
#define zeroB__GPIO_OFFSET 9
#define zeroC__GPIO_OFFSET 8

#define ECLIC_INT_DEVICE_zeroA 27
#define ECLIC_INT_DEVICE_zeroB 35
#define ECLIC_INT_DEVICE_zeroC 34
#define zeroA_HANDLER clic_irq27_handler
#define zeroB_HANDLER clic_irq35_handler
#define zeroC_HANDLER clic_irq34_handler
#define MANUELL_PWM       30  // PWM的值: 0-255

#define PWM_C_ON  {PWM1_REG(PWM_CMP1)= 0;PWM1_REG(PWM_CMP2)= 0;PWM2_REG(PWM_CMP1)= PWM;} 
#define PWM_B_ON  {PWM2_REG(PWM_CMP1)= 0;PWM1_REG(PWM_CMP1)= 0;PWM1_REG(PWM_CMP2)= PWM;} 
#define PWM_A_ON  {PWM2_REG(PWM_CMP1)= 0;PWM1_REG(PWM_CMP2)= 0;PWM1_REG(PWM_CMP1)= PWM;} 
#define PWM_OFF   {PWM2_REG(PWM_CMP1)= 0;PWM1_REG(PWM_CMP1)= 0;PWM1_REG(PWM_CMP2)= 0;}

#define  STEUER_A_H {PWM_A_ON}
#define  STEUER_B_H {PWM_B_ON}
#define  STEUER_C_H {PWM_C_ON}

#define  STEUER_A_L {GPIO_REG(GPIO_OUTPUT_VAL) &=~ ((0x1<< MOTO_C_L_GPIO_OFFSET)|(0x1<< MOTO_B_L_GPIO_OFFSET)); GPIO_REG(GPIO_OUTPUT_VAL) |= ((0x1<< MOTO_A_L_GPIO_OFFSET));  }
#define  STEUER_B_L { GPIO_REG(GPIO_OUTPUT_VAL) &=~ ((0x1<< MOTO_C_L_GPIO_OFFSET)|(0x1<< MOTO_A_L_GPIO_OFFSET));GPIO_REG(GPIO_OUTPUT_VAL) |= ((0x1<< MOTO_B_L_GPIO_OFFSET)); }
#define  STEUER_C_L { GPIO_REG(GPIO_OUTPUT_VAL) &=~ ((0x1<< MOTO_A_L_GPIO_OFFSET)|(0x1<< MOTO_B_L_GPIO_OFFSET));GPIO_REG(GPIO_OUTPUT_VAL) |= ((0x1<< MOTO_C_L_GPIO_OFFSET));  }
#define  STEUER_OFF GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~((0x1<< MOTO_C_L_GPIO_OFFSET) | (0x1<< MOTO_A_L_GPIO_OFFSET) | (0x1 << MOTO_B_L_GPIO_OFFSET)) ;

#define  SENSE_A {eclic_disable_interrupt (ECLIC_INT_DEVICE_zeroB);eclic_disable_interrupt (ECLIC_INT_DEVICE_zeroC);eclic_enable_interrupt (ECLIC_INT_DEVICE_zeroA);}
#define  SENSE_B {eclic_disable_interrupt (ECLIC_INT_DEVICE_zeroA);eclic_disable_interrupt (ECLIC_INT_DEVICE_zeroC);eclic_enable_interrupt (ECLIC_INT_DEVICE_zeroB);}
#define  SENSE_C {eclic_disable_interrupt (ECLIC_INT_DEVICE_zeroA);eclic_disable_interrupt (ECLIC_INT_DEVICE_zeroB);eclic_enable_interrupt (ECLIC_INT_DEVICE_zeroC);}

#define  SENSE_A_RISING_INT  {GPIO_REG(GPIO_RISE_IE) |= (1 << zeroA__GPIO_OFFSET);GPIO_REG(GPIO_FALL_IE) &=~(1 << zeroA__GPIO_OFFSET);} 
#define  SENSE_B_RISING_INT  {GPIO_REG(GPIO_RISE_IE) |= (1 << zeroB__GPIO_OFFSET);GPIO_REG(GPIO_FALL_IE) &=~(1 << zeroB__GPIO_OFFSET);} 
#define  SENSE_C_RISING_INT  {GPIO_REG(GPIO_RISE_IE) |= (1 << zeroC__GPIO_OFFSET);GPIO_REG(GPIO_FALL_IE) &=~(1 << zeroC__GPIO_OFFSET);} 

#define  SENSE_A_FALLING_INT  {GPIO_REG(GPIO_RISE_IE) &= ~(1 << zeroA__GPIO_OFFSET);GPIO_REG(GPIO_FALL_IE) |=(1 << zeroA__GPIO_OFFSET);} 
#define  SENSE_B_FALLING_INT  {GPIO_REG(GPIO_RISE_IE) &= ~ (1 << zeroB__GPIO_OFFSET);GPIO_REG(GPIO_FALL_IE) |=(1 << zeroB__GPIO_OFFSET);} 
#define  SENSE_C_FALLING_INT  {GPIO_REG(GPIO_RISE_IE) &= ~ (1 << zeroC__GPIO_OFFSET);GPIO_REG(GPIO_FALL_IE) |=(1 << zeroC__GPIO_OFFSET);} 

extern void pwm_init(void);
extern  void ZERO_init (void);
extern void Anwerfen(unsigned char pwm);
extern  void Delay_ms(size_t n); 
extern void delay_us ( int t); //延时1US   test
extern unsigned int  PWM;


#endif /* BLDC_H */