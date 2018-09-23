#ifndef _INCLUDE_SCZT_HEAD_H_
#define _INCLUDE_SCZT_HEAD_H_

#include <stdio.h>

#include "Main_City.h"
#include "STProLib.h"
#include "StructDef.h"
#include "SlzrTypeDef.h"
#include "BuCtrlFlag.H"


#if 0
//#define _debug_sys_info_
#define _debug_
//#define debug_IF2_TLVDeal_
//#define debug_deal_8583_data
//#define debug_GJ_TLVDeal_
//#define _debug_gprs
//#define _debug_gps_
#define _debug_CPU_
#endif


#ifdef _debug_
//extern void dprintf (const char *fmt, ...);
#define MSG_LOG(...) printf(__VA_ARGS__)
#define BCD_LOG(b, l, m) debugdata(b, l, m)
#else
#define MSG_LOG(...)
#define BCD_LOG(...)
#endif

#define SWITCH_PBOC	1	// 是否支持银联功能,影响银行卡,银联付款码(非乘车码),通过银联的微信支付宝付款码

#if SWITCH_PBOC
//#define PBOC_TEST1	// 银联测试参数,加上公交等链接
//#define  PBOC_TEST		//银联演示程序，只连接银联后台
#endif

#ifdef PBOC_TEST1
#define DOMAIN_SHANGHAI_UNIONPAY	"ipos.chinaums.com"
#define DEV_TYPE	"EFS3000L"
#else
#define DOMAIN_SHANGHAI_UNIONPAY	"upos.chinaums.com"
#define DEV_TYPE	"ESF3000L"
#endif

#define SWITCH_ODA_BACKEND	1

//用PC串口发送GPS轨迹信息，模拟车辆运行
//#define _GPS_Speed
//考勤功能
//#define _card_guard_
//#define _TEST_YANPIAO_
//#define _phone_test_
//数据通讯模块为CDMA，或MC323，不定义为900C 在具体城市定义中

//定义GPRS使用的COM口号 COM2
#ifdef Modem_BUS_
#define COM_GPRS COM1
#else
#define COM_GPRS COM2
#endif

#define GPS_COM COM5

//程序版本号
#define SOFT_VER_TIME_LOG 0x0103	// 1.03,与1.02一致
#define SOFT_VER_TIME "2018.09.23 23:25 CHANGSHA"	//

#define PRAGRAMMER "ZZC"

//#define SOFT_VER_TIME_LOG "201009061359BB"

#define _backLight_on_ 0
#define _watchdog_open

//刷卡的类型
//通用M1卡
#define CARDSTYLE_NORM1 1
//CPU卡
#define CARDSTYLE_CPU 2
//2.4G CPU卡
#define CARDSTYLE_24CPU 3
//艺达M1卡
#define CARDSTYLE_YIDAM1 4
//2.4G移动卡
#define CARDSTYLE_24Mobile 5
//2.4G电信卡
#define CARDSTYLE_24CT 6
//中科讯联2.4G 联通
#define CARDSTYLE_24ZKXL 7
//厦门盛华2.4G 联通、电信
#define CARDSTYLE_24XMSH 8
//安龙老卡
#define CARDSTYLE_ANLONG 9

#define SEND_FILE_ERROR_ 10

#define  CARDSTYLE_JTB  11
#define	 QPBOC_JTB      12 //交通电子现金
//#define LED_DISPLAYNUM 
#define BLACKSUM 16000

#ifdef GPRS_SOUD_ENABLE
#define MC323_Ring_sta	0xA1
#define MC323_Ring_key	0xA2
#define MC323_Ring_ing	0xA3
#define MC323_Ring_END	0xA4
#endif

//重启记数次数
#define RUN_RESTART_TIME 125

//10
#define RECORD_TTL 20
#define RECORD24G_TTL 20

#define null 0

#define _TEST_GPS 1
#define _TEST_GPRS 2
#define _TEST_Irda 3

/* key 115 Pressed  UP
key 115 Released
key 114 Pressed		DOWN
key 114 Released
key 139 Pressed		ENTER
key 139 Released
key 28 Pressed		ESC
key 28 Released*/
#ifndef SLZRKEY_UP
#define SLZRKEY_UP		115	// Arrow Up
#define SLZRKEY_DOWN	114	// Arrow Down
#define SLZRKEY_ESC		28		// Esc
#define SLZRKEY_ENTER	139		// Enter

#define SLZRKEY_F1		KEY_F1	// Function
#define SLZRKEY_F2		KEY_F2
#endif

