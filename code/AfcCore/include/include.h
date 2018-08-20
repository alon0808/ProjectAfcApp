
#ifndef __include_head_
#define  __include_head_
extern unsigned char DEBUG_COM; //COM4
//在库函数中定义,不用红外时可以做缓存使用
#define  MaxPackLen    	1024
extern unsigned char irda_rxBuf[MaxPackLen];
extern volatile unsigned char PVDflag;
//电压标志   1电压过低   2电压正常
#define PWR_LOW 1
#define PWR_HIG 2
//初始化CPU各端口
extern void port_init(void);
//取GPRS程序版本号,在应用程序最后64字节,版本号返回前14字节
extern void getGpsProVER(unsigned char *ver);
//取库函数版本
extern int getlibVer(void);
//mode = 0 出厂序列号
//mode = 1 程序标识
extern void getposSN(unsigned char *possn, unsigned char mode);
//读CPU序列号，只返回最低8位 压缩BCD码
extern void Get_SerialNum(unsigned char *ser);
//读F4机器CPU序列号，只返回最低8位 压缩BCD码
extern void Get_SerialNumF4(unsigned char *ser);
/*****************************************
* 功能: 延时
* 输入: 延时的时长（用软件延时,1大约=1ms）
* 输出: NULL
******************************************/
extern void Delay(unsigned int ticks);
extern void delay ( void );
/*      LED        */					  
#define LED_RED        ((unsigned char)1)	//红色
#define LED_GREEN      ((unsigned char)2)	//蓝色
#define LED_BLUE       ((unsigned char)4)	//橙色
#define LED_YELLOW     ((unsigned char)8)	//黄色
#define ALL_LED			(LED_RED|LED_GREEN|LED_BLUE|LED_YELLOW)
/*****************************************
* 功能: led口初始化
* 输入: led编号 = LED_RED/LED_GREEN
* 输出: NULL
******************************************/
extern void led_init(void);
/*****************************************
* 功能: led亮
* 输入: led编号 = LED_RED/LED_GREEN
* 输出: NULL
******************************************/
extern void led_on(unsigned char type);
/*****************************************
* 功能: led灭
* 输入: led编号 = LED_RED/LED_GREEN
* 输出: NULL
******************************************/
extern void led_off(unsigned char type);
/*****************************************
* 功能: led取反
* 输入: led编号 = LED_RED/LED_GREEN
* 输出: NULL
******************************************/
extern void led_convert(unsigned char type);
/*      LED        */
/******************************************/
/*      BEEP       */
/*******************************
 *	功能：蜂鸣器控制
 *	输入：
 *	  ucTimes:响的次数
 *	  ulBeepms:响延时
 *	  ulNoms:不响延时
 *	输出：无
 ********************************/
extern void beepinit(void);
extern void beep(unsigned char ucTimes, unsigned int ulBeepms, unsigned int ulNoms);
/*                KEY              */
#define KEY_1 (1<<0)
#define KEY_2 (1<<1)
#define KEY_3 (1<<2)
#define KEY_4 (1<<3)
#define KEY_5 (1<<4)
#define KEY_6 (1<<5)

extern unsigned char getkey(unsigned char beepE) ;
extern void key_init(void);

/**************************************/
/****************************************
*功能: 语音端口初始化
*输入: NULL
*输出: NULL
*****************************************/
extern void audioinit_old(void);
extern void audioinit(void);
extern void audio_reset(void);
/****************************************
*功能: 语音输出
*输入: x-输出的段号 1-16
*输出: NULL
*****************************************/
extern void audio_old(unsigned char x);
extern void audio(unsigned char x);
// LCD
#define DIS_CONVERT	(0x1)
#define DIS_CENTER	(0x1 << 1)
#define DIS_RIGHT	(0x1 << 2)
#define DIS_ClsLine	(0x1 << 3)
#define DIS_Cls		(0x1 << 4)
//LCD 显示初始化
extern void disinit(void);
/***************************************
*功能: 背光开关函数
*输入: flag 1:开 0:关
*输出: NULL
***************************************/
extern void dislight(unsigned char flag);
/***************************************
*功能: 显示函数
*输入: x-行 y-列 str-显示的STRING mode-是否反显
*输出: NULL
***************************************/
extern void display(unsigned char x, unsigned char y, const char *str, unsigned char mode);
//二维码大屏可显示16行，每行40个半角字符,
extern void miniDispstr(unsigned char x, unsigned char y, const char *lcd_string, unsigned char mode);
//0<=antNum <=6 antNum=0xff,不显示
extern void antDispstr(unsigned char x, unsigned char y, unsigned char antNum, unsigned char mode);

