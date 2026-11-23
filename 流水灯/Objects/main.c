#include <STC15F2K60S2.H>
#include <INTRINS.H>

unsigned char i;

void Delay200ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 10;
	j = 31;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
	P2=(P2&0x1F)|0xA0;
	P0=0x00;
	P2=(P2&0x1F)|0x80;
	while(1)
	{
		for(i=0;i<8;i++)
		{
			P0=_crol_(0xFE,i);
			Delay200ms();
		}
		for(i=0;i<8;i++)
		{
			P0=_cror_(0x7F,i);
			Delay200ms();
		}
	}
}