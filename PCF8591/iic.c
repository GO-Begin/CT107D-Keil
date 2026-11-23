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

/*用于读取PCF8591的AD值*/
unsigned char AD_Read(unsigned char Address)
{
	unsigned char Temp;
	
	/*开启数模转换*/
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(Address);
	I2CWaitAck();
	
	/*开始读取AD值*/
	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	
	/*扔掉上一次转化的旧值，防止多通道读串了*/
	I2CReceiveByte();
	I2CSendAck(0);		//发送应答位0，继续读字节
	
	/*得到最新转化的值*/
	Temp=I2CReceiveByte();
	I2CSendAck(1);		//发送应答位1，结束读取
	I2CStop();			//结束通信
	
	return Temp;
}

/*写入PCF8591，输出DA值*/
void DA_Write(unsigned char Data)
{
	/*开启DA转化，将Data写入PCF8591，输出对应电压*/
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x41);
	I2CWaitAck();
	I2CSendByte(Data);
	I2CWaitAck();
	I2CStop();
}