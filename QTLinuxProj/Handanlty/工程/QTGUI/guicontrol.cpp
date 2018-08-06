#define LTY_DEBUG
#include <stdio.h>
#include <pthread.h>
#include "ltyuicallinterface.h"
#include "guicontrol.h"

#include "LtyMsgUserInput.h"
#include "LtyMsgCfg.h"
#include "LtySocket.h"

#include "../../itsio/MCUCtrlStruct.h"
#include "version.h"
#include "LtyMsgRec.h"
#include "MainCtrlMsglist.h"

#include "Interface.h"
#include "LtyMsgSysCtrl.h"
#include "LtyMsgUpg.h"

#include <dirent.h>
#include <sys/mount.h>
#include "support/log.h"
#include "LtyMsgLog.h"

#include "LtyMsgItsAudio.h"
#include "interface.h"
#include <arpa/inet.h>
#include "CardSvr.h"
#include "AlgInterface.h"
#include "type.h"
#include "record/sqlitectrl.h"
#include "LtyMsgNet.h"

#define CURRENT_IS_D2	1

#define CURRENT_IS_P1	1


int g_upNum[DOORNUM] = {0, 0};
int g_aupNum[DOORNUM] = {0, 0};
int g_cupNum[DOORNUM] = {0, 0};
int g_downNum[DOORNUM] = {0, 0};
int g_adownNum[DOORNUM] = {0, 0};
int g_cdownNum[DOORNUM] = {0, 0};

pthread_mutex_t EncoderLock;



//������Ϣ����
void gui_system_starting_info_set(cJSON *root);

/*�˵�-��Ϣ*/
void gui_message_set(cJSON *root);

//�˵�-˾���˵�-�߼�����
void gui_driver_menu_set(cJSON *root);

/*�˵�-ϵͳ����-ע����Ϣ*/  //---------------------------ok
void gui_system_register_info_set(cJSON *root);
void gui_system_register_info_get();
/*�˵�-ϵͳ����-����ɼ�*/
void gui_system_coordinate_collection_set(cJSON *root);
/*�˵�-ϵͳ����-¼���ѯ*/
void gui_system_videotape_query_set(cJSON *root);
/*�˵�-ϵͳ����-CAM����*/
void gui_system_camera_config_set(cJSON *root);
/*�˵�-ϵͳ����-��������*/
void gui_system_passenger_flow_set(cJSON *root);
void gui_system_passenger_flow_get(int _type,void *_param);
/*�˵�-ϵͳ����-�洢����*/ //---------------------------ok  ����Ҫ���� Ӳ�̺�EMMC
void gui_system_storage_manage_set(cJSON *root);
void gui_system_storage_manage_get(int _type);
/*�˵�-ϵͳ����-����*/  //---------------------------ok   ����Ҫ����WIFI
void gui_system_network_set(cJSON *root);
void gui_system_network_get();
/*�˵�-ϵͳ����-����-WIFI*/
void gui_system_wifi_set(cJSON *root);
/*�˵�-ϵͳ����-I/O����*/ //---------------------------ok
void gui_system_switch_config_set(cJSON *root);
void gui_system_switch_config_get();
/*�˵�-ϵͳ����-�߼�����*/
void gui_system_senior_setup_set(cJSON *root);
void gui_system_pos_config_set(cJSON *root);


/*�˵�-ϵͳ����-ϵͳ����*/ //---------------------------ok
void gui_system_system_setup_set(cJSON *root);
void gui_system_system_setup_get();
/*�˵�-ϵͳ����-ϵͳά��*/ //---------------------------ok
void gui_system_maintain_set(cJSON *root);
void gui_system_maintain_get(char _type,char _main_type,char _operate_type,char _percent,const char *_failed_result);

/*�˵�-����״̬*/ //---------------------------ok  ���������Ҫ����
void gui_run_status_set(cJSON *root);
void gui_run_status_get(char _type,void *_param); 

/*�˵�-���ڱ���*/ //---------------------------ok
void gui_about_machine_set(cJSON *root);
void gui_about_machine_get();


/*ϵͳ����-�ػ�(����)*/  //---------------------------ok
void gui_system_operate_set(cJSON *root);
/*�������ʾ*/
void gui_videotape_video_set(cJSON *root);
void gui_videotape_video_get();
/*��������*/
void gui_service_term_set(cJSON *root);
/*����*/
void gui_light_set(cJSON *root);
void gui_light_get();
/*����*/
void gui_voice_set(cJSON *root);
void gui_voice_get();

//////////////////////////////////////////////////P1 start
void gui_main_page_set(cJSON *root);
void gui_main_page_get();
void gui_main_page_get_ex(char _type);

void gui_normal_set_info_set(cJSON *root);
void gui_normal_set_info_get();
void gui_qr_code_pay_info();
void gui_run_status_info_get(char _type,void *_param);
void gui_login_info_get();
void gui_two_dimensional_code_info();
void gui_system_key_up();
void gui_system_mouse_status();

void picture_manage_start();
static LTY_U32 PictureIOProc(LTY_STRING szRootFrom,LTY_STRING szRootDest, LTY_BOOL bImport,LTY_BOOL bNumber,LTY_U32 picture_total,LTY_CHAR _type);
void gui_ic_card_pay_info(void *_param);


//////////////////////////////////////////////////P1 end




/*
ת��32λ�������汾�ţ���ʽMajor[31~24] | Minor[23~16] | Release[15~0]
*/
void ConvertU32ToVersionString(LTY_U32 verNo, LTY_CHAR *verStr);
int FetchBSPVer(int type);
static void* mainctrl_parse_data(void* pArg);
int ltySendMessge2UiCallbackEx(cJSON *root);

int ltySendMessge2UiCallback(cJSON *root);


static cfg_para_svr_net_t g_stCfg_SvrNet;
static cfg_para_sys_net_t g_stCfg_sys_net;
static cfg_para_dev_inf_t m_stCfg_DevInfo;
static DEV_DOOR_CONFIG_INFO_T g_door_config_info;
static cfg_para_system_set_t g_stCfg_system_set;
pthread_t g_pid;
itsio_passenger_flow_t m_stPassengerFlow;

static LTY_U16 g_u16PassengerSum = 0;
static LTY_U16 g_u16PassengerUp = 0;
static LTY_U16 g_u16PassengerDown = 0;	


void *ApcPthread(void *param)
{
	int cnt = 0;
	COUNTERINFO info;
    memset(&m_stPassengerFlow, 0, sizeof(m_stPassengerFlow));
	//camer_ctrl(1);

	while (1)
	{
        //5s����
		if (cnt > 5)
		{
			int i;//, j;
			static int s_inNum = 0; 
			static int s_outNum = 0; 
			itsio_passenger_flow_t stInof;
			OUTPUTINFO *outInfo = NULL;//[DOORNUM];
			//stInof.u16Up5Sec = 5;
			//stInof.u16Down5Sec = 5;
			//LtyMessageSend(LTY_MSG_ITSIO_NOTIFY_PASSENGER_FLOW_ALL,0,(LTY_U32)&stInof,sizeof(itsio_passenger_flow_t));

			for (int i = 0; i < 2; i++)
			{
				outInfo = AlgGetInfomation(i);

				//վ��ͳ��
			#if 1
				if (outInfo->upNum != g_upNum[i] || outInfo->downNum != g_downNum[i])
				{
					//int in = 0, out = 0;
					
					g_upNum[i] = outInfo->upNum;
					g_downNum[i] = outInfo->downNum;
				}
			#endif

				g_cupNum[i] = outInfo->c_upNum;
				g_cdownNum[i] = outInfo->c_downNum;

				g_aupNum[i] = outInfo->a_upNum;
				g_adownNum[i] = outInfo->a_downNum;
			}
			
			{
				int in = 0,out = 0;
				int in1 = 0,out1 = 0;


				//for (i = 0; i < DOORNUM; i++)
				{
					in = g_aupNum[0] + g_aupNum[1];
					out = g_adownNum[0] + g_adownNum[1];

					in1 = g_cupNum[0] + g_cupNum[1];
					out1 = g_cdownNum[0] + g_cdownNum[1];
				}
				
				stInof.u16PassengerUp = in;
				stInof.u16PassengerDown = out;

				//if (g_terminusIn == 1)
				{
					stInof.u16Up5Sec = in - s_inNum;
					stInof.u16Down5Sec = out - s_outNum;
					stInof.u16PassengerSum = MIN(MAX(in1 - out1, 0), 60000);
					m_stPassengerFlow.u16PassengerSum = stInof.u16PassengerSum;
                    m_stPassengerFlow.u16PassengerUp = stInof.u16Up5Sec;
                    m_stPassengerFlow.u16PassengerDown = stInof.u16Down5Sec;

                    gui_passenger_flow_info(param, m_stPassengerFlow);
				}
				//else 
				{
					//stInof.u16Up5Sec = in - s_inNum;
					//stInof.u16Down5Sec = out- s_outNum;
					
					//stInof.u16PassengerSum = in1;
				}

				//LOGOUT("u16PassengerUp = %d\n", stInof.u16PassengerUp);
				//LOGOUT("u16PassengerDown = %d\n", stInof.u16PassengerDown);
				//LOGOUT("u16Up5Sec = %d\n", stInof.u16Up5Sec);
				//LOGOUT("u16Down5Sec = %d\n", stInof.u16Down5Sec);

				//LOGOUT("g_cupNum[0] = %d   g_cupNum[1] = %d   g_cdownNum[0] = %d   g_cdownNum[1] = %d\n", g_cupNum[0], g_cupNum[1], g_cdownNum[0],g_cdownNum[1]);
				//LOGOUT("stInof.u16PassengerSum = %d\n", stInof.u16PassengerSum);

				s_inNum = in;
				s_outNum = out;

				if (in1 < out1)
				{
					for (i = 0; i < DOORNUM; i++)
					{
						Clear_Counter(i, CLEARCH);

						g_cupNum[i] = 0;
						g_cdownNum[i] = 0;
						//s_inNum = 0;
						//s_outNum = 0;
					}
				}

				if (stInof.u16Up5Sec != 0 || stInof.u16Down5Sec != 0)
				{
					//LOGOUT("u16PassengerUp = %d\n", stInof_st.u16PassengerUp);
					//LOGOUT("u16PassengerDown = %d\n", stInof_st.u16PassengerDown);
					//LOGOUT("u16Up5Sec = %d\n", stInof.u16Up5Sec);
					//LOGOUT("u16Down5Sec = %d\n", stInof.u16Down5Sec);

					//LOGOUT("u16PassengerSum = %d\n", stInof.u16PassengerSum);
				
					//5��һ�ε���Ϣ
					LtyMessageSend(LTY_MSG_ITSIO_NOTIFY_PASSENGER_FLOW_ALL,0,(LTY_U32)&stInof,sizeof(itsio_passenger_flow_t));
				}
			}

			cnt = 0;
		}

		cnt++;
		
		sleep(1);
	}

	return NULL;
}

int Rs485State(int chnl, int state, char *cameraID)
{
	RS485STATE rs485State;

	rs485State.chnl = chnl;
	rs485State.state = state;
	
	memset(rs485State.cameraID, 0x00, 9);
	memcpy(rs485State.cameraID, cameraID, 8);
	//printf("chnl%d state%d cameraID:%s\n", chnl, state, cameraID);
	gui_run_status_info_get(0, (void*)&rs485State);
	gui_system_passenger_flow_get(4, (void*)&rs485State);

	return 0;
}

int Rs485CameraID(int chnl, char *cameraID)
{
	int chnlState[2] = {-1, -1};

	if (cameraID != NULL)
	{
		chnlState[chnl] = 1;	
	}

	//WAROUT("chnl%d cameraID:%s chnlState[chnl] = %d\n", chnl, cameraID, chnlState[chnl]);

	gui_system_passenger_flow_get(5, chnlState);
	
	return 0;
}

int UpdateCameraProgress(int chnl, int progress, int sum)
{	
	int data[2];

	data[0] = progress;
	data[1] = sum;
	gui_system_passenger_flow_get(2, &data);
	return 0;
}

int SendEncord(unsigned char *yuv, int size)
{
    pthread_mutex_lock(&EncoderLock);
	LtyMessageSend(LTY_MSG_CUSTOMERSTREAM, 0, (LTY_U32)yuv,size);
    pthread_mutex_unlock(&EncoderLock);
	return 0;
}

static LTY_RESULT ApcDoorDeal(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32Data, LTY_U32 u32Len, LTY_LPVOID lpPriv)
{
    //return ((CItsIO *)lpPriv)->MsgHandler(MsgType, u32Chn, u32Data, u32Len);
	itsio_io_status_t *pstIOStatus = (itsio_io_status_t *)u32Data;

	//printf("pstIOStatus->u32IOStatus 0x%x\n",pstIOStatus->u32IOStatus);
	
	if ((pstIOStatus->u32IOStatus & ITSIO_IOS_FDOOR))
	{
		//LOGOUT("ch0 door open\n");
		ApcSetChnlState(0, 1);
	}
	else 
	{
		//LOGOUT("ch0 door close\n");
		ApcSetChnlState(0, 0);
	}

	if ((pstIOStatus->u32IOStatus & ITSIO_IOS_MDOOR))
	{
		//LOGOUT("ch1 door open\n");
		ApcSetChnlState(1, 1);
	}
	else 
	{
		//LOGOUT("ch1 door close\n");
		ApcSetChnlState(1, 0);
	}

	if ((pstIOStatus->u32IOStatus & ITSIO_IOS_BDOOR))
	{
		//LOGOUT("ch2 door open\n");
		//ApcSetChnlState(2, 1);
	}
	else 
	{
		//LOGOUT("ch2 door close\n");
		//ApcSetChnlState(2, 0);
	}

	return 1;
}


int ApcAlgStart()
{
#ifdef PRODUCT_P1
	ApcIniterface(0);
#elif defined(PRODUCT_P2)
	ApcIniterface(1);
#else
	ApcIniterface(1);
#endif
	SysCreateThread(0x20000, NULL, ApcPthread, &g_pid);
	sleep(5);
    
    pthread_mutex_init(&EncoderLock,NULL);
    
	LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_IO_STATUS,  ApcDoorDeal, 0, NULL);
	
	SetSendVideoCallback((SEND_ENCODE_CALLBACK)SendEncord);
	SetSendRs485StateCallback(0, (SEND_Rs485_CALLBACK)Rs485State);
	SetSendRs485StateCallback(1, (SEND_Rs485_CALLBACK)Rs485State);
	
	SetSendCameraIDCallback(0, (SEND_CameraID_CALLBACK)Rs485CameraID);
	SetSendCameraIDCallback(1, (SEND_CameraID_CALLBACK)Rs485CameraID);

	SetUpdateCameraCallback(0, (SEND_UpdateCamera_CALLBACK)UpdateCameraProgress);
	SetUpdateCameraCallback(1, (SEND_UpdateCamera_CALLBACK)UpdateCameraProgress);
	return 0;
}

static cfg_para_svr_net_ex_t g_stCfg_svr_net_ex;

static void* create_gui_main_page(void* pArg)
{
	pthread_detach( pthread_self());
	printf("create_gui_main_page() start\n");
	lytUiappStart();
	printf("create_gui_main_page() end\n");
	return NULL;
}
#include "sysdesk.h"
int start_init_gui(int argc, const char* argv[], void* pArg)
{
    int ret;
    pthread_t th;
	pthread_t th_parse;
	printf("start_init_gui() start\n");
	
	SysDesk* pdesktop = NULL;

    pdesktop = new SysDesk();
    pdesktop->RegDeskMsg();

#if 0
	LtyMessageRegister(LTY_MSG_UPG_NOTIFY_LOCAL_PROGRESS,  GuiUpgradeMsgHandle, 0, NULL);
    LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_PASSENGER_FLOW,GuiUpgradeMsgHandle,0,NULL);
	LtyMessageRegister(LTY_MSG_STMDEV_NOTIFY_CONN_STATUS,GuiUpgradeMsgHandle,0,NULL);
	LtyMessageRegister(LTY_MSG_NOTIFY_HDD_STATUS,GuiUpgradeMsgHandle,0,NULL);
	LtyMessageRegister(LTY_MSG_NOTIFY_HDD_INIT_STATUS,GuiUpgradeMsgHandle,0,NULL);
#endif
	lytSendMsgToUiAppCall(NULL,kEnumUiMessageCallBack, ltySendMessge2UiCallback);


    ret = pthread_create(&th, NULL, create_gui_main_page, (void*)NULL);
    if (0 != ret){
        printf("start_init_gui() fail\n");
        return -1;
    }

	pthread_create( &th_parse, NULL,mainctrl_parse_data, (void*)NULL );

	printf("start_init_gui() end\n");

    return 0;
}

static void* mainctrl_parse_data(void* pArg)
{
	pthread_detach( pthread_self());
	unsigned int len = 0,addr = 0;
    
    char message_type;
	
	while(1)
	{
		if(MainCtrlMsgListGet(NULL,&len,&addr,1,&message_type))
		{
			printf("mainctrl_parse_data() MainCtrlMsgListGet<<<<<<<<<<<<<<<\n");
			cJSON *pRoot = (cJSON *)addr;
			if(pRoot)
			{
				ltySendMessge2UiCallbackEx(pRoot);
				cJSON_Delete(pRoot);
			}
		}
	}
	
	
	pthread_exit(0);
	
	return NULL;
}


int ltySendMessge2UiCallback(cJSON *root)
{
	MainCtrlMsgListAdd(root, 0, 0,0);
	return 0;
}
int ltySendMessge2UiCallbackEx(cJSON *root)
{
	cJSON *objValue = NULL;
	char szmsgtype[64];
	int str_len = 0;

	memset(szmsgtype, 0, sizeof(szmsgtype));
	if(root)
	{
		
		objValue = cJSON_GetObjectItem(root,"msg_type");	 
		if(objValue != NULL && objValue->valuestring)
		{
		 	str_len = strlen(objValue->valuestring);
			if(str_len > 64)str_len=64;
			memcpy(szmsgtype, objValue->valuestring, str_len);
		}


		printf("msg_type:%s \n", szmsgtype);
		char *out = NULL;
        out = cJSON_PrintUnformatted(root);
        if(out)
        {
            printf("%s\n",out);
			printf("out is not null: \n");
            free(out);
        }
		
		if(!strcmp(szmsgtype, "gui_station_info"))/*��վ*/
		{
			objValue = cJSON_GetObjectItem(root,"station_play");   
			if(objValue)
			{
				printf("ltySendMessge2UiCallback()  station_play\n");
				LtyMessagePost(LTY_MSG_USERIN_NOTIFY_NEXTSTATION, 1, 0, 0);
			}
			objValue = cJSON_GetObjectItem(root,"station_replay");   
			if(objValue)
			{
				printf("ltySendMessge2UiCallback()  station_replay\n");
				LtyMessagePost(LTY_MSG_USERIN_NOTIFY_REPEATCMD, 0, 0, 0);     
			}
			objValue = cJSON_GetObjectItem(root,"station_prev");   
			if(objValue)
			{
				printf("ltySendMessge2UiCallback()  station_prev\n");
				LtyMessagePost(LTY_MSG_USERIN_NOTIFY_PRESTATION, 0, 0, 0);
			}
			objValue = cJSON_GetObjectItem(root,"station_next");   
			if(objValue)
			{
				
#ifdef PRODUCT_P2
				printf("ltySendMessge2UiCallback()  station_next 1\n");
				LtyMessagePost(LTY_MSG_USERIN_NOTIFY_NEXTSTATION, 1, 0, 0);
				printf("ltySendMessge2UiCallback()  station_next 2\n");
				LtyMessagePost(LTY_MSG_USERIN_NOTIFY_NEXTSTATION, 1, 1, 0);
#else				
				printf("ltySendMessge2UiCallback()  station_next\n");
				LtyMessagePost(LTY_MSG_USERIN_NOTIFY_NEXTSTATION, 1, 0, 0);
#endif
			}
			
		}
		/*��ҳ*/
		else if(!strcmp(szmsgtype, "gui_voice"))/*����*/
		{
			gui_voice_set(root);
		}
		/*�˵�--˾���˵�--��������*/
		else if(!strcmp(szmsgtype, "gui_light"))/*����*/
		{
			gui_light_set(root);
		}
		else if(!strcmp(szmsgtype, "gui_service_term"))/*��������*/
		{
			gui_service_term_set(root);
		}
		else if(!strcmp(szmsgtype, "gui_videotape_video"))/*�������ʾ*/
		{
			gui_videotape_video_set(root);
		}
		//�˵�-���ڱ���
		else if(!strcmp(szmsgtype, "gui_about_machine"))
		{
			gui_about_machine_set(root);
		}
		//�˵�-����״̬
		else if(!strcmp(szmsgtype, "gui_run_status"))
		{
			gui_run_status_set(root);
		}
		//�˵�-˾���˵�-�߼�����
		else if(!strcmp(szmsgtype, "gui_driver_menu"))
		{
			gui_driver_menu_set(root);
		}
		/*�˵�-��Ϣ*/
		else if(!strcmp(szmsgtype, "gui_message"))
		{
			gui_message_set(root);
		}
		/*�˵�-ϵͳ����-ע����Ϣ*/
		else if(!strcmp(szmsgtype, "gui_system_register_info"))
		{
			gui_system_register_info_set(root);
		}
		/*������Ϣ�趨*/
		else if(!strcmp(szmsgtype, "gui_system_starting_info"))
		{

		    printf("gui_system_starting_info \n");
			gui_system_starting_info_set(root);
		}
		
		/*�˵�-ϵͳ����-����ɼ�*/
		else if(!strcmp(szmsgtype, "gui_system_coordinate_collection"))
		{
			gui_system_coordinate_collection_set(root);
		}
		/*�˵�-ϵͳ����-¼���ѯ*/
		else if(!strcmp(szmsgtype, "gui_system_videotape_query"))
		{
			gui_system_videotape_query_set(root);
		}
		/*�˵�-ϵͳ����-�洢����*/
		else if(!strcmp(szmsgtype, "gui_system_storage_manage"))
		{
			gui_system_storage_manage_set(root);
		}
		/*ϵͳ����-�ػ�(����)*/
		else if(!strcmp(szmsgtype, "gui_system_operate"))
		{
			gui_system_operate_set(root);
		}
		/*�˵�-ϵͳ����-����*/
		else if(!strcmp(szmsgtype, "gui_system_network"))
		{
			gui_system_network_set(root);
		}
		/*�˵�-ϵͳ����-����-WIFI*/
		else if(!strcmp(szmsgtype, "gui_system_wifi"))
		{
			gui_system_wifi_set(root);
		}
		/*�˵�-ϵͳ����-I/O����*/
		else if(!strcmp(szmsgtype, "gui_system_switch_config"))
		{
			gui_system_switch_config_set(root);
		}
		/*�˵�-ϵͳ����-�߼�����*/
		else if(!strcmp(szmsgtype, "gui_system_senior_setup"))
		{
			gui_system_senior_setup_set(root);
		}
		else if(!strcmp(szmsgtype, "gui_system_pos_config"))
		{
			gui_system_pos_config_set(root);
		}
		/*�˵�-ϵͳ����-ϵͳ����*/
		else if(!strcmp(szmsgtype, "gui_system_system_setup"))
		{
			gui_system_system_setup_set(root);
		}
		/*�˵�-ϵͳ����-ϵͳά��*/
		else if(!strcmp(szmsgtype, "gui_system_maintain"))
		{
			gui_system_maintain_set(root);
		}
		/*�˵�-ϵͳ����-CAM����*/
		else if(!strcmp(szmsgtype, "gui_system_camera_config"))
		{
			gui_system_camera_config_set(root);
		}
		/*�˵�-ϵͳ����-CAM����-PON CAM*/
		else if(!strcmp(szmsgtype, "gui_system_camera_pon_cam"))
		{
			gui_system_camera_config_set(root);
		}
		/*�˵�-ϵͳ����-��������*/
		else if(!strcmp(szmsgtype, "gui_system_passenger_flow"))
		{
			gui_system_passenger_flow_set(root);
		}
		else if(!strcmp(szmsgtype, "gui_main_page"))
		{
			gui_main_page_set(root);
		}
		else if(!strcmp(szmsgtype, "get_qr_pay_info"))
		{
			gui_qr_code_pay_info();
		}
		else if(!strcmp(szmsgtype, "gui_normal_set_info"))
		{
			gui_normal_set_info_set(root);
		}
		else if(!strcmp(szmsgtype, "gui_run_status_info"))
		{
			gui_run_status_info_get(0,NULL);
		}
		else if(!strcmp(szmsgtype, "gui_login_info"))
		{
			gui_login_info_get();
		}
		else if(!strcmp(szmsgtype, "gui_picture_play"))
		{
			picture_manage_start();
		}
	    else if(!strcmp(szmsgtype, "gui_starting_screen_flag"))
	    {
	        gui_starting_screen_flag_set(root);
	    }
		else if(!strcmp(szmsgtype, "system_notify_station"))
	    {   
	        // ������ȡstation��line��Ϣ
	        gui_station_and_line_set(root);
	    }	
		
	}
	return 0;
}


