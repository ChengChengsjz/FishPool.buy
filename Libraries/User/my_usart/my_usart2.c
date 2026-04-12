#include "sys.h"

/* USART2ȫ�ֽ��ռ�����(��¼��ǰ�������ݳ���) */
u16 USART2_RxCounter = 0;

/* USART2���ջ�����(1024�ֽڻ��λ�����) */
char USART2_RxBuff[1024];

/* ��������: USART2��ʼ������
 * Ӳ������:
 *   TX - PA2(�����������)
 *   RX - PA3(��������)
 * ͨ�Ų���:
 *   - ������: 115200bps
 *   - ����λ: 8λ
 *   - ֹͣλ: 1λ
 *   - У��λ: ��
 *   - ����  : ��
 * ��������:
 *   - ʹ�ܽ����ж�(USART_IT_RXNE)
 *   - ʹ��APB1����ʱ��(36MHz����ֵ) */
void My_USART2(void)
{
    GPIO_InitTypeDef  GPIO_InitStrue;  // GPIO���ýṹ��
    USART_InitTypeDef USART2_InitStrue; // USART���ýṹ��

    /* ʱ��ʹ������ */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // ʹ��USART2ʱ��

    /* TX����(PA2)���� */
    GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;      // �����������ģʽ
    GPIO_InitStrue.GPIO_Pin = USART2_GPIO_PIN_TX;    // PA2����(��궨��)
    GPIO_InitStrue.GPIO_Speed = GPIO_Speed_10MHz;    // ����ٶ�10MHz
    GPIO_Init(GPIOA, &GPIO_InitStrue);               // Ӧ��TX����

    /* RX����(PA3)���� */
    GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������ģʽ
    GPIO_InitStrue.GPIO_Pin = USART2_GPIO_PIN_RX;    // PA3����(��궨��)
    GPIO_Init(GPIOA, &GPIO_InitStrue);               // Ӧ��RX����

    /* USART�������� */
    USART2_InitStrue.USART_BaudRate = 115200;        // ������115200
    USART2_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������
    USART2_InitStrue.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // ʹ���շ�ģʽ
    USART2_InitStrue.USART_Parity = USART_Parity_No; // ��У��λ
    USART2_InitStrue.USART_StopBits = USART_StopBits_1; // 1λֹͣλ
    USART2_InitStrue.USART_WordLength = USART_WordLength_8b; // 8λ���ݳ���

    USART_Init(USART2, &USART2_InitStrue);           // Ӧ��USART����

    /* �ж���ʹ������ */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);   // ʹ�ܽ����ж�(�������ݼĴ����ǿ��ж�)
    USART_Cmd(USART2, ENABLE);                       // ʹ��USART2����

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

/* ��������: USART2��ʽ���������(����ʽ����)
 * ����˵��:
 *   fmt : ��ʽ���ַ���(֧��%d,%f�ȱ�׼��ʽ��)
 *   ... : �ɱ�����б�(��fmt�еĸ�ʽ��ƥ��)
 * ʵ������:
 *   1. ʹ��va_list�����ɱ����
 *   2. ͨ��vsprintf����ʽ���������256�ֽڷ��ͻ�����
 *   3. ��ѯ��ʽ����ֽڷ�������
 *   4. �ȴ����һ���ֽڷ������(TC��־)
 *     */
__attribute__((aligned(8))) char USART2_TxBuff[256];  // 发送缓冲区(8字节对齐)

void u2_printf(char* fmt, ...)
{
    unsigned int i = 0, length = 0;

    /* �ɱ�������� */
    va_list ap;
    va_start(ap, fmt);
    vsprintf(USART2_TxBuff, fmt, ap); // ��ʽ���ַ�����������
    va_end(ap);

    /* ������Ч���ݳ��� */
    length = strlen((const char*)USART2_TxBuff);

    /* ��ѯ�������� */
    while(i < length)
    {
        USART_SendByte(USART2, USART2_TxBuff[i]); // �������͵��ֽ�
        i++;
    }

    /* �ȴ��������(���TC��־����TXE) */
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


/* ��������: USART2����ʽ���ݰ�����
 * ����˵��:
 *   data - ���ݰ�ָ�룬�ṹΪ��
 *          [0]   : ���ȸ��ֽ�
 *          [1]   : ���ȵ��ֽ�(�ܳ���= data[0]<<8 | data[1])
 *          [2~n+1]: ʵ������(n=���ݳ���)
 * ִ������:
 *   1. �ȴ���һ�δ������(TC��־��λ)
 *   2. �������ݰ�ǰ���ֽڻ�ȡ���ݳ���
 *   3. ������ֵ�������ͺ����ֽ�
 *   4. ÿ���ֽڷ��ͺ�ȴ�Ӳ���������

 *    */
void u2_TxData(unsigned char *data)
{
    int	i;

    /* �ȴ���ʷ������� */
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

    /* �������Ȳ��������� */
    for(i = 1; i <= (data[0] * 256 + data[1]); i ++) // ����= data[0]<<8 | data[1]
    {
        USART_SendData(USART2, data[i + 1]); // ����data[2]~data[length+1]

        /* �����ȴ����ֽڷ������ */
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
}
