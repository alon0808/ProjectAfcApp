
#ifndef _STPROLIB_HEAD_
#define _STPROLIB_HEAD_

//typedef int time_t;        /* time value */
//字母BCD码转字符：
//ZBbuf-传入ZBlen字节的压缩后的设备号
//strin-传出8字节设备号，不够8位前面补0
//ZBlen-需要传出的长度,不能大于16字节
//返回：8正常，其它：说明长度不对，传入号超过了8位字符
extern unsigned char ZBcod2ASCII(unsigned char *ZBbuf, unsigned char ZBlen, char *strin);
//字符转字母BCD码：
//strin-传入8字节设备号，不够8位前面补0
//ZBbuf-传出DVlen字节的压缩后的设备号
//ZBlen-需要传出的长度,不能大于16字节
//返回：//返回：最小的有效字节，不包括前面的00，小于ZBlen正常，其它：说明长度不对，ZBlen字节保存不了这么多数
extern unsigned char ASCII2ZBcod(char *strin, unsigned char *ZBbuf, unsigned char ZBlen);
/*****************************************
应用库函函数
记录写入
DES
******************************************/
//库函数中变量定义
/*************DES*******************/

extern void DES_encrypt(unsigned char *plain_text,unsigned char *key_text,unsigned char *encrypt_text);
extern void DES_decrypt(unsigned char *encrypt_text,unsigned char *key_text,unsigned char *plain_text);
extern void DES3_decrypt(unsigned char *encrypt_text,unsigned char *key_text,unsigned char *plain_text);
extern void DES3_encrypt(unsigned char *plain_text,const unsigned char *key_text,unsigned char *encrypt_text);
extern void CountMac1(unsigned char *Key,unsigned char *random,unsigned char *_data,unsigned int len ,unsigned char *Result);
//需要时间比较长
//16字节密钥
/************************************************************************/
/* key : 密钥 16字节
   inittext : 初始值  8字节
   Source : 需加密的数据
   le : 加密数据的长度
   destin : 加密后的结果 4字节
                                                                     */
/************************************************************************/
extern void CountMac(unsigned char *key,unsigned char *inittext,unsigned char *Source,unsigned int le ,unsigned char *destin);
//8字节密钥
/************************************************************************/
/* key : 密钥 8字节
   inittext : 初始值 8字节
   Source : 需加密的数据
   le : 加密数据的长度
   destin : 加密后的结果 4字节
                                                                     */
/************************************************************************/
extern void SCountMac(unsigned char *key,unsigned char *inittext,unsigned char *Source,unsigned int le ,unsigned char *destin);

