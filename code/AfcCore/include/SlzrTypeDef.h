#ifndef SLZR_TYPE_DEF_H_
#define SLZR_TYPE_DEF_H_

//modify by lks

#define _debug_

#ifdef _debug_
#define MSG_LOG(...) printf(__VA_ARGS__)
#define BCD_LOG(b, l, m) debugdata(b, l, m)
#else
#define MSG_LOG(...)
#define BCD_LOG(...)
#endif



//记录空间2M
#define RECORD_SPACE_LENGH 0x200000


typedef signed char    SLZR_S8;
typedef unsigned char  SLZR_U8;
typedef signed short   SLZR_S16;
typedef unsigned short SLZR_U16;
typedef signed int     SLZR_S32;
typedef unsigned int   SLZR_U32;
typedef char           SLZR_CHAR;
typedef char*          SLZR_STRING;
typedef void           SLZR_VOID;
typedef void*          SLZR_LPVOID;
typedef unsigned long long int SLZR_U64;
typedef signed long long int   SLZR_S64;

typedef enum
{
	SLZR_FALSE = 0,
	SLZR_TRUE = 1
}SLZR_BOOL;

typedef struct slzr_buf_s
{
	SLZR_LPVOID pBuf;
	SLZR_U32 u32Size;

}slzr_buf_t;

typedef SLZR_S32 SLZR_CHN;




typedef struct {
	unsigned char year_h;
	unsigned char year_l;
	unsigned char month;
	unsigned char day;
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
}stcomtime;


#define SAM1 1
#define SAM2 2
#define SAM3 3
#define SAM4 4

#define PSAM_GJ SAM1
#define PSAM_YD SAM2

#define PSAM_TRUE  1
#define PSAM_FALSE 0

typedef struct {
	unsigned char SLot;			//此卡所在的卡座号
	unsigned char Flag;			//标识PSAM状态，是否正常。0正常，其它不正常。
	unsigned char CardNO[6];	//PSAM卡号
	unsigned char PSAMLiuShou[4];//PSAM返回的流水号
}stPsamInfo;




#define WHITELEN		 1000
#define WHITE_SNO_LEN		4 
#define FLASH_WHITE_END	  10000

// #define BLACKLEN     16000//20000//
// #define BLK_SNO_LEN	10	//一条黑名单长度
// #define FLASH_BLK_END 500000

#define BLK_SNO_LEN	10	//一条黑名单长度


#define shift_blk 10
#define NUM_blk  10      //黑名单文件每条所占的字节数

typedef int boolean;

#ifndef NULL
#define NULL (void *)0
#endif // !NULL

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

#ifndef FALSE
#define FALSE 0
#endif // !FALSE

#define ST_OK 0
#define ST_ERROR 1
//-----------------------------卡片类相关错误--------------------------------------
#define ERR_NO_PSAM			10		//找不到PSAM卡
#define ERR_SELECT_MW_ADF3	11		//选MW_ADF3文件
#define ERR_PINAUTH				12	//PIN认证错误
#define ERR_READ_MW_ADF3_EF01	13	//读MW_ADF3_EF01二进制文件出错
#define ERR_READ_RECORD_FILE	14
#define ERR_R_MW_ADF3_EF04		15	//读MW_ADF3_EF04记录文件出错
#define ERR_CMP_FILE			16 //比较文件出错
#define ERR_CMP_ADF3_EF04		17	//读两次MW_ADF3_EF04比较出错
#define ERR_R_MW_ADF3_EF06		18	//读采集采集序列号出错
#define ERR_R_MW_ADF3_EF05		19	//读读取黑名单的总条数出错
#define ERR_R_MW_ADF3_EF03		20	//读当前采集文件出错
#define ERR_R_MW_ADF3_EF02		21	//读历史采集文件出错
#define ERR_R_MW_ADF3_EF07		22	//读储值交易计数出错
#define ERR_R_MW_ADF3_EF08	    23	//读月票交易计数出错
#define ERR_R_MW_ADF3_EF09		24	//控制卡交易计数出错
#define ERR_CHEK_E2PROM_PO		50	//比较E2PROM的指针出错!
#define ERR_GET_KEY				51	//推导密钥
#define ERR_GET_RESPONSE		52	//取密钥应答	
#define ERR_INIT_PSAM			53	//PSAM 开始初始化
#define ERR_MW_ADF3_EF02		54	//读历史采集文件
#define ERR_UPDATE_BIN			55	//写二进制文件

#define ERR_MIFARE_LOAD_KEY		80	//MIFARE卡装载密钥
#define ERR_MIFARE_AUTHEN		81	//验证MIFARE卡密钥
#define ERR_MIFARE_READ			82  //读MIFARE卡出错
#define ERR_CARD_GESHI			83	//卡中数据校验出错
#define ERR_CARD_STOP_WORK		84	//该卡被停用
#define ERR_CARD_NO_SYSTEM		85	//该卡不是本系统所发的卡
#define ERR_CARD_NO_USE         86	//该没有启用	
#define ERR_MIFARE_WRITE		87	//写卡出错	
#define ERR_CARD_PUBLIC			88	//公共扇区出错
#define ERR_MONTH_BAD			89	//月票区的钱包格式出错
#define ERR_CARD_WHITE_BUS		90	//是白卡，还没有出售

#define ERR_READ_TIME			100	//读出时间出错
#define ERR_MONEY_SN			101	//钱包格式出错
#define ERR_CHARGE_MEONY		102	//请充值

#define SOUND_DEAL				120		//交易声音提示
#define SOUND_LEV_BUS			121		//发车卡声音提示
#define SOUND_ARV_BUS			122		//停车卡声音提示
#define SOUND_CHARGE_MONEY		123		//请充值提示
#define SOUND_FLING_MONEY		124		//请投币提示
#define DEAL_NO_FINISH			125		//交易没有完成提示
#define SOUND_DEAL_FREE			126		//自由消费提示(不扣钱和次)
#define SOUND_DI_BUS			127		//蜂鸣器响

///////////////////////////////目录分区表/////////////////////////////////
#define LIST_AREA			0x00		//目录区
#define PUBLISH_AREA		0x01		//发行区
#define DEALRECORD_AREA		0x03		//交易记录区3
#define PBULICMESSAGE_AREA	0x06		//公共信息区
#define ODDMONTH_AREA		0x07		//单月票区
#define DODMONTH_AREA		0x08		//双月票区
#define PUBLICMONEY_AREA	0x10		//公共钱包区
#define NOUSE_AREA			0xff		//未使用扇区

///////////////////////////////交易类型/////////////////////////////////
#define ID_REC_TOLL			2			//钱包消费
#define ID_REC_MON			3			//月票消费
#define ID_REC_MOBILE		4			//移动记录,(原来数据采集记录不用)
#define ID_REC_TIMESET		5			//时间设置
#define ID_REC_TIMEDELY		6			//时延设置
#define ID_REC_DEVNOSET		7			//车牌号设置
#define ID_REC_PAKAGE		8			//数据包分隔
#define ID_REC_DATE			9			//日期变更
#define ID_REC_DRIVENO		10			//司机卡记录
#define ID_REC_CLERK		11			//站务员卡记录
#define ID_REC_LINESET		12			//线路设置卡
#define ID_REC_BLKLIST		13			//黑名单卡刷卡
#define ID_REC_AUTHOR		14			//设备授权刷卡
#define ID_REC_DTRAN2		15			//数据采集
#define ID_REC_DEVSET		16			//设备号设置
#define ID_REC_AIRCON		17			//空调启动卡
#define ID_REC_MONSTT		18			//月票启动卡
#define ID_REC_LEVBUS		19			//发车卡
#define ID_REC_ARVBUS		20			//到站卡
#define ID_REC_TOLLSTT		21			//收费启动卡
#define ID_REC_DEVNOSET1	22			//车牌号号设置
#define ID_REC_NO			23			//包头文件 
#define ID_REC_FUEL1		24			//加油记录1
#define	ID_REC_FUEL2		25			//加油记录2		
#define ID_REC_LINENO		26			//设置线路号	
#define ID_REC_FDDown		27			//分段收费中的下车记录
#define ID_REC_FDUp			28			//分段收费中的上车记录
#define ID_REC_PURSE		80			//钱包充值
#define ID_REC_MONTH		81			//月票充值
#define ID_REC_DXBAOSI		90			//沈阳宝石刷卡记录
#define ID_REC_CARDLOCK		99			//琐卡交易

