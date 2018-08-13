
#ifndef _Head_Struct_def_file_
#define _Head_Struct_def_file_
//用于ST程序的库函数据，所以不能随意更改
///////////////////PSAM卡的信息//////////
typedef struct
{
	unsigned char serial[10];
	unsigned char verNo;
	unsigned char KeyType;
	unsigned char DefineData[2];
	unsigned char DeriveNo[6];
}PSAM_MESSAGE;
#define CARDMESSAGE_LEN		69
#define P_FIRST_VARY_LEN	32
typedef struct 
{
	unsigned int BlackListNum;	
	unsigned char PostCode[4];			
	unsigned char DeviceNo[9];			
	unsigned char DeviceNo_1[9];		
	unsigned char LineNoOld[4];				
	unsigned char LineNo[3];
	unsigned char count;					//最近一张卡连续打的次数
	unsigned char fileSno[2];				//采集序列号	
}pFistVary_1;

#define LEN_F_FLAG_VARY		7	

typedef struct
{
	unsigned char listSector;			
	unsigned char publishSector;		
	unsigned char publicMoneySector;	
	unsigned char publicMessageSector;
	unsigned char dealRecordSector[3];	 //!@	
	unsigned char oddMonthSector;		//月票所在扇区	
	unsigned char dodMonthSector;	
	
	unsigned char PublishKEYA[6];		
	unsigned char PublichKEYA[6];		
	unsigned char MonthKEYA[6];		
	unsigned char MoneyKEYA[6];
	unsigned char RecodeKEYA[6];
	
	unsigned char CityCode[2];			
	unsigned char TradeCode[2];	
	unsigned char PublishBicker[10];
	unsigned char PublishBickerH[4];
	unsigned char CardCheckCode[4];			
	unsigned char card_catalog;
	unsigned char Zicard_catalog;
	unsigned char MoneySectorType;	
	
	
	unsigned char catalogPOST;//目录所在扇区，云平台中使用
	
	unsigned char PbocTac[4];	
	unsigned char stuffNO[4];//发行区的发行日期，司机卡中用于员工编号
	
	unsigned char getdata_w;
	unsigned char getdata_flag;
	unsigned char WriteRecord_dis;
	unsigned char ProcessFlag;

	unsigned char FactorFlg[8];//发卡方标识
}Parameter5;
typedef struct 
{
	unsigned char packetStatus;			//包的状态
	unsigned short length;				//发送包长度
	unsigned char TTL;					//生存时间
	unsigned short Serial;				//发送序列号
	unsigned char tcpBuffer[250];		//最大可发送7个记录
}TCP_IP_PACKET1;

#define BIT_PACK_SERIAL			0
#define BIT_MONEY_POINT			2
#define BIT_MONTH_POINT			6
#define BIT_CONTROL_POINT		10
typedef struct 
{
	unsigned short PackSerial;			//GPRS发送包的序列号
	unsigned int MoneyDealPointer;		  
	unsigned int MonthDealPointer;		
	unsigned int ControlDealPointer;	
	unsigned char crc;					//校验
}Parameter2;

//***************************************************************************
//                       公共扇区的格式定义 
//***************************************************************************
#define CARD_PULICH_MESSAGE_LEN		8
#define Public_dealpo 0
#define Public_moneydealsum 1
#define Public_delaflag 3
#define Public_monthdealsum 4
#define Public_blackflag 6
#define Public_fileflag 7
/*typedef struct
{
	unsigned char DealPo;		//卡交易指针	0
	unsigned char MoneyDealSum[2];	//钱包交易次数  1
	unsigned char DelaFlag;		//交易进程标志  3
	unsigned char MonthDealSum[2];	//月票交易次数  4
	unsigned char BlackFlag;	//黑名单标志    6
	unsigned char FileFlag;		//文件表示      7
}Parameter8;
*/


#define BIT_SUM_MONEY		4
#define BIT_SUM_TI			0
#define BIT_SUM_MTI			2
#define BIT_CRC_BYTES		8
#define BIT_CUR_E2PROM		9
#define BIT_BAK_CRC			13
#define BIT_C_CUR_PO		14
//
#define BIT_HEAD_ADDRESS	20
#define BIT_HEAD_CRC		28
#define BIT_BAK_HEAD_ADDRESS	24
#define BIT_BAKC_HEADCRC		29

#define BIT_RECORD_COUNT	30

