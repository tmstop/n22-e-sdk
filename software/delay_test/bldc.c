// See LICENSE for license details.
#include "bldc.h"


#define MANUELL_PWM       30  // PWM的值: 0-255

volatile unsigned char Phase = 0;
unsigned int  PWM=0;
unsigned int TEST_MANUELL=30;  // 测试硬件  值: 0-255 (PWM)
unsigned int  CntKommutierungen = 0; //换向计数
__uint8_t SENSE_V=0;

void pwm_init(void)
{
  // SET IOF1
  GPIO_REG(GPIO_IOF_SEL) |= ((1<<MOTO_C_H_GPIO_OFFSET)|(1<<MOTO_A_H_GPIO_OFFSET)|(1<<MOTO_B_H_GPIO_OFFSET));
  GPIO_REG(GPIO_IOF_EN) |= ((1<<MOTO_C_H_GPIO_OFFSET)|(1<<MOTO_A_H_GPIO_OFFSET)|(1<<MOTO_B_H_GPIO_OFFSET));
  GPIO_REG(GPIO_OUTPUT_EN) |= ((1<<MOTO_C_H_GPIO_OFFSET)|(1<<MOTO_A_H_GPIO_OFFSET)|(1<<MOTO_B_H_GPIO_OFFSET));                          
  GPIO_REG(GPIO_INPUT_EN)  &= ~ ((1<<MOTO_C_H_GPIO_OFFSET)|(1<<MOTO_A_H_GPIO_OFFSET)|(1<<MOTO_B_H_GPIO_OFFSET));                          
  
  GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1<< MOTO_C_L_GPIO_OFFSET) | (0x1<< MOTO_A_L_GPIO_OFFSET) | (0x1 << MOTO_B_L_GPIO_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_EN)   |=  ((0x1<< MOTO_C_L_GPIO_OFFSET) | (0x1<< MOTO_A_L_GPIO_OFFSET) | (0x1 << MOTO_B_L_GPIO_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~((0x1<< MOTO_C_L_GPIO_OFFSET) | (0x1<< MOTO_A_L_GPIO_OFFSET) | (0x1 << MOTO_B_L_GPIO_OFFSET)) ;

  PWM1_REG(PWM_CFG)&=~ (PWM_CFG_CMP0CENTER|PWM_CFG_CMP1CENTER|PWM_CFG_CMP2CENTER);
  PWM2_REG(PWM_CFG)&=~ (PWM_CFG_CMP0CENTER|PWM_CFG_CMP1CENTER);
  
  
  GPIO_REG(GPIO_OUTPUT_XOR)|=((1<<MOTO_C_H_GPIO_OFFSET)|(1<<MOTO_A_H_GPIO_OFFSET)|(1<<MOTO_B_H_GPIO_OFFSET)) ;
  PWM1_REG(PWM_CMP0)= 1024;
  PWM2_REG(PWM_CMP0)= 1024; 
  PWM2_REG(PWM_CMP1)= 0;
  PWM1_REG(PWM_CMP1)= 0;
  PWM1_REG(PWM_CMP2)= 0;
  
  PWM1_REG(PWM_CFG) |= (PWM_CFG_ZEROCMP| PWM_CFG_ENALWAYS|PWM_CFG_DEGLITCH ); 
  PWM2_REG(PWM_CFG) |= (PWM_CFG_ZEROCMP| PWM_CFG_ENALWAYS|PWM_CFG_DEGLITCH);   
}
void ZERO_init (){
  GPIO_REG(GPIO_OUTPUT_EN)  &= ~((0x1 << zeroA__GPIO_OFFSET) | (0x1 << zeroB__GPIO_OFFSET)| (0x1 << zeroC__GPIO_OFFSET));
  GPIO_REG(GPIO_PULLUP_EN)  &= ~((0x1 << zeroA__GPIO_OFFSET) | (0x1 << zeroB__GPIO_OFFSET)| (0x1 << zeroC__GPIO_OFFSET));
  GPIO_REG(GPIO_INPUT_EN)   |=  ((0x1 << zeroA__GPIO_OFFSET) | (0x1 << zeroB__GPIO_OFFSET)| (0x1 << zeroC__GPIO_OFFSET));

  GPIO_REG(GPIO_RISE_IE) |= (1 << zeroA__GPIO_OFFSET);
  GPIO_REG(GPIO_RISE_IE) |= (1 << zeroB__GPIO_OFFSET);
  GPIO_REG(GPIO_RISE_IE) |= (1 << zeroC__GPIO_OFFSET);
}

 void Delay_ms(size_t n)
{
  unsigned long start_mtime, delta_mtime;

  // Don't start measuruing until we see an mtime tick
  unsigned long tmp = mtime_lo();
  do {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  do {
    delta_mtime = mtime_lo() - start_mtime;
  } while (delta_mtime < (n * TMR_FREQ/1000));

  
}
void delay_us ( int t) //延时1US   test
{
	volatile int i,j;
	for(j=0;j<t;j++)
	    for(i=0;i<1;i++);

}

__uint8_t SENSE ()
{
  switch(Phase)
  {
   case 0:  
          SENSE_V= (GPIO_REG(GPIO_INPUT_VAL)&(1<<zeroC__GPIO_OFFSET)&&1);
          break;
   case 1:  
	        SENSE_V= (GPIO_REG(GPIO_INPUT_VAL)&(1<<zeroB__GPIO_OFFSET)&&1);
          break;
   case 2:  
          SENSE_V= (GPIO_REG(GPIO_INPUT_VAL)&(1<<zeroA__GPIO_OFFSET)&&1);
          break;
   case 3:  
    	    SENSE_V= (GPIO_REG(GPIO_INPUT_VAL)&(1<<zeroC__GPIO_OFFSET)&&1);
          break;
   case 4:  
    	    SENSE_V= (GPIO_REG(GPIO_INPUT_VAL)&(1<<zeroB__GPIO_OFFSET)&&1);
          break;
   case 5:  
   	      SENSE_V= (GPIO_REG(GPIO_INPUT_VAL)&(1<<zeroA__GPIO_OFFSET)&&1);
          break;
  }
  return SENSE_V;
}
//############################################################################
/*补充注释：
比较器中断，实现闭环中断过零检测并换向
注意其中并不包含电角度延迟，即：换向提前角是30度
*/
void SIGNAL(void) //比较器中断 
{
  unsigned char sense = 0;
  
  do
  {
    if(SENSE()) sense = 1; else sense = 0;
    switch(Phase)
    {
      case 0:  
              STEUER_A_H;
              if(!sense) 
                {
                  
                  if(!SENSE()) 
                  {
                    STEUER_C_L;
                    SENSE_B_RISING_INT;
                //   GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroB__GPIO_OFFSET);
                //    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroB__GPIO_OFFSET);
                    SENSE_B; 
                //     GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroB__GPIO_OFFSET);
///GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroB__GPIO_OFFSET); 
                    Phase++; 
                    Phase %= 6;
                    CntKommutierungen++;
                    GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 30);
                    delay_us(100);
                    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET);
                  }
                }
                else 
                {
                  STEUER_B_L;
                  
                } 
              break;
      case 1:  
              STEUER_C_L;
              if(sense) 
                {
                 
                  if(SENSE()) 
                  {
                    STEUER_B_H;
                    SENSE_A_FALLING_INT;
                 //   GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroA__GPIO_OFFSET);
                 //   GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroA__GPIO_OFFSET); 
                    SENSE_A; 
                  //  GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroA__GPIO_OFFSET);
                 //   GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroA__GPIO_OFFSET); 
                    
                  
                    Phase++; 
                    Phase %= 6;
                    CntKommutierungen++;
                    GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 30);
                    delay_us(100);
                    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET);
                  }
                }
                else 
                {
                  STEUER_A_H;
                  
                } 

              break;
      case 2:  
              STEUER_B_H;
              if(!sense) 
                {
                  
                  if(!SENSE()) 
                  {
                    STEUER_A_L;
                    SENSE_C_RISING_INT;
                 //  GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                 //   GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET); 
                    SENSE_C; 
                 //    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                 //   GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET);  
                  
                    Phase++; 
                    Phase %= 6;
                    CntKommutierungen++;
                    GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 30);
                    delay_us(100);
                    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET);
                  }
                }
                else 
                {
                  STEUER_C_L;
                  
                } 
                  
              break;
      case 3:  
            STEUER_A_L;
              if(sense) 
                {
                 
                  if(SENSE()) 
                  {
                    STEUER_C_H;
                    SENSE_B_FALLING_INT;
                //   GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroB__GPIO_OFFSET);
                //    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroB__GPIO_OFFSET); 
                    SENSE_B; 
                //    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroB__GPIO_OFFSET);
                //    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroB__GPIO_OFFSET); 
                    Phase++; 
                    Phase %= 6;
                    CntKommutierungen++;
                    GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 30);
                    delay_us(100);
                    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET);
                  }
                }
                else 
                {
                  STEUER_B_H;
                  
                } 
                  

              break;
      case 4:  
            STEUER_C_H;
              if(!sense) 
                {
                 
                  if(!SENSE()) 
                  {
                    STEUER_B_L;
                    
                  SENSE_A_RISING_INT;
               //   GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroA__GPIO_OFFSET);
               //   GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroA__GPIO_OFFSET); 
                  SENSE_A; 
                //  GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroA__GPIO_OFFSET);
               //   GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroA__GPIO_OFFSET); 
                  Phase++; 
                  Phase %= 6;
                  CntKommutierungen++;
                  GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 30);
                  delay_us(100);
                  GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET); 
                  }
                }
              else 
                {
                  STEUER_A_L;
                  
                } 
                  
              break;
      case 5:  
              STEUER_B_L;
              if(sense) 
                {
                  
                  if(SENSE()) 
                  {
                    STEUER_A_H;
                    SENSE_C_FALLING_INT;
                    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET); 
                    SENSE_C; 
                    
                    Phase ++; 
                    Phase %= 6;
                    CntKommutierungen++;
                    GPIO_REG(GPIO_OUTPUT_VAL)^=(0x1 << 30);
                    delay_us(100);
                    GPIO_REG(GPIO_RISE_IP) = (0x1 << zeroC__GPIO_OFFSET);
                    GPIO_REG(GPIO_FALL_IP) = (0x1 << zeroC__GPIO_OFFSET); 
                  }
                }
                else 
                {
                  STEUER_C_H;
                  
                } 
              break;
    }
  }
  
