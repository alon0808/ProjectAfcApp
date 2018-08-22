/// @file PSAMCard.c
/// @brief 
/// @author Zhou
/// @version V1.0.0
/// @date 2018-06-19
#include "Macro_Proj.h"
//#include "LtyTypeDef.h"
//#include "LtyError.h"
#include "SlzrTypeDef.h"
#include "SlzrError.h"

#include "PsamCard.h"
#include "IcCardCtrlApi.h"

#include "debug_.h"

//#include "LtyMsgNet.h"
//#include "LtyMsgUpg.h"
//#include "LtyMsgUserInput.h"
//#include "LtyMsgItsSD.h"
//#include "LtyMsgSysCtrl.h"


//****************APDU*******************************//	 
#define RSLT_OK             0                           	//流程中操作数据确认
#define RSLT_ESC            1                           	//流程中操作取消        
#define RSLT_OVERTIME       2                           	//流程中超时
#define COMMANDERR 0xAA	
#define COMMTIMEOUT 0xBB
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//


//****************站点票价计算*******************************//
#define MAX_STATION_INSYSPARAM      256
#define SIZE_OF_EVERYPRICE          2
#define PRICE_MAXOFFSET             32512 //127*256
#define DISRATE_LEN                 64
#define DISSEG_LEN                  2
#define FULL_PERCENT                100
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//







//****************几个全局变量*******************************//


 
SLZR_U8 m_szKeyBufA[6];
SLZR_U8 m_szKeyBufB[6];

SLZR_U32 m_M1PsamTmkIndex;
SLZR_U32 m_CPUPsamTmkIndex;
SLZR_U8	m_szM1PsamList[6];
SLZR_U8	m_szCpuPsamList[6];

SLZR_U8	m_CardTypeIC;
SLZR_U32 m_CpuPsamNum;
SLZR_U32 m_M1PsamNum;


SLZR_U32 m_PsamNum;


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//




//****************CPsamCard Class*******************************//
SLZR_VOID CPsamCard(void)
{	
    m_PsamNum = 0;
    m_M1PsamNum = 0;
    m_M1PsamTmkIndex = 0;
    m_CPUPsamTmkIndex = 0;
	m_CardTypeIC = 0;
	memset(m_szKeyBufA, 0, sizeof(m_szKeyBufA));
    memset(m_szKeyBufB, 0, sizeof(m_szKeyBufB));
	memset(m_szM1PsamList, 0, sizeof(m_szM1PsamList));
    memset(m_szCpuPsamList, 0, sizeof(m_szCpuPsamList));
//	m_pIcCardCtrlApi = new CIcCardCtrlApi();
//	m_cUart          =  new CUart();
//	m_u8PriceTableUp = NULL;
//	m_u8CardTypeTable = NULL;
	

}   

SLZR_RESULT CPsamCard_QRCodeInit(SLZR_U8 *RcvBuff, SLZR_U32 *RcvBuff_len)
{
	SLZR_U32 u32Ret;

	Card_QRCodeInit();
	u32Ret = QRCodeInit(RcvBuff, RcvBuff_len);
	
	return u32Ret;
}
/*
SLZR_VOID CPsamCard::Release()
{	
	
    if ( m_pIcCardCtrlApi )        delete m_pIcCardCtrlApi;
    m_pIcCardCtrlApi = NULL;
	if ( m_CIcCardRC500 )        delete m_CIcCardRC500;
    m_CIcCardRC500 = NULL;
	if ( m_pIcCard )        delete m_pIcCard;
    m_pIcCard = NULL;
	if ( m_cUart)        delete m_cUart;
    m_cUart = NULL;	
	if (NULL != m_u8PriceTableUp){
        free(m_u8PriceTableUp);
        m_u8PriceTableUp = NULL;
    }
    if (NULL != m_u8CardTypeTable){
        free(m_u8CardTypeTable);
        m_u8CardTypeTable = NULL;
    }
}
*/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//







//****************PSAM*******************************//
SLZR_U32 DetectPsamSet(SLZR_U32 u32Num, SLZR_U8 *pBuf, SLZR_U32 *pLen, SLZR_U8 *pStatus) 
{
    SLZR_RESULT u32Ret = SLZR_SUCCESS;
    u32Ret = CmdDetectPsamSet(u32Num, pBuf, pLen, pStatus); 
    
    return u32Ret; 
}