void gui_system_passenger_flow_set(cJSON *root)
{
	cfg_camara_single_chn_param_t param;
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	//PASSENGER_FLOW_PARAM_T passenger_flow;
	
	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��������*/
			/*CAM1-1ͨ������*/
			objValue = cJSON_GetObjectItem(root,"cam1_1_enable");
			if(objValue)
			{/*ʹ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.cam1_1_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_id");
			if(objValue)
			{/*ID*/
				
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_guide");
			if(objValue)
			{/*������ 0:��ѡ�� 1:ѡ��*/
				//passenger_flow.cam1_1_guide = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_count_show_mode");
			if(objValue)
			{/*������ʾģʽ  0:��׼  1:��վ*/
				//passenger_flow.cam1_1_count_show_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_count_rule");
			if(objValue)
			{/*��������  //0:�Ͻ��³�  1:�½��ϳ�*/
				param.inoutType = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_noise_filter");
			if(objValue)
			{/*��������*/
				param.noiseFilter = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_sensitivity");
			if(objValue)
			{/*������*/
				param.sensitivity = objValue->valuedouble;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_count_type");
			if(objValue)
			{/*��������*/
				param.counterType = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_install_height");
			if(objValue)
			{/*��װ�߶�*/
				param.installH = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_filter_height");
			if(objValue)
			{/*���˸߶�*/
				param.filterHeight = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_count_line1");
			if(objValue)
			{/*�ϼ�����1*/
				param.beeline[0] = objValue->valueint;

				//if (param.beeline[0] < 90)
				{
					param.beeline[0] = param.beeline[0] * 144 / 90;
				}
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_count_line2");
			if(objValue)
			{/*�¼�����2*/
				param.beeline[1] = objValue->valueint;

				//if (param.beeline[1] >= 90)
				{
					param.beeline[1] = (90 + (90 - param.beeline[1])) * 144 / 90;
				}
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_count_line3");
			if(objValue)
			{/*��߽���3*/
				param.leftLimit = objValue->valueint;

				param.leftLimit = param.leftLimit * 180 / 90;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_count_line4");
			if(objValue)
			{/*�ұ߽���4*/
				param.rightLimit = objValue->valueint;
				param.rightLimit = (90 + (90 - param.rightLimit)) * 180 / 90;
			}

			WriteChannelParam(0, &param);
			
			//CAM2-1ͨ������-----------------------------------------------
			objValue = cJSON_GetObjectItem(root,"cam2_1_enable");
			if(objValue)
			{/*ʹ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.cam2_1_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_id");
			if(objValue)
			{/*ID*/
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_guide");
			if(objValue)
			{/*������ 0:��ѡ�� 1:ѡ��*/
				//passenger_flow.cam2_1_guide = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_count_show_mode");
			if(objValue)
			{/*������ʾģʽ  0:��׼  1:��վ*/
				//passenger_flow.cam2_1_count_show_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_count_rule");
			if(objValue)
			{/*��������  //0:�Ͻ��³�  1:�½��ϳ�*/
				param.inoutType = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_noise_filter");
			if(objValue)
			{/*��������*/
				param.noiseFilter = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_sensitivity");
			if(objValue)
			{/*������*/
				param.sensitivity = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_count_type");
			if(objValue)
			{/*��������*/
				param.counterType = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_install_height");
			if(objValue)
			{/*��װ�߶�*/
				param.installH = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_filter_height");
			if(objValue)
			{/*���˸߶�*/
				param.filterHeight = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_count_line1");
			if(objValue)
			{/*�ϼ�����1*/
				param.beeline[0] = objValue->valueint;

				//if (param.beeline[0] < 90)
				{
					param.beeline[0] = param.beeline[0] * 144 / 90;
				}
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_count_line2");
			if(objValue)
			{/*�¼�����2*/
				param.beeline[1] = objValue->valueint;

				//if (param.beeline[1] >= 90)
				{
					param.beeline[1] = (90 + (90 - param.beeline[1])) * 144 / 90;
				}
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_count_line3");
			if(objValue)
			{/*��߽���3*/
				param.leftLimit = objValue->valueint;

				param.leftLimit = param.leftLimit * 180 / 90;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_count_line4");
			if(objValue)
			{/*�ұ߽���4*/
				param.rightLimit = objValue->valueint;
				param.rightLimit = (90 + (90 - param.rightLimit)) * 180 / 90;
			}
			//CAM3-1ͨ������-----------------------------------------------
			objValue = cJSON_GetObjectItem(root,"cam3_1_enable");
			if(objValue)
			{/*ʹ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.cam3_1_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_id");
			if(objValue)
			{/*ID*/
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_guide");
			if(objValue)
			{/*������ 0:��ѡ�� 1:ѡ��*/
				//passenger_flow.cam3_1_guide = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_count_show_mode");
			if(objValue)
			{/*������ʾģʽ  0:��׼  1:��վ*/
				//passenger_flow.cam3_1_count_show_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_count_rule");
			if(objValue)
			{/*��������  //0:�Ͻ��³�  1:�½��ϳ�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_noise_filter");
			if(objValue)
			{/*��������*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_sensitivity");
			if(objValue)
			{/*������*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_count_type");
			if(objValue)
			{/*��������*/
				param.counterType = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_install_height");
			if(objValue)
			{/*��װ�߶�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_filter_height");
			if(objValue)
			{/*���˸߶�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_count_line1");
			if(objValue)
			{/*�ϼ�����1*/
				//objValue->valueint;

				
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_count_line2");
			if(objValue)
			{/*�¼�����2*/
				//objValue->valueint;

				
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_count_line3");
			if(objValue)
			{/*��߽���3*/
				//objValue->valueint;

			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_count_line4");
			if(objValue)
			{/*�ұ߽���4*/
				//objValue->valueint;
			}
			//CAM4-1ͨ������-----------------------------------------------
			objValue = cJSON_GetObjectItem(root,"cam4_1_enable");
			if(objValue)
			{/*ʹ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.cam4_1_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_id");
			if(objValue)
			{/*ID*/
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_guide");
			if(objValue)
			{/*������ 0:��ѡ�� 1:ѡ��*/
				//passenger_flow.cam4_1_guide = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_count_show_mode");
			if(objValue)
			{/*������ʾģʽ  0:��׼  1:��վ*/
				//passenger_flow.cam4_1_count_show_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_count_rule");
			if(objValue)
			{/*��������  //0:�Ͻ��³�  1:�½��ϳ�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_noise_filter");
			if(objValue)
			{/*��������*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_sensitivity");
			if(objValue)
			{/*������*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_count_type");
			if(objValue)
			{/*��������*/
				param.counterType = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_install_height");
			if(objValue)
			{/*��װ�߶�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_filter_height");
			if(objValue)
			{/*���˸߶�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_count_line1");
			if(objValue)
			{/*�ϼ�����1*/
				//objValue->valueint;

				
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_count_line2");
			if(objValue)
			{/*�¼�����2*/
				//objValue->valueint;

				
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_count_line3");
			if(objValue)
			{/*��߽���3*/
				//objValue->valueint;

			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_count_line4");
			if(objValue)
			{/*�ұ߽���4*/
				//objValue->valueint;
			}
			/*�߼����ã�����ʹ��---------------------------------------------*/
			objValue = cJSON_GetObjectItem(root,"real_dynamic_statistics");
			if(objValue)
			{/*ʵʱ��̬ͳ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.real_dynamic_statistics = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"open_door_statistics");
			if(objValue)
			{/*����ͳ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.open_door_statistics = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"in_station_statistics");
			if(objValue)
			{/*��վͳ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.in_station_statistics = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"out_station_statistics");
			if(objValue)
			{/*վ��ͳ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.out_station_statistics = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"speed_statistics_enable");
			if(objValue)
			{/*�ٶ�ͳ��ʹ��  0:��ѡ�� 1:ѡ��*/
				//passenger_flow.speed_statistics_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"speed_statistics_value");
			if(objValue)
			{/*�ٶ�ͳ��ֵ*/
				//passenger_flow.speed_statistics_value = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"single_loop_error_calibration");
			if(objValue)
			{/*��Ȧ���У׼*/
				//passenger_flow.single_loop_error_calibration = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"close_delay");
			if(objValue)
			{/*������ʱ*/
				//passenger_flow.close_delay = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"time_upload_time");
			if(objValue)
			{/*��ʱ�ϱ�ʱ��*/
				//passenger_flow.time_upload_time = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"image_mosaic");
			if(objValue)
			{/*ͼ��ƴ�ӡ�0:��ѡ�� 1:ѡ��*/
				//passenger_flow.image_mosaic = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"interval_width");
			if(objValue)
			{/*�����ȡ�CM*/
				//passenger_flow.interval_width = objValue->valueint;
			}
			//passenger_flow_setting(&passenger_flow);
			WriteChannelParam(1, &param);
		
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/
			gui_system_passenger_flow_get(1, NULL);
		}
		else if(3 == objValue->valueint)
		{/*��ȡĬ������*/
			gui_system_passenger_flow_get(1, NULL);
		}
		else if(4 == objValue->valueint)
		{/*ͬ��*/
			int cam_select = 0;

			objValue = cJSON_GetObjectItem(root,"cam_select");
			if(objValue)
			{/*0: cam1_1 1:cam2_1 2: cam3_1 3:cam4_1*/
				cam_select = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"operate");
			if(objValue)
			{/*1:��ʼͬ�� 2:ȡ��ͬ��*/
				//printf("11111111111\n");
				if(1 == objValue->valueint)
				{
					int ret = LoadCameraParam(cam_select, 1);
			
					gui_system_passenger_flow_get(2, &ret);
				}
				else
				{
					LoadCameraParam(cam_select, 0);
				}
			}
			objValue = cJSON_GetObjectItem(root,"mode");
			if(objValue)
			{/*1:���� 2:����*/

			}
		}
		else if(5 == objValue->valueint)
		{/*���汳��*/
			int cam_select = 0;

			objValue = cJSON_GetObjectItem(root,"cam_select");
			if(objValue)
			{/*1: cam1_1 2:cam2_1 3: cam3_1 4:cam4_1*/
				cam_select = objValue->valueint;
			}
			
			objValue = cJSON_GetObjectItem(root,"operate");
			if(objValue)
			{/*1:�������� 2:ȡ����������*/
				if(1 == objValue->valueint)
				{
					//printf("111111111111111\n");
					ApcStartLearnBKGround(cam_select - 1);
				}
				
			}

			APCOUTPUTINFO info;

			do
			{
				//printf("cam_select = %d\n", cam_select);
				GetAlgInfo(cam_select - 1, &info);

				//printf("2222222222 info->learnBKFrame = %d\n", info.learnBKFrame);
				gui_system_passenger_flow_get(3, &info);

				usleep(100 * 1000);
			} while(info.learnBKFrame < 201);
		}
		else if(6 == objValue->valueint)
		{/*������ */
			objValue = cJSON_GetObjectItem(root,"operate");
			if(objValue)
			{/*1:���������� 2:�뿪������*/

			}
			objValue = cJSON_GetObjectItem(root,"cam_select");
			if(objValue)
			{/*1: cam1_1 2:cam2_1 3: cam3_1 4:cam4_1*/

			}
		}
		else if(7 == objValue->valueint)
		{
#if 1
			objValue = cJSON_GetObjectItem(root,"operate");

			if(objValue)
			{/*//1:������������ʾ 2:��������ʾ*/
				if (1 == objValue->valueint)
				{
					printf("1111111111");
					DisplayAlgImage(1);
					
				}
				else if (2 == objValue->valueint)
				{
					printf("000000000");
					DisplayAlgImage(0);
				}
				gui_Alg_Image_get(objValue->valueint);
			}
#endif
		}
		
	}while(0);

}

void gui_system_passenger_flow_get(int _type,void *_param)
{
	cfg_camara_single_chn_param_t param;
	
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
		  break;
	   }	

		cJSON_AddStringToObject(root, "msg_type", "gui_system_passenger_flow");

		switch(_type)
		{
			case 1:
			{/*��ȡ������Ϣ*/
				//PASSENGER_FLOW_PARAM_T passenger_flow;
				
				cJSON_AddNumberToObject(root, "type", 1);

				param = ReadChannelParam(0);
				//get_passenger_flow_param(&passenger_flow);

				int cam1_1_enable = 1;//passenger_flow.cam1_1_enable;

				

				int cam1_1_guide = 0;//passenger_flow.cam1_1_guide;
				int cam1_1_count_show_mode = 0;//passenger_flow.cam1_1_count_show_mode;
				int cam1_1_count_rule = param.inoutType;
				int cam1_1_noise_filter = param.noiseFilter;
				float cam1_1_sensitivity = param.sensitivity;
				int cam1_1_count_type = param.counterType;
				int cam1_1_install_height = param.installH;
				int cam1_1_filter_height = param.filterHeight;

				if (param.beeline[0] > 144)
				{
					param.beeline[0] = 80;
				}

				if (param.beeline[1] <= 144)
				{
					param.beeline[1] = 176;
				}
				
				int cam1_1_count_line1 = param.beeline[0] * 90 / 144;
				int cam1_1_count_line2 = (144 - (param.beeline[1] - 144)) * 90 / 144;

				if (param.leftLimit > 180)
				{
					param.leftLimit = 18;
				}

				if (param.rightLimit <= 180)
				{
					param.rightLimit = 330;
				}
				
				int cam1_1_count_line3 = param.leftLimit / 2;
				int cam1_1_count_line4 = (180 - (param.rightLimit - 180)) / 2;

				cJSON_AddNumberToObject(root, "cam1_1_enable", cam1_1_enable);/*ʹ��  0:��ѡ�� 1:ѡ��*/
				//cJSON_AddNumberToObject(root, "cam1_1_is_connect", cam1_1_is_connect);/*1:���� 2:δ����*/ /********/
				//cJSON_AddStringToObject(root, "cam1_1_id", cam1_1_id); //ID
				//cJSON_AddNumberToObject(root, "cam1_1_param_file_exist", cam1_1_param_file_exist);/*�����ļ� 1:���� 2:������*/  /********/
				cJSON_AddNumberToObject(root, "cam1_1_guide", cam1_1_guide);/*������ 0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "cam1_1_count_show_mode", cam1_1_count_show_mode);/*������ʾģʽ  0:��׼  1:��վ*/
				cJSON_AddNumberToObject(root, "cam1_1_count_rule", cam1_1_count_rule);/*��������  //0:�Ͻ��³�  1:�½��ϳ�*/
				cJSON_AddNumberToObject(root, "cam1_1_noise_filter", cam1_1_noise_filter);/*��������*/
				cJSON_AddNumberToObject(root, "cam1_1_sensitivity", cam1_1_sensitivity);/*������*/
				cJSON_AddNumberToObject(root, "cam1_1_count_type", cam1_1_count_type);/*��������*/
				cJSON_AddNumberToObject(root, "cam1_1_install_height", cam1_1_install_height);/*��װ�߶�*/
				cJSON_AddNumberToObject(root, "cam1_1_filter_height", cam1_1_filter_height);/*���˸߶�*/
				/*CAM1-1ͨ������-�������*/
				if(cam1_1_count_line1 < 0 || cam1_1_count_line1 > 90)
					cam1_1_count_line1 = 50;
				if(cam1_1_count_line2 < 0 || cam1_1_count_line2 > 90)
					cam1_1_count_line2 = 70;
				if(cam1_1_count_line3 < 0 || cam1_1_count_line3 > 90)
					cam1_1_count_line3 = 9;
				if(cam1_1_count_line4 < 0 || cam1_1_count_line4 > 90)
					cam1_1_count_line4 = 15;
				cJSON_AddNumberToObject(root, "cam1_1_count_line1",cam1_1_count_line1);/*�ϼ�����1  50*/
				cJSON_AddNumberToObject(root, "cam1_1_count_line2",cam1_1_count_line2 );/*�¼�����2 70 */
				cJSON_AddNumberToObject(root, "cam1_1_count_line3", cam1_1_count_line3);/*��߽���3 9*/
				cJSON_AddNumberToObject(root, "cam1_1_count_line4", cam1_1_count_line4);/*�ұ߽���4 15*/

				param = ReadChannelParam(1);
				
				/*CAM2-1ͨ������*/
				int cam2_1_enable = 1;//passenger_flow.cam2_1_enable;

	
	
				int cam2_1_guide = 0;//passenger_flow.cam2_1_guide;
				int cam2_1_count_show_mode = 0;//passenger_flow.cam2_1_count_show_mode;
				int cam2_1_count_rule = param.inoutType;
				int cam2_1_noise_filter = param.noiseFilter;
				float cam2_1_sensitivity = param.sensitivity;
				int cam2_1_count_type = param.counterType;
				int cam2_1_install_height = param.installH;
				int cam2_1_filter_height = param.filterHeight;

				if (param.beeline[0] > 144)
				{
					param.beeline[0] = 80;
				}

				if (param.beeline[1] < 144)
				{
					param.beeline[1] = 176;
				}
				
				int cam2_1_count_line1 = param.beeline[0] * 90 / 144;
				int cam2_1_count_line2 = (144 - (param.beeline[1] - 144)) * 90 / 144;

				if (param.leftLimit > 180)
				{
					param.leftLimit = 18;
				}

				if (param.rightLimit < 180)
				{
					param.rightLimit = 330;
				}
				
				int cam2_1_count_line3 = param.leftLimit / 2;
				int cam2_1_count_line4 = (180 - (param.rightLimit - 180)) / 2;

				if(cam2_1_count_line1 < 0 || cam2_1_count_line1 > 90)
					cam2_1_count_line1 = 50;
				if(cam2_1_count_line2 < 0 || cam2_1_count_line2 > 90)
					cam2_1_count_line2 = 70;
				if(cam2_1_count_line3 < 0 || cam2_1_count_line3 > 90)
					cam2_1_count_line3 = 9;
				if(cam2_1_count_line4 < 0 || cam2_1_count_line4 > 90)
					cam2_1_count_line4 = 15;
				cJSON_AddNumberToObject(root, "cam2_1_enable", cam2_1_enable);/*ʹ��  0:��ѡ�� 1:ѡ��*/
				//cJSON_AddNumberToObject(root, "cam2_1_is_connect", cam2_1_is_connect); /*1:���� 2:δ����*/ /********/
				//cJSON_AddStringToObject(root, "cam2_1_id", cam2_1_id); /*ID*/
				//cJSON_AddNumberToObject(root, "cam2_1_param_file_exist", cam2_1_param_file_exist);/*�����ļ� 1:���� 2:������*/  /********/
				cJSON_AddNumberToObject(root, "cam2_1_guide", cam2_1_guide);/*������ 0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "cam2_1_count_show_mode", cam2_1_count_show_mode);/*������ʾģʽ  0:��׼  1:��վ*/
				cJSON_AddNumberToObject(root, "cam2_1_count_rule", cam2_1_count_rule);/*��������  //0:�Ͻ��³�  1:�½��ϳ�*/
				cJSON_AddNumberToObject(root, "cam2_1_noise_filter", cam2_1_noise_filter);/*��������*/
				cJSON_AddNumberToObject(root, "cam2_1_sensitivity", cam2_1_sensitivity);/*������*/
				cJSON_AddNumberToObject(root, "cam2_1_count_type",cam2_1_count_type);/*��������*/
				cJSON_AddNumberToObject(root, "cam2_1_install_height", cam2_1_install_height);/*��װ�߶�*/
				cJSON_AddNumberToObject(root, "cam2_1_filter_height", cam2_1_filter_height);/*���˸߶�*/
				/*CAM2-1ͨ������-�������*/
				cJSON_AddNumberToObject(root, "cam2_1_count_line1", cam2_1_count_line1);/*�ϼ�����1*/
				cJSON_AddNumberToObject(root, "cam2_1_count_line2", cam2_1_count_line2);/*�¼�����2*/
				cJSON_AddNumberToObject(root, "cam2_1_count_line3", cam2_1_count_line3);/*��߽���3*/
				cJSON_AddNumberToObject(root, "cam2_1_count_line4", cam2_1_count_line4);/*�ұ߽���4*/

				/*CAM3-1ͨ������*/
				int cam3_1_enable = 1;//passenger_flow.cam3_1_enable;
	
				
				
				int cam3_1_guide = 0;//passenger_flow.cam3_1_guide;
				int cam3_1_count_show_mode = 0;//passenger_flow.cam3_1_count_show_mode;
				int cam3_1_count_rule = 0;
				int cam3_1_noise_filter = 0;
				float cam3_1_sensitivity = 0;
				int cam3_1_count_type = 0;
				int cam3_1_install_height = 0;
				int cam3_1_filter_height = 0;

				int cam3_1_count_line1 = 0;
				int cam3_1_count_line2 = 0;
				int cam3_1_count_line3 = 0;
				int cam3_1_count_line4 = 0;

				if(cam3_1_count_line1 < 0 || cam3_1_count_line1 > 90)
					cam3_1_count_line1 = 50;
				if(cam3_1_count_line2 < 0 || cam3_1_count_line2 > 90)
					cam3_1_count_line2 = 70;
				if(cam3_1_count_line3 < 0 || cam3_1_count_line3 > 90)
					cam3_1_count_line3 = 9;
				if(cam3_1_count_line4 < 0 || cam3_1_count_line4 > 90)
					cam3_1_count_line4 = 15;
				cJSON_AddNumberToObject(root, "cam3_1_enable", cam3_1_enable);/*ʹ��  0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "cam3_1_guide", cam3_1_guide);/*������ 0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "cam3_1_count_show_mode", cam3_1_count_show_mode);/*������ʾģʽ  0:��׼  1:��վ*/
				cJSON_AddNumberToObject(root, "cam3_1_count_rule", cam3_1_count_rule);/*��������  //0:�Ͻ��³�  1:�½��ϳ�*/
				cJSON_AddNumberToObject(root, "cam3_1_noise_filter", cam3_1_noise_filter);/*��������*/
				cJSON_AddNumberToObject(root, "cam3_1_sensitivity", cam3_1_sensitivity);/*������*/
				cJSON_AddNumberToObject(root, "cam3_1_count_type",cam3_1_count_type);/*��������*/
				cJSON_AddNumberToObject(root, "cam3_1_install_height", cam3_1_install_height);/*��װ�߶�*/
				cJSON_AddNumberToObject(root, "cam3_1_filter_height", cam3_1_filter_height);/*���˸߶�*/
				/*CAM3-1ͨ������-�������*/
				cJSON_AddNumberToObject(root, "cam3_1_count_line1", cam3_1_count_line1);/*�ϼ�����1*/
				cJSON_AddNumberToObject(root, "cam3_1_count_line2", cam3_1_count_line2);/*�¼�����2*/
				cJSON_AddNumberToObject(root, "cam3_1_count_line3", cam3_1_count_line3);/*��߽���3*/
				cJSON_AddNumberToObject(root, "cam3_1_count_line4", cam3_1_count_line4);/*�ұ߽���4*/

				/*CAM4-1ͨ������*/
				int cam4_1_enable = 1;//passenger_flow.cam4_1_enable;
				
				
				
				int cam4_1_guide = 0;//passenger_flow.cam4_1_guide;
				int cam4_1_count_show_mode = 0;//passenger_flow.cam4_1_count_show_mode;
				int cam4_1_count_rule = 0;
				int cam4_1_noise_filter = 0;
				float cam4_1_sensitivity = 0;
				int cam4_1_count_type = 0;
				int cam4_1_install_height = 0;
				int cam4_1_filter_height = 0;

				int cam4_1_count_line1 = 0;
				int cam4_1_count_line2 = 0;
				int cam4_1_count_line3 = 0;
				int cam4_1_count_line4 = 0;

				if(cam4_1_count_line1 < 0 || cam4_1_count_line1 > 90)
					cam4_1_count_line1 = 50;
				if(cam4_1_count_line2 < 0 || cam4_1_count_line2 > 90)
					cam4_1_count_line2 = 70;
				if(cam4_1_count_line3 < 0 || cam4_1_count_line3 > 90)
					cam4_1_count_line3 = 9;
				if(cam4_1_count_line4 < 0 || cam4_1_count_line4 > 90)
					cam4_1_count_line4 = 15;
				cJSON_AddNumberToObject(root, "cam4_1_enable", cam4_1_enable);/*ʹ��  0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "cam4_1_guide", cam4_1_guide);/*������ 0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "cam4_1_count_show_mode", cam4_1_count_show_mode);/*������ʾģʽ  0:��׼  1:��վ*/
				cJSON_AddNumberToObject(root, "cam4_1_count_rule", cam4_1_count_rule);/*��������  //0:�Ͻ��³�  1:�½��ϳ�*/
				cJSON_AddNumberToObject(root, "cam4_1_noise_filter", cam4_1_noise_filter);/*��������*/
				cJSON_AddNumberToObject(root, "cam4_1_sensitivity", cam4_1_sensitivity);/*������*/
				cJSON_AddNumberToObject(root, "cam4_1_count_type",cam4_1_count_type);/*��������*/
				cJSON_AddNumberToObject(root, "cam4_1_install_height", cam4_1_install_height);/*��װ�߶�*/
				cJSON_AddNumberToObject(root, "cam4_1_filter_height", cam4_1_filter_height);/*���˸߶�*/
				/*CAM4-1ͨ������-�������*/
				cJSON_AddNumberToObject(root, "cam4_1_count_line1", cam4_1_count_line1);/*�ϼ�����1*/
				cJSON_AddNumberToObject(root, "cam4_1_count_line2", cam4_1_count_line2);/*�¼�����2*/
				cJSON_AddNumberToObject(root, "cam4_1_count_line3", cam4_1_count_line3);/*��߽���3*/
				cJSON_AddNumberToObject(root, "cam4_1_count_line4", cam4_1_count_line4);/*�ұ߽���4*/
				
				/*�߼�����*/
				/*�߼����ã�����ʹ��*/
				int real_dynamic_statistics = 0;//passenger_flow.real_dynamic_statistics;
				int open_door_statistics = 0;//passenger_flow.open_door_statistics;
				int in_station_statistics = 0;//passenger_flow.in_station_statistics;
				int out_station_statistics = 0;//passenger_flow.out_station_statistics;
				int speed_statistics_enable = 0;//passenger_flow.speed_statistics_enable;
				int speed_statistics_value = 0;//passenger_flow.speed_statistics_value;
				int single_loop_error_calibration = 0;//passenger_flow.single_loop_error_calibration;
				int close_delay = 0;//passenger_flow.close_delay;
				int time_upload_time = 0;//passenger_flow.time_upload_time;
				int image_mosaic = 0;//passenger_flow.image_mosaic;
				int interval_width = 0;//passenger_flow.interval_width;
				
				cJSON_AddNumberToObject(root, "real_dynamic_statistics", real_dynamic_statistics);/*ʵʱ��̬ͳ��  0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "open_door_statistics", open_door_statistics);/*����ͳ��  0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "in_station_statistics", in_station_statistics);/*��վͳ��  0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "out_station_statistics", out_station_statistics);/*վ��ͳ��  0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "speed_statistics_enable", speed_statistics_enable);/*�ٶ�ͳ��ʹ��  0:��ѡ�� 1:ѡ��*/
				cJSON_AddNumberToObject(root, "speed_statistics_value", speed_statistics_value);/*�ٶ�ͳ��ֵ*/
				cJSON_AddNumberToObject(root, "single_loop_error_calibration", single_loop_error_calibration);/*��Ȧ���У׼*/
				cJSON_AddNumberToObject(root, "close_delay", close_delay);/*������ʱ*/
				cJSON_AddNumberToObject(root, "time_upload_time", time_upload_time);/*��ʱ�ϱ�ʱ��*/
				
				cJSON_AddNumberToObject(root, "image_mosaic", image_mosaic);/*ͼ��ƴ�ӡ�0:��ѡ�� 1:ѡ��*/
				
				cJSON_AddNumberToObject(root, "interval_width", interval_width);/*�����ȡ�CM*/
				
				
			}break;
			case 2:
			{/*ͬ�����*/
				int *data;
				int return_type = 0;
				char return_result[20];

				data = (int*)_param;

				printf("prog = %d\n", data[0]);	
				
				cJSON_AddNumberToObject(root, "type", 3);

				if(data[0] == data[1])
				{
					return_type = 1;
				}
				else if(data[0] < data[1])
				{
					return_type = 3;
				}
				
				if(1 == return_type)
				{
					cJSON_AddNumberToObject(root, "return_type", 1);/*1:ͬ���ɹ�  2:ͬ��ʧ�� 3:�������*/
				}
				else if(2 == return_type)
				{
					cJSON_AddNumberToObject(root, "return_type", 2);/*1:ͬ���ɹ�  2:ͬ��ʧ�� 3:�������*/
					cJSON_AddStringToObject(root, "return_result", return_result);/*ͬ��ʧ�ܵ�ԭ��*/
				}
				else if(3 == return_type)
				{
					
					cJSON_AddNumberToObject(root, "return_type", 3);/*1:ͬ���ɹ�  2:ͬ��ʧ�� 3:�������*/
					printf("info->learnBKFrame = %d\n", data[0] * 100/data[1]);	
					cJSON_AddNumberToObject(root, "value", data[0] * 100/data[1]); /*������� 0~100*/
				}
			}break;
			case 3:
			{/*����������*/
				int return_type = 0;
				char return_result[20];

				APCOUTPUTINFO *info = (APCOUTPUTINFO*)_param;

				printf("info->learnBKFrame = %d\n", info->learnBKFrame);	
				
				cJSON_AddNumberToObject(root, "type", 3);

				if(201 == info->learnBKFrame)
				{
					return_type = 1;
				}
				else if(info->learnBKFrame < 200)
				{
					return_type = 3;
				}
				
				if(1 == return_type)
				{
					cJSON_AddNumberToObject(root, "return_type", 1);/*1:ͬ���ɹ�  2:ͬ��ʧ�� 3:�������*/
				}
				else if(2 == return_type)
				{
					cJSON_AddNumberToObject(root, "return_type", 2);/*1:ͬ���ɹ�  2:ͬ��ʧ�� 3:�������*/
					cJSON_AddStringToObject(root, "return_result", return_result);/*ͬ��ʧ�ܵ�ԭ��*/
				}
				else if(3 == return_type)
				{
					
					cJSON_AddNumberToObject(root, "return_type", 3);/*1:ͬ���ɹ�  2:ͬ��ʧ�� 3:�������*/
					printf("info->learnBKFrame = %d\n", info->learnBKFrame * 100/201);	
					cJSON_AddNumberToObject(root, "value", info->learnBKFrame * 100/201); /*������� 0~100*/
				}
			}break;
			case 4:
			{/*��ȡRS485״̬��ID*/
				RS485STATE *rs485State;
				//InperipheralStatu_t *pInperipheralStat = (InperipheralStatu_t *)_param;
				
				
				
				

				rs485State = (RS485STATE *)_param;
					
				cJSON_AddNumberToObject(root, "type", 4);
				//printf("cam1_1_is_connect:%d,cam2_1_is_connect:%d\n",pInperipheralStat->can1_status,pInperipheralStat->can2_status);
				
				//cJSON_AddNumberToObject(root, "cam3_1_is_connect", can3_status); 
				//cJSON_AddNumberToObject(root, "cam4_1_is_connect", can4_status); 

				
					
				if(rs485State->chnl == 0/*1 == pInperipheralStat->can1_status*/)
				{
					if (rs485State->state == 1)
					{
						cJSON_AddNumberToObject(root, "cam1_1_is_connect", rs485State->state/*pInperipheralStat->can1_status*/);/*1:���� 2:δ����*/ /********/
						cJSON_AddStringToObject(root, "cam1_1_id", rs485State->cameraID);
					}
					else
					{
						cJSON_AddNumberToObject(root, "cam1_1_is_connect", rs485State->state/*pInperipheralStat->can1_status*/);/*1:���� 2:δ����*/ /********/
						cJSON_AddStringToObject(root, "cam1_1_id", "");
					}
				}					
				
				if(rs485State->chnl == 1/*1 == pInperipheralStat->can2_status*/)
				{
					if (rs485State->state == 1)
					{
						cJSON_AddNumberToObject(root, "cam2_1_is_connect", rs485State->state/*pInperipheralStat->can2_status*/); /*1:���� 2:δ����*/ /********/
						cJSON_AddStringToObject(root, "cam2_1_id", rs485State->cameraID);
					}
					else
					{
						cJSON_AddNumberToObject(root, "cam2_1_is_connect", rs485State->state/*pInperipheralStat->can2_status*/); /*1:���� 2:δ����*/ /********/
						cJSON_AddStringToObject(root, "cam2_1_id", "");
					}
				}

				if(0)
					cJSON_AddStringToObject(root, "cam3_1_id", "");
				else
					cJSON_AddStringToObject(root, "cam3_1_id", "");
				
				if(0)
					cJSON_AddStringToObject(root, "cam4_1_id", "");
				else
					cJSON_AddStringToObject(root, "cam4_1_id", "");

			}break;
			case 5:
			{
				int *chnlState;
				//int cam1_1_param_file_exist = 0;
				//int cam2_1_param_file_exist = 0;

				chnlState = (int *)_param;
				cJSON_AddNumberToObject(root, "type", 5);

				if (chnlState[0] != -1)
				{
					cJSON_AddNumberToObject(root, "cam1_1_param_file_exist", chnlState[0]);/*�����ļ� 1:���� 2:������*/  /********/
				}

				if (chnlState[1] != -1)
				{
					cJSON_AddNumberToObject(root, "cam2_1_param_file_exist", chnlState[1]);/*�����ļ� 1:���� 2:������*/  /********/
				}
				
			}break;
			case 6:
			{
				char return_result[20];
				cJSON_AddNumberToObject(root, "type", 6);
			
				cJSON_AddStringToObject(root, "return_result", return_result);
			}break;
		}
		
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
		
	}while(0);

}


void gui_system_camera_pon_cam_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��������*/

			/*PON*/
			cJSON *pJson_array = cJSON_GetObjectItem(root,"pon_rows");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*�����С*/
                tasklist=pJson_array->child;/*�Ӷ���*/
            }
			
            for(int index = 0;index < array_size;index++)  /*����10��pon*/
            {
                
                objValue = cJSON_GetObjectItem(tasklist,"config_ip");
                if(objValue && objValue->valuestring)
                {/*IP��ַ*/
                    //objValue->valuestring;
                }
                objValue = cJSON_GetObjectItem(tasklist,"config_sound_recording");
                if(objValue)
                {/*¼��  0:��������1:�ر�*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"resolving_power_main");
                if(objValue)
                {/*�ֱ��ʡ��� 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"resolving_power_sub");
                if(objValue)
                {/*�ֱ��ʡ��� 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"frame_rate_main");
                if(objValue)
                {/*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"frame_rate_sub");
                if(objValue)
                {/*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"picture_main");
                if(objValue)
                {/*���ʡ��� 0:�� 1:�� 2:��*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"picture_sub");
                if(objValue)
                {/*���ʡ��� 0:�� 1:�� 2:��*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"identification");
                if(objValue)
                {/*��ʶ 0:IPC����� 1:�����豸*/
                    //objValue->valueint; 
                }
				
				
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }
			/*CAM*/
			pJson_array = cJSON_GetObjectItem(root,"cam_rows");
            array_size = 0;
            tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*�����С*/
                tasklist=pJson_array->child;/*�Ӷ���*/
            }
			
            for(int index = 0;index < array_size;index++)  /*����8��cam*/
            {
                
                objValue = cJSON_GetObjectItem(tasklist,"config_sound_recording");
                if(objValue)
                {/*¼��  0:��������1:�ر�*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"resolving_power_main");
                if(objValue)
                {/*�ֱ��ʡ��� 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"resolving_power_sub");
                if(objValue)
                {/*�ֱ��ʡ��� 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"frame_rate_main");
                if(objValue)
                {/*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"frame_rate_sub");
                if(objValue)
                {/*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"picture_main");
                if(objValue)
                {/*���ʡ��� 0:�� 1:�� 2:��*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"picture_sub");
                if(objValue)
                {/*���ʡ��� 0:�� 1:�� 2:��*/
                    //objValue->valueint; 
                }
				objValue = cJSON_GetObjectItem(tasklist,"identification");
                if(objValue)
                {/*��ʶ 0:��Ƶ¼�� 1:����¼��*/
                    //objValue->valueint; 
                }
				
				
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }

			
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/

		}
		else if(3 == objValue->valueint)
		{/*Ĭ������*/
			objValue = cJSON_GetObjectItem(root,"cam_type");
            if(objValue)
            {/*1:PON   2:CAM*/
                //objValue->valueint; 
            }
			objValue = cJSON_GetObjectItem(root,"number");
            if(objValue)
            {/*1~10  or 1~8*/
                //objValue->valueint; 
            }
		}
		else if(4 == objValue->valueint)
		{/*����*/
			objValue = cJSON_GetObjectItem(root,"cam_type");
            if(objValue)
            {/*1:PON   2:CAM*/
                //objValue->valueint; 
            }
			objValue = cJSON_GetObjectItem(root,"number");
            if(objValue)
            {/*1~10  or 1~8*/
                //objValue->valueint; 
            }
		}

	}while(0);
}

void gui_system_camera_pon_cam_get()
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
		  break;
	   }	

		cJSON_AddStringToObject(root, "msg_type", "gui_system_camera_pon_cam");

		int type = 0; /*1:��ȡ��������  2:��ȡĬ������ 3:������ȡ��IP*/
		if(1 == type)
		{
			int array_size = 10;
			cJSON *pJson_rows_pon = cJSON_CreateArray();
			cJSON_AddItemToObject(root, "pon_rows",pJson_rows_pon);

			cJSON_AddNumberToObject(root, "type",1);
			cJSON *pJson_row = NULL;
			
			for(int index = 0;index < array_size;index++) /*pon1~pon10*/
			{
				pJson_row = cJSON_CreateObject();
				cJSON_AddItemToArray(pJson_rows_pon,pJson_row);
			
				char config_ip[20];
				int config_sound_recording = 0;
				int resolving_power_main = 0;
				int resolving_power_sub = 0;
				int frame_rate_main = 0;
				int frame_rate_sub = 0;
				int picture_main = 0;
				int picture_sub = 0;
				int identification = 0;
			
				cJSON_AddStringToObject(pJson_row, "config_ip",config_ip);/*IP��ַ*/
				cJSON_AddNumberToObject(pJson_row, "config_sound_recording",config_sound_recording);/*¼��	0:��������1:�ر�*/
				cJSON_AddNumberToObject(pJson_row, "resolving_power_main",resolving_power_main);  /*�ֱ��ʡ��� 0:1080P	1:720P 2:D1  3:CIF	4:QCIF*/
				cJSON_AddNumberToObject(pJson_row, "resolving_power_sub",resolving_power_sub);	 /*�ֱ��ʡ��� 0:1080P  1:720P 2:D1	3:CIF  4:QCIF*/
				cJSON_AddNumberToObject(pJson_row, "frame_rate_main",frame_rate_main);		 /*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
				cJSON_AddNumberToObject(pJson_row, "frame_rate_sub",frame_rate_sub);		/*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
				cJSON_AddNumberToObject(pJson_row, "picture_main",picture_main);		  /*���ʡ��� 0:�� 1:�� 2:��*/
				cJSON_AddNumberToObject(pJson_row, "picture_sub",picture_sub);			 /*���ʡ��� 0:�� 1:�� 2:��*/
				cJSON_AddNumberToObject(pJson_row, "identification",identification);		/*��ʶ 0:IPC����� 1:�����豸*/
			
			}
			
			array_size = 8;
			cJSON *pJson_rows_cam = cJSON_CreateArray();
			cJSON_AddItemToObject(root, "cam_rows",pJson_rows_cam);
			
			
			for(int index = 0;index < array_size;index++) /*cam1-1~cam4-2*/
			{
				pJson_row = cJSON_CreateObject();
				cJSON_AddItemToArray(pJson_rows_cam,pJson_row);
			
				int config_sound_recording = 0;
				int resolving_power_main = 0;
				int resolving_power_sub = 0;
				int frame_rate_main = 0;
				int frame_rate_sub = 0;
				int picture_main = 0;
				int picture_sub = 0;
				int identification = 0;
				
				cJSON_AddNumberToObject(pJson_row, "config_sound_recording",config_sound_recording);/*¼��	0:��������1:�ر�*/
				cJSON_AddNumberToObject(pJson_row, "resolving_power_main",resolving_power_main);  /*�ֱ��ʡ��� 0:1080P	1:720P 2:D1  3:CIF	4:QCIF*/
				cJSON_AddNumberToObject(pJson_row, "resolving_power_sub",resolving_power_sub);	 /*�ֱ��ʡ��� 0:1080P  1:720P 2:D1	3:CIF  4:QCIF*/
				cJSON_AddNumberToObject(pJson_row, "frame_rate_main",frame_rate_main);		 /*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
				cJSON_AddNumberToObject(pJson_row, "frame_rate_sub",frame_rate_sub);		/*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
				cJSON_AddNumberToObject(pJson_row, "picture_main",picture_main);		  /*���ʡ��� 0:�� 1:�� 2:��*/
				cJSON_AddNumberToObject(pJson_row, "picture_sub",picture_sub);			 /*���ʡ��� 0:�� 1:�� 2:��*/
				cJSON_AddNumberToObject(pJson_row, "identification",identification);		  /*��ʶ 0:��Ƶ¼�� 1:����¼��*/
			}
		
		}
		else if(2 == type)
		{
			cJSON_AddNumberToObject(root, "type",2);

			int cam_type = 0;  /*1:pon  2:cam*/
			int number = 0;  /*����ʱ�Ĳ���*/
			
			cJSON_AddNumberToObject(root, "number",number);
			
			if(1 == cam_type)
			{
				char config_ip[20];
				int config_sound_recording = 0;
				int resolving_power_main = 0;
				int resolving_power_sub = 0;
				int frame_rate_main = 0;
				int frame_rate_sub = 0;
				int picture_main = 0;
				int picture_sub = 0;
				int identification = 0;

				cJSON_AddNumberToObject(root, "cam_type",1);
			
				cJSON_AddStringToObject(root, "config_ip",config_ip);/*IP��ַ*/
				cJSON_AddNumberToObject(root, "config_sound_recording",config_sound_recording);/*¼��	0:��������1:�ر�*/
				cJSON_AddNumberToObject(root, "resolving_power_main",resolving_power_main);  /*�ֱ��ʡ��� 0:1080P	1:720P 2:D1  3:CIF	4:QCIF*/
				cJSON_AddNumberToObject(root, "resolving_power_sub",resolving_power_sub);	 /*�ֱ��ʡ��� 0:1080P  1:720P 2:D1	3:CIF  4:QCIF*/
				cJSON_AddNumberToObject(root, "frame_rate_main",frame_rate_main);		 /*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
				cJSON_AddNumberToObject(root, "frame_rate_sub",frame_rate_sub);		/*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
				cJSON_AddNumberToObject(root, "picture_main",picture_main);		  /*���ʡ��� 0:�� 1:�� 2:��*/
				cJSON_AddNumberToObject(root, "picture_sub",picture_sub);			 /*���ʡ��� 0:�� 1:�� 2:��*/
				cJSON_AddNumberToObject(root, "identification",identification);		/*��ʶ 0:IPC����� 1:�����豸*/
			}
			else
			{
				int config_sound_recording = 0;
				int resolving_power_main = 0;
				int resolving_power_sub = 0;
				int frame_rate_main = 0;
				int frame_rate_sub = 0;
				int picture_main = 0;
				int picture_sub = 0;
				int identification = 0;

				cJSON_AddNumberToObject(root, "cam_type",2);

				cJSON_AddNumberToObject(root, "config_sound_recording",config_sound_recording);/*¼��	0:��������1:�ر�*/
				cJSON_AddNumberToObject(root, "resolving_power_main",resolving_power_main);  /*�ֱ��ʡ��� 0:1080P	1:720P 2:D1  3:CIF	4:QCIF*/
				cJSON_AddNumberToObject(root, "resolving_power_sub",resolving_power_sub);	 /*�ֱ��ʡ��� 0:1080P  1:720P 2:D1	3:CIF  4:QCIF*/
				cJSON_AddNumberToObject(root, "frame_rate_main",frame_rate_main);		 /*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
				cJSON_AddNumberToObject(root, "frame_rate_sub",frame_rate_sub);		/*֡�� �� 0~24 ��Ӧֵ�� 1~25*/
				cJSON_AddNumberToObject(root, "picture_main",picture_main);		  /*���ʡ��� 0:�� 1:�� 2:��*/
				cJSON_AddNumberToObject(root, "picture_sub",picture_sub);			 /*���ʡ��� 0:�� 1:�� 2:��*/
				cJSON_AddNumberToObject(root, "identification",identification);		  /*��ʶ 0:��Ƶ¼�� 1:����¼��*/
			}

		}
		else if(3 == type)
		{
			cJSON_AddNumberToObject(root, "type",3);

			//int cam_type = 0;  /*1:pon  2:cam*/
			int number = 0;  /*����ʱ�Ĳ���*/
			char config_ip[20];
			
			cJSON_AddNumberToObject(root, "number",number);
			cJSON_AddStringToObject(root, "config_ip",config_ip);/*IP��ַ*/
			cJSON_AddNumberToObject(root, "cam_type",1);
		}

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);	
	
}


void gui_system_camera_config_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��������*/
			/*ͨ������---------------*/
			objValue = cJSON_GetObjectItem(root,"videotape_standard");/*¼����ʽ  0:PAL 1:NTSC*/
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"auto_cover");/*�Զ����� 0:��  1:��*/
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"videotape_lock");/*¼��������0:1 1:2 2:3 3:4 4:5 5:6 6:7*/
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"videotape_prev");/*¼��Ԥ¼    min*/
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"network_transmission_mode");/*����ģʽ  0:����+���� 1:�ǳ����� 2:���� 3:���� 4:�ǳ�����*/
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"videotape_storage");/*¼��洢�� 0:EMMC+����SD�� 1:EMMC  2:����SD��*/
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"videotape_mode");/*¼��ģʽ  0:����¼��  1:��������¼�� 2:������¼��*/
			{
				//objValue->valueint;
			}
			/*���ã�����ͨ��*/
			cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*�����С*/
                tasklist=pJson_array->child;/*�Ӷ���*/
            }

            for(int index = 0;index < array_size;index++)  
            {
                
                objValue = cJSON_GetObjectItem(tasklist,"value");
                if(objValue)
                {
                    //objValue->valueint; /*ֵ*/
                }
                
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }
			/*��ʾ����---------------*/
			objValue = cJSON_GetObjectItem(root,"time_show_enable");/*ʱ����ʾʹ��*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"time_show_value");/*ʱ����ʾֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"passageway_name_enable");/*ͨ������ʹ��*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"passageway_name_value");/*ͨ������ֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"vehicle_number_enable");/*�������ʹ��*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"vehicle_number_value");/*�������ֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"coordinate_info_enable");/*������Ϣʹ��*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"coordinate_info_value");/*�������ֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"real_speed_enable");/*ʵʱ�ٶ�ʹ��*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"real_speed_value");/*ʵʱ�ٶ�ֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"station_info_enable");/*վ����Ϣʹ��*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"station_info_value");/*վ����Ϣֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			/*��������ͨ������---------------*/
			objValue = cJSON_GetObjectItem(root,"network_passageway_number");/*���������ʹ��ͨ����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon1_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon1_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon2_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon2_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon3_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon3_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon4_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon4_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon5_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon5_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon6_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon6_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon7_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon7_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon8_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon8_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon9_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon9_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"pon10_ip");
			if(objValue)
			{
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"pon10_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			/*ģ�������ͨ������---------------*/
			objValue = cJSON_GetObjectItem(root,"monitor_passageway_number");/*ģ�������ʹ��ͨ����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_1_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam1_2_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_1_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam2_2_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_1_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam3_2_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_1_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"cam4_2_select_value");
			if(objValue)
			{
				//objValue->valueint;
			}
			
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/
		
		}

		
	}while(0);

}
void gui_system_camera_config_get()
{
	do
		{
		   cJSON *root = NULL;
		   root = cJSON_CreateObject();
		   if(NULL == root)
		   {   
			  break;
		   }	
	
			cJSON_AddStringToObject(root, "msg_type", "gui_system_camera_config");

			
			/*ͨ������---------------*/
			int videotape_standard = 0;
			int auto_cover = 0;
			int videotape_lock = 0;
			int videotape_prev = 0;
			int network_transmission_mode = 0;
			int videotape_storage = 0;
			int videotape_mode = 0;
			
			cJSON_AddNumberToObject(root, "videotape_standard", videotape_standard);/*¼����ʽ  0:PAL 1:NTSC*/
			cJSON_AddNumberToObject(root, "auto_cover", auto_cover);/*�Զ����� 0:��  1:��*/
			cJSON_AddNumberToObject(root, "videotape_lock",videotape_lock );/*¼��������0:1 1:2 2:3 3:4 4:5 5:6 6:7*/
			cJSON_AddNumberToObject(root, "videotape_prev", videotape_prev);/*¼��Ԥ¼	min*/
			cJSON_AddNumberToObject(root, "network_transmission_mode", network_transmission_mode);/*����ģʽ	0:����+���� 1:�ǳ����� 2:���� 3:���� 4:�ǳ�����*/
			cJSON_AddNumberToObject(root, "videotape_storage", videotape_storage);/*¼��洢�� 0:EMMC+����SD�� 1:EMMC  2:����SD��*/
			cJSON_AddNumberToObject(root, "videotape_mode", videotape_mode);/*¼��ģʽ  0:����¼��  1:��������¼�� 2:������¼��*/
			/*���ã�����ͨ��*/

			int array_size = 18;
			cJSON *pJson_rows = cJSON_CreateArray();
			cJSON_AddItemToObject(root, "rows",pJson_rows);
			
			cJSON *pJson_row = NULL;
			int value = 0;
			for(int index = 0;index < array_size;index++)
			{
				pJson_row = cJSON_CreateObject();
				cJSON_AddItemToArray(pJson_rows,pJson_row);
				cJSON_AddNumberToObject(pJson_row, "value",value);
			}
			
			
			/*��ʾ����---------------*/
			int time_show_enable = 0;
			int time_show_value = 0;
			int passageway_name_enable = 0;
			int passageway_name_value = 0;
			int vehicle_number_enable = 0;
			int vehicle_number_value = 0;
			int coordinate_info_enable = 0;
			int coordinate_info_value = 0;
			int real_speed_enable = 0;
			int real_speed_value = 0;
			int station_info_enable = 0;
			int station_info_value = 0;
			
			cJSON_AddNumberToObject(root, "time_show_enable", time_show_enable);/*ʱ����ʾʹ��*/
			cJSON_AddNumberToObject(root, "time_show_value", time_show_value);/*ʱ����ʾֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			cJSON_AddNumberToObject(root, "passageway_name_enable", passageway_name_enable);/*ͨ������ʹ��*/
			cJSON_AddNumberToObject(root, "passageway_name_value", passageway_name_value);/*ͨ������ֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			cJSON_AddNumberToObject(root, "vehicle_number_enable", vehicle_number_enable);/*�������ʹ��*/
			cJSON_AddNumberToObject(root, "vehicle_number_value",vehicle_number_value );/*�������ֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			cJSON_AddNumberToObject(root, "coordinate_info_enable", coordinate_info_enable);/*������Ϣʹ��*/
			cJSON_AddNumberToObject(root, "coordinate_info_value", coordinate_info_value);/*�������ֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			cJSON_AddNumberToObject(root, "real_speed_enable", real_speed_enable);/*ʵʱ�ٶ�ʹ��*/
			cJSON_AddNumberToObject(root, "real_speed_value", real_speed_value);/*ʵʱ�ٶ�ֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			cJSON_AddNumberToObject(root, "station_info_enable", station_info_enable);/*վ����Ϣʹ��*/
			cJSON_AddNumberToObject(root, "station_info_value",station_info_value );/*վ����Ϣֵ��0:���� 1:���� 2:���� 3:���� 4:���� 5:����*/
			
			/*��������ͨ������---------------*/
			int network_passageway_number = 0;
			char pon1_ip[20];
			int pon1_select_value = 0;
			char pon2_ip[20];
			int pon2_select_value = 0;
			char pon3_ip[20];
			int pon3_select_value = 0;
			char pon4_ip[20];
			int pon4_select_value = 0;
			char pon5_ip[20];
			int pon5_select_value = 0;
			char pon6_ip[20];
			int pon6_select_value = 0;
			char pon7_ip[20];
			int pon7_select_value = 0;
			char pon8_ip[20];
			int pon8_select_value = 0;
			char pon9_ip[20];
			int pon9_select_value = 0;
			char pon10_ip[20];
			int pon10_select_value = 0;
			
			cJSON_AddNumberToObject(root, "network_passageway_number", network_passageway_number);/*���������ʹ��ͨ����*/
			cJSON_AddStringToObject(root, "pon1_ip", pon1_ip);
			cJSON_AddNumberToObject(root, "pon1_select_value",pon1_select_value );
			cJSON_AddStringToObject(root, "pon2_ip", pon2_ip);
			cJSON_AddNumberToObject(root, "pon2_select_value", pon2_select_value);
			cJSON_AddStringToObject(root, "pon3_ip", pon3_ip);
			cJSON_AddNumberToObject(root, "pon3_select_value", pon3_select_value);
			cJSON_AddStringToObject(root, "pon4_ip", pon4_ip);
			cJSON_AddNumberToObject(root, "pon4_select_value", pon4_select_value);
			cJSON_AddStringToObject(root, "pon5_ip", pon5_ip);
			cJSON_AddNumberToObject(root, "pon5_select_value", pon5_select_value);
			cJSON_AddStringToObject(root, "pon6_ip", pon6_ip);
			cJSON_AddNumberToObject(root, "pon6_select_value", pon6_select_value);
			cJSON_AddStringToObject(root, "pon7_ip",pon7_ip );
			cJSON_AddNumberToObject(root, "pon7_select_value",pon7_select_value );
			cJSON_AddStringToObject(root, "pon8_ip", pon8_ip);
			cJSON_AddNumberToObject(root, "pon8_select_value", pon8_select_value);
			cJSON_AddStringToObject(root, "pon9_ip", pon9_ip);
			cJSON_AddNumberToObject(root, "pon9_select_value", pon9_select_value);
			cJSON_AddStringToObject(root, "pon10_ip", pon10_ip);
			cJSON_AddNumberToObject(root, "pon10_select_value",pon10_select_value );
			
			/*ģ�������ͨ������---------------*/
			int monitor_passageway_number = 0;
			int cam1_1_select_value = 0;
			int cam1_2_select_value = 0;
			int cam2_1_select_value = 0;
			int cam2_2_select_value = 0;
			int cam3_1_select_value = 0;
			int cam3_2_select_value = 0;
			int cam4_1_select_value = 0;
			int cam4_2_select_value = 0;
			
			
			cJSON_AddNumberToObject(root, "monitor_passageway_number", monitor_passageway_number);/*ģ�������ʹ��ͨ����*/
			cJSON_AddNumberToObject(root, "cam1_1_select_value", cam1_1_select_value);
			cJSON_AddNumberToObject(root, "cam1_2_select_value", cam1_2_select_value);
			cJSON_AddNumberToObject(root, "cam2_1_select_value", cam2_1_select_value);
			cJSON_AddNumberToObject(root, "cam2_2_select_value", cam2_2_select_value);
			cJSON_AddNumberToObject(root, "cam3_1_select_value", cam3_1_select_value);
			cJSON_AddNumberToObject(root, "cam3_2_select_value", cam3_2_select_value);
			cJSON_AddNumberToObject(root, "cam4_1_select_value", cam4_1_select_value);
			cJSON_AddNumberToObject(root, "cam4_2_select_value", cam4_2_select_value);

			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			
		}while(0);

}


static LTY_RESULT WriteXmlFile(LTY_STRING szPathFrom, LTY_STRING szPathDest)
{
    LTY_U32 u32Len    = 0;
    LTY_U8  *pFileBuf = NULL;
    FILE *fp       = NULL;
    size_t size    = 0;
    LTY_RESULT ret = LTY_FAILURE;    
    struct stat stStat;

    if (stat(szPathFrom, &stStat) < 0)
    {
        ERROUT("[%s] stat() error, %s\n", strerror(errno),szPathFrom);
        ret = EDATALEN;
        goto QUIT;
    }

    DBGBOUT("%s, Len %u\n",szPathFrom,(LTY_U32)stStat.st_size);

    pFileBuf = (LTY_U8 *)malloc(stStat.st_size);
    if(NULL == pFileBuf)
    {
        ERROUT("malloc buf to file fail ....\n");
        ret = ENOMEM;
        goto QUIT;
    }
    
    fp = fopen(szPathFrom, "r");
    if(NULL == fp)
    {
        WAROUT("%s Open Src File Fail !\n",szPathFrom);
        ret = EREAD;
        goto QUIT;
    }

    size = fread(pFileBuf, 1, stStat.st_size, fp);
    if (size != (LTY_U32)stStat.st_size)
    {
        WAROUT("Read File Fail ...\n");
        ret = EREAD;
        goto QUIT;
    }
    
    DBGBOUT("size=%d, stStat.st_size=%d!\n",size,stStat.st_size);

    if(fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    fp = fopen(szPathDest, "wb+");
    if(NULL == fp)
    {
        WAROUT("%s Create Fail:%s !\n",szPathDest,strerror(errno));
        if(pFileBuf != NULL)
        {
            free(pFileBuf);
            pFileBuf = NULL;
        }    
        return ECREATE;
    }

    DBGBOUT("Open %s Success !\n",szPathDest);
   
    for(LTY_U32 u32WritenByte = 0; u32WritenByte < (LTY_U32)stStat.st_size;)
    {
        LTY_U32 u32SizeRet = 0;
    
        if(stStat.st_size - u32WritenByte > (1024 * 200))
            u32Len = 1024 * 200;
        else
            u32Len = stStat.st_size - u32WritenByte;

        u32SizeRet = fwrite(&pFileBuf[u32WritenByte], 1, u32Len, fp);
        if(u32SizeRet != u32Len)
        {
            WAROUT("Write File [%s] offset %u Fail \n",szPathDest, u32WritenByte);
            ret = EWRITE;
            goto QUIT;
        }
        
        u32WritenByte += u32SizeRet;

    }

    DBGPOUT("%s Export Success!\n",szPathDest);
    ret = LTY_SUCCESS;
    
QUIT:
    if(fp)
        fclose(fp);
    
    if(pFileBuf)
    {
        free(pFileBuf);
        pFileBuf = NULL;
    }

    return ret;
}

typedef enum
{
    TYPE_CFG_XML = 0,
    TYPE_CFG_VOICE,
    TYPE_CFG_PICTURE,
    TYPE_CFG_POS_VIOCE
}BUS_CFG_E;

LTY_RESULT RmDir(LTY_STRING DstDirName)
{
    DIR *pDIR = NULL;
    struct dirent *pstDirent;
    struct stat astat;
    LTY_CHAR DstName[128] = {0};
    LTY_RESULT ret;
    
    if(0 != access(DstDirName, F_OK))
    {    
        return LTY_FAILURE;
    }

    pDIR = opendir(DstDirName);
    if(pDIR != NULL)
    {
        while((pstDirent = readdir(pDIR)) != NULL)
        {
            if(strcmp(pstDirent->d_name,".") == 0 || strcmp(pstDirent->d_name,"..") == 0 || \
               strcmp(pstDirent->d_name,"lost+found") == 0 || strcmp(pstDirent->d_name,"FactoyCfg.xml") == 0 || \
               strcmp(pstDirent->d_name,"TerminalIndivid.xml") == 0)
                continue;

            memset(DstName,0x00,sizeof(DstName));
            snprintf(DstName,sizeof(DstName),"%s/%s",DstDirName,pstDirent->d_name);
            if(stat(DstName,&astat) < 0)
            {
                continue;
            }

            if(S_ISDIR(astat.st_mode))
            {
                RmDir(DstName);
                continue;
            }

            if(S_ISREG(astat.st_mode))
            {
                unlink(DstName);
            }  
        }
        closedir(pDIR);
        return LTY_SUCCESS;
    }
    
    printf("opendir %s error",DstDirName);
    return LTY_FAILURE;
}


LTY_RESULT CopyDir(LTY_STRING SrcDirName,LTY_STRING DstDirName,LTY_BOOL DeleFile)
{
    DIR *pDIR = NULL;
    struct dirent *pstDirent;
    struct stat astat;
    LTY_CHAR SrcName[128] = {0};
    LTY_CHAR DstName[128] = {0};
    LTY_RESULT ret;
    
    if(0 != access(SrcDirName, F_OK))
    {
        return LTY_FAILURE;
    }
    printf(">>>>>>>>>>>>>>>>>>>>enter %s<<<<<<<<<<<<<<<<<<\n",SrcDirName);    
    if(0 != access(DstDirName, F_OK))
	{	 
			LOGOUT("Create %s Dir\n",DstDirName);
			ret = mkdir(DstDirName, 0777);
			if(ret != LTY_SUCCESS && errno != EEXIST)
			{
				LOGOUT("Create %s Dir Fail:%s !!!!!\n",DstDirName,strerror(errno));
				return LTY_FAILURE;
			}
            DBGPOUT("Create %s Dir OK !!!!!\n",DstDirName);
	}
    
    pDIR = opendir(SrcDirName);
    if(pDIR != NULL)
    {
        while((pstDirent = readdir(pDIR)) != NULL)
        {
            
            if(strcmp(pstDirent->d_name,".") == 0 || strcmp(pstDirent->d_name,"..") == 0 || \
               strcmp(pstDirent->d_name,"lost+found") == 0 || strcmp(pstDirent->d_name,"FactoyCfg.xml") == 0 || \
               strcmp(pstDirent->d_name,"TerminalIndivid.xml") == 0)
			    continue;
          
            memset(SrcName,0x00,sizeof(SrcName));
            memset(DstName,0x00,sizeof(DstName));
            snprintf(SrcName,sizeof(SrcName),"%s/%s",SrcDirName,pstDirent->d_name);
            snprintf(DstName,sizeof(DstName),"%s/%s",DstDirName,pstDirent->d_name);
            if(stat(SrcName,&astat) < 0)
            {
                continue;
            }

            if(S_ISDIR(astat.st_mode))
            {
                CopyDir(SrcName,DstName,DeleFile);
                continue;
            }

            if(S_ISREG(astat.st_mode))
            {
                WriteXmlFile(SrcName,DstName);
                if(DeleFile == LTY_TRUE)//��ɾ��FactoyCfg.xml�ļ�
                    unlink(SrcName);
            }  
        }
        closedir(pDIR);
        printf(">>>>>>>>>>>>>>>>>>>>exit %s<<<<<<<<<<<<<<<<<<\n",SrcDirName);
        return LTY_SUCCESS;
    }
    printf(">>>>>>>>>>>>>>>>>>>>exit %s<<<<<<<<<<<<<<<<<<\n",SrcDirName);
    printf("opendir %s error",SrcDirName);
    return LTY_FAILURE;
}

#define MOUNT_PATH_UDISK "/mnt/udisk"
#define ROOT_PATH_SDISK "/mnt/sdisk"

#define DEV_PATH_MEDIA          "/mnt/qrdata/picture"
#define DEV_PATH_POSDATA        "/mnt/posdata"
#define DEV_PATH_POSWAV         "/mnt/qrdata/wav"

#define ROOT_BUS_CFG            "/mnt/udisk/BusCfg"
#define UDISK_PATH_LINECFG      "/mnt/udisk/BusCfg/Line"
#define UDISK_PATH_POSDATA      "/mnt/udisk/BusCfg/Posdata"
#define UDISK_PATH_MEDIA        "/mnt/udisk/BusCfg/Media"
#define UDISK_PATH_POSWAV       "/mnt/udisk/BusCfg/Poswav"

static void import_station_file()
{
	bool ret_value = false;
	const char *ret_error = "";
	LTY_RESULT ret = LTY_FAILURE;
	
	do
	{
		
#ifdef PRODUCT_L12
		umount(PATH_MOUNT_SD);
#endif
        LtyMessageSend(LTY_MSG_USERIN_EXPORTIN_CFG, 0,  EXPORTIN_START, 0);
#ifdef PRODUCT_L12
        ret = mount(NOD_SD, PATH_MOUNT_SD, "ext4", MS_NOSUID | MS_NODEV, NULL);
	    if (0 != ret)
	    {
        	ret_error = "����sdiskʧ��";
            break;
	    }
#endif
		MakeDirectory(UDISK_MOUNT_PATH);
		umount(UDISK_MOUNT_PATH);
        ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, NULL/*"utf8"*/);
        if(ret != 0)
        {
        	ret_error = "����U��ʧ��";
            break;
        } 

        if(0 != access(ROOT_BUS_CFG, F_OK))
        {
            ret_error = "δ�ҵ������ļ�";
            break;
        }

        if(0 == access(UDISK_PATH_LINECFG, F_OK))
        {
            RmDir((LTY_STRING)ROOT_PATH_SDISK);
            CopyDir((LTY_STRING)UDISK_PATH_LINECFG,(LTY_STRING)ROOT_PATH_SDISK,LTY_FALSE);
        }
        
#ifdef PRODUCT_P2
        if(0 == access(UDISK_PATH_MEDIA, F_OK))
        {
            RmDir((LTY_STRING)DEV_PATH_MEDIA);
            CopyDir((LTY_STRING)UDISK_PATH_MEDIA,(LTY_STRING)DEV_PATH_MEDIA,LTY_FALSE);
        }
        
        if(0 == access(UDISK_PATH_POSDATA, F_OK))
        {
            RmDir((LTY_STRING)DEV_PATH_POSDATA);
            CopyDir((LTY_STRING)UDISK_PATH_POSDATA,(LTY_STRING)DEV_PATH_POSDATA,LTY_FALSE);
        }
        
        if(0 == access(UDISK_PATH_POSWAV, F_OK))
        {
            RmDir((LTY_STRING)DEV_PATH_POSWAV);
            CopyDir((LTY_STRING)UDISK_PATH_POSWAV,(LTY_STRING)DEV_PATH_POSWAV,LTY_FALSE);
        }
#endif 
        
		ret_value = true;
	
	}while(0);

    sync();

	if(ret_value)
	{
		gui_system_maintain_get(2,4,3,0,NULL);
	}
	else
	{
		gui_system_maintain_get(2,4,4,0,ret_error);
	}

	umount(UDISK_MOUNT_PATH);
#ifdef PRODUCT_L12
	umount(PATH_MOUNT_SD);
#endif
    LtyMessageSend(LTY_MSG_USERIN_EXPORTIN_CFG, 0,  EXPORTIN_END, 0);
	LOGOUT("***********import_station_file Finish**********\n");
}

static void export_station_file()
{
	bool ret_value = false;
	const char *ret_error = "";
	LTY_RESULT ret = LTY_FAILURE;
	
	do
	{
#ifdef PRODUCT_L12
		umount(PATH_MOUNT_SD);
#endif
        LtyMessageSend(LTY_MSG_USERIN_EXPORTIN_CFG, 0,  EXPORTIN_START, 0);
#ifdef PRODUCT_L12
        ret = mount(NOD_SD, PATH_MOUNT_SD, "ext4", MS_NOSUID | MS_NODEV, NULL);
	    if (0 != ret)
	    {
        	ret_error = "����sdiskʧ��";
            break;
	    }
#endif
		
		MakeDirectory(UDISK_MOUNT_PATH);
		umount(UDISK_MOUNT_PATH);
        ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, NULL/*"utf8"*/);
        if(ret != 0)
        {
        	ret_error = "����U��ʧ��";
            //NewMessageBox(hwnd, cur_Language ? "Udisk Mount Fail" :"����U��ʧ��", "", MB_OK|MB_ICONINFORMATION);
            break;
        }  

	
		if(0 != access(ROOT_BUS_CFG, F_OK))
		{
			WAROUT("Will Create Path %s\n",ROOT_BUS_CFG);
			ret = mkdir(ROOT_BUS_CFG, 0777);
			if(ret != 0 && errno != EEXIST)
			{
				DBGPOUT("Create %s Dir Fail !!!!!\n",ROOT_BUS_CFG);
				ret_error = ROOT_BUS_CFG;
				break;
			}
		}
	
		gui_system_maintain_get(2,6,5,0,NULL);
        

        CopyDir((LTY_STRING)ROOT_PATH_SDISK,(LTY_STRING)UDISK_PATH_LINECFG,LTY_FALSE);

#ifdef PRODUCT_P2
        CopyDir((LTY_STRING)DEV_PATH_MEDIA,(LTY_STRING)UDISK_PATH_MEDIA,LTY_FALSE);
        CopyDir((LTY_STRING)DEV_PATH_POSDATA,(LTY_STRING)UDISK_PATH_POSDATA,LTY_FALSE);
        CopyDir((LTY_STRING)DEV_PATH_POSWAV,(LTY_STRING)UDISK_PATH_POSWAV,LTY_FALSE);
#endif
        sync();
		umount(UDISK_MOUNT_PATH);
#ifdef PRODUCT_L12
		umount(PATH_MOUNT_SD);
#endif
		ret_value = true;
		LtyMessageSend(LTY_MSG_USERIN_EXPORTIN_CFG, 0,  EXPORTIN_END, 0);
		
	}while(0);

	if(ret_value)
	{
		gui_system_maintain_get(2,6,3,0,NULL);
	}
	else
	{
		gui_system_maintain_get(2,6,4,0,ret_error);
	}
	
    LOGOUT("***********export_station_file Finish**********\n");
}

static void clean_station_file()
{
	bool ret_value = false;
	const char *ret_error = "";
	LTY_RESULT ret = LTY_FAILURE;


	do
	{
#ifdef PRODUCT_L12
		umount(PATH_MOUNT_SD);
#endif
        LtyMessageSend(LTY_MSG_USERIN_EXPORTIN_CFG, 0,  EXPORTIN_START, 0);
#ifdef PRODUCT_L12
        ret = mount(NOD_SD, PATH_MOUNT_SD, "ext4", MS_NOSUID | MS_NODEV, NULL);
	    if (0 != ret)
	    {
        	ret_error = "����sdiskʧ��";
            break;
	    }
#endif
        MakeDirectory(UDISK_MOUNT_PATH);
		umount(UDISK_MOUNT_PATH);
        ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, NULL/*"utf8"*/);
        if(ret != 0)
        {
        	ret_error = "����U��ʧ��";
            //NewMessageBox(hwnd, cur_Language ? "Udisk Mount Fail" :"����U��ʧ��", "", MB_OK|MB_ICONINFORMATION);
            break;
        }  

	
		if(0 != access(ROOT_BUS_CFG, F_OK))
		{
			WAROUT("Will Create Path %s\n",ROOT_BUS_CFG);
			ret = mkdir(ROOT_BUS_CFG, 0777);
			if(ret != 0 && errno != EEXIST)
			{
				DBGPOUT("Create %s Dir Fail !!!!!\n",ROOT_BUS_CFG);
				ret_error = ROOT_BUS_CFG;
				break;
			}
            DBGPOUT("Create %s Dir OK !!!!!\n",ROOT_BUS_CFG);
		}
        
		gui_system_maintain_get(2,7,5,0,NULL);
	    
        CopyDir((LTY_STRING)ROOT_PATH_SDISK,(LTY_STRING)UDISK_PATH_LINECFG,LTY_TRUE);
        
#ifdef PRODUCT_P2
        CopyDir((LTY_STRING)DEV_PATH_MEDIA,(LTY_STRING)UDISK_PATH_MEDIA,LTY_TRUE);
        CopyDir((LTY_STRING)DEV_PATH_POSDATA,(LTY_STRING)UDISK_PATH_POSDATA,LTY_TRUE);
        CopyDir((LTY_STRING)DEV_PATH_POSWAV,(LTY_STRING)UDISK_PATH_POSWAV,LTY_FALSE);
#endif
        sync();
        umount(UDISK_MOUNT_PATH);
#ifdef PRODUCT_L12
        umount(PATH_MOUNT_SD);
#endif
        
		ret_value = true;
        LtyMessageSend(LTY_MSG_USERIN_EXPORTIN_CFG, 0,  EXPORTIN_END, 0);
		
	}while(0);
    
	if(ret_value)
	{
		gui_system_maintain_get(2,7,3,0,NULL);
	}
	else
	{
		gui_system_maintain_get(2,7,4,0,ret_error);
	}
	
    LOGOUT("***********clean_station_file Finish**********\n");
}

static void format_station_file()
{
	do
	{
		LtyMessagePost(LTY_MSG_ITSSD_SET_PARA, SD_PARA_FORMAT, 0, 0);
	}while(0);
}


/*1.�����г���¼*/
void export_driving_record()
{
	//bool ret_value = false;
	const char *ret_error = "";

	do
	{
		LTY_RESULT ret = LTY_FAILURE;
		MakeDirectory(UDISK_MOUNT_PATH);
		umount(UDISK_MOUNT_PATH);
        ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, "utf8");
        if(ret != 0)
        {
        	ret_error = "����U��ʧ��";
            //NewMessageBox(hwnd, cur_Language ? "Udisk Mount Fail" :"����U��ʧ��", "", MB_OK|MB_ICONINFORMATION);
			gui_system_maintain_get(2,8,4,0,ret_error);
			break;
        }   
        
        ret = LtyMessagePost(LTY_MSG_ITSSD_NOTIFY_EXPORT_BUSREC, 0, 0, 0);

#if 0
		if(ret == LTY_SUCCESS)
			ret_value = true;
		else
			ret_value = false;
		sync();
		umount(UDISK_MOUNT_PATH);
#endif
				
	}while(0);
#if 0
	if(ret_value)
	{
		gui_system_maintain_get(2,8,3,0,NULL);
	}
	else
	{
		gui_system_maintain_get(2,8,4,0,ret_error);
	}
#endif
	
}

/*2.����г���־*/
void clean_driving_log()
{
	bool ret_value = false;
	char *ret_error = NULL;
	LTY_RESULT ret = LTY_FAILURE;
	do
	{
#if 0
		umount(UDISK_MOUNT_PATH);
        ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, "utf8");
        if(ret != 0)
        {
        	ret_error = "����U��ʧ��";
            //NewMessageBox(hwnd, cur_Language ? "Udisk Mount Fail" :"����U��ʧ��", "", MB_OK|MB_ICONINFORMATION);
            break;
        } 
#endif
		ret = LtyMessagePost(LTY_MSG_ITSSD_SET_REC_CLEAN, 0, 0, 0);
		if(ret == LTY_SUCCESS)
			ret_value = true;
		else
			ret_value = false;
			
		
	}while(0);

	if(ret_value)
	{
		gui_system_maintain_get(2,9,3,0,NULL);
	}
	else
	{
		gui_system_maintain_get(2,9,4,0,ret_error);
	}

}

static void* export_log_ProgressBar(void *arg)
{
	pthread_detach( pthread_self());
	
	cJSON *cjson = NULL;
	
	int formatprogress = 0;

	//chnl = (int*)arg;
	
	cjson = cJSON_CreateObject();
	if(NULL == cjson)
	{
		return NULL;
	}
	

	while(1)
	{
		int status, ret;
        ret = LtyMessageSend(LTY_MSG_LOG_GETSTATUS, 0, (LTY_U32)&status, 0);
        if (LTY_SUCCESS != ret){
            gui_system_maintain_get(1,10,4,0,"�޷���ȡ������Ϣ");
            break;
        }
        if (MTSB_BACKUPREC_DOING == status){
            gui_system_maintain_get(1,10,2,30,"");
        }
        else if (MTSB_BACKUPREC_SUCC == status){
            gui_system_maintain_get(1,10,3,formatprogress,"");
			break;
        }
        else if (MTSB_NOTENOUGH_HDDSPACE == status){
            gui_system_maintain_get(1,10,4,0,"������־ʧ��");
			break;
        }
		sleep(1);
		
	}
	
	return NULL;
}


/*3.����ϵͳ��־*/
void export_system_log()
{
	do
	{
		cfg_para_dev_inf_t stdevinfo;
        LOG_BAK_S stlogbakinfo;
		int ret = 0;

		
		

		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DEV_INF, (LTY_U32)&stdevinfo, sizeof(cfg_para_dev_inf_t));
        if (0 != ret){
            ERROUT("fail to get dev info...\n");
            return;
        }
        stlogbakinfo.VehicleNo = stdevinfo.u32VehicleSN;
        ret = LtyMessageSend(LTY_MSG_USERIN_NOTIFY_EXPORTLOG, 0, (LTY_U32)&stlogbakinfo, sizeof(LOG_BAK_S));
        if (-2 == ret){
            gui_system_maintain_get(1,10,4,0,"�Ҳ���U��");
        }
		else
		{
			pthread_t export_log_tht;
			int nchn = 0;
			
			pthread_create(&export_log_tht, NULL,export_log_ProgressBar, &nchn);
		}

		
	}while(0);

}

/*4.���ϵͳ��־*/
void clean_system_log()
{
	do
	{
		
	}while(0);

}

/*5.����������־*/
void export_operate_log()
{
	do
	{
		
	}while(0);

}

/*6.���������־*/
void clean_operate_log()
{
	do
	{
		
	}while(0);

}

#define PICTURE_UDISK_PATH  "/mnt/udisk/picture"
#define PICTURE_DEVICE_PATH "/mnt/qrdata/picture"

typedef struct picture_manage_s{
	int number;
	char path[512];
	int size;
	char status;
	unsigned char *data;
}picture_manage_t;

static picture_manage_t *g_picture_manage = NULL;
static int g_picture_total = 0;
static unsigned char IsUpdataPic = 0;

static LTY_U32 picture_cache(LTY_STRING szRootFrom,LTY_STRING szRootDest, LTY_BOOL bImport);

static void* play_picture_thread(void *arg)
{
    LTY_U8 play_time = 0;
    int picture_number = 0;
    
	pthread_detach( pthread_self());
    IsUpdataPic = 0;
	do
	{
		int ret=0;
#if 0
		umount(PATH_MOUNT_ADVERT);
		int ret = mount(NOD_ADVERT, PATH_MOUNT_ADVERT, "ext4", MS_NOSUID | MS_NODEV, NULL);
		if (0 != ret)
		{
			//ret_error = "����messageʧ��";
			break;
		}
#endif
	
		/*1.��ȡͼƬ����*/
		picture_number = PictureIOProc((LTY_STRING)PICTURE_DEVICE_PATH,(LTY_STRING)PICTURE_UDISK_PATH, LTY_TRUE,LTY_TRUE,picture_number,TYPE_CFG_PICTURE);
		if(picture_number <= 0)
		{
			break;
		}
		/*2.���ݷŵ�����*/
		g_picture_total = picture_number;
		g_picture_manage = (picture_manage_t *)malloc(g_picture_total * sizeof(picture_manage_t));

		picture_cache((LTY_STRING)PICTURE_DEVICE_PATH,(LTY_STRING)PICTURE_UDISK_PATH, LTY_TRUE);

		memset(&g_stCfg_system_set, 0, sizeof(cfg_para_system_set_t));
		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYSTEM_SET, (LTY_U32)&g_stCfg_system_set, sizeof(cfg_para_system_set_t));

		play_time = g_stCfg_system_set.picutre_play_time;

		if(0 == play_time)
			play_time = 3;

		/*3.����*/
		while(1)
		{
			for(int index = 0;index < g_picture_total;index++)
			{
			#if 0
				printf("number:%d,status:%d,size:%d,path:%s\n",
					g_picture_manage[index].number,
					g_picture_manage[index].status,
					g_picture_manage[index].size,
					g_picture_manage[index].path);
			#endif

			   cJSON *root = NULL;
			   root = cJSON_CreateObject();
			   if(root )
			   {   
				   //printf("play_picture_thread  addr:0x%x\n",(unsigned int)&g_picture_manage[index]);
			   
				   cJSON_AddStringToObject(root, "msg_type", "gui_picture_play");
				   cJSON_AddNumberToObject(root, "index", g_picture_manage[index].number);		
				   cJSON_AddStringToObject(root, "path", g_picture_manage[index].path);	
				   cJSON_AddNumberToObject(root, "addr", (unsigned int)&g_picture_manage[index]);
				   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			   	}

			    sleep(play_time);
                
                if(IsUpdataPic == 1)
                {
                    LOGOUT("******Updata Pic File******\n");
                    free(g_picture_manage);
                    g_picture_manage = NULL;
                    return NULL;
                }
			}
		}

		
		
	}while(0);
	return NULL;
}

void *picture_upgrade_finish(void *arg)
{
    cfg_para_system_set_t info;
    LTY_U8 play_time = 0;
    
    memset(&info, 0, sizeof(cfg_para_system_set_t));
    LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYSTEM_SET, (LTY_U32)&info, sizeof(cfg_para_system_set_t));
    
    play_time = info.picutre_play_time;
    LOGOUT("Pic Play time %d\n",play_time);
    
    if(play_time == 0)play_time=3;
    IsUpdataPic = 1;
    sleep(play_time+2);//��ȷ���Ƿ���ý��������ȴ�ý�����ر�
    picture_manage_start();//����ý�����
}
void picture_manage_start()
{
	int nchn = 0;
	pthread_t thread_handle;

	pthread_create(&thread_handle, NULL,play_picture_thread, &nchn);
}

static LTY_BOOL get_picture_info(LTY_STRING szRootFrom,int _index)
{

    FILE *fp       = NULL;
    size_t size    = 0;
    LTY_BOOL ret_value = LTY_FALSE;
    struct stat stStat;
	
	do
	{
		if (stat(szRootFrom, &stStat) < 0)
	    {
	        ERROUT("[%s] stat() error, %s\n", strerror(errno),szRootFrom);
	        break;
	    }

	    DBGBOUT("%s, Len %u\n",szRootFrom,(LTY_U32)stStat.st_size);

	    g_picture_manage[_index].data = (unsigned char *)malloc(stStat.st_size + 1);
	    if(NULL == g_picture_manage[_index].data)
	    {
	        ERROUT("malloc buf to file fail ....\n");
	        break;
	    }

		memset(g_picture_manage[_index].data,0,sizeof(stStat.st_size + 1));
		
	    fp = fopen(szRootFrom, "r");
	    if(NULL == fp)
	    {
	        WAROUT("Open Src File [%s] Fail !\n",szRootFrom);
	        break;
	    }
	   
	    size = fread(g_picture_manage[_index].data, 1, stStat.st_size, fp);
	    if (size != (LTY_U32)stStat.st_size)
	    {
	        WAROUT("Read File Fail ...\n");
	        break;
	    }

		g_picture_manage[_index].size = stStat.st_size;
		
	    if(fp)
	    {
	        fclose(fp);
	        fp = NULL;
	    }
		ret_value = LTY_TRUE;
		
	}while(0);

	return ret_value;
}

static LTY_U32 picture_cache(LTY_STRING szRootFrom,LTY_STRING szRootDest, LTY_BOOL bImport)
{
    LOGOUT("Root : %s\n",szRootFrom);
	
	DIR *pDIR;
	struct dirent *pstDirent;
    LTY_CHAR szPathFrom[256];
    LTY_CHAR szPathDest[256];
    LTY_U32  ret_number = 0;

    
    memset(szPathFrom, 0, sizeof(szPathFrom));
    memset(szPathDest, 0, sizeof(szPathDest));

    if(LTY_TRUE == bImport)
	pDIR = opendir(szRootFrom);
    else
        pDIR = opendir(szRootDest);
	if(pDIR == NULL)
    {   
		DBGOUT("open dir failue ! \n");
        return ret_number;
    }
    else
	{	
		while((pstDirent = readdir(pDIR)) != NULL)
		{	
            if(strcmp(pstDirent->d_name,".") == 0 || strcmp(pstDirent->d_name,"..") == 0)
			    continue;
            if(bImport)
            {
			    if(NULL == strcasestr(pstDirent->d_name, ".png") &&
	               NULL ==  strcasestr(pstDirent->d_name,".jpg"))
	            {
	                continue;
	            }
			
            }

            snprintf(szPathFrom, sizeof(szPathFrom), "%s/%s",szRootFrom,pstDirent->d_name);

			g_picture_manage[ret_number].number = ret_number;
			memset(g_picture_manage[ret_number].path,0,sizeof(g_picture_manage[ret_number].path));
			strncpy(g_picture_manage[ret_number].path,szPathFrom,sizeof(g_picture_manage[ret_number].path));

			if(LTY_TRUE == get_picture_info(szPathFrom,ret_number))
				g_picture_manage[ret_number].status = 1;
			else
				g_picture_manage[ret_number].status = 0;

			ret_number++;
            
            
		}
        
		closedir(pDIR);	
	}
    
	return ret_number;
}


static LTY_U32 PictureIOProc(LTY_STRING szRootFrom,LTY_STRING szRootDest, LTY_BOOL bImport,LTY_BOOL bNumber,LTY_U32 picture_total,LTY_CHAR _type)
{
    LOGOUT("Root : %s\n",szRootFrom);
	
	DIR *pDIR;
	struct dirent *pstDirent;
    LTY_CHAR szPathFrom[256];
    LTY_CHAR szPathDest[256];
    LTY_U32  ret_number = 0;
    LTY_RESULT ret = LTY_FAILURE;
    
    memset(szPathFrom, 0, sizeof(szPathFrom));
    memset(szPathDest, 0, sizeof(szPathDest));

    if(LTY_TRUE == bImport)
	pDIR = opendir(szRootFrom);
    else
        pDIR = opendir(szRootDest);
	if(pDIR == NULL)
    {   
		DBGOUT("open dir failue ! \n");
        return ret_number;
    }
    else
	{	
		while((pstDirent = readdir(pDIR)) != NULL)
		{	
            if(strcmp(pstDirent->d_name,".") == 0 || strcmp(pstDirent->d_name,"..") == 0)
			    continue;
            if(bImport)
            {
            #if 0
                if(TYPE_CFG_XML == enType)
                {
        			if(NULL == strcasestr(pstDirent->d_name, ".xml") ||
                       NULL !=  strcasestr(pstDirent->d_name,".bak"))
                    {
                        continue;
                    }
                }
                else
                {
        			if(NULL == strcasestr(pstDirent->d_name, ".lau") ||
                       NULL == strcasestr(pstDirent->d_name, ".LAU") ||
                       NULL !=  strcasestr(pstDirent->d_name,".bak"))
                    {
                        continue;
                    }
                }
			#endif

				if(TYPE_CFG_PICTURE == _type)
				{
					if(NULL == strcasestr(pstDirent->d_name, ".png") &&
		               NULL ==  strcasestr(pstDirent->d_name,".jpg"))
		            {
		                continue;
		            }
				}
				else if(TYPE_CFG_POS_VIOCE == _type)
				{
					if(NULL == strcasestr(pstDirent->d_name, ".wav"))
		            {
		                continue;
		            }				
				}
			    
			
            }
            
            //LOGOUT("Find : %s\n",pstDirent->d_name);
            
            snprintf(szPathFrom, sizeof(szPathFrom), "%s/%s",szRootFrom,pstDirent->d_name);
            snprintf(szPathDest, sizeof(szPathDest), "%s/%s",szRootDest,pstDirent->d_name);

            /*
            ��ʼ���룬���
            */
            if(bNumber)
            {
				ret_number++;
            }
			else
			{
				if(TYPE_CFG_PICTURE == _type)
				{
					LTY_U32 percent_val = 0;
					ret_number++;
					if(picture_total > 0)
						percent_val = (ret_number * 100)/picture_total;
					
					if(LTY_TRUE == bImport)
		            {
		                ret = WriteXmlFile(szPathFrom,szPathDest);
		                if(LTY_SUCCESS != ret)
		                    WAROUT("Import [%s] to [%s] Fail\n",szPathFrom,szPathDest);
		            }
		            else
		            {
		                //LOGOUT("remove %s ..\n",szPathDest);
		                if (unlink(szPathDest) < 0)
		                    WAROUT("remove %s failed\n", szPathDest);
		            }
					gui_system_maintain_get(1,18,2,percent_val,"");
					sleep(1);
				}
				else if(TYPE_CFG_POS_VIOCE == _type)
				{
					if(LTY_TRUE == bImport)
					{
						ret = WriteXmlFile(szPathFrom,szPathDest);
		                if(LTY_SUCCESS != ret)
		                    WAROUT("Import [%s] to [%s] Fail\n",szPathFrom,szPathDest);
					}
				}
				
			}
            
            
		}
        
		closedir(pDIR);	
	}
    
	return ret_number;
}



static void* import_picture_thread(void *arg)
{
	pthread_detach( pthread_self());
	const char *ret_error = "";
	LTY_U32 picture_number = 0;
	LTY_BOOL result = LTY_FALSE;
	LTY_RESULT ret = LTY_FAILURE;
	do
	{
		/*1.����U��*/
		umount(UDISK_MOUNT_PATH);
        ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, "utf8");
        if(ret != 0)
        {
        	ret_error = "����U��ʧ��";
            //NewMessageBox(hwnd, cur_Language ? "Udisk Mount Fail" :"����U��ʧ��", "", MB_OK|MB_ICONINFORMATION);
            break;
        }   
	#if 0	
		umount(PATH_MOUNT_ADVERT);
		ret = mount(NOD_ADVERT, PATH_MOUNT_ADVERT, "ext4", MS_NOSUID | MS_NODEV, NULL);
		if (0 != ret)
		{
			ret_error = "����messageʧ��";
			break;
		}
	#endif

		if(0 != access(PICTURE_UDISK_PATH, F_OK))
		{
			ERROUT("Can't Find Bus Cfg Root Path !!!\n");
			ret_error = "U�̲�����ͼƬ";
			break;
		}
		
		if(0 != access(PICTURE_DEVICE_PATH, F_OK))
		{	 
			WAROUT("Can't Find Bus Cfg Root Path ,Will Create Path %s\n",PICTURE_DEVICE_PATH);
			ret = mkdir("/mnt/qrdata/message", 0777);
			ret = mkdir(PICTURE_DEVICE_PATH, 0777);
			if(ret != 0 && errno != EEXIST)
			{
				DBGPOUT("Create %s Dir Fail !!!!!\n",PICTURE_DEVICE_PATH);
				ret_error = "�豸�ϴ���Ŀ¼ʧ��";
				break;
			}
		}
		
		/*2.�����Ƿ���ͼƬ����*/
		picture_number = PictureIOProc((LTY_STRING)PICTURE_UDISK_PATH,(LTY_STRING)PICTURE_DEVICE_PATH, LTY_TRUE,LTY_TRUE,picture_number,TYPE_CFG_PICTURE);
		if(picture_number <= 0)
		{
			ret_error = "û��ͼƬ����";
			break;
		}

		/*3.��ʼ����ͼƬ*/
		PictureIOProc((LTY_STRING)PICTURE_UDISK_PATH,(LTY_STRING)PICTURE_DEVICE_PATH, LTY_TRUE,LTY_FALSE,picture_number,TYPE_CFG_PICTURE);
		/*4.�������*/
		result = LTY_TRUE;

		sync();
		umount(UDISK_MOUNT_PATH);
		umount(PATH_MOUNT_ADVERT);
		
	}while(0);
	
	if(LTY_FALSE == result)
		gui_system_maintain_get(1,18,4,0,ret_error);
	else
		gui_system_maintain_get(1,18,3,0,"");

	return NULL;
}

/*������ͼƬ*/
void import_advert_picture()
{
	int nchn = 0;
	pthread_t thread_handle;

	pthread_create(&thread_handle, NULL,import_picture_thread, &nchn);
}

static void* export_picture_thread(void *arg)
{
	pthread_detach( pthread_self());
	
	const char *ret_error = "";
	LTY_U32 picture_number = 0;
	LTY_BOOL result = LTY_FALSE;
	LTY_RESULT ret = LTY_FAILURE;
	
	do
	{
		/*1.����U��*/
		umount(UDISK_MOUNT_PATH);
        ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, "utf8");
        if(ret != 0)
        {
        	ret_error = "����U��ʧ��";
            //NewMessageBox(hwnd, cur_Language ? "Udisk Mount Fail" :"����U��ʧ��", "", MB_OK|MB_ICONINFORMATION);
            break;
        }   
	//	umount(PATH_MOUNT_ADVERT);
	//	ret = mount(NOD_ADVERT, PATH_MOUNT_ADVERT, "ext4", MS_NOSUID | MS_NODEV, NULL);
	//	if (0 != ret)
	//	{
	//		ret_error = "����messageʧ��";
	//		break;
	//	}

		if(0 != access(PICTURE_DEVICE_PATH, F_OK))
		{
			ERROUT("Can't Find Bus Cfg Root Path !!!\n");
			ret_error = "�豸�ϲ�����ͼƬ";
			break;
		}
		
		if(0 != access(PICTURE_UDISK_PATH, F_OK))
		{	 
			WAROUT("Can't Find Bus Cfg Root Path ,Will Create Path %s\n",PICTURE_UDISK_PATH);
			ret = mkdir("/mnt/qrdata/message", 0777);
			ret = mkdir(PICTURE_UDISK_PATH, 0777);
			if(ret != 0 )
			{
				DBGPOUT("Create %s Dir Fail !!!!!\n",PICTURE_UDISK_PATH);
				ret_error = "U�̴���Ŀ¼ʧ��";
				break;
			}
		}
		
		/*2.�����Ƿ���ͼƬ����*/
		picture_number = PictureIOProc((LTY_STRING)PICTURE_DEVICE_PATH,(LTY_STRING)PICTURE_UDISK_PATH, LTY_TRUE,LTY_TRUE,picture_number,TYPE_CFG_PICTURE);
		if(picture_number <= 0)
		{
			ret_error = "û��ͼƬ����";
			break;
		}

		/*3.��ʼ����ͼƬ*/
		PictureIOProc((LTY_STRING)PICTURE_DEVICE_PATH,(LTY_STRING)PICTURE_UDISK_PATH, LTY_TRUE,LTY_FALSE,picture_number,TYPE_CFG_PICTURE);
		/*4.�������*/
		result = LTY_TRUE;
		sync();
		umount(UDISK_MOUNT_PATH);
		
	}while(0);
	
	if(LTY_FALSE == result)
		gui_system_maintain_get(1,19,4,0,ret_error);
	else
		gui_system_maintain_get(1,19,3,0,"");

	return NULL;
}

/*�������ͼƬ*/
void export_advert_picture()
{
	int nchn = 0;
	pthread_t thread_handle;

	pthread_create(&thread_handle, NULL,export_picture_thread, &nchn);
}

static void* clean_picture_thread(void *arg)
{
	pthread_detach( pthread_self());

	
	const char *ret_error = "";
	LTY_U32 picture_number = 0;
	LTY_BOOL result = LTY_FALSE;

	
	do
	{
	#if 0
		umount(PATH_MOUNT_ADVERT);
		int ret = mount(NOD_ADVERT, PATH_MOUNT_ADVERT, "ext4", MS_NOSUID | MS_NODEV, NULL);
		if (0 != ret)
		{
			ret_error = "����messageʧ��";
			break;
		}
	#endif
  
		if(0 != access(PICTURE_DEVICE_PATH, F_OK))  
		{
			ERROUT("Can't Find Bus Cfg Root Path !!!\n");
			ret_error = "�豸�ϲ�����ͼƬ";
			break;
		}
		

		/*1.�����Ƿ���ͼƬɾ��*/
		picture_number = PictureIOProc((LTY_STRING)PICTURE_UDISK_PATH,(LTY_STRING)PICTURE_DEVICE_PATH, LTY_FALSE,LTY_TRUE,picture_number,TYPE_CFG_PICTURE);
		if(picture_number <= 0)
		{
			ret_error = "û��ͼƬɾ��";
			break;
		}

		/*2.��ʼɾ��ͼƬ*/
		PictureIOProc((LTY_STRING)PICTURE_DEVICE_PATH,(LTY_STRING)PICTURE_DEVICE_PATH, LTY_FALSE,LTY_FALSE,picture_number,TYPE_CFG_PICTURE);
		/*3.ɾ������*/
		result = LTY_TRUE;

		
		umount(UDISK_MOUNT_PATH);
		
	}while(0);
	
	if(LTY_FALSE == result)
		gui_system_maintain_get(1,20,4,0,ret_error);
	else
		gui_system_maintain_get(1,20,3,0,"");
	return NULL;
}

/*������ͼƬ*/
void clean_advert_picture()
{
	int nchn = 0;
	pthread_t thread_handle;

	pthread_create(&thread_handle, NULL,clean_picture_thread, &nchn);
}



void gui_system_maintain_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
    LTY_RESULT ret = LTY_FAILURE;

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��ȡ�б�����*/

#if 0
			objValue = cJSON_GetObjectItem(root,"main_type");
			if(NULL == objValue)
			{
				break;
			}
			if(1 == objValue->valueint)
			{/*Ӧ�ó���*/
			}

			else if(2 == objValue->valueint)
			{/*MCU����*/
			}
			else if(3 == objValue->valueint)
			{/*UBOOT����*/
			}
			else if(4 == objValue->valueint)
			{/*���뱨վ�ļ�*/
			}
			else if(5 == objValue->valueint)
			{/*���뿪��LOGO�ļ�*/
			}
#endif			

		
		}
		else if(2 == objValue->valueint)
		{/*�û�����*/
			objValue = cJSON_GetObjectItem(root,"main_type");
			if(NULL == objValue)
			{
				break;
			}
			cJSON *objValue1 = NULL;
			objValue1 = cJSON_GetObjectItem(root,"behavior_operate");
			if(NULL == objValue1)
			{
				break;
			}
			int behavior_operate = objValue1->valueint;
			
			if(1 == objValue->valueint)
			{/*Ӧ�ó���*/
				if(1 == behavior_operate)
				{/*����*/
					upg_attr_local_t upgrade_path;
					int str_len = 0;
					
					memset(&upgrade_path, 0, sizeof(upg_attr_local_t));
					objValue = cJSON_GetObjectItem(root,"file_path");	 
					if(objValue != NULL && objValue->valuestring)
					{
						str_len = strlen(objValue->valuestring);
						if(str_len > 104)str_len=104;
						memcpy(upgrade_path.szFile, objValue->valuestring, str_len);	

						LTY_RESULT ret = LtyMessageSend(LTY_MSG_UPG_SET_SYSTEM_UPGRADE, UPG_MOD_LOCAL, (LTY_U32)&upgrade_path, sizeof(upgrade_path));
						DBGBOUT("ret = %d \n", ret);
					}
					
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			#if 0
			else if(2 == objValue->valueint)
			{/*MCU����*/
				if(1 == behavior_operate)
				{/*����*/
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(3 == objValue->valueint)
			{/*UBOOT����*/
				if(1 == behavior_operate)
				{/*����*/
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			#endif
			else if(4 == objValue->valueint)
			{/*���뱨վ�ļ�*/
				if(1 == behavior_operate)
				{/*����*/
					import_station_file();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			#if 0
			else if(5 == objValue->valueint)
			{/*���뿪��LOGO�ļ�*/
				if(1 == behavior_operate)
				{/*����*/
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			#endif
			else if(6 == objValue->valueint)
			{/*������վ�ļ�*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
					export_station_file();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(7 == objValue->valueint)
			{/*�����վ�ļ��汾*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
					clean_station_file();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			/*��־����-----------------------------------------------------------------start*/
			else if(8 == objValue->valueint)  
			{/*�����г���¼*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
					export_driving_record();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(9 == objValue->valueint)
			{/*����г���־*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
					clean_driving_log();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(10 == objValue->valueint)
			{/*����ϵͳ��־*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
					export_system_log();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(11 == objValue->valueint)
			{/*���ϵͳ��־*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
					clean_system_log();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(12 == objValue->valueint)
			{/*����������־*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
					export_operate_log();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(13 == objValue->valueint)
			{/*���������־*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
					clean_operate_log();
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			/*��־����-----------------------------------------------------------------end*/
			/*�߼�*/
			#if 0
			else if(14 == objValue->valueint)
			{/*�ָ�Ӧ�ó���*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(15 == objValue->valueint)
			{/*�ָ�MCU����*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(16 == objValue->valueint)
			{/*�ָ�UBoot����*/
				if(1 == behavior_operate)
				{/*ִ�й���*/
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			#endif
			else if(17 == objValue->valueint)
			{/*�ָ���������*/

				printf("gui_system_maintain_set() resume setting\n");
				if(1 == behavior_operate)
				{/*ִ�й���*/
					printf("gui_system_maintain_set() resume setting 1\n");
					ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_DEFAULT, 0, 0, 0);
					if(ret == LTY_SUCCESS)
						gui_system_maintain_get(2,17,3,0,NULL);
					else
						gui_system_maintain_get(2,17,4,0,"�ָ�ʧ��");
					
				}
				else if(2 == behavior_operate)
				{/*�û�ȡ��*/
				}
			}
			else if(18 == objValue->valueint)
			{/*������ͼƬ*/
				import_advert_picture();
			}
			else if(19 == objValue->valueint)
			{/*�������ͼƬ*/
				export_advert_picture();
			}
			else if(20 == objValue->valueint)
			{/*������ͼƬ*/
				clean_advert_picture();
			}
			else if(21 == objValue->valueint) 
			{/*����ˢ������*/
				if(1 == behavior_operate)
				{
					ret = LtyMessageSend(LTY_MSG_FLASH_CARD, EXPORTIN_POSPARAM, 0, 0);
					if(ret == LTY_SUCCESS)
						gui_system_maintain_get(2,21,3,0,NULL);
					else
						gui_system_maintain_get(2,21,4,0,"����ˢ������ʧ��");
				}

			}
			else if(22 == objValue->valueint)
			{/*����ˢ����¼*/
				if(1 == behavior_operate)
				{
	                const char *ret_error = "";
                    MakeDirectory(UDISK_MOUNT_PATH);
                    umount(UDISK_MOUNT_PATH);
                    ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, "utf8");
                    if(ret != 0)
                    {
                        ret_error = "����U��ʧ��";
						gui_system_maintain_get(2,22,4,0,ret_error);
                        break;
                    }  
				
                    ret = LtyMessageSend(LTY_MSG_FLASH_CARD, EXPORTOUT_POSRECORD, 0, 0);
					if(ret == LTY_SUCCESS)
						gui_system_maintain_get(2,22,3,0,NULL);
					else
						gui_system_maintain_get(2,22,4,0,"����ˢ����¼ʧ��");
	                
                    sync();
                    umount(UDISK_MOUNT_PATH);
				}
				
			}
			else if(23 == objValue->valueint)
			{/*���ˢ������*/
				if(1 == behavior_operate)
				{
					ret = LtyMessageSend(LTY_MSG_FLASH_CARD, CLEAN_POSPARAM, 0, 0);
					if(ret == LTY_SUCCESS)
						gui_system_maintain_get(2,23,3,0,NULL);
					else
						gui_system_maintain_get(2,23,4,0,"���ˢ������ʧ��");
				}
				
			}
			else if(24 == objValue->valueint)
			{/*���ˢ����¼*/
				if(1 == behavior_operate)
				{
	                const char *ret_error = "";
                    MakeDirectory(UDISK_MOUNT_PATH);
                    umount(UDISK_MOUNT_PATH);
                    ret = mount(UDISK_MOUNT_NOD, UDISK_MOUNT_PATH, "vfat", MS_NOSUID | MS_NODEV, "utf8");
                    if(ret != 0)
                    {
                        ret_error = "����U��ʧ��";
						gui_system_maintain_get(2,24,4,0,ret_error);
                        break;
                    }  

					ret = LtyMessageSend(LTY_MSG_FLASH_CARD, CLEAN_POSRECORD, 0, 0);
					if(ret == LTY_SUCCESS)
						gui_system_maintain_get(2,24,3,0,NULL);
					else
						gui_system_maintain_get(2,24,4,0,"���ˢ����¼ʧ��");
                    
	                sync();
                    umount(UDISK_MOUNT_PATH);
				}
				
			}
			else if(25 == objValue->valueint)
			{/*��ʽ����վ����*/
				if(1 == behavior_operate)
				{
					format_station_file();
				}
				
			}
		}
		
		
	}while(0);

}
/*
void gui_system_maintain_get(char _type,char _main_type,char _operate_type,char _percent,char *_failed_result)
char _type			:1:�б����  2:���ܲ���
char _main_type		:1:Ӧ�ó���
char _operate_type 	:1:��ȡ���� 2:��ȡ���� 3:�����ɹ� 4:����ʧ�� 5:���ڲ���
char _percent      	:����


*/
void gui_system_maintain_get(char _type,char _main_type,char _operate_type,char _percent,const char *_failed_result)
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	

	   	cJSON_AddStringToObject(root, "msg_type", "gui_system_maintain");

		//int type = 0; /*1:�б����  2:���ܲ���*/
		//int main_type = 0;
		//int operate_type = 0;/*1:��ȡ���� 2:��ȡ���� 3:�����ɹ� 4:����ʧ�� 5:���ڲ���*/
		switch(_type)
		{
			case 1:
			{
				cJSON_AddNumberToObject(root, "type", 1);
				cJSON_AddNumberToObject(root, "main_type", _main_type);
				cJSON_AddNumberToObject(root, "operate_type", _operate_type);
				if(1 == _main_type)
				{/*Ӧ�ó���*/
					if(1 == _operate_type)
					{/*��ȡ����*/
						char path[20];
						int number = 0;
						cJSON_AddNumberToObject(root, "number", number);/*�ļ���ţ���1��ʼ*/
						cJSON_AddStringToObject(root, "path", path);/*�ļ�·��*/
					}
					else if(2 == _operate_type)
					{/*��ȡ����*/
						//int percent = 0;/* 0~100*/
						cJSON_AddNumberToObject(root, "percent", _percent);
					}
					else if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/

						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
				}
				else if(10 == _main_type || /*����ϵͳ��־*/
						18 == _main_type || /*������ͼƬ*/
						19 == _main_type || /*�������ͼƬ*/
						20 == _main_type)   /*������ͼƬ*/
				{

					if(2 == _operate_type)
					{/*��ȡ����*/
						//int percent = 0;/* 0~100*/
						cJSON_AddNumberToObject(root, "percent", _percent);
					}
					else if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/

						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
				}
				
			}break;
			case 2:
			{
				cJSON_AddNumberToObject(root, "type", 2);
				cJSON_AddNumberToObject(root, "main_type", _main_type);
				cJSON_AddNumberToObject(root, "operate_type", _operate_type);

				if(4 == _main_type)
				{/*���뱨վ�ļ�*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/

						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;

						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(6 == _main_type)
				{/*������վ�ļ�*/  
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
					
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;

						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(7 == _main_type)
				{/*�����վ�ļ�*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
					
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;
	
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(8 == _main_type)
				{/*�����г���¼*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/

						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;

						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(9 == _main_type)
				{/*����г���־*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
			
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;

						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(10 == _main_type)
				{/*����ϵͳ��־*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
			
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;

						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(11 == _main_type)
				{/*���ϵͳ��־*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
			
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;

						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(12 == _main_type)
				{/*����������־*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
					
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;
					
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(13 == _main_type)
				{/*���������־*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
						
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;
				
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(14 == _main_type)
				{/*�ָ�Ӧ�ó���*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
						
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;
			
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(15 == _main_type)
				{/*�ָ�MCU����*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
						
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;
					
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(16 == _main_type)
				{/*�ָ�UBoot����*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
						
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;
				
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(17 == _main_type)
				{/*�ָ���������*/
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
						
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;
					
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				else if(21 == _main_type ||   //����ˢ������
					    22 == _main_type ||   //����ˢ����¼
					    23 == _main_type ||   //���ˢ������
					    24 == _main_type ||   //���ˢ����¼
					    25 == _main_type)     //��ʽ����վ����
				{
					if(3 == _operate_type)
					{/*�����ɹ�*/
						int result_value = 1;/* 1:�ɹ� 2:ʧ��*/
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
					else if(4 == _operate_type)
					{/*����ʧ��*/
						int result_value = 2;/* 1:�ɹ� 2:ʧ��*/
						//char result_str[20];
						cJSON_AddNumberToObject(root, "result_value", result_value);
						cJSON_AddStringToObject(root, "result_str", _failed_result);/*ʧ��ԭ��*/
					}
					else if(5 == _operate_type)
					{/*���ڲ���*/
						int result_value = 3;
					
						cJSON_AddNumberToObject(root, "result_value", result_value);
					}
				}
				
			}break;
		}	

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}

static void set_system_time(int _year,int _mon,int _mday,int _hour,int _min,int _sec)
{
	struct tm tm;
    time_t t;


    tm.tm_year  = _year - 1900;
    tm.tm_mon   = _mon - 1;
    tm.tm_mday  = _mday;
    tm.tm_hour  = _hour;
    tm.tm_min   = _min;
    tm.tm_sec   = _sec;
    tm.tm_wday  = 0;
    tm.tm_yday  = 0;
    tm.tm_isdst = 0;

	printf("set_system_time tm.tm_year: %d tm.tm_mon:%d tm.tm_mday:%d tm.tm_hour:%d tm.tm_min:%d tm.tm_sec:%d\n",
		   tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour,tm.tm_min,tm.tm_sec);

	t = mktime(&tm);
   if (t >= 0){
        //ret = LTY_SUCCESS;
        LOGOUT("success to set time %04d-%02d-%02d %02d:%02d:%02d...\n",
               _year, _mon, _mday, _hour, _min, _sec);
		LtyMessagePost(LTY_MSG_SYSCTRL_SETTIME_EXTEND, 0, 0, 0);
        LtyMessagePost(LTY_MSG_SYSCTRL_SETTIME, 0, (LTY_U32)&t, sizeof(LTY_U32));
    }
    else{
        //ret = LTY_FAILURE;
        ERROUT("fail to set time %04d-%02d-%02d %02d:%02d:%02d...\n",
               _year, _mon, _mday, _hour, _min, _sec);
    }
}

void system_setup_common_set(cJSON *root)
{
	cJSON *objValue = NULL;
	do
	{
		objValue = cJSON_GetObjectItem(root,"current_time_zone");
		if(objValue)
		{/*//��ǰѡ���ʱ��*/
		   //objValue->valueint;
		   	m_stCfg_DevInfo.u8eartharea = objValue->valueint;
    		m_stCfg_DevInfo.u8earthdirect = 0;
		   #if 0
		   memset(&g_stCfg_system_set.current_time_zone,0,sizeof(g_stCfg_system_set.current_time_zone));
		   memcpy(g_stCfg_system_set.current_time_zone,objValue->valuestring,sizeof(g_stCfg_system_set.current_time_zone));
		   #endif
		}
		#if 0
		objValue = cJSON_GetObjectItem(root,"time1");
		if(objValue && objValue->valuestring)
		{/*ʱ��1*/
		   //objValue->valuestring;
		}
		objValue = cJSON_GetObjectItem(root,"time2");
		if(objValue && objValue->valuestring)
		{/*ʱ��2*/
		   //objValue->valuestring;
		}
		#endif
		objValue = cJSON_GetObjectItem(root,"satellite_time_enable");
		if(objValue)
		{/*����Уʱ  0:δѡ��״̬ 1:ѡ��״̬*/
		   g_stCfg_system_set.satellite_time_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"center_time_enable");
		if(objValue)
		{/*����Уʱ  0:δѡ��״̬ 1:ѡ��״̬*/
		   g_stCfg_system_set.center_time_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"ntp_enable");
		if(objValue)
		{/*/NTPУʱʹ��  0:δѡ��״̬ 1:ѡ��״̬*/
		   g_stCfg_system_set.ntp_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"ntp_value");
		if(objValue)
		{/*NTPУʱֵ 0:time.windows.com 1:time.nist.gov 2:time-nw.nist.gov 3:time-a.nist.gov 4:time-b.nist.gov*/
		   g_stCfg_system_set.ntp_value = objValue->valueint;
		}

		objValue = cJSON_GetObjectItem(root,"user_set_time");
		if(objValue)
		{
			if(1 == objValue->valueint)
			{
				int tm_year = 0;
			    int tm_mon = 0;
			    int tm_mday = 0;
			    int tm_hour = 0;
			    int tm_min = 0;
			    int tm_sec = 0;
			   
			    
				objValue = cJSON_GetObjectItem(root,"tm_year");
				if(objValue)
				{
					tm_year = objValue->valueint;
				}
				objValue = cJSON_GetObjectItem(root,"tm_mon");
				if(objValue)
				{
					tm_mon = objValue->valueint;
				}
				objValue = cJSON_GetObjectItem(root,"tm_mday");
				if(objValue)
				{
					tm_mday = objValue->valueint;
				}
				objValue = cJSON_GetObjectItem(root,"tm_hour");
				if(objValue)
				{
					tm_hour = objValue->valueint;
				}
				objValue = cJSON_GetObjectItem(root,"tm_min");
				if(objValue)
				{
					tm_min = objValue->valueint;
				}
				objValue = cJSON_GetObjectItem(root,"tm_sec");
				if(objValue)
				{
					tm_sec = objValue->valueint;
				}
				printf("set_system_time()\n");
				set_system_time(tm_year,tm_mon,tm_mday,tm_hour,tm_min,tm_sec);
			}
		}
		int ret = 0;

		objValue = cJSON_GetObjectItem(root,"use_password");
		if(objValue)
		{/*ʹ������  0:δѡ��״̬ 1:ѡ��״̬*/
		   g_stCfg_system_set.use_password = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"user_password");
		if(objValue)
		{/*ʹ������  0:δѡ��״̬ 1:ѡ��״̬*/
		   g_stCfg_system_set.use_password = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"timeout_exit_enable");
		if(objValue)
		{/*//��ʱ�˳�ʹ��  0:δѡ��״̬ 1:ѡ��״̬*/
		   g_stCfg_system_set.timeout_exit_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"timeout_exit_value");
		if(objValue)
		{/*��ʱ�˳�ֵ(min) 0:1  1:2  2:3  3:5  4:10*/
		   g_stCfg_system_set.timeout_exit_value = objValue->valueint;
		}

		/*�߼�*/
		objValue = cJSON_GetObjectItem(root,"change_time");
		if(objValue)
		{/*�����л�ʱ��*/
			g_stCfg_system_set.change_time = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"voice_set");
		if(objValue)
		{/*�������� ֵ: 0~8*/
			
			g_stCfg_system_set.voice_set = objValue->valueint;
		}

		objValue = cJSON_GetObjectItem(root,"sleep_shutdown_value");
		if(objValue)
		{/*��ʱ�ػ�ֵ��(min)*/
		   g_stCfg_system_set.sleep_shutdown_value = objValue->valueint;
		   m_stCfg_DevInfo.u16DelaySdTime = objValue->valueint;
		}

		

		
		ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_SYSTEM_SET, (LTY_U32)&g_stCfg_system_set, sizeof(cfg_para_system_set_t));
		DBGBOUT("gui_system_system_setup_set() CFG_PARA_SYSTEM_SET ret = %d \n", ret);

		ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_DEV_INF, (LTY_U32)&m_stCfg_DevInfo, sizeof(m_stCfg_DevInfo));
		printf("m_stCfg_DevInfo ret:%d \n", ret);
	    if(LTY_SUCCESS != ret)
	    {
	        DBGPOUT("set m_stCfg_DevInfo config error, %s\n", LtyErrStr(ret));
	    }
		
	}while(0);
}

void system_setup_common_get(cJSON *root)
{
	do
	{
	   int ret = 0;
	   cfg_para_dev_inf_t stdevinfo;
	
		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYSTEM_SET, (LTY_U32)&g_stCfg_system_set, sizeof(cfg_para_system_set_t));
		if (LTY_SUCCESS != ret)
		{
			printf("gui_system_system_setup_get() CFG_PARA_SYSTEM_SET error\n");
			//break;
		}

		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DEV_INF, (LTY_U32)&stdevinfo, sizeof(cfg_para_dev_inf_t));
        if (0 != ret){
			printf("gui_system_system_setup_get() CFG_PARA_DEV_INF error\n");
        }
		
		cJSON_AddNumberToObject(root, "current_time_zone", stdevinfo.u8eartharea/*g_stCfg_system_set.current_time_zone*/);/*��ǰѡ���ʱ��*/
		//cJSON_AddStringToObject(root, "time1", time1); /*ʱ��1*/
		//cJSON_AddStringToObject(root, "time2", time2);/*ʱ��2*/
		cJSON_AddNumberToObject(root, "satellite_time_enable", g_stCfg_system_set.satellite_time_enable);/*����Уʱ  0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "center_time_enable",g_stCfg_system_set.center_time_enable );/*����Уʱ  0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "ntp_enable", g_stCfg_system_set.ntp_enable);/*NTPУʱʹ��	0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "ntp_value", g_stCfg_system_set.ntp_value); /*NTPУʱֵ 0:time.windows.com 1:time.nist.gov 2:time-nw.nist.gov 3:time-a.nist.gov 4:time-b.nist.gov*/

		cJSON_AddNumberToObject(root, "use_password", g_stCfg_system_set.use_password);/*ʹ������  0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "timeout_exit_enable", g_stCfg_system_set.timeout_exit_enable); /*��ʱ�˳�ʹ��	0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "timeout_exit_value", g_stCfg_system_set.timeout_exit_value);/*��ʱ�˳�ֵ(min) 0:1	1:2  2:3  3:5  4:10*/

		cJSON_AddNumberToObject(root, "change_time", g_stCfg_system_set.change_time);//�����л�ʱ��
		cJSON_AddNumberToObject(root, "voice_set", g_stCfg_system_set.voice_set);//�������� ֵ: 0~8
		cJSON_AddNumberToObject(root, "delay_shutdown", stdevinfo.u16DelaySdTime);//��ʱ�ػ�
		
	}while(0);

}


void gui_system_system_setup_set(cJSON *root)
{
		cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		/*ͨ������*/
		if(1 == objValue->valueint)
		{/*��������*/
			
			objValue = cJSON_GetObjectItem(root,"screen_mode");
			if(objValue)
			{/*��Ļģʽ 0:����ģʽ 1:����ģʽ*/
			   g_stCfg_system_set.screen_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"drusb_mode");
			if(objValue)
			{/*DRUSB�ӿ�ģʽ 0:��׼ 1:�ֱ�*/
			   g_stCfg_system_set.drusb_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"sleep_shutdown_enable");
			if(objValue)
			{/*��ʱ�ػ�ʹ��*/
			   g_stCfg_system_set.sleep_shutdown_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"sleep_shutdown_value");
			if(objValue)
			{/*��ʱ�ػ�ֵ��(min)*/
			   g_stCfg_system_set.sleep_shutdown_value = objValue->valueint;
			   m_stCfg_DevInfo.u16DelaySdTime = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"device_door_report_enable");
			if(objValue)
			{/*�豸�ſ����ϱ����ʹ��*/
			   g_stCfg_system_set.device_door_report_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"device_door_report_value");
			if(objValue)
			{/*�豸�ſ����ϱ����ֵ*/
			   g_stCfg_system_set.device_door_report_value = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"hard_disk_heating_enable");
			if(objValue)
			{/*Ӳ�̼���ʹ��*/
			   g_stCfg_system_set.hard_disk_heating_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"hard_disk_heating_value");
			if(objValue)
			{/*Ӳ�̼���ֵ*/
			   g_stCfg_system_set.hard_disk_heating_value = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"now_time_enable");
			if(objValue)
			{/*���㱨ʱʹ��*/
			   g_stCfg_system_set.now_time_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"now_time_value1");
			if(objValue)
			{/*���㱨ʱֵ1   h*/
			   g_stCfg_system_set.now_time_value1 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"now_time_value2");
			if(objValue)
			{/*���㱨ʱֵ2   h*/
			   g_stCfg_system_set.now_time_value2 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"voltage_enable");
			if(objValue)
			{/*����ѹ����ʹ��*/
			   g_stCfg_system_set.voltage_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"voltage_value1");
			if(objValue)
			{/*����ѹ����ֵ1   V*/
			   g_stCfg_system_set.voltage_value1 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"voltage_value2");
			if(objValue)
			{/*����ѹ����ֵ2   V*/
			   g_stCfg_system_set.voltage_value2 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"voltage_value3");
			if(objValue)
			{/*//����ѹ����ֵ3   min*/
			   g_stCfg_system_set.voltage_value3 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"temperature_enable");
			if(objValue)
			{/*���¶ȱ���ʹ��*/
			   g_stCfg_system_set.temperature_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"temperature_value1");
			if(objValue)
			{/*���¶ȱ���ֵ1   V*/
			   g_stCfg_system_set.temperature_value1 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"temperature_value2");
			if(objValue)
			{/*���¶ȱ���ֵ2   V*/
			   g_stCfg_system_set.temperature_value2 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"temperature_value3");
			if(objValue)
			{/*���¶ȱ���ֵ3   min*/
			   g_stCfg_system_set.temperature_value3 = objValue->valueint;
			}
			//add
			objValue = cJSON_GetObjectItem(root,"fan_control_enable");
			if(objValue)
			{//���ȿ���ʹ��
			   g_stCfg_system_set.fan_control_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"fan_control_value1");
			if(objValue)
			{//���ȿ���ֵ1
			   g_stCfg_system_set.fan_control_value1 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"fan_control_value2");
			if(objValue)
			{//���ȿ���ֵ2
			   g_stCfg_system_set.fan_control_value2 = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"picutre_play_time");
			if(objValue)
			{/*ͼƬ�ֲ�*/
			   g_stCfg_system_set.picutre_play_time = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"volumn_size");
			if(objValue)
			{/*������С 0:6 1:5 2:4 3:3 4:2 5:1 6:0*/
			   g_stCfg_system_set.volumn_size = objValue->valueint;
			   switch(g_stCfg_system_set.volumn_size)
			   	{
			   		case 0:
						system("tinymix 1 63");
						break;
					case 1:
						system("tinymix 1 60");
						break;
					case 2:
						system("tinymix 1 58");
						break;
					case 3:
						system("tinymix 1 55");
						break;
					case 4:
						system("tinymix 1 52");
						break;
					case 5:
						system("tinymix 1 46");
						break;
					case 6:
						system("tinymix 1 0");
						break;
					default:
						break;
			   	}
			}
			
			objValue = cJSON_GetObjectItem(root,"auto_lock_enable");
			if(objValue)
			{/*�Զ�����ʹ��  0:δѡ��״̬ 1:ѡ��״̬*/
			   g_stCfg_system_set.auto_lock_enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"auto_lock_value");
			if(objValue)
			{/*�Զ�����ֵ(min)  0:1  1:2  2:3  3:5  4:10*/
			   g_stCfg_system_set.auto_lock_value = objValue->valueint;
			}
			
			system_setup_common_set(root);

			
			


		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/
			gui_system_system_setup_get();
		}
			
	}while(0);

}
void gui_system_system_setup_get()
{
	do
	{
	   cJSON *root = NULL;
	   int ret = 0;
	   cfg_para_dev_inf_t stdevinfo;
	   
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	

	   	cJSON_AddStringToObject(root, "msg_type", "gui_system_system_setup");

		memset(&g_stCfg_system_set, 0, sizeof(cfg_para_system_set_t));
		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYSTEM_SET, (LTY_U32)&g_stCfg_system_set, sizeof(cfg_para_system_set_t));
		if (LTY_SUCCESS != ret)
		{
			printf("gui_system_system_setup_get() CFG_PARA_SYSTEM_SET error\n");
			//break;
		}

		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DEV_INF, (LTY_U32)&stdevinfo, sizeof(cfg_para_dev_inf_t));
        if (0 != ret){
			printf("gui_system_system_setup_get() CFG_PARA_DEV_INF error\n");
        }

		//ͨ������
		printf("gui_system_system_setup_get() stdevinfo.u16DelaySdTime:%d\n",stdevinfo.u16DelaySdTime);
		
		cJSON_AddNumberToObject(root, "screen_mode", g_stCfg_system_set.screen_mode); /*��Ļģʽ 0:����ģʽ 1:����ģʽ*/
		cJSON_AddNumberToObject(root, "drusb_mode", g_stCfg_system_set.drusb_mode);  /*DRUSB�ӿ�ģʽ 0:��׼ 1:�ֱ�*/
		cJSON_AddNumberToObject(root, "sleep_shutdown_enable",g_stCfg_system_set.sleep_shutdown_enable );/*��ʱ�ػ�ʹ��*/
		cJSON_AddNumberToObject(root, "sleep_shutdown_value", stdevinfo.u16DelaySdTime/*g_stCfg_system_set.sleep_shutdown_value*/); /*��ʱ�ػ�ֵ��(min)*/
		cJSON_AddNumberToObject(root, "device_door_report_enable", g_stCfg_system_set.device_door_report_enable);/*�豸�ſ����ϱ����ʹ��*/
		cJSON_AddNumberToObject(root, "device_door_report_value", g_stCfg_system_set.device_door_report_value);/*�豸�ſ����ϱ����ֵ*/
		cJSON_AddNumberToObject(root, "hard_disk_heating_enable", g_stCfg_system_set.hard_disk_heating_enable);/*Ӳ�̼���ʹ��*/
		cJSON_AddNumberToObject(root, "hard_disk_heating_value", g_stCfg_system_set.hard_disk_heating_value);/*Ӳ�̼���ֵ*/
		cJSON_AddNumberToObject(root, "now_time_enable", g_stCfg_system_set.now_time_enable);/*���㱨ʱʹ��*/
		cJSON_AddNumberToObject(root, "now_time_value1", g_stCfg_system_set.now_time_value1);/*���㱨ʱֵ1   h*/
		cJSON_AddNumberToObject(root, "now_time_value2", g_stCfg_system_set.now_time_value2);/*���㱨ʱֵ2   h*/
		cJSON_AddNumberToObject(root, "voltage_enable", g_stCfg_system_set.voltage_enable);/*����ѹ����ʹ��*/
		cJSON_AddNumberToObject(root, "voltage_value1", g_stCfg_system_set.voltage_value1);/*����ѹ����ֵ1   V*/
		cJSON_AddNumberToObject(root, "voltage_value2", g_stCfg_system_set.voltage_value2);/*����ѹ����ֵ2   V*/
		cJSON_AddNumberToObject(root, "voltage_value3", g_stCfg_system_set.voltage_value3);/*����ѹ����ֵ3   min*/
		cJSON_AddNumberToObject(root, "temperature_enable", g_stCfg_system_set.temperature_enable);/*���¶ȱ���ʹ��*/
		cJSON_AddNumberToObject(root, "temperature_value1", g_stCfg_system_set.temperature_value1);/*���¶ȱ���ֵ1   V*/
		cJSON_AddNumberToObject(root, "temperature_value2", g_stCfg_system_set.temperature_value2);/*���¶ȱ���ֵ2   V*/
		cJSON_AddNumberToObject(root, "temperature_value3", g_stCfg_system_set.temperature_value3);/*���¶ȱ���ֵ3   min*/
		//add
		cJSON_AddNumberToObject(root, "fan_control_enable", g_stCfg_system_set.fan_control_enable);//���ȿ���ʹ��
		cJSON_AddNumberToObject(root, "fan_control_value1", g_stCfg_system_set.fan_control_value1);//���ȿ���ֵ1
		cJSON_AddNumberToObject(root, "fan_control_value2", g_stCfg_system_set.fan_control_value2);//���ȿ���ֵ2
		cJSON_AddNumberToObject(root, "picutre_play_time", g_stCfg_system_set.picutre_play_time);/*ͼƬ�ֲ�*/
		cJSON_AddNumberToObject(root, "volumn_size", g_stCfg_system_set.volumn_size);/*������С*/
		//ϵͳʱ��
		system_setup_common_get(root);
		//�˻�����

		cJSON_AddNumberToObject(root, "auto_lock_enable", g_stCfg_system_set.auto_lock_enable);/*�Զ�����ʹ��  0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "auto_lock_value",g_stCfg_system_set.auto_lock_value ); /*�Զ�����ֵ(min)  0:1	1:2  2:3  3:5  4:10*/

#if 0
		int array_size = 30;
        cJSON *pJson_rows = cJSON_CreateArray();

        cJSON_AddItemToObject(root, "rows",pJson_rows);
	
        cJSON *pJson_row = NULL;
		char value[20];
		
        for(int index = 0;index < array_size;index++)  /*ʱ���б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",value); /*ʱ������*/
        }
#endif
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}

#include <stdio.h>
#include <string.h>
#include<fcntl.h>

//#define		LTY_SYS_PARAM_FILE          "./ltysystem.sys"


#if 0
typedef  struct ip_param_s
{
 	unsigned char ip_addr[50];  /*ip��ַ*/  
 	unsigned int  ip_port;	/*ip�˿�*/

}ip_param_t;

#endif

//������
typedef union
{
              unsigned char intbuf[2];
              unsigned short i;
} ShortUnon;

typedef union
{
              unsigned char longbuf[4];
              unsigned int  i;
} LongUnon;

int Read_cardparam(cardparam *Cardparam)
{
	int canshu;
	int result,i;
	unsigned char FileBuf[32];
	LongUnon I_TIME;
	ShortUnon T_TIME;
	
	canshu = open(LTY_SYS_PARAM_FILE,O_SYNC|O_RDWR);
	if(-1 == canshu)
	{
		//printf("Can't open /system.sys\n");
		return -1;
	}
	
	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,0, SEEK_SET);
	result = read(canshu,FileBuf,32);
	memcpy(Cardparam->SUPCARD_TYPE,FileBuf,32);

	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,44, SEEK_SET);
	result = read(canshu,FileBuf,4);
	memcpy(I_TIME.longbuf,FileBuf,4);
	Cardparam->TIMEINTER = I_TIME.i/60;

	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,126, SEEK_SET);
	result = read(canshu,FileBuf,3);
	memcpy(T_TIME.intbuf,FileBuf,2);
	Cardparam->TICKETTIME = T_TIME.i;
	Cardparam->CHARGMODE = FileBuf[2];

	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,50, SEEK_SET);
	result = read(canshu,FileBuf,2);
	Cardparam->DEVMODE = FileBuf[0];
	Cardparam->PSAMNUM = FileBuf[1] - 1;

	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,146, SEEK_SET);
	result = read(canshu,FileBuf,18);
	for(i=0;i<18;i++)
	{
		if(((FileBuf[i] == 0x55) && (FileBuf[i+1] == 0xaa)) && (i > 0))
		{
			Cardparam->SECTOR = FileBuf[i-1] - 1;
		}
	}

	result = lseek(canshu,334, SEEK_SET);
	result = read(canshu,Cardparam->host_ip,20);
	

	
	close(canshu);
	return result;
}

int Read_ip_param(ip_param_t *_ip_param)
{
	int canshu;
	int result/*,i*/;
	unsigned char FileBuf[50];
	LongUnon I_TIME;

	
	canshu = open(LTY_SYS_PARAM_FILE,O_SYNC|O_RDWR);
	if(-1 == canshu)
	{
		//printf("Can't open /system.sys\n");
		return -1;
	}

	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,76, SEEK_SET);
	result = read(canshu,FileBuf,40);
	memcpy(_ip_param->ip_addr,FileBuf,40);

	
	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,116, SEEK_SET);
	result = read(canshu,FileBuf,4);
	memcpy(I_TIME.longbuf,FileBuf,4);
	_ip_param->ip_port = I_TIME.i;


	close(canshu);
	return result;
}


int Read_local_ip_param(unsigned char *_ip)
{
	int canshu;
	int result/*,i*/;
	unsigned char FileBuf[50];
	//LongUnon I_TIME;

	printf("Read_local_ip_param()\n");
	
	canshu = open(LTY_SYS_PARAM_FILE,O_SYNC|O_RDWR);
	if(-1 == canshu)
	{
		printf("Can't open /system.sys\n");
		return -1;
	}

	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,164, SEEK_SET);
	result = read(canshu,FileBuf,28);
	memcpy(_ip,FileBuf,27);

	printf("Read_local_ip_param() %s\n",_ip);

	close(canshu);
	return result;
}

int Write_local_ip_param(unsigned char *_ip)
{
	int canshu;
	int result/*,i*/;
	unsigned char FileBuf[50];
	//LongUnon I_TIME;

	printf("Write_local_ip_param() %s\n",_ip);
	
	canshu = open(LTY_SYS_PARAM_FILE,O_SYNC|O_RDWR);
	if(-1 == canshu)
	{
		printf("Can't open /system.sys\n");
		return -1;
	}

	
	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,164, SEEK_SET);

	memcpy(FileBuf,_ip,27);
	result = write(canshu,FileBuf,28);


	close(canshu);
	return result;
}




int Write_cardparam(cardparam &Cardparam)
{
	int canshu;
	int result,i;
	unsigned char FileBuf[32];
	LongUnon I_TIME;
	ShortUnon T_TIME;
	
	canshu = open(LTY_SYS_PARAM_FILE,O_SYNC|O_RDWR);
	if(-1 == canshu)
	{
		//printf("Can't open /system.sys\n");
		return -1;
	}
	
	memset(FileBuf,0,sizeof(FileBuf));
	memcpy(FileBuf,Cardparam.SUPCARD_TYPE,32);
	result = lseek(canshu,0, SEEK_SET);
	result = write(canshu,FileBuf,32);

	memset(FileBuf,0,sizeof(FileBuf));
	I_TIME.i = Cardparam.TIMEINTER * 60;
	memcpy(FileBuf,I_TIME.longbuf,4);
	result = lseek(canshu,44, SEEK_SET);
	result = write(canshu,FileBuf,4);

	memset(FileBuf,0,sizeof(FileBuf));
	T_TIME.i = Cardparam.TICKETTIME;
	memcpy(FileBuf,T_TIME.intbuf,2);
	FileBuf[2] = Cardparam.CHARGMODE;
	result = lseek(canshu,126, SEEK_SET);
	result = write(canshu,FileBuf,3);

	memset(FileBuf,0,sizeof(FileBuf));
	FileBuf[0] = Cardparam.DEVMODE;
	FileBuf[1] = Cardparam.PSAMNUM + 1;
	result = lseek(canshu,50, SEEK_SET);
	result = write(canshu,FileBuf,2);

	memset(FileBuf,0,sizeof(FileBuf));
	if((Cardparam.SECTOR >= 0) && (Cardparam.SECTOR < 16))
	{
		for(i=0;i<Cardparam.SECTOR+1;i++)
		{
			FileBuf[i] = i + 1;
		}
		FileBuf[i] = 0x55;
		FileBuf[i+1] = 0xaa;
	}
	else
	{
		close(canshu);
		return -1;
	}
	result = lseek(canshu,146, SEEK_SET);
	result = write(canshu,FileBuf,18);

	result = lseek(canshu,334, SEEK_SET);
	result = write(canshu,Cardparam.host_ip,20);
	
	close(canshu);
	return result;
}

int Write_ip_param(ip_param_t &_ip_param)
{
	int canshu;
	int result/*,i*/;
	unsigned char FileBuf[50];
	LongUnon I_TIME;

	
	canshu = open(LTY_SYS_PARAM_FILE,O_SYNC|O_RDWR);
	if(-1 == canshu)
	{
		return -1;
	}

	
	memset(FileBuf,0,sizeof(FileBuf));
	result = lseek(canshu,76, SEEK_SET);

	memcpy(FileBuf,_ip_param.ip_addr,40);
	result = write(canshu,FileBuf,40);
	

	memset(FileBuf,0,sizeof(FileBuf));
	I_TIME.i = _ip_param.ip_port;
	memcpy(FileBuf,I_TIME.longbuf,4);
	result = lseek(canshu,116, SEEK_SET);
	result = write(canshu,FileBuf,4);

	close(canshu);
	return result;
}


void gui_system_pos_config_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��������*/
			cardparam card_param;
			memset(&card_param,0,sizeof(cardparam));
			
		
			objValue = cJSON_GetObjectItem(root,"DEVMODE");/*�ն�ģʽ:1:һ��� 2:���� 3:�ӻ�*/
			if(objValue)
			{//�ն�ģʽ
				card_param.DEVMODE = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"CHARGMODE");/*�շ�ģʽ: 1:һƱ�� 2:�ֶ��շ�*/
			if(objValue)
			{//�շ�ģʽ
				card_param.CHARGMODE = objValue->valueint;
				gui_main_page_get_ex(objValue->valueint);
			}
			objValue = cJSON_GetObjectItem(root,"PSAMNUM");/*PSAM������: 0:1 1:2 2:3 3:4*/
			if(objValue)
			{//PSAM����
				card_param.PSAMNUM = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"SECTOR");/*������: 0~15: 1~16*/
			if(objValue)
			{//֧������
				card_param.SECTOR = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"TIMEINTER");/*ˢ�����ʱ�� min*/
			if(objValue)
			{//ˢ��ʱ����
				card_param.TIMEINTER = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"TICKETTIME");/*��Ʊ�ж�ʱ�� min*/
			if(objValue)
			{//��Ʊ���ʱ��    
				card_param.TICKETTIME = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"host_ip");/*����IP*/
			if(objValue && objValue->valuestring)
			{	
				int str_len = strlen(objValue->valuestring);
				if(strlen(objValue->valuestring) > 20)
					str_len = 20;
				memcpy(card_param.host_ip,objValue->valuestring,str_len);
			}
			
			
			cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*�����С*/
                tasklist=pJson_array->child;/*�Ӷ���*/
            }

            for(int index = 0;index < array_size;index++)  /*����ѡ�� 0~31: 1~32 ��ע:0:��ѡ�� 1:ѡ��*/
            {
                
                objValue = cJSON_GetObjectItem(tasklist,"value");
                if(objValue)
                {
                    card_param.SUPCARD_TYPE[index] = objValue->valueint;
                }
                
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }
			// Write_cardparam(card_param);
			LtyMessagePost(LTY_MSG_POS_CFG_INFO_SET,0,(LTY_U32)&card_param,sizeof(cardparam));
		    printf("card_param.DEVMODE:%d, card_param.CHARGMODE:%d\n", card_param.DEVMODE, card_param.CHARGMODE);
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/
			gui_system_pos_config_get();
		}

	}while(0);
}
void gui_system_pos_config_get()
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	

	   cJSON_AddStringToObject(root, "msg_type", "gui_system_pos_config");
       LTY_RESULT ret;
	   cardparam card_param;
	   memset(&card_param,0,sizeof(cardparam));
	   // Read_cardparam(&card_param)
	   ret = LtyMessageSend(LTY_MSG_CFG_GET_POS_DATA, CFG_POS_CARD, (LTY_U32)&card_param, sizeof(cardparam));
	   
	   if (LTY_SUCCESS != ret)
	   {
	       printf("gui_system_pos_config_get() CFG_POS_CARD error\n");
	   }
	   cJSON_AddNumberToObject(root, "DEVMODE",card_param.DEVMODE);/*�ն�ģʽ:1:һ��� 2:���� 3:�ӻ�*/
	   cJSON_AddNumberToObject(root, "CHARGMODE",card_param.CHARGMODE);/*�շ�ģʽ: 1:һƱ�� 2:�ֶ��շ�*/
	   cJSON_AddNumberToObject(root, "PSAMNUM",card_param.PSAMNUM);/*PSAM������: 1~4*/
	   cJSON_AddNumberToObject(root, "SECTOR",card_param.SECTOR); /*������: 1~16*/
	   cJSON_AddNumberToObject(root, "TIMEINTER",card_param.TIMEINTER);/*ˢ�����ʱ�� min*/
	   cJSON_AddNumberToObject(root, "TICKETTIME",card_param.TICKETTIME);/*��Ʊ�ж�ʱ�� min*/
	   cJSON_AddStringToObject(root, "host_ip",(const char *)card_param.host_ip);/*����IP*/

		printf("gui_system_pos_config_get card_param.DEVMODE:%d\n",card_param.DEVMODE);
	    printf("gui_system_pos_config_get card_param.CHARGMODE:%d\n",card_param.CHARGMODE);
		gui_main_page_get_ex(card_param.CHARGMODE);
		
		int array_size = sizeof(card_param.SUPCARD_TYPE);
        cJSON *pJson_rows = cJSON_CreateArray();

        cJSON_AddItemToObject(root, "rows",pJson_rows);

        cJSON *pJson_row = NULL;
		
        for(int index = 0;index < array_size;index++)  /*����ѡ�� 0~31: 1~32 ��ע:0:��ѡ�� 1:ѡ��*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows,pJson_row);
            cJSON_AddNumberToObject(pJson_row, "value",card_param.SUPCARD_TYPE[index]); 
        }

	    ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);
	
}

void gui_system_senior_setup_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��������*/
			/*ͨ������*/
			objValue = cJSON_GetObjectItem(root,"location_mode");
			if(objValue)
			{/*//��λ��ʽ 0:��϶�λ 1:������λ 2:GPS��λ*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"custom_navigation_enable");
			if(objValue)
			{/*���Ե���ʹ��*/
			   //objValue->valueint;
			}
			/*���Ե���-����(start)*/
			objValue = cJSON_GetObjectItem(root,"corner_x");
			if(objValue)
			{/*�豸X����Գ�X��ת��*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"corner_y");
			if(objValue)
			{/*�豸Y����Գ�Y��ת��*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"corner_z");
			if(objValue)
			{/*�豸Z����Գ�Z��ת��*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"corner_gain");
			if(objValue)
			{/*ת��Gainֵ*/
			   //objValue->valueint;
			}
			/*���Ե���-����(end)*/
			objValue = cJSON_GetObjectItem(root,"station_mode");
			if(objValue)
			{/*//��վ��ʽ 0:�Զ� 1:�ֶ�*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"in_station_radius");
			if(objValue)
			{/*��վ�뾶*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"out_station_radius");
			if(objValue)
			{/*��վ�뾶*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"inflection_point_radius");
			if(objValue)
			{/*�յ�뾶*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"over_speed_still_tip");
			if(objValue)
			{/*���ٱ�������*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"over_speed_tip_times");
			if(objValue)
			{/*�������Ѵ���*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"non_operating_speed_limit");
			if(objValue)
			{/*����Ӫ����*/
			   //objValue->valueint;
			}
			/*�澯ʹ��*/
			objValue = cJSON_GetObjectItem(root,"urgent_acceleration_enable");
			if(objValue)
			{/*��������ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"urgent_acceleration_value");
			if(objValue)
			{/*��������ֵ  (km/s)*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"emergency_deceleration_enable");
			if(objValue)
			{/*��������ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"emergency_deceleration_value");
			if(objValue)
			{/*��������ֵ  (km/s)*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"hysteresis_alarm_enable");
			if(objValue)
			{/*��վ����ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"hysteresis_alarm_value");
			if(objValue)
			{/*��վ����ֵ*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"skip_alarm_enable");
			if(objValue)
			{/*Խվ����ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"skip_alarm_value");
			if(objValue)
			{/*Խվ����ֵ*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"fatigue_driving_check_enable");
			if(objValue)
			{/*ƣ�ͼ�ʻ���ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"fatigue_driving_check_value");
			if(objValue)
			{/*ƣ�ͼ�ʻ���ֵ*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"idling_enable");
			if(objValue)
			{/*����ʹ��ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"idling_value");
			if(objValue)
			{/*����ʹ��ֵ*/
			   //objValue->valueint;
			}
			/*CAN����*/
			objValue = cJSON_GetObjectItem(root,"can1_enable");
			if(objValue)
			{/*CAN1  0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"can2_enable");
			if(objValue)
			{/*CAN2  0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			/*�߼�����*/
		    objValue = cJSON_GetObjectItem(root,"volumn_adjustment");
			if(objValue)
			{/*��������ʹ�� 0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"idling_enable1");
			if(objValue)
			{/*����ʹ�� 0:δѡ��״̬ 1:ѡ��״̬*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"idling_value1");
			if(objValue)
			{/*����ֵ1*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"idling_value2");
			if(objValue)
			{/*����ֵ2*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"temperature_check_enable");
			if(objValue)
			{/*�¶ȼ��ʹ��*/
			   //objValue->valueint;
			}
			/*�¶ȼ��-����*/
			objValue = cJSON_GetObjectItem(root,"in_temperature_security1");
			if(objValue)
			{/*���°�ȫ��Χ1*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"in_temperature_security2");
			if(objValue)
			{/*���°�ȫ��Χ2*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"out_temperature_security1");
			if(objValue)
			{/*���°�ȫ��Χ1*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"out_temperature_security2");
			if(objValue)
			{/*���°�ȫ��Χ2*/
			   //objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"police_upload_time");
			if(objValue)
			{/*�����ϴ�ʱ����*/
			   //objValue->valueint;
			}
			
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/

		}
		else if(3 == objValue->valueint)
		{/*can1��������*/
			objValue = cJSON_GetObjectItem(root,"command");
			if(objValue)
			{
				
			}
		}
		else if(4 == objValue->valueint)
		{/*can2��������*/
			objValue = cJSON_GetObjectItem(root,"command");
			if(objValue)
			{
				
			}
		}
	}while(0);

}
void gui_system_senior_setup_get()
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	

	   cJSON_AddStringToObject(root, "msg_type", "gui_system_senior_setup");
	   int type = 0;  /*1:��ȡ���� 2:can1���ؽ�� 3:can2���ؽ��*/
	
		//cJSON_AddNumberToObject(root, "enable",enable );/*0:δѡ��״̬ 1:ѡ��״̬*/ 
		switch(type)
		{
			case 1:/*��ȡ����*/
			{
				
				/*ͨ������*/
			   int location_mode = 0;
			   char modular_mode[20];
				char software_version[20];
				char sn_number[20];
			   int custom_navigation_enable = 0;
			   int corner_x = 0;
			   int corner_y = 0;
			   int corner_z = 0;
			   int corner_gain = 0;
			   int station_mode = 0;
			   int in_station_radius = 0;
			   int out_station_radius = 0;
			   int inflection_point_radius = 0;
			   int over_speed_still_tip = 0;
			   int over_speed_tip_times = 0;
			   int non_operating_speed_limit = 0;

			   cJSON_AddNumberToObject(root, "type",1); 
			   cJSON_AddNumberToObject(root, "location_mode",location_mode); /*��λ��ʽ 0:��϶�λ 1:������λ 2:GPS��λ*/
			   cJSON_AddStringToObject(root, "modular_mode", modular_mode);/*ģ���ͺ�*/
			   cJSON_AddStringToObject(root, "software_version", software_version);/*����汾*/
			   cJSON_AddStringToObject(root, "sn_number", sn_number);/*S/N��*/
			 
			   cJSON_AddNumberToObject(root, "custom_navigation_enable",custom_navigation_enable );/*���Ե���ʹ��*/
			   /*���Ե���-����(start)*/
			   cJSON_AddNumberToObject(root, "corner_x", corner_x); /*�豸X����Գ�X��ת��*/
			   cJSON_AddNumberToObject(root, "corner_y",corner_y);/*�豸Y����Գ�Y��ת��*/
			   cJSON_AddNumberToObject(root, "corner_z", corner_z); /*�豸Z����Գ�Z��ת��*/
			   cJSON_AddNumberToObject(root, "corner_gain", corner_gain); /*ת��Gainֵ*/
			   /*���Ե���-����(end)*/
			   cJSON_AddNumberToObject(root, "station_mode", station_mode); /*��վ��ʽ 0:�Զ� 1:�ֶ�*/
			   cJSON_AddNumberToObject(root, "in_station_radius", in_station_radius);/*��վ�뾶*/
			   cJSON_AddNumberToObject(root, "out_station_radius", out_station_radius);/*��վ�뾶*/
			   cJSON_AddNumberToObject(root, "inflection_point_radius", inflection_point_radius);/*�յ�뾶*/
			   cJSON_AddNumberToObject(root, "over_speed_still_tip", over_speed_still_tip); /*���ٱ�������*/
			   cJSON_AddNumberToObject(root, "over_speed_tip_times", over_speed_tip_times);/*�������Ѵ���*/
			   cJSON_AddNumberToObject(root, "non_operating_speed_limit", non_operating_speed_limit);/*����Ӫ����*/
			   /*�澯ʹ��*/
			   int urgent_acceleration_enable = 0;
			   int urgent_acceleration_value = 0;
			   int emergency_deceleration_enable = 0;
			   int emergency_deceleration_value = 0;
			   int hysteresis_alarm_enable = 0;
			   int hysteresis_alarm_value = 0;
			   int skip_alarm_enable = 0;
			   int fatigue_driving_check_enable = 0;
			   int fatigue_driving_check_value = 0;
			   int idling_enable = 0;
			   int idling_value = 0;
			   int can1_enable = 0;
			   int can2_enable = 0;
			   int volumn_adjustment = 0;
			   int idling_enable1 = 0;
			   int idling_value1 = 0;
			   int idling_value2 = 0;
			   int temperature_check_enable = 0;
			   int in_temperature_security1 = 0;
			   int in_temperature_security2 = 0;
			   int out_temperature_security1 = 0;
			   int out_temperature_security2 = 0;
			   int police_upload_time = 0;
			   int skip_alarm_value = 0;
			   
			   cJSON_AddNumberToObject(root, "urgent_acceleration_enable", urgent_acceleration_enable);/*��������ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "urgent_acceleration_value", urgent_acceleration_value);/*��������ֵ	(km/s)*/
			   cJSON_AddNumberToObject(root, "emergency_deceleration_enable", emergency_deceleration_enable);/*��������ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "emergency_deceleration_value", emergency_deceleration_value);/*��������ֵ  (km/s)*/
			   cJSON_AddNumberToObject(root, "hysteresis_alarm_enable", hysteresis_alarm_enable);/*��վ����ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "hysteresis_alarm_value", hysteresis_alarm_value);/*��վ����ֵ*/
			   cJSON_AddNumberToObject(root, "skip_alarm_enable", skip_alarm_enable);/*Խվ����ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "skip_alarm_value", skip_alarm_value);/*Խվ����ֵ*/
			   cJSON_AddNumberToObject(root, "fatigue_driving_check_enable", fatigue_driving_check_enable);/*ƣ�ͼ�ʻ���ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "fatigue_driving_check_value", fatigue_driving_check_value);/*ƣ�ͼ�ʻ���ֵ*/
			   cJSON_AddNumberToObject(root, "idling_enable", idling_enable);/*����ʹ��ʹ�� // 0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "idling_value", idling_value);/*����ʹ��ֵ*/
			   /*CAN����*/
			   cJSON_AddNumberToObject(root, "can1_enable", can1_enable);/*CAN1  0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "can2_enable", can2_enable);/*CAN2  0:δѡ��״̬ 1:ѡ��״̬*/
			   /*�߼�����*/
			   cJSON_AddNumberToObject(root, "volumn_adjustment", volumn_adjustment);/*��������ʹ�� 0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "idling_enable1",idling_enable1);/*����ʹ�� 0:δѡ��״̬ 1:ѡ��״̬*/
			   cJSON_AddNumberToObject(root, "idling_value1", idling_value1);/*����ֵ1*/
			   cJSON_AddNumberToObject(root, "idling_value2", idling_value2);/*����ֵ2*/
			   cJSON_AddNumberToObject(root, "temperature_check_enable", temperature_check_enable); /*�¶ȼ��ʹ��*/
			   /*�¶ȼ��-����*/
			   cJSON_AddNumberToObject(root, "in_temperature_security1", in_temperature_security1); /*���°�ȫ��Χ1*/
			   cJSON_AddNumberToObject(root, "in_temperature_security2", in_temperature_security2);/*���°�ȫ��Χ2*/
			   cJSON_AddNumberToObject(root, "out_temperature_security1", out_temperature_security1);/*���°�ȫ��Χ1*/
			   cJSON_AddNumberToObject(root, "out_temperature_security2", out_temperature_security2);/*���°�ȫ��Χ2*/
			   cJSON_AddNumberToObject(root, "police_upload_time", police_upload_time); /*�����ϴ�ʱ����*/
			}break;
			case 2:/*can1���ؽ��*/
			{
				char result[20];
				cJSON_AddNumberToObject(root, "type",2);
				cJSON_AddStringToObject(root, "result", result);
				
			}break;
			case 3:/*can2���ؽ��*/
			{
				char result[20];
				cJSON_AddNumberToObject(root, "type",3);
				cJSON_AddStringToObject(root, "result", result);
				
			}break;
		}

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}

void switch_config_common_set(cJSON *root)
{
	do
	{
		LTY_RESULT ret;
		cJSON *objValue = NULL;
		
		/*���ż��*/
		memset(&g_door_config_info, 0, sizeof(g_door_config_info));
		
		switch_set_t stSwitchSet;
		
		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_SWITCH, (LTY_U32)&stSwitchSet, sizeof(stSwitchSet));
		if(LTY_SUCCESS != ret)
		{
			DBGPOUT("Get SD Switch Para Failure .....\n");
			break;
		}
		  
		
		objValue = cJSON_GetObjectItem(root,"enable");
		if(objValue)
		{/* 0:δѡ��״̬ 1:ѡ��״̬*/
			//g_door_config_info.enable = objValue->valueint;
			stSwitchSet.u8EnDoorDetect = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"car_door_number");
		if(objValue)
		{/*�������� 0:1 1:2 2:3*/
			//g_door_config_info.car_door_number = objValue->valueint;
			stSwitchSet.u8DoorNum = objValue->valueint + 1;
		}
		objValue = cJSON_GetObjectItem(root,"car_door_type");
		if(objValue)
		{/*��������  0:���������š�1:�������š�2:�����*/
			//g_door_config_info.car_door_type = objValue->valueint;
			if(1 == objValue->valueint)
				stSwitchSet.u8DoorType = 0;
			else
				stSwitchSet.u8DoorType = 1;
		}
		objValue = cJSON_GetObjectItem(root,"door_front");
		if(objValue)
		{/*��ǰ�ŵ�ƽ 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			//g_door_config_info.door_front = objValue->valueint;
			stSwitchSet.u8InverseDoor1PinFlag = objValue->valueint; 
		}
		objValue = cJSON_GetObjectItem(root,"door_middle");
		if(objValue)
		{/*�����ŵ�ƽ 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			//g_door_config_info.door_middle = objValue->valueint;
			stSwitchSet.u8InverseDoor2PinFlag = objValue->valueint; 
		}
		objValue = cJSON_GetObjectItem(root,"door_back");
		if(objValue)
		{/*�����ŵ�ƽ 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			//g_door_config_info.door_back = objValue->valueint;
			stSwitchSet.u8InverseDoor3PinFlag = objValue->valueint; 
		}
		/*���ż�⣭����*/
		objValue = cJSON_GetObjectItem(root,"speed_open_door_check");
		if(objValue)
		{/*���ٶȿ��ż��*/
			g_door_config_info.speed_open_door_check = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"station_open_door_check");
		if(objValue)
		{/*վ�俪�ż��*/
			g_door_config_info.station_open_door_check = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"arrive_open_door_check");
		if(objValue)
		{/*��վδ���ż��*/
			g_door_config_info.arrive_open_door_check = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"arrive_stop_door_check");
		if(objValue)
		{/*��վδͣ�����*/
			g_door_config_info.arrive_stop_door_check = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"speed_enable");
		if(objValue)
		{/*�쳣��������С�ٶ�ʹ�� 0:δѡ��״̬ 1:ѡ��״̬*/
			g_door_config_info.speed_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"speed");
		if(objValue)
		{/*�쳣��������С�ٶ�*/
			g_door_config_info.speed = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"frequency_enable");
		if(objValue)
		{/*�쳣����������ʹ�� 0:δѡ��״̬ 1:ѡ��״̬*/
			g_door_config_info.frequency_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"frequency");
		if(objValue)
		{/*�쳣���������Ѵ���*/
			g_door_config_info.frequency = objValue->valueint;
		}
		/*�������������*/
		objValue = cJSON_GetObjectItem(root,"back_car_monitor_enable");
		if(objValue)
		{/*�������� 0:δѡ��״̬ 1:ѡ��״̬*/
			g_door_config_info.back_car_monitor_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"back_car_monitor_status");
		if(objValue)
		{/*�������� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			g_door_config_info.back_car_monitor_status = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"no_brake_enable");
		if(objValue)
		{/*δɲ�� 0:δѡ��״̬ 1:ѡ��״̬*/
			g_door_config_info.no_brake_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"no_brake_status");
		if(objValue)
		{/*δɲ�� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			//g_door_config_info.no_brake_status = objValue->valueint;
			stSwitchSet.u8InverseBreakPinFlag = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"open_front_box_enable");
		if(objValue)
		{/*��ǰ�� 0:δѡ��״̬ 1:ѡ��״̬*/
			g_door_config_info.open_front_box_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"open_front_box_status");
		if(objValue)
		{/*��ǰ�� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			//g_door_config_info.open_front_box_status = objValue->valueint;
			stSwitchSet.u8InversePicketPinFlag = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"wiper_enable");
		if(objValue)
		{/*��� 0:δѡ��״̬ 1:ѡ��״̬*/
			g_door_config_info.wiper_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"wiper_status");
		if(objValue)
		{/*��� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			//g_door_config_info.wiper_status = objValue->valueint;
			stSwitchSet.u8YuGuaLevelFlag = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"left_lamp_enable");
		if(objValue)
		{/*��ת�� 0:δѡ��״̬ 1:ѡ��״̬*/
			g_door_config_info.left_lamp_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"left_lamp_status");
		if(objValue)
		{/*��ת�� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			g_door_config_info.left_lamp_status = objValue->valueint;
		}
				objValue = cJSON_GetObjectItem(root,"right_lamp_enable");
		if(objValue)
		{/*��ת�� 0:δѡ��״̬ 1:ѡ��״̬*/
			g_door_config_info.right_lamp_enable = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"right_lamp_status");
		if(objValue)
		{/*��ת�� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			g_door_config_info.right_lamp_status = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"io_input1");
		if(objValue)
		{/*I/O����1 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
			g_door_config_info.io_input1 = objValue->valueint;
		}
		ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_DOOR_CONFIG_SET_INFO, (LTY_U32)&g_door_config_info, sizeof(DEV_DOOR_CONFIG_INFO_T));
		DBGBOUT("gui_system_switch_config_set() CFG_PARA_DOOR_CONFIG_SET_INFO ret = %d \n", ret);
		
		ret = LtyMessageSend(LTY_MSG_ITSSD_SET_PARA, SD_PARA_SWITCH, (LTY_U32)&stSwitchSet, sizeof(stSwitchSet));
		DBGBOUT("gui_system_switch_config_set() SD_PARA_SWITCH ret = %d \n", ret);
	

	
	}while(0);
}

void switch_config_common_get(cJSON *root)
{
	do
	{
		
		int ret = 0;
		switch_set_t stSwitchSet;

		memset(&g_door_config_info, 0, sizeof(DEV_DOOR_CONFIG_INFO_T));
		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DOOR_CONFIG_SET_INFO, (LTY_U32)&g_door_config_info, sizeof(DEV_DOOR_CONFIG_INFO_T));
		if (LTY_SUCCESS != ret)
		{
			printf("gui_system_switch_config_get CFG_PARA_DOOR_CONFIG_SET_INFO error\n");
			break;
		}

	   	

		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_SWITCH, (LTY_U32)&stSwitchSet, sizeof(stSwitchSet));
	    if(LTY_SUCCESS != ret)
	    {
	        DBGPOUT("Get SD Switch Para Failure .....\n");
			break;
	    }

		cJSON_AddStringToObject(root, "msg_type", "gui_system_switch_config");
		/*���ż��*/
		char door_type = 0;
		if(0 == stSwitchSet.u8DoorType)
			door_type = 1;

		cJSON_AddNumberToObject(root, "enable",stSwitchSet.u8EnDoorDetect );/*0:δѡ��״̬ 1:ѡ��״̬*/ 
		cJSON_AddNumberToObject(root, "car_door_number",stSwitchSet.u8DoorNum - 1 );/*�������� 0:1 1:2 2:3*/
		cJSON_AddNumberToObject(root, "car_door_type",door_type );/*��������  0:���������š�1:�������š�2:�����*/
		cJSON_AddNumberToObject(root, "door_front",stSwitchSet.u8InverseDoor1PinFlag );/*��ǰ�ŵ�ƽ 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		cJSON_AddNumberToObject(root, "door_middle",stSwitchSet.u8InverseDoor2PinFlag );/*�����ŵ�ƽ 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		cJSON_AddNumberToObject(root, "door_back", stSwitchSet.u8InverseDoor3PinFlag);/*�����ŵ�ƽ 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		cJSON_AddNumberToObject(root, "io_input1", g_door_config_info.io_input1);/*I/O����1 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		/*���ż�⣭����*/

#if 0
		cJSON_AddNumberToObject(root, "speed_open_door_check", g_door_config_info.speed_open_door_check);/*���ٶȿ��ż��*/
		cJSON_AddNumberToObject(root, "station_open_door_check",g_door_config_info.station_open_door_check );/*վ�俪�ż��*/
		cJSON_AddNumberToObject(root, "arrive_open_door_check", g_door_config_info.arrive_open_door_check);/*��վδ���ż��*/
		cJSON_AddNumberToObject(root, "arrive_stop_door_check", g_door_config_info.arrive_stop_door_check);/*��վδͣ�����*/
		cJSON_AddNumberToObject(root, "speed_enable", g_door_config_info.speed_enable);/*�쳣��������С�ٶ�ʹ�� 0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "speed",g_door_config_info.speed );/*�쳣��������С�ٶ�*/
		cJSON_AddNumberToObject(root, "frequency_enable",g_door_config_info.frequency_enable );/*�쳣����������ʹ�� 0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "frequency", g_door_config_info.frequency);/*�쳣���������Ѵ���*/
		/*�������������*/

		
		cJSON_AddNumberToObject(root, "back_car_monitor_enable", g_door_config_info.back_car_monitor_enable);/*�������� 0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "back_car_monitor_status", g_door_config_info.back_car_monitor_status);/*�������� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		cJSON_AddNumberToObject(root, "no_brake_enable", g_door_config_info.no_brake_enable);/*δɲ�� 0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "no_brake_status",stSwitchSet.u8InverseBreakPinFlag );/*δɲ�� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		cJSON_AddNumberToObject(root, "open_front_box_enable",g_door_config_info.open_front_box_enable );/*��ǰ�� 0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "open_front_box_status", stSwitchSet.u8InversePicketPinFlag);/*��ǰ�� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		cJSON_AddNumberToObject(root, "wiper_enable", g_door_config_info.wiper_enable);/*��� 0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "wiper_status", stSwitchSet.u8YuGuaLevelFlag);/*��� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		cJSON_AddNumberToObject(root, "left_lamp_enable", g_door_config_info.left_lamp_enable);/*��ת�� 0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "left_lamp_status", g_door_config_info.left_lamp_status);/*��ת�� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
		cJSON_AddNumberToObject(root, "right_lamp_enable", g_door_config_info.right_lamp_enable);/*��ת�� 0:δѡ��״̬ 1:ѡ��״̬*/
		cJSON_AddNumberToObject(root, "right_lamp_status", g_door_config_info.right_lamp_status);/*��ת�� 0:�͵�ƽ��Ч��1:�ߵ�ƽ��Ч*/
#endif
	}while(0);
}


void gui_system_switch_config_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��������*/
			switch_config_common_set(root);
			
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/
			gui_system_switch_config_get();
		}
		

		
	}while(0);
}

void gui_system_switch_config_get()
{
	do
	{
	    cJSON *root = NULL;
		
		root = cJSON_CreateObject();
	   	if(NULL == root)
	   	{   
	   	 	break;
	   	}

		switch_config_common_get(root);



		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);
}


void gui_system_wifi_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��ȡWIFI��Ϣ*/
		
		}
		else if(2 == objValue->valueint)
		{/*����WIFI*/
			objValue = cJSON_GetObjectItem(root,"ssid_name");
			if(objValue && objValue->valuestring)
			{/*SSID����*/

			}
			objValue = cJSON_GetObjectItem(root,"security");
			if(objValue)
			{/*��ȫ�� 0:WEP  1:WPA-PSK  2:WPA2-PSK  3:WAPI*/

			}
			objValue = cJSON_GetObjectItem(root,"password");
			if(objValue && objValue->valuestring)
			{/*����*/
			
			}
		}
		else if(3 == objValue->valueint)
		{/*ֹͣWIFI����*/
		
		}

		
	}while(0);

}
void gui_system_wifi_get()
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	


		cJSON_AddStringToObject(root, "msg_type", "gui_system_wifi");

		int type = 0;
		int number = 1;
		char name[20];
		char signal[20];
		int lock = 0;
		

        cJSON_AddNumberToObject(root, "type",type);/*1:��ȡ���� 2:����WIFI�ɹ� 3:����WIFIʧ��*/
		cJSON_AddNumberToObject(root, "number",number);/*��� ��1��ʼ*/
		cJSON_AddStringToObject(root, "name",name); /*SSID����*/
        cJSON_AddStringToObject(root, "signal",signal);/*�ź�ǿ�ȡ�-70Db*/
		cJSON_AddNumberToObject(root, "lock",lock);/*0:������ 1:��*/


		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);
}


