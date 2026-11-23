#include <STC15F2K60S2.H>
#include "LED.h"
#include "Init.h"

unsigned char LED_Buf[]={0,0,0,0,0,0,0,0};
unsigned char RBM_Buf[]={0,0,0};

void main()
{
	System_Init();
	while(1)
	{
		LED(LED_Buf);
		Relay_Beep_Motor(RBM_Buf);
	}
}