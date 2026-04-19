#include "sys.h"



/* USART2全局接收缓冲区(锟斤拷录锟斤拷前锟斤拷锟斤拷锟斤拷锟捷筹拷锟斤拷) */

u16 USART2_RxCounter = 0;



/* USART2接收缓冲区(1024锟街节伙拷锟轿伙拷锟斤拷锟斤拷) */

char USART2_RxBuff[1024];



/* 函数功能: USART2初始化配置

 * 硬件连接:

 *   TX - PA2(锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟)

 *   RX - PA3(锟斤拷锟斤拷锟斤拷锟斤拷)

 * 通信参数:

 *   - 波特率: 115200bps

 *   - 数据位: 8位

 *   - 停止位: 1位

 *   - 校验位: 锟斤拷

 *   - 停止位  : 锟斤拷

 * 锟斤拷波特率:

 *   - 使能接收中断(USART_IT_RXNE)

 *   - 使用APB1总线时钟(36MHz锟斤拷锟斤拷值) */

void My_USART2(void)

{

    GPIO_InitTypeDef  GPIO_InitStrue;  // GPIO初始化结构体

    USART_InitTypeDef USART2_InitStrue; // USART初始化结构体



    /* 时钟使能配置 */

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 使能GPIOA时钟

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // 使能USART2时钟



    /* TX锟斤拷锟斤拷(PA2)锟斤拷锟斤拷 */

    GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;      // 复用推挽输出模式

    GPIO_InitStrue.GPIO_Pin = USART2_GPIO_PIN_TX;    // PA2锟斤拷锟斤拷(锟斤拷甓锟斤拷)

    GPIO_InitStrue.GPIO_Speed = GPIO_Speed_10MHz;    // 输出速度10MHz

    GPIO_Init(GPIOA, &GPIO_InitStrue);               // 应用TX引脚配置



    /* RX锟斤拷锟斤拷(PA3)锟斤拷锟斤拷 */

    GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入模式

    GPIO_InitStrue.GPIO_Pin = USART2_GPIO_PIN_RX;    // PA3锟斤拷锟斤拷(锟斤拷甓锟斤拷)

    GPIO_Init(GPIOA, &GPIO_InitStrue);               // 应用RX引脚配置



    /* USART参数配置 */

    USART2_InitStrue.USART_BaudRate = 9600;          // 波特率9600（MN316模块）

    USART2_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控

    USART2_InitStrue.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 使能收发模式

    USART2_InitStrue.USART_Parity = USART_Parity_No; // 无校验位

    USART2_InitStrue.USART_StopBits = USART_StopBits_1; // 1位停止位

    USART2_InitStrue.USART_WordLength = USART_WordLength_8b; // 8位数据长度



    USART_Init(USART2, &USART2_InitStrue);           // 应用USART配置



    /* 中断使能配置 */

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);   // 使能接收中断(锟斤拷锟斤拷锟斤拷锟捷寄达拷锟斤拷锟角匡拷锟叫讹拷)

    USART_Cmd(USART2, ENABLE);                       // 使能USART2模块



}

void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data)

{

    /* Check the parameters */

    assert_param(IS_USART_ALL_PERIPH(USARTx));

    assert_param(IS_USART_DATA(Data));



    /* Transmit Data */

    USARTx->DR = (Data & (uint16_t)0x01FF);



    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);



}



/* 锟斤拷波特率: USART2锟斤拷式锟斤拷锟斤拷锟斤拷锟斤拷锟(锟斤拷锟斤拷式锟斤拷锟斤拷)

 * 参数说明:

 *   fmt : 锟斤拷式锟斤拷锟街凤拷锟斤拷(支锟斤拷%d,%f锟饺憋拷准锟斤拷式锟斤拷)

 *   ... : 锟缴憋拷锟斤拷锟斤拷斜锟(锟斤拷fmt锟叫的革拷式锟斤拷匹锟斤拷)

 * 实波特率:

 *   1. 使用va_list处理可变参数

 *   2. 通过vsprintf将格式化字符串输出到256字节临时缓冲区

 *   3. 查询方式逐个字符发送

 *   4. 锟饺达拷锟斤拷锟揭伙拷锟斤拷纸诜锟斤拷锟斤拷锟斤拷(TC锟斤拷志)

 *     */

__attribute__((aligned(8))) char USART2_TxBuff[256];  // 鍙戦佺紦鍐插尯(8瀛楄妭瀵归綈)



void u2_printf(char* fmt, ...)

{

    unsigned int i = 0, length = 0;



    /* 可变参数初始化 */

    va_list ap;

    va_start(ap, fmt);

    vsprintf(USART2_TxBuff, fmt, ap); // 锟斤拷式锟斤拷锟街凤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷

    va_end(ap);



    /* 获取有效数据长度 */

    length = strlen((const char*)USART2_TxBuff);



    /* 查询方式发送数据 */

    while(i < length)

    {

        USART_SendByte(USART2, USART2_TxBuff[i]); // 发送单个字节

        i++;

    }



    /* 锟饺达拷锟斤拷锟斤拷锟斤拷锟(锟斤拷锟絋C锟斤拷志锟斤拷锟斤拷TXE) */

    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

}

//void USART2_IRQHandler(void)

//{



//	if(Connect_flag == 0)

//	{

//		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)

//		{

//

//			if(USART2->DR)

//			{

//				USART2_RxBuff[USART2_RxCounter]=USART_ReceiveData(USART2);

//				USART2_RxCounter++;

//			}

//

//		}

//	}

//	else

//	{

//		USART2_RxBuff[USART2_RxCounter] =USART_ReceiveData(USART2);

//		if(USART2_RxCounter == 0)

//		{

//			TIM_Cmd(TIM4,ENABLE);

//		}else

//		{

//			TIM_SetCounter(TIM4,0);

//		}

//		USART2_RxCounter++;

//	}

//};





/* 锟斤拷波特率: USART2锟斤拷锟斤拷式锟斤拷锟捷帮拷锟斤拷锟斤拷

 * 参数说明:

 *   data - 数据包指针，结构如下

 *          [0]   : 锟斤拷锟饺革拷锟街斤拷

 *          [1]   : 锟斤拷锟饺碉拷锟街斤拷(锟杰筹拷锟斤拷= data[0]<<8 | data[1])

 *          [2~n+1]: 实锟斤拷锟斤拷锟斤拷(n=锟斤拷锟捷筹拷锟斤拷)

 * 执波特率:

 *   1. 锟饺达拷锟斤拷一锟轿达拷锟斤拷锟斤拷锟(TC锟斤拷志锟斤拷位)

 *   2. 从数据包前两个字节获取数据长度

 *   3. 根据长度值循环发送数据字节

 *   4. 每个字节发送后等待传输完成



 *    */

void u2_TxData(unsigned char *data)

{

    int	i;



    /* 等待历史数据发送完成 */

    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);



    /* 循环发送所有数据 */

    for(i = 1; i <= (data[0] * 256 + data[1]); i ++) // 锟斤拷锟斤拷= data[0]<<8 | data[1]

    {

        USART_SendData(USART2, data[i + 1]); // 锟斤拷锟斤拷data[2]~data[length+1]



        /* 等待每个字节发送完成 */

        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

    }

}

