#ifndef _INCLUDED_TRANSACTION_M1_H
#define _INCLUDED_TRANSACTION_M1_H


enum emFlagListInfo
{
	AREA_CONTENT = 0x00,     //目录区
	AREA_ISSUE = 0x01,     //发行区
	AREA_TXNRECORD = 0x03,     //交易记录区
	AREA_PUBLICINFO = 0x06,     //公共信息区
	AREA_MONTHTOPUP = 0x07,     //单月票区
	AREA_MONTH = 0x08,     //双月票区
	AREA_EPURSE = 0x10,     //公共钱包区
	AREA_NOUSE = 0xFF      //未使用扇区
};

typedef struct
{
	TUINT8 ListSector;              //应用标识目录区    0
	TUINT8 PublishSector;           //发行区            1
	TUINT8 PublicMoneySector;       //公共钱包区        2
	TUINT8 HistoryRecordSector[3];  //钱包交易记录区A   3~5
	TUINT8 PublicMessageSector;     //公共信息区        6
	TUINT8 MonthTopupSector;        //月票充值区        7
	TUINT8 MonthMoneySector;        //月票钱包区        8
	TUINT8 RfuBytes[7];                //保留              9~15
}stM1_ListInfoBlock1;

typedef struct
{
	TUINT8 CityCode[2];         //城市代码          0~1
	TUINT8 TradeCode[2];        //行业代码          2~3
	TUINT8 PublishBicker[4];    //发行流水号        4~7
	TUINT8 CardCheckCode[4];    //卡认证码          8~11
	TUINT8 StartFlag;           //启用标识          12
	TUINT8 CardType;            //卡类              13
	TUINT8 KeyVer;              //密钥版本          14
	TUINT8 Verify;              //校验              15
}stM1_PublicSecBlock4;

typedef struct
{
	TUINT8 PublishDate[4];      //发行日期          0~3
	TUINT8 ValidDate[4];        //有效日期          4~7
	TUINT8 StartDate[4];        //启用有效期        8~11
	TUINT8 Deposit[2];          //卡押金            12~13
	TUINT8 Staffflag;           //保留              14
	TUINT8 Verify;              //校验              15
}stM1_PublicSecBlock5;

typedef struct
{
	TUINT8 PlusDate[5];         //充值时间(年~时)   0~4
	TUINT8 SrcMoney[4];         //原额/原点数       5~8
	TUINT8 PlusTimes[2];        //本次加款值        9~10
	TUINT8 Operator[4];         //操作员号          11~14
	TUINT8 Verify;              //校验              15
}stM1_PublicSecBlock6, st_MonthSecBlock28;

typedef struct {
	stM1_ListInfoBlock1 listInfoblock1;
	stM1_PublicSecBlock4 publicsecblock4;
	stM1_PublicSecBlock5 publicsecblock5;
	stM1_PublicSecBlock6 publicsecblock6;
}stM1_CardMapping;

typedef struct
{
	TUINT32 DealMoney;              //交易金额          0~3
	TUINT32 BeforeBalance;          //交易前余额        4~7
	TUINT32 AfterBalance;            //交易后余额        8~11
	TUINT8 AppSerialNo[10];         //CPU卡应用序列号   12~21
	TUINT8 OfflineNo[2];            //卡脱机交易号      22~23
	TUINT8 DealType;                //交易类型          24
	TUINT8  CardType;                //卡类              25
}stM1CARD_DEAL;

#ifndef _TRANSACTION_M1_N_FUNCTION_C

#ifdef __cplusplus
extern "C"
{
#endif


	extern int dealCardM1(stCardInfor *pCardInfor);


#ifdef __cplusplus
};
#endif

#endif

#endif