//32(宽)*32(高)点阵汉字,显示的点阵范围： 224*128
//x: 0--15, 共显示4行（一般取值0，4，8，12）		
//y: 0--27, 一行显示14个3216 ascii码(3232汉字7个)
/***************************************
*功能: 显示函数
*输入: x-行 y-列 str-显示的字符串 mode-是否反显,居中或靠右显示
*输出: NULL
***************************************/
extern void display_3232(unsigned char x, unsigned char y, const char *str, unsigned char mode);
/*********************************
*功能: 清显示屏
*输入: NULL
*输出: NULL
************************************/
extern void cls(void);



//显示的ASCII码字体大小
//#define Dis_Font16		16		//1608
#define Dis_Font32		32		//3216
#define Dis_Font48		48		//4824
#define Dis_Font64		64		//6432

/***************************************
*功能: 显示（数字+小数点）字符串函数
*输入: x   : 行, 0--15 
*      y    :列, 0--29 
*      str  : 显示的字符串 
*      mode : 是否反显,居中或靠右显示， 
*      size_font: 32/48/64
*输出: NULL
***************************************/
extern void display_AsciiDigit(unsigned char x, unsigned char y, const char *str, unsigned char mode, unsigned char size_font);

extern void displayBCD_fone(unsigned char x, unsigned char y, unsigned char *bcd, unsigned char len, unsigned char size_fone);


//显示字符，除小数点，最多显示三个字符，如果每个数字都加小数点为6个字符
extern void LED_Dis3(char *str);
//初始化数码管端口
extern void LED_3INT(void);
/*************************************
*功能: 软件复位
*输入: NULL
*输出: NULL
**************************************/
extern void ReStartCPU(void);
//清看门狗
extern void clr_wdt(void);
/********************************************
* 功能：IIC初始化
****************************************************/
extern void I2C_EE_Init(void);

//GPRS开关机脚，使管脚为高电平
extern void gprs_pwon_hi(void);
//extern void gprs_power_open(void);
//关闭，使管脚为低电平
extern void gprs_pwon_lo(void);
//extern void gprs_power_close(void);
//开启MC55i模块，延迟最少200ms
extern void gprs_power_switch(void);
//开启4.0电源控制
extern void gprs_4V_ON(void);
//关闭4.0电源控制
extern void gprs_4V_OFF(void);
//GPRS的串口TXD,RXD设为GPIO模式，低电平输出
extern void com2port(void);
/*             SPI                   */
/****************************************************SPI*/
/***************************************************
* 功能：SPI初始化程序
* 输入：NULL
* 输出：NULL
****************************************************/
//控制语音功放 1控制线高  0控制线低
extern void XFSoundPower(unsigned char ctrl);
//拉低语音模块复位线,模块将不工作,同时把语音功放电源也断开。
extern void XF_Reset_LOW(void);
//读取语音模块状态线，1忙，0闲
extern unsigned char XF_GetBusy(void);
extern void spiInit(void);
extern void XFcmdsend(unsigned char cmd, unsigned int uiLen, char *str);
#define voice2busIn 1	//输出到车内喇叭(默认)
#define voice2busOut 2	//输出到车外喇叭
#define voice2board 3	//输出到主板放大
extern void voice_switch(unsigned char mode);
extern void voice_init(void);
/********************************************
* flash采用2片2M byte的空间,第一片地址从0-1FFFFFH,第二片从200000H-3FFFFFH
* 功能：SPI Flash 读函数, 此写入函数据是AAI写入方式,写入的长度必需是2的倍数
* 输入：addr－地址; length-长度; writebuf-数据指针
* 输出：NULL
****************************************************/
extern void flashwrite(unsigned int addr, unsigned char *writebuf, unsigned int length);
extern void flashwritefont(unsigned int addr, unsigned char *writebuf, unsigned int length);
/********************************************
* flash采用2片2M byte的空间,第一片地址从0-1FFFFFH,第二片从200000H-3FFFFFH
* 功能：SPI Flash 读函数
* 输入：addr－地址; length-长度; rec_data-数据指针
* 输出：NULL
****************************************************/
extern int flashread(unsigned int addr,unsigned char *rec_data,unsigned int length);
// read status register
extern void flashRDSR(unsigned char chips, unsigned char *rec_data);
extern void flashReadID(unsigned char chips, unsigned char *rec_data);
extern char get_back_para(unsigned char *bpara);
#define BACK_HEAD	0	//4 备份结构头 00 FF 55 AA
#define BACK_DEVICE 4	//8 设备号备份
#define BACK_LINENO 12	//3 线路号备份
#define BACK_CURRENT_POINT 15 //4 当前指针备份
#define BACK_HEAD_POINT 19 //4 头指针备份
#define BACK_BCC_CHECK 23	//1 备份校验 共24字节
#define BACK_MAX_REC 2500	//最大存2500条
#define BACK_NUM 24//一条的长度
// extern int gBack_point;
// extern char save_back_para(unsigned char* device, unsigned char*line, unsigned char *cuP, unsigned char *hp)
// extern char init_back_para(void)

