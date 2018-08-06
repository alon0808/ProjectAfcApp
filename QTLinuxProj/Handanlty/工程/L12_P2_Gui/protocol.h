#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QWidget>


class CVideoOutput
{
public:
    CVideoOutput(){}
    char m_frame;
    char m_number;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    char m_close;
};
//关于本机
class CAboutMachineInfo
{
public:
    CAboutMachineInfo(){}
    QString m_application_program_version;  //应用程序版本
    QString m_file_system_version;          //根文件系统版本
    QString m_mcu_program_version;          //MCU程序版本
    QString m_system_resouce;               //系统资源
    QString m_system_plug_in;               //系统插件
    QString m_kernel_version;               //内核版本
    QString m_guide_version;                //引导版本
    QString m_keyboard_program_version;     //键盘程序版本
    QString m_device_serial_number;         //设备序列号
    QString m_mac_addr;                     //MAC地址
    QString m_cam1_1_parameter;             //CAM1-1参数
    QString m_cam2_3_parameter;             //CAM2-1参数

};
//运行状态
class CRunStatusMobileNetwork
{
public:
    CRunStatusMobileNetwork()
    {
        m_model_status = 2; //模块状态　1:正常 2:不正常
        m_sim_status = 2; //SIM卡　1:存在 2:不存在
        m_signal_status = 6;//信号质量 1:无信号 2:1格信号 3:2格信号 4:3格信号 5:4格信号 6:无插卡
        m_network_type = ""; //网络类型
        m_reset_times = 0; //复位次数
        //中心服务器1
        center_server1_status = 1;  //1:未使用 2:拔号中 3:连接成功
        center_server1_addr = "";            //服务器地址
        center_server1_port = 0;                //端口
        center_server1_type = "";            //网络类型
        center_server1_connect_times = 0;       //当日连接次数
        center_server1_last_connect = "";    //最后连接时间
        //中心服务器2
        center_server2_status = 1;  //1:未使用 2:拔号中 3:连接成功
        center_server2_addr = "";            //服务器地址
        center_server2_port = 0;                //端口
        center_server2_type = "";            //网络类型
        center_server2_connect_times = 0;       //当日连接次数
        center_server2_last_connect = "";    //最后连接时间
        //视频服务器1
        video_server_status = 1;  //1:未使用 2:拔号中 3:连接成功
        video_server_addr = "";            //服务器地址
        video_server_port = 0;                //端口
        video_server_type = "";            //网络类型
        video_server_connect_times = 0;       //当日连接次数
        video_server_last_connect = "";    //最后连接时间
        m_use_flow = "";                   //本月已使用流量
    }
    char m_model_status; //模块状态　1:正常 2:不正常
    char m_sim_status; //SIM卡　1:存在 2:不存在
    //kEnumNetworkSignalNone = 1,kEnumNetworkSignal1,kEnumNetworkSignal2,kEnumNetworkSignal3,kEnumNetworkSignal4,kEnumNetworkSignalError
    char m_signal_status;//信号质量 1:无信号 2:1格信号 3:2格信号 4:3格信号 5:4格信号 6:无插卡
    QString m_network_type; //网络类型
    int m_reset_times; //复位次数
    //中心服务器1
    char center_server1_status;  //1:未使用 2:拔号中 3:连接成功
    QString center_server1_addr;            //服务器地址
    int center_server1_port;                //端口
    QString center_server1_type;            //网络类型
    int center_server1_connect_times;       //当日连接次数
    QString center_server1_last_connect;    //最后连接时间
    //中心服务器2
    char center_server2_status;  //1:未使用 2:拔号中 3:连接成功
    QString center_server2_addr;            //服务器地址
    int center_server2_port;                //端口
    QString center_server2_type;            //网络类型
    int center_server2_connect_times;       //当日连接次数
    QString center_server2_last_connect;    //最后连接时间
    //视频服务器1
    char video_server_status;  //1:未使用 2:拔号中 3:连接成功
    QString video_server_addr;            //服务器地址
    int video_server_port;                //端口
    QString video_server_type;            //网络类型
    int video_server_connect_times;       //当日连接次数
    QString video_server_last_connect;    //最后连接时间
    QString m_use_flow;                   //本月已使用流量

};
class CRunStatusIO
{
public:
    CRunStatusIO(){}
    char front_door_status;     //前门 1:开启　2:关闭
    char middle_door_status;    //中门
    char back_door_status;      //后门
    char back_car_status;       //倒车
    char m_stop_car_status;     //刹车
    char m_wiper_status;        //雨刮
    char m_left_lamp_status;    //左转灯
    char m_right_lamp_status;   //右转灯
    char m_io1_status;          //I/O输出1
    char m_io2_status;          //I/O输出2
    char m_police1_status;      //报警输入1
    char m_police2_status;      //报警输入2

};
class CRunStatusGPS
{
public:
    CRunStatusGPS()
    {
        m_gps_number = 0;//卫星颗数
        m_longitude = "";//经度
        m_latitude = "";//纬度
        m_Azimuth = 0;//方位角
        m_height = 0;//海拔高度
        m_real_speed = 0;//实时速度
        memset(m_db_value,0,sizeof(m_db_value));
    }
    int m_gps_number;//卫星颗数
    QString m_longitude;//经度
    QString m_latitude;//纬度
    int m_Azimuth;//方位角
    int m_height;//海拔高度
    int m_real_speed;//实时速度
    QString m_firmware_version;
    int m_db_value[24 + 1];

};
class CRunStatusLocalNetwork
{
public:
    CRunStatusLocalNetwork(){
        m_socket_status = 0;
        m_host_from_status = 0;
    }

    /////////////////////////////////////////////////////////d2 start
    char local_network_status;          //状态  //1:已连接 2:未连接  3:连接异常  4:连接中
    QString local_network_ip;           //ip地址
    QString local_network_subnet_mask;  //子网掩码
    QString local_network_gateway;      //网关
    QString local_network_dns1;         //DNS1
    QString local_network_dns2;         //DNS2
    char m_socket_status;       //域socket状态
    char m_host_from_status;    //主从状态
    /////////////////////////////////////////////////////////d2 end

