#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

//#include <minigui/common.h>


#include "LtyCommon.h"
#include "sysdesk.h"
#include "LtyMsgItsIO.h"
#include "LtyMsgItsSD.h"
#include "LtyMsgException.h"
#include "LtyMsgItsStation.h"
#include "LtyMsgItsNW.h"
#include "LtyMsgStmNW.h"
#include "LtyMsgNet.h"
#include "LtyMsgAV.h"
#include "LtyMsgRec.h"
#include "LtyMsgGuiCfg.h"
#include "ltyonvifapi.h"
#include "LtyMsgItsAudio.h"
#include "LtyMsgUserInput.h"
#include "LtyMsgSysCtrl.h"
#include "LtyMsgCfg.h"
#include "mainpg.h"
#include "itsnw/TmpNWProtocol.h"



// #include "LtyMsgNet.h"



#include "guicontrol.h"
#include "cJSON.h"
#include "ltyuicallinterface.h"

#include "LtyMsgUpg.h"
#include "../../itsio/MCUCtrlStruct.h"

#include "QTGUI/CardSvr.h"
#include "inputmisc/QRScan.h"
extern void gui_run_status_get(char _type,void *_param);
extern void gui_main_page_get(PTQrPrice ptQrPrice);

static cfg_para_vdo_prev_t* g_vdoparam = NULL;

int g_qt_direction = -1;
LTY_U32 g_u32BusStationId = 0;


global_info_t g_global_info;


#define BUSSTOP_X           182
#define BUSSTOP_Y           158
#define BUSSTOP_W           390
#define BUSSTOP_H           108
#define STOP_INFO_X         151
#define STOP_INFO_Y         179
#define HDD_X               32
#define HDD_Y               0
#define ITSSD_X             92
#define ITSSD_Y             0
#define DVRSD_X             152
#define DVRSD_Y             0
#define PERIPHERAL_X        212
#define PERIPHERAL_Y        0
#define ACCON_X             272
#define ACCOFF_Y            0
#define DVR_CAME_X          332
#define DVR_CAME_Y          0
#define DVR_CAME_INDIV1_X   7
#define DVR_CAME_INDIV1_Y   16
#define DVR_CAME_INDIV2_X   (DVR_CAME_INDIV1_X + 16)
#define DVR_CAME_INDIV2_Y   DVR_CAME_INDIV1_Y
#define DVR_CAME_INDIV3_X   (DVR_CAME_INDIV1_X + 32)
#define DVR_CAME_INDIV3_Y   DVR_CAME_INDIV1_Y
#define DVR_CAME_INDIV4_X   DVR_CAME_INDIV1_X
#define DVR_CAME_INDIV4_Y   (DVR_CAME_INDIV1_Y + 20)
#define DVR_CAME_INDIV5_X   (DVR_CAME_INDIV1_X + 16)
#define DVR_CAME_INDIV5_Y   (DVR_CAME_INDIV1_Y + 20)
#define DVR_CAME_INDIV6_X   (DVR_CAME_INDIV1_X + 32)
#define DVR_CAME_INDIV6_Y   (DVR_CAME_INDIV1_Y + 20)
#define DVR_CAME_INDIV7_X   DVR_CAME_INDIV1_X
#define DVR_CAME_INDIV7_Y   (DVR_CAME_INDIV1_Y + 40)
#define DVR_CAME_INDIV8_X   (DVR_CAME_INDIV1_X + 16)
#define DVR_CAME_INDIV8_Y   (DVR_CAME_INDIV1_Y + 40)

#define GPS_SIG_X           392
#define GPS_SIG_Y           0
#define MOBILE_SIG_X        452
#define MOBILE_SIG_Y        0
#define MOBILE_SIG_X1       512
#define MOBILE_SIG_Y1       0
#define ITS_NET_X           572
#define ITS_NET_Y           0
#define DVR_NET_X           632
#define DVR_NET_Y           0
#define SYNC_MEDIA_X        640
#define SYNC_MEDIA_Y        0
#define USB_DEV_X           632
#define USB_DEV_Y           0
#define TIME_X              344
#define TIME_Y              110
#define TIME_W              161
#define TIME_H              40
#define DATE_X              130
#define DATE_Y              110
#define DATE_W              212
#define DATE_H              45
#define WEEK_X              510
#define WEEK_Y              110
#define WEEK_W              80
#define WEEK_H              45
#define SPEED_X             25
#define SPEED_Y             290
#define SPEED_LIMIT_X       73
#define SPEED_LIMIT_Y       415
#define FRONTDOOR_X         256
#define FRONTDOOR_Y         434
#define MIDDLEDOOR_X        334
#define MIDDLEDOOR_Y        434
#define BACKDOOR_X          414
#define BACKDOOR_Y          434
#define DIRECTION_X         33
#define DIRECTION_Y         518
#define LINE_LABEL_X        120
#define LINE_LABEL_Y        507
#define LINE_X              114
#define LINE_Y              538
#define NOTIFY_STATION_X    66
#define NOTIFY_STATION_Y    229
#define NOTIFY_STATION_W    54
#define NOTIFY_STATION_H    28

#define BUS_NUM_LABEL_X     220
#define BUS_NUM_LABEL_Y     506
#define BUS_NUM_LABEL_W     99
#define BUS_NUM_LABEL_H     33
#define BUS_NUM_X           216
#define BUS_NUM_Y           536
#define PLATE_NUM_LABEL_X   349 //车牌号码
#define PLATE_NUM_LABEL_Y   505
#define PLATE_NUM_X         326
#define PLATE_NUM_Y         536
#define BUS_DRIVER_LABEL_X  497
#define BUS_DRIVER_LABEL_Y  506
#define BUS_DRIVER_X        483
#define BUS_DRIVER_Y        536
#define BUSSTATUS_X         605
#define BUSSTATUS_Y         519
#define FIRSTLINE_MSG_X     198
#define FIRSTLINE_MSG_Y     263
#define FIRSTLINE_MSG_W     348
#define FIRSTLINE_MSG_H     154
/*
#define SECONDLINE_MSG_X    198
#define SECONDLINE_MSG_Y    301
#define THIRDLINE_MSG_X     198
#define THIRDLINE_MSG_Y     339
*/
#define WARN_IMAGE_X        585
#define WARN_IMAGE_Y        159
#define WARN_BLINK_X        0
#define WARN_BLINK_Y        0

#define BUS_STOP_ID         51
#define BUSSTOP_INFO_ID     52
#define BUSSTATUS_ID        53
#define TIME_ID             54
#define SPEED_ID            55
#define SPEED_LIMIT_ID      56
#define BUS_DIRECTION_ID    57
#define LINE_LABEL_ID       58
#define LINE_ID             59
#define BUS_NUM_ID          60
#define PLATE_NUM_LABEL_ID  61
#define PLATE_NUM_ID        62
#define BUS_DRIVER_LABEL_ID 63
#define BUS_DRIVER_ID       64
#define BUS_STATUS_ID       65
#define FIRSTLINE_MSG_ID    66
//#define SECONDLINE_MSG_ID   67
//#define THIRDLINE_MSG_ID    68
#define BUS_NUM_LABEL_ID    69
#define NOTIFY_STATION_ID   70
#define DATE_ID             71
#define WEEK_ID             72

#if 0
static handle* g_camera_flag = NULL;
static handle* g_came_indiv[8] = {NULL};
static handle* g_warninfo_image = NULL;
static handle* g_warnblink = NULL;
static handle* g_hddstatus = NULL;
static handle* g_fdoorstatus = NULL;
static handle* g_mdoorstatus = NULL;
static handle* g_bdoorstatus = NULL;
static handle* g_accstatus = NULL;
static handle* g_dvrnet = NULL;
static handle* g_itshandle = NULL;
static handle* g_g3signal = NULL;
static handle* g_g3signal1 = NULL;

static handle* g_psignalbar = NULL;
static handle* g_psignalbar1 = NULL;

static handle* g_gps  = NULL;
static handle* g_itssd = NULL;
static handle* g_dvrsd = NULL;
static handle* g_peripheral = NULL;
static handle* g_usb  = NULL;
static handle* g_talk = NULL;
static handle* g_devdoor = NULL;
static handle* g_syncflag = NULL;
static cfg_para_vdo_prev_t* g_vdoparam = NULL;

extern PLOGFONT g_font20, g_font36, g_font48,g_font24,g_font30, g_font42,g_font60, g_font144,
    g_digitfont120, g_digitfont42, g_digitfont80, g_font38, g_font32;
#endif


