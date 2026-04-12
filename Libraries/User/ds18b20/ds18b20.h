#ifndef _DS18B20_H_
#define _DS18B20_H_


/************************** DS18B20 连接引脚定义********************************/
#define      DS18B20_Dout_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      DS18B20_Dout_GPIO_CLK                       RCC_APB2Periph_GPIOA

#define      DS18B20_Dout_GPIO_PORT                      GPIOA
#define      DS18B20_Dout_GPIO_PIN                       GPIO_Pin_4



/************************** DS18B20 函数宏定义********************************/
#define      DS18B20_Dout_0	  DS18B20_Dout_GPIO_PORT->BRR  = DS18B20_Dout_GPIO_PIN
#define      DS18B20_Dout_1	  DS18B20_Dout_GPIO_PORT->BSRR = DS18B20_Dout_GPIO_PIN

#define      DS18B20_Dout_IN()	 ((DS18B20_Dout_GPIO_PORT->IDR  & DS18B20_Dout_GPIO_PIN)?1:0)


void DS18B20_Init(void);
void read_ds18b20(float *res_temp) ;

#endif
