#include "oled.h"
#include "stdlib.h"
#include "n22/drivers/n22_func.h"
#include "soc/drivers/soc.h"
#include "soc/drivers/board.h"
#include "n22/drivers/riscv_encoding.h"
#include "n22/drivers/n22_tmr.h"
#include "n22/drivers/n22_clic.h"

unsigned char  FontBuf[128];//�ֿ⻺��	
unsigned char  Font_Map[128];//ת������ֿ⻺��

void CL_Mem(void)
{
  unsigned char  i;
	 
	for(i=0;i<128;i++ )
	{
      Font_Map[i]=0;
//	  FontBuf[i]=0;
	  }

}
void GBZK_ROMOUTSET(void)		//�ֿ��������
{		

}

void GBZK_ROMOUTRESET(void)	 	//�ֿ��������
{		
	  	 
}

void GBZK_GPIO_Config(void)	 //�ֿ�оƬ��ʼ��
{		
   
/*	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ; //�ֿ�Ƭѡ	 PC12 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
   	GPIO_Init(GPIOC, &GPIO_InitStructure);		  
 		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	  //�ֿ�SPI���	PC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOC,GPIO_Pin_11 );  */


}


//------------��ָ��ֿ�IC---�о�԰--------

void ZK_command( unsigned char  data )
{
	SPI2_REG(SPI_REG_TXFIFO)=data;
	 

 }
 
//---���ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�--�о�԰�Ƽ�/
 unsigned char  get_data_from_ROM(void)
{
	unsigned char  i;
	unsigned char  ret_data=0; //�������ݳ�ʼ��
/*	SCLK_Set();//�ֿ�ʱ������
	GBZK_ROMOUTSET();  //��������

	for(i=0;i<8;i++)
	{
	
		
		SCLK_Clr();  //�ֿ�ʱ������  
		ret_data=ret_data<<1;
		if( ZK_OUT() )
			{ret_data=ret_data+1; 
			}
		else
			{ret_data=ret_data+0; 
			}
		SCLK_Set(); //�ֿ�ʱ������
	}
	GBZK_ROMOUTRESET();	 //��©���
	return(ret_data); //���ض�����һ���ֽ� */
}


//--����ص�ַ��AddrHigh����ַ���ֽ�,AddrMid����ַ���ֽ�,AddrLow����ַ���ֽڣ�����������DataLen���ֽڵ����ݵ� pBuff�ĵ�ַ//

void get_n_bytes_data_from_ROM(unsigned char  AddrHigh,unsigned char  AddrMid,unsigned char  AddrLow,unsigned char  *pBuff,unsigned char  DataLen )
{
 	unsigned char  i;
//	OLED_CS_Set(); //ȡ��OLEDƬѡ
	//ZK_CS_Clr(); //�ֿ�Ƭѡ
  //	SCLK_Clr();  //�ֿ�ʱ������
   	ZK_CS_Set();//ȡ���ֿ�Ƭѡ
	ZK_CS_Clr(); //�ֿ�Ƭѡ
//  	SCLK_Clr();  //�ֿ�ʱ������



   
	ZK_command(0x03);//дָ��
	ZK_command(AddrHigh);//д��ַ���ֽ�
	ZK_command(AddrMid);//д��ַ���ֽ�
	ZK_command(AddrLow);//д��ַ���ֽ�
	for(i = 0; i < DataLen; i++ )
	     *(pBuff+i) =get_data_from_ROM();//��һ���ֽ�����

	ZK_CS_Set();//ȡ���ֿ�Ƭѡ
}
 

 /////////////////////////////////////////////////////////////////////////////////
 ///////	Display_GB2312_String���������ַ�����ʾ����	/////
 //zk_num���壺���ֿ�ı�ʾ�� 1:GB2312_12*12,2:GB2312_15*16, 3:GB2312_24*24, 4:GB2312_32*32,
 //x�������� y��������
 //text[]��Ҫ��ʾ�ĺ���
 //**************************************************************************