#define ID_REC_CS456_ASCii	101			//0x65 北京考勤记录类型 #ifdef CS456_ASCii_

#define ID_REC_GPS			0xE0		//GPS记录 站台记录
#define ID_REC_GPS_OF		0xE3		//GPS记录 反向进站
#define ID_REC_GPS_OZ		0xE4		//GPS记录 反向出站
#define ID_REC_GPS_JZ		0xE5		//GPS记录 正向进站
#define ID_REC_GPS_JF		0xE6		//GPS记录 正向出站
#define ID_REC_START		50			//车载机自检

#define YCD_ID_TOLL			0x01
#define YCD_ID_MONTH        0x47


#define RET_MONEY_SN_EROR 20	//该卡的钱包扇区格式全错



///////////////////////////////固定长度定义/////////////////////////////////
#define CARD_MESSAGE_LEN		8
#define CITY_CODE_LEN			2
#define TRADE_CODE_LEN			2
#define PUBLISH_BICKER_LEN		4
#define CARD_CHECH_CODE_LEN		4
#define PRIVATE_PIN_LEN			6

///////////////////////////////按键常量定义/////////////////////////////////
#define NOT_BLACK				0
#define IS_BLACK				1

//////////////////////进程标志说明/////////////////////////////////
#define PRO_RESET				0		//将进程标志复位	
#define PRO_WRITE_MAIN_RECORD	0x10		//写主记录到E2PROM
#define PRO_WRITE_BACK_RECORD	0x8A		//写备份到E2PROM
#define PRO_WRITE_PSAM_POINTER1	0x77		//写各种指针到PSAM中
#define PRO_WRITE_PSAM_POINTER2	0x78
#define PRO_COUNT_MAC			0x79

#define WR_MONEY				0x90
#define WR_MONTH				0x91
#define WR_CONTROL				0x92	
#define REQUEST_TWO				0x98
///////////////////////错误处理常数定义/////////////////////////////

//***************************************************************************
//                       卡交易错误纠正的格式定义 
//***************************************************************************
#define IS_EFFECT					0x55	//错误信息有效	
#define IS_EFFECT_MONEY				0x77	//操作钱出错
#define IS_EFFECT_MONTH				0x88	//操作月票出错

#define OPER_TRANSFER_NO			0xa0	//在金额操作时执行mif_transfer不成功
#define OPER_RECORD_PROFILE			0xa1	//记录操作失败
#define OPER_PUBLIC_MESSAGE			0xa2	//写公共信息失败		

#define OPER_BLACK					0xa3	//操作黑名单
#define OPER_PUBLIC_MESSAGE_W		0xa4	//写公共信息出错
#define ERROR_MONTH					0x80
#define ERROR_MONEY					0x82
#define ERROR_BLACK					0x83
#define OPER_CHECK_MEONY			0x56	//验钱	
#define ERROR_DETAIL_LEN			32

#define ERROR_DETAIL_SUM			3
//////////////////////////////启用标志/////////////////////////////////
#define STARTUP_OK		0x02	//正常
#define STARTUP_NO		0x01	//没有启用		
#define STARTUP_STOP	0x03	//停用		
#define STARTUP_BLACK	0x04	//黑名单		

///////////////////////////////各种卡的运行标志/////////////////////////////////
							//充值错误
#define CARD_CHAGEERR	0x4F
#define CARD_BLACK		0x50			
#define CARD_STOPWORK	0x51			
#define CARD_NO_SYSTEM	0x52			
#define CARD_MONEY		0x53			
#define MONEY_CARD		0x53			
#define INVALID_CARD	0x54			
#define MONTH_CARD		0x55			
#define CONTROL_CARD	0x56
#define CARD_BLACK_PRO	0x57
#define CARD_NO_TIME	0x58	
#define CARD_HUIFU		0x59
#define CARD_NO_USE		0x5a			
#define CARD_PUBLIC_BAD	0x5b
#define CARD_NO_USER1	0x5c
#define CARD_DATA_BAD	0x5d
#define CARD_WRITE_CARD	0x5e
#define CARD_YEAR		0x5f
#define CARD_FREE		0x60
#define CARD_NO_Busins	0x61

#define CARD_forbid		0x62


#if 0
//****************************************************************
//              下面变量需要存放到铁电存储器
//****************************************************************
#define BIT_IS_FIRST_TIME	5					//4  第一次运行初始标志
#define BIT_F_FLAG_VARY		BIT_IS_FIRST_TIME+4	//7 发车到站状态
#define BIT_POINT_MESSAGE	BIT_F_FLAG_VARY+7	//17 交易流水信息
#define BIT_COMSUME_FILE	BIT_POINT_MESSAGE+17//32 消费信息文件
#define BIT_PRICETAB		BIT_COMSUME_FILE+32 //512 票价及语音信息文件 BIT_BASIC_MESSAGE

#define BIT_DEVICE_NO		BIT_PRICETAB+512	//9 设备终端号
#define BIT_DRICE_CARD	    BIT_DEVICE_NO+9		//4 司机卡号
#define BIT_LINENO			BIT_DRICE_CARD+4	//3 线路号
#define BIT_BLACK_SUM		BIT_LINENO+3		//4 黑名单
#define BIT_ERROR_DETAIL	BIT_BLACK_SUM+4		//32*3=96 刷卡错误标志
#define BIT_ERROR_POINT		BIT_ERROR_DETAIL+96	//1 刷卡错误指针
#define BIT_HISDATA_GATHER	BIT_ERROR_POINT+1	//210		历史采集文件
#define BIT_GET_FILESNO		BIT_HISDATA_GATHER+210//2 文件号
#define BIT_ZCYH			BIT_GET_FILESNO+2	// 3 转乘参数
//	//---不能连续刷卡时间表
#define BIT_INTERVAL_STAR	BIT_ZCYH+3	//1			开始指针
#define BIT_INTERVAL_END	BIT_INTERVAL_STAR+1		//1			结束指针
#define BIT_INTERVAL_DATA	BIT_INTERVAL_END+1		//500		保存时间表数据

#define FE_PACKSERIAL		BIT_INTERVAL_DATA+500 //2 TCP/IP包序列号
#define FE_REC_START		FE_PACKSERIAL+2		//4 记录首指针****************************************************
#define FE_REC_CURRENT		FE_REC_START+4		//4 记录当前指针****************************************************
#define FE_GPS_DIRECT		FE_REC_CURRENT+2		//9 GPS定位方向：标记+方向+时间
#define BIT_TCPIP_PACKET	FE_GPS_DIRECT+9    //500 要发送的IP包


