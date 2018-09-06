/// @file IcCardCtrlApi.cpp
/// @brief 
/// @author Jesse Xu <zhimo811@gmail.com>
/// @version V1.0.0
/// @date 2017-12-18
#include "Macro_Proj.h"
#include "IcCardCtrlApi.h"
#include "PsamCard.h"
#include "SlzrError.h"
#include "GPIOCtrl.h"

#include "debug_.h"

#include <string.h>
#include "szct.h"

#define _debug_IcApi_

#define GPIO_PIN_HI		0	/*置 高电平*/
#define GPIO_PIN_LOW	1	/*置 低电平*/

#define GPIO_DIR_IN		1	/*拉进*/
#define GPIO_DIR_OUT	2	/*推出*/


#define IC_R485_UART "ttyS6"
#define IC_CARD_UART "ttyS5"
#define QR_CODE_UART "ttyS4"

#define COMRECEIVE_DURATION_MS      10
#define COMRECV_TIMER               0

SLZR_U32 m_u485UartFd;
SLZR_U32 m_u32UartFd;
SLZR_U32 m_u32UartQRCodeFd;

typedef enum{
    CMD_BEEP = 0xA0,
    CMD_REQUEST,
    CMD_ANTICOLL,
    CMD_SELECT,
    CMD_AUTHENTICATION,
    CMD_HALT,
    CMD_READ,
    CMD_WRITE,
    CMD_LOADKEY,
    CMD_RATS,
    CMD_APDU,
    CMD_FIND_CARD,
    CMD_DETECT_PSAM1_CARD,  //ac
    CMD_PSAM1_APDU,
    CMD_DETECT_PSAM2_CARD,
    CMD_PSAM2_APDU,
    CMD_DETECT_PSAM3_CARD,
    CMD_PSAM3_APDU,
    CMD_DETECT_PSAM4_CARD,
    CMD_PSAM4_APDU,
    CMD_M1_REST_CSN,
    CMD_KEYA,
    CMD_KEYB,
    CMD_STM32Reset,
    CMD_M1_INC = 0xC0,
    CMD_M1_DEC,
    CMD_M1_RESTORE,
    CMD_M1_TRANSFER,
    CMD_UPGRADE = 0xc5,
    CMD_READ_VERSION,
    CMD_PSAM_bps = 0xc8,
    CMD_MAX
} UART_CMD;

SLZR_VOID CIcCardCtrlApi(void)
{
    m_u485UartFd = 0;
    m_u32UartFd = 0;
    m_u32UartQRCodeFd = 0;
}

#define CAMER_POWER 227   
//PH3
SLZR_U32 R485_Init(void)
{
	m_u485UartFd = 0;	

	InitGPIOPin(235, 0/*GPIO_PIN_HI*/, 2/*GPIO_DIR_OUT*/);

	//这个口软件要拉低
	InitGPIOPin(CAMER_POWER, 1/*GPIO_PIN_LOW*/,2/*GPIO_DIR_OUT*/);

	Uart_Init(&m_u485UartFd, (SLZR_U8*)IC_R485_UART, 19200);  

    return SLZR_SUCCESS;
}


SLZR_U32 Card_Init(void)
{
	m_u32UartFd = 0;	

   Uart_Init(&m_u32UartFd, (SLZR_U8*)IC_CARD_UART, 115200); 
//	CUart::Init(&m_u32UartFd, (SLZR_U8*)IC_CARD_UART, 38400); 
		
    return SLZR_SUCCESS;
}

SLZR_U32 Card_QRCodeInit(void)
{
	m_u32UartQRCodeFd = 0;
	
    Uart_Init(&m_u32UartQRCodeFd, (SLZR_U8*)QR_CODE_UART, 9600); 
		
    return SLZR_SUCCESS;
}

#define GPIO_3G2_RESET 81

int GPRS_ping_Hi(void)
{
	int ret;
	
	ret = InitGPIOPin(GPIO_3G2_RESET, GPIO_PIN_HI,GPIO_DIR_OUT);

//	ret = SetGPIOPin(GPIO_3G2_RESET, GPIO_PIN_HI,GPIO_DIR_OUT);

	return ret;
}