void Display_GB2312_String(unsigned char zk_num,unsigned int  x,unsigned int  y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	
	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
	unsigned char  n,h,w,d,p;// ��ͬ�����ֿ�ļ������
 	CL_Mem(); //�建�����飬����ᵼ��12*12��ʾ�쳣

	switch(zk_num)
		{  // n������h���ָ߶ȣ�w���ֿ�ȣ� d���ּ�࣬c��ҳ��С
		case '1':  BaseAdd=0x0;     n=24;  h=12; w=12; d=12; p=2;  break;  // 12*12  
		case '2':  BaseAdd=0x2C9D0; n=32;  h=16; w=16; d=16; p=2;  break;   // 15*16  
	    case '3':  BaseAdd=0x68190; n=72;  h=24; w=24; d=24; p=3;  break;   // 24*24  
	    case '4':  BaseAdd=0xEDF00; n=128; h=32; w=32; d=32; p=4;  break;   // 32*32  
 		}

		while((text[i]>0x00))
		{
		    if(x>(240-d))
		      {  y=y+p; 
			     x=0; 
				 	 }//�������
		
			if(((text[i]>=0xA1) &&(text[i]<=0xA9))&&(text[i+1]>=0xA1))
			{						
				//������壨GB2312�������� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺//
				//Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; ������ȡ��ַ///
			 	FontAddr = (text[i]- 0xA1)*94; 
				FontAddr += (text[i+1]-0xA1);
				FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
				
				AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ//
				AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ//
				AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ//
				get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[]"//
			  // 	get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[ ]" 
				zk_map(Font_Map,FontBuf,h,w);//�ַ�ת��
				Display_GB2312(zk_num,x,y);//��ʾһ�����ֵ�OLED��/ 
	 		 		}
	
			else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
		 	{						
				//������壨GB2312�� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺//
				//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; ������ȡ��ַ//
				 
				FontAddr = (text[i]- 0xB0)*94; 
				FontAddr += (text[i+1]-0xA1)+846;
				FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
				
				AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ//
				AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ//
				AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ//
				get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[ ]"//
				//get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[ ]"
				zk_map(Font_Map,FontBuf,h,w);//�ַ�ת��
				Display_GB2312(zk_num,x,y);//��ʾһ�����ֵ�OLED��/
		 		}
			 x=x+d; //��һ��������
		 	 i+=2;  //�¸��ַ�
			}
	 }


//***************************************************************
//  ��ʾ12*12 15*16 24*24 32*32 GB2312������ 2017-10�о�԰����ͨ��
//   zk_num������ ���е�����  GB_1212 1616 2424 3232 
//   x: ��ʼ�� 
//   y: ��ʼҳ 0~7
//*************************************************************** 