SLZR_U32 PsamApduSet(SLZR_U32 u32Num, SLZR_U8 *pApdu, SLZR_U32 u32Len, SLZR_U8 *pBuf, SLZR_U32 *pRdLen, SLZR_U8 *pStatus)
{
    SLZR_RESULT u32Ret = SLZR_SUCCESS;
    u32Ret = CmdPsamApduSet(u32Num, pApdu, u32Len, pBuf, pRdLen, pStatus); 
    return u32Ret; 
}

SLZR_U32 SendPsamData(SLZR_U8 *pDataIn , SLZR_U16 u16InLen , SLZR_U8 *pDataOut , SLZR_U16 *pOutLen)
{
    SLZR_U32 u32Ret = SLZR_SUCCESS;
    {
        ///_DBG_("\nDataIn:\n");
        int i = 0;
        for ( i = 0; i < u16InLen; i++)
            printf("%02x ", pDataIn[i]);
        printf("\n");
        ///_DBG_("\nEND");
    }
    
    u32Ret = PsamApduSet(m_PsamNum, pDataIn, (SLZR_U32)u16InLen, pDataOut, (SLZR_U32*)pOutLen, (SLZR_U8 *)&u32Ret);

    {
        ///_DBG_("\nDataOut:\n");
        int i = 0;
        for ( i = 0; i < *pOutLen; i++)
            printf("%02x ", pDataOut[i]);
        printf("\n");
        ///_DBG_("\nEND");
    }
	
    ///_DBG_("SendRecPasmApdu PsamNum[%d],  rslt=%d\n", m_PsamNum, u32Ret);

    return u32Ret;
}
	
SLZR_U32 PsamSelectApp(SLZR_U32 mode)
{
    ApduDataReq_t ApduSend;
    ApduDataResp_t ApduRec;
	
    memset(&ApduSend,0,sizeof(ApduSend));
    memset(&ApduRec,0,sizeof(ApduRec));
	
    if(mode == 0)
    {
        memcpy(ApduSend.szCommand,"\x00\xA4\x04\x00",4);//DDF:00A40400 06 BD A8 C9 E8 B2 BF    
        ApduSend.u8Lc = 14;
        memcpy(ApduSend.szDataIn,"1PAY.SYS.DDF01",14);//memcpy(binname,"1PAY.SYS.DDF01",14);
    }
    else if(mode  == 1)
    {	
        memcpy(ApduSend.szCommand,"\x00\xA4\x04\x00",4); 
        ApduSend.u8Lc = 0x06;
        memcpy(ApduSend.szDataIn,"\xbd\xa8\xc9\xe8\xb2\xbf",6);
    }	
    else if(mode  == 2)
    {
        memcpy(ApduSend.szCommand,"\x00\xA4\x00\x00",4);//DDF:00A40400 06 BD A8 C9 E8 B2 BF    
        ApduSend.u8Lc = 0x02;
        memcpy(ApduSend.szDataIn,"\x80\x11",2);
    }
    else if(mode == 3)
    {	
        memcpy(ApduSend.szCommand,"\x00\xA4\x00\x00",4);//DDF:00A40400 06 BD A8 C9 E8 B2 BF    
        ApduSend.u8Lc = 0x02;
        memcpy(ApduSend.szDataIn,"\x10\x01",2);
    }
					
	ApduSend.u8Le = 0x00;
	
    PsamCommand(&ApduSend,&ApduRec);
    ///_DBG_(">>>>>>>>>>>>>>>SWA[0x%02x] SWB[0x%02x]\n", ApduRec.u8SWA, ApduRec.u8SWB);
    if((ApduRec.u8SWA == 0x90) && (ApduRec.u8SWB == 0x00))
    {	
        ///_DBG_("SelectPsamApp IS OK\n");
        return SLZR_SUCCESS;
    }
		
    ///_ERR_("SelectPsamApp IS NOT OK\n");

    return SLZR_FAILURE;
}

