/*==========================================================================*
 *    Copyright(c) 2008-2009, SANLIAN., Ltd.
 *                     ALL RIGHTS RESERVED
 *
 *  PRODUCT  :
 *
 *  FILENAME : qPBOC.c
 *  CREATOR  :             DATE: 2009-08-05 10:11
 *  VERSION  : V1.00
 *  PURPOSE  :
 *
 *
 *  HISTORY  :
 *
 *==========================================================================*/
#ifndef _H_QPBOC_H
#define _H_QPBOC_H
#include "../include/include.h"
#include "xSys_Lantaiyuan.h"

 //需要打开  _debug_
 //#define QPBOC_DEBUG
 //#define QPBOC_DEBUG_ER

 //银行存贮空间长度
#define qPbocRECORD_LEN 256

#ifdef	_debug_ 
#define ERROR(...) printf(__VA_ARGS__)
#else
#define ERROR(...)
#endif
#if defined  _debug_stop
extern unsigned char stop;
#endif
typedef	unsigned char U8, INT8U;
typedef unsigned short U16, INT16U;
typedef unsigned int U32, INT32U;
typedef	signed char S8, INT8S;
typedef signed short S16, INT16S;
typedef signed int S32, INT32S;

#define EXTERN		extern

#ifndef IN
#define IN
#define OUT
#endif

#define SWAP16(x)	(((x & 0xff) << 8) | (x >> 8))
#define SWAP32(x)   (((x>>24)&0x000000ff) |((x>>8)&0x0000ff00)|((x<<8)&0x00ff0000)|((x<<24)&0xff000000))

//一拍通代码

#define INFO_81		"\x00\x00\x00\x00"	/*授权金额二进制*/
#define INFO_95     "\x00\x00\x00\x00\x00"                       /* 终端验证结果(TVR) */
#define INFO_9A		"\x00\x00\x00"								/*交易日期YYMMDD*/
#define INFO_9B		"\x00\x00"			                        /*交易状态信息TSI 都在应用初始化后做的所以都是默认为00*/
#define INFO_9C     "\x00"                                    	/* 交易类型 */
#define INFO_9F01	"\x11\x22\x33\x44\x55\x66"					/*收单行标识*/
#define INFO_9F02	"\x00\x00\x00\x00\x00\x00"					/*授权金额*/
#define INFO_9F03   "\x00\x00\x00\x00\x00\x00"                   /* 其它金额 */
#define INFO_9F04	"\x00\x00\x00\x00"	/*其他金额*/
#define INFO_9F09	"\x00\x20"	/*应用版本*/
#define INFO_9F15	"\x00\x01"	/*N4 商户分类码*/
#define INFO_9F16	"\xB4\xF3\xC3\xF7\xCE\xE5\xD6\xDE\xB3\xB5\xD4\xD8\x50\x4F\x53"	/*ans15 商户标识(大明五洲车载POS)*/
#define INFO_9F1A   "\x01\x56"                                  	/* 终端国家代码 */
#define INFO_9F1B	"\x00\x00\x00\x00"	/*终端最低限额*/
#define INFO_9F1C	"\x31\x32\x33\x34\x35\x36\x37\x38"	/*an8 终端标识ASC*/
#define INFO_9F1D	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
// #define INFO_9F1E	"EFS3000L"		/*an8 接口设备序列号，厂商分配的永久序列号DMWZ-A01*/
#define INFO_9F1E	"54004318"		/*an8 接口设备序列号，百富pos报文里面*/
#define INFO_9F21	"\x00\x00\x00"	/*交易时间HHMMSS*/
#define INFO_5F2A   "\x01\x56"                                 	/* 交易货币代码 */
#define INFO_5F2D   "\x63\x6ex65\x6ex00\x00x00\x00"           	/* 首选语言*/
//#define INFO_9F33	"\x00\x08\xC0"	/*b3 终端性能*/
//#define INFO_9F33	"\x00\x00\x01"	/*b3 终端性能*/
//#define INFO_9F33	"\x20\x89\xc8"	/*b3 终端性能*/
#define INFO_9F33	"\xE0\xE1\x80"	/*b3 终端性能百富pos报文里面*/
#define INFO_9F34	"\x00\x00\x00"	/*持卡人验证方法CVM结果*/
#define INFO_9F35	"\x26"		/*终端类型*/
#define INFO_5F36	"\x02"	/*交易货币指数*/
#define INFO_9F37	"\x00\x00\x00\x00"							/*不可预知数*/
#define INFO_9F39		"\x07"	/*销售点POS输入方式*/
#define INFO_9F3A	"\x00\x00\x00\x00"	/*参考货币金额*/
#define INFO_9F3C	"\x01\x56"	/*交易参考货币代码*/
#define INFO_9F3D		"\x02"	/*交易参考货币指数*/
#define INFO_9F40	"\xA0\x00\x10\x10\x01"		/*附加终端能力*/
#define INFO_9F41	"\x00\x00\x10\x43"	/*交易序号计数器*/
#define INFO_9F4E	"\xB4\xF3\xC3\xF7\xCE\xE5\xD6\xDE\xB3\xB5\xD4\xD8\x30\x31\x32"	/*商品名称*/
#define INFO_9F66   "\x28\x00\x00\x00"                           /* 终端交易属性 */ //28仅脱机QPBOC不支持纸质签名
#define INFO_9F66_1   "\x08\x00\x00\x00"                       /* 终端交易属性 */ //28仅脱机QPBOC不支持纸质签名
#define INFO_9F66_2   "\x28\x80\x00\x00"                       /* 终端交易属性 */ //28仅脱机QPBOC不支持纸质签名
#define INFR_8A			"Y1"						//写死"Y1"：脱机接受


