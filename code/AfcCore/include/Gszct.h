
#ifndef _GSZCT_HEAD_FILE_
#define _GSZCT_HEAD_FILE_

#include "StructDef.h"

//在库函数中用，不能更改
//#define M_PRO_VER "20090912160901"//20091011160901 测试中的版本号
//------------GPRS----------------
#define CMD_QURE_INFO	0xa0
#define CMD_RE_CONNECT	0xa2
#define CMD_DATA_GPRS	0xa1
#define CMD_DATA_GPS	0xa3
#define CMD_SET_PARA	0xa4
#define CMD_QURE_OTHER	0xa5
#define CMD_GPRS_CMD	0xa6
#define CMD_GPRS_OFF	0xa7
#define CMD_GPRS_ON		0xa8
#define CMD_RICEV_DATA	0xa9
#define CMD_RESTARD		0xaa
#define CMD_SEND_URT1	0xab//把数据发送到URT1
#define CMD_GET_ACK		0xac
#define CMD_24G_PROFILE	0xad

//#define BLACKLEN     16000//2                       0000//
//#define BLK_SNO_LEN	4	//一条黑名单长度

#define LEN_TCPIP_PACKET	500

#define CMD_GPRS_DERIVENO	0x02
#define CMD_GPRS_SEND_DATA  0x01
#define CMD_GPRS_RECORD		0x7e

#define CMD_GPRS_DEL_BLACK	0x8b

#define CMD_GPRS_TIME_BTM	0x04

#define CMD_GPRS_PRICE_BTM	0x09
#define CMD_BTM_AUDIO		0x08
#define CMD_BTM_PRICE_CARD	0x09
#define CMD_BTM_DREIVE		0x03


#define CMD_GCS_PING		0x8b
#define CMD_GPRS_PING		0x08	
#define CMD_SETUP_AUDIO		0x89
#define CMD_UPATE_DERIVE_NO	0x83
#define CMD_GPRS_TIME_GCS	0x84
#define CMD_GPRS_PRICE_GCS	0x8a

#define CMD_CHECK_BLACK_VER	0x85
#define CMD_UPDATE_BLACK_VER 0x86
#define CMD_GPRS_ADD_BLACK	0x87
#define CMD_DELE_BLACK		0x88
#define CMD_BLACK_ALL		0x8d	
#define CMD_BLACK_DEL_ALL	0x8c

#define CMD_DEAL_MODE		0x90
#define CMD_YCT_PRICETABLE	0x91
#define CMD_WRITE_CARD		0x92
#define CMD_SEND_WRITE		0x93
#define CMD_CHECK_MESSAGE	0x94

#define CMD_REACORD_ECHO	0x00
#define CMD_DERIVE_ECHO		0x1f
#define CMD_SETUP_TIME		0x23
#define CMD_AUDIO_REQUES	0x2a
#define CMD_PING_ECHO		0x2c		

#define MASK_RCV_BUF_INIT					0x00				//接收帧初始化状态码
#define MASK_RCV_BUF_USED					0x80				//该接收帧缓冲不空 含有数据
#define MASK_RCV_BUF_PEEKED					0x40				//这个帧已经被链路层处理 放进了应用栈

#define TCP_ID_VER 0x01		//TCP包版本
#define DVICE_STYLE 0x01	//设备类型

#define GPRS_SOCKET_CHECK_CMD  0x3A

#define GPRS_LINK_CMD	0xF0//模块关闭中
#define GPRS_TCPCLOSE_CMD 0xF1//TCPIP链接关闭中

//启动MC55i,需要开启，使电平为高
#define GPRS_LINK_UP 0xF3
//GPRS正在等模块注册
#define GPRS_AT_WAIT 0xF4
//命令关闭模块，AT^SMSO
#define GPRS_MC55_CLOSE 0xF5
//模块需要重启
#define GPRS_MODE_NEED_CLOSE 0xF6
//模块下电
#define GPRS_MODE_POWER_DOWN 0xF7
//模块上电
#define GPRS_MODE_POWER_ON 0xF8
//AT软件复位指令
#define GPRS_AT_RESET 0xFA
//模块电源关闭然后打开，重新开始GPRS测试
#define GPRS_HWPOWER_RESET 0xFE


