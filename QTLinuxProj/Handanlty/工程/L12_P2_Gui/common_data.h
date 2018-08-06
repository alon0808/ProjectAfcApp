#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <QWidget>
#include "protocol.h"

#include "class/common/define_screen.h"

// 聚焦的当前按钮
typedef enum
{
    evtfNone,
    evtfBoot,	// 启动	按钮
    evtfStop,	// 暂停	按钮
    evtfExit,	// 删除	按钮
    evtfOpenDir,
    evtfUninstall,
    evtfMoveMemory,	//移至SD卡或移至内存 按钮
    evtfMoveSDcard,
    evtfMoveUpdate,	//升级
    evtfMoveIgnore,	//忽略
    evtfMoveDetail,	//详情
    evtfMsgCopy,	//复制
    evtfMsgTransmit,//转发
    evtfMsgDelete,	//删除
    evtfEdit,		//编辑
    evtfSendMsg,	//发短信
    evtfDelete,		//删除
    evtfPlay,
    evtfFlash,
    evtfMore,        //更多
    evtfSend        //发送

}eViewTableFocus;

// 鼠标事件
typedef enum
{
    evtmeNone,
    evtmeUp,
    evtmeMove,
    evtmePress,
    evtmeDouble
}eViewTableMouseEvent;

enum eNetworktype
{
   NETWORK_TYPE_G3,
   NETWORK_TYPE_TMP
};


enum E_G3_SIM_STATUS
{
    E_G3_SIM_STATUS_UNKNOWN = 0,
    E_G3_SIM_STATUS_VALID,
    E_G3_SIM_STATUS_INVALID
};

// 当前状态，用于 正在下载
typedef enum
{
    evtsStop,	// 停止/暂停
    evtsBoot,	// 启动/下载中
    evtsMoveMemory,
    evtsMoveSDcard

}eViewTableState;

typedef struct _sMouseClickParam
{
    eViewTableMouseEvent m_MouseEvent;
    eViewTableFocus m_Focus;
    int m_row;
    int m_index;  //在一行中第几个图片
    int m_type;	  //0:图片， 1：显示数字
}sMouseClickParam;

class CVideotape
{
public:
    ~CVideotape(){}
    CVideotape(){}

public:
    int m_number;
    QString m_start_time;
    QString m_end_time;
    QString m_duration;
    int m_passageway;
    int m_enable;


};
//预设事件
class CPresetEvent
{
public:
    CPresetEvent(){}
    ~CPresetEvent(){}
public:
   int      m_number;//序号
   QString  m_content;//内容
   int      m_event_code;//事件代码
   QString  m_shortcut_key;  //快捷键

};
//重要短信　普通短信
class CImportantMessage
{
public:
    CImportantMessage()
    {
        m_calculate = false;
        m_more = false;
        m_status = 0;
    }
    ~CImportantMessage(){}
public:
    int m_number;        //序号
    QString m_recv_time; //接收时间
    QString m_content;   //实际的数据
    QString m_show_content;
    bool m_calculate;   //
    bool m_more;        //显示更多按钮
    int m_status;


};

//行车计划
class CVehiclePlan
{
public:
    CVehiclePlan(){}
    ~CVehiclePlan(){}
public:
    int m_number;        //序号
    QString m_recv_time; //接收时间
    QString m_start_time;//发车时间
    QString m_attribute; //属性

};

//电话簿
class CCallBook
{
public:
    CCallBook(){}
    ~CCallBook(){}
public:
    QString m_name;
    QString m_number;
};
//模拟摄像机
class CSimulation
{
public:
    CSimulation(){}
    ~CSimulation(){}
public:
    int m_number;           //序号
    QString m_passageway;   //通道
    QString m_av1;          //AV1通道
    QString m_av2;          //AV2通道
    QString m_attribute;    //属性
};
//网络状态
class CNetworkStatus
{
public:
    CNetworkStatus(){}
    ~CNetworkStatus(){}
public:
    int m_number;           //序号
    QString m_passageway;   //通道
    QString m_ip;           //IP地址
    QString m_mac;          //MAC地址
    QString m_attribute;    //属性
};

