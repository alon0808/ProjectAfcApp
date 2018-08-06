#include <time.h>

#define LTY_DEBUG
#define LTY_DBG_PREFIX "CCardClt::"
#define LTY_LOG_PREFIX "[CCardClt]"
#include "CardClt.h"


static int GetAmount(char *Amount,char * buffer);

#if 0
static LTY_RESULT CardMsgHandler(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize, LTY_LPVOID lpPriv)
{
    return ((CCardClt *)lpPriv)->OnMessage(MsgType, u32Chn, u32ParaData, u32ParaSize);
}
#endif
CCardClt::CCardClt()
{
    m_bIsConn = LTY_FALSE;
    //memset(m_stAudio, 0, sizeof(Cardclt_audio_t)*CardCLT_AUDIO_MAX);
}

CCardClt::~CCardClt()
{
    Release();
}

LTY_VOID CCardClt::CltClose()
{
    SCClose();
    m_bIsConn = LTY_FALSE;
}

LTY_RESULT CCardClt::Init(struct ev_loop *loop,int _test_type)
{
    LTY_RESULT ret;
	m_test_type = _test_type;
    ret = SCInit(loop, 1024*1024, 8*1024, 0);
    if(ret != LTY_SUCCESS)
        return ret;

    //LtyMessageRegister(LTY_MSG_Card_GET_AUDIO, CardMsgHandler, 0, this);

    SCConnect(LTY_SOCKET_UNIX, (LTY_STRING)CARD_SOCKET_FILE, 0);

    return LTY_SUCCESS;
}

LTY_VOID CCardClt::Release()
{

}
extern void BYTECrc16CCITT(unsigned char* pbyDataIn,unsigned short dwDataLen,unsigned char* abyCrcOut);
LTY_U32 CCardClt::SCOnRecv(LTY_U8 *pRcvData, LTY_U32 u32DataLen)
{
		LTY_U8 *pData;
		LTY_U32 uDealLen = 0;	 //已处理数据长度
		LTY_U32 uRemainLen; 	 //未处理数据长度
		//card_cmd_head_t* pHead;
		card_cmd_head_t cmd_head;
	
	//	  HEXOUT(pRcvData, u32DataLen);
		DBGOUT("----------------------------------start\n");
	
		DBGOUT("u32DataLen = %d\n",u32DataLen);
		for(LTY_U32 j = 0; j<u32DataLen; j++)
		{
			printf("0x%x ",pRcvData[j]);
		}
		printf("\n");
	
		while(uDealLen < u32DataLen)
		{
			//计算待处理数据偏移
			pData = pRcvData + uDealLen;
			//计算待处理数据长度
			uRemainLen = u32DataLen - uDealLen;
			//如果待处理数据长度小于包头长度，则表示数据未接收完整，返回继续等待
			
			if(uRemainLen < 4/*sizeof(card_cmd_head_t)*/)
				break;
			
			cmd_head.u8Flag 		= pData[0]; 	/*包头标识*/
			cmd_head.u8CmdMaster	= pData[1]; 	/*命令主*/
			cmd_head.u8CmdSlave 	= pData[2]; 	/*命令从*/
			cmd_head.u8DataLen		= pData[3]; 	/*数据长度*/		
	
			if(!check_command_head(&cmd_head))
			{
				ERROUT("error pData[0]:%d,pData[1]:%d,pData[2]:%d,pData[3]:%d\n",pData[0],pData[1],pData[2],pData[3]);
				break;
			}
			//如数据包未接收完整，返回继续等待
			if (uRemainLen < cmd_head.u8DataLen + sizeof(card_cmd_head_t))
			{	
				ERROUT("error uRemainLen:%d, u8DataLen(card_cmd_head_t):%d\n",uRemainLen,cmd_head.u8DataLen + sizeof(card_cmd_head_t));
				break;
			}
	#if 0
			memset(recv_buf,0,SOCKET_BUF_SIZE);
			if(cmd_head.u8DataLen > 0)
			{
				memcpy(recv_buf, &pData[4], cmd_head.u8DataLen);	
			}
	#endif
			memcpy(&cmd_head.u16Parity, &pData[4 + cmd_head.u8DataLen], 2);

			LTY_CHAR receive_data[3];
			LTY_U8 parity_data[3];
			LTY_U16 new_parity = 0;
			BYTECrc16CCITT(&pData[1], sizeof(card_cmd_head_t) + cmd_head.u8DataLen - 1 - 2,parity_data);

			printf("%s	parity_data[1]:%x,parity_data[0]:%x\n",__func__,parity_data[1],parity_data[0]);
			new_parity = (parity_data[0] << 8) + parity_data[1];
			
			if (cmd_head.u16Parity == new_parity)
			{
				//uDealLen += 4;
				uDealLen += (cmd_head.u8DataLen + sizeof(card_cmd_head_t));
				printf("%s	u16Parity error\n",__func__);
				continue;
			}
			
			DBGOUT("-------------------Parity success!\n");
			
			uDealLen += (cmd_head.u8DataLen + sizeof(card_cmd_head_t));
		}
	
		DBGOUT("Dealed data length %d\n", uDealLen);
	
		DBGOUT("----------------------------------end\n");
	
		return uDealLen;

}