//模块需要关闭TCP/IP
#define GPRS_NEED_CLOSEIP 0x2F

#define GPRS_Out_CMD 0xF2//GPRS分离
#define GPRS_SENDING_CMD 0xA0//正在发送数据，等收应答中
#define GPRS_RING_CMD 0xA1//有电话呼入
#define GPRS_CStatues 0xA2	//在过程中查询WIFI网络是否正常。用于检测网络是否离开(还没有使用)

#define TCPSTARTSTAT	22	//7TCP/IP开始值，之前的值是模块初始化进程


#define GPSDATA_LEN	34					//记录长度
#define GPRS_AT	0
#define GPRS_CLOSE_ATO	1
#define GPRS_SIM_STATE	2
#define GPRS_CIMI_STATE	3
#define GPRS_CREG_STATE	4
#define GPRS_CREG_C		5
#define GPRS_CGREG_STATE 6
#define GPRS_CGREG_C	7

#define GPRS_PDP_STATE	8
#define GPRS_QIACT		9
#define GPRS_QIACT_C	10
#define GPRS_QIDE		11 //B
#define GPRS_ICCIDC		12 //c
#define GPRS_QIOPEN		13 //D
#define GPRS_CLOSEIP	17
#define GPRS_FREE		22
#define GPRS_SEND_C		16
//#define LEN_BLACK_ADD 8
//#define LEN_BLACK_ADD 8
typedef struct 
{
	unsigned int ZLVer;//增量版本号
	unsigned int KLVer;//块版本号
}BLACK_ADD2;

typedef struct 
{
	unsigned char flag;
	unsigned short serial;
	unsigned short len;
	unsigned char cmd;
	unsigned char buffer[128];
}ECHO_1;

//通信帧结构定义
#define FRAME_SIZE							1500				//一帧数据最大字节长度
typedef struct tag_FRAME_BUF {
	unsigned char ucFrameType;									//帧类型字节
	unsigned char uiFrameLen[2];									//帧长度
	unsigned char FrameBuf[FRAME_SIZE];							//帧缓冲
} FRAME_BUF;

#define MAX_RCV_PACKET_NUM					5					//接收包缓冲大小
//应用包缓冲队列,用于存放数据链路层接收到的数据帧,包括应用层数据和控制帧数据
typedef struct tag_PACKET_BUF {
	FRAME_BUF LinkFrame;										//帧内容
	unsigned char ucItemStatus;									//帧状态标示 
} PACKET_RCV;

//--------------------------------
//00 00 00 04 00 12 34 56 78 1A 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

typedef struct
{
	unsigned char firstFlag[4];//第一次运行标志
	unsigned char deviceNo[9];//设备号
	unsigned char lineNo[3];//线路号
	unsigned char severIP[4];//服务器IP地址（没有可任意值）
	unsigned char severPort[2];//服务器端口号（没有可任意值）
	unsigned char priceTab[512];//票价及语音
}stSYSBUPARA;

#define RS_IN_SOUND 0x01	//进站报站
#define RS_OUT_SOUND 0x02	//出站报站
#define RS_IN_MAIN 0x04		//到达主站位置
#define RS_IN_SEND 0x10		//进站数据上传
#define RS_OUT_SEND 0x20	//出站数据上传
typedef struct  {
	unsigned char ucStation;			//当前站台号
	unsigned char ucPeStation;			//前一站台号
	unsigned char inOrOut;				//进站出站标志(1进2出)
	unsigned char ucOverSpeed;			//是否超速 正常00超速0C
	unsigned char ucDirect;				//方向 正0 负1
	unsigned char ucFlag;				//标志，是否报站低4字节，传数高4字节
}stRunStation;
extern stRunStation RunStation;				//运行中的站点信息
typedef struct  {
	unsigned char ucStation[2];
	unsigned int ulDisxx[2];
	unsigned int ulDisnn[2];
	unsigned char ucFlag[3];
}stFindStation;
extern stFindStation FindS;					//找站点变量

