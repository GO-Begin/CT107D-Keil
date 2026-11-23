#include <STC15F2K60S2.H>

/*键值变量*/
volatile unsigned char Key_Num;

/*返回键值*/
unsigned char MatrixKey()
{
	unsigned char Temp;
	EA=0;
	Temp=Key_Num;
	EA=1;
	Key_Num=0;
	return Temp;
}

/*得到当前按键的状态并返回*/
unsigned char Key_GetState()
{
	/*若无按键按下，则返回0*/
	unsigned char Key_State=0;
	
	P44=0;P42=1;P35=1;P34=1;
	if(P30==0)Key_State=7;
	if(P31==0)Key_State=6;
	if(P32==0)Key_State=5;
	if(P33==0)Key_State=4;
	
	P44=1;P42=0;P35=1;P34=1;
	if(P30==0)Key_State=11;
	if(P31==0)Key_State=10;
	if(P32==0)Key_State=9;
	if(P33==0)Key_State=8;
	
	P44=1;P42=1;P35=0;P34=1;
	if(P30==0)Key_State=15;
	if(P31==0)Key_State=14;
	if(P32==0)Key_State=13;
	if(P33==0)Key_State=12;
	
	P44=1;P42=1;P35=1;P34=0;
	if(P30==0)Key_State=19;
	if(P31==0)Key_State=18;
	if(P32==0)Key_State=17;
	if(P33==0)Key_State=16;
	
	return Key_State;
}

/*用于定时器1中断扫描键值*/
void Key_Loop()
{
	static unsigned char Key_New,Key_Last;
	Key_Last=Key_New;
	Key_New=Key_GetState();
	if(Key_New!=0&&Key_Last==0)
	Key_Num=Key_New;
}