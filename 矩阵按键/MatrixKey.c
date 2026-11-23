#include <STC15F2K60S2.H>

/*定义不稳定变量KeyNum，用于定时器检测键值*/
volatile unsigned char KeyNum;

/*实时读取键值，用于主函数调用返回值，判断是哪个按键按下*/
unsigned char MatrixKey()
{
	unsigned char Temp;
	EA=0;			//关闭中断，避免影响键值读取
	Temp=KeyNum;
	KeyNum=0;
	EA=1;			//再次打开中断，继续检测键值
	return Temp;
}

/*用于实时检测哪个按键按下并将值返回，用于MatrixKey_loop检测当前按键状态*/
unsigned char MatrixKey_GetState()
{
	unsigned char Key_State=0;	//重置键值，若无按键按下，则返回0
	
	P34=1;P35=1;P42=1;P44=0;
	if(P33==0)Key_State=4;
	if(P32==0)Key_State=5;
	if(P31==0)Key_State=6;
	if(P30==0)Key_State=7;
	
	P34=1;P35=1;P42=0;P44=1;
	if(P33==0)Key_State=8;
	if(P32==0)Key_State=9;
	if(P31==0)Key_State=10;
	if(P30==0)Key_State=11;
	
	P34=1;P35=0;P42=1;P44=1;
	if(P33==0)Key_State=12;
	if(P32==0)Key_State=13;
	if(P31==0)Key_State=14;
	if(P30==0)Key_State=15;
	
	P34=0;P35=1;P42=1;P44=1;
	if(P33==0)Key_State=16;
	if(P32==0)Key_State=17;
	if(P31==0)Key_State=18;
	if(P30==0)Key_State=19;
	
	return Key_State;
}

/*在定时器里定时调用，读取到下降沿则将KeyNum赋值*/
void MatrixKey_loop()
{
	static unsigned char Key_Now,Key_Last;
	Key_Last=Key_Now;			//Key_Last读取旧值
	Key_Now=MatrixKey_GetState();		//Key_Now得到最新的按键状态
	
	/*下降沿触发赋值*/
	if(Key_Now!=0&&Key_Last==0)
	KeyNum=Key_Now;
}