class CDay
{
public:
    CDay(){
        m_normal = false;
        m_police = false;
        m_current_month = true;
    }
    char m_number;
    bool m_normal;  //正常录像
    bool m_police;  //报警录像
    bool m_current_month;// 日/当月
};

//日期选择
class CDateSelect
{
public:
    CDateSelect(){}
    ~CDateSelect(){}
public:
    QList<CDay> m_list;

};

//查询结果
class CQueryResult
{
public:
    CQueryResult(){
        data_end = false;
    }
    ~CQueryResult(){}
public:

    int m_number;  //序号　从1开始
    QString m_date;//录像时间
    int m_time;//时长(S)
    QString m_passageway;//通道
    char m_position; //位置  0:硬盘 1:EMMC 2:SD卡
    char m_passageway_value; //通道位置 0:全部 1:CAM1-1 2:CAM1-2 3:CAM2-1 4:CAM2-2 5:CAM3-1 6:CAM3-2 7:CAM4-1 8:CAM4-2
                       //9:PON1 10:PON2 11:PON3 12:PON4 13:PON5 14:PON6 15:PON7 16:PON8 17:PON9 18:PON10
    qint64 m_size; //视频文件大小
    QString m_path;//视频文件路径
    bool data_end;

};
class CSoftwareUpgrade
{
public:
    CSoftwareUpgrade(){}
    ~CSoftwareUpgrade(){}
public:
    int m_number;
    QString m_file_name;


};

class CWifiSearch
{
public:
    CWifiSearch()
    {
        m_number = 0;
        m_lock = true;
    }
    ~CWifiSearch(){}
public:
    int m_number;   //序号 从1开始
    QString m_name;//SSID名称
    QString m_signal;//信号强度　-70Db
    bool m_lock;   //0:已连接 1:锁


};

//Q_DECLARE_METATYPE(CVideotape)

class CSDCardInfo
{
public:
    CSDCardInfo(){}
    QString m_line_name;        /*线路名*/
    QString m_driver_number;    /*驾驶员编号*/
    char m_up_down_flag;        //1:上行 2:下行
    QString m_bus_number;       /*车辆编号*/
    QString m_bus_license;      /*车牌号*/
    char m_auto_flag;           //1:手动  2:自动
    char m_operation_flag;      // 1:非营运 2:营运
    int m_current_limspeed;     //限速
    char m_sd_status;           /*SD卡状态 1:正常 0:异常*/
    QString m_first_station;
    QString m_last_station;
};

class CStationInfo
{
public:
    CStationInfo()
    {
        m_status = 0;              // 0:进站 1:出站
        m_prev_station = "";
        m_next_station = "";
        m_current_station = "";  /*当前站名*/
        m_current_id = 0;
        m_current_speed = 0;        /*当前速度*/
        m_current_limspeed = 0;     /*限速*/
        m_station_type = 0;        //1:最后一站　2:中间站 3:第一站
        m_flag = 0;                 //0:弹出报站
    }
    char m_status;              // 0:进站 1:出站
    QString m_prev_station;
    QString m_next_station;
    QString m_current_station;  /*当前站名*/
    int m_current_id;
    int m_current_speed;        /*当前速度*/
    int m_current_limspeed;     /*限速*/
    char m_station_type;        //1:最后一站　2:中间站 3:第一站
    int m_flag;                 //0:弹出报站

};

class CIoInfo
{
public:
    CIoInfo(){}
    char m_front_door;      //2:前门开 1:前门关
    char m_middle_door;     //2:中门开 1:中门关
    char m_back_door;       //2:后门开 1:后门关
    char m_back_vehicle;    //2:倒车　1:没有倒车

    char m_acc_status; // 1:acc开 2:acc关
    char m_door_status; //1:设备门开 2:1:设备门关
};

class CScheluingPlan
{
public:
    CScheluingPlan(){
        status = 0;
        m_msg_content = "";
        m_task_content = "";
    }
    char status;
    QString m_msg_content;
    QString m_task_content;
};

class CVideoLoss
{
public:
    CVideoLoss(){}
    int m_chan;
    int m_camstatus;

};