void gui_system_network_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	LTY_RESULT ret = 0;
	cfg_para_domain_s_t stNetDomainSet;
	cfg_para_domain_ex_s_t stNetDomainSetEx;
	net_terrace_set_t  stItsNet;

	memset(&stNetDomainSetEx,0,sizeof(cfg_para_domain_ex_s_t));
	
	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��������*/
			/*1:�ƶ�����-----------------------------------------------------*/
			objValue = cJSON_GetObjectItem(root,"mobile_network_enable");
			if(objValue)
			{/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
				g_stCfg_sys_net.stMobile.u8Enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"mobile_network_operator1");
			if(objValue)
			{/*��Ӫ��  0:�Զ� 1:�ֶ�*/
				g_stCfg_sys_net.stMobile.u8SearchMode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"mobile_network_type");
			if(objValue)
			{/*�������� 0:4G 1:3G 2:2G*/
				if(0 == objValue->valueint)
					g_stCfg_sys_net.stMobile.u8NetType = CFG_NET_MOBILE_NT_4G;
				else if(1 == objValue->valueint)
					g_stCfg_sys_net.stMobile.u8NetType = CFG_NET_MOBILE_NT_3G;
				else if(2 == objValue->valueint)
					g_stCfg_sys_net.stMobile.u8NetType = CFG_NET_MOBILE_NT_2G;
				else 
					g_stCfg_sys_net.stMobile.u8NetType = CFG_NET_MOBILE_NT_AUTO;
			}
			objValue = cJSON_GetObjectItem(root,"mobile_network_month_end_date");
			if(objValue)
			{/*�½���*/
				g_stCfg_sys_net.stMobile.u8Month = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"mobile_network_use_vpn");
			if(objValue)
			{/*�Ƿ�ʹ��VPN  0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
				g_stCfg_sys_net.stMobile.u8EnableAPN = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"mobile_network_vpn_name");
			if(objValue && objValue->valuestring)
			{/*VPN����*/
				memset(g_stCfg_sys_net.stMobile.szAPN, 0, sizeof(g_stCfg_sys_net.stMobile.szAPN));
				strncpy(g_stCfg_sys_net.stMobile.szAPN, objValue->valuestring,sizeof(g_stCfg_sys_net.stMobile.szAPN));
			}
			objValue = cJSON_GetObjectItem(root,"mobile_network_vpn_account");
			if(objValue && objValue->valuestring)
			{/*VPN�˺�*/
				memset(g_stCfg_sys_net.stMobile.szUsr, 0, sizeof(g_stCfg_sys_net.stMobile.szUsr));
				strncpy(g_stCfg_sys_net.stMobile.szUsr,objValue->valuestring, sizeof(g_stCfg_sys_net.stMobile.szUsr));
			}
			objValue = cJSON_GetObjectItem(root,"mobile_network_vpn_password");
			if(objValue && objValue->valuestring)
			{/*VPN����*/
				memset(g_stCfg_sys_net.stMobile.szPsw, 0, sizeof(g_stCfg_sys_net.stMobile.szPsw));
				strncpy(g_stCfg_sys_net.stMobile.szPsw,objValue->valuestring,sizeof(g_stCfg_sys_net.stMobile.szPsw));
			}
			/*2:���ķ�����1-----------------------------------------------------*/
			objValue = cJSON_GetObjectItem(root,"center_server1_enable");
			if(objValue)
			{/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
				g_stCfg_SvrNet.u8ItsEnable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"center_server1_protocol_type");
			if(objValue)
			{/*ѡ�е�Э������*/
				g_stCfg_SvrNet.u8ItsProtocol = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"center_server1_access_mode");
			if(objValue)
			{/*����ģʽ 0:�̼�IP��ַ 1:����*/
				g_stCfg_SvrNet.its_access_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"center_server1_ip");
			if(objValue && objValue->valuestring)
			{/*IP��ַ������*/
				LTY_U32 serverIp = 0;
				serverIp = SockStrToIp(objValue->valuestring);
				memcpy(&g_stCfg_SvrNet.u32ItsIP, &serverIp, sizeof(LTY_U32));
				memcpy(stNetDomainSet.u8ItsDomain,objValue->valuestring,sizeof(stNetDomainSet.u8ItsDomain));
				stItsNet.u32GPRSServerIPAddress     = ntohl(serverIp);
			}
			objValue = cJSON_GetObjectItem(root,"center_server1_port");
			if(objValue)
			{/*�˿�*/
				g_stCfg_SvrNet.u16ItsPort = objValue->valueint;
				stItsNet.u16GPRSServerPort = g_stCfg_SvrNet.u16ItsPort;
			}
			objValue = cJSON_GetObjectItem(root,"center_server1_data_send_interval");
			if(objValue)
			{/*���ݷ��ͼ�� (��)*/
				g_stCfg_SvrNet.u16ItsHBInterval = objValue->valueint;
			}
			//stItsNet.u8ForbidNetLinkFlag        = s32Proto;
		    
		    
		    //stItsNet.u8SendGPSPktInterval       = s32Interval;
		    //stItsNet.u8SendGPSPktJuLiInterval   = s32CanInterval;
		    //stItsNet.u16SleepSendGPSPktInterval = 0;

    		LtyMessageSend(LTY_MSG_ITSSD_SET_PARA, SD_PARA_NET_CFG, (LTY_U32)&stItsNet, sizeof(stItsNet));
			
			/*3:���ķ�����2-----------------------------------------------------*/
			objValue = cJSON_GetObjectItem(root,"center_server2_enable");
			if(objValue)
			{/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
				g_stCfg_svr_net_ex.u8ItsEnable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"center_server2_protocol_type");
			if(objValue)
			{/*ѡ�е�Э������*/
				g_stCfg_svr_net_ex.u8ItsProtocol = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"center_server2_access_mode");
			if(objValue)
			{/*����ģʽ 0:�̼�IP��ַ 1:����*/
				g_stCfg_svr_net_ex.its_access_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"center_server2_ip");
			ip_param_t ip_param;
			memset(&ip_param,0,sizeof(ip_param_t));
			if(objValue && objValue->valuestring)
			{/*IP��ַ������*/
				LTY_U32 serverIp = 0;
				serverIp = SockStrToIp(objValue->valuestring);
				memcpy(&g_stCfg_svr_net_ex.u32ItsIP, &serverIp, sizeof(LTY_U32));
				
				memcpy(stNetDomainSetEx.u8TbpDomain,objValue->valuestring,sizeof(stNetDomainSetEx.u8TbpDomain));
				memcpy(ip_param.ip_addr,objValue->valuestring,sizeof(ip_param.ip_addr));
			}
			objValue = cJSON_GetObjectItem(root,"center_server2_port");
			if(objValue)
			{/*�˿�*/
				g_stCfg_svr_net_ex.u16ItsPort = objValue->valueint;
				ip_param.ip_port = objValue->valueint;
			}
			Write_ip_param(ip_param);
			objValue = cJSON_GetObjectItem(root,"center_server2_data_send_interval");
			if(objValue)
			{/*���ݷ��ͼ�� (��)*/
				g_stCfg_svr_net_ex.u16ItsHBInterval = objValue->valueint;
			}
			
			/*4:��Ƶ������-----------------------------------------------------*/
			objValue = cJSON_GetObjectItem(root,"video_server_enable");
			if(objValue)
			{/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
				g_stCfg_SvrNet.u8StrmEnable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"video_server_protocol_type");
			if(objValue)
			{/*ѡ�е�Э������*/
				g_stCfg_SvrNet.u8StrmProtocol = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"video_server_access_mode");
			if(objValue)
			{/*����ģʽ 0:��̬IP 1:�Զ���ȡ*/
				g_stCfg_SvrNet.video_access_mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"video_server_ip");
			if(objValue && objValue->valuestring)
			{/*IP��ַ������*/
				LTY_U32 videoIp = 0;
				videoIp = SockStrToIp(objValue->valuestring);
				memcpy(&g_stCfg_SvrNet.u32StrmIP, &videoIp, sizeof(LTY_U32));
				memcpy(stNetDomainSet.u8StrmDomain,objValue->valuestring,sizeof(stNetDomainSet.u8StrmDomain));
			}
			objValue = cJSON_GetObjectItem(root,"video_server_port");
			if(objValue)
			{/*�˿�*/
				g_stCfg_SvrNet.u16StrmPort = objValue->valueint;
			}

			/*5:��������-----------------------------------------------------*/
			objValue = cJSON_GetObjectItem(root,"local_network_enable");
			if(objValue)
			{/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
				g_stCfg_sys_net.stEther.u8Enable = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"local_network_dhcp_server");
			if(objValue)
			{/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/ 
				g_stCfg_sys_net.stEther.u8DHcp = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"local_network_access_mode");
			if(objValue)
			{/*����ģʽ 0:��̬IP 1:�Զ���ȡ*/
				g_stCfg_sys_net.stEther.u8Mode = objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"local_network_ip");
			if(objValue && objValue->valuestring)
			{/*IP��ַ*/
				LTY_U32 network_ip = 0;
				network_ip = SockStrToIp(objValue->valuestring);
				memcpy(&g_stCfg_sys_net.stEther.u32IPAddr, &network_ip, sizeof(LTY_U32));

				unsigned char ip_addr[50];
				int ip_len;
				memset(&ip_addr,0,sizeof(ip_addr));
				if(strlen(objValue->valuestring) >= 28)
					ip_len = 27;
				memcpy(ip_addr,objValue->valuestring,ip_len);
				Write_local_ip_param(ip_addr);
				
			}
			objValue = cJSON_GetObjectItem(root,"local_network_subnet_mask");
			if(objValue && objValue->valuestring)
			{/*��������*/
				LTY_U32 network_subnet_mask = 0;
				network_subnet_mask = SockStrToIp(objValue->valuestring);
				memcpy(&g_stCfg_sys_net.stEther.u32NetMask, &network_subnet_mask, sizeof(LTY_U32));
			}
			objValue = cJSON_GetObjectItem(root,"local_network_gateway");
			if(objValue && objValue->valuestring)
			{/*����*/
				LTY_U32 network_gateway = 0;
				network_gateway = SockStrToIp(objValue->valuestring);
				memcpy(&g_stCfg_sys_net.stEther.u32GateWay, &network_gateway, sizeof(LTY_U32));
			}
			objValue = cJSON_GetObjectItem(root,"local_network_nds1");
			if(objValue && objValue->valuestring)
			{/*DNS1*/
				//objValue->valuestring;
				LTY_U32 network_nds1 = 0;
				network_nds1 = SockStrToIp(objValue->valuestring);
				memcpy(&g_stCfg_sys_net.stEther.u32DNS1, &network_nds1, sizeof(LTY_U32));
			}
			objValue = cJSON_GetObjectItem(root,"local_network_nds2");
			if(objValue && objValue->valuestring)
			{/*DNS2*/
				LTY_U32 network_nds2 = 0;
				network_nds2 = SockStrToIp(objValue->valuestring);
				memcpy(&g_stCfg_sys_net.stEther.u32DNS2, &network_nds2, sizeof(LTY_U32));
			}
			/*6:WIFI-----------------------------------------------------*/
			objValue = cJSON_GetObjectItem(root,"wifi_enable");
			if(objValue)
			{/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"wifi_access_mode");
			if(objValue)
			{/*����ģʽ 0:��̬IP 1:�Զ���ȡ*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"wifi_ip");
			if(objValue && objValue->valuestring)
			{/*IP��ַ*/
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"wifi_subnet_mask");
			if(objValue && objValue->valuestring)
			{/*��������*/
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"wifi_gateway");
			if(objValue && objValue->valuestring)
			{/*����*/
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"wifi_nds1");
			if(objValue && objValue->valuestring)
			{/*DNS1*/
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"wifi_nds2");
			if(objValue && objValue->valuestring)
			{/*DNS2*/
				//objValue->valuestring;
			}
			/*7.FTP-----------------------------------------------------*/
			objValue = cJSON_GetObjectItem(root,"ftp_enable");
			if(objValue)
			{/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"ftp_access_mode");
			if(objValue)
			{/*����ģʽ 0:��̬IP 1:����*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"ftp_ip");
			if(objValue && objValue->valuestring)
			{/*IP��ַ*/
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"ftp_gateway");
			if(objValue && objValue->valuestring)
			{/*����*/
				//objValue->valuestring;
			}
			ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_SYS_NET, (LTY_U32)&g_stCfg_sys_net, sizeof(cfg_para_sys_net_t));
			DBGBOUT("save network config ret = %d \n", ret);

			ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_SVR_NET, (LTY_U32)&g_stCfg_SvrNet, sizeof(cfg_para_svr_net_t));
			DBGBOUT("save media server config ret = %d \n", ret);

			ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_CENTER_SERVER2, (LTY_U32)&g_stCfg_svr_net_ex, sizeof(g_stCfg_svr_net_ex));
			printf("save center server2 ret:%d \n", ret);

			ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_DOMAIN_INFO, (LTY_U32)&stNetDomainSet, sizeof(stNetDomainSet));
			
			if(LTY_SUCCESS != ret)
			{
				DBGPOUT("gui_system_network_get() CFG_PARA_DOMAIN_INFO config error, %s\n", LtyErrStr(ret));
				
			}
			ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_DOMAIN_INFO_EX, (LTY_U32)&stNetDomainSetEx, sizeof(stNetDomainSetEx));

			
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/
			gui_system_network_get();
		}
		
		
		
	}while(0);

}
void gui_system_network_get()
{
	do
	{
	   cJSON *root = NULL;
	   LTY_RESULT ret;
	   
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	


		cJSON_AddStringToObject(root, "msg_type", "gui_system_network");


		memset(&g_stCfg_sys_net, 0, sizeof(g_stCfg_sys_net));
		memset(&g_stCfg_SvrNet, 0, sizeof(g_stCfg_SvrNet));


	    ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYS_NET, (LTY_U32)&g_stCfg_sys_net, sizeof(g_stCfg_sys_net));
		printf("gui_system_network_get() ret:%d \n", ret);
	    if(LTY_SUCCESS != ret)
	    {
	        DBGPOUT("gui_system_network_get() error 1, %s\n", LtyErrStr(ret));
	        
			break;
	    }
		
	    ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SVR_NET, (LTY_U32)&g_stCfg_SvrNet, sizeof(g_stCfg_SvrNet));
		printf("g_stCfg_SvrNet ret:%d \n", ret);
	    if(LTY_SUCCESS != ret)
	    {
	        DBGPOUT("gui_system_network_get() config error, %s\n", LtyErrStr(ret));
			break;
	    }


		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_CENTER_SERVER2, (LTY_U32)&g_stCfg_svr_net_ex, sizeof(g_stCfg_svr_net_ex));
		printf("g_stCfg_svr_net_ex ret:%d \n", ret);
		if(LTY_SUCCESS != ret)
		{
			DBGPOUT("gui_system_network_get() config error, %s\n", LtyErrStr(ret));
			break;
		}

		cfg_para_domain_s_t stNetDomainSet;
        ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DOMAIN_INFO, (LTY_U32)&stNetDomainSet, sizeof(stNetDomainSet));
		if(LTY_SUCCESS != ret)
		{
			DBGPOUT("gui_system_network_get() CFG_PARA_DOMAIN_INFO config error, %s\n", LtyErrStr(ret));
			break;
		}
		cfg_para_domain_ex_s_t stNetDomainSetEx;
		
		memset(&stNetDomainSetEx,0,sizeof(cfg_para_domain_ex_s_t));
		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DOMAIN_INFO_EX, (LTY_U32)&stNetDomainSetEx, sizeof(stNetDomainSetEx));
		if(LTY_SUCCESS != ret)
		{
			DBGPOUT("gui_system_network_get() CFG_PARA_DOMAIN_INFO_EX config error, %s\n", LtyErrStr(ret));
			break;
		}
		
		//1:�ƶ�����
		int mobile_network_enable = g_stCfg_sys_net.stMobile.u8Enable;
		int mobile_network_operator1 = g_stCfg_sys_net.stMobile.u8SearchMode;
		int mobile_network_type = 0;
		int mobile_network_month_end_date = g_stCfg_sys_net.stMobile.u8Month;
		int mobile_network_use_vpn = g_stCfg_sys_net.stMobile.u8EnableAPN;
		char mobile_network_vpn_name[32] = {0};
		char mobile_network_vpn_account[16] = {0};
		char mobile_network_vpn_password[16] = {0};
		char modular_name1[20] = {0};
		char modular_name2[20] = {0};
		char modular_version[20] = {0};
		char modular_imei[20] = {0};
		char modular_sn[20] = {0};
		char modular_meid[20] = {0};

		if(CFG_NET_MOBILE_NT_AUTO == g_stCfg_sys_net.stMobile.u8NetType)
			mobile_network_type = 0;
		else if(CFG_NET_MOBILE_NT_2G == g_stCfg_sys_net.stMobile.u8NetType)
			mobile_network_type = 2;
		else if(CFG_NET_MOBILE_NT_3G == g_stCfg_sys_net.stMobile.u8NetType)
			mobile_network_type = 1;
		else if(CFG_NET_MOBILE_NT_4G == g_stCfg_sys_net.stMobile.u8NetType)
			mobile_network_type = 0;

		memset(mobile_network_vpn_name, 0, sizeof(mobile_network_vpn_name));
		strcpy(mobile_network_vpn_name, g_stCfg_sys_net.stMobile.szAPN);

		memset(mobile_network_vpn_account, 0, sizeof(mobile_network_vpn_account));
		strcpy(mobile_network_vpn_account, g_stCfg_sys_net.stMobile.szUsr);

		memset(mobile_network_vpn_password, 0, sizeof(mobile_network_vpn_password));
		strcpy(mobile_network_vpn_password, g_stCfg_sys_net.stMobile.szPsw);

		cJSON_AddNumberToObject(root, "mobile_network_enable", mobile_network_enable);/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddNumberToObject(root, "mobile_network_operator1", mobile_network_operator1);/*��Ӫ��  0:�Զ� 1:�ֶ�*/
		cJSON_AddNumberToObject(root, "mobile_network_type", mobile_network_type);/*�������� 0:4G 1:3G 2:2G*/
		cJSON_AddNumberToObject(root, "mobile_network_month_end_date", mobile_network_month_end_date);/*�½���*/
		cJSON_AddNumberToObject(root, "mobile_network_use_vpn", mobile_network_use_vpn);/*�Ƿ�ʹ��VPN  0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddStringToObject(root, "mobile_network_vpn_name", mobile_network_vpn_name);/*VPN����*/
		cJSON_AddStringToObject(root, "mobile_network_vpn_account", mobile_network_vpn_account);/*VPN�˺�*/
		cJSON_AddStringToObject(root, "mobile_network_vpn_password", mobile_network_vpn_password);/*VPN����*/

		cJSON_AddStringToObject(root, "modular_name1", modular_name1);/*ģ���ͺ�����1 ����*/
		cJSON_AddStringToObject(root, "modular_name2", modular_name2);/*ģ���ͺ�����1 8300C*/
		cJSON_AddStringToObject(root, "modular_version", modular_version);/*����汾*/
		cJSON_AddStringToObject(root, "modular_imei", modular_imei);/*IMEI��*/
		cJSON_AddStringToObject(root, "modular_sn", modular_sn);/*S/N��*/
		cJSON_AddStringToObject(root, "modular_meid", modular_meid);/*MEID*/
		
		
		//2:���ķ�����1
		int center_server1_enable = g_stCfg_SvrNet.u8ItsEnable;
		int center_server1_protocol_type = g_stCfg_SvrNet.u8ItsProtocol;
		int center_server1_access_mode = g_stCfg_SvrNet.its_access_mode;
		char center_server1_ip[20] = {0};
		int center_server1_port = g_stCfg_SvrNet.u16ItsPort;
		int center_server1_data_send_interval = g_stCfg_SvrNet.u16ItsHBInterval;
		

		memset(center_server1_ip, 0, sizeof(center_server1_ip));
		strcpy(center_server1_ip, SockIpToStr(g_stCfg_SvrNet.u32ItsIP));
	
		cJSON_AddNumberToObject(root, "center_server1_enable", center_server1_enable);/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddNumberToObject(root, "center_server1_protocol_type", center_server1_protocol_type);/*ѡ�е�Э������*/
		cJSON_AddNumberToObject(root, "center_server1_access_mode", center_server1_access_mode);/*����ģʽ 0:�̼�IP��ַ 1:����*/
		cJSON_AddStringToObject(root, "center_server1_ip", (char *)stNetDomainSet.u8ItsDomain/*center_server1_ip*/);/*IP��ַ������*/
		cJSON_AddNumberToObject(root, "center_server1_port", center_server1_port);/*�˿�*/
		cJSON_AddNumberToObject(root, "center_server1_data_send_interval", center_server1_data_send_interval);/*���ݷ��ͼ�� (��)*/

		//cJSON *pJson_row = NULL;
		//char value[20] = {0};
		#if 0
		int array_size = 30;
        cJSON *pJson_rows1 = cJSON_CreateArray();

        cJSON_AddItemToObject(root, "rows1",pJson_rows1);
	
        
		
		
        for(int index = 0;index < array_size;index++)  /*Э���б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows1,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",value); /*Э������*/
        }
		#endif
		//3:���ķ�����2
		int center_server2_enable = g_stCfg_svr_net_ex.u8ItsEnable;
		int center_server2_protocol_type = g_stCfg_svr_net_ex.u8ItsProtocol;
		int center_server2_access_mode = g_stCfg_svr_net_ex.its_access_mode;
		char center_server2_ip[20] = {0};
		int center_server2_port = g_stCfg_svr_net_ex.u16ItsPort;
		int center_server2_data_send_interval = g_stCfg_svr_net_ex.u16ItsHBInterval;

		memset(center_server2_ip, 0, sizeof(center_server2_ip));
		strcpy(center_server2_ip, SockIpToStr(g_stCfg_svr_net_ex.u32ItsIP));