/***************************************
Key     ：16字节密钥,计算SESLK的3DES的密钥，比如在圈存时计算SESLK时，3DES的密钥为“圈存子密钥”(详见个人化信息)；
pInData1：参加计算SESLK的源数据，比如在圈存时pInData1为  伪随机数（ICC）||电子存折联机交易序号或电子钱包联机交易序号||“8000”
pInData2：参加计算MAC的源数据，比如在圈存时pInData2为  电子存折余额（交易前）或者电子钱包余额（交易前）||交易金额 || 交易类型标识 || 终端机编号。
data2len：pInData2的长度；
initData：8 bytes的0x00 数组；
mac     ：需要计算出来的MAC值结果。
****************************************/
extern void Mac(unsigned char *Key,unsigned char *pInData1,unsigned char *pInData2,int data2len,unsigned char *initData, unsigned char *mac);
//-----------------------------------------
/**************RecordFile***********************/
extern int getsysfile(unsigned char Bsec, unsigned char *flname);
//Bsec-块号（0－31）,文件名从起始地址开始查找完4K空间.
extern int savesysfile(unsigned char Bsec, unsigned int startAddr, unsigned int endAddr);
//取开始日期－结束日期的地址(1-31),只能取连续的天数
extern int getHisAddr(unsigned char* sdat, unsigned char* edat, unsigned int *sadd, unsigned int *eadd);
extern void writeBackRec(unsigned char*dat, int len, unsigned char mode);//取记录指针
//返回需要写入的指针,参数返回头指针
//mode == 1 只返回是否满
//extern int Get_Record_point(unsigned char *headp, unsigned char mode);
//清除并格式当前存贮区
extern void lib_clr_Record_point(void);
extern unsigned char libWriteRecord(unsigned int cFeaddr, unsigned char *buffer, unsigned char mode);
//---------------------------------------------------------
/**************************KeyFile**************/
extern unsigned char lget_MiKey(unsigned char gPSAMin, unsigned char mode, unsigned char *rekey);
//mode=1 安顺使用 0 默认咸阳  2 云平台
extern unsigned char Lib3DESKey(unsigned char *data, unsigned char *outKey, unsigned char mode);
//云平台 计算用户卡密钥
extern unsigned char Lib3DESCloudKey(unsigned char *data, unsigned char *outKey, unsigned char *desKey);
extern unsigned char LibGetMKey(unsigned char SamSlot, unsigned char *data, unsigned char mode);
//计算交易记录的MAC校验 独有KEY  长度不超过256字节
extern void CountRecMac(unsigned char *Source,unsigned int le ,unsigned char *destin);
//----------------------------------------------------------
/****************CRC**********************/
extern unsigned char CRC8_Tab(unsigned char *ucPtr, unsigned char ucLen);
extern unsigned int crc16_isr(unsigned char *Dat, unsigned int len);
extern unsigned int Getcrc16(unsigned char *r_data,unsigned int length, unsigned char cInhi, unsigned char cInlo);
extern unsigned int GetCrcWord(unsigned char* ptr,int length);
extern unsigned short cal_crc16(unsigned char *ptr, unsigned char len);
extern unsigned int GenerateCRC32(unsigned char *DataBuf,unsigned int len);
extern void crc_cu_16(unsigned char *SerBuffer, unsigned int n,unsigned int length);
extern unsigned char crc_ck_16(unsigned char *SerBuffer,unsigned int length);
extern unsigned char crc_c(unsigned char x,unsigned char temp);
extern unsigned char crc_ck(unsigned char *string_ck,unsigned char length);
extern void crc_cu(unsigned char *string_cu,unsigned char length);
//九江的CPU卡消费密钥
//extern unsigned getJiuJiang_CPUKey(unsigned char *Sdata, unsigned char *Rkey);
//-----------------------------------------------------------
/****************PUBLIC**********************/
extern unsigned char checkIP(char *IPb);
extern unsigned char bcdADD(unsigned char bcd1, unsigned char bcd2);
extern unsigned char bcdDEC(unsigned char bcd1, unsigned char bcd2);
extern unsigned int str2int(unsigned char *pstr, unsigned char len);
extern unsigned int BCD2int(unsigned char *pstr, unsigned char len);

extern void MessageBox(unsigned char ucIsError, char *pDispBuf);
extern void TimeAdd(unsigned char*outBCDTime, unsigned char*bcdTime, unsigned int addSec);

//两个时间相减，单位为秒。前面时间减后面时间。
extern int DataTimeDec(unsigned char *datatime1, unsigned char *datatime2);

//len-要比较的个数（以uL为单位）  dat-数据  ul-每个单位的长度（int为4 short为2）
extern unsigned int BinSCH(unsigned int len, unsigned char *dat, unsigned char *sdat, unsigned char uL);
//整数平方根
extern unsigned int sqrtint(unsigned int a);
//目录
//显示目录名称
//目录函数
//抬头显示 
//总条数
//超时时间 50ms单位
extern int MenuFrame(char menu[][20], void (*function[])(void), char *titlestr, unsigned char ucMENU_NUM, unsigned int dlywaite);//可以翻多页显示,
//显示屏能显示多少行,老屏显示8行，新屏显示16行,默认8行，如果要改动需要调用程序更改
extern void setCUM_PER_SCREEN(unsigned int cnum);
extern void dispageInfo(unsigned char maxROW, char disbuf[][20]);
//可以翻多页显示, 显示长度加大到30字符。
extern void dispageInfo_L(unsigned char maxROW, char disbuf[][30]);

extern void end_card(void);
extern unsigned char MifIsPurse(unsigned char *buffer);
extern unsigned char MiBlockInvalid(unsigned char mode,unsigned char count,unsigned char *ptr);
extern unsigned char profile(unsigned char mode);

