#include <STC15F2K60S2.H>

void LED(unsigned char* led)
{
	unsigned char Temp;
	Temp=led[0]<<0|led[1]<<1|led[2]<<2|led[3]<<3|led[4]<<4|led[5]<<5|led[6]<<6|led[7]<<7;
	
	P0=~Temp;
	P2=P2&0x1F|0x80;
	P2&=0x1F;
}

void Relay_Beep_Motor(unsigned char* RBM_Buf)
{
	unsigned char Temp;
	Temp=RBM_Buf[0]<<4|RBM_Buf[1]<<6|RBM_Buf[2]<<5;
	
	P0=Temp;
	P2=P2&0x1F|0xA0;
	P2&=0x1F;
}