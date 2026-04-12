#include "sys.h"

//模式切换标志位
uint8_t mode_selection = 0;
//页面显示标志位
uint8_t displayFlag = 0;
//阈值设置标志位
uint8_t thresholdFlag = 0;
//手动模式标志位
uint8_t manualFlag = 0;

//定时模式切换标志位
uint8_t TimeMode_Selection = 0;
//定时模式标志位
uint8_t TimeFlag = 0;
//定时投喂开关标志位
uint8_t FeedTimeFlag = 0;
//定时增氧开关标志位
uint8_t OxTimeFlag = 0;
//投喂执行标志位
uint8_t FeedRun = 0;
//增氧执行标志位
uint8_t OxRun = 0;

/*****************贯穿整个系统的函数 在主函数只需要调用这一个函数*/

void Mode_selection(void)
{
	//调用执行设备函数 此函数不能删除
	Action();
	
	//按键1 按下模式切换
	if ((isKey1) && (displayFlag == 0)) {
		isKey1 = isKey2 = isKey3 = isKey4 =0;
		mode_selection++;
		if(mode_selection == 2){
			System.Switch1 = System.Switch2 = System.Switch3= System.Switch4 = System.Switch5 = System.Switch6 = System.Switch7 =0;//进入手动界面关闭所有开关
		}
		oled_Clear();
	}
	
	if((isKey1) && (displayFlag != 0)){
		displayFlag = 0;
		isKey1 = 0;
		oled_Clear();
	}
	
	//模式切换标志位限位
	if (mode_selection >= 4){
		mode_selection = 0;
	}
	
	//根据标志位判断进入哪一个模式  手动模式下不调用自动控制函数其余可以调用
	switch (mode_selection)
	{
		case 0:
			Display();//传感器数据显示
			Automatic();//自动控制
			break;
		
		case 1:
			Timed_mode();//定时
			Automatic();//自动控制
        break;
		
		case 2:
			Threshold_Settings();//阈值设置
			Automatic();//自动控制
			break;
		
		case 3:
			Manual();//手动模式
			break;
	}
}

/****
自动控制逻辑判断
****/

void Automatic(void)
{
	if (SensorData.WtrTempVal < Threshold.WtrTempThreshold){//如果水温小于阈值
		System.Switch1 = 1;//继电器-加热开关标志位置1
	}
	else{
		System.Switch1 = 0;//继电器-加热开关标志位置0
	}
	
	if (SensorData.WtrLevelVal < Threshold.WtrLevelThreshold){//如果水位小于阈值
		System.Switch2 = 1;//继电器-补水开关标志位置1
	} 
	else{
		System.Switch2 = 0;//继电器-补水开关标志位置0
	}
	
	if (OxRun == 1){//如果增氧执行标志位为1
		System.Switch3 = 1;//继电器-增氧标志位置1
	} 
	else{
		System.Switch3 = 0;//继电器-增氧标志位置0
	}
	
	if (SensorData.TurbVal> Threshold.TurbThreshold){//如果浑浊度大于阈值
		System.Switch4 = 1;//继电器-换水开关标志位置1
	} 
	else{
		System.Switch4 = 0;//继电器-换水开关标志位置0
	}
	
	if (FeedRun == 1){//如果投喂执行标志位为1
		System.Switch5 = 1;//舵机-投喂标志位置1
	} 
	else{
		System.Switch5 = 0;//舵机-投喂标志位置0
	}
	
	if (SensorData.LightVal < Threshold.LightThreshold) {//如果光照强度小于阈值
		System.Switch6 = 1;//灯光标志位置1
	}
	else {
		System.Switch6 = 0;//灯光标志位置0
	}
	
	if (SensorData.WtrTempVal < Threshold.WtrTempThreshold || SensorData.WtrLevelVal < Threshold.WtrLevelThreshold||
		SensorData.TurbVal > Threshold.TurbThreshold || SensorData.LightVal < Threshold.LightThreshold ||Threshold.PHMin > (int)SensorData.PHVal ||(int)SensorData.PHVal > Threshold.PHMax) {//如果温度、水位、光照、浑浊度,PH值数据异常
		System.Switch7 = 1;//报警标志位置1
	}
	else {
		System.Switch7 = 0;//报警标志位置0
	}
}

