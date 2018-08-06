#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include<sys/ioctl.h>
#include<net/if.h>

#include "LtyMsgUserInput.h"
#include "LtyMsgCfg.h"
#include "LtyMsgDataCount.h"
#include "LtyMsgSysCtrl.h"

 #define LTY_DEBUG

#define LTY_DBG_PREFIX "CCardSvr::"
#define LTY_LOG_PREFIX "CCardSvr::"

#include "CardSvr.h"


typedef union
{
              unsigned char longbuf[8];
              unsigned long long i;
} LongLongUnon;

typedef union
{
              unsigned char longbuf[4];
              unsigned int  i;
} LongUnon;
extern global_info_t g_global_info;
typedef union
{
              unsigned char intbuf[2];
              unsigned short i;
} ShortUnon;



static void DealAmount(char *buffer,char *pAmount);

extern void gui_flahs_card_info(void *_param);
extern void gui_system_maintain_get(char _type,char _main_type,char _operate_type,char _percent,const char *_failed_result);;
extern void gui_ic_card_pay_info(void *_param);

CCardSvr::CCardSvr()
{

}

CCardSvr::~CCardSvr()
{
    Release();
}

LTY_RESULT CCardSvr::Init(struct ev_loop *loop)
{
    LTY_RESULT ret;

    unlink(CARD_SOCKET_FILE);

    ret = SSInit(loop, LTY_SOCKET_UNIX, (LTY_STRING)CARD_SOCKET_FILE, 0);
    if(LTY_SUCCESS != ret)
        return ret;

    ret = m_CardWorker.Init(loop);
    if(LTY_SUCCESS != ret)
        return ret;

    ret = SSStart();
    if(LTY_SUCCESS != ret)
        return ret;

    return LTY_SUCCESS;
}

LTY_BOOL CCardSvr::SSOnNewClient(int fd, LTY_U32 u32CltIP, LTY_U16 u16CltPort)
{
    DBGOUT("new connection\n");

    m_CardWorker.Attach(fd);

    return LTY_TRUE;
}

LTY_VOID CCardSvr::SSOnError(LTY_RESULT ret)
{
    DBGOUT("%s\n", LtyErrStr(ret));
}

LTY_VOID CCardSvr::SSOnTimer()
{

}

LTY_VOID CCardSvr::Release()
{
    SSStop();

    m_CardWorker.Release();
}

// CCard Woker start
CCardWorker::CCardWorker()
{
	m_u8NextStationID = 0;//网关需要最小id为2
	m_current_station_id = 0;
	m_u8StInOutFlug = 0;
	memset(&m_passenger_data,0,sizeof(m_passenger_data));
	memset(&m_station_data,0,sizeof(m_station_data));
	memset(&m_current_gps_data,0,sizeof(m_current_gps_data));
    m_stationprepare = LTY_FALSE;
    m_bcardconnect = LTY_FALSE;
}

CCardWorker::~CCardWorker()
{
    //LocalSynthClose(CardSessionId);
}

LTY_RESULT card_msg_handle(LTY_MSG_TYPE msg, LTY_U32 chan, LTY_U32 buf,
                                      LTY_U32 bufsize, LTY_LPVOID lpPriv)
{
	//printf("CCardWorker::Init() card_msg_handle \n");
     return ((CCardWorker*)lpPriv)->MsgHandle(msg, chan, buf, bufsize, lpPriv);
}

LTY_RESULT CCardWorker::Init(struct ev_loop *loop)
{
    LTY_RESULT ret;

    ret = SCInit(loop, 8*1024, 1024*1024, 0);

	LtyMessageRegister(LTY_MSG_FLASH_CARD, card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_GET_CARD_MONEY, card_msg_handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_PASSENGER_FLOW_ALL,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_GPS_INFO,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_NOTIFY_STATION,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_ITSSD_NOTIFY_INFO,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_ITSSD_NOTIFY_PARA,  card_msg_handle, 0, this);

	LtyMessageRegister(LTY_MSG_ITS_NOTIFY_WORKING_STATUS,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_IO_STATUS,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_EXCEPTION_NOTIFY_DOOR_STATUS,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_NOTIFY_OFFLINE,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_EXCEPTION_NOTIFY_OVERSPEED,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_NOTIFY_PARK,  card_msg_handle, 0, this);
	LtyMessageRegister(LTY_MSG_NOTIFY_PARK_STATUS,  card_msg_handle, 0, this);


	//printf("CCardWorker::Init() LTY_MSG_FLASH_CARD %d\n",result);

	if(ret != LTY_SUCCESS)
    {
        printf("Could not initilize the Card workder, reason (%s)\n", LtyErrStr(ret));
        return ret;
    }
	printf("LocalSynthInit OK\n");
	#if 0
	CardBuf[1].pBuf = calloc(1, 1000*1024);
    if( NULL == CardBuf[1].pBuf )
    {
        printf("Could not allocate memory for Card buffer\n");
        return LTY_FAILURE;;
    }

    CardBuf[1].u32Size = 1000*1024;
	#endif


	printf("CCardWorker->Init ok!\n");
    return LTY_SUCCESS;
}

const unsigned short g_awhalfCrc16CCITT[16]={ /* CRC */
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};



void BYTECrc16CCITT(unsigned char* pbyDataIn,unsigned short dwDataLen,unsigned char* abyCrcOut)
{
    unsigned short wCrc = 0;
    unsigned char byTemp;
    while (dwDataLen-- != 0)
    {
        byTemp = ((unsigned char)(wCrc>>8))>>4;
        wCrc <<= 4;
        wCrc ^= g_awhalfCrc16CCITT[byTemp^(*pbyDataIn/16)];
        byTemp = ((unsigned char)(wCrc>>8))>>4;
        wCrc <<= 4;
        wCrc ^= g_awhalfCrc16CCITT[byTemp^(*pbyDataIn&0x0f)];
        pbyDataIn++;
    }
    abyCrcOut[0] = wCrc/256;
    abyCrcOut[1] = wCrc%256;
}