int GPRS_ping_Lo(void)
{
	int ret;
	
	ret = InitGPIOPin(GPIO_3G2_RESET, GPIO_PIN_LOW,GPIO_DIR_OUT);
//	ret = SetGPIOPin(GPIO_3G2_RESET, GPIO_PIN_LOW,GPIO_DIR_OUT);
	
	return ret;
}

/*******************************************************************************
 *	函数名称：  Verifycount 
 *	函数介绍:	计算和校验 
 *	函数描述:  
 *	修改日期: 	2017-06-6 
 *	输入参数：  pPacket:要进行校验的数据len:数据长度 
 *	输出参数：  无 
 *	返回值  ：	计算结果
 *******************************************************************************/
SLZR_U8 VerifyCount(SLZR_U8 *pPacket, SLZR_U32 u32Len)
{
    SLZR_U8 u8Ret = 0;
    SLZR_U32 i = 0;

    for(i = 0 ; i < u32Len ; i++)
    {
        u8Ret += pPacket[i];
    }

    return u8Ret&0x00ff;
}

SLZR_U32 RunCmd(SLZR_U32 u32CmdId, SLZR_U8 *pSndData, SLZR_U32 u32SndDataLen, SLZR_U8 *pRcvData, SLZR_U32 *pRcvLen, SLZR_U8 *pStatus)
{
    SLZR_U32 u32Ret = SLZR_SUCCESS;
    SLZR_U8 szDataBuf[512] = {0}; 
    SLZR_U8 szRecvBuf[1024] = {0};
    SLZR_U32 u32RecvLen = 0;
    SLZR_U8 u8Status = 0;
	
    memset(szDataBuf, 0, sizeof(szDataBuf));
    memset(szRecvBuf, 0, sizeof(szRecvBuf));
    
    memcpy(szDataBuf, "\x7D\x7D\x7D", 3);
    szDataBuf[3] = u32CmdId;
    szDataBuf[4] = u32SndDataLen;
    
    if(u32SndDataLen > 0)
    {	
        memcpy(szDataBuf + 5, pSndData, u32SndDataLen);
    }	
     	
    szDataBuf[u32SndDataLen + 5] = VerifyCount(szDataBuf, u32SndDataLen + 5);
    memcpy(szDataBuf + (u32SndDataLen + 6), "\x7B\x7B\x7B", 3);
#if 0	
#ifdef _debug_IcApi_
	{	
        SLZR_U32 i = 0;
		printf("sendbuf:");
        for ( i = 0; i < u32SndDataLen + 9; i++)
            printf("[%02X ]", szDataBuf[i]);
        ///_DBG_("\n");
    }/**/
#endif
#endif
    u32Ret = UartSendAndRecv(szDataBuf, u32SndDataLen + 9, szRecvBuf, &u32RecvLen, &u8Status);
    if( u32Ret == SLZR_FAILURE)
        return SLZR_FAILURE;
#if 0
#ifdef _debug_IcApi_
	printf("RunCmd u32RecvLen:%d\n", u32RecvLen);
#endif
#endif
    if ( u32RecvLen > 0 )
    {
        memcpy(pRcvData, szRecvBuf, u32RecvLen);
        *pRcvLen = u32RecvLen;
    }	
    *pStatus = u8Status;
		
    return SLZR_SUCCESS;
}

SLZR_U32 CmdPsamApduSet(SLZR_U32 u32Num, SLZR_U8 *pApdu, SLZR_U32 u32Len, SLZR_U8 *pBuf, SLZR_U32 *pRdLen, SLZR_U8 *pStatus)
{
    SLZR_U32 u32Ret = SLZR_SUCCESS;
    SLZR_U8 szDataBuf[512] = {0};
    SLZR_U8 szRvBuf[512] = {0};
    SLZR_U32 u32RvLen = 0;
    SLZR_U8 u8Sta = 0;
    SLZR_U32 u32DataLen = 0;
    SLZR_U32 u32Cmd = 0;
	
    ///_DBG_("===APDU_PSAM%d_Card===\n",u32Num);
    memcpy(szDataBuf, pApdu, u32Len);
    u32DataLen = u32Len;

    if(u32Num == 0)
    {
        ///_ERR_("Apdu Psam Card Error!");
        return SLZR_FAILURE;
    }
   
    switch(u32Num)
    {
        case 1:
            u32Cmd = CMD_PSAM1_APDU;
            break;
        case 2:
            u32Cmd = CMD_PSAM2_APDU;
            break;
        case 3:
            u32Cmd = CMD_PSAM3_APDU;
            break;
        case 4:
            u32Cmd = CMD_PSAM4_APDU;
            break;

    }
    u32Ret = RunCmd(u32Cmd, szDataBuf, u32DataLen, szRvBuf, &u32RvLen, &u8Sta);
    if(u32Ret == SLZR_SUCCESS)
    {
        *pStatus = u8Sta;
        if(u32RvLen > 0)
        {
            memcpy(pBuf, szRvBuf, u32RvLen);
            *pRdLen = u32RvLen;
            //if((szRvBuf[u32RvLen - 2] != 0x90)||(szRvBuf[u32RvLen - 1] == 0x00))
            //	*status = 1;
            return SLZR_SUCCESS;
        }
    }   
    return SLZR_FAILURE;
}