#if 0
		ip_param_t ip_param;
		memset(&ip_param,0,sizeof(ip_param));
		int ret_value = Read_ip_param(&ip_param);
#endif
		
		cJSON_AddNumberToObject(root, "center_server2_enable", center_server2_enable);/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddNumberToObject(root, "center_server2_protocol_type", center_server2_protocol_type);/*ѡ�е�Э������*/
		cJSON_AddNumberToObject(root, "center_server2_access_mode", center_server2_access_mode);/*����ģʽ 0:�̼�IP��ַ 1:����*/
#ifdef PRODUCT_P2
		cJSON_AddStringToObject(root, "center_server2_ip", center_server2_ip/*center_server2_ip*/);
		cJSON_AddNumberToObject(root, "center_server2_port", center_server2_port);
#else
		cJSON_AddStringToObject(root, "center_server2_ip", (char *)stNetDomainSetEx.u8TbpDomain/*center_server2_ip*/);/*IP��ַ������*/
		cJSON_AddNumberToObject(root, "center_server2_port", center_server2_port);
#endif
		/*�˿�*/
		cJSON_AddNumberToObject(root, "center_server2_data_send_interval", center_server2_data_send_interval);/*���ݷ��ͼ�� (��)*/

		
		//DBGBOUT("ret_value = %d ip_addr:%s , ip_port:%d\n", ret_value,ip_param.ip_addr,ip_param.ip_port);
		
		#if 0
	    int array_size2 = 30;
        cJSON *pJson_rows2 = cJSON_CreateArray();

        cJSON_AddItemToObject(root, "rows2",pJson_rows2);
		
        for(int index = 0;index < array_size2;index++)  /*Э���б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows2,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",value); /*Э������*/
        }
		#endif
		//4:��Ƶ������
		int video_server_enable = g_stCfg_SvrNet.u8StrmEnable;
		int video_server_protocol_type = g_stCfg_SvrNet.u8StrmProtocol;
		int video_server_access_mode = g_stCfg_SvrNet.video_access_mode;
		char video_server_ip[20] = {0};
		int video_server_port = g_stCfg_SvrNet.u16StrmPort;

		memset(video_server_ip, 0, sizeof(video_server_ip));
		strcpy(video_server_ip, SockIpToStr(g_stCfg_SvrNet.u32StrmIP));
		

		cJSON_AddNumberToObject(root, "video_server_enable", video_server_enable);/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddNumberToObject(root, "video_server_protocol_type", video_server_protocol_type);/*ѡ�е�Э������*/
		cJSON_AddNumberToObject(root, "video_server_access_mode", video_server_access_mode);/*����ģʽ 0:��̬IP 1:�Զ���ȡ*/
		cJSON_AddStringToObject(root, "video_server_ip", (char *)stNetDomainSet.u8StrmDomain/*video_server_ip*/);/*IP��ַ������*/
		cJSON_AddNumberToObject(root, "video_server_port", video_server_port);/*�˿�*/