LTY_U32 CCardWorker::SCOnRecv(LTY_U8 *pRcvData, LTY_U32 u32DataLen)
{
    LTY_U8 *pData;
    LTY_U32 uDealLen = 0;    //已处理数据长度
    LTY_U32 uRemainLen;      //未处理数据长度
    //card_cmd_head_t* pHead;
	card_cmd_head_t cmd_head;

//    HEXOUT(pRcvData, u32DataLen);
    struct timeval tv;
    char buf[64];

    gettimeofday(&tv, NULL);
    strftime(buf, sizeof(buf)-1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
    fprintf(stderr, "%s.%03d: CCardWorker::SCOnRecv start\n", buf, (int)(tv.tv_usec / 1000));


#if 0
	DBGOUT("----------------------------------start\n");

	DBGOUT("u32DataLen = %d\n",u32DataLen);
    for(LTY_U32 j = 0; j<u32DataLen; j++)
    {
        printf("0x%x ",pRcvData[j]);
    }
    printf("\n");
#endif
    while(uDealLen < u32DataLen)
    {
        //计算待处理数据偏移
        pData = pRcvData + uDealLen;
        //计算待处理数据长度
        uRemainLen = u32DataLen - uDealLen;
        //如果待处理数据长度小于包头长度，则表示数据未接收完整，返回继续等待
		
		if(uRemainLen < 4/*sizeof(card_cmd_head_t)*/)
            break;
		
		cmd_head.u8Flag 		= pData[0];    	/*包头标识*/
		cmd_head.u8CmdMaster 	= pData[1]; 	/*命令主*/
		cmd_head.u8CmdSlave 	= pData[2];		/*命令从*/
		cmd_head.u8DataLen 		= pData[3];		/*数据长度*/		

		if(!check_command_head(&cmd_head))
		{
			printf("%s pData[0]:%d,pData[1]:%d,pData[2]:%d,pData[3]:%d\n",__func__,pData[0],pData[1],pData[2],pData[3]);
			break;
		}
		//如数据包未接收完整，返回继续等待
		if (uRemainLen < cmd_head.u8DataLen + sizeof(card_cmd_head_t))
        {	
        	printf("%s uRemainLen:%d, u8DataLen(card_cmd_head_t):%d\n",__func__,uRemainLen,cmd_head.u8DataLen + sizeof(card_cmd_head_t));
        	break;
		}

		memset(recv_buf,0,SOCKET_BUF_SIZE);
		if(cmd_head.u8DataLen > 0)
		{
			memcpy(recv_buf, &pData[4], cmd_head.u8DataLen);	
		}

		memcpy(&cmd_head.u16Parity, &pData[4 + cmd_head.u8DataLen], 2);

		for(int index = 0;index < (int)sizeof(card_cmd_head_t) + cmd_head.u8DataLen - 1 - 2;index++)
			printf("%x ",pData[1 + index]);
			
		printf("\n");
		
		//LTY_U16 new_parity = LtyVerifyCrc16(0, &pData[1], sizeof(card_cmd_head_t) + cmd_head.u8DataLen - 1 - 2);
		LTY_U16 new_parity = 0;
		LTY_U8 parity_data[3];
		BYTECrc16CCITT(&pData[1], sizeof(card_cmd_head_t) + cmd_head.u8DataLen - 1 - 2,parity_data);
		printf("%s  new_parity = %x %x\n",__func__,parity_data[0],parity_data[1]);
		new_parity = (parity_data[1] << 8) + parity_data[0];
		if (cmd_head.u16Parity !=new_parity )
        {
            //uDealLen += 4;
            uDealLen += (cmd_head.u8DataLen + sizeof(card_cmd_head_t));
			printf("%s  u16Parity error\n",__func__);
            continue;
        }
		//printf("SCOnRecv() cmd_head.u8CmdMaster:%d \n",cmd_head.u8CmdMaster);
		switch(cmd_head.u8CmdMaster)
		{
			/*2.显示控制命令--------------------------------*/
			case CARD_CMD_REQ_SHOW_CONTROL:
			{
				card_cmd_req_show_control(&cmd_head);
			}break;
			/*3.语音控制命令--------------------------------*/
			case CARD_CMD_REQ_VOICE_CONTROL:
			{
				card_cmd_req_voice_control(&cmd_head);		
			}break;
			/*4.参数导入导出控制命令------------------------*/
			case CARD_CMD_RSP_PARAM_CONTROL:
			{
				card_cmd_rsp_param_control(&cmd_head);
			}break;
			case CARD_CMD_RSP_GET_MONEY:
			{
				card_cmd_rsp_get_money(&cmd_head);
			}break;
			case CARD_CMD_REQ_STATION_INFO:
			{
				card_cmd_rsp_station_info(&cmd_head);
			}break;
			case CARD_CMD_REQ_TIME_ZONE:
			{
				card_cmd_req_time_zone(&cmd_head);
			}break;
			case CARD_CMD_RSP_PASSENGER_FLOW:
			{
				card_cmd_rsp_passenger_flow(&cmd_head);
			}break;
			case CARD_CMD_RSP_PARAM_CHANGE:
			{
				card_cmd_rsp_param_change(&cmd_head);
			}break;
			case CARD_CMD_REQ_HEARTBEAT:
			{
				card_cmd_req_heartbeat(&cmd_head);
			}break;
			case CARD_CMD_REQ_TBP_STATUS:
			{
				card_cmd_req_tbp_status(&cmd_head);
			}break;
			case CARD_CMD_RSP_STATION_IN_OUT:
			{
				card_cmd_rsp_sation_in_out(&cmd_head);
			}break;
			case CARD_CMD_REQ_ACC_NOTIFY:
			{
				card_cmd_acc_notify(&cmd_head);
			}break;

		}

        uDealLen += (cmd_head.u8DataLen + sizeof(card_cmd_head_t));
    }

    DBGOUT("Dealed data length %d\n", uDealLen);

	DBGOUT("----------------------------------end\n");

    return uDealLen;
}
#define ARRAY_LENGTH  290
void CCardWorker::card_cmd_req_show_control(card_cmd_head_t *cmd_head)
{
	card_cmd_head_t rsp_head;
	char req_result = 1;
	printf("card_cmd_req_show_control() ------------------ start\n");
	flash_card_result_t card_result;
	memset(&card_result,0,sizeof(card_result));
	do
	{
		int index = 0;
		char content_length = 0;
		int times_count = 0;
		char temp_content[ARRAY_LENGTH + 1] = {0};
		int read_length = 0;

		unsigned char show_id = 0;
		unsigned char show_time = 0;
		char show_content[ARRAY_LENGTH + 1] = {0};
		char message_tip_type = 0;
		while(1)
		{
			//获取长度
			content_length = recv_buf[read_length];
			
			memset(temp_content,0,sizeof(temp_content));

			memcpy(temp_content,&recv_buf[read_length + 1],content_length);

			
			if(0 == times_count)
				show_id = (unsigned char)temp_content[0];
			else if(1 == times_count)
				show_time = (unsigned char)temp_content[0];
			else if(2 == times_count)
				memcpy(show_content,temp_content,strlen(temp_content));
			else if(3 == times_count)
			{
				if(CARD_FLASH_MESSAGE == show_id)
				{
					message_tip_type = (unsigned char)temp_content[0];
				}
			}
			times_count++;
			read_length += (content_length + 1);
			if(read_length >= cmd_head->u8DataLen)
			{
				break;
			}
			
		}
		
		card_result.show_time = show_time;

		printf("card_cmd_req_show_control() show_id:%d,show_time:%d,show_content:%s\n",show_id,show_time,show_content);

		
		if(CARD_FLASH_SUCCESS == show_id/*recv_buf[0]*/)
		{/*刷卡成功*/ 
			char money[14] = {0};
			char money_total[14] = {0};
			char real_money[14] = {0};
			char real_money_total[14] = {0};
#if 0
			memcpy(money,show_content,12);
			memcpy(money_total,&show_content[13],12);
#endif
			
			LTY_STRING list_conent[50];
			int u32Cnt = 0;

			u32Cnt = LtyStrSpliteAttr((LTY_STRING)show_content, 0, '|', list_conent, 50);
			printf("card_cmd_req_show_control() u32Cnt:%d\n",u32Cnt);

			for(int index = 0;index < u32Cnt;index++)
			{
				printf("%s,",list_conent[index]);
			}

			printf("\n");
			
			if(u32Cnt >= 2)
			{
				DealAmount(list_conent[0],real_money);
				DealAmount(list_conent[1],real_money_total);
			}
			if(u32Cnt >= 4)
			{
				card_result.card_type = list_conent[2][0] - 48;
				int copy_number = 0;
				if(strlen(list_conent[3]) > 100)
					copy_number = 99;
				memcpy(card_result.card_number,list_conent[3],copy_number);
			}
			

			DBGOUT("money:%s  \n",money);
			DBGOUT("money_total:%s  \n",money_total);
			
			DBGOUT("real_money:%s  \n",real_money);
			DBGOUT("real_money_total:%s  \n",real_money_total);

			card_result.u8Type = CARD_FLASH_SUCCESS;
			memcpy(card_result.money,real_money,strlen(real_money));
			memcpy(card_result.money_total,real_money_total,strlen(real_money_total));

			gui_flahs_card_info(&card_result);

			

		}
		else if(CARD_FLASH_FAIL == show_id/*recv_buf[0]*/)
		{/*刷卡失败*/
			card_result.u8Type = CARD_FLASH_FAIL;
			//if(cmd_head->u8DataLen > 1)
				memcpy(card_result.fail_result,show_content,strlen(show_content));
			gui_flahs_card_info(&card_result);
		}
		else if(CARD_FLASH_ABNORMAL == show_id/*recv_buf[0]*/)	
		{/*刷卡异常*/
			card_result.u8Type = CARD_FLASH_ABNORMAL;
			//if(cmd_head->u8DataLen > 1)
				memcpy(card_result.fail_result,show_content,strlen(show_content));
			gui_flahs_card_info(&card_result);
		}
		else if(CARD_FLASH_BALANCE_LOW == show_id/*recv_buf[0]*/)
		{/*余额不足*/
			card_result.u8Type = CARD_FLASH_BALANCE_LOW;
			//if(cmd_head->u8DataLen > 1)
				memcpy(card_result.fail_result,show_content,strlen(show_content));
			gui_flahs_card_info(&card_result);
		}
		else if(CARD_FLASH_MESSAGE == show_id/*recv_buf[0]*/)
		{
			card_result.u8Type = CARD_FLASH_MESSAGE;
			//if(cmd_head->u8DataLen > 1)
				memcpy(card_result.fail_result,show_content,strlen(show_content));
			card_result.message_tip_type = message_tip_type;
			gui_flahs_card_info(&card_result);
		}	
		else if(QR_SCAN_SUCCESS == show_id)
		{/*扫码成功*/
			card_result.u8Type = QR_SCAN_SUCCESS;
			//memcpy(card_result.money,show_content,strlen(show_content));
			DealAmount(show_content,card_result.money);
			gui_flahs_card_info(&card_result);
		}
		else if(QR_SCAN_FAILED == show_id)
		{/*验码失败*/
			card_result.u8Type = QR_SCAN_FAILED;

			memcpy(card_result.fail_result,show_content,strlen(show_content));
			gui_flahs_card_info(&card_result);
		}
		else if(QR_SCAN_ABNORMAL == show_id)
		{/*扫码异常*/
			card_result.u8Type = QR_SCAN_ABNORMAL;

			memcpy(card_result.fail_result,show_content,strlen(show_content));
			gui_flahs_card_info(&card_result);
		}
		else
		{
			req_result = 0;
		}
		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= CARD_CMD_RSP_SHOW_CONTROL; 	/*命令主*/
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/
		rsp_head.u8DataLen 		= 1;			/*数据长度*/

		send_buf[0] = req_result;
		
		card_cmd_send_rsp(&rsp_head);
		
	
	}while(0);

	printf("card_cmd_req_show_control() -------------------- end\n");
	
}
void CCardWorker::card_cmd_req_voice_control(card_cmd_head_t *cmd_head) 
{
	card_cmd_head_t rsp_head;

	DBGOUT("-------------------------- start\n");
	do
	{
		if(1 == cmd_head->u8DataLen)
		{
			recv_buf[0];
		}

		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= CARD_CMD_RSP_VOICE_CONTROL; 	/*命令主*/
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/
		rsp_head.u8DataLen 		= 1;			/*数据长度*/

		send_buf[0] = 1;
		
		card_cmd_send_rsp(&rsp_head);

		//int play_id = 0;
		//play_id = strtol((const char *)recv_buf[0], NULL, 16);

		LtyMessageSend(LTY_MSG_APLAY_DATA, recv_buf[0], 0, 0);
		
	
	}while(0);
	DBGOUT("-------------------------- end\n");
}
void CCardWorker::card_cmd_rsp_param_control(card_cmd_head_t *cmd_head)
{
	DBGOUT("-------------------------- start\n");
	printf("CCardWorker::card_cmd_rsp_param_control start\n");
	do
	{
		LTY_CHAR message[1024] = {0};

		if(cmd_head->u8DataLen > 1)
			memcpy(message,&recv_buf[1],cmd_head->u8DataLen - 1);

		printf("CCardWorker::card_cmd_rsp_param_control 1 recv_buf[0]:%d\n",recv_buf[0]);
		printf("CCardWorker::card_cmd_rsp_param_control 1 cmd_head->u8CmdSlave:%d\n",cmd_head->u8CmdSlave);
		if(CARD_CMD_REQ_SLAVE_PARAM_IMPORT == cmd_head->u8CmdSlave)
		{

			if(1 == recv_buf[0])
				gui_system_maintain_get(2,21,3,0,NULL);
			else
				gui_system_maintain_get(2,21,4,0,message);			
		}
		else if(CARD_CMD_REQ_SLAVE_PARAM_EXPORT == cmd_head->u8CmdSlave)
		{
			if(1 == recv_buf[0])
				gui_system_maintain_get(2,22,3,0,NULL);
			else
				gui_system_maintain_get(2,22,4,0,message);	
		}
		else if(CARD_CMD_REQ_SLAVE_PARAM_CLEAN == cmd_head->u8CmdSlave)
		{
			if(1 == recv_buf[0])
				gui_system_maintain_get(2,23,3,0,NULL);
			else
				gui_system_maintain_get(2,23,4,0,message);	
		}
		else if(CARD_CMD_REQ_SLAVE_RECORD_CLEAN == cmd_head->u8CmdSlave)
		{
			if(1 == recv_buf[0])
				gui_system_maintain_get(2,24,3,0,NULL);
			else
				gui_system_maintain_get(2,24,4,0,message);	
		}
		
	}while(0);
	printf("CCardWorker::card_cmd_rsp_param_control end\n");
	DBGOUT("--------------------------- end\n");
}

void CCardWorker::card_cmd_rsp_get_money(card_cmd_head_t *cmd_head)
{
	flash_card_result_t card_result;
	
	printf("card_cmd_rsp_get_money() start\n");
	do
	{
		if(0x01 == recv_buf[0])
		{/*刷卡成功*/ 
			memset(&card_result,0,sizeof(flash_card_result_t));

			if(cmd_head->u8DataLen < 25)
				break;
			
			//memcpy(money,&recv_buf[1],12);
			//memcpy(money_total,&recv_buf[14],12);
			LTY_STRING list_conent[50];
			int u32Cnt = 0;

			u32Cnt = LtyStrSpliteAttr((LTY_STRING)&recv_buf[1], 0, '|', list_conent, 50);
			printf("card_cmd_req_show_control() u32Cnt:%d\n",u32Cnt);

			for(int index = 0;index < u32Cnt;index++)
			{
				printf("%s,",list_conent[index]);
			}

			printf("\n");
			
			if(u32Cnt >= 2)
			{
				LTY_CHAR money[15] = {0};
				LTY_CHAR money_total[15] = {0};
				LTY_CHAR money_qr[15] = {0};
				LTY_CHAR money_total_qr[15] = {0};
				
				DealAmount(list_conent[0],money);
				DealAmount(list_conent[1],money_total);

				memcpy(card_result.money,money,strlen(money));
				memcpy(card_result.money_total,money_total,strlen(money_total));

				if(u32Cnt >= 4)
				{
					DealAmount(list_conent[2],money_qr);
					DealAmount(list_conent[3],money_total_qr);

					memcpy(card_result.money_qr,money_qr,strlen(money_qr));
					memcpy(card_result.money_total_qr,money_total_qr,strlen(money_total_qr));
				}
				if(cmd_head->u8DataLen > 52)
				{
					//LTY_U16 ic_number;
					//LTY_U16 qr_number;
					ShortUnon get_number;

					memcpy(get_number.intbuf,&recv_buf[53],2);
					card_result.ic_number = get_number.i;
					
					memcpy(get_number.intbuf,&recv_buf[56],2);
					card_result.qr_number = get_number.i;
				}
				printf("ic_number:%d,qr_number%d\n",card_result.ic_number,card_result.qr_number);
				
			}
			
#if 0		
			DealAmount(money,real_money);
			DealAmount(money_total,real_money_total);

			DBGOUT("card_cmd_rsp_get_money() money:%s  \n",money);
			DBGOUT("card_cmd_rsp_get_money() money_total:%s  \n",money_total);
			
			DBGOUT("card_cmd_rsp_get_money() real_money:%s  \n",real_money);
			DBGOUT("card_cmd_rsp_get_money() real_money_total:%s  \n",real_money_total);

			memcpy(card_result.money,real_money,sizeof(real_money));
			memcpy(card_result.money_total,real_money_total,sizeof(real_money_total));
#endif

			gui_ic_card_pay_info(&card_result);
		}
		
	}while(0);
	printf("card_cmd_rsp_get_money() end\n");
}

extern int g_qt_direction;
extern LTY_U32 g_u32BusStationId;


void CCardWorker::card_cmd_rsp_station_info(card_cmd_head_t *cmd_head)
{
	printf("LtyMessageSend() card_cmd_rsp_station_info start\n");
	
	do
	{
		card_cmd_head_t rsp_head;
		LTY_U32 u32LineNo = 0;
		LongUnon temp_info;
		ShortUnon short_info;
		bus_plate_set_t stBusPlate;
		
		temp_info.i = g_u32BusStationId;
			
		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= CARD_CMD_RSP_STATION_INFO; 	/*命令主*/	
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/
                                                                  
		rsp_head.u8DataLen 		= 1 + 11 + 1 + 12 + 1 + 1 + 1 + 4 + 1 + sizeof(m_current_gps_data);			/*数据长度*/
		memset(send_buf,0,sizeof(send_buf));
		send_buf[0] = 11;
		send_buf[1] = g_global_info.direction;/*g_qt_direction*/;/*  0：上行;1：下行*/
		send_buf[2] = '|';
		memcpy(&send_buf[3],temp_info.longbuf,4);  /*站点序号*/
		send_buf[7] = '|';
		printf("g_global_info.u32GPRSLineNo:%d\n",g_global_info.u32GPRSLineNo);
		temp_info.i = g_global_info.u32GPRSLineNo;
		memcpy(&send_buf[8],temp_info.longbuf,4);  /*线路号*/
		
		send_buf[12] = 12;
		temp_info.i = g_global_info.u32Longitude;
		memcpy(&send_buf[13],temp_info.longbuf,4);  /*经度*/
		send_buf[17] = '|';
		temp_info.i = g_global_info.u32Latitude;
		memcpy(&send_buf[18],temp_info.longbuf,4);  /*纬度*/
		send_buf[22] = '|';
		short_info.i = g_global_info.u16Angle;
		memcpy(&send_buf[23],short_info.intbuf,2);  /*方位角*/
		
		cfg_para_dev_inf_t stCfg_DevInfo;
		memset(&stCfg_DevInfo,0,sizeof(cfg_para_dev_inf_t));
		int ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DEV_INF, (LTY_U32)&stCfg_DevInfo, sizeof(stCfg_DevInfo));
		printf("stCfg_DevInfo.u8LineAttr::%d \n", stCfg_DevInfo.u8LineAttr);

		send_buf[25] = 1;
		send_buf[26] = stCfg_DevInfo.u8LineAttr;  /*0:普通 1:专线*/

		memset(&stBusPlate,0,sizeof(stBusPlate));
		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_BUS_PLATE, (LTY_U32)&stBusPlate, sizeof(stBusPlate));
        if(LTY_SUCCESS != ret)
        {
            DBGPOUT("Get SD Palate Para Fail !!!!\n");
        }
		
		LTY_U32 device_id = atoi(stBusPlate.charBusNoBuf);
		printf("stBusPlate.charBusNoBuf:%s  device_id:%d\n",stBusPlate.charBusNoBuf,device_id);
		send_buf[27] = 4;
		temp_info.i = device_id;
		memcpy(&send_buf[28],temp_info.longbuf,4); /*车辆编号*/
		send_buf[32] = sizeof(m_current_gps_data);

        LongLongUnon SCAN_TIME;
        time_t tScanTime = 0;
        time(&tScanTime);
        SCAN_TIME.i = tScanTime - 8*60*60;
        memcpy(&m_current_gps_data, SCAN_TIME.longbuf, 8);
        memcpy(&send_buf[33],&m_current_gps_data,sizeof(m_current_gps_data)); //GPS DATA
		
		card_cmd_send_rsp(&rsp_head);
		
	
	}while(0);

	printf("LtyMessageSend() card_cmd_rsp_station_info end\n");
	
}