LTY_BOOL CCardClt::check_command_head(card_cmd_head_t *pcmd_head)
{
	LTY_BOOL ret_value = LTY_FALSE;
		
	do
	{
		/*第1字节*/
		if(CARD_CMD_REQ_FLAG != pcmd_head->u8Flag)
			break;

		/*第2字节*/
		switch(pcmd_head->u8CmdMaster)
		{
			/*2.显示控制命令--------------------------------*/
			case CARD_CMD_REQ_SHOW_CONTROL:
			case CARD_CMD_RSP_SHOW_CONTROL:
		    /*3.语音控制命令--------------------------------*/
			case CARD_CMD_REQ_VOICE_CONTROL:
			case CARD_CMD_RSP_VOICE_CONTROL:	
			{
				/*第3字节*/
				if(CARD_CMD_REQ_SLAVE == pcmd_head->u8CmdSlave)
				{
					ret_value = LTY_TRUE;
				}
			}break;
			/*4.参数导入导出控制命令------------------------*/
			case CARD_CMD_REQ_PARAM_CONTROL:
			case CARD_CMD_RSP_PARAM_CONTROL:
			{
				/*第3字节*/
				if(CARD_CMD_REQ_SLAVE_PARAM_IMPORT == pcmd_head->u8CmdSlave || 
				   CARD_CMD_REQ_SLAVE_PARAM_EXPORT == pcmd_head->u8CmdSlave)
				{
					ret_value = LTY_TRUE;
				}
				
			}break;
			
		}
		
	}while(0);

	return ret_value;
	
}


LTY_VOID CCardClt::SCOnConnect(LTY_RESULT ret)
{
	DBGOUT(" test111  ret:%d\n",ret);
    if(ret != LTY_SUCCESS)
    {
        DBGPOUT("connect failed, %s\n", LtyErrStr(ret));
        SCReconnect(5);
    }
    else
    {
        DBGPOUT("connect ok\n");
        m_bIsConn = LTY_TRUE;
        //LtyMessageSend(LTY_MSG_Card_CONNECT_SUCCESS, 0, 0, 0);
        test_data();
    }
}

LTY_VOID CCardClt::SCOnError(LTY_RESULT ret)
{
    DBGOUT("%s\n", LtyErrStr(ret));
    CltClose();
    SCReconnect(5);
}

LTY_VOID CCardClt::SCOnTimer()
{

}