////////////////////////////////////////////////////////////////////////////////
////////					功能开关
////////////////////////////////////////////////////////////////////////////////
#define HONG_WAI_XIAN_GET_DATA_EN 	
////////////////////////////////错误编码规范////////////////////////////////////////
#define ST_OK			0			//操作正常
#define ST_ERROR		1			//操作失败
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
//*****************错误提示信息************************
#define TASK_ERROR_MESSAGE_PRIO	2
#define TASK_PSAM_PRIO		4
#define TASK_FEROM_PRIO		3
#define TASK_EEROM_PRIO		5
#define TASK_MIFARE_PRIO	7
#define TASK_GPRS_PRIO		8
#define TASK_GPRS_SEND		9
#define TASK_TIME_PRIO		10
#define TASK_KEY_BOARD_PRIO	11
#define TASK_DISP_PRIO		12

#define TASK_CARD_MANAGE_PRIO	37
#define TASK_MASTER_PRIO		39
#define TASK_RECORD_QUERY_PRIO	40
#define TASK_AUDIO_PRIO		56
#define TASK_BEEP_PRIO		57
#define TASK_MAC_PRIO		58
#define TASK_RECORD_WRITE_PRIO	59
#define TASK_GPRS_CONNENT_PRIO	60


#define ERR_NO_CURRENT_RECORD 	1
#define ERR_SEND_CMDPACKAGE 		2
#define	ERR_FILENAME_PACKAGE 		3
#define	ERR_SEND_DATAPACKAGE		4
#define	ERR_RECIVE_ACKPACKAGE		5
#define	ERR_PACKAGE_LENGTH		6
#define	ERR_RECIVE_DATAPACKAGE		7
#define	ERR_READ_DATE				8
#define	ERR_RECIVE_CMDPACKAGE		10
#define	ERR_SERIALNO				11
#define	ERR_PROOF_TIME				12
#define ERR_LOAD_BLACKCARD		13

#define ERR_MIF_REQUEST		1		////M1卡寻卡错误
#define ERR_MIF_ANTICOLL	2		//M1卡防冲突错误
#define ERR_MIF_AUTEN		3		//M1密码认证错误
#define ERR_MIF_SELECT		4		//M1卡片选卡错误
#define ERR_MIF_READ		5		//M1读卡错误
#define ERR_MIF_WRITE		6		//M1写卡错误
#define ERR_MIF_INC			7		//M1加值错误
#define ERR_MIF_TYPE		8		//M1卡类型错误
#define ERR_MIF_INBLACK		9		//M1卡号在黑名单上
#define ERR_MIF_PURSE		10		//钱包金额过大
#define	ERR_MIF_FORMAT		11		//M1片数据格式错误
//-----------------------------主机通讯错误--------------------------------------
#define ERR_OPEN_PORT		20		//打开端口错
#define ERR_CLOSE_PORT		21		//关闭端口错
#define ERR_COM_HARDWARE	22		//硬件出错
#define ERR_COM_TIMEOUT		23		//超时错误
#define ERR_COM_COMMAND		24		//命令号非法
#define ERR_COM_FORMAT		25		//命令格式错误
#define ERR_COM_SEND		26		//发送命令错误
#define ERR_COM_RECEIVE		27		//接受命令错误
//-----------------------------存储器相关错误--------------------------------------
#define ERR_MEM_EXHAUST		40		//存储器空间耗尽
#define ERR_MEM_WRITE		41		//写记录错误
#define ERR_MEM_READ		42		//读记录错误
//-----------------------------数据采集相关错误--------------------------------------
#define ERR_BOX_TYPE		50		//采集盒类型错误
#define ERR_BOX_BLIST		51		//采集盒黑名单数据格式错误
#define ERR_BOX_FCB			52		//采集盒文件控制块数据格式错误
#define ERR_BOX_NOTEXT		53		//没有发现采集盒
#define ERR_BOX_READ		54		//采集盒数据读错误
#define ERR_BOX_WRITE		55		//采集盒数据写错误
#define ERR_BOX_OVER		56		//采集盒数据溢出
//-----------------------------设备类相关错误--------------------------------------
#define ERR_CHG_NUM			60		//充值金额非法
#define ERR_CHG_DEVTYPE		61		//设备类型错误
#define ERR_CHG_DEVATHEN	62		//设备未授权
#define ERR_CHG_NOINIT		63		//设备没有初始化
//-----------------------------PSAM卡相关错误--------------------------------------
#define ERR_PSAM_NOREPEN	70		//没有复位应答
#define ERR_PSAM_FILESEL	71		//文件选择错误
#define ERR_PSAM_PINAUTH	72		//PIN认证错误
#define ERR_PSAM_PAGELEN	73		//长度错误
#define ERR_PSAM_NOINFO		74		//无信息提供
#define ERR_PSAM_FILEINVAL	75		//选择文件无效
#define ERR_PSAM_AUTHEN		76		//认证出错
#define ERR_PSAM_WREEPROM	77		//写EEPROM失败
#define ERR_PSAM_DATALEN	78		//数据长度错误
#define ERR_PSAM_INVASTATE	79		//无效的状态
#define ERR_PSAM_FILETYPE	80		//文件类型不匹配
#define ERR_PSAM_SSTATE		81		//安全状态不满足
#define ERR_PSAM_KEYLOCK	82		//密钥已被锁住
#define ERR_PSAM_DATAIN		83		//引用数据无效
#define ERR_PSAM_TERM		84		//引用条件不足
#define ERR_PSAM_SCPAGKE	85		//安全报文数据项不正确
#define ERR_PSAM_DATAFLD	86		//数据域参数不正确
#define ERR_PSAM_FUNC		87		//功能不支持
#define ERR_PSAM_NOFILE		88		//没有找到文件
#define ERR_PSAM_RECORD		89		//没有找到记录
#define ERR_PSAM_P1P2		90		//P1，P2参数不正确
#define ERR_PSAM_INDATA		91		//未找到引用数据
#define ERR_PSAM_PARM		92		//参数错误
#define ERR_PSAM_ICINS		93		//不正确的INS
#define ERR_PSAM_ICCLA		94		//不正确的CLA
#define ERR_PSAM_UNKNOW		95		//未定义的错误
#define ERR_PSAM_IVMAC		96		//MAC无效
#define ERR_PSAM_PLOCK		97		//应用永久锁定
#define ERR_PASM_BUDGET		98		//金额不足
#define ERR_PSAM_SPKEY		99		//密钥索引不支持
#define ERR_PSAM_SPMAC		100		//所需MAC不支持