//add by zhgfan 2018-1-5
typedef enum {
	PREPAID_CARD,
	CREDIT_CARD
}EMV_CARDTYPE;

typedef enum {
	E_cash,
	PAY_Online,
	ODA,
	UICS
}PBOC_CHANNEL;
typedef union
{
	INT8U	CharBuff[2];
	INT16U	IntBuff;
} TYPE_CHAR_INT;

typedef union
{
	INT8U	CharBuff[4];
	INT16U	IntBuff[2];
	INT32U	LongBuff;
}TYPE_CHAR_LONG;

// PBOC 定义的错误代码
enum FUN_QPBOC_RETURN_ERRCODE {
	qPBOC_SUCESS_ECU = 0,			//4 命令执行成功
	qPBOC_TRADE_ABORT = 1,			//4 交易终止
	qPBOC_APDU_CMD = 84,			//4 卡操作APDU失败
	qPBOC_CARD_ACTIVE = 85,			//4 激活卡失败
	qPBOC_CARD_ANTICOLL = 86,			//4 防冲突失败
	qPBOC_NO_CARD = 87,			//4 没有检测到卡
	qPBOC_NOOK_PATH = 88,			//4 没有匹配的交易路径
	qPBOC_POS_NOFUNCTION = 89,			//4 该终端不支持该功能
	qPBOC_NO_QPBOC = 90,			//4 不支持QPBOC功能
	qPBOC_FCI_LOSTDATA = 91,			//4 FCI模块强制数据缺失
	qPBOC_MUTL_CARD = 92,			//4 检测到多张卡
	qPBOC_TRADEMEONY_ZERO = 93,			//4 交易金额为0
	qPBOC_MONEYOVERLIMIT = 94,			//4 交易终止:交易金额超过终端非接触卡交易限额
	qPBOC_APPFILE_FAILE = 95,			//4 选择文件失败
	qPBOC_APPFILE_INVALID = 96,			//4 应用文件无效
	qPBOC_APPFILE_NO_FIND = 97,			//4 应用文件没有找到
	qPBOC_APPFILE_LOCK = 98,			//4 应用文件已经被锁
	qPBOC_SELECT_NO_APP = 99,			//4 应用中没有发现应用文件，交易终止
	qPBOC_PUBLISHKEY_LEN = 100,		//4 发卡行公钥长度与CA KEY长度不相等
	qPBOC_RSA_ERROR = 101,		//4 计算RSA出错
	qPBOC_PUBLISHC_FORMAT = 102,		//4 发卡行公钥证书格式不对
	qPBOC_PUBLISHC_SIGN = 103,		//4 发卡行公钥证书SHA值错
	qPBOC_PUBLISHC_FLAG = 104,		//4 发卡行公钥证书发卡行标识出错
	qPBOC_READ_SYSTIME = 105,		//4 读系统时间出错
	qPBOC_PUBLISHC_EXP = 106,		//4 发卡行公钥证书过期
	qPBOC_PUBLISHC_PKEYF = 107,		//4 发卡行公钥标识
	qPBOC_ICC_NOEXIT = 108,		//4 IC卡公钥证书没有找到
	qPBOC_IKEYSGLENNO = 109,		//4 签名静态数据长度跟发卡行公钥长度不一致 
	qPBOC_ICC_LEN = 110,		//4 IC卡的公钥证书长度不对
	qPBOC_ICC_FORMAT = 111,		//4 恢复IC卡公钥证书格式出错
	qPBOC_ICC_STAG_NO82 = 112,		//4 静态应用标签中没有找到82
	qPBOC_OFFLINEAUTENDATA_INVALID = 113,	//4 脱机认证数据非法
	qPBOC_ICC_SIGN = 114,		//4 IC卡公钥证书SHA值错
	qPBOC_ICC_PANFLAG = 115,		//4 IC卡的账号与主账号不一致
	qPBOC_ICC_EXP = 116,		//4 IC卡公钥证书过期
	qPBOC_ICC_PKEYF = 117,		//4 IC卡公钥标识
	qPBOC_DDA_DATAFORMAT = 118,		//4 DDA某些数据没有找打
	qPBOC_DDA_DATAKEYLEN = 119,		//4 DDA数据与IC卡PKEY长度不一致
	qPBOC_DDA_SIGN = 120,		//4 IC卡公钥证书SHA值错
	qPBOC_CAINDEX_NOEXIT = 121,		//4 CA公钥索引不存在
	qPBOC_PUBLISHC_NOEXIT = 122,		//4 发卡行公钥证书不存在
	qPBOC_PUBLISHE_NOEXIT = 123,		//4 发卡行公钥指数不存在
	qPBOC_CAKEYRID_NOEXIT = 124,		//4 卡的RID没有找到
	qPBOC_SIGNSTATIC_NOEXIT = 125,		//4 签名的静态数据
	qPBOC_AFL_NOEXIT = 126,		//4 AFL 不存在
	qPBOC_STATICDATA_NOEXIT = 127,		//4 需要认证的静态数据不存在
	qPBOC_CAPKEY_NOEXIT = 128,		//4 CA公钥不存在
	qPBOC_NOFIND_AUTEN = 129,		//4 没有合适的认证方法
	qPBOC_ICCE_NOEXIT = 130,		//4 IC卡公钥指数没有找到
	qPBOC_ICSIGNDYDATA_NOEXIT = 131,		//4 签名的动态数据没有知道
	qPBOC_NOUNDOWNDATA_NOEXIT = 132,		//4 不可预见数据不存在
	qPBOC_NO_CDA = 134,		//4 QPBOC 不支持CDA
	qPBOC_RESUL_AAC = 135,		//4 脱机拒绝
	qPBOC_RESUL_TC = 136,		//4 脱机批准
	qPBOC_RESUL_ARQC = 137,		//4 联机批准
	qPBOC_RESUL_RFU = 138,		//4 未知行为
	qPBOC_NOFIND_9F10 = 139,		//4 没有找到9F10卡片交易属性
	qPBOC_AACARQC_NOM = 140,		//4 脱机拒绝或联机缺少必需数据
	qPBOC_TC_NOM = 141,		//4 脱机批准缺少必需数据
	qPBOC_9F38_NOEXIT = 142,		//4 PDOL不存在
	qPBOC_9F66_NOEXIT = 143,		//4 终端交易属性不存在
	qPBOC_SHA_FLAG = 144,		//4 哈希算法标识错误
	qPBOC_AFL_SFI_ERROR = 145,		//4 AFL 中SFI出错
	qPBOC_OFFMONEY0 = 146,		//4 脱机交易金额为0
	qPBOC_OFFNOLINE = 147,		//4 脱机设备不能联机
	qPBOC_LINERESUL_AAC = 148,		//4 设备不能联机拒绝
	qPBOC_VER_NO_SAME = 149,		//4 终端和卡应用版本不一致
	qPBOC_CONTRY_OUTTRADE = 150,		//4 国家代码不同，不能进行国际交易
	qPBOC_CONTRY_INTRADE = 151,		//4 国家代码相同,不能进行国内交易
	qPBOC_NOATM_INVALID = 152,		//4 非ATM终端上无效
	qPBOC_ATM_INVALID = 153,		//4 ATM终端上无效
	qPBOC_STARTTIME_INVLAID = 154,		//4 卡没有生效
	qPBOC_ENDTIME_INVLIAD = 155,		//4 卡已失效
	qPBOC_BLACK_CARD = 156,		//4 黑名单卡 
	qPBOC_REPEAT_TAG = 157,		//4 重复标签出现
	qPBOC_TVL_INVALID = 158,		//4 TVL 格式非法
	qPBOC_DATA_NO70 = 159,		//4 数据模块错误
	qPBOC_DATA_SIGAN_NO70 = 160,
	qPBOC_QpboC_SIGAN_NO = 161,	//卡信息没找到
	qPBOC_QpboC_tryAgain = 162,	//需要重刷
	qPBOC_CARD_forbid = 163,	//卡禁止刷
	qPBOC_SDA_DATAFORMAT = 164,		//4 SDA某些数据没有找打
	qPBOC_SDA_DATAKEYLEN = 165,		//4 SDA数据与IC卡PKEY长度不一致
	qPBOC_SDA_SIGN = 166,		//4 SDA SHA值错
	qPBOC_lostData = 167,		//4 数据缺失
	qPBOC_CARD_forbid_oda = 168,		//4 0da
	qPBOC_DEFAULT = 0xFFF
};

