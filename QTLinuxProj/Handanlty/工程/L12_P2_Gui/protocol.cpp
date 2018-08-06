#include "protocol.h"

#include "class/cJSON.h"
#include "class/ltyuicallinterface.h"

#include "common_data.h"

CProtocol *CProtocol::m_single = NULL;

CProtocol *CProtocol::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CProtocol;
    }
    return m_single;
}

CProtocol::CProtocol(QWidget *parent) :
    QWidget(parent)
{

}

void CProtocol::call_json_camera_set(CVideoOutput &_videoOutput)
{
    switch(_videoOutput.m_frame)
    {
        case CProtocol::kEnumFrame1:    //1画面
        {
            char passageway[16];
            passageway[0] = _videoOutput.m_number;
            CProtocol::getInstance()->call_json_camera_set(_videoOutput.m_frame,passageway,0,0,0,0);
        }break;
        case CProtocol::kEnumFrame4:        //4画面
        {
            char passageway[16];
            passageway[0] = 1;
            passageway[1] = 2;
            passageway[2] = 3;
            passageway[3] = 4;
            CProtocol::getInstance()->call_json_camera_set(_videoOutput.m_frame,passageway,0,0,0,0);

        }break;
        case CProtocol::kEnumFrame9:        //9画面
        {
            char passageway[16];
            CProtocol::getInstance()->call_json_camera_set(_videoOutput.m_frame,passageway,0,0,0,0);
        }break;
        case CProtocol::kEnumFrame16:        //16画面
        {
            char passageway[16];
            CProtocol::getInstance()->call_json_camera_set(_videoOutput.m_frame,passageway,0,0,0,0);
        }break;
        case CProtocol::kEnumFrameDefine:
        {
            char passageway[16];
            passageway[0] = _videoOutput.m_number;
            CProtocol::getInstance()->call_json_camera_set(_videoOutput.m_frame,passageway,_videoOutput.m_x,_videoOutput.m_y,_videoOutput.m_width,_videoOutput.m_height);
        }break;
        case CProtocol::kEnumFramePassengerFlow:        //客流界面
        {
            char passageway[16];
            CProtocol::getInstance()->call_json_camera_set(_videoOutput.m_frame,passageway,0,0,0,0);
        }break;
        case kEnumFramePassengerDefine:
        {
            char passageway[16];
            passageway[0] = _videoOutput.m_number;
            CProtocol::getInstance()->call_json_camera_set(_videoOutput.m_frame,passageway,_videoOutput.m_x,_videoOutput.m_y,_videoOutput.m_width,_videoOutput.m_height,_videoOutput.m_close);
        }break;
    }
}
void CProtocol::call_json_camera_set(char _frame,char *_passageway,int _startX,int _startY,int _width,int _height,char _show)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        if(NULL == _passageway)
        {
            cJSON_Delete(root);
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_videotape_video"); /*摄像机显示*/

        cJSON_AddNumberToObject(root, "type", _frame);/* 1:1画面、  2:4画面、 3:9画面、 4:16画面*/

        switch(_frame)
        {
            case CProtocol::kEnumFrame1:    //1画面
            {

                cJSON_AddNumberToObject(root, "number", _passageway[0]);/* 1~16通道*/
            }break;
            case CProtocol::kEnumFrame4:        //4画面
            {
                cJSON_AddNumberToObject(root, "number1", _passageway[0]);/* 1~16通道*/
                cJSON_AddNumberToObject(root, "number2", _passageway[1]);/* 1~16通道*/
                cJSON_AddNumberToObject(root, "number3", _passageway[2]);/* 1~16通道*/
                cJSON_AddNumberToObject(root, "number4", _passageway[3]);/* 1~16通道*/

            }break;
            case CProtocol::kEnumFrame9:        //9画面
            case CProtocol::kEnumFrame16:        //16画面
            {
            }break;
            case kEnumFrameDefine:
            {
                cJSON_AddNumberToObject(root, "number", _passageway[0]);/* 1~16通道*/
                cJSON_AddNumberToObject(root, "start_x", _startX);/*起始X坐标*/
                cJSON_AddNumberToObject(root, "start_y", _startY);/*起始Y坐标*/
                cJSON_AddNumberToObject(root, "width", _width);/*显示宽度*/
                cJSON_AddNumberToObject(root, "height", _height);/*显示高度*/
            }break;
            case kEnumFramePassengerDefine:
            {
                cJSON_AddNumberToObject(root, "number", _passageway[0]);/* 1:cam_1, 2:cam_2*/
                cJSON_AddNumberToObject(root, "show", _show);
                if(1 == _show)
                {
                    cJSON_AddNumberToObject(root, "start_x", _startX);/*起始X坐标*/
                    cJSON_AddNumberToObject(root, "start_y", _startY);/*起始Y坐标*/
                    cJSON_AddNumberToObject(root, "width", _width);/*显示宽度*/
                    cJSON_AddNumberToObject(root, "height", _height);/*显示高度*/
                }


            }break;

        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}
//关于本机
void CProtocol::call_json_about_machine_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_about_machine"); /*摄像机显示*/

        cJSON_AddNumberToObject(root, "type", _type);

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}
//运行状态
void CProtocol::call_json_run_status_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_run_status");

        cJSON_AddNumberToObject(root, "type", _type);

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}
/*
char _action: 1:获取 2:发送预设事件代码 3:司机卡号
char _type:1:预设事件 2:消息 3:行车计划
*/
void CProtocol::call_json_message_set(char _action,char _type,int _code,QString _card_number)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_message");
        cJSON_AddNumberToObject(root, "action", _action);
        switch(_action)
        {
            case 1:
            {
                cJSON_AddNumberToObject(root, "type", _type); //1:预设事件 2:消息 3:行车计划
            }break;
            case 2:
            {
                cJSON_AddNumberToObject(root, "send_code", _code);
            }break;
            case 3:
            {
                cJSON_AddNumberToObject(root, "card_length", _card_number.length());
                cJSON_AddStringToObject(root, "card_number", _card_number.toUtf8().data());
            }break;
        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

void CProtocol::call_json_starting_screen_flag_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();
        if(!root)
        {
            break;
        }
        cJSON_AddStringToObject(root, "msg_type", "gui_starting_screen_flag");
        switch(_type)
        {
            case kEnumSave:
            {
                cJSON_AddNumberToObject(root, "type", 1);  //保存
                //１表示已经从导航界面进入主画面
                cJSON_AddNumberToObject(root, "screen_flag", 1);
            }break;
            case kEnumGet:
            {
                cJSON_AddNumberToObject(root, "type", 2); //获取数据
            }break;
        }


        gui_send_message2system(root);


    }while(0);

}

void CProtocol::call_json_system_starting_info_set(char _type)
{

    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_starting_info");
        switch(_type)
        {
            case kEnumSave:
            {
                cJSON_AddNumberToObject(root, "type", 1);  //保存
                cJSON_AddStringToObject(root, "cityID",CCommonData::getInstance()->m_CSystemStartingInfo.m_CityLine.toUtf8().data());
                cJSON_AddStringToObject(root, "compontID",CCommonData::getInstance()->m_CSystemStartingInfo.m_CompLine.toUtf8().data());
                cJSON_AddStringToObject(root, "carID", CCommonData::getInstance()->m_CSystemStartingInfo.m_CarLIne.toUtf8().data());
                cJSON_AddStringToObject(root, "lineID", CCommonData::getInstance()->m_CSystemStartingInfo.m_LineLine.toUtf8().data());
            }break;
            case kEnumGet:
            {
                cJSON_AddNumberToObject(root, "type", 2); //获取数据
            }break;
        }


        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
 }
//系统管理-注册信息
void CProtocol::call_json_system_register_info_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_register_info");
        switch(_type)
        {
            case kEnumSave:
            {
                cJSON_AddNumberToObject(root, "type", 1);  //保存
                cJSON_AddNumberToObject(root, "line",CCommonData::getInstance()->m_CSystemRegisterInfo.m_select_line);  //线路
                cJSON_AddNumberToObject(root, "line_attribute",CCommonData::getInstance()->m_CSystemRegisterInfo.m_line_attribute);
                cJSON_AddStringToObject(root, "car_number", CCommonData::getInstance()->m_CSystemRegisterInfo.m_car_number.toUtf8().data());//车辆编号
                cJSON_AddStringToObject(root, "manufacturer_number", CCommonData::getInstance()->m_CSystemRegisterInfo.m_manufacturer_number.toUtf8().data());//厂商编号
                cJSON_AddNumberToObject(root, "select_rovince", CCommonData::getInstance()->m_CSystemRegisterInfo.m_select_rovince); //用户选择的省份
                cJSON_AddStringToObject(root, "license_plate", CCommonData::getInstance()->m_CSystemRegisterInfo.m_license_plate.toUtf8().data());//车牌号码
                cJSON_AddStringToObject(root, "device_id", CCommonData::getInstance()->m_CSystemRegisterInfo.m_device_id.toUtf8().data());//设备ID号
            }break;
            case kEnumGet:
            {
                cJSON_AddNumberToObject(root, "type", 2); //获取数据
            }break;
        }


        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