class CDriverInfo
{
public:
    CDriverInfo(){}
    QString m_driver_number;//驾驶员编号
    char m_qian_tui;//签到或签退：0（签到），1（签退）
    int m_on_duty_time;//上班时间
    int m_off_duty_time;//下班时间

};

class CBusPlate
{
public:
    CBusPlate(){}
    int m_gprs_line_no;//GPRS线路号(线路编号)
    QString m_bus_number;//车辆编号
    QString m_bus_license;//车辆牌号
    char svr_type;//0:非运营，1:运营
    int HowTimeIntoSleep;//休眠判据1：车辆停留多长时间后，进入休眠状态（秒）,0(此参数不使用)
    char IntoSleepBeginTime;//休眠判据2：车辆进入休眠状态的起始时间（时，0~23）
    char IntoSleepEndTime;//休眠判据2：车辆退出休眠状态的终止时间（时，0~24）
    char CheMenTiShiCount;//车门未关提示次数，即异常开门报警次数：0（不报警），255（连续报警），其它（报警次数）
    char AbortMenSpeedLimit;//异常开门报警的限速：允许的最小速度限值，单位km/h
    char ShuDuDisplayType;//速度显示方式
    char LianXuBoFlag;//连续报站使能：0(关闭)、1(允许中断)，使能或禁止中断还没完成的语音
    char ZhanMingChongFuFlag;//站名重复使能：0(关闭)、1(开启)，使能或禁止站名音重复2次
    char GPSEnFlag;//自动报站使能：0(关闭)、1(开启)
    char OpMode;//报站方式：0(两键式预报与报站)、1(一键式只预报)、2(一键式只报站)、3(一键式预报和报站)
    char LanguageFlags;//报站语言：。0(不报站)、1(普通话)、2(英语)、3(普通话＋英语)、4(方言)、5(普通话＋方言)、6(英语＋方言)、7(普通话＋英语＋方言)
    char WaiYinEnableFlag;//外音开关：0（禁止外音播报），1（开启外音播报）
    char WaiYinOffBeginTime;//外音关闭起始时间：外音关闭起始时间（时,0~23）
    char WaiYinOffEndTime;//外音关闭终止时间外音关闭终止时间（时,0~24）
    char BaoShiYinEnable;//报时音使能：0（禁止报时音播报），1（开启报时音播报）
    char BaoShiYinOffBeginTime;//报时音关闭起始时间：外音关闭起始时间（时,0~23）
    char BaoShiYinOffEndTime;//报时音关闭终止时间：外音关闭终止时间（时,0~24）

};

class CPassengerFlow
{
public:
    CPassengerFlow(){}

    int m_up_vehicle;
    int m_down_vehicle;
    int m_within_vehicle;

};

class CGuiVoice
{
public:
    CGuiVoice(){
    m_volume_within = 1;
    m_volume_out = 1;
    m_volume_TTS = 1;
    m_light = 0;  //0:自动　1:非常亮　2:一般 3:暗 4:非常暗
    }

    int m_volume_within;/*内音量*/
    int m_volume_out;/*外音量*/
    int m_volume_TTS;/*TTS音*/ /*司机喇叭*/
    int m_light; //亮度

};

class CPassengerInfo
{
public:
    CPassengerInfo()
    {
        passenger_sum = 0;
        passenger_up = 0;
        passenger_down = 0;
    }
        int passenger_sum;
        int passenger_up;
        int passenger_down;

};

class SystemProgressInfo
{
public:
    SystemProgressInfo()
    {
        total_size = 0;
        current_size = 0;
        status = 0;
    }
    qint64 total_size;
    qint64 current_size;
    int status; //0 conitue; 1 finish; -1 failed.
    QString m_result;
};

class CUserLoginInfo
{
public:
    CUserLoginInfo()
    {
        use_dynamic = 0;
        use_sd_data = 0;
        admin_password = "";
        sd_format_password = "";
        sd_admin_password = "";
    }

    char use_dynamic;           /*是否使用动态密码0 不使用 1 使用*/
    char use_sd_data;           /*1:使用SD卡数据，0:使用存储分区数据*/
    QString admin_password;     //分区数据:管理员密码
    QString sd_format_password; //SD格式化密码
    QString sd_admin_password;  //SD管理员密码

};

