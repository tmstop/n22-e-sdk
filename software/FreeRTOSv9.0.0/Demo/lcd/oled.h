//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2017-10-10
//  ����޸�   : 
//  ��������   : ���ֿ�TFT_LCD����˵��(STM32ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND=��Դ��
//              VCC=��Դ��
//              CLK=PC6
//              MOSI=PC7
//              RES=PC9
//              DC=PC8
//							BLKĬ�ϱ���򿪣�����Ҫ�رձ��⣬��BLK�õ͵�ƽ����
//							CS1=PC10
//							����ΪLCD�Ĳ����ܽţ���������ֿ����漸���ſ��Բ�����
//              ----------------------------------------------------------------
//              MISO=PC11
//              CS2=PC12
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰���� 2017-10-10
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 


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
#define LCD_W 320
#define LCD_H 480
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//--------------------------------------------------------------------------
/*#define SCLK_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_6)//oled���ֿ⹫��spiʱ��
#define SCLK_Set() GPIO_SetBits(GPIOC,GPIO_Pin_6)
 
#define SDIN_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_7)//oled���ֿ⹫��spi����
#define SDIN_Set() GPIO_SetBits(GPIOC,GPIO_Pin_7)	*/

#define OLED_DC_Clr()   GPIO_REG(GPIO_OUTPUT_VAL) &=~(0x1 << 25)//oled ��������
#define OLED_DC_Set()   GPIO_REG(GPIO_OUTPUT_VAL) |=(0x1 << 25)

#define OLED_RST_Clr() GPIO_REG(GPIO_OUTPUT_VAL) &=~(0x1 << 24)//oled RES
#define OLED_RST_Set() GPIO_REG(GPIO_OUTPUT_VAL) |=(0x1 << 24)

#define OLED_CS_Clr()  GPIO_REG(GPIO_OUTPUT_VAL) &=~(0x1 << 28)//oledƬѡ
#define OLED_CS_Set()  GPIO_REG(GPIO_OUTPUT_VAL) |=(0x1 << 28)

//----------------------------------------------------------------------------
//#define ZK_CS_Clr()  GPIO_REG(GPIO_OUTPUT_VAL) &=~(0x1 << 23)
//#define ZK_CS_Set()  GPIO_REG(GPIO_OUTPUT_VAL) |= (0x1<<23)

//#define ZK_OUT()    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//��ȡָ���ܽŵ�����
//--------------------------------------------------------------------------------- 


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

extern  u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ
void delay_ms ( int t) ;
void Delayus ( int t) ;
void Lcd_Init(void); 
void LCD_Clear(unsigned long Color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da); //��������-8λ����
void LCD_WR_DATA(unsigned long da);
void LCD_WR_REG(char da);

void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_DrawPoint_big(u16 x,u16 y);//��һ�����
u16  LCD_ReadPoint(u16 x,u16 y); //����
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//��ʾ����
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//��ʾ2������
void LCD_ShowString(u16 x,u16 y,const u8 *p);		 //��ʾһ���ַ���,16����
 
void showhanzi(unsigned int x,unsigned int y,unsigned char index);

void Set_Pos(unsigned char x, unsigned char y);//������ʾλ��
void Set_Pixel(unsigned char x,unsigned char y);//��һ����
void Asc6_8(unsigned char x,unsigned char y,unsigned char ch[]);//д��һ���׼ASCII�ַ��� 6*8
void Asc8_16(unsigned char x,unsigned char y,unsigned char ch[]);//д��һ���׼ASCII�ַ��� 8*16
void Show_Line( unsigned char a, unsigned char b, unsigned char c,unsigned char Data);// ��one page ��Χ�ڻ�����
void Draw_Rectangle(unsigned char p1,unsigned char p2,unsigned char x1, unsigned char x2);//  �����ο�
void HZ16_16( unsigned char x, unsigned char y, unsigned char num);//��ʾ16*16������
void Show_HZ16_16(unsigned char  x,unsigned char  y,unsigned char num1,unsigned char num2);//д��һ��16*16����
void HZ12_16( unsigned char x, unsigned char y, unsigned char num);	//	��ʾ12*16������
void Show_HZ12_16(unsigned char  x,unsigned char  y,unsigned char num1,unsigned char num2);// д��һ��12*16����
void Fill_RAM(unsigned char Data);//ȫ�����
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d);//���ֻ���ȫ�����
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);// ���ֻ���ȫ����ʾͼƬ
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c);//��ֱ����
void Horizontal_Scroll(unsigned char DIRECTION,unsigned char p1, unsigned char p2, unsigned char t, unsigned char c1,unsigned char c2);//ˮƽ����
void Deactivate_Scroll(void);//  ֹͣˮƽ���ߴ�ֱ������ֹͣRAM���ݱ�����д
void Fade_In(void);//����
void Fade_Out(void);//����
void Sleep(unsigned char a);//˯��ģʽ




void CL_Mem(void);
void GBZK_ROMOUTSET(void);		//�ֿ��������
void GBZK_ROMOUTRESET(void);	 	//�ֿ��������
void GBZK_GPIO_Config(void);	 //�ֿ�оƬ��ʼ��
void ZK_command( unsigned char  data );
unsigned char  get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(unsigned char  AddrHigh,unsigned char  AddrMid,unsigned char  AddrLow,unsigned char  *pBuff,unsigned char  DataLen );
void Display_Asc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[]);
void Display_Asc( unsigned char zk_num,unsigned int x, unsigned int y );
void zk_map(  unsigned char *getdate, unsigned char *putdata, unsigned char high, unsigned char width);
void Display_GB2312_String(unsigned char zk_num,unsigned int  x,unsigned int  y, unsigned char  text[]);
void Display_GB2312( unsigned char zk_num,unsigned int x, unsigned int y );

//������ɫ
#define WHITE         	 0xFCFCFC

#define RED           	 0xFC0000


					  		 
#endif  
	 
	 



