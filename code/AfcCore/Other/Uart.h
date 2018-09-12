/// @file Uart.h
/// @brief 
/// @author Jesse Xu <zhimo811@gmail.com>
/// @version V1.0.0
/// @date 2018-01-05

#include "LtyCommon.h"
#include "ev.h"
#include "LtyTypeDef.h"

class CUart 
{
public:
    CUart();
    virtual ~CUart();
    LTY_U32 Init(struct ev_loop *loop, LTY_U32 *pUartFd, LTY_U8 *pUartDev, LTY_U32 u32Speed);
    LTY_VOID Release();
		
public:
    LTY_U32 UartRead(LTY_U32 u32UartFd, LTY_U8 *pBuff, LTY_U32 u32Len);
    LTY_U32 UartWrite(LTY_U32 u32UartFd, LTY_U8 *pBuff, LTY_U32 u32Len);
    LTY_VOID UartTcFlush(LTY_U32 u32UartFd);
	
	LTY_U32 UartInit(const LTY_U8 *pCom, LTY_U32 u32Speed);
	 LTY_U64 GetTimeUsec(void);
private:
    
    LTY_U32 UartOpen(LTY_U8 *pUartDev);
    LTY_VOID UartSetSpeed(LTY_U32 u32UartFd, LTY_U32 u32Speed);
    LTY_U32 UartSetParity(LTY_U32 u32UartFd, LTY_U32 u32DataBits, LTY_U32 u32StopBits, LTY_U32 u32Parity);
   
};
