void Delay(unsigned int x)		//@12.000MHz
{
	unsigned char i, j;
	while(x--)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}