#define SHOW_TIME_TIMER  100
#define ID_TALK_TIMER    101
#define ID_DEBUS_TIMER   102
#define FLAG_OVERSPEED   0x00000001
#define FLAG_OFFLINE     0x00000002
#define MYICONDIR       "res/pic/"
#define SHUTDOWN_INFO   0
#define SHUTING         "正在关机中......"
#define SD_INFO         1
#define CHECKINGSD      "SD卡初始化中......"
#define TALK_X       493
#define TALK_Y       438
#define TALK_TEXT_X  5
#define TALK_TEXT_Y  5
#define TALK_TEXT_W  40
#define TALK_TEXT_H  40
#define TALK_TEXT_ID 74
#define DEV_DOOR_X   164
#define DEV_DOOR_Y   438
//static int g_talkstatus = 0; //没有喊话状态
typedef struct ltyMapKey_Gui_S{
    LTY_U32 u32Guikey;
    LTY_U32 u32keycode;
}MapKey_Gui_S;

typedef struct ltyCAM_POS_S{
    LTY_U32 u32X;
    LTY_U32 u32Y;
}CAM_POS_S;

typedef struct ltyCAM_STATUS_S{
    LTY_S32 s32chan;
    LTY_S32 camstatus;
}CAM_STATUS_S;
typedef struct ltySYNC_STATUS_S{
    LTY_S32 s32Syncstatus;
    LTY_S32 s32Syncway;
}SYNC_STATUS_S;

#ifndef NR_KEYS
#define NR_KEYS                         128
#endif

#define SCANCODE_USER                   (NR_KEYS + 1)


#define SCANCODE_1                      2
#define SCANCODE_2                      3
#define SCANCODE_3                      4
#define SCANCODE_4                      5
#define SCANCODE_5                      6
#define SCANCODE_6                      7
#define SCANCODE_7                      8
#define SCANCODE_8                      9
#define SCANCODE_9                      10
#define SCANCODE_0                      11

#define SCANCODE_ENTER                  28


MapKey_Gui_S g_astGuiKeyTable[]=
{
    {KEYPAD_KEY_F1, SCANCODE_1},
    {KEYPAD_KEY_F2, SCANCODE_2},
    {KEYPAD_KEY_F3, SCANCODE_3},
    {KEYPAD_KEY_F4, SCANCODE_4},
    {KEYPAD_KEY_F5, SCANCODE_5},
    {KEYPAD_KEY_F6, SCANCODE_6},
    {KEYPAD_KEY_F7, SCANCODE_7},
    {KEYPAD_KEY_F8, SCANCODE_8},
    {KEYPAD_KEY_F9, SCANCODE_9},
    {KEYPAD_KEY_F0, SCANCODE_0},
    {KEYPAD_KEY_PLAY, SCANCODE_PLAY},
    {KEYPAD_KEY_CANCEL, SCANCODE_CANCEL},
    {KEYPAD_KEY_CONFIRM, SCANCODE_ENTER},
    {KEYPAD_KEY_MENU, SCANCODE_TFT_MENU},
    {KEYPAD_KEY_TALK, SCANCODE_TALK},
    {KEYPAD_KEY_TALKOVER, SCANCODE_TALKOVER},
    {KEYPAD_KEY_AV, SCANCODE_TFT_AV},
    {KEYPAD_KEY7_F1, SCANCODE_T7_F1},
    {KEYPAD_KEY7_F2, SCANCODE_T7_F2},
    {KEYPAD_KEY7_F3, SCANCODE_T7_F3},
    {KEYPAD_KEY7_F4, SCANCODE_T7_F4},
    {KEYPAD_KEY7_F5, SCANCODE_T7_F5},
    {KEYPAD_KEY7_F6, SCANCODE_T7_F6},
    {KEYPAD_KEY7_F7, SCANCODE_T7_F7},
    {KEYPAD_KEY7_F8, SCANCODE_T7_F8},
    {KEYPAD_KEY7_F9, SCANCODE_T7_F9},
    {KEYPAD_KEY7_F0, SCANCODE_T7_F0},
    {KEYPAD_KEY7_PLAY, SCANCODE_T7_PLAY},
    {KEYPAD_KEY7_REPLAY, SCANCODE_T7_REPLAY},
    {KEYPAD_KEY7_MENU, SCANCODE_T7_MENU},
    {KEYPAD_KEY7_EXIT, SCANCODE_T7_EXIT},
    {KEYPAD_KEY7_FUN, SCANCODE_T7_FUN},
    {KEYPAD_KEY7_UP, SCANCODE_T7_UP},
    {KEYPAD_KEY7_LEFT, SCANCODE_T7_LEFT},
    {KEYPAD_KEY7_RIGHT, SCANCODE_T7_RIGHT},
    {KEYPAD_KEY7_DOWN, SCANCODE_T7_DOWN}
};

extern void gui_system_maintain_get(char _type,char _main_type,char _operate_type,char _percent,const char *_failed_result);
extern void gui_system_key_up(int keycode);

extern void gui_two_dimensional_code_info(void  *_param);
SysDesk::SysDesk()
{ 
	m_warnflag = 0;
	m_curspeed = 0;
    m_curlimspeed = 0;

	g_vdoparam = (cfg_para_vdo_prev_t*)malloc(sizeof(cfg_para_vdo_prev_t));
    if (NULL == g_vdoparam){
        return ;
    }
	LTY_RESULT ret;
    ret = LtyMessageSend(LTY_MSG_CFG_GET_COMMON_DATA, CFG_PARA_VDO_PREV, (LTY_U32)g_vdoparam, sizeof(cfg_para_vdo_prev_t));
    if (LTY_SUCCESS != ret){
        ERROUT("fail to get Vdoprev data..\n");
        return;
    }
}

