#include <STC15F2K60S2.H>
#include "INTRINS.h"

sbit SCL=P2^0;	//时钟线
sbit SDA=P2^1;	//数据线

#define DELAY_TIME	10

/*用于I2C延时，维持高/低电平*/
static void I2C_Delay(unsigned char n)
{
    do
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();		
    }
    while(n--);      	
}

/*I2C起始通信信号*/
void I2CStart(void)
{
    SDA = 1;
    SCL = 1;
	I2C_Delay(DELAY_TIME);
    SDA = 0;
	I2C_Delay(DELAY_TIME);
    SCL = 0;    
}

/*I2C结束通信信号*/
void I2CStop(void)
{
    SDA = 0;
    SCL = 1;
	I2C_Delay(DELAY_TIME);
    SDA = 1;
	I2C_Delay(DELAY_TIME);
}

/*I2C发送一个字节*/
void I2CSendByte(unsigned char byt)
{
    unsigned char i;
	
	/*1个字节＝8个bit，循环八次*/
    for(i=0; i<8; i++){
        SCL = 0;
		I2C_Delay(DELAY_TIME);
		
		/*从最高位开始依次发送*/
        if(byt & 0x80){
            SDA = 1;
        }
        else{
            SDA = 0;
        }
		I2C_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
		I2C_Delay(DELAY_TIME);
    }
	
    SCL = 0;  
}

/*I2C接收一个字节*/
unsigned char I2CReceiveByte(void)
{
	unsigned char da;
	unsigned char i;
	for(i=0;i<8;i++){   
		SCL = 1;
		I2C_Delay(DELAY_TIME);
		
		/*从da最低位依次填充*/
		da <<= 1;
		if(SDA) 
			da |= 0x01;
		SCL = 0;
		I2C_Delay(DELAY_TIME);
	}
	return da;    
}

/*等待从机应答*/
unsigned char I2CWaitAck(void)
{
	unsigned char Ackbit;
	
    SCL = 1;
	I2C_Delay(DELAY_TIME);
    Ackbit = SDA; 
    SCL = 0;
	I2C_Delay(DELAY_TIME);
	
	return Ackbit;
}

/*发送应答位*/
void I2CSendAck(unsigned char Ackbit)
{
    SCL = 0;
    SDA = Ackbit; 
	I2C_Delay(DELAY_TIME);
    SCL = 1;
	I2C_Delay(DELAY_TIME);
    SCL = 0; 
	SDA = 1;
	I2C_Delay(DELAY_TIME);
}

/*AT24C02写入函数*/
void E2PROM_Write(unsigned char* str,unsigned char Address,unsigned char len)
{
	/*选中AT24C02并发送存储地址*/
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(Address);
	I2CWaitAck();
	
	/*分别将len个值用数组存起来*/
	while(len--)
	{
		I2CSendByte(*str++);
		I2CWaitAck();
	}
	I2CStop();
}

/*AT24C02读取函数*/
void E2PROM_Read(unsigned char* str,unsigned char Address,unsigned char len)
{
	/*选中AT24C02并发送读取地址*/
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(Address);
	I2CWaitAck();
	
	/*进行读取*/
	I2CStart();
	I2CSendByte(0xA1);
	I2CWaitAck();
	
	/*len次读取，将储存的值读到str里*/
	while(len--)
	{
		*str++=I2CReceiveByte();
		if(len)
		I2CSendAck(0);
		else
		I2CSendAck(1);
	}
	I2CStop();
}