while(((!SENSE()) && sense) || (SENSE() && !sense));

}


//############################################################################
/*补充注释：
开环顺序换向算法，注意换向时必须同步修改比较器端口及触发沿
以便在反相感生电动势到达切换条件时，自动切换到闭环运转状态
*/
void Manuell(void)
{
 switch(Phase)
  {
   case 0:  
           STEUER_A_H;
		       STEUER_B_L;
           SENSE_C_FALLING_INT;
           SENSE_C; 
           
         
          break;
   case 1:  
           STEUER_A_H;
		       STEUER_C_L;
          SENSE_B_RISING_INT;
           SENSE_B; 
           
          break;
   case 2:  
           STEUER_B_H;
		       STEUER_C_L;
           SENSE_A_FALLING_INT;
           SENSE_A; 
           
          break;
   case 3:  
           STEUER_B_H;
		       STEUER_A_L;
           SENSE_C_RISING_INT;
           SENSE_C; 
           
          break;
   case 4:  
           STEUER_C_H;
		      STEUER_A_L;
           SENSE_B_FALLING_INT;
          SENSE_B; 
          
          break;
   case 5:  
           STEUER_C_H;
		       STEUER_B_L;
            
            SENSE_A_RISING_INT;
            SENSE_A; 
           
         
          break;
  }
}
//############################################################################
//
void Anwerfen(unsigned char pwm) //启动马达
//############################################################################
/*补充注释：
开环启动算法
*/
{
    unsigned long timer = 400,i,P=0;
    
    PWM = pwm;
    //SetPWM();//，设置pwm占空比
    Manuell();
    /*补充注释： 
开环顺序换向算法，注意换向时必须同步修改比较器端口及触发沿 
以便在反相感生电动势到达切换条件时，自动切换到闭环运转状态 
*/////////////////////////////////////////////////// 
    Delay_ms(40);
   //PWM = pwm;
    while(1)
        {
          for(i=0;i<timer; i++) 
              {
              
              delay_us(35);  // 
              } 
          timer-= timer/15+1;
          if(timer < 80) { if(TEST_MANUELL) timer = 80; else return; }
          Manuell();
          Phase++;
          Phase %= 6;
        //  printf("Phase=%d\n",Phase);
        //如果模拟比较器中断标志置位,绿灯亮
        P++;
          if( P>70)
          {
             TEST_MANUELL=0;
               
         }
        }
}