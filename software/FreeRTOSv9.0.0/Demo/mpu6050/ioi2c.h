// See LICENSE for license details.


#ifndef IOI2C_H
#define IOI2C_H
#include "soc/drivers/soc.h"
#define IIC_SDA_GPIO_OFFSET 12
#define IIC_SCL_GPIO_OFFSET 13

//IO方向设置
#define SDA_IN()  {GPIO_REG(GPIO_OUTPUT_EN)  &=~(0x1 << IIC_SDA_GPIO_OFFSET );GPIO_REG(GPIO_INPUT_EN)   |=  (0x1 << IIC_SDA_GPIO_OFFSET); }
#define SDA_OUT() {GPIO_REG(GPIO_OUTPUT_EN)  |= (0x1 << IIC_SDA_GPIO_OFFSET );GPIO_REG(GPIO_INPUT_EN)   &= ~ (0x1 << IIC_SDA_GPIO_OFFSET);}


	 
#define READ_SDA()  (GPIO_REG(GPIO_INPUT_VAL) & (0x1 << IIC_SDA_GPIO_OFFSET))&&1  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
int IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(__uint8_t txd);			//IIC发送一个字节
__uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
int IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(__uint8_t daddr,__uint8_t addr,__uint8_t data);
__uint8_t IIC_Read_One_Byte(__uint8_t daddr,__uint8_t addr);	 
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
__uint8_t IICwriteBytes(__uint8_t dev, __uint8_t reg, __uint8_t length, __uint8_t* data);
__uint8_t IICwriteBits(__uint8_t dev,__uint8_t reg,__uint8_t bitStart,__uint8_t length,__uint8_t data);
__uint8_t IICwriteBit(__uint8_t dev,__uint8_t reg,__uint8_t bitNum,__uint8_t data);
__uint8_t IICreadBytes(__uint8_t dev, __uint8_t reg, __uint8_t length, __uint8_t *data);

int i2cWrite(__uint8_t addr, __uint8_t reg, __uint8_t len, __uint8_t *data);
int i2cRead(__uint8_t addr, __uint8_t reg, __uint8_t len, __uint8_t *buf);




#endif /* IOI2C_H */