// 密码校验
SLZR_U32 CmdAuthenticationSet(SLZR_U32 u32KeyMode, SLZR_U32 u32Block, SLZR_U8 *pStatus)
{
    SLZR_U8 szDataBuf[512] = {0};
    SLZR_U8 szRvBuf[512] = {0};
    SLZR_U8 u8Sta = 0;
    SLZR_U32 u32RvLen = 0;
    SLZR_U32 u32DataLen = 0;
    SLZR_U8 u32Ret = SLZR_SUCCESS;

    szDataBuf[0] = u32KeyMode;
    szDataBuf[1] = u32Block;
    u32DataLen = 2;
    ///_DBG_("===Authentication Key===\n");
    u32Ret = RunCmd(CMD_AUTHENTICATION, szDataBuf, u32DataLen, szRvBuf, &u32RvLen, &u8Sta);//command_authentication
    //if((ret == SLZR_SUCCESS) && (rvlen == 0))
    if(u32Ret == SLZR_SUCCESS)
    {	
        *pStatus = u8Sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}

//终止
SLZR_U32 CmdHaltSet(SLZR_U8 *pStatus)
{
    SLZR_U8 szDataBuf[512] = {0};
    SLZR_U8 szRvBuf[512] = {0};
    SLZR_U32 u32RvLen = 0;
    SLZR_U8 u8Sta = 0;
    SLZR_U32 u32DataLen = 0;
    SLZR_U32 u32Ret = 0;
	
    ///_DBG_("===Halt card===\n");
    u32Ret = RunCmd(CMD_HALT, szDataBuf, u32DataLen, szRvBuf, &u32RvLen, &u8Sta);
    //if((ret == SLZR_SUCCESS) && (rvlen == 0))
    if(u32Ret == SLZR_SUCCESS)
    {
        *pStatus = u8Sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}

//读数据
SLZR_U32 CmdReadSet(SLZR_U32 block,SLZR_U8 *buf,SLZR_U32 *len, SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===read M1card data===\n");
    datalen = 1;
    databuf[0] = block;//读取长度
    ret = RunCmd(CMD_READ,databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        if(rvlen > 0)
        {
            memcpy(buf, rvbuf, rvlen);
            *len = rvlen;
            return SLZR_SUCCESS;
        }
    }
    return SLZR_FAILURE;
}

//写数据
SLZR_U32 CmdWriteSet(SLZR_U32 block, SLZR_U8 *buf, SLZR_U32 len, SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===write M1card data===\n");
    databuf[0] = block;
    memcpy(&databuf[1],buf,len);
    datalen = len + 1;
    ret = RunCmd(CMD_WRITE,databuf,datalen, rvbuf, &rvlen, &sta);
    //if((ret == SLZR_SUCCESS) && (rvlen == 0))
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}

//将密码装入读卡器RAM中
SLZR_U32 CmdLoadkeySet(SLZR_U8 *buf,SLZR_U32 len, SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===Loadkey===\n");
    memcpy(databuf,buf,len);
    datalen = len;
    ret = RunCmd(CMD_LOADKEY,databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}
		
//对CPU卡RATS（复位CPU卡）
SLZR_U32 CmdRatsSet(SLZR_U8 *buf,SLZR_U32 *len,SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===reset cpu card===\n");
    datalen = 0;
    ret = RunCmd(CMD_RATS,databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        if(rvlen > 0)
        {
            memcpy(buf, rvbuf, rvlen);
            *len = rvlen;
        }
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}

SLZR_U32 CmdApduSet(SLZR_U8 *inbuf,SLZR_U32 inlen,SLZR_U8 *outbuf,SLZR_U32 *outlen,SLZR_U8 *status, SLZR_U8 *sw)
{
	
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[1024] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;
	
    ///_DBG_("===APDU===\n");
    memcpy(databuf,inbuf,inlen);
    datalen = inlen;
    ret = RunCmd(CMD_APDU,databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {		
        *status = sta;
        if(rvlen > 0)
        {	
            memcpy(sw,&rvbuf[rvlen-2],2);
			
			outbuf[0] = 0x02; //要兼容原来的程序，在前面加了1位。
            memcpy(outbuf+1, rvbuf, rvlen);
            *outlen = rvlen+1;
            return SLZR_SUCCESS;
        }		
    }		
    return SLZR_FAILURE;		
}

//执行寻卡命令操作，含寻卡、防冲突、选卡
SLZR_U32 CmdFindCardSet(SLZR_U8 *buf,SLZR_U32 *len,SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    /////_DBG_("===find card===\n");
    datalen = 0;
    ret = RunCmd(CMD_FIND_CARD,databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        if(rvlen > 0)
        {
            memcpy(buf, rvbuf, rvlen);
            *len = rvlen;
            return SLZR_SUCCESS;
        }
    }
    return SLZR_FAILURE;
}


//钱包增值
SLZR_U32 CmdM1IncrementSet(SLZR_U32 block, SLZR_U8 *buf, SLZR_U32 len, SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===M1 Increment===\n");
    if((buf == NULL) || (len != 4))
    {
        return SLZR_FAILURE;
    }
    databuf[0] = block;
    memcpy(&databuf[1],buf,len);
    datalen = len + 1;
    ret = RunCmd(CMD_M1_INC,databuf,datalen, rvbuf, &rvlen, &sta);
    //if((ret == SLZR_SUCCESS) && (rvlen == 0))
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}

//钱包减值
SLZR_U32 CmdM1DecrementSet(SLZR_U32 block, SLZR_U8 *buf, SLZR_U32 len, SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;
	
    ///_DBG_("===M1 Decrement===\n");
    if((buf == NULL) || (len != 4))
    {
        return SLZR_FAILURE;
    }
    databuf[0] = block;
    memcpy(&databuf[1],buf,len);
    datalen = len + 1;
    ret = RunCmd(CMD_M1_DEC,databuf,datalen, rvbuf, &rvlen, &sta);
    //if((ret == SLZR_SUCCESS) && (rvlen == 0))
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}


//钱包恢复
SLZR_U32 CmdM1RestoreSet(SLZR_U32 block, SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===M1 Restore===\n");
    databuf[0] = block;
    datalen = 1;
    ret = RunCmd(CMD_M1_RESTORE,databuf,datalen, rvbuf, &rvlen, &sta);
    //if((ret == SLZR_SUCCESS) && (rvlen == 0))
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}


//钱包备份
SLZR_U32 CmdM1RestoreTransferSet(SLZR_U8 Toblock, SLZR_U8 Fromblock, SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===M1 RestoreTransfer===\n");
    databuf[0] = Toblock;
    databuf[1] = Fromblock;
    datalen = 2;
    ret = RunCmd(CMD_M1_TRANSFER,databuf,datalen, rvbuf, &rvlen, &sta);
    //if((ret == SLZR_SUCCESS) && (rvlen == 0))
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}

SLZR_U32 CmdM1ResetCsnSet(SLZR_U8 *buf,SLZR_U32 *len,SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===M1 reset===\n");
    datalen = 0;
    ret = RunCmd(CMD_M1_REST_CSN,databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        if(rvlen > 0)
        {
            memcpy(buf, rvbuf, rvlen);
            *len = rvlen;
            return SLZR_SUCCESS;
        }
    }
    return SLZR_FAILURE;
}


SLZR_U32 CmdStm32ResetSet(SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===STM32 reset===\n");
    datalen = 0;
    ret = RunCmd(CMD_STM32Reset,databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}


SLZR_U32 CmdStm32UpgradeSet(SLZR_U8 allnum, SLZR_U8 nownum, SLZR_U8 *upbuf, SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;

    ///_DBG_("===STM32 Upgrade===\n");
    memset(databuf,0,sizeof(databuf));
    datalen = 130;
    databuf[0] = allnum;
    databuf[1] = nownum;
    memcpy(databuf,upbuf,128);
    ret = RunCmd(CMD_UPGRADE, databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}


SLZR_U32 CmdStm32VersionSet(SLZR_U8 *status)
{
    SLZR_U8 databuf[512] = {0};
    SLZR_U8 rvbuf[512] = {0};
    SLZR_U32 rvlen = 0;
    SLZR_U8 sta;
    SLZR_U32 datalen,ret;
	
    ///_DBG_("===STM32 Version===\n");
    datalen = 0;
    ret = RunCmd(CMD_READ_VERSION, databuf,datalen, rvbuf, &rvlen, &sta);
    if(ret == SLZR_SUCCESS)
    {
        *status = sta;
        return SLZR_SUCCESS;
    }
    return SLZR_FAILURE;
}

SLZR_U32 CmdDetectPsamSet(SLZR_U32 u32Num, SLZR_U8 *pBuf, SLZR_U32 *pLen, SLZR_U8 *pStatus)
{
    SLZR_RESULT u32Ret = SLZR_SUCCESS;
    SLZR_U8 szDataBuf[512] = {0}; 
    SLZR_U8 szRsvBuf[512] = {0}; 
    SLZR_U32 u32RsvLen = 0;
    SLZR_U8 u8Sta = 0;
    SLZR_U32 u32DataLen = 0;
    SLZR_U32 u32Cmd = 0;
    
    ///_DBG_("Start Detect PSAM %d Card\n", u32Num);
    
    if ( u32Num == 0 )
    {
        ///_ERR_("Psam Card Num Error!\n");
        return SLZR_FAILURE;
    }
    switch ( u32Num )
    {
        case 1 : 
            u32Cmd = CMD_DETECT_PSAM1_CARD;
            break;
        case 2:
            u32Cmd = CMD_DETECT_PSAM2_CARD;
            break;
        case 3:
            u32Cmd = CMD_DETECT_PSAM3_CARD;
            break;	
        case 4:
            u32Cmd = CMD_DETECT_PSAM4_CARD;
            break;
        default :
            break;
    }
    
    u32Ret = RunCmd(u32Cmd, szDataBuf, u32DataLen, szRsvBuf, &u32RsvLen, &u8Sta);
    if ( SLZR_SUCCESS == u32Ret )
    {	
        *pStatus = u8Sta;
        if ( u32RsvLen > 0 )
        {	
            memcpy(pBuf, szRsvBuf, u32RsvLen);
            *pLen = u32RsvLen;		
            {
				printf("%2x_Rsvbuf:", u32Cmd);
                SLZR_U32 i = 0;    
                for ( i = 0; i < u32RsvLen; i++)
                    printf("%x ", pBuf[i]);
                ///_DBG_("END");
            }
            return SLZR_SUCCESS;
        }
    }
			
    return SLZR_FAILURE;
}
   
SLZR_U32 UartWriteData(SLZR_U8 *pSendData, SLZR_U32 u32SendLen)
{
    SLZR_U32 u32Ret = SLZR_SUCCESS;

    UartTcFlush(m_u32UartFd);
    u32Ret = UartWrite(m_u32UartFd, pSendData, u32SendLen);
    if ( u32Ret == SLZR_FAILURE )
    {
        return SLZR_FAILURE;
    }
    
    /*{
        SLZR_U32 i = 0;    
        for ( i = 0; i < u32SendLen; i++)
            printf("%x ", pSendData[i]);
        ///_DBG_("END");
    }*/
    return SLZR_SUCCESS;
}

SLZR_U32 UartReadData(SLZR_U8 *pRecvData, SLZR_U32 *pRcvLen)
{
    SLZR_U32 u32Ret = SLZR_SUCCESS;
    SLZR_U32 u32RcvLen = 0;
    SLZR_U8 szRcvBuf[1024] = {0};
    u32Ret = UartRead(m_u32UartFd, szRcvBuf, 5);

	//SLZR_U8 szBuf[10];	memcpy(szBuf, szRcvBuf, 4);
	
    if ( u32Ret == SLZR_FAILURE )
    {
		printf("[%s] ERROR0----\r\n", __FUNCTION__);
        ///_ERR_("Uart Read Error!");
        return SLZR_FAILURE;
    }
	

    if( (szRcvBuf[0] != 0x7B) || (szRcvBuf[1] != 0x7B) || (szRcvBuf[2] != 0x7B) )
    {
        ///_ERR_("Uart Read Data Error!!");
        return SLZR_FAILURE;
    }	
    if ( (szRcvBuf[4] >= 0) && (szRcvBuf[4] < 256) )
    {	
        u32Ret = UartRead(m_u32UartFd, &szRcvBuf[5], szRcvBuf[4] + 5);
        if ( u32Ret > 0)
        {	
            u32RcvLen = szRcvBuf[4] + 10;
            memcpy(pRecvData, szRcvBuf, u32RcvLen);
            *pRcvLen = u32RcvLen;
#if 0		
#ifdef _debug_IcApi_
            {
                SLZR_U32 i = 0;
				printf("SUCCESS:");
                for ( i = 0; i < u32RcvLen; i++)
                    printf("%02X ", pRecvData[i]);
				printf("\r\n");
                ///_DBG_("END");
            }
#endif
#endif
            return SLZR_SUCCESS;
        }
    }
	
    ///_ERR_("Uart Read Error!!");
    return SLZR_FAILURE;
}

SLZR_U32 UartSendAndRecv(SLZR_U8 *pSendData, SLZR_U32 u32SendLen, SLZR_U8 *pRecvData, SLZR_U32 *pRcvLen, SLZR_U8 *u8Status)
{
    SLZR_U32 u32Ret = SLZR_SUCCESS;
    SLZR_U8 szDataBuf[1024] = {0};
    SLZR_U32 u32Len = 0, ri=0;
	unsigned char cmd;

	cmd = pSendData[3];//保存命令。有时收到的返回命令和发送的不一致
	
revagain:

	// ///_DBG_("UartWriteData!!");
    u32Ret = UartWriteData(pSendData, u32SendLen);
#if 0
#ifdef _debug_IcApi_
	printf("\n****\n");	
#endif
#endif
    if ( u32Ret == SLZR_FAILURE )
        return SLZR_FAILURE;
#if 0
#ifdef _debug_IcApi_
	printf("Send End start Rev\r\n");
#endif
#endif

	// ///_DBG_("UartReadData!!");
    u32Ret = UartReadData(szDataBuf, &u32Len);
#if 0
#ifdef _debug_IcApi_
	printf("\nRcv:");
	for(SLZR_U32 i=0; i< u32Len; i++)	printf("%02X", szDataBuf[i]);
	printf("\n");    
#endif
#endif
    if ( u32Ret == SLZR_FAILURE )
        return SLZR_FAILURE;

	if(szDataBuf[3] != cmd){
		if(ri++ > 3)
			return SLZR_FAILURE;
		goto revagain;		//收到了另一命令,重发3次。
	}

	//_DBG_(format,...)("u32Len:%d, ", u32Len);for(int i=0; i< u32Len; i++)	_DBG_("%x", szDataBuf[i]);
    if( szDataBuf[4] > 0 )
    {
        memcpy(pRecvData, &szDataBuf[5], szDataBuf[4]);
        *pRcvLen = szDataBuf[4];

//_DBG_(format,...)("u32Len:%d, ", u32Len);for(int i; i< u32Len; i++)	_DBG_("%x", szDataBuf[i]);

    }
    *u8Status = szDataBuf[u32Len - 5];
   // ///_DBG_("PSAM_Card buffdata[rvlen-5]== %d\n",szDataBuf[u32Len - 5]);
    return SLZR_SUCCESS;
}
   
SLZR_U32 UartRead_QRCodeData(SLZR_U8 *pRecvData, SLZR_U32 *u32SendLen)
{
    SLZR_U32 u32Ret = SLZR_SUCCESS;
    SLZR_U8 szRcvBuf[1024] = {0};
    u32Ret = UartRead(m_u32UartQRCodeFd, szRcvBuf, 5);
	SLZR_U8 value = 0xFF;
	int i =0;

	//SLZR_U8 szBuf[10];	memcpy(szBuf, szRcvBuf, 4);
	
    if ( u32Ret == SLZR_FAILURE )
    {
        ///_ERR_("Uart Read Error!");
        return SLZR_FAILURE;
    }
	
    if( (szRcvBuf[0] != 0x7B) || (szRcvBuf[1] != 0x7B) || (szRcvBuf[2] != 0x7B) )
    {
        printf("Uart Read Data Error!!");
       // return SLZR_FAILURE;
    }	
	_DBG_("m_u32UartFd:%d\n", m_u32UartQRCodeFd);
	i = 5;
   do
    {	
        u32Ret = UartRead(m_u32UartQRCodeFd, &value, 1);

		//_ERR_("UartRead111:%d, %d, %02X!!\n", u32Ret, i, value);
        if ( u32Ret > 0)
        {
        	if (value == '\r' || value == '\n'){
				break;
        		}
        
        	szRcvBuf[i] = value;
        	++i;
           // return SLZR_SUCCESS;
        }
		else{
			return SLZR_FAILURE;
			}
    } while (value != '\r' && value != '\n');

   szRcvBuf[i] = '\0';

   strcpy((char *)pRecvData, (char *)szRcvBuf);
	
    //_ERR_("Uart Read Error!!");
    return SLZR_SUCCESS;
}

SLZR_U32 CMDPSAM_BPS(SLZR_U32 u32Num, SLZR_U8 *pBuf, SLZR_U32 *pLen, SLZR_U8 *pStatus)
{
    SLZR_RESULT u32Ret = SLZR_SUCCESS;
    SLZR_U8 szDataBuf[512] = {0}; 
    SLZR_U8 szRsvBuf[512] = {0}; 
    SLZR_U32 u32RsvLen = 0;
    SLZR_U8 u8Sta = 0;
    SLZR_U32 u32DataLen = 1;
    SLZR_U32 u32Cmd = 0;
    
    ///_DBG_("Start Detect PSAM %d Card\n", u32Num);
    
    if ( u32Num == 0 )
    {
        ///_ERR_("Psam Card Num Error!\n");
        return SLZR_FAILURE;
    }

	u32Cmd = CMD_PSAM_bps;
	szDataBuf[0] = 0x00;//	数据0x00 表示9600 bps,   	数据0x01 表示38400 bps
    
    u32Ret = RunCmd(u32Cmd, szDataBuf, u32DataLen, szRsvBuf, &u32RsvLen, &u8Sta);
    if ( SLZR_SUCCESS == u32Ret )
    {	
        *pStatus = u8Sta;
        if ( u32RsvLen > 0 )
        {	
            memcpy(pBuf, szRsvBuf, u32RsvLen);
            *pLen = u32RsvLen;		
            {
				printf("%02X_bps:", u32Cmd);
                SLZR_U32 i = 0;    
                for ( i = 0; i < u32RsvLen; i++)
                    printf("%02X ", pBuf[i]);
                ///_DBG_("END");
            }
            return SLZR_SUCCESS;
        }
    }
printf("CMDPSAM_BPS\n");
			
    return SLZR_FAILURE;

}



SLZR_U32 CMD_PSAMAUTH(SLZR_U32 u32Num, SLZR_U8 *pBuf, SLZR_U32 *pLen, SLZR_U8 *pStatus)
{
    SLZR_RESULT u32Ret = SLZR_SUCCESS;
    SLZR_U8 szDataBuf[512] = {0}; 
    SLZR_U8 szRsvBuf[512] = {0}; 
    SLZR_U32 u32RsvLen = 0;
    SLZR_U8 u8Sta = 0;
    SLZR_U32 u32DataLen = 7;
    SLZR_U32 u32Cmd = 0;
    
    ///_DBG_("Start Detect PSAM %d Card\n", u32Num);
    
    if ( u32Num == 0 )
    {
        ///_ERR_("Psam Card Num Error!\n");
        return SLZR_FAILURE;
    }
	else if( u32Num == 1)
	{
		u32Cmd = CMD_PSAM1_APDU;
	}
	else if( u32Num == 2)
	{
		u32Cmd = CMD_PSAM2_APDU;
	}
	else if( u32Num == 3)
	{
		u32Cmd = CMD_PSAM3_APDU;
	}
	else if( u32Num == 4)
	{
		u32Cmd = CMD_PSAM4_APDU;
	}

	//00A40000023F00
	szDataBuf[0] = 0x00; szDataBuf[1] = 0xA4; szDataBuf[2] = 0x00; szDataBuf[3] = 0x00; 
	szDataBuf[4] = 0x02; szDataBuf[5] = 0x3F; szDataBuf[6] = 0x00; 
 printf("AUTH:");   
    u32Ret = RunCmd(u32Cmd, szDataBuf, u32DataLen, szRsvBuf, &u32RsvLen, &u8Sta);
    if ( SLZR_SUCCESS == u32Ret )
    {	
        *pStatus = u8Sta;
        if ( u32RsvLen > 0 )
        {	
            memcpy(pBuf, szRsvBuf, u32RsvLen);
            *pLen = u32RsvLen;		
            {
				printf("%02X_bps:", u32Cmd);
                SLZR_U32 i = 0;    
                for ( i = 0; i < u32RsvLen; i++)
                    printf("%02X ", pBuf[i]);
                ///_DBG_("END");
            }
            return SLZR_SUCCESS;
        }
    }
printf("CMDPSAM_BPS\n");
			
    return SLZR_FAILURE;

}

SLZR_U32 CmdPSAMbps(SLZR_U8 BpsFlag)
{
	SLZR_U8 szDataBuf[512] = {0};
	SLZR_U8 szRvBuf[512] = {0};
	SLZR_U32 u32RvLen = 0;
	SLZR_U8 u8Sta = 0;
	SLZR_U32 u32DataLen = 0;
	SLZR_U32 u32Ret = 0;
	if(1==BpsFlag)
		szDataBuf[0] = 0x01;  //38400
	else
		szDataBuf[0] = 0x00;   //9600
		
	u32DataLen=1;    _DBG_("设置PSAM波特率\n");
	u32Ret = RunCmd(0xC8, szDataBuf, u32DataLen, szRvBuf, &u32RvLen, &u8Sta);
	//if((ret == LTY_SUCCESS) && (rvlen == 0))
	if(u32Ret == LTY_SUCCESS)
			{        return LTY_SUCCESS;    }
	
	return LTY_FAILURE;
}

extern void SetR485_EN(int on_off);

SLZR_U32 R485WriteData(SLZR_U8 *pSendData, SLZR_U32 u32SendLen)
{
	int di;
    SLZR_U32 u32Ret = SLZR_SUCCESS;
	
	SetR485_EN(1);
	usleep(1000);

    UartTcFlush(m_u485UartFd);
    u32Ret = UartWrite(m_u485UartFd, pSendData, u32SendLen);
	PRINT_DEBUG("R485WriteData:%02X, %02X\n", m_u485UartFd, u32Ret);
	BCD_LOG(pSendData, u32SendLen, 1);
    if ( u32Ret == SLZR_FAILURE )
    {
        return SLZR_FAILURE;
    }
//     SLZR_U32 i = 0; 
// 	printf("[%s] m_u485UartFd=%d write:", __FUNCTION__, m_u485UartFd);
//     for ( i = 0; i < u32SendLen; i++)
//         printf("%02x ", pSendData[i]);
// 	printf("\r\n");
	
	di = u32SendLen*1000;

	usleep(di);	//要延时，不然数据会丢失，而且发的数据越多，需要延时就越大
	SetR485_EN(0);

    return SLZR_SUCCESS;
}

SLZR_U32 R485ReadData(SLZR_U8 *pRecvData, SLZR_U32 *pRcvLen)
{
    SLZR_U32 u32Ret = SLZR_SUCCESS;
    SLZR_U32 u32RcvLen = 0;
    SLZR_U8 szRcvBuf[1024] = {0};
	
//	SetR485_EN(1);


	//PRINT_DEBUG("R485ReadData:%02X\n", m_u485UartFd);
    u32Ret = UartRead(m_u485UartFd, szRcvBuf, 1);

	PRINT_DEBUG("R485ReadData111:%d, %02X,%02X\n", u32Ret, m_u485UartFd, szRcvBuf[0]);
//	printf("[%s] m_u485UartFd=%d\r\n", __FUNCTION__, m_u485UartFd);

    if ( u32Ret == SLZR_FAILURE )
    {
//		printf("[%s] ERROR0----\r\n", __FUNCTION__);
        return SLZR_FAILURE;
    }
	
// 	for (int i = 0; i < u32Ret; i++)
//             printf("%02x ", szRcvBuf[i]);

	memcpy(pRecvData, szRcvBuf, u32Ret);
	*pRcvLen = u32Ret;
           
	return SLZR_SUCCESS;
}