typedef struct {
	unsigned char flag;					//标志
	unsigned char direct;				//方向
	unsigned char buff[7];				//日期时间
}stGpsFED;
extern stGpsFED gGpsFeD;						//GPS方向存于铁电中

#define ADDR_FLASH_LINE1 FLASH_GPS_LINE_START			//第一条线路信息保存的地址 使用前32K空间
#define ADDR_FLASH_LINE2 FLASH_GPS_LINE_START+0x8000	//第二条线路信息保存的地址 使用后32K空间

#define GPS_SEND_DLY 5				//GPS非站点上报时间
#define GPS_RE_SEND_DLY	65000			//重发GPS信息
#define GPSVALID				0x41	//有效
#define GPSINVALID				0x56	//无效
#define GPSUES					0xaa	//使用中

extern stGpsPar gGPsPar;				//基本信息
extern GPS_INFO gGpsInfo;		//GPS信息,应用程序判断的缓冲
//--------GPS------------------

#ifdef _Gps_demo_auto_
extern unsigned int autodly;
extern unsigned char autoEnable;
#endif

extern void SoundFreeManage(char *sstr);

#define IDSoundPowerOn 0xa1
#define IDSoundStartST 0xa2
#define IDSoundEndST 0xa3
#define IDSoundOUT 0xa4
#define IDSoundIN1 0xa5
#define IDSoundComm 0xa6
#define IDSoundDefin 0xa7
#define IDSoundRound 0xa8
#define IDSoundFree 0xa9
#define IDSoundOverSpeed 0xB1 //超速
#define IDSoundOverStation 0xB2//越站

#define IDSoundOverSpeedAT 0xB3 //超速
#define IDSoundOverStationAT 0xB4 //超速

#define XFStartCompose	0x01		//start Compose sound
#define XFStopCompose	0x02		//stop compose sound
#define XFPauseCompose	0x03		//Pause compose soud
#define XFResumeCompose	0x04		//
#define XFQueryStatues	0x21		//Query XF-s4240 statues
#define XFPowerDown		0x88		//
////////////////////////////////////////////
#define GCMD_HEAD 0
#define GCMD_Address 3
#define GCMD_CmdCode 7
#define GCMD_GnCode 9
#define GCMD_CmdPara 10
#define GCMD_Ln 14
#define GCMD_Data 16
/////////移动数据结构定义--------------------------
#define MobileAID "\xD1\x56\x00\x01\x01\x80\x03\x80\x00\x00\x00\x01\x00\x00\x00\x00"

