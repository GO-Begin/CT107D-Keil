#include <STC15F2K60S2.H>

/*数码管段码表*/
unsigned char code Nixie_Table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0x7F};

/*数码管主函数*/
void Nixie(unsigned char Column,unsigned char Num,unsigned char point)
{
	/*消影*/
	P2=(P2&0x1F)|0xE0;
	P0=0xFF;
	
	/*位选*/
	P2=(P2&0x1F)|0xC0;
	P0=0x01<<Column-1;
	
	/*段选并判断是否有小数点*/
	P2=(P2&0x1F)|0xE0;
	if(point)
		P0=Nixie_Table[Num]&0x7F;	//有小数点则将第一位置0，点亮小数点
	else
		P0=Nixie_Table[Num];
}