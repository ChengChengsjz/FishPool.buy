#include "sys.h"
 
/********************************************************************************/
/* OLED鏄剧ず妯″潡鍩虹閰嶇疆瀹忓畾涔 */
#define OLED_MODE 0       // OLED鎺ュ彛妯″紡锛0=I2C锛1=SPI锛岄渶涓庣‖浠跺尮閰嶏級
#define SIZE 8            // 鍩虹瀛椾綋澶у皬锛8x8鍍忕礌锛
#define XLevelL     0x00  // 鏄惧瓨鍒楀湴鍧浣庝綅璧峰嬪
#define XLevelH     0x10  // 鏄惧瓨鍒楀湴鍧楂樹綅璧峰嬪硷紙缁勫悎浣跨敤璁剧疆鏄惧瓨绐楀彛锛
#define Max_Column 128    // 灞忓箷鏈澶у垪鏁帮紙鍍忕礌瀹藉害锛
#define Max_Row     64    // 灞忓箷鏈澶ц屾暟锛堝儚绱犻珮搴︼級
#define Brightness 0xFF   // 鍒濆嬩寒搴﹀硷紙0x00-0xFF锛孎F涓烘渶浜锛
#define X_WIDTH     128   // 鐗╃悊灞忓箷瀹藉害锛堝儚绱狅級
#define Y_WIDTH     64    // 鐗╃悊灞忓箷楂樺害锛堝儚绱狅級
                      
#define OLED_CMD  0  // 鎿嶄綔绫诲瀷锛氬啓鍛戒护锛堟帶鍒跺瘎瀛樺櫒/鍙傛暟锛
#define OLED_DATA 1  // 鎿嶄綔绫诲瀷锛氬啓鏁版嵁锛堟樉瀛樺唴瀹癸級

/********************************************************************************/
/* 鍩虹纭浠舵帶鍒跺嚱鏁 */
void Delay_50ms(unsigned int Del_50ms);  // 闃诲炲欢鏃跺嚱鏁帮紙鍗曚綅锛50ms鍛ㄦ湡鏁帮級
void Delay_1ms(unsigned int Del_1ms);    // 闃诲炲欢鏃跺嚱鏁帮紙鍗曚綅锛1ms鍛ㄦ湡鏁帮級

/* I2C鍗忚灞傛搷浣 */
void IIC_Start(void);                   // 鐢熸垚I2C璧峰嬫潯浠讹紙SCL楂樻椂SDA涓嬮檷娌匡級
void IIC_Stop(void);                    // 鐢熸垚I2C鍋滄㈡潯浠讹紙SCL楂樻椂SDA涓婂崌娌匡級
void Write_IIC_Command(unsigned char IIC_Command); // 鍙戦佸崟瀛楄妭鍛戒护锛堣嚜鍔ㄨ剧疆DC绾夸负CMD妯″紡锛
void Write_IIC_Data(unsigned char IIC_Data);       // 鍙戦佸崟瀛楄妭鏁版嵁锛堣嚜鍔ㄨ剧疆DC绾夸负DATA妯″紡锛
void Write_IIC_Byte(unsigned char IIC_Byte);       // 鍘熷婭2C瀛楄妭鍙戦侊紙闇鎵嬪姩鎺у埗DC绾跨姸鎬侊級
void IIC_Wait_Ack(void);                // 绛夊緟浠庤惧囧簲绛旓紙妫娴婼DA绾夸綆鐢靛钩锛

/* OLED鏄剧ず鎺у埗灞 */
void OLED_WR_Byte(unsigned dat,unsigned cmd);  // 閫氱敤瀛楄妭鍐欏叆锛坈md=0鍐欏懡浠わ紝cmd=1鍐欐暟鎹锛
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size); // 鎸囧畾鍧愭爣鏄剧ず瀛楃
void OLED_Set_Pos(unsigned char x, unsigned char y); // 璁剧疆鏄惧瓨鍐欏叆璧峰嬪潗鏍囷紙x:0-127, y:0-7瀵瑰簲8椤碉級
void fill_picture(unsigned char fill_Data);    // 鍏ㄥ睆濉鍏呯粺涓鏁版嵁锛堢敤浜庢竻灞忔垨娴嬭瘯锛
 


/* 鍑芥暟鐢虫槑 -----------------------------------------------*/
/* extern void delay_us(unsigned int nus); */
/* 鍙橀噺瀹氫箟 ----------------------------------------------*/


/*-----------------------------------------------------------
 * 鍑芥暟鍚嶏細I2C_delay
 * 鍔熻兘锛欼2C鎬荤嚎鏃跺簭鎺у埗鐭寤舵椂
 * 璇存槑锛
 *   - 閫氳繃绌哄惊鐜瀹炵幇寰濡欑骇寤舵椂
 *   - 缁忓疄娴媔=5涓烘渶浣庢湁鏁堝硷紙24MHz涓婚戜笅绾0.5us锛
 *   - 寤舵椂绮惧害鍙楃紪璇戝櫒浼樺寲鍜屼富棰戝奖鍝嶏紝绉绘嶉渶閲嶆柊鏍″噯
 *-----------------------------------------------------------*/
static void I2C_delay(void)
{
    unsigned char i=5; // 杩欓噷鍙浠ヤ紭鍖栭熷害锛岀粡娴嬭瘯鏈浣庡埌5杩樿兘鍐欏叆
    while(i) 
    { 
        i--; 
    }  
}

/*-----------------------------------------------------------
 * 鍑芥暟鍚嶏細delay5ms
 * 鍔熻兘锛氫骇鐢熺害5ms寤舵椂锛堢簿搴﹁緝浣庯級
 * 璇存槑锛
 *   - 绌哄惊鐜瀹炵幇姣绉掔骇寤舵椂
 *   - 瀹為檯寤舵椂鏃堕棿涓庝富棰戝己鐩稿叧锛堢ず渚嬪煎熀浜庣壒瀹氭椂閽熼厤缃锛
 *   - 寤鸿鏀圭敤瀹氭椂鍣ㄥ疄鐜扮簿纭寤舵椂
 *-----------------------------------------------------------*/
static void delay5ms(void)
{    
    int i=50;  // 寰鐜娆℃暟缁忛獙鍊硷紙24MHz涓嬬害5ms锛
    
    while(i) 
    { 
        i--; 
    }  
}
 
/*
 * 鍑芥暟鍚嶏細I2C_Start
 * 鍔熻兘锛氱敓鎴怚2C璧峰嬫潯浠跺苟妫娴嬫荤嚎鐘舵
 * 杩斿洖鍊硷細
 *   TRUE  - 璧峰嬫潯浠剁敓鎴愭垚鍔
 *   FALSE - 鎬荤嚎蹇欙紙SDA琚鎷変綆锛夋垨鎬荤嚎閿欒锛堟棤娉曟媺浣嶴DA锛
 * 鏃跺簭璇存槑锛
 *   1. 鍒濆嬬姸鎬侊細SCL鍜孲DA鍧囦负楂樼數骞筹紙鎬荤嚎绌洪棽锛
 *   2. 妫娴嬫荤嚎鏄鍚﹁鍗犵敤锛圫DA=0琛ㄧず鎬荤嚎蹇欙級
 *   3. 浜х敓璧峰嬫潯浠讹紙SDA鍦⊿CL楂樼數骞虫湡闂翠骇鐢熶笅闄嶆部锛
 *   4. 楠岃瘉璧峰嬫潯浠舵湁鏁堟э紙鎴愬姛鎷変綆SDA锛
 */