// typedef struct{
// 	unsigned char LineNo[3];
// 	unsigned char buPrice[4];
// 	unsigned char rate[32];
// 	unsigned char deviceNO[8];
// 	unsigned char IPaddr[4];//IP地址 POS
// 	unsigned char Port[2];//端口号 POS
// 	unsigned char stopflag;//是否要暂停刷卡 POS
// 	unsigned char ManagePassWord[3];//6位密码 3 字节 POS
// 	unsigned char reservee[3];		//--------60
// 	unsigned char UserKeyA[16];//存密钥卡的密钥
// 	unsigned char UserKeyACrc32[4];//kEY CRC
// 	unsigned char UserKeyB[16];//存密钥卡的密钥
// 	unsigned char UserKeyBCrc32[4];//kEY CRC  -----100
// 	
// 	unsigned char BussinessNo[6];//商户编号2字节+CRC32 4字节 ＝6字节 
// 	unsigned char ReseverAPN[17];//POS机中是APN
// 	unsigned char catalogPOST;//目录区所在位置
// 	unsigned char voice[32];//车载机语音代码，充值机：月票信息区
// 	unsigned char plusMonthPrice[4];//月票附加费
// 	unsigned char wifi_SSID[10];	//WIFI SSID
// 	unsigned char wifi_KEK[20];		//WIFI KEY
// 	unsigned char wifi_CrC32[4];	//WIFI KEY 和 WIFI SSID 的校验码 
// 	unsigned char FDModeFlag;		//分段程序中设置的标志，如果标志=0x5E为后门机，标志为0xA0为前门机，如要标志都无效内通过硬件来判断。	
// 	unsigned char linever[2];		//2017-5-8线路票价版本    低在后
// 	unsigned char CSN_BUSVer[2];	//票价版本
// 	unsigned char Reserver[15];//
// 	unsigned char DriverNO[4];
// 	unsigned char Crc32[4];
// }stCpuInfo;//存于CPU FLASH中的数据。

typedef struct{
	unsigned char LineNo[3];
	unsigned char buPrice[4];
	unsigned char rate[32];
	unsigned char deviceNO[8];
	unsigned char IPaddr[4];//IP地址 POS
	unsigned char Port[2];//端口号 POS
	unsigned char stopflag;//是否要暂停刷卡 POS
	unsigned char ManagePassWord[3];//6位密码 3 字节 POS
	unsigned char PlusSta;			//附加费启用标志
	unsigned char reservee[2];		//--------60
	unsigned char UserKeyA[16];//存密钥卡的密钥
	unsigned char UserKeyACrc32[4];//kEY CRC
	unsigned char UserKeyB[16];//存密钥卡的密钥
	unsigned char UserKeyBCrc32[4];//kEY CRC  -----100
	
	unsigned char BussinessNo[6];//商户编号2字节+CRC32 4字节 ＝6字节 
	unsigned char disprice[3];	//启用附加费时候该显示的票价，只用于界面显示用2+xor1
	unsigned char ReseverAPN[14];// 没用
	unsigned char catalogPOST;//目录区所在位置
	unsigned char voice[32];//车载机语音代码，充值机：月票信息区
	unsigned char plusMonthPrice[4];//月票附加费
	unsigned char wifi_SSID[18];	//WIFI SSID
	unsigned char wifi_KEK[10];		//WIFI KEY
	unsigned char wifi_CrC32[4];	//WIFI KEY 和 WIFI SSID 的校验码 
// 	unsigned char  buBalance[4];		//余额
// 	unsigned char  buMoney[4];			//折扣后票价
	unsigned char linever[2];		//线路票价版本    priceVER 20140731 QUAN
	unsigned char CSN_BUSVer[2];
	unsigned char Time_price[16];
	unsigned char plusPrice[4];//	附加费使用
	unsigned char Crc32[4];
}stCpuInfo;//存于CPU FLASH中的数据。
extern unsigned char getbussinessNO(unsigned char *dat);
extern unsigned char getWifiInfo(unsigned char *SSID, unsigned char *KEYE);

extern void wifiTlh_main(void);
#endif
