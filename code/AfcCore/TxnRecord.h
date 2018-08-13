/*
 * TxnRecord.h
 *
 *  Created on: May 13, 2015
 *      Author: jasonz
 */

#ifndef TXNRECORD_H_
#define TXNRECORD_H_

#include "szct.h"

//typedef	unsigned char U8,INT8U;
	

union charToshort
{
	unsigned short intbuffer;
	unsigned char charbuffer[2];
};

union charToint
{
	unsigned int intbuffer;
	unsigned char charbuffer[4];
};
union charTolong
{
	unsigned long longbuffer;
	unsigned char charbuffer[4];
};

#undef RECORD_LEN
#define RECORD_LEN 256		//���ͨ �Լ��ĸ�ʽ
#define DATALEN 256//240
#define qPbocRECORD_LEN 256
//#endif

#undef MAX_REC_NUM
#define MAX_REC_NUM	(LEN_EEPROM / RECORD_LEN)   //eeprom�д�ŵ�����¼��Ŀ


//һ��ͨ�Ľ�������



#define PBOC_FAIL           0x41
#define PBOC_MONEY          0x42

//��������
#define ID_DEALD            0x06        //����
#define ID_DEALF            0x09        //��������
#define ID_BLACK            0x50        //������������¼
#define ID_MANAGE           0x60        //������¼
#define ID_ODA				0x67
#define ID_Electronic		0xE0
#define ID_ONLINE			0x66



#define ID_REC_CPUAFLFail   0       //�����ֽ�����ʧ��
#define ID_REC_CPUFDDAFail  0x5E    //�����ֽ�����ʧ��
#define ID_REC_TOLL         2       //Ǯ������
#define ID_REC_MON          3       //��Ʊ����

#define ID_REC_DEVNOSET     7       //���ƺ�����
#define ID_REC_PAKAGE       8       //���ݰ��ָ�
#define ID_REC_DATE         9       //���ڱ��

#define ID_REC_BLKLIST      13      //��������ˢ��
#define ID_REC_AUTHOR       14      //�豸��Ȩˢ��
#define ID_REC_DTRAN2       15      //���ݲɼ�
#define ID_REC_AIRCON       17      //�յ�������
#define ID_REC_TOLLSTT      21      //�շ�������
#define ID_REC_DEVNOSET1    22      //���ƺź�����
#define ID_REC_NO           23      //��ͷ�ļ�
#define ID_REC_FUEL1        24      //���ͼ�¼1
#define ID_REC_FUEL2        25      //���ͼ�¼2
#define ID_REC_LINENO       26
#define ID_REC_PURSE        80      //Ǯ����ֵ
#define ID_REC_MONTH        81      //��Ʊ��ֵ
#define ID_REC_CARDLOCK     99      //��������






enum
{
    TXN_TYPE_DEBIT=0x06,
    TXN_SUBTYPE_FREE=0x01,
    TXN_SUBTYPE_EPURSE=0x02,
    TXN_SUBTYPE_MONTH=0x03,
    TXN_SUBTYPE_SMALL=0x04,
    TXN_TYPE_CONTROL=0xC1
};

// enum
// {
//     PHY_TYPE_M1=0x01,
//     PHY_TYPE_CPU=0x02,
//     PHY_TYPE_PBOC=0x03,
//     PHY_TYPE_JTB=0x04
// };

//�������Ͷ���
#define MODE_LNT_M1				0x1F
#define MODE_LNT_CPU			0x02
#define MODE_ZJB_M1			0x09//ס����M1
#define MODE_ZJB_CPU        0x08	

typedef struct //���Ѽ�¼
{
    U8 RecordType;              //��¼���ͣ��ۿ�����ֵ�����ѣ�hex   1
    U8 AssRecordType;           //����������                        2 //0x01 ������� 0x02 ��ֵ���� 0x03 ��Ʊ���� 0x04С������
    U8 UserCardLen;             //                                  3
    U8 CityCode[2];             //bcd
    U8 TradeCode[2];            //bcd
    U8 UserCardCode[15];        //bcd                               22
    U8 PhyCardCode[8];          //hex                               30
    U8 CardTypeMain;            // 31
    U8 CardTypeSub;             // 32
    U8 SamCode[8];              //��ǰSAM���� BCD 6                 40
    U8 SamSn[4];                //SAM����ˮ�� HEX 4                 44
    U8 TxnDate[3];              //��ǰ��������yymmdd                47
    U8 TxnTime[3];              //ʱ��                              50
    U8 BusPrice[3];             //Ʊ�� HEX 3                        53
    U8 AddPrice[3];            //�������и��ӽ����ܺ� HEX 3      56
    U8 Amount[5];               //BCD ��ǰ���׽��/����/�ۿ�Ѻ��    61
    U8 Balance[6];              // BCD ���׺����/����              67
    U8 LocalTSn[4];             //������ˮ�� bcd 4                  71
    U8 LocalTsnSub[4];          //bcd                               75
    U8 CardSeq[2];              //hex                               77
    U8 Credits[3];              //                                  80
    U8 OperID[4];               //Hex 4                             84
    U8 PhyType[2];              //�������� hex 2                  86
    U8 TerminalID[8];           //�ն˱�� asc 8                    94
    U8 DoorNum;                 //asc                               95
    U8 LineNum[4];              //��·�������·�� BCD 3            99
    U8 GetOnOff;                //bcd 01 on, 02 off                 100
    U8 LastDealSAMNo[6];        //�ϴν���SAM���� BCD 6             106
    U8 LastDealTime[6];         //�ϴν�������ʱ�� BCD 7            112
    U8 CardExpireDate[3];       //bcd                               115
    U8 CurPSAMTermNum[6];       //bcd 6                             121
    U8 CardTxnMode;             //01-PBOC1.0, 02-PBOC2.0, 03-M1, 04-PBOC3.0       122
    U8 CardCalcType[2];         //BCD                               124
    U8 CardInnerVer;            //BCD                               125
    U8 TAC[8];                  //hex                               133
    U8 FinanceData[117];        //bcd                               250
    U8 TransferFlag;            //bcd 00 normal, 01 transfer        251
    U8 RFU[5];                  //hex                               256
}stICCard_256Record;

