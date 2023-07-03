#include "rtc.h"
#include "led.h"
#include "delay.h"
#include "usart.h" 
#include "beep.h"
#include "gui.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//RTC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/5
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//********************************************************************************
//修改说明
//V1.1 20140726
//新增:RTC_Get_Week函数,用于根据年月日信息,得到星期信息.
////////////////////////////////////////////////////////////////////////////////// 
extern int alarm_allow;
extern int alarm_clock_working;
extern void show_date(void);
extern void show_time(void);
extern void draw_time(void);
extern void show_temprature(void);
extern int now;
NVIC_InitTypeDef   NVIC_InitStructure;

//RTC时间设置
//hour,min,sec:小时,分钟,秒钟
//ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败 
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	ErrorStatus sta;
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	RTC_TimeTypeInitStructure.RTC_H12=ampm;
	sta=RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	set_alarm_clock(get_next_alarm_clock());
	return sta;
	
}
//RTC日期设置
//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7,0,非法!)
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败 
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	ErrorStatus sta;
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;
	sta=RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
	set_alarm_clock(get_next_alarm_clock());
	return sta;
}

//RTC初始化
//返回值:0,初始化成功;
//       1,LSE开启失败;
//       2,进入初始化模式失败;
void init_alarm_clock(void);
u8 My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
	init_alarm_clock();
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//是否第一次配置?
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE 开启    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
			{
			retry++;
			delay_ms(10);
			}
		if(retry==0)return 1;		//LSE 开启失败. 
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC同步分频系数(0~7FFF)
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC设置为,24小时格式
    RTC_Init(&RTC_InitStructure);
 
		RTC_Set_Time(10,43,1,RTC_H12_AM);	//设置时间
		RTC_Set_Date(23,6,22,4);		//设置日期
	 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//标记已经初始化过了
	} 
 
	return 0;
}

//设置闹钟时间(按星期闹铃,24小时制)
//week:星期几(1~7) @ref  RTC_Alarm_Definitions
//hour,min,sec:小时,分钟,秒钟
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//关闭闹钟A 
	
  RTC_TimeTypeInitStructure.RTC_Hours=hour;//小时
	RTC_TimeTypeInitStructure.RTC_Minutes=min;//分钟
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;//秒
	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
  
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;//星期
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//按星期闹
	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;//精确匹配星期，时分秒
	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
 
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);//清除RTC闹钟A的标志
  EXTI_ClearITPendingBit(EXTI_Line17);//清除LINE17上的中断标志位 
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);//开启闹钟A中断
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//开启闹钟A 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE17
  EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
}

//周期性唤醒定时器设置  
/*wksel:  @ref RTC_Wakeup_Timer_Definitions
#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
*/
//cnt:自动重装载值.减到0,产生中断.
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RTC_WakeUpCmd(DISABLE);//关闭WAKE UP
	
	RTC_WakeUpClockConfig(wksel);//唤醒时钟选择
	
	RTC_SetWakeUpCounter(cnt);//设置WAKE UP自动重装载寄存器
	
	
	RTC_ClearITPendingBit(RTC_IT_WUT); //清除RTC WAKE UP的标志
  EXTI_ClearITPendingBit(EXTI_Line22);//清除LINE22上的中断标志位 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//开启WAKE UP 定时器中断
	RTC_WakeUpCmd( ENABLE);//开启WAKE UP 定时器　
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE22
  EXTI_Init(&EXTI_InitStructure);//配置
 
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
}

//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{ int length;
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A中断?
	{
		alarm_clock_working=1;
		length=50;
		RTC_ClearFlag(RTC_FLAG_ALRAF);//清除中断标志
		alarm_allow=1;
		BEEP=1;
		while(alarm_allow&&length--){
			if(now==CLOCK){
			show_date();
			show_time();
			draw_time();
			show_temprature();
			}
			BEEP=!BEEP;
			LED1=!LED1;
			delay_ms(100);
		}
		BEEP=0;
		LED1=1;
		alarm_clock_working=0;
		set_alarm_clock(get_next_alarm_clock());
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//清除中断线17的中断标志 											 
}