    char m_pon1_status;   //1:已连接 2:未连接  3:连接异常  4:连接中
    QString m_pon1_ip;

    char m_pon2_status;
    QString m_pon2_ip;

    char m_pon3_status;
    QString m_pon3_ip;

    char m_pon4_status;
    QString m_pon4_ip;

    char m_pon5_status;
    QString m_pon5_ip;

    char m_pon6_status;   //1:已连接 2:未连接  3:连接异常  4:连接中
    QString m_pon6_ip;

    char m_pon7_status;
    QString m_pon7_ip;

    char m_pon8_status;
    QString m_pon8_ip;

    char m_pon9_status;
    QString m_pon9_ip;

    char m_pon10_status;
    QString m_pon10_ip;
};
class CRunStatusDevice
{
public:
    CRunStatusDevice(){}
    char m_device_door; //设备门  1:开启  2:关闭
    int m_fan_speed;//风扇转速
    int m_input_voltage;//输入电压
    int m_device_temperature;//设备温度
    int m_car_out_temperature;//车外温度
    int m_car_in_temperature;//车内温度
    char m_serial1_port_status;//串口1状态  1:连接  2:未连接
    char m_serial2_port_status;//串口2状态
    char m_serial3_port_status;//串口3状态
    char m_serial4_port_status;//串口4状态
    char m_serial5_port_status;//串口5状态
    char m_can1_status;//CAN1状态
    char m_can2_status;//CAN2状态

};
class CStationStatus
{
public:
    CStationStatus()
    {
        start_station = "";
        end_station = "";
        current_station = "";
        current_id = 0;
        status = 1;
    }
    QString start_station;      //起始站
    QString end_station;        //终点站
    QString current_station;    //当前站
    int current_id;
    char status;                //1:上行 2:下行

};

class CPeripheralStatus
{
public:
    CPeripheralStatus(){}
    unsigned char probe1;
    unsigned char probe2;
    unsigned char probe3;
    unsigned char probe4;
    unsigned char probe5;
    unsigned char probe6;
    unsigned char probe7;
    unsigned char probe8;
    unsigned char probe9;
    unsigned char probe10;
};

class CRunStatusInfo
{
public:
    CRunStatusInfo()
    {
        m_CRunStatusMobileNetwork = NULL;
        m_CRunStatusIO = NULL;
        m_CRunStatusGPS = NULL;
        m_CRunStatusLocalNetwork = NULL;
        m_CRunStatusDevice = NULL;
        m_CPeripheralStatus = NULL;
    }

    char m_type;
    CRunStatusMobileNetwork *m_CRunStatusMobileNetwork;
    CRunStatusIO *m_CRunStatusIO;
    CRunStatusGPS *m_CRunStatusGPS;
    CRunStatusLocalNetwork *m_CRunStatusLocalNetwork;
    CRunStatusDevice *m_CRunStatusDevice;
    CStationStatus *m_CStationInfo;
    CPeripheralStatus *m_CPeripheralStatus;

};

//司机菜单-高级设置
class CDriverMenuInfo
{
public:
    CDriverMenuInfo()
    {
        memset(passageway,0,sizeof(passageway));
        travel_direction = 0;//行驶方向　0:上行 1:下行
        deputy_page_show_mode = 0;//副主页显示模式　0:标准 1:全屏
        out_door_video_change = 0;//下客门视频切换方式　0:开下客门 1:进站 2:进站或开下客门
        out_door_video_passageway = 0;/*下客门视频通道  0:1 1:2 2:3 3:4 4:5 5:6 6:7 7:8 8:9 9:10
          10:11 11:12 12:13 13:14 14:15 15:16 16:17 17:18 */
        back_car_video_passageway = 0;//倒车视频通道
        start_plan_passageway = 0;//发车计划声道　0:内喇叭 1:司机喇叭 2:外喇叭
        over_speed_warning_passageway = 0;//超速警示声道　0:内喇叭 1:司机喇叭 2:外喇叭
        message_outtime = 0;//消息超时退出 0:5s 1:10s 2:20s 3:30s 4:60s 5:永不
        video_default_frame = 0;/*视频默认画面 0:16分格 1:9分格 2:4分格 3:1 4:2 5:3 6:4 7:5 8:6 9:7 10:8
          11:9 12:10 13:11 14:12 15:13 16:14 17:15 18:16*/
        video_round_robin = 0;//视频轮循 0: 不轮循 1:轮循
        round_robin_passageway = 0;//轮循通道 0:顺序　1:随机
        round_robin_interval = 0;//轮循间隔  0:3S 1:5S 2:10S 3:20S 4:30S 5:50S

    }
    char travel_direction;//行驶方向　0:上行 1:下行
    char deputy_page_show_mode;//副主页显示模式　0:标准 1:全屏
    char out_door_video_change;//下客门视频切换方式　0:开下客门 1:进站 2:进站或开下客门
    char out_door_video_passageway;/*下客门视频通道  0:1 1:2 2:3 3:4 4:5 5:6 6:7 7:8 8:9 9:10
      10:11 11:12 12:13 13:14 14:15 15:16 16:17 17:18 */
    char back_car_video_passageway;//倒车视频通道
    char start_plan_passageway;//发车计划声道　0:内喇叭 1:司机喇叭 2:外喇叭
    char over_speed_warning_passageway;//超速警示声道　0:内喇叭 1:司机喇叭 2:外喇叭
    char message_outtime;//消息超时退出 0:5s 1:10s 2:20s 3:30s 4:60s 5:永不
    char video_default_frame;/*视频默认画面 0:16分格 1:9分格 2:4分格 3:1 4:2 5:3 6:4 7:5 8:6 9:7 10:8
      11:9 12:10 13:11 14:12 15:13 16:14 17:15 18:16*/
    char video_round_robin;//视频轮循 0: 不轮循 1:轮循
    char round_robin_passageway;//轮循通道 0:顺序　1:随机
    char round_robin_interval;//轮循间隔  0:3S 1:5S 2:10S 3:20S 4:30S 5:50S
    char passageway[30];//通道选择　0 ~16 (1分格)