static unsigned char I2C_Start(void)
{
    SDA_H;          // 閲婃斁SDA绾匡紙鎷夐珮锛
    SCL_H;          // 閲婃斁SCL绾匡紙鎷夐珮锛
    I2C_delay();    // 绛夊緟鐢靛钩绋冲畾
    
    /* 鎬荤嚎鐘舵佹娴 */
    if(!SDA_read)   // 妫娴婼DA绾挎槸鍚︿负浣庣數骞
        return FALSE; // 鎬荤嚎琚鍏朵粬璁惧囧崰鐢锛堣繑鍥為敊璇锛
    
    /* 鐢熸垚璧峰嬫潯浠 */
    SDA_L;          // 鍦⊿CL楂樼數骞虫湡闂存媺浣嶴DA锛堣捣濮嬩俊鍙凤級
    I2C_delay();
    
    /* 楠岃瘉璧峰嬩俊鍙 */
    if(SDA_read)    // 妫娴婼DA绾挎槸鍚︽垚鍔熸媺浣
        return FALSE; // SDA鏃犳硶琚鎷変綆锛堟荤嚎寮傚父锛
    
    SDA_L;          // 淇濇寔SDA浣庣數骞筹紙閰嶅悎鍚庣画SCL鎿嶄綔锛
    I2C_delay();
    return TRUE;     // 璧峰嬫潯浠剁敓鎴愭垚鍔
}
 
/*
 * 鍑芥暟鍚嶏細I2C_Stop
 * 鍔熻兘锛氱敓鎴怚2C鍋滄㈡潯浠跺苟閲婃斁鎬荤嚎
 * 鏃跺簭璇存槑锛
 *   1. 纭淇漇CL鍏堢疆浣庯紙閬垮厤鍦⊿CL楂樻椂鐩存帴鏀瑰彉SDA褰㈡垚璧峰嬫潯浠讹級
 *   2. 灏哠DA缃浣庡缓绔嬬ǔ瀹氱姸鎬
 *   3. 鍏堟媺楂楽CL鍐嶆媺楂楽DA锛屽湪SCL楂樼數骞虫湡闂碨DA涓婂崌娌垮舰鎴愬仠姝㈡潯浠
 
 */
static void I2C_Stop(void)
{
    SCL_L;          // 鍏堝皢SCL缃浣庯紝涓虹數骞冲垏鎹㈠噯澶囧畨鍏ㄧ幆澧
    I2C_delay();    // 绛夊緟鐢靛钩绋冲畾
    SDA_L;          // SDA缃浣庡缓绔嬫槑纭鐢靛钩鐘舵
    I2C_delay();
    SCL_H;          // 鎷夐珮SCL锛堟ゆ椂SCL鍜孲DA鍧囦负楂橈級
    I2C_delay();
    SDA_H;          // 鍦⊿CL楂樼數骞虫湡闂撮噴鏀維DA锛堜骇鐢熷仠姝㈡潯浠剁殑涓婂崌娌匡級
    I2C_delay();    // 淇濊瘉鍋滄㈡潯浠舵寔缁鏃堕棿锛>4us锛
}
 
/*
 * 鍑芥暟鍚嶏細I2C_Ack
 * 鍔熻兘锛氱敓鎴怚2C搴旂瓟淇″彿锛圓CK锛
 * 鏃跺簭璇存槑锛
 *   1. SCL缃浣庣數骞筹紙鏁版嵁绾垮厑璁稿彉鍖栵級
 *   2. SDA缃浣庣數骞筹紙ACK淇″彿鏍稿績鍔ㄤ綔锛
 *   3. SCL缃楂樼數骞筹紙涓绘満妫娴婣CK淇″彿锛
 *   4. SCL鎭㈠嶄綆鐢靛钩锛堜负鍚庣画浼犺緭鍑嗗囷級
 * 鍗忚瑙勮寖锛
 *   - 搴旂瓟淇″彿鍙戠敓鍦ㄧ9涓鏃堕挓鍛ㄦ湡锛堟瘡涓瀛楄妭浼犺緭鍚庣殑ACK鍛ㄦ湡锛
 *   - SDA浣庣數骞宠〃绀烘垚鍔熸帴鏀讹紙ACK锛夛紝楂樼數骞宠〃绀烘棤搴旂瓟锛圢ACK锛
 */
static void I2C_Ack(void)
{	
    SCL_L;          /* 鏃堕挓绾跨疆浣庯紝鍏佽告暟鎹绾垮彉鍖 */
    I2C_delay();    /* 绛夊緟鐢靛钩绋冲畾锛堟弧瓒硉LOW鏃跺簭锛 */
    
    SDA_L;          /* 鏁版嵁绾跨疆浣庯紙鐢熸垚ACK淇″彿锛 */
    I2C_delay();    /* 淇濇寔鏁版嵁绋冲畾锛堟弧瓒硉SU:DAT鏃跺簭锛 */
    
    SCL_H;          /* 鏃堕挓绾跨疆楂橈紙涓绘満妫娴婣CK淇″彿锛 */
    I2C_delay();    /* 缁存寔楂樼數骞筹紙婊¤冻tHIGH鏃跺簭锛 */
    
    SCL_L;          /* 鏃堕挓绾挎仮澶嶄綆鐢靛钩锛堢粨鏉烝CK鍛ㄦ湡锛 */
    I2C_delay();    /* 涓轰笅涓鏁版嵁浼犺緭鍛ㄦ湡鍑嗗 */
}
 
/*
 * 鍑芥暟鍚嶏細I2C_NoAck
 * 鍔熻兘锛氱敓鎴怚2C闈炲簲绛斾俊鍙凤紙NACK锛
 * 鍗忚瑙勮寖锛
 *   1. 鍦↖2C鍗忚涓锛屼富鏈哄湪绗9涓鏃堕挓鍛ㄦ湡淇濇寔SDA楂樼數骞宠〃绀篘ACK
 *   2. NACK鐢ㄤ簬浠ヤ笅鍦烘櫙锛
 *      - 涓绘満鎺ユ敹瀹屾渶鍚庝竴涓瀛楄妭鏁版嵁
 *      - 浠庢満鏃犳硶鍝嶅簲鏃跺己鍒剁粓姝浼犺緭
 * 鏃跺簭璇存槑锛
 *   1. SCL鍏堢疆浣庣數骞筹紙鏁版嵁绾垮彉鍖栧畨鍏ㄦ湡锛
 *   2. SDA缃楂樼數骞冲缓绔婲ACK鐘舵
 *   3. SCL缃楂樼數骞充娇浠庢満閿佸瓨NACK淇″彿
 *   4. SCL鎭㈠嶄綆鐢靛钩瀹屾垚淇″彿鍛ㄦ湡
 
 */
static void I2C_NoAck(void)
{	
    SCL_L;          // 鏃堕挓绾跨疆浣庯紝鍏佽告暟鎹绾垮彉鍖
    I2C_delay();    // 绛夊緟鐢靛钩绋冲畾锛坱LOW鏃堕挓浣庡懆鏈熸椂闂达級
    
    SDA_H;          // 鏁版嵁绾跨疆楂橈紙NACK淇″彿鏍稿績鍔ㄤ綔锛
    I2C_delay();    // 淇濇寔鏁版嵁绋冲畾锛坱SU:DAT鏁版嵁寤虹珛鏃堕棿锛
    
    SCL_H;          // 鏃堕挓绾跨疆楂橈紙浠庢満鍦ㄦや笂鍗囨部閲囨牱NACK锛
    I2C_delay();    // 缁存寔楂樼數骞筹紙tHIGH鏃堕挓楂樺懆鏈熸椂闂达級
    
    SCL_L;          // 鏃堕挓绾挎仮澶嶄綆鐢靛钩锛堢粨鏉烴ACK鍛ㄦ湡锛
    I2C_delay();    // 鎬荤嚎绌洪棽鍑嗗囷紙tBUF鎬荤嚎绌洪棽鏃堕棿锛
}
 
