#include <STC15F2K60S2.H>
#include <INTRINS.h>

sbit US_TX=P1^0;
sbit US_RX=P1^1;

/*用于US_TX保持10us高电平*/
void Delay10us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}

/*超声波读取距离*/
unsigned char UltraSound_Read()
{
	/*初始化*/
	unsigned int Time=0;
	CMOD=0x00;
	CH=CL=0;
	CF=0;
	
	/*测距触发信号*/
	US_TX=1;
	Delay10us();
	US_TX=0;
	while(US_RX==0);
	
	/*测距，当PCA计时溢出或接受到回波停止计时*/
	CR=1;
	while(CF==0&&US_RX==1);
	CR=0;
	
	/*溢出处理*/
	if(CF==1)
	{
		CF=0;
		return 0;
	}
	
	/*收到回波处理*/
	else
	{
		Time=CH<<8|CL;
		return Time*0.017;
	}
}