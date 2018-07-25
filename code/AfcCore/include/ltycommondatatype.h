/*************************************************************
  File name:
    rmcommondatatype.h
  Author:

  Date:
  Description:
    公共数据类型
*************************************************************/
#ifndef _LTYCOMMONDATATYPE_H_
#define _LTYCOMMONDATATYPE_H_
#include <stdlib.h>
#include <stdio.h>
//#include "rmcommondefine.h"

//视频制式
typedef enum {
    VIDEO_SYS_PAL = 0,/*PAL*/
    VIDEO_SYS_NTSC,/*NTSC*/
    VIDEO_SYS_INVALID
}video_system_e;

//视频分辨率
typedef enum {
    VIDEO_RES_CIF = 0,
    VIDEO_RES_D1,
    VIDEO_RES_720P,
    VIDEO_RES_1080P,
    VIDEO_RES_INVALID
}video_resolution_e;

//视频帧率
typedef enum {
    VIDEO_FR_25 = 0,
    VIDEO_FR_20,
    VIDEO_FR_12,
    VIDEO_FR_6,
    VIDEO_FR_3,
    VIDEO_FR_1,
    VIDEO_FR_INVALID
}video_framerate_e;

//视频画质
typedef enum {
    VIDEO_QUA_NORMAL = 0,
    VIDEO_QUA_GOOD,
    VIDEO_QUA_PERFECT,
    VIDEO_QUA_INVALID
}video_quality_e;

//视频码率
typedef enum {
    VIDEO_BR_1024K  = 0,
    VIDEO_BR_1536K,
    VIDEO_BR_768K,
    VIDEO_BR_512K,
    VIDEO_BR_384K,
    VIDEO_BR_256K,
    VIDEO_BR_INVALID,
}video_bitrate_e;

//视频预览模式
typedef enum {
    VIDEO_PREV_4 = 0,
    VIDEO_PREV_1,
    VIDEO_PREV_2,
    VIDEO_PREV_9,
    VIDEO_PREV_16,
	VIDEO_PREV_AUTO,
    VIDEO_PREV_INVALID
}video_preview_e;

//音频编码格式
typedef enum {
    AUDIO_TYPE_PCM   = 0,
    AUDIO_TYPE_G726  = 1,
    AUDIO_TYPE_ADPCM = 2,
    AUDIO_TYPE_G711  = 3,
    AUDIO_TYPE_G711U = 4,
    AUDIO_TYPE_INVALID
}audio_type_e;

typedef enum {
    AUDIO_ATTR_8K_16BIT = 0,
    AUDIO_ATTR_16K_16BIT = 1,
    AUDIO_ATTR_INVALID
}audio_attr_e;
/*
typedef enum {
    AUDIO_CHN_INSIDE = 0,
    AUDIO_CHN_OUTSIDE,
    AUDIO_CHN_KEYPAD,
    AUDIO_CHN_INVALID
}audio_channel_e;
*/

#define SPEAKER_CHN_MAX 3
typedef enum {
    SPEAKER_CHN_INSIDE = 0,
    SPEAKER_CHN_OUTSIDE,
    SPEAKER_CHN_KEYPAD,
    SPEAKER_CHN_INVALID
}speaker_channel_e;

//视频OSD位置
typedef enum {
    OSD_POS_DISABLE = 0,
    OSD_POS_TOPLEFT,
    OSD_POS_BOTTOMLEFT,
    OSD_POS_TOPRIGHT,
    OSD_POS_BOTTOMRIGHT,
    OSD_POS_INVALID
}osd_position_e;

typedef enum {
    CONNECT_STA_DISABLE = 0,  //禁止
    CONNECT_STA_FATAL,        //严重异常
    CONNECT_STA_WAIT,         //等待
    CONNECT_STA_LINK,         //建立连接
    CONNECT_STA_LOGIN,        //登录
    CONNECT_STA_NORMAL,       //正常
    CONNECT_STA_INVALID
}connect_status_e;

typedef enum _recordpackettime_{
    RPT_15, RPT_30, RPT_45, RPT_60
}recordpackettime_t;

typedef enum _datemode_{/*日期模式*/
    DM_LINE_YMD,/*2007-08-21，即年月日*/
    DM_BIAS_MDY,/*08/21/2007，即月日年*/
}datemode_t;


typedef enum _week_{/*星期*/
    WK_SUNDAY,
    WK_MONDAY,
    WK_TUESDAY,
    WK_WEDNESDAY,
    WK_THURSDAY,
    WK_FRIDAY,
    WK_SATURDAY
}week_t;

typedef struct _datetime_{/*日期*/
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    week_t week;
}datetime_t;


typedef struct _rectanglearea_{/*矩形*/
    int x, y, w, h;
}rectanglearea_t;

typedef enum _guilanguage_{/*语言*/
    GL_SIMPLECHINESE,/*简体中文*/
    GL_ENGLISH,/*英文*/
    GL_GREMAN,/*简体中文*/
    GL_JAPANESE,/*英文*/
    GL_ITALY,/*简体中文*/
    GL_RUSSIAN/*英文*/
}guilanguage_t;