//只存贮当前车载机数据，每次都从0开始，采集完后全区擦除，并移到历史区
//GPRS则依次向下存贮，前面已经发送的写成0，并移到历史区。
//前一个字节写入使用标识，如果此标识不正确，则认为是第一次使用，全擦除。
#define FLASH1_START 0
#define FLASH2_START1 0x200000
#define FLASH2_START2END 0x300000
#define FLASH2_BAKR_LEN 0x100000

//记录到这说明可以清空当前数据区了 0x80000 可存8192条
#define FLASH_RCLR 0x80000
//记录到这条说明记录满了 和清除之间有7616条空间
#define FLASH_MAX  0xF7000
// 1024K空间存贮当前数据，可存16384条
#define	LEN_EEPROM 0x100000
// 最多存贮的条数 在Get_Record_point 中使用，
#define FLASH_REC_MAX 16000

#define FLASH_M_REC_START 0x100000//移动记录存贮区 共用256K空间 到0x140000
#define FLASH_M_REC_END 0x140000//结束区
#define LEN_M_REC 0x40000//总数据区大小256K

#define FLASH_FD_Ticket_START 0x150000 //分段收费32K，可存120*120个站点的票价。前16字节是头固定为“此线路信息有效”
#define FLASH_FD_Ticket_END 0x158000 //到170000没有使用

#define FLASH_FD_Ticket_TEMP_START 0x158000//暂存分段票价
#define FLASH_FD_Ticket_TEMP_END   0X160000

#define FLASH_backRec_Pointer 0x170000	//每天的历史指针区,每天4K * (31+1)天 ＝ 128K  在PROLIB 中也有这定义,不能更改
#define FLASH_HisAddr_END 0x190000

#define FLASH_TXQRC_START	0x1A0000	//-0x1A1000 4K空间   腾讯密二维码密钥区 4K   //与下面重复，不用到腾讯可以暂时不管
#define FLASH_TXQRCE_END	0x1A1000	

// #define FLASH_TEST_BLOCK	0x1A1000	//-0x1A2000 4K空间　用于测试FLASH是否正常　开机时读写此块
// #define FLASH_TEMP_PRICE	0x1A2000	//-0x1A3000 4K空间　用于存贮下载的票价信息
// #define FLASH_TEMP_RECORD	0x1A6000	//-0x1A7000 4K空间　用于存贮准备写进flash文件系统的临时记录，开机加载判断，并删除
//直到0x1B0000 还没有用
#define FLASH_WHITE_START 0x1A7000//城市互联互通城市名单空间：4K    前面6字节传“有效”和长度2字节，高位在前
#define FLASH_WHITE_END	  0x1A8000

#define FLASH_WHITE_START_JTB	0x1A8000//JTB城市互联互通城市名单空间：4K    前面6字节传“有效”和长度2字节 ，高位在前  数据域里面前2字节是版本高位在前
#define FLASH_WHITE_END_JTB		0x1A9000


 #define FLASH_TAC_START	0x1AE000	//- 4K空间   腾讯二维码里面用于校验本地数据的tackey 4K 
 #define FLASH_TAC_END	0x1AF000

#if 0	// 根据兴义的银联代码
#define FLASH_MOBILE_START	0x1A0000	//-0x1A1000 4K空间   移动参数存贮区 4K 
#define FLASH_MOBILE_END	0x1A1000	
#define FLASH_TEST_BLOCK	0x1A1000	//-0x1A2000 4K空间　用于测试FLASH是否正常　开机时读写此块
#define FLASH_TEMP_PRICE	0x1A2000	//-0x1A3000 4K空间　用于存贮下载的票价信息
#define FLASH_TEMP_RECORD	0x1A6000	//-0x1A7000 4K空间　用于存贮准备写进flash文件系统的临时记录，开机加载判断，并删除

