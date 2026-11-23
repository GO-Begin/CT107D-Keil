#include <STC15F2K60S2.H>
#include "Init.h"
#include "Timer1.h"
#include "SEG.h"
#include "BEEP.h"
#include "MatrixKey.h"
#include "Timer0.h"

/*变量声明区*/
unsigned char SEG_Num[]={'#',10,10,10,10,10,10,10,10};	//用于数码管段选
unsigned char i;										//用于数码管位选
unsigned char Time_20ms,Time_240ms;						//20ms每次扫描数码管，240ms是BEEP_MODE=3时的周期
unsigned char BEEP_Flag,BEEP_Flag_New,BEEP_Flag_Old;	//蜂鸣器标志位，确定BEEP_Flag的值是否变化
unsigned char KeyNum;									//键值
unsigned char Min,Sec,MS;								//倒计时的分，秒，毫秒
unsigned char T0cnt;									//倒计时中断计数，10次为10毫秒
unsigned char BEEP_MODE;								//BEEP_MODE模式选择
unsigned int Time_1000ms,Time_500ms;					//1000ms是BEEP_MODE=1时的周期，500ms是BEEP_MODE=2时的周期

/*按键处理函数*/
void Key_Process()
{
	if(KeyNum)
	{
		/*按下对应按键操控Min，Sec，MS的值并有防溢出处理*/
		if(KeyNum==7)
		Min++;
		if(KeyNum==6&&Min!=0)
		Min--;
		if(KeyNum==11&&Sec!=59)
		Sec++;
		if(KeyNum==10&&Sec!=0)
		Sec--;
		if(KeyNum==15&&MS!=99)
		MS++;
		if(KeyNum==14&&MS!=0)
		MS--;
		
		/*按下S18，开始倒计时*/
		if(KeyNum==18)
		{
			Timer0_Init();
			BEEP_MODE=1;
		}
		
		/*按下S19，时间清零*/
		if(KeyNum==19)
		Min=Sec=MS=0;
	}
}

/*数码管处理函数*/
void SEG_Process()
{
	/*用于数码管位选和段选*/
	if(++i>8)i=1;
	
	/*处理小数点*/
	if(SEG_Num[i]>='.')
	SEG_Show(i,SEG_Num[i]-'.',1);
	else
	SEG_Show(i,SEG_Num[i],0);
}

void main()
{
	/*初始化*/
	System_Init();
	Timer1_Init();
	
	ET0=0;
	while(1)
	{
		/*读取键值*/
		KeyNum=MatrixKey();
		Key_Process();
		
		/*处理BEEP_Flag的新旧值*/
		BEEP_Flag_Old=BEEP_Flag_New;
		BEEP_Flag_New=BEEP_Flag;
		
		/*数码管静态*/
		if(BEEP_MODE==0)
		{
			SEG_Num[3]=11;
			SEG_Num[6]=11;
		}
		
		/*当BEEP_Flag的值发生变化，控制蜂鸣器与LED*/
		if(BEEP_Flag_Old!=BEEP_Flag_New)
		{
			ET1=0;	//暂时关闭定时器1中断，避免影响P0寄存器
			
			/*BEEP_Flag为1时打开蜂鸣器和LED*/
			if(BEEP_Flag)
			BEEP_ON();
			
			/*BEEP_Flag为0时关闭蜂鸣器和LED*/
			else
			BEEP_OFF();
			ET1=1;
		}
		
		/*实时更新时间*/
		SEG_Num[1]=Min/10;
		SEG_Num[2]=Min%10;
		SEG_Num[4]=Sec/10;
		SEG_Num[5]=Sec%10;
		SEG_Num[7]=MS/10;
		SEG_Num[8]=MS%10;
	}
}

/*定时器1中断函数，用于扫描数码管，定时扫描按键与控制蜂鸣器与LED的频率*/
void Timer1_Routine() interrupt 3
{
	SEG_Process();
	
	Time_20ms++;
	
	/*BEEP_MODE=1时为慢速模式*/
	if(BEEP_MODE==1)
	{
		Time_1000ms++;
		if(Time_1000ms>=1000)
		Time_1000ms=0;
		
		/*需要响蜂鸣器与LED闪烁的时间段*/
		if(Time_1000ms<200||(Time_1000ms>400&&Time_1000ms<600))
		{
			SEG_Num[3]=11;
			SEG_Num[6]=11;
			BEEP_Flag=1;
		}
		
		/*关闭蜂鸣器与LED熄灭的时间段*/
		else
		{
			SEG_Num[3]=10;
			SEG_Num[6]=10;
			BEEP_Flag=0;
		}
	}
	
	/*BEEP_MODE=2时为中速模式*/
	else if(BEEP_MODE==2)
	{
		Time_500ms++;
		if(Time_500ms>=500)
		Time_500ms=0;
		
		/*需要响蜂鸣器与LED闪烁的时间段*/
		if(Time_500ms<125||(Time_500ms>250&&Time_500ms<375))
		{
			SEG_Num[3]=11;
			SEG_Num[6]=11;
			BEEP_Flag=1;
		}
		
		/*关闭蜂鸣器与LED熄灭的时间段*/
		else
		{
			SEG_Num[3]=10;
			SEG_Num[6]=10;
			BEEP_Flag=0;
		}
	}
	
	/*BEEP_MODE=3为快速模式*/
	else if(BEEP_MODE==3)
	{
		Time_240ms++;
		if(Time_240ms>=240)
		Time_240ms=0;
		
		
		/*需要响蜂鸣器与LED闪烁的时间段*/
		if(Time_240ms<60||(Time_240ms>120&&Time_240ms<180))
		{
			SEG_Num[3]=11;
			SEG_Num[6]=11;
			BEEP_Flag=1;
		}
		
		/*关闭蜂鸣器与LED熄灭的时间段*/
		else
		{
			SEG_Num[3]=10;
			SEG_Num[6]=10;
			BEEP_Flag=0;
		}
	}
	
	/*每20ms扫描一次矩阵按键*/
	if(Time_20ms>=20)
	{
		Time_20ms=0;
		Key_Loop();
	}
}

/*定时器0中断函数，用于倒计时*/
void Timer0_Routine() interrupt 1
{
	
	/*每10ms给MS的个位+1*/
	T0cnt++;
	if(T0cnt>=10)
	{
		T0cnt=0;
		if(MS==0)
		{
			
			/*总时间为0即倒计时结束时关闭定时器0，蜂鸣器和LED常开*/
			if(Sec==0&&Min==0)
			{
				ET0=0;
				BEEP_MODE=0;
				BEEP_ON();
			}
			
			/*Min还有值而Sec为0时借位给Sec和MS*/
			else if(Sec==0)
			{
				Min--;
				Sec=59;
				MS=99;
			}
			
			/*Sec还有值时借位给MS*/
			else if(Sec!=0)
			{
				Sec--;
				MS=99;
			}
		}
		
		/*MS有值，自身减1*/
		else
		MS--;
	}
	
	/*仅剩10秒时切换中速模式*/
	if(Min==0&&Sec==10&&MS==0)
	BEEP_MODE=2;
	
	/*仅剩5秒时切换快速模式*/
	if(Min==0&&Sec==5&&MS==0)
	BEEP_MODE=3;
}