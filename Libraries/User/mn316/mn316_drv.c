#include "sys.h"
#include "mn316_drv.h"

/* MN316ȫ״̬ */
MN316_STR strMN316_info;
MN316_RCV_STR strMN316_rcv;

/* ATָб */
static MN316_CMD_TYPE strMN316_cmd[5];

/* ڽ*/
static unsigned char is_new_CMD_rx = 0;
static unsigned char CMD_rx_buff[400];

/* ڷʱ */
static unsigned int last_pub_cnt = 0;

/************************************
send_data_to_dev
ܣͨUSART2ݵMN316ģ
data-зݵָ룻len-ݳ
ֵ޷ֵ
************************************/
void send_data_to_dev(char *data, unsigned short len)
{
    unsigned int i;
    for(i = 0; i < len; i++)
    {
        while(!(USART2->SR & USART_FLAG_TXE));
        USART2->DR = data[i];
        while(!(USART2->SR & USART_FLAG_TC));
    }
}

/************************************
USART_rcv_ch
ܣսյֽ뻺
ch-յֽڣstr_rcv-ջṹָ
ֵ޷ֵ
************************************/
void USART_rcv_ch(unsigned char ch, MN316_RCV_STR *str_rcv)
{
    if(str_rcv->rcv_cnt < MN316_RX_BUFF_SIZE)
    {
        str_rcv->buff[str_rcv->rcv_cnt] = ch;
        str_rcv->rcv_cnt++;
        str_rcv->rcv_trick_ms = get_sys_tick();
    }
}

/************************************
USARTx_RCVHandler
ܣͨUSARTжϴ
USARTX-ڱָ
ֵ޷ֵ
************************************/
void USARTx_RCVHandler(USART_TypeDef *USARTX)
{
    unsigned char ch;
    if((USARTX->SR & USART_FLAG_RXNE) || (USARTX->SR & USART_FLAG_ORE))
    {
        ch = USARTX->DR;
        USART_ClearFlag(USARTX, USART_FLAG_RXNE);
        if(USARTX == USART2)
        {
            USART_rcv_ch(ch, &strMN316_rcv);
        }
    }
}

/************************************
USART2_IRQHandler
ܣUSART2жϷ
ֵ޷ֵ
************************************/
void USART2_IRQHandler(void)
{
    USARTx_RCVHandler(USART2);
}

/************************************
write_cmd
ܣATָȴӦԵ
strCMD-ָָ
ֵ1-ɹ2-ٴľʧܣ0-
************************************/
static unsigned char write_cmd(MN316_CMD_TYPE *strCMD)
{
    char *pStr;
    if(strMN316_info.Cmd_stu == 0)
    {
        send_data_to_dev(strCMD->cmd, strlen(strCMD->cmd));
        strMN316_info.Cmd_stu = 1;
        strMN316_info.cmd_send_trick = get_sys_tick();
        memset(strMN316_rcv.buff, 0, MN316_RX_BUFF_SIZE);
        strMN316_rcv.rcv_cnt = 0;
    }
    else if(strMN316_info.Cmd_stu == 1)
    {
        if(get_sys_tick() - strMN316_info.cmd_send_trick < strCMD->wait_time)
        {
            if(strMN316_rcv.rcv_cnt >= 2)
            {
                pStr = strstr((char *)strMN316_rcv.buff, strCMD->respond);
                if(pStr)
                {
                    strMN316_info.Cmd_stu = 0;
                    strMN316_info.retry_cnt = 0;
                    return 1;
                }
            }
        }
        else
        {
            strMN316_info.Cmd_stu = 0;
            strMN316_info.retry_cnt++;
            if(strMN316_info.retry_cnt > strCMD->retry_cnt)
            {
                strMN316_info.retry_cnt = 0;
                return 2;
            }
        }
    }
    return 0;
}

