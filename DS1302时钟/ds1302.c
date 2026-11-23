#include <STC15F2K60S2.H>
#include "INTRINS.h"

/*SCK为时钟线，SDA为数据线，RST为开始写入的标志*/
sbit SCK=P1^7;
sbit SDA=P2^3;
sbit RST=P1^3;

/*写入的基础操作，用于后续封装*/
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

/*对某个地址写入*/
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

/*读取某个地址的值*/
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

/*设置时钟初始值，参数为十进制，写入时要转为十六进制*/
void RTC_Set(unsigned char *RTC_Buf)
{
	Write_Ds1302_Byte(0x8E,0x00);
	Write_Ds1302_Byte(0x80,RTC_Buf[2]/10*16+RTC_Buf[2]%10);//提取十进制的十位和个位，十位*16+个位得到对应十六进制数
	Write_Ds1302_Byte(0x82,RTC_Buf[1]/10*16+RTC_Buf[1]%10);
	Write_Ds1302_Byte(0x84,RTC_Buf[0]/10*16+RTC_Buf[0]%10);
}

/*读取时钟现在的值，读取为十六进制，要转为十进制*/
void RTC_Read(unsigned char *RTC_Buf)
{
	RTC_Buf[2]=Read_Ds1302_Byte(0x81)/16*10+Read_Ds1302_Byte(0x81)%16;//提取十六进制的十位和个位，十位*10+个位得到对应十进制
	RTC_Buf[1]=Read_Ds1302_Byte(0x83)/16*10+Read_Ds1302_Byte(0x83)%16;
	RTC_Buf[0]=Read_Ds1302_Byte(0x85)/16*10+Read_Ds1302_Byte(0x85)%16;
}