#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
#include "gui.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F407¿ª·¢°å
//Íâ²¿ÖĞ¶Ï Çı¶¯´úÂë	   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2014/5/4
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//Íâ²¿ÖĞ¶Ï0·şÎñ³ÌĞ

//ÖÓµã½çÃæÏÂ£º
//°´wk_upÉèÖÃÄÖÖÓ
//°´key1Ğ£¶ÔÊ±¼ä

//°´¼üĞ£¶ÔÊ±¼ä½çÃæÏÂ£º
//°´wk_upÔö¼Ó
//°´key1¼õĞ¡
//°´key0È·ÈÏ
//°´key2ÇĞ»»Ğ£¶ÔÕß

//´®¿ÚĞ£¶ÔÊ±¼ä£º
//
//
extern int now;
extern int add;
extern int sub;
extern int change_hour;
extern int alarm_allow;
extern int confirm;
void EXTI0_IRQHandler(void)
{

  delay_ms(10);	//Ïû¶¶
	if(WK_UP==1)	 
	{
	if(now==CLOCK){
	
		now=CLOCKBUTTONS;
	}
	else if(now==CLOCKBUTTONS)
	  now=CLOCK;
  }
	 EXTI_ClearITPendingBit(EXTI_Line0); //Çå³ıLINE0ÉÏµÄÖĞ¶Ï±êÖ¾Î» 
}	
//Íâ²¿ÖĞ¶Ï2·şÎñ³ÌĞò
void EXTI2_IRQHandler(void)
{
	/*
	u8 show1[50]="key0:confirm";
	u8 show2[50]="key2:cancel";
	*/
	delay_ms(10);	//Ïû¶¶ 
	if(KEY2==0)	 
	{
	if(now==CLOCK){
	
		now=CLOCKBUTTONS;
	}
	else if(now==CLOCKBUTTONS)
	  now=CLOCK;
	}
	 EXTI_ClearITPendingBit(EXTI_Line2);//Çå³ıLINE2ÉÏµÄÖĞ¶Ï±êÖ¾Î» 
}
//Íâ²¿ÖĞ¶Ï3·şÎñ³ÌĞò
void EXTI3_IRQHandler(void)
{
	
	delay_ms(10);	//Ïû¶¶
  if(KEY1==0)	 
	{
	if(now==CLOCK){
	
		now=CLOCKBUTTONS;
	}
	else if(now==CLOCKBUTTONS){
	  now=CLOCK;
	}
	}	
	 EXTI_ClearITPendingBit(EXTI_Line3);  //Çå³ıLINE3ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}
//Íâ²¿ÖĞ¶Ï4·şÎñ³ÌĞò
void EXTI4_IRQHandler(void)
{
	delay_ms(10);	//Ïû¶¶
  if(KEY0==0)	 
	{
  if(now==CLOCK){
		now=CLOCKBUTTONS;
	}
	else if(now==CLOCKBUTTONS){
	  now=CLOCK;
	}
	
	}		
	 EXTI_ClearITPendingBit(EXTI_Line4);//Çå³ıLINE4ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
}
	   
//Íâ²¿ÖĞ¶Ï³õÊ¼»¯³ÌĞò
//³õÊ¼»¯PE2~4,PA0ÎªÖĞ¶ÏÊäÈë.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //°´¼ü¶ÔÓ¦µÄIO¿Ú³õÊ¼»¯
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//Ê¹ÄÜSYSCFGÊ±ÖÓ
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 Á¬½Óµ½ÖĞ¶ÏÏß2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 Á¬½Óµ½ÖĞ¶ÏÏß3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 Á¬½Óµ½ÖĞ¶ÏÏß4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 Á¬½Óµ½ÖĞ¶ÏÏß0
	
  /* ÅäÖÃEXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ÖĞ¶ÏÊÂ¼ş
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //ÉÏÉıÑØ´¥·¢ 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//Ê¹ÄÜLINE0
  EXTI_Init(&EXTI_InitStructure);//ÅäÖÃ
	
	/* ÅäÖÃEXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ÖĞ¶ÏÊÂ¼ş
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ÏÂ½µÑØ´¥·¢
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ÖĞ¶ÏÏßÊ¹ÄÜ
  EXTI_Init(&EXTI_InitStructure);//ÅäÖÃ
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//Íâ²¿ÖĞ¶Ï0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//ÇÀÕ¼ÓÅÏÈ¼¶0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//×ÓÓÅÏÈ¼¶2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  NVIC_Init(&NVIC_InitStructure);//ÅäÖÃ
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//Íâ²¿ÖĞ¶Ï2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//ÇÀÕ¼ÓÅÏÈ¼¶3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//×ÓÓÅÏÈ¼¶2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  NVIC_Init(&NVIC_InitStructure);//ÅäÖÃ
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//Íâ²¿ÖĞ¶Ï3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//ÇÀÕ¼ÓÅÏÈ¼¶2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//×ÓÓÅÏÈ¼¶2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  NVIC_Init(&NVIC_InitStructure);//ÅäÖÃ
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//Íâ²¿ÖĞ¶Ï4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//ÇÀÕ¼ÓÅÏÈ¼¶1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//×ÓÓÅÏÈ¼¶2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  NVIC_Init(&NVIC_InitStructure);//ÅäÖÃ
	   
}