/* 鍑芥暟鍔熻兘: 绛夊緟I2C浠庤惧囪繑鍥濧CK搴旂瓟淇″彿
 * 杩斿洖鍊  : 杩斿洖TRUE(1)琛ㄧず鏀跺埌ACK锛岃繑鍥濬ALSE(0)琛ㄧず鏃燗CK
 * 鎵ц屾祦绋:
 *   1. 鎷変綆鏃堕挓绾(SCL_L)鍚鍔ˋCK妫娴嬫祦绋
 *   2. 閲婃斁鏁版嵁绾(SDA_H)浣夸粠璁惧囧彲浠ユ帶鍒禨DA
 *   3. 鎷夐珮鏃堕挓绾(SCL_H)鐢熸垚ACK妫娴嬫椂閽熻剦鍐
 *   4. 鍦ㄦ椂閽熼珮鐢靛钩鏈熼棿妫娴婼DA鐘舵侊細
 *      - 楂樼數骞: 浠庤惧囨棤搴旂瓟(NACK)
 *      - 浣庣數骞: 浠庤惧囨湁搴旂瓟(ACK)
 *   5. 鏈缁堜繚鎸佹椂閽熺嚎浣庣數骞(SCL_L)缁撴潫ACK妫娴
 
 */
static unsigned char I2C_WaitAck(void)
{
	SCL_L;                    // 鎷変綆鏃堕挓绾垮紑濮婣CK妫娴
	I2C_delay();              // 淇濇寔鏃跺簭
	SDA_H;                    // 閲婃斁鏁版嵁绾(鍒囨崲涓鸿緭鍏ユā寮)
	I2C_delay();              // 绛夊緟鎬荤嚎绋冲畾
	SCL_H;                    // 鏃堕挓绾夸笂鍗囨部(浠庤惧囧湪姝ら樁娈垫媺浣嶴DA)
	I2C_delay();              // 淇濇寔鏃堕挓楂樼數骞虫椂闂
	
	if(SDA_read)              // 妫娴婼DA鐢靛钩鐘舵
	{                         // 楂樼數骞 -> 鏃燗CK
      SCL_L;                // 缁撴潫ACK妫娴嬪墠鎷変綆鏃堕挓
	  I2C_delay();
      return FALSE;          // 杩斿洖鏃犲簲绛旂姸鎬
	}
	
	SCL_L;                    // 鏀跺埌ACK鍚庝繚鎸佹椂閽熶綆鐢靛钩
	I2C_delay();
	return TRUE;              // 杩斿洖鎴愬姛鏀跺埌ACK
}
 
/* 鍑芥暟鍔熻兘: 閫氳繃I2C鎬荤嚎鍙戦佸崟瀛楄妭鏁版嵁(MSB浼樺厛)
 * 鍙傛暟     : SendByte - 瑕佸彂閫佺殑瀛楄妭鏁版嵁
 * 鎵ц屾祦绋:
 *   1. 寰鐜8娆′緷娆″彂閫佹瘡涓猙it(浠庢渶楂樹綅鍒版渶浣庝綅)
 *   2. 姣忎釜bit鍙戦佹ラ:
 *      a. 鎷変綆SCL杩涘叆鏁版嵁鍑嗗囬樁娈
 *      b. 鏍规嵁褰撳墠bit鍊艰剧疆SDA鐢靛钩(楂樼數骞=1,浣庣數骞=0)
 *      c. 宸︾Щ鏁版嵁鍑嗗囦笅涓涓猙it
 *      d. 鎷夐珮SCL鐢熸垚鏃堕挓涓婂崌娌(鏁版嵁閲囨牱鐐)
 *      e. 淇濇寔鏃堕挓楂樼數骞虫弧瓒虫椂搴忚佹眰
 *   3. 鏈缁堜繚鎸丼CL浣庣數骞崇粨鏉熶紶杈
 
 */
static void I2C_SendByte(unsigned char SendByte) 
{
    unsigned char i=8;
    while(i--)                  // 寰鐜鍙戦8涓猙it
    {
        SCL_L;                  // 鎷変綆鏃堕挓寮濮嬫暟鎹鍑嗗
        I2C_delay();           // 淇濇寔浣庣數骞虫椂闂
        
        /* 璁剧疆鏁版嵁绾跨數骞 */
        if(SendByte&0x80)       // 妫娴嬫渶楂樹綅(0x80=10000000b)
            SDA_H;              // 鍙戦侀昏緫1
        else 
            SDA_L;              // 鍙戦侀昏緫0
            
        SendByte<<=1;           // 宸︾Щ鍑嗗囦笅涓涓猙it(MSB浼樺厛)
        I2C_delay();            // 淇濊瘉鏁版嵁寤虹珛鏃堕棿
        
        SCL_H;                  // 鏃堕挓涓婂崌娌(浠庢満鍦ㄦら樁娈甸噰鏍)
        I2C_delay();            // 淇濇寔楂樼數骞虫椂闂
    }
    SCL_L;                      // 鏈缁堜繚鎸佹椂閽熶綆鐢靛钩(鎬荤嚎绌洪棽)
}
 
/* 鍑芥暟鍔熻兘: 閫氳繃I2C鎬荤嚎鎺ユ敹鍗曞瓧鑺傛暟鎹(MSB浼樺厛)
 * 杩斿洖鍊  : 鎺ユ敹鍒扮殑瀹屾暣瀛楄妭鏁版嵁
 * 鎵ц屾祦绋:
 *   1. 鍒濆嬪寲鏁版嵁绾夸负楂樼數骞(SDA_H)杩涘叆杈撳叆妯″紡
 *   2. 寰鐜8娆′緷娆℃帴鏀舵瘡涓猙it(浠庢渶楂樹綅鍒版渶浣庝綅)
 *   3. 姣忎釜bit鎺ユ敹姝ラ:
 *      a. 宸︾Щ鎺ユ敹瀛楄妭鍑嗗囧瓨鍌ㄦ柊bit
 *      b. 鎷変綆SCL_L寮濮嬫暟鎹閲囨牱鍛ㄦ湡
 *      c. 鎷夐珮SCL_H鐢熸垚鏃堕挓涓婂崌娌(浠庢満鍦ㄦら樁娈佃緭鍑烘暟鎹)
 *      d. 鍦⊿CL楂樼數骞虫湡闂磋诲彇SDA鐘舵
 *      e. 鏍规嵁SDA鐢靛钩璁剧疆鎺ユ敹瀛楄妭鏈浣庝綅
 *   4. 鏈缁堜繚鎸丼CL_L浣庣數骞崇粨鏉熶紶杈
 
 */
static unsigned char I2C_RadeByte(void)  
{ 
    unsigned char i=8;
    unsigned char ReceiveByte=0;

    SDA_H;                    // 閲婃斁鏁版嵁绾(鍒囨崲涓鸿緭鍏ユā寮)
    while(i--)                 // 寰鐜鎺ユ敹8涓猙it
    {
      ReceiveByte<<=1;         // 宸︾Щ鑵惧嚭鏈浣庝綅(MSB First)
      
      SCL_L;                  // 鎷変綆鏃堕挓寮濮嬮噰鏍峰懆鏈
      I2C_delay();            // 淇濇寔浣庣數骞虫椂闂
      SCL_H;                  // 鏃堕挓涓婂崌娌(浠庢満杈撳嚭鏁版嵁绋冲畾)
      I2C_delay();            // 淇濇寔楂樼數骞虫椂闂
      
      if(SDA_read)            // 妫娴婼DA鐢靛钩鐘舵
      {
        ReceiveByte|=0x01;    // 鏈浣庝綅缃1(灏忕瀛樺偍)
      }
    }
    SCL_L;                    // 鏈缁堟媺浣庢椂閽熺粨鏉熶紶杈
    return ReceiveByte;       // 杩斿洖瀹屾暣鎺ユ敹瀛楄妭
}


