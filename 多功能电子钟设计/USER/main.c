#include "sys.h"
#include "delay.h"  
#include "usart.h"  
#include "led.h"
#include "lcd.h"
#include "usmart.h"
#include "rtc.h"
#include "math.h"
#include "gui.h"
#include "adc.h"
#include "beep.h"
#include "exti.h"
#include "tpad.h"
#include "touch.h"
//ALIENTEK 探索者STM32F407开发板 实验15
//RTC实时时钟 实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
int main(void)
{ 
	//u8 t=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	BEEP_Init();         	//初始化蜂鸣器端口
	usmart_dev.init(84); 	//初始化USMART	
	LED_Init();					  //初始化LED
 	LCD_Init();					  //初始化LCD
	My_RTC_Init();		 		//初始化RTC
  Adc_Init();         //内部温度传感器ADC初始化
	EXTIX_Init();       //初始化外部中断输入 
	tp_dev.init();
	run_menu();

  while(1);
}