void CCardWorker::card_cmd_req_time_zone(card_cmd_head_t *cmd_head)
{
	printf("card_cmd_req_time_zone() start 22222\n");
	
	do
	{
		card_cmd_head_t rsp_head;
			
		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= 0xc1;/*CARD_CMD_RSP_TIME_ZONE*/; 	/*命令主*/	
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/

		rsp_head.u8DataLen 		= 1;			/*数据长度*/
		memset(send_buf,0,sizeof(send_buf));

		cfg_para_dev_inf_t stdevinfo;
		int ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DEV_INF, (LTY_U32)&stdevinfo, sizeof(cfg_para_dev_inf_t));
        if (0 != ret){
			printf("card_cmd_req_time_zone() CFG_PARA_DEV_INF error\n");
        }
		memset(&send_buf,0,sizeof(send_buf));
		send_buf[0] = stdevinfo.u8eartharea;

		
		card_cmd_send_rsp(&rsp_head);
		
	
	}while(0);

	printf("card_cmd_req_time_zone end\n");

}

void CCardWorker::card_cmd_req_passenger_flow()
{
	printf("card_cmd_req_passenger_flow() start\n");
	
	do
	{
		card_cmd_head_t rsp_head;
			
		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= CARD_CMD_REQ_PASSENGER_FLOW; 	/*命令主*/	
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/

		rsp_head.u8DataLen 		= sizeof(m_passenger_data);			/*数据长度*/
		memset(send_buf,0,sizeof(send_buf));

		memset(&send_buf,0,sizeof(send_buf));

		memcpy(&send_buf,&m_passenger_data,sizeof(m_passenger_data));
		
		card_cmd_send_rsp(&rsp_head);
		
	
	}while(0);

	printf("card_cmd_req_passenger_flow() end\n");
}