#define RSA_NO_PADDING		3

#define 	A1_MSD			0x80
#define	A1_NPBOC		0x40
#define	A1_QPBOC		0x20
#define	A1_YPBOC		0x10
#define	A1_OFFLINE		0x08
#define	A1_ONLINEPIN	0x04
#define	A1_ODA			0x01
#define	A1_SIGN		0x02
#define	A2_ONLINEKEY	0x80
#define	A2_CVM			0x40

#define	TYPE_AUTEN_NO				0x00
#define	TYPE_AUTEN_SDA				0x01
#define	TYPE_AUTEN_DDA				0x02
#define	TYPE_AUTEN_CDA				0x03

#define	AIP_SDA					0x40		//4 支持SDA
#define	AIP_DDA					0x20		//4 支持DDA		
#define	AIP_CVM					0x10		//4 支持持卡人认证
#define	AIP_POS_RISP				0x08		//4 执行终端风险管理
#define	AIP_PUBLISH_AUTEN			0x04		//4 支持发卡行认证
#define	AIP_CDA					0x01		//4 支持CDA

//bytes1
#define	CA_MANINPUT				0x80		//4 手工键盘输入
#define	CA_MAG						0x40		//4 磁条
#define	CA_CONTACTCARD			0x20		//4 接触式IC卡
//bytes2
#define	CA_ICPINAUTEN				0x80		//4 IC卡明文PIN验证
#define	CA_DESPINAUTEN			0x40		//4 IC卡加密PIN验证
#define 	CA_SIGN_PAGE				0x20		//4 签名纸
#define	CA_NO_CVM					0x08		//4 无需CVM	
#define	CA_MAN_CVM				0x01		//4 持卡人证件验证
//tytes3
#define	CA_SDA						0x80		//4 静态数据认证
#define	CA_DDA						0x40		//4 动态数据认证
#define	CA_GULP_CARD				0x20		//4 吞卡
#define	CA_CDA			