void Display_GB2312( unsigned char zk_num,unsigned int x, unsigned int y )
{
   	unsigned char i,k;
 
		
	switch(zk_num)
	{
		case '1':  
			 {
		 	Address_set(x,y,x+15,y+11);
			 for(i=0;i<24;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
			 }break;  // 12*12  

		case '2': 
			 {
		 	Address_set(x,y,x+15,y+15);
			 for(i=0;i<32;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
		  }break;     // 15*16 

	    case '3':  
			 {
		 	Address_set(x,y,x+23,y+23);
			 for(i=0;i<72;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
		  }break;     // 24*24  

	    case '4':  
			 {
		 	Address_set(x,y,x+31,y+31);
			 for(i=0;i<128;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
		  }break;    // 32*32  

			}
 	 


 
}


 

 
void Display_Asc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ

	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
    unsigned char  n,h,w,d,c;// ��ͬ�����ֿ�ļ������
    CL_Mem(); //�建������ ,����ᵼ��6*12��ʾ�쳣

	switch(zk_num)
	{	// n������h���ָ߶ȣ�w���ֿ�ȣ� d���ּ�࣬c��ҳ��С
		case '1':  BaseAdd=0x1DDF80; n=8;  h=7;  w=5;  d=6 ; c=1;  break;	 //	  5x7 ASCII
		case '2':  BaseAdd=0x1DE280; n=8;  h=8;  w=7;  d=8;  c=1;  break;	 //   7x8 ASCII
		case '3':  BaseAdd=0x1DBE00; n=12; h=12; w=6;  d=6;  c=2;  break;	 //  6x12 ASCII
	   case '4':  BaseAdd=0x1DD780; n=16; h=16; w=8;  d=8;  c=2;  break;	 //  8x16 ASCII	
	   case '5':  BaseAdd=0x1DFF00; n=48; h=24; w=12; d=12; c=3;  break;	 //  12x24 ASCII
	 	case '6':  BaseAdd=0x1E5A50; n=64; h=32; w=16; d=16; c=4;  break;	 //  16x32 ASCII

			}

	while((text[i]>0x00))
	{	
	   if(x>(240-d))   //�������
	      {  y=y+c; 
		     x=0; 
			 	 }
	   if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		    FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
	      	zk_map(Font_Map,FontBuf, h, w);	//���ݱ任
			Display_Asc(zk_num,x,y);/*��ʾһ��ascii��OLED�� */
  
		}
    i+=1;  //�¸�����
	x+=d;//��һ�������� 
	}	    
	
}


//***************************************************************
//  ��ʾ ASCII 2017-10�о�԰����ͨ��
//zk_num���壺���ֿ�ı�ʾ�� 1:5x7 ASCII,2:7x8 ASCII, 3:6x12 ASCII, 4:8x16 ASCII,	5: 12x24 ASCII,6:16x32 ASCII;
//   x: Start Column  ��ʼ��  
//   y: Start page   ��ʼҳ 0~7 
//*************************************************************** 
void Display_Asc( unsigned char zk_num,unsigned int x, unsigned int y )
{
    
    unsigned char i  ,k;
 
		
	switch(zk_num)
	{
	    case '1':   
			 {
		 	Address_set(x,y,x+7,y+7);
			 for(i=0;i<7;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
			 }break;    //	  5x7 ASCII

	 	case '2':   	
			 {
		 	Address_set(x,y,x+7,y+7);
			 for(i=0;i<8;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
			 }break;   //	  7x8 ASCII

	    case '3':   
			 {
		 	Address_set(x,y,x+7,y+11);
			 for(i=0;i<12;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
			 }break;  //  6x12 ASCII

	    case '4':  
	 {
		 	Address_set(x,y,x+7,y+15);
			 for(i=0;i<16;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
			 }break;     //  8x16 ASCII

	    case '5':  
	 {
		 	Address_set(x,y,x+15,y+24);
			 for(i=0;i<48;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
			 }break;    //  12x24 ASCII

	    case '6':  	 
	 {
		 	Address_set(x,y,x+15,y+31);
			 for(i=0;i<64;i++)		 
	{
		for(k=0;k<8;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k))!=0)
		{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
	
	 }
			 }break;   //  16x32 ASCII


			}
 

}





//--------------------------------------------------------------------------------
// void lcdram_map( BYTE *getdate,BYTE *putdata,BYTE high,BYTE width,BYTE style )
// ����ת������  
//putdata ԭ���ֵ� (���ú���)
//getdate Ŀ�꺺�ֵ㣬high ԭ���ָ߶ȣ�width ԭ���ֿ�ȣ�
//------------------------------------------------------------------------------
void zk_map(  unsigned char *getdate, unsigned char *putdata, unsigned char high, unsigned char width)	 //byte
{
	unsigned short i,j,wbyte;			   //word
	unsigned char i_8;
	wbyte = (width+7)/8;
//	hbyte = (high+7)/8;

		for( i = 0; i < high; i++ )
		{
			for( j = 0; j < width; j++ )
			{
				i_8 = i/8;
				if((*(putdata+wbyte*i+j/8)&(0x80>>(j%8))) > 0)
					getdate[i_8*width+j] |= (0x01<<(i%8));
				else
					getdate[i_8*width+j] &= (~(0x01<<(i%8)));

			}
	
			 }
	}