#define COMSUM_MESSAGE_FILE_LEN		32
typedef struct
{
	unsigned short SumTi;			//当前消费次数	0
	unsigned short SumMti;			//当前月票卡刷卡次数 2
	unsigned int SumMoney;			//当前消费总额  4
	
	unsigned char BR_Crcbyte;			//当前指针的校验位 8
	unsigned char BR_BAK_crc;			//备份E2PROM的指针的校验位 9
	unsigned char resever[2];
	unsigned int BR_curE2pointer;		//E2PROM的当前指针 12
	unsigned int BR_c_cur_po;			//备份E2PROM的指针 16
	
	unsigned int BR_HeadAddress;		//起始地址 20
	unsigned int BR_BAK_HeadAddress;  //备份开始地址24
	unsigned char BR_HeadCRC;			//开始地址的校验码28
	unsigned char BR_BACK_HeadCRC;		//备份开始地址的校验码29
	unsigned short recordCount;		//记录E2PROM中总共有的记录条数 30
}Parameter3;
#define LAST_LIST_LEN			50				//最大保存卡数量
#define POINT_MESSAGE_LEN		17
#define RECORD_DEAL				0xba
#define RECORD_NONE				0

/*typedef struct
{
	unsigned char stop_flag;			//停车状态	1(停)0(运行)
	unsigned char PlusSta;				//是否收取附加费		
	unsigned char set_device_status;	//设备卡刷卡标志
	unsigned char set_time_status;		//时间设置卡刷卡标志
	unsigned char set_line_status;		//线路卡刷卡标志
	unsigned char WriteCardFlag;
	unsigned char verrrr;
}fFlagVary_1;

typedef struct
{
	unsigned char busProVer[2];//程序版本
	unsigned char busBLKVer[2];//黑名单版本
	unsigned char busLineVer[2];//站点限速信息版本2B
	unsigned char busVoiceVer[2];//公钥信息版本2B   如果有，需要转成16进制数
	
	unsigned char newbusProVer[2];//程序版本
	unsigned char newbusBLKVer[2];//黑名单版本
	unsigned char newbusLineVer[2];//线路信息版本
	unsigned char newbusVoiceVer[2];//语音文件版本
}stBusVerIfo;*/

//当黑名单。程序下载中止后重新发起的延时，单位：秒
#define GPRS_Login_dly  40
typedef struct{
	unsigned char MISS_flag;//当前是否有效标志
	unsigned char Miss_style;//任务类型，GnCode
	unsigned char MISS_MAXPack[4];//最大包号
	unsigned char Miss_ver[2];//程序版本号
	unsigned char Miss_ALL_LEn[4];//程序总字节数
	unsigned char Miss_StartData[7];//程序启动日期时间
	unsigned char Miss_PPack[2];//正在更新的包序号
	unsigned char Miss_Time;//最后程序通讯时间
	unsigned char Miss_rever[5];//备用
}stbusdownpro;//一载程序参数 	最大包号 4B+程序版本 2B +程序总字节数据4B+程序启用日期4B+程序启用时间3B

typedef struct{
	unsigned char Miss_cmdCode[2];//CMDCODE，说明此数组当前功能，黑名单、线路
	unsigned char Miss_style;//任务类型，GnCode
	unsigned int MISS_MAXPack;//最大包号
	unsigned char Miss_ver[2];//下载版本号
	unsigned int Miss_ALL_LEn;//需下载总字节数
	unsigned short Miss_PPack;//正在更新的包序号
	unsigned char Miss_time;
}stbusdownPara;//下载参数 	

typedef struct{
	unsigned char MISS_flag;//当前是否有效标志
	unsigned char Miss_style;//任务类型，GnCode
	unsigned char MISS_MAXPack[4];//最大包号
	unsigned char Miss_ver[2];//程序版本号
	unsigned char Miss_ALL_LEn[4];//程序总字节数
	unsigned char Miss_StartData[7];//程序启动日期时间
	unsigned char Miss_PPack[2];//正在更新的包序号
	unsigned char Miss_Time;//最后程序通讯时间
	unsigned char Miss_Line[2];//线路
	unsigned char Miss_rever[3];//备用
}stGPSdownLine;//一载程序参数 	最大包号 4B+程序版本 2B +程序总字节数据4B+程序启用日期4B+程序启用时间3B

// typedef struct{
// 	unsigned char MISS_Sreil[2];//任务编号
// 	unsigned char Miss_style;//任务类型
// 	unsigned char Miss_ver[14];//更新完成后最终的黑名版本号
// 	unsigned char Miss_filever[14];//黑名单正在更新的文件
// 	unsigned char Miss_len[4];//任务大小
// 	unsigned char Miss_pesent[4];//任务下载情况 1B 完成百分比（0x00-0x64）+ 4B 数据在文件中偏移
// 	unsigned char MISS_UNIT;//任务项，指明是什么任务
// }stbusblkdownPara;//一载黑名单任务表