    char out_door_video_passageway_enable;//下客门视频通道使能
    char back_car_video_passageway_enable;//倒车视频通道使能
    char out_door_delay_quit; //下客门延时退出时长
};

//画面初始启动设定信息
class CSystemStartingInfo
{
public:
    CSystemStartingInfo()
    {
       m_CityLine = "";
       m_CompLine = "";
       m_LineLine = "";
       m_CarLIne = "";
    }
      QString m_CityLine;
      QString m_CompLine;
      QString m_CarLIne;
      QString m_LineLine;

};

//系统管理-注册信息
class CSystemRegisterInfo
{
public:
    CSystemRegisterInfo()
    {
        m_line.clear(); //线路列表
        m_select_line = 0; //用户选择的线路
        m_line_attribute = 0;//线路属性
        m_rovince_list.clear(); //省份列表 粤　湘
        m_select_rovince = 0; //用户选择的省份
        m_car_number = "";//车辆编号
        m_manufacturer_number = "";//厂商编号
        m_license_plate = "";//车牌号码
        m_device_id = "";//设备ID号
    }
    QList<QString> m_line; //线路列表
    char m_select_line; //用户选择的线路
    char m_line_attribute;//线路属性
    QList<QString> m_rovince_list; //省份列表 粤　湘
    char m_select_rovince; //用户选择的省份
    QString m_car_number;//车辆编号
    QString m_manufacturer_number;//厂商编号
    QString m_license_plate;//车牌号码
    QString m_device_id;//设备ID号

};
//系统管理-坐标采集
class CSystemCoordinateCollection
{
public:
    CSystemCoordinateCollection()
    {
        m_gps_success = 0;
    }
    QList<QString> m_line_list; //线路列表
    QList<QString> m_site_list;//站点列表
    QList<QString> m_speed_limiting_section_list;//限速段列表
    QList<QString> m_inflection_point_list; //拐点列表
    QList<QString> m_station_list;  //场站列表
    QList<QString> m_field_coordinate_list; //场坐标列表
    char m_direction; //0:上行 1:下行

    QString m_longitude;//经度
    QString m_latitude; //纬度
    int m_azimuth;//方位角
    int m_speed; //时速

    char m_current_line;//当前线路
    char m_current_site;//当前站点
    char m_current_speed_limiting_section;//当前限速段
    char m_current_inflection_point;//当前拐点
    char m_current_station;//当前场站
    char m_current_field_coordinate;//当前场坐标
    char m_gps_success;  //0:gps fail 1:success

};


class VideotapeQueryList  //录像查询天列表
{
public:
    VideotapeQueryList()
    {
        m_position = 0;
        m_year = 0;
        m_month = 0;
        m_day = 0;
        start_hour = 0;
        start_minute = 0;
        start_second = 0;
        end_hour= 0;
        end_minute = 0;
        end_second = 0;
        channel = 0;

    }
    char m_position; //位置  0:硬盘 1:EMMC 2:SD卡
    int m_year; //年
    int m_month;//月
    int m_day;  //日
    int start_hour;
    int start_minute;
    int start_second;
    int end_hour;
    int end_minute;
    int end_second;
    int channel;


    void *param;

};

class VideotapeDay  //录像查询天
{
public:
    VideotapeDay(){}

    char m_day;
    //char m_status; // 1:正常录像　2:报警录像 3:存在正常录像和报警录像
    char normal;
    char alarm;

};

class VideotapeQueryDate  //录像查询日期结果
{
public:
    VideotapeQueryDate(){}
    char m_position; //位置  0:硬盘 1:EMMC 2:SD卡
    int m_year; //年
    int m_month;//月
    QList<VideotapeDay> m_day_list;

};

class CMonthInfo
{
public:
    CMonthInfo(){}
    int m_year;
    QList<char>  m_month_list;


};

//系统管理-录像查询
class CSystemVideotapeQuery
{
public:
    CSystemVideotapeQuery(){}
    //char m_position; //位置  0:硬盘 1:EMMC 2:SD卡
    QList<CMonthInfo>  m_list;


};

//回放
class CSystemPlayBack
{
public:
    CSystemPlayBack(){}
    enum
    {
        kEnumPrevPassageway,//上一个通道
        kEnumPrevVideo,     //上一个视频 1
        kEnumPrevPlay,      //后退 1
        kEnumStart,         //播放
        kEnumStop,          //停止
        kEnumBackPlay,      //前进 1
        kEnumBackVideo,     //下一个视频 1
        kEnumNextPassageway, //下一个通道
        kEnumRefreshPlayList, //更新播放列表
        kEnumVoice,  //不静音
        kEnumNoVoice, //静音
        kEnumExit     //退出
    };

    char m_operate_type;
    /*char m_prev_passageway; //上一个通道
    char m_prev_video; //上一个视频 1
    char m_prev_play;  //后退 1
    char m_start;      // 1:播放 2:停止
    char m_back_play;  //前进 1
    char m_back_video; //下一个视频 1
    char m_prev_passageway; //下一个通道
    */
    QList<int> m_play_list;

};

//导出
class CSystemVideoExport
{
public:
    CSystemVideoExport(){}
    int m_number;           //文件个数
    QList<int> m_list;  //索引列表
    char m_export_type;     //1:原始数据  2:AVI数据
    char m_operate_type;  //1:导出数据  2:取消导出

    //int value; /*1:正在导出 2:导出成功 3:导出失败*/
    char m_type;
    char m_play_type;
    int m_value;
    QString result;/*导出失败原因*/

