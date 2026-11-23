#include <STC15F2K60S2.H>
#include "SEG.h"
#include "Timer1.h"
#include "Init.h"
#include "iic.h"

/*变量声明区*/
unsigned char SEG_Num[]={'#',10,10,10,10,10,10,10,10};	//数码管段选
unsigned char i;										//配合SEG_Num[]，用于数码管位选与段选
unsigned int AD1_100x,AD3_100x,DA_100x;					//AD1_100x为PCF8591，0x41x模式下电压的一百倍，依次类推
unsigned char Flag_100ms,AD_Flag;						//用于定时器1控制每100ms进行一次主循环内的ADDA通信操作

void main()
{
	/*初始化*/
	Timer1_Init();
	System_Init();
	
	while(1)
	{
		/*每100ms进行一次PCF8591通信*/
		if(AD_Flag)
		{
			/*进行对应转化，将0-500的参数转化成0-255*/
			DA_Write(DA_100x*51/100.0f);
			
			/*进行对应转化，将0-255转化为0-5.0的范围*/
			AD1_100x=AD_Read(0x41)*100/51.0f;
			AD3_100x=AD_Read(0x43)*100/51.0f;
			
			/*标志位清零，用于下次通信*/
			AD_Flag=0;
		}
		
		/*AD1的值（光敏电阻）*/
		SEG_Num[1]=AD1_100x/100+'.';
		SEG_Num[2]=AD1_100x/10%10;
		SEG_Num[3]=AD1_100x%10;
		
		/*AD3的值（滑动变阻器）*/
		SEG_Num[6]=AD3_100x/100+'.';
		SEG_Num[7]=AD3_100x/10%10;
		SEG_Num[8]=AD3_100x%10;
	}
}

/*数码管处理函数，用于定时器1扫描*/
void SEG_Process()
{
	if(++i>8)i=1;
	
	/*小数点处理*/
	if(SEG_Num[i]>='.')
	SEG_Show(i,SEG_Num[i]-'.',1);
	else
	SEG_Show(i,SEG_Num[i],0);
}

/*定时器1中断*/
void Timer1_Routine() interrupt 3
{
	/*每100ms标志位置1*/
	Flag_100ms++;
	if(Flag_100ms>=100)
	{
		AD_Flag=1;
	}
	
	SEG_Process();
}