#ifndef _MN316_DRV_H_
#define _MN316_DRV_H_

#include "sys.h"

/* 一字串长度限制 */
#define MN316_JSON_MAX_LEN  256
#define MN316_RX_BUFF_SIZE  256

/* 连接参数（中国移动OneNet平台） */
#define MN316_MQTT_SERVER   "mqtts.heclouds.com"
#define MN316_MQTT_PORT     1883
#define MN316_CLIENT_ID     "MN316"
#define MN316_KEEP_ALIVE    120
#define MN316_USERNAME      "382J6WcAh6"
#define MN316_PASSWORD      "version=2018-10-31&res=products%2F382J6WcAh6%2Fdevices%2FMN316&et=1956502861&method=md5&sign=4JTji5nRHPuzIoCznghHZQ%3D%3D"
#define MN316_SUB_TOPIC     "$sys/382J6WcAh6/MN316/thing/property/post/reply"
#define MN316_PUB_TOPIC     "$sys/382J6WcAh6/MN316/thing/property/post"

/* MN316网络连接状态 */
typedef enum
{
    MN316_STATE_NULL = 0,
    MN316_STATE_AT_CHECK,
    MN316_STATE_MQTT_CFG,
    MN316_STATE_MQTT_OPEN,
    MN316_STATE_MQTT_SUB,
    MN316_STATE_READY
} MN316_NET_STATE;

/* MN316模块状态 */
typedef struct
{
    unsigned char Net_stu;              /* 网络连接状态 */
    unsigned char Cmd_stu;              /* 命令状态 */
    unsigned int cmd_send_trick;        /* 指令发送时间戳 */
    unsigned char retry_cnt;            /* 重试计数 */
    unsigned short rcv_idle_cnt;        /* 接收空闲计数(秒) */
    unsigned char offline_cnt;          /* 离线次数 */
    unsigned char MQTT_Connect_flag;    /* MQTT连接标志 */
    unsigned int rcv_trick_ms;          /* 最后接收时间戳 */
} MN316_STR;

/* 串口接收缓冲 */
typedef struct
{
    unsigned int rcv_trick_ms;
    unsigned char buff[MN316_RX_BUFF_SIZE];
    unsigned int rcv_cnt;
} MN316_RCV_STR;

/* AT指令结构 */
typedef struct
{
    char cmd[256];
    char respond[20];
    unsigned short wait_time;
    unsigned short retry_cnt;
} MN316_CMD_TYPE;

extern MN316_STR strMN316_info;
extern MN316_RCV_STR strMN316_rcv;

unsigned int get_sys_tick(void);
void MN316_init(void);
void MN316_run_handle(void);
void USARTx_RCVHandler(USART_TypeDef *USARTX);
void USART2_IRQHandler(void);
void USART_rcv_ch(unsigned char ch, MN316_RCV_STR *str_rcv);
void send_data_to_dev(char *data, unsigned short len);
void MN316_publish_sensor_data(void);

#endif
