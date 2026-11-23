#include <STC15F2K60S2.H>
#include "MatrixKey.h"
#include "Timer1.h"
#include "Init.h"
#include "Nixie.h"

unsigned char T1cnt,i;		//i当作Nixie_Num的选择位，T1cnt计时触发几次中断
unsigned char KeyNumber;	//KeyNumber当作Key()的返回值，即按下的键值

/*用于数码管显示8位数字，默认第一二位显示0，其余不显示*/
/*用#去除0位，确保数列位置与数码管位选一致*/
unsigned char Nixie_Num[]={'#',0,0,10,10,10,10,10,10}; 

void main()
{
	/*进行定时器1及一系列初始化*/
	Timer1_Init();
	System_Init();
	
	while(1)
	{
		KeyNumber=MatrixKey();	//KeyNumber实时读取Key()的返回键值
		if(KeyNumber)
		{
			Nixie_Num[1]=KeyNumber/10;
			Nixie_Num[2]=KeyNumber%10;
		}
	}
}

void Timer1_Routine() interrupt 3
{
	/*用于8位数码管显示*/
	i++;
	if(i>=9)
	i=1;		//跳过第一位'#'
	if(Nixie_Num[i]>='.')
		Nixie(i,Nixie_Num[i]-'.',1); //小数点处理
	else
		Nixie(i,Nixie_Num[i],0);
	
	/*设置调用Key_loop的频率*/
	T1cnt++;
	if(T1cnt>=20) 	//20毫秒调用一次MatrixKey_loop检测键值
	{
		T1cnt=0;	//T1计数器清零，用于下一次计数
		MatrixKey_loop();
	}
}