void CCardWorker::card_cmd_rsp_passenger_flow(card_cmd_head_t *cmd_head)
{
	do
	{
	
		printf("card_cmd_rsp_passenger_flow respone!\n");
		
	
	}while(0);
}

void CCardWorker::card_cmd_req_sation_in_out()
{
	printf("card_cmd_req_sation_in_out() start\n");
	
	do
	{
		card_cmd_head_t rsp_head;
			
		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= CARD_CMD_REQ_STATION_IN_OUT; 	/*命令主*/	
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/

		rsp_head.u8DataLen 		= sizeof(lty_station_data_t);			/*数据长度*/
		memset(send_buf,0,sizeof(send_buf));

		memset(&send_buf,0,sizeof(send_buf));
	

		memcpy(&send_buf,&m_station_data,sizeof(lty_station_data_t));
		
		card_cmd_send_rsp(&rsp_head);
		
	
	}while(0);

	printf("card_cmd_req_sation_in_out() end\n");
}

void CCardWorker::card_cmd_rsp_sation_in_out(card_cmd_head_t *cmd_head)
{
 	do
	{
	
		printf("card_cmd_rsp_sation_info respone!\n");
		
	
	}while(0);
}

void CCardWorker::card_cmd_acc_notify(card_cmd_head_t *cmd_head)
{
	do
	{
	
		printf("card_cmd_acc_notify respone!\n");
		
		LtyMessageSend(LTY_MSG_SYSCTRL_NOTIFY_SYS_SHUTDOWN, 0, SC_SD_ACC, 0);
		
	
	}while(0);
}


void CCardWorker::card_cmd_rsp_param_change(card_cmd_head_t *cmd_head)
{
	do
	{
	
		printf("card_cmd_rsp_param_change\n");
		
	
	}while(0);

}
extern void gui_system_heartbeat_notify();
void CCardWorker::card_cmd_req_heartbeat(card_cmd_head_t *cmd_head)
{
	printf("card_cmd_req_heartbeat() start\n");
	
	do
	{
		card_cmd_head_t rsp_head;
			
		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= CARD_CMD_RSP_HEARTBEAT; 		/*命令主*/	
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/

		rsp_head.u8DataLen 		= 0;			/*数据长度*/

		card_cmd_send_rsp(&rsp_head);

		gui_system_heartbeat_notify();
		
	
	}while(0);

	printf("card_cmd_req_heartbeat() end\n");

}

extern void gui_system_tbp_connect_status(char _status,char _type);
void CCardWorker::card_cmd_req_tbp_status(card_cmd_head_t *cmd_head)
{
	printf("card_cmd_req_tbp_status() start\n");
	
	do
	{
		if(cmd_head->u8DataLen > 0)
		{
			printf("card_cmd_req_tbp_status() value:%d\n",recv_buf[0]);
			gui_system_tbp_connect_status(recv_buf[0],cmd_head->u8CmdSlave);	
		}
		

		card_cmd_head_t rsp_head;
			
		rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
		rsp_head.u8CmdMaster 	= CARD_CMD_RSP_TBP_STATUS; 		/*命令主*/	
		rsp_head.u8CmdSlave 	= cmd_head->u8CmdSlave;//CARD_CMD_REQ_SLAVE;			/*命令从*/

		rsp_head.u8DataLen 		= 0;			/*数据长度*/
		
		card_cmd_send_rsp(&rsp_head);

		
	
	}while(0);

	printf("card_cmd_req_tbp_status() end\n");

}