typedef struct{
	unsigned char Single[8];//8个字节的标志说明
	unsigned int StartAddr;//首地址
	unsigned int SendAddr;//已经发送到的地址
	unsigned int EndAddr;//需要发送到的结束地址，在批结算时使用,不是批结算时使用gMobilREC_End
	unsigned int all_number;//已经发送的条数
	unsigned int all_money;//已经发送成功的统计金额
	unsigned int SendPiCI;//此批次号
	unsigned char BCC;//校验
}stMobilSend;//移动发送参数结构 共２９字节

//*****************错误编码标识定义(共32位)******
#define ERROR_GPRS		(0x0001)	//GPRS模块错误
#define ERROR_SAM		(0x0002)	//SAM卡出错
#define ERROR_PSAM1		(0x0004)	//PSAM1出错    使用了
#define ERROR_PSAM2		(0x0008)	//PSAM2出错    使用了
#define ERROR_DEVICE	(0x0010)	//设备号错误
#define ERROR_LINENO	(0x0020)	//线路号错误
#define ERROR_DATETIME	(0x0040)	//时间错误
#define ERROR_24READER	(0x0080)	//时间错误    使用了

#define ERROR_CONNECT_MOBILE	(0x0100)	//连接移动错误
// #define ERROR_LOGIN_MOBILE		(0x0200)	//登陆移动平台错误
// #define ERROR_TRANSFER_MOBILE	(0x0400)//传送移动数据错误
// #define ERROR_DUIZANG_MOBILE	(0x0800)//移动对帐不平

#define ERROR_BUS_CONNECT		(0x1000)//连接公交错误
// #define ERROR_BUS_LOGN			(0x2000)//登陆公交错误
// #define ERROR_BUS_TRANSFER		(0x4000)//上传公交数据错误

#define Gmode_900C	0
#define Gmode_MG323 1
#define Gmode_MC323 2
#define Gmode_OTHER 3
#define Gmode_WIFI_TLH 4

#define Gmode_MODEM 0x80

//报警
#define ALARM_YY			0x01	//进入运营
#define ALARM_TC			0x02	//退出运营
#define ALARM_TH			0x03	//请求通话
#define ALARM_GZ			0x04	//车辆故障
#define ALARM_DQ			0x05	//盗抢
#define ALARM_YJ			0x06	//拥挤
#define ALARM_SC			0x07	//塞车
#define ALARM_SG			0x08	//事故
#define ALARM_JF			0x09	//纠纷
#define ALARM_XX			0x0A	//休息
#define ALARM_LY			0x0B	//车辆另用

typedef struct 
{
	unsigned char centry; //BCD码
	unsigned char year;   //BCD码
	unsigned char month;  //BCD码
	unsigned char day;    //BCD码
	unsigned char hour;   //BCD码
	unsigned char min;    //BCD码
	unsigned char sec;    //BCD码
}TimeGPS;
typedef struct tag_GPS_INFO
{
	unsigned int Latitude;		//纬度	[度][分][分小数][分小数]		如果是南纬,则纬度最高位置1
	unsigned int Longitude;	 //经度	[度][度][分][分小数][分小数]	如果是西经,则经度最高位置1
	unsigned int velocity;		//速度
	TimeGPS UTCtime;			//UTC 时间 [时][分][秒][秒小数] BCD码
	unsigned char SataletNum;	//卫星数量
	unsigned int uiCog;		//航向
	unsigned char ucValid;		//数据是否有效 1=有效，0=无效
	unsigned char ucState;		//GPS装态
}GPS_INFO;
typedef struct
{
	unsigned char EnableGps;			//禁止GPS信息 0禁止 1允许
}stGpsPar;

typedef struct  {
	unsigned short uiVer;		//版本号
	unsigned int uiGaddr;		//起始地址
	unsigned char usNum;		//语音总数
	unsigned int uiSaddr[50];	//每条语音的起始地址
	unsigned char index;        //正在放第几段
}stSoundInfo;

#define GPRS_SISW 0xE1
#define GPRS_SISR 0xE2
#define GPRS_SISRing 0xE3
typedef struct{
	unsigned char ucSendstyle; //GPRS数据状态 0xE1=需要发送数据内容 0xE2=需要发送取数据命令
	unsigned char ucReadStyle;
	unsigned int uiNum;	//需要发送的长度
}stGPrsDataStyle;

#define LINEINFO_ROW_LEN	32	//线路信息每条长度
#define STATION_MAX_COUNT	50
#define LINEINFO_HEAD_LEN	10	//线路头长度
typedef struct{
	unsigned char ucId;			//站点编号
	unsigned int ulLongitudeIN;	//进站点经度
	unsigned int ulLatitudeIN;	//进站点纬度
	unsigned int ulHxiangIN;	//航向
	unsigned int ulLongitude;	//站点经度
	unsigned int ulLatitude;	//站点纬度
	unsigned int ulHxiang;	//航向
	unsigned int ulLongitudeOUT;//站点经度
	unsigned int ulLatitudeOUT;	//站点纬度
	unsigned int ulHxiangOUT;	//航向
	char name[23];				//站点名称
	unsigned char Sstyle;		//报站性质0 进站，1 转弯
}stStationInfo;					//站点信息结构 32

