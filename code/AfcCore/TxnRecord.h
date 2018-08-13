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
#define RECORD_LEN 256		//羊城通 自己的格式
#define DATALEN 256//240
#define qPbocRECORD_LEN 256
//#endif

#undef MAX_REC_NUM
#define MAX_REC_NUM	(LEN_EEPROM / RECORD_LEN)   //eeprom中存放的最大记录数目


//一拍通的交易类型



#define PBOC_FAIL           0x41
#define PBOC_MONEY          0x42

//交易类型
#define ID_DEALD            0x06        //消费
#define ID_DEALF            0x09        //复合消费
#define ID_BLACK            0x50        //黑名单锁卡记录
#define ID_MANAGE           0x60        //管理卡记录
#define ID_ODA				0x67
#define ID_Electronic		0xE0
#define ID_ONLINE			0x66



#define ID_REC_CPUAFLFail   0       //电子现金消费失败
#define ID_REC_CPUFDDAFail  0x5E    //电子现金消费失败
#define ID_REC_TOLL         2       //钱包消费
#define ID_REC_MON          3       //月票消费

#define ID_REC_DEVNOSET     7       //车牌号设置
#define ID_REC_PAKAGE       8       //数据包分隔
#define ID_REC_DATE         9       //日期变更

#define ID_REC_BLKLIST      13      //黑名单卡刷卡
#define ID_REC_AUTHOR       14      //设备授权刷卡
#define ID_REC_DTRAN2       15      //数据采集
#define ID_REC_AIRCON       17      //空调启动卡
#define ID_REC_TOLLSTT      21      //收费启动卡
#define ID_REC_DEVNOSET1    22      //车牌号号设置
#define ID_REC_NO           23      //包头文件
#define ID_REC_FUEL1        24      //加油记录1
#define ID_REC_FUEL2        25      //加油记录2
#define ID_REC_LINENO       26
#define ID_REC_PURSE        80      //钱包充值
#define ID_REC_MONTH        81      //月票充值
#define ID_REC_CARDLOCK     99      //琐卡交易






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

//物理卡类型定义
#define MODE_LNT_M1				0x1F
#define MODE_LNT_CPU			0x02
#define MODE_ZJB_M1			0x09//住建部M1
#define MODE_ZJB_CPU        0x08	