void CCardWorker::card_cmd_send_rsp(card_cmd_head_t *cmd_head)
{
	LTY_U8 sendbuf[128] = {0};
	lty_buf_t temp_buf;
	LTY_RESULT ret;
	static LTY_U64 send_count = 0;
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, cmd_head,4);
	if(cmd_head->u8DataLen > 0)
	{
		memcpy(sendbuf + 4, send_buf,cmd_head->u8DataLen);
	}
	LTY_U16 new_parity = 0;
	LTY_U8 parity_data[3];
	BYTECrc16CCITT(&sendbuf[1], sizeof(card_cmd_head_t) + cmd_head->u8DataLen - 1 - 2,parity_data);
	printf("%s  new_parity = %x %x\n",__func__,parity_data[0],parity_data[1]);
	cmd_head->u16Parity = (parity_data[1] << 8) + parity_data[0];
	//cmd_head->u16Parity = LtyVerifyCrc16(0, &sendbuf[1], sizeof(card_cmd_head_t) + cmd_head->u8DataLen - 1 - 2);
	
	memcpy(sendbuf + 4 + cmd_head->u8DataLen, &cmd_head->u16Parity,2);

    lty_buf_t buf;
    
    buf.pBuf = &sendbuf;
    buf.u32Size = sizeof(card_cmd_head_t) + cmd_head->u8DataLen;


	DBGOUT("u32DataLen = %d\n",buf.u32Size);
    for(LTY_U32 j = 0; j<buf.u32Size; j++)
    {
        printf("0x%x ",sendbuf[j]);
    }
    printf("\n");

    ret = SCSend(&buf, 1);
    if ( LTY_SUCCESS != ret )
        ERROUT("%s\n", LtyErrStr(ret));

	struct timeval tv;
    char buf111[64];

    gettimeofday(&tv, NULL);
    strftime(buf111, sizeof(buf111)-1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
	send_count++;
    fprintf(stderr, "%s.%03d: CCardWorker::card_cmd_send_rsp send_count=%lld\n", buf111, (int)(tv.tv_usec / 1000),send_count);
	
}

void CCardWorker::request_import_export(char _type)
{
	card_cmd_head_t rsp_head;
	printf("LtyMessageSend() request_import_export start _type:%d\n",_type);
	DBGOUT("-------------------------- start\n");





	rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
	rsp_head.u8CmdMaster 	= CARD_CMD_REQ_PARAM_CONTROL; 	/*命令主*/	
	rsp_head.u8DataLen 		= 0;			/*数据长度*/
	if(1 == _type)
	{
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE_PARAM_IMPORT;			/*命令从*/
	}
	else if(2 == _type)
	{
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE_PARAM_EXPORT;			/*命令从*/
	}
	else if(3 == _type)
	{
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE_PARAM_CLEAN;			/*命令从*/
	}
	else if(4 == _type)
	{
		rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE_RECORD_CLEAN;			/*命令从*/
	}
	

	card_cmd_send_rsp(&rsp_head);
	
	DBGOUT("-------------------------- end\n");
	
}

void CCardWorker::request_get_money()
{
	card_cmd_head_t rsp_head;
	printf("LtyMessageSend() request_import_export start\n");
	DBGOUT("-------------------------- start\n");


	rsp_head.u8Flag 		= CARD_CMD_REQ_FLAG;    		/*包头标识*/
	rsp_head.u8CmdMaster 	= CARD_CMD_REQ_GET_MONEY; 	/*命令主*/	
	rsp_head.u8DataLen 		= 0;			/*数据长度*/

	rsp_head.u8CmdSlave 	= CARD_CMD_REQ_SLAVE;			/*命令从*/

	card_cmd_send_rsp(&rsp_head);
	
	DBGOUT("-------------------------- end\n");
	
}



LTY_BOOL CCardWorker::check_command_head(card_cmd_head_t *pcmd_head)
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
			case CARD_CMD_REQ_GET_MONEY:
			case CARD_CMD_RSP_GET_MONEY:
			case CARD_CMD_REQ_STATION_INFO:
			case CARD_CMD_RSP_STATION_INFO:
			/*7.获取时区命令-------------------------------- */
			case CARD_CMD_REQ_TIME_ZONE:
			case CARD_CMD_RSP_TIME_ZONE:
			/*8.获取客流命令-------------------------------- */
			case CARD_CMD_REQ_PASSENGER_FLOW:
			case CARD_CMD_RSP_PASSENGER_FLOW:
			/*9.通知参数命令-------------------------------- */
			case CARD_CMD_REQ_PARAM_CHANGE:
			case CARD_CMD_RSP_PARAM_CHANGE:
			/*10.心跳包命令-------------------------------- */
			case CARD_CMD_REQ_HEARTBEAT:
			case CARD_CMD_RSP_HEARTBEAT:
			/*进出站命令---------------------------------------*/
			case CARD_CMD_REQ_STATION_IN_OUT:
			case CARD_CMD_RSP_STATION_IN_OUT:
			/*13.ACC关机通知命令---------------------------------------*/	
			case CARD_CMD_REQ_ACC_NOTIFY:
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
				   CARD_CMD_REQ_SLAVE_PARAM_EXPORT == pcmd_head->u8CmdSlave ||
				   CARD_CMD_REQ_SLAVE_PARAM_CLEAN == pcmd_head->u8CmdSlave ||
				   CARD_CMD_REQ_SLAVE_RECORD_CLEAN == pcmd_head->u8CmdSlave)
				{
					ret_value = LTY_TRUE;
				}
				
			}break;
			/*11.TBP网络状态命令-------------------------------- */
			case CARD_CMD_REQ_TBP_STATUS:
			case CARD_CMD_RSP_TBP_STATUS:
			{
				if(0x01 == pcmd_head->u8CmdSlave || 
				   0x02 == pcmd_head->u8CmdSlave)
				{
					ret_value = LTY_TRUE;
				}
			}break;
		}
		
	}while(0);

	return ret_value;
	
}


LTY_VOID CCardWorker::SCOnError(LTY_RESULT ret)
{
    DBGOUT("%s\n", LtyErrStr(ret));
    m_bcardconnect = LTY_FALSE;
}

LTY_VOID CCardWorker::SCOnTimer()
{

}

LTY_RESULT CCardWorker::Attach(int fd)
{
    LTY_RESULT ret;
    ret = SCAttach(fd);
    if(LTY_SUCCESS != ret){
        WAROUT("Attach Fail \n");
    }
    else{
        LOGOUT("Card work connect success\n");
    }
    m_bcardconnect = LTY_TRUE;
    if (LTY_TRUE == m_stationprepare){
        card_cmd_req_sation_in_out();
    }
    
    return ret;
}

LTY_VOID CCardWorker::Release()
{
    SCClose();

    if(CardBuf[1].pBuf)
        free(CardBuf[1].pBuf);
}
#if 0
LTY_RESULT CCardWorker::TextConvert(LTY_CHAR *srcEnc, LTY_CHAR *srcBuf, LTY_U16 srcBufLen, LTY_CHAR *destEnc, LTY_CHAR *destBuf)
{
#ifdef STR_DBG_OUT
    DBGOUT("Input: (%s)(length %d)\n", srcBuf, srcBufLen);
#endif

    if( LTY_SUCCESS == TextEncoding(destEnc, srcEnc, srcBuf, srcBufLen, destBuf, TEXT_OUT_LEN) )
    {
//        DBGOUT("Output: (%s)\n", destBuf);
    }
    else
    {
        ERROUT("Could not convert the local encoding text to specified encoding\n");
        return LTY_FAILURE;
    }

    return LTY_SUCCESS;
}

LTY_RESULT CCardWorker::OnCmdReqVideo(card_cmd_head_t* pHead, LTY_STRING pStr, LTY_U32 u32Len)
{
    LTY_CHAR textUTF8[1024];
    card_cmd_head_t stHead;
    LTY_RESULT ret;

    memset(textUTF8, 0, TEXT_OUT_LEN);

    if( LTY_SUCCESS != TextConvert((LTY_CHAR*)LTY_LOCAL_ENC, pStr, u32Len, (LTY_CHAR*)"UTF-8", textUTF8) )
    {
        ERROUT("Could not convert text to UTF-8 text\n");
        ret = ETEXTENC;
        goto QUIT;
    }

    if( LTY_FALSE == LocalSynth(ttsSessionId, (char *)textUTF8, &ttsBuf[1]) )
    {
        ERROUT("Could not generate TTS data\n");
        ret = ETTSGEN;
        goto QUIT;
    }

    DBGOUT("TTS data ready\n");

    stHead.u32Flag = MKFLAG('L', 'T', 'Y', 'T');
    stHead.u16CmdType = TTS_CMD_RSP_AUDIO;
    stHead.u32DataLen = ttsBuf[1].u32Size;
    stHead.u32PrivData1 = pHead->u32PrivData1;
    stHead.u32PrivData2 = pHead->u32PrivData2;
    stHead.u16Parity = LtyVerifyCrc16(0, &stHead.u16CmdType, sizeof(card_cmd_head_t)-6);

    ttsBuf[0].pBuf = &stHead;
    ttsBuf[0].u32Size = sizeof(card_cmd_head_t);

    ret = SCSend(ttsBuf, 2, 1);
    if ( LTY_SUCCESS != ret )
        ERROUT("%s\n", LtyErrStr(ret));

QUIT:
    lty_buf_t buf[2];

    stHead.u32Flag = MKFLAG('L', 'T', 'Y', 'T');
    stHead.u16CmdType = TTS_CMD_RSP_DONE;
    stHead.u32DataLen = sizeof(ret);
    stHead.u32PrivData1 = pHead->u32PrivData1;
    stHead.u32PrivData2 = pHead->u32PrivData2;
    stHead.u16Parity = LtyVerifyCrc16(0, &stHead.u16CmdType, sizeof(card_cmd_head_t)-6);

    buf[0].pBuf = &stHead;
    buf[0].u32Size = sizeof(card_cmd_head_t);
    buf[1].pBuf = &ret;
    buf[1].u32Size = sizeof(ret);

    SCSend(buf, 2);

    return LTY_SUCCESS;
}
#endif

