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

/*包头*/
#define CARD_CMD_REQ_FLAG		0x02

/*从命令*/
#define CARD_CMD_REQ_SLAVE  	0x01
#if 0
/*1.握手命令------------------------------------*/
#define CARD_CMD_REQ_HANDSHAKE  0xb1
#define CARD_CMD_RSP_HANDSHAKE  0xb2

#define CARD_HANDSHAKE_SUCCESS		1
#endif
/*2.显示控制命令--------------------------------*/
#define CARD_CMD_REQ_SHOW_CONTROL  0xb3
#define CARD_CMD_RSP_SHOW_CONTROL  0xb4







/*2.2 返回结果*/
#define CARD_SHOW_CONTROL_SUCCESS	1  /*显示成功*/
#define CARD_SHOW_CONTROL_FAIL		0  /*显示失败*/

#if 0
/*3.指示灯控制命令--------------------------------*/
#define CARD_CMD_REQ_LED_CONTROL  0xb5
#define CARD_CMD_RSP_LED_CONTROL  0xb6

#define CARD_CMD_REQ_SLAVE_LED_RUN  	0x01  	/*运行指示灯*/
#define CARD_CMD_REQ_SLAVE_LED_GREEN  	0x02	/*绿灯*/
#define CARD_CMD_REQ_SLAVE_LED_RED  	0x03	/*红灯*/


/*3.1 控制灯的状态*/
#define CARD_LED_STATUS_ON		1
#define CARD_LED_STATUS_OFF		0
#endif

/*3.语音控制命令--------------------------------*/
#define CARD_CMD_REQ_VOICE_CONTROL  0xb7
#define CARD_CMD_RSP_VOICE_CONTROL  0xb8

/*3.2 返回结果*/
#define CARD_VOICE_CONTROL_SUCCESS	1  /*播放成功*/
#define CARD_VOICE_CONTROL_FAIL		0  /*播放失败*/

/*4.参数导入导出控制命令--------------------------------*/
#define CARD_CMD_REQ_PARAM_CONTROL  0xb9
#define CARD_CMD_RSP_PARAM_CONTROL  0xba

/*4.1 请求类型*/
#define CARD_CMD_REQ_SLAVE_PARAM_IMPORT 0x01  /*参数导入*/
#define CARD_CMD_REQ_SLAVE_PARAM_EXPORT 0x02  /*参数导出*/
#define CARD_CMD_REQ_SLAVE_PARAM_CLEAN  0x03  /*清除参数*/
#define CARD_CMD_REQ_SLAVE_RECORD_CLEAN 0x04  /*清除记录*/



#define CARD_PARAM_CONTROL_SUCCESS	1  /*导入/导出成功*/
#define CARD_PARAM_CONTROL_FAIL		0  /*导入/导出失败*/


/*5.获取金额控制命令--------------------------------*/
#define CARD_CMD_REQ_GET_MONEY  0xbb
#define CARD_CMD_RSP_GET_MONEY  0xbc

/*6.获取站点信息命令--------------------------------*/
#define CARD_CMD_REQ_STATION_INFO  0xbd
#define CARD_CMD_RSP_STATION_INFO  0xbe

/*7.获取时区命令-------------------------------- */
#define CARD_CMD_REQ_TIME_ZONE  0xc0
#define CARD_CMD_RSP_TIME_ZONE  0xc1

/*8.获取客流命令-------------------------------- */
#define CARD_CMD_REQ_PASSENGER_FLOW  0xc2
#define CARD_CMD_RSP_PASSENGER_FLOW  0xc3

/*9.通知参数命令-------------------------------- */
#define CARD_CMD_REQ_PARAM_CHANGE  0xc4
#define CARD_CMD_RSP_PARAM_CHANGE  0xc5

/*10.心跳包命令-------------------------------- */
#define CARD_CMD_REQ_HEARTBEAT  0xc6
#define CARD_CMD_RSP_HEARTBEAT  0xc7

/*11.TBP网络状态命令-------------------------------- */
#define CARD_CMD_REQ_TBP_STATUS  0xc8
#define CARD_CMD_RSP_TBP_STATUS  0xc9

/*12.进出站命令---------------------------------------*/
#define CARD_CMD_REQ_STATION_IN_OUT  0xca
#define CARD_CMD_RSP_STATION_IN_OUT  0xcb

/*13.ACC关机通知命令---------------------------------------*/
#define CARD_CMD_REQ_ACC_NOTIFY  	0xcc


typedef struct card_cmd_head_s
{
	LTY_U8 u8Flag;    			/*包头标识*/
	LTY_U8 u8CmdMaster; 		/*命令主*/
	LTY_U8 u8CmdSlave;			/*命令从*/
	LTY_U8 u8DataLen;			/*数据长度*/
	//LTY_CHAR *u8DataContent;	/*数据内容*/
	LTY_U16 u16Parity;          /*CRC校验*/
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
	LTY_U32 u32Longitude;             //经度，0.1秒
    LTY_U32 u32Latitude;              //纬度，0.1秒
    LTY_U16 u16Angle;                 //方位角，0-359度
    LTY_U32 u32GPRSLineNo;
	LTY_CHAR station_status;   //0:进站 1:离站
	LTY_U32 direction;
}global_info_t;