//RTC WAKE UP中断服务函数
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP中断?
	{ 
		RTC_ClearFlag(RTC_FLAG_WUTF);	//清除中断标志
	}    
	EXTI_ClearITPendingBit(EXTI_Line22);//清除中断线22的中断标志 								
}
 
//闹钟时间
struct time time_begin={0,0,0};
struct alarm_clock my_alarm_clock;
int seconds_between(struct time begin,struct time end){//获取两时间之间的秒数
	  struct time temp1=begin,temp2=end;
	  int length=0; 
	  if(temp1.week==0&&temp2.week==0){
			 length=(temp2.hour-temp1.hour)*60*60+(temp2.minute-temp1.minute)*60;
			 if(length<0)
				 length+=60*60*24;
				 
		}
	  else if(temp1.week==0||temp2.week==0){
		    length=(temp2.hour-temp1.hour)*60*60+(temp2.minute-temp1.minute)*60;
			if(length<0)
				 length+=60*60*24;
		}
		else{
		   length=(temp2.week-temp1.week)*60*60*24+(temp2.hour-temp1.hour)*60*60+(temp2.minute-temp1.minute)*60;
				if(length<0)
				 length+=60*60*24*7;
		}
		return length;
}
void copy_to_time(RTC_DateTypeDef const*src,RTC_TimeTypeDef const*src1,struct time*dst){
    dst->week=src->RTC_WeekDay;
	  dst->hour=src1->RTC_Hours;
	  dst->minute=src1->RTC_Minutes;
}
void init_alarm_clock(void){
     my_alarm_clock.point=0;
     my_alarm_clock.maxsize=6;
}

struct time get_next_alarm_clock(void){//获取下一个要设置的闹钟
    RTC_DateTypeDef RTC_DateStruct;	
	  RTC_TimeTypeDef RTC_TimeStruct;
   	int length,i,min_length;
	  struct time now,dst;
	
	  RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	  RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	
	  
	  copy_to_time(&RTC_DateStruct,&RTC_TimeStruct,&now);
	  min_length=60*60*24*7;
	
	  for(i=0;i<my_alarm_clock.point;i++){
			
	  length=seconds_between(now,my_alarm_clock.clock[i]);
		
		if(length!=0&&length<min_length){
			
		min_length=length;
		
		dst=my_alarm_clock.clock[i];
		}
		}
	  
		return dst;
}
void set_alarm_clock(struct time temp){
    RTC_DateTypeDef RTC_DateStruct;	
	  RTC_TimeTypeDef RTC_TimeStruct;
	  int week=temp.week;
   	struct time now;
	 
	  
	  RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	  RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	
	 
	  copy_to_time(&RTC_DateStruct,&RTC_TimeStruct,&now); 
	  if(week==0){
		  if(now.hour>temp.hour||(now.hour==temp.hour&&now.minute>temp.minute)){
			week=now.week+1;
			}
			else{
			week=now.week;
			}
		} 
	  RTC_Set_AlarmA(week,temp.hour,temp.minute,0);
	  
}
int allow_adding(void){//是否允许添加
	
return my_alarm_clock.point<my_alarm_clock.maxsize;
}
int not_exist(struct time temp){//不存在这个闹钟
	int i;
  for(i=0;i<my_alarm_clock.point;i++){
	 if(temp.week==my_alarm_clock.clock[i].week&&temp.hour==my_alarm_clock.clock[i].hour&&temp.minute==my_alarm_clock.clock[i].minute){
	   return 0;
	 }
  }
	return 1;
}
void add_alarm_clock(struct time temp){//添加一个闹钟
	//set_alarm_clock(temp);
	//my_alarm_clock.maxsize=10;
if(allow_adding()&&not_exist(temp)){
my_alarm_clock.clock[my_alarm_clock.point++]=temp;
if(my_alarm_clock.point==1)
	set_alarm_clock(temp);
else{
  set_alarm_clock(get_next_alarm_clock());
}
}/**/
}
void drop_alarm_clock(int i){
for(;i<my_alarm_clock.point-1;i++){
my_alarm_clock.clock[i]=my_alarm_clock.clock[i+1];
}
my_alarm_clock.point--;

if(my_alarm_clock.point==0)
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
else
  set_alarm_clock(get_next_alarm_clock());
}