static void* start_card_server_thread(void* pArg)
{
	pthread_detach( pthread_self());

    CCardSvr *cCardSvr;
    LTY_RESULT ret;
    struct ev_loop *loop;

#if 0
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        ERROUT("Fork failed\n");
        exit(errno);
    }
    else
    {
        if( pid > 0 )
        {
            exit(0);
        }
    }
#endif
	printf("Start cCardSvr!\n");
    cCardSvr = new CCardSvr;
    if (NULL == cCardSvr){
        ERROUT("Failed to allocate mem for CCardSvr\n");
        return NULL;
    }
	
    loop = ev_default_loop(EVBACKEND_EPOLL);
    ev_set_io_collect_interval(loop, 0.005);
    ev_set_timeout_collect_interval(loop, 0.1);
	printf("cCardSvr Init!\n");
    ret = cCardSvr->Init(loop);
    if(LTY_SUCCESS != ret)
    {
        ERROUT("%s\n", LtyErrStr(ret));
        return NULL;
    }

    printf("==CardSvr v1.0 start==================\n");

    ev_run(loop, 0);

    cCardSvr->Release();
    delete cCardSvr;

    return NULL;
}


void start_card_server()
{
	pthread_t th;

	pthread_create(&th, NULL, start_card_server_thread, (void*)NULL);
	
}

static void DealAmount(char *buffer,char *pAmount)
{
	int i,j;
	int flag=0;
	memset(pAmount,0,14);
    
	for(i=0,j=0;i<12;i++)
	{

      	switch(buffer[i])
		{
		case  '0':
		   if(flag!=0||i==9)//'0'不在首位
			{pAmount[j]=buffer[i];flag=1;j++;}
			break;
		default:
			flag=1;
			pAmount[j]=buffer[i];
			j++;
		}

		if(i==9)
		{
			pAmount[j]='.';
			j++;
		}
	}

	return;
}
 



LTY_RESULT CCardWorker::MsgHandle(LTY_MSG_TYPE msg, LTY_U32 chan, LTY_U32 buf,
              LTY_U32 bufsize, LTY_LPVOID lpPriv)
{
	switch(msg)
	{
		case LTY_MSG_FLASH_CARD:
		{
			request_import_export(chan);
		}break;
		case LTY_MSG_GET_CARD_MONEY:
		{
			request_get_money();
		}break;
		case LTY_MSG_ITSIO_NOTIFY_PASSENGER_FLOW_ALL:
		{
			OnMsgPassenger((itsio_passenger_flow_t *)buf);
		}break;
		case LTY_MSG_ITSIO_NOTIFY_GPS_INFO:
        	OnMsgGpsInf((itsio_gps_info_t *)buf);
        break;
		case LTY_MSG_NOTIFY_STATION:
        	OnMsgStationInOff((BusStation_S *)buf);
        break;
		case LTY_MSG_ITSSD_NOTIFY_INFO:
        	OnMsgSDPrepare((sd_data_t *)buf);
        break;
		case LTY_MSG_ITSSD_NOTIFY_PARA:
        	OnMsgSDParaChange((enum SD_PARA_TYPE)chan);
        break;
		case LTY_MSG_ITS_NOTIFY_WORKING_STATUS:
       	 	OnMsgWorkingStatus((its_working_status_t *)buf);
        break;
		case LTY_MSG_ITSIO_NOTIFY_IO_STATUS:
        	OnMsgIOStatus((itsio_io_status_t *)buf);
        break;
		case LTY_MSG_EXCEPTION_NOTIFY_DOOR_STATUS:
        	OnMsgDoorExcp(chan, (exception_door_status_t *)buf);
        break;
		case LTY_MSG_NOTIFY_OFFLINE:
        	OnMsgOffLine((OffLine_S *)buf);
        break;
		case LTY_MSG_EXCEPTION_NOTIFY_OVERSPEED:
        	OnMsgOverSpeed((exception_overspeed_t *)buf);
        break;
		case LTY_MSG_NOTIFY_PARK:
        	OnMsgParkInOff((ParkAttr_S *)buf);
        break;
		case LTY_MSG_NOTIFY_PARK_STATUS:
        	OnMsgParkStatus((ParkStatus_S *)buf);
        break;
	}
	
	return 0;
}


LTY_VOID CCardWorker::OnMsgGpsInf(itsio_gps_info_t *pstGpsInf)
{
#if 0
    m_stGpsInf.u32Latitude  = pstGpsInf->u32Latitude2;
    m_stGpsInf.u32Longitude = pstGpsInf->u32Longitude2;
    m_stGpsInf.u8Speed      = pstGpsInf->u16Speed / 10;
    m_stGpsInf.u8Angle      = pstGpsInf->u16Angle / 2;
    m_stGpsInf.u8Altitude   = abs(pstGpsInf->s16Altitude);
#endif
	
	
	m_passenger_data.stPassengerHd.u32Lnt = pstGpsInf->u32Longitude2;   /*经度  蓝泰源格式的坐标：第一字节为度，第二字节为分，第三和第四字节为分的小数位、其中第三字节为高位、第四字节为低位，且这2个字节组成一个字、是分的小数位绝对数值的16进制数字。经纬度精确到分的小数点后4位*/
    m_passenger_data.stPassengerHd.u32Lat = pstGpsInf->u32Latitude2; /*纬度， 同上*/


	LTY_U32 u32Val;
    int ret = LtyMessageSend(LTY_MSG_DATA_COUNT_GET_MILEAGE, MILEAGE_TYPE_NEXT_STATION, (LTY_U32)&u32Val, sizeof(u32Val));
    if (LTY_SUCCESS != ret)
        u32Val = 0;

	//m_station_data.gps_data.LOC_TIME = time(NULL); //	定位时间
	LTY_U32 gps_time = time(NULL);
	LongLongUnon temp_longlong;
	LongUnon temp_long;
	ShortUnon temp_short;

	temp_longlong.i = gps_time;
	memcpy(m_current_gps_data.LOC_TIME,temp_longlong.longbuf,8);
	
	temp_long.i = pstGpsInf->u32Longitude;
	memcpy(m_current_gps_data.LON,temp_long.longbuf,4);
	temp_long.i = pstGpsInf->u32Latitude;
	memcpy(m_current_gps_data.LAT,temp_long.longbuf,4);
	//m_station_data.gps_data.LON = pstStation->pstGpsInfo->u32Longitude;      //	经度，单位为1*10-6度。
	//m_station_data.gps_data.LAT = pstStation->pstGpsInfo->u32Latitude;	  //	纬度，单位为1*10-6度。
	temp_short.i = pstGpsInf->u16Speed/10;
	memcpy(m_current_gps_data.VEC1,temp_short.intbuf,2);
	//m_station_data.gps_data.VEC1 = pstStation->pstGpsInfo->u16Speed/10;     //	速度，指卫星定位车载终端设备上传的行车速度信息，为必填项。单位为千米每小时(km/h)。【取整】
	m_current_gps_data.VEC2;     //	行驶记录速度，指车辆行驶记录设备上传的行车速度信息。单位为千米每小时(km/h)。【取整】
	m_current_gps_data.DISTANCE; //	车辆当日总里程数，指车辆上传的行车里程数。单位为米(m)。终端设备每天开机时候清零；【取整】
	temp_short.i = pstGpsInf->u16Angle;
	memcpy(m_current_gps_data.DIRECTION,temp_short.intbuf,2);	
	//m_station_data.gps_data.DIRECTION = pstStation->pstGpsInfo->u16Angle;//	方向，0-359，单位为度(°)，正北为0，顺时针。
	temp_short.i = pstGpsInf->s16Altitude;
	memcpy(m_current_gps_data.ALTITUDE,temp_short.intbuf,2);
	//m_station_data.gps_data.ALTITUDE = pstStation->pstGpsInfo->s16Altitude; //	海拔高度，单位为米(m)。
	temp_short.i = u32Val;
	memcpy(m_current_gps_data.DIS_NXT_STOP,temp_short.intbuf,2);
	//m_station_data.gps_data.DIS_NXT_STOP = u32Val;//	距离下站距离，单位米（m）
	temp_long.i = htons(u32Val / ((pstGpsInf->u16Speed *1000 /3600) + 2));
	memcpy(m_current_gps_data.TIME_NXT_STOP,temp_long.longbuf,4);
	//m_station_data.gps_data.TIME_NXT_STOP = htons(u32Val / ((pstStation->pstGpsInfo->u16Speed *1000 /3600) + 2));;//	距离下站的时间，单位秒（s）
	//m_station_data.gps_data.INDOOR_TEMP;  //	车内温度，单位为度。其它地方赋值
	m_current_gps_data.NXT_STOP_NO = m_u8NextStationID;  //	下一个站点序号。
	//m_station_data.gps_data.GPS_SIGNAL;   //	强调范围0～32，12以下为信号弱；
	m_current_gps_data.CAN_NO = 0;       //	CanBus 协议编号;0标识未连接can总线。非0就标识CAN总线版本号。
	temp_short.i = 0;
	memcpy(m_current_gps_data.CAN_LTH,temp_short.intbuf,2);
	//m_station_data.gps_data.CAN_LTH = 0;      //	后续数据 CanBus 数据块长度（CanBus 协议编号为0 时本字段也为 0）
	



}

