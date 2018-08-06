#ifndef LTY_CARD_SVR_H
#define LTY_CARD_SVR_H

#include "LtyTypeDef.h"
#include "LtySocketServer.h"
#include "LtySocketClient.h"
#include "LtyLog.h"
//#include "TextEnc.h"
//#include "../../tts/TTSProto.h"
#include "LtyCommon.h"

#include "LtyMsgItsNW.h"
#include "LtyMsgException.h"
#include "LtyMsgItsStation.h"


#define CARD_SOCKET_FILE "/var/run/CARD_SOCKET"


#define SOCKET_BUF_SIZE 		1024

/*��ͷ*/
#define CARD_CMD_REQ_FLAG		0x02

/*������*/
#define CARD_CMD_REQ_SLAVE  	0x01
#if 0
/*1.��������------------------------------------*/
#define CARD_CMD_REQ_HANDSHAKE  0xb1
#define CARD_CMD_RSP_HANDSHAKE  0xb2

#define CARD_HANDSHAKE_SUCCESS		1
#endif
/*2.��ʾ��������--------------------------------*/
#define CARD_CMD_REQ_SHOW_CONTROL  0xb3
#define CARD_CMD_RSP_SHOW_CONTROL  0xb4







/*2.2 ���ؽ��*/
#define CARD_SHOW_CONTROL_SUCCESS	1  /*��ʾ�ɹ�*/
#define CARD_SHOW_CONTROL_FAIL		0  /*��ʾʧ��*/

#if 0
/*3.ָʾ�ƿ�������--------------------------------*/
#define CARD_CMD_REQ_LED_CONTROL  0xb5
#define CARD_CMD_RSP_LED_CONTROL  0xb6

#define CARD_CMD_REQ_SLAVE_LED_RUN  	0x01  	/*����ָʾ��*/
#define CARD_CMD_REQ_SLAVE_LED_GREEN  	0x02	/*�̵�*/
#define CARD_CMD_REQ_SLAVE_LED_RED  	0x03	/*���*/


/*3.1 ���ƵƵ�״̬*/
#define CARD_LED_STATUS_ON		1
#define CARD_LED_STATUS_OFF		0
#endif

/*3.������������--------------------------------*/
#define CARD_CMD_REQ_VOICE_CONTROL  0xb7
#define CARD_CMD_RSP_VOICE_CONTROL  0xb8

/*3.2 ���ؽ��*/
#define CARD_VOICE_CONTROL_SUCCESS	1  /*���ųɹ�*/
#define CARD_VOICE_CONTROL_FAIL		0  /*����ʧ��*/

/*4.�������뵼����������--------------------------------*/
#define CARD_CMD_REQ_PARAM_CONTROL  0xb9
#define CARD_CMD_RSP_PARAM_CONTROL  0xba

/*4.1 ��������*/
#define CARD_CMD_REQ_SLAVE_PARAM_IMPORT 0x01  /*��������*/
#define CARD_CMD_REQ_SLAVE_PARAM_EXPORT 0x02  /*��������*/
#define CARD_CMD_REQ_SLAVE_PARAM_CLEAN  0x03  /*�������*/
#define CARD_CMD_REQ_SLAVE_RECORD_CLEAN 0x04  /*�����¼*/



#define CARD_PARAM_CONTROL_SUCCESS	1  /*����/�����ɹ�*/
#define CARD_PARAM_CONTROL_FAIL		0  /*����/����ʧ��*/


/*5.��ȡ����������--------------------------------*/
#define CARD_CMD_REQ_GET_MONEY  0xbb
#define CARD_CMD_RSP_GET_MONEY  0xbc

/*6.��ȡվ����Ϣ����--------------------------------*/
#define CARD_CMD_REQ_STATION_INFO  0xbd
#define CARD_CMD_RSP_STATION_INFO  0xbe

/*7.��ȡʱ������-------------------------------- */
#define CARD_CMD_REQ_TIME_ZONE  0xc0
#define CARD_CMD_RSP_TIME_ZONE  0xc1

/*8.��ȡ��������-------------------------------- */
#define CARD_CMD_REQ_PASSENGER_FLOW  0xc2
#define CARD_CMD_RSP_PASSENGER_FLOW  0xc3

/*9.֪ͨ��������-------------------------------- */
#define CARD_CMD_REQ_PARAM_CHANGE  0xc4
#define CARD_CMD_RSP_PARAM_CHANGE  0xc5