#if 0
		int array_size3 = 30;
        cJSON *pJson_rows3 = cJSON_CreateArray();

        cJSON_AddItemToObject(root, "rows3",pJson_rows3);
		
        for(int index = 0;index < array_size3;index++)  /*Э���б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows3,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",value); /*Э������*/
        }
#endif
		//5:��������
		int local_network_enable = g_stCfg_sys_net.stEther.u8Enable;
		int local_network_dhcp_server = g_stCfg_sys_net.stEther.u8DHcp;
		int local_network_access_mode = g_stCfg_sys_net.stEther.u8Mode;
		char local_network_ip[32] = {0};
		char local_network_subnet_mask[32] = {0};
		char local_network_gateway[32] = {0};
		char local_network_nds1[32] = {0};
		char local_network_nds2[32] = {0};
		
		memset(local_network_ip, 0, sizeof(local_network_ip));
		strcpy(local_network_ip, SockIpToStr(g_stCfg_sys_net.stEther.u32IPAddr));

		memset(local_network_gateway, 0, sizeof(local_network_gateway));
		strcpy(local_network_gateway, SockIpToStr(g_stCfg_sys_net.stEther.u32GateWay));

		memset(local_network_subnet_mask, 0, sizeof(local_network_subnet_mask));
		strcpy(local_network_subnet_mask, SockIpToStr(g_stCfg_sys_net.stEther.u32NetMask));

		memset(local_network_nds1, 0, sizeof(local_network_nds1));
		strcpy(local_network_nds1, SockIpToStr(g_stCfg_sys_net.stEther.u32DNS1));

		memset(local_network_nds2, 0, sizeof(local_network_nds2));
		strcpy(local_network_nds2, SockIpToStr(g_stCfg_sys_net.stEther.u32DNS2));

		cJSON_AddNumberToObject(root, "local_network_enable",local_network_enable );/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddNumberToObject(root, "local_network_dhcp_server",local_network_dhcp_server );/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddNumberToObject(root, "local_network_access_mode", local_network_access_mode);/*����ģʽ 0:��̬IP 1:�Զ���ȡ*/
		cJSON_AddStringToObject(root, "local_network_ip", local_network_ip);/*IP��ַ*/
		cJSON_AddStringToObject(root, "local_network_subnet_mask", local_network_subnet_mask);/*��������*/
		cJSON_AddStringToObject(root, "local_network_gateway", local_network_gateway);/*����*/
		cJSON_AddStringToObject(root, "local_network_nds1", local_network_nds1);/*DNS1*/
		cJSON_AddStringToObject(root, "local_network_nds2", local_network_nds2);/*DNS2*/

		unsigned char ip_addr[30] = {0};

		Read_local_ip_param(ip_addr);
		
		printf("ip_addr:%s ,local_network_ip:%s\n",ip_addr,local_network_ip);

		if(strcmp((const char *)ip_addr,local_network_ip))	
			Write_local_ip_param((unsigned char *)local_network_ip);

		
		//6:WIFI
		int wifi_enable = 0;
		int wifi_access_mode = 0;
		char wifi_ip[20] = {0};
		char wifi_subnet_mask[20] = {0};
		char wifi_gateway[20] = {0};
		char wifi_nds1[20] = {0};
		char wifi_nds2[20] = {0};
		cJSON_AddNumberToObject(root, "wifi_enable",wifi_enable);/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddNumberToObject(root, "wifi_access_mode", wifi_access_mode);/*����ģʽ 0:��̬IP 1:�Զ���ȡ*/
		cJSON_AddStringToObject(root, "wifi_ip", wifi_ip);/*IP��ַ*/
		cJSON_AddStringToObject(root, "wifi_subnet_mask", wifi_subnet_mask);/*��������*/
		cJSON_AddStringToObject(root, "wifi_gateway", wifi_gateway);/*����*/
		cJSON_AddStringToObject(root, "wifi_nds1", wifi_nds1);/*DNS1*/
		cJSON_AddStringToObject(root, "wifi_nds2", wifi_nds2);/*DNS2*/
		
		//7:FTP
		int ftp_enable = 0;
		int ftp_access_mode = 0;
		char ftp_ip[20] = {0};
		char ftp_gateway[20] = {0};
		cJSON_AddNumberToObject(root, "ftp_enable",ftp_enable );/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddNumberToObject(root, "ftp_access_mode", ftp_access_mode);/*����ģʽ 0:��̬IP 1:����*/
		cJSON_AddStringToObject(root, "ftp_ip",ftp_ip );/*IP��ַ*/
		cJSON_AddStringToObject(root, "ftp_gateway", ftp_gateway);/*����*/



		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}