/* 鍑芥暟鍔熻兘: 鍚慖2C浠庤惧囨墽琛屽崟瀛楄妭鏁版嵁鍐欏叆鎿嶄綔
 * 鍙傛暟     : 
 *   SlaveAddress - 浠庤惧囧湴鍧(7浣嶅湴鍧锛屼笉鍚璇诲啓浣)
 *   REG_Address  - 鐩鏍囧瘎瀛樺櫒鍦板潃
 *   REG_data     - 寰呭啓鍏ユ暟鎹
 * 杩斿洖鍊  : TRUE(1)鎿嶄綔鎴愬姛锛孎ALSE(0)鎿嶄綔澶辫触
 * 鎵ц屾祦绋:
 *   1. 鍙戣捣I2C璧峰嬩俊鍙
 *   2. 鍙戦佷粠璁惧囧湴鍧+鍐欐爣蹇椾綅(鍦板潃鑷鍔ㄥ乏绉1浣嶏紝鏈浣庝綅缃0)
 *   3. 鍙戦佺洰鏍囧瘎瀛樺櫒鍦板潃
 *   4. 鍙戦佸緟鍐欏叆鏁版嵁
 *   5. 姣忔″彂閫佸悗妫娴嬩粠璁惧囧簲绛
 *   6. 鍙戦佸仠姝淇″彿缁撴潫閫氫俊
 *   7. 寤舵椂5ms纭淇濆啓鍏ュ畬鎴
 
 */
unsigned char Single_Write(unsigned char SlaveAddress,
                          unsigned char REG_Address,
                          unsigned char REG_data)
{
    if(!I2C_Start())          // 鍙戣捣璧峰嬩俊鍙
    {
        return FALSE;         // 鎬荤嚎鍗犵敤妫娴嬪け璐
    }
    
    // 鍙戦佽惧囧湴鍧(鑷鍔ㄦ坊鍔犲啓鏍囧織浣)
    I2C_SendByte(SlaveAddress); 
    if(!I2C_WaitAck()){       // 妫娴嬪湴鍧ACK
        I2C_Stop(); 
        return FALSE;         // 鍦板潃鏃犲簲绛
    }
    
    I2C_SendByte(REG_Address); // 鍙戦佸瘎瀛樺櫒鍦板潃
    I2C_WaitAck();            // 鏈澶勭悊ACK澶辫触(闇鏀硅繘)
    
    I2C_SendByte(REG_data);    // 鍙戦佸啓鍏ユ暟鎹
    I2C_WaitAck();            // 鏈澶勭悊ACK澶辫触(闇鏀硅繘)
    
    I2C_Stop();               // 缁堟㈤氫俊
    delay5ms();               // 绛夊緟鏁版嵁鎸佷箙鍖(EEPROM绛夐渶瑕)
    return TRUE;              // 杩斿洖鎿嶄綔鎴愬姛鐘舵
}

/* 鍑芥暟鍔熻兘: 浠嶪2C浠庤惧囨墽琛屽崟瀛楄妭鏁版嵁璇诲彇鎿嶄綔
 * 鍙傛暟     : 
 *   SlaveAddress - 浠庤惧囧湴鍧(7浣嶅湴鍧锛屼笉鍚璇诲啓浣)
 *   REG_Address  - 瑕佽诲彇鐨勭洰鏍囧瘎瀛樺櫒鍦板潃
 * 杩斿洖鍊  : 
 *   鎴愬姛鏃惰繑鍥炶诲彇鐨勫瓧鑺傛暟鎹(0x00-0xFF)
 *   澶辫触鏃惰繑鍥濬ALSE(0x00)(涓庢暟鎹0鍐茬獊锛岄渶涓氬姟灞傚垽鏂)
 * 鎵ц屾祦绋:
 *   1. 鍙戣捣璧峰嬩俊鍙 -> 鍙戦佽惧囧湴鍧(鍐欐ā寮)
 *   2. 鍙戦佸瘎瀛樺櫒鍦板潃 -> 閲嶅嶈捣濮嬩俊鍙
 *   3. 鍙戦佽惧囧湴鍧(璇绘ā寮) -> 鎺ユ敹鏁版嵁
 *   4. 鍙戦丯ACK缁堟浼犺緭 -> 鍙戦佸仠姝淇″彿
 
 */
unsigned char Single_Read(unsigned char SlaveAddress, 
                         unsigned char REG_Address)
{   
    unsigned char REG_data;     	
    // 绗涓闃舵碉細鍐欏瘎瀛樺櫒鍦板潃
    if(!I2C_Start()) return FALSE;    // 鍚鍔ㄦ荤嚎澶辫触
    I2C_SendByte(SlaveAddress);       // 鍙戦佽惧囧湴鍧+鍐欐爣蹇
    if(!I2C_WaitAck()){               // 妫娴嬪湴鍧ACK
        I2C_Stop(); 
        return FALSE;                // 浠庢満鏃犲簲绛
    }
    I2C_SendByte(REG_Address);        // 鍙戦佺洰鏍囧瘎瀛樺櫒鍦板潃
    I2C_WaitAck();                    // 鏈澶勭悊ACK澶辫触(寤鸿鏀硅繘)
    
    // 绗浜岄樁娈碉細璇诲彇鏁版嵁
    I2C_Start();                      // 閲嶅嶈捣濮嬩俊鍙
    I2C_SendByte(SlaveAddress + 1);   // 璁惧囧湴鍧+璇绘爣蹇(LSB=1)
    I2C_WaitAck();
    
    REG_data = I2C_RadeByte();        // 璇诲彇鏁版嵁瀛楄妭
    I2C_NoAck();                      // 鍙戦丯ACK缁堟㈣诲彇
    I2C_Stop();                       // 閲婃斁鎬荤嚎
    
    return REG_data;                  // 杩斿洖璇诲彇缁撴灉
}		      


/* 鍑芥暟鍔熻兘: 鍚慖2C浠庤惧囨墽琛屽氬瓧鑺傝繛缁鍐欏叆鎿嶄綔
 * 鍙傛暟     : 
 *   SlaveAddress - 浠庤惧囧湴鍧(7浣嶅湴鍧锛屼笉鍚璇诲啓浣)
 *   REG_Address  - 璧峰嬪瘎瀛樺櫒鍦板潃
 *   REG_data     - 寰呭啓鍏ユ暟鎹缂撳啿鍖烘寚閽
 *   num          - 瑕佸啓鍏ョ殑瀛楄妭鏁
 * 杩斿洖鍊  : TRUE(1)鎿嶄綔鎴愬姛锛孎ALSE(0)鎿嶄綔澶辫触
 * 鎵ц屾祦绋:
 *   1. 鍙戣捣璧峰嬩俊鍙 -> 鍙戦佽惧囧湴鍧(鍐欐ā寮)
 *   2. 鍙戦佸瘎瀛樺櫒璧峰嬪湴鍧
 *   3. 寰鐜鍙戦佹暟鎹缂撳啿鍖哄唴瀹
 *   4. 姣忎釜瀛楄妭鍙戦佸悗妫娴婣CK
 *   5. 鍙戦佸仠姝淇″彿骞跺欢鏃5ms
 
 */
unsigned char IIC_Mult_Write(unsigned char SlaveAddress,
                            unsigned char REG_Address,
                            unsigned char *REG_data,
                            unsigned char num)
{   
    unsigned char i = 0;
    // 鍚鍔ㄤ紶杈撳簭鍒
    if(!I2C_Start()) return FALSE;     // 鎬荤嚎鍚鍔ㄦ娴
    I2C_SendByte(SlaveAddress);        // 鍙戦佽惧囧湴鍧+鍐欐爣蹇
    if(!I2C_WaitAck()){               // 妫娴嬪湴鍧ACK
        I2C_Stop(); 
        return FALSE;
    }
    I2C_SendByte(REG_Address);         // 鍙戦佸瘎瀛樺櫒璧峰嬪湴鍧
    I2C_WaitAck();                     // 鏈澶勭悊ACK澶辫触(闇鏀硅繘)

    // 寰鐜鍙戦佹暟鎹
    do{
        I2C_SendByte(REG_data[i]);     // 鍙戦佹暟鎹瀛楄妭
        I2C_WaitAck();                 // 鏈澶勭悊ACK澶辫触(鏁版嵁鍙鑳戒涪澶)
        i++; 
        num--;    
    } while (num);                     // 鐩村埌鍙戦佸畬鎸囧畾瀛楄妭鏁
    
    I2C_Stop();                        // 缁堟㈤氫俊
    delay5ms();                        // 绛夊緟瀛樺偍瀹屾垚(EEPROM绛夐渶瑕)
    return TRUE;                       // 杩斿洖鎿嶄綔鐘舵
}