//#define BIT_SETUP_AUDIO		BIT_TCPIP_PACKET+500//32 语音
#define BIT_SETUP_AUDIO		BIT_TCPIP_PACKET+500//20 没用
#define BIT_BAK_ADDR		BIT_SETUP_AUDIO+20	//12,首地址+尾地址+校验
#define BIT_BLACK_Ver		BIT_BAK_ADDR +12//32	黑名单版本号(CN) 没用，版本放在黑名单文件中,现在存放黑名单16块的条数，每两个字节一条

#define BIT_RE_SUM			BIT_BLACK_Ver+32	//4 发送总包
#define BIT_TCP_REV_SUM		BIT_RE_SUM+4//4 接收总包
#define BIT_GPRS_IP			BIT_TCP_REV_SUM+4//4 IP地址
#define BIT_GPRS_PORT		BIT_GPRS_IP+4//2 端口号
#define BIT_SEND_SUM		BIT_GPRS_PORT+2//4 总发送包数据

#define BIT_MobilStyle		BIT_SEND_SUM+4//60 签到状态
#define BIT_MobilPOSNS		BIT_MobilStyle+60//4 收单方终端系统跟踪号 POS流水
#define BIT_MobilREC_Start	BIT_MobilPOSNS+4//4 移动记录开始指针
#define BIT_MobilREC_End	BIT_MobilREC_Start+4//4 移动记录结束指针

#define BIT_MobilePRO_INFO	BIT_MobilREC_End+4//27 移动GPRS程序下载参数(任务)表
#define BIT_BUSDownPRO_INFO	BIT_MobilePRO_INFO+27//27 公交GPRS程序下载参数(任务)表
#define BIT_BUSblkDownPRO_INFO	BIT_BUSDownPRO_INFO+27//39 公交GPRS程序下载参数(任务)表

#define BIT_SendInfo		BIT_BUSblkDownPRO_INFO+40//44 上送记录信息区
#define BIT_PICINO_BAK		BIT_SendInfo+44			//7 批次备份
//发送时的序列号 //5字节，后一字节是校验码。
#define BIT_SendSeril		BIT_PICINO_BAK+7
//6 + 24 + 7 =37运行中的参数 + 找点参数 + 时间
#define BIT_GPS_RUN			BIT_SendSeril+6
//中英文存贮 1字节
#define BIT_CHN_ENG			BIT_GPS_RUN+38
//4 移动记录开始指针 备份
#define BIT_MobilREC_Start_BAK	BIT_CHN_ENG+1
//4 移动记录结束指针 备份
#define BIT_MobilREC_End_BAK	BIT_MobilREC_Start_BAK+4
//1 移动记录开始指针 备份校验
#define BIT_MobilREC_Start_BAK_CRC	BIT_MobilREC_End_BAK+4
//32 黑名单
#define BIT_BLACK_2BLOK BIT_MobilREC_Start_BAK_CRC+4
//27 公交GPRS程序下载参数(任务)表
#define BIT_GPSDownLine_INFO	BIT_BLACK_2BLOK+32
//4 pboc 记录流水号
#define BIT_PBOC_NS	BIT_GPSDownLine_INFO+32
//160  历史指针日期，每天更新一次历史指针和统计数据 日期时间7+[卡号+充值累计金额4+...]+CRC32(4)   8张卡
#define BIS_PERDat_Info (BIT_PBOC_NS+4)
//4 司机员工编号
#define BIT_STUFFER_NO (BIS_PERDat_Info+160)
//20 历史记录上传地址.用于GPRS传输历史记录指针.(首地址4+长度4+日期时间7+保留1+CRC324)=20字节,首地址是相对地址,要加上FLASH2_START1
#define BIT_HISREC_SND (BIT_STUFFER_NO+4) //2536
//8 累计刷卡计数 次数4+校验4
#define BIT_SWAPCARD_TIMES BIT_HISREC_SND+20
//8 设置的刷卡上限 次数4+校验4
#define BIT_SWTIMES_MAX BIT_SWAPCARD_TIMES+8
//28 月统计，保存前一个月和当前月的刷卡统计 2 * (年月2+刷卡总次4+刷卡总金额4+CRC32校验4) = 28字节
#define BIT_MOTH_TOJI_2 BIT_SWTIMES_MAX+8
//8 车载机禁用标识，在后台签到返回及心跳包中更新 目前只有银联卡是否启用的。"启"+"0x00"+标识1+CRC32校验4 = 8字节
#define BIT_Center_Ctrl_BU_Data (BIT_MOTH_TOJI_2+28)
//4 单位,部门编号
#define BIT_UNIT BIT_Center_Ctrl_BU_Data+8		
// 136灰记录存储，暂用128字节，“有效”4 + 128 data+4CRC = 136
#define BIT_UNKNOW_RECORD	BIT_UNIT + 4

//----------------------------------
//28(24) 调度命令信息  调度数据存在7168开始的地址，7K，铁电共8K能用1024字节
#define BIT_GPSDiaoDuCMD_INFO	7168
//65+7 语音或文字信息内容,可存64字节，32中文字 +7字节日期时间,还有个结束符
#define BIT_GPS_MESSAGE		BIT_GPSDiaoDuCMD_INFO+28
//48 调度营运信息
#define BIT_GPSDDYY_INFO	BIT_GPS_MESSAGE+72
//1 语音音量
#define BIT_GPSSOUND_V		BIT_GPSDDYY_INFO+48
//2 调度用到的子线路号
#define BIT_GPSChildLine	BIT_GPSSOUND_V+1
//4 键盘输入的临时票价
#define BIT_KeyPrice		BIT_GPSChildLine+2
//已经用到铁电的最大地址
#define BIT_FE_END_ADDR		BIT_KeyPrice+4	//7318
#endif
///////////////////////////////各种卡的定义/////////////////////////////////			
#define CARD_DATA_GATHER	61				//数据采集卡
#define CARD_DEV_SET		62				//设备设置卡
#define CARD_LINE_SET		63				//线路设置卡
#define CARD_NUMBER_SET		64				//数码设置卡
#define CARD_DELAY_SET		65				//延时卡
#define CARD_TOLL_START		66				//收费启动卡
#define CARD_TIME_SET		67				//时间设置卡
#define CARD_LEV_BUS		68				//发车卡
#define CARD_ARV_BUS		69				//到站卡
#define CARD_DRIVER_BUS		70				//司机卡
#define CARD_CLERK_BUS		71				//站务员卡
#define CARD_AIR_BUS		72				//空调卡
#define CARD_MONTH_STA		73				//月票启动卡

#define CARD_WHITE_BUS		50				//白卡	
#define CARD_WHITE_MOBILE	51				//手机白卡
#define CARD_WHITE_CPU		52				//CPU白卡

//////////////////////////用户卡的定义///////////////////////////
//用户卡 
#define CARD_NORMAL			0				//普通A卡
#define CARD_STUDENT		1				//学生A卡(九江 义务教育卡)

#define CARD_OLDMAN			2				//老年卡
#define CARD_STUFF			3				//员工卡
#define CARD_COUPON			4				//（邯郸 成人月票卡(市内)）
#define CARD_RELATION_BUS	5				//（邯郸 成人月票卡(市外)）
#define	CARD_HANDICAPPED	6				//残疾人卡 (九江低保卡) (邯郸 普通B卡)
#define CARD_NORMAL_B		7				// 邯郸 邯钢专线卡

#define CARD_CAP_BUS		8				//[邯郸 联通手机卡】