void gui_system_operate_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*2:����*/
	
			LtyMessagePost(LTY_MSG_SYSCTRL_SET_SYS_SHUTDOWN, 0, SC_SD_REBOOT, 0);
		}
		else if(2 == objValue->valueint)
		{/*�ػ�*/
			
		}
		
		
		
	}while(0);

}

void update_sdcardformat_progress(LTY_U32 total_size, LTY_U32 cur_size, LTY_S32 state)
{
	cJSON *root = NULL;
	root = cJSON_CreateObject();
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "sdcard_format_progress");
		cJSON_AddNumberToObject(root, "riu_id", 1);
		cJSON_AddNumberToObject(root, "total_size", total_size);
		cJSON_AddNumberToObject(root, "cur_size", cur_size);
		cJSON_AddNumberToObject(root, "status", state);

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}

	return ;
}


static void* format_sdcard_ProgressBar(void *arg)
{
	pthread_detach( pthread_self());
	
	cJSON *cjson = NULL;
	
	//int *chnl = 0;
	double val = 0.0;
	double val1 = 1.0;
	int formatprogress = 0;

	//chnl = (int*)arg;
	
	cjson = cJSON_CreateObject();
	if(NULL == cjson)
	{
		return NULL;
	}
	
	cJSON_AddStringToObject(cjson, "msg_type", "sdcard_format_progressbar");
	cJSON_AddNumberToObject(cjson, "riu_id", val1);
	cJSON_AddNumberToObject(cjson, "value", val);
	cJSON_AddNumberToObject(cjson, "status", val1);

	while(1)
	{//0 conitue; 1 finish; -1 failed.
		LTY_CHAR u8formatstatus;
        LtyMessageSend(LTY_MSG_REC_GETFMSTATUS, FORMAT_STATUS, (LTY_U32)&u8formatstatus, sizeof(LTY_U32));
        if (MTSB_FORMATHDD_DOING == u8formatstatus){
			LOGOUT("formating ---- %d%% \n",formatprogress);
            LtyMessageSend(LTY_MSG_REC_GETFMSTATUS, FORMAT_PROG, (LTY_U32)&formatprogress, sizeof(int));
			update_sdcardformat_progress(100,formatprogress,0);
        }
        else if (MTSB_FORMATHDD_FAIL == u8formatstatus){
    		update_sdcardformat_progress(100,formatprogress,-1);
			break;
        }
        else if (MTSB_FORMATHDD_NOHDD  == u8formatstatus){
			update_sdcardformat_progress(100,formatprogress,-1);
			break;
        }
        else if (MTSB_FORMATHDD_NETRELAY == u8formatstatus){
  			update_sdcardformat_progress(100,formatprogress,-1);
			break;
        }
        else if (MTSB_FORMATHDD_SUCC  == u8formatstatus){
			update_sdcardformat_progress(100,formatprogress,1);
			break;
        }
		sleep(1);
	
		#if 0
		LtyMessageSend(LTY_MSG_REC_GETFMSTATUS, FORMAT_PROG, (LTY_U32)&formatprogress, sizeof(int));
		LOGOUT("formating ---- %d%% \n",formatprogress);
		if(formatprogress > 0)
		{//0 conitue; 1 finish; -1 failed.
			#if 0
			cJSON_ReplaceItemInObject(cjson, "value", cJSON_CreateNumber((double)formatprogress));
			cJSON_ReplaceItemInObject(cjson, "status", cJSON_CreateNumber(val1));
			lytSendMsgToUiAppCall(cjson, ltySendMessge2UiCallback);
			#endif
			update_sdcardformat_progress(100,formatprogress,0);
			timeout = 0;
		}
		else if(timeout > 60)
		{
			update_sdcardformat_progress(100,formatprogress,-1);
			#if 0
			cJSON_ReplaceItemInObject(cjson, "value", cJSON_CreateNumber(val));
			cJSON_ReplaceItemInObject(cjson, "status", cJSON_CreateNumber(val));
			#endif
			break;
		}
		sleep(1);
		timeout++;
		if(100 == formatprogress)
		{
			update_sdcardformat_progress(100,100,1);
			printf("format finish.\n");
			break;
		}
		#endif
	}

	//lytSendMsgToUiAppCall(cjson, ltySendMessge2UiCallback);
	//cJSON_Delete(cjson);


	
	return NULL;
}



static int get_format_progress()
{
	int nchn = 0;
	
	pthread_t format_sdcard_tht;
	pthread_create(&format_sdcard_tht, NULL,format_sdcard_ProgressBar, &nchn);

	return 0;
}

void gui_system_storage_manage_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*2:��ȡ����*/
			gui_system_storage_manage_get(2);
			
		}
		else if(2 == objValue->valueint)
		{/*��ʽ��*/
			objValue = cJSON_GetObjectItem(root,"format");
			if(1 == objValue->valueint)
			{/*Ӳ��*/
			
			}
			else if(2 == objValue->valueint)
			{/*SD��*/
				LTY_RESULT ret = LtyMessageSend(LTY_MSG_REC_FORMATHDD, 0, 0, 0);
				if(-1 == ret)
				{
					update_sdcardformat_progress(100,50,-1);
					break;
				}
				DBGBOUT("gui_system_storage_manage_set ret = %d \n", ret);
				get_format_progress();
				//update_sdcardformat_progress(100,50,0);
			}
			else if(3 == objValue->valueint)
			{/*EMMC*/
			
			}
		}
		else if(3 == objValue->valueint)
		{/*�û�ֹͣ��ʽ��*/
			objValue = cJSON_GetObjectItem(root,"format");
			if(1 == objValue->valueint)
			{/*Ӳ��*/
			
			}
			else if(2 == objValue->valueint)
			{/*SD��*/
			
			}
			else if(3 == objValue->valueint)
			{/*EMMC*/
			
			}
		}
		
		
	}while(0);

}
void gui_system_storage_manage_get(int _type)
{
	do
	{
	   cJSON *root = NULL;
	   LTY_RESULT ret;
	   LTY_U32 storage_type = 0;
	   LTY_S32  nEnable = 1;
	   LTY_U32 u32sdcapa=0;
	   LTY_CHAR aszcapa[10] = {0};
	   int status = 1;
	   
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	
	   #if 1
		if(1 == _type)
			storage_type = 0;
		else if(2 == _type)
			storage_type = 1;
		#endif

		cJSON_AddStringToObject(root, "msg_type", "gui_system_storage_manage");

		ret = LtyMessageSend(LTY_MSG_GET_CAPA, storage_type, (LTY_U32)&u32sdcapa, sizeof(LTY_U32));
	

		
		if (LTY_SUCCESS != ret){
			LOGOUT("gui_system_storage_manage_get failed ret=%d .\n", ret);
			nEnable = 0;
			status = 2;
		}
		

		char surplus_storage[32] = {0};
		
		
		
		memset(aszcapa, 0, sizeof(aszcapa));
		sprintf(aszcapa, "%dG", u32sdcapa/1024);
		
        cJSON_AddNumberToObject(root, "type",_type);/*1:Ӳ�� 2:SD�� 3:EMMC*/
		cJSON_AddNumberToObject(root, "enable",nEnable);/*0:δѡ��״̬ 1:ѡ��״̬ 2:����״̬*/
		cJSON_AddStringToObject(root, "surplus_storage",surplus_storage); /*ʣ��������20G*/
        cJSON_AddStringToObject(root, "total_storage",aszcapa);/*�ܵ�������32G*/
		cJSON_AddNumberToObject(root, "status",status);/*1:���� 2:������*/


		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}


void gui_system_videotape_query_set(cJSON *root)
{
		cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��ȡ��������*/
			
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����Ϣ*/
			objValue = cJSON_GetObjectItem(root,"position");
			if(objValue)
			{/*λ��  0:Ӳ�� 1:EMMC 2:SD��*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"year");
			if(objValue)
			{/*��*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"month");
			if(objValue)
			{/*��*/
				//objValue->valueint;
			}
		}		
		else if(3 == objValue->valueint)
		{/*��ȡ�б���Ϣ*/
			objValue = cJSON_GetObjectItem(root,"position");
			if(objValue)
			{/*λ��  0:Ӳ�� 1:EMMC 2:SD��*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"year");
			if(objValue)
			{/*��*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"month");
			if(objValue)
			{/*��*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"day");
			if(objValue)
			{/*��*/
				//objValue->valueint;
			}
		}
		else if(4 == objValue->valueint)
		{
		    objValue = cJSON_GetObjectItem(root,"operate_type");
            if(NULL == objValue)
            {
            	break;
            }
			if(1 == objValue->valueint)
			{/*//��һ��ͨ��*/
			}
			else if(2 == objValue->valueint)
			{/*��һ����Ƶ*/
			}
			else if(3 == objValue->valueint)
			{/*����*/
			}
			else if(4 == objValue->valueint)
			{/*����*/
			}
			else if(5 == objValue->valueint)
			{/*ֹͣ*/
			}
			else if(6 == objValue->valueint)
			{/*ǰ��*/
			}
			else if(7 == objValue->valueint)
			{/*��һ����Ƶ*/
			}
			else if(8 == objValue->valueint)
			{/*��һ��ͨ��*/
			}
			else if(9 == objValue->valueint)
			{/*ˢ�²����б�*/
				cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
	            int array_size = 0;
	            cJSON *tasklist = NULL;
	            if(NULL != pJson_array)
	            {
	                array_size = cJSON_GetArraySize(pJson_array);/*�����С*/
	                tasklist=pJson_array->child;/*�Ӷ���*/
	            }

	            for(int index = 0;index < array_size;index++)  
	            {
	                
	                objValue = cJSON_GetObjectItem(tasklist,"value");
	                if(objValue)
	                {
	                    //objValue->valuestring; /*�ļ�·��*/
	                }
	                
	                tasklist = tasklist->next;
	                if(NULL == tasklist)
	                {
	                    break;
	                }
	            }
			}
			else if(10 == objValue->valueint)
			{/*������*/
			}
			else if(11 == objValue->valueint)
			{/*����*/
			}
			
		}
		else if(5 == objValue->valueint)
		{/*�����ļ���Ƶ�ļ�*/

			
		    objValue = cJSON_GetObjectItem(root,"operate_type");
            if(NULL == objValue)
            {
            	break;
            }
			if(1 == objValue->valueint)
			{/*��������*/

				objValue = cJSON_GetObjectItem(root,"export_type");
	            if(objValue)
	            {
	            	//objValue->valueint;/*1:ԭʼ����  2:AVI����*/
	            }
			
				cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
	            int array_size = 0;
	            cJSON *tasklist = NULL;
	            if(NULL != pJson_array)
	            {
	                array_size = cJSON_GetArraySize(pJson_array);/*�����С*/
	                tasklist=pJson_array->child;/*�Ӷ���*/
	            }

	            for(int index = 0;index < array_size;index++)  
	            {
	                
	                objValue = cJSON_GetObjectItem(tasklist,"value");
	                if(objValue)
	                {
	                    //objValue->valuestring; /*�ļ�·��*/
	                }
	                
	                tasklist = tasklist->next;
	                if(NULL == tasklist)
	                {
	                    break;
	                }
	            }
			}
			else if(2 == objValue->valueint)
			{/*ȡ������*/
				
			}
			
		}
	}while(0);
	

}

void gui_system_videotape_query_get()
{
		
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	




		cJSON_AddStringToObject(root, "msg_type", "gui_system_videotape_query");
		int type = 1;
		switch(type)
		{
			case 1:/*��ȡ��������*/
			{
				int array_size = 30;
		        cJSON *pJson_rows = cJSON_CreateArray();
				cJSON_AddNumberToObject(root, "type",1);
		        cJSON_AddItemToObject(root, "rows",pJson_rows);

		        cJSON *pJson_row = NULL;
				int year = 2016;
				
		        for(int index = 0;index < array_size;index++)  /*���б�*/
		        {
		            pJson_row = cJSON_CreateObject();
		            cJSON_AddItemToArray(pJson_rows,pJson_row);
		            cJSON_AddNumberToObject(pJson_row, "value",year); /*��*/
		        }
			}break;
			case 2:/*��ȡ����Ϣ*/
			{
			    int position = 0;
				int year = 0;
				int month = 0;
				cJSON_AddNumberToObject(root, "type",2);
				cJSON_AddNumberToObject(root, "position",position);/*λ��  0:Ӳ�� 1:EMMC 2:SD��*/
				cJSON_AddNumberToObject(root, "year",year);/*��*/
				cJSON_AddNumberToObject(root, "month",month);/*��*/

				int array_size = 30;
		        cJSON *pJson_rows = cJSON_CreateArray();

		        cJSON_AddItemToObject(root, "rows",pJson_rows);

		        cJSON *pJson_row = NULL;
				int day = 1;
				int status = 1;
				
		        for(int index = 0;index < array_size;index++)  /*���б�*/
		        {
		            pJson_row = cJSON_CreateObject();
		            cJSON_AddItemToArray(pJson_rows,pJson_row);
		            cJSON_AddNumberToObject(pJson_row, "day",day); /*1~31��*/
					cJSON_AddNumberToObject(pJson_row, "status",status);/*1:����¼��2:����¼�� 3:��������¼��ͱ���¼��*/
		        }
				
				
			}break;
			case 3:/*��ȡ�б���Ϣ*/
			{
				int number = 0;
				char date[20];
				int time = 0;
				int passageway_value = 0;
				int size = 0;
				int position = 0;
				char path[20];
				
				cJSON_AddNumberToObject(root, "type",3);
				cJSON_AddNumberToObject(root, "number",number);/*��š���1��ʼ*/
				cJSON_AddStringToObject(root, "date",date);/*¼��ʱ��:2016-11-04 13:23:23*/
				cJSON_AddNumberToObject(root, "time",time);/*ʱ��(s)*/
				cJSON_AddNumberToObject(root, "position",position);/*λ��  0:Ӳ�� 1:EMMC 2:SD��*/
				/* ͨ��λ�� 0:ȫ�� 1:CAM1-1 2:CAM1-2 3:CAM2-1 4:CAM2-2 5:CAM3-1 6:CAM3-2 7:CAM4-1 8:CAM4-2
				   9:PON1 10:PON2 11:PON3 12:PON4 13:PON5 14:PON6 15:PON7 16:PON8 17:PON9 18:PON10*/
				cJSON_AddNumberToObject(root, "passageway_value",passageway_value);
				cJSON_AddNumberToObject(root, "size",size); /*��Ƶ�ļ���С*/
				cJSON_AddStringToObject(root, "path",path);/*��Ƶ�ļ�·��*/

				
				
			}break;
			case 4:/*�ط�*/
			{
				int output = 0;
				cJSON_AddNumberToObject(root, "type",4);
				cJSON_AddNumberToObject(root, "output",output);/* 1:����Ƶ����� 2:û����Ƶ�����*/
			}break;
			case 5:/*����*/
			{
				int value = 0;
				char result[20];
				cJSON_AddNumberToObject(root, "type",5);
				cJSON_AddNumberToObject(root, "value",value);/*1:���ڵ��� 2:�����ɹ� 3:����ʧ��*/
				cJSON_AddStringToObject(root, "result",result);/*����ʧ��ԭ��*/
			}break;
		}
			
		

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}


void gui_system_coordinate_collection_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*1:����*/
			objValue = cJSON_GetObjectItem(root,"current_line");
			if(objValue)
			{/*��ǰ��·*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"current_site");
			if(objValue)
			{/*��ǰվ��*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"current_speed_limiting_section");
			if(objValue)
			{/*��ǰ���ٶ�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"current_inflection_point");
			if(objValue)
			{/*��ǰ�յ�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"current_station");
			if(objValue)
			{/*��ǰ��վ*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"current_field_coordinate");
			if(objValue)
			{/*��ǰ������*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"direction");
			if(objValue)
			{/*//0:���� 1:����*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"longitude");
			if(objValue)
			{/*����*/
				//objValue->valuestring;
			}
			objValue = cJSON_GetObjectItem(root,"latitude");
			if(objValue)
			{/*γ��*/
				//objValue->valuestring;
			}
			
			objValue = cJSON_GetObjectItem(root,"azimuth");
			if(objValue)
			{/*��λ��*/
				//objValue->valueint;
			}

			objValue = cJSON_GetObjectItem(root,"speed");
			if(objValue)
			{/*ʱ��*/
				//objValue->valueint;
			}

			
		}
		else if(2 == objValue->valueint)
		{/*2:��ȡ����*/
		
		}
		
		
	}while(0);

}