//系统管理-坐标采集
void CProtocol::call_json_system_coordinate_collection_set(char _type,char _type1)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_coordinate_collection");
        switch(_type)
        {
            case kEnumSave:
            {
                cJSON_AddNumberToObject(root, "type", 1);  //保存
                cJSON_AddNumberToObject(root, "current_line",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_line);//当前线路
                cJSON_AddNumberToObject(root, "current_site",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_site);//当前站点
                cJSON_AddNumberToObject(root, "current_speed_limiting_section",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_speed_limiting_section);//当前限速段
                cJSON_AddNumberToObject(root, "current_inflection_point",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_inflection_point);//当前拐点
                cJSON_AddNumberToObject(root, "current_station",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_station);//当前场站
                cJSON_AddNumberToObject(root, "current_field_coordinate",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_field_coordinate);//当前场坐标
                cJSON_AddNumberToObject(root, "direction",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_direction);  //0:上行 1:下行
                cJSON_AddStringToObject(root, "longitude",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_longitude.toUtf8().data());//经度
                cJSON_AddStringToObject(root, "latitude",CCommonData::getInstance()->m_CSystemCoordinateCollection.m_latitude.toUtf8().data());//纬度
                cJSON_AddNumberToObject(root, "azimuth", CCommonData::getInstance()->m_CSystemCoordinateCollection.m_azimuth);//方位角
                cJSON_AddNumberToObject(root, "speed", CCommonData::getInstance()->m_CSystemCoordinateCollection.m_speed);//时速

                cJSON_AddNumberToObject(root, "button_type", _type1);

            }break;
            case kEnumGet:
            {
                cJSON_AddNumberToObject(root, "type", 2); //获取数据
                cJSON_AddNumberToObject(root, "get_type", _type1);
            }break;
        }


        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

//系统管理-录像查询
void CProtocol::call_json_system_videotape_query_set(char _type,void *_param)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        cJSON_AddStringToObject(root, "msg_type", "gui_system_videotape_query");

        switch(_type)
        {
            case CSystemVideotapeQueryEx::kEnumGetDate:  //获取年月数据
            {
                //CSystemVideotapeQuery *pSystemVideotapeQuery = (CSystemVideotapeQuery *)_param;

                cJSON_AddNumberToObject(root, "type", 1);

            }break;
            case CSystemVideotapeQueryEx::kEnumGetMonth: //获取月信息
            {
                VideotapeQueryDate *pVideotapeQueryDate = (VideotapeQueryDate *)_param;

                cJSON_AddNumberToObject(root, "type", 2);
                cJSON_AddNumberToObject(root, "position", pVideotapeQueryDate->m_position); //位置  0:硬盘 1:EMMC 2:SD卡
                cJSON_AddNumberToObject(root, "year", pVideotapeQueryDate->m_year);     //年
                cJSON_AddNumberToObject(root, "month", pVideotapeQueryDate->m_month);   //月


            }break;
            case CSystemVideotapeQueryEx::kEnumGetDayList://获取列表信息
            {
                VideotapeQueryList *pVideotapeQueryList = (VideotapeQueryList *)_param;

                cJSON_AddNumberToObject(root, "type", 3);
                cJSON_AddNumberToObject(root, "position", pVideotapeQueryList->m_position);//位置  0:硬盘 1:EMMC 2:SD卡
                cJSON_AddNumberToObject(root, "year", pVideotapeQueryList->m_year);//年
                cJSON_AddNumberToObject(root, "month", pVideotapeQueryList->m_month);//月
                cJSON_AddNumberToObject(root, "day", pVideotapeQueryList->m_day);//日

                cJSON_AddNumberToObject(root, "start_hour", pVideotapeQueryList->start_hour);//日
                cJSON_AddNumberToObject(root, "start_minute", pVideotapeQueryList->start_minute);//日
                cJSON_AddNumberToObject(root, "start_second", pVideotapeQueryList->start_second);//日
                cJSON_AddNumberToObject(root, "end_hour", pVideotapeQueryList->end_hour);//日
                cJSON_AddNumberToObject(root, "end_minute", pVideotapeQueryList->end_minute);//日
                cJSON_AddNumberToObject(root, "end_second", pVideotapeQueryList->end_second);//日
                cJSON_AddNumberToObject(root, "channel", pVideotapeQueryList->channel);//日


            }break;
            case CSystemVideotapeQueryEx::kEnumPlayBack:  //回放
            {
                CSystemPlayBack *pCSystemPlayBack = (CSystemPlayBack *)_param;
                cJSON_AddNumberToObject(root, "type", 4);


                switch(pCSystemPlayBack->m_operate_type)
                {
                    case CSystemPlayBack::kEnumPrevPassageway://上一个通道
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 1);
                    }break;
                    case CSystemPlayBack::kEnumPrevVideo:     //上一个视频 1
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 2);
                    }break;
                    case CSystemPlayBack::kEnumPrevPlay:      //后退 1
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 3);
                    }break;
                    case CSystemPlayBack::kEnumStart:         //播放
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 4);
                    }break;
                    case CSystemPlayBack::kEnumStop:          //停止
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 5);
                    }break;
                    case CSystemPlayBack::kEnumBackPlay:      //前进 1
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 6);
                    }break;
                    case CSystemPlayBack::kEnumBackVideo:     //下一个视频 1
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 7);
                    }break;
                    case CSystemPlayBack::kEnumNextPassageway: //下一个通道
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 8);
                    }break;
                    case CSystemPlayBack::kEnumRefreshPlayList: //刷新播放列表
                    {
                        cJSON_AddNumberToObject(root, "operate_type", 9);
                        int array_size = pCSystemPlayBack->m_play_list.size();
                        cJSON *pJson_rows = cJSON_CreateArray();
                        cJSON_AddItemToObject(root, "rows",pJson_rows);

                        cJSON *pJson_row = NULL;

                        for(int index = 0;index < array_size;index++)  /*播放文件列表*/
                        {
                            pJson_row = cJSON_CreateObject();
                            cJSON_AddItemToArray(pJson_rows,pJson_row);
                            cJSON_AddNumberToObject(pJson_row, "value",pCSystemPlayBack->m_play_list.at(index));
                        }

                    }break;
                    case CSystemPlayBack::kEnumVoice:  //不静音
                    {
                        cJSON_AddNumberToObject(root, "operate_type",10);
                    }break;
                    case CSystemPlayBack::kEnumNoVoice: //静音
                    {
                        cJSON_AddNumberToObject(root, "operate_type",11);
                    }break;
                    case CSystemPlayBack::kEnumExit: //退出
                    {
                        cJSON_AddNumberToObject(root, "operate_type",12);
                    }break;

                }


            }break;
            case CSystemVideotapeQueryEx::kEnumExport:     //导出
            {
                CSystemVideoExport *pCSystemVideoExport = (CSystemVideoExport *)_param;

                cJSON_AddNumberToObject(root, "type", 5);
                cJSON_AddNumberToObject(root, "export_type", pCSystemVideoExport->m_export_type);//1:原始数据  2:AVI数据
                cJSON_AddNumberToObject(root, "operate_type", pCSystemVideoExport->m_operate_type);//1:导出数据  2:取消导出

                cJSON_AddNumberToObject(root, "year", pCSystemVideoExport->m_year);//年
                cJSON_AddNumberToObject(root, "month", pCSystemVideoExport->m_month);//月
                cJSON_AddNumberToObject(root, "day", pCSystemVideoExport->m_day);//日

                cJSON_AddNumberToObject(root, "start_hour", pCSystemVideoExport->start_hour);
                cJSON_AddNumberToObject(root, "start_minute", pCSystemVideoExport->start_minute);//
                cJSON_AddNumberToObject(root, "start_second", pCSystemVideoExport->start_second);//
                cJSON_AddNumberToObject(root, "end_hour", pCSystemVideoExport->end_hour);//
                cJSON_AddNumberToObject(root, "end_minute", pCSystemVideoExport->end_minute);//
                cJSON_AddNumberToObject(root, "end_second", pCSystemVideoExport->end_second);//
                cJSON_AddNumberToObject(root, "channel", pCSystemVideoExport->channel);//

                if(1 == pCSystemVideoExport->m_operate_type)
                {
                    int array_size = pCSystemVideoExport->m_list.size();
                    cJSON *pJson_rows = cJSON_CreateArray();
                    cJSON_AddItemToObject(root, "rows",pJson_rows);

                    cJSON *pJson_row = NULL;

                    for(int index = 0;index < array_size;index++)  /*导出文件列表*/
                    {
                        pJson_row = cJSON_CreateObject();
                        cJSON_AddItemToArray(pJson_rows,pJson_row);
                        cJSON_AddNumberToObject(pJson_row, "number",pCSystemVideoExport->m_list.at(index));
                    }
                }


            }break;
        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}


//系统管理-存储管理
void CProtocol::call_json_system_storage_manage_set(char _type,char _format)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_storage_manage");

        switch(_type)
        {
            case 1://获取数据
            {
                cJSON_AddNumberToObject(root, "type", 1);

            }break;
            case 2://格式化
            {
                cJSON_AddNumberToObject(root, "type", 2);
                cJSON_AddNumberToObject(root, "format", _format);
            }break;
            case 3://用户停止格式化
            {
                cJSON_AddNumberToObject(root, "type", 3);
                cJSON_AddNumberToObject(root, "format", _format);

            }break;
        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}
//系统操作-关机(重启)
void CProtocol::call_json_system_operate_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_operate");

        switch(_type)
        {
            case 1://重启
            {
                cJSON_AddNumberToObject(root, "type", 1);

            }break;
            case 2://关机
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;

        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}
