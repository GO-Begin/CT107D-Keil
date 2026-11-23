#include <STC15F2K60S2.H>

/*数码管对照表*/
unsigned char code SEG_Table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF};

/*数码管显示函数*/
void SEG_Show(unsigned char Column,unsigned char Num,unsigned char Point)
{
	/*消影*/
	P2=P2&0x1F|0xE0;
	P0=0xFF;
	
	/*位选*/
	P2=P2&0x1F|0xC0;
	P0=0x01<<Column-1;
	
	/*段选*/
	P2=P2&0x1F|0xE0;
	
	/*小数点处理*/
	if(Point)
	P0=SEG_Table[Num]&0x7F;
	else
	P0=SEG_Table[Num];
}