SLZR_U32 CpuPsamInit(void)
{
    SLZR_RESULT u32Ret = SLZR_SUCCESS;
    SLZR_U8     buf[100] = {0};
    
    u32Ret = PsamReadMerchant(buf,1);//读取psam卡号即终端编号
    if(u32Ret != SLZR_SUCCESS)
    {
        ///_ERR_("[PsamReadMerchant get PsamTerminalNo error]\n");
        return SLZR_FAILURE; 
    }
    else
    {
        ///_DBG_("[PsamReadMerchant get PsamTerminalNo] = %s \n",m_szCpuPsamList);
    }
	                         	
    u32Ret = PsamSelectApp(2);//选择互联互通的应用
    u32Ret = SLZR_SUCCESS; ///////////////////////////////////////////////Jesse
    if(u32Ret != SLZR_SUCCESS)
    {	
        ///_ERR_("[PsamSelectApp DF error]\n");
        return SLZR_FAILURE; 
    }
			
    					
    u32Ret = PsamReadMerchant(buf,3);//选择互联互通的应用
    if(u32Ret != SLZR_SUCCESS)
    {
        ///_ERR_("[PsamReadMerchant get PsamTmkIndex error]\n");
        return SLZR_FAILURE; 
    }   
    else
    {	
        ///_DBG_("[PsamReadMerchant get PsamTmkIndex] = %d \n", m_CPUPsamTmkIndex);
    }
	
    memcpy(buf,"\x12\x34\x56\x78\x00\x01\x00\x00\x00\x00\x06\x20\x17\x06\x26\x14\x24\x00\x01\x00\x62\x28\x00\x01\x00\x00\x11\x17\x09\x99\x10\x01\xFF\xFF\xFF\xFF",36);

    return u32Ret;
}

SLZR_VOID DealPsamCardData(SLZR_U8 *pTempSendData, SLZR_U16 u16TempDataLen, ApduDataResp_t *pApdu_r)
{
    SLZR_U16 u16TempLen = 0;
    SLZR_U16 EMVRxLen = 0;
    SLZR_U8 EMVRxBuf[300] = {0};
    SLZR_RESULT u32Ret = SLZR_SUCCESS;

    u32Ret = SendPsamData(pTempSendData, u16TempDataLen, EMVRxBuf, &EMVRxLen);

    if (u32Ret != SLZR_SUCCESS)
    {
        pApdu_r->u8ReadCardDataOk = COMMANDERR;
        ///_ERR_("SendPsamData Error!");
        return;
    }
				
    u16TempLen = EMVRxLen;
    pApdu_r->u8ReadCardDataOk = 1;
    if (u16TempLen > 2)
    {
        pApdu_r->u16LenOut = u16TempLen - 2;
        memcpy(pApdu_r->szDataOut, &EMVRxBuf[0], pApdu_r->u16LenOut);
        pApdu_r->u8SWA = EMVRxBuf[u16TempLen - 2];
        pApdu_r->u8SWB = EMVRxBuf[u16TempLen - 1];
    }	
    else if (u16TempLen == 2)
    {   
        pApdu_r->u16LenOut = 0;
        pApdu_r->u8SWA = EMVRxBuf[u16TempLen - 2];
        pApdu_r->u8SWB = EMVRxBuf[u16TempLen - 1];
    }	
    else
    {
        ///_ERR_("COMMANDERR!");
        pApdu_r->u8ReadCardDataOk = COMMANDERR;
    }    
	  
    return;
}

SLZR_VOID PsamCommand(ApduDataReq_t *ApduSend, ApduDataResp_t *apdu_r)     
{
    SLZR_U8 szIcCmd[300] = {0};
    SLZR_U32 u32Len = 0;

    memset(szIcCmd, 0, sizeof(szIcCmd));
    memcpy(szIcCmd, ApduSend->szCommand, 4);
    u32Len = u32Len + 4;
    ApduSend->u8Lc &= 0x00ff;
	
    if (ApduSend->u8Lc > 0)  
    {
        szIcCmd[u32Len++] = ApduSend->u8Lc;

        if (ApduSend->u8Lc > 255)
        {
            ApduSend->u8Lc = 255;         
        }
        memcpy(&szIcCmd[u32Len], ApduSend->szDataIn, ApduSend->u8Lc);
		
        u32Len += ApduSend->u8Lc;
    }	
    else       
    {    
        szIcCmd[u32Len++] = 0;
    }		
    if (ApduSend->u8Le > 0)    
    {	
        ApduSend->u8Le &= 0x00ff;
        szIcCmd[u32Len++] = (ApduSend->u8Le);
    }	
    else  
    {            
        szIcCmd[u32Len++] = 0;
    }
	
    memset(apdu_r, 0, sizeof(ApduDataResp_t));
    DealPsamCardData(szIcCmd, (unsigned short)u32Len, apdu_r);
}

