 
#include "DataScope_DP.h"
 
unsigned char DataScope_OutPut_Buffer[42] = {0};	   //Ž®¿Ú·¢ËÍ»º³åÇø


//º¯ÊýËµÃ÷£ºœ«µ¥Ÿ«¶Èž¡µãÊýŸÝ×ª³É4×ÖœÚÊýŸÝ²¢ŽæÈëÖž¶šµØÖ· 
//žœŒÓËµÃ÷£ºÓÃ»§ÎÞÐèÖ±œÓ²Ù×÷ŽËº¯Êý 
//target:Ä¿±êµ¥Ÿ«¶ÈÊýŸÝ
//buf:ŽýÐŽÈëÊý×é
//beg:Öž¶šŽÓÊý×éµÚŒžžöÔªËØ¿ªÊŒÐŽÈë
//º¯ÊýÎÞ·µ»Ø 
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //µÃµœfloatµÄµØÖ·
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
 
 
//º¯ÊýËµÃ÷£ºœ«Žý·¢ËÍÍšµÀµÄµ¥Ÿ«¶Èž¡µãÊýŸÝÐŽÈë·¢ËÍ»º³åÇø
//Data£ºÍšµÀÊýŸÝ
//Channel£ºÑ¡ÔñÍšµÀ£š1-10£©
//º¯ÊýÎÞ·µ»Ø 
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
	if ( (Channel > 10) || (Channel == 0) ) return;  //ÍšµÀžöÊýŽóÓÚ10»òµÈÓÚ0£¬Ö±œÓÌø³ö£¬²»ÖŽÐÐº¯Êý
  else
  {
     switch (Channel)
		{
      case 1:  Float2Byte(&Data,DataScope_OutPut_Buffer,1); break;
      case 2:  Float2Byte(&Data,DataScope_OutPut_Buffer,5); break;
		  case 3:  Float2Byte(&Data,DataScope_OutPut_Buffer,9); break;
		  case 4:  Float2Byte(&Data,DataScope_OutPut_Buffer,13); break;
		  case 5:  Float2Byte(&Data,DataScope_OutPut_Buffer,17); break;
		  case 6:  Float2Byte(&Data,DataScope_OutPut_Buffer,21); break;
		  case 7:  Float2Byte(&Data,DataScope_OutPut_Buffer,25); break;
		  case 8:  Float2Byte(&Data,DataScope_OutPut_Buffer,29); break;
		  case 9:  Float2Byte(&Data,DataScope_OutPut_Buffer,33); break;
		  case 10: Float2Byte(&Data,DataScope_OutPut_Buffer,37); break;
		}
  }	 
}


//º¯ÊýËµÃ÷£ºÉú³É DataScopeV1.0 ÄÜÕýÈ·Ê¶±ðµÄÖ¡žñÊœ
//Channel_Number£¬ÐèÒª·¢ËÍµÄÍšµÀžöÊý
//·µ»Ø·¢ËÍ»º³åÇøÊýŸÝžöÊý
//·µ»Ø0±íÊŸÖ¡žñÊœÉú³ÉÊ§°Ü 
unsigned char DataScope_Data_Generate(unsigned char Channel_Number)
{
	if ( (Channel_Number > 10) || (Channel_Number == 0) ) { return 0; }  //ÍšµÀžöÊýŽóÓÚ10»òµÈÓÚ0£¬Ö±œÓÌø³ö£¬²»ÖŽÐÐº¯Êý
  else
  {	
	 DataScope_OutPut_Buffer[0] = '$';  //Ö¡Í·
		
	 switch(Channel_Number)   
   { 
		 case 1:   DataScope_OutPut_Buffer[5]  =  5; return  6; break;   
		 case 2:   DataScope_OutPut_Buffer[9]  =  9; return 10; break;
		 case 3:   DataScope_OutPut_Buffer[13] = 13; return 14; break;
		 case 4:   DataScope_OutPut_Buffer[17] = 17; return 18; break;
		 case 5:   DataScope_OutPut_Buffer[21] = 21; return 22; break; 
		 case 6:   DataScope_OutPut_Buffer[25] = 25; return 26; break;
		 case 7:   DataScope_OutPut_Buffer[29] = 29; return 30; break;
		 case 8:   DataScope_OutPut_Buffer[33] = 33; return 34; break;
		 case 9:   DataScope_OutPut_Buffer[37] = 37; return 38; break;
     case 10:  DataScope_OutPut_Buffer[41] = 41; return 42; break;
   }	 
  }
	return 0;
}