/***************************************************
    图片类型
 **************************************************/
typedef enum _picturetype_{
    PT_BITMAP,
    PT_GIF,
    PT_UNKNOWN
}picturetype_t;



typedef enum _timemode_{/*时间模式*/
    TM_HHMM,/*时分*/
    TM_HHMMSS,/*时分秒*/
    TM_12_HHMM,/*12 小时制 时分*/
    TM_12_HHMMSS/*12 小时制 时分*/
}timemode_t;

typedef enum _decodermode_{/*解码模式*/
    DM_ONECHN,/*一个通道解码*/
    DM_FOURCHN,/*四个通道解码*/
    DM_INVALID
}decodermode_t;

typedef enum pipmode{
    PM_TWO,
    PM_THREE,
    PM_INVLAID
}pipmode_t;

typedef enum _recordtype_{
    RT_NORMAL,
    RT_ALARM,
    RT_INVALID
}recordtype_t;

typedef enum _searchreplayfiletype_{
    TM_30MINUTE,
    TM_10MINUTE,
    TM_1MINUTE,
    TM_SECOND
}searchreplayfiletype_t;

typedef enum av_frametype_s{
    AV_FT_IFRAME,  //视频I帧
    AV_FT_PFRAME,  //视频P帧
    AV_FT_AFRAME,  //音频帧
    AV_FT_TFRAME,  //对讲帧
    AV_FT_INVALID
}av_frametype_e;

typedef struct _frameinfo_{
    unsigned long long pts;
    int FrameLen;
    av_frametype_e type;
}frameinfo_t;


typedef struct _list_{
    char *data;
    _list_ *pNext;
}list_t;


typedef struct _recfileinfo_{
    video_resolution_e res;
}recfileinfo_t;


typedef enum _playrecordstate_{
    PRS_NORMALPLAY,
    PRS_FASTFORWARD2X,
    PRS_FASTFORWARD4X,
    PRS_FASTFORWARD8X,
    PRS_FASTBACK2X,
    PRS_FASTBACK4X,
    PRS_FASTBACK8X,
    PRS_FRAMEPLAY,
    PRS_PAUSE,
    PRS_SLOWPLAY2X,
    PRS_SLOWPLAY4X,
    PRS_SLOWPLAY8X,
    PRS_FASTFORWARD16X,
    PRS_FASTBACK16X,
    PRS_INVALID
}playrecordstate_t;


typedef enum _hddstate_{
    HDDS_NONE,
    HDDS_UNFORMAT,
    HDDS_OK,
    HDDS_BAD,
    HDDS_INVALID
}hddstate_t;

typedef enum _networkstate_{
    NS_GETTING,/*正在获取*/
    NS_OK,/*已经成功*/
    NS_INVALID
}networkstate_t;
enum IPC_DEFINE_TYPE
{
IPC_HAIKANG=0,
IPC_JY=1,
IPC_DAHUA=2,
IPC_HUANGHE_PRIVATE=3,
IPC_SONY_PRIVATE=4,
IPC_PANASONIC=5,
IPC_HAIXING=6,
IPC_SS=7,
IPC_HUAYU=8,
IPC_OKS=9,
IPC_XUNMEI=10,
IPC_CHAOLIU=11,
IPC_SAMSUMG_HI=12,
IPC_SAMSUMG=13,
IPC_PUSHUNDA=14,
IPC_ZHONGKONG=15,
IPC_HONGDI=16,
IPC_SIP=17,
IPC_ONVIF=18,    

IPC_JieGao=131,   
IPC_LangChi=132,
IPC_HongDy_Qiu=133,
IPC_Anbong=134,
IPC_HIK=135,
IPC_HUANTAI_TST=136,
IPC_kanwe_tonganshun=137,
IPC_boli=138,
IPC_grand_720p=139,
IPC_HengYe=140,
IPC_JINGYANG=141,   

IPC_JieGao1080=161,   
IPC_SONY_RTSP=166,
IPC_AXIS=167,
IPC_BOSCH=168,
IPC_MINGJING=169,
IPC_HH_RTSP=170,
IPC_CVBS=171
};

//void GetNetworkState(networkstate_t *pState, void *para)

#define LED_RECORD  0
#define LED_ALARM   1
#define LED_ERROR   2
#define LED_HDDFULL 3

//typedef bool (*GetDateTimeFuncPtr)(datetime_t *pDateTime);
//typedef bool (*SearchRecFileByTimePointFuncPtr)(datetime_t *pDateTime, channelid_t channel, char *pFileName, unsigned long *pOffset);

#define MACHINE_SERIAL_LEN 10
#define EDGE_ALIGN(x, align)  ((((x)-1)/(align)+1)*(align))

/* 喇叭定义*/
/*
#define TOTAL_SPEAKERS  3
typedef enum
{
    SPEAKER_INDOOR = 0,
    SPEAKER_OUT,
    SPEAKER_DRIVER
} LTY_SPEAKERS;
*/
#endif/* _RMCOMMONDATATYPE_H_ */


