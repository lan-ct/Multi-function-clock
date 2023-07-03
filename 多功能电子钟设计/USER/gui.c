#include "gui.h"
u16 bk_color = BLACK;
const float PI=3.1415926;
int now;
int confirm=1;
int change_hour=1;
int add=0;
int sub=0;
int alarm_allow=1;
char usart_data[20];
int pos=0;
int input=0;
int alarm_clock_working=0;
int dropi=0;
int last_dropi=0;
int continue_=0;
int stop=0;
int restart=0;
int hour_count_down;
int minute_count_down;
int second_count_down;

extern struct alarm_clock my_alarm_clock;
struct button set_time_button;
struct button set_clock_button;
struct button countdown_button;
struct button count_button;
struct button set_time_port_button;
struct button set_time_click_button;
struct button add_button1;
struct button add_button2;
struct button add_button3;
struct button add_button4;
struct button add_button5;
struct button add_button6;
struct button sub_button1;
struct button sub_button2;
struct button sub_button3;
struct button sub_button4;
struct button sub_button5;
struct button sub_button6;
struct button confirm_button;
struct button cancel_button;
struct button drop_button;
struct button add_button;
struct button alarm_clock_list[6];
struct button continue_button;
struct button stop_button;
struct button restart_button;

void draw_point(int x1,int y1,u16 color){
	POINT_COLOR=color;
	LCD_DrawPoint(x1,y1);
}
void fill_circle(int uRow,int uCol,int size,u16 color){
  int i=0;   
	POINT_COLOR=color;
	for(;i<=size;i++){
	LCD_Draw_Circle(uRow,uCol,size-i);
	}
	LCD_DrawRectangle(uRow,uCol,uRow+1,uCol+1);
}
void draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color){
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return;
	if(size==0){draw_point(x1,y1,color);return;} 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance){xerr-=distance;uRow+=incx;}  
		if(yerr>distance){yerr-=distance;uCol+=incy;}
	}  
} 

void back(void){
		LCD_Clear(bk_color);
	  BACK_COLOR=bk_color;
	      
}
void clock(void){
	 
	  int width=4,i,midx=250,midy=250,r=200,show;
	  int size=2,length=size*10,x,y,x1,y1;
	  u16 lineColor=RED;
  	back();
    POINT_COLOR=BRRED;
	  for(i=0;i<width;i++)
	  LCD_Draw_Circle(midx,midy,r+i);
    fill_circle(midx,midy,width,BRRED);
	  for(i=0;i<12;i++){
			x=midx+r*(cos(PI*2*i/12));
			y=midy+r*(sin(PI*2*i/12));
			x1=midx+(r-length)*(cos(PI*2*i/12));
			y1=midy+(r-length)*(sin(PI*2*i/12));
		  draw_bline(x,y,x1,y1,size,lineColor);
			x1=midx+(r-length*2)*(cos(PI*2*i/12-PI/3));
			y1=midy+(r-length*2)*(sin(PI*2*i/12-PI/3));
			show=i+1;
			LCD_ShowNum(x1,y1,show,show/10+1,16);
		}
}
void show_date(void){
	  
	  u8 tbuf[50];
		RTC_DateTypeDef RTC_DateStruct;	
	  RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	  sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
		LCD_ShowString(160,500,210,24,24,tbuf);
	  sprintf((char*)tbuf,"week:%d",RTC_DateStruct.RTC_WeekDay); 
		LCD_ShowString(200,540,210,24,24,tbuf);
	//LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
}
void show_time(void){
	    RTC_TimeTypeDef RTC_TimeStruct;
	    u8 tbuf[40];
      RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
			LCD_ShowString(160,580,210,24,24,tbuf);	
	   if(alarm_clock_working==0){
	    if(RTC_TimeStruct.RTC_Minutes==0&&RTC_TimeStruct.RTC_Seconds<5){
			  BEEP=!BEEP;
				LED0=!LED0;
			}
			else{
			  BEEP=0;
				LED0=1;
			}
		 }
}
void draw_time(void){
	   clear_time_area();
	   draw_hour();
	   draw_minute();
	   draw_second();
}