LTY_RESULT SysDesk::MsgHandle(LTY_MSG_TYPE msg, LTY_U32 chan, LTY_U32 buf,
              LTY_U32 bufsize, LTY_LPVOID lpPriv)
{
	PIC_MODE picmode;
	//printf("MsgHandle start\n");
    switch(msg){
		case LTY_MSG_ITSIO_NOTIFY_GPS_INFO:
		{
			itsio_gps_info_t* pgpsinfo = (itsio_gps_info_t*)buf;
            char aszspeed[10];
			cJSON *root = NULL;
			//printf("MsgHandle system_gps_info\n");
            if(1 == (pgpsinfo->u16Flag & 0x01)){
                sprintf(aszspeed, "%d", pgpsinfo->u16Speed/10);
                //SetWindowText(GetDlgItem(hWnd, SPEED_ID), aszspeed);
                //m_curspeed = pgpsinfo->u16Speed;
                //lty_ui_image_changeimage(g_gps, 2);
                root = cJSON_CreateObject();
				if(root )
			    {   
			    	//printf("MsgHandle system_gps_info  1\n");
					LTY_CHAR aszLongitude[15] = {0}, aszLatitude[15] = {0};
					
				    sprintf(aszLongitude, "%d%d.%04d", pgpsinfo->u32Longitude/36000, pgpsinfo->u32Longitude%36000/600,
                       pgpsinfo->u32Longitude%36000%600*10000/600);
                    sprintf(aszLatitude, "%d%d.%04d", pgpsinfo->u32Latitude/36000, pgpsinfo->u32Latitude%36000/600,
                       pgpsinfo->u32Latitude%36000%600*10000/600);

				  g_global_info.u32Longitude = pgpsinfo->u32Longitude;			 //经度，0.1秒
				  g_global_info.u32Latitude = pgpsinfo->u32Latitude; 			 //纬度，0.1秒
				  g_global_info.u16Angle = pgpsinfo->u16Angle;				 //方位角，0-359度

						
				   cJSON_AddStringToObject(root, "msg_type", "system_gps_info");
				   cJSON_AddNumberToObject(root, "gps_speed", pgpsinfo->u16Speed/10);
				   cJSON_AddNumberToObject(root, "gps_status", 2);
				   cJSON_AddNumberToObject(root, "gps_number", pgpsinfo->u8StarNum);
				   cJSON_AddNumberToObject(root, "gps_altitude", pgpsinfo->s16Altitude);/*海拔高度，米*/
				   cJSON_AddNumberToObject(root, "gps_angle", pgpsinfo->u16Angle);/*方位角，0-359度*/
				   cJSON_AddNumberToObject(root, "gps_speed", pgpsinfo->u16Speed);/*速度，0.1公里/小时*/
				   cJSON_AddStringToObject(root, "gps_longitude", aszLongitude);/*经度*/
				   cJSON_AddStringToObject(root, "gps_latitude", aszLatitude);  /*纬度*/          


				   //printf("MsgHandle system_gps_info() u16Speed:%d,u8StarNum:%d\n",pgpsinfo->u16Speed,pgpsinfo->u8StarNum);

				   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

			    }
            }
		}break;
	    case LTY_MSG_ITSIO_NOTIFY_GPS_EXCEPTION:
		{
			cJSON *root = NULL;
			root = cJSON_CreateObject();
			if(root )
		    {   
			   cJSON_AddStringToObject(root, "msg_type", "system_gps_info");
			   cJSON_AddNumberToObject(root, "gps_status", 1);
			   
			   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

		    }
			
		}break;
		case LTY_MSG_ITSIO_NOTIFY_GPS_INVAIL:
		{
			cJSON *root = NULL;
			root = cJSON_CreateObject();
			if(root )
		    {   
			   cJSON_AddStringToObject(root, "msg_type", "system_gps_info");
			   cJSON_AddNumberToObject(root, "gps_status", 1);
			   
			   ltySendMessageToUi(root,kEnumUiMessageOther, NULL);

		    }
		}break;
		case LTY_MSG_ITSSD_NOTIFY_INFO:
		{
			sd_data_t* psdinfo = (sd_data_t*)buf;
            //SendMessage(m_hSysStartInfo, MSG_CLOSE, 0, 0);
            //m_hSysStartInfo = HWND_INVALID;

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_itssd_notify_info");
            if (1 == psdinfo->u8VailFlag){
                LTY_S32 s32direct;
                LTY_S32 s32cursta;
                //LTY_CHAR aszLimitSpeed[16];
                //m_bloadsd = true;
                //SetWindowText(GetDlgItem(hWnd, LINE_ID), psdinfo->tLinePara.tLineBase.charLineName);
				cJSON_AddStringToObject(root, "line_name", psdinfo->tLinePara.tLineBase.charLineName);/*线路名*/
				
                if (0 == psdinfo->tBusPara.tDriverBase.u8QianTuiFlag){
                    //SetWindowText(GetDlgItem(hWnd, BUS_DRIVER_ID), psdinfo->tBusPara.tDriverBase.charDriverNoBuf);
					cJSON_AddStringToObject(root, "driver_number", psdinfo->tBusPara.tDriverBase.charDriverNoBuf);/*驾驶员编号*/
                }
                else{
                    //SetWindowText(GetDlgItem(hWnd, BUS_DRIVER_ID), "");
					cJSON_AddStringToObject(root, "driver_number", "");
                }
				
                g_qt_direction = psdinfo->tLinePara.tLineBase.u8UpOrDownFlag;
                if (DIRECTION_UP == psdinfo->tLinePara.tLineBase.u8UpOrDownFlag){
                    //SetWindowText(GetDlgItem(hWnd,BUS_DIRECTION_ID),"上行");
                    cJSON_AddNumberToObject(root, "up_down_flag", 1);/*上行*/
                }
                else{
                    //SetWindowText(GetDlgItem(hWnd,BUS_DIRECTION_ID),"下行");
                    cJSON_AddNumberToObject(root, "up_down_flag", 2);/*下行*/
                }
                //SetWindowText(GetDlgItem(hWnd,BUS_NUM_ID), psdinfo->tBusPara.tEquBase.charBusNoBuf);
                //SetWindowText(GetDlgItem(hWnd, PLATE_NUM_ID), psdinfo->tBusPara.tEquBase.charBusLicenseBuf);
				cJSON_AddStringToObject(root, "bus_number", psdinfo->tBusPara.tEquBase.charBusNoBuf);/*车辆编号*/
				cJSON_AddStringToObject(root, "bus_license", psdinfo->tBusPara.tEquBase.charBusLicenseBuf);/*车牌号*/
				
                if (0 == psdinfo->tBusPara.tBusConfig.u8GPSEnFlag){
                    //SetWindowText(GetDlgItem(hWnd, NOTIFY_STATION_ID),"手动");
					cJSON_AddNumberToObject(root, "auto_flag", 1);  /*手动*/
                }
                else{
                    //SetWindowText(GetDlgItem(hWnd, NOTIFY_STATION_ID),"自动");
                    cJSON_AddNumberToObject(root, "auto_flag", 2);  /*自动*/
                }
                if (0 == psdinfo->tBusPara.tBusConfig.u8YingYunType){
                    //SetWindowText(GetDlgItem(hWnd,BUS_STATUS_ID), "非营运");
                    cJSON_AddNumberToObject(root, "operation_flag", 1); /*非营运*/
                }
                else{
                    //SetWindowText(GetDlgItem(hWnd,BUS_STATUS_ID), "营运");
					cJSON_AddNumberToObject(root, "operation_flag", 2);/*营运*/
                }
                s32direct = psdinfo->tLinePara.tLineBase.u8UpOrDownFlag;
                if (DIRECTION_DOWN == s32direct){
                    s32direct = DIRECTION_UP;
                }
                else{
                    s32direct = DIRECTION_DOWN;
                }
                s32cursta = psdinfo->tLinePara.tLineBase.u8CurrentLineStation;

				g_global_info.u32GPRSLineNo = psdinfo->tLinePara.tLineBase.u32GPRSLineNo;
				
                //sprintf(aszLimitSpeed, "%d",
                  //     psdinfo->tLinePara.tLineAssign[s32direct].tNormStation.tNormStationZuoBiao[s32cursta].u8StationBwLimitSpeed);
                //SetWindowText(GetDlgItem(hWnd, SPEED_LIMIT_ID), aszLimitSpeed);
                cJSON_AddNumberToObject(root, "current_limspeed", psdinfo->tLinePara.tLineAssign[s32direct].tNormStation.tNormStationZuoBiao[s32cursta].u8StationBwLimitSpeed);
                //lty_ui_image_changeimage(g_itssd, 2);

				//LTY_U32 u32SpecialDotNum[2];
    			//line_special_dot_t stSpecialDotList[2][SPECIAL_DOT_NUM_MAX];
    			int last_pos = psdinfo->tLinePara.tLineAssign[s32direct].u8StationCount;
				
				cJSON_AddStringToObject(root, "first_station", psdinfo->tLinePara.tLineAssign[s32direct].tNormStation.tNormStationZuoBiao[0].aszStationName);
				cJSON_AddStringToObject(root, "last_station", psdinfo->tLinePara.tLineAssign[s32direct].tNormStation.tNormStationZuoBiao[last_pos - 1].aszStationName);
				
				cJSON_AddNumberToObject(root, "sd_status", 1);/*SD卡状态 1:正常 0:异常*/
                //SetWindowText(GetDlgItem(hWnd, LINE_ID), psdinfo->tLinePara.tLineBase.charLineName);

				ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
				
            }
            else{
				cJSON_Delete(root);
                return -1;
            }			
		}break;
		case LTY_MSG_NOTIFY_STATION:
		{
            station_info(buf,0);
		    
		}break;
		case LTY_MSG_NOTIFY_DIRECTION_CHANGE:
		{
			Direction_S* pstdirect = (Direction_S*)buf;
			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_direction_change");

			g_qt_direction = pstdirect->u32Direction;
			
            if (DIRECTION_UP == pstdirect->u32Direction){
                //SetWindowText(GetDlgItem(hWnd, BUS_DIRECTION_ID), "上行");
				cJSON_AddNumberToObject(root, "up_down_flag", 1);/*上行*/
            }
            else{
                //SetWindowText(GetDlgItem(hWnd, BUS_DIRECTION_ID), "下行");
				cJSON_AddNumberToObject(root, "up_down_flag", 2);/*下行*/
            }
            if (1 == pstdirect->s32Enter){
                //SetWindowText(GetDlgItem(hWnd, BUSSTOP_INFO_ID), "本站");
                //etWindowText(GetDlgItem(hWnd, BUS_STOP_ID), pstdirect->pszBusStationName);
                cJSON_AddNumberToObject(root, "status", 1);

				#if 0
				if(pstdirect->pszPrevStationName)
					cJSON_AddStringToObject(root, "prev_station", pstdirect->pszPrevStationName);	
				#endif
				if(pstdirect->pszBusStationName)
					cJSON_AddStringToObject(root, "current_station", pstdirect->pszBusStationName);/*当前站名*/
				cJSON_AddNumberToObject(root, "current_id", pstdirect->s32CurrStationId);
				if(pstdirect->pszNextStationName)
					cJSON_AddStringToObject(root, "next_station", pstdirect->pszNextStationName);

            }
			
            else{
                //SetWindowText(GetDlgItem(hWnd, BUSSTOP_INFO_ID), "下站");
                //SetWindowText(GetDlgItem(hWnd, BUS_STOP_ID), pstdirect->pszNextStationName);
                cJSON_AddNumberToObject(root, "status", 2);
				#if 0
				if(pstdirect->pszPrevStationName)
					cJSON_AddStringToObject(root, "prev_station", pstdirect->pszPrevStationName);
                cJSON_AddStringToObject(root, "current_station", pstdirect->pszNextStationName);/*当前站名*/
				if(pstdirect->pszNextStationName)
					cJSON_AddStringToObject(root, "next_station", pstdirect->pszNextStationName);
				#endif
            }
			

			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		}break;
		case LTY_MSG_NOTIFY_OFFLINE:
		{

		}break;
		case LTY_MSG_NET_NOTIFY_G3_MODEM_STATUS: 
		{
			g3_modem_status_t* p3gstatus = (g3_modem_status_t*)buf;
            //printf("the signal is %d......\n", p3gstatus->u16SigQual);
			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_g3_modem_status");
			
            if (0 == p3gstatus->u32ModemSeq || 1 == p3gstatus->u32ModemSeq){
				int signal_value = 0;
				if(p3gstatus->u16SigQual <= 0)
					signal_value = 5;
				else if(p3gstatus->u16SigQual > 0 && p3gstatus->u16SigQual < 1)
					signal_value = 0;
				else if(p3gstatus->u16SigQual >= 1 && p3gstatus->u16SigQual < 8)
					signal_value = 1;
				else if(p3gstatus->u16SigQual >= 8 && p3gstatus->u16SigQual < 16)
					signal_value = 2;
				else if(p3gstatus->u16SigQual >= 16 && p3gstatus->u16SigQual < 24)
					signal_value = 3;
				else if(p3gstatus->u16SigQual >= 24 && p3gstatus->u16SigQual <= 31)
					signal_value = 4;
				
				cJSON_AddNumberToObject(root, "g3_signal", signal_value);

				int model_status = 1;
				int sim_status = 0;
				const char *network_type = "";
				int reset_times = p3gstatus->u16HWResetCnt;
					

				if(G3_NETWORK_SRVTYPE_UMTS == p3gstatus->enNWType)
					network_type = "UMTS";
				else if(G3_NETWORK_SRVTYPE_GSM_GPRS == p3gstatus->enNWType)
					network_type = "GSM_GPRS";
				else if(G3_NETWORK_SRVTYPE_EDGE == p3gstatus->enNWType)
					network_type = "EDGE";
				else if(G3_NETWORK_SRVTYPE_WCDMA == p3gstatus->enNWType)
					network_type = "WCDMA";
				else if(G3_NETWORK_SRVTYPE_HSDPA == p3gstatus->enNWType)
					network_type = "HSDPA";
				else if(G3_NETWORK_SRVTYPE_HSUPA == p3gstatus->enNWType)
					network_type = "HSUPA";
				else if(G3_NETWORK_SRVTYPE_HSDPA_HSUPA == p3gstatus->enNWType)
					network_type = "HSDPA_HSUPA";
				else if(G3_NETWORK_SRVTYPE_CDMA == p3gstatus->enNWType)
					network_type = "CDMA";
				else if(G3_NETWORK_SRVTYPE_HDR == p3gstatus->enNWType)
					network_type = "HDR";
				else if(G3_NETWORK_SRVTYPE_CDMA_HDR == p3gstatus->enNWType)
					network_type = "CDMA_HDR";
				else if(G3_NETWORK_SRVTYPE_TDSCDMA == p3gstatus->enNWType)
					network_type = "TDSCDMA";
				else if(G3_NETWORK_SRVTYPE_HSPA_PLUS == p3gstatus->enNWType)
					network_type = "HSPA_PLUS";
				else if(G3_NETWORK_SRVTYPE_LTE_TDD == p3gstatus->enNWType)
					network_type = "LTE_TDD";
				else if(G3_NETWORK_SRVTYPE_LTE_FDD == p3gstatus->enNWType)
					network_type = "LTE_FDD";
				else if(G3_NETWORK_SRVTYPE_EVDO == p3gstatus->enNWType)
					network_type = "EVDO";
				else if(G3_NETWORK_SRVTYPE_AMPS == p3gstatus->enNWType)
					network_type = "AMPS";
				else if(G3_NETWORK_SRVTYPE_EVDO_TDS == p3gstatus->enNWType)
					network_type = "EVDO_TDS";
				else if(G3_NETWORK_SRVTYPE_GPS == p3gstatus->enNWType)
					network_type = "GPS";
				else if(G3_NETWORK_SRVTYPE_GSM_WCDMA == p3gstatus->enNWType)
					network_type = "GSM_WCDMA";
				else if(G3_NETWORK_SRVTYPE_UNICOM == p3gstatus->enNWType)
					network_type = "UNICOM";
				else if(G3_NETWORK_SRVTYPE_UMTS_HSUPA == p3gstatus->enNWType)
					network_type = "UMTS_HSUPA";

				if(G3_SIM_STATUS_VALID == p3gstatus->enSimStatus)
					sim_status = 1;
				
				cJSON_AddNumberToObject(root, "model_status", model_status);/*模块状态*/
				cJSON_AddNumberToObject(root, "sim_status", sim_status);/*SIM卡*/
				cJSON_AddStringToObject(root, "network_type", network_type);/*网络类型*/
				cJSON_AddNumberToObject(root, "reset_times", reset_times);/*复位次数*/
				
				
				
                //lty_ui_progressbar_setvalue(g_psignalbar, p3gstatus->u16SigQual, 31);
                ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
            }
			else
			{
				cJSON_Delete(root);
			}
			
		}break;
		case LTY_MSG_ITSNW_NOTIFY_CONN_STATUS:/*调度平台连接*/
		{
			 stmnw_conn_status_t* pstitsstatus;
            pstitsstatus = (stmnw_conn_status_t*)buf;

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_itsnw_connect_status");
			
            if (CONNECT_STA_DISABLE == pstitsstatus->enStatus){
                //lty_ui_image_changeimage(g_itshandle, 1);
                //lty_ui_image_show(g_itshandle);
                cJSON_AddNumberToObject(root, "status", 1);  /*未连接状态*/
                
            }
            else if (CONNECT_STA_NORMAL == pstitsstatus->enStatus){
                //lty_ui_image_changeimage(g_itshandle, 2);
                //lty_ui_image_show(g_itshandle);
                cJSON_AddNumberToObject(root, "status", 2); /*正常状态*/
            }
            else if (CONNECT_STA_LINK == pstitsstatus->enStatus || CONNECT_STA_WAIT == pstitsstatus->enStatus){
                //lty_ui_image_changeimage(g_itshandle, 3);
                //lty_ui_image_show(g_itshandle);
                cJSON_AddNumberToObject(root, "status", 3); /*正在连接状态*/
            }
            else if (CONNECT_STA_FATAL == pstitsstatus->enStatus){
                //lty_ui_image_changeimage(g_itshandle, 4);
                //lty_ui_image_show(g_itshandle);
                cJSON_AddNumberToObject(root, "status", 1);/*未连接状态*/
            }		
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		}break;
		case LTY_MSG_STMDEV_NOTIFY_CONN_STATUS:/*视频后台连接*/
		{
			stmnw_conn_status_t* pststmstatus;
            pststmstatus = (stmnw_conn_status_t*)buf;

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_stmdev_connect_status");
			
            if (CONNECT_STA_DISABLE == pststmstatus->enStatus){
                //lty_ui_image_changeimage(g_dvrnet, 1);
                //lty_ui_image_show(g_dvrnet);
                cJSON_AddNumberToObject(root, "status", 1);  /*未连接状态*/
            }
            else if (CONNECT_STA_NORMAL == pststmstatus->enStatus){
                //lty_ui_image_changeimage(g_dvrnet, 2);
                //lty_ui_image_show(g_dvrnet);
                cJSON_AddNumberToObject(root, "status", 2); /*正常状态*/
            }
            else if (CONNECT_STA_LINK == pststmstatus->enStatus || CONNECT_STA_WAIT == pststmstatus->enStatus){
                //lty_ui_image_changeimage(g_dvrnet, 3);
                //lty_ui_image_show(g_dvrnet);
                cJSON_AddNumberToObject(root, "status", 3); /*正在连接状态*/
            }
            else if (CONNECT_STA_FATAL == pststmstatus->enStatus){
                //lty_ui_image_changeimage(g_dvrnet, 4);
                //lty_ui_image_show(g_dvrnet);
                cJSON_AddNumberToObject(root, "status", 1);/*未连接状态*/
            }		
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		}break;
		case LTY_MSG_EXCEPTION_NOTIFY_OVERSPEED:
		{
			exception_overspeed_t* poverspeed = (exception_overspeed_t*)buf;

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_exception_overspeed");
			
            if (1 == poverspeed->bIsStart){
                m_warnflag |= FLAG_OVERSPEED;
                //lty_ui_image_addtext(g_warninfo_image, 1, "超速" );
                //lty_ui_image_startblink(g_warnblink, 15);
                //lty_ui_image_show(g_warninfo_image);
                cJSON_AddNumberToObject(root, "status", 1); /*超速*/
            }
            else{
                m_warnflag &= ~FLAG_OVERSPEED;
                //lty_ui_image_deltext(g_warninfo_image, 1);
                //if (0 == m_warnflag){
                  //  lty_ui_image_stopblink(g_warnblink);
                //}
                cJSON_AddNumberToObject(root, "status", 2);/*不超速*/
                
            }
            if (0 == m_warnflag){
                //lty_ui_image_hide(g_warninfo_image);
            }		
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		}break;
		case LTY_MSG_EXCEPTION_NOTIFY_RATE_LIMIT:
		{
			LTY_U32 u32limitspeed = (LTY_U32)buf;
            //char aszlimitspeed[16];
            //sprintf(aszlimitspeed, "%d", u32limitspeed/10);

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_exception_rate_limit");

			cJSON_AddNumberToObject(root, "current_limspeed", u32limitspeed);/*限速*/

            //SetWindowText(GetDlgItem(hWnd, SPEED_LIMIT_ID), aszlimitspeed);
			#if 0
			if (m_viewpg != HWND_INVALID){
                SendMessage(m_viewpg, MSG_DESK_LIMIT_SPEED, wParam, lParam);
            }
            if (m_hdebus != HWND_INVALID){
                SendMessage(m_hdebus, MSG_DESK_LIMIT_SPEED, wParam, lParam);
            }
			#endif
            m_curlimspeed = u32limitspeed;
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			
		}break;
		case LTY_MSG_ITSIO_NOTIFY_IO_STATUS:
		{
			itsio_io_status_t* pstiostatus = (itsio_io_status_t*)buf;

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_itsio_io_status");
			
            if (pstiostatus->u32IOStatus & ITSIO_IOS_FDOOR){/*前门 */
                //lty_ui_image_changeimage(g_fdoorstatus, 2);
                //lty_ui_image_show(g_fdoorstatus);
                cJSON_AddNumberToObject(root, "io_fdoor", 2);
            }
            else{
                //lty_ui_image_changeimage(g_fdoorstatus, 1);
                //lty_ui_image_show(g_fdoorstatus);
                cJSON_AddNumberToObject(root, "io_fdoor", 1);
            }
            if (pstiostatus->u32IOStatus & ITSIO_IOS_MDOOR){/*中门 */
                //lty_ui_image_changeimage(g_mdoorstatus, 2);
                //lty_ui_image_show(g_mdoorstatus);
                cJSON_AddNumberToObject(root, "io_mdoor", 2);
            }
            else{
                //lty_ui_image_changeimage(g_mdoorstatus, 1);
                //lty_ui_image_show(g_mdoorstatus);
                cJSON_AddNumberToObject(root, "io_mdoor", 1);
            }
            if (pstiostatus->u32IOStatus & ITSIO_IOS_BDOOR){/*后门*/
                //lty_ui_image_changeimage(g_bdoorstatus, 2);
                //lty_ui_image_show(g_bdoorstatus);
                cJSON_AddNumberToObject(root, "io_bdoor", 2);
            }
            else{
                //lty_ui_image_changeimage(g_bdoorstatus, 1);
                //lty_ui_image_show(g_bdoorstatus);
                cJSON_AddNumberToObject(root, "io_bdoor", 1);
            }
            if (pstiostatus->u32IOStatus & ITSIO_IOS_BACK){/*倒车*/
				cJSON_AddNumberToObject(root, "io_back_vehicle", 2);/*倒车*/
                picmode.channal = -1;
                switch(g_vdoparam->u8BackcarAv){
                    case L10_AV1:
                        picmode.channal = 0;
                        break;
                    case L10_AV2:
                        picmode.channal = 1;
                        break;
                    case L10_AV3:
                        picmode.channal = 2;
                        break;
                    case L10_AV4:
                        picmode.channal = 3;
                        break;
                    case L10_AV5:
                        picmode.channal = 4;
                        break;
                    case L10_AV6:
                        picmode.channal = 5;
                        break;
                    case L10_AV7:
                        picmode.channal = 6;
                        break;
                    case L10_AV8:
                        picmode.channal = 7;
                        break;
                    default:
                        printf("no back car av.....\n");
                        break;
                }

                if (-1 != picmode.channal){
					cJSON_AddNumberToObject(root, "io_back_vehicle", 1);/*没有倒车*/
					#if 0
					picmode.picmode = VIDEO_PREV_1;
                    itsio_gps_info_t stgpsinfo;
                    if (HWND_INVALID == m_backcar){
                        LOGOUT("will create back car viewpage...\n");
                        m_backcar = createviewpage(&picmode, TRUE, hWnd);
                    }
                    stgpsinfo.u16Speed = m_curspeed;
                    SendMessage(m_backcar, MSG_DESK_LIMIT_SPEED, (WPARAM)m_curlimspeed, 0);
                    SendMessage(m_backcar, MSG_GPS_INFO, (WPARAM)&stgpsinfo, 0);
					#endif
					
                }
            }
            else{
				#if 0
                if (HWND_INVALID != m_backcar){
                    LOGOUT("will destroy back car video\n");
                    SendMessage(m_backcar, MSG_CLOSE, 0, 0);
                }
				#endif
            }	
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		}break;
		case LTY_MSG_NOTIFY_HDD_STATUS:
		case LTY_MSG_NOTIFY_HDD_INIT_STATUS:
		{
			 int status = (int)buf;

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_hdd_status");

			printf("hdd status:%d\n",status);
            if (HDD_NORMAL == status){
                //lty_ui_image_changeimage(g_hddstatus, 3);
                //lty_ui_image_show(g_hddstatus);
                cJSON_AddNumberToObject(root, "hdd_status", 1);
            }
            else if (HDD_NODISK == status){
                //lty_ui_image_changeimage(g_hddstatus, 2);
                //lty_ui_image_show(g_hddstatus);
                cJSON_AddNumberToObject(root, "hdd_status", 2);
            }
            else{
                //lty_ui_image_changeimage(g_hddstatus, 1);
                //lty_ui_image_show(g_hddstatus);
                cJSON_AddNumberToObject(root, "hdd_status", 3);
            }	
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		}break;
	    case LTY_MSG_USERIN_NOTIFY_CARD:
		{
			//char aszcard[64];
            //POP_MSG_S stmsg;
            LTY_U32 u32card = *(LTY_U32*)buf;

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_userin_card");
			cJSON_AddNumberToObject(root, "card_number", u32card);

			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			
			#if 0
			snprintf(aszcard, sizeof(aszcard), "%s%u", cur_Language ? "Card Number:" : "卡号:", u32card);
            stmsg.bconfirm = 0;
            stmsg.breponse = 0;
            stmsg.msg = aszcard;
            //printf("the card is %s.....\n", aszcard);
            SendMessage(m_hDesk, MSG_POPWIN, (WPARAM)&stmsg, 0);	
			#endif
		}break;
		case LTY_MSG_ITS_NOTIFY_SCHEDUING_PLAN:
		{
			//int msgtype = (int)buf;
            struct tm tm;
            time_t t;
            //POP_MSG_S stmsg;
            
            char aszTime[256];
            its_scheduing_plan_t* psche = (its_scheduing_plan_t*)buf;
            //stmsg.bconfirm = 0;
            //stmsg.breponse = 0;
            //stmsg.msg = psche->szText;    
            if(psche->u32DepartTime > 0){
                t = (time_t)psche->u32DepartTime;
            }
            else{
                t = time(NULL);
            }

            if(NULL == localtime_r(&t, &tm)){
                return -1;
            }
			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_its_scheluing_plan");
			
            if (0 == psche->u8Cancel){
                //sprintf(aszTime, "发车时间 %02d月%02d日 %02d:%02d", tm.tm_mon + 1, tm.tm_mday, 
                  //  tm.tm_hour, tm.tm_min);
                sprintf(aszTime, " %02d月%02d日 %02d:%02d", tm.tm_mon + 1, tm.tm_mday, 
                    tm.tm_hour, tm.tm_min);
                if(psche->u32CmdTime)
                {
                	//SetWindowText(GetDlgItem(hWnd, FIRSTLINE_MSG_ID), aszTime);
                	cJSON_AddNumberToObject(root, "cmd_time_status", 1); 
					cJSON_AddStringToObject(root, "msg_content", aszTime);
                }
                else if(psche->szText)
                {
                	//SetWindowText(GetDlgItem(hWnd, FIRSTLINE_MSG_ID), psche->szText);
                	cJSON_AddNumberToObject(root, "cmd_time_status", 2);
					cJSON_AddStringToObject(root, "msg_content", psche->szText);
                }
                    
            }
            else{
				
                //sprintf(aszTime, "计划取消\r%02d:%02d", tm.tm_hour, tm.tm_min);
                sprintf(aszTime, "\r%02d:%02d", tm.tm_hour, tm.tm_min);
                //SetWindowText(GetDlgItem(hWnd, FIRSTLINE_MSG_ID), aszTime);
                cJSON_AddNumberToObject(root, "cmd_time_status", 3);
				cJSON_AddStringToObject(root, "msg_content", psche->szText);
            }
            ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
					
		}break;
	    case LTY_MSG_ITS_NOTIFY_TASK_INFO:
		{
			its_task_info_t* ptask = (its_task_info_t*)buf;
			
			//stmsg.breponse = ptask->u8Respond;
            if (1 == ptask->u8Respond){
                //stmsg.bconfirm = 1;
                //stmsg.u32MsgSN = ptask->u32MsgSN;
            }
            else{
                //stmsg.bconfirm = 0;
                if (0x02 == ptask->u8CmdType){
                    return 0;
                }
            }
			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_its_task_info");

			cJSON_AddStringToObject(root, "msg_content", ptask->szMsgText); 
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			
            //stmsg.msg = ptask->szMsgText;
            //SetWindowText(GetDlgItem(hWnd, FIRSTLINE_MSG_ID), "");
            //CreatePopWin(&stmsg);	
			
		}break;
	    case LTY_MSG_STATION_NOTIFY_INIT:
		{
			station_info(buf,0xf);						
		}break;
	    case LTY_MSG_STATION_NOTIFY_NEXTSTATION:
		{
			station_info(buf,0xf);						
		}break;
	    case LTY_MSG_STATION_NOTIFY_PRESTATION:
		{
			station_info(buf,0xf);						
		}break;
	    case LTY_MSG_STATION_NOTIFY_TYPE:
		{
			LTY_U32 u32notifytype = (LTY_U32)buf;
			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_station_type");



            if (NOTIFYSTATION_AUTO == u32notifytype){
                //SetWindowText(GetDlgItem(hWnd, NOTIFY_STATION_ID),"自动");
                cJSON_AddNumberToObject(root, "status", 1); 
            }
            else{
                //SetWindowText(GetDlgItem(hWnd, NOTIFY_STATION_ID),"手动");
                cJSON_AddNumberToObject(root, "status", 2); 
            }			
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			
		}break;
	    case LTY_MSG_SYSCTRL_NOTIFY_SYS_SHUTDOWN:
		{
			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_shutdown");

			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
									
		}break;
	    case LTY_MSG_EXCEPTION_NOTIFY_ANALOG:
		{
			exception_analog_t* pstexcep = (exception_analog_s*)buf;
			LTY_U32 u32oper = chan;
			cJSON *root = NULL;
			root = cJSON_CreateObject();
			
			if(!root)
			{
				break;
			}

			cJSON_AddStringToObject(root, "msg_type", "system_exception_analog");
			
			switch(u32oper){
				case ANALOG_ACC:
					if (1 == pstexcep->bIsStart){
						//SendMessage(m_hDesk, MSG_EXDEV_EXCEP, EXCEP_ACC, 1); /*acc开*/
						cJSON_AddNumberToObject(root, "acc_status", 1);
					}
					else{
						//SendMessage(m_hDesk, MSG_EXDEV_EXCEP, EXCEP_ACC, 0);/*acc关*/
						cJSON_AddNumberToObject(root, "acc_status", 2);
					}
					break;
				case ANALOG_DEV_DOOR:
					printf("receiv dev door exception message....\n");
					if (1 == pstexcep->bIsStart){
						//SendMessage(m_hDesk, MSG_EXDEV_EXCEP, EXCEP_DEV_DOOR, 1);/*门开*/
						cJSON_AddNumberToObject(root, "door_status", 1);
					}
					else{
						//SendMessage(m_hDesk, MSG_EXDEV_EXCEP, EXCEP_DEV_DOOR, 0);/*门关*/
						cJSON_AddNumberToObject(root, "door_status", 2);
					}
					break;
				default:
					break;
			}
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
									
		}break;
	    case LTY_MSG_ITS_NOTIFY_WORKING_STATUS:
		{
			its_working_status_t* pststatus = (its_working_status_t*)buf;
			cJSON *root = NULL;
			root = cJSON_CreateObject();

			cJSON_AddStringToObject(root, "msg_type", "system_working_status");
			
			if(!root)
			{
				break;
			}
			cJSON_AddNumberToObject(root, "status", pststatus->u8Enable);
			
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
            //SendMessage(m_hDesk, MSG_BUSSTATUS_SET, BUS_RUN_INFO, pststatus->u8Enable);	

			
		}break;
	    case LTY_MSG_ITSSD_NOTIFY_PARA:
		{
			cJSON *root = NULL;
			root = cJSON_CreateObject();
			
			if(!root)
			{
				break;
			}

			cJSON_AddStringToObject(root, "msg_type", "system_itssd_para");
			
			if (SD_PARA_DIRECTION == chan){
                LTY_U8 u8Dir = 0;
                LTY_RESULT ret;
                ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_DIRECTION, (LTY_U32)&u8Dir, 1);
                if(ret != LTY_SUCCESS)
                {
                    ERROUT("Get Cur Direction Fail !!!!!!\n");
                    break;
                }
				cJSON_AddNumberToObject(root, "status",1);
				cJSON_AddNumberToObject(root, "value",u8Dir); //换向
                //SendMessage(m_hDesk, MSG_BUSSTATUS_SET, BUS_DIRECTION, (LPARAM)u8Dir);
                
            }
            else if (SD_PARA_BUS_PLATE == chan){
                LTY_RESULT ret;
                bus_plate_set_t stPlateSet;
                ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_BUS_PLATE, (LTY_U32)&stPlateSet, sizeof(stPlateSet));
                if(LTY_SUCCESS != ret)
                {
                    ERROUT("Get Bus Plate Info Fail, Set DayMileage = 0 !!!!! ret = %d,%s\n",ret,LtyErrStr(ret));           
                    break;
                }
                //SendMessage(m_hDesk, MSG_BUSSTATUS_SET, BUS_DEV_INFO, (LPARAM)&stPlateSet);
                cJSON_AddNumberToObject(root, "status",2);
				cJSON_AddNumberToObject(root, "u32GPRSLineNo",stPlateSet.u32GPRSLineNo);
				cJSON_AddStringToObject(root, "charBusNoBuf",stPlateSet.charBusNoBuf);
				cJSON_AddStringToObject(root, "charBusLicenseBuf",stPlateSet.charBusLicenseBuf);
				cJSON_AddNumberToObject(root, "u8SvrType",stPlateSet.u8SvrType);
				cJSON_AddNumberToObject(root, "u16HowTimeIntoSleep",stPlateSet.u16HowTimeIntoSleep);
				cJSON_AddNumberToObject(root, "u8IntoSleepBeginTime",stPlateSet.u8IntoSleepBeginTime);
				cJSON_AddNumberToObject(root, "u8IntoSleepEndTime",stPlateSet.u8IntoSleepEndTime);
				cJSON_AddNumberToObject(root, "u8CheMenTiShiCount",stPlateSet.u8CheMenTiShiCount);
				cJSON_AddNumberToObject(root, "u8AbortMenSpeedLimit",stPlateSet.u8AbortMenSpeedLimit);
				cJSON_AddNumberToObject(root, "u8ShuDuDisplayType",stPlateSet.u8ShuDuDisplayType);
				cJSON_AddNumberToObject(root, "u8LianXuBoFlag",stPlateSet.u8LianXuBoFlag);
				cJSON_AddNumberToObject(root, "u8ZhanMingChongFuFlag",stPlateSet.u8ZhanMingChongFuFlag);
				cJSON_AddNumberToObject(root, "u8GPSEnFlag",stPlateSet.u8GPSEnFlag);
				cJSON_AddNumberToObject(root, "u8OpMode",stPlateSet.u8OpMode);
				cJSON_AddNumberToObject(root, "u8LanguageFlags",stPlateSet.u8LanguageFlags);
				cJSON_AddNumberToObject(root, "u8WaiYinEnableFlag",stPlateSet.u8WaiYinEnableFlag);
				cJSON_AddNumberToObject(root, "u8WaiYinOffBeginTime",stPlateSet.u8WaiYinOffBeginTime);
				cJSON_AddNumberToObject(root, "u8WaiYinOffEndTime",stPlateSet.u8WaiYinOffEndTime);
				cJSON_AddNumberToObject(root, "u8BaoShiYinEnable",stPlateSet.u8BaoShiYinEnable);
				cJSON_AddNumberToObject(root, "u8BaoShiYinOffBeginTime",stPlateSet.u8BaoShiYinOffBeginTime);
				cJSON_AddNumberToObject(root, "u8BaoShiYinOffEndTime",stPlateSet.u8BaoShiYinOffEndTime);
				
            }
            else if (SD_PARA_DRIVER_STATUS == chan){
                LTY_RESULT ret;
                driver_base_t stbase;//LTY_MSG_ITSSD_SET_PARA
                ret = LtyMessageSend(LTY_MSG_ITSSD_GET_INFO, SD_PARA_DRIVER_STATUS, (LTY_U32)&stbase, sizeof(stbase));
                if(LTY_SUCCESS != ret)
                {
                    ERROUT("Get Bus driver Info Fail, Set DayMileage = 0 !!!!! ret = %d,%s\n",ret,LtyErrStr(ret));           
                    break;
                }
                //SendMessage(m_hDesk, MSG_BUSSTATUS_SET, BUS_DRIVERINFO, (LPARAM)&stbase);
                cJSON_AddNumberToObject(root, "status",3);
				cJSON_AddStringToObject(root, "charDriverNoBuf",stbase.charDriverNoBuf);//驾驶员编号
				cJSON_AddNumberToObject(root, "u8QianTuiFlag",stbase.u8QianTuiFlag);//签到或签退：0（签到），1（签退）
				cJSON_AddNumberToObject(root, "u32OnDutyTime",stbase.u32OnDutyTime);//上班时间：2000年起秒数。全0xff表示无效
				cJSON_AddNumberToObject(root, "u32OffDutyTime",stbase.u32OffDutyTime);//下班时间：2000年起秒数。全0xff表示无效
            }	
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
		}break;
		case LTY_MSG_AV_NOTIFY_VIDEOLOSS:
		{
			//CAM_STATUS_S stcamstatus;
			int i;
			for (i = 0; i < 32;i++){
				if (0 != (chan & (1 << i))){
					chan = i;
					break;
				}
			}

			cJSON *root = NULL;
			root = cJSON_CreateObject();

			if(!root)
			{
				break;
			}
			cJSON_AddStringToObject(root, "msg_type", "system_av_videoloss");

			cJSON_AddNumberToObject(root, "chan", (LTY_S32)chan);
			cJSON_AddNumberToObject(root, "camstatus", (LTY_S32)buf);
			
			#if 0
            stcamstatus.s32chan = (LTY_S32)chan;
            stcamstatus.camstatus = (LTY_S32)buf;
            SendMessage(m_hDesk, MSG_BUSSTATUS_SET, BUS_CAM_INFO, (LPARAM)&stcamstatus);	
			#endif
			ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			
		}break;
        case LTY_MSG_USERIN_NOTIFY_KEYPAD_KEY:{
            LTY_S32 i;
            LTY_U32 u32key = buf;

			cJSON *root = NULL;
			
            for (i = 0; i < (LTY_S32)sizeof(g_astGuiKeyTable);i++){
                if (u32key == g_astGuiKeyTable[i].u32Guikey){
                    break;
                }
            }

           // PostMessage(m_hDesk,MSG_KEYDOWN, g_astGuiKeyTable[i].u32keycode, 0);
		   printf("keycode11:0x%x \n", u32key);
		   printf("keycode:%d \n", g_astGuiKeyTable[i].u32keycode);
		   root = cJSON_CreateObject();
		   if(root )
		   {   
			   cJSON_AddStringToObject(root, "msg_type", "system_key_up");
			   cJSON_AddNumberToObject(root, "key_code", u32key);
			   cJSON_AddNumberToObject(root, "riu_id", 1);
			   //lytSendMsgToUiAppCall
			   ltySendMessageToUi(root,kEnumUiMessageKeypad,NULL);
			   //cJSON_Delete(root);
		   }
        }
        break;
		case LTY_MSG_UPG_NOTIFY_LOCAL_PROGRESS:
		{
			
			if(UPG_SP_PROGRESS == chan)
			{ 
				upg_sp_progress_t *stProg = (upg_sp_progress_t *)buf;
				printf("stProg u32CurPre =%d stProg->u32AllPre=%d \n", stProg->u32CurPre, stProg->u32AllPre);

				gui_system_maintain_get(1,1,2,stProg->u32AllPre,NULL);
				
	
				usleep(600000);

			}
			else if(UPG_SP_FINISH == chan)
			{
	
			}            
			else if(UPG_SP_DONE == chan)
			{
				upg_sp_done_t   *stDone = (upg_sp_done_t *)buf;
				printf("stDone ret =%d\n", stDone->ret);
				if(stDone->u32FileFail > 0)	
					gui_system_maintain_get(1,1,4,0,stDone->pszResult);
				else
					gui_system_maintain_get(1,1,3,0,NULL);
			
				LtyMessagePost(LTY_MSG_UPG_SET_UPGRADE_DONE, UPG_MOD_LOCAL, 0, 0);
				
			}
			
		}break;
		case LTY_MSG_ITSIO_GET_MCU_RUN_STATUS:
		{
			mcu_run_status_t* pststatus = (mcu_run_status_t*)buf;

			//printf("LTY_MSG_ITSIO_GET_MCU_RUN_STATUS 1");
			if(NULL == pststatus)
				break;

			//printf("LTY_MSG_ITSIO_GET_MCU_RUN_STATUS 2");
			gui_run_status_get(5,pststatus);
			
		}break;
		case LTY_MSG_KEY_UP:
		{
			int *pKey = (int *)buf;
			//printf("LTY_MSG_KEY_UP val is %d------\n",*pKey);
			gui_system_key_up(*pKey);
		}break;
		case LTY_MSG_USERIN_NOTIFY_TOUCH:
		{
			
			int touch_result = 0;

			//break;/*测试*/
			
			TOUCH_ATTR_S *pTouch = (TOUCH_ATTR_S *)buf;
			if(NULL == pTouch)
			{
				printf("MsgHandle if(NULL == pTouch)\n");
				break;
			}
			touch_result = lytTouchEvent(pTouch);
			if(0 == touch_result)
			{
				//printf("MsgHandle if(0 == lytTouchEvent(pTouch))\n");
				break;
			}
			extern int ltySendMessge2UiCallback(cJSON *root);
			lytSendMsgToUiAppCall((cJSON *)pTouch,kEnumUiMessageTouch,ltySendMessge2UiCallback);

			#if 0
			cJSON *root = NULL;
			root = cJSON_CreateObject();
		   if(root )
		   {   //printf("MsgHandle %d,%d,%d\n",pTouch->x,pTouch->y,pTouch->up);
			   cJSON_AddStringToObject(root, "msg_type", "system_touch");
			   cJSON_AddNumberToObject(root, "touch_x", pTouch->x);
			   cJSON_AddNumberToObject(root, "touch_y", pTouch->y);
			   cJSON_AddNumberToObject(root, "touch_up", pTouch->up);

			   ltySendMessageToUi(root,kEnumUiMessageTouch, NULL);
			   //cJSON_Delete(root);
			   //delete pTouch;

		   }
		   #endif
		}break;
		case LTY_MSG_QR_PRICE:
		{
			PTQrPrice pstQrPrice = (PTQrPrice)buf;
			gui_main_page_get(pstQrPrice);
		}break;
		case LTY_MSG_SYSCTRL_SET_SYS_SHUTDOWN:
		{
			printf("LTY_MSG_SYSCTRL_SET_SYS_SHUTDOWN\n");
			if(SC_SD_REBOOT == buf)
			{
				cJSON *root = NULL;
				root = cJSON_CreateObject();
				cJSON_AddStringToObject(root, "msg_type", "gui_system_reboot");

				ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
			}
			
		}break;
		case LTY_MSG_ITSSD_NOTIFY_EXPORT_BUSREC_STATUS:
		{
			//LTY_U32 chan, LTY_U32 buf,
			printf("LTY_MSG_ITSSD_NOTIFY_EXPORT_BUSREC_STATUS\n");
			if(chan == EXPORT_UDISK)
			{
				sync();
				//umount(UDISK_MOUNT_PATH);
				if(LTY_SUCCESS == buf)
				{
					gui_system_maintain_get(2,8,3,0,NULL);
				}
				else
				{
					gui_system_maintain_get(2,8,4,0,"faild!");
				}
			}
			
		}break;
        case LTY_MSG_ITSSD_NOTIFY_FORMAT_SUC:{
            gui_system_maintain_get(2,25,3,0,NULL);
        }
        break;
		case LTY_MSG_NET_NOTIFY_G3_NETWORK_STATUS:
	    {
	      printf("LTY_MSG_NET_NOTIFY_G3_NETWORK_STATUS\n");
          g3_network_status_t* pNetworkStatus = (g3_network_status_t*)buf;
		  printf(">>>>>>>>>>>>>>>enStatus[%d]\n", pNetworkStatus->enStatus);
		  gui_g3_network_get(pNetworkStatus->enStatus, E_NETWORK_TYPE_G3);
	    }break;
        case LTY_MSG_TMPNW_NOTIFY_LOGIN_STATUS:
        {
          printf("LTY_MSG_TMPNW_NOTIFY_LOGIN_STATUS chan=%d\n",chan);
		  LTY_U32 errorTtpe = (LTY_U32)buf;
          gui_tmp_network_get(chan, E_TMP_TYPE_LOGIN, errorTtpe);
        }break;
        case LTY_MSG_TMPNW_NOTIFY_CONN1_STATUS:
        {
          gui_tmp_network_get(chan, E_TMP_TYPE_CONN, 0);
        }break;
		case LTY_MSG_XML_FINIS_NOTI:
	    {
			if(LINE_CFG == chan)
			{
			    printf("LTY_MSG_XML_FINIS_NOTI LINE_CFG == chan\n");
		    	gui_system_pos_config_get();
			}
		   
	    }break;
    }
	//printf("MsgHandle end\n");
    return 0;
}
void SysDesk::station_info(LTY_U32 buf,LTY_U32 _type)
{
	PIC_MODE picmode;
	//HDC hdc;
    //SIZE size, smallsize;
    //PLOGFONT pfont;
    //RECT rect;
    BusStation_S* psdstationinfo = (BusStation_S*)buf;
    int flag = (int)_type;

	cJSON *root = NULL;
	root = cJSON_CreateObject();
	if(!root)
   	{
		return;   		
   	}
	cJSON_AddStringToObject(root, "msg_type", "system_notify_station");

	//g_u32BusStationId = psdstationinfo->u32BusStationId;

    if (ENTER_STATION == psdstationinfo->s32Status){
		
		cJSON_AddNumberToObject(root, "status", 0);//进站 
		cJSON_AddStringToObject(root, "current_station", psdstationinfo->pszBusStationName);/*当前站名*/
		cJSON_AddNumberToObject(root, "current_id", psdstationinfo->u32BusStationId);
		cJSON_AddNumberToObject(root, "current_speed", m_curlimspeed);/*当前速度*/
		cJSON_AddNumberToObject(root, "current_limspeed", m_curspeed);/*限速*/
		if (LAST_STATION_FLAG == psdstationinfo->u32flag)
		{
			cJSON_AddNumberToObject(root, "station_type", 1);
			cJSON_AddStringToObject(root, "prev_station", psdstationinfo->pszPrevStationName);
		}
		else if(INTERVAL_STATION_FLAG == psdstationinfo->u32flag)
		{
			cJSON_AddNumberToObject(root, "station_type", 2);
			cJSON_AddStringToObject(root, "prev_station", psdstationinfo->pszPrevStationName);
			cJSON_AddStringToObject(root, "next_station", psdstationinfo->pszNextStationName);
		}
		else if(FIRST_STATION_FLAG == psdstationinfo->u32flag)
		{
			cJSON_AddNumberToObject(root, "station_type", 3);
			cJSON_AddStringToObject(root, "next_station", psdstationinfo->pszNextStationName);
		}

		g_global_info.station_status = 0;
    }
	else
	{
		g_global_info.station_status = 1;
		cJSON_AddNumberToObject(root, "status", 1);//出站 
		//cJSON_AddStringToObject(root, "current_station", psdstationinfo->pszNextStationName);
    }		

	cJSON_AddNumberToObject(root, "flag", flag);
	
    if (ENTER_STATION == psdstationinfo->s32Status){
        if (0xf != flag){
            picmode.channal = -1;
            picmode.picmode = VIDEO_PREV_1;
            switch(g_vdoparam->u8DebusAv){
                case L10_AV1:
                    picmode.channal = 0;
                    break;
                case L10_AV2:
                    picmode.channal = 1;
                    break;
                case L10_AV3:
                    picmode.channal = 2;
                    break;
                case L10_AV4:
                    picmode.channal = 3;
                    break;
                case L10_AV5:
                    picmode.channal = 4;
                    break;
                case L10_AV6:
                    picmode.channal = 5;
                    break;
                case L10_AV7:
                    picmode.channal = 6;
                    break;
                case L10_AV8:
                    picmode.channal = 7;
                    break;
                default:
                    break;
            }
			
            if (-1 != picmode.channal/* && hWnd == GetActiveWindow()*/){
                //LOGOUT("enter station will enter debus view....\n");
				#if 0
				itsio_gps_info_t stgpsinfo;
                if (HWND_INVALID == m_hdebus){
                    m_hdebus = createviewpage(&picmode, TRUE, hWnd);
                }
                stgpsinfo.u16Speed = m_curspeed;
                SendMessage(m_hdebus, MSG_DESK_LIMIT_SPEED, (WPARAM)m_curlimspeed, 0);
                SendMessage(m_hdebus, MSG_GPS_INFO, (WPARAM)&stgpsinfo, 0);
                if (LAST_STATION_FLAG == psdstationinfo->u32flag){
                    SetTimer(hWnd, ID_DEBUS_TIMER, 20 * 100);
                }
				#endif	
            }
			
        }
    }
    else{
		#if 0
        if (HWND_INVALID != m_hdebus){
            SendMessage(m_hdebus, MSG_CLOSE, 0, 0);
        }
        SetWindowText(GetDlgItem(hWnd, BUSSTOP_INFO_ID), "下站");
        SetWindowText(GetDlgItem(hWnd, BUS_STOP_ID), psdstationinfo->pszNextStationName);
		#endif
    }
	ltySendMessageToUi(root,kEnumUiMessageOther, NULL);
}