typedef struct{
	unsigned char MSG60_LLL[2];//BCD BM#60自定义数据长度 = 0072
	unsigned char MSG60_1[16];//CN 应用标识AID 变长，5～16个字节，后补空格（0X20）
	unsigned char MSG60_2[14];//CN 应用版本号 格式：YYYYMMDDHHMMSS
	unsigned char MSG60_3[2];//N KEK密钥索引号 00~02
	unsigned char MSG60_4[14];//CN 参数版本号 格式：YYYYMMDDHHMMSS
	unsigned char MSG60_5[2];//CN POS消息类型码 F0－FF(F0)
	unsigned char MSG60_6[2];//CN POS网管消息类型码 00－FF 01:签到	
	unsigned char MSG60_7[6];//CN 批次号 
	unsigned char MSG60_8[8];//CN 操作员号 变长，2～8个字节，后补空格（0X20）
	unsigned char MSG60_9[8];//B 保留 
}stP8583_60;//
typedef struct{
	unsigned char LLL[2];//BCD BM#60自定义数据长度 = 0072
	unsigned char MSG1[3];//借记总笔数 CN
	unsigned char MSG2[12];//借记总金额 CN
	unsigned char MSG3[3];//贷记总笔数 CN
	unsigned char MSG4[12];//贷记总金额 CN
	unsigned char MSG5[3];//转帐总笔数 CN
	unsigned char MSG6[12];//转帐总金额 CN
}stP8583_63;
typedef struct{
	unsigned char LLL[2];//BCD BM#60自定义数据长度 = 0072
	unsigned char MSG1;//批送后续标志 0x01：有  0x00：最后一个包 无后续包 CN
	unsigned char MSG2;//明细数量 0x01--0x05 CN
	unsigned char MSG3[415];//明细1-5 B
}stP8583_61;
#ifdef hb_c
typedef struct{//签到状态 保存到铁电中，重启后继续操作
	unsigned char SingInDate[7];//签到时间
	unsigned char LoadStyle;//状态，0位：KEK需下载，1位：参数需下载，2位：程序需下载
	unsigned char ISOK;//参数是否有效  此参数需要开机时判断赋值
	//------工作密钥 WK密文------------------
	unsigned char PIN_KEY[16];//
	unsigned char PIN_KEY_CHECKVALUE[4];//
	unsigned char MAC_KEY[16];//
	unsigned char MAC_KEY_CHECKVALUE[4];
	//---------------------------------------
	unsigned char resum[11];//保留
}stMobilStyle;//签到状态，包括其它任务信息，KEK 程序 参数下载状态，开机需检测