/************************************
MN316_init
ܣʼATָб
ֵ޷ֵ
************************************/
void MN316_init(void)
{
    sprintf(strMN316_cmd[1].cmd,
            "AT+MQTTCFG=\"" MN316_MQTT_SERVER "\","
            "%d,"
            "\"" MN316_CLIENT_ID "\","
            "%d,"
            "\"" MN316_USERNAME "\","
            "\"" MN316_PASSWORD "\","
            "1,0\r\n",
            MN316_MQTT_PORT, MN316_KEEP_ALIVE);
    strcpy(strMN316_cmd[1].respond, "OK");
    strMN316_cmd[1].wait_time = 3000;
    strMN316_cmd[1].retry_cnt = 3;

    strcpy(strMN316_cmd[2].cmd, "AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");
    strcpy(strMN316_cmd[2].respond, "OK");
    strMN316_cmd[2].wait_time = 5000;
    strMN316_cmd[2].retry_cnt = 3;

    strcpy(strMN316_cmd[3].cmd, "AT+MQTTSUB=\"" MN316_SUB_TOPIC "\",0\r\n");
    strcpy(strMN316_cmd[3].respond, "OK");
    strMN316_cmd[3].wait_time = 3000;
    strMN316_cmd[3].retry_cnt = 3;

    strcpy(strMN316_cmd[0].cmd, "AT\r\n");
    strcpy(strMN316_cmd[0].respond, "OK");
    strMN316_cmd[0].wait_time = 2000;
    strMN316_cmd[0].retry_cnt = 3;
}

/************************************
ֽתΪHEXַ(д)
src-ԭʼݣlen-ݳȣdest-HEX
ֵHEXַĳ
************************************/
static unsigned short ascii_to_hex(const unsigned char *src, unsigned short len, char *dest)
{
    static const char hex_chars[] = "0123456789ABCDEF";
    unsigned short i;
    for(i = 0; i < len; i++)
    {
        dest[i * 2]     = hex_chars[(src[i] >> 4) & 0x0F];
        dest[i * 2 + 1] = hex_chars[src[i] & 0x0F];
    }
    return len * 2;
}

/************************************
MN316_publish_sensor_data
ܣJSONʽݵMQTT
ֵ޷ֵ
************************************/
void MN316_publish_sensor_data(void)
{
    char json[MN316_JSON_MAX_LEN];
    char hex_buf[MN316_JSON_MAX_LEN * 2];
    char at_cmd[512];
    unsigned short hex_len, json_len;

    /* 第一包：Temperature, Humidity, Lightness (3个参数) */
    json_len = sprintf(json,
                       "{\"id\":\"123\",\"version\":\"1.0\",\"params\":{\"Temperature\":{\"value\":%d},\"Humidity\":{\"value\":%d},\"Lightness\":{\"value\":%d}}}",
                       SensorData.WtrTempVal, SensorData.WtrLevelVal, SensorData.LightVal);
    hex_len = ascii_to_hex((const unsigned char *)json, json_len, hex_buf);
    sprintf(at_cmd,
            "AT+MQTTPUB=\"$sys/382J6WcAh6/MN316/thing/property/post\",0,0,0,%d,%s\r\n",
            json_len, hex_buf);
    send_data_to_dev(at_cmd, strlen(at_cmd));

    /* 第二包：Turbidity, pHValue (2个参数) */
    json_len = sprintf(json,
                       "{\"id\":\"123\",\"version\":\"1.0\",\"params\":{\"Turbidity\":{\"value\":%d},\"pHValue\":{\"value\":%.1f}}}",
                       SensorData.TurbVal, SensorData.PHVal);
    hex_len = ascii_to_hex((const unsigned char *)json, json_len, hex_buf);
    sprintf(at_cmd,
            "AT+MQTTPUB=\"$sys/382J6WcAh6/MN316/thing/property/post\",0,0,0,%d,%s\r\n",
            json_len, hex_buf);
    send_data_to_dev(at_cmd, strlen(at_cmd));
}

