/// @file IcCardCtrlApi.h
/// @brief 
/// @author 
/// @version V1.0.0
/// @date 2018-06-18

#include "LtyCommon.h"
#include "LtyTimer.h"
#include "SlzrTypeDef.h"
#include "Uart.h"


typedef enum {
	prr_Start = 0,
	prr_lenStart = 3,
	prr_Cmd = prr_Start + prr_lenStart,
	prr_Len,
	prr_Data = prr_Len + 2,
	prr_lenHead = 3 + 1 + 2,
	prr_lenTail = 1 + 1 + 3,
	prr_lenHeadTail = prr_lenHead + prr_lenTail,
	prr_lenEnd = 3,
}emPosReaderRes;


extern SLZR_U32 m_u32UartFd;
extern SLZR_U32 m_u32UartQRCodeFd;

extern SLZR_VOID CIcCardCtrlApi(void);
extern SLZR_U32 Card_Init(void);
extern SLZR_U32 Card_QRCodeInit(void);
extern SLZR_U8 VerifyCount(SLZR_U8 *pPacket, SLZR_U32 u32Len);
extern SLZR_U32 RunCmd(SLZR_U32 u32CmdId, SLZR_U8 *pSndData, SLZR_U32 u32SndDataLen, SLZR_U8 *pRcvData, SLZR_U32 *pRcvLen, SLZR_U8 *pStatus);
extern SLZR_U32 CmdPsamApduSet(SLZR_U32 u32Num, SLZR_U8 *pApdu, SLZR_U32 u32Len, SLZR_U8 *pBuf, SLZR_U32 *pRdLen, SLZR_U8 *pStatus);
extern SLZR_U32 CmdAuthenticationSet(SLZR_U32 u32KeyMode, SLZR_U32 u32Block, SLZR_U8 *pStatus);
extern SLZR_U32 CmdHaltSet(SLZR_U8 *pStatus);
extern SLZR_U32 CmdReadSet(SLZR_U32 block, SLZR_U8 *buf, SLZR_U32 *len, SLZR_U8 *status);
extern SLZR_U32 CmdWriteSet(SLZR_U32 block, SLZR_U8 *buf, SLZR_U32 len, SLZR_U8 *status);
extern SLZR_U32 CmdLoadkeySet(SLZR_U8 *buf, SLZR_U32 len, SLZR_U8 *status);
extern SLZR_U32 CmdRatsSet(SLZR_U8 *buf, SLZR_U32 *len, SLZR_U8 *status);
extern SLZR_U32 CmdApduSet(SLZR_U8 *inbuf, SLZR_U32 inlen, SLZR_U8 *outbuf, SLZR_U32 *outlen, SLZR_U8 *status, SLZR_U8 *sw);
extern SLZR_U32 CmdFindCardSet(SLZR_U8 *buf, SLZR_U32 *len, SLZR_U8 *status);
extern SLZR_U32 CmdM1IncrementSet(SLZR_U32 block, SLZR_U8 *buf, SLZR_U32 len, SLZR_U8 *status);
extern SLZR_U32 CmdM1DecrementSet(SLZR_U32 block, SLZR_U8 *buf, SLZR_U32 len, SLZR_U8 *status);
extern SLZR_U32 CmdM1RestoreSet(SLZR_U32 block, SLZR_U8 *status);
extern SLZR_U32 CmdM1RestoreTransferSet(SLZR_U8 Toblock, SLZR_U8 Fromblock, SLZR_U8 *status);
extern SLZR_U32 CmdM1ResetCsnSet(SLZR_U8 *buf, SLZR_U32 *len, SLZR_U8 *status);
extern SLZR_U32 CmdStm32ResetSet(SLZR_U8 *status);
extern SLZR_U32 CmdStm32UpgradeSet(SLZR_U8 allnum, SLZR_U8 nownum, SLZR_U8* upbuf, SLZR_U8 *status);
extern SLZR_U32 CmdStm32VersionSet(SLZR_U8 *status);
extern SLZR_U32 CmdDetectPsamSet(SLZR_U32 u32Num, SLZR_U8 *pBuf, SLZR_U32 *pLen, SLZR_U8 *pStatus);
//extern SLZR_U32 UartReadData(SLZR_U8 *pRecvData, SLZR_U32 *pRcvLen);
extern SLZR_U32 UartWriteData(SLZR_U8 *pSendData, SLZR_U32 u32SendLen);
extern SLZR_U32 UartSendAndRecv(SLZR_U8 *pSendData, SLZR_U32 u32SendLen, SLZR_U8 *pRecvData, SLZR_U32 *pRcvLen, SLZR_U8 *u8Status);
extern SLZR_U32 UartRead_QRCodeData(SLZR_U8 *pRecvData, SLZR_U32 *u32SendLen);
extern SLZR_U32 CMDPSAM_BPS(SLZR_U32 u32Num, SLZR_U8 *pBuf, SLZR_U32 *pLen, SLZR_U8 *pStatus);
extern SLZR_U32 CMD_PSAMAUTH(SLZR_U32 u32Num, SLZR_U8 *pBuf, SLZR_U32 *pLen, SLZR_U8 *pStatus);
extern SLZR_U32 CmdPSAMbps(LTY_U8 BpsFlag);

extern SLZR_U32 R485_Init(void);
extern SLZR_U32 R485WriteData(SLZR_U8 *pSendData, SLZR_U32 u32SendLen);
extern SLZR_U32 R485ReadData(SLZR_U8 *pRecvData, SLZR_U32 *pRcvLen);
extern int GPRS_ping_Hi(void);
extern int GPRS_ping_Lo(void);