//byte1
#define	TVR_NO_OFFLINEAUTEN		0x80		//4 未进行脱机数据认证
#define	TVR_OFFLINEAUTEN_FAILE	0x40		//4 脱机数据认证失败
#define	TVR_ICDATA_LOST			0x20		//4 IC卡数据缺失
#define	TVR_IC_ABORTFILE			0x10		//4 IC卡存在于异常文件中
#define	TVR_OFFDDAAUTENFAILE		0x08		//4 脱机动态数据认证失败
#define	TVR_CDAAUTENFAILE			0x04		//4 复合动态数据认证失败
//bytes2
#define	TVR_ICPOS_VERERR			0x80		//4 IC卡和POS版本不一致
#define	TVR_APP_EXP				0x40		//4 应用已过期
#define	TVR_APP_INVALID			0x20		//4 应用没有生效
#define	TVR_ICNOREQSERVER			0x10		//4 卡片不允许所请求的服务
#define	TVR_NEW_CARD				0x08		//4 新卡
//bytes3
#define	TVR_CVMAUTEN_FAILE		0x80		//4 持卡人验证失败
#define	TVR_NOUNDOW_CVM			0x40		//4 未知的CVM
#define	TVR_PINTRY_SUMOVER		0x20		//4 PIN重试次数超限
#define	TVR_INPIN_KEYERROR		0x10		//4 要求输入PIN，但密码键盘不存在或工作不正常
#define	TVR_INPIN_NOPIN			0x08		//4 要求输入PIN，密码键盘存在，但未输入PIN
#define	TVR_INONLINEPIN			0x04		//4 输入联机PIN
//tyes4
#define	TVR_OVERLLIMITMONEY		0x80		//4 交易超过最低限额
#define	TVR_OVER_OFFLINE_LIMITL	0x40		//4 超过连续脱机交易下限
#define	TVR_OVER_OFFLINE_LIMITH 	0x20		//4 超过连续脱机交易上限
#define	TVR_RAMSELONLINEAUTEN	0x10		//4 交易被随机选择联机交易
#define	TVR_BRANDSELONLINEAUTEN	0x08		//4 商户选择联机交易
//bytes5
#define	TVR_USE_DEFAULT_PDOL		0x80		//4 使用缺省TDOL
#define	TVR_PUBLISH_AUTEN_FAILE	0x40		//4 发卡行认证失败
#define	TVR_BEFGC_CRE_FAILE		0x20		//4 最后一次GENERATE AC命令之前脚本处理失败
#define	TVR_AFGC_CRE_FAILE		0x10		//4 最后一次GENERATE AC命令之后脚本处理失败