/************************************
ܣյATӦؼ
************************************/
static void process_rx_buffer(void)
{
    if(get_sys_tick() - strMN316_rcv.rcv_trick_ms >= 20 && strMN316_rcv.rcv_cnt)
    {
        strMN316_rcv.buff[strMN316_rcv.rcv_cnt] = '\0';

        if(strstr((char *)strMN316_rcv.buff, "Automatic"))
        {
            mode_selection = 0;
        }
        if(strstr((char *)strMN316_rcv.buff, "TimeFlag"))
        {
            mode_selection = 1;
        }
        if(strstr((char *)strMN316_rcv.buff, "ThresholdMODE"))
        {
            mode_selection = 2;
        }
        if(strstr((char *)strMN316_rcv.buff, "Manual"))
        {
            mode_selection = 3;
            System.Switch1 = System.Switch2 = System.Switch3 =
            System.Switch4 = System.Switch5 = System.Switch6 = System.Switch7 = 0;
        }
        if(strstr((char *)strMN316_rcv.buff, "Switch1ON"))  { mode_selection = 0; System.Switch1 = 1; }
        if(strstr((char *)strMN316_rcv.buff, "Switch1OFF")) { mode_selection = 0; System.Switch1 = 0; }
        if(strstr((char *)strMN316_rcv.buff, "Switch2ON"))  { mode_selection = 1; System.Switch2 = 1; }
        if(strstr((char *)strMN316_rcv.buff, "Switch2OFF")) { mode_selection = 1; System.Switch2 = 0; }
        if(strstr((char *)strMN316_rcv.buff, "Switch3ON"))  { mode_selection = 2; System.Switch3 = 1; }
        if(strstr((char *)strMN316_rcv.buff, "Switch3OFF")) { mode_selection = 2; System.Switch3 = 0; }
        if(strstr((char *)strMN316_rcv.buff, "Switch4ON"))  { mode_selection = 3; System.Switch4 = 1; }
        if(strstr((char *)strMN316_rcv.buff, "Switch4OFF")) { mode_selection = 3; System.Switch4 = 0; }
        if(strstr((char *)strMN316_rcv.buff, "Switch5ON"))  { mode_selection = 4; System.Switch5 = 1; }
        if(strstr((char *)strMN316_rcv.buff, "Switch5OFF")) { mode_selection = 4; System.Switch5 = 0; }
        if(strstr((char *)strMN316_rcv.buff, "Switch6ON"))  { mode_selection = 5; System.Switch6 = 1; }
        if(strstr((char *)strMN316_rcv.buff, "Switch6OFF")) { mode_selection = 5; System.Switch6 = 0; }
        if(strstr((char *)strMN316_rcv.buff, "Switch7ON"))  { mode_selection = 6; System.Switch7 = 1; }
        if(strstr((char *)strMN316_rcv.buff, "Switch7OFF")) { mode_selection = 6; System.Switch7 = 0; }
        if(strstr((char *)strMN316_rcv.buff, "WtrTempThresholdDown"))  { mode_selection = 0; Threshold.WtrTempThreshold -= 1; }
        if(strstr((char *)strMN316_rcv.buff, "WtrTempThresholdAdd"))   { mode_selection = 0; Threshold.WtrTempThreshold += 1; }
        if(strstr((char *)strMN316_rcv.buff, "WtrLevelThresholdDown")) { mode_selection = 1; Threshold.WtrLevelThreshold -= 1; }
        if(strstr((char *)strMN316_rcv.buff, "WtrLevelThresholdAdd"))  { mode_selection = 1; Threshold.WtrLevelThreshold += 1; }
        if(strstr((char *)strMN316_rcv.buff, "LightThresholdDown"))    { mode_selection = 2; Threshold.LightThreshold -= 10; }
        if(strstr((char *)strMN316_rcv.buff, "LightThresholdAdd"))     { mode_selection = 2; Threshold.LightThreshold += 10; }
        if(strstr((char *)strMN316_rcv.buff, "TurbThersholdDown"))     { mode_selection = 3; Threshold.TurbThreshold -= 10; }
        if(strstr((char *)strMN316_rcv.buff, "TurbThersholdAdd"))      { mode_selection = 3; Threshold.TurbThreshold += 10; }
        if(strstr((char *)strMN316_rcv.buff, "PHMaxDown"))  { mode_selection = 4; Threshold.PHMax -= 1; }
        if(strstr((char *)strMN316_rcv.buff, "PHMaxAdd"))   { mode_selection = 4; Threshold.PHMax += 1; }
        if(strstr((char *)strMN316_rcv.buff, "PHMinDown"))  { mode_selection = 5; Threshold.PHMin -= 1; }
        if(strstr((char *)strMN316_rcv.buff, "PHMinAdd"))   { mode_selection = 5; Threshold.PHMin += 1; }
        if(strstr((char *)strMN316_rcv.buff, "FeedMode"))   { TimeMode_Selection = 0; Threshold.FeedHour -= 1; }
        if(strstr((char *)strMN316_rcv.buff, "OxMode"))     { TimeMode_Selection = 1; Threshold.FeedHour += 1; }
        if(strstr((char *)strMN316_rcv.buff, "HourDown"))   { if(TimeMode_Selection == 0) Threshold.FeedHour -= 1; else Threshold.OxHour -= 1; }
        if(strstr((char *)strMN316_rcv.buff, "HourAdd"))    { if(TimeMode_Selection == 0) Threshold.FeedHour += 1; else Threshold.OxHour += 1; }
        if(strstr((char *)strMN316_rcv.buff, "MinuteDown")) { if(TimeMode_Selection == 0) Threshold.FeedMinute -= 1; else Threshold.OxMinute -= 1; }
        if(strstr((char *)strMN316_rcv.buff, "MinuteAdd"))  { if(TimeMode_Selection == 0) Threshold.FeedMinute += 1; else Threshold.OxMinute += 1; }
        if(strstr((char *)strMN316_rcv.buff, "TimeDown"))   { if(TimeMode_Selection == 0) Threshold.FeedTime -= 1; else Threshold.OxTime -= 1; }
        if(strstr((char *)strMN316_rcv.buff, "TimeAdd"))    { if(TimeMode_Selection == 0) Threshold.FeedTime += 1; else Threshold.OxTime += 1; }
        if(strstr((char *)strMN316_rcv.buff, "TimeON"))     { FeedTimeFlag = 1; }
        if(strstr((char *)strMN316_rcv.buff, "TimeOFF"))    { FeedTimeFlag = 0; }

        memset(strMN316_rcv.buff, 0, MN316_RX_BUFF_SIZE);
        strMN316_rcv.rcv_cnt = 0;
    }
}

