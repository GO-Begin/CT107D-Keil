#include <STC15F2K60S2.H>
#include "Init.h"
#include "SEG.h"
#include "Timer1.h"
#include "UltraSound.h"
#include "Delay.h"

/*变量声明区*/
unsigned char SEG_Num[]={'#',10,10,10,10,10,10,10,10};
unsigned char i,Distence;

void main()
{
	/*初始化*/
	System_Init();
	Timer1_Init();
	
	while(1)
	{
		/*每60ms读取一次距离*/
		Distence=UltraSound_Read();
		Delay(60);
	}
}

/*数码管处理区*/
void SEG_Process()
{
	if(++i>=9)i=1;
	
	/*显示更新区*/
	SEG_Num[1]=Distence/100;
	SEG_Num[2]=Distence/10%10;
	SEG_Num[3]=Distence%10;
	
	/*显示处理区*/
	if(SEG_Num[i]>='.')
	SEG_Show(i,SEG_Num[i]-'.',1);
	else
	SEG_Show(i,SEG_Num[i],0);
}

/*每1ms进一次中断*/
void Timer1_Routine() interrupt 3
{
	SEG_Process();
}