#define ERR_SELECT_MAIN		102		//选主文件出错
#define ERR_SELECT_ADF3		103		//选择ADF3文件出错
#define ERR_SELECT_ADF3_EF01 104	//选择参考文件出错
#define ERR_READ_ADF3_EF01	105		//读参考文件出错	
#define ERR_READ_READFILE	106		//读消费文件出错
#define ERR_ADD_READFILE	107		//追加循环文件出错	
#define ERR_INIT_FOR_DESCRYPT	108	//MAC初始化出错
#define ERR_DAOCHUKEY_1			109  //导出第一扇区密钥
#define ERR_MAC					110		//MAC 加密运算出错

#define ERR_LOAD_KEY			112	
#define ERR_MIF_AUTHEN	    	113
#define ERR_GET_M1KEY			114		//推导M1卡密钥出错	
//#define ERR_CARD_GESHI			115		//卡中数据校验出错
#define ERR_MIF_CHANGE			116		//减值函数出错
#define ERR_MIF_TRANSFER		117	
//#define ERR_UPDATE_BIN			118		//写二进制文件出错
#define ERR_EEPROM_FULL			200
#define ERR_BUFFER_FULL			201
#define ERR_CARD_RESTORE		202
//------------文件标识定义-------------
#define KEYA				0
#define KEYB				1
#define PSAM_HEAD_FLAG		0x3b
#define MW_CITY_STD			0x86 	//明华PSAM卡标志	
#define MW_EF15				0x15	//应用公共信息标识
#define MW_MF				0x3f00	//主文件标识	
#define MW_ADF3				0x1003	//非接触ADF3目录应用标识
#define MW_ADF2				0x1002	//一卡通专用目录应用标识
#define MW_ADF3_EF01		0x01	//基本信息文件标识
#define MW_ADF3_EF04		0x04	//消费信息文件
#define RECORD_CAPTION		0x30	//交易记录MAC密钥标识
#define RECORD_KEY_TYPE		0x08	//交易记录MAC密钥类型	
#define MW_ADF3_EF05		0x05	//黑名单信息文件的类型	
#define MW_ADF3_EF02		0x02	//历史采集文件密钥标识		
#define MW_ADF3_EF03		0x03    //当前采集文件	
#define MW_ADF3_EF06		0x06	//文件序列号
#define MW_ADF3_EF07		0x07	//储值交易记数
#define MW_ADF3_EF08		0x08	//月票交易记数
#define MW_ADF3_EF09		0x09	//控制卡交易记数
#define MW_KEY1_A			0x20	//发行扇区A密钥标识	
#define MW_KEY2_A			0x60	//记录扇区和公共信息区A密钥标识
#define MW_KEY3_A			0xa0	//月票区A密钥标识	
#define MW_KEY4_A			0x40	//公共钱包区的A密钥标识	
#define MW_KEY5_RECORD		0x30	//交易数据的密钥标识	