LTY_VOID CCardWorker::OnMsgPassenger(itsio_passenger_flow_t *pstPassengerFlow)
{
	do
	{
		printf("OnMsgPassenger() \n");
		
		m_passenger_data.u8Up5Sec = pstPassengerFlow->u16Up5Sec; /*5秒之内上的客流*/
		m_passenger_data.u8Down5Sec = pstPassengerFlow->u16Down5Sec; /*5秒之内下的客流*/
		m_passenger_data.u16Up = pstPassengerFlow->u16PassengerSum; /*车上总人数*/

		m_passenger_data.stPassengerHd.u32DateTime   = time(NULL);
		printf("OnMsgPassenger() %x\n",m_passenger_data.stPassengerHd.u32DateTime);
		//time_t test_time;
		//time(&test_time);

		// time_t t;  
     	//time(&t);
		
		//printf("OnMsgPassenger() test_time:%x\n",t);
		
		//m_passenger_data.stPassengerHd.u32Lnt; //前面已赋值  
    	//m_passenger_data.stPassengerHd.u32Lat; //前面已赋值
		m_passenger_data.stPassengerHd.u8Status = m_unStatus.u8Status;

		m_passenger_data.stPassengerHd.u8StationID   = m_current_station_id;

		if(0 == m_passenger_data.stPassengerHd.u8StationID)
        	m_passenger_data.stPassengerHd.u8StationID   = 1;

		m_passenger_data.stPassengerHd.u8StationAttr = m_u8StInOutFlug;
		
		card_cmd_req_passenger_flow();
		
	}while(0);
}

LTY_VOID CCardWorker::OnMsgStationInOff(BusStation_S *pstStation)
{
	bus_plate_set_t stBusPlate;

	m_current_station_id = pstStation->u32BusStationId;
    m_u8NextStationID = pstStation->u32NextStationId;

	if(ENTER_STATION == pstStation->s32Status)
    {
        m_u8StInOutFlug = 0;
    }
    else if(LEAVE_STATION == pstStation->s32Status)
    {
        m_u8StInOutFlug = 1;
    }

	memset(&stBusPlate,0,sizeof(stBusPlate));
	int ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_BUS_PLATE, (LTY_U32)&stBusPlate, sizeof(stBusPlate));
    if(LTY_SUCCESS != ret)
    {
        DBGPOUT("Get SD Palate Para Fail !!!!\n");
    }
	
	//m_station_data.VEH_STATE; //车辆运行状态
	//m_station_data.STOP_NO = (u8NextStationID == 0 ? 1 : (u8NextStationID - 1));   //当前公交站点序号
	LTY_U8  u8NextStationID_temp = m_current_station_id;//(u8NextStationID == 0 ? 1 : (u8NextStationID - 1));
	LongUnon station_id;
	station_id.i = u8NextStationID_temp;

	g_u32BusStationId = u8NextStationID_temp;
	
	memcpy(m_station_data.STOP_NO,station_id.longbuf,4);
	//m_station_data.DRIVER_ID[10]; //司机编号
	char vehicle_id = strlen(stBusPlate.charBusNoBuf);
	if(vehicle_id > 6)
		vehicle_id = 6;
	memcpy(m_station_data.VEHICLE_ID,stBusPlate.charBusNoBuf,vehicle_id);
	m_station_data.VEHICLE_ID[6]; //车辆编号 

	m_station_data.STOP_FLAG = pstStation->u32flag;  //公交站点标识；0x00，首战；0xff，末站；0x02，拐弯点；0x03，安全事故黑点；
	m_station_data.STOP_RPT = pstStation->u32NotifyStation;   //报站方式，0x01，GPS自动报站；0x00，手动报站；0x02，表示调试；
	m_station_data.IN_OUT_FLAG = m_u8StInOutFlug; //进出站标识；0，进站；1，离站
	if(1 == pstStation->s32direction)
		m_station_data.ONDIRECTION_FLAG = 0; //0:上行 1:下行
	else
		m_station_data.ONDIRECTION_FLAG = 1;

	LTY_U32 u32Val;
    ret = LtyMessageSend(LTY_MSG_DATA_COUNT_GET_MILEAGE, MILEAGE_TYPE_NEXT_STATION, (LTY_U32)&u32Val, sizeof(u32Val));
    if (LTY_SUCCESS != ret)
        u32Val = 0;

		
	//m_station_data.gps_data.LOC_TIME = time(NULL); //	定位时间
	LTY_U32 gps_time = time(NULL);
	LongLongUnon temp_longlong;
	LongUnon temp_long;
	ShortUnon temp_short;

	temp_longlong.i = gps_time;
	memcpy(m_station_data.gps_data.LOC_TIME,temp_longlong.longbuf,8);
	
	temp_long.i = pstStation->pstGpsInfo->u32Longitude;
	memcpy(m_station_data.gps_data.LON,temp_long.longbuf,4);
	temp_long.i = pstStation->pstGpsInfo->u32Latitude;
	memcpy(m_station_data.gps_data.LAT,temp_long.longbuf,4);
	//m_station_data.gps_data.LON = pstStation->pstGpsInfo->u32Longitude;      //	经度，单位为1*10-6度。
	//m_station_data.gps_data.LAT = pstStation->pstGpsInfo->u32Latitude;	  //	纬度，单位为1*10-6度。
	temp_short.i = pstStation->pstGpsInfo->u16Speed/10;
	memcpy(m_station_data.gps_data.VEC1,temp_short.intbuf,2);
	//m_station_data.gps_data.VEC1 = pstStation->pstGpsInfo->u16Speed/10;     //	速度，指卫星定位车载终端设备上传的行车速度信息，为必填项。单位为千米每小时(km/h)。【取整】
	m_station_data.gps_data.VEC2;     //	行驶记录速度，指车辆行驶记录设备上传的行车速度信息。单位为千米每小时(km/h)。【取整】
	m_station_data.gps_data.DISTANCE; //	车辆当日总里程数，指车辆上传的行车里程数。单位为米(m)。终端设备每天开机时候清零；【取整】
	temp_short.i = pstStation->pstGpsInfo->u16Angle;
	memcpy(m_station_data.gps_data.DIRECTION,temp_short.intbuf,2);	
	//m_station_data.gps_data.DIRECTION = pstStation->pstGpsInfo->u16Angle;//	方向，0-359，单位为度(°)，正北为0，顺时针。
	temp_short.i = pstStation->pstGpsInfo->s16Altitude;
	memcpy(m_station_data.gps_data.ALTITUDE,temp_short.intbuf,2);
	//m_station_data.gps_data.ALTITUDE = pstStation->pstGpsInfo->s16Altitude; //	海拔高度，单位为米(m)。
	temp_short.i = u32Val;
	memcpy(m_station_data.gps_data.DIS_NXT_STOP,temp_short.intbuf,2);
	//m_station_data.gps_data.DIS_NXT_STOP = u32Val;//	距离下站距离，单位米（m）
	temp_short.i = htons(u32Val / ((pstStation->pstGpsInfo->u16Speed *1000 /3600) + 2));
	memcpy(m_station_data.gps_data.TIME_NXT_STOP,temp_short.intbuf,2);
	//m_station_data.gps_data.TIME_NXT_STOP = htons(u32Val / ((pstStation->pstGpsInfo->u16Speed *1000 /3600) + 2));;//	距离下站的时间，单位秒（s）
	//m_station_data.gps_data.INDOOR_TEMP;  //	车内温度，单位为度。其它地方赋值
	m_station_data.gps_data.NXT_STOP_NO = m_u8NextStationID;//(u8NextStationID == 0 ? 1 : (u8NextStationID));  //	下一个站点序号。
	//m_station_data.gps_data.GPS_SIGNAL;   //	强调范围0～32，12以下为信号弱；
	m_station_data.gps_data.CAN_NO = 0;       //	CanBus 协议编号;0标识未连接can总线。非0就标识CAN总线版本号。
	temp_short.i = 0;
	memcpy(m_station_data.gps_data.CAN_LTH,temp_short.intbuf,2);
	//m_station_data.gps_data.CAN_LTH = 0;      //	后续数据 CanBus 数据块长度（CanBus 协议编号为0 时本字段也为 0）
    //if (ENTER_STATION == pstStation->s32Status 
    //    || FIRST_STATION_FLAG == pstStation->u32flag){
	    card_cmd_req_sation_in_out();
    //}
	g_global_info.direction = m_station_data.ONDIRECTION_FLAG;
	
	
}

