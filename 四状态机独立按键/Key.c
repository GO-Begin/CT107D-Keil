#include <STC15F2K60S2.H>
#include "Key.h"

/*按键事件（0表示无按键按下，非0表示有按键按下，主循环处理完后需清0，便于下次使用）*/
unsigned char Key_Event=0;

/*此函数用于扫描按键并得到对应键值，适合每10ms在中断中调用
*使用此函数需传入配置好的按键结构体的地址与按键对应引脚
*Key_t定义结构体指针变量*key，结构体包含按键状态state(枚举)，按键按下的次数count以及按键的键值key_val
*pin_level表示引脚电平
*Key_t结构体和KeyState_t枚举详情可见于Key.h
*/
void Key_Scan_StateMachine(Key_t *key,unsigned char pin_level)
{
	/*对枚举情况进行对应处理*/
	switch(key->state)
	{
		/*按键未按下状态*/
		case KEY_IDLE:
			/*引脚低电平，表示按键被按下*/
			if(pin_level==0)
			{
				/*进入按下抖动状态，计数器重新计数*/
				key->state=KEY_DEBOUNCE_DOWN;
				key->count=0;
			}
			break;
		
		/*按下抖动状态*/
		case KEY_DEBOUNCE_DOWN :
			/*采样到低电平*/
			if(pin_level==0)
			{
				key->count++;
				if(key->count>=2)
				{
					/*两次10ms采样都是低电平，按键抖动一般不超过20ms，则可认为对应按键引脚电平已稳定在低电平*/
					key->state=KEY_PRESSED;
					Key_Event=key->key_val;
				}
			}
			/*采样到高电平，还在抖动或未按下按键时误进入抖动状态，此时回到未按下状态重新采样*/
			else
			key->state=KEY_IDLE;
			break;
			
		/*按键已确认按下（等待松开）*/
		case KEY_PRESSED :
			/*出现高电平，即按键被松开*/
			if(pin_level==1)
			{
				/*进入松开抖动状态，计数器重新计数*/
				key->state=KEY_DEBOUNCE_UP;
				key->count=0;
			}
			break;
			
		/*松开抖动状态*/
		case KEY_DEBOUNCE_UP :
			/*采样到高电平*/
			if(pin_level==1)
			{
				key->count++;
				/*连续两次10ms采样到高电平，则按键抖动已结束，回到未按下状态*/
				if(key->count>=2)
				key->state=KEY_IDLE;
			}
			/*采样到低电平，还在抖动或未松开时误进入抖动状态，回到引脚电平稳定状态继续采样*/
			else
			key->state=KEY_PRESSED;
			break;
	}
}