LTY_RESULT Desk_Msg_Handle(LTY_MSG_TYPE msg, LTY_U32 chan, LTY_U32 buf,
                                      LTY_U32 bufsize, LTY_LPVOID lpPriv)
{
        return ((SysDesk*)lpPriv)->MsgHandle(msg, chan, buf, bufsize, lpPriv);
}

LTY_S32 SysDesk::RegDeskMsg()
{
    LtyMessageRegister(LTY_MSG_USERIN_NOTIFY_KEYPAD_KEY, Desk_Msg_Handle, 0, this);
	LtyMessageRegister(LTY_MSG_USERIN_NOTIFY_TOUCH, Desk_Msg_Handle, 0, this);

	LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_GPS_INFO, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_GPS_EXCEPTION, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_GPS_INVAIL, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITSSD_NOTIFY_INFO, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_NOTIFY_STATION, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_NOTIFY_DIRECTION_CHANGE, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_NOTIFY_OFFLINE, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_NET_NOTIFY_G3_MODEM_STATUS, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITSNW_NOTIFY_CONN_STATUS, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_STMDEV_NOTIFY_CONN_STATUS, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_EXCEPTION_NOTIFY_OVERSPEED, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_EXCEPTION_NOTIFY_RATE_LIMIT, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITSIO_NOTIFY_IO_STATUS, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_NOTIFY_HDD_STATUS,Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_NOTIFY_HDD_INIT_STATUS, Desk_Msg_Handle, 0, this);
    //LtyMessageRegister(LTY_MSG_GUI_POPMSG, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_USERIN_NOTIFY_CARD, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITS_NOTIFY_SCHEDUING_PLAN, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITS_NOTIFY_TASK_INFO, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_STATION_NOTIFY_INIT, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_STATION_NOTIFY_NEXTSTATION, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_STATION_NOTIFY_PRESTATION, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_STATION_NOTIFY_TYPE, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_SYSCTRL_NOTIFY_SYS_SHUTDOWN, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_EXCEPTION_NOTIFY_ANALOG, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITS_NOTIFY_WORKING_STATUS, Desk_Msg_Handle, 0, this);
    //LtyMessageRegister(LTY_MSG_ITSSD_SET_PARA, Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_ITSSD_NOTIFY_PARA, Desk_Msg_Handle, 0, this);
	
	LtyMessageRegister(LTY_MSG_AV_NOTIFY_VIDEOLOSS, Desk_Msg_Handle, 0, this);

	LtyMessageRegister(LTY_MSG_UPG_NOTIFY_LOCAL_PROGRESS,  Desk_Msg_Handle, 0, this);
	LtyMessageRegister(LTY_MSG_ITSIO_GET_MCU_RUN_STATUS,  Desk_Msg_Handle, 0, this);

	LtyMessageRegister(LTY_MSG_KEY_UP,  Desk_Msg_Handle, 0, this);
    LtyMessageRegister(LTY_MSG_QR_PRICE, Desk_Msg_Handle, 0, this);

	LtyMessageRegister(LTY_MSG_SYSCTRL_SET_SYS_SHUTDOWN,Desk_Msg_Handle,0,this);
	LtyMessageRegister(LTY_MSG_ITSSD_NOTIFY_EXPORT_BUSREC_STATUS, Desk_Msg_Handle,0,this);
    LtyMessageRegister(LTY_MSG_ITSSD_NOTIFY_FORMAT_SUC, Desk_Msg_Handle, 0, this);
	LtyMessageRegister(LTY_MSG_NET_NOTIFY_G3_NETWORK_STATUS, Desk_Msg_Handle, 0, this);
	LtyMessageRegister(LTY_MSG_TMPNW_NOTIFY_LOGIN_STATUS, Desk_Msg_Handle, 0, this);
	LtyMessageRegister(LTY_MSG_TMPNW_NOTIFY_CONN1_STATUS, Desk_Msg_Handle, 0, this);
	LtyMessageRegister(LTY_MSG_XML_FINIS_NOTI, Desk_Msg_Handle, 0, this);

	
	
	memset(&g_global_info,0,sizeof(global_info_t));
	
    return 0;
}