#define MF_CARD_MES			0x0015
#define MF_TREE_MES			0x0001
#define MF_ZHOUDUANG_MES	0x0016
#define ADF1				0x1001
#define ADF1_PUBLIC_MES		0x0017
#define ADF2				0x1002
#define ADF2_KEY			0x0
#define ADF3				0x1003
#define ADF2_PUBLIC_MES		0x0002
#define ADF2_DEAL_COUNT		0x0004

#ifdef _debug_
#define WAIT_TIME 300
#else
#ifdef BUS_SYBAOSHI_
#define WAIT_TIME 20
#elif defined ZongKong_CPU_ //中控的CPU单独处理
#define WAIT_TIME 2
#elif defined _HD_XIAOER_ //邯郸小额消费等待时间2小时
#define WAIT_TIME 20
#else
#define WAIT_TIME 300
#endif
#endif

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

///////////////////////////////语音端口号//////////////////////////////
#ifdef AUDIO_SOUND_OLD //老的16段语音
//两片8段语音
#define Audio_DONG			1		//咚
#define Audio_CY			2		//优扶卡
#define Audio_STUDENT		3		//学生卡
#define Audio_BIRTHDAY		4		//寿星卡
#define Audio_STUFF			5		//员工卡
#define Audio_OLDMAN		6		//老人卡
#define Audio_PLUS_MONEY	7		//请充值
#define Audio_FLING_MONEY	8		//请投币
#define Audio_YH			9		//优惠卡
#define Audio_LOVER			10		//爱心卡
#define Audio_CYman			11		//优抚卡(男声)
#define Audio_STUFFman		12		//员工卡(男声)
#define Audio_OLDMANman		13		//老年卡(男声)
#define Audio_INVALID		14		//非法卡
#define Audio_TRY_AGAIN		15		//请重刷
#define Audio_STUDENTman	16		//学生卡(男声
#define Audio_MONTH			15		//月票卡
//宝鸡语音
#elif defined BUS_BAOJI_
#define Audio_DONG			1		//咚
#define Audio_CY			2		//优扶卡
#define Audio_STUDENT		3		//学生卡
#define Audio_BIRTHDAY		4		//寿星卡
#define Audio_STUFF			5		//员工卡
#define Audio_OLDMAN		6		//老年卡
#define Audio_PLUS_MONEY	7		//请充值
#define Audio_FLING_MONEY	8		//请投币
#define Audio_INVALID		9		//非法卡
#define Audio_FAMILY		10		//家属卡
#define Audio_Zilu			11		//子女卡
#define Audio_YWJYu			12		//请年审
#define Audio_JC			13		//稽查卡
#define Audio_LOVER			14		//爱心卡
#define Audio_TRY_AGAIN		15		//请重刷
#define Audio_GSK			16		//挂失卡
#define Audio_FACE			17		//发车卡
#define Audio_DaoZan		18		//到站卡
#define Audio_CAIJI			19		//采集卡
#define Audio_Siji			20		//司机卡
#define Audio_YH			21		//优惠卡
#define Audio_Wuxiao		22		//无效卡
#define Audio_JianDu		23		//监督卡
#define Audio_XianLu		24		//线路卡
#define Audio_DiaoDu		25		//调度卡 站务员卡
#define Audio_GuanLi		26		//管理卡
#define Audio_DaZe			27		//打折卡
#define Audio_MONTH			28		//月票卡
#define Audio_QianDaoEnd	29		//签到完毕
#define Audio_zuanxian		30		//专线卡
#define Audio_TuanTi		31		//团体卡
#define Audio_YuerDi5		32		//余额低于5元