void draw_hour(void){
     RTC_TimeTypeDef RTC_TimeStruct;
	   int midx=250,midy=250;
//	   u8 tbuf[50];
	   int length=100,x,y,x1,y1;
	   u16 lineColor=BLUE;
     POINT_COLOR=BRRED;
     RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	   if(RTC_TimeStruct.RTC_Hours>12)
	   RTC_TimeStruct.RTC_Hours-=12;
	   x=midx+(length)/3*2*(cos(PI*2*RTC_TimeStruct.RTC_Hours/12+PI*2/12*RTC_TimeStruct.RTC_Minutes/60-PI/2));
		 y=midy+(length)/3*2*(sin(PI*2*RTC_TimeStruct.RTC_Hours/12+PI*2/12*RTC_TimeStruct.RTC_Minutes/60-PI/2));
		 x1=midx;
		 y1=midy;
		 POINT_COLOR=lineColor;
		 LCD_DrawLine(x,y,x1,y1);
}
void draw_minute(void){
	
	   RTC_TimeTypeDef RTC_TimeStruct;
	   int midx=250,midy=250;
	
	   int length=100,x,y,x1,y1;
	   u16 lineColor=GREEN;
     POINT_COLOR=BRRED;
     RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	
	   x=midx+(length)*(cos(PI*2*RTC_TimeStruct.RTC_Minutes/60-PI/2));
		 y=midy+(length)*(sin(PI*2*RTC_TimeStruct.RTC_Minutes/60-PI/2));
		 x1=midx;
		 y1=midy;
		 POINT_COLOR=lineColor;
		 LCD_DrawLine(x,y,x1,y1);
	    
}
void draw_second(void){
	   RTC_TimeTypeDef RTC_TimeStruct;
	   int midx=250,midy=250;
	
	   int length=100,x,y,x1,y1;
	   u16 lineColor=RED;
     POINT_COLOR=BRRED;
     RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	
	   x=midx+(length)*(cos(PI*2*RTC_TimeStruct.RTC_Seconds/60-PI/2));
		 y=midy+(length)*(sin(PI*2*RTC_TimeStruct.RTC_Seconds/60-PI/2));
		 x1=midx;
		 y1=midy;
	   POINT_COLOR=lineColor;
		 LCD_DrawLine(x,y,x1,y1);
}
void clear_time_area(void){
	  int midx=250,midy=250;
	  int length=100;
	  LCD_Fill(midx-length,midy-length,midx+length,midy+length,bk_color);
	  //fill_circle(midx,midy,length,bk_color);
}
void show_temprature(void){	 
   	int begin=140;
    int temp=Get_Temprate();	//得到温度值 
	  //160,580,210,24,24,tbuf
  	LCD_ShowString(begin,620,230,24,24,"Temprature: 00.00C");//先在固定位置显示小数点	 

		if(temp<0)
		{
			temp=-temp;
			LCD_ShowString(begin+11*12,620,16,16,16,"-");	    //显示负号
		}else LCD_ShowString(begin+11*12,620,16,16,16," ");	//无符号
		//LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
		//	LCD_ShowNum(x1,y1,show,show/10+1,16);
		LCD_ShowxNum(begin+12*12,620,temp/100,2,24,24);		//显示整数部分
		LCD_ShowxNum(begin+15*12,620,temp%100,2,24,24);		//显示小数部分 
}
void set_time_click_buttons(void){
   int beginx=10,beginy=10,height=100,width=100,between=200,betweeny=10;
   
	
   add_button1.x=beginx;
   add_button1.y=beginy;
   add_button1.x1=beginx+width;
   add_button1.y1=beginy+height;
   add_button1.color=RED;
	 strcpy(add_button1.str,"+");
	 draw_button(add_button1);
	
	 sub_button1.x= add_button1.x1+between;
   sub_button1.y= add_button1.y;
   sub_button1.x1=sub_button1.x+width;
   sub_button1.y1=sub_button1.y+height;
   sub_button1.color=RED;
	 strcpy(sub_button1.str,"-");
	 draw_button(sub_button1);
	
	 add_button2.x=beginx;
   add_button2.y=add_button1.y1+betweeny;
   add_button2.x1=add_button2.x+width;
   add_button2.y1=add_button2.y+height;
   add_button2.color=RED;
	 strcpy(add_button2.str,"+");
	 draw_button(add_button2);
	 
	 sub_button2.x= add_button1.x1+between;
   sub_button2.y= add_button2.y;
   sub_button2.x1=sub_button2.x+width;
   sub_button2.y1=sub_button2.y+height;
   sub_button2.color=RED;
	 strcpy(sub_button2.str,"-");
	 draw_button(sub_button2);
	
	 add_button3.x=beginx;
   add_button3.y=add_button2.y1+betweeny;
   add_button3.x1=add_button3.x+width;
   add_button3.y1=add_button3.y+height;
   add_button3.color=RED;
	 strcpy(add_button3.str,"+");
	 draw_button(add_button3);
	 
	 sub_button3.x= add_button3.x1+between;
   sub_button3.y= add_button3.y;
   sub_button3.x1=sub_button3.x+width;
   sub_button3.y1=sub_button3.y+height;
   sub_button3.color=RED;
	 strcpy(sub_button3.str,"-");
	 draw_button(sub_button3);
	
   add_button4.x=beginx;
   add_button4.y=add_button3.y1+betweeny;
   add_button4.x1=add_button4.x+width;
   add_button4.y1=add_button4.y+height;
   add_button4.color=RED;
	 strcpy(add_button4.str,"+");
	 draw_button(add_button4);
	 
	 sub_button4.x= add_button4.x1+between;
   sub_button4.y= add_button4.y;
   sub_button4.x1=sub_button4.x+width;
   sub_button4.y1=sub_button4.y+height;
   sub_button4.color=RED;
	 strcpy(sub_button4.str,"-");
	 draw_button(sub_button4);
	 
	 add_button5.x=beginx;
   add_button5.y=add_button4.y1+betweeny;
   add_button5.x1=add_button5.x+width;
   add_button5.y1=add_button5.y+height;
   add_button5.color=RED;
	 strcpy(add_button5.str,"+");
	 draw_button(add_button5);
	 
	 sub_button5.x= add_button5.x1+between;
   sub_button5.y= add_button5.y;
   sub_button5.x1=sub_button5.x+width;
   sub_button5.y1=sub_button5.y+height;
   sub_button5.color=RED;
	 strcpy(sub_button5.str,"-");
	 draw_button(sub_button5);
	 
	
	 
	 confirm_button.x=beginx;
   confirm_button.y=add_button5.y1+betweeny*2;
   confirm_button.x1=confirm_button.x+width;
   confirm_button.y1=confirm_button.y+height;
   confirm_button.color=RED;
	 strcpy(confirm_button.str,"confirm");
	 draw_button(confirm_button);
	
	
	 cancel_button.x= confirm_button.x1+between;
   cancel_button.y= confirm_button.y;
   cancel_button.x1=cancel_button.x+width;
   cancel_button.y1=cancel_button.y+height;
   cancel_button.color=RED;
	 strcpy(cancel_button.str,"cancel");
	 draw_button(cancel_button);
}
void set_time_click_click(void){

if(tp_dev.scan(0)){
if(tp_dev.x[0]>add_button1.x&&tp_dev.x[0]<add_button1.x1&&tp_dev.y[0]>add_button1.y&&tp_dev.y[0]<add_button1.y1)
{
  add=1;
}
else if(tp_dev.x[0]>add_button2.x&&tp_dev.x[0]<add_button2.x1&&tp_dev.y[0]>add_button2.y&&tp_dev.y[0]<add_button2.y1)
{
  add=2;
}
else if(tp_dev.x[0]>add_button3.x&&tp_dev.x[0]<add_button3.x1&&tp_dev.y[0]>add_button3.y&&tp_dev.y[0]<add_button3.y1)
{
  add=3;
}
else if(tp_dev.x[0]>add_button4.x&&tp_dev.x[0]<add_button4.x1&&tp_dev.y[0]>add_button4.y&&tp_dev.y[0]<add_button4.y1)
{
  add=4;
}
else if(tp_dev.x[0]>add_button5.x&&tp_dev.x[0]<add_button5.x1&&tp_dev.y[0]>add_button5.y&&tp_dev.y[0]<add_button5.y1)
{
 add=5;
}
else if(tp_dev.x[0]>sub_button1.x&&tp_dev.x[0]<sub_button1.x1&&tp_dev.y[0]>sub_button1.y&&tp_dev.y[0]<sub_button1.y1)
{
  sub=1;
}
else if(tp_dev.x[0]>sub_button2.x&&tp_dev.x[0]<sub_button2.x1&&tp_dev.y[0]>sub_button2.y&&tp_dev.y[0]<sub_button2.y1)
{
  sub=2;
}
else if(tp_dev.x[0]>sub_button3.x&&tp_dev.x[0]<sub_button3.x1&&tp_dev.y[0]>sub_button3.y&&tp_dev.y[0]<sub_button3.y1)
{
  sub=3;
}
else if(tp_dev.x[0]>sub_button4.x&&tp_dev.x[0]<sub_button4.x1&&tp_dev.y[0]>sub_button4.y&&tp_dev.y[0]<sub_button4.y1)
{
  sub=4;
}
else if(tp_dev.x[0]>sub_button5.x&&tp_dev.x[0]<sub_button5.x1&&tp_dev.y[0]>sub_button5.y&&tp_dev.y[0]<sub_button5.y1)
{
  sub=5;
}
else if(tp_dev.x[0]>confirm_button.x&&tp_dev.x[0]<confirm_button.x1&&tp_dev.y[0]>confirm_button.y&&tp_dev.y[0]<confirm_button.y1)
{
	confirm=1;
  now=CLOCK;
}
else if(tp_dev.x[0]>cancel_button.x&&tp_dev.x[0]<cancel_button.x1&&tp_dev.y[0]>cancel_button.y&&tp_dev.y[0]<cancel_button.y1)
{
  confirm=0;
  now=CLOCK;
}
}
}
int get_max_day(int year,int month){
    
	  int day1[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
		if(year%100!=0&&year%4==0||year%400==0){
		day1[2]=29;
		}
		return day1[month];
}
int get_length_year(int year){
    if(year%100!=0&&year%4==0||year%400==0){
		return 366;
		}
		return 365;
}
int get_week_between(int year,int month,int day){
    int day1[13]={0,31,28,31,30,31,30,31,31,30,31,30,31},temp;
    int day2[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
		RTC_DateTypeDef RTC_DateStruct;	
		int week,lower=1;
	  RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
		RTC_DateStruct.RTC_Year+=2000;
		week=0;
		
		
		if(year>RTC_DateStruct.RTC_Year||(year==RTC_DateStruct.RTC_Year&&month>RTC_DateStruct.RTC_Month)||(year==RTC_DateStruct.RTC_Year&&month==RTC_DateStruct.RTC_Month&&day>RTC_DateStruct.RTC_Date)){
		  temp=year;
			year=RTC_DateStruct.RTC_Year;
			RTC_DateStruct.RTC_Year=temp;
			temp=month;
			month=RTC_DateStruct.RTC_Month;
			RTC_DateStruct.RTC_Month=temp;
			temp=day;
			day=RTC_DateStruct.RTC_Date;
			RTC_DateStruct.RTC_Date=temp;
			lower=!lower;
		}
		if(year%100!=0&&year%4==0||year%400==0){
		day1[2]=29;
		
		}
		if( RTC_DateStruct.RTC_Year%100!=0&& RTC_DateStruct.RTC_Year%4==0||RTC_DateStruct.RTC_Year%400==0){
		day2[2]=29;
	
		}

		if(year<RTC_DateStruct.RTC_Year){
		while(year<RTC_DateStruct.RTC_Year){
		for(;month<13;month++){
		   week+=day1[month];
		}
		month=1;
		year++;
		}
		
		for(;month<RTC_DateStruct.RTC_Month;month++){
		   week+=day2[month];
		}
		week+=RTC_DateStruct.RTC_Date-day;
		}
		else if(month<RTC_DateStruct.RTC_Month){
			for(;month<RTC_DateStruct.RTC_Month;month++){
		   week+=day2[month];
		  }
			week+=RTC_DateStruct.RTC_Date-day;
		}
		else if(day<RTC_DateStruct.RTC_Date){
		  week+=RTC_DateStruct.RTC_Date-day;
		}
		week%=7;
		if(lower){
		week=RTC_DateStruct.RTC_WeekDay-week;
		if(week<1){
		 week+=7;
		}
		}
		else{
		week=RTC_DateStruct.RTC_WeekDay+week;
		if(week>7){
		week-=7;
		}
		}
		return week;
}
void set_time_click(void){   
	    int beginx=10,beginy=10,height=100,width=100,betweeny=10,i=0;
      RTC_TimeTypeDef RTC_TimeStruct;
	    
	    int year,month,date,hour,minute,week;
      RTC_DateTypeDef RTC_DateStruct;	
	    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	    year=RTC_DateStruct.RTC_Year+2000;
	    month=RTC_DateStruct.RTC_Month;
	    date=RTC_DateStruct.RTC_Date;
	    week=RTC_DateStruct.RTC_WeekDay;    
	
	    LCD_Clear(bk_color);
	    BACK_COLOR=LIGHTBLUE;
	    POINT_COLOR=LIGHTGREEN;
      RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	    hour=RTC_TimeStruct.RTC_Hours;
	    minute=RTC_TimeStruct.RTC_Minutes;
	
	    set_time_click_buttons();
	    
	    LCD_ShowString(beginx+width+10,beginy+height/2,100,24,24,"year:");
	    LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,year,4,24,24);		
			
	    LCD_ShowString(beginx+width+10,beginy+height/2+(height+betweeny)*++i,100,24,24,"month:");
		  LCD_ShowxNum(beginx+width+10+12*6,beginy+height/2+(height+betweeny)*i,month,4,24,24);		
			
			LCD_ShowString(beginx+width+10,beginy+height/2+(height+betweeny)*++i,100,24,24,"date:");
		  LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*i,date,4,24,24);		

			
			LCD_ShowString(beginx+width+10,beginy+height/2+(height+betweeny)*++i,100,24,24,"hour:");
			LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*i,hour,2,24,24);		
			
			LCD_ShowString(beginx+width+10,beginy+height/2+(height+betweeny)*++i,100,24,24,"minute:");
			LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*i,minute,2,24,24);	
			
			
			
	    while(now==SETTIMECLICK){
			  set_time_click_click();
				switch(add){
					case 1:year++;if(year<0)year=0;LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,year,4,24,24);break;
					case 2:month++;if(month<1)month=12;if(month>12)month=1;LCD_ShowxNum(beginx+width+10+12*6,beginy+height/2+(height+betweeny),month,4,24,24);break;
					case 3:date++;if(date<1)date=get_max_day(year,month);if(date>get_max_day(year,month))date=1;LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*2,date,4,24,24);break;
				  case 4:hour++; if(hour<0)hour=23;if(hour>23)hour=0;LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*3,hour,2,24,24);break;	
				  case 5:minute++;if(minute<0)minute=59;if(minute>59)minute=0;LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*4,minute,2,24,24);break;
				}
				add=0;
				switch(sub){
					case 1:year--;if(year<0)year=0;LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,year,4,24,24);
					case 2:month--;if(month<1)month=12;if(month>12)month=1;LCD_ShowxNum(beginx+width+10+12*6,beginy+height/2+(height+betweeny),month,4,24,24);
					case 3:date--;if(date<1)date=get_max_day(year,month);if(date>get_max_day(year,month))date=1;LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*2,date,4,24,24);break;
				  case 4:hour--; if(hour<0)hour=23;if(hour>23)hour=0;LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*3,hour,2,24,24);break;	
				  case 5:minute--;if(minute<0)minute=59;if(minute>59)minute=0;LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*4,minute,2,24,24);break;
				}
				sub=0;

				delay_ms(100);
			}
			if(confirm==1){
			week=get_week_between(year,month,date);
			RTC_Set_Date(year-2000,month,date,week);
			RTC_Set_Time(hour,minute,0,RTC_H12_AM);
			}
	    
}
void set_time_str(char*str){
     int hour=0,minute=0;
	   int i=0;
	if(str[0]=='0'&&str[1]!=':')
		return;
	while(str[i]!=':'){
	if(str[i]<'0'||str[i]>'9')
		return;
	hour*=10;
	hour+=str[i]-'0';
	i++;
	}
	i++;
	while(str[i]!='\0'){
	if(str[i]<'0'||str[i]>'9')
		return;
	minute*=10;
	minute+=str[i]-'0';
	i++;
	}
	if(hour>23||minute>59){
	return;
	}
	RTC_Set_Time(hour,minute,0,RTC_H12_AM);
}
void set_time_usart_buttons(void){
   int beginx=10,beginy=10,height=100,width=100,between=200,betweeny=10+height;
   confirm_button.x=beginx;
   confirm_button.y=beginy+betweeny*6;
   confirm_button.x1=confirm_button.x+width;
   confirm_button.y1=confirm_button.y+height;
   confirm_button.color=RED;
	 strcpy(confirm_button.str,"confirm");
	 draw_button(confirm_button);
	
	
	 cancel_button.x= confirm_button.x1+between;
   cancel_button.y= confirm_button.y;
   cancel_button.x1=cancel_button.x+width;
   cancel_button.y1=cancel_button.y+height;
   cancel_button.color=RED;
	 strcpy(cancel_button.str,"cancel");
	 draw_button(cancel_button);
}
void set_time_usart_click(void){
	
if(tp_dev.scan(0)){
if(tp_dev.x[0]>confirm_button.x&&tp_dev.x[0]<confirm_button.x1&&tp_dev.y[0]>confirm_button.y&&tp_dev.y[0]<confirm_button.y1)
{
	confirm=1;
  now=CLOCK;
}
else if(tp_dev.x[0]>cancel_button.x&&tp_dev.x[0]<cancel_button.x1&&tp_dev.y[0]>cancel_button.y&&tp_dev.y[0]<cancel_button.y1)
{
  confirm=0;
  now=CLOCK;
}
	 }
}
void set_time_usart(void){
     
	    //u8 show3[50];
     
	     usart_data[0]='\0';
       LCD_Clear(bk_color);
	 
	     confirm=1;
	     usart_data[0]='\0';
	     POINT_COLOR=WHITE;
	     LCD_ShowString(40,300,210,24,24,(u8*)"input:");
	     set_time_usart_buttons();
	     while(now==SETTIMEUSART){	 
	  	 if(input==1){
			 LCD_Fill(40+12*6,300,40+12*11,320, bk_color);
			 LCD_ShowString(40+12*6,300,210,24,24,(u8*)usart_data);
			 input=0;
			 }
			 set_time_usart_click();
			 delay_ms(100);
			 }
			if(confirm==1){
			  set_time_str(usart_data);
			}
			usart_data[0]='\0';
}
void draw_button(struct button b){
      LCD_Fill(b.x,b.y,b.x1,b.y1,b.color);
	    POINT_COLOR=LIGHTGREEN;
	
	    LCD_DrawLine(b.x,b.y,b.x,b.y1);
	    LCD_DrawLine(b.x1,b.y,b.x1,b.y1);
	    
	    LCD_DrawLine(b.x,b.y,b.x1,b.y);
	    LCD_DrawLine(b.x,b.y1,b.x1,b.y1);
	
	    POINT_COLOR=WHITE;
	    BACK_COLOR=b.color;
	    LCD_ShowString(b.x+10,(b.y+b.y1)/2-10,(b.x1-b.x)*4,24,24,(u8*)b.str);
	    BACK_COLOR=bk_color;
}
void draw_between(struct button left,struct button right,int color){
      LCD_Fill(left.x1,left.y,right.x,right.y1,color);
	    POINT_COLOR=LIGHTGREEN;
	
	    LCD_DrawLine(left.x1,left.y,right.x,right.y);
	    LCD_DrawLine(left.x1,left.y,left.x1,left.y1);
	    
	    LCD_DrawLine(left.x1,left.y1,right.x,right.y1);
	    LCD_DrawLine(right.x1,right.y,right.x1,right.y1);
	
	    BACK_COLOR=color;
	    BACK_COLOR=bk_color;
}
void clock_buttons_click(void){
if(tp_dev.scan(0)){
if(tp_dev.x[0]>set_time_button.x&&tp_dev.x[0]<set_time_button.x1&&tp_dev.y[0]>set_time_button.y&&tp_dev.y[0]<set_time_button.y1)
{
  now=SETTIME;
}
else if(tp_dev.x[0]>set_clock_button.x&&tp_dev.x[0]<set_clock_button.x1&&tp_dev.y[0]>set_clock_button.y&&tp_dev.y[0]<set_clock_button.y1){
	now=ALARMCLOCK;
}
else if(tp_dev.x[0]>count_button.x&&tp_dev.x[0]<count_button.x1&&tp_dev.y[0]>count_button.y&&tp_dev.y[0]<count_button.y1){
	now=COUNT;
}
}
}
void clock_buttons(void){
	 int beginx=2,beginy=660,height=100,width=150;
   
	
   set_time_button.x=beginx;
	 set_time_button.y=beginy;
   set_time_button.x1=beginx+width;
   set_time_button.y1=beginy+height;
   set_time_button.color=RED;
	 strcpy(set_time_button.str,"set-time");
	 draw_button(set_time_button);
	
	 set_clock_button.x=beginx+width;
	 set_clock_button.y=beginy;
   set_clock_button.x1=set_clock_button.x+width;
   set_clock_button.y1=set_clock_button.y+height;
   set_clock_button.color=RED;
	 strcpy(set_clock_button.str,"alarm");
	 draw_button(set_clock_button);
	
	
	 count_button.x=beginx+width*2;
	 count_button.y=beginy;
   count_button.x1= count_button.x+width;
   count_button.y1= count_button.y+height;
   count_button.color=RED;
	 strcpy(count_button.str,"count");
	 draw_button(count_button);
}
void show_clock(void){
	back();
  clock();
//	clock_buttons();
	while(now==CLOCK) 
	{		
			//每100ms更新一次显示数据
			show_date();
			show_time();
			draw_time();
			show_temprature();
	 // BEEP=!BEEP;
		  
		 // clock_buttons_click();
		  delay_ms(100);/**/
	}	
}
void set_alarm_clock_menu_buttons(void){
   int beginx=100,beginy=100,height=200,width=200;
	
   add_button.x=beginx;
	 add_button.y=beginy;
   add_button.x1=beginx+width;
   add_button.y1=beginy+height;
   add_button.color=RED;
	 strcpy(add_button.str,"add alarm clock");
	 draw_button(add_button);
	
	 drop_button.x=beginx;
	 drop_button.y=beginy+height*2;
   drop_button.x1=drop_button.x+width;
   drop_button.y1=drop_button.y+height;
   drop_button.color=RED;
	 strcpy(drop_button.str,"drop alarm clock");
	 draw_button(drop_button);
}
void set_alarm_clock_menu_click(void){
if(tp_dev.scan(0)){
if(tp_dev.x[0]>add_button.x&&tp_dev.x[0]<add_button.x1&&tp_dev.y[0]>add_button.y&&tp_dev.y[0]<add_button.y1)
{
  now=ADDALARMCLOCK;
}
else if(tp_dev.x[0]>drop_button.x&&tp_dev.x[0]<drop_button.x1&&tp_dev.y[0]>drop_button.y&&tp_dev.y[0]<drop_button.y1)
{
  now=DROPALARMCLOCK;
}
}
}
void set_alarm_clock_menu(void){
	  now=ALARMCLOCK;
	  back();
	  set_alarm_clock_menu_buttons();
	  while(now==ALARMCLOCK){
		set_alarm_clock_menu_click();
		delay_ms(100);
		};
}
void add_alarm_clock_buttons(void){
 int beginx=10,beginy=10,height=100,width=100,between=200,betweeny=10+height;
   
	
   add_button1.x=beginx;
   add_button1.y=beginy;
   add_button1.x1=beginx+width;
   add_button1.y1=beginy+height;
   add_button1.color=RED;
	 strcpy(add_button1.str,"+");
	 draw_button(add_button1);
	
	 sub_button1.x= add_button1.x1+between;
   sub_button1.y= add_button1.y;
   sub_button1.x1=sub_button1.x+width;
   sub_button1.y1=sub_button1.y+height;
   sub_button1.color=RED;
	 strcpy(sub_button1.str,"-");
	 draw_button(sub_button1);
	
	 add_button2.x=beginx;
   add_button2.y=add_button1.y1+betweeny;
   add_button2.x1=add_button2.x+width;
   add_button2.y1=add_button2.y+height;
   add_button2.color=RED;
	 strcpy(add_button2.str,"+");
	 draw_button(add_button2);
	 
	 sub_button2.x= add_button1.x1+between;
   sub_button2.y= add_button2.y;
   sub_button2.x1=sub_button2.x+width;
   sub_button2.y1=sub_button2.y+height;
   sub_button2.color=RED;
	 strcpy(sub_button2.str,"-");
	 draw_button(sub_button2);
	
	 add_button3.x=beginx;
   add_button3.y=add_button2.y1+betweeny;
   add_button3.x1=add_button3.x+width;
   add_button3.y1=add_button3.y+height;
   add_button3.color=RED;
	 strcpy(add_button3.str,"+");
	 draw_button(add_button3);
	 
	 sub_button3.x= add_button3.x1+between;
   sub_button3.y= add_button3.y;
   sub_button3.x1=sub_button3.x+width;
   sub_button3.y1=sub_button3.y+height;
   sub_button3.color=RED;
	 strcpy(sub_button3.str,"-");
	 draw_button(sub_button3);
	 
	 confirm_button.x=beginx;
   confirm_button.y=beginy+betweeny*6;
   confirm_button.x1=confirm_button.x+width;
   confirm_button.y1=confirm_button.y+height;
   confirm_button.color=RED;
	 strcpy(confirm_button.str,"confirm");
	 draw_button(confirm_button);
	
	
	 cancel_button.x= confirm_button.x1+between;
   cancel_button.y= confirm_button.y;
   cancel_button.x1=cancel_button.x+width;
   cancel_button.y1=cancel_button.y+height;
   cancel_button.color=RED;
	 strcpy(cancel_button.str,"cancel");
	 draw_button(cancel_button);
	
}
void add_alarm_clock_click(void){
     if(tp_dev.scan(0)){
if(tp_dev.x[0]>add_button1.x&&tp_dev.x[0]<add_button1.x1&&tp_dev.y[0]>add_button1.y&&tp_dev.y[0]<add_button1.y1)
{
  add=1;
}
else if(tp_dev.x[0]>add_button2.x&&tp_dev.x[0]<add_button2.x1&&tp_dev.y[0]>add_button2.y&&tp_dev.y[0]<add_button2.y1)
{
  add=2;
}
else if(tp_dev.x[0]>add_button3.x&&tp_dev.x[0]<add_button3.x1&&tp_dev.y[0]>add_button3.y&&tp_dev.y[0]<add_button3.y1)
{
  add=3;
}
else if(tp_dev.x[0]>sub_button1.x&&tp_dev.x[0]<sub_button1.x1&&tp_dev.y[0]>sub_button1.y&&tp_dev.y[0]<sub_button1.y1)
{
  sub=1;
}
else if(tp_dev.x[0]>sub_button2.x&&tp_dev.x[0]<sub_button2.x1&&tp_dev.y[0]>sub_button2.y&&tp_dev.y[0]<sub_button2.y1)
{
  sub=2;
}
else if(tp_dev.x[0]>sub_button3.x&&tp_dev.x[0]<sub_button3.x1&&tp_dev.y[0]>sub_button3.y&&tp_dev.y[0]<sub_button3.y1)
{
  sub=3;
}
else if(tp_dev.x[0]>confirm_button.x&&tp_dev.x[0]<confirm_button.x1&&tp_dev.y[0]>confirm_button.y&&tp_dev.y[0]<confirm_button.y1)
{
	confirm=1;
  now=CLOCK;
}
else if(tp_dev.x[0]>cancel_button.x&&tp_dev.x[0]<cancel_button.x1&&tp_dev.y[0]>cancel_button.y&&tp_dev.y[0]<cancel_button.y1)
{
  confirm=0;
  now=CLOCK;
}
}
}
void add_alarm_clock_gui(void){
      RTC_TimeTypeDef RTC_TimeStruct;
	    int week,hour,minute;
	     int beginx=10,beginy=10,height=100,width=100,betweeny=10;	   
	    LCD_Clear(bk_color);
	   
	    POINT_COLOR=LIGHTGREEN;
      RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	    hour=RTC_TimeStruct.RTC_Hours;
	    minute=RTC_TimeStruct.RTC_Minutes;
			week=0;
	    add_alarm_clock_buttons();
			LCD_ShowString(beginx+width+10,beginy+height/2,100,24,24,"week:");
	    LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,week,4,24,24);		
			
	    LCD_ShowString(beginx+width+10,beginy+height/2+(height+betweeny)*2,100,24,24,"hour:");
		  LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*2,hour,4,24,24);		
			
			LCD_ShowString(beginx+width+10,beginy+height/2+(height+betweeny)*4,100,24,24,"minute:");
		  LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*4,minute,4,24,24);		
			
	    while(now==ADDALARMCLOCK){	
			add_alarm_clock_click();
				switch(add){
	        case 1:week++;if(week<0)week=7;if(week>7)week=0;  LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,week,4,24,24);break;				
				  case 2:hour++; if(hour<0)hour=23;if(hour>23)hour=0; LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*2,hour,4,24,24);break;	
				  case 3:minute++;if(minute<0)minute=59;if(minute>59)minute=0; LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*4,minute,4,24,24);break;
				}
				add=0;
				switch(sub){
	        case 1:week--;if(week<0)week=7;if(week>7)week=0;  LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,week,4,24,24);break;				
				  case 2:hour--; if(hour<0)hour=23;if(hour>23)hour=0; LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2+(height+betweeny)*2,hour,4,24,24);break;	
				  case 3:minute--;if(minute<0)minute=59;if(minute>59)minute=0; LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*4,minute,4,24,24);break;
				}
				sub=0;
				delay_ms(100);
	    }
			if(confirm==1){
			 struct time temp;
			 temp.hour=hour;
			 temp.minute=minute;
			 temp.week=week;
		   add_alarm_clock(temp);
			}
}
void drop_alarm_clock_buttons(void){
   int i=0;   
	 int beginx=10,beginy=10,height=100,width=400,between=height+10;
	 u8 buf[30];
	 for(i=0;i<my_alarm_clock.point;i++){
	 alarm_clock_list[i].x=beginx;
	 alarm_clock_list[i].y=beginy+between*i;
   alarm_clock_list[i].x1=beginx+width;
	 alarm_clock_list[i].y1=alarm_clock_list[i].y+height;
	 sprintf((char*)buf,"week:%02d %02d:%02d",my_alarm_clock.clock[i].week,my_alarm_clock.clock[i].hour,my_alarm_clock.clock[i].minute); 
	 strcpy( alarm_clock_list[i].str,(char*)buf);
	 alarm_clock_list[i].color=RED;
	 draw_button( alarm_clock_list[i]);
	 }
	 if(my_alarm_clock.point!=0){
	 i=my_alarm_clock.point;
	 confirm_button.x=beginx;
   confirm_button.y=beginy+between*i;
   confirm_button.x1=confirm_button.x+width;
   confirm_button.y1=confirm_button.y+height;
   confirm_button.color=RED;
	 strcpy(confirm_button.str,"confirm");
	 draw_button(confirm_button);
	 }
	 i++;
	 cancel_button.x= beginx;
   cancel_button.y= beginy+between*i;
   cancel_button.x1=cancel_button.x+width;
   cancel_button.y1=cancel_button.y+height;
   cancel_button.color=RED;
	 strcpy(cancel_button.str,"cancel");
	 draw_button(cancel_button);
}
void drop_alarm_clock_click(void){
int i=0;
if(tp_dev.scan(0)){
if(tp_dev.x[0]>confirm_button.x&&tp_dev.x[0]<confirm_button.x1&&tp_dev.y[0]>confirm_button.y&&tp_dev.y[0]<confirm_button.y1)
{
	confirm=1;
  now=CLOCK;
}
else if(tp_dev.x[0]>cancel_button.x&&tp_dev.x[0]<cancel_button.x1&&tp_dev.y[0]>cancel_button.y&&tp_dev.y[0]<cancel_button.y1)
{
  confirm=0;
  now=CLOCK;
}

for(i=0;i<my_alarm_clock.point;i++){
if(tp_dev.x[0]>alarm_clock_list[i].x&&tp_dev.x[0]<alarm_clock_list[i].x1&&tp_dev.y[0]>alarm_clock_list[i].y&&tp_dev.y[0]<alarm_clock_list[i].y1)
{
	last_dropi=dropi;
  dropi=i;
	alarm_clock_list[last_dropi].color=RED;
	draw_button(alarm_clock_list[last_dropi]);
	alarm_clock_list[dropi].color= GRAY;
	draw_button( alarm_clock_list[dropi]);
	break;
}
}
}
}
void drop_alarm_clock_gui(void){
	    dropi=0;    
	    LCD_Clear(bk_color);
	    drop_alarm_clock_buttons();
	    alarm_clock_list[dropi].color= GRAY;
	    draw_button( alarm_clock_list[dropi]);
	    while(now==DROPALARMCLOCK){	
      drop_alarm_clock_click();
			delay_ms(100);				
	    }
			if(confirm==1){
			if(my_alarm_clock.point>0){
			drop_alarm_clock(dropi);
			}
			}
}

