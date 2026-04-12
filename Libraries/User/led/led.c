#include "sys.h"
void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    // 使能LED所在GPIO端口的时钟
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

	// 配置GPIO初始化参数
	GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN1;      // 选择要控制的GPIO引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  // 设置为推挽输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 设置IO口输出速度为50MHz

	// 根据设定参数初始化GPIO
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}