#define CARD_JILIAN_BUS		9				// (邯郸 建行卡)
#define CARD_YOUFU_BUS		10				//（邯郸 优抚卡）
#define CARD_17RD_BUS		11				//(九江 17路专用卡)
#define CARD_JTB			12				//市民卡/交通部的卡类
#define CARD_MOTH_CI		13				//邯郸次卡  可以随意刷
#define CARD_LIANGTONG_STUF 14              //手机员工卡


#define CARD_QRC_LTY 15              //蓝泰源二维码


#define  CARD_FENDUAN_Line	33		//分段线路票价卡





typedef enum
{
	CARDSTYLE_NULL = 0,
	//通用M1卡
	CARDSTYLE_NORM1,
	//CPU卡
	CARDSTYLE_CPU,
	//2.4G CPU卡
	CARDSTYLE_24CPU,
	//艺达M1卡
	CARDSTYLE_YIDAM1,
	//2.4G移动卡
	CARDSTYLE_24Mobile,	// 5
	   //2.4G电信卡
	   CARDSTYLE_24CT,
	   //中科讯联2.4G 联通
	   CARDSTYLE_24ZKXL,
	   //厦门盛华2.4G 联通、电信
	   CARDSTYLE_24XMSH,
	   //安龙老卡
	   CARDSTYLE_ANLONG,

	   SEND_FILE_ERROR_,	// 10
	   CARDSTYLE_JTB,
	   QPBOC_JTB,   //交通电子现金
	   CARDQR_CODE,	//二维码
	   CARDSTYLE_QR_PBOC,
	   CARDSTYLE_PBOC,
	   CARDSTYLE_QPBOC,
	   CARDSTYLE_UNPAY_ODA,	// 15
	   CARDSTYLE_QR_UNPAY_BUS,
}CardCand_enum;


#define ID_REC_QRC_GZT		0xA6		//贵州通二维码记录
#define CARDSTYLE_QR_TX			0xA4		//腾讯二维码记录
#define ID_REC_QRC_TX		0xA4		//腾讯二维码记录
#define ID_REC_QRC_ZFB		0xA5		//支付宝二维码记录
#define ID_REC_QRC_PBOC		0xA7		//银联二维码S
// A8是可充二维码
#define ID_REC_QRC_UNPAY_BUS	0xA9		//银联乘车码

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


//******************基本信息结构定义************	
#define LEN_CITY_CODE		0
#define LEN_DEVICE_NO		4
#define LEN_DEVICE_No1		13
#define LEN_LINE_NO			17
#define LEN_PRICE			20

#define RES_PUBLIC_D 0x01
#define RES_PUBLIC_FLAG 0x02
#define RES_PUBLIC_Z 0x04
#define RES_PUBLIC_F 0x08
#define RES_MONEY_D 0x10
#define RES_MONEY_Z 0x20
#define RES_MONEY_F 0x40

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
typedef struct
{
	unsigned char DealPo;		//卡交易指针	0
	unsigned char MoneyDealSum[2];	//钱包交易次数  1
	unsigned char DelaFlag;		//交易进程标志  3
	unsigned char MonthDealSum[2];	//月票交易次数  4
	unsigned char BlackFlag;	//黑名单标志    6
	unsigned char FileFlag;		//文件表示      7
}Parameter8;

typedef struct
{
	unsigned char old_Purcs_Time[7];
	unsigned char old_Purcs_LineNum[3];
	unsigned char old_Purcs_Device[4];
	unsigned char resear[2];
}oldPursIfo;



#define ERROR_DETAIL_SUM			3
#define ERROR_DETAIL_LEN			32
typedef struct
{
	unsigned char effFlag;		//错误类型，月票错，还是钱包错
	unsigned char Cardno[4];	//错误的卡号
	unsigned char ErrorStep;	//错误进度，扣钱【次】错，还是写公共区错
	unsigned char OpSector;		//没用，
	unsigned char OpBlock;		//没用，
	unsigned char TollValue[4];	//交易金额
	unsigned char AferToll[4];	//交易后余额
	unsigned char ToChange[16];	//公共信息区数据【写公共信息区错时，有用】
}Parameter6;

//00 00 00 04 00 12 34 56 78 1A 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
typedef struct
{
	unsigned char FCcode[2];	//发卡方代码
	unsigned char citycode[2];	//城市代码
	//	unsigned char algorithm;	//算法支持
	unsigned char tradcoed[2];		//行业代码
	unsigned char recre[2];			//保留 卡类和启用标识
	unsigned char QYflag;			//应用类型标识（启用标志 01启用）1
	unsigned char YYVer;			//应用版本1
	unsigned char FYFlag[2];		//互通标识2
	unsigned char YYNO[8];			//应用序列号8
	unsigned char StartDate[4];		//应用启动日期（YYYYMMDD） 4
	unsigned char EndDate[4];		//应用有效日期（YYYYMMDD） 4
	unsigned char rFu2[2];			//预留2
	unsigned char cardtype;		//卡类型
	unsigned char Ymoney;		//押金 单位：元
	unsigned char rFu3[5];		//预留
}stJSBF15;
typedef struct
{
	unsigned char cPublisherSign[8];	/* 发卡方标识 */
	unsigned char cAppType;				/* 应用版本号 */
	unsigned char cCitycode[2];			/* 城市代码	*/
	unsigned char cCardType;			/* 卡应用类型 */
	unsigned char cApp_sn[10];			/* 应用序列号 5个00H字节 + 1个卡片类型字节+ 4个流水字节 卡号为：后5个字节 */
	unsigned char cIssueDate[4];		/* 应用启用日期 */
	unsigned char cValidDate[4];		/* 有效日期 */
	unsigned char cSing;				/* 是否记名 */
	unsigned char cYaJin[2];			/* 押金 */
	unsigned char cMinremain[2];		/* 最低余额限制 */
	unsigned char cUseFlah;				/* 使用标记 */
	unsigned char cMaxPur[4];			/* 最高消费金额 */
	unsigned char cLinezf;				/* 联机账户标记 */
	unsigned char cRFU[9];				/* 保留 */
}typea_public_inf_zaozuang;				//枣庄CPU卡信息结构

typedef struct
{
	unsigned char cTermSnq[4];			/* 终端机交易序号 */
	unsigned char cDateTime[7];			/* 交易日期时间 */
	unsigned char cMac[4];				/* 交易MAC码 */
}pboc_debit_capp_purchase;

typedef struct
{
	unsigned char cBalance[4];			/* 电子钱包余额 */
	unsigned char cSnq[2];				/* 电子钱包交易序列号 */
	unsigned char cOverdraftLimit[3];	/* 透支限制 */
	unsigned char cKeyVer;				/* 密钥版本 */
	unsigned char cKeyEn;				/* 密钥标识 */
	unsigned char cRand[4];				/* 随机数 */
}pboc_init_capp_purchase_res;
typedef struct
{
	unsigned char cRand[4];				/* 随机数 */
	unsigned char cSnq[2];				/* 电子钱包交易序列号 */
	unsigned char cMoney[4];			/* 交易金额 */
	unsigned char cType;				/* 交易类型 */
	unsigned char cDateTime[7];			/* 交易日期时间 */
	unsigned char cKeyVer;				/* 密钥版本 */
	unsigned char cKeyEn;				/* 密钥标识 */
	unsigned char cKeyFactor[8];		/* 8密钥分散因子 */
	unsigned char cPublisherSign[8];		/* 8密钥分散因子 */
	unsigned char cReaderMAC[12];//读头MAC,用于PSAM卡校验读头是否合法
}pboc_sam_init_purchase;


#define LAST_LIST_LEN			50				//最大保存卡数量
#define POINT_MESSAGE_LEN		17
#define RECORD_DEAL				0xba
#define RECORD_NONE				0