/****
根据执行设备的标志位来开关执行设备
****/

void Action(void)
{
	if (System.Switch1 == 1) {//继电器-加热标志位为1
		RELAY1_ON;//打开继电器-加热
	} else {
		RELAY1_OFF;//关闭继电器-加热
	}
	
	if (System.Switch2 == 1) {//继电器-补水标志位为1
		RELAY2_ON;//打开继电器-补水
	} else {
		RELAY2_OFF;//关闭继电器-补水
	}
	
	if (System.Switch3 == 1) {//继电器-增氧标志位为1
		RELAY3_ON;//打开继电器-增氧
	} else {
		RELAY3_OFF;//关闭继电器-增氧
	}
	
	if (System.Switch4 == 1) {//继电器-换水标志位为1
		RELAY4_ON;//打开继电器-换水
	} else {
		RELAY4_OFF;//关闭继电器-换水
	}
	
	if (System.Switch5 == 1) {//舵机-投喂开关标志位为1
        Control_SG90(2000);//打开舵机-投喂（舵机转动135°）
    } else {
        Control_SG90(500);//关闭舵机-投喂（舵机转回0°）
    }
	
	if (System.Switch6 == 1) {//灯光标志位为1
		LED1_ON;//打开灯光
	} else {
		LED1_OFF;//关闭灯光
	}
	
	if (System.Switch7 == 1) {//报警标志位为1
		BEEP_ON;//打开蜂鸣器
	} else {
		BEEP_OFF;//关闭蜂鸣器
	}
	
	//投喂定时时间计算
	if (FeedTimeFlag == 1)
	{
		if (Threshold.FeedSecond == 0) 
		{
			if (Threshold.FeedMinute > 0)
			{
				Threshold.FeedMinute--;
				Threshold.FeedSecond = 59;
			} 		
			else if(Threshold.FeedMinute==0)
			{
				if (Threshold.FeedHour > 0) {
					Threshold.FeedHour--;
					Threshold.FeedMinute = 60;
				} 
				else {
					FeedTimeFlag = 0;
					FeedRun=1;
				}
			}				
		}
	} 
		
	//增氧定时时间计算
	if (OxTimeFlag == 1)
	{
		if (Threshold.OxSecond == 0)
		{
			if (Threshold.OxMinute > 0)
			{
				Threshold.OxMinute --;
				Threshold.OxSecond = 59;
			}
			else if(Threshold.OxMinute == 0) 
			{
				if (Threshold.OxHour > 0) {
					Threshold.OxHour --;
					Threshold.OxMinute = 59;
				}
				else{
					OxTimeFlag = 0;
					OxRun=1;
				}
			}
		}
	}
}

/****
传感器数据显示
****/

