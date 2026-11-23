#include <STC15F2K60S2.H>
#include "Timer1.h"
#include "Timer0.h"
#include "Init.h"
#include "SEG.h"

unsigned char SEG_Num[]={'#',10,10,10,10,10,10,10,10};
unsigned char i;
unsigned char Time_200ms;
unsigned int freq;

void main()
{
	System_Init();
	Timer0_Init();
	Timer1_Init();
	while(1)
	{
		SEG_Num[1]=freq/10000;
		SEG_Num[2]=freq/1000%10;
		SEG_Num[3]=freq/100%10;
		SEG_Num[4]=freq/10%10;
		SEG_Num[5]=freq%10;
	}
}

void SEG_Process()
{
	if(++i>8)i=1;
	
	if(SEG_Num[i]>='.')
	SEG_Show(i,SEG_Num[i]-'.',1);
	else
	SEG_Show(i,SEG_Num[i],0);
}

void Timer1_Routine() interrupt 3
{
	SEG_Process();
	Time_200ms++;
	if(Time_200ms>=200)
	{
		Time_200ms=0;
		freq=TH0<<8|TL0;
		freq*=5;
		TH0=TL0=0;
	}
}