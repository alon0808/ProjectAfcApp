#ifndef LTYCOMMONDEFINE_H
#define LTYCOMMONDEFINE_H




enum
{
    kEnumNetworkNone,
    kEnumNetwork2G,
    kEnumNetwork3G,
    kEnumNetwork4G
};
//网络信号
enum
{
    kEnumNetworkSignalNone = 1,
    kEnumNetworkSignal1,
    kEnumNetworkSignal2,
    kEnumNetworkSignal3,
    kEnumNetworkSignal4,
    kEnumNetworkSignalError
};
//GPS状态
enum
{
    kEnumGpsNormal,
    kEnumGpsAbnormal
};
//ACC状态
enum
{
    kEnumAcc1,
    kEnumAcc2l
};
//设备门（机器上）
enum
{
    kEnumDeviceDoorOpen,
    kEnumDeviceDoorClose
};
//设备异常（机器上）
enum
{
    kEnumDeviceNormal,
    kEnumDeviceAbnormal
};

//硬盘状态
enum
{
    kEnumHardDiskNormal,
    kEnumHardDiskAbnormal1,
    kEnumHardDiskAbnormal2
};

//平台连接状态
enum
{
    kEnumPlatformNormal,
    kEnumPlatformAbnormal1,
    kEnumPlatformAbnormal2,
    kEnumPlatformConnect
};


//G3连接状态
enum NETWORK_STATUS
{
    kEnumNetworkNormal,
    kEnumNetworkFailed,
    kEnumNetworkConnecting,
    kEnumStatusOther

};

//TMP连接状态
enum TMP_STATUS
{
    kEnumTMP_DOING = 0,
    kEnumTMP_SUCCESS,
    kEnumTMP_FAILURE,
    kEnumTMP_SNDOING = 3,
    kEnumTMP_SNSUCCESS = 4,
    kEnumTMP_SNFAILURE = 5, 
};

//Network 类型
enum  NETWORK_TYPE
{
    kEnumNetworkG3 = 1,
    kEnumNetworkTMP,
    kEnumTypeOther
};


enum G3_NETWORK_STATUS
{
    G3_NETWORK_STATUS_FORBIDDEN = 0,
    G3_NETWORK_STATUS_WAITTING,
    G3_NETWORK_STATUS_DIALING,
    G3_NETWORK_STATUS_NORMAL,
    G3_NETWORK_STATUS_STOPPING
};

//TMP 类型
enum TMP_TYPE
{
    kEnumTMPLogin = 1,
    kEnumTMPConnect,
    kEnumTMPOther
};


//消息状态
enum
{
    kEnumMessageStatus1,
    kEnumMessageStatus2
};

//车门的状态
enum
{
    kEnumDoorOpen,
    kEnumDoorClose
};

//超速报警状态
enum
{
    kEnumSpeedAlertStart,
    kEnumSpeedAlertStop
};

class CEventData
{
public:
    CEventData()
    {
        m_i_data = 0;
        m_str_data = "";
        m_param1 = 0;
    }
    int m_i_data;
    QString m_str_data;
    int m_param1;//扩展用
};

//车的状态
enum
{
    kEnumVehicleStationNull,    //没有车
    kEnumVehicleStationFront,   //靠近站
    kEnumVehicleStationCurrent, //到站
    kEnumVehicleStationBack    //离开站
};

enum
{
    kEnumStationIn,
    kEnumStationOut,
    kEnumStationStart,
    kEnumStationEnd
};

enum
{
    kEnumSdNormalNormal,
    kEnumSdAbnormal1,
    kEnumError

};

enum
{
    //二维码
    kEnumQrStatusNormal,
    kEnumQrStatusSuccess,
    kEnumQrStatusFail,
    kEnumQrStatusabNormal,
    kEnumQrStatusInvalid,
    //刷卡
    kEnumCardStatusSuccess,
    kEnumCardStatusFail,
    kEnumCardStatusAbnormal,
    kEnumCardStatusFlow,
    kEnumCardStatusMessage
};
//  收费模式
enum
{
   e_Charge_Mode_One_Ticket = 1,
   e_Charge_Mode_Block_Charge,
   e_Charge_Mode_Other
};