//直到0x1B0000 还没有用
#define FLASH_QRCTAC_START  0X1A8000    // 存对称密钥信息
#define FLASH_QRCTAC_END    0X1A9000
#else   //暂时不管腾讯
#define FLASH_MOBILE_START	0x1A0000	//-0x1A1000 4K空间   移动参数存贮区 4K 
#define FLASH_MOBILE_END	0x1A1000	
#define FLASH_TEST_BLOCK	0x1A1000	//-0x1A2000 4K空间　用于测试FLASH是否正常　开机时读写此块
#define FLASH_TEMP_PRICE	0x1A2000	//-0x1A3000 4K空间　用于存贮下载的票价信息
#define FLASH_WHT_BANK		0x1A3000	//-0x1A4000 4K空间　用于存贮下载的银联白名单
#define FLASH_TEMP_RECORD	0x1A4000	//-0x1A5000 4K空间　用于存贮准备写进flash文件系统的临时记录，开机加载判断，并删除
#define FLASH_CACERT		0x1A5000	//-0x1A6000 4K空间　用于存贮证书文件 4 字节长度+  cacert-test.pem证书文本
#define FLASH_PBOC_PARA		0x1A6000	//-0x1A6000 4K空间　用于存贮银联的参数，密钥之类的，贴点存一份，这存一份开机检测，
#define FLASH_QRCTAC_START  0X1A8000    // 存对称密钥信息
#define FLASH_QRCTAC_END    0X1A9000
#define FLASH_CACERT_BACKUP	0x150000	//-0x1A6000 4K空间　用于存贮证书文件 在这里存备份
#define FLASH_RE_RECORD		0x1A000	//-0x1AB000 4K空间　用于存贮证书文件
//直到0x1B0000 还没有用
#endif





#define FLASH_TEMP_START	0x1B0000	//-0x1C0000 临时数据存贮区　64K
#define FLASH_TEMP_END		0x1D0000	//-0x1C0000 临时数据存贮区　64K  //GPS

#define FLASH_GPS_LINE_START 0x1C0000	//GPS 线路信息开始 128K
#define FLASH_GPS_LENE_VER	 0x1CFFC0	//总包的版本号，存在后64字节处, 包括线路、限速、广告
#define FLASH_GPS_LENE_END	 0x1D0000	//GPS 线路信息结束 

#define FLASH_GPS_SOUND_START 0x1D0000	//GPS 语音开始 64K  这里被用于公钥存贮
#define FLASH_GPS_SOUND_END	  0x1E0000	//GPS 语音结束


#define FLASH_PUBLICKEY_START	0x1E0000	//软件算RSA，保存公钥区。32K
#define FLASH_PUBLICKEY_END		0x1E8000

#define FLASH_BLK_START 0x1F0000	//黑名单空间：64K 65536 每块4K
#define FLASH_BLK_END	0x200000


#define FLASH_GPS_AD_START 0x3B0000	//GPS语音广告开始 64K
#define FLASH_GPS_AD_END	  0x3B8000	//GPS 语音广告结束

#define FLASH_ZK_START 0x3C0000		//字库空间:256K
#define FLASH_ZK_END 0x400000

#define FLASH_ZK_START_3232		0x500000	//字库空间:1M, 32*32点阵
#define FLASH_ZK_END_3232		0x5FFFFF

#define FLASH_BU_PARA			0x3AF000	//车载机固定参数区 4K 2009.2.18 在库函数据中写入

#define FLASH_RECORD_POINT		0x3B0000	//存贮指针信息 64K
#define FLASH_RECORD_POINT_END	0x3C0000	//-0x3C0000 存贮指针信息

#define FLASH_RE     0x410000
#define FLASH_RE_END 0x420000
#define  FLASH_ODA        0x600000
#define FLASH_0DA_END     0x700000
//由于F407程序空间比F103增大较多，最大可达512- 64 = 448K
/*********************************
//F103定义
//#define FLASH_PRO_FLAG		0x33FFC0	//程序存贮标志区64字节
//#define FLASH_PRO_START		0x300000	//程序存贮开始位置   
//#define FLASH_PRO_END			0x33B800	//0x33C000//238K程序空间 18K的引导空间,最后64字节放GPRS序版本号
*********************************/
#define FLASH_PRO_FLAG			0x3700C0	//程序存贮标志区64字节	(0x3700C0-0x3700FF)
#define FLASH_PRO_START			0x300000	//程序存贮开始位置   
#define FLASH_PRO_END			0x370000	//448k程序空间,32K的引导空间,32K参数空间


#define ERASE4KBYTESECTOR 1
#define ERASE32KBYTESECTOR 2
#define ERASE64KBYTESECTOR 3
#define ERASECHIP 0xaa

#define MASK4K 0x1000
#define MASK32K 0x8000
#define MASK64K 0x10000

#define FLASH1 1
#define FLASH2 2