/* 鍑芥暟鍔熻兘: 浠嶪2C浠庤惧囪繛缁璇诲彇澶氫釜瀛楄妭鏁版嵁
 * 鍙傛暟     : 
 *   SlaveAddress - 7浣嶄粠璁惧囧湴鍧(涓嶅惈璇诲啓浣)
 *   REG_Address  - 鐩鏍囪捣濮嬪瘎瀛樺櫒鍦板潃
 *   REG_data     - 鏁版嵁鎺ユ敹缂撳啿鍖烘寚閽
 *   num          - 瑕佽诲彇鐨勫瓧鑺傛暟(鈮1)
 * 杩斿洖鍊  : TRUE(1)鎿嶄綔鎴愬姛锛孎ALSE(0)鎿嶄綔澶辫触
 * 鎵ц屾祦绋:
 *   1. 鍐欑浉浣嶉樁娈:
 *     a. 鍙戦佽捣濮嬩俊鍙
 *     b. 鍙戦佽惧囧湴鍧+鍐欐爣蹇
 *     c. 鍙戦佺洰鏍囧瘎瀛樺櫒鍦板潃
 *   2. 璇荤浉浣嶉樁娈:
 *     a. 閲嶅嶈捣濮嬩俊鍙
 *     b. 鍙戦佽惧囧湴鍧+璇绘爣蹇
 *     c. 寰鐜鎺ユ敹鏁版嵁:
 *       - 鍓峃-1涓瀛楄妭鍙戦丄CK缁х画璇诲彇
 *       - 鏈鍚1涓瀛楄妭鍙戦丯ACK缁堟㈣诲彇
 *   3. 鍙戦佸仠姝淇″彿閲婃斁鎬荤嚎
 
 */
unsigned char IIC_Mult_Read(unsigned char SlaveAddress,
                           unsigned char REG_Address,
                           unsigned char *REG_data,
                           unsigned char num)
{   
    unsigned char i = 0;
    // 绗涓闃舵碉細鍐欏瘎瀛樺櫒鍦板潃
    if(!I2C_Start()) return FALSE;    // 璧峰嬩俊鍙峰け璐
    I2C_SendByte(SlaveAddress);       // 璁惧囧湴鍧+鍐欐爣蹇(LSB=0)
    if(!I2C_WaitAck()){               // 鍦板潃ACK妫娴
        I2C_Stop(); 
        return FALSE;
    }
    I2C_SendByte(REG_Address);        // 鍙戦佸瘎瀛樺櫒璧峰嬪湴鍧
    I2C_WaitAck();                    // 鏈澶勭悊ACK澶辫触(寤鸿鏀硅繘)

    // 绗浜岄樁娈碉細璇诲彇鏁版嵁娴
    I2C_Start();                      // 閲嶅嶈捣濮嬩俊鍙
    I2C_SendByte(SlaveAddress + 1);   // 璁惧囧湴鍧+璇绘爣蹇(LSB=1)
    I2C_WaitAck();                    // 鏈澶勭悊ACK澶辫触(寤鸿鏀硅繘)

    // 寰鐜鎺ユ敹鏁版嵁瀛楄妭
    num--;                           // 璋冩暣璁℃暟鍣(N-1涓狝CK)
    while (num--) {
        REG_data[i] = I2C_RadeByte(); // 璇诲彇瀛楄妭瀛樺叆缂撳啿鍖
        i++;
        I2C_Ack();                   // 鍙戦丄CK缁х画璇诲彇
    }
    REG_data[i] = I2C_RadeByte();     // 璇诲彇鏈鍚庝竴涓瀛楄妭
    I2C_NoAck();                     // 鍙戦丯ACK缁堟㈣诲彇
    I2C_Stop();                      // 閲婃斁鎬荤嚎

    return TRUE;                     // 杩斿洖鎿嶄綔鎴愬姛
}

/* 1ms寤舵椂鍑芥暟(杩戜技鍊硷紝鍏蜂綋鏃堕暱闇鏍规嵁涓婚戞牎鍑) */
void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)	// 澶栧眰寰鐜鎺у埗鎬诲欢鏃舵绉掓暟
	{	
		for(j=0;j<123;j++); // 鍐呭眰寰鐜瀹炵幇绾1ms寤舵椂锛11.0592MHz鍏稿瀷鍊硷級
	}
}			   

/* OLED I2C鎿嶄綔鐘舵佹爣蹇 
 * 0:澶辫触 1:鎴愬姛 (鐢盨ingle_Write杩斿洖鍊兼洿鏂) */
unsigned char OLED_IIC_stu = 0;

/* OLED鍗曞瓧鑺傚啓鍏ュ嚱鏁 
 * dat : 瑕佸啓鍏ョ殑鏁版嵁/鍛戒护
 * cmd : 鏁版嵁绫诲瀷閫夋嫨 
 *       1=鏁版嵁妯″紡(0X40) 
 *       0=鍛戒护妯″紡(0X00) */
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd) // 鏁版嵁鍐欏叆妯″紡
	{
        OLED_IIC_stu = Single_Write(0x78,0X40,dat); // 0x78涓篛LED鍣ㄤ欢鍦板潃
	}
	else   // 鍛戒护鍐欏叆妯″紡
	{
        OLED_IIC_stu = Single_Write(0x78,0X00,dat); // 0x00涓哄懡浠ゅ瘎瀛樺櫒鍦板潃
	}
}

/* 鍏ㄥ睆濉鍏呭嚱鏁 
 * fill_Data : 濉鍏呮ā寮(0x00鍏ㄧ伃/0xFF鍏ㄤ寒/0xAA妫嬬洏鏍肩瓑) */
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)	// 閬嶅巻8涓椤碉紙OLED姣忛〉瀵瑰簲8琛岋級
	{
		OLED_WR_Byte(0xb0+m,0);	// 璁剧疆椤靛湴鍧锛0xB0~0xB7
		OLED_WR_Byte(0x00,0);	// 鍒楀湴鍧浣4浣嶏細0x00
		OLED_WR_Byte(0x10,0);	// 鍒楀湴鍧楂4浣嶏細0x10锛堢粍鍚堝悗浠0鍒楀紑濮嬶級
		for(n=0;n<128;n++)     // 閬嶅巻128鍒
        {
            OLED_WR_Byte(fill_Data,1); // 鎸佺画鍐欏叆濉鍏呮暟鎹
        }
	}
}

/* 鍧愭爣瀹氫綅鍑芥暟 
 * x : 鍒楀湴鍧(0-127) 
 * y : 椤靛湴鍧(0-7) 
 * 娉ㄦ剰锛歄LED绾靛悜浠ラ〉涓哄崟浣嶏紝姣忛〉8涓鍍忕礌琛 */
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);          // 璁剧疆椤靛湴鍧锛0xB0+y锛
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD); // 鍒楀湴鍧楂4浣嶏紙0x10|楂樺崐瀛楄妭锛
	OLED_WR_Byte((x&0x0f),OLED_CMD);        // 鍒楀湴鍧浣4浣嶏紙0x00|浣庡崐瀛楄妭锛
}    
	  
/////
//寮鍚疧LED鏄剧ず    
void oled_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC鍛戒护
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//鍏抽棴OLED鏄剧ず     
void oled_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC鍛戒护
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	
	   			 
//娓呭睆鍑芥暟,娓呭畬灞,鏁翠釜灞忓箷鏄榛戣壊鐨!鍜屾病鐐逛寒涓鏍!!!	  
void oled_Clear(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //璁剧疆椤靛湴鍧锛0~7锛
		OLED_WR_Byte (0x00,OLED_CMD);      //璁剧疆鏄剧ず浣嶇疆鈥斿垪浣庡湴鍧
		OLED_WR_Byte (0x10,OLED_CMD);      //璁剧疆鏄剧ず浣嶇疆鈥斿垪楂樺湴鍧   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //鏇存柊鏄剧ず
}

