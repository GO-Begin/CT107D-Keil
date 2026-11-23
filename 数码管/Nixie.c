#include <STC15F2K60S2.H>

unsigned char code Nixie_num[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0x7F};

void Delay2ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}


void Nixie(unsigned char Column,unsigned char Num)
{
	P2=(P2&0x1F)|0xE0;
	P0=0xFF;
	
	P2=(P2&0x1F)|0xC0;
	P0=0x01<<(Column-1);
	
	P2=(P2&0x1F)|0xE0;
	P0=Nixie_num[Num];
	Delay2ms();
}