/*10.����������-------------------------------- */
#define CARD_CMD_REQ_HEARTBEAT  0xc6
#define CARD_CMD_RSP_HEARTBEAT  0xc7

/*11.TBP����״̬����-------------------------------- */
#define CARD_CMD_REQ_TBP_STATUS  0xc8
#define CARD_CMD_RSP_TBP_STATUS  0xc9

/*12.����վ����---------------------------------------*/
#define CARD_CMD_REQ_STATION_IN_OUT  0xca
#define CARD_CMD_RSP_STATION_IN_OUT  0xcb

/*13.ACC�ػ�֪ͨ����---------------------------------------*/
#define CARD_CMD_REQ_ACC_NOTIFY  	0xcc


typedef struct card_cmd_head_s
{
	LTY_U8 u8Flag;    			/*��ͷ��ʶ*/
	LTY_U8 u8CmdMaster; 		/*������*/
	LTY_U8 u8CmdSlave;			/*�����*/
	LTY_U8 u8DataLen;			/*���ݳ���*/
	//LTY_CHAR *u8DataContent;	/*��������*/
	LTY_U16 u16Parity;          /*CRCУ��*/
#if 0
    LTY_U32 u32Flag;
    LTY_U16 u16Parity;
    LTY_U16 u16CmdType;
    LTY_U32 u32DataLen;
    LTY_U32 u32PrivData1;
    LTY_U32 u32PrivData2;
#endif	
}PACKED(1) card_cmd_head_t;





typedef struct global_info_s
{
	LTY_U32 u32Longitude;             //���ȣ�0.1��
    LTY_U32 u32Latitude;              //γ�ȣ�0.1��
    LTY_U16 u16Angle;                 //��λ�ǣ�0-359��
    LTY_U32 u32GPRSLineNo;
	LTY_CHAR station_status;   //0:��վ 1:��վ
	LTY_U32 direction;
}global_info_t;

typedef struct lty_passenger_head_s
{
    LTY_U32 u32DateTime;/* ����ʱ�� UTC*/
    LTY_U32 u32Lnt;   /*����  ��̩Դ��ʽ�����꣺��һ�ֽ�Ϊ�ȣ��ڶ��ֽ�Ϊ�֣������͵����ֽ�Ϊ�ֵ�С��λ�����е����ֽ�Ϊ��λ�������ֽ�Ϊ��λ������2���ֽ����һ���֡��Ƿֵ�С��λ������ֵ��16�������֡���γ�Ⱦ�ȷ���ֵ�С�����4λ*/
    LTY_U32 u32Lat; /*γ�ȣ� ͬ��*/
    LTY_U8  u8Status; /*�����ڵڶ��ṹ��*/
    LTY_U8  u8StationID;/*��ǰվ�� ��1��ʼ*/
    LTY_U8  u8StationAttr;  //վ���ʶ�� 1�ֽڣ�0(���վ)��ff(�յ�վ)������(��;վ)
    LTY_U32 u32LineId;     //��·ID
	LTY_U32 u32CityStaID;  //����վ�����
}PACKED(1) lty_passenger_head_t;

typedef union
{
    LTY_U8 u8Status;
    struct
    {
        LTY_U8 u1Working   : 1;  //0:δӪ�� 1:Ӫ��
        LTY_U8 u1Dir       : 1;  //0:���� 1:����
        LTY_U8 u1FDoor     : 1;  //0:ǰ�Ź� 1:ǰ�ſ�
        LTY_U8 u1BDoor     : 1;  //0:���Ź� 1:���ſ�
        LTY_U8 u1OffLine   : 1;  //0:������ʻ 1:������ʻ
        LTY_U8 u1Event     : 1;  //����
        LTY_U8 u1OverSpeed : 1;  //0:δ���� 1:������
        LTY_U8 u1Park      : 1;  //0:վ���� 1:վ����
    }PACKED(1) stStatus;
}PACKED(1) itsnw_bus_status1_u;

typedef struct lty_passenger_data_s
{
	lty_passenger_head_t stPassengerHd;
    LTY_U8  u8Up5Sec; /*5��֮���ϵĿ���*/
	LTY_U8  u8Down5Sec; /*5��֮���µĿ���*/
	LTY_U16	u16Up; /*����������*/
}PACKED(1) lty_passenger_data_t;