//宝鸡语音	#elif defined BUS_BAOJI_
#else//新32段语音
//一片32段语音
#define Audio_INVALID		2		//非法卡
#define Audio_NOMONEY		3		//金额不足//余额不足
#define Audio_FAMILY		4		//家属卡
#define Audio_Zilu			5		//子女卡
#define Audio_YWJYu			6		//亲情卡  请年审  义务教育卡（九江）
#define Audio_STUDENT		7		//学生卡
#define Audio_STUFF			9		//员工卡
#define Audio_OLDMAN		10		//老人卡
#define Audio_LOVER			11		//爱心卡
#ifdef SOUND_New_B
#define Audio_Man_STUFF		12		//员工卡(男)
#else
#define Audio_TuanTi		12		//团体卡
#endif
#define Audio_Wuxiao		13		//无效卡
#define Audio_zuanxian		14		//专线卡 签到完毕, 请下车
#define Audio_MONTH			15		//月票卡
#define Audio_PuTong		16		//普通卡
#define Audio_YH			17		//优惠卡
#define Audio_DONG			18		//咚
#define Audio_DingDang		19		//咚咚，电信卡
#define Audio_Di			20		//嘀
#ifdef SOUND_New_B
#define Audio_NODRen		21		//不能带人乘车
#else
#define Audio_YongJun		21		//拥军卡
#endif
#define Audio_SuaKaOK		22		//刷卡成功
#define Audio_YouDai		23		//优待卡
#define Audio_Siji			24		//司机卡
#define Audio_PLUS_MONEY	25		//请充值..B=请及时充值
#ifdef SOUND_New_B
#define Audio_MAN_STUDENT	26		//学生卡男
#else
#define Audio_BIRTHDAY		26		//敬老卡
#endif
#define Audio_CY			27		//优扶卡
#define Audio_FULI			28		//福利卡
#define Audio_GuaShi		29		//挂失卡,请交回
#ifdef SOUND_New_B
#define Audio_MAN_Month		30		//月票卡男
#else
#define Audio_XiaoXueShen	30		//小学生卡
#endif
#define Audio_XueShenYP		31		//学生月票卡
#ifdef SOUND_New_B
#define Audio_Man_FULI		32		//福利卡男
#else
#define Audio_YuerDi5		32		//余额低于5元
#endif

#define Audio_no_di			0xee	//没语音，只是蜂鸣器响

#define Audio_TRY_AGAIN		1		//请重刷 
#define Audio_FLING_MONEY	8		//请投币

#endif

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
//******************PurseProcess()返回代码///////////////
#define RET_MONEY_SN_EROR 20	//该卡的钱包扇区格式全错

#ifdef MOBILE_XIANYANG_
typedef struct
{
	unsigned char Tyear;//年份
	unsigned short Ttimes;//使用次数
}CardUserTimes;
#endif

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
#define BIT_GPSDownLine_INFO	(BIT_BLACK_2BLOK+32)
//4 pboc 记录流水号
#define BIT_PBOC_NS	BIT_GPSDownLine_INFO+32
//160  历史指针日期，每天更新一次历史指针和统计数据 日期时间7+[卡号+充值累计金额4+...]+CRC32(4)   8张卡
#define BIS_PERDat_Info (BIT_PBOC_NS+4)
//4 司机员工编号
#define BIT_STUFFER_NO (BIS_PERDat_Info+160)
//20 历史记录上传地址.用于GPRS传输历史记录指针.(首地址4+长度4+日期时间7+保留1+CRC324)=20字节,首地址是相对地址,要加上FLASH2_START1
#define BIT_HISREC_SND (BIT_STUFFER_NO+4) //2536
//8 累计刷卡计数 次数4+校验4
#define BIT_SWAPCARD_TIMES (BIT_HISREC_SND+20)
//8 设置的刷卡上限 次数4+校验4
#define BIT_SWTIMES_MAX (BIT_SWAPCARD_TIMES+8)
//28 月统计，保存前一个月和当前月的刷卡统计 2 * (年月2+刷卡总次4+刷卡总金额4+CRC32校验4) = 28字节
#define BIT_MOTH_TOJI_2 BIT_SWTIMES_MAX+8
//8 车载机禁用标识，在后台签到返回及心跳包中更新 目前只有银联卡是否启用的。"启"+"0x00"+标识1+CRC32校验4 = 8字节
#define BIT_Center_Ctrl_BU_Data (BIT_MOTH_TOJI_2+28)
//4 单位,部门编号
#define BIT_UNIT (BIT_Center_Ctrl_BU_Data+8)
// 136灰记录存储，暂用128字节，“有效”4 + 128 data+4CRC = 136
#define BIT_UNKNOW_RECORD	(BIT_UNIT + 4)

#define BIS_BLK_63          (BIT_UNKNOW_RECORD+136)     //  0x1A29,
#define BBIS_BLK_63_LEN      26


//批上送信息sizeof(stTradeBatchinfo)) =24
#define BIT_TradeBatchinfo		BIS_BLK_63+BBIS_BLK_63_LEN
//stMobilStyle = 88字节
#define BIT_SingInStyle		(BIT_TradeBatchinfo+ 24) // 签到状态
//pos 的参数信息  stMobileParameter  64
#define BIT_qpbpc_para			BIT_SingInStyle +  88+30   //192

//4 pboc 记录流水号 999999 后+1
#define BIT_PBOC_NS_BIG		(BIT_qpbpc_para+64+64+128) 

//500字节，st_qpoc_repurse 结构体 用于存储冲正的临时记录，如果有的时候要发送给后台
#define BIT_repurse_infor	BIT_PBOC_NS_BIG+4
#define BIT_repurse_infor_LEN      850