void set_time_buttons(void){
   int beginx=100,beginy=100,height=200,width=200;
	
   set_time_port_button.x=beginx;
	 set_time_port_button.y=beginy;
   set_time_port_button.x1=beginx+width;
   set_time_port_button.y1=beginy+height;
   set_time_port_button.color=RED;
	 strcpy(set_time_port_button.str,"port");
	 draw_button(set_time_port_button);
	
	 set_time_click_button.x=beginx;
	 set_time_click_button.y=beginy+height*2;
   set_time_click_button.x1=set_time_click_button.x+width;
   set_time_click_button.y1=set_time_click_button.y+height;
   set_time_click_button.color=RED;
	 strcpy(set_time_click_button.str,"click");
	 draw_button(set_time_click_button);
}
void set_time_buttons_click(void){
if(tp_dev.scan(0)){
if(tp_dev.x[0]>set_time_port_button.x&&tp_dev.x[0]<set_time_port_button.x1&&tp_dev.y[0]>set_time_port_button.y&&tp_dev.y[0]<set_time_port_button.y1)
{
  now=SETTIMEUSART;
}
else if(tp_dev.x[0]>set_time_click_button.x&&tp_dev.x[0]<set_time_click_button.x1&&tp_dev.y[0]>set_time_click_button.y&&tp_dev.y[0]<set_time_click_button.y1){
	now=SETTIMECLICK;
}
}
}
void set_time_menu(void){
	   back();
	   set_time_buttons();
	   while(now==SETTIME){
		 set_time_buttons_click();
		 }
}
void count_click(void){
if(tp_dev.scan(0)){
if(tp_dev.x[0]>continue_button.x&&tp_dev.x[0]<continue_button.x1&&tp_dev.y[0]>continue_button.y&&tp_dev.y[0]<continue_button.y1)
{ 
	 if(stop==1){
	 stop=0; 
	 continue_=1;
	 }
}
else if(tp_dev.x[0]>stop_button.x&&tp_dev.x[0]<stop_button.x1&&tp_dev.y[0]>stop_button.y&&tp_dev.y[0]<stop_button.y1)
{
	 stop=1;
}
else if(tp_dev.x[0]>restart_button.x&&tp_dev.x[0]<restart_button.x1&&tp_dev.y[0]>restart_button.y&&tp_dev.y[0]<restart_button.y1)
{
   restart=1;
	 stop=0;
}
else if(tp_dev.x[0]>confirm_button.x&&tp_dev.x[0]<confirm_button.x1&&tp_dev.y[0]>confirm_button.y&&tp_dev.y[0]<confirm_button.y1)
{
   now=CLOCK;
}
} 
}
void count_buttons(void){
  
	 int beginx=10,beginy=10,height=50,width=400,between=height+10;
	
   continue_button.x=beginx;
   continue_button.y=beginy+between*9;
   continue_button.x1= continue_button.x+width;
   continue_button.y1=continue_button.y+height;
   continue_button.color=RED;
	 strcpy(continue_button.str,"continue");
	 draw_button( continue_button);
	
	 stop_button.x=continue_button.x;
   stop_button.y=continue_button.y+between;
   stop_button.x1=stop_button.x+width;
   stop_button.y1=stop_button.y+height;
   stop_button.color=RED;
	 strcpy(stop_button.str,"stop");
	 draw_button(stop_button);
	 
	 restart_button.x=continue_button.x;
   restart_button.y=stop_button.y+between;
   restart_button.x1=restart_button.x+width;
   restart_button.y1=restart_button.y+height;
   restart_button.color=RED;
	 strcpy(restart_button.str,"restart");
	 draw_button(restart_button);
	 
	 confirm_button.x=continue_button.x;
   confirm_button.y=restart_button.y+between;
   confirm_button.x1=confirm_button.x+width;
   confirm_button.y1=confirm_button.y+height;
   confirm_button.color=RED;
	 strcpy( confirm_button.str,"exit");
	 draw_button(confirm_button);
}
void count_menu(void){
	   u8 tbuf[20];
     int hour=0,minute=0,seconds=0; 
	   int midx=250,midy=250;
	   u16 lineColor=RED;
	   int length=100,x,y,x1,y1;
	   RTC_TimeTypeDef Start_RTC_TimeStruct,Now_RTC_TimeStruct;
     
	   back();
     clock();
	   count_buttons();
	   sprintf((char*)tbuf,"Time:%02d:%02d:%02d",hour,minute,seconds); 
		 LCD_ShowString(160,480,210,24,24,tbuf);	
	   x=midx+(length)*(cos(PI*2*seconds/60-PI/2));
		 y=midy+(length)*(sin(PI*2*seconds/60-PI/2));
		 x1=midx;
		 y1=midy;
	   POINT_COLOR=lineColor;
		 LCD_DrawLine(x,y,x1,y1);
	   restart=0;
	   continue_=0;
	   stop=1;
	   while(now==COUNT){
		 count_click();
		 
		 if(restart==1){
		   RTC_GetTime(RTC_Format_BIN,&Start_RTC_TimeStruct);
			 restart=0;
		 }
		 if(continue_==1){
			 RTC_GetTime(RTC_Format_BIN,&Now_RTC_TimeStruct);
		   Start_RTC_TimeStruct.RTC_Hours=Now_RTC_TimeStruct.RTC_Hours-hour;
			 Start_RTC_TimeStruct.RTC_Minutes=Now_RTC_TimeStruct.RTC_Minutes-minute;
			 Start_RTC_TimeStruct.RTC_Seconds=Now_RTC_TimeStruct.RTC_Seconds-seconds;
			 continue_=0;
		 }
     if(stop==0){
     RTC_GetTime(RTC_Format_BIN,&Now_RTC_TimeStruct);
		 hour=Now_RTC_TimeStruct.RTC_Hours-Start_RTC_TimeStruct.RTC_Hours;
	   minute=Now_RTC_TimeStruct.RTC_Minutes-Start_RTC_TimeStruct.RTC_Minutes;
		 seconds=Now_RTC_TimeStruct.RTC_Seconds-Start_RTC_TimeStruct.RTC_Seconds;
     if(seconds<0){
		 seconds+=60;
		 minute--;
		 }
     if(minute<0){
		 minute+=60;
		 hour--;
		 }
		 clear_time_area();
		 sprintf((char*)tbuf,"Time:%02d:%02d:%02d",hour,minute,seconds); 
		 LCD_ShowString(160,480,210,24,24,tbuf);	
	   x=midx+(length)*(cos(PI*2*seconds/60-PI/2));
		 y=midy+(length)*(sin(PI*2*seconds/60-PI/2));
		 x1=midx;
		 y1=midy;
	   POINT_COLOR=lineColor;
		 LCD_DrawLine(x,y,x1,y1);
		 }
		 
		 delay_ms(100);
		 }
}
void count_down_buttons(void){
   int beginx=10,beginy=10,height=50,width=400,between=height+10;
	
   continue_button.x=beginx;
   continue_button.y=beginy+between*9;
   continue_button.x1= continue_button.x+width;
   continue_button.y1=continue_button.y+height;
   continue_button.color=RED;
	 strcpy(continue_button.str,"continue");
	 draw_button( continue_button);
	
	 stop_button.x=continue_button.x;
   stop_button.y=continue_button.y+between;
   stop_button.x1=stop_button.x+width;
   stop_button.y1=stop_button.y+height;
   stop_button.color=RED;
	 strcpy(stop_button.str,"stop");
	 draw_button(stop_button);
	 
	 confirm_button.x=continue_button.x;
   confirm_button.y=stop_button.y+between;
   confirm_button.x1=confirm_button.x+width;
   confirm_button.y1=confirm_button.y+height;
   confirm_button.color=RED;
	 strcpy( confirm_button.str,"exit");
	 draw_button(confirm_button);
}
void count_down_click(void){
if(tp_dev.scan(0)){
if(tp_dev.x[0]>continue_button.x&&tp_dev.x[0]<continue_button.x1&&tp_dev.y[0]>continue_button.y&&tp_dev.y[0]<continue_button.y1)
{ 
	 if(stop==1){
	 stop=0; 
	 continue_=1;
	 }
}
else if(tp_dev.x[0]>stop_button.x&&tp_dev.x[0]<stop_button.x1&&tp_dev.y[0]>stop_button.y&&tp_dev.y[0]<stop_button.y1)
{
	 stop=1;
}
else if(tp_dev.x[0]>confirm_button.x&&tp_dev.x[0]<confirm_button.x1&&tp_dev.y[0]>confirm_button.y&&tp_dev.y[0]<confirm_button.y1)
{
   now=CLOCK;
}
} 
}
void count_down(void){
     u8 tbuf[20];
     int hour=hour_count_down,minute=minute_count_down,seconds=second_count_down; 
	   int midx=250,midy=250;
	   u16 lineColor=RED;
	   int length=100,x,y,x1,y1;
	   RTC_TimeTypeDef Start_RTC_TimeStruct,Now_RTC_TimeStruct;
     
	   back();
     clock();
	   count_down_buttons();
	   sprintf((char*)tbuf,"Time:%02d:%02d:%02d",hour,minute,seconds); 
		 LCD_ShowString(160,480,210,24,24,tbuf);	
	   x=midx+(length)*(cos(PI*2*seconds/60-PI/2));
		 y=midy+(length)*(sin(PI*2*seconds/60-PI/2));
		 x1=midx;
		 y1=midy;
	   POINT_COLOR=lineColor;
		 LCD_DrawLine(x,y,x1,y1);
	   restart=0;
	   continue_=0;
	   stop=1;
	   while(now==COUNTDOWN){
		 count_down_click();
			if(hour==0&&minute==0&&seconds==0){
			now=CLOCK;
			}
		 if(continue_==1){
			 RTC_GetTime(RTC_Format_BIN,&Now_RTC_TimeStruct);
		   Start_RTC_TimeStruct.RTC_Hours=Now_RTC_TimeStruct.RTC_Hours-hour;
			 Start_RTC_TimeStruct.RTC_Minutes=Now_RTC_TimeStruct.RTC_Minutes-minute;
			 Start_RTC_TimeStruct.RTC_Seconds=Now_RTC_TimeStruct.RTC_Seconds-seconds;
			 continue_=0;
		 }
     if(stop==0){
     RTC_GetTime(RTC_Format_BIN,&Now_RTC_TimeStruct);
		 hour=hour_count_down-(Now_RTC_TimeStruct.RTC_Hours-Start_RTC_TimeStruct.RTC_Hours);
	   minute=minute_count_down-(Now_RTC_TimeStruct.RTC_Minutes-Start_RTC_TimeStruct.RTC_Minutes);
		 seconds=second_count_down-(Now_RTC_TimeStruct.RTC_Seconds-Start_RTC_TimeStruct.RTC_Seconds);
		 if(seconds<0){
		 seconds+=60;
		 minute--;
		 }
     if(minute<0){
		 minute+=60;
		 hour--;
		 }
		 clear_time_area();
		 sprintf((char*)tbuf,"Time:%02d:%02d:%02d",hour,minute,seconds); 
		 LCD_ShowString(160,480,210,24,24,tbuf);	
	   x=midx+(length)*(cos(PI*2*seconds/60-PI/2));
		 y=midy+(length)*(sin(PI*2*seconds/60-PI/2));
		 x1=midx;
		 y1=midy;
	   POINT_COLOR=lineColor;
		 LCD_DrawLine(x,y,x1,y1);
		 }
		 delay_ms(100);
		 }
}
void set_count_down_buttons(void){
   int beginx=10,beginy=10,height=100,width=100,between=200,betweeny=10;
   add_button1.x=beginx;
   add_button1.y=beginy;
   add_button1.x1=beginx+width;
   add_button1.y1=beginy+height;
   add_button1.color=RED;
	 strcpy(add_button1.str,"+");
	 draw_button(add_button1);
	
	 sub_button1.x= add_button1.x1+between;
   sub_button1.y= add_button1.y;
   sub_button1.x1=sub_button1.x+width;
   sub_button1.y1=sub_button1.y+height;
   sub_button1.color=RED;
	 strcpy(sub_button1.str,"-");
	 draw_button(sub_button1);
	
	draw_between(add_button1,sub_button1,LIGHTBLUE);
	 
	 
	
	 add_button2.x=beginx;
   add_button2.y=add_button1.y1+betweeny;
   add_button2.x1=add_button2.x+width;
   add_button2.y1=add_button2.y+height;
   add_button2.color=RED;
	 strcpy(add_button2.str,"+");
	 draw_button(add_button2);
	 
	 sub_button2.x= add_button1.x1+between;
   sub_button2.y= add_button2.y;
   sub_button2.x1=sub_button2.x+width;
   sub_button2.y1=sub_button2.y+height;
   sub_button2.color=RED;
	 strcpy(sub_button2.str,"-");
	 draw_button(sub_button2);
	 
	 draw_between(add_button2,sub_button2,LIGHTBLUE);
	
	 add_button3.x=beginx;
   add_button3.y=add_button2.y1+betweeny;
   add_button3.x1=add_button3.x+width;
   add_button3.y1=add_button3.y+height;
   add_button3.color=RED;
	 strcpy(add_button3.str,"+");
	 draw_button(add_button3);
	 
	 sub_button3.x= add_button3.x1+between;
   sub_button3.y= add_button3.y;
   sub_button3.x1=sub_button3.x+width;
   sub_button3.y1=sub_button3.y+height;
   sub_button3.color=RED;
	 strcpy(sub_button3.str,"-");
	 draw_button(sub_button3);
	 
	 draw_between(add_button3,sub_button3,LIGHTBLUE);
	 
	 betweeny=10+height;
	 confirm_button.x=beginx;
   confirm_button.y=beginy+betweeny*6;
   confirm_button.x1=confirm_button.x+width;
   confirm_button.y1=confirm_button.y+height;
   confirm_button.color=RED;
	 strcpy(confirm_button.str,"confirm");
	 draw_button(confirm_button);
	
	 cancel_button.x= confirm_button.x1+between;
   cancel_button.y= confirm_button.y;
   cancel_button.x1=cancel_button.x+width;
   cancel_button.y1=cancel_button.y+height;
   cancel_button.color=RED;
	 strcpy(cancel_button.str,"cancel");
	 draw_button(cancel_button);
}
void set_count_down_click(void){
 if(tp_dev.scan(0)){
if(tp_dev.x[0]>add_button1.x&&tp_dev.x[0]<add_button1.x1&&tp_dev.y[0]>add_button1.y&&tp_dev.y[0]<add_button1.y1)
{
  add=1;
}
else if(tp_dev.x[0]>add_button2.x&&tp_dev.x[0]<add_button2.x1&&tp_dev.y[0]>add_button2.y&&tp_dev.y[0]<add_button2.y1)
{
  add=2;
}
else if(tp_dev.x[0]>add_button3.x&&tp_dev.x[0]<add_button3.x1&&tp_dev.y[0]>add_button3.y&&tp_dev.y[0]<add_button3.y1)
{
  add=3;
}
else if(tp_dev.x[0]>sub_button1.x&&tp_dev.x[0]<sub_button1.x1&&tp_dev.y[0]>sub_button1.y&&tp_dev.y[0]<sub_button1.y1)
{
  sub=1;
}
else if(tp_dev.x[0]>sub_button2.x&&tp_dev.x[0]<sub_button2.x1&&tp_dev.y[0]>sub_button2.y&&tp_dev.y[0]<sub_button2.y1)
{
  sub=2;
}
else if(tp_dev.x[0]>sub_button3.x&&tp_dev.x[0]<sub_button3.x1&&tp_dev.y[0]>sub_button3.y&&tp_dev.y[0]<sub_button3.y1)
{
  sub=3;
}
else if(tp_dev.x[0]>confirm_button.x&&tp_dev.x[0]<confirm_button.x1&&tp_dev.y[0]>confirm_button.y&&tp_dev.y[0]<confirm_button.y1)
{
	confirm=1;
  now=COUNTDOWN;
}
else if(tp_dev.x[0]>cancel_button.x&&tp_dev.x[0]<cancel_button.x1&&tp_dev.y[0]>cancel_button.y&&tp_dev.y[0]<cancel_button.y1)
{
  confirm=0;
  now=CLOCK;
}
}
}
void set_count_down_menu(void){
     
	    int second=0,hour=0,minute=0;
	    int beginx=10,beginy=10,height=100,width=100,betweeny=10,i=0;	   
	    LCD_Clear(bk_color);
	    
	    POINT_COLOR=WHITE;
      BACK_COLOR=LIGHTBLUE;
			LCD_ShowString(beginx+width+10,beginy+height/2,100,24,24,"hour:");
	    LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,hour,4,24,24);		
			
	    LCD_ShowString(beginx+width+10,beginy+height/2+(height+betweeny)*++i,100,24,24,"minute:");
		  LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*i,minute,4,24,24);		
			
			LCD_ShowString(beginx+width+10,beginy+height/2+(height+betweeny)*++i,100,24,24,"second:");
		  LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*i,second,4,24,24);		
			 set_count_down_buttons();
	    
	    while(now==SETCOUNTDOWN){	
		 set_count_down_click();
				switch(add){
	        case 1:hour++; if(hour<0)hour=23;if(hour>23)hour=0;LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,hour,4,24,24);	break;				
				  case 2:minute++;if(minute<0)minute=59;if(minute>59)minute=0;LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny),minute,4,24,24);	break;
				  case 3:second++;if(second<0)minute=59;if(second>59)second=0; LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*2,second,4,24,24);	break;
				}
				add=0;
				switch(sub){
				  case 1:hour--; if(hour<0)hour=23;if(hour>23)hour=0;LCD_ShowxNum(beginx+width+10+12*5,beginy+height/2,hour,4,24,24);	break;				
				  case 2:minute--;if(minute<0)minute=59;if(minute>59)minute=0;LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny),minute,4,24,24);	break;
				  case 3:second--;if(second<0)minute=59;if(second>59)second=0; LCD_ShowxNum(beginx+width+10+12*7,beginy+height/2+(height+betweeny)*2,second,4,24,24);	break;
				}
				sub=0;
				delay_ms(100);
	    }
			if(confirm==1){
			   hour_count_down=hour;
				 minute_count_down=minute;
				 second_count_down=second;
			}
}
void clock_with_buttons(void){
  back();
  clock();
	clock_buttons();
	while(now==CLOCKBUTTONS) 
	{		
			//每100ms更新一次显示数据
			show_date();
			show_time();
			draw_time();
			show_temprature();
	 // BEEP=!BEEP;
		  
		  clock_buttons_click();
		  delay_ms(100);/**/
	}	
}
void run_menu(void){
	   now=CLOCK;
    while(1){
		 switch(now){
			 case CLOCK:show_clock();break;
			 case SETTIME:set_time_menu();break;
			 case SETTIMECLICK:set_time_click();break;
			 case SETTIMEUSART:set_time_usart();break;
			 case ALARMCLOCK:set_alarm_clock_menu();break;
			 case ADDALARMCLOCK:add_alarm_clock_gui();break;
			 case DROPALARMCLOCK:drop_alarm_clock_gui();break;
			 case COUNT:count_menu();break;
			 case CLOCKBUTTONS:clock_with_buttons();break;
		 }
		 delay_ms(100);
		 } /* */
}