    int m_year; //年
    int m_month;//月
    int m_day;  //日
    int start_hour;
    int start_minute;
    int start_second;
    int end_hour;
    int end_minute;
    int end_second;
    int channel;

};


class CSystemVideotapeQueryEx
{
public:
    enum
    {
        kEnumGetDate = 1,  //获取年月数据
        kEnumGetMonth = 2, //获取月信息
        kEnumGetDayList = 3,//获取列表信息
        kEnumPlayBack = 4,  //回放
        kEnumExport = 5    //导出


    };
    CSystemVideotapeQueryEx()
    {
        m_CSystemVideotapeQuery = NULL;
        m_VideotapeQueryDate = NULL;
        m_VideotapeQueryList = NULL;
        m_CSystemVideoExport = NULL;
        m_result = "";
    }
    char m_type;
    CSystemVideotapeQuery *m_CSystemVideotapeQuery;
    VideotapeQueryDate *m_VideotapeQueryDate;
    VideotapeQueryList *m_VideotapeQueryList;
    CSystemVideoExport *m_CSystemVideoExport;

    QString m_result;
};
//系统管理-CAM配置-PON信息
class CSystemPonInfo
{
public:
    CSystemPonInfo()
    {
        ip = "";//PON-1通道IP
        select_value = 1;//PON通道使能  0:未选择　1:选择
        //PON通道-配置
        config_ip = "";             //IP地址
        config_sound_recording = 1;//录音  0:开启　　1:关闭
        resolving_power_main = 0;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        resolving_power_sub = 0;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        frame_rate_main = 0;       //帧率 主 0~24 对应值是 1~25
        frame_rate_sub = 0;        //帧率 子 0~24 对应值是 1~25
        picture_main = 0;          //画质　主 0:高 1:中 2:低
        picture_sub = 0;           //画质　子 0:高 1:中 2:低
        identification = 0;        //标识 0:IPC摄像机 1:其他设备
        m_image = 0;
    }
    QString ip;//PON-1通道IP
    char select_value;//PON通道使能  0:未选择　1:选择
    //PON通道-配置
    QString config_ip;             //IP地址
    char config_sound_recording;//录音  0:开启　　1:关闭
    char resolving_power_main;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    char resolving_power_sub;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    char frame_rate_main;       //帧率 主 0~24 对应值是 1~25
    char frame_rate_sub;        //帧率 子 0~24 对应值是 1~25
    char picture_main;          //画质　主 0:高 1:中 2:低
    char picture_sub;           //画质　子 0:高 1:中 2:低
    char identification;        //标识 0:IPC摄像机 1:其他设备
    char m_image; //镜像
};
//系统管理-CAM配置-CAM信息
class CSystemCamInfo
{
public:
    CSystemCamInfo()
    {
        select_value = 1;//CAM通道使能  0:未选择　1:选择
        //CAM通道-配置
        config_sound_recording = 0;//录音  0:开启　　1:关闭
        resolving_power_main = 0;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        resolving_power_sub = 0;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        frame_rate_main = 0;       //帧率 主 0~24 对应值是 1~25
        frame_rate_sub = 0;        //帧率 子 0~24 对应值是 1~25
        picture_main = 0;          //画质　主 0:高 1:中 2:低
        picture_sub = 0;           //画质　子 0:高 1:中 2:低
        identification = 0;        //标识 0:视频录像 1:客流录像
        m_image = 0;
    }

    char select_value;//CAM通道使能  0:未选择　1:选择
    //CAM通道-配置
    char config_sound_recording;//录音  0:开启　　1:关闭
    char resolving_power_main;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    char resolving_power_sub;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    char frame_rate_main;       //帧率 主 0~24 对应值是 1~25
    char frame_rate_sub;        //帧率 子 0~24 对应值是 1~25
    char picture_main;          //画质　主 0:高 1:中 2:低
    char picture_sub;           //画质　子 0:高 1:中 2:低
    char identification;        //标识 0:视频录像 1:客流录像
    char m_image; //镜像
};

//系统管理-CAM配置
class CSystemCameraConfig
{
public:
    CSystemCameraConfig(){}

    //通用设置---------------
    char videotape_standard;//录像制式  0:PAL 1:NTSC
    char auto_cover;//自动覆盖 0:是  1:否
    char videotape_lock;//录像锁定　0:1 1:2 2:3 3:4 4:5 5:6 6:7
    char videotape_prev;//录像预录    min
    char network_transmission_mode;//网传模式  0:流畅+清晰 1:非常清晰 2:清晰 3:流畅 4:非常流畅
    char videotape_storage;//录像存储器 0:EMMC+外置SD卡 1:EMMC  2:外置SD卡
    char videotape_mode;//录像模式  0:镜像录像  1:报警备份录像 2:子码流录像
    //配置－镜像通道
    QList<char> m_passageway_list;
    //显示设置---------------
    char time_show_enable;//时间显示使能
    char time_show_value;//时间显示值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
    char passageway_name_enable;//通道名称使能
    char passageway_name_value;//通道名称值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
    char vehicle_number_enable;//车辆编号使能
    char vehicle_number_value;//车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
    char coordinate_info_enable;//坐标信息使能
    char coordinate_info_value;//坐标信息值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
    char real_speed_enable;//实时速度使能
    char real_speed_value;//实时速度值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
    char station_info_enable;//站点信息使能
    char station_info_value;//站点信息值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    //模拟摄像机通道设置---------------
    char monitor_passageway_number;//模拟数量
    CSystemCamInfo cam1;
    CSystemCamInfo cam2;
    CSystemCamInfo cam3;
    CSystemCamInfo cam4;
    CSystemCamInfo cam5;
    CSystemCamInfo cam6;

    CSystemCamInfo cam_default;

