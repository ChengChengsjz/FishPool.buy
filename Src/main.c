#include "sys.h"

SENSOR SensorData;//传感器数据结构定义
THRESHOLD Threshold;//阈值结构定义
SYSTEM System;//系统标志位结构定义


float Watertemp;
int adc_vel;
float  tempData = 0;
float PH_DAT;
#define PH_READ_TIMES 10//PH读取次数
int main(void)
{
    delay_init();		//延时初始化
    BdgFlashInit();		//Flash初始化，阈值数据初始化
    NVIC_Config();		//中断优先级配置
    KEY_Init();			//按键初始化
    Beep_Init();		//蜂鸣器初始化
    oled_Init();		//OLED初始化
    Adc_Init();			//ADC初始化
    LED_GPIO_Config();	//LED GPIO配置
    RELAY_GPIO_Config();//继电器GPIO配置
    DS18B20_Init();		//DS18B20温度传感器初始化
    SG90_Init();		//SG90舵机初始化
    My_USART2();		//USART2初始化，用于ESP8266通信，波特率115200
    WIFI_RESET_init();	//ESP8266复位引脚初始化
    Ali_MsessageInit();	//阿里云消息初始化
    TIM1_Int_Init(9999, 7199); //定时器1初始化，定时模式
    TIM2_Init(499, 7199);//定时器2初始化，定时扫描按键
    oled_Clear();		//OLED清屏

    /*******************************************/
    while (1) {
        ESP8266_run_handle();//处理MQTT连接与通信
        //采集水温
        read_ds18b20(&Watertemp);
        SensorData.WtrTempVal = (uint8_t)Watertemp;
        SensorData.WtrLevelVal = Get_Adc_Average(5, 5) * 50 / 4096; //读取水位
        SensorData.LightVal = (int)R_to_Lux(Get_Adc_Average(0, 5)); //读取光照强度
        //读取PH值
        SensorData.PHVal = (float)Get_Adc_Average(7, 10) / 4096 * 3.3; //读取PH
        SensorData.PHVal = -5.8887 * SensorData.PHVal + 21.677;

        if (SensorData.PHVal <= 0.0) {
            SensorData.PHVal = 0.0;
        }

        if (SensorData.PHVal >= 14.0) {
            SensorData.PHVal = 14.0;
        }

        //采集浊度
        adc_vel = Get_Adc_Average(6, 5);

        if (adc_vel > 2854) {
            SensorData.TurbVal = 100 - (adc_vel  - 2855) * 100 / 1241; //浊度转换公式
        } else {
            SensorData.TurbVal = 100;
        }

        SensorData.TurbVal *= 12;

        if (SensorData.TurbVal == 12) {
            SensorData.TurbVal = 0;
        }

        Mode_selection();//模式选择（按键1选择模式）
        save_Threshold();//保存阈值，如有改变则写入Flash存储
    }
}