// Dev 模式
enum
{
   e_Dev_Mode_One_Mac = 1,
   e_Dev_Mode_Main_Mac,
   e_Dev_Mode_Slave_Mac
};

enum
{
    //头部显示
    kEnumMainPageSignal1,//SIM卡　2G 3G 4G
    kEnumMainPageSignal2, //SIM卡信号强弱
    kEnumMainPageGpsNumber,//GPS个数
    kEnumMainPageGpsStatus,//GPS状态
    kEnumMainPageAccStatus,//ACC状态
    kEnumMainPageDeviceDoorStatus, //设备门（机器上）状态
    kEnumMainPageDeviceAbnorma,//设备异常（机器上）
    kEnumMainPageHardDiskStatus,//硬盘状态
    kEnumMainPagePlatformStatus,//平台连接状态 (调度平台)
    kEnumMainPageTMPPlatformStatus,//平台连接状态 (调度平台)
    kEnumMainPagePlatformStatus1,//平台连接状态 (视频服务器)
    kEnumMainPageMessageStatus, //消息状态
    kEnumMainPageSdStatus,
    //中间左边显示
    kEnumMainPageVehicleModel, //车的型号
    kEnumMainPageVehicleUpDown, //车是上行或下行
    kEnumMainPageVehicleStatus, //车的状态(运营中)
    kEnumMainPageLeftDistance, //左边车的距离
    kEnumMainPageRightDistance,//右边车的距离
    kEnumMainPageLeftDistance1, //左边车的距离(泰国)
    kEnumMainPageRightDistance1,//右边车的距离(泰国)
    kEnumMainPageUpdateStationName,//当前站名
    kEnumMainPageUpdateStationName1,//当前站名
    kEnumMainPageCurrentStationName,//当前站名
    kEnumMainPageCurrentStationName1,//当前站名
    kEnumMainPageCurrentMessage, //当前短信信息
    kEnumMainPageStartTime,      //发车时间
    kEnumMainPageUpVehicleNumber,    //上车人数
    kEnumMainPageDownVehicleNumber,  //下车人数
    kEnumMainPageWithinVehicleNumber,//车内人数
    kEnumMainPageDriverName,     //驾驶员名字
    kEnumMainPageVehicleNumber,//车辆编号

    kEnumMainPageFrontVehicle,  //前车的距离
    kEnumMainPageBackVehicle,//后车的距离

    kEnumMainPageFrontStation1,     //前一站1
    kEnumMainPageFrontStation2,     //前一站2
    kEnumMainPageBackStation1,      //前一站1
    kEnumMainPageBackStation2,      //前一站2

    kEnumMainPageVehicle1,    //前车
    kEnumMainPageVehicle2,    //当前车
    kEnumMainPageVehicle3,    //后车

    kEnumMainPageStationStatus,//到站状态

    //中间右边显示
    kEnumMainPageLimitSpeed,    //限速
    kEnumMainPageCurrentSpeed,  //当前速度
    kEnumMainPageSpeedAlert,    //超速警报
    kEnumMainPageFrontDoor,     //前门
    kEnumMainPageBackDoor,       //后门
    //////////////////P2
    kEnumMainPageAdvert,          //（广告图片）左右显示
    kEnumMainPageVerificationRrStatus,
    kEnumMainPageLine,
    kEnumMainPageChargeType,    //票价类型: 一票制 or 分段收费
    kEnumMainPageAmount,        //金额
    kEnumMainPageBalance,       //余额
    kEnumMainPageArriveStation, //到站
    kEnumMainPageFromStatus,    //从机状态
    kEnumMainPageFromMachine    //当前是从机
};

//系统管理－－导航栏右边的宽高
#define NAVIGATE_RIGHT_WIDGET_WIDTH   1024 - 250
#define NAVIGATE_RIGHT_WIDGET_HEIGHT   768 - 82