struct pMonthTime_1	//保存不能连刷卡信息。
{
	unsigned char endptr;
	unsigned char startptr;
	unsigned char LastList[LAST_LIST_LEN][7];              //月票保存时间表
};

#define LEN_CARD_RECORD 16

typedef struct {
	int headp;	//头指针
	int endp;	//尾指针
	unsigned int crc;	//校验
}stFileRecordPoint;

typedef struct
{
	unsigned char DealTime[4];			//交易时间4
	unsigned char Balance[4];		    //交易前余额
	unsigned char DealMoney[3];			//交易金额
	unsigned char DealType;				//交易类型
	unsigned char DeviceNo[4];			//设备号
}cardRecord1;

#define  ON_BUS			3	//钱包上车
#define	 OFF_BUS		4	//钱包下车
#define  COVER_BUS		8 //钱包补票
#define  ON_Nomal		6//普通消费
#define FeRC_Write	1
#define FeRC_Dlelt	2
#define FeRC_Check	3

#define NO_CITY_UNION	0x63
#define APP_LOCK		0x64
#define NO_JTB_UNTION	0x65

#define NO_WriteNUM		0x80//不在白名单内
#define NO_START		0x81//未启用
#define NO_Ech			0x83//不是互联卡
#define NO_APP			0x84//未启用
#define PursAgian		0x85//补票
#define LocakCard		0x86//本地卡
#define YiDiCard		0x87//异地卡

#define ID_REC_HJY_REC		100			//灰交易记录类型



#define RECORD_LEN 64
#define RECORD_JTB_LEN 128

#define PBOC_RECORD_LEN 150  //上传的银行卡实际有效数据是150个字节（132+4+1+4）
#define QRC_RECORD_LEN 248  //上传的银行卡实际有效数据是248个字节（132+4+1+4）

#define TXQRCRECORD_LEN 256


//在车站上车必须是2 6  其他不允许上车
#define BUS_Star			(1<<0) //1 3 5 7刷发车卡 1 0XFE单数刷了发车	进站要清掉
#define BUS_InStation		(1<<1) //在车站了 2 5 6在车站  0XFD
#define BUS_ON				(1<<2) //有标识为 4 上车记录 0XFB
#define BUS_TES				(1<<7) //测试记录

#define BUS_CL_Star			0xFE //清掉发车卡
#define BUS_CL_InStation	0xFD//不在车站
#define BUS_CL_ON			0xFB//下车
#define BUS_CL_TES			0x7F//测试记录清除


typedef struct
{
	unsigned char cRand[4];				/* 随机数 */
	unsigned char cSnq[2];				/* 电子钱包交易序列号 */
	unsigned char cMoney[4];			/* 交易金额 */
	unsigned char cType;				/* 交易类型 */
	unsigned char cDateTime[7];			/* 交易日期时间 */
	unsigned char cKeyVer;				/* 密钥版本 */
	unsigned char cKeyEn;				/* 密钥标识 */
	unsigned char cPublisherSign[8];	/* 8卡应用序列号卡号后8字节*/
	unsigned char cKeyFactor[8];		/* 8发卡机构编码 */
	unsigned char cReaderMAC[12];//读头MAC,用于PSAM卡校验读头是否合法
}pboc_sam_init_purchaseJTB;
typedef struct//有数值大小区分的 一律为小端模式
{
	//29以下是当次刷卡的数据
	unsigned char rPublishNumb[10];		//BCD发行卡流水号卡号
	unsigned char rCardType;			//卡类
	unsigned char rCardCand;			//卡型:卡类型  M11卡  2 CPU卡 11 现金
	unsigned char rSAMxulie[4];			//PSAM终端交易序列号 (如果是云平台 填 我们设备自动生成的流水)
	unsigned char rPSAMNO[6];			//PSAM卡号72  BCD
	unsigned char rDeviceNo[4];			//设备号我们 机器的设备号（车牌号）(HEX)
	unsigned char rDealType;			//交易类型	HEX
	unsigned char rPiaoJia[2];			//票价 大端
	//20
	unsigned char rMoneyDealSum[2];		//钱包累计交易次数    应用交易计数器 9F36HEX
	unsigned char rDealMoney[3];		//交易金额 HEX 9F02
	unsigned char rAfterMoney[4];		//余额23大端
	unsigned char rDealTime[7];			//交易时间YYYY/MM/DD/HH/MM/SS 22个字节 
	unsigned char rCPUTAC[4];			//CPU交易认证码 *交易CPU卡返回的认证码
	//20//
	unsigned char rDriverNo[4];			//司机号 大端
	unsigned char rLineNo[3];			//线路号 大端
	unsigned char rGPSzhandian;			//站点编号
	unsigned char rGPSfangxian;			//GPS方向 上行/下行
	unsigned char rFactorFlg[8];		//下车机构标识
	unsigned char rCityCode[2];			//城市代码
	unsigned char rHUIJIAOYI;			//灰交易 0正常 1不正常
	//43//记录文件的上笔数据 (上车记录)
	unsigned char rDealTimeOLD[7];		//交易时间YYYY/MM/DD/HH/MM/SS* BCD
	unsigned char rPSAMNOOLD[6];		//上笔交易终端 BCD 就是PSAM卡号
	unsigned char rDriverNoOLD[4];		//司机号 
	unsigned char rLineNoOLD[3];		//线路号 
	unsigned char rGPSzhandianOld;		//上车站点编号
	unsigned char rCityCodeOLD[2];		//上车城市代码
	unsigned char rFactorFlgOLD[8];		//上车机构标识
	unsigned char rProviceOLD[3];		//上车的车牌 一个汉字+一个字母(暂时不用)
	unsigned char rDeviceNoOLD[4];		//上车车牌号(HEX)
	//......................................................
	unsigned char rProvice[3];			//下车的车牌 一个汉字+一个字母(暂时不用)
	unsigned char rZwyNum[4];			//下车站务员编号
	unsigned char rBusTC[2];			//趟次
	//以上一共104字节
	//
	//	QPBOC_RC	  rQpboc_Rc;			//QPBOC专有数据 70个字节
	unsigned char rBusFlag;				//不同状态的刷卡记录 
	unsigned char reasea[5];			//保留
	unsigned char rProVer[2];			//程序版本号
	unsigned char rTTTT[4];				//CRC校验四个字节AA


}DEALRECODE_JTB;

typedef struct
{
	unsigned char rPublishNumb[4];		//发行卡流水号
	unsigned char rDeviceNo[4];			//设备号
	unsigned char rCardType;			//卡类
	unsigned char rDealType;			//交易类型
	unsigned char rMoneyDealSum[2];		//钱包累计交易次数 [分段(上车记录)-全程票价]
	unsigned char rAfterMoney[4];		//原额
	unsigned char rDealMoney[3];		//交易金额
	unsigned char rDealTime[7];			//交易时间YYYY/MM/DD/HH/MM/SS
	unsigned char rRemain;				//保留 [分段 站点编号]
	unsigned char rPublishNumb_a;		//发行卡流水号最高位 
	//	unsigned char rTradCode[2];			//行业代码 用于区分相同密钥的不同城市或应用-------
	unsigned char rDriverNo[4];			//司机号

	unsigned char rCardDealNumb[4];		//设备交易流水
	unsigned char rLineNo[2];			//线路号
	unsigned char rResever1[4];			//设备唯一序列号 42  邯郸的 部门编号
	unsigned char ruserTimes[2];		//使用次数 司机号高位  [分段 全程时间] 如果是中控的记录和下面共18字节为卡片的身份证号
	unsigned char oldLineNO[2];			//旧线路号 [分段 上车刷卡次数]
	unsigned char PbocTac[4];			//pboc消费TAC码。
	unsigned char rBussinessNo[2];		//商户编号
	unsigned char rstuffernum[4];		//司机员工编号
	unsigned char rCardCand;			//卡型:手机卡\艺达卡\公交卡\CPU卡
	unsigned char rReseverX;			//保留 
	unsigned char rProVer[2];			//程序版本号
	unsigned char rTAC[4];				//校验码
}DEALRECODE;

