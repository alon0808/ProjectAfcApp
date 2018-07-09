#ifndef _INCLUDED_TRANSACTION_M1_H
#define _INCLUDED_TRANSACTION_M1_H


enum emFlagListInfo
{
	AREA_CONTENT = 0x00,     //Ŀ¼��
	AREA_ISSUE = 0x01,     //������
	AREA_TXNRECORD = 0x03,     //���׼�¼��
	AREA_PUBLICINFO = 0x06,     //������Ϣ��
	AREA_MONTHTOPUP = 0x07,     //����Ʊ��
	AREA_MONTH = 0x08,     //˫��Ʊ��
	AREA_EPURSE = 0x10,     //����Ǯ����
	AREA_NOUSE = 0xFF      //δʹ������
};

typedef struct
{
	TUINT8 ListSector;              //Ӧ�ñ�ʶĿ¼��    0
	TUINT8 PublishSector;           //������            1
	TUINT8 PublicMoneySector;       //����Ǯ����        2
	TUINT8 HistoryRecordSector[3];  //Ǯ�����׼�¼��A   3~5
	TUINT8 PublicMessageSector;     //������Ϣ��        6
	TUINT8 MonthTopupSector;        //��Ʊ��ֵ��        7
	TUINT8 MonthMoneySector;        //��ƱǮ����        8
	TUINT8 RfuBytes[7];                //����              9~15
}stM1_ListInfoBlock1;

typedef struct
{
	TUINT8 CityCode[2];         //���д���          0~1
	TUINT8 TradeCode[2];        //��ҵ����          2~3
	TUINT8 PublishBicker[4];    //������ˮ��        4~7
	TUINT8 CardCheckCode[4];    //����֤��          8~11
	TUINT8 StartFlag;           //���ñ�ʶ          12
	TUINT8 CardType;            //����              13
	TUINT8 KeyVer;              //��Կ�汾          14
	TUINT8 Verify;              //У��              15
}stM1_PublicSecBlock4;

typedef struct
{
	TUINT8 PublishDate[4];      //��������          0~3
	TUINT8 ValidDate[4];        //��Ч����          4~7
	TUINT8 StartDate[4];        //������Ч��        8~11
	TUINT8 Deposit[2];          //��Ѻ��            12~13
	TUINT8 Staffflag;           //����              14
	TUINT8 Verify;              //У��              15
}stM1_PublicSecBlock5;

typedef struct
{
	TUINT8 PlusDate[5];         //��ֵʱ��(��~ʱ)   0~4
	TUINT8 SrcMoney[4];         //ԭ��/ԭ����       5~8
	TUINT8 PlusTimes[2];        //���μӿ�ֵ        9~10
	TUINT8 Operator[4];         //����Ա��          11~14
	TUINT8 Verify;              //У��              15
}stM1_PublicSecBlock6, st_MonthSecBlock28;

typedef struct {
	stM1_ListInfoBlock1 listInfoblock1;
	stM1_PublicSecBlock4 publicsecblock4;
	stM1_PublicSecBlock5 publicsecblock5;
	stM1_PublicSecBlock6 publicsecblock6;
}stM1_CardMapping;

typedef struct
{
	TUINT32 DealMoney;              //���׽��          0~3
	TUINT32 BeforeBalance;          //����ǰ���        4~7
	TUINT32 AfterBalance;            //���׺����        8~11
	TUINT8 AppSerialNo[10];         //CPU��Ӧ�����к�   12~21
	TUINT8 OfflineNo[2];            //���ѻ����׺�      22~23
	TUINT8 DealType;                //��������          24
	TUINT8  CardType;                //����              25
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