typedef struct lty_gps_data_s
{
	LTY_U8 LOC_TIME[8];//LTY_U64 LOC_TIME; //	��λʱ��
	LTY_U8 LON[4];//LTY_U32 LON;      //	���ȣ���λΪ1*10-6�ȡ�
	LTY_U8 LAT[4];//LTY_U32 LAT;	  //	γ�ȣ���λΪ1*10-6�ȡ�
	LTY_U8 VEC1[2];//LTY_U16 VEC1;     //	�ٶȣ�ָ���Ƕ�λ�����ն��豸�ϴ����г��ٶ���Ϣ��Ϊ�������λΪǧ��ÿСʱ(km/h)����ȡ����
	LTY_U8 VEC2[2];//LTY_U16 VEC2;     //	��ʻ��¼�ٶȣ�ָ������ʻ��¼�豸�ϴ����г��ٶ���Ϣ����λΪǧ��ÿСʱ(km/h)����ȡ����
	LTY_U8 DISTANCE[4];//LTY_U32 DISTANCE; //	�����������������ָ�����ϴ����г����������λΪ��(m)���ն��豸ÿ�쿪��ʱ�����㣻��ȡ����
	LTY_U8 DIRECTION[2];//LTY_U16 DIRECTION;//	����0-359����λΪ��(��)������Ϊ0��˳ʱ�롣
	LTY_U8 ALTITUDE[2];//LTY_U16 ALTITUDE; //	���θ߶ȣ���λΪ��(m)��
	LTY_U8 DIS_NXT_STOP[2];//LTY_U16 DIS_NXT_STOP;//	������վ���룬��λ�ף�m��
	LTY_U8 TIME_NXT_STOP[4];//LTY_U32 TIME_NXT_STOP;//	������վ��ʱ�䣬��λ�루s��
	LTY_U8 INDOOR_TEMP;  //	�����¶ȣ���λΪ�ȡ�
	LTY_U8 NXT_STOP_NO;  //	��һ��վ����š�
	LTY_U8 GPS_SIGNAL;   //	ǿ����Χ0��32��12����Ϊ�ź�����
	LTY_U8 CAN_NO;       //	CanBus Э����;0��ʶδ����can���ߡ���0�ͱ�ʶCAN���߰汾�š�
	LTY_U8 CAN_LTH[2];//LTY_U16 CAN_LTH;      //	�������� CanBus ���ݿ鳤�ȣ�CanBus Э����Ϊ0 ʱ���ֶ�ҲΪ 0��
	//CAN_DATA		BYTES//	CanBus ��Ϣ����Ϊ 0 ʱ���ֶ�Ϊ�գ�Э���������CanBus ����Э���ĵ�;	
	
}PACKED(1) lty_gps_data_t;



typedef struct lty_station_data_s
{
	LTY_U8  VEH_STATE; //��������״̬
	LTY_U8 STOP_CODE[4]; //���й���վ�����
	LTY_U8 STOP_NO[4];//LTY_U32 STOP_NO;   //��ǰ����վ�����
	LTY_U8  DRIVER_ID[10]; //˾�����
	LTY_U8  VEHICLE_ID[6]; //������� 
	LTY_U8  STOP_FLAG;  //����վ���ʶ��0x00����ս��0xff��ĩվ��0x02������㣻0x03����ȫ�¹ʺڵ㣻
	LTY_U8  STOP_RPT;   //��վ��ʽ��0x01��GPS�Զ���վ��0x00���ֶ���վ��0x02����ʾ���ԣ�
	LTY_U8  IN_OUT_FLAG; //����վ��ʶ��0����վ��1����վ
	LTY_U8  ONDIRECTION_FLAG; //0:���� 1:����
	lty_gps_data_t gps_data;

	
}PACKED(1) lty_station_data_t;



class CCardWorker : private CSocketClient
{
public:
    CCardWorker();
    virtual ~CCardWorker();

    LTY_RESULT Init(struct ev_loop *loop);
    LTY_RESULT Attach(int fd);

    LTY_VOID Release();

    int CardSessionId;

    LTY_RESULT MsgHandle(LTY_MSG_TYPE msg, LTY_U32 chan, LTY_U32 buf, 
          LTY_U32 bufsize, LTY_LPVOID lpPriv);

private:
    lty_buf_t CardBuf[2]; // �������ɵ�Card���ݣ�ͨ��Domain Socket���͵��ͻ���

