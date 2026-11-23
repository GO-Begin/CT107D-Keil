#include <STC15F2K60S2.H>
#include "Nixie.h"

void main()
{
	while(1)
	{
		Nixie(1,5);
		Nixie(2,2);
		Nixie(3,0);
		Nixie(4,1);
		Nixie(5,3);
		Nixie(6,1);
		Nixie(7,4);
		Nixie(8,11);
	}
}