void Display(void)
{
	if(isKey2)
	{
		isKey2 = 0;
		oled_Clear();
		displayFlag = !displayFlag;
	}
	
	if(isKey3)
	{
		isKey3 = 0;
		oled_Clear();
		displayFlag = 2;
	}
	
	if(isKey4)
	{
		isKey4 = 0;
		oled_Clear();
		displayFlag = 3;
	}
	
	//第一个页面
	if(displayFlag == 0)
	{
		//智能鱼缸
		oled_ShowCHinese(16 * 2, 2 * 0, 0);
		oled_ShowCHinese(16 * 3, 2 * 0, 1);
		oled_ShowCHinese(16 * 4, 2 * 0, 2);
		oled_ShowCHinese(16 * 5, 2 * 0, 3);
		
		//水温
		oled_ShowCHinese(16 * 0, 2 * 1, 22);
		oled_ShowCHinese(16 * 1, 2 * 1, 23);
		oled_ShowString(16 * 3, 2 * 1, ":", 16);
		oled_ShowNum(16 * 4, 2 * 1, SensorData.WtrTempVal, 2, 16);//显示水温数据
		oled_ShowCHinese(16 * 5, 2 * 1, 24);
		
		//水位
		oled_ShowCHinese(16 * 0, 2 * 2, 22);
		oled_ShowCHinese(16 * 1, 2 * 2, 25);
		oled_ShowString(16 * 3, 2 * 2, ":", 16);
		oled_ShowNum(16 * 4, 2 * 2, SensorData.WtrLevelVal, 2, 16);//显示水位数据
		oled_ShowString(16 * 5, 2 * 2, "mm", 16);
		
		//光照强度
		oled_ShowCHinese(16 * 0, 2 * 3, 19);
		oled_ShowCHinese(16 * 1, 2 * 3, 26);
		oled_ShowString(16 * 3, 2 * 3, ":", 16);
		oled_ShowNum(16 * 4, 2 * 3, SensorData.LightVal, 3, 16);//显示光照强度
		oled_ShowString(16 * 6, 2 * 3, "Lux", 16);
	}
	
	//第二个页面
	if(displayFlag == 1)
	{
		//浊度
		oled_ShowCHinese(16 * 0, 2 * 0, 28);
		oled_ShowCHinese(16 * 1, 2 * 0, 29);
		oled_ShowString(16 * 3, 2 * 0, ":", 16);
		oled_ShowNum(16 * 4, 2 * 0, SensorData.TurbVal, 4, 16);//显示浊度数据
		oled_ShowString(16 * 6, 2 * 0, "NTU", 16);
		
		
		
		//PH值
		oled_ShowString(16 * 0, 2 * 1, "PH", 16);
		oled_ShowString(16 * 3, 2 * 1, ":", 16);
		OLED_ShowFNum(16 * 4, 2 * 1, SensorData.PHVal,16);//显示PH数据
		
		
		
		
	}
	
	//MQTT连接显示
	if(displayFlag == 2){
		MqttCon_Display();
	}
	
	//调用显示订阅主题的参数
	if(displayFlag == 3){
		Topic_Display();
	}
}

