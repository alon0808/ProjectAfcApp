#ifndef NAVIGATE_LEFT_H
#define NAVIGATE_LEFT_H

#include <QWidget>

#include <QVBoxLayout>
#include <QTableWidget>
#include <QScrollArea>
#include <QMap>

#include "./class/common/navigateleftbutton.h"

#include "class/common/define_screen.h"

class CNavigateLeft : public QWidget
{
    Q_OBJECT
public:
    explicit CNavigateLeft(int _buttonWidth,int _buttonHeight,int _height,QWidget *parent = 0);
    enum
    {
        kEnumListMin,
        //菜单
        kEnumRegisterInfo,          //注册信息
        kEnumCoordinateCollection,  //坐标采集
        kEnumVideotapeQuery,        //录像查询
        kEnumCameraConfig,          //摄像机配置
        kEnumPassengerFlow,         //客流统计
        kEnumStorageManage,         //存储管理
        kEnumNetwork,               //网络
        kEnumSwitchConfig,          //开关量配置
        kEnumSeniorSetUp,           //高级设置
        kEnumPosConfig,             //POS配置
        kEnumSystemSetUp,           //系统设置
        kEnumSystemMaintain,        //系统维护
        kEnumHistoryInfo,            //历史信息
        //消息列表
        kEnumPresetEvent,           //预设事件
        kEnumImportantMessage,      //重要短信
        kEnumNormalMessage,         //普通短信
        kEnumVehiclePlan,           //行车计划
        //司机菜单
        kEnumBaseSet,               //基本设置
        kEnumSeniorSet,             //高级设置
        //关于本机
        kEnumAbountMachine,         //关于本机
        kEnumManufactorInfo,        //厂家信息
        //运行状态
        kEnumMobileNetwork,         //移动网络
        kEnumSwitchStatus,          //开关量
        kEnumLocationModular,       //定位模块
        kEnumLocalNetwork,          //本地网络
        kEnumDeviceStatus,          //设备
        kEnumStationInfo,           //站点信息
        kEnumPeripheral,            //外设
        kEnumListMax
    };
signals:
    void signalCurrentClassificationChanged(int,QString);
public slots:
    void slotButtonClicked();
    void add_button(const int _id,const QString& _name);
    void slotSetDefaultSelected(int _id);
public:
    void navigate_left_init();
    void set_scroll_value(bool _max);
    void navigate_up_down(bool _flag);
    bool active_button_clicked();
    bool set_focus_first();
private:
    void setCurrentType(const int index);
    bool get_active_button(char &_type,bool _down = true);
    bool get_pressed_button(char &_type,bool _down = true);
    void get_first_last_button();

    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_vboxlayout_widget;
    QTableWidget *m_table_widget;
    QScrollArea *m_scrollarea_1;

    int m_CurrentId;
    QMap<int,NavigateLeftButton*> m_pButtonMap;
    int m_button_width;
    int m_button_height;

    CMyDefineScreen *m_define_screen;

    NavigateLeftButton *m_first_button;
    char m_first_type;
    NavigateLeftButton *m_last_button;
    char m_last_type;

};

#endif // NAVIGATE_LEFT_H



