LTY_VOID CCardWorker::OnDirChange(Direction_S* pstdirect)
{
    m_u8NextStationID = pstdirect->s32NextStationId;
	m_current_station_id = pstdirect->s32CurrStationId;
	m_unStatus.stStatus.u1Dir = (DIRECTION_DOWN == pstdirect->u32Direction? 1 : 0);   
	
}

LTY_VOID CCardWorker::OnMsgSDPrepare(sd_data_t *pstSdData)
{
	LTY_RESULT ret = LTY_FAILURE;
	cfg_para_lang_t stLan;
	bus_plate_set_t stBusPlate;

	m_unStatus.stStatus.u1Working = (1==pstSdData->tBusPara.tBusConfig.u8YingYunType?1:0);
	m_unStatus.stStatus.u1Dir = (1==pstSdData->tLinePara.tLineBase.u8UpOrDownFlag?0:1);
	m_passenger_data.stPassengerHd.u32LineId = pstSdData->tLinePara.tLineBase.u32GPRSLineNo;

	m_station_data.VEH_STATE = pstSdData->tBusPara.tBusConfig.u8YingYunType;

	g_global_info.direction = m_unStatus.stStatus.u1Dir;
	g_u32BusStationId = pstSdData->tLinePara.tLineBase.u8CurrentLineStation;

	memset(&stBusPlate,0,sizeof(stBusPlate));
	ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_BUS_PLATE, (LTY_U32)&stBusPlate, sizeof(stBusPlate));
    if(LTY_SUCCESS != ret)
    {
        DBGPOUT("Get SD Palate Para Fail !!!!\n");
    }

	LongUnon station_id;
	station_id.i = g_u32BusStationId;

	memcpy(m_station_data.STOP_NO,station_id.longbuf,4);
	//m_station_data.DRIVER_ID[10]; //司机编号
	char vehicle_id = strlen(stBusPlate.charBusNoBuf);
	if(vehicle_id > 6)
		vehicle_id = 6;
	memcpy(m_station_data.VEHICLE_ID, stBusPlate.charBusNoBuf,vehicle_id);
	//m_station_data.VEHICLE_ID[6]; //车辆编号 

	if (0 == g_u32BusStationId){
	    m_station_data.STOP_FLAG = 0;  //公交站点标识；0x00，首战；0xff，末站；0x02，拐弯点；0x03，安全事故黑点；
    }
	else if (g_u32BusStationId == pstSdData->tLinePara.tLineAssign[m_unStatus.stStatus.u1Dir].u8StationCount - 1){
		m_station_data.STOP_FLAG = 0xff;
	}
	else{
		m_station_data.STOP_FLAG = 0x55;
	}
	m_station_data.STOP_RPT = 0;   //报站方式，0x01，GPS自动报站；0x00，手动报站；0x02，表示调试；
	m_station_data.IN_OUT_FLAG = 0; //进出站标识；0，进站；1，离站

	m_station_data.ONDIRECTION_FLAG = m_unStatus.stStatus.u1Dir; //0:上行 1:下行
    m_stationprepare = LTY_TRUE;
    if (LTY_TRUE == m_bcardconnect){
        card_cmd_req_sation_in_out();
    }
}

LTY_VOID CCardWorker::OnMsgSDParaChange(enum SD_PARA_TYPE enParaType)
{
    sd_data_t *pstPara;
    LTY_RESULT ret;

    ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_ALL, (LTY_U32)&pstPara, sizeof(pstPara));
    if(LTY_SUCCESS != ret)
    {
        WAROUT("get itssd info failed\n");
        return;
    }
	
	g_global_info.direction = (1==pstPara->tLinePara.tLineBase.u8UpOrDownFlag?0:1);
	g_u32BusStationId = pstPara->tLinePara.tLineBase.u8CurrentLineStation;

	switch (enParaType)
    {
    case SD_PARA_NET_CFG:
    case SD_PARA_UPDATE:
        OnMsgSDPrepare(pstPara);
        break;

    case SD_PARA_DIRECTION:
        m_unStatus.stStatus.u1Dir = (pstPara->tLinePara.tLineBase.u8UpOrDownFlag == 1 ? 0 : 1);
        //m_u8GpsDir = m_stGpsInf.unStatus.stStatus.u1Dir;
        break;

    case SD_PARA_BUS_PLATE:
        m_unStatus.stStatus.u1Working = (pstPara->tBusPara.tBusConfig.u8YingYunType == 0 ? 0 : 1);   
        //m_stDevSN.u16BusSN  = atoi(pstPara->tBusPara.tEquBase.charBusNoBuf);
        //m_stDevSN.u16BusSN  = htons(m_stDevSN.u16BusSN);
        break;
        
    default:
        break;
    }
}

LTY_VOID CCardWorker::OnMsgWorkingStatus(its_working_status_t *pstWorking)
{
    m_unStatus.stStatus.u1Working = (1==pstWorking->u8Enable?1:0);
}

LTY_VOID CCardWorker::OnMsgIOStatus(itsio_io_status_t *pstIOStatus)
{
    m_unStatus.stStatus.u1FDoor = (pstIOStatus->u32IOStatus & ITSIO_IOS_FDOOR) ? 1 : 0;
    m_unStatus.stStatus.u1BDoor = (pstIOStatus->u32IOStatus & ITSIO_IOS_BDOOR) ? 1 : 0;
//    m_stGpsInf.s8TempIn = pstIOStatus->s8TempIn;    
    
   // m_stGpsInf.s8TempIn = 33;    
   // m_stGpsInf.u8SigQual = pstIOStatus->s8TempOut;
	m_station_data.gps_data.INDOOR_TEMP = pstIOStatus->s8TempIn;
	m_current_gps_data.INDOOR_TEMP = pstIOStatus->s8TempIn;

}

LTY_VOID CCardWorker::OnMsgDoorExcp(LTY_U32 u32Type, exception_door_status_t *pstDoorStatus)
{
    //ie_event_comm_t stEvtDoorExcp;

    OnMsgIOStatus(pstDoorStatus->pstIoStatus);

}

LTY_VOID CCardWorker::OnMsgOffLine(OffLine_S *pstOffLine)
{

    m_unStatus.stStatus.u1OffLine = (OFF_LINE==pstOffLine->s32Status ? 1 : 0);

}

LTY_VOID CCardWorker::OnMsgOverSpeed(exception_overspeed_t *pstOverSpeed)
{
    m_unStatus.stStatus.u1OverSpeed = (pstOverSpeed->bIsStart ? 1 : 0);
}


LTY_VOID CCardWorker::OnMsgParkInOff(ParkAttr_S *pstPark)
{

    m_unStatus.stStatus.u1Park = (ENTER_PARK==pstPark->s32Status ? 1 : 0);


}

LTY_VOID CCardWorker::OnMsgParkStatus(ParkStatus_S *pstParkStatus)
{
    DBGPOUT("Park Status : %u\n",pstParkStatus->s32Status);

    m_unStatus.stStatus.u1Park = (pstParkStatus->s32Status == INPARK ? 1 : 0);

    return;
}