#define BIT_PBOC_NS_8583           (BIT_repurse_infor + BIT_repurse_infor_LEN)     //  0x1019,银联用的11域交易流水4字节 BIT_PBOC_NS 这个有时候不对，会被覆盖，或者就是没有自增没有存储上，
#define BIT_PBOC_NS_8583_LEN       4

#define BIS_PAY_MODE          (BIT_PBOC_NS_8583+BIT_PBOC_NS_8583_LEN)     //  0x1A29,
#define BIS_PAY_MODE_LEN      4
#define BIT_BAK_ADDR_add		(BIS_PAY_MODE+BIS_PAY_MODE_LEN)	//12,首地址+尾地址+校验
#define BIT_BAK_ADDR_add_LEN 12

#define BIT_TMS_FILE_DOWNLOAD (BIT_BAK_ADDR_add+BIT_BAK_ADDR_add_LEN)     //  0x1A29,
#define BIT_TMS_FILE_DOWNLOAD_LEN      216

//192   保存银行临时记录
#define BIT_qPBOC_Rec_Temp (BIT_TMS_FILE_DOWNLOAD+BIT_TMS_FILE_DOWNLOAD_LEN)

#define BIT_END_ADDR		(BIT_qPBOC_Rec_Temp+256)

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

// typedef struct  {
// 	unsigned char ucValid;				//命令是否有效
// 	unsigned char Cmd;					//命令
// 	unsigned short usLen;				//长度 data+BCC
// 	unsigned short usIndex;
// 	unsigned char databuf[10];			//数据 data+BCC
// }stLcdCmd;
//--------------记录------------------

// #ifdef BUS_HANDAN_
// #define RECORD_LEN 32
// #else
#define RECORD_LEN 64
//#endif

#define MAX_REC_NUM	(LEN_EEPROM / RECORD_LEN)   //eeprom中存放的最大记录数目
//**********************************************
//           交易记录的格式定义				  //
//**********************************************
#if 0
#ifdef BUS_HANDAN_XX
typedef struct
{
	unsigned char rPublishNumb[4];		//发行卡流水号
	unsigned char rCardDealNumb[4];		//卡片交易流水
	unsigned char rCardType;			//卡类
	unsigned char rDealType;			//交易类型

	unsigned char rMoneyDealSum[2];		//钱包累计交易次数	
	unsigned char rAfterMoney[4];		//原额
	unsigned char rDealMoney[3];		//交易金额
	unsigned char rDealTime[7];			//交易时间YYYY/MM/DD/HH/MM/SS
	unsigned char TTL;					//记录生存时间
	unsigned char rRecordType;			//记录类型
	unsigned char rMAC[4];				//记录的MAC吗
}DEALRECODE_32B;	//邯郸

typedef struct 		//控制卡交易
{
	unsigned char rPublishNumb[4];		//发行卡流水号
	unsigned char rCardDealNumb[4];		//卡片交易流水
	unsigned char rCardType;			//卡类
	unsigned char rDealType;			//交易类型

	unsigned char rDealTime[7];			//采集时间
	unsigned char rDriveNo[4];			//设备号
	unsigned char rLineNo[4];			//线路编号
	unsigned char rLineNO_1;			//线路分号
	unsigned char TTL;					//记录生存时间
	unsigned char rRecordType;			//记录类型
	unsigned char rMAC[4];				//记录的MAC吗
}Get_data_2;
typedef struct
{
	unsigned char rPublishNumb[4];		//发行卡流水号
	unsigned char rCardDealNumb[4];		//卡片交易流水
	unsigned char rCardType;			//卡类
	unsigned char rDealType;			//交易类型

	unsigned char rDealTime[7];			//采集时间
	unsigned char MoneySum[4];				//本次消费总金额
	unsigned char MonthSum[2];				//本次月票总次
	unsigned char RecordSum[2];				//本包总记录数
	unsigned char rReserveByte;			//保留
	unsigned char TTL;					//记录生存时间
	unsigned char rRecordType;			//记录类型
	unsigned char rMAC[4];				//记录的MAC吗
}Get_data_1;

typedef struct 		//控制卡交易
{
	unsigned char rPublishNumb[4];		//发行卡流水号
	unsigned char rCardDealNumb[4];		//卡片交易流水
	unsigned char rCardType;			//卡类
	unsigned char rDealType;			//交易类型

	unsigned char rDelaTime[7];			//交易时间
	unsigned char rPublishNumb2[4];		//发行卡流水号
	unsigned char rReserveByte[5];		//保留
	unsigned char TTL;					//记录生存时间
	unsigned char rRecordType;			//记录类型
	unsigned char rMAC[4];				//记录的MAC吗
}Record_Control_32B;	//邯郸
#else  //#ifdef BUS_HANDAN_
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

#endif //#ifdef BUS_HANDAN_
#endif