    //网络摄像通道设置---------------
    char network_passageway_number;//IPC数量
    CSystemPonInfo pon1;
    CSystemPonInfo pon2;
    CSystemPonInfo pon3;
    CSystemPonInfo pon4;
    CSystemPonInfo pon5;
    CSystemPonInfo pon6;
    CSystemPonInfo pon7;
    CSystemPonInfo pon8;
    CSystemPonInfo pon9;
    CSystemPonInfo pon10;

    CSystemPonInfo pon_default;




};
//系统管理-客流统计(CAM1-1通道设置 CAM2-1通道设置)
class CSystemPassengerSet
{
public:
    CSystemPassengerSet()
    {
        m_enable = 0; //使能  0:不选择 1:选择
        is_connect = 2; // 1:连接  2:未连接
        m_id = ""; //ID
        param_file_exist = 2; //1:存在　　2:不存在
        param_file_name = "";
        guide = 0;//辅助线 0:不选择 1:选择
        count_show_mode = 0;//计数显示模式  0:标准  1:进站
        count_rule = 0;//计数规则  //0:上进下出  1:下进上出
        noise_filter = 0;//噪声过滤
        sensitivity = 0;//灵敏度
        count_type = 0;//计数类型
        install_height = 0;//安装高度
        filter_height = 0;//过滤高度
        //检测区域
        count_line1 = 0;//上计数线1
        count_line2 = 0;//下计数线2
        count_line3 = 0;//左边界线3
        count_line4 = 0;//右边界线4
    }
    char m_enable; //使能  0:不选择 1:选择
    char is_connect; // 1:连接  2:未连接
    QString m_id; //ID
    char param_file_exist; //1:存在　　2:不存在
    QString param_file_name;
    char guide;//辅助线 0:不选择 1:选择
    char count_show_mode;//计数显示模式  0:标准  1:进站
    char count_rule;//计数规则  //0:上进下出  1:下进上出
    int noise_filter;//噪声过滤
    double sensitivity;//灵敏度
    int count_type;//计数类型
    int install_height;//安装高度
    int filter_height;//过滤高度
    //检测区域
    int count_line1;//上计数线1
    int count_line2;//下计数线2
    int count_line3;//左边界线3
    int count_line4;//右边界线4


};

class CPassengerResult
{
public:
    CPassengerResult(){}
    int m_type;
    QString m_return_result;
    int m_value;
    int m_cam_select;

};

//系统管理-客流统计
class CSystemPassengerFlow
{
public:
    CSystemPassengerFlow(){}
    //CAM1-1通道设置
    CSystemPassengerSet m_cam1_1_set;
    //CAM2-1通道设置
    CSystemPassengerSet m_cam2_1_set;
    //CAM3-1通道设置
    CSystemPassengerSet m_cam3_1_set;
    //CAM4-1通道设置
    CSystemPassengerSet m_cam4_1_set;
    //高级设置
    //高级设置－计数使能
    char real_dynamic_statistics;//实时动态统计  0:不选择 1:选择
    char open_door_statistics;//开门统计  0:不选择 1:选择
    char in_station_statistics;//进站统计  0:不选择 1:选择
    char out_station_statistics;//站外统计  0:不选择 1:选择
    char speed_statistics_enable;//速度统计使能  0:不选择 1:选择
    char speed_statistics_value;//速度统计值
    char single_loop_error_calibration;//单圈误差校准
    char close_delay;//关门延时
    char time_upload_time;//定时上报时间

    char image_mosaic;//图像拼接　0:不选择 1:选择
    char interval_width;//间隔宽度　CM

};

//系统管理-存储管理
class CSystemStorageManage
{
public:
    CSystemStorageManage(){}
    char m_type;   //1:硬盘 2:SD卡 3:EMMC
    char m_enable; // 0:未选择状态 1:选中状态 2:禁用状态
    QString m_surplus_storage; //剩余容量　20G
    QString m_total_storage; //总的容量　32G
    char m_status; // 1:正常 2:不正常

};

//系统管理-网络-移动网络
class CMobileNetworkInfo
{
public:
    CMobileNetworkInfo(){}
    char m_enable; // 0:未选择状态 1:选中状态 2:禁用状态
    QString modular_name1; //模块型号名称1 龙尚
    QString modular_name2; //模块型号名称1 8300C
    QString modular_version;//软件版本
    QString modular_imei;//IMEI号
    QString modular_sn; //S/N号
    QString modular_meid;//MEID
    char operator1;//运营商  0:自动 1:手动
    char network_type; //网络类型 0:4G 1:3G 2:2G
    char month_end_date;//月结日
    char use_vpn; //是否使用VPN  0:未选择状态 1:选中状态 2:禁用状态
    QString vpn_name;     //VPN名称
    QString vpn_account;  //VPN账号
    QString vpn_password; //VPN密码


};

//系统管理-网络-中心服务器
class CCenterServerInfo
{
public:
    CCenterServerInfo(){}
    char m_enable; // 0:未选择状态 1:选中状态 2:禁用状态
    QList<QString> protocol_type_list;  //协议类型列表
    int protocol_type; //选中的协议类型
    char access_mode; //接入模式 0:固件IP地址 1:域名
    QString m_ip;//IP地址或域名
    int m_port; //端口
    int m_data_send_interval; //数据发送间隔 (秒)


};
//系统管理-网络-视频服务器
class CVideoServerInfo
{
public:
    CVideoServerInfo(){}
    char m_enable; // 0:未选择状态 1:选中状态 2:禁用状态
    QList<QString> protocol_type_list;  //协议类型列表
    int protocol_type; //选中的协议类型
    char access_mode; //接入模式 0:静态IP 1:自动获取
    QString m_ip;//IP地址或域名
    int m_port; //端口

};

//系统管理-网络-本地网络
class CLocalNetworkInfo
{
public:
    CLocalNetworkInfo(){}
    char m_enable; // 0:未选择状态 1:选中状态 2:禁用状态
    char m_dhcp_server; // 0:未选择状态 1:选中状态 2:禁用状态
    char access_mode; //接入模式 0:静态IP 1:自动获取
    QString m_ip;//IP地址
    QString subnet_mask;//子网掩码
    QString gateway;//网关
    QString nds1;//DNS1
    QString nds2;//DNS2

};