typedef struct
{
	unsigned char rFlag[8];
	unsigned char rCardDealNumb[4];		//设备交易流水
	unsigned char rDeviceNo[4];			//设备号(序列号)
	unsigned char rCardType;			//卡类
	unsigned char rDealType;			//交易类型
	unsigned char rPublishNumb_a;		//发行卡流水号最高位  
	unsigned char rRemain;				//保留   //司机卡高位


//	unsigned char ruserTimes[2];		//使用次数 司机号高位  [分段 全程时间] 如果是中控的记录和下面共18字节为卡片的身份证号
	unsigned char rAfterMoney[4];		//原额
	unsigned char rDealMoney[3];		//交易金额
	unsigned char rDealTime[7];			//交易时间YYYY/MM/DD/HH/MM/SS
	unsigned char rDriverNo[4];			//司机号
	unsigned char rDeiceSleNo[4];		//设备自编号 //校验码
	unsigned char rLineNo[2];			//线路号
	unsigned char rProVer[2];			//程序版本号
	unsigned char rPrice_NA[2];			//票价
	unsigned char QRC_record[204];		//*二维码记录（明文） 字符串，BASE64格式，以0x00为结束符。（此字段为二维码交易记录，需要直接传给腾讯服务器）。
//	unsigned char QRC_record[212];		//*二维码记录（明文） 字符串，BASE64格式，以0x00为结束符。（此字段为二维码交易记录，需要直接传给腾讯服务器）。
	unsigned char rTAC[4];				// 校验码
}DEALRECODE_OTHQRC;	//二维码记录

typedef struct
{
	unsigned char flag;			//标识（1字节）
	unsigned char citycode[6];	// 6字节城市代码）
	unsigned char ZCcode[9];	//注册唯一码（9字节）
	unsigned char datetime[10];	//日期时间（10字节）
	unsigned char checkdata[10];	//验证码（10字节校验））
	unsigned char longitude[4];		//经度（4字节）
	unsigned char latitude[4];		///维度 4字节 
	unsigned char direction[4];		//方向4字节
	unsigned char speed[4];			//速度4字节

}stltyQRCandGPS;

typedef struct {
	unsigned char falg[4];//有效标识
	unsigned char RCE[128];//记录
	unsigned char CRC[4];	//CRC
}FeUnKnowREC;
typedef struct
{
	unsigned char cPublisherSign[8];	/* 发卡机构标识 */
	unsigned char cAppType;				/* 应用类型标识 02 其他不正常 */
	unsigned char cStarFlag;			/* 发卡机构应用版本 *启用标识*/
	unsigned char cApp_sn[10];			/* 应用序列号*/
	unsigned char cIssueDate[4];		/* 应用启用日期 */
	unsigned char cValidDate[4];		/* 应用有效日期 */
	unsigned char cFCI[2];				/* 发卡机构自定义FCI数据 */
}stJTB15;		//交通部95文件结构      30byte


typedef struct
{
	unsigned char cCardType;			/* 卡类型标识 */
	unsigned char cStuffcode;		/*本行职工标识*/
	unsigned char cName[20];			//持卡人姓名
	unsigned char cCredent[32];			/* 证件号 */
	unsigned char cCretype;				//证件类型
}stJTB16;		//交通部96文件结构     55byte
typedef struct
{
	unsigned char intercode[4];			/* 国际代码 */
	unsigned char provcode[2];			/*省级代码*/
	unsigned char Citycode[2];		/*城市代码*/
	unsigned char car_type[2];			//互通卡种  00非互通卡 01 互通卡
	unsigned char card_catalog;			//卡种类型
	unsigned char reser[49];			//预留
}stJTB17;		//交通部97文件:管理信息文件  60byte

typedef struct
{
	unsigned char rEPsearl[2];			/* EP联机或者脱机交易序列号 */
	unsigned char rOverdraft[3];		/*透支限额*/
	unsigned char rPursPrice[4];		//交易金额
	unsigned char rPursType;			/*交易类型标识*/
	unsigned char rPSAMnum[6];			//终端机编号	
	unsigned char rPursTime[7];			//交易时间
}stJTB18;		//交通部98文件:  23

typedef struct//w文件号 2702   128字节
{
	unsigned char rID[2];				//1-2交易标识  2702  公共汽车应用信息
	unsigned char rlen;					//3记录长度
	unsigned char rOk;					//4有效标识  固定01
	unsigned char rpursFlag;			//5交易标识1 采用分是分段扣费/复合消费    2 预授权消费
	unsigned char rLock;				//6锁应用标识 0没有锁 1锁定
	unsigned char rPursSearl[8];		//7-14交易流水BCD
	unsigned char rPursState;			//15交易状态标识1	00：初始值 01：上车 02：下车
	unsigned char rCityCodeON[2];		//16-17上车城市代码
	unsigned char rFactoryOn[8];		//18-25上车机构代码
	unsigned char rYunYinCode[2];		//26-27-上车运行代码
	unsigned char rLineNum[2];			//28～29上车线路号	2	HEX（高字节在前）
	unsigned char rStationOn;			//30上车站点
	unsigned char rBusNum[8];			//31-38上车车辆号Ascii
	unsigned char rPsamOn[8];			//39-46上车终端号BCD
	unsigned char rPursTimeOn[7];		//47-53上车时间
	unsigned char rPriceMax[4];			//54～57	标注金额4用于逃票追缴 HEX（高字节在前）
	unsigned char rDirection;			//58方向
	unsigned char rCityCodeOff[2];		//59-60下车城市代码
	unsigned char rFactoryOff[8];		//61-68下车机构代码
	unsigned char rYunYinCodeOff[2];	//69-70下车运营代码HEX
	unsigned char rLineNumOff[2];		//71～72下车线路号	2	HEX（高字节在前）
	unsigned char rStationOff;			//73下车站点
	unsigned char rBusNumOff[8];		//74-81下车车辆号Ascii
	unsigned char rPsamOff[8];			//82-89下车终端
	unsigned char rPursTimeOff[7];		//90-96下车时间
	unsigned char rPrice[4];			//97-100交易金额
	unsigned char rMileage[2];			//101-102乘车里程 HEX
	//30								//103-128预留26

//	unsigned char rCiytCodeY[2];		//103-104预留信息城市代码
//	unsigned char rlenY;				//105预留信息长度 
	//16
	unsigned char rDriverNum[4];		//106-19上车司机号
	unsigned char rCwuyuanNum[4];		//110-113 上车乘务员编号
	unsigned char rTangCi[2];			//114-115趟次
	unsigned char rTimeMax[2];			//116-117最长时间,单位 分钟
	unsigned char rName[10];			//119-128站点名称Ascii  五个汉字
	unsigned char rCH[4];				//118 预留

//	unsigned char rResear[8];			//预留信息内容
}stJTB1A;		//交通部1A变长记录文件结构     55byte
typedef struct//1E   48字节
{
	unsigned char eDealType;	//00初始值  01：EC入站/上车/进场等；02：EC出站/下车/出场等；03 EP入 04 EP出站/下车/出场等05：EC单次消费；   
								// 	06：EP单次消费；07：EC补票08：EP补票；09：EC充值；0A：EP充值；其他预留。						
	unsigned char eDeviceNum[8];//终端编号BCD
	unsigned char eTracode;		//10行业代码 01地铁 02公交 03城市水上客运 04出租汽车 05租赁汽车 06公共自行车 07停车收费 
								//08长途客运 09轮渡 0A城际高速铁路 0B民航 0C高速公路收费 0E市郊城铁（慢速） 0F～12 本规范预留
	unsigned char eLineNUm[4];	//11-14线路号 BCD
	unsigned char eRF[3];		//15-17RF
	unsigned char eDmoney[4];	//18-21交易金额 大端HEX
	unsigned char eYuE[4];		//22-25余额
	unsigned char eDealTime[7]; //26-32交易日期时间
	unsigned char eCityco[2];	//33-34受理方城市代码
	unsigned char eFactoryco[8];//35-42机构代码
	unsigned char eErrPriceMax[4];//43-46不完整交易最大金额
	unsigned char eTimeMax[2];	//47-48最长时间,单位 分钟大



	unsigned char N[6];			//预留
}stJTB1E;		//1E 48字节