#define	TSI_OFFAUTEN_EXE			0x80		//4 脱机数据认证已进行
#define	TSI_CVMAUTEN_EXE			0x40		//4 持卡人验证已进行
#define	TSI_CARDRIST_EXE			0x20		//4 卡片风险管理已进行
#define	TSI_PUBLISHAUTEN_EXE		0x10		//4 发卡行认证已进行
#define	TSI_POSRIST_EXE			0x08		//4 终端风险管理已进行
#define	TSI_CRE_EXE				0x04		//4 脚本处理已进行


//标签定义
#define 	T_FCI_MODULE			0x6F		//4 FCI 模块
#define	T_SPECAI_FILE_DF		0x84		//4 专用文件名DF
#define	T_FCI_SPE_MOUDLE		0xA5		//4 FCI 专用模块
#define 	T_FCI_ISSIA_DATA		0xBF0C		//4 FCI 发卡行自定义数据
#define	T_DIR_ENTER			0x61		//4 目录入口
#define	T_APP_FILE_AID			0x4F		//4 DF名(AID)
#define 	T_APP_TARGE			0x50		//4 应用标签
#define	T_APP_USER_PRIOR		0x87		//4 应用优先指示器
#define	T_DEV_TRADE_AATR		0x9F66		//4 终端交易属性
#define 	T_ACCREDIT_MONEY		0x9F02		//4 授权金额
#define	T_OTHER_MONEY			0x9F03		//4 其他金额
#define	T_DEV_COUNTY_CODE	0x9F1A		//4 终端国家代码
#define	T_DEV_TVR				0x95		//4 终端验证结果
#define	T_TRADE_COIN_CODE	0x5F2A		//4 交易货币代码
#define	T_TRADE_TIME			0x9A		//4 交易日期
#define	T_TRADE_TYPE			0x9C		//4 交易类型
#define	T_FORECAST_DATA		0x9F37		//4 不可预见的数据
#define 	T_AIP					0x82
#define	T_ATC					0x9F36
#define	T_AFL					0x94
#define	T_AAC					0x9F26		//Application Authentication Cryptogram
#define 	T_IAD					0x9F10		// Issuer Application Data
#define	T_MSD2					0x57
#define	T_PANS					0x5F34
#define	T_SDAD					0x9F4B
#define	T_CARDTRAT				0x9F6C		//4 卡片交易属性
#define	T_OFFMONEY			0x9F5D		//4 可用的脱机金额
#define	T_MONEYCODE			0x9F51		//4 已使用的货币代码
#define	T_ADDINGMSG			0x9F68		//4 卡片附加信息
#define	 T_LAST_INLINECNT		0x9F13		//4 上次联机交易计数器
#define	 T_PINTRYSUM			0x9F17		//4 PIN 重试的次数
#define 	T_CVM_MONEY_OVER		0x9F6B		//4 CVM 限额
#define	T_CVMNAME				0x5F20		//4 CVM 的姓名

#define	T_AVLIDTIME			0x5F24		//4 失效日期
#define	T_MAINPAN				0x5A		//4 主账号
#define	T_APP_CTRL				0x9F07		//4 应用版本号
#define	T_CVM_LIST				0x8E		//4 CVM 列表
#define	T_IAC_DEFAULT			0x9F0D		//4 发卡行的默认行为代码
#define	T_IAC_REFUSE			0x9F0E		//4 发卡行的拒绝行为代码
#define	T_IAC_ONLINE			0x9F0F		//4 发卡行的联机行为代码
#define	T_CONTRYCODE			0x5F28		//4 发卡行的国家代码


#define	T_ICPUBKEY_E			0x9F47		//4 IC卡的公钥指数
#define	T_ICPUBKEY_N_MOD		0x9F48	//4 	//IC卡公钥的余项
#define	T_DDOL					0x9F49		//4 动态数据对象列表
#define	T_SDATAG				0x9F4A		//4 静态数据认证标签
#define	T_NAME					0x9F0B		//4 持卡人姓名
#define	T_SERVERCODE			0x5F30		//4 服务码
#define	T_APP_VER				0x9F08		//4 应用版本号
#define	T_APP_VALID			0x5F25		//4 应用生效日期
#define	T_E_PUBISH				0x9F23		//4 发卡行公钥指数
#define	T_N_PUBLISH_MOD		0x92		//4 发卡行公钥余项
#define	T_PULISHAUTENCODE	0x9F74		//4 发卡行授权码

