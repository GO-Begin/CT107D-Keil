#ifndef __I2C_H__
#define __I2C_H__

void E2PROM_Write(unsigned char* str,unsigned char Address,unsigned char len);
void E2PROM_Read(unsigned char* str,unsigned char Address,unsigned char len);

#endif