SLZR_U32 M1PsamInit(void)
{
    SLZR_RESULT u32Ret = SLZR_SUCCESS;
    SLZR_U8 buf[100] = {0};

    u32Ret = PsamReadMerchant(buf,0);//读取psam卡号即终端编号
    if(u32Ret != SLZR_SUCCESS)
    {
        ///_ERR_("[PsamReadMerchant get PsamTerminalNo error]\n");
        return SLZR_FAILURE; 
    }


    u32Ret = PsamSelectApp(3);//选择邯郸psam卡应用
    if(u32Ret != SLZR_SUCCESS)
    {
        ///_ERR_("[PsamSelectApp DF error]\n");
        return SLZR_FAILURE; 
    }
									
    u32Ret = PsamReadMerchant(buf,2);//读取psam卡密钥索引
    if(u32Ret != SLZR_SUCCESS)
    {
        ///_ERR_("[PsamReadMerchant get PsamTmkIndex error]\n");
        return SLZR_FAILURE; 
    }
		
    return u32Ret;
}

SLZR_U32 PsamReadMerchant(SLZR_U8 *pOutBuf, SLZR_S32 mode)
{
    ApduDataReq_t ApduSend;
    ApduDataResp_t ApduRec;
	
    memset(&ApduSend,0,sizeof(ApduSend));
    memset(&ApduRec,0,sizeof(ApduRec));
    if(mode == 0) 
    {		
        memcpy(ApduSend.szCommand,"\x00\xB0\x96\x00",4);     
        ApduSend.u8Lc = 0x00;     
        ApduSend.u8Le = 0x06;
    }    
    else if(mode == 1)
    {	
        memcpy(ApduSend.szCommand,"\x00\xB0\x96\x00",4);     
        ApduSend.u8Lc = 0x00;     
        ApduSend.u8Le = 0x06;//19
    }   
    else if(mode == 2)
    {
        memcpy(ApduSend.szCommand,"\x00\xB0\x97\x00",4);     
        ApduSend.u8Lc = 0x00;     
        ApduSend.u8Le = 0x01;
    }
    else if(mode == 3)
    {			
        memcpy(ApduSend.szCommand,"\x00\xB0\x97\x00",4);     
        ApduSend.u8Lc = 0x00;     
        ApduSend.u8Le = 0x01;
    }	
    PsamCommand(&ApduSend,&ApduRec);
    if((ApduRec.u8SWA == 0x90) && (ApduRec.u8SWB == 0x00))
    {
        ///_DBG_("ReadPsamMerchant szDataOut[0] = [%x]\n",ApduRec.szDataOut[0]);
        if(mode == 0)
        {
            memcpy(m_szM1PsamList,ApduRec.szDataOut,6); 
        }	
        else if(mode == 1)
        {	
            memcpy(m_szCpuPsamList,ApduRec.szDataOut,6); 
        }	
        else if(mode == 2)
            m_M1PsamTmkIndex = ApduRec.szDataOut[0];
        else if(mode == 3)
            m_CPUPsamTmkIndex = ApduRec.szDataOut[0];
        else                                           
            memcpy(pOutBuf,ApduRec.szDataOut,0x1E);
    }	                           
    else
    {	
        ///_DBG_("[PsamCommand error]:ApduRec.SWA = [%02x],ApduRec.SWB = [%02x]\n",ApduRec.u8SWA,ApduRec.u8SWB);
        return SLZR_FAILURE;
    }       
    return SLZR_SUCCESS;
}	

