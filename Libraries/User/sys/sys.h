#ifndef __SYS_H

#define __SYS_H

// 标准库

#include "math.h"

#include "stdio.h"

#include "string.h"



#include "stm32f10x.h"     			//微控制器头文件

#include "../delay/delay.h" 			//内核SysTick



#include "stdarg.h" 				//keil提供的变参函数



#include "../time/time1.h"			// 定时器2头文件

#include "../time/time2.h"			// 定时器2头文件

#include "../adc/adc.h"				//ADC驱动

#include "../led/led.h"				//LED驱动

#include "../key/key.h"				//KEY驱动



#include "../relay/relay.h"       	//继电器驱动

#include "../illumination/illumination.h"//光照传感器驱动

#include "../flash/BridgeFlash.h" 	//Flash驱动

#include "../beep/bsp_beep.h" 		//蜂鸣器驱动

#include "../ds18b20/ds18b20.h"		//DS18B20温度传感器驱动

#include "../servo/servo.h"			//舵机驱动

#include "../my_usart/my_usart2.h" 	//USART串口驱动

#include "../mn316/mn316_drv.h" 	//MN316 NB-IoT驱动



#include "../oled/bsp_oled_iic.h" 	//OLED驱动

#include "../oled/oledFont.h"     	//字体库

#include "../menu/menu.h"         	//OLED显示驱动







/* 直接操作寄存器的IO函数 */

#define	digitalHi(p,i)		 {p->BSRR=i;}	 //设置为高电平		

#define digitalLo(p,i)		 {p->BRR=i;}	 //设置为低电平

#define digitalToggle(p,i)   {p->ODR ^=i;} 	 //电平状态翻转



#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 

#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 

#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 



//IO口地址映射

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 

#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 

#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C    



#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 

#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 

#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 

 

//IO口操作，只对单个IO口！



#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 

#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 



#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 

#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 



#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 

#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 



//传感器数据结构体

typedef struct

{

	uint8_t WtrTempVal;//水温

	uint8_t WtrLevelVal;//水位

    uint16_t LightVal;//光照强度

	uint16_t TurbVal;//浊度

	float PHVal;//PH值

}SENSOR;



//阈值数据结构体

typedef struct

{

	uint8_t WtrTempThreshold;//水输入值

	uint8_t WtrLevelThreshold;//水位阈值

    uint16_t LightThreshold;//输入强输入值

	uint16_t TurbThreshold;//浊度阈值

	uint8_t PHMax;//PH值MIN阈值

	uint8_t PHMin;//PH值MIN阈值

	uint8_t FeedHour;//投喂时

	uint8_t FeedMinute;//投喂秒

	uint8_t FeedSecond;//投喂秒

	uint8_t FeedTime;//投喂执行时间

	

	uint8_t OxHour;//输入时

	uint8_t OxMinute;//字体库斤拷

	uint8_t OxSecond;//字体库斤拷

	uint8_t OxTime;//增氧执行时间	

}THRESHOLD;



//系统标志位结构体

typedef struct

{

	uint8_t mqttflag;//MQTT标志位

    uint8_t Switch1;//继电器-加热标志位

	uint8_t Switch2;//继电器-排水标志位

	uint8_t Switch3;//继电器-照明标志位

	uint8_t Switch4;//继电器-排水标志位

	uint8_t Switch5;//舵机-投喂标志位

	uint8_t Switch6;//灯光标志位

	uint8_t Switch7;//水泵标志位

}SYSTEM;







extern THRESHOLD Threshold;//全局阈值结构体

extern SENSOR SensorData;//全局传感器数据结构体

extern SYSTEM System;//全局系统标志位结构体





void NVIC_Config(void);



#endif