typedef struct //消费记录
{
    U8 RecordType;              //记录类型（售卡，充值，消费）hex   1
    U8 AssRecordType;           //副交易类型                        2 //0x01 免费消费 0x02 储值消费 0x03 月票消费 0x04小额消费
    U8 UserCardLen;             //                                  3
    U8 CityCode[2];             //bcd
    U8 TradeCode[2];            //bcd
    U8 UserCardCode[15];        //bcd                               22
    U8 PhyCardCode[8];          //hex                               30
    U8 CardTypeMain;            // 31
    U8 CardTypeSub;             // 32
    U8 SamCode[8];              //当前SAM卡号 BCD 6                 40
    U8 SamSn[4];                //SAM卡流水号 HEX 4                 44
    U8 TxnDate[3];              //当前交易日期yymmdd                47
    U8 TxnTime[3];              //时间                              50
    U8 BusPrice[3];             //票价 HEX 3                        53
    U8 AddPrice[3];            //其他所有附加金额的总和 HEX 3      56
    U8 Amount[5];               //BCD 当前交易金额/次数/售卡押金    61
    U8 Balance[6];              // BCD 交易后余额/次数              67
    U8 LocalTSn[4];             //本地流水号 bcd 4                  71
    U8 LocalTsnSub[4];          //bcd                               75
    U8 CardSeq[2];              //hex                               77
    U8 Credits[3];              //                                  80
    U8 OperID[4];               //Hex 4                             84
    U8 PhyType[2];              //物理卡类型 hex 2                  86
    U8 TerminalID[8];           //终端编号 asc 8                    94
    U8 DoorNum;                 //asc                               95
    U8 LineNum[4];              //线路号与分线路号 BCD 3            99
    U8 GetOnOff;                //bcd 01 on, 02 off                 100
    U8 LastDealSAMNo[6];        //上次交易SAM卡号 BCD 6             106
    U8 LastDealTime[6];         //上次交易日期时间 BCD 7            112
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
    U8 ProjFlag;                //1项目序号                      0          HEX
    U8 Flag[2];                 //0记录类型                      1~2        HEX
    U8 Tag9F26[8];              //2应用密文AC                    3~10       BCD
    U8 Tag9F27;                 //3密文信息数据CID               11         HEX
    U8 Tag9F10_Len;             //4发卡行应用数据有效长度        12         BCD
    U8 Tag9F10[32];             //5发卡行应用数据                13~44      BCD
    U8 Tag9F37[4];              //6不可预知数                    45~48      BCD
    U8 Tag9F36[2];              //7应用交易计数器                49~50      HEX 高位在前
    U8 Tag95[5];                //8终端验证结果TVR               51~55      BCD
    U8 Tag9A[6];                //9交易日期+交易时间             56~61      BCD (YYMMDDhhmmss)
    U8 Tag9C;                   //11交易类型                     62         BCD
    U8 Tag5F34;                 //12应用主账号序列号             63         BCD 缺省为99
    U8 Tag9F02[6];              //13授权金额/交易金额            64~69      BCD 高位在前
    U8 Tag5F2A[2];              //14交易货币代码                 70~71      BCD
    U8 Tag82[2];                //15应用交互特征AIP              72~73      BCD
    U8 Tag9F1A[2];              //16终端国家代码                 74~75      BCD
    U8 Tag9F03[6];              //17其他金额                     76~81      BCD
    U8 Tag9F33[3];              //18终端性能                     82~84      BCD
    U8 Tag9F35;                 //19终端类型                     85         BCD
    U8 Tag9F1E[8];              //20接口设备IFD序列号/出厂编号   86~93      ASCII
    U8 Tag5F24[3];              //21应用失效日期                 94~96      BCD
    U8 Tag5A_Len;               //22应用PAN/卡账号有效长度       97         HEX
    U8 Tag5A[19];               //23应用PAN/卡账号               98~116     BCD
    U8 Tag9F41[4];              //24终端交易序列计数器           117~120    BCD
    U8 Tag9F5D[6];              //25交易后余额                   121~126    BCD
    U8 Tag84_Len;               //26专用文件名称有效长度         127        HEX
    U8 Tag84[16];               //27专用文件名称                 128~143    HEX
    U8 Tag9F09[2];              //28应用版本号                   144~145    HEX
    U8 Tag9F34[3];              //29持卡人验证结果               146~148    HEX
    U8 Tag9F63_Len;             //30卡产品标识有效长度           149        HEX
    U8 Tag9F63[16];             //31卡产品标识                   150~165    HEX
    U8 Tag9F74_Len;             //32电子现金发卡行授权码有效长度 166        HEX
    U8 Tag9F74[6];              //33电子现金发卡行授权码         167~172    HEX
    U8 Tag8A[2];                //34授权响应码                   173~174    ASCII
    U8 PosEntryMode[2];         //35服务点输入方式码             175~176    BCD     前两位:05接触式IC卡交易,07非接触式IC卡交易,后两位:X0:交易时输入密码填写X=1,无密码时填写X=2.
    U8 TerminalReadAb;          //36终端读取能力                 177        BCD     05:可接触式读取IC卡,同时服务点输入方式码前两位必须为05;06:可接触式和非接触式读取IC卡,同时服务点输入方式码前两位必须为07
    U8 IcConditionCode;         //37IC卡条件代码                 178        BCD     00未使用,01上一笔交易不是IC卡交易或是一笔成功的IC卡交易,02一笔交易虽是IC卡交易但失败
    U8 TerminalNo[6];           //38本次交易设备编号             179~184    BCD
    U8 SerialNo[8];             //39逻辑卡号                     185~192    BCD     城市代码+行业代码+逻辑卡号序列号
    U8 PhysicCardNo[8];         //40物理卡号                     193~200    BCD
    U8 Price[4];                //41票价信息                     201~204    HEX     高位在前
    U8 DealType[2];             //42交易类型                     205~206    BCD     02充值,06消费,09复合消费
    U8 AppendDealType[2];       //43附加交易类型                 207~208    BCD     由支付方式代码(高位)和交易分类代码(低位)组成
    U8 DrivecardNo[5];          //44司机卡号                     209~213    BCD
    U8 LineNo[3];               //45线路号                       214~216    ASC
    U8 CardType[2];             //46卡类                         217~218    BCD
    U8 BusNo[8];                //47车辆编号                     219~226    ASCII
    U8 PhysicalType;            //48物理卡类型                   227        BCD     省通CPU卡(02)/省通M1(01)/省通新M1(1F)/金融IC卡(03)
    U8 Reserved[12];            //49保留                         228~239    BCD
}stPBOC_Record;


	//湛江

