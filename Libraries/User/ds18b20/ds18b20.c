#include "sys.h"
uint8_t DS18B20_Reset(void) 
{
    // 拉低总线 480μs（复位脉冲）
    DS18B20_Dout_0;
    delay_us(480);

    // 释放总线（上拉电阻拉高）
    DS18B20_Dout_1;
    delay_us(60); // 等待 60μs

    // 检测存在脉冲（60-240μs 低电平）
    uint8_t response = DS18B20_Dout_IN();
    delay_us(200); // 等待剩余时间

    return (response == 0); // 返回 1 表示检测到设备
}

void DS18B20_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;  // 定义GPIO初始化结构体

    RCC_APB2PeriphClockCmd(DS18B20_Dout_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = DS18B20_Dout_GPIO_PIN;  // 选择引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // 高速模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // 推挽输出模式
    GPIO_Init(DS18B20_Dout_GPIO_PORT, &GPIO_InitStructure); // 应用配置

    DS18B20_Dout_1;  // 宏定义，等同于 GPIO_SetBits(DS18B20_Dout_GPIO_PORT, DS18B20_Dout_GPIO_PIN)
    delay_ms(100);    // 延时100ms，确保稳定

    if (DS18B20_Reset())  // 调用复位函数（返回1表示检测到设备）
    {
        DS18B20_Dout_1;   // 再次拉高总线（保持空闲状态）
    }
}

void DS18B20_WriteBit(uint8_t bit) 
{
    // 拉低总线至少 1μs
    DS18B20_Dout_0;
    delay_us(2);

    // 写 1：释放总线；写 0：保持低电平
    if (bit) 
    {
        DS18B20_Dout_1;
    }
    delay_us(60); // 保持至少 60μs
    DS18B20_Dout_1; // 释放总线
    delay_us(2);
}

void DS18B20_WriteByte(uint8_t byte) 
{
    for (uint8_t i = 0; i < 8; i++) 
    {
        DS18B20_WriteBit(byte & 0x01);
        byte >>= 1;
    }
}

uint8_t DS18B20_ReadBit(void) 
{
    uint8_t bit = 0;


    // 拉低总线至少 1μs
    DS18B20_Dout_0;
    delay_us(1);

    // 释放总线并切换为输入模式
    DS18B20_Dout_1; // 释放总线
    delay_us(20); // 等待 10μs 后采样

    // 读取总线状态
    bit = DS18B20_Dout_IN();
    delay_us(40); // 保持总计 60μs
    return bit;
}

uint8_t DS18B20_ReadByte(void) 
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) 
    {
        byte |= (DS18B20_ReadBit() << i);
    }
    return byte;
}

void read_ds18b20(float *res_temp)    
{
    uint8_t lsb;
    uint8_t msb;
    unsigned short temp = 0;
    //static unsigned int start_cvt = 0;

    static unsigned char step = 0;
    if (step == 0)
    {
        if (DS18B20_Reset())
        {
            DS18B20_WriteByte(0xCC); // 跳过 ROM
            DS18B20_WriteByte(0x44); // 启动转换
            step = 1;
            //start_cvt = HAL_GetTick();
        }
    }
    else if (step == 1)
    {
        //if (HAL_GetTick() - start_cvt > 750)
        {
            if (DS18B20_Reset())
            {
                DS18B20_WriteByte(0xCC);
                DS18B20_WriteByte(0xBE); // 读取数据

                lsb = DS18B20_ReadByte();
                msb = DS18B20_ReadByte();
                temp = ((msb << 8) | lsb);
                
                *res_temp = (temp>>4) + (temp & 0x0F) * 0.0625;
                step = 0;
            }
        }
    }

} 