#define	PT_MAN_ONLINE			1			//4 有服务员:仅仅联机
#define	PT_MAN_ONLINEOFF		2			//4 有服务员:有联机能力的脱机
#define	PT_MAN_OFFLINE		3			//4 有服务员:仅仅脱机
#define	PT_NOMAN_ONLINE		4			//4 自助的：仅仅联机
#define	PT_NOMAN_ONLINEOFF	5			//4 自助的：有联机能力的脱机
#define	PT_NOMAN_OFFLINE		6			//4 自助的：仅仅脱机

#define	PT_ORG_BANK			0x10		//4 金融机构
#define	PT_ORG_SHOP			0x20		//4 商户机构
#define	PT_ORG_CVMAN			0x30		//4 持卡人

//bytes1
#define	PTA_CASH					0x80		//4 现金
#define	PTA_COMMODITY				0x40		//4 商品
#define	PTA_SERVER					0x20		//4 服务
#define	PTA_RETURNCASH			0x10		//4 反现
#define	PTA_CHECK					0x08		//4 查询
#define	PTA_TURNCASH				0x04		//4 转账
#define	PTA_DEBIT					0x02		//4 付款
#define	PTA_MANAGE				0x01		//4 管理
//bytes2
#define	PTA_DEPOSIT				0x80		//4 存款交易
//bytes3
#define	PTA_NUMKEY				0x80		//4 数字键
#define	PTA_ALPKEY					0x40		//4 字母和特殊字符
#define	PTA_COMMANDKEY			0x20		//4 命令键
#define	PTA_FUNCTIONKEY			0x10		//4 功能键
//byte4
#define	PTA_PRN_SERVER			0x80		//4 打印，服务员
#define	PTA_PRN_CVM				0x40		//4 打印，持卡人
#define	PTA_DIS_SERVER			0x20		//4 显示，服务员
#define	PTA_DIS_CVM				0x10		//4 显示，持卡人
#define	PTA_TAB10					0x02		//4 编码表10
#define	PTA_TAB9					0x01		//4 编码表9
//byte5
#define	PTA_TAB8					0x80		//4 编码表8
#define	PTA_TAB7					0x40		//4 编码表7
#define	PTA_TAB6					0x20		//4 编码表6
#define	PTA_TAB5					0x10		//4 编码表5
#define	PTA_TAB4					0x08		//4 编码表4
#define	PTA_TAB3					0x04		//4 编码表3
#define	PTA_TAB2					0x02		//4 编码表2
#define	PTA_TAB1					0x01		//4 编码表1

#define	B_PUBLISHC_HEADER					0		//4 恢复数据头1
#define	B_PUBLISHC_CERF					1		//4 证书格式	1
#define	B_PUBLISHC_ISSID					2		//4 发卡行标识 4
#define	B_PUBLISHC_EXPTIME				6		//4 失效日期	2
#define	B_PUBLISHC_CSERIAL				8		//4 证书序列号	3
#define	B_PUBLISHC_SHAFLAG				11		//4 SHA算法标识	1
#define	B_PUBLISHC_PKEYFLAG				12		//4 公钥算法标识	1
#define	B_PUBLISHC_PKEYLEN				13		//4 公钥长度		1
#define	B_PUBLISHC_PKEYELEN				14		//4 公钥指数长度	1
#define	B_PUBLISHC_PKEY					15		//4 公钥最左边字节


#define	B_ICC_HEADER						0		//4 恢复数据头1
#define	B_ICC_CERF							1		//4 证书格式	1
#define	B_ICC_APPPAN						2		//4 应用主账号10
#define	B_ICC_EXPTIME						12		//4 失效日期	2
#define	B_ICC_CSERIAL						14		//4 证书序列号	3
#define	B_ICC_SHAFLAG						17		//4 SHA算法标识	1
#define	B_ICC_ICPKEYFLAG					18		//4 公钥算法标识	1
#define	B_ICC_PKEYLEN						19		//4 公钥长度		1
#define	B_ICC_PKEYELEN						20		//4 公钥指数长度	1
#define	B_ICC_ICPKEYLEFT					21		//4 公钥最左边字节	

//4 输出结果信息
typedef struct
{
	INT8U 	Result[2];			//4 交易结果	0
	INT8U 	TradeType;			//4 交易类型	2
	INT8U 	TradeMoney[4];		//4 交易金额	3
	INT8U	BefMoney[4];		//4 交易后金额	7
	INT8U 	D_9F4C[8];			//4 IC卡动态数据11
	INT8U 	TVR[5];				//4 交易结果	19
}QPBOC_RESULT_TYPE;