typedef struct
{
    U8 ProjFlag;                //1��Ŀ���                      0          HEX
    U8 Flag[2];                 //0��¼����                      1~2        HEX
    U8 Tag9F26[8];              //2Ӧ������AC                    3~10       BCD
    U8 Tag9F27;                 //3������Ϣ����CID               11         HEX
    U8 Tag9F10_Len;             //4������Ӧ��������Ч����        12         BCD
    U8 Tag9F10[32];             //5������Ӧ������                13~44      BCD
    U8 Tag9F37[4];              //6����Ԥ֪��                    45~48      BCD
    U8 Tag9F36[2];              //7Ӧ�ý��׼�����                49~50      HEX ��λ��ǰ
    U8 Tag95[5];                //8�ն���֤���TVR               51~55      BCD
    U8 Tag9A[6];                //9��������+����ʱ��             56~61      BCD (YYMMDDhhmmss)
    U8 Tag9C;                   //11��������                     62         BCD
    U8 Tag5F34;                 //12Ӧ�����˺����к�             63         BCD ȱʡΪ99
    U8 Tag9F02[6];              //13��Ȩ���/���׽��            64~69      BCD ��λ��ǰ
    U8 Tag5F2A[2];              //14���׻��Ҵ���                 70~71      BCD
    U8 Tag82[2];                //15Ӧ�ý�������AIP              72~73      BCD
    U8 Tag9F1A[2];              //16�ն˹��Ҵ���                 74~75      BCD
    U8 Tag9F03[6];              //17�������                     76~81      BCD
    U8 Tag9F33[3];              //18�ն�����                     82~84      BCD
    U8 Tag9F35;                 //19�ն�����                     85         BCD
    U8 Tag9F1E[8];              //20�ӿ��豸IFD���к�/�������   86~93      ASCII
    U8 Tag5F24[3];              //21Ӧ��ʧЧ����                 94~96      BCD
    U8 Tag5A_Len;               //22Ӧ��PAN/���˺���Ч����       97         HEX
    U8 Tag5A[19];               //23Ӧ��PAN/���˺�               98~116     BCD
    U8 Tag9F41[4];              //24�ն˽������м�����           117~120    BCD
    U8 Tag9F5D[6];              //25���׺����                   121~126    BCD
    U8 Tag84_Len;               //26ר���ļ�������Ч����         127        HEX
    U8 Tag84[16];               //27ר���ļ�����                 128~143    HEX
    U8 Tag9F09[2];              //28Ӧ�ð汾��                   144~145    HEX
    U8 Tag9F34[3];              //29�ֿ�����֤���               146~148    HEX
    U8 Tag9F63_Len;             //30����Ʒ��ʶ��Ч����           149        HEX
    U8 Tag9F63[16];             //31����Ʒ��ʶ                   150~165    HEX
    U8 Tag9F74_Len;             //32�����ֽ𷢿�����Ȩ����Ч���� 166        HEX
    U8 Tag9F74[6];              //33�����ֽ𷢿�����Ȩ��         167~172    HEX
    U8 Tag8A[2];                //34��Ȩ��Ӧ��                   173~174    ASCII
    U8 PosEntryMode[2];         //35��������뷽ʽ��             175~176    BCD     ǰ��λ:05�Ӵ�ʽIC������,07�ǽӴ�ʽIC������,����λ:X0:����ʱ����������дX=1,������ʱ��дX=2.
    U8 TerminalReadAb;          //36�ն˶�ȡ����                 177        BCD     05:�ɽӴ�ʽ��ȡIC��,ͬʱ��������뷽ʽ��ǰ��λ����Ϊ05;06:�ɽӴ�ʽ�ͷǽӴ�ʽ��ȡIC��,ͬʱ��������뷽ʽ��ǰ��λ����Ϊ07
    U8 IcConditionCode;         //37IC����������                 178        BCD     00δʹ��,01��һ�ʽ��ײ���IC�����׻���һ�ʳɹ���IC������,02һ�ʽ�������IC�����׵�ʧ��
    U8 TerminalNo[6];           //38���ν����豸���             179~184    BCD
    U8 SerialNo[8];             //39�߼�����                     185~192    BCD     ���д���+��ҵ����+�߼��������к�
    U8 PhysicCardNo[8];         //40������                     193~200    BCD
    U8 Price[4];                //41Ʊ����Ϣ                     201~204    HEX     ��λ��ǰ
    U8 DealType[2];             //42��������                     205~206    BCD     02��ֵ,06����,09��������
    U8 AppendDealType[2];       //43���ӽ�������                 207~208    BCD     ��֧����ʽ����(��λ)�ͽ��׷������(��λ)���
    U8 DrivecardNo[5];          //44˾������                     209~213    BCD
    U8 LineNo[3];               //45��·��                       214~216    ASC
    U8 CardType[2];             //46����                         217~218    BCD
    U8 BusNo[8];                //47�������                     219~226    ASCII
    U8 PhysicalType;            //48��������                   227        BCD     ʡͨCPU��(02)/ʡͨM1(01)/ʡͨ��M1(1F)/����IC��(03)
    U8 Reserved[12];            //49����                         228~239    BCD
}stPBOC_Record;


	//տ��

