#include <STC15F2K60S2.H>
#include "Nixie.h"
#include "Init.h"
#include "Timer1.h"

unsigned char Nixie_num[]={'#',10,10,10,10,10,10,10,10};
unsigned char i;

void main()
{
	Init();
	Timer1Init();
	while(1)
	{
		if(P33==0)
			Nixie_num[1]=1+'.';
	}
}

void Timer1_Routine() interrupt 3
{
	i++;
	if(i==9)i=1;
	if(Nixie_num[i]>='.')
	Nixie(i,Nixie_num[i]-'.',1);
	else
	Nixie(i,Nixie_num[i],0);	
}