#define MAIN_HEAD_WIDTH     1024
#define MAIN_NAVIGATE_LEFT       250
#define MAIN_HEAD_HEIGHT    82

#define COM_CLOSEBTN_RIGHT_MARGIN	6

#define MSGBOX_BTN_MARGIN	28
#define MSGBOX_ICON_WIDTH	46
#define MSGBOX_ICON_HEIGHT	46
#define MSGBOX_BTN_SPACE	14

#define COM_MSGBOX_WIDTH	432
#define COM_MSGBOX_HEIGHT	212

#define BASE_COLOR2					"#4dbeff"


#define COM_SHADOWWIDGET_MARGIN	5
#define COM_SHADOWWIDGET_MARGIN_DOUBLE	10

enum
{
    E_LOGICAL_NORMAL,
    E_LOGICAL_UP,
    E_LOGICAL_DOWN

};

#define SORT_ICON_WIDTH		24
#define SORT_ICON_HEIGHT	24

#define SORT_ICON_RES_UP		":/Img/res/common/ic_normal/sortUp.png"
#define SORT_ICON_RES_DOWN		":/Img/res/common/ic_normal/sortDown.png"


#define MODELDATA_EVENT_BOOT  0
#define MODELDATA_EVENT_STOP  1
#define MODELDATA_EVENT_EXIT  2
#define MODELMOUSE_EVENT_RIGHT_CLICK	3
#define MODELDATA_EVENT_MORE    4
#define MODELDATA_EVENT_SEND    5

#define NAVIGATE_LEFT    46
#define NAVIGATE_RIGHT   46

#define ROW_HEIGHT 88

#define COMBOBOX_WIDTH  230//150
#define COMBOBOX_HEIGHT 65

#define EDIT_WIDTH  230
#define EDIT_HEIGHT 65

#define MARGIN_RIGHT    20

#define TEXT_FONT 26

#define NAVIGATE_WIDTH 220
#define NAVIGATE_HEIGHT 80

#define VOICE_MAX    15

////////////////////////////////////////////////////////////////////////////////UI通讯消息

#define  UDISK_PATH    "/mnt/udisk"
#if defined(APPLICATION_TYPE_D2)
#define  UPGRADE_PATH  "/mnt/udisk/ltyd2"
#elif defined(APPLICATION_TYPE_P2)
#define  UPGRADE_PATH  "/mnt/udisk/ltyp2"
#else
#define  UPGRADE_PATH  "/mnt/udisk/ltyappq"
#endif


#define UDISK_MOUNT_NOD "/dev/diskUSB1"
enum
{//弹出对话框，用户点确定或取消记录
    kEnumPopDialogNull,
    kEnumPopDialogClickOk,
    kEnumPopDialogClickCancel
};

#define KEYPAD_KEY7_F0      0x50
#define KEYPAD_KEY7_F1      0x51
#define KEYPAD_KEY7_F2      0x52
#define KEYPAD_KEY7_F3      0x53
#define KEYPAD_KEY7_F4      0x54
#define KEYPAD_KEY7_F5      0x55
#define KEYPAD_KEY7_F6      0x56
#define KEYPAD_KEY7_F7      0x57
#define KEYPAD_KEY7_F8      0x58
#define KEYPAD_KEY7_F9      0x59
#define KEYPAD_KEY7_PLAY    0x5a
#define KEYPAD_KEY7_REPLAY  0x5b
#define KEYPAD_KEY7_MENU    0x5c
#define KEYPAD_KEY7_EXIT    0x5d
#define KEYPAD_KEY7_FUN     0x5e
#define KEYPAD_KEY7_UP      0x5f
#define KEYPAD_KEY7_LEFT    0x60
#define KEYPAD_KEY7_RIGHT   0x61
#define KEYPAD_KEY7_DOWN    0x62



#endif // LTYCOMMONDEFINE_H






