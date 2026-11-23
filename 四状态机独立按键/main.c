#include <STC15F2K60S2.H>
#include "SEG.h"
#include "Timer1.h"
#include "Init.h"
#include "Key.h"

#define S4 P33
#define S5 P32
#define S6 P31
#define S7 P30

/*变量声明区*/
unsigned char SEG_Num[]={'#',0,10,10,10,10,10,10,10};
unsigned char i;
unsigned char T1cnt;
unsigned char current_key;

/*结构体变量声明*/
Key_t key1={KEY_IDLE,0,4};
Key_t key2={KEY_IDLE,0,5};
Key_t key3={KEY_IDLE,0,6};
Key_t key4={KEY_IDLE,0,7};

void main()
{
	/*初始化*/
	System_Init();
	Timer1_Init();
	
	while(1)
	{
		current_key=0;
		
		/*有按键按下时*/
		if(Key_Event!=0)
		{
			current_key=Key_Event;
			Key_Event=0;
		}
		
		/*根据读取键值进行对应操作*/
		if(current_key==4)
	    SEG_Num[1]++;
		else if(current_key==5)
		SEG_Num[1]--;
	}
}

/*数码管处理函数*/
void SEG_Process()
{
	if(++i>8)i=1;
	
	/*小数点处理*/
	if(SEG_Num[i]>='.')
	SEG_Show(i,SEG_Num[i]-'.',1);
	else
	SEG_Show(i,SEG_Num[i],0);
}

/*定时器1中断函数*/
void Timer1_Routine() interrupt 3
{
	/*每10ms进行一次全体独立按键扫描*/
	T1cnt++;
	if(T1cnt>=10)
	{
		T1cnt=0;
		Key_Scan_StateMachine(&key1,S4);
		Key_Scan_StateMachine(&key2,S5);
		Key_Scan_StateMachine(&key3,S6);
		Key_Scan_StateMachine(&key4,S7);
	}
	
	/*数码管扫描*/
	SEG_Process();
}