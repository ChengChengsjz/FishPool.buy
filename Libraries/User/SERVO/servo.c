#include "sys.h"

void SG90_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;  // 定义GPIO初始化结构体
    
    // 使能舵机控制引脚的GPIO时钟（APB2总线）
    RCC_APB2PeriphClockCmd(SG90_GPIO_CLK, ENABLE);
    
    // 配置GPIO参数
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出模式
    GPIO_InitStruct.GPIO_Pin = SG90_GPIO_PIN;       // 选择控制引脚
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 高速输出（50MHz）
    
    // 初始化GPIO
    GPIO_Init(SG90_GPIO_PORT, &GPIO_InitStruct);
}

/*传入参数对应转动多少角度   500us对应0度
	500us,0°
	1000us,45°
	1500us,90°
	2000us,135°
	2500us,180°				  
*/
void Control_SG90(uint32_t us)
{
	int i=0;
	for(i=0;i<10;i++)
	{
		if(us<=20000)
		{
			PBout(5)=1;
			delay_us(us);
			PBout(5)=0;
			delay_us(20000-us);
		}
	}
}