SLZR_U32 M1CardRead(SLZR_U32 u32KeyMode, SLZR_U32 u32Block, SLZR_U8 *sendbuf, SLZR_U8 *recbuf, SLZR_U32 *recbuflen)
{
	SLZR_U8 status = 0;
	SLZR_U32 ret = 0, keyblock;

	printf("M1卡读卡开始 \n");
	ret = CmdLoadkeySet(sendbuf,6, &status);
	if((ret == MI_OK) && (status == MI_OK))
	{
	
		keyblock = ((u32Block / 4 + 1) * 4)- 1;
			
		ret = CmdAuthenticationSet(u32KeyMode, keyblock, &status);
		if((ret == MI_OK) && (status == MI_OK))
		{
			ret = CmdReadSet(u32Block, recbuf, recbuflen, &status);
			if((ret == MI_OK) && (status == MI_OK))
			{
			//	printf("M1_SUCC_%d:", *recbuflen);
			//	for(i=0;i<*recbuflen;i++)					printf("%02x",recbuf[i]);
			//	printf("\n");		
				return SLZR_SUCCESS;		
			}
			else
			{
				printf("读扇区信息1失败 \n");
				return ERDBLOCK;
			}
		
		}
		else
		{
			printf("CmdAuthenticationSet失败 \n");
			return EAUTHENSET;
		}
	}
	else
	{
		printf("CmdLoadkeySet失败 \n");
		return ELOADKEYSET;
	}

}


SLZR_U32 CPUCardRead(SLZR_U8 *recbuf, SLZR_U32 *recbuflen)
{		
	SLZR_U8 sw[100], inbuf[50], status = 0;
	SLZR_U32 inbuf_len = 0, ret, i;
			
		//inbuf[0] = 0x00, inbuf[1] = 0xA4, inbuf[2] = 0x00, inbuf[3] = 0x00, inbuf[4] = 0x02, inbuf[5] = 0x3F, inbuf[6] = 0x00;
		inbuf[0] = 0x00, inbuf[1] = 0xA4, inbuf[2] = 0x04, inbuf[3] = 0x00, inbuf[4] = 0x0e, inbuf[5] = 0x32, inbuf[6] = 0x50;
		inbuf[7] = 0x41, inbuf[8] = 0x59, inbuf[9] = 0x2E, inbuf[10] = 0x53, inbuf[11] = 0x59, inbuf[12] = 0x53, inbuf[13] = 0x2E;
		inbuf[14] = 0x44, inbuf[15] = 0x44, inbuf[16] = 0x46, inbuf[17] = 0x30, inbuf[18] = 0x31;
		inbuf_len = 19;
		printf("inbuf_%d:", inbuf_len);
				for(i=0;i<inbuf_len;i++)
					printf("%02x,",inbuf[i]);
				printf("\n");
	
	printf("CPU卡读卡开始 \n"); 	
	
	printf("CPU卡复位...");
	ret = CmdRatsSet(sw, &inbuf_len,&status);
	if((ret == MI_OK) && (status == MI_OK))
	{
		printf("CPU卡复位成功");
	}
	else
		return EAUTHENSET;
	
	printf("CPU卡读卡信息...");
	ret = CmdApduSet(inbuf, inbuf_len, recbuf, recbuflen,&status,sw);
	if((ret == MI_OK) && (status == MI_OK))
	{
	//	printf("M1_SUCC_%d:", *recbuflen);
	//	for(i=0; i<*recbuflen; i++)			printf("%02x",recbuf[i]);
	//	printf("\n");

		return SLZR_SUCCESS;
	}
	else
	{
		printf("读信息失败 \n");
		return ERDBLOCK;
	}

}