/******************
功能：定时模式
参数：无
返回值：无
******************/
void Timed_mode()
{
	//定时
	oled_ShowCHinese(16 * 2, 2 * 0, 4);
	oled_ShowCHinese(16 * 3, 2 * 0, 5);
	
	//按下选中
	if (isKey2) {
		TimeFlag++;
		if (TimeFlag > 4){
			TimeFlag = 0;
		}
		isKey2 = 0;
	}
	
	//选择设置投喂还是增氧
	if (TimeFlag == 0){
		oled_ShowString(16 * 7, 2 * 0, "*", 16);
		if (isKey3) {
			TimeMode_Selection=!TimeMode_Selection;
			isKey3 = 0;
		}
	}
	else{
		oled_ShowString(16 * 7, 2 * 0, "  ", 16);
	}
		
	//显示投喂还是增氧
	if (TimeMode_Selection == 0){
		//投喂
		oled_ShowCHinese(16 * 4, 2 * 0, 36);
		oled_ShowCHinese(16 * 5, 2 * 0, 37);
	} 
	if (TimeMode_Selection == 1){
		//增氧
		oled_ShowCHinese(16 * 4, 2 * 0, 33);
		oled_ShowCHinese(16 * 5, 2 * 0, 34);
	}		
		
	//如果为投喂设置
	if(TimeMode_Selection == 0)
	{
		//定时
		oled_ShowCHinese(16 * 0, 2 * 1, 4);
		oled_ShowCHinese(16 * 1, 2 * 1, 5);
		oled_ShowString(16 * 2, 2 * 1, ":", 16);
		oled_ShowNum(16 * 3, 2 * 1, Threshold.FeedHour, 2, 16);//时
		oled_ShowString(16 * 4, 2 * 1, ":", 16);
		oled_ShowNum(16 * 5, 2 * 1, Threshold.FeedMinute, 2, 16);//分
		oled_ShowString(16 * 6, 2 * 1, ":", 16);
		oled_ShowNum(16 * 7, 2 * 1, Threshold.FeedSecond, 2, 16);//秒
		
		//执行时间
		oled_ShowCHinese(16 * 0, 2 * 2, 38);
		oled_ShowCHinese(16 * 1, 2 * 2, 39);
		oled_ShowCHinese(16 * 2, 2 * 2, 40);
		oled_ShowCHinese(16 * 3, 2 * 2, 41);
		oled_ShowString(16 * 4, 2 * 2, ":", 16);
		oled_ShowNum(16 * 5, 2 * 2, Threshold.FeedTime, 3, 16);	 
		
		//定时开关
		oled_ShowCHinese(16 * 0, 2 * 3, 4);
		oled_ShowCHinese(16 * 1, 2 * 3, 5);
		oled_ShowCHinese(16 * 2, 2 * 3, 14);
		oled_ShowCHinese(16 * 3, 2 * 3, 16);
		oled_ShowString(16 * 4, 2 * 3, ":", 16);
			
		//设置时
		if (TimeFlag == 1) {
			oled_ShowString(16 * 3 + 8, 2 * 1, "_", 16);
		
			if (isKey3) {
				Threshold.FeedHour++;
				isKey3 = 0;
			}
		
			if (isKey4) {
				Threshold.FeedHour--;
				isKey4 = 0;
			}
		}
		else{
		}
		
		//设置分
		if (TimeFlag == 2) {
			oled_ShowString(16 * 5 + 8, 2 * 1, "_", 16);
		
			if (isKey3) {
				Threshold.FeedMinute++;
				isKey3 = 0;
			}
		
			if (isKey4) {
				Threshold.FeedMinute--;
				isKey4 = 0;
			}
		}
		else{
		}
		
 
			
		//设置喂食时间
		if(TimeFlag==3){
			oled_ShowString(16 * 7, 2 * 2, "*", 16);
			if(isKey3){
				Threshold.FeedTime++;
				isKey3=0;
			}
			if(isKey4){
				Threshold.FeedTime--;
				isKey4=0;
		}		
			
		}else   oled_ShowString(16 * 7, 2 * 2, "  ", 16);
		
		
		//定时开关
		if (TimeFlag == 4) {
			oled_ShowString(16 * 7, 2 * 3, "*", 16);
			
			if (isKey3) {
				FeedTimeFlag = !FeedTimeFlag;//定时开关标志位取反
				isKey3 = 0;
			}
			
			//定时时间清零
			if (isKey4) {
				Threshold.FeedHour = 0;
				Threshold.FeedMinute = 0;
				Threshold.FeedSecond = 0;
				Threshold.FeedTime = 0;
				isKey4 = 0;
			}
		}	else {
			oled_ShowString(16 * 7, 2 * 3, "  ", 16);
		}
	
		//显示开关
		if (FeedTimeFlag == 1){
			oled_ShowCHinese(16 * 5, 2 * 3, 14);
			oled_ShowCHinese(16 * 6, 2 * 3, 15);
		}
		else{
			oled_ShowCHinese(16 * 5, 2 * 3, 16);
			oled_ShowCHinese(16 * 6, 2 * 3, 17);
		}		
	}
		
	//如果是增氧设置
	if(TimeMode_Selection==1)
	{
		//定时
		oled_ShowCHinese(16 * 0, 2 * 1, 4);
		oled_ShowCHinese(16 * 1, 2 * 1, 5);
		oled_ShowString(16 * 2, 2 * 1, ":", 16);
		oled_ShowNum(16 * 3, 2 * 1, Threshold.OxHour, 2, 16);//时
		oled_ShowString(16 * 4, 2 * 1, ":", 16);
		oled_ShowNum(16 * 5, 2 * 1, Threshold.OxMinute, 2, 16);//分
		oled_ShowString(16 * 6, 2 * 1, ":", 16);
		oled_ShowNum(16 * 7, 2 * 1, Threshold.OxSecond, 2, 16);//秒
		
		//执行时间
		oled_ShowCHinese(16 * 0, 2 * 2, 38);
		oled_ShowCHinese(16 * 1, 2 * 2, 39);
		oled_ShowCHinese(16 * 2, 2 * 2, 40);
		oled_ShowCHinese(16 * 3, 2 * 2, 41);
		oled_ShowString(16 * 4, 2 * 2, ":", 16);
		oled_ShowNum(16 * 5, 2 * 2, Threshold.OxTime, 3, 16);	 
		
		//定时开关
		oled_ShowCHinese(16 * 0, 2 * 3, 4);
		oled_ShowCHinese(16 * 1, 2 * 3, 5);
		oled_ShowCHinese(16 * 2, 2 * 3, 14);
		oled_ShowCHinese(16 * 3, 2 * 3, 16);
		oled_ShowString(16 * 4, 2 * 3, ":", 16);
			
		//设置时
		if (TimeFlag == 1) {
			oled_ShowString(16 * 3 + 8, 2 * 1, "_", 16);
			
			if (isKey3) {
				Threshold.OxHour++;
				isKey3 = 0;
			}
		
			if (isKey4) {
				Threshold.OxHour--;
				isKey4 = 0;
			}
		}
		else{
		}
		
		//设置分
		if (TimeFlag == 2) {
			oled_ShowString(16 * 5 + 8, 2 * 1, "_", 16);
		
			if (isKey3) {
				Threshold.OxMinute++;
				isKey3 = 0;
			}
		
			if (isKey4) {
				Threshold.OxMinute--;
				isKey4 = 0;
			}
		}
		else{
		}
		
		 
			
		//设置喂食时间
		if(TimeFlag==3){
			oled_ShowString(16 * 7, 2 * 2, "*", 16);
			if(isKey3){
				Threshold.OxTime++;
				isKey3=0;
			}
			if(isKey4){
				Threshold.OxTime--;
				isKey4=0;
		}		
			
		}else   oled_ShowString(16 * 7, 2 * 2, "  ", 16);
		
		
		//定时开关
		if (TimeFlag == 4){
			oled_ShowString(16 * 7, 2 * 3, "*", 16);
			
			if (isKey3){
				OxTimeFlag = !OxTimeFlag;//定时开关标志位取反
				isKey3 = 0;
			}
			
			//定时时间清零
			if (isKey4)
			{
				Threshold.OxHour = 0;
				Threshold.OxMinute = 0;
				Threshold.OxSecond = 0;
				Threshold.OxTime=0;
				isKey4 = 0;
			}
		}
		else{
			oled_ShowString(16 * 7, 2 * 3, "  ", 16);
		}
	
		//显示开关
		if (OxTimeFlag == 1){
			oled_ShowCHinese(16 * 5, 2 * 3, 14);
			oled_ShowCHinese(16 * 6, 2 * 3, 15);
		} 
		else{
			oled_ShowCHinese(16 * 5, 2 * 3, 16);
			oled_ShowCHinese(16 * 6, 2 * 3, 17);
		}
	}
	
	if(Threshold.FeedHour > 24){
		Threshold.FeedHour = 0;
	}
	if(Threshold.FeedMinute > 60){
		Threshold.FeedMinute = 0;
	}
	if(Threshold.FeedSecond > 60){
		Threshold.FeedSecond = 0;
	}
	
	if(Threshold.OxHour > 24){
		Threshold.OxHour = 0;
	}
	if(Threshold.OxMinute > 60){
		Threshold.OxMinute = 0;
	}
	if(Threshold.OxSecond > 60){
		Threshold.OxSecond = 0;
	}
	
	if(Threshold.FeedTime > 99){
		Threshold.FeedTime = 0;
	}
	
	if(Threshold.OxTime > 99){
		Threshold.OxTime = 0;
	}
}

