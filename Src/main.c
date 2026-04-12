#include "sys.h"

SENSOR SensorData;//�������ṹ�嶨��
THRESHOLD Threshold;//��ֵ�ṹ�嶨��
SYSTEM System;//ϵͳ��־λ�ṹ�嶨��


float Watertemp;
int adc_vel;
float  tempData = 0;
float PH_DAT;
#define PH_READ_TIMES 10//PH��ȡ����
int main(void)
{
    delay_init();		//��ʱ������ʼ��
    BdgFlashInit();		//flash��ʼ��������ֵ������ʼ��
    NVIC_Config();		//�ж����ȼ�����
    KEY_Init();			//������ʼ��
    Beep_Init();		//��������ʼ��
    oled_Init();		//oled��ʼ��
    Adc_Init();			//ADC��ʼ��
    LED_GPIO_Config();	//LED�Ƴ�ʼ��
    RELAY_GPIO_Config();//�̵�����ʼ��
    DS18B20_Init();		//DS18B20��ʼ��
    SG90_Init();		//�����ʼ��
    My_USART2();		//����2��ʼ������ESP8266ͨ���ã������ʣ�115200
    WIFI_RESET_init();	//ESP8266��λ���ų�ʼ��
    Ali_MsessageInit();	//������������صı�����ʼ��
    TIM1_Int_Init(9999, 7199); //��ʱ��1��ʼ��,���ڶ�ʱģʽ
    TIM2_Init(499, 7199);//��ʱ��2��ʼ������ʱɨ�谴��
    oled_Clear();		//����

    /*******************************************/
    while (1) {
        ESP8266_run_handle();//����MQTT����������֮ͨ��
        //�ɼ�ˮ��
        read_ds18b20(&Watertemp);
        SensorData.WtrTempVal = (uint8_t)Watertemp;
        SensorData.WtrLevelVal = Get_Adc_Average(5, 5) * 50 / 4096; //��ȡˮλ
        SensorData.LightVal = (int)R_to_Lux(Get_Adc_Average(0, 5)); // ��ȡ����ǿ��
        //��ȡPHֵ
        SensorData.PHVal = (float)Get_Adc_Average(7, 10) / 4096 * 3.3; //��ȡPH
        SensorData.PHVal = -5.8887 * SensorData.PHVal + 21.677;

        if (SensorData.PHVal <= 0.0) {
            SensorData.PHVal = 0.0;
        }

        if (SensorData.PHVal >= 14.0) {
            SensorData.PHVal = 14.0;
        }

        //�ɼ��Ƕ�
        adc_vel = Get_Adc_Average(6, 5);

        if (adc_vel > 2854) {
            SensorData.TurbVal = 100 - (adc_vel  - 2855) * 100 / 1241; //�Ƕ�ת����ʽ
        } else {
            SensorData.TurbVal = 100;
        }

        SensorData.TurbVal *= 12;

        if (SensorData.TurbVal == 12) {
            SensorData.TurbVal = 0;
        }

        Mode_selection();//ģʽѡ�񣨰���1ѡ��ģʽ��
        save_Threshold();// ����ֵ�����иı��Ǵ���flash����
    }
}