void gui_system_coordinate_collection_get()
{
	
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	

		int current_line = 0;
		int current_site = 0;
		int current_speed_limiting_section = 0;
		int current_inflection_point = 0;
		int current_station = 0;
		int current_field_coordinate = 0;
		int direction= 0;
		char longitude[20] = {0};
		char latitude[20] = {0};
		int azimuth = 0;
		int speed = 0;


		cJSON_AddStringToObject(root, "msg_type", "gui_system_coordinate_collection");

		int array_size = 30;
        cJSON *pJson_rows1 = cJSON_CreateArray();
		cJSON *pJson_rows2 = cJSON_CreateArray();
		cJSON *pJson_rows3 = cJSON_CreateArray();
		cJSON *pJson_rows4 = cJSON_CreateArray();
		cJSON *pJson_rows5 = cJSON_CreateArray();
		cJSON *pJson_rows6 = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "rows1",pJson_rows1);
		cJSON_AddItemToObject(root, "rows2",pJson_rows2);
		cJSON_AddItemToObject(root, "rows3",pJson_rows3);
		cJSON_AddItemToObject(root, "rows4",pJson_rows4);
		cJSON_AddItemToObject(root, "rows5",pJson_rows5);
		cJSON_AddItemToObject(root, "rows6",pJson_rows6);
        cJSON *pJson_row = NULL;
		char line_list[20];
		
        for(int index = 0;index < array_size;index++)  /*��·�б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows1,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",line_list); /*��·����*/
        }
		for(int index = 0;index < array_size;index++)  /*վ���б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows2,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",line_list); /*վ������*/
        }
		for(int index = 0;index < array_size;index++)  /*���ٶ��б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows3,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",line_list); /*���ٶ�����*/
        }
		for(int index = 0;index < array_size;index++)  /*�յ��б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows4,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",line_list); /*�յ�����*/
        }
		for(int index = 0;index < array_size;index++)  /*��վ�б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows5,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",line_list); /*��վ����*/
        }
		for(int index = 0;index < array_size;index++)  /*�������б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows6,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",line_list); /*����������*/
        }

        cJSON_AddNumberToObject(root, "current_line",current_line);//��ǰ��·
        cJSON_AddNumberToObject(root, "current_site",current_site);//��ǰվ��
        cJSON_AddNumberToObject(root, "current_speed_limiting_section",current_speed_limiting_section);//��ǰ���ٶ�
        cJSON_AddNumberToObject(root, "current_inflection_point",current_inflection_point);//��ǰ�յ�
        cJSON_AddNumberToObject(root, "current_station",current_station);//��ǰ��վ
        cJSON_AddNumberToObject(root, "current_field_coordinate",current_field_coordinate);//��ǰ������
        cJSON_AddNumberToObject(root, "direction",direction);  //0:���� 1:����
        cJSON_AddStringToObject(root, "longitude",longitude);//����
        cJSON_AddStringToObject(root, "latitude",latitude);//γ��
        cJSON_AddNumberToObject(root, "azimuth", azimuth);//��λ��
        cJSON_AddNumberToObject(root, "speed",speed);//ʱ��


		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);
}



void gui_station_and_line_set(cJSON *root)
{
   printf("gui_station_and_line_set\n");

   cJSON *objValue = NULL;
   objValue = cJSON_GetObjectItem(root,"type");
	
	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*1:����*/
		}
		else if(2 == objValue->valueint)
		{/*2:��ȡ����*/
		    LtyMessageSend(LTY_MSG_STATION_LINE_GET, 0, 0, 0);	
		}
	 }while(0);

}


void gui_starting_screen_flag_set(cJSON *root)
{
   printf("gui_starting_screen_set:11111111111111111111111111111111111 % \n");
   LTY_RESULT ret;
 
   cJSON *objValue = NULL;
   objValue = cJSON_GetObjectItem(root,"type");
	
	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*1:����*/
			starting_screen_flag_set(root);		
		}
		else if(2 == objValue->valueint)
		{/*2:��ȡ����*/
			gui_starting_screen_flag_get();
		}
	 }while(0);
  
}

void starting_screen_flag_set(cJSON *root)
{
     printf("starting_screen_flag_set>>>>>>>>>>>>>>>>> \n");
     LTY_U32 screen_flag = 0;
	 cJSON *objValue = NULL;
	 objValue = cJSON_GetObjectItem(root,"screen_flag");
	 if (NULL != objValue)
	 {
	    LTY_RESULT ret = LtyMessageSend(LTY_MSG_FACTORY_INFO_SET, -1, objValue->valueint, sizeof(objValue->valueint));
		DBGBOUT("ret = %d \n", ret);

	 }

	 	 
}

void gui_Alg_Image_get(LTY_U32 _oprType      )
{   
    printf("starting_screen_flag_set>>>>>>>%d\n", _oprType);
    cJSON *root = NULL;
	root = cJSON_CreateObject();
	if(root)
	{
		cJSON_AddStringToObject(root, "msg_type", "gui_alg_image_update");

		cJSON_AddNumberToObject(root, "type", _oprType);		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}


}


void gui_starting_screen_flag_get()
{   
    printf("gui_starting_screen_flag_get>>>>>>>>>>>>>>> \n");
    LTY_U32 screen_flag = 0;
    LtyMessageSend(LTY_MSG_FACTORY_INFO_GET, 0, (LTY_U32)&screen_flag,0);
	printf("gui_starting_screen_flag_get:%d\n", screen_flag);
   
    cJSON *root = NULL;
	root = cJSON_CreateObject();

	
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "gui_starting_screen_flag");

	    cJSON_AddNumberToObject(root, "screen_flag", screen_flag);
		
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
    }
    

}


void starting_info_common_set(cJSON *root)
{  
   printf("starting_info_common_set: %d \n");
   LTY_RESULT ret;
   cJSON *objValue = NULL;
	
   do
   {
	  	start_info_set_s startInfo;
	    memset(&startInfo, 0, sizeof(start_info_set_s));
		LTY_U32 str_len;
		objValue = cJSON_GetObjectItem(root,"cityID");	
		if(NULL != objValue &&  objValue->valuestring)
	    {
	      printf("starting_info_common_set:cityID %s \n",objValue->valuestring);
		  startInfo.strCityNo = objValue->valuestring;
	    }
		objValue = cJSON_GetObjectItem(root,"compontID");
	    if(NULL != objValue &&  objValue->valuestring)
	    {
	      printf("starting_info_common_set:compontID %s \n",objValue->valuestring);
	      startInfo.strComponyNo = objValue->valuestring;
	    }
		objValue = cJSON_GetObjectItem(root,"carID");
	    if(NULL != objValue &&  objValue->valuestring)
	    {
	      printf("starting_info_common_set:carID %s \n",objValue->valuestring);
	      startInfo.strBusNo = objValue->valuestring;
	    }
	    objValue = cJSON_GetObjectItem(root,"lineID");
	    if(NULL != objValue &&   objValue->valuestring)
	    {
	      printf("starting_info_common_set:lineID %s \n",objValue->valuestring);
	      startInfo.strLineNo = objValue->valuestring;
	    }	
			
		LTY_RESULT ret = LtyMessageSend(LTY_MSG_START_INFO_SET, 0xFF, (LTY_U32)&startInfo, sizeof(startInfo));
		DBGBOUT("ret = %d \n", ret);

   }while(0);

}



void register_info_common_set(cJSON *root)
{

	LTY_RESULT ret;
	cJSON *objValue = NULL;
	
	do
	{
		bus_plate_set_t stBusPlate;
			
		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_BUS_PLATE, (LTY_U32)&stBusPlate, sizeof(stBusPlate));
        if(LTY_SUCCESS != ret)
        {
            DBGPOUT("Get SD Palate Para Fail !!!!\n");
            break;
        }

		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DEV_INF, (LTY_U32)&m_stCfg_DevInfo, sizeof(m_stCfg_DevInfo));
		printf("m_stCfg_DevInfo ret:%d \n", ret);
	    if(LTY_SUCCESS != ret)
	    {
	        DBGPOUT("get m_stCfg_DevInfo config error, %s\n", LtyErrStr(ret));
	    }
				
		objValue = cJSON_GetObjectItem(root,"line");
		if(objValue)
		{/*��·*/
			LTY_U32 u32LineNo = 0;

			u32LineNo = objValue->valueint;
			ret = LtyMessageSend(LTY_MSG_ITSSD_SET_PARA, SD_PARA_LINENO, (LTY_U32)&u32LineNo, 4);
			printf("gui_system_register_info_set() SD_PARA_LINENO  ret:%d \n", ret);
		}
		objValue = cJSON_GetObjectItem(root,"line_attribute");
		if(objValue)
		{/*��·*/
			m_stCfg_DevInfo.u8LineAttr = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"car_number");
		if(objValue)
		{/*�������*/
			//m_stCfg_DevInfo.u32VehicleSN = atoi(objValue->valuestring);
			memset(&stBusPlate.charBusNoBuf,0,sizeof(stBusPlate.charBusNoBuf));
			memcpy(stBusPlate.charBusNoBuf, objValue->valuestring, 15);
			printf("gui_system_register_info_set() %s\n", stBusPlate.charBusNoBuf);
		}
		
		objValue = cJSON_GetObjectItem(root,"manufacturer_number");
		if(objValue)
		{/*���̱��*/
			//objValue->valuestring;
		}
		
		objValue = cJSON_GetObjectItem(root,"select_rovince");
		if(objValue)
		{/*�û�ѡ���ʡ��*/
			m_stCfg_DevInfo.u8Rovince = objValue->valueint;
		}
		objValue = cJSON_GetObjectItem(root,"license_plate");
		if(objValue)
		{/*���ƺ���*/
			memset(&stBusPlate.charBusLicenseBuf,0,sizeof(stBusPlate.charBusLicenseBuf));
			memcpy(stBusPlate.charBusLicenseBuf, objValue->valuestring, 15);
		}
#if 0
		objValue = cJSON_GetObjectItem(root,"device_id");
		if(objValue)
		{/*�豸ID��*/
			//objValue->valuestring;
		}
#endif
    	ret = LtyMessageSend(LTY_MSG_ITSSD_SET_PARA, SD_PARA_BUS_PLATE, (LTY_U32)&stBusPlate, sizeof(stBusPlate));

		if(LTY_SUCCESS != ret)
        {
            DBGPOUT("LTY_MSG_ITSSD_SET_PARA SD Palate Para Fail !!!!\n");
            break;
        }
		
		#if 1
		ret = LtyMessageSend(LTY_MSG_CFG_SET_COMMON_SAVE, CFG_PARA_DEV_INF, (LTY_U32)&m_stCfg_DevInfo, sizeof(m_stCfg_DevInfo));
		printf("m_stCfg_DevInfo ret:%d \n", ret);
	    if(LTY_SUCCESS != ret)
	    {
	        DBGPOUT("get m_stCfg_DevInfo config error, %s\n", LtyErrStr(ret));
	    }
		#endif
	
	}while(0);
}

void register_info_common_get(cJSON *root)
{
	do
	{
		
		char device_id[20] = {0};
		
		
		LTY_RESULT ret;
		LTY_U32 u32LineNo = 0;

		all_line_name_t stAllLineName;
		bus_plate_set_t stBusPlate;
		
		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_LINE_NAME, (LTY_U32)&stAllLineName, sizeof(stAllLineName));
		if(LTY_SUCCESS != ret)
		{
			DBGPOUT("gui_system_register_info_get() SD_PARA_LINE_NAME error, %s\n", LtyErrStr(ret));
		}
		
		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_LINENO, (LTY_U32)&u32LineNo, 4);
		
		DBGPOUT("gui_system_register_info_get() u8LineNo:%d\n",u32LineNo);
		if(LTY_SUCCESS != ret)
		{
			DBGPOUT("gui_system_register_info_get() SD_PARA_LINENO error, %s\n", LtyErrStr(ret));
		}

		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DEV_INF, (LTY_U32)&m_stCfg_DevInfo, sizeof(m_stCfg_DevInfo));
		printf("m_stCfg_DevInfo ret:%d \n", ret);
	    if(LTY_SUCCESS != ret)
	    {
	        DBGPOUT("get m_stCfg_DevInfo config error, %s\n", LtyErrStr(ret));
	    }

		memset(&stBusPlate,0,sizeof(stBusPlate));
		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_BUS_PLATE, (LTY_U32)&stBusPlate, sizeof(stBusPlate));
        if(LTY_SUCCESS != ret)
        {
            DBGPOUT("Get SD Palate Para Fail !!!!\n");
        }

		
		int array_size = stAllLineName.u32LineNum;
        cJSON *pJson_rows = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "rows",pJson_rows);
        cJSON *pJson_row = NULL;
        for(int index = 0;index < array_size;index++)  /*��·�б�*/
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows,pJson_row);
            cJSON_AddStringToObject(pJson_row, "value",stAllLineName.tLineName[index].charLineName); /*��·����*/
        }

	
        cJSON_AddNumberToObject(root, "line",u32LineNo);  							/*�û�ѡ�����·*/
		cJSON_AddNumberToObject(root, "line_attribute",m_stCfg_DevInfo.u8LineAttr);
		cJSON_AddStringToObject(root, "car_number", stBusPlate.charBusNoBuf);					/*�������*/
        cJSON_AddStringToObject(root, "manufacturer_number", "L");	/*���̱��*/
        cJSON_AddStringToObject(root, "license_plate", stBusPlate.charBusLicenseBuf);				/*���ƺ���*/
        cJSON_AddStringToObject(root, "device_id", device_id);						/*�豸ID��*/

		cJSON_AddNumberToObject(root, "select_rovince", m_stCfg_DevInfo.u8Rovince); /*�û�ѡ���ʡ��*/
		
	
	}while(0);
}

void gui_system_register_info_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	
	do
	{
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*1:����*/
			register_info_common_set(root);		
		}
		else if(2 == objValue->valueint)
		{/*2:��ȡ����*/
			gui_system_register_info_get();
		}
		
		
	}while(0);
	
}

void gui_system_starting_info_set(cJSON *root)
{  
    printf("gui_system_starting_info_set \n");
  
   cJSON *objValue = NULL;
   objValue = cJSON_GetObjectItem(root,"type");
	
	do
	{
	    printf("gui_system_starting_info_set: %d \n",objValue->valueint);
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{
			starting_info_common_set(root);		
		}
		else if(2 == objValue->valueint)
		{
			// gui_system_register_info_get();
		}
		
		
	}while(0);
	

}


void gui_system_register_info_get()
{
	

	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	

	    cJSON_AddStringToObject(root, "msg_type", "gui_system_register_info");
	    
		register_info_common_get(root);

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}


void gui_message_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	do
	{
		if(NULL == objValue)
		{
			break;
		}
		//1:Ԥ���¼� 2:��Ϣ 3:�г��ƻ�
		if(1 == objValue->valueint)
		{/*1:��ȡԤ���¼�*/
		
		}
		else if(2 == objValue->valueint)
		{/*2:��ȡ��Ϣ*/
		
		}
		else if(3 == objValue->valueint)
		{/*3:��ȡ�г��ƻ�*/
			
		}
		
	}while(0);
}

void gui_message_get()
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	
		int type = 1;
	    cJSON_AddStringToObject(root, "msg_type", "gui_driver_menu");
	    cJSON_AddNumberToObject(root, "type", type);//1:Ԥ���¼� 2:��Ϣ 3:�г��ƻ�

		switch(type)
		{
			case 1:
			{
				char message_event[20];
				int code = 1;
				cJSON_AddStringToObject(root, "message", message_event);
				cJSON_AddNumberToObject(root, "code", code);
			}break;
			case 2:
			{
				char message[20];
				cJSON_AddStringToObject(root, "message", message);
			}break;
			case 3:
			{
				char time[20];
				int status = 1;
				cJSON_AddStringToObject(root, "time", time);
				cJSON_AddNumberToObject(root, "status", status);/*1:�ȴ�ִ�� 2:ִ�����*/
				
			}break;
		}
		
        
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}


void gui_driver_menu_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	do
	{
		if(NULL == objValue)
		{
			break;
		}
		if(1 == objValue->valueint)/*����*/
		{
			objValue = cJSON_GetObjectItem(root,"travel_direction");
			if(objValue)
			{/*��ʻ����*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"deputy_page_show_mode");
			if(objValue)
			{/*����ҳ��ʾģʽ*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"out_door_video_change");
			if(objValue)
			{/*�¿�����Ƶ�л���ʽ*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"out_door_video_passageway");
			if(objValue)
			{/*�¿�����Ƶͨ��*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"back_car_video_passageway");
			if(objValue)
			{/*������Ƶͨ��*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"start_plan_passageway");
			if(objValue)
			{/*�����ƻ�����*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"over_speed_warning_passageway");
			if(objValue)
			{/*���پ�ʾ����*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"message_outtime");
			if(objValue)
			{/*��Ϣ��ʱ�˳�*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"video_default_frame");
			if(objValue)
			{/*��ƵĬ�ϻ���*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"video_round_robin");
			if(objValue)
			{/*��Ƶ��ѭ*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"round_robin_passageway");
			if(objValue)
			{/*��ѭͨ��*/
				//objValue->valueint;
			}

			cJSON *pJson_array=cJSON_GetObjectItem(root,"rows");/*ȡ����*/
			
	        int array_size=cJSON_GetArraySize(pJson_array);/*�����С*/
	        cJSON *tasklist=pJson_array->child;/*�Ӷ���*/
	        
	        for(int index = 0;index < array_size;index++) /*30������*/
	        {
	            objValue = cJSON_GetObjectItem(tasklist,"value");
	            if(objValue)
	            {
	                //objValue->valueint;
	            }
	            tasklist = tasklist->next;
	            if(NULL == tasklist)
	            {
	                break;
	            }
	        }
					
		}
		else if(2 == objValue->valueint)/*��ȡ*/
		{
		
		}
		else if(3 == objValue->valueint)/*��ȡĬ��*/
		{
		
		}


	}while(0);
	

}
void gui_driver_menu_get()
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	

	    cJSON_AddStringToObject(root, "msg_type", "gui_driver_menu");
	    cJSON_AddNumberToObject(root, "type", 2);/*2:��ȡ����*/
		int travel_direction = 0;
		int deputy_page_show_mode = 0;
		int out_door_video_change = 0;
		int out_door_video_passageway = 0;
		int back_car_video_passageway = 0;
		int start_plan_passageway = 0;
		int over_speed_warning_passageway = 0;
		int message_outtime = 0;
		int video_default_frame = 0;
		int video_round_robin = 0;
		int round_robin_passageway = 0;
		
        cJSON_AddNumberToObject(root, "travel_direction", travel_direction);/*��ʻ����*/
        cJSON_AddNumberToObject(root, "deputy_page_show_mode", deputy_page_show_mode);/*����ҳ��ʾģʽ*/
        cJSON_AddNumberToObject(root, "out_door_video_change", out_door_video_change);/*�¿�����Ƶ�л���ʽ*/
        cJSON_AddNumberToObject(root, "out_door_video_passageway", out_door_video_passageway);/*�¿�����Ƶͨ��*/
        cJSON_AddNumberToObject(root, "back_car_video_passageway", back_car_video_passageway);/*������Ƶͨ��*/
        cJSON_AddNumberToObject(root, "start_plan_passageway", start_plan_passageway);/*�����ƻ�����*/
        cJSON_AddNumberToObject(root, "over_speed_warning_passageway", over_speed_warning_passageway);/*���پ�ʾ����*/
        cJSON_AddNumberToObject(root, "message_outtime", message_outtime);/*��Ϣ��ʱ�˳�*/
        cJSON_AddNumberToObject(root, "video_default_frame", video_default_frame);/*��ƵĬ�ϻ���*/
        cJSON_AddNumberToObject(root, "video_round_robin", video_round_robin);/*��Ƶ��ѭ*/
        cJSON_AddNumberToObject(root, "round_robin_passageway", round_robin_passageway);/*��ѭͨ��*/

        int array_size = 30;
        cJSON *pJson_rows = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "rows",pJson_rows);
        cJSON *pJson_row = NULL;
        for(int index = 0;index < array_size;index++)
        {
            pJson_row = cJSON_CreateObject();
            cJSON_AddItemToArray(pJson_rows,pJson_row);
            cJSON_AddNumberToObject(pJson_row, "value",index); /*index��ֵ��Ҫ��д*/
        }
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);
}


void gui_run_status_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	if(objValue)
	{
		if(1 == objValue->valueint)/*�ƶ�����*/
		{
			gui_run_status_get(1,NULL);
		}
		else if(2 == objValue->valueint)/*I/O*/
		{
			gui_run_status_get(2,NULL);
		}
		else if(3 == objValue->valueint)/*��λģ��*/
		{
			gui_run_status_get(3,NULL);
		}
		else if(4 == objValue->valueint)/*��������*/
		{
			gui_run_status_get(4,NULL);
		}
		else if(5 == objValue->valueint)/*�豸*/
		{
			gui_run_status_get(5,NULL);
		}
	}

}
void gui_run_status_get(char _type,void *_param)
{
   cJSON *root = NULL;
   root = cJSON_CreateObject();
   if(root )
   {   
		
	   /*1:�ƶ����� 2:I/O  3:��λģ�� 4:��������  5:�豸*/
	   cJSON_AddStringToObject(root, "msg_type", "gui_run_status");
	   cJSON_AddNumberToObject(root, "type", _type);
       switch(_type)
       {
       		case 1:/*1:�ƶ����� */
		    {
				int model_status = 1;
				int sim_status = 1;
				int signal_status = 1;
				char network_type[20] = {0};
				int reset_times = 0;
				
				char center_server1_addr[32] = {0};
			
				char center_server1_type[32] = {0};
				int center_server1_connect_times = 0;
				char center_server1_last_connect[32] = {0};
			
				char center_server2_addr[32] = {0};
				int center_server2_port = 0;
				char center_server2_type[32] = {0};
				int center_server2_connect_times = 0;
				char center_server2_last_connect[32] = {0};

				char video_server_addr[32] = {0};
			
				char video_server_type[32] = {0};
				int video_server_connect_times = 0;
				char video_server_last_connect[32] = {0};
				char use_flow[32] = {0};
				LTY_RESULT ret;

				cfg_para_domain_s_t stNetDomainSet;
                ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_DOMAIN_INFO, (LTY_U32)&stNetDomainSet, sizeof(stNetDomainSet));
				if(LTY_SUCCESS != ret)
			    {
			        DBGPOUT("gui_run_status_get() config error, %s\n", LtyErrStr(ret));
					break;
			    }
				
	
				memset(&g_stCfg_SvrNet, 0, sizeof(g_stCfg_SvrNet));

				
			    ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SVR_NET, (LTY_U32)&g_stCfg_SvrNet, sizeof(g_stCfg_SvrNet));

  
				
				printf("g_stCfg_SvrNet ret:%d \n", ret);
			    if(LTY_SUCCESS != ret)
			    {
			        DBGPOUT("gui_run_status_get() config error, %s\n", LtyErrStr(ret));
					break;
			    }

				
				cJSON_AddNumberToObject(root, "model_status", model_status);/*ģ��״̬��1:���� 2:������*/
				cJSON_AddNumberToObject(root, "sim_status", sim_status);/*SIM����1:���� 2:������*/
				cJSON_AddNumberToObject(root, "signal_status", signal_status);/*�ź����� 1:���ź� 2:1���ź� 3:2���ź� 4:3���ź� 5:4���ź� 6:�޲忨*/
				cJSON_AddStringToObject(root, "network_type", network_type);/*��������*/
				cJSON_AddNumberToObject(root, "reset_times", reset_times);/*��λ����*/
				//���ķ�����1

				if(LTY_SUCCESS != ret)
				{
					DBGPOUT("gui_system_network_get() CFG_PARA_DOMAIN_INFO config error, %s\n", LtyErrStr(ret));
					break;
				}


				memset(center_server1_addr, 0, sizeof(center_server1_addr));
				strcpy(center_server1_addr, SockIpToStr(g_stCfg_SvrNet.u32ItsIP));
				
				//cJSON_AddNumberToObject(root, "center_server1_status", center_server1_status);/*1:δʹ�� 2:�κ��� 3:���ӳɹ�*/
				cJSON_AddStringToObject(root, "center_server1_addr", (char *)stNetDomainSet.u8ItsDomain);/*�滻Ϊ��������ַ*/
				cJSON_AddNumberToObject(root, "center_server1_port", g_stCfg_SvrNet.u16ItsPort/*center_server1_port*/);/*�˿�*/
				cJSON_AddStringToObject(root, "center_server1_type", center_server1_type);/*��������*/
				cJSON_AddNumberToObject(root, "center_server1_connect_times", center_server1_connect_times);/*�������Ӵ���*/
				cJSON_AddStringToObject(root, "center_server1_last_connect", center_server1_last_connect);/*�������ʱ��*/
				//���ķ�����2

				ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_CENTER_SERVER2, (LTY_U32)&g_stCfg_svr_net_ex, sizeof(g_stCfg_svr_net_ex));
				char center_server2_ip[20] = {0};
	            memset(center_server2_ip, 0, sizeof(center_server2_ip));
				strcpy(center_server2_ip, SockIpToStr(g_stCfg_svr_net_ex.u32ItsIP));
				center_server2_port = g_stCfg_svr_net_ex.u16ItsPort;
			    
				
				//cJSON_AddNumberToObject(root, "center_server2_status", center_server2_status);/*1:δʹ�� 2:�κ��� 3:���ӳɹ�*/
				cJSON_AddStringToObject(root, "center_server2_addr", center_server2_ip);/*��������ַ*/
				cJSON_AddNumberToObject(root, "center_server2_port", center_server2_port);/*�˿�*/
				cJSON_AddStringToObject(root, "center_server2_type", center_server2_type);/*��������*/
				cJSON_AddNumberToObject(root, "center_server2_connect_times", center_server2_connect_times);/*�������Ӵ���*/
				cJSON_AddStringToObject(root, "center_server2_last_connect", center_server2_last_connect);/*�������ʱ��*/
				//��Ƶ������1

				memset(video_server_addr, 0, sizeof(video_server_addr));
				strcpy(video_server_addr, SockIpToStr(g_stCfg_SvrNet.u32StrmIP));
				
			    //cJSON_AddNumberToObject(root, "video_server_status", video_server_status);/*1:δʹ�� 2:�κ��� 3:���ӳɹ�*/
				cJSON_AddStringToObject(root, "video_server_addr", (char *)stNetDomainSet.u8StrmDomain);/*��������ַ*/
				cJSON_AddNumberToObject(root, "video_server_port", g_stCfg_SvrNet.u16StrmPort/*video_server_port*/);/*�˿�*/
				cJSON_AddStringToObject(root, "video_server_type", video_server_type);/*��������*/
				cJSON_AddNumberToObject(root, "video_server_connect_times", video_server_connect_times);/*�������Ӵ���*/
				cJSON_AddStringToObject(root, "video_server_last_connect", video_server_last_connect);/*�������ʱ��*/

                SZFLOWCNT aFlow;
                memset(&aFlow,0x00,sizeof(aFlow));
                
				ret = LtyMessageSend(LTY_MSG_SYSCTRL_SET_4G_FLOW, 0, (LTY_U32)&aFlow,0);

                LOGOUT("aFlow.TXTolCnt=%lld,aFlow.TXPreTolCnt=%lld\n",aFlow.TXTolCnt,aFlow.TXPreTolCnt);
                LOGOUT("aFlow.RXTolCnt=%lld,aFlow.RXPreTolCnt=%lld\n",aFlow.RXTolCnt,aFlow.RXPreTolCnt);
                snprintf(use_flow,sizeof(use_flow),"RX:%lldM,TX:%lldM",aFlow.RXTolCnt/1024/1024,aFlow.TXTolCnt/1024/1024);
				cJSON_AddStringToObject(root, "use_flow", use_flow);/*������ʹ������*/
				
			}break;
			case 2:/*2:I/O*/
		    {
				int front_door_status = 2;
				int middle_door_status = 2;
				int back_door_status = 2;
				int back_car_status = 2;
				int stop_car_status = 2;
				int wiper_status = 2;
				int left_lamp_status = 2;
				int right_lamp_status = 2;
				int io1_status = 2;
				int io2_status = 2;
				int police1_status = 2;
				int police2_status = 2;
				
				
				cJSON_AddNumberToObject(root, "front_door_status", front_door_status);//ǰ�� 1:������2:�ر�
				cJSON_AddNumberToObject(root, "middle_door_status", middle_door_status);//����
				cJSON_AddNumberToObject(root, "back_door_status", back_door_status);//����
				cJSON_AddNumberToObject(root, "back_car_status", back_car_status);//����
				cJSON_AddNumberToObject(root, "stop_car_status", stop_car_status);//ɲ��
				cJSON_AddNumberToObject(root, "wiper_status", wiper_status);//���
				cJSON_AddNumberToObject(root, "left_lamp_status", left_lamp_status);//��ת��
				cJSON_AddNumberToObject(root, "right_lamp_status", right_lamp_status);//��ת��
				cJSON_AddNumberToObject(root, "io1_status", io1_status);//I/O���1
				cJSON_AddNumberToObject(root, "io2_status", io2_status);//I/O���2
				cJSON_AddNumberToObject(root, "police1_status", police1_status);//��������1
				cJSON_AddNumberToObject(root, "police2_status", police2_status);//��������2
				
			}break;
			case 3:/*3:��λģ��*/
		    {
				int front_door_status = 1;
				char longitude[20] = {0};
				char latitude[20] = {0};
				int Azimuth = 1;
				int height = 1;
				int real_speed = 1;
				
				cJSON_AddNumberToObject(root, "gps_number", front_door_status);//���ǿ���
				cJSON_AddStringToObject(root, "longitude", longitude);//����
				cJSON_AddStringToObject(root, "latitude", latitude);//γ��
				cJSON_AddNumberToObject(root, "Azimuth", Azimuth);//��λ��
				cJSON_AddNumberToObject(root, "height", height);//���θ߶�
				cJSON_AddNumberToObject(root, "real_speed", real_speed);//ʵʱ�ٶ�

				
			}break;
			case 4:/*4:��������*/
		    {
				int pon1_status = 1;
				char pon1_ip[20];
				int pon2_status = 1;
				char pon2_ip[20];
				int pon3_status = 1;
				char pon3_ip[20];
				int pon4_status = 1;
				char pon4_ip[20];
				int pon5_status = 1;
				char pon5_ip[20];
				int pon6_status = 1;
				char pon6_ip[20];
				int pon7_status = 1;
				char pon7_ip[20];
				int pon8_status = 1;
				char pon8_ip[20];
				int pon9_status = 1;
				char pon9_ip[20];
				int pon10_status = 1;
				char pon10_ip[20];

				if(1 == CURRENT_IS_D2)
				{
					int local_network_status = 0;
					char local_network_ip[32] = {0};
					char local_network_subnet_mask[32] = {0};
					char local_network_gateway[32] = {0};
					char local_network_dns1[32] = {0};
					char local_network_dns2[32] = {0};
					LTY_RESULT ret;
					
					memset(&g_stCfg_sys_net, 0, sizeof(g_stCfg_sys_net));


				    ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYS_NET, (LTY_U32)&g_stCfg_sys_net, sizeof(g_stCfg_sys_net));
					printf("gui_run_status_get() ret:%d \n", ret);
				    if(LTY_SUCCESS != ret)
				    {
				        DBGPOUT("gui_run_status_get() error 1, %s\n", LtyErrStr(ret));
				        
						break;
				    }
					
					memset(local_network_ip, 0, sizeof(local_network_ip));
					strcpy(local_network_ip, SockIpToStr(g_stCfg_sys_net.stEther.u32IPAddr));

					memset(local_network_gateway, 0, sizeof(local_network_gateway));
					strcpy(local_network_gateway, SockIpToStr(g_stCfg_sys_net.stEther.u32GateWay));

					memset(local_network_subnet_mask, 0, sizeof(local_network_subnet_mask));
					strcpy(local_network_subnet_mask, SockIpToStr(g_stCfg_sys_net.stEther.u32NetMask));

					memset(local_network_dns1, 0, sizeof(local_network_dns1));
					strcpy(local_network_dns1, SockIpToStr(g_stCfg_sys_net.stEther.u32DNS1));

					memset(local_network_dns2, 0, sizeof(local_network_dns2));
					strcpy(local_network_dns2, SockIpToStr(g_stCfg_sys_net.stEther.u32DNS2));

					
					cJSON_AddNumberToObject(root, "local_network_status", local_network_status);
					cJSON_AddStringToObject(root, "local_network_ip", local_network_ip);
					cJSON_AddStringToObject(root, "local_network_subnet_mask", local_network_subnet_mask);
					cJSON_AddStringToObject(root, "local_network_gateway", local_network_gateway);
					cJSON_AddStringToObject(root, "local_network_dns1", local_network_dns1);
					cJSON_AddStringToObject(root, "local_network_dns2", local_network_dns2);

				}
				else
				{
					cJSON_AddNumberToObject(root, "pon1_status", pon1_status);//1:������ 2:δ����  3:�����쳣  4:������
					cJSON_AddStringToObject(root, "pon1_ip", pon1_ip);

					cJSON_AddNumberToObject(root, "pon2_status", pon2_status);
					cJSON_AddStringToObject(root, "pon2_ip", pon2_ip);

					cJSON_AddNumberToObject(root, "pon3_status", pon3_status);
					cJSON_AddStringToObject(root, "pon3_ip", pon3_ip);

					cJSON_AddNumberToObject(root, "pon4_status", pon4_status);
					cJSON_AddStringToObject(root, "pon4_ip", pon4_ip);

					cJSON_AddNumberToObject(root, "pon5_status", pon5_status);
					cJSON_AddStringToObject(root, "pon5_ip", pon5_ip);

					cJSON_AddNumberToObject(root, "pon6_status", pon6_status);
					cJSON_AddStringToObject(root, "pon6_ip", pon6_ip);

					cJSON_AddNumberToObject(root, "pon7_status", pon7_status);
					cJSON_AddStringToObject(root, "pon7_ip", pon7_ip);

					cJSON_AddNumberToObject(root, "pon8_status", pon8_status);
					cJSON_AddStringToObject(root, "pon8_ip", pon8_ip);

					cJSON_AddNumberToObject(root, "pon9_status", pon9_status);
					cJSON_AddStringToObject(root, "pon9_ip", pon9_ip);

					cJSON_AddNumberToObject(root, "pon10_status", pon10_status);
					cJSON_AddStringToObject(root, "pon10_ip", pon10_ip);
				}
				
			}break;
			case 5:/*5:�豸*/
		    {
				int device_door = 1;
				int fan_speed = 1;
				
				
				int car_out_temperature = 1;
				int car_in_temperature = 1;
				int serial1_port_status = 1;
				int serial2_port_status = 1;
				int serial3_port_status = 1;
				int serial4_port_status = 1;
				int serial5_port_status = 1;
				int can1_status = 1;
				int can2_status = 1;

				mcu_run_status_t* pststatus = (mcu_run_status_t*)_param;

				
				cJSON_AddNumberToObject(root, "device_door", device_door);//�豸��  1:����  2:�ر�
				cJSON_AddNumberToObject(root, "fan_speed", fan_speed);//����ת��
				if(pststatus)
				{
					cJSON_AddNumberToObject(root, "input_voltage", pststatus->u32InputVoltage);//�����ѹ
					cJSON_AddNumberToObject(root, "device_temperature", pststatus->u32DeviceTemperature);//�豸�¶�
				}
				else
				{
					cJSON_AddNumberToObject(root, "input_voltage", 0);//�����ѹ
					cJSON_AddNumberToObject(root, "device_temperature", 0);//�豸�¶�
				}
				cJSON_AddNumberToObject(root, "car_out_temperature", car_out_temperature);//�����¶�
				cJSON_AddNumberToObject(root, "car_in_temperature", car_in_temperature);//�����¶�
				cJSON_AddNumberToObject(root, "serial1_port_status", serial1_port_status);//����1״̬  1:����  2:δ����
				cJSON_AddNumberToObject(root, "serial2_port_status", serial2_port_status);//����2״̬  1:����  2:δ����
				cJSON_AddNumberToObject(root, "serial3_port_status", serial3_port_status);//����3״̬  1:����  2:δ����
				cJSON_AddNumberToObject(root, "serial4_port_status", serial4_port_status);//����4״̬  1:����  2:δ����
				cJSON_AddNumberToObject(root, "serial5_port_status", serial5_port_status);//����5״̬  1:����  2:δ����
				cJSON_AddNumberToObject(root, "can1_status", can1_status);//CAN1״̬
				cJSON_AddNumberToObject(root, "can2_status", can2_status);//CAN2״̬
				
			}break;
       }

	   
	   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

   }
		
	
}


void gui_about_machine_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	if(objValue)
	{
		if(1 == objValue->valueint)//���ڱ���
		{
			gui_about_machine_get();
		}
		
	}
}
LTY_RESULT  get_file_line(char *pInputName, char *pOutputBuf, int cnt)
{  
    FILE * fp;  
    int i=0;  
    char * line = NULL;  
    size_t len = 0;  
    ssize_t read;  
    fp = fopen(pInputName, "r");  
    if (fp == NULL)  
     return -1;  
  
    if(cnt<=0)  
         return LTY_FAILURE;  
           
    while ((read = getline(&line, &len, fp)) != -1) {  
        ++i;  
        if(i>=cnt)  
            break;  
    }  
  
    if (line)  
    {  
        memcpy(pOutputBuf,line,strlen(line));  
        free(line);  
        return LTY_SUCCESS;   
    }  
  
    return LTY_FAILURE;  
}  

#define EEPROM_NOD "/dev/at24cxx"
LTY_RESULT GetProductSNInFo(LTY_CHAR *szProductSN,LTY_U16 Len)
{
    LTY_CHAR  bufferww[15+1] = {0};
    ssize_t ret = 0;
    ssize_t Len1 = 0;
    
    if(Len < 15)return LTY_FAILURE;
        
    int fd = open(EEPROM_NOD, O_RDONLY);
	if(fd < 0)
    {
	    LOGOUT("open %s failed.\n",EEPROM_NOD);
        return LTY_FAILURE;    
	}
    
    Len1 = sizeof(bufferww)-1;
    
    ret = read(fd,bufferww,Len1);
    if(ret == Len1)
    {
        memcpy(szProductSN,bufferww,Len1);
        LOGOUT("\r\n");
        LOGOUT("read SN %s\n",bufferww);
        LOGOUT("\r\n");
        close(fd);
        return LTY_SUCCESS;
    }
    
    close(fd);
    LOGOUT("read SN failed(%d).\n",ret);
    return LTY_FAILURE;
}

//���Ȱ汾����SN�ŵ��ѷ������豸
LTY_RESULT SetProductSNInFo()
{
    LTY_CHAR  csnbuf[100] = {0};
    LTY_U32   icnt = 0;
    LTY_CHAR  bufferww[20] = {0};
    LTY_CHAR  FileName[128] = {0};
    LTY_CHAR  szProductSN[15] = {0};
    ssize_t   ret = 0,Len = 0;
    
    memcpy(FileName,"/sys/class/sunxi_info/sys_info",strlen("/sys/class/sunxi_info/sys_info"));
    
    if(LTY_SUCCESS == get_file_line(FileName,csnbuf,3)){
        szProductSN[0] = 'L'; 
        for(icnt=0; icnt<10; icnt++)
            szProductSN[icnt + 1] = (LTY_S8)csnbuf[icnt + 20];

        LTY_U16 crc16 = LtyVerifyCrc16(0x4c23, &csnbuf[20], 32);

        LOGOUT("crc16 =0x%x\n",crc16);
        sprintf(bufferww,"%04x",crc16);
        for(icnt =0;icnt<4;icnt++)
            szProductSN[11+icnt]= bufferww[icnt];

        int fd = open(EEPROM_NOD, O_WRONLY);
	    if(fd < 0)
        {
		    LOGOUT("open %s failed.\n",EEPROM_NOD);
            return LTY_FAILURE;    
	    }
        
        Len = 15;
        ret = write(fd,szProductSN,Len);
        if(ret != Len)
        {
            LOGOUT("write %s failed.\n",EEPROM_NOD);
            close(fd);
            return LTY_FAILURE;    
        }
        
        LOGOUT("write SN %s Success.\n",szProductSN);
        close(fd);
        return LTY_SUCCESS;
    }
    
    return LTY_FAILURE;
}

