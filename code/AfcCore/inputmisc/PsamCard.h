/// @file PsamCard.h
/// @brief 
/// @author 
/// @version V1.0.0
/// @date 2018-06-15

#pragma once 

#include "LtyCommon.h"

#include "LtyTimer.h"
#include "LtyMessage.h"

#include "ev.h"

#include "InitSystem.h"
//#include "IcCard.h"
//#include "LightBeep.h"
#include <pthread.h>
#include <functional>
#include <stdexcept>
#include <unistd.h>
#include "SlzrTypeDef.h"
#include "SlzrError.h"

//#include "SLZRMsgCfg.h"


//****************票价折率文件存贮*******************************//	 
#define PATH_ST_PRICE               "/mnt/sdisk/LineCfg/price_table"
#define PRICETABLEOFUP              "/mnt/posdata/price/sectionup.sys"
#define PRICETABLEOFDOWN            "/mnt/posdata/price/section.sys"
#define CARDTYPETABLE                "/mnt/posdata/cardtype/cardtype.sys"
#define PRICE_TABLE_SEC             127
#define PRICE_TABLE_SIZEPERSEC      256
#define CARD_TYPE_TABLE_SEC         1
#define CARD_TYPE_TABLE_SIZEPERSEC  256
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//


//****************APDU*******************************//
#define APDU_SEND_DATA_MAX_LEN   300

typedef struct ApduDataReq_r{
    SLZR_U8 szCommand[4];   // CLA  INS  P1  P2 
    SLZR_U8 u8Lc;
    SLZR_U8 szDataIn[APDU_SEND_DATA_MAX_LEN];
    SLZR_U8 u8Le;
    SLZR_U8 u8EnableCancel;
}ApduDataReq_t;

typedef struct ApduDataResp_r{
    SLZR_U8  u8ReadCardDataOk;
    SLZR_U16 u16LenOut; 
    SLZR_U8  szDataOut[APDU_SEND_DATA_MAX_LEN];
    SLZR_U8  u8SWA;
    SLZR_U8  u8SWB;
}ApduDataResp_t;
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

//****************几个全局变量*******************************//	 


extern CardLanSector LanSec;		//用户扇区
extern SLZR_U8 XFBuf1[16],XFBuf2[16];
extern card_buf  test; 
extern SLZR_U8 m_szKeyBufA[6];
extern SLZR_U8 m_szKeyBufB[6];
extern LongUnon m_HostValue,DecValue,m_DevNum;
extern LongUnon m_BasePrice;
extern LongUnon m_tranPrice,m_qTranPrice;
extern SLZR_U32 m_M1PsamTmkIndex;
extern SLZR_U32 m_CPUPsamTmkIndex;
extern SLZR_U8	m_szM1PsamList[6];
extern SLZR_U8	m_szCpuPsamList[6];

extern SLZR_U8	m_CardTypeIC;
extern LongUnon m_totalPrice,m_totalnum;  //总笔数  总金额
extern  SLZR_U32 m_CpuPsamNum;
extern  SLZR_U32 m_M1PsamNum;

extern SLZR_U8 m_TicketDuration; //刷卡时间间隔
extern SLZR_U32 m_CityId;   //城市代码 
extern SLZR_U32 m_u32Direction;
extern unsigned short m_SectionNum; //总站数


extern SLZR_VOID CPsamCard(void);
extern SLZR_RESULT CPsamCard_Init(void);
extern SLZR_RESULT CPsamCard_QRCodeInit(SLZR_U8 *RcvBuff, SLZR_U32 *RcvBuff_len);
extern SLZR_U32 DetectPsamSet(SLZR_U32 u32Num, SLZR_U8 *pBuf, SLZR_U32 *pLen, SLZR_U8 *pStatus);
extern SLZR_U32 PsamApduSet(SLZR_U32 u32Num, SLZR_U8 *pApdu, SLZR_U32 u32Len, SLZR_U8 *pBuf, SLZR_U32 *pRdLen, SLZR_U8 *pStatus);
extern SLZR_U32 SendPsamData(SLZR_U8 *pDataIn , SLZR_U16 u16InLen , SLZR_U8 *pDataOut , SLZR_U16 *pOutLen);
extern SLZR_U32 PsamSelectApp(SLZR_U32 mode);
extern SLZR_U32 CpuPsamInit(void);
extern SLZR_VOID DealPsamCardData(SLZR_U8 *pTempSendData, SLZR_U16 u16TempDataLen, ApduDataResp_t *pApdu_r);
extern SLZR_VOID PsamCommand(ApduDataReq_t *ApduSend, ApduDataResp_t *apdu_r);
extern SLZR_U32 M1PsamInit(void);
extern SLZR_U32 PsamReadMerchant(SLZR_U8 *pOutBuf, SLZR_S32 mode);
extern SLZR_U32 PsamCardInit();
extern SLZR_U32 SearchCard(SLZR_U8 *pData, SLZR_U32 *pLen);
extern SLZR_U32 QRCodeInit(SLZR_U8 *RcvBuff, SLZR_U32 *RcvBuff_len);