/****
阈值设置
****/
void Threshold_Settings(void)
{
    //按键2按下选中设置阈值
    if (isKey2) {
        isKey2 = 0;
        thresholdFlag++;

        //限位标志 
        if (thresholdFlag >= 6) {
            thresholdFlag = 0;
        }
		// 翻页时清屏
        if (thresholdFlag == 3 || thresholdFlag == 0)
        {
            oled_Clear();
        }
    }

    //调用光标显示函数
    Asterisk(thresholdFlag);

    //按键3按下阈值加
    if (isKey3) {
        isKey3 = 0;//清除按键3标志位

        switch (thresholdFlag) 
		{
			case 0:
				Threshold.WtrTempThreshold ++;//水温阈值+1
				break;
			case 1:
				Threshold.WtrLevelThreshold ++;//水位阈值+1
				break;
			case 2:
				Threshold.LightThreshold += 10;//光照强度阈值+10
				break;
			case 3:
				Threshold.TurbThreshold += 10;//浊度阈值+10
				break;
			case 4:
				Threshold.PHMax += 1;//PHmax阈值+1
				break;
			case 5:
				Threshold.PHMin += 1;//PHmin阈值+1
				break;
        }
    }

    //按键4按下阈值减
    if (isKey4) {
        isKey4 = 0;

        switch (thresholdFlag) {
			case 0:
				Threshold.WtrTempThreshold --;//水温阈值-1
				break;
			case 1:
				Threshold.WtrLevelThreshold --;//水位阈值-1
				break;
			case 2:
				Threshold.LightThreshold -= 10;//光照强度阈值-10
				break;
			case 3:
				Threshold.TurbThreshold -= 10;//浊度阈值-10
				break;
			case 4:
				Threshold.PHMax -= 1;//PHmax阈值-1
				break;
			case 5:
				Threshold.PHMin -= 1;//PHmin阈值-1
				break;
        }
    }
    //如果标志位小于4 显示第一页
    if (thresholdFlag < 3) {
		//阈值设置
		oled_ShowCHinese(16 * 2, 2 * 0, 8);
		oled_ShowCHinese(16 * 3, 2 * 0, 9);
		oled_ShowCHinese(16 * 4, 2 * 0, 10);
		oled_ShowCHinese(16 * 5, 2 * 0, 11);
		
		//水温阈值
		oled_ShowCHinese(16 * 0, 2 * 1, 22);
		oled_ShowCHinese(16 * 1, 2 * 1, 23);
		oled_ShowCHinese(16 * 2, 2 * 1, 8);
		oled_ShowCHinese(16 * 3, 2 * 1, 9);
		oled_ShowString(16 * 4, 2 * 1, ":", 16);
		oled_ShowNum(16 * 5, 2 * 1, Threshold.WtrTempThreshold, 2, 16);//显示水温阈值
		
		//水位阈值
		oled_ShowCHinese(16 * 0, 2 * 2, 22);
		oled_ShowCHinese(16 * 1, 2 * 2, 25);
		oled_ShowCHinese(16 * 2, 2 * 2, 8);
		oled_ShowCHinese(16 * 3, 2 * 2, 9);
		oled_ShowString(16 * 4, 2 * 2, ":", 16);
		oled_ShowNum(16 * 5, 2 * 2, Threshold.WtrLevelThreshold, 2, 16);//显示水位阈值

		//光照阈值
		oled_ShowCHinese(16 * 0, 2 * 3, 19);
		oled_ShowCHinese(16 * 1, 2 * 3, 26);
		oled_ShowCHinese(16 * 2, 2 * 3, 8);
		oled_ShowCHinese(16 * 3, 2 * 3, 9);
		oled_ShowString(16 * 4, 2 * 3, ":", 16);
		oled_ShowNum(16 * 5, 2 * 3, Threshold.LightThreshold, 3, 16);//显示光照强度阈值
	}
	 else
    {
        //浊度阈值
        oled_ShowCHinese(16 * 0, 2 * 0, 28);
        oled_ShowCHinese(16 * 1, 2 * 0, 29);
        oled_ShowCHinese(16 * 2, 2 * 0, 8);
        oled_ShowCHinese(16 * 3, 2 * 0, 9);
        oled_ShowString(16 * 4, 2 * 0, ":", 16);
        oled_ShowNum(16 * 5, 2 * 0, Threshold.TurbThreshold, 4, 16);//显示浊度阈值
		
		
		//PHmax值
		oled_ShowString(16 * 0, 2 * 1, "PH", 16);
		oled_ShowCHinese(16 * 1, 2 * 1, 42);
    oled_ShowCHinese(16 * 2, 2 * 1, 44);
		oled_ShowString(16 * 3, 2 * 1, ":", 16);
		oled_ShowNum(16 * 4, 2 * 1, Threshold.PHMax, 2, 16);//显示PHmax阈值
		
		//PHmin值
		oled_ShowString(16 * 0, 2 * 2, "PH", 16);
		oled_ShowCHinese(16 * 1, 2 * 2, 43);
    oled_ShowCHinese(16 * 2, 2 * 2, 44);	
		oled_ShowString(16 * 3, 2 * 2, ":", 16);
		oled_ShowNum(16 * 4, 2 * 2,Threshold.PHMin, 2, 16);//显示PHmin阈值
		
		
    }
	
	/*阈值限幅*/
	
	//水温
    if (Threshold.WtrTempThreshold > 99){
        Threshold.WtrTempThreshold = 99;
    }
	if (Threshold.WtrTempThreshold < 1){
        Threshold.WtrTempThreshold = 0;
    }
	
	//水位
    if (Threshold.WtrLevelThreshold > 99){
        Threshold.WtrLevelThreshold = 99;
    }
	if (Threshold.WtrLevelThreshold < 1){
        Threshold.WtrLevelThreshold = 0;
    }
	
	//光照强度
    if (Threshold.LightThreshold > 999){
        Threshold.LightThreshold = 999;
    }
	if (Threshold.LightThreshold < 1){
        Threshold.LightThreshold = 0;
    }
	
	//浑浊度
    if (Threshold.TurbThreshold > 1199){
        Threshold.TurbThreshold = 1199;
    }
	if (Threshold.TurbThreshold < 1){
        Threshold.TurbThreshold = 0;
    }
}