typedef struct
{
    U8 NowDealFixNo[6];          //2���ν����豸���             0~5        BCD
    U8 offLineDealNo[4];         //3�ѻ����׺�                   6~9        HEX   ��λ��ǰ
    U8 NowDealTime[7];           //4���ν��׵�ʱ��               10~16      BCD
    U8 TicketCardNo[8];          //5���д���+��ҵ����+�߼�����   17~24      BCD
    U8 PhysicCardNo[8];          //6������                     25~32      HEX
    U8 DealMoney[4];             //7���׽��                     33~36      HEX ��ǰ
    U8 Price[4];                 //8Ʊ����Ϣ                     37~40      HEX ��ǰ
    U8 RemainningMoney[4];       //9�������                     41~44      HEX ��ǰ
    U8 DealType[2];              //10���ν�������                45~46      HEX ���ѣ�0x06 ��ֵ��0x02 ��������0x50
    U8 AppendDealType[2];        //11���ӽ�������                47~48      HEX ��������0x02 ��Ʊ0x03
    U8 TicketCZCount[2];         //12Ʊ����ֵ���׼���            49~50      HEX
    U8 TicketXFCount[2];         //13Ʊ�����ѽ��׼���            51~52      HEX
    U8 LastDealFixNo[6];         //14�ϴν���SAM����             53~58      BCD
    U8 LastDealTime[7];          //15�ϴν��׵�ʱ�������        59~65      BCD
    U8 TAC[4];                   //16TAC��                       66~69      HEX
    U8 DrivecardNo[5];           //17˾������                    70~74      BCD
    U8 LineNO[3];                //18��·��                      75~77      ASC
    U8 CardType[2];              //19����                        78~79      HEX
    U8 BusID[8];                 //20�������                    80~87      ASC
    U8 PhysicCardType;           //21��������                  88         BCD
    U8 Process;                  //22���̱�־                    89         HEX
    U8 SignFlag;                 //23ǩ�����,˾��D Ա��S        90         ASC
    U8 CommFlag[2];               //24������ͨ��ʶ                91~92      BCD
	U8 HCFlag;					//���˱�־93			�����Ż�����:01, �ǻ����Ż�����:00
    U8 Reserved[2];              //25Ԥ����                      94~95      BCD
}stICCard_Record;


union URecord
{
    U8 Bytes[DATALEN];
    U8 Flag[3];
    stPBOC_Record PBOCRecord;
    stICCard_Record ICCardRecord;
};





//տ���Żݲ���
typedef struct
{
	unsigned char CardFlag ;   				//�������жϣ�1:M1��    2:CPU��
	unsigned char CardTypeFlag ; 				//0:��ʾLNT    ����ı�ʾZJB
	
	unsigned char PrivilegeNumber ;			//�Żݴ���  �����ʱ����M1������ʵ���Ժ�cpu��һ���ýṹ�� ��ʱûʱ���
	unsigned char HC_Flag ;					//���׼�¼�л��˱�־	//�����Żݱ�־ 1 ��ʶ����
	unsigned char Line_Youhui_Flag ;			//Ĭ��Ϊ���Ż���·  �ӷ���·�Ż�ȡ  0����ʵ�ֻ�����·�����������Ի�����·
	unsigned int Youhui_Price_Ratio;		//�Ż���·Ʊ���ۿ�
	
}st_YOUHU_Infor;




// bool ReadTxn(U32 offset, void* data, U32 len);
// bool WriteTxn(U32 offset, const void* data, U32 len);
// bool SaveTxn(const void* txnData);
// 
// int GetUnCollectCount();
// int GetUnCollectBytes();
// int TxnCollect(U8* buf);
// bool TxnHeadForward(int bytes);
// bool TxnTailForward(int bytes);
// bool RecordStoreFull();
// void BuildDebitTxn(stICCard_256Record* rec, const RecordNeedInfo_t* recNeed, bool isPBOC, U8 txnType, U8 subTxnType);



#endif /* TXNRECORD_H_ */