class CWifiConnectInfo
{
public:
    CWifiConnectInfo(){}

    char m_type; // 1:连接 2:断开

    QString m_ssid_name;//SSID名称
    char m_security;    //安全性 0:WEP  1:WPA-PSK  2:WPA2-PSK  3:WAPI
    QString m_password; //密码

};

//系统管理-网络-WIFI
class CWifiInfo
{
public:
    CWifiInfo(){}
    char m_enable; // 0:未选择状态 1:选中状态 2:禁用状态
    char access_mode; //接入模式 0:静态IP 1:自动获取
    QString m_ip;//IP地址
    QString subnet_mask;//子网掩码
    QString gateway;//网关
    QString nds1;//DNS1
    QString nds2;//DNS2

};
//系统管理-网络-FTP
class CFtpInfo
{
public:
    CFtpInfo(){}
    char m_enable; // 0:未选择状态 1:选中状态 2:禁用状态
    char access_mode; //接入模式 0:静态IP 1:域名
    QString m_ip;//IP地址
    //QString subnet_mask;//子网掩码
    QString gateway;//网关

};
//系统管理-网络
class CSystemNetwork
{
public:
    CSystemNetwork()
    {
        m_CMobileNetworkInfo = NULL;
        m_CCenterServerInfo1 = NULL;
        m_CCenterServerInfo2 = NULL;
        m_CVideoServerInfo = NULL;
        m_CLocalNetworkInfo = NULL;
        m_CWifiInfo = NULL;
        m_CFtpInfo = NULL;
        m_select_wifi = NULL;
    }
    char m_type; // 1:移动网络 2:中心服务器1 3:中心服务器2 4:视频服务器 5:本地网络 6:WIFI 7:FTP 8:选择WIFI列表
    CMobileNetworkInfo *m_CMobileNetworkInfo;
    CCenterServerInfo *m_CCenterServerInfo1;
    CCenterServerInfo *m_CCenterServerInfo2;
    CVideoServerInfo *m_CVideoServerInfo;
    CLocalNetworkInfo *m_CLocalNetworkInfo;
    CWifiInfo *m_CWifiInfo;
    CFtpInfo *m_CFtpInfo;
    void *m_select_wifi;
};

//系统管理-I/O配置
class CSystemIoConfig
{
public:
    CSystemIoConfig(){}
    //车门检测
    char m_enable; // 0:未选择状态 1:选中状态
    char m_car_door_number;//车门数量 0:1 1:2 2:3
    char m_car_door_type;//车门类型  0:两线气动门　1:传感器门　2:不检测
    char m_door_front;//开前门电平 0:低电平有效　1:高电平有效
    char m_door_middle;//开中门电平 0:低电平有效　1:高电平有效
    char m_door_back;//开后门电平 0:低电平有效　1:高电平有效
    char m_io_input1;//I/O输入1 0:低电平有效　1:高电平有效
    //车门检测－配置
    char speed_open_door_check; //带速度开门检测
    char station_open_door_check;//站间开门检测
    char arrive_open_door_check;//到站未开门检测
    char arrive_stop_door_check;//到站未停车检测
    char m_speed_enable; //异常开关门最小速度使能 0:未选择状态 1:选中状态
    int m_speed; //异常开关门最小速度
    char m_frequency_enable; //异常开关门提醒使能 0:未选择状态 1:选中状态
    int m_frequency;//异常开关门提醒次数
    //其他开关量检测
    char back_car_monitor_enable;//倒车监视 0:未选择状态 1:选中状态
    char back_car_monitor_status;//倒车监视 0:低电平有效　1:高电平有效
    char no_brake_enable; //未刹车 0:未选择状态 1:选中状态
    char no_brake_status; //未刹车 0:低电平有效　1:高电平有效
    char open_front_box_enable;//开前箱 0:未选择状态 1:选中状态
    char open_front_box_status;//开前箱 0:低电平有效　1:高电平有效
    char wiper_enable;//雨刮 0:未选择状态 1:选中状态
    char wiper_status;//雨刮 0:低电平有效　1:高电平有效
    char left_lamp_enable;//左转灯 0:未选择状态 1:选中状态
    char left_lamp_status;//左转灯 0:低电平有效　1:高电平有效
    char right_lamp_enable;//右转灯 0:未选择状态 1:选中状态
    char right_lamp_status;//右转灯 0:低电平有效　1:高电平有效

};

//CAN调试
class CCanInfo
{
public:
    CCanInfo(){}
    char m_type; // 1:CAN1  2:CAN2
    QString m_command;  //测试命令
    QString m_result;   //返回结果

};

//系统管理-高级设置
class CSystemSeniorSetup
{
public:
    CSystemSeniorSetup(){}
    //通用配置
    char location_mode; //定位方式 0:混合定位 1:北斗定位 2:GPS定位
    QString modular_mode;//模块型号
    QString software_version;//软件版本
    QString sn_number;//S/N号
    char custom_navigation_enable;//惯性导航使能
    //惯性导航-配置(start)
    int corner_x; //设备X轴相对车X轴转角
    int corner_y; //设备Y轴相对车Y轴转角
    int corner_z; //设备Z轴相对车Z轴转角
    int corner_gain; //转弯Gain值
    //惯性导航-配置(end)
    char m_station_mode; //报站方式 0:自动 1:手动
    int in_station_radius;//进站半径
    int out_station_radius;//出站半径
    int inflection_point_radius;//拐点半径
    int over_speed_still_tip; //超速保持提醒
    int over_speed_tip_times;//超速提醒次数
    int non_operating_speed_limit;//非运营限速
    char operating_mode;   //0:营运　1:非营运

