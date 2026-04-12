#ifndef __SYS_H
#define __SYS_H
// ��׼��
#include "math.h"
#include "stdio.h"
#include "string.h"

#include "stm32f10x.h"     			//�̼���ͷ�ļ�
#include "../delay/delay.h" 			//�ں�SysTick

#include "stdarg.h" 				//keil�ṩ������

#include "../time/time1.h"			// ��ʱ��2����
#include "../time/time2.h"			// ��ʱ��2����
#include "../adc/adc.h"				//adc����
#include "../led/led.h"				//LED����
#include "../key/key.h"				//KEY����

#include "../relay/relay.h"       	//RELAY����
#include "../illumination/illumination.h"//��������
#include "../flash/BridgeFlash.h" 	//Flash����
#include "../beep/bsp_beep.h" 		//����������
#include "../ds18b20/ds18b20.h"		//ds18b20����
#include "../servo/servo.h"			//�������
#include "../my_usart/my_usart2.h" 	//usart����
#include "../mqttwifi/esp8266_drv.h" //wifi����
#include "../mqttwifi/mqtt_drv.h" 	//mqtt����

#include "../oled/bsp_oled_iic.h" 	//oled����
#include "../oled/oledFont.h"     	//�����
#include "../menu/menu.h"         	//OLED��ʾ



/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i)   {p->ODR ^=i;} 	 //�����ת״̬

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

//���������ݽṹ��
typedef struct
{
	uint8_t WtrTempVal;//ˮ��
	uint8_t WtrLevelVal;//ˮλ
    uint16_t LightVal;//����ǿ��
	uint16_t TurbVal;//���Ƕ�
	float PHVal;//PHֵ
}SENSOR;

//��ֵ���ݽṹ��
typedef struct
{
	uint8_t WtrTempThreshold;//ˮ����ֵ
	uint8_t WtrLevelThreshold;//ˮλ��ֵ
    uint16_t LightThreshold;//����ǿ����ֵ
	uint16_t TurbThreshold;//�Ƕ���ֵ
	uint8_t PHMax;//PHֵMAX��ֵ
	uint8_t PHMin;//PHֵMAX��ֵ
	uint8_t FeedHour;//Ͷιʱ
	uint8_t FeedMinute;//Ͷι��
	uint8_t FeedSecond;//Ͷι��
	uint8_t FeedTime;//Ͷιִ��ʱ��
	
	uint8_t OxHour;//����ʱ
	uint8_t OxMinute;//������
	uint8_t OxSecond;//������
	uint8_t OxTime;//����ִ��ʱ��	
}THRESHOLD;

//ϵͳ��־λ�ṹ��
typedef struct
{
	uint8_t mqttflag;//MQTT��־λ
    uint8_t Switch1;//�̵���-���ȱ�־λ
	uint8_t Switch2;//�̵���-��ˮ��־λ
	uint8_t Switch3;//�̵���-������־λ
	uint8_t Switch4;//�̵���-��ˮ��־λ
	uint8_t Switch5;//���-Ͷι��־λ
	uint8_t Switch6;//�ƹ��־λ
	uint8_t Switch7;//������־λ
}SYSTEM;



extern THRESHOLD Threshold;//������ֵ�ṹ��
extern SENSOR SensorData;//���崫�����ṹ��
extern SYSTEM System;//����ϵͳ��־λ�ṹ��


void NVIC_Config(void);

#endif