#define SPEED_MAX_COUNT 50
typedef struct{
	unsigned char ID;//序号
	unsigned char lineNo[3];
	unsigned char ucDirect;		//(正0反1)
	unsigned int ulMaxSpeed;	//最大速
	unsigned char ucStationMax;
	unsigned int uiVer;		//线路信息版本
	stStationInfo StationInfo[STATION_MAX_COUNT];//一条线路保存50个站点信息
}stLineInfo;
/*
typedef struct
{
	unsigned char old_Purcs_Time[7];
	unsigned char old_Purcs_LineNum[3];
	unsigned char old_Purcs_Device[4];
	unsigned char resear[2];
}oldPursIfo;*/
typedef struct{
	unsigned char ucId;			//编号
	unsigned short usRDNo;		//路段编号
	unsigned short usTIme1;		//时段一
	unsigned short usNSpeed1;	//限速一
	unsigned short usTIme2;		//时段二
	unsigned short usNSpeed2;	//限速二
	unsigned short usTIme3;		//时段三
	unsigned short usNSpeed3;	//限速三
	unsigned int ulLongitude;//站点经度
	unsigned int ulLatitude;	//站点纬度
	unsigned int ulHxiang;	//航向
}stNSpeedInfo_inf;				//限速点结构 32

typedef struct{
	unsigned char lineNo[3];
	unsigned char ucDirect;		//(正0反1)
	unsigned char ucStationMax;
	unsigned int uiVer;		//线路信息版本
	stNSpeedInfo_inf SpeedPInfo[SPEED_MAX_COUNT];//一条线路每个方向保存50个限速点信息
}stNSpeedInfo;

typedef struct{
	unsigned char ucId;			//站点编号
	unsigned char ulLongitude[4];	//经度
	unsigned char ulLatitude[4];	//纬度
	char name[22];				//站点名称
	unsigned char Sstyle;		//报站性质0 进站，1 转弯
}stStationInfoT;					//站点信息结构 32
typedef struct{
	unsigned char ID;//序号
	unsigned char lineNo[3];
	unsigned char ucDirect;		//(正0反1)
	unsigned char ulMaxSpeed[4];	//最大速
	unsigned char ucStationMax;
	unsigned char uiVer[4];		//线路信息版本
	stStationInfoT StationInfo[STATION_MAX_COUNT];//一条线路保存50个站点信息
}stLineInfoT;

typedef struct{
	unsigned char ucCMD;//指令代码
	unsigned char ZLseiral[2];//指令流水,调度编号
	unsigned char ucZiLine[4];//子线路编号
	unsigned char ucBUSNo[4];//汽车编号
	unsigned char FCTime[2];//发车时分
	unsigned char StartSno;//起始站号
	unsigned char EndSno;//终点站号
	unsigned char Date[4];//此调度的日期
	unsigned char ucDDstyle;//是否需要显示
	unsigned char ucStartYorN;//是否停运
	unsigned char ref[3];//备用。
}stDiaoDuCMD;

//调度营运信息处理
//当前限速、人次、
typedef struct{
	unsigned int uiALLLiCeng;	//里程
	unsigned int licLON;	//里程经度
	unsigned int licLA;		//里程纬度
	unsigned int licJD;		//上次里程角度，如果角度差太多就加上里程
	unsigned char limitcounter;//超速时间记数
	unsigned char nearS;//最近的限速点
	unsigned char REFFF;//保留
	unsigned char limitID;	//当前限速的站点号
	unsigned char limitRDNo[2];//限速路段
	unsigned char licengTime[7];//里程生成时间
	unsigned char licCounter;	//里程计数 计到5累加一次
	unsigned char RenCIUp;		//人次
	unsigned char RenCiDown;	//人次
	unsigned char overSpeedStart[7];//超速起始时间
	unsigned char overSpeedRDNO[2];//超速路段编号2B
	unsigned char ONRun;		//是否营运代码1B
	unsigned char CRC16[2];		//校验
}stDiaoDuinfo;

typedef struct{
	unsigned char alamFlag;//报警标识
	unsigned char style;//状态
	unsigned char reSendTimes;//重发次数
}stAlammessage;

//备份指针结构
typedef struct{
	unsigned int startAddr;
	unsigned int endAddr;
	unsigned char dateTIme[7];
	unsigned char Reserv;
	unsigned int tCrc32;
}stBackRecPointer;
#define _FileNameNum (4096/sizeof(stBackRecPointer))
#endif