extern void CheckHzk(unsigned char mode);
/*********************************************
*功能: flash擦除
*输入: mode-擦除一页的大小 addr-擦除页内的地址
*输出: NULL
**********************************************/
extern void flasherase(unsigned char mode, unsigned int addr);
extern void flasherasefont(unsigned char mode, unsigned int addr);
/*           timer                */
extern void timer0_init (void);
/*---------------------------------------------------------
NAME: SET TIMER
FUNC：设置全局的TIME[] 数组
-----------------------------------------------------------*/
extern void set_timer(unsigned int tick);
extern void set_timer1(unsigned int tick);
/*---------------------------------------------------------
NAME: SET TIMER
FUNC：设置全局的TIME1[] 数组
-----------------------------------------------------------*/
extern void set_timer0(unsigned int tick, unsigned char mode);
/*---------------------------------------------------------
              得到定时的时间
-----------------------------------------------------------*/
extern unsigned int get_timer(void);
extern unsigned int get_timer0(unsigned char mode);
extern unsigned int get_timer1(void);
/********************************************************/
/***************************************************
* 功能：s延时程序
* 输入：xms-延时的值
* 输出：NULL
****************************************************/
extern void delayxs(unsigned int xs);
/***************************************************
* 功能：ms延时程序
* 输入：xms-延时的值
* 输出：NULL
****************************************************/
extern void delayxms(unsigned int xms);
extern void delayus ( void );
extern void Delay(unsigned int ticks);
extern void spicssel(unsigned char index);
extern unsigned char spisndbyte(unsigned char data);
// USART 
#define COM1 1
#define COM2 2
#define COM3 3
#define COM4 4
#define COM5 5
#define COM6 6
#define COM_QR  7	//F407串口6, 接二维码模块
/***************************************************
* 功能：串口开中断初始化
* 输入：com－串口号 1串口1（DEBUG口RS232输出及TLL[JP1])，2串口2(红外及GPRS)，4串口4(RS232),5串口5(TLL[JP2])
* 输出：NULL
* 注意:串口3是串口1的复用,用于输出到协议板
		开启中断时要写中断服务程序(在stm32f10x_it.c中)
****************************************************/
extern void COM_it_enable(unsigned char com);
/***************************************************
* 功能：串口关闭中断初始化
* 输入：com－串口号 1串口1（DEBUG口RS232输出及TLL[JP1])，2串口2(红外及GPRS)，4串口4(RS232),5串口5(TLL[JP2])
* 输出：NULL
* 注意:串口3是串口1的复用,用于输出到协议板
****************************************************/
extern void COM_it_disable(unsigned char com);
/***************************************************
* 功能：串口初始化程序
* 输入：com－串口号 1串口1（DEBUG口RS232输出及TLL[JP1])，2串口2(红外及GPRS)，4串口4(RS232),5串口5(TLL[JP2])
		uibaud-波特率
* 输出：NULL
* 注意:串口3是串口1的复用,用于输出到协议板
****************************************************/
extern void com_init(unsigned char com, unsigned int uibaud);
/***************************************************
* 功能：串口发送程序
* 输入：com－串口号; uiLen-发送数据长度; SenBuf-发送数据缓冲
* 输出：NULL
* 注意：串口1、2、3采用DMA发送方式，COM2最多发送1024个字节  COM1\3最多发送300字节
	缓冲COM2用的是"comSendbuff"  com1\3用的是"irda_rxBuf"
	DMA方式，不能紧接着调用多次，否则上一次没发完，就发送这次导致上次的数据发送不完整
	COM2:使用DMA方式，COM1\3当参数com高4位不为0时采用DMA方式、否则不用DMA。
****************************************************/
extern unsigned char comSendbuff[1024];
extern void com_snd(unsigned char com, unsigned int uiLen, unsigned char *SendBuf);
/***************************************************
* 功能：串口接收程序
* 输入：com－串口号; RevBuf-接收字节缓冲
* 输出：0 成功，其它 超时
****************************************************/
extern int com_rev(unsigned char com, unsigned int *RevBuf);
/***************************************************
* 功能：串口打印出字符串
* 输入：com－串口号; str-字节缓冲
* 输出：0 成功，其它 超时
****************************************************/
extern void debugstringCOM(unsigned char com, char *str);
/***************************************************
* 功能：串口打印出十六进制数
* 输入：com－串口号; value-字节缓冲,uclen-长度,mode-发送完成后是否插入回车(\r\n)
* 输出：0 成功，其它 超时
****************************************************/
extern void debugdataCOM(unsigned char com, unsigned char *value, unsigned int uclen, unsigned char mode);
//下面是固定串口发送
extern void debugstring4(char *str);
//extern void debugstring(char *str);
//mode最高位为1时为串口4输出,否则串口1输出
//extern void debugdata(unsigned char *value, unsigned int uclen, unsigned char mode);
//mode最高位为1时为串口4输出,否则串口1输出
extern void debugstringC(char *str, unsigned char mode);
/***************************************************
* 功能：串口红外功能初始化程序
* 输入：uibaud-波特率
* 输出：NULL
****************************************************/
extern void irda_init(unsigned int uibaud);
extern void Irda_It( void );
/***************************************************
* 功能：红外发送,需使能发送，禁止接收
* 输入：uilen=发送字节数,RevBuf-发送字节缓冲
* 输出：0=发送正确，其它-无效
****************************************************/
extern int irda_snd(unsigned int uilen, unsigned char *value);
extern void irda_it_En(void);
extern void irda_it_dis(void);
/********************************************
* 功能：时间初始化
* 输入：NULL
* 输出：NULL
******************************************/
extern void timeinit(void);
/********************************************
* 功能：读取时间
* 输入：time-时间数据指针
* 输出：0
*********************************************/
extern int timeread(unsigned char *time);
/********************************************
* 功能：设置RTC时间
* 输入：time-时间数据指针 ssmmhh ddmmyyyy
* 输出：0
****************************************************/
extern int timewrite(unsigned char *time);

