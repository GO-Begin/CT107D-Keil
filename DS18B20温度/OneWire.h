#ifndef __OneWire_H__
#define __OneWire_H__

void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit init_ds18b20(void);

#endif
