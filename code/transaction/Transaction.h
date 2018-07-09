#ifndef _INCLUDED_TRANSACTION_H
#define _INCLUDED_TRANSACTION_H

#define _CITY_CODE_ZZ_V	"\x41\x20"	// code of ZhuZhou

#define WHICH_CITY_CODE	_CITY_CODE_ZZ_V	


// card application type
typedef enum {
	CARD_INVALID_VAL = -1,			// 卡类型非法,0xFFFFFFFFFF
	CARD_NORMAL = 0,			// 普通卡A,0x0
	CARD_STUDENT = 1,			// 学生卡,0x1
	CARD_OLDMAN = 2,			// 老人卡,0x2
	CARD_STAFF = 3,			// 员工卡,0x3
	CARD_STAFFTEMP = 4,			// 临时员工卡,0x4
	CARD_FAMILY = 5,			// 家属卡,0x5
	CARD_CY = 6,			// 残疾人卡,0x6
	CARD_LAST = 7,			// 普通卡B,0x7
	CARD_LINE_SET1 = 22,			// 线路设置卡大连,0x16
	CARD_LINE_SET2 = 23,			// 线路设置卡移动,0x17
	CARD_LINE_SET3 = 24,			// 线路设置卡省通,0x18
	CARD_DATA_GATHER = 61,			// 数据采集卡,0x3D
	CARD_DEV_SET = 62,			// 设备设置卡,0x3E
	CARD_LINE_SET = 63,			// 线路设置卡,0x3F
	CARD_NUMBER_SET = 64,			// 数码设置卡,0x40
	CARD_DELAY_SET = 65,			// 延时卡,0x41
	CARD_TOLL_START = 66,			// 收费启动卡,0x42
	CARD_TIME_SET = 67,			// 时间设置卡,0x43
	CARD_LEV_BUS = 68,			// 发车卡,0x44
	CARD_ARV_BUS = 69,			// 到站卡,0x45
	CARD_DRIVER_BUS = 70,			// 司机卡,0x46
	CARD_CLERK_BUS = 71,			// 站务员卡,0x47
	CARD_AIRCON = 72,			// 空调卡,0x48
	CARD_MONTH_STA = 73,			// 月票启动卡,0x49
	CARD_WHITE_BUS = 50,			// 白卡,0x32
}emCardAppType;

typedef enum {
	ST_ERROR = 0,			// ,0x0
	ST_OK = 1,			// tap card success,0x1
	ST_OUT = 2,			// 重新寻卡,0x2
	CARD_BLACK = 80,			// 黑名单卡标识,0x50
	CARD_NO_SYSTEM = 82,			// ,0x52
	CARD_MONEY = 83,			// m1 普通钱包处理,0x53
	CARD_INVALID = 84,			// 灰名单标识,0x54
	CARD_MONTH = 85,			// ,0x55
	CARD_CONTROL = 86,			// 管理卡,0x56
	CARD_BLACK_PRO = 87,			// 黑名单卡标识,0x57
	CARD_NO_USE = 90,			// 该卡没有启用,0x5A
	CARD_FREE = 94,			// ,0x5E
	CARD_STOP_WORK = 95,			// 该卡被停用,0x5F
	CARD_CANT_CONTINUOUS = 98,			// 继续刷卡,0x62
	MONEY_OUT_LIMIT = 100,			// 该卡余额大于限额,0x64
	MONEY_NEGATIVE = 101,			// 该卡钱包为负值,0x65
	CPU_COMPLETE = 104,			// 完成记录,0x68
	TAP_AGAIN_1 = 112,			// 密钥错误,0x70
	TAP_AGAIN_2 = 113,			// 请重刷,0x71
	NINEFLAG = 65535,			// 9000结尾标志,0xFFFF
	FORBINCITT = 128,			// 禁用区域卡,0x80
	CARD_NOT_INTERFLOW = 132,			// 非互联互通卡,0x84
	PSAM_ERROR = 133,			// 请插入PSAM卡,0x85
	NOTLNTCPU = 254,			// 非岭南通CPU卡,0xFE
}emCardExeRet;


typedef enum {
	Err_YearAudit = 161,			// ,0xA1
	Err_BeforeStartDate = 162,			// ,0xA2
	Err_NotHuTong = 163,			// 交通部-非互通卡,0xA3
	Err_NEGATIVE = 164,			// 该卡钱包为负值,0xA4
	Err_OUT_LIMIT = 165,			// 该卡余额大于限额,0xA5
	Err_NoInSystem = 166,			// ,0xA6
	Err_EpurseForbid = 167,			// ,0xA7
	Err_Invalid = 168,			// ,0xA8
	Err_CardOperFail = 169,			// 操作卡片错误,0xA9
	Err_ContinuousCard = 170,			// 不能连续刷卡片,0xAA
	Err_StopCard = 171,			// 停用卡,0xAB
	Err_MonthData = 172,			// 月票区错误,0xAC
	Err_MoneyData = 173,			// 钱包区错误,0xAD
	Err_GetData = 174,			// 采集数据错误,0xAE
	Err_Record = 175,			// 记录错误,0xAF
	Err_MonthMoney = 176,			// 处理月票钱包区错误,0xB0
	Err_InMoney = 177,			// 钱包余额不足,0xB1
	Err_ExMoney = 178,			// 钱包超极限,0xB2
	Err_Card = 179,			// 卡异常,0xB3
	Err_GetMiKey = 180,			// 计算密钥错误,0xB4
	Err_Expired = 181,			// 超有效期,0xB5
	Err_CardBlack = 182,			// 黑名单卡,0xB6
	Err_CardNo = 183,			// 卡未启用,0xB7
	Err_CardStop = 184,			// 卡已停用,0xB8
	Err_InMonth = 185,			// 钱包余次不足,0xB9
	Err_initpsam = 186,			// ,0xBA
	Err_IsLNT_MIF = 187,			// ,0xBB
	Err_NotWhite = 188,			// ,0xBC
	Err_UnExistCard = 189,			// 卡类不存在,0xBD
	Err_KeyIndex = 190,			// ,0xBE
	Err_NotInterFlow = 191,			// 非互联互通卡,0xBF
	Err_KeyError = 192,			// 密钥不对,0xC0
	Err_Money = 193,			// 扣款不正确,0xC1
	Err_MonthData1 = 194,			// 月票区错误,0xC2
	Err_MonthData2 = 195,			// 月票区错误,0xC3
	Err_AppLocked = 196,			// 应用已锁定,0xC4
	Err_LockCardFail = 197,			// 黑名单卡,锁卡失败,0xC5
	Err_MAC2Des = 198,			// 校验MAC2失败,0xC6
}emErrorMsg;

typedef enum
{
	CT_USER_CARD,
	CT_CONTROL_CARD,
	CT_OTHER
}emCardKind;

#ifdef __cplusplus
extern "C"
{
#endif


	extern int GetCardKind(int cardType);
	extern int InitTransaction(void);
	/**
	* @Description - process card transaction
	*
	* @Return int(ERet_Code) - 0 is successful
	*/
	extern int ProcessCardTransaction(void);

#ifdef __cplusplus
};
#endif


#endif