/********************************************
* 功能：检测时间是否有效
* 输入：time-时间数据指针 7字节 BCD码
* 输出：0-无效   1-有效
****************************************************/
extern unsigned char time_invalid(unsigned char *time_buff);
/********************************************
* 功能：检测时间是否有效字符
* 输入：time-时间数据指针  14字节 ASC码
* 输出：0-无效   1-有效
****************************************************/
extern unsigned char time_invalid_str(unsigned char *time_buff);
extern void setdatetime(void);
//14字节日期时间 是否合法 0不合法  1合法
extern unsigned char time_long_invalid(unsigned char *time_long_buff);
//7字节长度时间比较, 1 scrt>dest  -1 dest>scrt 0 相等
extern int cmpttime(unsigned char *scrt, unsigned char *dest);
extern int SysSetTime(unsigned char *time);
/********************************************
* 功能：写铁电
* 输入：Address_f-铁电地址 length-长度 Framdata-数据指针
* 输出：NULL
****************************************************/
extern void fewrite(unsigned int Address_f,unsigned int length, unsigned char *Framdata);
/********************************************
* 功能：读取铁电内容
* 输入：Address_f-铁电地址 length-长度 Framdata-数据指针
* 输出：NULL
****************************************************/
extern void  feread(unsigned int Address_f,unsigned int length,unsigned char *rec_data);
/********************************************
* 功能：写铁电  写入再读出比较 只比较前256个
* 输入：Address_f-铁电地址 length-长度 Framdata-数据指针
* 输出：NULL
****************************************************/
extern void sysfewrite(unsigned int Address_f,unsigned int length, unsigned char *Framdata);
/********************************************
* 功能：读取铁电内容 多次读出比较 只比较前256个
* 输入：Address_f-铁电地址 length-长度 Framdata-数据指针
* 输出：NULL
****************************************************/
extern void sysferead(unsigned int Address_f,unsigned int length,unsigned char *rec_data);

extern void open_wdt(void);
//文件系统空单是上面的值减去8M。
extern unsigned int ugflash_capacity;

//mifare

#define PICC_REQIDL        0x26         //!< request idle
#define PICC_REQALL        0x52         //!< request all
#define PICC_ANTICOLL1     0x93         //!< anticollision level 1 106 kBaud
#define PICC_ANTICOLL2     0x95         //!< anticollision level 2
#define PICC_ANTICOLL3     0x97         //!< anticollision level 3
#define PICC_AUTHENT1A     0x60         //!< authentication using key A
#define PICC_AUTHENT1B     0x61         //!< authentication using key B
#define PICC_READ          0x30         //!< read block
#define PICC_WRITE         0xA0         //!< write block
#define PICC_UL_WRITE      0xA2         //!< ultralight write block
#define PICC_DECREMENT     0xC0         //!< decrement value
#define PICC_INCREMENT     0xC1         //!< increment value
#define PICC_RESTORE       0xC2         //!< restore command code
#define PICC_TRANSFER      0xB0         //!< transfer command code
#define PICC_HALT          0x50         //!< halt

extern unsigned char RC531FIFOLevelCheck(void);
extern char PcdRfReset(unsigned char ms);
extern unsigned char InitRC531( void );
extern unsigned char GetTypeAUID( unsigned char *pUID );

extern unsigned char MifareAuth(unsigned char key_type,
								unsigned char *snr,
								unsigned char key_sector,
								unsigned char block);
// ==================================================================
//
extern unsigned char MifareAuthKey(unsigned char auth_mode,
								   unsigned char *snr,
								   unsigned char *keys,
								   unsigned char block);