void CCardClt::test_data()
{
	card_cmd_head_t rsp_head;
	//char *send_data = "this is test!";
	DBGOUT("-----start\n");
	do
	{
		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= CARD_CMD_REQ_SHOW_CONTROL; 	/*命令主*/
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/
		

		//memset(send_buf,0,sizeof(send_buf));
		send_buf[0] = 1;
		send_buf[1] = m_test_type;
		send_buf[2] = 1;
		send_buf[3] = 10;
/*
#define CARD_FLASH_SUCCESS		1	刷卡成功
#define CARD_FLASH_FAIL			2	刷卡失败
#define CARD_FLASH_ABNORMAL		3	刷卡异常
#define CARD_FLASH_BALANCE_LOW	4	余额不足

*/
		if(CARD_FLASH_SUCCESS == m_test_type)
		{
			char money[14] = {0};
			char money_total[14] = {0};
			char money_temp[14] = {0};
			char money_total_temp[14] = {0};

			memcpy(money_temp,"3.50",sizeof(money_temp));
			memcpy(money_total_temp,"120.34",sizeof(money_total_temp));

			GetAmount(money_temp,money);
			DBGOUT("%s  %s\n",money_temp,money);
			GetAmount(money_total_temp,money_total);
			DBGOUT("%s  %s\n",money_total_temp,money_total);
			memcpy(&send_buf[1],money,12);

			memcpy(&send_buf[13],money_total,12);	
			rsp_head.u8DataLen 		= 25;			/*数据长度*/
		}
		else if(CARD_FLASH_FAIL == m_test_type)
		{
			char *result_fail = "this is test fail!";
			send_buf[4] = strlen(result_fail);
			memcpy(&send_buf[5],result_fail,strlen(result_fail));

			rsp_head.u8DataLen 		= 5 + strlen(result_fail);		/*数据长度*/
		}
		else if(CARD_FLASH_ABNORMAL == m_test_type)
		{
			char *result_fail = "this is test ABNORMAL!";
			send_buf[4] = strlen(result_fail);
			memcpy(&send_buf[5],result_fail,strlen(result_fail));

			rsp_head.u8DataLen 		= 5 + strlen(result_fail);		/*数据长度*/
		}
		else if(CARD_FLASH_BALANCE_LOW == m_test_type)
		{
			char *result_fail = "this is test LOW!";
			send_buf[4] = strlen(result_fail);
			memcpy(&send_buf[5],result_fail,strlen(result_fail));

			rsp_head.u8DataLen 		= 5 + strlen(result_fail);		/*数据长度*/
		}

		card_cmd_send_rsp(&rsp_head);	
		
	}while(0);
	DBGOUT("-----end\n");
	
}

void CCardClt::card_cmd_send_rsp(card_cmd_head_t *cmd_head)
{
	LTY_U8 sendbuf[128] = {0};
	lty_buf_t temp_buf;
	LTY_RESULT ret;
	
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, cmd_head,4);
	if(cmd_head->u8DataLen > 0)
	{
		memcpy(sendbuf + 4, send_buf,cmd_head->u8DataLen);
	}
	LTY_U8 parity_data[3];		
	BYTECrc16CCITT(&sendbuf[1], sizeof(card_cmd_head_t) + cmd_head->u8DataLen - 1 - 2,parity_data);
	
	//cmd_head->u16Parity = LtyVerifyCrc16(0, &sendbuf[1], sizeof(card_cmd_head_t) + cmd_head->u8DataLen - 1 - 2);
	cmd_head->u16Parity = (parity_data[1] << 8) + parity_data[0];
	
	memcpy(sendbuf + 4 + cmd_head->u8DataLen, &cmd_head->u16Parity,2);

    lty_buf_t buf;
    
    buf.pBuf = &sendbuf;
    buf.u32Size = sizeof(card_cmd_head_t) + cmd_head->u8DataLen;

	ret = SCSend(&buf, 1);
    if ( LTY_SUCCESS != ret )
        ERROUT("%s\n", LtyErrStr(ret));

	struct timeval tv;
    char buf111[64];

    gettimeofday(&tv, NULL);
    strftime(buf111, sizeof(buf111)-1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
    fprintf(stderr, "%s.%03d: CCardClt::card_cmd_send_rsp\n", buf111, (int)(tv.tv_usec / 1000));

    ret = SCSend(&buf, 1);
    if ( LTY_SUCCESS != ret )
        ERROUT("%s\n", LtyErrStr(ret));

	
}

static int GetAmount(char *Amount,char * buffer)
{
	int i;
	int atmlen = 0;
	atmlen = strlen(Amount);
	for(i = atmlen - 2;i <= atmlen;i++)
		Amount[i]=Amount[i+1];
	memset(buffer,'0',12);
	memcpy(buffer,Amount,12);//递交给全局变量
	for(i=0;i<12;i++)
	{
		if(*(buffer+i)==' ')
			*(buffer+i)=0x30;
		else
			break;
	}

	if (atol(buffer)<1)//金额不能为零
	{
		//BEEP_MSG("\n金额输入不能为零",2,1);
		return -1;
	}
}