/************************************
MN316_run_handle
ܣMN316״̬ATָ̺MQTTͨ
ֵ޷ֵ
************************************/
void MN316_run_handle(void)
{
    unsigned char ret;
    static unsigned int init_tick = 0;

    /* ʼ״̬ѡATָ */
    if(strMN316_info.Net_stu == MN316_STATE_NULL)
    {
        MN316_init();
        strMN316_info.Net_stu = MN316_STATE_AT_CHECK;
        init_tick = get_sys_tick();
    }

    /* ڳʼǰȴ500msģϵ */
    if(strMN316_info.Net_stu == MN316_STATE_AT_CHECK)
    {
        if(get_sys_tick() - init_tick < 500) return;

        ret = write_cmd(&strMN316_cmd[0]);
        if(ret == 1)
        {
            strMN316_info.Net_stu = MN316_STATE_MQTT_CFG;
        }
        else if(ret == 2)
        {
            strMN316_info.Net_stu = MN316_STATE_NULL;
        }
    }
    else if(strMN316_info.Net_stu == MN316_STATE_MQTT_CFG)
    {
        ret = write_cmd(&strMN316_cmd[1]);
        if(ret == 1)
        {
            strMN316_info.Net_stu = MN316_STATE_MQTT_OPEN;
        }
        else if(ret == 2)
        {
            strMN316_info.Net_stu = MN316_STATE_NULL;
        }
    }
    else if(strMN316_info.Net_stu == MN316_STATE_MQTT_OPEN)
    {
        ret = write_cmd(&strMN316_cmd[2]);
        if(ret == 1)
        {
            strMN316_info.Net_stu = MN316_STATE_MQTT_SUB;
            strMN316_info.MQTT_Connect_flag = 1;
        }
        else if(ret == 2)
        {
            strMN316_info.Net_stu = MN316_STATE_NULL;
        }
    }
    else if(strMN316_info.Net_stu == MN316_STATE_MQTT_SUB)
    {
        ret = write_cmd(&strMN316_cmd[3]);
        if(ret == 1)
        {
            strMN316_info.Net_stu = MN316_STATE_READY;
            memset(strMN316_rcv.buff, 0, MN316_RX_BUFF_SIZE);
            strMN316_rcv.rcv_cnt = 0;
        }
        else if(ret == 2)
        {
            strMN316_info.Net_stu = MN316_STATE_NULL;
        }
    }
    /* ӳɹ״̬ */
    else if(strMN316_info.Net_stu == MN316_STATE_READY)
    {
        if(get_sys_tick() - last_pub_cnt >= 1000)
        {
            last_pub_cnt = get_sys_tick();
            MN316_publish_sensor_data();
        }

        process_rx_buffer();

        strMN316_info.rcv_idle_cnt = (get_sys_tick() - strMN316_rcv.rcv_trick_ms) / 1000;
        if(strMN316_info.rcv_idle_cnt > 30)
        {
            strMN316_info.Net_stu = MN316_STATE_NULL;
            strMN316_info.offline_cnt++;
        }
    }
}
