#ifndef __Key_H__
#define __Key_H__

/*声明外部变量，表示按键事件*/
extern unsigned char Key_Event;

/*按键状态枚举*/
typedef enum{
	KEY_IDLE=0,				//按键未按下状态
	KEY_DEBOUNCE_DOWN,		//按键按下抖动状态
	KEY_PRESSED,			//引脚电平稳定状态
	KEY_DEBOUNCE_UP			//松开抖动状态
}KeyState_t;				//枚举关键词

/*按键结构体*/
typedef struct{
	KeyState_t state;		//按键状态枚举
	unsigned char count;	//按键计数器，用于确定是否连续出现高/低电平
	unsigned char key_val;	//按键键值
}Key_t;						//结构体关键词

/*按键扫描函数声明*/
void Key_Scan_StateMachine(Key_t *key,unsigned char pin_level);

#endif