    //告警使能
    char urgent_acceleration_enable; //紧急加速使能 // 0:未选择状态 1:选中状态
    int urgent_acceleration_value;//紧急加速值  (km/s)
    char emergency_deceleration_enable;//紧急减速使能 // 0:未选择状态 1:选中状态
    int emergency_deceleration_value;//紧急减速值 (km/s)
    char hysteresis_alarm_enable;//滞站报警使能 // 0:未选择状态 1:选中状态
    int hysteresis_alarm_value;//滞站报警值
    char skip_alarm_enable;//越站报警使能
    int skip_alarm_value;//越站报警值
    char fatigue_driving_check_enable;//疲劳驾驶检测使能
    int fatigue_driving_check_value;//疲劳驾驶检测值
    char idling_enable;//怠速使能使能
    int idling_value;//怠速使能值
    //CAN设置
    char can1_enable;//CAN1  0:未选择状态 1:选中状态
    char can2_enable;//CAN2  0:未选择状态 1:选中状态

    //高级设置
    char volumn_adjustment; //音量调整使能 0:未选择状态 1:选中状态
    char idling_enable1;//怠速使能 0:未选择状态 1:选中状态
    int idling_value1;//怠速值1
    int idling_value2;//怠速值2
    char temperature_check_enable; //温度检测使能
    //温度检测-配置
    int in_temperature_security1;//内温安全范围1
    int in_temperature_security2;//内温安全范围2
    int out_temperature_security1;//外温安全范围1
    int out_temperature_security2;//外温安全范围2
    int police_upload_time; //报警上传时间间隔


};

//系统管理-POS配置
class CSystemPosSetup
{
public:
    CSystemPosSetup()
    {
        DEVMODE = 0;   //终端模式
        CHARGMODE = 0; //收费模式
        PSAMNUM = 0;   //PSAM个数
        memset(SUPCARD_TYPE,0,sizeof(SUPCARD_TYPE));  //支持卡类型
        SECTOR = 0;     //支持扇区
        TIMEINTER = 0;   //刷卡时间间隔
        TICKETTIME = 0;  //逃票最大时间
        m_host_ip = "";
    }
    unsigned char DEVMODE;   //终端模式
    unsigned char CHARGMODE; //收费模式
    unsigned char PSAMNUM;   //PSAM个数
    unsigned char SUPCARD_TYPE[32];  //支持卡类型
    unsigned char SECTOR;     //支持扇区
    unsigned int TIMEINTER;   //刷卡时间间隔
    unsigned short TICKETTIME;  //逃票最大时间
    QString m_host_ip;
};

//系统管理-系统设置
class CSystemSystemSetup
{
public:
    CSystemSystemSetup(){
        picutre_play_time = 0;
        m_current_time_zone = 0;
    }
    //通用配置
    char screen_mode; //屏幕模式 0:高清模式 1:标清模式
    char drusb_mode;  //DRUSB接口模式 0:标准 1:灾备
    char sleep_shutdown_enable; //延时关机使能
    int sleep_shutdown_value; //延时关机值　(min)
    char device_door_report_enable; //设备门开启上报间隔使能
    int device_door_report_value; //设备门开启上报间隔值
    char hard_disk_heating_enable;//硬盘加热使能
    int hard_disk_heating_value; //硬盘加热值
    char now_time_enable;//整点报时使能
    int now_time_value1;//整点报时值1   h
    int now_time_value2;//整点报时值2   h
    char voltage_enable;//超电压反警使能
    int voltage_value1;//超电压报警值1  V
    int voltage_value2;//超电压报警值2  V
    int voltage_value3;//超电压报警值3  min
    char temperature_enable;//超温度报警使能
    int temperature_value1;//超温度报警值1  V
    int temperature_value2;//超温度报警值2  V
    int temperature_value3;//超温度报警值3  min
    //add
    char fan_control_enable;//风扇控制使能
    int fan_control_value1;//风扇控制值1
    int fan_control_value2;//风扇控制值2
    int picutre_play_time; //图片轮播时间
    char volumn_size; //声音大小
    //系统时间
    QList<QString > m_time_zone_list;//时区列表
    int m_current_time_zone; //当前选择的时区
    QString m_time1;//时间1
    QString m_time2;//时间2
    char satellite_time_enable;//卫星校时  0:未选择状态 1:选中状态
    char center_time_enable;//中心校时  0:未选择状态 1:选中状态
    char ntp_enable;//NTP校时使能 0:未选择状态 1:选中状态
    char ntp_value; //NTP校时值 0:time.windows.com 1:time.nist.gov 2:time-nw.nist.gov 3:time-a.nist.gov 4:time-b.nist.gov

    char user_set_time; //用户设置时间　１：设置 0：不设置
    int tm_year;
    int tm_mon;
    int tm_mday;
    int tm_hour;
    int tm_min;
    int tm_sec;

    //账户设置
    char use_password;//使用密码 0:未选择状态 1:选中状态
    char auto_lock_enable; //自动锁定使能
    char auto_lock_value;  //自动锁定值(min)  0:1  1:2  2:3  3:5  4:10
    char timeout_exit_enable;//超时退出使能
    char timeout_exit_value;//超时退出值(min) 0:1  1:2  2:3  3:5  4:10
};