typedef struct
{
	unsigned char rDealTime[7];			//交易时间YYYY/MM/DD/HH/MM/SS
	unsigned char rPublishNumb[2];		//没用
	unsigned char rDealType;			//交易类型 10

	unsigned char rDeviceNo[4];			//设备号唯一号
	unsigned char rCardType[2];			// 没用
	unsigned char rName[16];			//姓名区
	unsigned char rNameID[16];			//证件号
	unsigned char rCardID[16];			//卡号
}DEALRECODE_CS456_ASCii;

typedef struct
{
	unsigned char rDeviceNo[4];//设备号
	unsigned char rLineNo[2];//线路号
	unsigned char AID1[3];//应用标识AID1 CN 保留不用
	unsigned char dealType;//交易类型	0x04
	unsigned char AID2[3];//应用标识AID2 CN 保留不用
	unsigned char rDriverNo[4];	//司机号
	unsigned char dealCardNo[10];//交易卡号	钱包卡号 N
	unsigned char posSerial[3];//POS终端交易流水号 N 压缩BCD码

	unsigned char KEYflagNO;//密钥索引号 CN
	unsigned char KEYVer;//密钥版本号 N
	unsigned char TAC[4];//消费充值TAC B
	unsigned char purseMoney[4];//消费交易金额 B
	unsigned char pursestyle;//交易类型标识 B
	unsigned char deviceNo[6];//POS终端号 CN
	unsigned char PSAMdealSerial[4];//终端交易序号	PSAM卡产生 B 用于保存　终端标识码。
	unsigned char date[4];//消费交易日期（终端）CN
	unsigned char time[3];//消费交易时间（终端）CN
	unsigned char afterMoney[4];//钱包余额（交易后余额）B
	unsigned char reserver[2];//保留－－－－－－－－－－－－－－共64字节

// 	unsigned char PSAMdealSerial[4];	//终端交易序号	PSAM卡产生 B
// 	unsigned char rDeviceNo[4];			//设备号 POS终端号
// 	unsigned char rMobilCardType;		//移动交易类型	0x01:充值 0x02:脱机消费 0x12撤销 0x13退货 CN
// 	unsigned char rDealType;			//交易类型 0x04 手机交易
// 	unsigned char rResever0[2];		//
// 	unsigned char rAfterMoney[4];		//原额
// 	unsigned char rDealMoney[3];		//交易金额
// 	unsigned char rDealTime[7];			//交易时间YYYY/MM/DD/HH/MM/SS
// 	unsigned char rResever1[2];			//
// 	unsigned char rDriverNo[4];			//司机号
// 	
// 	unsigned char rCardDealNumb[4];		//POS终端号 移动PSAM卡号
// 	unsigned char rLineNo[2];			//线路号
// 	unsigned char rMAC[10];				//交易卡号	钱包卡号 N
// 	unsigned char rResever[14];			//保留
// 	unsigned char rCrc[2];				//校验码
}MobileDEALRECODE;

typedef struct
{
	unsigned char rDeviceNo[8];			//设备号
	unsigned char posHardW_flag;		//车载机硬件标识
	unsigned char rDealType;			//交易类型  50
	unsigned char busBLKVer[7];         //黑名单版本号(BCD码)
	unsigned char busProVer[7];			//程序版本号(BCD码)
	unsigned char busLineVer[7];		//线路信息版本号(BCD码)
	unsigned char CurRecordPointer[4];		//当前记录指针
	unsigned char CurPointer[4];			//起始地址
	unsigned char BlackListNum[4];         //黑名单数量

	unsigned char rLineNo[2];			//线路号
	unsigned char rResever[19];			//保留
}SD_DEALRECODE;//绍东的记录格式_wxliu_V3.27_2010.12.15

typedef struct {
	unsigned char citycode[2];
	unsigned char hanye[2];
	unsigned char deviceNo[4];//设备号
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
	unsigned char Rever[96];//预留
}stPricetable;//新的票价

typedef struct {
	unsigned char flag[4];//标志：“黑单”
	unsigned char index[3];// 位置
	unsigned char DorI;//增加或删除标志
	unsigned char CardNo[5];//最多5字节长黑名单号
	unsigned char BCC;//校验
}stAddBLKTEMP;

typedef struct {
	unsigned char cardNo[4];//此段操作员卡号
	unsigned int chargeA;//钱包消费总金额
	unsigned int RseverA;//次数消费总金额
	unsigned int SaleA;//刷卡总次数
}stSatone;
typedef struct {
	unsigned char DataTime[7];
	stSatone infoBuf[8];
	unsigned char resever[8];
	unsigned int sCrc32;
}stStaeInfo;//统计数据记录结构 148

