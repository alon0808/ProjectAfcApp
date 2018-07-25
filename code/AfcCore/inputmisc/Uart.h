/// @file Uart.h
/// @brief 
/// @author 
/// @version V1.0.0
/// @date 2018-06-05

#include "LtyCommon.h"
#include "ev.h"
#include "LtyTimer.h"	
#include "SlzrTypeDef.h"

	
extern SLZR_U32 Uart_Init(SLZR_U32 *pUartFd, SLZR_U8 *pUartDev, SLZR_U32 u32Speed);
extern SLZR_U64 Uart_GetTimeUsec(void);
extern SLZR_U32 UartRead(SLZR_U32 u32UartFd, SLZR_U8 *pBuff, SLZR_U32 u32Len);
extern SLZR_U32 UartWrite(SLZR_U32 u32UartFd, SLZR_U8 *pBuff, SLZR_U32 u32Len);
extern SLZR_U32 UartInit(const SLZR_U8 *pCom, SLZR_U32 u32Speed);
extern SLZR_U32 UartOpen(SLZR_U8 *pUartDev);
extern SLZR_VOID UartSetSpeed(SLZR_U32 u32UartFd, SLZR_U32 u32Speed);
extern SLZR_U32 UartSetParity(SLZR_U32 u32UartFd, SLZR_U32 u32DataBits, SLZR_U32 u32StopBits, SLZR_U32 u32Parity);
extern SLZR_VOID UartTcFlush(SLZR_U32 u32UartFd);


