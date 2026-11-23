#include <STC15F2K60S2.H>
#include "Timer1.h"
#include "SEG.h"
#include "DS1302.h"
#include "Init.h"

/*变量声明区*/
unsigned char SEG_Num[]={'#',10,10,10,10,10,10,10,10};
unsigned char i;
unsigned char RTC_Set_Buf[3]={12,00,00};
unsigned char Hour,Min,Sec;

void main()
{
	/*初始化*/
	System_Init();
	Timer1_Init();
	RTC_Set(RTC_Set_Buf);
	
	while(1)
	{
		/*刷新数码管的数据，使数码管实时显示DS1302时钟的值*/
		SEG_Num[1]=RTC_Set_Buf[0]/10;//时的十位
		SEG_Num[2]=RTC_Set_Buf[0]%10;//时的个位
		SEG_Num[3]=11;				 //显示'-'，装饰作用
		SEG_Num[4]=RTC_Set_Buf[1]/10;//分的十位
		SEG_Num[5]=RTC_Set_Buf[1]%10;//分的个位
		SEG_Num[6]=11;				 //显示'-'，装饰作用
		SEG_Num[7]=RTC_Set_Buf[2]/10;//秒的十位
		SEG_Num[8]=RTC_Set_Buf[2]%10;//秒的个位
	}
}

/*数码管显示处理区*/
void SEG_Process()
{
	/*依次显示八个数码管的值*/
	i++;
	if(i>8)i=1;
	
	RTC_Read(RTC_Set_Buf);//读取DS1302时钟的值，用RTC_Set_Buf接收
	
	/*小数点处理区*/
	if(SEG_Num>='.')
	SEG_Show(i,SEG_Num[i]-'.',1);
	else
	SEG_Show(i,SEG_Num[i],0);		
}

void Timer1_Routine() interrupt 3
{
	SEG_Process();
}