typedef struct {
	INT16U    T;
	INT32U 	L;
	INT8U	V[248];
}BER_TVL;

typedef struct {
	INT32U 	TagValue;
	INT32U 	SizeOff;
}TAG_OFFSTU;

typedef struct
{
	TAG_OFFSTU 	t_4F;				//4 DF名称
	TAG_OFFSTU	t_50;				//4 应用标签
	TAG_OFFSTU 	t_57;				//4 2磁道数据
	TAG_OFFSTU 	t_5A;				//4 应用主账号(PAN)
	TAG_OFFSTU 	t_61;				//4 目录入口
	TAG_OFFSTU 	t_6F;				//4 FCI模块
	TAG_OFFSTU  t_81;				//4 交易金额HEX (4)
	TAG_OFFSTU	t_82;				//4 应用交互特征
	TAG_OFFSTU	t_84;				//4 专用文件控制名DF
	TAG_OFFSTU	t_8C;				//CDOL1
	TAG_OFFSTU 	t_8E;				//4 持卡人验证方法(CVM)列表
	TAG_OFFSTU 	t_8F;				//4 CA公钥索引
	TAG_OFFSTU 	t_90;				//4 发卡行公钥证书
	TAG_OFFSTU 	t_93;				//4 签名的静态应用数据
	TAG_OFFSTU 	t_92;				//4 发卡行公钥余数
	TAG_OFFSTU 	t_94;				//4 应用文件定位器
	TAG_OFFSTU  t_95;				//4 终端验证结果
	TAG_OFFSTU  t_9A;				//4 交易日期
	TAG_OFFSTU  t_9C;				//4 交易类型
	TAG_OFFSTU  t_A5;				//4 FCI专用模块
	TAG_OFFSTU  t_5F20;				//4 持卡人姓名
	TAG_OFFSTU 	t_5F24;				//4 失效日期
	TAG_OFFSTU 	t_5F25;				//4 应用生效日期
	TAG_OFFSTU 	t_5F28;				//4 发卡行国家代码
	TAG_OFFSTU 	t_5F2A;				//4 交易货币代码
	TAG_OFFSTU 	t_5F2D;				//4 首选语言
	TAG_OFFSTU 	t_5F30;				//4 服务码
	TAG_OFFSTU 	t_5F34;				//4 应用PAN序列号
	TAG_OFFSTU 	t_9F02;				//4 交易授权金额（不包括调整）
	TAG_OFFSTU 	t_9F03;				//4 与交易相关的第二金额，表示返现金额
	TAG_OFFSTU  t_9F06;				//4 //4  选文件AID
	TAG_OFFSTU 	t_9F07;				//4 应用用途控制 -AUC
	TAG_OFFSTU 	t_9F08;				//4 应用版本号
	TAG_OFFSTU 	t_9F0B;				//4 持卡人姓名扩展
	TAG_OFFSTU 	t_9F0D;				//4 发卡行行为代码（IAC）-缺省
	TAG_OFFSTU 	t_9F0E;				//4 发卡行行为代码（IAC）-拒绝
	TAG_OFFSTU 	t_9F0F;				//4 发卡行行为代码（IAC）-联机
	TAG_OFFSTU 	t_9F10;				//4 发卡行应用数据
	TAG_OFFSTU 	t_9F11;				//4 发卡行代码表索引
	TAG_OFFSTU 	t_9F12;				//4 应用首选名称
	TAG_OFFSTU  t_9F1A;				//4 终端国家代码
	TAG_OFFSTU	t_9F26;				//4 应用密文
	TAG_OFFSTU	t_9F27;				//4 密文信息数据
	TAG_OFFSTU 	t_9F32;				//4 发卡行公钥指数
	TAG_OFFSTU 	t_9F36;				//4 ATC
	TAG_OFFSTU 	t_9F37;				//4 不可预见的数据
	TAG_OFFSTU	t_9F38;				//4 PDOL
	TAG_OFFSTU	t_9F42;				//4 应用货币代码 如果CVM中要求金额检查，需要此数据
	TAG_OFFSTU	t_9F45;				//4 数据认证码
	TAG_OFFSTU 	t_9F46;				//4 IC卡公钥证书
	TAG_OFFSTU 	t_9F47;				//4 IC卡公钥指数
	TAG_OFFSTU 	t_9F48;				//4 IC卡公钥的余项
	TAG_OFFSTU 	t_9F49;				//4 动态数据认证数据对象列表（DDOL）
	TAG_OFFSTU 	t_9F4A;				//4 静态数据认证标签列表
	TAG_OFFSTU 	t_9F4B;				//4 签名动态应用数据
	TAG_OFFSTU 	t_9F4C;				//4 IC动态应用数据
	TAG_OFFSTU 	t_9F4D;				//4 日志入口	
	TAG_OFFSTU	t_9F51;				//0156为人民币  双免交易使用
	TAG_OFFSTU	t_DF71;				//0156为人民币双免交易使用
	TAG_OFFSTU 	t_9F5D;				//4 可用脱机消费金额
	TAG_OFFSTU	t_9F66;				//4 终端交易属性
	TAG_OFFSTU 	t_9F6C;				//4 卡片交易属性
	TAG_OFFSTU 	t_9F74;				//4 发卡行授权码
	TAG_OFFSTU	t_9F79;				//4 电子现金余额
	TAG_OFFSTU	t_BF0C;				//4 文件控制信息（FCI）发卡行自定义数据
	TAG_OFFSTU	t_DF61;				//4 文件控制信息（FCI）发卡行自定义数据

}BIT_TVL_TAG;


