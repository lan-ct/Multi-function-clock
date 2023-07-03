#include "lcd.h"
#include "math.h"
#include "stdio.h"
#include "rtc.h"
#include "adc.h"
#include "beep.h"
#include "delay.h"
#include "led.h"
#include "string.h"
#include "tpad.h"
#include "touch.h"
#define CLOCK 1
#define SETTIME 2
#define SETTIMECLICK 3
#define SETTIMEUSART 4
#define ALARMCLOCK 5
#define ADDALARMCLOCK 6
#define DROPALARMCLOCK 7
#define COUNTDOWN 8
#define COUNT 9
#define SETCOUNTDOWN 10 
#define CLOCKBUTTONS 11
struct button{
int x;
int y;
int x1;
int y1;
int color;
char str[20];
};
void draw_point(int x1, int y1, u16 color);
void fill_circle(int uRow,int uCol,int size,u16 color);
void draw_bline(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color);
void back(void);
void clock(void);
void show_date(void);
void show_time(void);
void draw_time(void);
void draw_hour(void);
void draw_minute(void);
void draw_second(void);
void clear_time_area(void);
void show_temprature(void);
void set_time_menu(void);
void show_clock(void);
void run_menu(void);
void set_time_click(void);
void add_alarm_clock_gui(void);
void drop_alarm_clock_gui(void);
void draw_button(struct button b);