typedef struct{
	unsigned char posSerilNo[16];//POS终端序列号
	unsigned char posNo[8];//pos终端号
	unsigned char shopNo[15];//商户编号
	unsigned char shopName[20];//商户名称
	unsigned char posPiCiNo[6];//POS批次号
	unsigned char puchaceTimeOut;//pos交易超时时间 单位秒
	unsigned char tikectNumber;//找印票据单数
	unsigned char prchaceType[8];//允许交易类型,8字节位图
	unsigned char posAcktime[8];//pos回响周期,单位秒（前补0以BCD码表示）
	unsigned char oneMaxMoney[12];//终端单笔交易上限 单位分（前补0以BCD码表示）
	unsigned char posMaxmoney[12];//终端累计交易金额上限 单位分（前补0以BCD码表示）
	unsigned char congZenMactimes;//允许冲正最大次数
	unsigned char posMaxPurTimes[3];//终端每批最大交易笔数 000-999（前补0以BCD码表示）
	unsigned char posRunStyle;//终端运行状态 
	unsigned char posOKGoTimes[8];//平均无故障运行时间单位：秒(前补0以BCD码表示)
	unsigned char posOnlineTimes[8];//POS在线时间 单位：秒(前补0以BCD码表示)
	unsigned char posProNewVer[14];//终端应用程序版本号--中心给的版本号
	unsigned char Ver[14];//终端参数版本号 YYYYMMDDHHMMSS
	unsigned char purGate[15];//交易网关地址
	unsigned char pruAPN[20];//交易APN
	unsigned char purIP[15];//交易IP地址 IP地址，带点
	unsigned char purPort[6];//交易端口号 000000－999999（前补0以BCD码表示）
	unsigned char manageGate[15];//管理网关地址
	unsigned char manageAPN[20];//管理APN
	unsigned char manageIP[15];//管理IP地址
	unsigned char managePort[6];//管理端口号
	unsigned char organNo[10];//机构编号 POS所属机构的编号
	unsigned char organName[20];//机构名称简写 POS所属机构名称简写

	unsigned char KEK_SN;//密钥索引
	unsigned char KEK[48];//KEK
	unsigned char addPICINO_limit;//批次号加次数,如果次数大于5次,就不再加.
	unsigned char gMobil_Settlement_error;//同一批次批结错误次数
	unsigned char NUllbuf[6];//
}stMobileParameter;//中心下发的参数缓冲结构
#endif
typedef struct{
	unsigned char MSGtypeID[2];//BCD MSG-TYPE-ID 0800
	unsigned char BIT_MAP[8];//B BIT MAP
	unsigned char MSG11[6];//BCD 收单方系统跟踪号
	unsigned char MSG12[6];//BCD 收单方所在地时间
	unsigned char MSG13[8];//BCD 收单方所在地日期
	unsigned char MSG40[16];//ASCII 终端序列号
	unsigned char MSG41[8];//ASCII 收单方终端标识码=POS终端号
	unsigned char MSG42[15];//ASCII 受卡方标识码=商户号
	unsigned char MSG60_LLL[2];//BCD BM#60自定义数据长度 = 0072
	unsigned char MSG60_1[16];//CN 应用标识AID 变长，5～16个字节，后补空格（0X20）
	unsigned char MSG60_2[14];//CN 应用版本号 格式：YYYYMMDDHHMMSS
	unsigned char MSG60_3[2];//N KEK密钥索引号 00~02
	unsigned char MSG60_4[14];//CN 参数版本号 格式：YYYYMMDDHHMMSS
	unsigned char MSG60_5[2];//CN POS消息类型码 F0－FF(F0)
	unsigned char MSG60_6[2];//CN POS网管消息类型码 00－FF 01:签到	
	unsigned char MSG60_7[6];//CN 批次号 
	unsigned char MSG60_8[8];//CN 操作员号 变长，2～8个字节，后补空格（0X20）
	unsigned char MSG60_9[8];//B 保留 
//	unsigned char MSG64[8];//MAC 
}stSingIn;
typedef struct{
	unsigned char MSGtypeID[2];//BCD MSG-TYPE-ID 0800
	unsigned char BIT_MAP[8];//B BIT MAP
	unsigned char MSG11[6];//BCD 收单方系统跟踪号
	unsigned char MSG12[6];//BCD 收单方所在地时间
	unsigned char MSG13[8];//BCD 收单方所在地日期
	unsigned char MSG15[4];//BCD 清算日期 MMDD
	unsigned char MSG60_LLL[2];//BCD BM#60自定义数据长度 = 0072
	unsigned char MSG60_1[16];//CN 应用标识AID 变长，5～16个字节，后补空格（0X20）
	unsigned char MSG60_2[14];//CN 应用版本号 格式：YYYYMMDDHHMMSS
	unsigned char MSG60_3[2];//N KEK密钥索引号 00~02
	unsigned char MSG60_4[14];//CN 参数版本号 格式：YYYYMMDDHHMMSS
	unsigned char MSG60_5[2];//CN POS消息类型码 F0－FF(F0)
	unsigned char MSG60_6[2];//CN POS网管消息类型码 00－FF 01:签到	
	unsigned char MSG60_7[6];//CN 批次号 
	unsigned char MSG60_8[8];//CN 操作员号 变长，2～8个字节，后补空格（0X20）
	unsigned char MSG60_9[8];//B 保留 
	unsigned char MSG63_LLL[2];//BCD BM#63自定义长度 
	unsigned char MSG63_1[3];//BCD 借记总笔数 
	unsigned char MSG63_2[12];//BCD 借记总金额
	unsigned char MSG63_3[3];//BCD 贷记总笔数
	unsigned char MSG63_4[12];//BCD 贷记总金额
	unsigned char MSG63_5[3];//BCD 转帐总笔数
	unsigned char MSG63_6[12];//BCD 转帐总金额
	unsigned char MSG64[8];//MAC 
}stSendSettlement;//结算
typedef struct{
	unsigned char MSGtypeID[2];//BCD MSG-TYPE-ID 0800
	unsigned char BIT_MAP[8];//B BIT MAP
	unsigned char MSG11[6];//BCD 收单方系统跟踪号
	unsigned char MSG12[6];//BCD 收单方所在地时间
	unsigned char MSG13[8];//BCD 收单方所在地日期
	unsigned char MSG15[4];//BCD 清算日期 MMDD
	unsigned char MSG60_LLL[2];//BCD BM#60自定义数据长度 = 0072
	unsigned char MSG60_1[16];//CN 应用标识AID 变长，5～16个字节，后补空格（0X20）
	unsigned char MSG60_2[14];//CN 应用版本号 格式：YYYYMMDDHHMMSS
	unsigned char MSG60_3[2];//N KEK密钥索引号 00~02
	unsigned char MSG60_4[14];//CN 参数版本号 格式：YYYYMMDDHHMMSS
	unsigned char MSG60_5[2];//CN POS消息类型码 F0－FF(F0)
	unsigned char MSG60_6[2];//CN POS网管消息类型码 00－FF 01:签到	
	unsigned char MSG60_7[6];//CN 批次号 
	unsigned char MSG60_8[8];//CN 操作员号 变长，2～8个字节，后补空格（0X20）
	unsigned char MSG60_9[8];//B 保留 
	unsigned char MSG64[8];//MAC 
}stSendTradeBatch;//批上送命令
//--TLV
typedef struct  
{
	unsigned char DAP_FF[3];//0xff 0x01 + 交易类型 0x02: 参数传递
	unsigned char DAP_28[18];//0x28 16+POS终端序列号(16)
	unsigned char DAP_29[10];//0x29 8+POS终端号(8)
	unsigned char DAP_2A[17];//0x2a 15+商户编号(15)
	unsigned char DAP_90[18];//90 16+应用标识符AID(16)
	unsigned char DAP_aa[16];//0xaa 14+终端参数版本号 YYYYMMDDhhssmm
}stDownLoadAppParameter1;//参数下载命令 POS终端在参数初始化的请求中不带POS终端号，但在参数更新时要带POS终端号；
						//POS终端在参数初始化时，终端参数版本号为全0，在参数更新时，应带上当前POS终端的参数版本号。