#define	 SUM_APP_LIST_AID			5
typedef struct {
	INT8U 	LenAID;
	INT8U 	DF_AID[20];
	INT8U 	LenTag;
	INT8U 	AppTag[12];
	INT8U 	LenPro;
	INT8U 	AppPrio[2];
}FCI_MODULE;
typedef struct
{
	unsigned char rpurSerial[6];//0-5记录流水 BCD
	unsigned char rBankCardNo[18];//6--23 银联卡编号 ASC
	unsigned char rcardMainNO[10];//24-33 卡主帐户 BCD
	unsigned char rcardlimitdata[2];//34-45 卡有效日期 YYMM
	unsigned char rcardseial[2];//36-37 卡片序列号 BCD
	unsigned char rUseKeytext[8];//38-45 应用密文 HEX
	unsigned char rUsemsginfo;//46 应用信息数据 HEX
	unsigned char rpublisBanduse[32];//47-78 发卡行应用数据 HEX
	unsigned char rRand[4];//79-82 不可预知数 HEX
	unsigned char rdealTimer[2];//83-84 应用交易计数器HEX
	unsigned char rchekend[5];//85-89 终端验证结果 HEX
	unsigned char ryyjfType[2];//90-91 应用交互特征 HEX
	unsigned char ruseVer[2];//92-93 应用版本号 HEX
	unsigned char rSrlTimer[2];//94-95 交易序列计数器 HEX
	unsigned char rLicense[6];//96-101 电子现金发卡行授权码 HEX
	unsigned char rLicenseACK[2];//102-103 授权响应码 HEX
	unsigned char rBuPOSSerial[3];//104-106 接口设备序列号 BCD  POS机编号
	unsigned char rDealMoney[2];//107-108 交易金额 HEX 低位在后
	unsigned char rAferMoney[4];//109-112 交易后余额 HEX 低位在后
	unsigned char rDatatiem[7];//113-116 交易日期 YYYYMMDD//117-119 交易时间 HHMMSS
	unsigned char rdealType;//120 交易类型 HEX
	unsigned char rbusinessCode[2];//121-122 运营商户代码 BCD
	unsigned char rBuLineDevNo[6];//123-128 线路编号+汽车编号 3字节线路编号+3字节线路编号 BCD
	unsigned char rNull;//129 预留
	unsigned char rCrc16[2];//130-131 记录校验码 前130字节的CRC16  HEX
	unsigned char rDriverNo[4];//132-135 司机编号，低位在前
	unsigned char rfDDAResult;	//136 认证结果
	unsigned char rCardType;	//137 卡类
	unsigned char rDeviceSerial[4];//138-141//设备序列号

	unsigned char rZYADD[8];//142-149 专用文件名称
	unsigned char rCDdata[16];//150-165 2磁道数据

	unsigned char rBBB[18];	//166-183 保留
}stPbocRec;
typedef struct
{
	INT32U	e_Key;
	INT32U	n_KeyLen;
	INT8U	n_Key[256];
}KEY_PUBLICMSG;



extern BIT_TVL_TAG QpbocTVLFCI;
EXTERN unsigned char GetDateTime(void);
extern void get_BER_TVL(unsigned int  bitPos, BER_TVL *OutTVL);
extern void emv_set_pay_channel(PBOC_CHANNEL channel);
extern PBOC_CHANNEL emv_get_pay_channel(void);
extern void emv_set_pay_channel(PBOC_CHANNEL channel);
extern void getMobileParameter_test(void);
extern unsigned char qrDisp_history(unsigned char mode, unsigned int Index);
extern EMV_CARDTYPE emv_get_card_type(void);
extern void emv_set_card_type(EMV_CARDTYPE type);



#include "RamStorage.h"


#endif