SLZR_U32 SearchPsamCard()
{
	SLZR_U32 i, u32Len, u32Ret;
    SLZR_U32 u32Flag = 0;
    SLZR_U8  u8Star = 0;
    SLZR_U8  szCardBuf[100] = {0};
	
	while(1)
	{													
		for(i = 1; i < 5; i++)
		{	 
						
			u32Ret = DetectPsamSet(i, szCardBuf, &u32Len, &u8Star);
			if ( SLZR_SUCCESS == u32Ret )
			{																			
				//_WARN_("Detect Psam Card \n");
				m_PsamNum = i;
				if ( (u32Flag != 1) && ((u32Ret = CpuPsamInit()) == SLZR_SUCCESS) )
				{				
					u32Flag += 1; 
					m_CpuPsamNum = i; 
					printf("Find CPU PSAM, NUM[%d]\n", m_CpuPsamNum);
				} else if ( (u32Flag != 2) && ((u32Ret = M1PsamInit()) == SLZR_SUCCESS) ) 
				{	
					u32Flag += 2;
					m_M1PsamNum = i;
					printf("Find M1 PSAM, NUM[%d]\n", m_M1PsamNum);
					
				}
				else if((u32Ret = CMD_PSAMAUTH(i, szCardBuf, &u32Len, &u8Star)) == SLZR_SUCCESS )
				{
					printf("PSAM AUTH is OK!\n");
				}
				else
					printf("No Find M1 OR CPU PSAM!\n");
				u32Ret = SLZR_SUCCESS;
		   }		   
		}	
		
		//PSAM卡错误提示		
		if(u32Flag==0)
		{								
			printf("PSAM卡异常 \n");
				
		}
		else	
			break;
		
		 usleep(100000);
	}

	return SLZR_SUCCESS;
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
//****************寻卡*******************************//
SLZR_U32 SearchCard(SLZR_U8 *pData, SLZR_U32 *pLen)
{		
    static SLZR_U32	u32RcvLen = 0;
    static SLZR_U8 RcvBuf[256]= {0};
    SLZR_U8 bLoop = SLZR_TRUE;
    SLZR_U32 u32Step = 0;
    SLZR_U32 u32Ret = SLZR_SUCCESS;
    SLZR_U8  u8Status = SLZR_SUCCESS; 
    SLZR_U32 i = 0;
	
    u32Step = 1;
    m_CardTypeIC = 0;
	//_DBG_("Search READ start!!---11");
    while(bLoop == SLZR_TRUE)
    {
        switch(u32Step)
        {
            case 1:
                for(i=0; i<5; i++)
                {
                	_DBG_("Search READ start!!---22");
                    u32Ret = CmdFindCardSet(RcvBuf, &u32RcvLen, &u8Status);
                    if((u32Ret == SLZR_SUCCESS) && (u8Status == SLZR_SUCCESS))
                    {
						//_DBG_("%s", RcvBuf);
                    	_DBG_("Search READ start!!---55");						
                        u32Step++;
                        break;
                    }
					_DBG_("Search READ start!!---44");
                    usleep(1000);
                }   
                if(i==5) bLoop = SLZR_FALSE;
                break;
			
            case 2:
               /* m_ReadCardNum =0;
                {
                    unsigned char i;
                    _DBG_("[RC500] Read RESET = %d\n",u32RcvLen);
                    for(i =0 ; i < u32RcvLen; i ++)
                    {
                        _DBG_("%02X ",RcvBuf[i]);
                    }
                    printf("\n");
                }	
				*/	
                if(RcvBuf[4] == 0x20)
                {								
                    m_CardTypeIC = 0x20;
                }   
                else if (RcvBuf[4] == 0x08)
                {	
                    m_CardTypeIC = 0x08;
                }	
                else
                {		
                    m_CardTypeIC = RcvBuf[4];
                }																																													
                u32Step = m_CardTypeIC;
                    			
               // memset(&m_CardInfo, 0, sizeof(CardInform));
                //memcpy(m_CardInfo.UserIcNo,RcvBuf,4);
               /// m_pIcCard->Setm_CardInform(&m_CardInfo);
                memcpy(pData,RcvBuf, u32RcvLen);
                *pLen = u32RcvLen;
                bLoop = SLZR_FALSE;
                break;
			
            default :
                bLoop = SLZR_FALSE;
                break;
        }
    }
    return u32Step;
}



SLZR_U32 QRCodeInit(SLZR_U8 *RcvBuff, SLZR_U32 *RcvBuff_len)
{
//	SLZR_U8  u8Star;	//,RcvBuff[512]
	SLZR_U32 Ret;	//RcvBuff_len, 
	
	printf("读二维码123:\n");

	while(1)
	//for(i=0; i<5; i++)
	{
		Ret = UartRead_QRCodeData(RcvBuff, RcvBuff_len);
		if(Ret == MI_OK)	// && *RcvBuff_len > 5
		{
	//		printf("QRCode-:");	{	printf("RcvBuff-:%s\n", RcvBuff);}
			return SLZR_SUCCESS;
		}
	}
	printf("结束\n");
	
	return SLZR_FAILURE;
}