typedef struct  
{
	unsigned char DAP_FF[3];//0xff 0x01 + 交易类型 0x02: 参数传递
	unsigned char DAP_28[18];//0x28 16+POS终端序列号(16)
	unsigned char DAP_90[18];//90 16+应用标识符AID(16)
	unsigned char DAP_aa[16];//0xaa 14+终端参数版本号 YYYYMMDDhhssmm
}stDownLoadAppParameter0;//初始化参数,不带POS终端号

typedef struct{
	unsigned char DAP_FF[3];//0xff 0x01 + 类型 0x03:程序下载
	unsigned char DAP_FE[3];//0xfe 1 + 命令 0x01:程序下载
	unsigned char DAP_28[18];//0x28 16+POS终端序列号(16)
	unsigned char DAP_90[18];//90 16+应用标识符AID(16)
	unsigned char DAP_FC_TAG;//0xfc 任务表 FC + 27 + 数据
	unsigned char DAP_FC_LEN;//27
	unsigned char DAP_FC_1[2];//任务编号 0x0001-0xffff
	unsigned char DAP_FC_2;//任务类型 0x00: 应用程序
	unsigned char DAP_FC_3[14];//版本号 应用程序版本号,初始化时设为0
	unsigned char DAP_FC_4;//动作 0x00:覆盖；0x01：增加；0x02：删除
	unsigned char DAP_FC_5[4];//任务大小 应用程序文件大小（HEX表示）
	unsigned char DAP_FC_6[5];//任务下载情况 1B 完成百分比（0x00-0x64）+ 4B 数据在文件中偏移
}stDownloadApplication;//应用程序请求下载结构
typedef struct{
	unsigned char DAP_FF[3];//0xff 0x01 + 类型 0x03:程序下载
	unsigned char DAP_FE[3];//0xfe 1 + 命令 0x02:程序下载
	unsigned char DAP_28[18];//0x28 16+POS终端序列号(16)
	unsigned char DAP_90[18];//90 16+应用标识符AID(16)
	unsigned char DAP_FC_TAG;//0xfc 任务表 FC + 27 + 数据
	unsigned char DAP_FC_LEN;//17
	unsigned char DAP_FC_1[2];//任务编号 0x0001-0xffff
	unsigned char DAP_FC_2;//任务类型 0x00: 应用程序
	unsigned char DAP_FC_3[14];//版本号 应用程序版本号,初始化时设为0
}stDownloadAppResultNotification;//应用程序下载结果通知请求结果
typedef struct{
	unsigned char DAP_FF[3];//0xff 0x01 + 交易类型 0x03: KEK传递
	unsigned char DAP_28[18];//0x28 16+POS终端序列号(16)
	unsigned char DAP_29[10];//0x29 8+POS终端号(8)
	unsigned char DAP_2A[17];//0x2a 15+商户编号(15)
	unsigned char DAP_90[18];//90 16+应用标识符AID(16)
}stInitializeForKEK;//KEK下载