// ==================================================================
//
extern unsigned char MifareRead(unsigned char cBlockNo, unsigned char *pRdData);
// ==================================================================
//
extern unsigned char MifareWrite(unsigned char cBlockNo, unsigned char *pWrData);
// ==================================================================
//
extern unsigned char MifareValueDebit(unsigned char cSubCommand, unsigned char cBlockNo, unsigned char *pValue);
// ==================================================================
//
extern unsigned char MifareTransfer(unsigned char cBlockNo);
// ==================================================================
//
extern unsigned char MifareValue(unsigned char cSubCommand, unsigned char cBlockNo, unsigned char *pValue);

extern unsigned char MifareHalt(void);

extern unsigned char RC531Close( void );
extern unsigned int RfPassthrough( unsigned char *dst, unsigned int iMaxlen, unsigned char *src, unsigned int sendlen);
//返回错误寄存器的值
extern unsigned short RC531errReg(void);

extern void Rc531TxClose(void);
extern void Rc531TxOpen(void);

//---type B
extern unsigned char GetTypeBUID( unsigned char *pUID );
extern unsigned char RC531ReqB( unsigned char *rev );
extern unsigned int RfPassthrough_TypeB(unsigned char *dst, unsigned int iMaxlen, unsigned char *src, unsigned int sendlen);
//返回：22=RC522读头，31=RC531读头，其它=读头初始化错误。
extern unsigned char getRCMode(void);
//PSAM

#define SAM1 1
#define SAM2 2
#define SAM3 3
#define SAM4 4

#define SAM_VOL3 3
#define SAM_VOL5 5

typedef struct
	{
		unsigned char CLA;
		unsigned char INS;
		unsigned char P1;
		unsigned char P2;

		unsigned char LC;
		unsigned char DATA[256];

		unsigned char LE;
	}ISO7816_ADPU_SEND;

typedef struct
	{
		unsigned char LE;
		unsigned char DATA[256];

		unsigned char SW1;
		unsigned char SW2;
	}ISO7816_ADPU_RESPONSE;

/*********************************
*功能: SAM卡控制初始化
*输入: chips-SAM卡号,vol-电压,baud-波特率
*输出: NULL
**********************************/
extern void saminit(void);
/*******************************************
* 功能：读TDA8020状态
* 输入：sam-SAM卡序号 value－读出的值
* 输出: NULL
**********************************************/
#define PSAM_PRES 0x01
#define PSAM_PRESL 0x01<<1
#define PSAM_IO 0x01<<2
#define PSAM_SUPL 0x01<<3
#define PSAM_PROT 0x01<<4
#define PSAM_MUTE 0x01<<5
#define PSAM_EARLY 0x01<<6
#define PSAM_ACTIV 0x01<<7
extern unsigned char DeactiveCard(unsigned char ucCardSelec);
extern void TDA8020Read(unsigned char sam, unsigned char* value);

extern unsigned char SamColdResetNPPS(unsigned char CardSelect, unsigned int uiRate, 
						   unsigned char ucVoltage, unsigned char *atr);
/*---------------------------------------------------------
NAME ： SamColdReset
FUNC :  冷复位SAM 卡
IN   ：
OUT  ： 成功返回接收到的长度
		失败返回0
-----------------------------------------------------------*/
extern unsigned char SamColdReset(unsigned char CardSelect, unsigned int uiRate, 
						   unsigned char ucVoltage, unsigned char *atr);
//						   unsigned char *Rec_len);
/*---------------------------------------------------------
NAME ： SamSendApdu
FUNC :  与SAM卡交换APDU数据包(T0协议)
IN   ：
OUT  ： 成功返回1
		失败返回0
-----------------------------------------------------------*/
extern unsigned short SamdSendAPDUT0(unsigned char *buffer, unsigned short length,
							  unsigned char *rbuffer);
/*---------------------------------------------------------
NAME ： SamSendApdu
FUNC :  与SAM卡交换APDU数据包
IN   ：
OUT  ： 成功返回0
		失败返回其他
-----------------------------------------------------------*/
//extern unsigned char SamSendApdu(unsigned char *pTxBuf, unsigned short iLen, unsigned char *pRxBuf);
//extern unsigned char IccCommand(unsigned char Slot, ISO7816_ADPU_SEND *ApduSend, 
//						 ISO7816_ADPU_RESPONSE *ApduRecv);