typedef struct
{
	unsigned char kid;
	unsigned char PublisherSign[8];
	unsigned char ReceiverSign[8];
	unsigned char startDate[4];
	unsigned char endDate[4];
}sam_pub;



typedef struct
{
	unsigned char busProVer[2];//程序版本
	unsigned char busBLKVer[2];//黑名单版本
	unsigned char CSN_BUSVer[2];//线路信息版本
	unsigned char busLineVer[2];//站点限速信息版本2B
	unsigned char busVoiceVer[2];//公钥信息版本2B   如果有，需要转成16进制数
	//	unsigned char WHT_BUSVer[2];
	unsigned char White_Ver[2];//住建部城市白名单信息版本  低位在后
	unsigned char White_Ver_JTB[2];//交通部城市白名单信息版本  低位在后
	unsigned char Linux4GVer[2];//Linux模块版本号

	unsigned char busticketVer[2];//票价版本
	unsigned char busTXTPKVer[2];//腾讯公钥版本

	unsigned int BlackListNum;		//黑名单长度,条数
	unsigned int TXPublicKeyLen;	//腾讯公钥长度 
	unsigned int WHTBusListNum;		//住建部白名单长度 
	unsigned int WHTJTBListNum;		//交通部白名单长度 
}stBusVerIfo;


#define MAXList 3
#define MAXLINE 2048
#define _SOCKET_MAXNUM 4

typedef struct {
	unsigned char netwwwOk;	//网络已经接通
	unsigned char PPPtimes;	//连续拔号次数

	unsigned char gmissflag;				//任务标识，指示当前的通讯任务状态
	unsigned char GPRSLinkProcess;			//网络通讯连接进程
	unsigned char isNetOK[_SOCKET_MAXNUM];	//各链接是否连接成功

	unsigned char ISOK;						//平台是否签到成功
	//------工作密钥 WK密文------------------
	unsigned char PIN_KEY[16];				//没用
	unsigned int crc_pinkey;
	unsigned char MAC_KEY[16];				//
	unsigned int crc_mackey;
	//---------------------------------------
	unsigned char gcmdcode[100];			//访问令牌，LLVAR，最多99个字符,前一个字节是长度

	unsigned int gSendGLogin;				//发心跳包计数器
	unsigned char GprsCardList[20];			//SIM卡序号

	unsigned int gprsErrorTimes;			//连接错误计数
}stGPRSinfo;


typedef struct
{
	unsigned int colStyle;			//采集类型,0为按日期，1按流水号,,,当前只处理了日期采集
	unsigned char startdate[4];		//类型为0时，下发下来的日期4 YYYYMMDD ;类型为1时为起始流水号，低位在前
	unsigned char enddate[4];		//类型为0时，下发下来的日期4 YYYYMMDD ;类型为1时为结束流水号，低位在前
	char sfilename[100];			//正在发送文件名
	unsigned int filelen;			//获取文件后文件总大小，防止超过这个，
	unsigned int offset;			//当前文件发送位置
	unsigned int scrc32;

}stFAT_hisRec;		//历史记录文件 在fat文件系统里 20字节
#define BIT_REORD_HIS_LEN (sizeof(stFAT_hisRec))

//卡信息结构
typedef struct {
	unsigned int c_serial;			//卡序列号。
	CardCand_enum gMCardCand;		//卡类型


	unsigned char listSector;
	unsigned char publishSector;
	unsigned char publicMoneySector;
	unsigned char publicMessageSector;
	unsigned char dealRecordSector[3];	 //!@	
	unsigned char oddMonthSector;
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
	unsigned char JTBcard_catalog;
	unsigned char Zicard_catalog;
	unsigned char MoneySectorType;

	unsigned char nnAudioflag;	//声音提示标识

	unsigned char catalogPOST;//目录所在扇区，云平台中使用

	unsigned char PbocTac[4];
	unsigned char stuffNO[4];//发行区的发行日期，司机卡中用于员工编号

	unsigned char getdata_w;
	unsigned char getdata_flag;
	unsigned char WriteRecord_dis;
	unsigned char FactorFlg[8];//机构代码
	unsigned char ProcessFlag;

	unsigned char gucRestorFlag;	//用于标识卡内各信息的状态，如钱包是否正常，正副钱包及信息区是否一致等
	unsigned char CardType; //卡的类型 本地/异地
	unsigned char gucEndTimeFlag;//是否显示需要年审标识
	unsigned char gucSoudnorm;		//是否提示请充值标识

}stCardInfo;

#ifndef _BITS8_ATTR_
#define _BITS8_ATTR_
typedef struct {
	unsigned char bit0 : 1;	//111
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
}stBits8;

typedef union {
	unsigned char dataCh;
	stBits8 bits;
}unBits8;
#endif


typedef struct {
	char dataFlag;
	int bufLen;
	unsigned char Sendbuf[MAXLINE];
}stNetSendList;
typedef struct {
	char dataFlag;
	int bufLen;
	unsigned char Revbuf[MAXLINE];
}stNetRevList;


//黑名单63域说明,银联的
typedef struct
{
	unsigned char 	Organization_[8];		//
									//	INT8U	up_time[8];		//
	unsigned char 	card_no[10];

}QPBOC_TYPE_63;


//同一个连接重连时间延时30秒
#define CONNECT_DLYTIME 30
typedef struct {
	char linkNo;
	char APN[40];
	char IPaddr[16];
	int port;
	long pcTime;	//上次连接时间
					// // HEX，共8位，每位为1控制有效。0位：HTTPS，1位为0用APN1，1位为1用APN2.
	unBits8 linkAttr;	// 链接属性
}stServerInfo;

