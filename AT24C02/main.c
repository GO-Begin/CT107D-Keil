#include <STC15F2K60S2.H>
#include "SEG.h"
#include "Delay.h"
#include "Timer1.h"
#include "I2C.h"
#include "Init.h"

/*变量声明区*/
unsigned char SEG_Num[]={'#',10,10,10,10,10,10,10,10};
unsigned char i;

/*将需要展示的值存下去，数码管扫描新的数组*/
unsigned char Buf_Old[]={5,2,0,1,3,1,4,10};
unsigned char Buf_New[8]={0};

void main()
{
	/*初始化*/
	Timer1_Init();
	System_Init();
	
	/*数组储存与读取*/
	EA=0;
	E2PROM_Write(Buf_Old,0x00,8);
	EA=1;
	Delay(5);
	EA=0;
	E2PROM_Read(Buf_New,0x00,8);
	EA=1;
	
	while(1)
	{	
		/*刷新数码管*/
		SEG_Num[1]=Buf_New[0];
		SEG_Num[2]=Buf_New[1];
		SEG_Num[3]=Buf_New[2];
		SEG_Num[4]=Buf_New[3];
		SEG_Num[5]=Buf_New[4];
		SEG_Num[6]=Buf_New[5];
		SEG_Num[7]=Buf_New[6];
		SEG_Num[8]=Buf_New[7];
	}
}

/*数码管处理函数，用于定时器扫描*/
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
	SEG_Process();
}