typedef struct{
	unsigned char AID[16];//应用标识AID CN
	unsigned char dealType;//交易类型	0x01:充值 0x02:脱机消费 0x12撤销 0x13退货 CN
	unsigned char dealCardNo[10];//交易卡号	钱包卡号 N
	unsigned char dealMoney[6];//交易金额 N
	unsigned char dealDate[4];//交易日期yyyymmdd N
	unsigned char dealtime[3];//终端交易时间 hhmmss N
	unsigned char posSerial[3];//POS终端交易流水号 N

	unsigned char Cardtag[8];//用来唯一标识发卡方的一个数字 CN
	unsigned char KEYflagNO;//密钥索引号 CN
	unsigned char KEYVer;//密钥版本号 N
	unsigned char TAC[4];//消费充值TAC B
	unsigned char purseMoney[4];//消费交易金额 B
	unsigned char pursestyle;//交易类型标识 B
	unsigned char deviceNo[6];//POS终端号 CN
	unsigned char PSAMdealSerial[4];//终端交易序号	PSAM卡产生 B
	unsigned char date[4];//消费交易日期（终端）CN
	unsigned char time[3];//消费交易时间（终端）CN
	unsigned char afterMoney[4];//钱包余额（交易后余额）B
}stMobileRecord;
#define MOBILE_REC_LEN 128//每条移动消费记录长度

typedef struct{
	unsigned char MISS_Sreil[2];//任务编号
	unsigned char Miss_style;//任务类型
	unsigned char Miss_ver[14];//版本号
	unsigned char Miss_dongzuo;//动作 0x00:覆盖；0x01：增加；0x02：删除
	unsigned char Miss_len[4];//任务大小
	unsigned char Miss_pesent[5];//任务下载情况 1B 完成百分比（0x00-0x64）+ 4B 数据在文件中偏移
}stdownProgPara;//一载程序参数

typedef struct{
	unsigned char STX;
	unsigned char alen[2];//高位在前,低位在后
	unsigned char Data[512];//包括data+LRC+EXT(0x03)
}stG24ReadP;

typedef struct{
	unsigned short T;
	unsigned short L;
	unsigned char link;
	unsigned char V[1500];
}stTcpIpSp;//MG323发送数据缓存。发送数据先存在这里，如果发送多需要依次发送。回应，记录。

#define TcpIPFree 0xA5
#define TcpIPBusy 0x55

//车载机任务标志
#define MISS_IF1 0x10//需要登陆到IF1
#define MISS_M_SINGIN 0x11		//0x01 登陆移动,签到.
#define MISS_M_Settlement 0x12	//0x02 批结算
#define MISS_M_TradeBatch 0x13	//0x03 批上送交易
#define MISS_IF2 0x20//需要登陆到IF2
#define MISS_M_DPARAM 0x21		//0x01 下载参数
#define MISS_M_DPGRAM 0x22		//0x02 程序下载请求
#define MISS_M_DPGRAMOK 0x23	//0x03 程序下载结果通知
#define MISS_M_KEK 0x24			//0x04 KEK下载
#define MISS_M_ActiveTest 0x25	//0x05 回响