void oled_On(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //璁剧疆椤靛湴鍧锛0~7锛
		OLED_WR_Byte (0x00,OLED_CMD);      //璁剧疆鏄剧ず浣嶇疆鈥斿垪浣庡湴鍧
		OLED_WR_Byte (0x10,OLED_CMD);      //璁剧疆鏄剧ず浣嶇疆鈥斿垪楂樺湴鍧   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //鏇存柊鏄剧ず
}

/* 鍑芥暟鍔熻兘: 鍦∣LED鎸囧畾浣嶇疆鏄剧ず鍗曚釜瀛楃
 * 鍙傛暟璇存槑:
 *   x,y       : 鏄剧ず璧峰嬪潗鏍(x:0-127鍒, y:0-7椤)
 *   chr       : 瑕佹樉绀虹殑ASCII瀛楃(瀹為檯鏀鎸佽寖鍥磋嗗瓧搴撹屽畾)
 *   Char_Size : 瀛椾綋澶у皬閫夋嫨(16=16x8鐐归樀, 鍏朵粬鍊=6x8鐐归樀)
 * 瀹炵幇缁嗚妭:
 *   - 瀛楃︽樉绀哄熀浜庨勫瓨瀛楁ā鏁版嵁(F8X16/F6x8鏁扮粍)
 *   - 16px瀛椾綋鍒嗕袱娆″啓鍏ワ紝涓婂崐椤靛拰涓嬪崐椤靛悇8琛屾暟鎹
 *   - 鑷鍔ㄦ崲琛屽勭悊: x瓒呯晫鏃跺嶄綅鍒0鍒楀苟涓嬬Щ2椤(16px瀛椾綋閫傜敤)
 * 娉ㄦ剰浜嬮」:
 *   - 瀛楀簱鏁扮粍闇鍖呭惈浠庣┖鏍煎瓧绗(ASCII 32)寮濮嬬殑瀛楀舰鏁版嵁
 *   - 椤靛湴鍧y鍙傛暟瀹為檯瀵瑰簲OLED鐨8鍍忕礌琛屽尯鍧 */
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size)
{      	
	unsigned char c=0,i=0;	
	c = chr - ' ';  // 璁＄畻瀛楀簱鍋忕Щ閲(绌烘牸瀛楃︿负瀛楀簱璧风偣)
	
	/* 鍧愭爣瓒呯晫澶勭悊(16px瀛椾綋鑷鍔ㄦ崲琛) */
	if(x > Max_Column-1){
		x = 0; 
		y = y + 2;  // 16px瀛椾綋鍗2椤甸珮搴︼紝鎹㈣屾椂澧炲姞2椤
	}
	
	if(Char_Size ==16)  // 16x8鐐归樀瀛椾綋澶勭悊
	{
		/* 鍐欏叆涓婂崐椤垫暟鎹(鍓8琛) */
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i], OLED_DATA); // 绱㈠紩璁＄畻: 姣忎釜瀛楃16瀛楄妭
		
		/* 鍐欏叆涓嬪崐椤垫暟鎹(鍚8琛) */
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i+8], OLED_DATA);
	}
	else  // 6x8鐐归樀瀛椾綋澶勭悊
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA); // 姣忎釜瀛楃6瀛楄妭
	}
}

/* 鍑芥暟鍔熻兘: 绠鏄撴暣鏁板箓璁＄畻(m鐨刵娆℃柟)
 * 鍙傛暟璇存槑:
 *   m : 搴曟暟(寤鸿鑼冨洿0-65535)
 *   n : 鎸囨暟(寤鸿鑼冨洿0-16, 闃叉㈢粨鏋滄孩鍑)
 * 杩斿洖鍊  : m^n璁＄畻缁撴灉(鏃犵﹀彿鏁村瀷)
 * 娉ㄦ剰浜嬮」:
 *   - 浠呴傜敤浜庨潪璐熸暣鏁拌繍绠
 *   - 鏃犳孩鍑轰繚鎶わ紝闇纭淇濈粨鏋滃湪unsigned int鑼冨洿鍐
 *   - 鍏稿瀷搴旂敤鍦烘櫙: OLED鏄剧ず鏁板兼椂鐨勪綅鏉冭＄畻 */
unsigned int oled_pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--) result *= m; // 閫氳繃绱涔樺疄鐜板箓杩愮畻
	return result;
}
		  
/* 鍑芥暟鍔熻兘: 澶氫綅鏁板瓧鏄剧ず(鏀鎸佽嚜鍔ㄥ幓鍓嶅奸浂)
 * 鍙傛暟璇存槑:
 *   x,y    : 璧峰嬪潗鏍(x:鍒,y:椤)
 *   num    : 寰呮樉绀烘暟鍊(鏀鎸0~4294967295)
 *   len    : 鏄剧ず浣嶆暟(鍥哄畾浣嶆暟鏄剧ず)
 *   size2  : 瀛椾綋澶у皬(16/8 瀵瑰簲涓嶅悓瀛楀簱)
 * 瀹炵幇鐗规:
 *   - 鑷鍔ㄨ烦杩囧墠瀵奸浂鏄剧ず(淇濈暀鏈鍚庝竴浣嶉浂)
 *   - 鏁板瓧闂磋窛鑷鍔ㄨ＄畻(size2/2)
 *   - 鏀鎸佽秴杩囨樉绀轰綅鏁版椂鎴鏂楂樹綅 */
void oled_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2)
{         	
	unsigned char t,temp;
	unsigned char enshow=0; // 鍓嶅奸浂鏍囧織(0:鏈閬囧埌鏈夋晥鏁板瓧)					   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10; // 鎻愬彇鎸囧畾浣嶇殑鏁板瓧
		/* 鍓嶅奸浂澶勭悊閫昏緫 */
		if(enshow==0&&t<(len-1)) // 鍏佽告渶鍚庝竴浣嶆樉绀洪浂
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2); // 鐢ㄧ┖鏍兼浛浠ｅ墠瀵奸浂
				continue;
			}else enshow=1; // 閬囧埌闈為浂鏁板瓧鍚庡紑濮嬫樉绀 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); // 鏁板瓧杞珹SCII鐮佹樉绀
	}
} 

/* 鍑芥暟鍔熻兘: 瀛楃︿覆鏄剧ず(鏀鎸佽嚜鍔ㄦ崲琛)
 * 鍙傛暟璇存槑:
 *   chr        : 瀛楃︿覆鎸囬拡(闇浠'\0'缁撳熬)
 *   Char_Size  : 瀛椾綋澶у皬(16/8)
 * 鏄剧ず鐗规:
 *   - 瀛楃﹂棿璺濆浐瀹8鍍忕礌(閫傜敤浜8px瀹藉瓧浣)
 *   - x>120鏃舵崲琛屽埌涓嬩竴琛岄栧垪
 *   - 鎹㈣屾椂y澧炲姞2椤(閫傞厤16px楂樺害瀛椾綋) */
void oled_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8; // 姝ヨ繘8鍒(鏍囧噯ASCII瀛楃﹀藉害)
		/* 杈圭晫鎹㈣屽勭悊 */
		if(x>120){x=0;y+=2;} // 棰勭暀8鍒椾綑閲忛槻姝㈡孩鍑
		j++;
	}
}