typedef struct
{
    U8 NowDealFixNo[6];          //2本次交易设备编号             0~5        BCD
    U8 offLineDealNo[4];         //3脱机交易号                   6~9        HEX   高位在前
    U8 NowDealTime[7];           //4本次交易的时间               10~16      BCD
    U8 TicketCardNo[8];          //5城市代码+行业代码+逻辑卡号   17~24      BCD
    U8 PhysicCardNo[8];          //6物理卡号                     25~32      HEX
    U8 DealMoney[4];             //7交易金额                     33~36      HEX 低前
    U8 Price[4];                 //8票价信息                     37~40      HEX 低前
    U8 RemainningMoney[4];       //9本次余额                     41~44      HEX 低前
    U8 DealType[2];              //10本次交易类型                45~46      HEX 消费：0x06 充值：0x02 黑名单：0x50
    U8 AppendDealType[2];        //11附加交易类型                47~48      HEX 本地消费0x02 月票0x03
    U8 TicketCZCount[2];         //12票卡充值交易计数            49~50      HEX
    U8 TicketXFCount[2];         //13票卡消费交易计数            51~52      HEX
    U8 LastDealFixNo[6];         //14上次交易SAM卡号             53~58      BCD
    U8 LastDealTime[7];          //15上次交易的时间和日期        59~65      BCD
    U8 TAC[4];                   //16TAC码                       66~69      HEX
    U8 DrivecardNo[5];           //17司机卡号                    70~74      BCD
    U8 LineNO[3];                //18线路号                      75~77      ASC
    U8 CardType[2];              //19卡类                        78~79      HEX
    U8 BusID[8];                 //20车辆编号                    80~87      ASC
    U8 PhysicCardType;           //21物理卡类型                  88         BCD
    U8 Process;                  //22过程标志                    89         HEX
    U8 SignFlag;                 //23签到标记,司机D 员工S        90         ASC
    U8 CommFlag[2];               //24互联互通标识                91~92      BCD
	U8 HCFlag;					//换乘标志93			换乘优惠消费:01, 非换乘优惠消费:00
    U8 Reserved[2];              //25预留用                      94~95      BCD
}stICCard_Record;


union URecord
{
    U8 Bytes[DATALEN];
    U8 Flag[3];
    stPBOC_Record PBOCRecord;
    stICCard_Record ICCardRecord;
};





//湛江优惠参数
typedef struct
{
	unsigned char CardFlag ;   				//卡类型判断，1:M1卡    2:CPU卡
	unsigned char CardTypeFlag ; 				//0:表示LNT    其余的表示ZJB
	
	unsigned char PrivilegeNumber ;			//优惠次数  这个暂时用于M1卡，其实可以和cpu卡一样用结构体 暂时没时间改
	unsigned char HC_Flag ;					//交易记录中换乘标志	//换乘优惠标志 1 标识换乘
	unsigned char Line_Youhui_Flag ;			//默认为非优惠线路  从分线路号获取  0可以实现换乘线路，其他不可以换乘线路
	unsigned int Youhui_Price_Ratio;		//优惠线路票价折扣
	
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