    LTY_VOID SCOnError(LTY_RESULT ret);
    LTY_U32  SCOnRecv(LTY_U8 *pRcvData, LTY_U32 u32DataLen);
    LTY_VOID SCOnTimer();

	LTY_BOOL check_command_head(card_cmd_head_t *pcmd_head);

	void card_cmd_req_show_control(card_cmd_head_t *cmd_head);
	void card_cmd_req_voice_control(card_cmd_head_t *cmd_head);	
	void card_cmd_rsp_param_control(card_cmd_head_t *cmd_head);
	void card_cmd_rsp_get_money(card_cmd_head_t *cmd_head);
	void card_cmd_rsp_station_info(card_cmd_head_t *cmd_head);
	void card_cmd_req_time_zone(card_cmd_head_t *cmd_head);
	void card_cmd_req_passenger_flow();
	void card_cmd_rsp_passenger_flow(card_cmd_head_t *cmd_head);
	void card_cmd_rsp_param_change(card_cmd_head_t *cmd_head);
	void card_cmd_req_heartbeat(card_cmd_head_t *cmd_head);
	void card_cmd_req_tbp_status(card_cmd_head_t *cmd_head);
    void card_cmd_req_sation_in_out();
	void card_cmd_rsp_sation_in_out(card_cmd_head_t *cmd_head);
	void card_cmd_acc_notify(card_cmd_head_t *cmd_head);
	
	LTY_VOID OnMsgPassenger(itsio_passenger_flow_t *pstPassengerFlow);
	LTY_VOID OnMsgGpsInf(itsio_gps_info_t *pstGpsInf);
	LTY_VOID OnMsgStationInOff(BusStation_S *pstStation);
	LTY_VOID OnDirChange(Direction_S* pstdirect);
	LTY_VOID OnMsgSDPrepare(sd_data_t *pstSdData);
	LTY_VOID OnMsgSDParaChange(enum SD_PARA_TYPE enParaType);
	LTY_VOID OnMsgWorkingStatus(its_working_status_t *pstWorking);
	LTY_VOID OnMsgIOStatus(itsio_io_status_t *pstIOStatus);
	LTY_VOID OnMsgDoorExcp(LTY_U32 u32Type, exception_door_status_t *pstDoorStatus);
	LTY_VOID OnMsgOffLine(OffLine_S *pstOffLine);
	LTY_VOID OnMsgOverSpeed(exception_overspeed_t *pstOverSpeed);
	LTY_VOID OnMsgParkInOff(ParkAttr_S *pstPark);
	LTY_VOID OnMsgParkStatus(ParkStatus_S *pstParkStatus);
	
	void card_cmd_send_rsp(card_cmd_head_t *cmd_head);
	void request_import_export(char _type);
	void request_get_money();

	char recv_buf[1024];
	char send_buf[1024];

	lty_passenger_data_t m_passenger_data; 
	LTY_U8 m_u8NextStationID;       	//��վվ����ţ�Ĭ��0
	LTY_U8 m_current_station_id;
	LTY_U8  m_u8StInOutFlug;					//0վ�ڣ�1վ��
	itsnw_bus_status1_u m_unStatus;

	lty_station_data_t m_station_data;
	lty_gps_data_t m_current_gps_data;
	LTY_BOOL m_bcardconnect;
    LTY_BOOL m_stationprepare;
	//LTY_RESULT TextConvert(LTY_CHAR *srcEnc, LTY_CHAR *srcBuf, LTY_U16 srcBufLen, LTY_CHAR *destEnc, LTY_CHAR *destBuf);
    //LTY_RESULT OnCmdReqVideo(tts_cmd_head_t* pHead, LTY_STRING pStr, LTY_U32 u32Len);
};

class CCardSvr : private CSocketServer
{
public:
    CCardSvr();
    virtual ~CCardSvr();
    LTY_RESULT Init(struct ev_loop *loop);
    LTY_VOID Release();

private:
    CCardWorker m_CardWorker;

    LTY_VOID SSOnTimer();
    LTY_VOID SSOnError(LTY_RESULT ret);
    LTY_BOOL SSOnNewClient(int fd, LTY_U32 u32CltIP, LTY_U16 u16CltPort);

};


void start_card_server();

#endif