/* 鍑芥暟鍔熻兘: 16x16鐐归樀姹夊瓧鏄剧ず(鍙岄〉妯″紡)  
 * 鍙傛暟璇存槑:  
 *   x : 璧峰嬪垪鍧愭爣(0-127)  
 *   y : 璧峰嬮〉鍧愭爣(0-6) 姣忎釜姹夊瓧鍗2椤甸珮搴  
 *   no: 姹夊瓧鍦ㄥ瓧搴撲腑鐨勭储寮曞彿(0~n)  
 * 瀛楀簱瑕佹眰:  
 *   Hzk[][16]浜岀淮鏁扮粍锛屾瘡涓姹夊瓧鍗32瀛楄妭锛  
 *   - [2*no]   涓烘眽瀛椾笂鍗婇儴鍒嗘暟鎹(鍓16瀛楄妭)  
 *   - [2*no+1] 涓烘眽瀛椾笅鍗婇儴鍒嗘暟鎹(鍚16瀛楄妭)  
 * 鎵ц屾祦绋:  
 *   1. 瀹氫綅鍒版寚瀹氶〉鐨勮捣濮嬪垪  
 *   2. 鍐欏叆鍓16瀛楄妭鍒颁笂鍗婇〉(y)  
 *   3. 瀹氫綅鍒颁笅涓椤(y+1)  
 *   4. 鍐欏叆鍚16瀛楄妭鍒颁笅鍗婇〉  
 
 *     */  
void oled_ShowCHinese(unsigned char x,unsigned char y,unsigned char no)  
{                    
    unsigned char t,adder=0;  // adder涓烘棤鏁堝彉閲(鍘嗗彶閬楃暀)
    
    /* 鍐欏叆姹夊瓧涓婂崐閮ㄥ垎(8鍍忕礌琛) */  
    OLED_Set_Pos(x,y);        // 璁剧疆璧峰嬩綅缃(鍒梮,椤祔)  
    for(t=0;t<16;t++)         // 閬嶅巻16鍒楁暟鎹  
    {  
        OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);  // 鍙戦佸瓧搴撲笂鍗婇儴鏁版嵁  
        adder+=1;             // 鏃犳晥鎿嶄綔(鍙鍒犻櫎)  
    }      
    
    /* 鍐欏叆姹夊瓧涓嬪崐閮ㄥ垎(8鍍忕礌琛) */  
    OLED_Set_Pos(x,y+1);      // 璺宠浆鍒颁笅涓椤(y+1)  
    for(t=0;t<16;t++)         // 閬嶅巻16鍒楁暟鎹  
    {      
        OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA); // 鍙戦佸瓧搴撲笅鍗婇儴鏁版嵁  
        adder+=1;             // 鏃犳晥鎿嶄綔(鍙鍒犻櫎)  
    }                      
}  

/* 鍑芥暟鍔熻兘: 鏄剧ず鍗佽繘鍒舵暣鍨嬫暟鍊(甯︾畝鍗曟牸寮忓寲)
 * 鍙傛暟璇存槑:
 *   x,y    : 鏄剧ず璧峰嬪潗鏍
 *   Dnum   : 瑕佹樉绀虹殑鏁村瀷鏁板(-999~9999)
 *   size1  : 瀛椾綋澶у皬(8/16)
 * 瀹炵幇鐗规:
 *   - 浣跨敤%3d鏍煎紡淇濊瘉鑷冲皯鏄剧ず3浣嶆暟瀛(涓嶈冻琛ョ┖鏍)
 *   - Data[5]=0寮哄埗鎴鏂涓5瀛楃(鍚缁撴潫绗)
 
 *    */
void OLED_ShowDNum(unsigned char x,unsigned char y,int Dnum,unsigned char size1)
{
    uint8_t Data[20]= " ";      // 鏍煎紡鍖栫紦鍐插尯(瀹為檯鏈夋晥闀垮害琚闄愬埗涓4瀛楃+缁撴潫绗)
    sprintf((char*)Data,"%3d",Dnum);  // 鏍煎紡鍖栦负鑷冲皯3浣嶅崄杩涘埗鏁
    Data[5] = 0;                // 寮哄埗缁堟㈠瓧绗︿覆(闃叉㈤暱鏁板兼孩鍑)
    oled_ShowString(x,y,Data,size1); 
}

/* 鍑芥暟鍔熻兘: 鏄剧ず娴鐐规暟鍊(淇濈暀1浣嶅皬鏁)
 * 鍙傛暟璇存槑:
 *   Fnum   : 瑕佹樉绀虹殑娴鐐规暟鍊(-999.9~9999.9)
 * 瀹炵幇鐗规:
 *   - 浣跨敤%.1f鏍煎紡淇濈暀1浣嶅皬鏁
 *   - Data[5]=0寮哄埗鎴鏂涓5瀛楃(鍚缁撴潫绗)
 
 *  */
void OLED_ShowFNum(unsigned char x,unsigned char y,float Fnum,unsigned char size1)
{
    uint8_t Data[20]= " ";      // 鏍煎紡鍖栫紦鍐插尯(瀹為檯鏈夋晥闀垮害琚闄愬埗涓5瀛楃+缁撴潫绗)
    sprintf((char*)Data,"%.1f",Fnum); // 淇濈暀1浣嶅皬鏁版牸寮忓寲
    Data[4] = 0;                // 寮哄埗缁堟㈠瓧绗︿覆(绀轰緥"123.4"鍗5瀛楄妭)
    oled_ShowString(x,y,Data,size1);
}

/* 鍑芥暟鍔熻兘: OLED浣嶅浘缁樺埗(瀛樺湪娼滃湪鍧愭爣璁＄畻闂棰)
 * 鍙傛暟璇存槑:
 *   x0,y0 : 璧峰嬪垪鍧愭爣(0-127)鍜岃捣濮嬮〉鍧愭爣(0-7)
 *   x1,y1 : 缁撴潫鍒楀潗鏍(搴斿ぇ浜巟0)鍜岀粨鏉熷儚绱犺屽潗鏍(0-63)
 *   BMP[] : 浣嶅浘鏁版嵁鏁扮粍(鎸夌旱鍚戝垎椤靛瓨鍌锛屾瘡椤靛寘鍚玿1-x0涓鍒楁暟鎹)
 * 瀹炵幇娴佺▼:
 *   1. 璁＄畻缁撴潫鍍忕礌琛屽瑰簲鐨勬婚〉鏁帮細y = y1/8 + 浣欐暟澶勭悊
 *   2. 绾靛悜閫愰〉閬嶅巻(y0鍒拌＄畻寰楀埌鐨剏鍊)
 *   3. 姣忛〉鍐呮í鍚戦愬垪鍐欏叆鏁版嵁(x0鍒皒1)
 *   4. 鏁版嵁鎸夊垪椤哄簭杩炵画瀛樺偍锛屾瘡鍒1瀛楄妭(8鍍忕礌琛)
 
 *    */
void oled_DrawBMP(unsigned char x0, unsigned char y0,
                 unsigned char x1, unsigned char y1,
                 unsigned char BMP[])
{ 	
	unsigned int j=0;          // 鏁版嵁缂撳啿鍖虹储寮
	unsigned char x,y;         // 鍒/椤靛惊鐜鍙橀噺
	
	/* 椤垫暟璁＄畻 */
	if(y1%8==0) y=y1/8;       // 瀹岀編瀵归綈鏃剁洿鎺ラ櫎
	else y=y1/8+1;            // 鏈瀵归綈鏃跺炲姞1椤
	
	/* 绾靛悜椤甸亶鍘  */
	for(y=y0;y<y1;y++)        // y0浣滀负璧峰嬮〉锛寉1浣滀负缁撴潫椤
	{
		OLED_Set_Pos(x0,y);  // 璁剧疆褰撳墠椤佃捣濮嬪垪
		/* 妯鍚戝垪閬嶅巻 */
		for(x=x0;x<x1;x++)
		{      
			OLED_WR_Byte(BMP[j++],OLED_DATA); // 鍐欏叆鍒楁暟鎹
		}
	}
}
//= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =  //
/* 鍑芥暟鍔熻兘: SSD1306 OLED鏄剧ず灞忓垵濮嬪寲
 * 鎵ц屾祦绋:
 *   1. GPIO鍒濆嬪寲: 閰嶇疆PA11(SDA),PA12(SCL)涓烘帹鎸借緭鍑
 *   2. 鍙戦佸垵濮嬪寲鍛戒护搴忓垪閰嶇疆鏄剧ず鍙傛暟
 *   3. 娓呭睆骞跺紑鍚鏄剧ず
 
 *    */
