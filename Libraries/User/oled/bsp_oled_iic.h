#ifndef __OLEDIIC_H
#define __OLEDIIC_H	

/* OLED I2C 椹卞姩澶存枃浠
 * 閫傜敤纭浠: SSD1306鎺у埗鍣 + 7浣嶅湴鍧0x3C(鍐欏湴鍧0x78)
 * 灞忓箷鍒嗚鲸鐜: 128x64
 * 閫氫俊鏂瑰紡: GPIO妯℃嫙I2C鍗忚
 * 瀛椾綋鏀鎸: 6x8 ASCII / 8x16 ASCII / 16x16姹夊瓧
 * 鍧愭爣绯昏存槑:
 *   - X杞磋寖鍥: 0-127 (鍒楀湴鍧)
 *   - Y杞磋寖鍥: 0-7   (椤靛湴鍧锛屾瘡椤8涓鍍忕礌琛) */

#include "sys.h"  // 渚濊禆绯荤粺绾уご鏂囦欢(鍖呭惈STM32瀵勫瓨鍣ㄥ畾涔)

/* 鐘舵佸畯瀹氫箟 */
#define TRUE        1  // 閫昏緫鐪熷
#define FALSE       0  // 閫昏緫鍋囧

/* 纭浠跺紩鑴氶厤缃 (浠GPIOA涓轰緥) */
#define SCL_1_PORT    GPIOA  // SCL鏃堕挓绾跨鍙
#define SCL_1_PIN     GPIO_Pin_12  // PA12浣滀负SCL寮曡剼
#define SDA_1_PORT    GPIOA  // SDA鏁版嵁绾跨鍙
#define SDA_1_PIN     GPIO_Pin_11  // PA11浣滀负SDA寮曡剼

/* GPIO鐢靛钩鎿嶄綔瀹 
 * 浣跨敤BRR/BSRR瀵勫瓨鍣ㄥ疄鐜板師瀛愭搷浣
 * BSRR: 浣16浣嶈剧疆寮曡剼锛岄珮16浣嶅嶄綅寮曡剼
 * BRR : 澶嶄綅寮曡剼瀵勫瓨鍣 */
#define SCL_H         SCL_1_PORT->BSRR = SCL_1_PIN  // SCL缃楂樼數骞
#define SCL_L         SCL_1_PORT->BRR  = SCL_1_PIN  // SCL缃浣庣數骞
#define SDA_H         SDA_1_PORT->BSRR = SDA_1_PIN  // SDA缃楂樼數骞
#define SDA_L         SDA_1_PORT->BRR  = SDA_1_PIN  // SDA缃浣庣數骞

/* GPIO杈撳叆鐘舵佽诲彇瀹 
 * IDR瀵勫瓨鍣ㄥ瓨鍌ㄥ紩鑴氳緭鍏ョ姸鎬 */
#define SCL_read      SCL_1_PORT->IDR  & SCL_1_PIN  // 璇诲彇SCL鐢靛钩鐘舵
#define SDA_read      SDA_1_PORT->IDR  & SDA_1_PIN  // 璇诲彇SDA鐢靛钩鐘舵

/**************************** 鍔熻兘鍑芥暟澹版槑 ******************************/

/* 鍒濆嬪寲SSD1306鎺у埗鍣 
 * 鍖呭惈GPIO鍒濆嬪寲鍜岄厤缃鍛戒护搴忓垪 */
void oled_Init(void);

/* 棰滆壊鍙嶇浉鏄剧ず鎺у埗 
 * 鍙傛暟: i=0姝ｅ父妯″紡, i=1鍙嶈壊妯″紡 */
void oled_ColorTurn(uint8_t i);

/* 灞忓箷鏄剧ず鏂瑰悜鎺у埗 
 * 鍙傛暟: i=0姝ｅ父鏂瑰悜, i=1鏃嬭浆180搴 */
void oled_DisplayTurn(uint8_t i);

/* 娓呭睆鍑芥暟 
 * 灏咷DDRAM鍏ㄩ儴鍐0x00 */
void oled_Clear(void);

/* 鏄剧ず寮鍏虫帶鍒 
 * 鍏抽棴鏄剧ず鍙闄嶄綆鍔熻 */
void oled_Display_On(void);  // 寮鍚鏄剧ず
void oled_Display_Off(void); // 鍏抽棴鏄剧ず

/* 瀛楃︿覆鏄剧ず鍑芥暟 
 * 鍙傛暟:
 *   x,y: 璧峰嬪潗鏍 
 *   p: 瀛楃︿覆鎸囬拡(闇浠'\0'缁撳熬)
 *   Char_Size: 瀛椾綋澶у皬(8/16) */
void oled_ShowString(unsigned char x,unsigned char y, unsigned char *p,unsigned char Char_Size);

/* 16x16鐐归樀姹夊瓧鏄剧ず 
 * 鍙傛暟:
 *   no: 瀛楀簱鏁扮粍绱㈠紩(姣忎釜姹夊瓧鍗32瀛楄妭) */
void oled_ShowCHinese(unsigned char x,unsigned char y,unsigned char no);

/* 浣嶅浘鏄剧ず鍑芥暟 
 * 鍙傛暟:
 *   x0,y0: 璧峰嬪潗鏍 
 *   x1,y1: 缁撴潫鍧愭爣(闇澶т簬璧峰嬪潗鏍)
 *   BMP: 浣嶅浘鏁版嵁鏁扮粍(绾靛悜鍒嗛〉瀛樺偍) */
void oled_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

/* 鏁板瓧鏄剧ず绯诲垪鍑芥暟 
 * 鍙傛暟:
 *   num: 瑕佹樉绀虹殑鏁板 
 *   len: 鏄剧ず浣嶆暟 
 *   size: 瀛椾綋澶у皬 */
void oled_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size); // 瀛楃︽樉绀
void OLED_ShowDNum(unsigned char x,unsigned char y,int Dnum,unsigned char size1); // 鍗佽繘鍒舵暣鍨嬫樉绀
void OLED_ShowFNum(unsigned char x,unsigned char y,float Fnum,unsigned char size1); // 娴鐐规暟鏄剧ず

/* I2C鎿嶄綔鐘舵佹爣蹇 
 * 0:澶辫触 1:鎴愬姛 (鐢卞簳灞傚嚱鏁版洿鏂) */
extern unsigned char OLED_IIC_stu; 

#endif