/****
手动模式
****/

void Manual(void)
{

    //按下按键2选中
    if (isKey2) {
        isKey2 = 0;
        manualFlag++;

        //限位
        if (manualFlag >= 7) {
            manualFlag = 0;
        }
		
		if (manualFlag == 3 || manualFlag == 0)
        {
            oled_Clear();
        }
    }

    //调用光标显示函数
    Asterisk(manualFlag);
	
    //按下按键3状态取反
    if (isKey3) {
        isKey3 = 0;

        switch (manualFlag) 
		{
			case 0:
				System.Switch1 = !System.Switch1;//继电器-加热状态取反
				break;
				
			case 1:
				System.Switch2 = !System.Switch2;//继电器-补水状态取反
				break;
			
			case 2:
				System.Switch3 = !System.Switch3;//继电器-增氧状态取反
				break;
			
			case 3:
				System.Switch4 = !System.Switch4;//继电器-换水状态取反
				break;
			
			case 4:
				System.Switch5 = !System.Switch5;//舵机-投喂状态取反
				break;
			
			case 5:
				System.Switch6 = !System.Switch6;//灯光状态取反
				break;
			
			case 6:
				System.Switch7 = !System.Switch7;//报警状态取反
				break;
        }
    }
    //如果标志位小于3 显示第一页
    if (manualFlag < 3) {
		//手动模式
		oled_ShowCHinese(16 * 2, 2 * 0, 12);
		oled_ShowCHinese(16 * 3, 2 * 0, 13);
		oled_ShowCHinese(16 * 4, 2 * 0, 6);
		oled_ShowCHinese(16 * 5, 2 * 0, 7);
		
		//加热
		oled_ShowCHinese(16 * 0, 2 * 1, 30);
		oled_ShowCHinese(16 * 1, 2 * 1, 31);
		oled_ShowString(16 * 2, 2 * 1, ":", 16);
		
		if (System.Switch1 == 1) {
			oled_ShowCHinese(16 * 3, 2 * 1, 14);
			oled_ShowCHinese(16 * 4, 2 * 1, 15);//开启
		} else {
			oled_ShowCHinese(16 * 3, 2 * 1, 16);
			oled_ShowCHinese(16 * 4, 2 * 1, 17);//关闭
		}
		
		//补水
		oled_ShowCHinese(16 * 0, 2 * 2, 32);
		oled_ShowCHinese(16 * 1, 2 * 2, 22);
		oled_ShowString(16 * 2, 2 * 2, ":", 16);
		
		if (System.Switch2 == 1) {
			oled_ShowCHinese(16 * 3, 2 * 2, 14);
			oled_ShowCHinese(16 * 4, 2 * 2, 15);//开启
		} else {
			oled_ShowCHinese(16 * 3, 2 * 2, 16);
			oled_ShowCHinese(16 * 4, 2 * 2, 17);//关闭
		}
		
		//增氧
		oled_ShowCHinese(16 * 0, 2 * 3, 33);
		oled_ShowCHinese(16 * 1, 2 * 3, 34);
		oled_ShowString(16 * 2, 2 * 3, ":", 16);
		
		if (System.Switch3 == 1) {
			oled_ShowCHinese(16 * 3, 2 * 3, 14);
			oled_ShowCHinese(16 * 4, 2 * 3, 15);//开启
		} else {
			oled_ShowCHinese(16 * 3, 2 * 3, 16);
			oled_ShowCHinese(16 * 4, 2 * 3, 17);//关闭
		}
	}
	else{
		//换水
		oled_ShowCHinese(16 * 0, 2 * 0, 35);
		oled_ShowCHinese(16 * 1, 2 * 0, 22);
		oled_ShowString(16 * 2, 2 * 0, ":", 16);
		
		if (System.Switch4 == 1) {
			oled_ShowCHinese(16 * 3, 2 * 0, 14);
			oled_ShowCHinese(16 * 4, 2 * 0, 15);//开启
		} else {
			oled_ShowCHinese(16 * 3, 2 * 0, 16);
			oled_ShowCHinese(16 * 4, 2 * 0, 17);//关闭
		}
		
		//投喂
		oled_ShowCHinese(16 * 0, 2 * 1, 36);
		oled_ShowCHinese(16 * 1, 2 * 1, 37);
		oled_ShowString(16 * 2, 2 * 1, ":", 16);
		
		if (System.Switch5 == 1) {
			oled_ShowCHinese(16 * 3, 2 * 1, 14);
			oled_ShowCHinese(16 * 4, 2 * 1, 15);//开启
		} else {
			oled_ShowCHinese(16 * 3, 2 * 1, 16);
			oled_ShowCHinese(16 * 4, 2 * 1, 17);//关闭
		}
		
		//灯光
		oled_ShowCHinese(16 * 0, 2 * 2, 18);
		oled_ShowCHinese(16 * 1, 2 * 2, 19);
		oled_ShowString(16 * 2, 2 * 2, ":", 16);
		
		if (System.Switch6 == 1) {
			oled_ShowCHinese(16 * 3, 2 * 2, 14);
			oled_ShowCHinese(16 * 4, 2 * 2, 15);//开启
		} else {
			oled_ShowCHinese(16 * 3, 2 * 2, 16);
			oled_ShowCHinese(16 * 4, 2 * 2, 17);//关闭
		}
		
		//报警
		oled_ShowCHinese(16 * 0, 2 * 3, 20);
		oled_ShowCHinese(16 * 1, 2 * 3, 21);
		oled_ShowString(16 * 2, 2 * 3, ":", 16);
		
		if (System.Switch7 == 1) {
			oled_ShowCHinese(16 * 3, 2 * 3, 14);
			oled_ShowCHinese(16 * 4, 2 * 3, 15);//开启
		} else {
			oled_ShowCHinese(16 * 3, 2 * 3, 16);
			oled_ShowCHinese(16 * 4, 2 * 3, 17);//关闭
		}
	}
}

void Asterisk(uint8_t A)
{
    if (A == 3 || A == 7)
    {
        oled_ShowString(16 * 7, 2 * 0, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 0, " ", 16);
    }

    if (A == 0 || A == 4)
    {
        oled_ShowString(16 * 7, 2 * 1, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 1, " ", 16);
    }

    if (A == 1 || A == 5)
    {
        oled_ShowString(16 * 7, 2 * 2, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 2, " ", 16);
    }

    if (A == 2 || A == 6)
    {
        oled_ShowString(16 * 7, 2 * 3, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 3, " ", 16);
    }
}
