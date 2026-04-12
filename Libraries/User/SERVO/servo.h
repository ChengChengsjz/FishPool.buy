#ifndef _SERVO_H_
#define _SERVO_H_

#include "sys.h"

#define SG90_GPIO_PIN   GPIO_Pin_5
#define SG90_GPIO_PORT  GPIOB
#define SG90_GPIO_CLK  RCC_APB2Periph_GPIOB

void SG90_Init(void);
void Control_SG90(uint32_t us);

#endif