//系统管理-网络
void CProtocol::call_json_system_network_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_network");

        switch(_type)
        {
            case kEnumSave: //保存数据
            {
                cJSON_AddNumberToObject(root, "type", 1);

                //1:移动网络
                cJSON_AddNumberToObject(root, "mobile_network_enable", CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->m_enable);
                cJSON_AddNumberToObject(root, "mobile_network_operator1", CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->operator1);
                cJSON_AddNumberToObject(root, "mobile_network_type", CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->network_type);
                cJSON_AddNumberToObject(root, "mobile_network_month_end_date", CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->month_end_date);
                cJSON_AddNumberToObject(root, "mobile_network_use_vpn", CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->use_vpn);
                cJSON_AddStringToObject(root, "mobile_network_vpn_name", CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_name.toUtf8().data());
                cJSON_AddStringToObject(root, "mobile_network_vpn_account", CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_account.toUtf8().data());
                cJSON_AddStringToObject(root, "mobile_network_vpn_password", CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_password.toUtf8().data());

                //2:中心服务器1
                cJSON_AddNumberToObject(root, "center_server1_enable", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_enable);
                cJSON_AddNumberToObject(root, "center_server1_protocol_type", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->protocol_type);
                cJSON_AddNumberToObject(root, "center_server1_access_mode", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->access_mode);
                cJSON_AddStringToObject(root, "center_server1_ip", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_ip.toUtf8().data());
                cJSON_AddNumberToObject(root, "center_server1_port", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_port);
                cJSON_AddNumberToObject(root, "center_server1_data_send_interval", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_data_send_interval);

                //3:中心服务器2
                cJSON_AddNumberToObject(root, "center_server2_enable", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_enable);
                cJSON_AddNumberToObject(root, "center_server2_protocol_type", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->protocol_type);
                cJSON_AddNumberToObject(root, "center_server2_access_mode", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->access_mode);
                cJSON_AddStringToObject(root, "center_server2_ip", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_ip.toUtf8().data());
                cJSON_AddNumberToObject(root, "center_server2_port", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_port);
                cJSON_AddNumberToObject(root, "center_server2_data_send_interval", CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_data_send_interval);


                //4:视频服务器
                cJSON_AddNumberToObject(root, "video_server_enable", CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_enable);
                cJSON_AddNumberToObject(root, "video_server_protocol_type", CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->protocol_type);
                cJSON_AddNumberToObject(root, "video_server_access_mode", CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->access_mode);
                cJSON_AddStringToObject(root, "video_server_ip", CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_ip.toUtf8().data());
                cJSON_AddNumberToObject(root, "video_server_port", CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_port);

                //5:本地网络
                cJSON_AddNumberToObject(root, "local_network_enable", CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_enable);
                cJSON_AddNumberToObject(root, "local_network_dhcp_server", CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_dhcp_server);
                cJSON_AddNumberToObject(root, "local_network_access_mode", CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->access_mode);
                cJSON_AddStringToObject(root, "local_network_ip", CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_ip.toUtf8().data());
                cJSON_AddStringToObject(root, "local_network_subnet_mask", CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->subnet_mask.toUtf8().data());
                cJSON_AddStringToObject(root, "local_network_gateway", CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->gateway.toUtf8().data());
                cJSON_AddStringToObject(root, "local_network_nds1", CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->nds1.toUtf8().data());
                cJSON_AddStringToObject(root, "local_network_nds2", CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->nds2.toUtf8().data());

                //6:WIFI
                cJSON_AddNumberToObject(root, "wifi_enable", CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_enable);
                cJSON_AddNumberToObject(root, "wifi_access_mode", CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->access_mode);
                cJSON_AddStringToObject(root, "wifi_ip", CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_ip.toUtf8().data());
                cJSON_AddStringToObject(root, "wifi_subnet_mask", CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->subnet_mask.toUtf8().data());
                cJSON_AddStringToObject(root, "wifi_gateway", CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->gateway.toUtf8().data());
                cJSON_AddStringToObject(root, "wifi_nds1", CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->nds1.toUtf8().data());
                cJSON_AddStringToObject(root, "wifi_nds2", CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->nds2.toUtf8().data());

                //7:FTP
                cJSON_AddNumberToObject(root, "ftp_enable", CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_enable);
                cJSON_AddNumberToObject(root, "ftp_access_mode", CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->access_mode);
                cJSON_AddStringToObject(root, "ftp_ip", CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_ip.toUtf8().data());
                cJSON_AddStringToObject(root, "ftp_gateway", CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->gateway.toUtf8().data());

            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;
        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

//系统管理-网络-WIFI
void CProtocol::call_json_system_wifi_set(char _type,void *_param)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_wifi");

        switch(_type)
        {
            case kEnumWifiInfo://获取WIFI信息
            {
                cJSON_AddNumberToObject(root, "type", 1);

            }break;
            case kEnumWifiConnect://连接WIFI
            {
                CWifiConnectInfo *pCWifiConnectInfo = (CWifiConnectInfo *)_param;

                cJSON_AddNumberToObject(root, "type", 2);
                cJSON_AddStringToObject(root, "ssid_name", pCWifiConnectInfo->m_ssid_name.toUtf8().data());//SSID名称
                cJSON_AddNumberToObject(root, "security", pCWifiConnectInfo->m_security);//安全性 0:WEP  1:WPA-PSK  2:WPA2-PSK  3:WAPI
                cJSON_AddStringToObject(root, "password", pCWifiConnectInfo->m_password.toUtf8().data());//密码

            }break;
            case kEnumWifiStop://停止WIFI连接
            {
                cJSON_AddNumberToObject(root, "type", 3);

            }break;

        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

//系统管理-I/O配置
void CProtocol::call_json_system_switch_config_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_switch_config");

        switch(_type)
        {
            case kEnumSave: //保存数据
            {
                cJSON_AddNumberToObject(root, "type", 1);
                //车门检测
                cJSON_AddNumberToObject(root, "enable", CCommonData::getInstance()->m_CSystemIoConfig.m_enable);// 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "car_door_number", CCommonData::getInstance()->m_CSystemIoConfig.m_car_door_number);//车门数量 0:1 1:2 2:3
                cJSON_AddNumberToObject(root, "car_door_type", CCommonData::getInstance()->m_CSystemIoConfig.m_car_door_type);//车门类型  0:两线气动门　1:传感器门　2:不检测
                cJSON_AddNumberToObject(root, "door_front", CCommonData::getInstance()->m_CSystemIoConfig.m_door_front);//开前门电平 0:低电平有效　1:高电平有效
                cJSON_AddNumberToObject(root, "door_middle", CCommonData::getInstance()->m_CSystemIoConfig.m_door_middle);//开中门电平 0:低电平有效　1:高电平有效
                cJSON_AddNumberToObject(root, "door_back", CCommonData::getInstance()->m_CSystemIoConfig.m_door_back);//开后门电平 0:低电平有效　1:高电平有效
                //车门检测－配置
                cJSON_AddNumberToObject(root, "speed_open_door_check", CCommonData::getInstance()->m_CSystemIoConfig.speed_open_door_check);//带速度开门检测
                cJSON_AddNumberToObject(root, "station_open_door_check", CCommonData::getInstance()->m_CSystemIoConfig.station_open_door_check);//站间开门检测
                cJSON_AddNumberToObject(root, "arrive_open_door_check", CCommonData::getInstance()->m_CSystemIoConfig.arrive_open_door_check);//到站未开门检测
                cJSON_AddNumberToObject(root, "arrive_stop_door_check", CCommonData::getInstance()->m_CSystemIoConfig.arrive_stop_door_check);//到站未停车检测
                cJSON_AddNumberToObject(root, "speed_enable", CCommonData::getInstance()->m_CSystemIoConfig.m_speed_enable);//异常开关门最小速度使能 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "speed", CCommonData::getInstance()->m_CSystemIoConfig.m_speed);//异常开关门最小速度
                cJSON_AddNumberToObject(root, "frequency_enable", CCommonData::getInstance()->m_CSystemIoConfig.m_frequency_enable);//异常开关门提醒使能 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "frequency", CCommonData::getInstance()->m_CSystemIoConfig.m_frequency);//异常开关门提醒次数
                cJSON_AddNumberToObject(root, "io_input1", CCommonData::getInstance()->m_CSystemIoConfig.m_io_input1);//I/O输入1 0:低电平有效　1:高电平有效
                //其他开关量检测
                cJSON_AddNumberToObject(root, "back_car_monitor_enable", CCommonData::getInstance()->m_CSystemIoConfig.back_car_monitor_enable);//倒车监视 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "back_car_monitor_status", CCommonData::getInstance()->m_CSystemIoConfig.back_car_monitor_status);//倒车监视 0:低电平有效　1:高电平有效
                cJSON_AddNumberToObject(root, "no_brake_enable", CCommonData::getInstance()->m_CSystemIoConfig.no_brake_enable);//未刹车 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "no_brake_status", CCommonData::getInstance()->m_CSystemIoConfig.no_brake_status);//未刹车 0:低电平有效　1:高电平有效
                cJSON_AddNumberToObject(root, "open_front_box_enable", CCommonData::getInstance()->m_CSystemIoConfig.open_front_box_enable);//开前箱 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "open_front_box_status", CCommonData::getInstance()->m_CSystemIoConfig.open_front_box_status);//开前箱 0:低电平有效　1:高电平有效
                cJSON_AddNumberToObject(root, "wiper_enable", CCommonData::getInstance()->m_CSystemIoConfig.wiper_enable);//雨刮 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "wiper_status", CCommonData::getInstance()->m_CSystemIoConfig.wiper_status);//雨刮 0:低电平有效　1:高电平有效
                cJSON_AddNumberToObject(root, "left_lamp_enable", CCommonData::getInstance()->m_CSystemIoConfig.left_lamp_enable);//左转灯 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "left_lamp_status", CCommonData::getInstance()->m_CSystemIoConfig.left_lamp_status);//左转灯 0:低电平有效　1:高电平有效
                cJSON_AddNumberToObject(root, "right_lamp_enable", CCommonData::getInstance()->m_CSystemIoConfig.right_lamp_enable);//右转灯 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "right_lamp_status", CCommonData::getInstance()->m_CSystemIoConfig.right_lamp_status);//右转灯 0:低电平有效　1:高电平有效

            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;
        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}
//系统管理-高级设置
void CProtocol::call_json_system_senior_setup_set(char _type,void *_param)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_senior_setup");

        switch(_type)
        {
            case kEnumSave: //保存数据
            {
                cJSON_AddNumberToObject(root, "type", 1);

                //通用配置
                cJSON_AddNumberToObject(root, "location_mode", CCommonData::getInstance()->m_CSystemSeniorSetup.location_mode); //定位方式 0:混合定位 1:北斗定位 2:GPS定位
                cJSON_AddNumberToObject(root, "custom_navigation_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.custom_navigation_enable);//惯性导航使能
                //惯性导航-配置(start)
                cJSON_AddNumberToObject(root, "corner_x", CCommonData::getInstance()->m_CSystemSeniorSetup.corner_x); //设备X轴相对车X轴转角
                cJSON_AddNumberToObject(root, "corner_y", CCommonData::getInstance()->m_CSystemSeniorSetup.corner_y);//设备Y轴相对车Y轴转角
                cJSON_AddNumberToObject(root, "corner_z", CCommonData::getInstance()->m_CSystemSeniorSetup.corner_z); //设备Z轴相对车Z轴转角
                cJSON_AddNumberToObject(root, "corner_gain", CCommonData::getInstance()->m_CSystemSeniorSetup.corner_gain); //转弯Gain值
                //惯性导航-配置(end)
                cJSON_AddNumberToObject(root, "station_mode", CCommonData::getInstance()->m_CSystemSeniorSetup.m_station_mode); //报站方式 0:自动 1:手动
                cJSON_AddNumberToObject(root, "in_station_radius", CCommonData::getInstance()->m_CSystemSeniorSetup.in_station_radius);//进站半径
                cJSON_AddNumberToObject(root, "out_station_radius", CCommonData::getInstance()->m_CSystemSeniorSetup.out_station_radius);//出站半径
                cJSON_AddNumberToObject(root, "inflection_point_radius", CCommonData::getInstance()->m_CSystemSeniorSetup.inflection_point_radius);//拐点半径
                cJSON_AddNumberToObject(root, "over_speed_still_tip", CCommonData::getInstance()->m_CSystemSeniorSetup.over_speed_still_tip); //超速保持提醒
                cJSON_AddNumberToObject(root, "over_speed_tip_times", CCommonData::getInstance()->m_CSystemSeniorSetup.over_speed_tip_times);//超速提醒次数
                cJSON_AddNumberToObject(root, "non_operating_speed_limit", CCommonData::getInstance()->m_CSystemSeniorSetup.non_operating_speed_limit);//非运营限速
                cJSON_AddNumberToObject(root, "operating_mode", CCommonData::getInstance()->m_CSystemSeniorSetup.operating_mode);  //0:营运　1:非营运

                //告警使能
                cJSON_AddNumberToObject(root, "urgent_acceleration_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.urgent_acceleration_enable);//紧急加速使能 // 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "urgent_acceleration_value", CCommonData::getInstance()->m_CSystemSeniorSetup.urgent_acceleration_value);//紧急加速值  (km/s)
                cJSON_AddNumberToObject(root, "emergency_deceleration_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.emergency_deceleration_enable);//紧急减速使能 // 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "emergency_deceleration_value", CCommonData::getInstance()->m_CSystemSeniorSetup.emergency_deceleration_value);//紧急减速值  (km/s)
                cJSON_AddNumberToObject(root, "hysteresis_alarm_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.hysteresis_alarm_enable);//滞站报警使能 // 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "hysteresis_alarm_value", CCommonData::getInstance()->m_CSystemSeniorSetup.hysteresis_alarm_value);//滞站报警值
                cJSON_AddNumberToObject(root, "skip_alarm_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.skip_alarm_enable);//越站报警使能 // 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "skip_alarm_value", CCommonData::getInstance()->m_CSystemSeniorSetup.skip_alarm_value);//越站报警值
                cJSON_AddNumberToObject(root, "fatigue_driving_check_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.fatigue_driving_check_enable);//疲劳驾驶检测使能 // 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "fatigue_driving_check_value", CCommonData::getInstance()->m_CSystemSeniorSetup.fatigue_driving_check_value);//疲劳驾驶检测值
                cJSON_AddNumberToObject(root, "idling_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.idling_enable);//怠速使能使能 // 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "idling_value", CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value);//怠速使能值
                //CAN设置
                cJSON_AddNumberToObject(root, "can1_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.can1_enable);//CAN1  0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "can2_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.can2_enable);//CAN2  0:未选择状态 1:选中状态
                //高级设置
                cJSON_AddNumberToObject(root, "volumn_adjustment", CCommonData::getInstance()->m_CSystemSeniorSetup.volumn_adjustment);//音量调整使能 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "idling_enable1", CCommonData::getInstance()->m_CSystemSeniorSetup.idling_enable1);//怠速使能 0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "idling_value1", CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value1);//怠速值1
                cJSON_AddNumberToObject(root, "idling_value2", CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value2);//怠速值2
                cJSON_AddNumberToObject(root, "temperature_check_enable", CCommonData::getInstance()->m_CSystemSeniorSetup.temperature_check_enable); //温度检测使能
                //温度检测-配置
                cJSON_AddNumberToObject(root, "in_temperature_security1", CCommonData::getInstance()->m_CSystemSeniorSetup.in_temperature_security1); //内温安全范围1
                cJSON_AddNumberToObject(root, "in_temperature_security2", CCommonData::getInstance()->m_CSystemSeniorSetup.in_temperature_security2);//内温安全范围2
                cJSON_AddNumberToObject(root, "out_temperature_security1", CCommonData::getInstance()->m_CSystemSeniorSetup.out_temperature_security1);//外温安全范围1
                cJSON_AddNumberToObject(root, "out_temperature_security2", CCommonData::getInstance()->m_CSystemSeniorSetup.out_temperature_security2);//外温安全范围2
                cJSON_AddNumberToObject(root, "police_upload_time", CCommonData::getInstance()->m_CSystemSeniorSetup.police_upload_time); //报警上传时间间隔


            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;
            case 3:  //can1
            {
                cJSON_AddNumberToObject(root, "type", 3);
                cJSON_AddStringToObject(root, "command", CCommonData::getInstance()->m_CCanInfo1.m_command.toUtf8().data());

            }break;
            case 4:  //can2
            {
                cJSON_AddNumberToObject(root, "type", 4);
                cJSON_AddStringToObject(root, "command", CCommonData::getInstance()->m_CCanInfo2.m_command.toUtf8().data());

            }break;

        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

//系统管理-系统设置
void CProtocol::call_json_system_system_setup_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_system_setup");

        switch(_type)
        {
            case kEnumSave: //保存数据
            {
                cJSON_AddNumberToObject(root, "type", 1);

                //通用配置
                cJSON_AddNumberToObject(root, "screen_mode", CCommonData::getInstance()->m_CSystemSystemSetup.screen_mode); //屏幕模式 0:高清模式 1:标清模式
                cJSON_AddNumberToObject(root, "drusb_mode", CCommonData::getInstance()->m_CSystemSystemSetup.drusb_mode);  //DRUSB接口模式 0:标准 1:灾备
                cJSON_AddNumberToObject(root, "sleep_shutdown_enable", CCommonData::getInstance()->m_CSystemSystemSetup.sleep_shutdown_enable);//延时关机使能
                cJSON_AddNumberToObject(root, "sleep_shutdown_value", CCommonData::getInstance()->m_CSystemSystemSetup.sleep_shutdown_value); //延时关机值　(min)
                cJSON_AddNumberToObject(root, "device_door_report_enable", CCommonData::getInstance()->m_CSystemSystemSetup.device_door_report_enable);//设备门开启上报间隔使能
                cJSON_AddNumberToObject(root, "device_door_report_value", CCommonData::getInstance()->m_CSystemSystemSetup.device_door_report_value);//设备门开启上报间隔值
                cJSON_AddNumberToObject(root, "hard_disk_heating_enable", CCommonData::getInstance()->m_CSystemSystemSetup.hard_disk_heating_enable);//硬盘加热使能
                cJSON_AddNumberToObject(root, "hard_disk_heating_value", CCommonData::getInstance()->m_CSystemSystemSetup.hard_disk_heating_value);//硬盘加热值
                cJSON_AddNumberToObject(root, "now_time_enable", CCommonData::getInstance()->m_CSystemSystemSetup.now_time_enable);//整点报时使能
                cJSON_AddNumberToObject(root, "now_time_value1", CCommonData::getInstance()->m_CSystemSystemSetup.now_time_value1);//整点报时值1   h
                cJSON_AddNumberToObject(root, "now_time_value2", CCommonData::getInstance()->m_CSystemSystemSetup.now_time_value2);//整点报时值2   h
                cJSON_AddNumberToObject(root, "voltage_enable", CCommonData::getInstance()->m_CSystemSystemSetup.voltage_enable);//超电压反警使能
                cJSON_AddNumberToObject(root, "voltage_value1", CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value1);//超电压反警值1   V
                cJSON_AddNumberToObject(root, "voltage_value2", CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value2);//超电压反警值2   V
                cJSON_AddNumberToObject(root, "voltage_value3", CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value3);//超电压反警值3   min
                cJSON_AddNumberToObject(root, "temperature_enable", CCommonData::getInstance()->m_CSystemSystemSetup.temperature_enable);//超温度报警使能
                cJSON_AddNumberToObject(root, "temperature_value1", CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value1);//超温度报警值1   V
                cJSON_AddNumberToObject(root, "temperature_value2", CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value2);//超温度报警值2   V
                cJSON_AddNumberToObject(root, "temperature_value3", CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value3);//超温度报警值3   min
                //add
                cJSON_AddNumberToObject(root, "fan_control_enable", CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_enable);//风扇控制使能
                cJSON_AddNumberToObject(root, "fan_control_value1", CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_value1);//风扇控制值1
                cJSON_AddNumberToObject(root, "fan_control_value2", CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_value2);//风扇控制值2
                cJSON_AddNumberToObject(root, "picutre_play_time", CCommonData::getInstance()->m_CSystemSystemSetup.picutre_play_time);//图片轮播
                cJSON_AddNumberToObject(root, "volumn_size", CCommonData::getInstance()->m_CSystemSystemSetup.volumn_size);//声音大小

                cJSON_AddNumberToObject(root, "current_time_zone", CCommonData::getInstance()->m_CSystemSystemSetup.m_current_time_zone);//当前选择的时区
                //系统时间
                cJSON_AddStringToObject(root, "time1", CCommonData::getInstance()->m_CSystemSystemSetup.m_time1.toUtf8().data()); //时间1
                cJSON_AddStringToObject(root, "time2", CCommonData::getInstance()->m_CSystemSystemSetup.m_time2.toUtf8().data());//时间2
                cJSON_AddNumberToObject(root, "satellite_time_enable", CCommonData::getInstance()->m_CSystemSystemSetup.satellite_time_enable);//卫星校时  0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "center_time_enable", CCommonData::getInstance()->m_CSystemSystemSetup.center_time_enable);//中心校时  0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "ntp_enable", CCommonData::getInstance()->m_CSystemSystemSetup.ntp_enable);//NTP校时使能  0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "ntp_value", CCommonData::getInstance()->m_CSystemSystemSetup.ntp_value); //NTP校时值 0:time.windows.com 1:time.nist.gov 2:time-nw.nist.gov 3:time-a.nist.gov 4:time-b.nist.gov

                cJSON_AddNumberToObject(root, "user_set_time",CCommonData::getInstance()->m_CSystemSystemSetup.user_set_time); //用户设置时间　１：设置 0：不设置
                cJSON_AddNumberToObject(root, "tm_year",CCommonData::getInstance()->m_CSystemSystemSetup.tm_year);
                cJSON_AddNumberToObject(root, "tm_mon",CCommonData::getInstance()->m_CSystemSystemSetup.tm_mon);
                cJSON_AddNumberToObject(root, "tm_mday",CCommonData::getInstance()->m_CSystemSystemSetup.tm_mday);
                cJSON_AddNumberToObject(root, "tm_hour",CCommonData::getInstance()->m_CSystemSystemSetup.tm_hour);
                cJSON_AddNumberToObject(root, "tm_min",CCommonData::getInstance()->m_CSystemSystemSetup.tm_min);
                cJSON_AddNumberToObject(root, "tm_sec",CCommonData::getInstance()->m_CSystemSystemSetup.tm_sec);

                //账户设置
                cJSON_AddNumberToObject(root, "use_password", CCommonData::getInstance()->m_CSystemSystemSetup.use_password);//使用密码  0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "auto_lock_enable", CCommonData::getInstance()->m_CSystemSystemSetup.auto_lock_enable);//自动锁定使能  0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "auto_lock_value", CCommonData::getInstance()->m_CSystemSystemSetup.auto_lock_value); //自动锁定值(min)  0:1  1:2  2:3  3:5  4:10
                cJSON_AddNumberToObject(root, "timeout_exit_enable", CCommonData::getInstance()->m_CSystemSystemSetup.timeout_exit_enable); //超时退出使能  0:未选择状态 1:选中状态
                cJSON_AddNumberToObject(root, "timeout_exit_value", CCommonData::getInstance()->m_CSystemSystemSetup.timeout_exit_value);//超时退出值(min) 0:1  1:2  2:3  3:5  4:10


            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;


        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

//系统管理-系统维护
void CProtocol::call_json_system_maintain_set(char _type,char _type_main,char _behavior_operate,QString _filename)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_maintain");

        switch(_type)
        {

            case kEnumSystemMaintainList: //获取列表数据
            {
                cJSON_AddNumberToObject(root, "type", 1);

                if(CSystemSystemMaintain::kEnumApplicationProgram == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 1);//应用程序
                else if(CSystemSystemMaintain::kEnumMcuProgram == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 2);//MCU程序
                else if(CSystemSystemMaintain::kEnumUBootProgram == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 3);//UBOOT程序
                //报站文件
                else if(CSystemSystemMaintain::kEnumImportStationFile == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 4);//导入报站文件
                //高级
                else if(CSystemSystemMaintain::kEnumImportLogoFile == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 5);//导入开机LOGO文件

            }break;
            case kEnumSystemMaintainOperate: //用户操作
            {
                cJSON_AddNumberToObject(root, "type", 2);

                if(CSystemSystemMaintain::kEnumApplicationProgram == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 1);//应用程序
                else if(CSystemSystemMaintain::kEnumMcuProgram == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 2);//MCU程序
                else if(CSystemSystemMaintain::kEnumUBootProgram == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 3);//UBOOT程序
                //报站文件
                else if(CSystemSystemMaintain::kEnumImportStationFile == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 4);//导入报站文件
                //高级
                else if(CSystemSystemMaintain::kEnumImportLogoFile == _type_main)
                    cJSON_AddNumberToObject(root, "main_type", 5);//导入开机LOGO文件
                //报站文件
                else if(CSystemSystemMaintain::kEnumExportStationFile == _type_main)//导出报站文件
                    cJSON_AddNumberToObject(root, "main_type", 6);
                else if(CSystemSystemMaintain::kEnumClearStationFile == _type_main)   //清除报站文件版本
                    cJSON_AddNumberToObject(root, "main_type", 7);
                //日志/记录
                else if(CSystemSystemMaintain::kEnumExportVehicleRecord == _type_main)//导出行车记录
                    cJSON_AddNumberToObject(root, "main_type", 8);
                else if(CSystemSystemMaintain::kEnumClearVehicleRecord == _type_main)//清除行车日志
                    cJSON_AddNumberToObject(root, "main_type", 9);
                else if(CSystemSystemMaintain::kEnumExportSystemLog == _type_main)//导出系统日志
                    cJSON_AddNumberToObject(root, "main_type", 10);
                else if(CSystemSystemMaintain::kEnumClearSystemLog == _type_main)//清除系统日志
                    cJSON_AddNumberToObject(root, "main_type", 11);
                else if(CSystemSystemMaintain::kEnumExportOperationLog == _type_main)//导出操作日志
                    cJSON_AddNumberToObject(root, "main_type", 12);
                else if(CSystemSystemMaintain::kEnumClearOperationLog == _type_main)//清除操作日志
                    cJSON_AddNumberToObject(root, "main_type", 13);
                //高级
                else if(CSystemSystemMaintain::kEnumResumeApplicationProgram == _type_main)//恢复应用程序
                    cJSON_AddNumberToObject(root, "main_type", 14);
                else if(CSystemSystemMaintain::kEnumResumeMcuProgram == _type_main)//恢复MCU程序
                    cJSON_AddNumberToObject(root, "main_type", 15);
                else if(CSystemSystemMaintain::kEnumResumeUBootProgram == _type_main)//恢复UBoot程序
                    cJSON_AddNumberToObject(root, "main_type", 16);
                else if(CSystemSystemMaintain::kEnumResumeFactoryConfig == _type_main)//恢复出厂设置
                    cJSON_AddNumberToObject(root, "main_type", 17);
                else if(CSystemSystemMaintain::kEnumImportPicture == _type_main)//导入广告图片
                    cJSON_AddNumberToObject(root, "main_type", 18);
                else if(CSystemSystemMaintain::kEnumExportPicture == _type_main)//导出广告图片
                    cJSON_AddNumberToObject(root, "main_type", 19);
                else if(CSystemSystemMaintain::kEnumCleanPicture == _type_main)//清除广告图片
                    cJSON_AddNumberToObject(root, "main_type", 20);
                else if(CSystemSystemMaintain::kEnumImportCardParam == _type_main)  //导入刷卡参数
                    cJSON_AddNumberToObject(root, "main_type", 21);
                else if(CSystemSystemMaintain::kEnumExportCardParam == _type_main)  //导出刷卡参数
                    cJSON_AddNumberToObject(root, "main_type", 22);
                else if(CSystemSystemMaintain::kEnumCleanCardParam == _type_main)   //清除刷卡参数
                    cJSON_AddNumberToObject(root, "main_type", 23);
                else if(CSystemSystemMaintain::kEnumCleanCardRecord == _type_main)  //清除刷卡记录
                    cJSON_AddNumberToObject(root, "main_type", 24);
                else if(CSystemSystemMaintain::kEnumFormatStationFile == _type_main)//格式化报站分区
                    cJSON_AddNumberToObject(root, "main_type", 25);


                cJSON_AddNumberToObject(root, "behavior_operate", _behavior_operate);
                cJSON_AddStringToObject(root, "file_path", _filename.toUtf8().data());

            }break;



        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

//系统管理-CAM配置
void CProtocol::call_json_system_camera_config(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_camera_config");

        switch(_type)
        {
            case kEnumSave: //保存数据
            {
                cJSON_AddNumberToObject(root, "type", 1);

                //通用设置---------------
                cJSON_AddNumberToObject(root, "videotape_standard", CCommonData::getInstance()->m_CSystemCameraConfig.videotape_standard);//录像制式  0:PAL 1:NTSC
                cJSON_AddNumberToObject(root, "auto_cover", CCommonData::getInstance()->m_CSystemCameraConfig.auto_cover);//自动覆盖 0:是  1:否
                cJSON_AddNumberToObject(root, "videotape_lock", CCommonData::getInstance()->m_CSystemCameraConfig.videotape_lock);//录像锁定　0:1 1:2 2:3 3:4 4:5 5:6 6:7
                cJSON_AddNumberToObject(root, "videotape_prev", CCommonData::getInstance()->m_CSystemCameraConfig.videotape_prev);//录像预录    min
                cJSON_AddNumberToObject(root, "network_transmission_mode", CCommonData::getInstance()->m_CSystemCameraConfig.network_transmission_mode);//网传模式  0:流畅+清晰 1:非常清晰 2:清晰 3:流畅 4:非常流畅
                cJSON_AddNumberToObject(root, "videotape_storage", CCommonData::getInstance()->m_CSystemCameraConfig.videotape_storage);//录像存储器 0:EMMC+外置SD卡 1:EMMC  2:外置SD卡
                cJSON_AddNumberToObject(root, "videotape_mode", CCommonData::getInstance()->m_CSystemCameraConfig.videotape_mode);//录像模式  0:镜像录像  1:报警备份录像 2:子码流录像
                //配置－镜像通道


                int array_size = CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.size();
                cJSON *pJson_rows = cJSON_CreateArray();
                cJSON_AddItemToObject(root, "rows",pJson_rows);

                cJSON *pJson_row = NULL;

                for(int index = 0;index < array_size;index++)
                {
                    pJson_row = cJSON_CreateObject();
                    cJSON_AddItemToArray(pJson_rows,pJson_row);
                    cJSON_AddNumberToObject(pJson_row, "value",CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(index));
                }


                //显示设置---------------
                cJSON_AddNumberToObject(root, "time_show_enable", CCommonData::getInstance()->m_CSystemCameraConfig.time_show_enable);//时间显示使能
                cJSON_AddNumberToObject(root, "time_show_value", CCommonData::getInstance()->m_CSystemCameraConfig.time_show_value);//时间显示值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
                cJSON_AddNumberToObject(root, "passageway_name_enable", CCommonData::getInstance()->m_CSystemCameraConfig.passageway_name_enable);//通道名称使能
                cJSON_AddNumberToObject(root, "passageway_name_value", CCommonData::getInstance()->m_CSystemCameraConfig.passageway_name_value);//通道名称值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
                cJSON_AddNumberToObject(root, "vehicle_number_enable", CCommonData::getInstance()->m_CSystemCameraConfig.vehicle_number_enable);//车辆编号使能
                cJSON_AddNumberToObject(root, "vehicle_number_value", CCommonData::getInstance()->m_CSystemCameraConfig.vehicle_number_value);//车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
                cJSON_AddNumberToObject(root, "coordinate_info_enable", CCommonData::getInstance()->m_CSystemCameraConfig.coordinate_info_enable);//坐标信息使能
                cJSON_AddNumberToObject(root, "coordinate_info_value", CCommonData::getInstance()->m_CSystemCameraConfig.coordinate_info_value);//车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
                cJSON_AddNumberToObject(root, "real_speed_enable", CCommonData::getInstance()->m_CSystemCameraConfig.real_speed_enable);//实时速度使能
                cJSON_AddNumberToObject(root, "real_speed_value", CCommonData::getInstance()->m_CSystemCameraConfig.real_speed_value);//实时速度值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下
                cJSON_AddNumberToObject(root, "station_info_enable", CCommonData::getInstance()->m_CSystemCameraConfig.station_info_enable);//站点信息使能
                cJSON_AddNumberToObject(root, "station_info_value", CCommonData::getInstance()->m_CSystemCameraConfig.station_info_value);//站点信息值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

                //模拟摄像机通道设置---------------
                cJSON_AddNumberToObject(root, "monitor_passageway_number", CCommonData::getInstance()->m_CSystemCameraConfig.monitor_passageway_number);//模拟摄像机使用通道数

                //网络摄像通道设置---------------
                cJSON_AddNumberToObject(root, "network_passageway_number", CCommonData::getInstance()->m_CSystemCameraConfig.network_passageway_number);//网络摄像机使用通道数


            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;

        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}


//系统管理-CAM配置-PON-CAM
void CProtocol::call_json_system_pon_cam(char _type,char _type1,char _number)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_camera_pon_cam");

        switch(_type)
        {
            case kEnumSave: //保存数据
            {
                cJSON_AddNumberToObject(root, "type", 1);


                int array_size = CCommonData::getInstance()->m_CSystemCameraConfig.monitor_passageway_number;
                cJSON *pJson_rows_cam = cJSON_CreateArray();
                cJSON *pJson_row = NULL;

                cJSON_AddItemToObject(root, "cam_rows",pJson_rows_cam);

                CSystemCamInfo *pCamInfoTemp = NULL;

                for(int index = 0;index < array_size;index++) //cam1~cam8
                {
                    pJson_row = cJSON_CreateObject();
                    cJSON_AddItemToArray(pJson_rows_cam,pJson_row);
                    if(0 == index)
                        pCamInfoTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam1;
                    if(1 == index)
                        pCamInfoTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam2;
                    if(2 == index)
                        pCamInfoTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam3;
                    if(3 == index)
                        pCamInfoTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam4;
                    if(4 == index)
                        pCamInfoTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam5;
                    if(5 == index)
                        pCamInfoTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam6;


                    cJSON_AddNumberToObject(pJson_row, "config_sound_recording",pCamInfoTemp->config_sound_recording);//录音  0:开启　　1:关闭
                    cJSON_AddNumberToObject(pJson_row, "resolving_power_main",pCamInfoTemp->resolving_power_main);  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
                    cJSON_AddNumberToObject(pJson_row, "resolving_power_sub",pCamInfoTemp->resolving_power_sub);   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
                    cJSON_AddNumberToObject(pJson_row, "frame_rate_main",pCamInfoTemp->frame_rate_main);       //帧率 主 0~24 对应值是 1~25
                    cJSON_AddNumberToObject(pJson_row, "frame_rate_sub",pCamInfoTemp->frame_rate_sub);        //帧率 子 0~24 对应值是 1~25
                    cJSON_AddNumberToObject(pJson_row, "picture_main",pCamInfoTemp->picture_main);          //画质　主 0:高 1:中 2:低
                    cJSON_AddNumberToObject(pJson_row, "picture_sub",pCamInfoTemp->picture_sub);           //画质　子 0:高 1:中 2:低
                    cJSON_AddNumberToObject(pJson_row, "identification",pCamInfoTemp->identification);        //标识 0:视频录像 1:客流录像
                    cJSON_AddNumberToObject(pJson_row, "image",pCamInfoTemp->m_image);
                }

                array_size = CCommonData::getInstance()->m_CSystemCameraConfig.network_passageway_number;
                cJSON *pJson_rows_pon = cJSON_CreateArray();
                cJSON_AddItemToObject(root, "pon_rows",pJson_rows_pon);


                CSystemPonInfo *pTemp = NULL;
                for(int index = 0;index < array_size;index++) //pon1~pon10
                {
                    pJson_row = cJSON_CreateObject();
                    cJSON_AddItemToArray(pJson_rows_pon,pJson_row);

                    if(0 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon1;
                    if(1 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon2;
                    if(2 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon3;
                    if(3 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon4;
                    if(4 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon5;
                    if(5 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon6;
                    if(6 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon7;
                    if(7 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon8;
                    if(8 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon9;
                    if(9 == index)
                        pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon10;

                    cJSON_AddStringToObject(pJson_row, "config_ip",pTemp->config_ip.toUtf8().data());//IP地址
                    cJSON_AddNumberToObject(pJson_row, "config_sound_recording",pTemp->config_sound_recording);//录音  0:开启　　1:关闭
                    cJSON_AddNumberToObject(pJson_row, "resolving_power_main",pTemp->resolving_power_main);  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
                    cJSON_AddNumberToObject(pJson_row, "resolving_power_sub",pTemp->resolving_power_sub);   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
                    cJSON_AddNumberToObject(pJson_row, "frame_rate_main",pTemp->frame_rate_main);       //帧率 主 0~24 对应值是 1~25
                    cJSON_AddNumberToObject(pJson_row, "frame_rate_sub",pTemp->frame_rate_sub);        //帧率 子 0~24 对应值是 1~25
                    cJSON_AddNumberToObject(pJson_row, "picture_main",pTemp->picture_main);          //画质　主 0:高 1:中 2:低
                    cJSON_AddNumberToObject(pJson_row, "picture_sub",pTemp->picture_sub);           //画质　子 0:高 1:中 2:低
                    cJSON_AddNumberToObject(pJson_row, "identification",pTemp->identification);        //标识 0:IPC摄像机 1:其他设备
                    cJSON_AddNumberToObject(pJson_row, "image",pTemp->m_image);
                }

            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;
            case kEnumDefault: //默认数据
            {
                cJSON_AddNumberToObject(root, "type", 3);
                cJSON_AddNumberToObject(root, "cam_type", _type1); //1:PON   2:CAM
                cJSON_AddNumberToObject(root, "number", _number);//1~10  or 1~8


            }break;
            case kEnumSearch:
            {
                cJSON_AddNumberToObject(root, "type", 4);
                cJSON_AddNumberToObject(root, "cam_type", _type1); //1:PON   2:CAM
                cJSON_AddNumberToObject(root, "number", _number);//1~10  or 1~8

            }break;

        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}


//系统管理-客流统计
void CProtocol::call_json_system_passenger_flow(char _type,char _number,char _cam1_2)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_passenger_flow");

        switch(_type)
        {
            case kEnumSave: //保存数据
            {
                cJSON_AddNumberToObject(root, "type", 1);

                //CAM1-1通道设置

                cJSON_AddNumberToObject(root, "cam1_1_enable", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_enable);//使能  0:不选择 1:选择

                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.is_connect;

                cJSON_AddStringToObject(root, "cam1_1_id", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_id.toUtf8().data()); //ID

           //     CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.param_file_exist;//guoyang

                cJSON_AddNumberToObject(root, "cam1_1_guide", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.guide);//辅助线 0:不选择 1:选择

                cJSON_AddNumberToObject(root, "cam1_1_count_show_mode", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_show_mode);//计数显示模式  0:标准  1:进站
                cJSON_AddNumberToObject(root, "cam1_1_count_rule", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_rule);//计数规则  //0:上进下出  1:下进上出
                cJSON_AddNumberToObject(root, "cam1_1_noise_filter", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.noise_filter);//噪声过滤
                cJSON_AddNumberToObject(root, "cam1_1_sensitivity", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.sensitivity);//灵敏度
                cJSON_AddNumberToObject(root, "cam1_1_count_type", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_type);//计数类型
                cJSON_AddNumberToObject(root, "cam1_1_install_height", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.install_height);//安装高度
                cJSON_AddNumberToObject(root, "cam1_1_filter_height", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.filter_height);//过滤高度
                //CAM1-1通道设置-检测区域
                cJSON_AddNumberToObject(root, "cam1_1_count_line1", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line1);//上计数线1
                cJSON_AddNumberToObject(root, "cam1_1_count_line2", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line2);//下计数线2
                cJSON_AddNumberToObject(root, "cam1_1_count_line3", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line3);//左边界线3
                cJSON_AddNumberToObject(root, "cam1_1_count_line4", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line4);//右边界线4

                //CAM2-1通道设置
                cJSON_AddNumberToObject(root, "cam2_1_enable", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_enable);//使能  0:不选择 1:选择

                //CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.is_connect;

                cJSON_AddStringToObject(root, "cam2_1_id", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_id.toUtf8().data()); //ID

                //CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.param_file_exist;

                cJSON_AddNumberToObject(root, "cam2_1_guide", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.guide);//辅助线 0:不选择 1:选择

                cJSON_AddNumberToObject(root, "cam2_1_count_show_mode", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_show_mode);//计数显示模式  0:标准  1:进站
                cJSON_AddNumberToObject(root, "cam2_1_count_rule", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_rule);//计数规则  //0:上进下出  1:下进上出
                cJSON_AddNumberToObject(root, "cam2_1_noise_filter", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.noise_filter);//噪声过滤
                cJSON_AddNumberToObject(root, "cam2_1_sensitivity", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.sensitivity);//灵敏度
                cJSON_AddNumberToObject(root, "cam2_1_count_type", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_type);//计数类型
                cJSON_AddNumberToObject(root, "cam2_1_install_height", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.install_height);//安装高度
                cJSON_AddNumberToObject(root, "cam2_1_filter_height", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.filter_height);//过滤高度
                //CAM2-1通道设置-检测区域
                cJSON_AddNumberToObject(root, "cam2_1_count_line1", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line1);//上计数线1
                cJSON_AddNumberToObject(root, "cam2_1_count_line2", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line2);//下计数线2
                cJSON_AddNumberToObject(root, "cam2_1_count_line3", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line3);//左边界线3
                cJSON_AddNumberToObject(root, "cam2_1_count_line4", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line4);//右边界线4


                //CAM3-1通道设置
                cJSON_AddNumberToObject(root, "cam3_1_enable", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_enable);//使能  0:不选择 1:选择

                //CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.is_connect;

                cJSON_AddStringToObject(root, "cam3_1_id", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_id.toUtf8().data()); //ID

                //CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.param_file_exist;

                cJSON_AddNumberToObject(root, "cam3_1_guide", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.guide);//辅助线 0:不选择 1:选择

                cJSON_AddNumberToObject(root, "cam3_1_count_show_mode", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_show_mode);//计数显示模式  0:标准  1:进站
                cJSON_AddNumberToObject(root, "cam3_1_count_rule", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_rule);//计数规则  //0:上进下出  1:下进上出
                cJSON_AddNumberToObject(root, "cam3_1_noise_filter", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.noise_filter);//噪声过滤
                cJSON_AddNumberToObject(root, "cam3_1_sensitivity", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.sensitivity);//灵敏度
                cJSON_AddNumberToObject(root, "cam3_1_count_type", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_type);//计数类型
                cJSON_AddNumberToObject(root, "cam3_1_install_height", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.install_height);//安装高度
                cJSON_AddNumberToObject(root, "cam3_1_filter_height", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.filter_height);//过滤高度
                //CAM3-1通道设置-检测区域
                cJSON_AddNumberToObject(root, "cam3_1_count_line1", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line1);//上计数线1
                cJSON_AddNumberToObject(root, "cam3_1_count_line2", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line2);//下计数线2
                cJSON_AddNumberToObject(root, "cam3_1_count_line3", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line3);//左边界线3
                cJSON_AddNumberToObject(root, "cam3_1_count_line4", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line4);//右边界线4

                //CAM4-1通道设置
                cJSON_AddNumberToObject(root, "cam4_1_enable", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_enable);//使能  0:不选择 1:选择

                //CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.is_connect;

                cJSON_AddStringToObject(root, "cam4_1_id", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_id.toUtf8().data()); //ID

                //CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.param_file_exist;

                cJSON_AddNumberToObject(root, "cam4_1_guide", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.guide);//辅助线 0:不选择 1:选择

                cJSON_AddNumberToObject(root, "cam4_1_count_show_mode", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_show_mode);//计数显示模式  0:标准  1:进站
                cJSON_AddNumberToObject(root, "cam4_1_count_rule", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_rule);//计数规则  //0:上进下出  1:下进上出
                cJSON_AddNumberToObject(root, "cam4_1_noise_filter", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.noise_filter);//噪声过滤
                cJSON_AddNumberToObject(root, "cam4_1_sensitivity", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.sensitivity);//灵敏度
                cJSON_AddNumberToObject(root, "cam4_1_count_type", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_type);//计数类型
                cJSON_AddNumberToObject(root, "cam4_1_install_height", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.install_height);//安装高度
                cJSON_AddNumberToObject(root, "cam4_1_filter_height", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.filter_height);//过滤高度
                //CAM4-1通道设置-检测区域
                cJSON_AddNumberToObject(root, "cam4_1_count_line1", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line1);//上计数线1
                cJSON_AddNumberToObject(root, "cam4_1_count_line2", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line2);//下计数线2
                cJSON_AddNumberToObject(root, "cam4_1_count_line3", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line3);//左边界线3
                cJSON_AddNumberToObject(root, "cam4_1_count_line4", CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line4);//右边界线4


                //高级设置
                //高级设置－计数使能

                cJSON_AddNumberToObject(root, "real_dynamic_statistics", CCommonData::getInstance()->m_CSystemPassengerFlow.real_dynamic_statistics);//实时动态统计  0:不选择 1:选择
                cJSON_AddNumberToObject(root, "open_door_statistics", CCommonData::getInstance()->m_CSystemPassengerFlow.open_door_statistics);//开门统计  0:不选择 1:选择
                cJSON_AddNumberToObject(root, "in_station_statistics", CCommonData::getInstance()->m_CSystemPassengerFlow.in_station_statistics);//进站统计  0:不选择 1:选择
                cJSON_AddNumberToObject(root, "out_station_statistics", CCommonData::getInstance()->m_CSystemPassengerFlow.out_station_statistics);//站外统计  0:不选择 1:选择
                cJSON_AddNumberToObject(root, "speed_statistics_enable", CCommonData::getInstance()->m_CSystemPassengerFlow.speed_statistics_enable);//速度统计使能  0:不选择 1:选择
                cJSON_AddNumberToObject(root, "speed_statistics_value", CCommonData::getInstance()->m_CSystemPassengerFlow.speed_statistics_value);//速度统计值
                cJSON_AddNumberToObject(root, "single_loop_error_calibration", CCommonData::getInstance()->m_CSystemPassengerFlow.single_loop_error_calibration);//单圈误差校准
                cJSON_AddNumberToObject(root, "close_delay", CCommonData::getInstance()->m_CSystemPassengerFlow.close_delay);//关门延时
                cJSON_AddNumberToObject(root, "time_upload_time", CCommonData::getInstance()->m_CSystemPassengerFlow.time_upload_time);//定时上报时间

                cJSON_AddNumberToObject(root, "image_mosaic", CCommonData::getInstance()->m_CSystemPassengerFlow.image_mosaic);//图像拼接　0:不选择 1:选择

                cJSON_AddNumberToObject(root, "interval_width", CCommonData::getInstance()->m_CSystemPassengerFlow.interval_width);//间隔宽度　CM



            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;
            case kEnumDefault://获取默认数据
            {
                cJSON_AddNumberToObject(root, "type", 3);
                cJSON_AddNumberToObject(root, "number", _number);

            }break;
            case 4: //同步操作
            {
                cJSON_AddNumberToObject(root, "type", 4);
                cJSON_AddNumberToObject(root, "operate", _number);//1:开始同步 2:取消同步
                cJSON_AddNumberToObject(root, "cam_select", _cam1_2);//0: cam1_1 1:cam2_1 2: cam1_1 3:cam2_1

            }break;
            case 5: //保存背景
            {
                cJSON_AddNumberToObject(root, "type", 5);
                cJSON_AddNumberToObject(root, "operate", _number);//1:背景保存 2:取消背景保存
                cJSON_AddNumberToObject(root, "cam_select", _cam1_2);//1: cam1_1 2:cam2_1

            }break;
            case 6:
            {
                cJSON_AddNumberToObject(root, "type", 6);
                cJSON_AddNumberToObject(root, "operate", _number);//1:进入区域检测 2:离开区域检测
                cJSON_AddNumberToObject(root, "cam_select", _cam1_2);//1: cam1_1 2:cam2_1
            }break;
            case 7:
            {
                cJSON_AddNumberToObject(root, "type", 7);
                cJSON_AddNumberToObject(root, "operate", _number);//1:隐藏主界面显示 2:主界面显示
            }break;

        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

#include "menu/driver_menu_widget.h"
void CProtocol::call_json_menu_driver(char _action,char _value,bool _save)
{
    do
    {
        //call_json_test();
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        switch(_action)
        {
           case CBaseSetWidget::kEnumVolumeSet:  /*1:设置声音*/
           {
                cJSON_AddStringToObject(root, "msg_type", "gui_voice");/*声音*/
                cJSON_AddNumberToObject(root, "type", CBaseSetWidget::kEnumVolumeSet);
                cJSON_AddNumberToObject(root, "volume_within",CCommonData::getInstance()->m_CGuiVoice.m_volume_within );/*内音量*/
                cJSON_AddNumberToObject(root, "volume_out", CCommonData::getInstance()->m_CGuiVoice.m_volume_out);/*外音量*/
                cJSON_AddNumberToObject(root, "volume_TTS", CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS);/*TTS音*/ /*司机喇叭*/
           }break;
           case  CBaseSetWidget::kEnumVolumeGet:   /*3:获取声音*/
           {
                cJSON_AddStringToObject(root, "msg_type", "gui_voice");/*声音*/
                cJSON_AddNumberToObject(root, "type", CBaseSetWidget::kEnumVolumeGet);
           }break;
           case CBaseSetWidget::kEnumLightSet:   //设置亮度
           {
                cJSON_AddStringToObject(root, "msg_type", "gui_light");/*亮度*/
                cJSON_AddNumberToObject(root, "type", 1);
                cJSON_AddNumberToObject(root, "value", _value);//0:自动　1:非常亮　2:一般 3:暗 4:非常暗
                cJSON_AddNumberToObject(root, "save_data", _save);

           }break;
           case CBaseSetWidget::kEnumLightGet:   //获取亮度
           {
                cJSON_AddStringToObject(root, "msg_type", "gui_light");/*亮度*/
                cJSON_AddNumberToObject(root, "type", 2);
           }break;
           case CBaseSetWidget::kEnumSeniorSet: //高级设置保存
           {
                cJSON_AddStringToObject(root, "msg_type", "gui_driver_menu");
                cJSON_AddNumberToObject(root, "type", 1); //1:保存 2:获取 3:默认
                cJSON_AddNumberToObject(root, "travel_direction", CCommonData::getInstance()->m_CDriverMenuInfo.travel_direction);//行驶方向
                cJSON_AddNumberToObject(root, "deputy_page_show_mode", CCommonData::getInstance()->m_CDriverMenuInfo.deputy_page_show_mode);//副主页显示模式
                cJSON_AddNumberToObject(root, "out_door_video_change", CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_change);//下客门视频切换方式
                cJSON_AddNumberToObject(root, "out_door_video_passageway", CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway);//下客门视频通道
                cJSON_AddNumberToObject(root, "back_car_video_passageway", CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway);//倒车视频通道
                cJSON_AddNumberToObject(root, "start_plan_passageway", CCommonData::getInstance()->m_CDriverMenuInfo.start_plan_passageway);//发车计划声道
                cJSON_AddNumberToObject(root, "over_speed_warning_passageway", CCommonData::getInstance()->m_CDriverMenuInfo.over_speed_warning_passageway);//超速警示声道
                cJSON_AddNumberToObject(root, "message_outtime", CCommonData::getInstance()->m_CDriverMenuInfo.message_outtime);//消息超时退出
                cJSON_AddNumberToObject(root, "video_default_frame", CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame);//视频默认画面
                cJSON_AddNumberToObject(root, "video_round_robin", CCommonData::getInstance()->m_CDriverMenuInfo.video_round_robin);//视频轮循
                cJSON_AddNumberToObject(root, "round_robin_passageway", CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_passageway);//轮循通道
                cJSON_AddNumberToObject(root, "round_robin_interval", CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval);//轮循间隔

                cJSON_AddNumberToObject(root, "out_door_video_passageway_enable", CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway_enable);//下客门视频通道使能
                cJSON_AddNumberToObject(root, "back_car_video_passageway_enable", CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway_enable);//倒车视频通道使能
                cJSON_AddNumberToObject(root, "out_door_delay_quit", CCommonData::getInstance()->m_CDriverMenuInfo.out_door_delay_quit);  //下客门延时退出时长

                int array_size = sizeof(CCommonData::getInstance()->m_CDriverMenuInfo.passageway);
                cJSON *pJson_rows = cJSON_CreateArray();
                cJSON_AddItemToObject(root, "rows",pJson_rows);
                cJSON *pJson_row = NULL;
                for(int index = 0;index < array_size;index++)
                {
                    pJson_row = cJSON_CreateObject();
                    cJSON_AddItemToArray(pJson_rows,pJson_row);
                    cJSON_AddNumberToObject(pJson_row, "value", CCommonData::getInstance()->m_CDriverMenuInfo.passageway[index]);
                }


           }break;
           case CBaseSetWidget::kEnumSeniorGet: //获取高级设置
           {
                cJSON_AddStringToObject(root, "msg_type", "gui_driver_menu");
                cJSON_AddNumberToObject(root, "type", 2); //1:保存 2:获取 3:默认
           }break;
           case CBaseSetWidget::kEnumSeniorDefault:
           {
                cJSON_AddStringToObject(root, "msg_type", "gui_driver_menu");
                cJSON_AddNumberToObject(root, "type", 3); //1:保存 2:获取 3:默认
           }break;
        }


        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

void CProtocol::request_system_message(const char *_msg)
{
    cJSON *root = cJSON_CreateObject();
    if(root)
    {
        cJSON_AddStringToObject(root, "msg_type",_msg);
        gui_send_message2system(root);

    }
}

void CProtocol::call_json_station_name_set(char _type)
{
     do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();
        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "system_notify_station");
        switch(_type)
        {
            case kEnumSave: //保存数据
            {
            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;
        }
        gui_send_message2system(root);

    }while(0);
}


//系统管理-POS配置
void CProtocol::call_json_system_pos_config_set(char _type)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }

        cJSON_AddStringToObject(root, "msg_type", "gui_system_pos_config");

        switch(_type)
        {
            case kEnumSave: //保存数据
            {
                cJSON_AddNumberToObject(root, "type", 1);

                cJSON_AddNumberToObject(root, "DEVMODE", CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE);     //终端模式
                cJSON_AddNumberToObject(root, "CHARGMODE", CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE); //收费模式
                cJSON_AddNumberToObject(root, "PSAMNUM", CCommonData::getInstance()->m_CSystemPosSetup.PSAMNUM);     //PSAM个数
                //支持卡类型

                int array_size = sizeof(CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE);
                cJSON *pJson_rows = cJSON_CreateArray();
                cJSON_AddItemToObject(root, "rows",pJson_rows);
                cJSON *pJson_row = NULL;
                for(int index = 0;index < array_size;index++)
                {
                    pJson_row = cJSON_CreateObject();
                    cJSON_AddItemToArray(pJson_rows,pJson_row);
                    cJSON_AddNumberToObject(pJson_row, "value", CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[index]);
                }

                cJSON_AddNumberToObject(root, "SECTOR",CCommonData::getInstance()->m_CSystemPosSetup.SECTOR);         //支持扇区
                cJSON_AddNumberToObject(root, "TIMEINTER", CCommonData::getInstance()->m_CSystemPosSetup.TIMEINTER);  //刷卡时间间隔
                cJSON_AddNumberToObject(root, "TICKETTIME", CCommonData::getInstance()->m_CSystemPosSetup.TICKETTIME);//逃票最大时间
                cJSON_AddStringToObject(root, "host_ip", CCommonData::getInstance()->m_CSystemPosSetup.m_host_ip.toUtf8().data());
            }break;
            case kEnumGet:  //获取数据
            {
                cJSON_AddNumberToObject(root, "type", 2);

            }break;
        }

        gui_send_message2system(root);

    }while(0);
}



