//extern void clr_wdt(void);

////////////////////////////////GPS//////////////////////////

typedef struct
{
	unsigned char rDeviceNO[4];			//0-3 设备号车辆编号
	unsigned char rDeviceSl[4];			//4-7 序列号终端机编号
	unsigned char rinRun;				//8 是否营运代码1B
	unsigned char rDealType;			//9 交易类型  E0--E6

	unsigned char ZULine[4];			//10-13 主线路编号
	unsigned char ZiLine[4];			//14-17 子线路编号
	unsigned char ulCardNo[4];			//18-21 司机卡号

	unsigned char stationNo;			//22 站点编号
	unsigned char RunDirect;			//23 方向
	unsigned char LuduanNo[2];			//24-25 路段编号2B

	unsigned char MTimesUP;				//26 上车人次
	unsigned char MTimesDown;			//27 下车人次
	unsigned char LiCengALL[4];			//28-31 里程4B___32

	unsigned char DealTime[7];			//32-38 时间(超速结束时间)
	unsigned char STARTDealTime[7];		//39-45 时间(超速开始时间)

	unsigned char rCardDealNumb1[4];	//46-49 设备交易流水
	unsigned char RFU1[2];				//50-51 保留字节__52

	unsigned char RFU[10];				//52-61 保留字节
	unsigned char CRC16[2];				//62-63 不用__共64字节
}RECORD_GPS;

////////以上是GPS结构定义///////////////////////
////////电信2.4G 沈阳宝石相关结构//////////
#define SELPHONE_valid 0xA5
#define SELPHONE_SENDED 0xAA
typedef struct
{
	char Style;				//标识，0xA5有效
	char SellPhoneNo[14];	//手机号
	char SendMsg[70];		//要发送短信的内容，20个汉字UNICODE编码
	unsigned char CrC[4];	//CrC32校验

	unsigned char len;		//发送数据的长度,因是UNICODE编码，不能用str相关函数.
	unsigned char times;	//已经发送次数，如果发送3次不成功，延时。
	unsigned char TLL;		//延时，如果发送多次没成功，就延迟一段时间
	unsigned char index;	//在存贮区的位置号
}stSelMsg; //61字节

typedef struct
{
	char style;
	unsigned char datatime[7];
	char SellPhoneNo[14];	//手机号
	unsigned char reseve[6];//备用
	unsigned char CrC[4];	//CrC32校验
}stSaveMSG;	//32 保存需要发送的信息，手机和日期时间 BIT_GPSDiaoDuCMD_INFO  1K ＝ 32条
//----------------------------------------------
//-------------分段收费结构定义-----------------
typedef struct
{
	unsigned char getupFlag;	//0:上车标志=AA
	unsigned char times;		//1:上车刷卡次数 (HEX)
	unsigned char stationNo;	//2:上车时的站点编号 (HEX)
	unsigned char Allprice[2];	//3-4:线路全程票价 （HEX）
	unsigned char LineNo[3];	//5-7线路编号 其中:5-6=线路编号(BCD),7=子线编号(BCD)
	unsigned char getupTime[7];	//8-14上车时间 (年月日时分秒=YYYYMMDDHHMMSS)
	unsigned char Cxor;			//15：校验
}stFDCard1;
typedef struct
{
	unsigned char deviceNo[4];	//0-3:上车时的设备编号（汽车编号）(BCD)
	unsigned char DriverNo[3];	//4-6:上车时的司机编号 (BCD)
	unsigned char NstationNO;	//7:反向站点编号（HEX）
	unsigned char BUSerial[4];	//8-11:上车时的设备序列号 (BCD)
	unsigned char getdownTimes;	//12:下车刷卡次数=00（HEX））
	unsigned char timeDly[2];	//13-14:本次上车允许最长时间长度 (HEX 单位分钟)
	unsigned char Cxor;			//15：校验
}stFDCard2;
typedef struct {
	unsigned char LineNo[3];	//线路编号，和公用的一致
	unsigned char CXulieNo;		//票价卡序号
	unsigned char CPriceNo[2];	//票价分类后的个数
	unsigned char StationNum;	//线路总站数
	unsigned char PlusPrice[2];	//附加费
	unsigned char LianxuDly;	//连续刷卡延时时长
	unsigned char NeedMaxTime[2];	//本线路单趟运行最大时长
	unsigned char TottalPrice[2];	//本线路全程票价
	unsigned char StartPrice;		//起步价，同站下车的价格
	unsigned char Cxor;			//校验
}stFDLineHead;	//分段线路头，卡中是第1扇区每2块。
//----------------------------------------------

#include "SlzrTypeDef.h"
#include "UtilityProc.h"

#endif
