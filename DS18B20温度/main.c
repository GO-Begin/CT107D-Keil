#include <STC15F2K60S2.H>
#include "Init.h"
#include "SEG.h"
#include "Timer1.h"
#include "DS18B20.h"

/*变量声明区*/
unsigned char SEG_Num[]={'#',10,10,10,10,10,10,10,10};
unsigned char i,T1cnt;
unsigned int Temperaturex100;

void main()
{
	/*初始化*/
	System_Init();
	Timer1_Init();
	
	while(1)
	{
		/*读取DS18B20传感器温度值，只取前四位*/
		Temperaturex100=Temperature_Read()*100;
		SEG_Num[1]=Temperaturex100/1000%10;
		SEG_Num[2]=Temperaturex100/100%10+'.';//第二位加上小数点
		SEG_Num[3]=Temperaturex100/10%10;
		SEG_Num[4]=Temperaturex100%10;
	}
}

/*数码管显示，用于定时器调用*/
void SEG_Process()
{
	i++;
	if(i>=9)i=1;			//显示第一位到第九位的SEG_Num值
	
	if(SEG_Num[i]>='.')		//判断是否有小数点
	SEG_Show(i,SEG_Num[i]-'.',1);
	else
	SEG_Show(i,SEG_Num[i],0);
}

void Timer1_Routine() interrupt 3
{
	SEG_Process();			//数码管显示
}