typedef struct {
	unsigned char citycode[2];
	unsigned char hanye[2];
	unsigned char startDateTime[7];//启用时间
	unsigned char lineVer[2];//版本
	unsigned char busPrice[4];//基本票价
	unsigned char rate[32];//折扣
	unsigned char voice[32];//语音代码
	unsigned char plusrate[32];//是否连续扣费
	unsigned char cardControl[32];//卡控制
	unsigned char plusPrice[4];//附加费票价
	unsigned char plusMonthPrice[4];//附加费票价
	//
	unsigned char ChangeType;	//换乘标识AA
	unsigned char ChangeRate;	//换乘折扣
	unsigned char ChangeTime;	//换乘时间 单位 分钟


	unsigned char DeviceNo[9];		//设备号


	unsigned char LineNo[3];
	unsigned char stopflag;					//是否要暂停刷卡 POS
	unsigned char ManagePassWord[3];//6位密码 3 字节 POS
	unsigned char reservee[3];		//--------60
	unsigned char UserKeyA[16];//存密钥卡的密钥
	unsigned char UserKeyACrc32[4];//kEY CRC
	unsigned char UserKeyB[16];//存密钥卡的密钥
	unsigned char UserKeyBCrc32[4];//kEY CRC  -----100

	unsigned char BussinessNo[6];//商户编号2字节+CRC32 4字节 ＝6字节 
	unsigned char ReseverAPN[17];//POS机中是APN
	unsigned char catalogPOST;//目录区所在位置
	unsigned char wifi_SSID[10];	//WIFI SSID
	unsigned char wifi_KEK[20];		//WIFI KEY
	unsigned char wifi_CrC32[4];	//WIFI KEY 和 WIFI SSID 的校验码 
	unsigned char Reserver[18];//

	unsigned char SN[4];		//设备序列号

	stServerInfo gServerInfo[_SOCKET_MAXNUM];

	unsigned char Null[1024];

	unsigned char Crc32[4];

	QPBOC_TYPE_63 TYPE_63;
	int batchNo;

}stDeviceParatable;//新的票价

typedef struct
{
	unsigned char stop_flag;			//停车状态	1(停)0(运行)
	unsigned char PlusSta;				//是否收取附加费		
	unsigned char set_device_status;	//设备卡刷卡标志
	unsigned char set_time_status;		//时间设置卡刷卡标志
	unsigned char set_line_status;		//线路卡刷卡标志
	unsigned char WriteCardFlag;
	unsigned char verrrr;
	unsigned char restore_flag;		//不为0，则需要刷新界面到主界面
	unsigned char g24GDisFlash;		//用于延时刷新界面

	unsigned char DriverNO[4];		//司机号
	unsigned char StufferNO[4];		//乘务员号

	unsigned short SumTi;			//当前消费次数	0
	unsigned short SumMti;			//当前月票卡刷卡次数 2
	unsigned int SumMoney;			//当前消费总额  4


	unsigned short PackSerial;			//GPRS发送包的序列号
	unsigned int MoneyDealPointer;		//设备流水号，钱包  
	unsigned int MonthDealPointer;		//设备流水号，月票
	unsigned int ControlDealPointer;	//设备流水号，控制



}fFlagVary_1;	//车辆状态标识

#ifndef _ProtocolErrCode_
#define _ProtocolErrCode_
typedef enum {
	PECUnkown = 0x1000,
	PECParam,
	PECFunction,
	PECSucess = 0x9000
}ProtocolErrCode;
#endif // !_ProtocolErrCode_

// #ifndef _RetCode_
// #define _RetCode_
// typedef enum {
// 	Ret_Error = -32768,
// 		Ret_Err_Param,
// 		Ret_Err_OverFlow,
// 		Ret_Err_Function,
// 		Ret_Err_Format,
// 		Ret_Err_TimeOut,
// 		Ret_Ok = 0
// }RetCode;
// #endif // !_RetCode_



// function, little endian
// #define SET_INT16(buf, len)	(buf)[0] = ((len) >> 8) & 0x0FF; (buf)[1] = ((len)) & 0x0FF
#define GET_INT16_B(buf) (((buf)[0]<<8)|((buf)[1]&0x0FF))	// 大端存储
//#define GET_INT16(buf) (((buf)[1]<<8)|((buf)[0]&0x0FF))	// 小端存储


#define voice_invalid		"/mnt/qrdata/wav/invalidcard.wav"		//非法卡
#define voice_studentcard	"/mnt/qrdata/wav/studentcard.wav"		//学生卡
#define voice_jianhangcard	"/mnt/qrdata/wav/jianhangcard.wav"	//建行卡
#define voice_stuffcard		"/mnt/qrdata/wav/stuffcard.wav"		//员工卡
#define voice_jiaotongcard	"/mnt/qrdata/wav/jiaotongcard.wav"	//交通卡
#define voice_jinglaocard	"/mnt/qrdata/wav/jinglaocard.wav"		//敬老卡
#define voice_liantongcard	"/mnt/qrdata/wav/liantongcard.wav"	//联通卡
#define voice_youfucard		"/mnt/qrdata/wav/youfucard.wav"		//优扶卡
#define voice_monthcard		"/mnt/qrdata/wav/monthcard.wav"		//月票卡
#define voice_youhuicard	"/mnt/qrdata/wav/youhuicard.wav"		//优惠卡
#define voice_normalcard	"/mnt/qrdata/wav/normalcard.wav"		//普通卡
#define voice_zhuanxiancard "/mnt/qrdata/wav/zhuanxiancard.wav"	//专线卡
#define voice_plsgeton		"/mnt/qrdata/wav/plsgeton.wav"		//请上车
#define voice_drivercard	"/mnt/qrdata/wav/drivercard.wav"		//司机卡
#define voice_showagain		"/mnt/qrdata/wav/showagain.wav"		//请重刷
#define voice_shuamashangche "/mnt/qrdata/wav/shuamashangche.wav"	//刷码成功,请上车
#define voice_WelCome		"/mnt/qrdata/wav/success.wav"			//欢迎乘车
#define voice_chargemonty	"/mnt/qrdata/wav/chargemonty.wav"		//请充值
#define voice_flingmoney	"/mnt/qrdata/wav/flingmoney.wav"		//请投币
#define voice_refleshRQC	"/mnt/qrdata/wav/refleshRQC.wav"		//请刷新二维码*
#define voice_invalidRQC	"/mnt/qrdata/wav/invalidRQC.wav"		//无效二维码*

//文件相关定义
#define WorkDir "/mnt/posdata"

//设备信息文件,保存不是经常更改的设备信息，比如商户号、设备号、票价、线路号等
#define _File_DevicePara "/DevicePara.inf"
#define _File_DevicePara_mode 0
//设备状态标识，保存的信息变化相对比较频繁的信息。
#define _File_BuInfo "/BusInfo.inf"
#define _File_BuInfo_mode 1

//当前记录文件，当前记录文件初始他2M字节。然后用指针文件来进行记录写入。
#define _File_Record_Cur	"/currecord.rec"
#define _File_Record_Point	"/RecordPoint.inf"
#define _File_BusVer		"/BusVer.ver"

#define _File_MothDelayTime		"/MothDelay.inf"
#define _File_ErrorCard		"/ErrorCard.inf"
//9、下载信息文件, 用于保存下载进度，以便后面可以续传
#define _File_FileDownPara "/FileDownPara.inf"

//主要用于保存临时下载的文件，此文件经过处理后会被删除，或有新的文件下载时会被覆盖
#define _File_TempFile "/TempFile.tmp"
//采集信息文件，用于标识需要采集的信息内容。
#define _File_His_Collect "/ColHisinfo.inf"
//公交黑名单文件
#define _File_BlackList_bus "/Blacklist.lst"
//腾讯公钥
#define _File_TXPublicKey "/TXpublicKey.lst"
//住建部白名单公钥
#define _File_WiteListBUS "/WiteListBUS.lst"
//交通部白名单公钥
#define _File_WiteListJTB "/WiteListJTB.lst"
//参数文件（不直接使用，只是用于备份）
#define _File_Bus_csn "/Buscsn"
//票价文件（不直接使用，只是用于备份）
#define _File_Bus_PRI "/BusPRI"




#endif

