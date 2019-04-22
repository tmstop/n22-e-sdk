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
#include "oled.h"
#include "bmp.h"
/*
 * Demonstrate how to print a greeting message on standard output
 * and exit.
 *
 * WARNING: This is a build-only project. Do not try to run it on a
 * physical board, since it lacks the device specific startup.
 */
u8 ref=0;//Ë¢ÐÂÏÔÊ¾
u16 vx=15542,vy=11165;  //±ÈÀýÒò×Ó£¬´ËÖµ³ýÒÔ1000Ö®ºó±íÊ¾¶àÉÙ¸öADÖµ´ú±íÒ»¸öÏñËØµã
u16 chx=140,chy=146;//Ä¬ÈÏÏñËØµã×ø±êÎª0Ê±µÄADÆðÊ¼Öµ

void xianshi()//ÏÔÊ¾ÐÅÏ¢
{
//	u16 lx,ly;
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
	showhanzi(10,0,0);  //ÖÐ
	showhanzi(45,0,1);  //¾°
	showhanzi(80,0,2);  //Ô°
	showhanzi(115,0,3);  //µç
	showhanzi(150,0,4);  //×Ó
  LCD_ShowString(10,35,"3.5 TFT SPI 3320*480");
//	LCD_ShowString(10,55,"LCD_W:");	LCD_ShowNum(70,55,LCD_W,3);
	//LCD_ShowString(110,55,"LCD_H:");LCD_ShowNum(160,55,LCD_H,3);
//	lx=10;ly=75;
}
void showimage() //ÏÔÊ¾40*40Í¼Æ¬
{
  	int i,j,k;

	for(k=3;k<8;k++)
	{
	   	for(j=0;j<6;j++)
		{
			Address_set(40*j,40*k,40*j+39,40*k+39);		//×ø±êÉèÖÃ
		    for(i=0;i<1600;i++)
			 {

			 LCD_WR_DATA8(image[i*3+2]);
			  	 LCD_WR_DATA8(image[i*3+1]);
				 LCD_WR_DATA8(image[i*3]);
			 }
		 }
	}
	ref=0;
}
int main(int argc, char **argv)
{
  
  // u16 BACK_COLOR, POINT_COLOR;   //±³¾°É«£¬»­±ÊÉ«
//  printf("Hello asdfas        !" "\n");
 Lcd_Init();  LCD_Clear(0xFCFCFC);
 printf("\n asdasdasdasd\n");
  while(1)
  {
	  LCD_Clear(0xFCFCFC);
    //  xianshi(); //ÏÔÊ¾ÐÅÏ¢
	//  xianshi(); //ÏÔÊ¾40*40Í¼Æ¬
	 // delay_ms(1000);
	//	xianshi(); //ÏÔÊ¾ÐÅÏ¢
	    showimage(); //ÏÔÊ¾40*40Í¼Æ¬



  }

  return 0;

}