extern unsigned char Get_Challenge(unsigned char Slot, unsigned char *Receivebuf);
extern unsigned char  Select_FileF(unsigned char Slot,unsigned char *file_ID, unsigned char len);
extern unsigned char  Select_File(unsigned char Slot, unsigned int file_ID);
extern unsigned char Verify_private_PIN(unsigned char Slot, unsigned char length, unsigned char *ptr);
extern unsigned char Change_private_PIN(unsigned char Slot, unsigned char length,unsigned char *ptr);
extern unsigned char  Read_Binary(unsigned char Slot, unsigned char file_ID,unsigned char offset,unsigned char length,unsigned char *ptr);
extern unsigned char  Update_Binary(unsigned char Slot, unsigned char cla,unsigned char file_ID,unsigned char offset,unsigned char length,unsigned char *ptr);
extern unsigned char Get_MifKey(unsigned char Slot, unsigned char p1, unsigned char p2, unsigned char length,unsigned char *ptr);
extern unsigned char  Read_Record(unsigned char Slot, unsigned char file_ID,unsigned char recordNO,unsigned char length,unsigned char *ptr);
////////////////////////////////////////////////////////////////////////
//
//                   更新记录文件
//
////////////////////////////////////////////////////////////////////////
extern unsigned char  Update_Record(unsigned char Slot, unsigned char file_ID,unsigned char recordNO,unsigned char length,unsigned char *ptr);
///////////////////////////////////////////////////////////////////////////
//
//                       追加记录文件
//
///////////////////////////////////////////////////////////////////////////
extern unsigned char Append_Record(unsigned char Slot, unsigned char cla,unsigned char file_ID,unsigned char length,unsigned char *ptr);
/////////////////////////////////////////////////////////////////////////////
//
//                 内部认证
//
/////////////////////////////////////////////////////////////////////////////
extern unsigned char  Internal_Auth(unsigned char Slot, unsigned char *ptr, unsigned char Keyno);
///////////////////////////////////////////////////////////////
//
//       though  external authen  psam
//
///////////////////////////////////////////////////////////////

extern unsigned char  External_Auth(unsigned char Slot, unsigned char *ptr, unsigned char Keyno);
////////////////////////////////////////////////////////////////////////////
//
//        init_for_descrypt(uchar *cardNO)说明
//             输入: cardNO--指向用户卡物理卡号(4字节)
//             输出: PSAM 卡将利用卡中指定的密钥,产生一个
//                   临时密钥(过程密钥)，临时密钥在PSAM卡
//                  下电后自动消失，不可读。
//
///////////////////////////////////////////////////////////////////////////
extern unsigned char init_for_descrypt(unsigned char Slot, unsigned char p1,unsigned char p2,unsigned char length,unsigned char *ptr);
///////////////////////////////////////////////////////////////
//
//       generate key 生成子密钥（过程密钥）
//
//////////////////////////////////////////////////////////////
extern unsigned char MAC(unsigned char Slot, unsigned char p1,unsigned char p2,unsigned char length,unsigned char *ptr);
extern unsigned char IccCommand(unsigned char Slot, ISO7816_ADPU_SEND *ApduSend, ISO7816_ADPU_RESPONSE *ApduRecv);
//"G" GPS信息    “B” 黑名单   “S”GPS语音
extern void Move_temp(char typee);
//headaddr:头指针  len:当前记录长度
extern void com_up_data(unsigned char *headaddr, unsigned int len);
//mode == 1 需要发包尾 =2 天水数据格式（影响按时间采集 32字节记录暂进没此功能）
//mode == 0x53 不把黑名单保存在黑名单区，保存在临时区
extern void get_data(unsigned char mode);
extern void get_data_more(unsigned char mode);
extern unsigned char sys_bu_para_save(unsigned char *para, unsigned int uilen);
extern unsigned char sys_bu_para_load(unsigned char *para, unsigned int uilen);
//CRC32 硬件算法
extern unsigned int __cpuCrc32(unsigned char *val, unsigned int len);

//从存贮空间4字节处开始存入，长度不超过220字节
extern void cpuInfoWrite(unsigned char *val, unsigned char len);
extern void cpuInfoRead(unsigned char *val, unsigned int offset, unsigned int len);
//在语音标识处写入标识位置:0x4000+240,使用空间8字节
//mode:=1写入 0清除 只有两种语音混和使用,
extern void cpuWNewSoundChip(unsigned char mode);
//返回1 新，0旧
unsigned char getAudioStyle(void);

extern void over_turn(unsigned char length,unsigned char *ptr);
extern unsigned char ValidDevNo(unsigned char *dvcbuf);
extern unsigned char ValidlineNo(unsigned char*lineB);

extern unsigned int __atol(char *strASC);

//-----------CAN总线-----------------------------
extern void CAN_Initiation(void);
//接收,每次最多收8个字节.如果对方发送的数据大于8个字节,需要多次接收后组包.
extern unsigned char can_rev(unsigned char *RevBuf);
//发送 返回0成功,1失败[请检查缆线是否接好]
extern unsigned char can_snd(unsigned char *sndbuf, unsigned int sndlen);
//--------------------------------------

extern void CountPoint(unsigned char mode);
extern void SetTcpPacketTTLTime(unsigned char val);

 extern int close_rfile_Wrecord(void);
 extern int open_rfile_Wrecord(void);
#endif