class CDriverMenuInfo;


class CPlayPicture
{
public:
    CPlayPicture()
    {
        m_path = "";
    }
    QString m_path;
    QImage m_image;
};

class CGetQrPayInfo
{
public:
    CGetQrPayInfo()
    {
        driver_name = "";
        ic_card_tang = "";
        qr_tang = "";
        driver_id = "";
        ic_card_ban = "";
        qr_ban = "";
        ic_card_number = 0;
        qr_number = 0;
    }
    QString driver_name;
    QString ic_card_tang;
    QString qr_tang;
    QString driver_id;
    QString ic_card_ban;
    QString qr_ban;
    int ic_card_number;
    int qr_number;

};

class CGetPassengerInfo
{
public:
    CGetPassengerInfo()
    {
        up = 0;
        down = 0;
        within = 0;
        tang = 0;
        ban = 0;
    }
    int up;
    int down;
    int within;
    int tang;
    int ban;

};

class CCommonData : public QWidget
{
    Q_OBJECT
public:
    explicit CCommonData(QWidget *parent = 0);
    static CCommonData *getInstance();
    
signals:
    
public slots:
    
public:

    bool messagebox_data_change();
    QString get_scroll_sheet();
    int get_message_box_quit();

    CSDCardInfo m_sd_info;
    CStationInfo m_station_info;
    char m_g3_signal;
    char m_sim_status;
    char m_itsnw_status;//调度平台连接
    char m_stmdev_status;//视频平台连接
    char m_over_speed_status;//超速状态
    char m_tmp_status; //tmp 连接状态

    CIoInfo m_io_info;
    char m_hdd_status; //硬盘状态
    int card_number;  //刷卡号
    CScheluingPlan m_scheluing_plan;
    char m_station_type; //1:自动 2:手动
    char m_work_status;
    int m_naviFinish;

    CVideoLoss m_video_loss;
    CDriverInfo m_driver_info;

    CBusPlate m_bus_plate;

    CPassengerFlow m_passenger_flow;
    char station_status;

/////////////////////////////主界面
    CGuiVoice m_CGuiVoice; //声音

    CDriverMenuInfo m_CDriverMenuInfo;
    //画面初始启动设定信息
    CSystemStartingInfo m_CSystemStartingInfo;
    //菜单-系统管理-注册信息
    CSystemRegisterInfo m_CSystemRegisterInfo;
    //菜单-系统管理-坐标采集
    CSystemCoordinateCollection m_CSystemCoordinateCollection;

    //菜单-系统管理-网络
    CSystemNetwork m_CSystemNetwork;
    //菜单-系统管理-I/O配置
    CSystemIoConfig m_CSystemIoConfig;
    //菜单-系统管理-高级设置
    CSystemSeniorSetup m_CSystemSeniorSetup;
    //菜单-系统管理-系统设置
    CSystemSystemSetup m_CSystemSystemSetup;
    //菜单-系统管理-系统维护
    CSystemSystemMaintain m_CSystemSystemMaintain;
    //菜单-系统管理-CAM配置
    CSystemCameraConfig m_CSystemCameraConfig;
    //菜单-系统管理-客流统计
    CSystemPassengerFlow m_CSystemPassengerFlow;

    CRunStatusInfo m_runStatusInfo;

    CPassengerInfo m_CPassengerInfo;
    CSystemPosSetup m_CSystemPosSetup;

    QMap<int,CPlayPicture> m_play_picture_list;
    CPlayPicture m_CPlayPicture;

    CCanInfo m_CCanInfo1;
    CCanInfo m_CCanInfo2;

    CSystemVideotapeQueryEx m_CSystemVideotapeQueryEx;
    CUserLoginInfo m_CUserLoginInfo;
    CGetQrPayInfo m_CGetQrPayInfo;
    CGetPassengerInfo m_CGetPassengerInfo;

    static CCommonData *m_single;

    CMyDefineScreen *m_define_screen;
    QString m_snNum;

};

#endif // COMMON_DATA_H