void oled_Init(void)
{ 		
    /* GPIO鍒濆嬪寲闃舵 */
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 浣胯兘GPIOA鏃堕挓
 
    /* SCL(PA12)閰嶇疆 */
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // 鎺ㄦ尳杈撳嚭妯″紡
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // 楂橀熸ā寮
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;            // SCL寮曡剼
 	GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* SDA(PA11)閰嶇疆 */  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            // SDA寮曡剼
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 鎬荤嚎鍒濆嬬姸鎬佺疆楂 */
	GPIO_SetBits(GPIOA, GPIO_Pin_12);  // SCL楂樼數骞
 	GPIO_SetBits(GPIOA, GPIO_Pin_11);  // SDA楂樼數骞

    /* 璁惧囧垵濮嬪寲闃舵 */
    while (!Single_Write(0x78,0X00,0xae)); // 鎸佺画灏濊瘯鍏抽棴鏄剧ず鐩村埌鎴愬姛
    Delay_1ms(400);                       // 鍏抽敭寤舵椂纭淇濈‖浠剁ǔ瀹

    /* SSD1306閰嶇疆鍛戒护搴忓垪 */
	OLED_WR_Byte(0xAE,OLED_CMD); // 鍏抽棴鏄剧ず
	OLED_WR_Byte(0x00,OLED_CMD); // 璁剧疆鍒楀湴鍧浣4浣嶈捣濮嬩负0
	OLED_WR_Byte(0x10,OLED_CMD); // 璁剧疆鍒楀湴鍧楂4浣嶈捣濮嬩负0
	OLED_WR_Byte(0x40,OLED_CMD); // 璁剧疆鏄剧ず璧峰嬭屽湴鍧(琛0)
	OLED_WR_Byte(0xB0,OLED_CMD); // 璁剧疆椤靛湴鍧璧峰嬮〉(椤0)
	OLED_WR_Byte(0x81,OLED_CMD); // 瀵规瘮搴︽帶鍒舵ā寮
	OLED_WR_Byte(0xFF,OLED_CMD); // 瀵规瘮搴︽渶澶у255
	OLED_WR_Byte(0xA1,OLED_CMD); // 娈甸噸鏄犲皠(姘村钩缈昏浆)
	OLED_WR_Byte(0xA6,OLED_CMD); // 姝ｅ父鏄剧ず妯″紡(闈炲弽鑹)
	OLED_WR_Byte(0xA8,OLED_CMD); // 璁剧疆澶嶇敤姣旂巼
	OLED_WR_Byte(0x3F,OLED_CMD); // 1/64鍗犵┖姣(閫傜敤浜128x64灞)
	OLED_WR_Byte(0xC8,OLED_CMD); // COM杈撳嚭鎵鎻忔柟鍚(閫嗗悜)
	OLED_WR_Byte(0xD3,OLED_CMD); // 鏄剧ず鍋忕Щ璁剧疆
	OLED_WR_Byte(0x00,OLED_CMD); // 鏃犲亸绉
	OLED_WR_Byte(0xD5,OLED_CMD); // 鏄剧ず鏃堕挓鍒嗛
	OLED_WR_Byte(0x80,OLED_CMD); // 寤鸿榛樿ゅ(鍒嗛戞瘮1)
	OLED_WR_Byte(0xD8,OLED_CMD); // 鍖哄煙棰滆壊妯″紡
	OLED_WR_Byte(0x05,OLED_CMD); // 绂佺敤鍖哄煙棰滆壊妯″紡
	OLED_WR_Byte(0xD9,OLED_CMD); // 棰勫厖鐢靛懆鏈熻剧疆
	OLED_WR_Byte(0xF1,OLED_CMD); // Phase1=15 DCLK, Phase2=1 DCLK
	OLED_WR_Byte(0xDA,OLED_CMD); // COM寮曡剼纭浠堕厤缃
	OLED_WR_Byte(0x12,OLED_CMD); // 椤哄簭COM寮曡剼锛岀佺敤宸﹀彸COM鍋忕Щ
	OLED_WR_Byte(0xDB,OLED_CMD); // VCOMH鐢靛帇绛夌骇
	OLED_WR_Byte(0x30,OLED_CMD); // 绾0.83*VCC
	OLED_WR_Byte(0x8D,OLED_CMD); // 鐢佃嵎娉佃剧疆
	OLED_WR_Byte(0x14,OLED_CMD); // 浣胯兘鐢佃嵎娉(蹇呴』寮鍚)
	OLED_WR_Byte(0xAF,OLED_CMD); // 寮鍚鏄剧ず

	oled_Clear();  // 娓呭睆鍒濆嬪寲鏄剧ず缂撳瓨
}


/* 鍑芥暟鍔熻兘: 鏄剧ず棰滆壊鍙嶇浉鎺у埗
 * 鍙傛暟璇存槑: 
 *   i - 鏄剧ず妯″紡閫夋嫨
 *       0: 姝ｅ父鏄剧ず(榛戝簳鐧藉瓧)
 *       1: 鍙嶈壊鏄剧ず(鐧藉簳榛戝瓧)
 * 瀹炵幇鍘熺悊:
 *   閫氳繃鍙戦丼SD1306鐨凙6/A7鍛戒护鍒囨崲鍏ㄥ眬棰滆壊鏄犲皠
 
 *    */
void oled_ColorTurn(uint8_t i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xA6,OLED_CMD); // 0xA6: 姝ｅ父鍍忕礌妯″紡(GDDRAM鏁版嵁1琛ㄧず浜)
    }
    if(i==1)
    {
        OLED_WR_Byte(0xA7,OLED_CMD); // 0xA7: 鍙嶈壊鍍忕礌妯″紡(GDDRAM鏁版嵁0琛ㄧず浜)
    }
}

/* 鍑芥暟鍔熻兘: 灞忓箷鏄剧ず鏂瑰悜鏃嬭浆鎺у埗
 * 鍙傛暟璇存槑:
 *   i - 鏃嬭浆妯″紡閫夋嫨
 *       0: 姝ｅ父鏂瑰悜鏄剧ず
 *       1: 180搴︽棆杞鏄剧ず
 * 瀹炵幇鍘熺悊:
 *   缁勫悎浣跨敤娈甸噸鏄犲皠(A0/A1)鍜孋OM鎵鎻忔柟鍚(C0/C8)鍛戒护瀹炵幇纭浠剁骇鏃嬭浆
 * 纭浠堕厤缃:
 *   - 0xC8: COM杈撳嚭閫嗗悜鎵鎻(浠嶤OM63鍒癈OM0)
 *   - 0xA1: 娈甸噸鏄犲皠(鍒楀湴鍧127鏄犲皠鍒癝EG0)
 *   - 0xC0: COM杈撳嚭姝ｅ父鎵鎻(浠嶤OM0鍒癈OM63)
 *   - 0xA0: 娈垫ｅ父鏄犲皠(鍒楀湴鍧0鏄犲皠鍒癝EG0)
 
 *   */
void oled_DisplayTurn(uint8_t i)
{
    if(i==0)  // 鏍囧噯鏂瑰悜
    {
        OLED_WR_Byte(0xC8,OLED_CMD); // COM閫嗗悜鎵鎻(閰嶅悎鐗╃悊甯冨眬)
        OLED_WR_Byte(0xA1,OLED_CMD); // 鍒楀湴鍧鍙嶅悜(SEG127->SEG0)
    }
    if(i==1)  // 鏃嬭浆180搴
    {
        OLED_WR_Byte(0xC0,OLED_CMD); // COM姝ｅ悜鎵鎻
        OLED_WR_Byte(0xA0,OLED_CMD); // 鍒楀湴鍧姝ｅ父鏄犲皠
    }
}







