#include <STC15F2K60S2.H>
#include "OneWire.h"

/*声明温度二进制的高八位与低八位*/
unsigned char High,Low;

/*读取DS18B20的温度数据*/
float Temperature_Read()
{
	/*DS18B20开始监测温度*/
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	
	/*读取DS18B20的数据，存入High与Low*/
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);

	Low=Read_DS18B20();
	High=Read_DS18B20();
	
	/*返回DS18B20的数据，并转化成真实温度*/
	return (High<<8|Low)*0.0625f;
}