void gui_about_machine_get()
{
	cJSON *root = NULL;
   root = cJSON_CreateObject();

	DBGOUT("gui_about_machine_get start\n");
   
   if(root )
   {   //���ڱ���
		char application_program_version[20] = {0};
		char file_system_version[20] = {0};
		char mcu_program_version[20] = {0};
		char system_resouce[20] = {0};
		char system_plug_in[20] = {0};
		char kernel_version[20] = {0};
		char guide_version[20] = {0};
		
		
		char mac_addr[20] = {0};
		char cam1_1_parameter[20] = {0};
		char cam2_1_parameter[20] = {0};

		LTY_RESULT ret;
	    
	    cfg_para_version_t stVer;
	    mcu_version_info_t stMcuVer;

	    char csnbuf[100]={0}; 
		char bufferww[20];
		int icnt;
	    ret = LtyMessageSend(LTY_MSG_CFG_GET_VERSION_DATA, 0, (LTY_U32)&stVer, sizeof(stVer));
	    if (LTY_SUCCESS != ret)
	    {
	        ERROUT("Failed to read version, %s\n", LtyErrStr(ret));
	        return;
	    }

		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYS_NET, (LTY_U32)&g_stCfg_sys_net, sizeof(g_stCfg_sys_net));

		sprintf(mac_addr, "%02x.%02x.%02x.%02x.%02x.%02x",
					(g_stCfg_sys_net.stEther.u8MAC[0]), (g_stCfg_sys_net.stEther.u8MAC[1]), (g_stCfg_sys_net.stEther.u8MAC[2]),
					(g_stCfg_sys_net.stEther.u8MAC[3] ), (g_stCfg_sys_net.stEther.u8MAC[4]), (g_stCfg_sys_net.stEther.u8MAC[5]));

	    ret = LtyMessageSend(LTY_MSG_ITSIO_GET_MCU_VER, 0, (LTY_U32)&stMcuVer, sizeof(stMcuVer));
	    if(LTY_SUCCESS == ret)
	    {
	        ConvertU32ToVersionString(stMcuVer.u32SoftVersion, mcu_program_version); 
	    }

	    sprintf(application_program_version, "%s", LTY_VERSION);

		

	   ConvertU32ToVersionString(VERSION_MAJOR <<24  |VERSION_MINOR <<16 |  FetchBSPVer(ROOTFSVER), file_system_version);
	   ConvertU32ToVersionString(stVer.u32ResVer, system_resouce);
	   ConvertU32ToVersionString(stVer.u32PluginVer, system_plug_in);
	   ConvertU32ToVersionString(VERSION_MAJOR <<24  |VERSION_MINOR <<16 |FetchBSPVer(KERNELVER), kernel_version);
	   ConvertU32ToVersionString(VERSION_MAJOR <<24  |VERSION_MINOR <<16 |FetchBSPVer(BOOTVER), (LTY_CHAR*)guide_version);
		/* ���к�:  Lxxxxxx (11�ַ�)*/
       GetProductSNInFo(stVer.szProductSN,sizeof(stVer.szProductSN));
        
	   cJSON_AddStringToObject(root, "msg_type", "gui_about_machine");
	   cJSON_AddStringToObject(root, "application_program_version", application_program_version);//Ӧ�ó���汾
	   cJSON_AddStringToObject(root, "file_system_version",file_system_version ); //���ļ�ϵͳ�汾
	   cJSON_AddStringToObject(root, "mcu_program_version", mcu_program_version); //MCU����汾
	   cJSON_AddStringToObject(root, "system_resouce", system_resouce);//ϵͳ��Դ
	   cJSON_AddStringToObject(root, "system_plug_in",system_plug_in );//ϵͳ���
	   cJSON_AddStringToObject(root, "kernel_version",kernel_version );//�ں˰汾
	   cJSON_AddStringToObject(root, "guide_version", guide_version);//�����汾
	   cJSON_AddStringToObject(root, "keyboard_program_version", "V1.0.0"/*keyboard_program_version*/);//���̳���汾
	   
	   cJSON_AddStringToObject(root, "device_serial_number",stVer.szProductSN/*device_serial_number*/ );//�豸���к�
	   cJSON_AddStringToObject(root, "mac_addr", mac_addr);//MAC��ַ
	   cJSON_AddStringToObject(root, "cam1_1_parameter",cam1_1_parameter);//CAM1-1����
	   cJSON_AddStringToObject(root, "cam2_3_parameter",cam2_1_parameter);//CAM2-3����
#if CURRENT_IS_P1
		char price_ver[20] = {0};
		char line_ver[20] = {0};
	   cJSON_AddStringToObject(root, "price_ver",price_ver);
	   cJSON_AddStringToObject(root, "line_ver",line_ver);
#endif
	   
	   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

   }

   DBGOUT("gui_about_machine_get end\n");

}

void gui_videotape_video_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	if(objValue)
	{/* 1:1���桢  2:4���桢 3:9���桢 4:16���� 5:�Զ��廭��*/

		if(1 == objValue->valueint)
		{/*1:1����*/
			objValue = cJSON_GetObjectItem(root,"number");/*ͨ����*/
			if(objValue)
			{
				//objValue->valueint;
			}
		}
		else if(2 == objValue->valueint)
		{/*2:4����*/
			objValue = cJSON_GetObjectItem(root,"number1");/*ͨ����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"number2");/*ͨ����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"number3");/*ͨ����*/
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"number4");/*ͨ����*/
			if(objValue)
			{
				//objValue->valueint;
			}
		
		}
		else if(3 == objValue->valueint)
		{/*3:9����*/
		}
		else if(4 == objValue->valueint)
		{/*16����*/
		}
		else if(5 == objValue->valueint)
		{/*5:�Զ��廭��*/
			objValue = cJSON_GetObjectItem(root,"number");
			if(objValue)
			{
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"start_x");
			if(objValue)
			{/*��ʼX����*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"start_y");
			if(objValue)
			{/*��ʼY����*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"width");
			if(objValue)
			{/*��ʾ���*/
				//objValue->valueint;
			}
			objValue = cJSON_GetObjectItem(root,"height");
			if(objValue)
			{/*��ʾ�߶�*/
				//objValue->valueint;
			}
			
		}
		
	}
	
}

void gui_videotape_video_get()
{
	cJSON *root = NULL;
   root = cJSON_CreateObject();
   if(root )
   {   
   	   int number = 0;
	   int show = 1; 
	   int type = 0;
	   
	   cJSON_AddStringToObject(root, "msg_type", "gui_videotape_video");
       cJSON_AddNumberToObject(root, "type", type);/* 1:1���桢  2:4���桢 3:9���桢 4:16���� 5:�Զ��廭��*/
	   cJSON_AddNumberToObject(root, "number", number);	/*ͨ����*/
	   cJSON_AddNumberToObject(root, "show", show); /*1:��ʾ 2:����ʾ*/
	   
	   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

   }
}


void gui_service_term_set(cJSON *root)
{
    cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	if(objValue)
	{
		if(1 == objValue->valueint) /*���ò���ֵ*/
		{
			objValue = cJSON_GetObjectItem(root,"value");
			if(objValue)
			{/*ֵ��Χ1~9*/
				//objValue->valueint;
			}
		}
		else if(2 == objValue->valueint) /*���ò�������*/
		{/* 1:*/
			objValue = cJSON_GetObjectItem(root,"value");
			if(objValue)
			{/*1:�������� 2:�ں��� 3:�⺰�� */
				//objValue->valueint;
			}
		}
	
	}
}




void gui_light_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	if(objValue)
	{
		if(1 == objValue->valueint) /*��������*/
		{
			objValue = cJSON_GetObjectItem(root,"value");
			if(objValue)
			{/*0:�Զ���1:�ǳ�����2:һ�� 3:�� 4:�ǳ���*/
				//objValue->valueint;
			}
		}
		else if(2 == objValue->valueint) /*��ȡ����*/
		{
			
		}
	
	}
}
void gui_light_get()
{
   cJSON *root = NULL;
   root = cJSON_CreateObject();
   if(root )
   {   
   	   int volume_light = 0;

	   cJSON_AddStringToObject(root, "msg_type", "gui_light");
	   cJSON_AddNumberToObject(root, "value", volume_light);	/*0:�Զ���1:�ǳ�����2:һ�� 3:�� 4:�ǳ���*/
	   
	   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

   }
}

void gui_voice_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");
	if(objValue)
	{
		if(1 == objValue->valueint || 2 == objValue->valueint) 
		{ /*1:��������*/  /*2:��������*/

			volume_set_t stVoice;
			LTY_RESULT ret = LTY_FAILURE;

		    ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_VOLUME, (LTY_U32)&stVoice, sizeof(stVoice));
		    if(LTY_SUCCESS != ret)
		    {
		    	DBGPOUT("Get Voice Para Fail !!!\n");
		    	return;
		    }
		
			objValue = cJSON_GetObjectItem(root,"volume_within");/*������*/  //0~32
			if(objValue)
			{
				int current_value = 0;
				int voice_value = 0;
				
				current_value = objValue->valueint;
				if(0 == current_value)
					voice_value = 0;
				else if(32 == current_value)
					voice_value = 15;
				else
				{
					voice_value = (current_value * 15)/32;
				}
				stVoice.u8InDoorVolume = voice_value;
	
			}
			objValue = cJSON_GetObjectItem(root,"volume_out");/*������*/ //0~32
			if(objValue)
			{
				int current_value = 0;
				int voice_value = 0;
				
				current_value = objValue->valueint;
				if(0 == current_value)
					voice_value = 0;
				else if(32 == current_value)
					voice_value = 15;
				else
				{
					voice_value = (current_value * 15)/32;
				}
				stVoice.u8OutDoorVolume = voice_value;
			}
			objValue = cJSON_GetObjectItem(root,"volume_TTS");/*TTS��*/ /*˾������*/ //0~32
			if(objValue)
			{
				int current_value = 0;
				int voice_value = 0;
				
				current_value = objValue->valueint;
				if(0 == current_value)
					voice_value = 0;
				else if(32 == current_value)
					voice_value = 15;
				else
				{
					voice_value = (current_value * 15)/32;
				}
				stVoice.u8TTSVolume = voice_value;
			}
			
			ret = LtyMessageSend(LTY_MSG_ITSSD_SET_PARA, SD_PARA_VOLUME, (LTY_U32)&stVoice, sizeof(stVoice));
			if(LTY_SUCCESS != ret)
	        {
	            DBGOUT("Set Volum Seting Fail ,%s ...\n",LtyErrStr(ret));
	        }
			
			objValue = cJSON_GetObjectItem(root,"volume_play");/*��������*/
			if(objValue)
			{
				/*1:������ 2:������ 3:TTS��*/
				if(1 == objValue->valueint)
				{
					LTY_CHAR pStr[64];
			        memset(pStr, 0, sizeof(pStr));

					sprintf(pStr, "%s","�������ܵ���ϵͳ�������ԣ������������ȷ���");

			        LtyMessagePost(LTY_MSG_ITSAUD_SET_TTS_STREAM, SPEAKER_CHN_INSIDE, (LTY_U32)&pStr, strlen(pStr) + 1);
				}
				else if(2 == objValue->valueint)
				{
					LTY_CHAR pStr[64];
        			memset(pStr, 0, sizeof(pStr));

					sprintf(pStr, "%s","�������ܵ���ϵͳ�������ԣ������������ȷ���");
        			LtyMessagePost(LTY_MSG_ITSAUD_SET_TTS_STREAM, SPEAKER_CHN_OUTSIDE, (LTY_U32)&pStr, strlen(pStr) + 1);
				}
				else if(3 == objValue->valueint)
				{
					LTY_CHAR pStr[64];
			        memset(pStr, 0, sizeof(pStr));
	
					sprintf(pStr, "%s","�������ܵ���ϵͳ�������ԣ�������˾���������ȷ���");

			        LtyMessagePost(LTY_MSG_ITSAUD_SET_TTS_STREAM, SPEAKER_CHN_KEYPAD, (LTY_U32)&pStr, strlen(pStr) + 1);
				}
				gui_voice_get();
				
			}
			
		}
		else if(3 == objValue->valueint) /*��ȡ����*/
		{
			gui_voice_get();
		}
		
		
	}
}

void gui_voice_get()
{
   cJSON *root = NULL;
   root = cJSON_CreateObject();
   if(root )
   {   
   	   int volume_within = 0;
	   int volume_out = 0;
	   int volume_TTS = 0;
   
   		volume_set_t stVoice;
		LTY_RESULT ret = LTY_FAILURE;	

		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_VOLUME, (LTY_U32)&stVoice, sizeof(stVoice));
	    if(LTY_SUCCESS != ret)
	    {
	    	DBGPOUT("Get Voice Para Fail !!!\n");
	    }

	   volume_within = (stVoice.u8InDoorVolume * 32)/15;
	   volume_out = (stVoice.u8OutDoorVolume * 32)/15;
	   volume_TTS = (stVoice.u8TTSVolume * 32)/15;
	   
	   cJSON_AddStringToObject(root, "msg_type", "gui_voice");
	   cJSON_AddNumberToObject(root, "volume_within", volume_within);	/*������*/
	   cJSON_AddNumberToObject(root, "volume_out", volume_out);		/*������*/
	   cJSON_AddNumberToObject(root, "volume_TTS", volume_TTS);		/*TTS��*/ /*˾������*/

	   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

   }
}

void gui_main_page_set(cJSON *root)
{
	//gui_main_page_get();
	
}
void gui_main_page_get(PTQrPrice ptQrPrice)
{
   cJSON *root = NULL;
   root = cJSON_CreateObject();
   if(root )
   {   
   	   
	   cJSON_AddStringToObject(root, "msg_type", "gui_main_page");

	   //int price_number = 3;
	   //int price_type = 1; /*1:һƱ�� 2:�ֶ��շ�*/
	   float fprice = (float)(ptQrPrice->ticketprice) / 100;
	   printf("----debug-----fprice is %f\n",fprice);
	   cJSON_AddNumberToObject(root, "price_number", fprice);
	   //cJSON_AddNumberToObject(root, "price_type", ptQrPrice->tickettype);
	   
	   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

   }
}

void gui_main_page_get_ex(char _type)
{
   printf("gui_main_page_get_excard_param.CHARGMODE:%d\n",_type);
   cJSON *root = NULL;
   root = cJSON_CreateObject();
   if(root )
   {   
   	   
	   cJSON_AddStringToObject(root, "msg_type", "gui_main_page");
	   cJSON_AddNumberToObject(root, "price_type", _type);
	   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

   }
}


void gui_qr_code_pay_info()
{

	cJSON *root = NULL;
	root = cJSON_CreateObject();
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "get_qr_pay_info");


		LtyMessageSend(LTY_MSG_GET_CARD_MONEY, 0, 0, 0);
		
		cfg_para_pay_info_t stPayInfo;
#if 0
		flash_card_result_t carc_result;
        memset(&carc_result, 0, sizeof(carc_result));
		// memcpy(carc_result.money,"1",sizeof(carc_result.money));
		// memcpy(carc_result.money_total,"8",sizeof(carc_result.money_total));
		gui_ic_card_pay_info(&carc_result);
#endif
		int ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_PAY_INFO, (LTY_U32)&stPayInfo, sizeof(cfg_para_pay_info_t));
		if (LTY_SUCCESS != ret)
		{
			printf("gui_qr_code_pay_info() error ret:%d\n",ret);
		}

		cJSON_AddStringToObject(root, "driver_name", stPayInfo.driver_name);/*˾������*/
		cJSON_AddStringToObject(root, "driver_id", stPayInfo.driver_id);/*˾������*/
        LTY_CHAR money[32];
        memset(money,0x00,sizeof(money));
        snprintf(money,sizeof(money),"%.01f",stPayInfo.total_money / 10.0);
        LOGOUT("total_money =%s\n",money);
        cJSON_AddStringToObject(root, "total_money", money);/*���˽��*/
        cJSON_AddNumberToObject(root, "one_total_money", stPayInfo.one_total_money);/*������*/
	    _DBG_("money[%d] one money[%d]\n",
                stPayInfo.total_money, stPayInfo.one_total_money);	
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}


}

void gui_ic_card_pay_info(void *_param)
{

	cJSON *root = NULL;
	root = cJSON_CreateObject();

	flash_card_result_t *card_result = (flash_card_result_t *)_param;
	
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "get_qr_pay_info");
		
		//cfg_para_pay_info_t stPayInfo;


		cJSON_AddStringToObject(root, "ic_card_tang", card_result->money);	/*IC�����˽��*/
		cJSON_AddStringToObject(root, "ic_card_ban", card_result->money_total);/*IC��������*/
		cJSON_AddStringToObject(root, "money_qr", card_result->money_qr);	/*��ά�뵱�˽��*/
		cJSON_AddStringToObject(root, "money_total_qr", card_result->money_total_qr);/*��ά�뵱����*/
		cJSON_AddNumberToObject(root, "ic_number", card_result->ic_number);
		cJSON_AddNumberToObject(root, "qr_number", card_result->qr_number);

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}


}



void gui_flahs_card_info(void *_param)
{

	cJSON *root = NULL;
	
	flash_card_result_t *card_result = (flash_card_result_t *)_param;
	root = cJSON_CreateObject();
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "get_flash_card_info");

		cJSON_AddNumberToObject(root, "type", card_result->u8Type);
		cJSON_AddStringToObject(root, "money", card_result->money);/*��ǰ���ѽ��*/
		cJSON_AddStringToObject(root, "money_total", card_result->money_total);/*���*/
		cJSON_AddStringToObject(root, "fail_result", card_result->fail_result);/*ˢ��ʧ��ԭ��*/
		cJSON_AddNumberToObject(root, "show_time", card_result->show_time);
		cJSON_AddNumberToObject(root, "message_tip_type", card_result->message_tip_type);
		cJSON_AddNumberToObject(root, "card_type", card_result->card_type);
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}


}



void gui_normal_set_info_set(cJSON *root)
{
	cJSON *objValue = NULL;
	objValue = cJSON_GetObjectItem(root,"type");

	
	do
	{
		
		if(NULL == objValue)
		{
			break;
		}
		
		if(1 == objValue->valueint)
		{/*��������*/
			/*1:ע����Ϣ-----------------------------------------------------*/
			register_info_common_set(root);
			/*2:I/O����-----------------------------------------------------*/
			switch_config_common_set(root);
			/*3:ϵͳʱ��-----------------------------------------------------*/
			system_setup_common_set(root);
			
			/*4:�û�����-----------------------------------------------------*/
			
			/*5:�߼�-----------------------------------------------------*/
	

			
		}
		else if(2 == objValue->valueint)
		{/*��ȡ����*/
			gui_normal_set_info_get();
		}
		
		
		
	}while(0);

}

void gui_normal_set_info_get()
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	


		cJSON_AddStringToObject(root, "msg_type", "gui_normal_set_info");

		
		/*1.ע����Ϣ*/
		register_info_common_get(root);
		//2.I/O����
		switch_config_common_get(root);
		//3.ϵͳʱ��
		system_setup_common_get(root);
		//4.�û�����

		//5.�߼�
	
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);
}

void gui_run_status_info_get(char _type,void *_param)
{
	int ret;
	if(_param == NULL)
	{
		DBGBOUT("_param == null-- \n");
		return;
	}
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	
	   cJSON_AddStringToObject(root, "msg_type", "gui_run_status_info");
		switch (_type)
		{
			case 0:
			{
				RS485STATE *rs485State;
				rs485State = (RS485STATE *)_param;
				//cJSON_AddNumberToObject(root, "nfc_status", nfc_status);/*NFC״̬  1:������2:������*/
				if(rs485State->chnl == 0/*1 == pInperipheralStat->can1_status*/)
				{
					if (rs485State->state == 1)
					{
						cJSON_AddNumberToObject(root, "can1_status", rs485State->state/*pInperipheralStat->can1_status*/);/*1:���� 2:δ����*/ /********/
						cJSON_AddStringToObject(root, "can1_number", rs485State->cameraID);
					}
					else
					{
						cJSON_AddNumberToObject(root, "can1_status", rs485State->state/*pInperipheralStat->can1_status*/);/*1:���� 2:δ����*/ /********/
						cJSON_AddStringToObject(root, "can1_number", "");
					}
				}

				if(rs485State->chnl == 1/*1 == pInperipheralStat->can1_status*/)
				{
					if (rs485State->state == 1)
					{
						cJSON_AddNumberToObject(root, "can2_status", rs485State->state/*pInperipheralStat->can1_status*/);/*1:���� 2:δ����*/ /********/
						cJSON_AddStringToObject(root, "can2_number", rs485State->cameraID);
					}
					else
					{
						cJSON_AddNumberToObject(root, "can2_status", rs485State->state/*pInperipheralStat->can1_status*/);/*1:���� 2:δ����*/ /********/
						cJSON_AddStringToObject(root, "can2_number", "");
					}
				}			
				//cJSON_AddNumberToObject(root, "qr_code", qr_code);/*��ά���ͷ	1:������2:������*/
			
				//cJSON_AddNumberToObject(root, "rs485_status", rs485_status);/*RS485����״̬ 1:���ӡ�2:δ����*/
				//cJSON_AddNumberToObject(root, "can_status", can_status);/*CAN����״̬	1:���ӡ�2:δ����*/

			}break;
			case 1:
			{
						//���ķ�����״̬-------------------------
				int center_server_status = 1;/*���ķ�����״̬ 1:���ڰκ� 2:�κųɹ�  3:�κ�ʧ��*/
				char last_connect_time[20] = {0};/*�������ʱ��*/
				
				LTY_RESULT ret;
				
				memset(&g_stCfg_SvrNet, 0, sizeof(g_stCfg_SvrNet));
				
				
				ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SVR_NET, (LTY_U32)&g_stCfg_SvrNet, sizeof(g_stCfg_SvrNet));
				printf("g_stCfg_SvrNet ret:%d \n", ret);
				if(LTY_SUCCESS != ret)
				{
					DBGPOUT("gui_system_network_get() config error, %s\n", LtyErrStr(ret));
					break;
				}
				char center_server1_addr[32] = {0};
				int center_server1_connect_times = 0;
				
				memset(center_server1_addr, 0, sizeof(center_server1_addr));
				strcpy(center_server1_addr, SockIpToStr(g_stCfg_SvrNet.u32ItsIP));
				
				cJSON_AddNumberToObject(root, "center_server_status", center_server_status);/*���ķ�����״̬ 1:���ڰκ� 2:�κųɹ�	3:�κ�ʧ��*/
				cJSON_AddStringToObject(root, "server_addr", center_server1_addr);/*��������ַ*/
				cJSON_AddNumberToObject(root, "server_port", g_stCfg_SvrNet.u16ItsPort);/*�˿�*/
				cJSON_AddNumberToObject(root, "current_connect_times", center_server1_connect_times);/*�������Ӵ���*/
				cJSON_AddStringToObject(root, "last_connect_time", last_connect_time);/*�������ʱ��*/
				

			}break;
			case 3:
			{

				//��������-------------------------
				int local_network_status = 2;/*�������� 1:������  2:δ���� 3:��������*/
				char mac_addr[20] = {0};/*MAC��ַ*/
				char local_network_ip[32] = {0};
				char local_network_subnet_mask[32] = {0};
				char local_network_gateway[32] = {0};
				char local_network_dns1[32] = {0};
				char local_network_dns2[32] = {0};
				
				memset(&g_stCfg_sys_net, 0, sizeof(g_stCfg_sys_net));
				
				ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYS_NET, (LTY_U32)&g_stCfg_sys_net, sizeof(g_stCfg_sys_net));
				printf("gui_run_status_get() ret:%d \n", ret);
				if(LTY_SUCCESS != ret)
				{
					DBGPOUT("gui_run_status_get() error 1, %s\n", LtyErrStr(ret));
					
					break;
				}
				
				memset(local_network_ip, 0, sizeof(local_network_ip));
				strcpy(local_network_ip, SockIpToStr(g_stCfg_sys_net.stEther.u32IPAddr));
				
				memset(local_network_gateway, 0, sizeof(local_network_gateway));
				strcpy(local_network_gateway, SockIpToStr(g_stCfg_sys_net.stEther.u32GateWay));
				
				memset(local_network_subnet_mask, 0, sizeof(local_network_subnet_mask));
				strcpy(local_network_subnet_mask, SockIpToStr(g_stCfg_sys_net.stEther.u32NetMask));
				
				memset(local_network_dns1, 0, sizeof(local_network_dns1));
				strcpy(local_network_dns1, SockIpToStr(g_stCfg_sys_net.stEther.u32DNS1));
				
				memset(local_network_dns2, 0, sizeof(local_network_dns2));
				strcpy(local_network_dns2, SockIpToStr(g_stCfg_sys_net.stEther.u32DNS2));
				
				cJSON_AddNumberToObject(root, "local_network_status", local_network_status);/*�������� 1:������  2:δ���� 3:��������*/
				cJSON_AddStringToObject(root, "ip_addr", local_network_ip);/*IP��ַ*/
				cJSON_AddStringToObject(root, "subnet_mask", local_network_subnet_mask);/*��������*/
				cJSON_AddStringToObject(root, "gateway", local_network_gateway);/*����*/
				cJSON_AddStringToObject(root, "dns1", local_network_dns1);/*DNS1*/
				cJSON_AddStringToObject(root, "dns2", local_network_dns2);/*DNS2*/
				cJSON_AddStringToObject(root, "mac_addr", mac_addr);/*MAC��ַ*/

			}
				break;
			case 4:
			{
				
				//��Ƶ������״̬-------------------------
				int video_server_status = 1;/*��Ƶ������״̬ 1:���ڰκ� 2:�κųɹ�	3:�κ�ʧ��*/
				//char video_addr[20] = {0};/*��������ַ*/
				//int video_port = 80;/*�˿�*/
				int video_current_connect_times = 0;/*�������Ӵ���*/
				char video_last_connect_time[20] = {0};/*�������ʱ��*/
				char video_server_addr[32] = {0};
				
				memset(video_server_addr, 0, sizeof(video_server_addr));
				strcpy(video_server_addr, SockIpToStr(g_stCfg_SvrNet.u32StrmIP));
				
				
				cJSON_AddNumberToObject(root, "video_server_status", video_server_status);/*��Ƶ������״̬ 1:���ڰκ� 2:�κųɹ�  3:�κ�ʧ��*/
				cJSON_AddStringToObject(root, "video_addr", video_server_addr);/*��������ַ*/
				cJSON_AddNumberToObject(root, "video_port", g_stCfg_SvrNet.u16StrmPort);/*�˿�*/
				cJSON_AddNumberToObject(root, "video_current_connect_times", video_current_connect_times);/*�������Ӵ���*/
				cJSON_AddStringToObject(root, "video_last_connect_time", video_last_connect_time);/*�������ʱ��*/

			}break;
			default:
				break;
		}
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);
}

void gui_login_info_get()
{
	do
	{
	   cJSON *root = NULL;
	   root = cJSON_CreateObject();
	   if(NULL == root)
	   {   
	   	  break;
	   }	
		//char login_name[20] = "test";
		//char login_password[20] = "123456";

		cJSON_AddStringToObject(root, "msg_type", "gui_login_info");

		
		sd_data_t* psdinfo;
		LTY_RESULT ret;
		cfg_para_sys_psw_t adminpasswd;
		char use_sd_data = 1;
		
		bus_plate_set_t stBusPlate;
		
		memset(&stBusPlate,0,sizeof(stBusPlate));

		ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_BUS_PLATE, (LTY_U32)&stBusPlate, sizeof(stBusPlate));
        if(LTY_SUCCESS != ret)
        {
            DBGPOUT("Get SD Palate Para Fail !!!!\n");
            break;
        }


		ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_SYS_PSW, (LTY_U32)&adminpasswd,
					   sizeof(cfg_para_sys_psw_t));
		if (LTY_SUCCESS != ret){
			ERROUT("Can't get passwd from config\n");
			break;
		}
		if (0 == adminpasswd.u8UseDynamicPass){
			ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_ALL, (LTY_U32)&psdinfo,
						   4);
			if (LTY_SUCCESS != ret){ 
				ERROUT("Can't get passwd from config\n");
				use_sd_data = 0;
				
			}

			if(1 == use_sd_data)
			{
				cJSON_AddStringToObject(root, "sd_format_password", psdinfo->tBusPara.tEquBase.charProjectPwd);
				cJSON_AddStringToObject(root, "sd_admin_password", psdinfo->tBusPara.tEquBase.charBaoZhanQiPwd);
			}

			
		}
#if 0
		else{
			time_t now;
			char aszdynamicpwd[8];
			struct tm tm;
			now = time(0);
			localtime_r(&now, &tm);
			sprintf(aszdynamicpwd, "%d%d%d%d%d%d",(tm.tm_mon + 1)%10, 
			(tm.tm_mon + 1)/10, tm.tm_mday%10, tm.tm_mday/10, tm.tm_hour%10, 
			tm.tm_hour/10);
			//if (0 == strcmp(asztepwd, aszdynamicpwd))
		}
#endif
		cJSON_AddNumberToObject(root, "use_dynamic", adminpasswd.u8UseDynamicPass);/*�Ƿ�ʹ�ö�̬����0 ��ʹ�� 1 ʹ��*/
		cJSON_AddNumberToObject(root, "use_sd_data", use_sd_data);	  /*1:ʹ��SD�����ݣ�0:ʹ�ô洢��������*/
		
		cJSON_AddStringToObject(root, "admin_password", adminpasswd.szAdminPwd);

	
	    cJSON_AddStringToObject(root, "login_name", stBusPlate.charBusNoBuf);/*�û���*/


	    cJSON_AddStringToObject(root, "login_password", adminpasswd.szAdminPwd);/*����*/
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		
	}while(0);

}


void gui_passenger_flow_info(void *_param, itsio_passenger_flow_s passenger)
{  
    g_u16PassengerSum = passenger.u16PassengerSum;
    g_u16PassengerUp += passenger.u16PassengerUp;
    g_u16PassengerDown += passenger.u16PassengerDown;
    cJSON *root = NULL;
    root = cJSON_CreateObject();
    if(root)
    {
        cJSON_AddStringToObject(root, "msg_type", "passenger_flow_info");
        cJSON_AddNumberToObject(root, "riu_id", 1);
        cJSON_AddNumberToObject(root, "passenger_sum", g_u16PassengerSum);
        cJSON_AddNumberToObject(root, "passenger_up", g_u16PassengerUp);
        cJSON_AddNumberToObject(root, "passenger_down", g_u16PassengerDown);

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
    }
}


void gui_system_tbp_connect_status(char _status,char _type)
{
	
	cJSON *root = NULL;
	root = cJSON_CreateObject();

	LOGOUT(">>>>>>_status=%d<<<<<<<<\n",_status);
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "gui_system_tbp_connect_status");

		cJSON_AddNumberToObject(root, "type", _type);
		if(1 == _status)
			cJSON_AddNumberToObject(root, "status", 2);
		else
			cJSON_AddNumberToObject(root, "status", 3);
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}


}

void gui_g3_network_get(char _status,char _type)
{    
    cJSON *root = NULL;
	root = cJSON_CreateObject();

	
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "gui_g3_network_status");

		cJSON_AddNumberToObject(root, "type", _type);

	    cJSON_AddNumberToObject(root, "status", _status);
		
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}


}


void gui_tmp_network_get(LTY_U32 _status, LTY_U32 _type, LTY_U32 errorTtpe)
{   
    cJSON *root = NULL;
	root = cJSON_CreateObject();
	printf("aaaaaaaaaaaa _status =%d\n",_status);
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "gui_tmp_connect_status");

		cJSON_AddNumberToObject(root, "type", _type);

	    cJSON_AddNumberToObject(root, "status", _status);
		cJSON_AddNumberToObject(root, "errorType", errorTtpe);
		
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}

}



void gui_system_heartbeat_notify()
{
	
	cJSON *root = NULL;
	root = cJSON_CreateObject();

	
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "gui_system_heartbeat_notify");

		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}


}


typedef enum __qrcode_state__
{
	QRCODE_IDENTIFY_SUCCESS=1,
	QRCODE_DEST_STA_NOT_EXIST,
	QRCODE_INVALID,
	QRCODE_EXPIRE,
	QRCODE_HAD_CHECKED,
}qrcode_state_e;

typedef struct QRPrice_s
{
	qrcode_state_e    eQRcodeSta;
	int  			  price;
}QRPrice_t;

void gui_two_dimensional_code_info(void  *_param)
{
	cJSON *root = NULL;
	QRPrice_t* qr_iden = (QRPrice_t*)_param;
	root = cJSON_CreateObject();
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "two_dimensional_code_info");
		
		float money = (float)(qr_iden->price)/100;
		
		cJSON_AddNumberToObject(root, "identify_flag",qr_iden->eQRcodeSta);/*״̬*/
		cJSON_AddNumberToObject(root, "money", money);
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}
}

void gui_system_key_up(int keycode)
{
	cJSON *root = NULL;
	root = cJSON_CreateObject();
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "system_key_up");
		
		//int keycode = 0;
		
		cJSON_AddNumberToObject(root, "key_code", keycode);
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}
}


void CheckMouse(void)
{
	if((access("/dev/input/mouse0",F_OK))!=-1)  
	{  
        cJSON *root = NULL;
		root = cJSON_CreateObject();
		if(root )
		{
			cJSON_AddStringToObject(root, "msg_type", "mouse_status");
			cJSON_AddNumberToObject(root, "riu_id", 1);
			cJSON_AddNumberToObject(root, "status", 11);

			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			//cJSON_Delete(root);
			DBGPOUT("Have Mouse In!\n");
		}
    }  
	else
	{
		cJSON *root = NULL;
		root = cJSON_CreateObject();
		if(root )
		{
			cJSON_AddStringToObject(root, "msg_type", "mouse_status");
			cJSON_AddNumberToObject(root, "riu_id", 1);
			cJSON_AddNumberToObject(root, "status", 10);

			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			//cJSON_Delete(root);
			DBGPOUT("NO Have Mouse In ....!\n");
		}
   }
	return ;
}


void gui_system_mouse_status(LTY_S32 status)
{
	cJSON *root = NULL;
	root = cJSON_CreateObject();
	if(root )
	{
		cJSON_AddStringToObject(root, "msg_type", "mouse_status");
		
		DBGPOUT("gui_system_mouse_status() status:%d\n",status);
		
		cJSON_AddNumberToObject(root, "status", status);
		
		ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	}
}

void gui_test_heartbeat()
{
	static LTY_U32  msg_sn = 0;
	unsigned int max_int = 0-1;

	if(msg_sn < max_int)
    	msg_sn++;
	else
		msg_sn = 0;

	struct timeval tv;
    char buf[64];

    gettimeofday(&tv, NULL);
    strftime(buf, sizeof(buf)-1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
   // fprintf(stderr, "%s.%03d: CCardWorker::SCOnRecv start\n", buf, (int)(tv.tv_usec / 1000));

	
	// LOGOUT("thread1:ev [%s:%d] index=%d\n",buf, (int)(tv.tv_usec / 1000),msg_sn);

	cJSON *root = NULL;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "msg_type", "gui_test_heartbeat");

	ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
	
}

void ConvertU32ToVersionString(LTY_U32 verNo, LTY_CHAR *verStr)
{
    if( NULL == verStr )
        return;

    DBGPOUT("ver 0x%x",verNo);
    
    sprintf(verStr, "%d.%d.%d", (verNo & 0xff000000)>>24, (verNo & 0x00ff0000)>>16, (verNo & 0x0000ffff));

    return;
}

/*
 *  @funcname:		FetchBSPVer
 *  @Description:	Get BSP Version
 *  @Input:				0:boot 1:kernel 2:rootfs
 *  @Output:			None
 *  @Return:			Version in integer
 */
int FetchBSPVer(int type){
	FILE *fp = NULL;
	char *buf;
	char *ptr;
	int ret = -1 ;
	//int ver =-1;

	
	/*Allocate Memory*/
	buf = (char *)calloc(sizeof(char) , 16);
	if ( NULL == buf ){
		ret = -1;
		goto OUT;
	}
	
	switch ( type ){
		case BOOTVER:
			fp=fopen(BOOT_VER_FILE , "r");
			break;
		case KERNELVER:
		fp=fopen(KERNEL_VER_FILE , "r");
			break;
		case ROOTFSVER:
			/*Open Rootfs Version File*/
			fp=fopen(ROOTFS_VER_FILE , "r");
			break;
		default:
			break;
	}
	if(NULL == fp ){
		ret = -2;
		goto OUT;
	}

	/*Read a Line in*/
	buf = fgets( buf , 16 , fp ) ;

	/*Find if there is a space, yes:we change it to \n , no: do nothing*/
	ptr = strchr(buf ,' ' );
	if (ptr)
	*ptr = '\0';
	/*Version string like model_svnNO.*/
	ptr = strchr(buf ,'_' );
	if (NULL == ptr){
		ret = -3;
		goto OUT;
	}
	/*Skip '_'*/
	ptr ++;
	ret= strtol(ptr , NULL , 10 );
	if (ret <= 0){
		ret = -4;
		goto OUT;
	}

OUT:
	if (buf){
		free(buf);
		buf=NULL;
	}
	if (fp){
		fclose(fp);
		fp=NULL;
	}
	return ret;
}


static LTY_U32 hcomm_mcu_reive_message(cJSON *cjson, char _type,void *pStream)
{
	char szCommType[128] = {0};
	int  nRet = -1;	
	cJSON *pCJSONFunCode = NULL;
	unsigned int nStrLen = 0;


	//printf("hcomm_mcu_reive_message start\n");
	if(NULL == cjson)
	{
		printf("cjson is null \n");
		return nRet;
	}
	
	pCJSONFunCode = cJSON_GetObjectItem(cjson,"msg_type");   
	if(pCJSONFunCode != NULL  && pCJSONFunCode->valuestring)
	{
		nStrLen = strlen(pCJSONFunCode->valuestring);
		if(nStrLen > 128)nStrLen=128;
		memcpy(szCommType, pCJSONFunCode->valuestring, nStrLen);
	}

	//printf("szCommType: %s \n", szCommType);
	//out = cJSON_PrintUnformatted(cjson);
	//printf("ltySendMessageToUi ok=%s .\n", out);

	//printf("hcomm_mcu_reive_message 1\n");
	nRet = lytSendMsgToUiAppCall(cjson,_type ,ltySendMessge2UiCallback);
	//printf("hcomm_mcu_reive_message 2\n");

    ///printf("hcomm_mcu_reive_message end\n");
	return nRet;
}

int ltySendMessageToUi(cJSON *root,char _type, void *pStream)
{
	int nRet = -1;
	if(NULL == root)
	{
		printf("root is NULL.\n");
		return nRet;
	}
	//printf("send call back.\n");
	
	nRet = hcomm_mcu_reive_message(root,_type, pStream);

	return 0;
}

