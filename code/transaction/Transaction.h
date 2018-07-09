#ifndef _INCLUDED_TRANSACTION_H
#define _INCLUDED_TRANSACTION_H

#define _CITY_CODE_ZZ_V	"\x41\x20"	// code of ZhuZhou

#define WHICH_CITY_CODE	_CITY_CODE_ZZ_V	


// card application type
typedef enum {
	CARD_INVALID_VAL = -1,			// �����ͷǷ�,0xFFFFFFFFFF
	CARD_NORMAL = 0,			// ��ͨ��A,0x0
	CARD_STUDENT = 1,			// ѧ����,0x1
	CARD_OLDMAN = 2,			// ���˿�,0x2
	CARD_STAFF = 3,			// Ա����,0x3
	CARD_STAFFTEMP = 4,			// ��ʱԱ����,0x4
	CARD_FAMILY = 5,			// ������,0x5
	CARD_CY = 6,			// �м��˿�,0x6
	CARD_LAST = 7,			// ��ͨ��B,0x7
	CARD_LINE_SET1 = 22,			// ��·���ÿ�����,0x16
	CARD_LINE_SET2 = 23,			// ��·���ÿ��ƶ�,0x17
	CARD_LINE_SET3 = 24,			// ��·���ÿ�ʡͨ,0x18
	CARD_DATA_GATHER = 61,			// ���ݲɼ���,0x3D
	CARD_DEV_SET = 62,			// �豸���ÿ�,0x3E
	CARD_LINE_SET = 63,			// ��·���ÿ�,0x3F
	CARD_NUMBER_SET = 64,			// �������ÿ�,0x40
	CARD_DELAY_SET = 65,			// ��ʱ��,0x41
	CARD_TOLL_START = 66,			// �շ�������,0x42
	CARD_TIME_SET = 67,			// ʱ�����ÿ�,0x43
	CARD_LEV_BUS = 68,			// ������,0x44
	CARD_ARV_BUS = 69,			// ��վ��,0x45
	CARD_DRIVER_BUS = 70,			// ˾����,0x46
	CARD_CLERK_BUS = 71,			// վ��Ա��,0x47
	CARD_AIRCON = 72,			// �յ���,0x48
	CARD_MONTH_STA = 73,			// ��Ʊ������,0x49
	CARD_WHITE_BUS = 50,			// �׿�,0x32
}emCardAppType;

typedef enum {
	ST_ERROR = 0,			// ,0x0
	ST_OK = 1,			// tap card success,0x1
	ST_OUT = 2,			// ����Ѱ��,0x2
	CARD_BLACK = 80,			// ����������ʶ,0x50
	CARD_NO_SYSTEM = 82,			// ,0x52
	CARD_MONEY = 83,			// m1 ��ͨǮ������,0x53
	CARD_INVALID = 84,			// ��������ʶ,0x54
	CARD_MONTH = 85,			// ,0x55
	CARD_CONTROL = 86,			// ����,0x56
	CARD_BLACK_PRO = 87,			// ����������ʶ,0x57
	CARD_NO_USE = 90,			// �ÿ�û������,0x5A
	CARD_FREE = 94,			// ,0x5E
	CARD_STOP_WORK = 95,			// �ÿ���ͣ��,0x5F
	CARD_CANT_CONTINUOUS = 98,			// ����ˢ��,0x62
	MONEY_OUT_LIMIT = 100,			// �ÿ��������޶�,0x64
	MONEY_NEGATIVE = 101,			// �ÿ�Ǯ��Ϊ��ֵ,0x65
	CPU_COMPLETE = 104,			// ��ɼ�¼,0x68
	TAP_AGAIN_1 = 112,			// ��Կ����,0x70
	TAP_AGAIN_2 = 113,			// ����ˢ,0x71
	NINEFLAG = 65535,			// 9000��β��־,0xFFFF
	FORBINCITT = 128,			// ��������,0x80
	CARD_NOT_INTERFLOW = 132,			// �ǻ�����ͨ��,0x84
	PSAM_ERROR = 133,			// �����PSAM��,0x85
	NOTLNTCPU = 254,			// ������ͨCPU��,0xFE
}emCardExeRet;


typedef enum {
	Err_YearAudit = 161,			// ,0xA1
	Err_BeforeStartDate = 162,			// ,0xA2
	Err_NotHuTong = 163,			// ��ͨ��-�ǻ�ͨ��,0xA3
	Err_NEGATIVE = 164,			// �ÿ�Ǯ��Ϊ��ֵ,0xA4
	Err_OUT_LIMIT = 165,			// �ÿ��������޶�,0xA5
	Err_NoInSystem = 166,			// ,0xA6
	Err_EpurseForbid = 167,			// ,0xA7
	Err_Invalid = 168,			// ,0xA8
	Err_CardOperFail = 169,			// ������Ƭ����,0xA9
	Err_ContinuousCard = 170,			// ��������ˢ��Ƭ,0xAA
	Err_StopCard = 171,			// ͣ�ÿ�,0xAB
	Err_MonthData = 172,			// ��Ʊ������,0xAC
	Err_MoneyData = 173,			// Ǯ��������,0xAD
	Err_GetData = 174,			// �ɼ����ݴ���,0xAE
	Err_Record = 175,			// ��¼����,0xAF
	Err_MonthMoney = 176,			// ������ƱǮ��������,0xB0
	Err_InMoney = 177,			// Ǯ������,0xB1
	Err_ExMoney = 178,			// Ǯ��������,0xB2
	Err_Card = 179,			// ���쳣,0xB3
	Err_GetMiKey = 180,			// ������Կ����,0xB4
	Err_Expired = 181,			// ����Ч��,0xB5
	Err_CardBlack = 182,			// ��������,0xB6
	Err_CardNo = 183,			// ��δ����,0xB7
	Err_CardStop = 184,			// ����ͣ��,0xB8
	Err_InMonth = 185,			// Ǯ����β���,0xB9
	Err_initpsam = 186,			// ,0xBA
	Err_IsLNT_MIF = 187,			// ,0xBB
	Err_NotWhite = 188,			// ,0xBC
	Err_UnExistCard = 189,			// ���಻����,0xBD
	Err_KeyIndex = 190,			// ,0xBE
	Err_NotInterFlow = 191,			// �ǻ�����ͨ��,0xBF
	Err_KeyError = 192,			// ��Կ����,0xC0
	Err_Money = 193,			// �ۿ��ȷ,0xC1
	Err_MonthData1 = 194,			// ��Ʊ������,0xC2
	Err_MonthData2 = 195,			// ��Ʊ������,0xC3
	Err_AppLocked = 196,			// Ӧ��������,0xC4
	Err_LockCardFail = 197,			// ��������,����ʧ��,0xC5
	Err_MAC2Des = 198,			// У��MAC2ʧ��,0xC6
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