#define MISS_GJ 0x40	//需要登陆到公交
#define MISS_G_LOGINGJ 0x40		//0xF0 登陆公交后台
#define MISS_G_DBLKD 0x41		//0xF1 下载黑名单 减  现在是增减都在一个包里，不需要区分
#define MISS_G_UREC 0x42		//2 上传数据
#define MISS_G_FILES 0x43		//3 下载文件 程序,所有的文件下载都使用此命令
#define MISS_G_DLine	0x44		//3 下载文件 线路参数
#define MISS_G_DSound	0x45		//3 下载文件 语音参数
#define MISS_G_DPROOK	0x46	//下载程序结果
#define MISS_G_DBLKI 0x47		//0xF1 下载黑名单 加
#define MISS_G_DLineOK	0x48	//下载程序结果
#define MISS_G_DSoundOK	0x49	//下载程序结果
#define MISS_G_HART 0x4A		//心跳包
#define MISS_G_GPS 0x4B			//GPS massage
#define MISS_G_TOO 0x4C			//终端机告知指令 -2002: 终端机主动发送终端机基本信息给接收机。
#define MISS_G_ALAM 0x4D		//发送报警等GPS信息
#define MISS_G_PRICE 0x4E		//下载票价线路信息
#define MISS_G_FREE 0x4F		//连接到公交，但是空闲中

#define MISS_G_TREC 0x80		//实时处理的二维码数据

#define MISS_G_MSG1 0x60		//需要发送短信
#define MISS_G_MSG2 0x61		//需要发送内容

#define MISS_G_TMS	0x90

#define MISS_HTTP 0x80			//80以上的任务都是HTTP下载的
#define MISS_HTTP_BLK 0x81		//通过LINUX模块下载黑名单文件 
#define MISS_HTTP_EC20 0x82		//通过LINUX模块下载LInux模块程序 
#define MISS_HTTP_PRO 0x83		//通过LINUX模块下载车载机程序 

#define MISS_G_SINGOUT 0x70		//终端签退

#define MISS_COM_PARA 0x80		//串口下载程序中
//签到状态，0位：KEK需下载，1位：参数需下载，2位：程序需下载
#define LoadStyle_KEK (0x01<<0)
#define LoadStyle_para (0x01<<1)
#define LoadStyle_program (0x01<<2)
//--------------------------------------------------------------------------
extern stSoundInfo gSoundInfo;					//语音
extern void GPRS_INIT(unsigned int band);
extern void GPS_PAR_CLR(void);
extern void gps_par_init(void);
extern void main_GpsRs(void);
extern void showGps(unsigned char mode);
extern void gprs_par_init(void);
//Rstr:返回的字符
extern unsigned char GetInSound(char *Rstr);
extern void SetSevice(void);
extern void ShowGprsStatus(void);
//查GPRS板数据，buf：要发送的数据，len：发送数据的长度
extern void GPRSSASK(unsigned char *dat, unsigned char cmd, unsigned int len);
extern void closeGPS(void);
#ifdef _GPS_DIAODU_
extern void Save_DIAODUCMD_info(void);
extern void Load_DIAODUCMD_info(void);
extern void save_DD_YinYun_info(void);
#endif
//extern void gps_config_hw(void);
extern unsigned char gpsTimeOK;

//extern volatile unsigned char gllstyle;//GLL计数，如果超过两次都没有，就认为是G595，发送配置信息.

extern Parameter2 POINT_MESSAGE;
//extern stLcdCmd gLcdCmd;
extern fFlagVary_1 F_FLAG_VARY;

#define GPS_RCV_BUFFER_SIZE 100

extern volatile unsigned char gucGPSindex;
extern unsigned char gps_RcvBuf[GPS_RCV_BUFFER_SIZE+1];	//GPS信息接收缓冲
#endif