//系统管理-系统维护
class CSystemSystemMaintain
{
public:
    CSystemSystemMaintain()
    {
        m_param = NULL;
    }
    enum
    {
        //系统升级
        kEnumApplicationProgram, //应用程序
        kEnumMcuProgram,
        kEnumUBootProgram,
        //报站文件
        kEnumImportStationFile,//导入报站文件
        kEnumExportStationFile,//导出报站文件
        kEnumClearStationFile,//清除报站文件版本
        kEnumFormatStationFile,//格式化报站分区
        //客流配置
        kEnumImportPassengerFlowFile,//导入客流配置文件
        kEnumParameterConfig,//参数配置
        //日志/记录
        kEnumExportVehicleRecord,//导出行车记录
        kEnumClearVehicleRecord,//清除行车日志
        kEnumExportSystemLog,//导出系统日志
        kEnumClearSystemLog,//清除系统日志
        kEnumExportOperationLog,//导出操作日志
        kEnumClearOperationLog,//清除操作日志
        //高级
        kEnumImportLogoFile,//导入开机LOGO文件
        kEnumResumeApplicationProgram,//恢复应用程序
        kEnumResumeMcuProgram,//恢复MCU程序
        kEnumResumeUBootProgram,//恢复UBoot程序
        kEnumResumeFactoryConfig,//恢复出厂设置
        kEnumImportPicture, //导入广告图片
        kEnumExportPicture, //导出广告图片
        kEnumCleanPicture,   //清除广告图片
        kEnumImportCardParam, //导入刷卡参数
        kEnumExportCardParam,   //导出刷卡参数
        kEnumCleanCardParam,    //清除刷卡参数
        kEnumCleanCardRecord    //清除刷卡记录
    };
    enum
    {
        kEnumGetData = 1,//获取数据
        kEnumGetProgress,//获取进度
        kEnumOperate,    //正在操作
        kEnumSuccess,    //操作成功
        kEnumFail        //操作失败
    };
    char m_operate_type;
    QString m_result; //失败原因
    int percent; //更新进度
    void *m_param;


};



class CProtocol : public QWidget
{
    Q_OBJECT
public:
    explicit CProtocol(QWidget *parent = 0);
    static CProtocol *getInstance();
    enum
    {//设置画面
        kEnumFrame1 = 1,    //1画面
        kEnumFrame4,        //4画面
        kEnumFrame9,        //9画面
        kEnumFrame16,       //16画面
        kEnumFrameDefine,   //自定义1画面的位置
        kEnumFramePassengerFlow, //客流界面
        kEnumFramePassengerDefine //客流自定义
    };
    enum
    {//显示或隐藏画面
        KEnumCameraShow = 1,
        KEnumCameraHide
    };
    enum
    {//设置通道
        kEnumPassageway1 = 1,   //1通道
        kEnumPassageway2,       //2通道
        kEnumPassageway3,       //3通道
        kEnumPassageway4,       //4通道
        kEnumPassageway5,       //5通道
        kEnumPassageway6,       //6通道
        kEnumPassageway7,       //7通道
        kEnumPassageway8,       //8通道
        kEnumPassageway9,       //9通道
        kEnumPassageway10,      //10通道
        kEnumPassageway11,      //11通道
        kEnumPassageway12,      //12通道
        kEnumPassageway13,      //13通道
        kEnumPassageway14,      //14通道
        kEnumPassageway15,      //15通道
        kEnumPassageway16       //16通道
    };

    enum
    {
        kEnumRunStatusMobileNetwork = 1,//移动网络
        kEnumRunStatusIO = 2,           //I/O
        kEnumRunStatusGPS = 3,          //定位模块
        kEnumRunStatusLocalNetwork = 4, //本地网络
        kEnumRunStatusDevice = 5,        //设备
        kEnumRunStatusStation = 6,       //站点信息
        kEnumRunStatusPeripheral = 7     //外设
    };
    enum
    {
        kEnumPresetEvent = 1,       //预设事件
        kEnumMessage,               //普通短信
        kEnumVehiclePlan            //行车计划
    };

    enum
    {
        kEnumSave = 1, //保存数据
        kEnumGet = 2,   //获取数据
        kEnumDefault = 3,   //默认数据
        kEnumSearch = 4   //搜索
    };
    enum
    {
        kEnumHardDisk = 1,
        kEnumSDCard = 2,
        kEnumEmmc = 3
    };
    enum
    {
        kEnumWifiInfo,   //获取WIFI信息
        kEnumWifiConnect,//连接WIFI
        kEnumWifiStop    //停止WIFI连接
    };

    enum
    {
        kEnumSystemMaintainList, //获取列表数据
        kEnumSystemMaintainOperate //用户操作
    };
    enum
    {
        kEnumBehaviorNormal = 1,//正常操作
        kEnumBehaviorStop       //用户取消
    };

signals:
    
public slots:

public:
    void call_json_camera_set(char _frame,char *_passageway,int _startX,int startY,int _width,int _height,char _show = 2);
    void call_json_camera_set(CVideoOutput &_videoOutput);
    void call_json_about_machine_set(char _type);
    void call_json_run_status_set(char _type);
    void call_json_message_set(char _action,char _type,int _code = 0,QString _card_number = "");
    void call_json_system_register_info_set(char _type);
    void call_json_system_starting_info_set(char _type);
    void call_json_starting_screen_flag_set(char _type); // 开机画面选择标记
    void call_json_system_coordinate_collection_set(char _type,char _type1);
    void call_json_system_videotape_query_set(char _type,void *_param);
    void call_json_system_storage_manage_set(char _type,char _format);
    void call_json_system_operate_set(char _type);
    void call_json_system_network_set(char _type);
    void call_json_system_wifi_set(char _type,void *_param);
    void call_json_system_switch_config_set(char _type);
    void call_json_system_senior_setup_set(char _type,void *_param = NULL);
    void call_json_system_system_setup_set(char _type);
    void call_json_system_maintain_set(char _type,char _type_main,char _behavior_operate = kEnumBehaviorNormal,QString _filename = "");
    void call_json_system_camera_config(char _type);
    void call_json_system_pon_cam(char _type,char _type1 = 1,char _number = 1);
    void call_json_system_passenger_flow(char _type,char _number = 1,char _cam1_2 = 1);
    void call_json_menu_driver(char _action,char _value,bool _save = true);
    void request_system_message(const char *_msg);
    void call_json_system_pos_config_set(char _type);
    void call_json_station_name_set(char _type);

    static CProtocol *m_single;

};

#endif // PROTOCOL_H