typedef struct lty_passenger_head_s
{
    LTY_U32 u32DateTime;/* 客流时间 UTC*/
    LTY_U32 u32Lnt;   /*经度  蓝泰源格式的坐标：第一字节为度，第二字节为分，第三和第四字节为分的小数位、其中第三字节为高位、第四字节为低位，且这2个字节组成一个字、是分的小数位绝对数值的16进制数字。经纬度精确到分的小数点后4位*/
    LTY_U32 u32Lat; /*纬度， 同上*/
    LTY_U8  u8Status; /*定义在第二结构体*/
    LTY_U8  u8StationID;/*当前站点 从1开始*/
    LTY_U8  u8StationAttr;  //站点标识： 1字节，0(起点站)、ff(终点站)、其它(中途站)
    LTY_U32 u32LineId;     //线路ID
	LTY_U32 u32CityStaID;  //城市站点序号
}PACKED(1) lty_passenger_head_t;

typedef union
{
    LTY_U8 u8Status;
    struct
    {
        LTY_U8 u1Working   : 1;  //0:未营运 1:营运
        LTY_U8 u1Dir       : 1;  //0:上行 1:下行
        LTY_U8 u1FDoor     : 1;  //0:前门关 1:前门开
        LTY_U8 u1BDoor     : 1;  //0:后门关 1:后门开
        LTY_U8 u1OffLine   : 1;  //0:正常行驶 1:离线行驶
        LTY_U8 u1Event     : 1;  //无用
        LTY_U8 u1OverSpeed : 1;  //0:未超速 1:超速中
        LTY_U8 u1Park      : 1;  //0:站场外 1:站场内
    }PACKED(1) stStatus;
}PACKED(1) itsnw_bus_status1_u;

typedef struct lty_passenger_data_s
{
	lty_passenger_head_t stPassengerHd;
    LTY_U8  u8Up5Sec; /*5秒之内上的客流*/
	LTY_U8  u8Down5Sec; /*5秒之内下的客流*/
	LTY_U16	u16Up; /*车上总人数*/
}PACKED(1) lty_passenger_data_t;


typedef struct lty_gps_data_s
{
	LTY_U8 LOC_TIME[8];//LTY_U64 LOC_TIME; //	定位时间
	LTY_U8 LON[4];//LTY_U32 LON;      //	经度，单位为1*10-6度。
	LTY_U8 LAT[4];//LTY_U32 LAT;	  //	纬度，单位为1*10-6度。
	LTY_U8 VEC1[2];//LTY_U16 VEC1;     //	速度，指卫星定位车载终端设备上传的行车速度信息，为必填项。单位为千米每小时(km/h)。【取整】
	LTY_U8 VEC2[2];//LTY_U16 VEC2;     //	行驶记录速度，指车辆行驶记录设备上传的行车速度信息。单位为千米每小时(km/h)。【取整】
	LTY_U8 DISTANCE[4];//LTY_U32 DISTANCE; //	车辆当日总里程数，指车辆上传的行车里程数。单位为米(m)。终端设备每天开机时候清零；【取整】
	LTY_U8 DIRECTION[2];//LTY_U16 DIRECTION;//	方向，0-359，单位为度(°)，正北为0，顺时针。
	LTY_U8 ALTITUDE[2];//LTY_U16 ALTITUDE; //	海拔高度，单位为米(m)。
	LTY_U8 DIS_NXT_STOP[2];//LTY_U16 DIS_NXT_STOP;//	距离下站距离，单位米（m）
	LTY_U8 TIME_NXT_STOP[4];//LTY_U32 TIME_NXT_STOP;//	距离下站的时间，单位秒（s）
	LTY_U8 INDOOR_TEMP;  //	车内温度，单位为度。
	LTY_U8 NXT_STOP_NO;  //	下一个站点序号。
	LTY_U8 GPS_SIGNAL;   //	强调范围0～32，12以下为信号弱；
	LTY_U8 CAN_NO;       //	CanBus 协议编号;0标识未连接can总线。非0就标识CAN总线版本号。
	LTY_U8 CAN_LTH[2];//LTY_U16 CAN_LTH;      //	后续数据 CanBus 数据块长度（CanBus 协议编号为0 时本字段也为 0）
	//CAN_DATA		BYTES//	CanBus 信息长度为 0 时本字段为空，协议内容另见CanBus 厂商协议文档;	
	
}PACKED(1) lty_gps_data_t;



typedef struct lty_station_data_s
{
	LTY_U8  VEH_STATE; //车辆运行状态
	LTY_U8 STOP_CODE[4]; //城市公交站点编码
	LTY_U8 STOP_NO[4];//LTY_U32 STOP_NO;   //当前公交站点序号
	LTY_U8  DRIVER_ID[10]; //司机编号
	LTY_U8  VEHICLE_ID[6]; //车辆编号 
	LTY_U8  STOP_FLAG;  //公交站点标识；0x00，首战；0xff，末站；0x02，拐弯点；0x03，安全事故黑点；
	LTY_U8  STOP_RPT;   //报站方式，0x01，GPS自动报站；0x00，手动报站；0x02，表示调试；
	LTY_U8  IN_OUT_FLAG; //进出站标识；0，进站；1，离站
	LTY_U8  ONDIRECTION_FLAG; //0:上行 1:下行
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
    lty_buf_t CardBuf[2]; // 保存生成的Card数据，通过Domain Socket发送到客户端

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
	LTY_U8 m_u8NextStationID;       	//下站站点序号，默认0
	LTY_U8 m_current_station_id;
	LTY_U8  m_u8StInOutFlug;					//0站内，1站外
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

