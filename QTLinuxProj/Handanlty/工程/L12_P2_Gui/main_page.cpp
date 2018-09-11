#include "main_page.h"


#include "./class/common_interface.h"
#include "./class/common/cycloprogress.h"

#include "service_dialog.h"
#include "voice_dialog.h"
#include "video_widget.h"

#include "LtyCommonDefine.h"
#include "./class/msglist.h"

#include <QComboBox>
#include <QListView>

#include "./menu/message_widget.h"
#include "./menu/run_status_widget.h"

#include "common_data.h"

#include <QDebug>
#include "class/ltyuicallinterface.h"

#include "class/common/main_message_dialog.h"

#include "class/ltystring.h"
#include "mainwindow.h"

#include "class/common/my_messagebox.h"
#include "main_page2.h"

//站名显示的长度
#define STATION_NAME_SHOW_FONT_SIZE 84

#define SUPPORT_TAIGUO 1

#define VEHICLE_STATION_NAME_SHOW_MODE  2

#if 0
#if SUPPORT_TAIGUO
#define STATION_NAME_SHOW_LENGTH    (548 - (260 + 6 + 16)) + 10
#else
#define STATION_NAME_SHOW_LENGTH    (548 - (260 + 6 + 16))
#endif
#endif

#define STATION_NAME_SHOW_LENGTH    (721 - 382)  //382

#define MESSAGE_SHOW_LENGTH     400





#define USB_QT_TIMER    0

#define MENU_KEY_SCREEN_SHOT

extern int screen_flag;

static CMainPage *gMainPageThis = NULL;


CMainPage *CMainPage::getInstance()
{
    return gMainPageThis;
}


//border:1px solid #494949;
CMainPage::CMainPage(QWidget *parent) :
    QDialog(parent)
{
    gMainPageThis = this;
    this->setFixedSize(1024,768);
    this->setStyleSheet("QDialog{background-color:#1f2024;}");
    this->setWindowFlags(Qt::FramelessWindowHint/*|Qt::WindowStaysOnTopHint*/);

/*
    QDate date;
    date.setDate(2016,12,11);
    int week = date.dayOfWeek();
    week = week;
    int month = date.daysInMonth();
*/

#ifndef CURRENT_ARM_RUN
    //返回按钮
    m_pushbutton_back = new QPushButton(this);
    m_pushbutton_back->setFixedSize(70,28);
    m_pushbutton_back->move(431,123);
    m_pushbutton_back->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_back->setText("返回");

    //m_pushbutton_back->setStyleSheet(MENU_BUTTON_GROUP_SELECTED_QSS);
    //m_pushbutton_back->setIcon(QIcon(":/img/res/nav_button_back.png"));
    //m_pushbutton_back->setIconSize(QSize(18,18));

    connect(m_pushbutton_back,SIGNAL(clicked()),SLOT(slot_pushbutton_back()));
#endif



    m_widget_bottom = new QWidget(this);
    m_widget_bottom->setFixedSize(1024,205);
    m_widget_bottom->move(0,768 - 205);
    m_widget_bottom->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/bottom_bg.png);}");
    //L12 delete
    m_widget_bottom->hide();

#if 0
    m_widget_bottom1 = new QWidget(this);
    m_widget_bottom1->setFixedSize(1024,6);
    m_widget_bottom1->move(0,768 - 109 - 6);
    m_widget_bottom1->setStyleSheet("QWidget {border:0px;background-color:#25262b;}");
#endif


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetMainPage,this);
    // L12 delete
    m_CHeadWidget->hide();


    //main_middle_init();
    main_middle_init_new();
    main_bottom_init();
    main_page_info_init();

    connect(m_pushbutton_station_name,SIGNAL(clicked()),this,SLOT(slot_pushbutton_station_name()));


    //connect( this, SIGNAL(signal_main_page_message(unsigned int)), this,	SLOT(slot_main_page_message(unsigned int)));

   // connect(this,SIGNAL(signal_reset_touch_event()),this,SLOT(slot_reset_touch_event()));

    //pthread_create( &thrd_msg_parse, NULL,guiMsgDataParse, this );


    //m_time_touch_reset = new QTime;
    //m_time_touch_reset->start();


    //im->updateHandler(QWSInputMethod::FocusIn);


    m_label_test_picture = new QLabel(this);
    // L12 delete
    m_label_test_picture->hide();
    m_label_test_picture->move(0,0);
    //m_label_test_picture->setFixedSize(1024,768);
    m_label_test_picture->setFixedSize(640,480);
    m_label_test_picture->hide();

    m_pushbutton_test = new QPushButton(this);
    // L12 delete
    m_pushbutton_test->hide();
    m_pushbutton_test->setFixedSize(100,50);
    m_pushbutton_test->move(431 + 70 + 10,100);
    m_pushbutton_test->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_test->setText("测试");
    m_pushbutton_test->hide();

    connect(m_pushbutton_test,SIGNAL(clicked()),SLOT(slot_pushbutton_test()));

    connect(this,SIGNAL(signal_touch_mouse_event(int,int,bool)),this,SLOT(slot_touch_mouse_event(int,int,bool)));

#if 0
    QTextEdit *pTestEdit = new QTextEdit(this);
    pTestEdit->move(100,20);
    pTestEdit->setFixedSize(100,100);
#endif
    m_timer_vehicle = new QTimer(this);
    m_timer_vehicle->setInterval(5000);

    main_signal_slot_init();
#if 0
    m_timer_test = new QTimer;
    m_timer_test->setInterval(500);
    connect(m_timer_test, SIGNAL(timeout()), this, SLOT(slot_timer_test()));
    m_timer_test->start();
#endif
    m_timer_station = new QTimer;
    //m_timer_station->setInterval(1000);
    connect(m_timer_station, SIGNAL(timeout()), this, SLOT(slot_timer_station()));
    //m_timer_station->start();


    m_timer_key_5s = new QTimer;
    m_timer_key_5s->setInterval(5000);
    connect(m_timer_key_5s, SIGNAL(timeout()), this, SLOT(slot_timer_key_5s()));
    m_key_times = 0;
    m_key_back_times = 0;


}

void CMainPage::main_signal_slot_init()
{
    connect(m_timer_vehicle, SIGNAL(timeout()), this, SLOT(slot_timer_vehicle()));
    m_timer_vehicle->start();
}

void CMainPage::slot_pushbutton_back()
{
    //close();
    emit signal_pushbutton_back();
}

void CMainPage::main_page_info_init()
{
    CEventData eventData;

    main_page_notify(kEnumMainPageSignal1,kEnumNetworkNone,0);                //SIM卡　2G 3G 4G
    main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignalError,0);           //SIM卡信号强弱
    main_page_notify(kEnumMainPageGpsNumber,0,0);                           //GPS个数
    main_page_notify(kEnumMainPageGpsStatus,kEnumGpsAbnormal,0);              //GPS状态
    main_page_notify(kEnumMainPageFrontDoor,kEnumDoorClose,0);               //前门
    main_page_notify(kEnumMainPageBackDoor,kEnumDoorClose,0);               //后门
    //main_page_notify(kEnumMainPageAccStatus,kEnumAcc1,0);                   //ACC状态
    //main_page_notify(kEnumMainPageDeviceDoorStatus,kEnumDeviceDoorOpen,0);  //设备门（机器上）状态
    main_page_notify(kEnumMainPageDeviceAbnorma,kEnumDeviceAbnormal,0);     //设备异常（机器上）
    main_page_notify(kEnumMainPageHardDiskStatus,kEnumHardDiskAbnormal1,0);    //硬盘状态
    //在page_2中增加了初始化:req_tbp_status
    // main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformAbnormal1,0);    //平台连接状态
    main_page_notify(kEnumMainPagePlatformStatus1,kEnumPlatformAbnormal1,0);    //平台连接状态
    main_page_notify(kEnumMainPageMessageStatus,kEnumMessageStatus1,0);     //消息状态

    //在page_2中增加了初始化:req_sd_status
    // main_page_notify(kEnumMainPageSdStatus,kEnumError,0);

    eventData.m_str_data = tr("");//;tr("深圳世界之窗地铁站－测试的");
    main_page_notify(kEnumMainPageUpdateStationName,0,&eventData);

    eventData.m_str_data = tr("");
    main_page_notify(kEnumMainPageVehicleModel,0,&eventData);               //车的型号
    eventData.m_str_data = tr("上行");
    main_page_notify(kEnumMainPageVehicleUpDown,0,&eventData);              //车是上行或下行

    eventData.m_str_data = tr("运营中");
    main_page_notify(kEnumMainPageVehicleStatus,0,&eventData);              //车的状态(运营中)

    main_page_notify(kEnumMainPageUpVehicleNumber,0,0);       //上车人数
    main_page_notify(kEnumMainPageDownVehicleNumber,0,0);      //下车人数
    main_page_notify(kEnumMainPageWithinVehicleNumber,0,0);   //车内人数

    main_page_notify(kEnumMainPageLimitSpeed,80,0);     //限速
    main_page_notify(kEnumMainPageCurrentSpeed,0,0);   //当前速度

    eventData.m_str_data = tr(":/img_p2/res/p2/left_right/guanggao.jpg");
    main_page_notify(kEnumMainPageAdvert,0,&eventData);

    //CCommonData::getInstance()->m_CPlayPicture.m_image.loadFromData((const uchar *)pData->data,pData->size);
    //emit signal_main_page2_picture(CCommonData::getInstance()->m_CPlayPicture);

/*
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
*/
    //kEnumCardStatusSuccess  kEnumQrStatusNormal  kEnumQrStatusSuccess
    eventData.m_str_data = tr("PSAM卡检测失败\n请检查");
    //eventData.m_i_data = 10;
    //eventData.m_param1 = 2;
    main_page_notify(kEnumMainPageVerificationRrStatus,kEnumQrStatusNormal,&eventData);

    eventData.m_str_data = tr("");
    eventData.m_i_data = 1;
    // 已在main_page2 进行初始化，参考req_station_and_line()
    // main_page_notify(kEnumMainPageLine,0,&eventData);   //线路
    // main_page_notify(kEnumMainPageChargeType,1,0);
    eventData.m_str_data = tr("0.00");
    main_page_notify(kEnumMainPageAmount,0,&eventData);   //金额
    eventData.m_str_data = tr("0.00");
    main_page_notify(kEnumMainPageBalance,0,&eventData);   //余额
    //eventData.m_str_data = tr("世界之窗-测试到站的");
    //main_page_notify(kEnumMainPageArriveStation,0,&eventData);   //到站

    eventData.m_str_data = tr("");//tr("之窗-测试到站的");
    main_page_notify(kEnumMainPageUpdateStationName1,0,&eventData);


    if(1 == VEHICLE_STATION_NAME_SHOW_MODE)
    {
        eventData.m_str_data = tr("0.5");
        main_page_notify(kEnumMainPageFrontVehicle,0,&eventData);
        eventData.m_str_data = tr("2.3");
        main_page_notify(kEnumMainPageBackVehicle,0,&eventData);
    }
    else if(2 == VEHICLE_STATION_NAME_SHOW_MODE)
    {
        eventData.m_str_data = tr("");//tr("西丽法庭");
        main_page_notify(kEnumMainPageFrontStation1,0,&eventData);
        eventData.m_str_data = tr("");//tr("茶光");
        main_page_notify(kEnumMainPageFrontStation2,0,&eventData);

        eventData.m_str_data = tr("");//tr("松坪山");
        main_page_notify(kEnumMainPageBackStation1,0,&eventData);
        eventData.m_str_data = tr("");//tr("科陆大厦");
        main_page_notify(kEnumMainPageBackStation2,0,&eventData);

        //kEnumMainPageVehicle1
        //eventData.m_str_data = tr("0.3");
        //main_page_notify(kEnumMainPageVehicle1,kEnumVehicleStationBack,&eventData);  //后车

        //eventData.m_str_data = tr("0.3");
        main_page_notify(kEnumMainPageVehicle2,kEnumVehicleStationCurrent,&eventData);

        //eventData.m_str_data = tr("2.3");
        //main_page_notify(kEnumMainPageVehicle3,kEnumVehicleStationBack,&eventData); //前车

        m_label_vehicle_3->hide();
        m_label_vehicle_3_distance->hide();

        m_label_vehicle_1->hide();
        m_label_vehicle_1_distance->hide();


        main_page_notify(kEnumMainPageStationStatus,kEnumStationIn,0);


    }


#if 0


    eventData.m_str_data = tr("1km");
    main_page_notify(kEnumMainPageLeftDistance,0,&eventData);               //左边车的距离
    eventData.m_str_data = tr("0.5km");
    main_page_notify(kEnumMainPageRightDistance,0,&eventData);              //右边车的距离
    if(1 == SUPPORT_TAIGUO)
    {
        eventData.m_str_data = tr("0.5km");
        main_page_notify(kEnumMainPageLeftDistance1,0,&eventData);               //左边车的距离(泰国)
        eventData.m_str_data = tr("0.2km");
        main_page_notify(kEnumMainPageRightDistance1,0,&eventData);              //右边车的距离(泰国)
    }

    CDynTextParam param_station_message;
    param_station_message.m_control_this = m_label_message_content;
    param_station_message.m_control_type = CDynamicText::kEnumLabel;
    param_station_message.m_control_id   = kEnumLabelMessageShow;
    param_station_message.m_control_source_text = tr("普通短信：雨天路滑 请小心驾驶,车速不能过快，安全为主！");
    param_station_message.m_control_font_size = 26;
    param_station_message.m_control_show_length = MESSAGE_SHOW_LENGTH;

    m_dynamic_text_message->set_text(param_station_message);

    eventData.m_str_data = tr("2016.08.08 12:12");
    main_page_notify(kEnumMainPageStartTime,0,&eventData);                  //发车时间

    eventData.m_str_data = tr("欧阳小飞");
    main_page_notify(kEnumMainPageDriverName,0,&eventData);                 //驾驶员名字
    eventData.m_str_data = tr("65535");
    main_page_notify(kEnumMainPageVehicleNumber,0,&eventData);              //车辆编号

    eventData.m_str_data = tr("运营中");
    main_page_notify(kEnumMainPageVehicleStatus,0,&eventData);              //车的状态(运营中)
    eventData.m_str_data = tr("超速警报");
    main_page_notify(kEnumMainPageSpeedAlert,kEnumSpeedAlertStart,&eventData);//超速警报

#endif
}

void CMainPage::main_middle_init_new()
{
    m_widget_middle_left = new QWidget(this);
    // L12 delete
    m_widget_middle_left->hide();
    m_widget_middle_left->setFixedSize(107,384);
    m_widget_middle_left->move(0,130);
    m_widget_middle_left->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/left_bg.png);}");

    m_widget_middle_right = new QWidget(this);
    // L12 delete
    m_widget_middle_right->hide();
    m_widget_middle_right->setFixedSize(107,384);
    m_widget_middle_right->move(1024 - 107,130);
    m_widget_middle_right->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/right_bg.png);}");

    if(1 == VEHICLE_STATION_NAME_SHOW_MODE)
    {
        m_widget_front_vehicle = new QWidget(this);
        // L12 delete
        m_widget_front_vehicle->hide();
        m_widget_front_vehicle->setFixedSize(30,38);
        m_widget_front_vehicle->move(306,192);
        m_widget_front_vehicle->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/before.png);}");

        m_label_front_vehicle1 = new QLabel(this);
        // L12 delete
        m_label_front_vehicle1->hide();
        m_label_front_vehicle1->setFixedSize(70,35);
        m_label_front_vehicle1->move(343,186);
        m_label_front_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 46px;color:#80e3ff;}");

        m_label_front_vehicle2 = new QLabel(this);
        // L12 delete
        m_label_front_vehicle2->hide();
        m_label_front_vehicle2->setFixedSize(70,35);
        m_label_front_vehicle2->move(396,203);
        m_label_front_vehicle2->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 23px;color:#8ea2c0;}");
        m_label_front_vehicle2->setText(tr("km"));

        m_widget_back_vehicle = new QWidget(this);
        // L12 delete
        m_widget_back_vehicle->hide();
        m_widget_back_vehicle->setFixedSize(30,38);
        m_widget_back_vehicle->move(614,192);
        m_widget_back_vehicle->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/rear.png);}");

        m_label_back_vehicle1 = new QLabel(this);
        // L12, delete
        m_label_back_vehicle1->hide();
        m_label_back_vehicle1->setFixedSize(70,35);
        m_label_back_vehicle1->move(651,186);
        m_label_back_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 46px;color:#80e3ff;}");

        m_label_back_vehicle2 = new QLabel(this);
         // L12, delete
        m_label_back_vehicle2->hide();
        m_label_back_vehicle2->setFixedSize(70,35);
        m_label_back_vehicle2->move(704,203);
        m_label_back_vehicle2->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 23px;color:#8ea2c0;}");
        m_label_back_vehicle2->setText(tr("km"));
    }
    else if(2 == VEHICLE_STATION_NAME_SHOW_MODE)
    {
        /*
            QWidget *m_widget_station_bg;     //5个站点背景
    QLabel *m_label_front_station_1; //上一个站1
    QLabel *m_label_front_station_2;//上一个站2

    QLabel *m_label_back_station_1;//下一个站1
    QLabel *m_label_back_station_2;//下一个站2

    QLabel *m_label_vehicle_1; //前车
    QLabel *m_label_vehicle_1_distance;//前车到下站的距离
    QLabel *m_label_vehicle_2;//当前车
    QLabel *m_label_vehicle_2_distance;//当前车到下站的距离
    QLabel *m_label_vehicle_3;//后车
    QLabel *m_label_vehicle_3_distance;//后车到下站的距离
        */
        m_widget_station_bg = new QWidget(this);
        // L12 delete
        m_widget_station_bg->hide();
        m_widget_station_bg->setFixedSize(640,20);
        m_widget_station_bg->move(192,240 - 10);
        m_widget_station_bg->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/circuit_diagram.png);}");

        m_label_front_station_1 = new QLabel(this);
        // L12 delete
        m_label_front_station_1->hide();
        m_label_front_station_1->setFixedSize(140,40);
        m_label_front_station_1->move(162,272 - 10);
        m_label_front_station_1->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:30px;color:#80e3ff;}");
        //m_label_front_station_1->setText(tr("西丽法庭"));
        m_label_front_station_1->setAlignment(Qt::AlignCenter);

        m_label_front_station_2 = new QLabel(this);
        // L12 delete
        m_label_front_station_2->hide();
        m_label_front_station_2->setFixedSize(140,40);
        m_label_front_station_2->move(162 + 140,272 - 10);
        m_label_front_station_2->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:30px;color:#80e3ff;}");
        //m_label_front_station_2->setText(tr("茶光"));
        m_label_front_station_2->setAlignment(Qt::AlignCenter);

        m_label_back_station_1 = new QLabel(this);
        // L12 delete
        m_label_back_station_1->hide();
        m_label_back_station_1->setFixedSize(140,40);
        m_label_back_station_1->move(590 - 5,272 - 10);
        m_label_back_station_1->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:30px;color:#80e3ff;}");
        //m_label_back_station_1->setText(tr("松坪山"));
        m_label_back_station_1->setAlignment(Qt::AlignCenter);

        m_label_back_station_2 = new QLabel(this);
        // L12 delete
        m_label_back_station_2->hide();
        m_label_back_station_2->setFixedSize(140,40);
        m_label_back_station_2->move(590 - 5 + 140,272 - 10);
        m_label_back_station_2->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:30px;color:#80e3ff;}");
        //m_label_back_station_2->setText(tr("科陆大厦"));
        m_label_back_station_2->setAlignment(Qt::AlignCenter);

        m_label_vehicle_1 = new QLabel(this);
         // L12 delete
        m_label_vehicle_1->hide();
        m_label_vehicle_1->setFixedSize(31,26);
        m_label_vehicle_1->move(317,203 - 10);
        // m_label_vehicle_1->setPixmap(QPixmap(":/img/res/main_page/bus.png"));

        int pos_x = m_label_vehicle_1->pos().x() + m_label_vehicle_1->width()/2 - 100/2;
        int pos_y = m_label_vehicle_1->pos().y() - 50 - 5;
        m_label_vehicle_1_distance = new QLabel(this);
        // L12 delete
        m_label_vehicle_1_distance->hide();
        m_label_vehicle_1_distance->setFixedSize(100,50);
        m_label_vehicle_1_distance->move(pos_x,pos_y - 10);
        m_label_vehicle_1_distance->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:bold 46px;color:#80e3ff;}");
        m_label_vehicle_1_distance->setAlignment(Qt::AlignCenter);
        //m_label_vehicle_1_distance->setText(tr("0.5"));

        m_label_vehicle_2 = new QLabel(this);
        // L12 delete
        m_label_vehicle_2->hide();
        m_label_vehicle_2->setFixedSize(31,26);
        m_label_vehicle_2->move(495,203 - 10);
        // m_label_vehicle_2->setPixmap(QPixmap(":/img/res/main_page/bus.png"));

        pos_x = m_label_vehicle_2->pos().x() + m_label_vehicle_2->width()/2 - 100/2;
        pos_y = m_label_vehicle_2->pos().y() - 50 - 5;
        m_label_vehicle_2_distance = new QLabel(this);
        // L12 delete
        m_label_vehicle_2_distance->hide();
        m_label_vehicle_2_distance->setFixedSize(100,50);
        m_label_vehicle_2_distance->move(pos_x,pos_y - 10);
        m_label_vehicle_2_distance->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:bold 46px;color:#80e3ff;}");
        m_label_vehicle_2_distance->setAlignment(Qt::AlignCenter);


        m_label_vehicle_3 = new QLabel(this);
        // L12 delete
        m_label_vehicle_3->hide();
        m_label_vehicle_3->setFixedSize(31,26);
        m_label_vehicle_3->move(578,203 - 10);
       // m_label_vehicle_3->setPixmap(QPixmap(":/img/res/main_page/bus_2.png"));

        pos_x = m_label_vehicle_3->pos().x() + m_label_vehicle_3->width()/2 - 100/2;
        pos_y = m_label_vehicle_3->pos().y() - 50 - 5;
        m_label_vehicle_3_distance = new QLabel(this);
        // L12 delete
        m_label_vehicle_3_distance->hide();
        m_label_vehicle_3_distance->setFixedSize(100,50);
        m_label_vehicle_3_distance->move(pos_x,pos_y - 10);
        m_label_vehicle_3_distance->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:bold 46px;color:#8ea2c0;}");
        m_label_vehicle_3_distance->setAlignment(Qt::AlignCenter);
        //m_label_vehicle_3_distance->setText(tr("2.3"));

        m_label_station_status = new QLabel(this);
        // L12 delete
        m_label_station_status->hide();
        m_label_station_status->setFixedSize(200,50);
        m_label_station_status->move(400 + 20,324);
        m_label_station_status->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:30px;color:#8ea2c0;}");
        m_label_station_status->setAlignment(Qt::AlignCenter);

    }

    //箭头左
    m_pushbutton_arrow_left = new QPushButton(this);
     // L12 delete
     m_pushbutton_arrow_left->hide();
    //箭头右
    m_pushbutton_arrow_right = new QPushButton(this);
    // L12 delete
    m_pushbutton_arrow_right->hide();
    //站台名称
    m_pushbutton_station_name = new QPushButton(this);
    // L12 delete
    m_pushbutton_station_name->hide();

    if(1 == VEHICLE_STATION_NAME_SHOW_MODE)
    {
        QString str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/left_arrow.png").arg(":/img/res/main_page/left_arrow_hover.png").arg(":/img/res/main_page/left_arrow_hover.png");

        CCommonInterface ::init_new_button(m_pushbutton_arrow_left,251,321,50,77,str_sheet);

        str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/right_arrow.png").arg(":/img/res/main_page/right_arrow_hover.png").arg(":/img/res/main_page/right_arrow_hover.png");

        CCommonInterface ::init_new_button(m_pushbutton_arrow_right,721,321,50,77,str_sheet);

        CCommonInterface ::init_new_button(m_pushbutton_station_name,350/*382*/,312,STATION_NAME_SHOW_LENGTH,90);
    }
    else if(2 == VEHICLE_STATION_NAME_SHOW_MODE)
    {
        QString str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/left_arrow.png").arg(":/img/res/main_page/left_arrow_hover.png").arg(":/img/res/main_page/left_arrow_hover.png");


        CCommonInterface ::init_new_button(m_pushbutton_arrow_left,257,373 + 10,50,77,str_sheet);

        str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/right_arrow.png").arg(":/img/res/main_page/right_arrow_hover.png").arg(":/img/res/main_page/right_arrow_hover.png");


        CCommonInterface ::init_new_button(m_pushbutton_arrow_right,726,373 + 10,50,77,str_sheet);

        CCommonInterface ::init_new_button(m_pushbutton_station_name,350/*382*/,364 + 10,STATION_NAME_SHOW_LENGTH,90);
    }


    connect(m_pushbutton_arrow_left,SIGNAL(clicked()),this,SLOT(slot_pushbutton_arrow_left()));
    connect(m_pushbutton_arrow_right,SIGNAL(clicked()),this,SLOT(slot_pushbutton_arrow_right()));






    m_dynamic_text_station_name  = new CDynamicText(this);
    // L12 delete
    m_dynamic_text_station_name->hide();
    connect(m_dynamic_text_station_name,SIGNAL(signal_dynamic_text_event(CDynTextParam)),this,SLOT(slot_dynamic_text_event(CDynTextParam)));
    ////////////////////////////////////////////////////////////////// middle bottom

    m_label_bianma1 = new QLabel(this);
    // L12 delete
    m_label_bianma1->hide();
    m_label_bianma1->setFixedSize(150,35);
    m_label_bianma1->move(340,526);
    m_label_bianma1->setStyleSheet("QLabel{border:0px;background:transparent;font:30px;color:#8ea2c0;}");

    m_label_bianma2 = new QLabel(this);
    // L12 delete
    m_label_bianma2->hide();
    m_label_bianma2->setFixedSize(73,35);
    m_label_bianma2->move(483,526);
    m_label_bianma2->setStyleSheet("QLabel{border:0px;background:transparent;font:30px;color:#8ea2c0;}");

    QString str_sheet = "QPushButton{border:0px solid #ffffff;background:transparent;font:30px;color:#8ea2c0;}";
    m_pushbutton_bianma3 = new QPushButton(this);
    // L12 delete
    m_pushbutton_bianma3->hide();
    CCommonInterface ::init_new_button(m_pushbutton_bianma3,613 - 20,526 - 17,150,70,str_sheet);
    connect(m_pushbutton_bianma3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_bianma3()));

    ////////////////////////////////////////////////////////////////// middle left
    m_label_up_vehicle1 = new QLabel(this);
    // L12 delete
    m_label_up_vehicle1->hide();
    m_label_up_vehicle1->setFixedSize(45,25);
    m_label_up_vehicle1->move(36 - 6,185);
    m_label_up_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#8ea2c0; }");
    m_label_up_vehicle1->setText(tr("上车"));

    m_label_up_vehicle2 = new QLabel(this);
    // L12 delete
    m_label_up_vehicle2->hide();
    m_label_up_vehicle2->setFixedSize(70,50);
    m_label_up_vehicle2->move(25 - 6,210);
    m_label_up_vehicle2->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:bold 46px;color:#94c6ff; }");
    m_label_up_vehicle2->setAlignment(Qt::AlignCenter);

    m_label_down_vehicle1 = new QLabel(this);
    // L12 delete
    m_label_down_vehicle1->hide();
    m_label_down_vehicle1->setFixedSize(45,25);
    m_label_down_vehicle1->move(37 - 6,288);
    m_label_down_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#8ea2c0; }");
    m_label_down_vehicle1->setText(tr("下车"));

    m_label_down_vehicle2 = new QLabel(this);
    // L12 delete
    m_label_down_vehicle2->hide();
    m_label_down_vehicle2->setFixedSize(70,50);
    m_label_down_vehicle2->move(25 - 6,311);
    m_label_down_vehicle2->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 46px;color:#94c6ff; }");
    m_label_down_vehicle2->setAlignment(Qt::AlignCenter);

    m_label_within_vehicle1 = new QLabel(this);
    // L12 delete
    m_label_within_vehicle1->hide();
    m_label_within_vehicle1->setFixedSize(70,50);
    m_label_within_vehicle1->move(37 - 6,381);
    m_label_within_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#8ea2c0; }");
    m_label_within_vehicle1->setText(tr("车内"));

    m_label_within_vehicle2 = new QLabel(this);
    // L12 delete
    m_label_within_vehicle2->hide();
    m_label_within_vehicle2->setFixedSize(70,50);
    m_label_within_vehicle2->move(25 - 6,416);
    m_label_within_vehicle2->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 46px;color:#94c6ff; }");
    m_label_within_vehicle2->setAlignment(Qt::AlignCenter);

    ////////////////////////////////////////////////////////////////// middle right
    m_label_limit_speed1 = new QLabel(this);
    // L12 delete
    m_label_limit_speed1->hide();
    m_label_limit_speed1->setFixedSize(80,50);
    m_label_limit_speed1->move(936,214);
    m_label_limit_speed1->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 61px;color:#f7382a; }");
    //m_label_limit_speed1->setText(tr("80"));
    m_label_limit_speed1->setAlignment(Qt::AlignCenter);

    m_label_limit_speed2 = new QLabel(this);
    // L12 delete
    m_label_limit_speed2->hide();
    m_label_limit_speed2->setFixedSize(65,25);
    m_label_limit_speed2->move(952 - 5,268);
    m_label_limit_speed2->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#8ea2c0; }");
    m_label_limit_speed2->setText(tr("km/h"));

    m_label_current_speed = new QLabel(this);
    // L12 delete
    m_label_current_speed->hide();
    m_label_current_speed->setFixedSize(100,70);
    m_label_current_speed->move(922,360);
    m_label_current_speed->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 80px;color:#94fbff; }");
    //m_label_current_speed->setText(tr("50"));
    m_label_current_speed->setAlignment(Qt::AlignCenter);



}

void CMainPage::main_middle_init()
{
    QString str_sheet = "";
    ////////////////////////////////////////////////////////中间左边显示

   // QWidget *m_widget_bianma1; //车的编号1
    //QLabel *m_label_bianma1; //车的编号2

    //车的编号1
    m_widget_bianma1 = new QWidget(this);
     // L12 delete
    m_widget_bianma1->hide();
    m_widget_bianma1->setFixedSize(24,26);
    m_widget_bianma1->move(15,144);
    m_widget_bianma1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_qh.png);}");

    //车的编号2
    m_label_bianma1 = new QLabel(this);
    // L12 delete
    m_label_bianma1->hide();
    m_label_bianma1->setFixedSize(73,30);
    m_label_bianma1->move(15 + 24 + 10,144 - 4);
    m_label_bianma1->setStyleSheet("QLabel{border:0px;background:transparent;font:31px;color:#ffffff;}");


    m_widget_line1 = new QWidget(this);
    // L12 delete
    m_widget_line1->hide();
    m_widget_line1->setFixedSize(1,22);
    m_widget_line1->move(116 + 10,144);
    m_widget_line1->setStyleSheet("QWidget {border:0px;background-color:#435261;}");

    m_label_bianma2 = new QLabel(this);
    // L12 delete
    m_label_bianma2->hide();
    m_label_bianma2->setFixedSize(48,30);
    m_label_bianma2->move(131 + 10,144 - 4);
    m_label_bianma2->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff;}");


    m_widget_line2 = new QWidget(this);
    // L12 delete
    m_widget_line2->hide();
    m_widget_line2->setFixedSize(1,22);
    m_widget_line2->move(193 + 5,144);
    m_widget_line2->setStyleSheet("QWidget {border:0px;background-color:#435261;}");

#if 0
    m_label_bianma3 = new QLabel(this);
    m_label_bianma3->setFixedSize(48 + 20,30);
    m_label_bianma3->move(196 + 15,144 - 4);
    m_label_bianma3->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff;}");
#endif

    m_widget_middle1 = new QWidget(this);
    // L12 delete
    m_widget_middle1->hide();
    m_widget_middle1->setFixedSize(690,27);
    m_widget_middle1->move(35,241);
    if(1 == SUPPORT_TAIGUO)
        m_widget_middle1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_zdxx2-2.png);}");
    else
        m_widget_middle1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_zdxx2.png);}");

    //车1
    m_widget_vehicle1 = new QWidget(this);
    // L12 delete
    m_widget_vehicle1->hide();
    m_widget_vehicle1->setFixedSize(38,31);
    if(1 == SUPPORT_TAIGUO)
        m_widget_vehicle1->move(35 + 2,241 - 5 - 31);
    else
        m_widget_vehicle1->move(65,241 - 5 - 31);
    m_widget_vehicle1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_bus1.png);}");
    //车2
    m_widget_vehicle2 = new QWidget(this);
    // L12 delete
    m_widget_vehicle2->hide();
    m_widget_vehicle2->setFixedSize(38,31);
    m_widget_vehicle2->move(362 + 10,241 - 5 - 31);
    m_widget_vehicle2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_bus1.png);}");
    //车3
    m_widget_vehicle3 = new QWidget(this);
    // L12 delete
    m_widget_vehicle3->hide();
    m_widget_vehicle3->setFixedSize(38,31);
    if(1 == SUPPORT_TAIGUO)
        m_widget_vehicle3->move(648 - 10 - 3,241 - 5 - 31);
    else
        m_widget_vehicle3->move(648 + 17,241 - 5 - 31);
    m_widget_vehicle3->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_bus2.png);}");

    if(1 == SUPPORT_TAIGUO)
    {
        m_widget_vehicle1_1 = new QWidget(this);
        // L12 delete
        m_widget_vehicle1_1->hide();
        m_widget_vehicle1_1->setFixedSize(38,31);
        m_widget_vehicle1_1->move(245,241 - 5 - 31);
        m_widget_vehicle1_1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_bus1.png);}");

        m_widget_vehicle2_1 = new QWidget(this);
        // L12 delete
        m_widget_vehicle2_1->hide();
        m_widget_vehicle2_1->setFixedSize(38,31);
        m_widget_vehicle2_1->move(554 + 10,241 - 5 - 31);
        m_widget_vehicle2_1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_bus2.png);}");
    }

    ////////////////////////////////////////////////////////////////站台信息start
    //显示到站背景
    m_widget_middle2 = new QWidget(this);
    // L12 delete
    m_widget_middle2->hide();
    m_widget_middle2->setFixedSize(400,110);
    m_widget_middle2->move(183,282);
    if(1 == SUPPORT_TAIGUO)
        m_widget_middle2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_dqzd1.png);}");
    else
        m_widget_middle2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_dqzd.png);}");

    if(1 != SUPPORT_TAIGUO)
    {
        //本站
        m_label_local_station = new QLabel(this);
        m_label_local_station->setFixedSize(42,65);
        m_label_local_station->move(201 + 3,315 + 3);
        m_label_local_station->setStyleSheet("QLabel{border:0px;background:transparent;font:26px;color:#ffffff;}");
        m_label_local_station->setText(tr("本站"));
        m_label_local_station->setAlignment(Qt::AlignVCenter);
        m_label_local_station->setWordWrap(true);
		m_label_local_station->hide();
    }


    //箭头左
    m_pushbutton_arrow_left = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
            .arg(":/img/res/zj_zjt.png");
    if(1 == SUPPORT_TAIGUO)
        CCommonInterface ::init_new_button(m_pushbutton_arrow_left,183 + 23,282 + 25 + (110 - 25 -38)/2,16,38,str_sheet);
    else
        CCommonInterface ::init_new_button(m_pushbutton_arrow_left,260 + 6,282 + 25 + (110 - 25 -38)/2,16,38,str_sheet);

    //箭头右
    m_pushbutton_arrow_right = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
            .arg(":/img/res/zj_yjt.png");
    if(1 == SUPPORT_TAIGUO)
        CCommonInterface ::init_new_button(m_pushbutton_arrow_right,183 + 400 - 23 - 16,282 + 25 + (110 - 25 -38)/2,16,38,str_sheet);
    else
        CCommonInterface ::init_new_button(m_pushbutton_arrow_right,548,282 + 25 + (110 - 25 -38)/2,16,38,str_sheet);

    //站台名称
    m_pushbutton_station_name = new QPushButton(this);
    if(1 == SUPPORT_TAIGUO)
        CCommonInterface ::init_new_button(m_pushbutton_station_name,260 + 6 + 16 - 30,282 + 25 + (110 - 25 -38)/2,STATION_NAME_SHOW_LENGTH,38);
    else
        CCommonInterface ::init_new_button(m_pushbutton_station_name,260 + 6 + 16,282 + 25 + (110 - 25 -38)/2,STATION_NAME_SHOW_LENGTH,38);

    m_dynamic_text_station_name  = new CDynamicText(this);
    connect(m_dynamic_text_station_name,SIGNAL(signal_dynamic_text_event(CDynTextParam)),this,SLOT(slot_dynamic_text_event(CDynTextParam)));


    //左边距离
    m_label_left_distance = new QLabel(this);
    m_label_left_distance->setFixedSize(51,28);
    m_label_left_distance->move(59,273);
    m_label_left_distance->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff;}");


    //右边距离
    m_label_right_distance = new QLabel(this);
    m_label_right_distance->setFixedSize(51+ 20,28);
    m_label_right_distance->move(642,273);
    m_label_right_distance->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff;}");
    if(1 == SUPPORT_TAIGUO)
    {
        m_label_left_distance1 = new QLabel(this);
        m_label_left_distance1->setFixedSize(51 + 20,28);
        m_label_left_distance1->move(200,273);
        m_label_left_distance1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff;}");

        m_label_right_distance1 = new QLabel(this);
        m_label_right_distance1->setFixedSize(51+ 20,28);
        m_label_right_distance1->move(494 + 10,273);
        m_label_right_distance1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff;}");
    }


    ////////////////////////////////////////////////////////////////站台信息end

    //显示普通短信背景
    m_widget_middle3 = new QWidget(this);
    m_widget_middle3->setFixedSize(555,65);
    m_widget_middle3->move(102,442);
    m_widget_middle3->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_ggbj.png);}");

    //显示短信内容
    m_label_message_content = new QLabel(this);
    m_label_message_content->setFixedSize(MESSAGE_SHOW_LENGTH,65);
    m_label_message_content->move(102 + 90,442);
    m_label_message_content->setStyleSheet("QLabel{border:0px;background:transparent;font:26px;color:#ffffff;}");
    m_label_message_content->setAlignment(Qt::AlignCenter);

    m_dynamic_text_message = new  CDynamicText(this);
    connect(m_dynamic_text_message,SIGNAL(signal_dynamic_text_event(CDynTextParam)),this,SLOT(slot_dynamic_text_event(CDynTextParam)));

    //下一趟发车时间1
    m_label_next_start_time1 = new QLabel(this);
    m_label_next_start_time1->setFixedSize(140,26);
    m_label_next_start_time1->move(15,550);
    m_label_next_start_time1->setStyleSheet("QLabel{border:0px;background:transparent;font:18px;color:#ffffff; }");
    m_label_next_start_time1->setText(tr("下一趟发车时间："));

    //下一趟发车时间2
    m_label_next_start_time2 = new QLabel(this);
    m_label_next_start_time2->setFixedSize(250,26);
    m_label_next_start_time2->move(15 + 140 + 5,550);
    m_label_next_start_time2->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff; }");


    //上车
    m_widget_up_vehicle = new QWidget(this);
    m_widget_up_vehicle->setFixedSize(120,52);
    m_widget_up_vehicle->move(15,586);
    m_widget_up_vehicle->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_ggk.png);}");




    //下车
    m_widget_down_vehicle = new QWidget(this);
    m_widget_down_vehicle->setFixedSize(120,52);
    m_widget_down_vehicle->move(15 + (120 + 6),586);
    m_widget_down_vehicle->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_ggk.png);}");




    //车内
    m_widget_within_vehicle = new QWidget(this);
    m_widget_within_vehicle->setFixedSize(120,52);
    m_widget_within_vehicle->move(15 + (120 + 6) * 2,586);
    m_widget_within_vehicle->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_ggk.png);}");




    m_label_driver = new QLabel(this);
    m_label_driver->setFixedSize(61,24);
    m_label_driver->move(420,601);
    m_label_driver->setStyleSheet("QLabel{border:0px;background:transparent;font:18px;color:#ffffff; }");
    m_label_driver->setText(tr("驾驶员:"));

    m_label_driver_name = new QLabel(this);
    m_label_driver_name->setFixedSize(90,26);
    m_label_driver_name->move(420 + 61 + 5,601);
    m_label_driver_name->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff; }");




    m_widget_line = new QWidget(this);
    m_widget_line->setFixedSize(1,18);
    m_widget_line->move(420 + 61 + 5 + 90 + 15 ,601 + 4);
    m_widget_line->setStyleSheet("QWidget {border:0px;background-color:#435261;}");

    m_label_vehicle1 = new QLabel(this);
    m_label_vehicle1->setFixedSize(75,22);
    m_label_vehicle1->move(420 + 61 + 5 + 90 + 15 + 15,601);
    m_label_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:18px;color:#ffffff; }");
    m_label_vehicle1->setText(tr("车辆编号:"));

    m_label_vehicle2 = new QLabel(this);
    m_label_vehicle2->setFixedSize(66,22);
    m_label_vehicle2->move(420 + 61 + 5 + 90 + 15 + 15 + 75 + 3,601);
    m_label_vehicle2->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff; }");


    m_CSteeringWheel = new CSteeringWheel(this);
    m_CSteeringWheel->move(1024 - 266,64);

    connect(m_CSteeringWheel,SIGNAL(signal_pushbutton_door_1()),this,SLOT(slot_pushbutton_door_1()));
    connect(m_CSteeringWheel,SIGNAL(signal_pushbutton_door_2()),this,SLOT(slot_pushbutton_door_2()));
}


void CMainPage::main_bottom_init()
{

    QString str_sheet = "";

    //播放

    m_pushbutton_play = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/play.png").arg(":/img/res/main_page/play_hover.png").arg(":/img/res/main_page/play_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_play,121,614,100,129,str_sheet);
#if 0
    m_pushbutton_play = new QPushButton(146,109,NavigateLeftButton::BottomMode,this);
    m_pushbutton_play->setNormalIconStr(":/img/res/dh_sp1.png");
    m_pushbutton_play->setPressedIconStr(":/img/res/dh_sp2.png");
    m_pushbutton_play->setActiveIconStr(":/img/res/dh_sp2.png");
    m_pushbutton_play->move(146 * 0,768 - 109);
    m_pushbutton_play->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_play->setIconSize(30,39);
#endif
    //重播

    m_pushbutton_replay = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/replay.png").arg(":/img/res/main_page/replay_hover.png").arg(":/img/res/main_page/replay_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_replay,291,614,100,129,str_sheet);
#if 0
    m_pushbutton_replay = new NavigateLeftButton(146,109,NavigateLeftButton::BottomMode,this);
    m_pushbutton_replay->setNormalIconStr(":/img/res/dh_sx1.png");
    m_pushbutton_replay->setPressedIconStr(":/img/res/dh_sx2.png");
    m_pushbutton_replay->setActiveIconStr(":/img/res/dh_sx2.png");
    m_pushbutton_replay->move(146 * 1,768 - 109);
    m_pushbutton_replay->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_replay->setIconSize(35,39);
#endif

    //服务用语
    m_pushbutton_service_terams = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/voice.png").arg(":/img/res/main_page/voice_hover.png").arg(":/img/res/main_page/voice_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_service_terams,631,614,100,129,str_sheet);
#if 0
    m_pushbutton_service_terams = new NavigateLeftButton(146,109,NavigateLeftButton::BottomMode,this);
    m_pushbutton_service_terams->setNormalIconStr(":/img/res/dh_xx1.png");
    m_pushbutton_service_terams->setPressedIconStr(":/img/res/dh_xx2.png");
    m_pushbutton_service_terams->setActiveIconStr(":/img/res/dh_xx2.png");
    m_pushbutton_service_terams->move(146 * 2,768 - 109);
    m_pushbutton_service_terams->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_service_terams->setIconSize(30,39);
#endif
    //菜单
    m_pushbutton_menu = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/menu.png").arg(":/img/res/main_page/menu_hover.png").arg(":/img/res/main_page/menu_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_menu,461,614,100,129,str_sheet);

#if 0
    m_pushbutton_menu = new NavigateLeftButton(146,109,NavigateLeftButton::BottomMode,this);

    m_pushbutton_menu->setNormalIconStr(":/img/res/dh_fl1.png");
    m_pushbutton_menu->setPressedIconStr(":/img/res/dh_fl2.png");
    m_pushbutton_menu->setActiveIconStr(":/img/res/dh_fl2.png");
    m_pushbutton_menu->move(146 * 3,768 - 109);
    m_pushbutton_menu->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_menu->setIconSize(30,38);
#endif

    //16画面分

    m_pushbutton_16_frame = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/video.png").arg(":/img/res/main_page/video_hover.png").arg(":/img/res/main_page/video_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_16_frame,801,614,100,129,str_sheet);
#if 0
    m_pushbutton_16_frame = new NavigateLeftButton(146,109,NavigateLeftButton::BottomMode,this);
    m_pushbutton_16_frame->setNormalIconStr(":/img/res/dh_lx1.png");
    m_pushbutton_16_frame->setPressedIconStr(":/img/res/dh_lx2.png");
    m_pushbutton_16_frame->setActiveIconStr(":/img/res/dh_lx2.png");
    m_pushbutton_16_frame->move(146 * 4,768 - 109);
    m_pushbutton_16_frame->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_16_frame->setIconSize(36,39);
#endif
    //短信上传事件
#if 0
    m_pushbutton_message_upload = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res//dh_sc1.png").arg(":/img/res/dh_sc2.png").arg(":/img/res/dh_sc2.png");

    CCommonInterface ::init_new_button(m_pushbutton_message_upload,0 + 146 * 5 + (146 -36)/2,768 - 109 + (109 - 39)/2,36,39,str_sheet);

    m_pushbutton_message_upload = new NavigateLeftButton(146,109,NavigateLeftButton::BottomMode,this);
    m_pushbutton_message_upload->setNormalIconStr(":/img/res/dh_sc1.png");
    m_pushbutton_message_upload->setPressedIconStr(":/img/res/dh_sc2.png");
    m_pushbutton_message_upload->setActiveIconStr(":/img/res/dh_sc2.png");
    m_pushbutton_message_upload->move(146 * 5,768 - 109);
    m_pushbutton_message_upload->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_message_upload->setIconSize(36,39);

    //查看设备运行状态

    m_pushbutton_run_status = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res//dh_xtgl1.png").arg(":/img/res/dh_xtgl2.png").arg(":/img/res/dh_xtgl2.png");

    CCommonInterface ::init_new_button(m_pushbutton_run_status,0 + 146 * 6 + (146 -36)/2,768 - 109 + (109 - 39)/2,36,39,str_sheet);

    m_pushbutton_run_status = new NavigateLeftButton(146,109,NavigateLeftButton::BottomMode,this);
    m_pushbutton_run_status->setNormalIconStr(":/img/res/dh_xtgl1.png");
    m_pushbutton_run_status->setPressedIconStr(":/img/res/dh_xtgl2.png");
    m_pushbutton_run_status->setActiveIconStr(":/img/res/dh_xtgl2.png");
    m_pushbutton_run_status->move(146 * 6,768 - 109);
    m_pushbutton_run_status->setFocusPolicy(Qt::NoFocus);
    m_pushbutton_run_status->setIconSize(48,39);
#endif

    connect(m_pushbutton_play,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play()));
    connect(m_pushbutton_replay,SIGNAL(clicked()),this,SLOT(slot_pushbutton_replay()));
    connect(m_pushbutton_service_terams,SIGNAL(clicked()),this,SLOT(slot_pushbutton_service_terams()));
    connect(m_pushbutton_menu,SIGNAL(clicked()),this,SLOT(slot_pushbutton_menu()));
    connect(m_pushbutton_16_frame,SIGNAL(clicked()),this,SLOT(slot_pushbutton_16_frame()));
    //connect(m_pushbutton_message_upload,SIGNAL(clicked()),this,SLOT(slot_pushbutton_message_upload()));
    //connect(m_pushbutton_run_status,SIGNAL(clicked()),this,SLOT(slot_pushbutton_run_status()));

    connect(this,SIGNAL(signal_main_page_message_upload()),this,SLOT(slot_pushbutton_message_upload()));
    connect(this,SIGNAL(signal_main_page_run_status()),this,SLOT(slot_pushbutton_run_status()));


}
#include "arrive_widget.h"
//播放
void CMainPage::slot_pushbutton_play()
{
    emit signal_pushbutton_play();
}
//重播
void CMainPage::slot_pushbutton_replay()
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        cJSON_AddStringToObject(root, "msg_type", "gui_station_info");

        cJSON_AddNumberToObject(root, "station_replay", 1);

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}
//服务用语
void CMainPage::slot_pushbutton_service_terams()
{
    static CServiceDialog *pServiceDialog = NULL;

    if(NULL == pServiceDialog)
    {
        pServiceDialog = new CServiceDialog(this);
        pServiceDialog->move((1024 - 723)/2,(768 - 551)/2);;
    }
    pServiceDialog->exec();

}


//菜单
void CMainPage::slot_pushbutton_menu()
{
    emit signal_pushbutton_menu();
}
//16画面分
void CMainPage::slot_pushbutton_16_frame()
{
#if 0
    static CVideoWidget *pVideoWidget = NULL;

    if(NULL == pVideoWidget)
    {
        pVideoWidget = new CVideoWidget(this);
#ifdef CURRENT_ARM_RUN
        pVideoWidget->move(0,0);
#else
        pVideoWidget->move(20,50);
#endif
    }
    pVideoWidget->exec();
#endif

    emit signal_pushbutton_16_frame();
}
//短信上传事件
void CMainPage::slot_pushbutton_message_upload()
{
#if 0
    static CMessageWidget *pMessageWidget = NULL;

    if(NULL == pMessageWidget)
    {
        pMessageWidget = new CMessageWidget(this);
        pMessageWidget->move(0,0);
    }
    pMessageWidget->exec();
    #endif
}
//查看设备运行状态
void CMainPage::slot_pushbutton_run_status()
{
    static CRunStatusWidget *pRunStatusWidget = NULL;

    if(NULL == pRunStatusWidget)
    {
        pRunStatusWidget = new CRunStatusWidget(this);
        pRunStatusWidget->move(0,0);
    }
    pRunStatusWidget->exec();
}

void CMainPage::slot_pushbutton_bianma3()
{
    //static bool show_change = true;
#if 0
    CMainMessageDialog::getInstance()->move((1024 - 723)/2,(768 - 551)/2);;

    if(show_change)
    {
        show_change = false;
        CMainMessageDialog::getInstance()->set_show_content(CMainMessageDialog::kEnumOperateDialog,
                    tr("2016-10-31 10点50分"),
                    tr("4小时30分"));
    }
    else
    {
        show_change = true;
        CMainMessageDialog::getInstance()->set_show_content(CMainMessageDialog::kEnumMessageDialog,
            tr("雨天路滑，请小心驾驶!"),
            "");
    }

    CMainMessageDialog::getInstance()->exec();
#endif
    QString show_content = tr("无数据");

    CMainMessageDialog::getInstance()->move((1024 - 723)/2,(768 - 551)/2);;

    if(1 == CCommonData::getInstance()->m_scheluing_plan.status)
    {
        show_content = /*tr("发车时间") + */CCommonData::getInstance()->m_scheluing_plan.m_msg_content;
        CMainMessageDialog::getInstance()->set_show_content(CMainMessageDialog::kEnumOperateDialog,
                    show_content,
                    tr(""));
    }
    else if(2 == CCommonData::getInstance()->m_scheluing_plan.status)
    {
        show_content = CCommonData::getInstance()->m_scheluing_plan.m_msg_content;
            CMainMessageDialog::getInstance()->set_show_content(CMainMessageDialog::kEnumMessageDialog,
        show_content,
        "");
    }
    else if(3 == CCommonData::getInstance()->m_scheluing_plan.status)
    {
        show_content = tr("计划取消") + CCommonData::getInstance()->m_scheluing_plan.m_msg_content;
            CMainMessageDialog::getInstance()->set_show_content(CMainMessageDialog::kEnumMessageDialog,
        show_content,
        "");
    }
    else
    {
          CMainMessageDialog::getInstance()->set_show_content(CMainMessageDialog::kEnumOperateDialog,
                    tr(""),
                    tr(""));
    }

    CMainMessageDialog::getInstance()->exec();

}

void CMainPage::main_page_notify(int _msg,int _event,void *_param)
{
    QString str_sheet = "";

    switch(_msg)
    {
        ////////////////////////////////////////////////////////////////////头部显示 start
        case kEnumMainPageSignal1://SIM卡　2G 3G 4G
        case kEnumMainPageSignal2: //SIM卡信号强弱
        case kEnumMainPageGpsNumber://GPS个数
        case kEnumMainPageGpsStatus://GPS状态
        case kEnumMainPageAccStatus://ACC状态
        case kEnumMainPageDeviceDoorStatus: //设备门（机器上）状态
        case kEnumMainPageDeviceAbnorma://设备异常（机器上）
        case kEnumMainPageHardDiskStatus://硬盘状态
        case kEnumMainPagePlatformStatus://平台连接状态
        case kEnumMainPagePlatformStatus1://平台连接状态
        case kEnumMainPageTMPPlatformStatus://TMP平台连接状态
        case kEnumMainPageMessageStatus: //消息状态
        case kEnumMainPageFrontDoor:     //前门
        case kEnumMainPageBackDoor:       //后门
        case kEnumMainPageSdStatus:
        {
            m_CHeadWidget->head_widget_notify(_msg,_event,_param);
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
             emit signal_main_page_notify(_msg,_event,_param);
#endif
        }break;
        ////////////////////////////////////////////////////////////////////头部显示 end

        ////////////////////////////////////////////////////////////////////中间左边显示　start
        case kEnumMainPageVehicleModel: //车的型号
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_bianma1->setText(pData->m_str_data);
#if defined(APPLICATION_TYPE_P2)
            emit signal_main_page_notify(_msg,_event,_param);
#endif
        }break;
        case kEnumMainPageVehicleUpDown: //车是上行或下行
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_bianma2->setText(pData->m_str_data);

            if(pData->m_str_data.contains("上行"))
                CCommonData::getInstance()->m_runStatusInfo.m_CStationInfo->status = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CStationInfo->status = 2;

            CCommonData::getInstance()->m_runStatusInfo.m_type = 6;

            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

        }break;
        case kEnumMainPageVehicleStatus: //车的状态(运营中)
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_pushbutton_bianma3->setText(pData->m_str_data);
        }break;
        case kEnumMainPageLeftDistance: //左边车的距离
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_left_distance->setText(pData->m_str_data);
        }break;
        case  kEnumMainPageRightDistance://右边车的距离
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_right_distance->setText(pData->m_str_data);
        }break;
        case kEnumMainPageLeftDistance1: //左边车的距离(泰国)
        {
            if(1 != SUPPORT_TAIGUO)
                break;
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_left_distance1->setText(pData->m_str_data);
        }break;
            case kEnumMainPageRightDistance1://右边车的距离(泰国)
        {
            if(1 != SUPPORT_TAIGUO)
                break;
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_right_distance1->setText(pData->m_str_data);
        }break;
        case kEnumMainPageUpdateStationName:
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
            {
                CDynTextParam param_station_name;
                param_station_name.m_control_this = m_pushbutton_station_name;
                param_station_name.m_control_type = CDynamicText::kEnumPushbutton;
                param_station_name.m_control_id   = kEnumPushbuttonStationName;
                param_station_name.m_control_source_text = pData->m_str_data;
                param_station_name.m_control_font_size = STATION_NAME_SHOW_FONT_SIZE;
                param_station_name.m_control_show_length = STATION_NAME_SHOW_LENGTH;

                m_dynamic_text_station_name->set_text(param_station_name);
                CCommonData::getInstance()->m_station_info.m_current_station = pData->m_str_data;
                emit signal_main_page_arrive_widget_event(CArriveWidget::kEnumSetStationName,0,pData->m_str_data);

                CCommonData::getInstance()->m_runStatusInfo.m_type = 6;
                CCommonData::getInstance()->m_runStatusInfo.m_CStationInfo->current_station = pData->m_str_data;
                CCommonData::getInstance()->m_runStatusInfo.m_CStationInfo->current_id = pData->m_i_data;
                emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
            }
#if defined(APPLICATION_TYPE_P2)
            emit signal_main_page_notify(_msg,_event,_param);
#endif
        }break;
        case  kEnumMainPageCurrentStationName://当前站名
        {
             CEventData *pData = (CEventData *)_param;
            if(pData)
                m_pushbutton_station_name->setText(pData->m_str_data);

        }break;
        case  kEnumMainPageCurrentMessage: //当前短信信息
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_message_content->setText(pData->m_str_data);
        }break;
        case  kEnumMainPageStartTime:      //发车时间
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_next_start_time2->setText(pData->m_str_data);
        }break;
        case  kEnumMainPageUpVehicleNumber:    //上车人数
        {
            m_label_up_vehicle2->setText(QString::number(_event));
            CCommonData::getInstance()->m_passenger_flow.m_up_vehicle = _event;
            emit signal_main_page_arrive_widget_event(CArriveWidget::kEnumSetUpVehicle,_event,"");
        }break;
        case  kEnumMainPageDownVehicleNumber:  //下车人数
        {
            m_label_down_vehicle2->setText(QString::number(_event));
            CCommonData::getInstance()->m_passenger_flow.m_down_vehicle = _event;
            emit signal_main_page_arrive_widget_event(CArriveWidget::kEnumSetDownVehicle,_event,"");
        }break;
        case  kEnumMainPageWithinVehicleNumber://车内人数
        {
            m_label_within_vehicle2->setText(QString::number(_event));
            CCommonData::getInstance()->m_passenger_flow.m_within_vehicle = _event;
            emit signal_main_page_arrive_widget_event(CArriveWidget::kEnumSetWithinVehicle,_event,"");
        }break;
        case  kEnumMainPageDriverName:     //驾驶员名字
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                m_label_driver_name->setText(pData->m_str_data);
        }break;
        case  kEnumMainPageVehicleNumber://车辆编号
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
                 m_label_vehicle2->setText(pData->m_str_data);
        }break;
        //新增加20161101
        case kEnumMainPageFrontVehicle:  //前车的距离
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
            {
                 m_label_front_vehicle1->setText(pData->m_str_data);
                 int width = CCommonInterface::get_string_length(pData->m_str_data,46);
                 m_label_front_vehicle1->setText(pData->m_str_data);
                 m_label_front_vehicle1->setFixedWidth(width);
                 m_label_front_vehicle2->move(m_label_front_vehicle1->pos().x() + width + 5,203);
            }
        }break;
        case kEnumMainPageBackVehicle://后车的距离
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
            {
                 int width = CCommonInterface::get_string_length(pData->m_str_data,46);
                 m_label_back_vehicle1->setText(pData->m_str_data);
                 m_label_back_vehicle1->setFixedWidth(width);
                 m_label_back_vehicle2->move(m_label_back_vehicle1->pos().x() + width + 5,203);
            }
        }break;
        case kEnumMainPageFrontStation1:     //前一站1
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
            {
                CCommonInterface::get_string_limitEx(pData->m_str_data,m_label_front_station_1->width(),pData->m_str_data,30);
                 m_label_front_station_1->setText(pData->m_str_data);

                CCommonData::getInstance()->m_runStatusInfo.m_type = 6;
                CCommonData::getInstance()->m_runStatusInfo.m_CStationInfo->start_station = CCommonData::getInstance()->m_sd_info.m_first_station;
                emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
             }
        }break;
        case kEnumMainPageFrontStation2:     //前一站2
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
            {
                CCommonInterface::get_string_limitEx(pData->m_str_data,m_label_front_station_2->width(),pData->m_str_data,30);
                 m_label_front_station_2->setText(pData->m_str_data);
            }
        }break;
        case kEnumMainPageBackStation1:      //前一站1
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
            {
                 CCommonInterface::get_string_limitEx(pData->m_str_data,m_label_back_station_1->width(),pData->m_str_data,30);
                 m_label_back_station_1->setText(pData->m_str_data);
            }
        }break;
        case kEnumMainPageBackStation2:      //前一站2
        {
            CEventData *pData = (CEventData *)_param;
            if(pData)
            {
                CCommonInterface::get_string_limitEx(pData->m_str_data,m_label_back_station_2->width(),pData->m_str_data,30);
                 m_label_back_station_2->setText(pData->m_str_data);

                CCommonData::getInstance()->m_runStatusInfo.m_type = 6;
                CCommonData::getInstance()->m_runStatusInfo.m_CStationInfo->end_station = CCommonData::getInstance()->m_sd_info.m_last_station;
                emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
            }
        }break;
        case kEnumMainPageVehicle1:    //前车
        {
            if(kEnumVehicleStationFront == _event)//靠近站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_1_distance->setText(pData->m_str_data);

                m_label_vehicle_1->move(317,203);
                int pos_x = m_label_vehicle_1->pos().x() + m_label_vehicle_1->width()/2 - 100/2;
                int pos_y = m_label_vehicle_1->pos().y() - 50 - 5;

                m_label_vehicle_1_distance->move(pos_x,pos_y);

                m_label_vehicle_1->show();
                m_label_vehicle_1_distance->show();
            }
            else if(kEnumVehicleStationCurrent == _event)//到站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_1_distance->setText(pData->m_str_data);

                m_label_vehicle_1->move(317 + 38,203);
                int pos_x = m_label_vehicle_1->pos().x() + m_label_vehicle_1->width()/2 - 100/2;
                int pos_y = m_label_vehicle_1->pos().y() - 50 - 5;

                m_label_vehicle_1_distance->move(pos_x,pos_y);

                m_label_vehicle_1->show();
                m_label_vehicle_1_distance->show();
            }
            else if(kEnumVehicleStationBack == _event)//离开站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_1_distance->setText(pData->m_str_data);

                m_label_vehicle_1->move(317 + 80,203);
                int pos_x = m_label_vehicle_1->pos().x() + m_label_vehicle_1->width()/2 - 100/2;
                int pos_y = m_label_vehicle_1->pos().y() - 50 - 5;

                m_label_vehicle_1_distance->move(pos_x,pos_y);

                m_label_vehicle_1->show();
                m_label_vehicle_1_distance->show();

            }
            else
            {
                m_label_vehicle_1->hide();
                m_label_vehicle_1_distance->hide();
            }

        }break;
        case kEnumMainPageVehicle2:    //当前车
        {
           if(kEnumVehicleStationFront == _event)//靠近站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_2_distance->setText(pData->m_str_data);

                m_label_vehicle_2->move(465,203);
                int pos_x = m_label_vehicle_2->pos().x() + m_label_vehicle_2->width()/2 - 100/2;
                int pos_y = m_label_vehicle_2->pos().y() - 50 - 5;

                m_label_vehicle_2_distance->move(pos_x,pos_y);

                m_label_vehicle_2->show();
                m_label_vehicle_2_distance->hide();
            }
            else if(kEnumVehicleStationCurrent == _event)//到站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_2_distance->setText(pData->m_str_data);

                m_label_vehicle_2->move(465 + 33,203);
                int pos_x = m_label_vehicle_2->pos().x() + m_label_vehicle_2->width()/2 - 100/2;
                int pos_y = m_label_vehicle_2->pos().y() - 50 - 5;

                m_label_vehicle_2_distance->move(pos_x,pos_y);

                m_label_vehicle_2->show();
                m_label_vehicle_2_distance->hide();
            }
            else if(kEnumVehicleStationBack == _event)//离开站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_2_distance->setText(pData->m_str_data);

                m_label_vehicle_2->move(465 + 65,203);
                int pos_x = m_label_vehicle_2->pos().x() + m_label_vehicle_2->width()/2 - 100/2;
                int pos_y = m_label_vehicle_2->pos().y() - 50 - 5;

                m_label_vehicle_2_distance->move(pos_x,pos_y);

                m_label_vehicle_2->show();
                m_label_vehicle_2_distance->hide();

            }
            else
            {
                m_label_vehicle_2->hide();
                m_label_vehicle_2_distance->hide();
            }


        }break;
        case kEnumMainPageVehicle3:    //后车
        {
            if(kEnumVehicleStationFront == _event)//靠近站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_3_distance->setText(pData->m_str_data);

                m_label_vehicle_3->move(578,203);
                int pos_x = m_label_vehicle_3->pos().x() + m_label_vehicle_3->width()/2 - 100/2;
                int pos_y = m_label_vehicle_3->pos().y() - 50 - 5;

                m_label_vehicle_3_distance->move(pos_x,pos_y);

                m_label_vehicle_3->show();
                m_label_vehicle_3_distance->show();
            }
            else if(kEnumVehicleStationCurrent == _event)//到站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_3_distance->setText(pData->m_str_data);

                m_label_vehicle_3->move(578 + 60,203);
                int pos_x = m_label_vehicle_3->pos().x() + m_label_vehicle_3->width()/2 - 100/2;
                int pos_y = m_label_vehicle_3->pos().y() - 50 - 5;

                m_label_vehicle_3_distance->move(pos_x,pos_y);

                m_label_vehicle_3->show();
                m_label_vehicle_3_distance->show();
            }
            else if(kEnumVehicleStationBack == _event)//离开站
            {
                CEventData *pData = (CEventData *)_param;
                if(pData)
                    m_label_vehicle_3_distance->setText(pData->m_str_data);

                m_label_vehicle_3->move(578 + 90,203);
                int pos_x = m_label_vehicle_3->pos().x() + m_label_vehicle_3->width()/2 - 100/2;
                int pos_y = m_label_vehicle_3->pos().y() - 50 - 5;

                m_label_vehicle_3_distance->move(pos_x,pos_y);

                m_label_vehicle_3->show();
                m_label_vehicle_3_distance->show();

            }
            else
            {
                m_label_vehicle_3->hide();
                m_label_vehicle_3_distance->hide();
            }

        }break;
        ////////////////////////////////////////////////////////////////////中间左边显示　end
        ////////////////////////////////////////////////////////////////////中间右边显示　start
        case kEnumMainPageLimitSpeed:    //限速
        {
             m_label_limit_speed1->setText(QString::number(_event));
             CCommonData::getInstance()->m_station_info.m_current_limspeed = _event;
             emit signal_main_page_arrive_widget_event(CArriveWidget::kEnumSetLimitSpeed,_event,"");
        }break;
        case kEnumMainPageCurrentSpeed:  //当前速度
        {
             m_label_current_speed->setText(QString::number(_event));
             CCommonData::getInstance()->m_station_info.m_current_speed = _event;
             emit signal_main_page_arrive_widget_event(CArriveWidget::kEnumSetCurrentSpeed,_event,"");
        }break;
        case kEnumMainPageSpeedAlert:    //超速警报
        {
            //m_CSteeringWheel->steering_wheel_notify(_msg,_event,_param);
        }break;
        case kEnumMainPageStationStatus:
        {
            QString str_status = "";
            if(kEnumStationIn == _event)
            {
                CCommonData::getInstance()->station_status = 1;
                str_status = CLtyString::getInstance()->m_station_in;
            }
            else
            {
                CCommonData::getInstance()->station_status = 2;
                str_status = CLtyString::getInstance()->m_station_out;
            }
#if defined(APPLICATION_TYPE_P2)
            emit signal_main_page_notify(_msg,_event,_param);
#endif
            m_label_station_status->setText(str_status);
            emit signal_main_page_arrive_widget_event(CArriveWidget::kEnumSetVehicleStatus,0,str_status);
        }break;

        ////////////////////////////////////////////////////////////////////中间右边显示　end
        case kEnumMainPageAdvert:
        case kEnumMainPageVerificationRrStatus:
        case kEnumMainPageLine:
        case kEnumMainPageChargeType:
        case kEnumMainPageAmount:        //金额
        case kEnumMainPageBalance:       //余额
        case kEnumMainPageArriveStation:  //到站
        case kEnumMainPageUpdateStationName1:
        case kEnumMainPageFromStatus:
        case kEnumMainPageFromMachine:
        {
#if defined(APPLICATION_TYPE_P2)
           if(kEnumMainPageChargeType ==  _msg)
           {
                qDebug()<<"main_page_notify kEnumMainPageChargeType  _event:"<<_event;
           }
           emit signal_main_page_notify(_msg,_event,_param);
#endif
        }break;

    }
}

void CMainPage::set_current_station_name(QString _name)
{



}

void CMainPage::slot_pushbutton_station_name()
{
    if(CVoiceDialog::getIsEmpty())
    {
        connect(this,SIGNAL(signal_voice_dialog_event(int,int,int)),CVoiceDialog::getInstance(),SLOT(slot_voice_dialog_event(int,int,int)));
    }

    CVoiceDialog::getInstance()->move((1024 - 723)/2,(768 - 551)/2);
    CVoiceDialog::getInstance()->data_init();
    CVoiceDialog::getInstance()->exec();
}

void CMainPage::slot_dynamic_text_event(const CDynTextParam &_param)
{
    QString str_sheet = "";
    switch(_param.m_control_id)
    {
        case CMainPage::kEnumPushbuttonStationName:
        {
            switch(_param.m_action)
            {
                case CDynamicText::kEnumAlignmentLeft: //文字左对齐
                {
                    QPushButton *pButton = (QPushButton *)_param.m_control_this;
                       #ifdef CURRENT_ARM_RUN
                       str_sheet = QString("QPushButton {text-align:left;border:0px;background:transparent;font:bold 84px;color:#ffffff; }");
                       #else
                       str_sheet = QString("QPushButton {text-align:left;border:0px;background:transparent;font: 84px;color:#ffffff; }");
                       #endif
                       CCommonInterface::modify_button_sheet(pButton,str_sheet);
                        //QFont button_font = pButton->font();
                        //button_font.setBold(true);
                        //pButton->setFont(button_font);

                }break;
                case CDynamicText::kEnumAlignmentRight://文字居中对齐
                {
                    QPushButton *pButton = (QPushButton *)_param.m_control_this;
                    #ifdef CURRENT_ARM_RUN
                    str_sheet = QString("QPushButton {text-align:center;border:0px;background:transparent;font:bold 84px;color:#ffffff; }");
                    #else
                    str_sheet = QString("QPushButton {text-align:center;border:0px;background:transparent;font: 84px;color:#ffffff; }");
                    #endif
                    CCommonInterface::modify_button_sheet(pButton,str_sheet);
                    //QFont button_font = pButton->font();
                      //  button_font.setBold(true);
                       // pButton->setFont(button_font);
                }break;
                case CDynamicText::kEnumUpdateText:    //更新文本显示
                {
                    CEventData eventData;
                    eventData.m_str_data = _param.m_control_show_text;
                    main_page_notify(kEnumMainPageCurrentStationName,0,&eventData);         //当前站名

                }break;
            }
        }break;
    case CMainPage::kEnumLabelMessageShow:
    {
        switch(_param.m_action)
        {
            case CDynamicText::kEnumAlignmentLeft: //文字左对齐
            {
                m_label_message_content->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }break;
            case CDynamicText::kEnumAlignmentRight://文字居中对齐
            {
                m_label_message_content->setAlignment(Qt::AlignCenter);
            }break;
            case CDynamicText::kEnumUpdateText:    //更新文本显示
            {
                CEventData eventData;
                eventData.m_str_data = _param.m_control_show_text;
                main_page_notify(kEnumMainPageCurrentMessage,0,&eventData);

            }break;
        }
    }break;
    }
}

void CMainPage::slot_pushbutton_door_1()
{

}
void CMainPage::slot_pushbutton_door_2()
{

}

int CMainPage::slot_main_page_message(unsigned int addr)
{
    cJSON *cjson = (cJSON *)addr;
    int byRet;
    if(NULL == cjson)
        return 0;
    byRet = main_page_handle(cjson);
    cJSON_Delete(cjson);


    return byRet;
}

#if 0
#include <QDebug>
static TOUCH_ATTR_S gTouchAttr;
#include "./class/common/customevent.h"

static void *guiMsgDataParse(void *arg)
{
    pthread_detach( pthread_self());

    unsigned int len = 0,addr = 0;
    CMainPage *mWiget = (CMainPage *)arg;
    char message_type;
    while(1)
    {
        gTouchAttr.x = -110;
        if(GuiMsgListGet(&gTouchAttr,&len,&addr,1,&message_type))
        {
            if(mWiget)
            {

                if(kEnumUiMessageTouch == message_type)
                {

                    if(mWiget->current_first_press())
                    {


                        static QTime gui_msg_tick;
                        static bool first_start = true;
                        if(first_start)
                        {
                            gui_msg_tick.start();
                            first_start = false;
                        }

                        int tick_count = gui_msg_tick.elapsed();
                        if(tick_count >= 5)
                        {
                           // g_time_tick.begin();
                            gui_msg_tick.restart();
                            //QApplication::postEvent((QObject *)mWiget, new CustomEvent(CustomEvent::m_lty_root, (void *)addr));
                            if(-110 != gTouchAttr.x)
                            {
                                mWiget->handle_mouse_event(gTouchAttr.x,gTouchAttr.y,false);
                            }

                        }


                    }
                    else
                    {

                        if(-110 != gTouchAttr.x)
                        {

                            mWiget->handle_mouse_event(gTouchAttr.x,gTouchAttr.y,false);
                        }
                        //QApplication::postEvent((QObject *)mWiget, new CustomEvent(CustomEvent::m_lty_root, (void *)addr));
                    }
                }
                else
                {
                    QApplication::postEvent((QObject *)mWiget, new CustomEvent(CustomEvent::m_lty_root, (void *)addr));
                }

            }
        }
    }

    pthread_exit(0);
}

void CMainPage::customEvent(QEvent *e)
{
    CustomEvent *pCustom = static_cast<CustomEvent *>(e);
    if (NULL ==  pCustom)
    {
        return;
    }

    switch (e->type())
    {
        case QEVENT_LTY_ROOT:
        {
            emit signal_main_page_message((unsigned int)pCustom->m_pParam);
            //delete pData;

        }break;
    }
}
#endif
#include <QTextCodec>
//QString GBK2UTF8(const char *inStr)
//{
//    QTextCodec *gbk = QTextCodec::codecForName("GB2312");
//    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

//    QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8
//    return g2u;
//}

QString gb2312ToUtf8(char *_gb2312)
{

    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode= gb2312Codec->toUnicode(_gb2312);
    QByteArray ByteUtf8= utf8Codec->fromUnicode(strUnicode);

    //strGb2312= ByteUtf8.data();
    return ByteUtf8;
}


#include <QWSServer>

void sendCharacter(unsigned int character)
{

    QPointer<QWidget> w = QApplication::focusWidget();

    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, character, Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);



    QKeyEvent keyRelease(QEvent::KeyRelease, character, Qt::NoModifier, QString());
    QApplication::sendEvent(w, &keyRelease);
}

void CMainPage::quit_interface()
{

    int point_x = CMyDefineScreen::getInstance()->get_change_factor_x(950) + MainWindow::getInstance()->pos().x();
    int point_y = CMyDefineScreen::getInstance()->get_change_factor_y(40) + MainWindow::getInstance()->pos().y();

    QPoint point(point_x,point_y);

    CCommonInterface::input_panel_control(false);
    if(CMyMouseEvent::getInstance()->is_head_back(point))
    {
        CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_EXIT 1");
        CMyMouseEvent::getInstance()->touch_click_event(point.x(),point.y(),false);
        CMyMouseEvent::getInstance()->touch_click_event(point.x(),point.y(),true);
    }

}

bool CMainPage::keyboard_value(int _key)
{
    bool ret_value = false;
    do
    {
        QWidget *active_window = QApplication::activeWindow();
        QWidget *stack_window = NULL;

        if(CCommonInterface::input_panel_control(false))
        {
            ret_value = true;
            break;
        }
        //CCommonInterface::input_panel_control(false);

        qDebug() << active_window;
        if(active_window)
        {
            if("MainWindow" == active_window->objectName())
            {
                stack_window = MainWindow::getInstance()->get_stack_widget();
                if(stack_window && (
#ifndef APPLICATION_TYPE_D2
                "CVideoWidget" == stack_window->objectName() ||
#endif
                "CArriveWidget" == stack_window->objectName() ||
                "CMenuWidget" == stack_window->objectName()
                #if 1
                || "CSystemManage" == stack_window->objectName()
                #endif
                ))
                {
                    CParentWidget *pParentWidget = (CParentWidget *)stack_window;
                    ret_value = pParentWidget->keyboard_value(_key);
                }
            }
            else if("CVoiceDialog" == active_window->objectName() ||
                    "CMyMessageBox" == active_window->objectName() ||
                    "CCardDialog" == active_window->objectName() ||
                    "CMainMessageDialog" == active_window->objectName() ||
                    "CMyExport" == active_window->objectName() ||
                    "CPasswordDialog" == active_window->objectName() ||
                    "CPassword1Dialog" == active_window->objectName() ||
                    "CProgressBox" == active_window->objectName() ||
                    "CServiceDialog" == active_window->objectName())
            {
                  CParentWidget *pParentWidget = (CParentWidget *)active_window;
                  ret_value = pParentWidget->keyboard_value(_key);
            }

        }

    }while(0);
    return ret_value;
}

void CMainPage::gui_system_key_up(cJSON *root,bool _monitor,int _value)
{
    cJSON *pCJSONFunCode = NULL;
    int key_value = 0;
    do
    {
        if(_monitor)
        {
            key_value = _value;
        }
        else
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"key_code");
            if(pCJSONFunCode)
            {
                key_value = pCJSONFunCode->valueint;
            }
            CCommonInterface::printf_debug("gui_system_key_up value:0x" + QString::number(pCJSONFunCode->valueint,16));
        }

#ifndef APPLICATION_TYPE_D2
        if(keyboard_value(key_value))
        {
            CCommonInterface::printf_debug("keyboard_value() return :" + QString::number(key_value,16));
            return;
        }
#endif
        if(!_monitor)
        {
            if(KEYPAD_KEY7_FUN != pCJSONFunCode->valueint)
            {
                m_key_times = 0;
            }
        }

        switch(key_value)
        {
            case KEYPAD_KEY7_F0:
            {
                //sendKeyEvent( Qt::Key_1,Qt::Key_1,0,1,0);
                //sendKeyEvent( Qt::Key_1,Qt::Key_1,0,0,0);
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F0");
                //QWSServer::sendKeyEvent(Qt::Key_1,Qt::Key_1,0,1,0);
                //QWSServer::sendKeyEvent(Qt::Key_1,Qt::Key_1,0,0,0);
                sendCharacter(Qt::Key_0);


            }break;
            case KEYPAD_KEY7_F1:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F1");
                sendCharacter(Qt::Key_1);

            }break;
            case KEYPAD_KEY7_F2:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F2");
                sendCharacter(Qt::Key_2);
            }break;
            case KEYPAD_KEY7_F3:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F3");
                sendCharacter(Qt::Key_3);
            }break;
            case KEYPAD_KEY7_F4:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F4");
                sendCharacter(Qt::Key_4);
            }break;
            case KEYPAD_KEY7_F5:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F5");
                sendCharacter(Qt::Key_5);
            }break;
            case KEYPAD_KEY7_F6:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F6");
                sendCharacter(Qt::Key_6);
            }break;
            case KEYPAD_KEY7_F7:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F7");
                sendCharacter(Qt::Key_7);
            }break;
            case KEYPAD_KEY7_F8:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F8");
                sendCharacter(Qt::Key_8);
            }break;
            case KEYPAD_KEY7_F9:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_F9");
                sendCharacter(Qt::Key_9);
            }break;
            case KEYPAD_KEY7_PLAY:
            {
#ifndef APPLICATION_TYPE_D2
                if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id() ||
                   MainWindow::kEnumPlay == MainWindow::getInstance()->get_current_widget_id())
                    slot_pushbutton_play();
#endif
            }break;
            case KEYPAD_KEY7_REPLAY:
            {
#ifndef APPLICATION_TYPE_D2
                if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id() ||
                   MainWindow::kEnumPlay == MainWindow::getInstance()->get_current_widget_id())
                    slot_pushbutton_replay();
#endif
            }break;
            case KEYPAD_KEY7_MENU:
            {
                int enter_value = Qt::Key_Return;
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_MENU");
                if(MainWindow::getInstance()->isVisible() && MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
                    slot_pushbutton_menu();
                else
                    sendCharacter(enter_value);//sendCharacter(Qt::Key_Return);
                qDebug() << "KEYPAD_KEY7_MENU:" << QString::number(enter_value,16);
            }break;
            case KEYPAD_KEY7_EXIT:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_EXIT");
/////////////////////////////////////////////////////////////////////////start
                quit_interface();//star
/////////////////////////////////////////////////////////////////////////end
#if 0
                if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
                {
                    if(!MainWindow::getInstance()->isVisible())
                    {
                         emit signal_mainwindow_show(true);
                         CProtocol::getInstance()->call_json_system_passenger_flow(7,2,0);
                    }
                }
#endif


            }break;
            case KEYPAD_KEY7_FUN:
            {
                CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_FUN");
#if 0
                if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
                {
                    m_key_times++;
                    if(!m_timer_key_5s->isActive())
                        m_timer_key_5s->start();

                    if(m_key_times >= 3)
                    {
                        m_key_times = 0;

                        emit signal_mainwindow_show(false);
                        CProtocol::getInstance()->call_json_system_passenger_flow(7,1,0);
                    }

                }
#endif
            }break;
            case KEYPAD_KEY7_UP:
            {
                 CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_UP");

#ifndef APPLICATION_TYPE_D2
                 if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
                    slot_pushbutton_arrow_left();
                 else
                    sendCharacter(Qt::Key_Up);
#endif
            }break;
            case KEYPAD_KEY7_LEFT:
            {
                 CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_LEFT");
                //QWSServer::sendKeyEvent(Qt::Key_Left,Qt::Key_Left,0,1,0);
                //QWSServer::sendKeyEvent(Qt::Key_Left,Qt::Key_Left,0,0,0);
#ifndef APPLICATION_TYPE_D2
                sendCharacter(Qt::Key_Left);
#endif
            }break;
            case KEYPAD_KEY7_RIGHT:
            {
                 CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_RIGHT");
                //QWSServer::sendKeyEvent(Qt::Key_Right,Qt::Key_Right,0,1,0);
                //QWSServer::sendKeyEvent(Qt::Key_Right,Qt::Key_Right,0,0,0);
#ifndef APPLICATION_TYPE_D2
                sendCharacter(Qt::Key_Right);
#endif
            }break;
            case KEYPAD_KEY7_DOWN:
            {
                 CCommonInterface::printf_debug("gui_system_key_up value  KEYPAD_KEY7_DOWN");
#ifndef APPLICATION_TYPE_D2
                 if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
                    slot_pushbutton_arrow_right();
                 else
                    sendCharacter(Qt::Key_Down);
#endif
            }break;
        }


    }while(0);
}

void CMainPage::gui_system_touch(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        int touch_x = 0;
        int touch_y = 0;
        int touch_up = 0;

        pCJSONFunCode = cJSON_GetObjectItem(root,"touch_x");
        if(pCJSONFunCode)
        {
            touch_x = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"touch_y");
        if(pCJSONFunCode)
        {
            touch_y = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"touch_up");
        if(pCJSONFunCode)
        {
            touch_up = pCJSONFunCode->valueint;
        }


        //touch_clicked_event(touch_x,touch_y);
        bool release = false;
        if(touch_up == 0)
            release = true;
        CMyMouseEvent::getInstance()->touch_click_event(touch_x,touch_y,release);
    }while(0);
}

void CMainPage::gui_starting_screen_flag(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
     do
     {
        pCJSONFunCode = cJSON_GetObjectItem(root,"screen_flag");
        if(pCJSONFunCode)
        {
            screen_flag = pCJSONFunCode->valueint;
            qDebug()<<"CMainPage:: gui_starting_screen_flag(cJSON *root)<<<<<<< screen_flag:"<< screen_flag;
         }

    }while(0);

}

void  CMainPage::gui_alg_image_update(cJSON *root)
{
    qDebug()<<"CMainPage:: gui_alg_image_update(cJSON *root)<<<<<<<";
    int type = -1;
    cJSON *pCJSONFunCode = NULL;
    do
     {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(pCJSONFunCode)
        {
            type = pCJSONFunCode->valueint;
            emit signal_alg_image_update(type);
        }
    }while(0);

}

void  CMainPage::gui_tmp_status(cJSON *root)
{
     qDebug()<<"CMainPage:: gui_tmp_login_status(cJSON *root)<<<<<<<";
     int state = -1;
     int type = -1;
     int errorType = -1;
     cJSON *pCJSONFunCode = NULL;
     do
     {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(pCJSONFunCode)
        {
            type = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {
            state = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"errorType");
        if(pCJSONFunCode)
        {
            errorType = pCJSONFunCode->valueint;
        }
	    if(kEnumTMPLogin == type)
	    {    
	         qDebug()<<"CMainPage:: gui_tmp_login_status(cJSON *root)<<<<<<< state:"<<state;
	         main_page_notify(kEnumMainPageTMPPlatformStatus,state,0);
	    	 CCommonData::getInstance()->m_tmp_status = state;

		}
        emit signal_tmp_status(type, state, errorType);
    }while(0);

}
void gui_tmp_connect_status(cJSON *root)
{
        qDebug()<<"CMainPage:: gui_tmp_connect_status(cJSON *root)<<<<<<<";
        cJSON *pCJSONFunCode = NULL;
}


void CMainPage:: gui_g3_network_status(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
   int state = -1;
   int type = -1;

     do
     {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(pCJSONFunCode)
        {
            //G3 network
            if(1 == pCJSONFunCode->valueint)
            {
                type = kEnumNetworkG3;
            }
            // TMP network
            else if (2 == pCJSONFunCode->valueint)
            {
                type =  kEnumNetworkTMP;
            }
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {
            state = pCJSONFunCode->valueint;
        }

       emit signal_g3_network_status(type, state);
    }while(0);
}

void CMainPage::gui_system_gps_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        int gps_status = 0;
         int gps_speed = 0;
         int gps_number = 0;
         pCJSONFunCode = cJSON_GetObjectItem(root,"gps_status");
        if(pCJSONFunCode)
        {
            gps_status = pCJSONFunCode->valueint;
        }

        //CCommonInterface::printf_debug("main_page_handle() gps_status:" + QString::number(gps_status));

        if(2 == gps_status)
        {
             pCJSONFunCode = cJSON_GetObjectItem(root,"gps_speed");
            if(pCJSONFunCode)
            {
                gps_speed = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"gps_number");
            if(pCJSONFunCode)
            {
                gps_number = pCJSONFunCode->valueint;
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_gps_number = gps_number;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"gps_altitude");
            if(pCJSONFunCode)
            {/*海拔高度，米*/
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"gps_angle");
            if(pCJSONFunCode)
            {/*方位角，0-359度*/
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_Azimuth = pCJSONFunCode->valueint;
                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_azimuth = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"gps_speed");
            if(pCJSONFunCode)
            {/*速度，0.1公里/小时*/
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_real_speed = pCJSONFunCode->valueint;
                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_speed = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"gps_longitude");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*经度*/
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_longitude = pCJSONFunCode->valuestring;
                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_longitude = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"gps_latitude");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*纬度*/
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_latitude = pCJSONFunCode->valuestring;
                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_latitude = pCJSONFunCode->valuestring;
            }
            main_page_notify(kEnumMainPageGpsNumber,gps_number,0);           //GPS个数
            main_page_notify(kEnumMainPageGpsStatus,kEnumGpsNormal,0);       //GPS状态
            main_page_notify(kEnumMainPageCurrentSpeed,gps_speed,0);

            CCommonData::getInstance()->m_runStatusInfo.m_type = 3;
            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

            CCommonData::getInstance()->m_CSystemCoordinateCollection.m_gps_success = 1;
            emit signal_coordinate_collection_widget_event(2,CCommonData::getInstance()->m_CSystemCoordinateCollection);
        }
        else
        {
            CCommonData::getInstance()->m_CSystemCoordinateCollection.m_gps_success = 0;
            emit signal_coordinate_collection_widget_event(2,CCommonData::getInstance()->m_CSystemCoordinateCollection);

            main_page_notify(kEnumMainPageGpsStatus,kEnumGpsAbnormal,0);       //GPS状态
            main_page_notify(kEnumMainPageGpsNumber,-1,0);                     //GPS个数

            main_page_notify(kEnumMainPageCurrentSpeed,0,0);
        }
    }while(0);
}

void CMainPage::gui_system_gps_db(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;

    //qDebug() << "CMainPage::gui_system_gps_db()";
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"firmware_version");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_firmware_version = pCJSONFunCode->valuestring;
        }

         cJSON *pJson_array=cJSON_GetObjectItem(root,"rows");/*取数组*/

        int array_size=cJSON_GetArraySize(pJson_array);/*数组大小*/
        cJSON *tasklist=pJson_array->child;/*子对象*/

        for(int index = 0;index < array_size;index++)
        {
            pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_db_value[index] = pCJSONFunCode->valueint;
            }
            tasklist = tasklist->next;
            if(NULL == tasklist)
            {
                break;
            }
            if(index >= 24)
            {
                break;
            }
        }

         CCommonData::getInstance()->m_runStatusInfo.m_type = 3;
         emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
    }while(0);

}



void CMainPage::gui_system_itssd_notify_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    CEventData eventData;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"line_name");
        if(pCJSONFunCode)
        {//线路名
            CCommonData::getInstance()->m_sd_info.m_line_name = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"driver_number");
        if(pCJSONFunCode)
        {/*驾驶员编号*/
            CCommonData::getInstance()->m_sd_info.m_driver_number = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"up_down_flag");
        if(pCJSONFunCode)
        {//1:上行 2:下行
            CCommonData::getInstance()->m_sd_info.m_up_down_flag = pCJSONFunCode->valueint;
        }
        if(1 == CCommonData::getInstance()->m_sd_info.m_up_down_flag)
        {
            eventData.m_str_data = tr("上行");
            main_page_notify(kEnumMainPageVehicleUpDown,0,&eventData);
        }
        else
        {
            eventData.m_str_data = tr("下行");
            main_page_notify(kEnumMainPageVehicleUpDown,0,&eventData);
        }

        eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_line_name;
        eventData.m_i_data = CCommonData::getInstance()->m_sd_info.m_up_down_flag;
        main_page_notify(kEnumMainPageLine,0,&eventData);   //线路

        pCJSONFunCode = cJSON_GetObjectItem(root,"bus_number");
        if(pCJSONFunCode)
        {/*车辆编号*/
            CCommonData::getInstance()->m_sd_info.m_bus_number = pCJSONFunCode->valuestring;
        }
        eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_bus_number;
        main_page_notify(kEnumMainPageVehicleModel,0,&eventData);              //车辆编号
        pCJSONFunCode = cJSON_GetObjectItem(root,"bus_license");
        if(pCJSONFunCode)
        {/*车牌号*/
            CCommonData::getInstance()->m_sd_info.m_bus_license = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"auto_flag");
        if(pCJSONFunCode)
        {//1:手动  2:自动
            CCommonData::getInstance()->m_sd_info.m_auto_flag = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"operation_flag");
        if(pCJSONFunCode)
        {// 1:非营运 2:营运
            CCommonData::getInstance()->m_sd_info.m_operation_flag = pCJSONFunCode->valueint;
        }
        if(2 == CCommonData::getInstance()->m_sd_info.m_operation_flag)
        {
            eventData.m_str_data = tr("运营中");
            main_page_notify(kEnumMainPageVehicleStatus,0,&eventData);
        }
        else
        {
            eventData.m_str_data = tr("非运营");
            main_page_notify(kEnumMainPageVehicleStatus,0,&eventData);
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"current_limspeed");
        if(pCJSONFunCode)
        {//限速
            CCommonData::getInstance()->m_sd_info.m_current_limspeed = pCJSONFunCode->valueint/10;
        }
        main_page_notify(kEnumMainPageLimitSpeed,CCommonData::getInstance()->m_sd_info.m_current_limspeed,0);                //限速
        pCJSONFunCode = cJSON_GetObjectItem(root,"sd_status");
        if(pCJSONFunCode)
        {/*SD卡状态 1:正常 0:异常*/
            CCommonData::getInstance()->m_sd_info.m_sd_status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"first_station");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_sd_info.m_first_station = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"last_station");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_sd_info.m_last_station = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }
        eventData.m_str_data = tr("");
        main_page_notify(kEnumMainPageFrontStation1,0,&eventData);

        eventData.m_str_data = tr("");
        main_page_notify(kEnumMainPageFrontStation2,0,&eventData);

        eventData.m_str_data = tr("");
        main_page_notify(kEnumMainPageBackStation1,0,&eventData);

        eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_last_station;
        main_page_notify(kEnumMainPageBackStation2,0,&eventData);

#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
        CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumGet,1);
#endif

    }while(0);
}

void CMainPage::gui_system_notify_station(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    CEventData eventData;
    //static bool first = true;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {// 0:进站 1:出站
            CCommonData::getInstance()->m_station_info.m_status = pCJSONFunCode->valueint;
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"flag");
        if(pCJSONFunCode)
        {//0:弹出报站
            CCommonData::getInstance()->m_station_info.m_flag = pCJSONFunCode->valueint;
        }

#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
        if(0 == CCommonData::getInstance()->m_station_info.m_status)
        {//0:进站
            if(0 == CCommonData::getInstance()->m_station_info.m_flag)
            {
                if(1 == CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway_enable)
                {
                    if(m_timer_station->isActive())
                        m_timer_station->stop();

                    emit signal_driver_enter_station(true);
                }
            }
        }
        else
        {
            if(1 == CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway_enable)
            {
                 char time_seconds = 0;

                 if(m_timer_station->isActive())
                     m_timer_station->stop();

                 if(1 == CCommonData::getInstance()->m_CDriverMenuInfo.out_door_delay_quit)
                    time_seconds = 10;
                 else if(2 == CCommonData::getInstance()->m_CDriverMenuInfo.out_door_delay_quit)
                    time_seconds = 15;
                 else if(3 == CCommonData::getInstance()->m_CDriverMenuInfo.out_door_delay_quit)
                    time_seconds = 20;
                 else
                    time_seconds = 5;
                 m_timer_station->setInterval(time_seconds * 1000);
                 m_timer_station->start();
            }
            //emit signal_driver_enter_station(false);
        }
#endif

        if(1 == CCommonData::getInstance()->m_station_info.m_status)
        {
            if(CCommonData::getInstance()->m_station_info.m_current_station == CCommonData::getInstance()->m_sd_info.m_first_station)
            {
                qDebug() << "kEnumMainPageStationStatus  kEnumStationStart  1.0";
                main_page_notify(kEnumMainPageStationStatus,kEnumStationStart,0);
            }
            else if(CCommonData::getInstance()->m_station_info.m_current_station == CCommonData::getInstance()->m_sd_info.m_last_station)
            {
                qDebug() << "kEnumMainPageStationStatus  kEnumStationEnd  1.0";
                main_page_notify(kEnumMainPageStationStatus,kEnumStationEnd,0);
            }
            else
            {
                qDebug() << "kEnumMainPageStationStatus  kEnumStationOut  1.0";
               main_page_notify(kEnumMainPageStationStatus,kEnumStationOut,0);
            }
            break;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"current_station");
        if(pCJSONFunCode)
        {/*当前站名*/
            CCommonData::getInstance()->m_station_info.m_current_station = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }

        if(CCommonData::getInstance()->m_station_info.m_current_station == CCommonData::getInstance()->m_sd_info.m_first_station)
        {
            qDebug() << "kEnumMainPageStationStatus  kEnumStationStart  1";
            main_page_notify(kEnumMainPageStationStatus,kEnumStationStart,0);
        }
        else if(CCommonData::getInstance()->m_station_info.m_current_station == CCommonData::getInstance()->m_sd_info.m_last_station)
        {
            qDebug() << "kEnumMainPageStationStatus  kEnumStationEnd  1";
            main_page_notify(kEnumMainPageStationStatus,kEnumStationEnd,0);
        }
        else
        {
            qDebug() << "kEnumMainPageStationStatus  kEnumStationIn  1";
            main_page_notify(kEnumMainPageStationStatus,kEnumStationIn,0);
        }



        pCJSONFunCode = cJSON_GetObjectItem(root,"current_id");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_station_info.m_current_id = pCJSONFunCode->valueint;
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"current_speed");
        if(pCJSONFunCode)
        {/*当前速度*/
            CCommonData::getInstance()->m_station_info.m_current_speed = pCJSONFunCode->valueint/10;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"current_limspeed");
        if(pCJSONFunCode)
        {/*限速*/
            CCommonData::getInstance()->m_station_info.m_current_limspeed = pCJSONFunCode->valueint/10;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"station_type");
        if(pCJSONFunCode)
        {//1:最后一站　2:中间站 3:第一站
            CCommonData::getInstance()->m_station_info.m_station_type = pCJSONFunCode->valueint;
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"prev_station");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_station_info.m_prev_station = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"next_station");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_station_info.m_next_station = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }
        CCommonInterface::printf_debug("main_page_handle() system_notify_station " + QString::number(CCommonData::getInstance()->m_station_info.m_status));
        if(0 == CCommonData::getInstance()->m_station_info.m_status)
        {// 进站

            eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_current_station;
            eventData.m_i_data = CCommonData::getInstance()->m_station_info.m_current_id;
            main_page_notify(kEnumMainPageUpdateStationName,0,&eventData);

            CCommonInterface::printf_debug("main_page_handle() system_notify_station m_station_type:" + QString::number(CCommonData::getInstance()->m_station_info.m_station_type));
            if(1 == CCommonData::getInstance()->m_station_info.m_station_type)
            {//最后一站
                eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_first_station;
                main_page_notify(kEnumMainPageFrontStation1,0,&eventData);

                eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_prev_station;
                main_page_notify(kEnumMainPageFrontStation2,0,&eventData);

                eventData.m_str_data = tr("");
                main_page_notify(kEnumMainPageBackStation1,0,&eventData);
                eventData.m_str_data = tr("");
                main_page_notify(kEnumMainPageBackStation2,0,&eventData);
            }
            else if(2 == CCommonData::getInstance()->m_station_info.m_station_type)
            {//中间站

                if(CCommonData::getInstance()->m_station_info.m_prev_station !=
                   CCommonData::getInstance()->m_sd_info.m_first_station)
                {
                     eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_first_station;
                     main_page_notify(kEnumMainPageFrontStation1,0,&eventData);
                }
                else
                {
                    eventData.m_str_data = tr("");
                    main_page_notify(kEnumMainPageFrontStation1,0,&eventData);
                }

                eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_prev_station;
                main_page_notify(kEnumMainPageFrontStation2,0,&eventData);

                eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_next_station;
                main_page_notify(kEnumMainPageBackStation1,0,&eventData);

                if(CCommonData::getInstance()->m_station_info.m_next_station ==
                   CCommonData::getInstance()->m_sd_info.m_last_station)
                {
                    eventData.m_str_data = tr("");
                    main_page_notify(kEnumMainPageBackStation2,0,&eventData);
                }
                else
                {
                    eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_last_station;
                    main_page_notify(kEnumMainPageBackStation2,0,&eventData);
                }
            }
            else if(3 == CCommonData::getInstance()->m_station_info.m_station_type)
            {//第一站

                eventData.m_str_data = tr("");
                main_page_notify(kEnumMainPageFrontStation1,0,&eventData);

                eventData.m_str_data = tr("");
                main_page_notify(kEnumMainPageFrontStation2,0,&eventData);

                eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_next_station;
                main_page_notify(kEnumMainPageBackStation1,0,&eventData);

                eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_last_station;
                main_page_notify(kEnumMainPageBackStation2,0,&eventData);
            }

            if(0 == CCommonData::getInstance()->m_station_info.m_flag)
            {
                //slot_pushbutton_play();
                //还需要通知到站等信息
            }
        }
    }while(0);
}

void CMainPage::gui_system_direction_change(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    CEventData eventData;
    do
    {
        qDebug() << "gui_system_direction_change";
        pCJSONFunCode = cJSON_GetObjectItem(root,"up_down_flag");
        if(pCJSONFunCode)
        {//1:上行 2:下行
            CCommonData::getInstance()->m_sd_info.m_up_down_flag = pCJSONFunCode->valueint;

            if(1 == CCommonData::getInstance()->m_sd_info.m_up_down_flag)
            {
                eventData.m_str_data = tr("上行");
                main_page_notify(kEnumMainPageVehicleUpDown,0,&eventData);
            }
            else
            {
                eventData.m_str_data = tr("下行");
                main_page_notify(kEnumMainPageVehicleUpDown,0,&eventData);
            }
        }
#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
        QTimer::singleShot(1000,this,SLOT(slot_timer_collection_data()));
#endif
        eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_line_name;
        eventData.m_i_data = CCommonData::getInstance()->m_sd_info.m_up_down_flag;
        main_page_notify(kEnumMainPageLine,0,&eventData);   //线路

        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        char status = 0;
        if(pCJSONFunCode)
        {
            status = CCommonData::getInstance()->m_sd_info.m_up_down_flag = pCJSONFunCode->valueint;
        }

        if(2 == status)
        {
            //qDebug() << "kEnumMainPageStationStatus  kEnumStationOut  2";
            //main_page_notify(kEnumMainPageStationStatus,kEnumStationOut,0);
            break;
        }
        //qDebug() << "kEnumMainPageStationStatus  kEnumStationIn  2";
        //main_page_notify(kEnumMainPageStationStatus,kEnumStationIn,0);

        pCJSONFunCode = cJSON_GetObjectItem(root,"current_station");
        if(pCJSONFunCode)
        {/*当前站名*/
            CCommonData::getInstance()->m_station_info.m_current_station = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }
        else
        {

        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"current_id");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_station_info.m_current_id = pCJSONFunCode->valueint;
        }
        //CCommonData::getInstance()->m_station_info.m_current_station = CCommonData::getInstance()->m_sd_info.m_last_station;
#if 0
        CDynTextParam param_station_name;
        param_station_name.m_control_this = m_pushbutton_station_name;
        param_station_name.m_control_type = CDynamicText::kEnumPushbutton;
        param_station_name.m_control_id   = kEnumPushbuttonStationName;
        param_station_name.m_control_source_text = CCommonData::getInstance()->m_station_info.m_current_station;
        param_station_name.m_control_font_size = STATION_NAME_SHOW_FONT_SIZE;
        param_station_name.m_control_show_length = STATION_NAME_SHOW_LENGTH;
#endif


#if 0
        pCJSONFunCode = cJSON_GetObjectItem(root,"prev_station");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_station_info.m_prev_station = gb2312ToUtf8(pCJSONFunCode->valuestring);
            eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_prev_station;
            main_page_notify(kEnumMainPageFrontStation2,0,&eventData);
        }
#endif
        pCJSONFunCode = cJSON_GetObjectItem(root,"next_station");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_station_info.m_next_station = gb2312ToUtf8(pCJSONFunCode->valuestring);
            eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_next_station;
            main_page_notify(kEnumMainPageBackStation1,0,&eventData);
        }
        //交换起始站和终点站
        QString str_first = CCommonData::getInstance()->m_sd_info.m_first_station;
        QString str_last = CCommonData::getInstance()->m_sd_info.m_last_station;
        CEventData eventData;

        CCommonData::getInstance()->m_sd_info.m_first_station = str_last;
        CCommonData::getInstance()->m_sd_info.m_last_station = str_first;

        eventData.m_str_data = tr("");
        main_page_notify(kEnumMainPageFrontStation1,0,&eventData);

        eventData.m_str_data = tr("");
        main_page_notify(kEnumMainPageFrontStation2,0,&eventData);

        eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_last_station;
        main_page_notify(kEnumMainPageBackStation2,0,&eventData);


        eventData.m_i_data = CCommonData::getInstance()->m_station_info.m_current_id;
        eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_current_station;
        main_page_notify(kEnumMainPageUpdateStationName,0,&eventData);

    }while(0);
}

void CMainPage::slot_timer_collection_data()
{
    CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumGet,1);
}

void CMainPage::gui_system_g3_modem_status(cJSON *root)
{
   cJSON *pCJSONFunCode = NULL;
  #if 1


    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"g3_signal");
        if(NULL == pCJSONFunCode)
        {
            break;
        }

        CCommonData::getInstance()->m_g3_signal = pCJSONFunCode->valueint;
        CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_signal_status = pCJSONFunCode->valueint;

        if( CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_sim_status == 1)
        {
         int temp = CCommonData::getInstance()->m_g3_signal;
        if(1 == CCommonData::getInstance()->m_g3_signal)

            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignal1,0);
        else if(2 == CCommonData::getInstance()->m_g3_signal)
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignal2,0);
        else if(3 == CCommonData::getInstance()->m_g3_signal)
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignal3,0);
        else if(4 == CCommonData::getInstance()->m_g3_signal)
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignal4,0);
        else if(5 == CCommonData::getInstance()->m_g3_signal)
        {
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignalError,0);
            main_page_notify(kEnumMainPageSignal1,kEnumNetworkNone,0);
        }
        else
        {
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignalNone,0);
            main_page_notify(kEnumMainPageSignal1,kEnumNetworkNone,0);
        }

        }
        else
        {
           main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignalError,0);

        }
#endif
 #if  0
    do
    {

        pCJSONFunCode = cJSON_GetObjectItem(root,"g3_signal");
        if(NULL == pCJSONFunCode)
        {
            break;
        }

        CCommonData::getInstance()->m_g3_signal = pCJSONFunCode->valueint;
        CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_signal_status = pCJSONFunCode->valueint;
        {
        if(1 == CCommonData::getInstance()->m_g3_signal)
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignal1,0);
        else if(2 == CCommonData::getInstance()->m_g3_signal)
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignal2,0);
        else if(3 == CCommonData::getInstance()->m_g3_signal)
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignal3,0);
        else if(4 == CCommonData::getInstance()->m_g3_signal)
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignal4,0);
        else if(5 == CCommonData::getInstance()->m_g3_signal)
        {
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignalError,0);
            main_page_notify(kEnumMainPageSignal1,kEnumNetworkNone,0);
        }
        else
        {
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignalNone,0);
            main_page_notify(kEnumMainPageSignal1,kEnumNetworkNone,0);
        }
        }
#endif

        pCJSONFunCode = cJSON_GetObjectItem(root,"model_status");
        if(pCJSONFunCode)
        {/*模块状态*/
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_model_status = pCJSONFunCode->valueint;
            pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"sim_status");
        if(pCJSONFunCode)
        {/*SIM卡*/
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_sim_status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"network_type");
        if(pCJSONFunCode)
        {/*网络类型*/
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_network_type = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"reset_times");
        if(pCJSONFunCode)
        {/*复位次数*/
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_reset_times = pCJSONFunCode->valueint;
        }
        CCommonData::getInstance()->m_runStatusInfo.m_type = 1;
        emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
        emit signal_network_status(NETWORK_TYPE_G3,  CCommonData::getInstance()->m_runStatusInfo);

    }while(0);
}
void CMainPage::gui_system_itsnw_connect_status(cJSON *root)
{
    qDebug()<<"CMainPage:: gui_system_itsnw_connect_status(cJSON *root)<<<<";
    cJSON *pCJSONFunCode = NULL;

    do
    {
        //CCommonInterface::printf_debug("main_page_handle() system_itsnw_connect_status start");
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(NULL == pCJSONFunCode)
        {
            break;
        }

        CCommonData::getInstance()->m_itsnw_status = pCJSONFunCode->valueint;
        //CCommonInterface::printf_debug("main_page_handle() system_itsnw_connect_status:" + QString::number(CCommonData::getInstance()->m_itsnw_status));
        if(1 == CCommonData::getInstance()->m_itsnw_status)/*未连接状态*/
        {
#ifndef APPLICATION_TYPE_P2
            main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformAbnormal1,0);
 #endif
            main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignalError,0);
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_status = 2;
        }
        else if(2 == CCommonData::getInstance()->m_itsnw_status)/*正常状态*/
        {
#ifndef APPLICATION_TYPE_P2
            main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformNormal,0);

#endif
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_status = 3;
        }
        else if(3 == CCommonData::getInstance()->m_itsnw_status)/*正在连接状态*/
        {
#ifndef APPLICATION_TYPE_P2
            main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformConnect,0);
#endif
             main_page_notify(kEnumMainPageSignal2,kEnumNetworkSignalError,0);
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_status = 2;
        }

        CCommonData::getInstance()->m_runStatusInfo.m_type = 1;
        emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
        // emit signal_network_status(NETWORK_TYPE_TMP,  CCommonData::getInstance()->m_runStatusInfo);



    }while(0);
}

void CMainPage::gui_system_tbp_connect_status(cJSON *root)
{

    cJSON *pCJSONFunCode = NULL;
    do
    {
        //CCommonInterface::printf_debug("main_page_handle() gui_system_tbp_connect_status start");
        int type = 0;
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(pCJSONFunCode)
        {
            type = pCJSONFunCode->valueint;
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        if(1 == type)
        {
            if(1 == pCJSONFunCode->valueint)/*未连接状态*/
            {
                if(e_Dev_Mode_Slave_Mac != CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
                    main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformConnect,0);
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status = 2;
            }
            else if(2 == pCJSONFunCode->valueint)/*正常状态*/
            {
                if(e_Dev_Mode_Slave_Mac != CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
                    main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformNormal,0);
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status = 3;
            }
            else if(3 == pCJSONFunCode->valueint)/*正在连接状态*/
            {
                if(e_Dev_Mode_Slave_Mac != CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
                    main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformConnect,0);
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status = 2;
            }

            CCommonData::getInstance()->m_runStatusInfo.m_type = 1;
            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
        }
        else if(2 == type)
        {
            if(e_Dev_Mode_Slave_Mac == CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
            {
                if(2 == pCJSONFunCode->valueint)
                    main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformNormal,0);
                else
                    main_page_notify(kEnumMainPagePlatformStatus,kEnumPlatformConnect,0);
            }


            if(2 == pCJSONFunCode->valueint)/*正常状态*/
            {
                if(e_Dev_Mode_Slave_Mac == CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
                {
                    qDebug() << "from status success............................";
                    #if 1
                    //emit signal_mainwindow_show(true);
                    main_page_notify(kEnumMainPageFromStatus,1,0);
                    #endif
                }
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_host_from_status = 1;
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->local_network_status = 1;
            }
            else
            {
                if(e_Dev_Mode_Slave_Mac == CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
                {
                    qDebug() << "from status faild............................";
                    #if 1
                    emit signal_mainwindow_show(true);
                    main_page_notify(kEnumMainPageFromStatus,0,0);
                    #endif
                }
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_host_from_status = 0;
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->local_network_status = 2;
            }
            CCommonData::getInstance()->m_runStatusInfo.m_type = 4;
            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
        }

        //CCommonInterface::printf_debug("main_page_handle() gui_system_tbp_connect_status end");

    }while(0);
}

void CMainPage::gui_system_stmdev_connect_status(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        CCommonData::getInstance()->m_stmdev_status = pCJSONFunCode->valueint;
        CCommonInterface::printf_debug("main_page_handle() system_stmdev_connect_status:" + QString::number(CCommonData::getInstance()->m_stmdev_status));
        if(1 == CCommonData::getInstance()->m_stmdev_status)/*未连接状态*/
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_status = 2;
            main_page_notify(kEnumMainPagePlatformStatus1,kEnumPlatformAbnormal1,0);
        }
        else if(2 == CCommonData::getInstance()->m_stmdev_status)/*正常状态*/
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_status = 3;
            main_page_notify(kEnumMainPagePlatformStatus1,kEnumPlatformNormal,0);
        }
        else if(3 == CCommonData::getInstance()->m_stmdev_status)/*正在连接状态*/
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_status = 2;
            main_page_notify(kEnumMainPagePlatformStatus1,kEnumPlatformConnect,0);
        }


        CCommonData::getInstance()->m_runStatusInfo.m_type = 1;
        emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

    }while(0);
}
void CMainPage::gui_system_exception_overspeed(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
         pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        CCommonData::getInstance()->m_over_speed_status = pCJSONFunCode->valueint;

        if(1 == CCommonData::getInstance()->m_over_speed_status)
        {/*超速*/
        }
        else
        {/*不超速*/
        }

    }while(0);
}
void CMainPage::gui_system_exception_rate_limit(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"current_limspeed");
        if(pCJSONFunCode)
        {//限速
            CCommonData::getInstance()->m_sd_info.m_current_limspeed = pCJSONFunCode->valueint/10;
        }
        main_page_notify(kEnumMainPageLimitSpeed,CCommonData::getInstance()->m_sd_info.m_current_limspeed,0);
    }while(0);
}
void CMainPage::gui_system_itsio_io_status(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_fdoor");
        if(pCJSONFunCode)
        {//2:前门开 1:前门关
            CCommonData::getInstance()->m_io_info.m_front_door = pCJSONFunCode->valueint;
            if(2 == CCommonData::getInstance()->m_io_info.m_front_door)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->front_door_status = 1;
                main_page_notify(kEnumMainPageFrontDoor,kEnumDoorOpen,0);
            }
            else
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->front_door_status = 2;
                main_page_notify(kEnumMainPageFrontDoor,kEnumDoorClose,0);
            }

        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_mdoor");
        if(pCJSONFunCode)
        {//2:中门开 1:中门关
            CCommonData::getInstance()->m_io_info.m_middle_door = pCJSONFunCode->valueint;

            if(2 == CCommonData::getInstance()->m_io_info.m_middle_door)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->middle_door_status = 1;
            }
            else
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->middle_door_status = 2;
            }
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_bdoor");
        if(pCJSONFunCode)
        {//2:后门开 1:后门关
            CCommonData::getInstance()->m_io_info.m_back_door = pCJSONFunCode->valueint;

            if(2 == CCommonData::getInstance()->m_io_info.m_back_door)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->back_door_status = 1;
                main_page_notify(kEnumMainPageBackDoor,kEnumDoorOpen,0);
            }
            else
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->back_door_status = 2;
                main_page_notify(kEnumMainPageBackDoor,kEnumDoorClose,0);
            }
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"io_back");//倒车
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
            {
                if(1 == CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway_enable)
                {
                    if(!MainWindow::getInstance()->is_back_car_show_mode())
                    {
                        emit signal_back_car_notify(true);
                        emit signal_driver_enter_station(true);
                    }
                }

                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->back_car_status = 1;
            }
            else
            {
                if(MainWindow::kEnumPlay == MainWindow::getInstance()->get_current_widget_id() && MainWindow::getInstance()->is_back_car_show_mode())
                    emit signal_back_car_notify(false);
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->back_car_status = 2;
            }
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_ddoor");//设备门
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_device_door = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_device_door = 2;
        }





        CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_io1_status = 2;
        CCommonData::getInstance()->m_runStatusInfo.m_type = 2;
        emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

        CCommonData::getInstance()->m_runStatusInfo.m_type = 5;
        emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
#if 0
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_back_vehicle");
        if(pCJSONFunCode)
        {//2:倒车　1:没有倒车
            CCommonData::getInstance()->m_io_info.m_back_door = pCJSONFunCode->valueint;
        }
#endif
    }while(0);
}
void CMainPage::gui_system_hdd_status(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
         pCJSONFunCode = cJSON_GetObjectItem(root,"hdd_status");
        if(NULL == pCJSONFunCode)
        {
            break;
        }

        CCommonData::getInstance()->m_hdd_status = pCJSONFunCode->valueint;


#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        if(1 == CCommonData::getInstance()->m_hdd_status)
            main_page_notify(kEnumMainPageSdStatus,kEnumSdNormalNormal,0);
        else if(3 == CCommonData::getInstance()->m_hdd_status)
            main_page_notify(kEnumMainPageSdStatus,kEnumSdAbnormal1,0);
        else
            main_page_notify(kEnumMainPageSdStatus,kEnumError,0);
#else
        if(1 == CCommonData::getInstance()->m_hdd_status)
            main_page_notify(kEnumMainPageHardDiskStatus,kEnumHardDiskNormal,0);
        else if(3 == CCommonData::getInstance()->m_hdd_status)
            main_page_notify(kEnumMainPageHardDiskStatus,kEnumHardDiskAbnormal2,0);
        else
            main_page_notify(kEnumMainPageHardDiskStatus,kEnumHardDiskAbnormal1,0);
#endif

    }while(0);
}
void CMainPage::gui_system_userin_card(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"card_number");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->card_number = pCJSONFunCode->valueint;
        }
    }while(0);
}
void CMainPage::gui_system_its_scheluing_plan(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"cmd_time_status");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_scheluing_plan.status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"msg_content");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_scheluing_plan.m_msg_content = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }
        QString show_content = "";
        if(1 == CCommonData::getInstance()->m_scheluing_plan.status)
        {
            show_content = tr("发车时间") + CCommonData::getInstance()->m_scheluing_plan.m_msg_content;
        }
        else if(2 == CCommonData::getInstance()->m_scheluing_plan.status)
        {
            show_content = CCommonData::getInstance()->m_scheluing_plan.m_msg_content;
        }
        else if(3 == CCommonData::getInstance()->m_scheluing_plan.status)
        {
            show_content = tr("计划取消") + CCommonData::getInstance()->m_scheluing_plan.m_msg_content;
        }
    }while(0);
}
void CMainPage::gui_system_its_task_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"msg_content");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_scheluing_plan.m_task_content = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }

        CMainMessageDialog::getInstance()->move((1024 - 723)/2,(768 - 551)/2);;

        CMainMessageDialog::getInstance()->set_show_content(CMainMessageDialog::kEnumMessageDialog,
                CCommonData::getInstance()->m_scheluing_plan.m_task_content,
                "");

        CMainMessageDialog::getInstance()->exec();


    }while(0);
}
void CMainPage::gui_system_station_type(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_station_type = pCJSONFunCode->valueint;
        }
    }while(0);
}
void CMainPage::gui_system_shutdown(cJSON *root)
{
    //cJSON *pCJSONFunCode = NULL;
    do
    {

    }while(0);
}
void CMainPage::gui_system_exception_analog(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"acc_status");
        if(pCJSONFunCode)
        {// 1:acc开 2:acc关
            CCommonData::getInstance()->m_io_info.m_acc_status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"door_status");
        if(pCJSONFunCode)
        {//1:设备门开 2:1:设备门关
            CCommonData::getInstance()->m_io_info.m_door_status =pCJSONFunCode->valueint;
        }
    }while(0);
}
void CMainPage::gui_system_working_status(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    CEventData eventData;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_work_status = pCJSONFunCode->valueint;
        }
        if(1 == CCommonData::getInstance()->m_work_status)
        {
            eventData.m_str_data = tr("运营中");
            main_page_notify(kEnumMainPageVehicleStatus,0,&eventData);
        }
        else
        {
            eventData.m_str_data = tr("非运营");
            main_page_notify(kEnumMainPageVehicleStatus,0,&eventData);
        }
    }while(0);
}
void CMainPage::gui_system_itssd_para(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    CEventData eventData;
    do
    {
         char status = 0;
         pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {
            status = pCJSONFunCode->valueint;
        }
        if(1 == status)
        {
            char value = 0;
            pCJSONFunCode = cJSON_GetObjectItem(root,"value");
            if(pCJSONFunCode)
            {
                value = pCJSONFunCode->valueint;
            }
            if(1 == value)
                eventData.m_str_data = tr("上行");
            else
                eventData.m_str_data = tr("下行");

            main_page_notify(kEnumMainPageVehicleUpDown,0,&eventData);
        }
                else if(2 == status)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"u32GPRSLineNo");
            if(pCJSONFunCode)
            {//GPRS线路号(线路编号)
                CCommonData::getInstance()->m_bus_plate.m_gprs_line_no = pCJSONFunCode->valueint;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"charBusNoBuf");
            if(pCJSONFunCode)
            {//车辆编号
                CCommonData::getInstance()->m_bus_plate.m_bus_number = pCJSONFunCode->valuestring;
            }
            eventData.m_str_data = CCommonData::getInstance()->m_bus_plate.m_bus_number;
            main_page_notify(kEnumMainPageVehicleModel,0,&eventData);
            pCJSONFunCode = cJSON_GetObjectItem(root,"charBusLicenseBuf");
            if(pCJSONFunCode)
            {//车辆牌号
                CCommonData::getInstance()->m_bus_plate.m_bus_license = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8SvrType");
            if(pCJSONFunCode)
            {//0:非运营，1:运营
                CCommonData::getInstance()->m_bus_plate.svr_type = pCJSONFunCode->valueint;

                if(1 == CCommonData::getInstance()->m_bus_plate.svr_type)
                {
                    eventData.m_str_data = tr("运营中");
                    main_page_notify(kEnumMainPageVehicleStatus,0,&eventData);
                }
                else
                {
                    eventData.m_str_data = tr("非运营");
                    main_page_notify(kEnumMainPageVehicleStatus,0,&eventData);
                }

            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u16HowTimeIntoSleep");
            if(pCJSONFunCode)
            {//休眠判据1：车辆停留多长时间后，进入休眠状态（秒）,0(此参数不使用)
                CCommonData::getInstance()->m_bus_plate.HowTimeIntoSleep = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8IntoSleepBeginTime");
            if(pCJSONFunCode)
            {//休眠判据2：车辆进入休眠状态的起始时间（时，0~23）
                CCommonData::getInstance()->m_bus_plate.IntoSleepBeginTime = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8IntoSleepEndTime");
            if(pCJSONFunCode)
            {//休眠判据2：车辆退出休眠状态的终止时间（时，0~24）
                CCommonData::getInstance()->m_bus_plate.IntoSleepEndTime = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8CheMenTiShiCount");
            if(pCJSONFunCode)
            {//车门未关提示次数，即异常开门报警次数：0（不报警），255（连续报警），其它（报警次数）
                CCommonData::getInstance()->m_bus_plate.CheMenTiShiCount = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8AbortMenSpeedLimit");
            if(pCJSONFunCode)
            {//异常开门报警的限速：允许的最小速度限值，单位km/h
                CCommonData::getInstance()->m_bus_plate.AbortMenSpeedLimit = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8ShuDuDisplayType");
            if(pCJSONFunCode)
            {//速度显示方式
                CCommonData::getInstance()->m_bus_plate.ShuDuDisplayType = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"u8LianXuBoFlag");
            if(pCJSONFunCode)
            {//连续报站使能：0(关闭)、1(允许中断)，使能或禁止中断还没完成的语音
                CCommonData::getInstance()->m_bus_plate.LianXuBoFlag = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"u8ZhanMingChongFuFlag");
            if(pCJSONFunCode)
            {//站名重复使能：0(关闭)、1(开启)，使能或禁止站名音重复2次
                CCommonData::getInstance()->m_bus_plate.ZhanMingChongFuFlag = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8GPSEnFlag");
            if(pCJSONFunCode)
            {//自动报站使能：0(关闭)、1(开启)
                CCommonData::getInstance()->m_bus_plate.GPSEnFlag = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8OpMode");
            if(pCJSONFunCode)
            {//报站方式：0(两键式预报与报站)、1(一键式只预报)、2(一键式只报站)、3(一键式预报和报站)
                CCommonData::getInstance()->m_bus_plate.OpMode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8LanguageFlags");
            if(pCJSONFunCode)
            {//报站语言：。0(不报站)、1(普通话)、2(英语)、3(普通话＋英语)、4(方言)、5(普通话＋方言)、6(英语＋方言)、7(普通话＋英语＋方言)
                CCommonData::getInstance()->m_bus_plate.LanguageFlags = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8WaiYinEnableFlag");
            if(pCJSONFunCode)
            {//外音开关：0（禁止外音播报），1（开启外音播报）
                CCommonData::getInstance()->m_bus_plate.WaiYinEnableFlag = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8WaiYinOffBeginTime");
            if(pCJSONFunCode)
            {//外音关闭起始时间：外音关闭起始时间（时,0~23）
                CCommonData::getInstance()->m_bus_plate.WaiYinOffBeginTime = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8WaiYinOffEndTime");
            if(pCJSONFunCode)
            {//外音关闭终止时间外音关闭终止时间（时,0~24）
                CCommonData::getInstance()->m_bus_plate.WaiYinOffEndTime = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8BaoShiYinEnable");
            if(pCJSONFunCode)
            {//报时音使能：0（禁止报时音播报），1（开启报时音播报）
                CCommonData::getInstance()->m_bus_plate.BaoShiYinEnable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8BaoShiYinOffBeginTime");
            if(pCJSONFunCode)
            {//报时音关闭起始时间：外音关闭起始时间（时,0~23）
                CCommonData::getInstance()->m_bus_plate.BaoShiYinOffBeginTime = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8BaoShiYinOffEndTime");
            if(pCJSONFunCode)
            {//报时音关闭终止时间：外音关闭终止时间（时,0~24）
                CCommonData::getInstance()->m_bus_plate.BaoShiYinOffEndTime = pCJSONFunCode->valueint;
            }
        }
        else if(3 == status)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"charDriverNoBuf");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_driver_info.m_driver_number = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"u8QianTuiFlag");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_driver_info.m_qian_tui = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"u32OnDutyTime");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_driver_info.m_on_duty_time = pCJSONFunCode->valueint;
            }
              pCJSONFunCode = cJSON_GetObjectItem(root,"u32OffDutyTime");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_driver_info.m_off_duty_time = pCJSONFunCode->valueint;
            }
        }
    }while(0);
}
void CMainPage::gui_system_av_videoloss(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"chan");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_video_loss.m_chan = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"camstatus");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_video_loss.m_camstatus = pCJSONFunCode->valueint;
        }
    }while(0);
}
void CMainPage::gui_voice(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        int volume_within = 0;
        int volume_out = 0;
        int volume_TTS = 0;
        pCJSONFunCode = cJSON_GetObjectItem(root,"volume_within");
        if(pCJSONFunCode)
        {
            volume_within = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"volume_out");
        if(pCJSONFunCode)
        {
            volume_out = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"volume_TTS");
        if(pCJSONFunCode)
        {
            volume_TTS = pCJSONFunCode->valueint;
        }
        emit signal_voice_dialog_event(volume_within,volume_out,volume_TTS);
        CCommonData::getInstance()->m_CGuiVoice.m_volume_within = volume_within;
        CCommonData::getInstance()->m_CGuiVoice.m_volume_out = volume_out;
        CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS = volume_TTS;
    }while(0);
}
void CMainPage::gui_light(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        int light = 0;
        pCJSONFunCode = cJSON_GetObjectItem(root,"value");
        if(pCJSONFunCode)
        {
            light = pCJSONFunCode->valueint;
        }
        CCommonData::getInstance()->m_CGuiVoice.m_light = light;
        emit signal_driver_menu_light_event(light);

    }while(0);
}
void CMainPage::gui_videotape_video(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        int type = 0;
        int number = 0;
        int show = 0;
        bool output_show = false;
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(pCJSONFunCode)
        {
            type = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"number");
        if(pCJSONFunCode)
        {
            number = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"show");
        if(pCJSONFunCode)
        {
            show = pCJSONFunCode->valueint;
        }
        if(1 == show)
            output_show = true;

        if(MainWindow::kEnum16Video == MainWindow::getInstance()->get_current_widget_id())
            emit signal_vidoe_widget_event(type,number,output_show);
        else if(MainWindow::kEnum16Video == MainWindow::getInstance()->get_current_widget_id())
            emit signal_arrive_widget_notify_event(type,number,output_show);

    }while(0);
}
void CMainPage::gui_about_machine(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
                CAboutMachineInfo about_machien_info;

        pCJSONFunCode = cJSON_GetObjectItem(root,"application_program_version");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//应用程序版本
            about_machien_info.m_application_program_version = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"file_system_version");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//根文件系统版本
           about_machien_info.m_file_system_version = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"mcu_program_version");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//MCU程序版本
           about_machien_info.m_mcu_program_version = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"system_resouce");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//系统资源
            about_machien_info.m_system_resouce = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"system_plug_in");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//系统插件
            about_machien_info.m_system_plug_in = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"kernel_version");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//内核版本
            about_machien_info.m_kernel_version = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"guide_version");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//引导版本
           about_machien_info.m_guide_version =  pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"keyboard_program_version");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//键盘程序版本
            about_machien_info.m_keyboard_program_version = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"device_serial_number");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//设备序列号
            about_machien_info.m_device_serial_number = pCJSONFunCode->valuestring;
            if(NULL != CCommonData::getInstance())
            {
                CCommonData::getInstance()->m_snNum = pCJSONFunCode->valuestring;
            }
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"mac_addr");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//MAC地址
            about_machien_info.m_mac_addr = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_parameter");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//CAM1-1参数
            about_machien_info.m_cam1_1_parameter = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_3_parameter");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {//CAM2-1参数
            about_machien_info.m_cam2_3_parameter = pCJSONFunCode->valuestring;
        }
        emit signal_about_machine_widget_event(about_machien_info);
    }while(0);
}
void CMainPage::gui_run_status(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        //qDebug() << "CMainPage::gui_run_status start";
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(NULL == pCJSONFunCode)
        {
            break;
        }

        CCommonData::getInstance()->m_runStatusInfo.m_type = pCJSONFunCode->valueint;
        if(1 == pCJSONFunCode->valueint)
        {//1:移动网络

#if 0
            pCJSONFunCode = cJSON_GetObjectItem(root,"model_status");
            if(pCJSONFunCode)
            {//模块状态　1:正常 2:不正常
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_model_status = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"sim_status");
            if(pCJSONFunCode)
            {//SIM卡　1:存在 2:不存在
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_sim_status = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"signal_status");
            if(pCJSONFunCode)
            {//信号质量 1:无信号 2:1格信号 3:2格信号 4:3格信号 5:4格信号 6:无插卡
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_signal_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"network_type");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//网络类型
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_network_type = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"reset_times");
            if(pCJSONFunCode)
            {//复位次数
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_reset_times = pCJSONFunCode->valueint;
            }
            #endif
            //中心服务器1
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_status");
            if(pCJSONFunCode)
            {//1:未使用 2:拔号中 3:连接成功
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_status = pCJSONFunCode->valueint;
            }
           pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_addr");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//服务器地址
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_addr = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_port");
            if(pCJSONFunCode)
            {//端口
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_port = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_type");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//网络类型
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_type = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_connect_times");
            if(pCJSONFunCode)
            {//当日连接次数
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_connect_times = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_last_connect");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//最后连接时间
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_last_connect = pCJSONFunCode->valuestring;
            }
            //中心服务器2
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_status");
            if(pCJSONFunCode)
            {//1:未使用 2:拔号中 3:连接成功
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_addr");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//服务器地址
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_addr = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_port");
            if(pCJSONFunCode)
            {//端口
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_port = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_type");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//网络类型
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_type = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_connect_times");
            if(pCJSONFunCode)
            {//当日连接次数
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_connect_times = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_last_connect");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//最后连接时间
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_last_connect = pCJSONFunCode->valuestring;
            }
            //视频服务器
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_status");
            if(pCJSONFunCode)
            {//1:未使用 2:拔号中 3:连接成功
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_addr");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//服务器地址
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_addr = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_port");
            if(pCJSONFunCode)
            {//端口
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_port = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_type");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//网络类型
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_type = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_connect_times");
            if(pCJSONFunCode)
            {//当日连接次数
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_connect_times = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_last_connect");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//最后连接时间
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->video_server_last_connect = pCJSONFunCode->valuestring;
            }

             pCJSONFunCode = cJSON_GetObjectItem(root,"use_flow");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//本月已使用流量
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->m_use_flow = pCJSONFunCode->valuestring;
            }
            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

        }
        else if(2 == pCJSONFunCode->valueint)
        {//2:I/O


            pCJSONFunCode = cJSON_GetObjectItem(root,"front_door_status");
            if(pCJSONFunCode)
            {//前门 1:开启　2:关闭
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->front_door_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"middle_door_status");
            if(pCJSONFunCode)
            {//中门
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->middle_door_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"back_door_status");
            if(pCJSONFunCode)
            {//后门
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->back_door_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"back_car_status");
            if(pCJSONFunCode)
            {//倒车
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->back_car_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"stop_car_status");
            if(pCJSONFunCode)
            {//刹车
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_stop_car_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"wiper_status");
            if(pCJSONFunCode)
            {//雨刮
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_wiper_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"left_lamp_status");
            if(pCJSONFunCode)
            {//左转灯
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_left_lamp_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"right_lamp_status");
            if(pCJSONFunCode)
            {//右转灯
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_right_lamp_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"io1_status");
            if(pCJSONFunCode)
            {//I/O输出1
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_io1_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"io2_status");
            if(pCJSONFunCode)
            {//I/O输出2
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_io2_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"police1_status");
            if(pCJSONFunCode)
            {//报警输入1
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_police1_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"police2_status");
            if(pCJSONFunCode)
            {//报警输入2
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_police2_status = pCJSONFunCode->valueint;
            }

            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

        }
        else if(3 == pCJSONFunCode->valueint)
        {//3:定位模块


            pCJSONFunCode = cJSON_GetObjectItem(root,"gps_number");
            if(pCJSONFunCode)
            {//卫星颗数
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_gps_number = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"longitude");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//经度
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_longitude = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"latitude");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//纬度
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_latitude = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"Azimuth");
            if(pCJSONFunCode)
            {//方位角
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_Azimuth = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"height");
            if(pCJSONFunCode)
            {//海拔高度
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"real_speed");
            if(pCJSONFunCode)
            {//实时速度
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusGPS->m_real_speed = pCJSONFunCode->valueint;
            }
            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

        }
        else if(4 == pCJSONFunCode->valueint)
        {//4:本地网络

            pCJSONFunCode = cJSON_GetObjectItem(root,"pon1_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon1_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon1_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon1_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon2_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon2_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon2_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon2_ip = pCJSONFunCode->valuestring;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"pon3_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon3_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon3_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon3_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon4_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon4_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon4_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon4_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon5_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon5_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon5_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon5_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon6_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon6_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon6_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon6_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon7_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon7_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon7_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon7_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon8_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon8_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon8_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon8_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon9_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon9_status  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon9_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon9_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon10_status");
            if(pCJSONFunCode)
            {//1:已连接 2:未连接  3:连接异常  4:连接中
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon10_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"pon10_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_pon10_ip = pCJSONFunCode->valuestring;
            }
            ////////////////////////////////////////////////////////////////////////D2 start
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_status");
            if(pCJSONFunCode)
            {
              //CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->local_network_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_ip");
            if(pCJSONFunCode)
            {
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->local_network_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_subnet_mask");
            if(pCJSONFunCode)
            {
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->local_network_subnet_mask = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_gateway");
            if(pCJSONFunCode)
            {
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->local_network_gateway = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_dns1");
            if(pCJSONFunCode)
            {
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->local_network_dns1 = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_dns2");
            if(pCJSONFunCode)
            {
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->local_network_dns2 = pCJSONFunCode->valuestring;
            }

            ////////////////////////////////////////////////////////////////////////D2 end

            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

        }
        else if(5 == pCJSONFunCode->valueint)
        {//5:设备


            pCJSONFunCode = cJSON_GetObjectItem(root,"device_door");
            if(pCJSONFunCode)
            {//设备门  1:开启  2:关闭
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_device_door  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"fan_speed");
            if(pCJSONFunCode)
            {//风扇转速
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_fan_speed = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"input_voltage");
            if(pCJSONFunCode)
            {//输入电压
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_input_voltage = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"device_temperature");
            if(pCJSONFunCode)
            {//设备温度
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_device_temperature = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"car_out_temperature");
            if(pCJSONFunCode)
            {//车外温度
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_car_out_temperature = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"car_in_temperature");
            if(pCJSONFunCode)
            {//车内温度
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_car_in_temperature = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"serial1_port_status");
            if(pCJSONFunCode)
            {//串口1状态  1:连接  2:未连接
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_serial1_port_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"serial2_port_status");
            if(pCJSONFunCode)
            {//串口2状态  1:连接  2:未连接
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_serial2_port_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"serial3_port_status");
            if(pCJSONFunCode)
            {//串口3状态  1:连接  2:未连接
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_serial3_port_status  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"serial4_port_status");
            if(pCJSONFunCode)
            {//串口4状态  1:连接  2:未连接
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_serial4_port_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"serial5_port_status");
            if(pCJSONFunCode)
            {//串口5状态  1:连接  2:未连接
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_serial5_port_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"can1_status");
            if(pCJSONFunCode)
            {//CAN1状态
               CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_can1_status = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"can2_status");
            if(pCJSONFunCode)
            {//CAN2状态
              CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_can2_status = pCJSONFunCode->valueint;
            }
            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

        }
        else if(7 == pCJSONFunCode->valueint)
        {//7:外设

            QString str_show_content = tr("探头");
            bool has_police = false;

            pCJSONFunCode = cJSON_GetObjectItem(root,"probe1");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe1 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "1 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe2");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe2 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "2 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe3");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe3 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "3 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe4");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe4 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "4 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe5");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe5 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "5 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe6");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe6 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "6 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe7");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe7 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "7 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe8");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe8 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "8 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe9");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe9 = pCJSONFunCode->valueint;
                if(2 == pCJSONFunCode->valueint)
                {
                    str_show_content += "9 ";
                    has_police = true;
                }
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"probe10");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_runStatusInfo.m_CPeripheralStatus->probe10 = pCJSONFunCode->valueint;
            }
            //qDebug() << "CMainPage::gui_run_status 1";
            emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);


            if(has_police && !CMainMessageDialog::getInstance()->isVisible())
            {
                str_show_content += "报警!";
                qDebug() << "CMainPage::gui_run_status police!!";
                CMainMessageDialog::getInstance()->move((1024 - 723)/2,(768 - 551)/2);

                CMainMessageDialog::getInstance()->set_show_content(CMainMessageDialog::kEnumMessageDialog,
                    str_show_content,
                    "");
                CMainMessageDialog::getInstance()->exec();
            }

        }

    }while(0);
}
void CMainPage::gui_driver_menu(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
         pCJSONFunCode = cJSON_GetObjectItem(root,"travel_direction");
        if(pCJSONFunCode)
        {/*行驶方向*/
            CCommonData::getInstance()->m_CDriverMenuInfo.travel_direction = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"deputy_page_show_mode");
        if(pCJSONFunCode)
        {/*副主页显示模式*/
            CCommonData::getInstance()->m_CDriverMenuInfo.deputy_page_show_mode = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"out_door_video_change");
        if(pCJSONFunCode)
        {/*下客门视频切换方式*/
            CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_change = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"out_door_video_passageway");
        if(pCJSONFunCode)
        {/*下客门视频通道*/
            CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"back_car_video_passageway");
        if(pCJSONFunCode)
        {/*倒车视频通道*/
            CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"start_plan_passageway");
        if(pCJSONFunCode)
        {/*发车计划声道*/
            CCommonData::getInstance()->m_CDriverMenuInfo.start_plan_passageway = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"over_speed_warning_passageway");
        if(pCJSONFunCode)
        {/*超速警示声道*/
            CCommonData::getInstance()->m_CDriverMenuInfo.over_speed_warning_passageway = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"message_outtime");
        if(pCJSONFunCode)
        {/*消息超时退出*/
            CCommonData::getInstance()->m_CDriverMenuInfo.message_outtime = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"video_default_frame");
        if(pCJSONFunCode)
        {/*视频默认画面*/
            CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"video_round_robin");
        if(pCJSONFunCode)
        {/*视频轮循*/
            CCommonData::getInstance()->m_CDriverMenuInfo.video_round_robin = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"round_robin_passageway");
        if(pCJSONFunCode)
        {/*轮循通道*/
            CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_passageway = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"round_robin_interval");
        if(pCJSONFunCode)
        {/*轮循通道*/
            CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"out_door_video_passageway_enable");
        if(pCJSONFunCode)
        {//下客门视频通道使能
            CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"back_car_video_passageway_enable");
        if(pCJSONFunCode)
        {//倒车视频通道使能
            CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"out_door_delay_quit");
        if(pCJSONFunCode)
        {//下客门延时退出时长
            CCommonData::getInstance()->m_CDriverMenuInfo.out_door_delay_quit = pCJSONFunCode->valueint;
        }

        cJSON *pJson_array=cJSON_GetObjectItem(root,"rows");/*取数组*/

        int array_size=cJSON_GetArraySize(pJson_array);/*数组大小*/
        cJSON *tasklist=pJson_array->child;/*子对象*/
        int get_value = 0;
        for(int index = 0;index < array_size;index++) /*30个数据*/
        {
            pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CDriverMenuInfo.passageway[index] = get_value = pCJSONFunCode->valueint;
            }
            tasklist = tasklist->next;
            if(NULL == tasklist)
            {
                break;
            }
            if(index >= 30)
            {
                break;
            }
        }
        emit signal_driver_menu_widget_event(CCommonData::getInstance()->m_CDriverMenuInfo);
    }while(0);
}
void CMainPage::gui_message(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        if(1 == pCJSONFunCode->valueint)
        {
            CCommonQueryMode mode;
            CPresetEvent *pPresetEvent = NULL;
            pPresetEvent = new CPresetEvent;

            pCJSONFunCode = cJSON_GetObjectItem(root,"message");
            if(pCJSONFunCode)
            {
                pPresetEvent->m_content = gb2312ToUtf8(pCJSONFunCode->valuestring);
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"code");
            if(pCJSONFunCode)
            {
                pPresetEvent->m_event_code = pCJSONFunCode->valueint;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"number");
            if(pCJSONFunCode)
            {
                pPresetEvent->m_number = pCJSONFunCode->valueint;
            }

            mode.data = pPresetEvent;
            emit signal_message_widget_notify_event(1,mode);
        }
        else if(2 == pCJSONFunCode->valueint)
        {
            CCommonQueryMode mode;
            CImportantMessage *pMessage = new CImportantMessage;
            pCJSONFunCode = cJSON_GetObjectItem(root,"message");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {
                pMessage->m_content = gb2312ToUtf8(pCJSONFunCode->valuestring);
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"status");
            if(pCJSONFunCode)
            {
                pMessage->m_status = pCJSONFunCode->valueint;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"time");
            if(pCJSONFunCode)
            {
                pMessage->m_recv_time = QDateTime::fromMSecsSinceEpoch(pCJSONFunCode->valuedouble).toString("yyyy-MM-dd hh:mm");;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"number");
            if(pCJSONFunCode)
            {
                pMessage->m_number = pCJSONFunCode->valueint;
            }

            mode.data = pMessage;
            emit signal_message_widget_notify_event(2,mode);
        }
        else if(3 == pCJSONFunCode->valueint)
        {
            CCommonQueryMode mode;
            CVehiclePlan *pVehiclePlan = new CVehiclePlan;
            //int status = 0;

            pCJSONFunCode = cJSON_GetObjectItem(root,"time");
            if(pCJSONFunCode)
            {
                pVehiclePlan->m_recv_time = pCJSONFunCode->valuestring;
            }

             pCJSONFunCode = cJSON_GetObjectItem(root,"number");
            if(pCJSONFunCode)
            {
                pVehiclePlan->m_number = pCJSONFunCode->valueint;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"message");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {
                pVehiclePlan->m_attribute = gb2312ToUtf8(pCJSONFunCode->valuestring);
            }
#if 0
            pCJSONFunCode = cJSON_GetObjectItem(root,"status");
            if(pCJSONFunCode)
            {
                status = pCJSONFunCode->valueint;
            }
            if(1 == status)
                pVehiclePlan->m_attribute = tr("等待执行");
            else
                pVehiclePlan->m_attribute = tr("执行完毕");
#endif
            mode.data = pVehiclePlan;
            emit signal_message_widget_notify_event(3,mode);
        }

    }while(0);

}

int CMainPage::main_page_handle(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    char szCommType[64];
    int nStrLen = 0;

    pCJSONFunCode = cJSON_GetObjectItem(root,"msg_type");
    if(pCJSONFunCode != NULL  && pCJSONFunCode->valuestring)
    {
        nStrLen = strlen(pCJSONFunCode->valuestring);
        if(nStrLen > 64)nStrLen=64;

        memset(szCommType, 0, sizeof(szCommType));
        memcpy(szCommType, pCJSONFunCode->valuestring, nStrLen);
    }
#if 1
    bool debug_output = false;
    if(0 == strcmp(szCommType, "gui_system_videotape_query"))
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(3 == pCJSONFunCode->valueint)
        {
            debug_output = false;
        }
    }

    if(0 == strcmp(szCommType, "get_flash_card_info") ||
       0 == strcmp(szCommType, "gui_system_register_info") ||
       0 == strcmp(szCommType, "gui_system_tbp_connect_status"))
    {
        debug_output = false;
    }

    if(
        0 != strcmp(szCommType, "system_gps_info") &&
       0 != strcmp(szCommType, "passenger_flow_info") &&
       0 != strcmp(szCommType, "system_itsio_io_status") &&
       0 != strcmp(szCommType, "system_io_raw")
       )
    {

        char *out = NULL;
        if(debug_output)
        {
            printf("gui recv() szCommType: %s \n", szCommType);
            out = cJSON_PrintUnformatted(root);
            if(out)
            {
                CCommonInterface::printf_debug(out);
                free(out);
            }
        }

    }
#endif

    do
    {
        if(!strcmp(szCommType, "system_key_up"))
        {
#ifdef APPLICATION_TYPE_P2
            gui_system_key_up_4(root);
#else
            gui_system_key_up(root);
#endif
        }
        else if(!strcmp(szCommType, "system_touch"))
        {
            gui_system_touch(root);
        }
        else if(!strcmp(szCommType, "system_gps_info"))
        {
            gui_system_gps_info(root);
        }
        else if(!strcmp(szCommType, "system_gps_db"))
        {
            gui_system_gps_db(root);
        }
        else if(!strcmp(szCommType, "system_itssd_notify_info"))
        {
            gui_system_itssd_notify_info(root);
        }
        else if(!strcmp(szCommType, "system_notify_station"))
        {
            gui_system_notify_station(root);
        }
        else if(!strcmp(szCommType, "system_direction_change"))
        {
            gui_system_direction_change(root);
        }
        else if(!strcmp(szCommType, "system_g3_modem_status"))
        {
            gui_system_g3_modem_status(root);
        }
        else if(!strcmp(szCommType, "system_itsnw_connect_status"))/*调度平台连接*/
        {
            gui_system_itsnw_connect_status(root);
        }
        else if(!strcmp(szCommType, "system_stmdev_connect_status"))/*视频后台连接*/
        {
            gui_system_stmdev_connect_status(root);
        }
        else if(!strcmp(szCommType, "system_exception_overspeed"))
        {
            gui_system_exception_overspeed(root);
        }
        else if(!strcmp(szCommType, "system_exception_rate_limit"))
        {
            gui_system_exception_rate_limit(root);
        }
        else if(!strcmp(szCommType, "system_itsio_io_status"))
        {
            gui_system_itsio_io_status(root);
        }
        else if(!strcmp(szCommType, "system_hdd_status"))
        {
            gui_system_hdd_status(root);
        }
        else if(!strcmp(szCommType, "system_userin_card"))
        {
            gui_system_userin_card(root);
        }
        else if(!strcmp(szCommType, "system_its_scheluing_plan"))
        {
            gui_system_its_scheluing_plan(root);
        }
        else if(!strcmp(szCommType, "system_its_task_info"))
        {
            gui_system_its_task_info(root);
        }
        else if(!strcmp(szCommType, "system_station_type"))
        {
             gui_system_station_type(root);
        }
        else if(!strcmp(szCommType, "system_shutdown"))
        {
            gui_system_shutdown(root);
             //关机
        }
        else if(!strcmp(szCommType, "system_exception_analog"))
        {
            gui_system_exception_analog(root);
        }
        else if(!strcmp(szCommType, "system_working_status"))
        {
            gui_system_working_status(root);
        }
        else if(!strcmp(szCommType, "system_itssd_para"))
        {
            gui_system_itssd_para(root);
        }
        else if(!strcmp(szCommType, "system_av_videoloss"))
        {
            gui_system_av_videoloss(root);

        }
        else if(!strcmp(szCommType, "gui_voice"))
        {
            gui_voice(root);
        }
        else if(!strcmp(szCommType, "gui_light"))
        {
            gui_light(root);
        }
        else if(!strcmp(szCommType, "gui_videotape_video"))
        {
            gui_videotape_video(root);
        }
        else if(!strcmp(szCommType, "gui_about_machine"))
        {
            gui_about_machine(root);
        }
        else if(!strcmp(szCommType, "gui_run_status"))
        {
            gui_run_status(root);
        }
        else if(!strcmp(szCommType, "gui_driver_menu"))
        {
            gui_driver_menu(root);

        }
        else if(!strcmp(szCommType, "gui_message"))
        {
            gui_message(root);
        }
        else if(!strcmp(szCommType, "gui_system_register_info"))
        {/*菜单-系统管理-注册信息*/
            qDebug()<<"CMainPage::trcmp(szCommType gui_system_register_info ";
            gui_system_register_info(root);
        }
        else if(!strcmp(szCommType, "gui_system_coordinate_collection"))
        {/*菜单-系统管理-坐标采集*/
            gui_system_coordinate_collection(root);
        }
        else if(!strcmp(szCommType, "gui_system_videotape_query"))
        {/*菜单-系统管理-录像查询*/
            gui_system_videotape_query(root);
        }
        else if(!strcmp(szCommType, "gui_system_storage_manage"))
        {/*菜单-系统管理-存储管理*/
            gui_system_storage_manage(root);
        }
        else if(!strcmp(szCommType, "gui_system_network"))
        {/*菜单-系统管理-网络*/
            gui_system_network(root);
        }
        else if(!strcmp(szCommType, "gui_system_wifi"))
        {/*菜单-系统管理-网络-WIFI*/
            gui_system_wifi(root);
        }
        else if(!strcmp(szCommType, "gui_system_switch_config"))
        {/*菜单-系统管理-I/O配置*/
            gui_system_switch_config(root);
        }
        else if(!strcmp(szCommType, "gui_system_senior_setup"))
        {/*菜单-系统管理-高级设置*/
            gui_system_senior_setup(root);
        }
        else if(!strcmp(szCommType, "gui_system_system_setup"))
        {/*菜单-系统管理-系统设置*/
            gui_system_system_setup(root);
        }
        else if(!strcmp(szCommType, "gui_system_maintain"))
        {/*菜单-系统管理-系统维护*/
            gui_system_maintain(root);
        }
        else if(!strcmp(szCommType, "gui_system_camera_config"))
        {/*菜单-系统管理-CAM配置*/
            gui_system_camera_config(root);
        }
        else if(!strcmp(szCommType, "gui_system_camera_pon_cam"))
        {/*菜单-系统管理-CAM配置-PON-CAM*/
            gui_system_camera_pon_cam(root);
        }
        else if(!strcmp(szCommType, "gui_system_passenger_flow"))
        {/*菜单-系统管理-客流统计*/
            gui_system_passenger_flow(root);
        }
        else if(!strcmp(szCommType, "passenger_flow_info"))
        {
            gui_passenger_flow_info(root);
        }
        else if(!strcmp(szCommType, "sdcard_format_progress"))
        {
            gui_sdcard_format_progress(root);
        }
        else if(!strcmp(szCommType, "system_bus_interval"))
        {
            gui_system_bus_interval(root);
        }
        else if(!strcmp(szCommType, "gui_login_info"))
        {
            gui_login_info(root);
        }
        else if(!strcmp(szCommType, "two_dimensional_code_info"))
        {
            gui_two_dimensional_code_info(root);
        }
        else if(!strcmp(szCommType, "gui_picture_play"))
        {
            gui_picture_play(root);
        }
        else if(!strcmp(szCommType, "get_qr_pay_info"))
        {
            gui_get_qr_pay_info(root);
        }
        else if(!strcmp(szCommType, "system_io_raw"))
        {
            gui_system_io_raw(root);
        }
        else if(!strcmp(szCommType, "gui_main_page"))
        {
            gui_main_page(root);
        }
        else if(!strcmp(szCommType, "get_flash_card_info"))
        {
            gui_get_flash_card_info(root);
        }
        else if(!strcmp(szCommType, "gui_system_reboot"))
        {
            gui_system_reboot(root);
        }
        else if(!strcmp(szCommType, "gui_system_pos_config"))
        {
            gui_system_pos_cofnig(root);
        }
        else if(!strcmp(szCommType, "gui_system_tbp_connect_status"))
        {
            gui_system_tbp_connect_status(root);
        }
        else if(!strcmp(szCommType, "gui_system_heartbeat_notify"))
        {
            gui_system_heartbeat_notify(root);
        }
        else if(!strcmp(szCommType, "mouse_status"))
        {
            gui_mouse_info(root);
        }
        else if(!strcmp(szCommType, "gui_test_heartbeat"))
        {
            gui_test_heartbeat(root);
        }
        else if(!strcmp(szCommType, "gui_g3_network_status"))
        {
            gui_g3_network_status(root);
        }
        else if (!strcmp(szCommType, "gui_starting_screen_flag"))
        {
           qDebug()<<"szCommType:: gui_starting_screen_flag(cJSON *root)<<<<<<<<<";
           gui_starting_screen_flag(root);
        }
        else if (!strcmp(szCommType, "gui_tmp_connect_status"))
        {
            gui_tmp_status(root);
        }

        else if (!strcmp(szCommType, "gui_tmp_login_status"))
        {
            gui_tmp_status(root);
        }

        else if (!strcmp(szCommType, "gui_alg_image_update"))
        {
            qDebug()<<"szCommType:: gui_alg_image_update(cJSON *root)<<<<<<<<<";
            gui_alg_image_update(root);
        }

    }while(0);
    return 0;
}



/*
void CMainPage::gui_(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {

    }while(0);
}
*/
#include <syslog.h>
void LtyLogOut(int priority, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsyslog(priority, fmt, ap);
    va_end(ap);
}
#define LOG_INFO	6
#define LTY_LOG_PREFIX ""
#define COLOR_WHITE     "\033[0;39m"
#define COLOR_GREEN     "\033[0;32m"
#define LOGOUT(fmt, args...)  do{LtyLogOut(LOG_INFO, "[LOG]" LTY_LOG_PREFIX fmt, ##args); \
                                printf(COLOR_GREEN LTY_LOG_PREFIX "%s: " fmt COLOR_WHITE, __func__, ##args);}while(0)

void CMainPage::gui_test_heartbeat(cJSON *root)
{
    static unsigned int  msg_sn = 0;
    unsigned int max_int = 0-1;

    if(msg_sn < max_int)
        msg_sn++;
    else
        msg_sn = 0;

    //LOGOUT("thread2:gui [%s] index=%d\n",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz").toUtf8().data(),msg_sn);
    //CCommonInterface::printf_debug();

}

#ifdef CURRENT_ARM_RUN
#include <QWSServer>
#endif
void CMainPage::gui_mouse_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;

    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {
            #ifdef CURRENT_ARM_RUN
            if(11 == pCJSONFunCode->valueint)
            {
                QWSServer::setCursorVisible(true);
            }
            else
            {
                QWSServer::setCursorVisible(false);
            }
            #endif
        }


     }while(0);
}

void CMainPage::gui_system_heartbeat_notify(cJSON *root)
{
    CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusLocalNetwork->m_socket_status = 0;
    CCommonData::getInstance()->m_runStatusInfo.m_type = 4;
    emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
}

void CMainPage::gui_system_pos_cofnig(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    static bool first_init = true;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"DEVMODE");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE = pCJSONFunCode->valueint;
            qDebug()<<"gui_system_pos_cofnig CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE:"<<CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"CHARGMODE");
        if(pCJSONFunCode)
        {

            CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE = pCJSONFunCode->valueint;
            qDebug()<<"gui_system_pos_cofnig CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE:"<<CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"PSAMNUM");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemPosSetup.PSAMNUM = pCJSONFunCode->valueint;
           qDebug()<<"gui_system_pos_cofnig CommonData::getInstance()->m_CSystemPosSetup.PSAMNUM:"<<CCommonData::getInstance()->m_CSystemPosSetup.PSAMNUM;

        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"SECTOR");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemPosSetup.SECTOR = pCJSONFunCode->valueint;
            qDebug()<<"gui_system_pos_cofnig CommonData::getInstance()->m_CSystemPosSetup.SECTOR:"<<CCommonData::getInstance()->m_CSystemPosSetup.SECTOR;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"TIMEINTER");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemPosSetup.TIMEINTER = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"TICKETTIME");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemPosSetup.TICKETTIME = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"host_ip");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            CCommonData::getInstance()->m_CSystemPosSetup.m_host_ip = pCJSONFunCode->valuestring;
        }

        cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
        int array_size = 0;
        cJSON *tasklist = NULL;
        if(NULL != pJson_array)
        {
            array_size = cJSON_GetArraySize(pJson_array);/*数组大小*/
            tasklist=pJson_array->child;/*子对象*/
        }

        for(int index = 0;index < array_size;index++)
        {

            pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[index] = pCJSONFunCode->valueint; /*值*/
            }

            tasklist = tasklist->next;
            if(NULL == tasklist)
            {
                break;
            }
        }
        emit signal_pos_config_widget_event(CCommonData::getInstance()->m_CSystemPosSetup);

        if(first_init)
        {
            first_init = false;
            //qDebug() << "DEVMODE:" << QString::number(CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE);
            if(e_Dev_Mode_Slave_Mac == CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
            {
            #if 1
                emit signal_mainwindow_show(true);
                //qDebug() << "DEVMODE: enter";
                main_page_notify(kEnumMainPageFromStatus,0,0);
                main_page_notify(kEnumMainPageFromMachine,0,0);
            #endif
            }
        }


    }while(0);
}

void CMainPage::gui_system_reboot(cJSON *root)
{
    //cJSON *pCJSONFunCode = NULL;
    do
    {
        if(CMyMessageBox::getInstance()->isVisible())
            CMyMessageBox::getInstance()->slot_pushbutton_close();

        MainWindow::getInstance()->slot_pushbutton_shutdown();

    }while(0);
}

#define CARD_FLASH_SUCCESS		1	/*刷卡成功*/
#define CARD_FLASH_FAIL			2	/*刷卡失败*/
#define CARD_FLASH_ABNORMAL		3	/*刷卡异常*/
#define CARD_FLASH_BALANCE_LOW	4	/*余额不足*/
#define CARD_FLASH_MESSAGE		5	/*自定义消息*/
#define QR_SCAN_SUCCESS			6	/*扫码成功*/
#define QR_SCAN_FAILED			7	/*验码失败*/
#define QR_SCAN_ABNORMAL		8	/*扫码异常*/

void CMainPage::gui_get_flash_card_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        char type = 0;
        QString str_money = "";
        QString str_money_total = "";
        QString str_fail_result = "";
        int show_time = 0;
        int message_tip_type = 0;
        int card_type = 0;

        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        type = pCJSONFunCode->valueint;

        printf("gui_get_flash_card_info() type:%d\n", type);


        pCJSONFunCode = cJSON_GetObjectItem(root,"money");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            str_money = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"money_total");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            str_money_total = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"fail_result");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            str_fail_result = gb2312ToUtf8(pCJSONFunCode->valuestring);
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"show_time");
        if(pCJSONFunCode)
        {
            show_time = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"message_tip_type");
        if(pCJSONFunCode)
        {
            message_tip_type = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"card_type");
        if(pCJSONFunCode)
        {
            card_type = pCJSONFunCode->valueint;
        }

        bool goto_homepage = true;
        if(str_fail_result.contains("PSAM",Qt::CaseInsensitive))
        {
            goto_homepage = false;
        }

        CEventData eventData;
        eventData.m_i_data = show_time;
        if(CARD_FLASH_SUCCESS == type)
        {

            printf("gui_get_flash_card_info() CARD_FLASH_SUCCESS\n");
            eventData.m_param1 = card_type;
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusSuccess,&eventData);
            eventData.m_str_data = str_money + tr("元");
            main_page_notify(kEnumMainPageAmount,0,&eventData);   //金额
            eventData.m_str_data = str_money_total;
            main_page_notify(kEnumMainPageBalance,0,&eventData);   //余额
        }
        else if(CARD_FLASH_FAIL == type)
        {
             printf("gui_get_flash_card_info() CARD_FLASH_FAIL\n");

            eventData.m_str_data = str_fail_result;
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusFail,&eventData);
        }
        else if(CARD_FLASH_ABNORMAL == type)
        {
             printf("gui_get_flash_card_info() CARD_FLASH_ABNORMAL\n");

            eventData.m_str_data = str_fail_result;
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusAbnormal,&eventData);
        }
        else if(CARD_FLASH_BALANCE_LOW == type)
        {
            printf("gui_get_flash_card_info() CARD_FLASH_BALANCE_LOW\n");
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusFlow,0);
        }
        else if(CARD_FLASH_MESSAGE == type)
        {
               printf("gui_get_flash_card_info() CARD_FLASH_MESSAGE\n");

            eventData.m_str_data = str_fail_result;
            eventData.m_param1 = message_tip_type;
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusMessage,&eventData);
        }
        else if(QR_SCAN_SUCCESS == type)
        {/*扫码成功*/
            eventData.m_str_data = str_money + tr("元");
            main_page_notify(kEnumMainPageAmount,0,&eventData);   //金额

            if(e_Charge_Mode_One_Ticket == CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE)
                eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_last_station;
            else
                eventData.m_str_data = CCommonData::getInstance()->m_station_info.m_current_station;

            main_page_notify(kEnumMainPageUpdateStationName1,0,&eventData);

            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumQrStatusSuccess,&eventData);
        }
        else if(QR_SCAN_FAILED == type)
        {/*验码失败*/
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumQrStatusFail,&eventData);
        }
        else if(QR_SCAN_ABNORMAL == type)
        {/*扫码异常*/
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumQrStatusabNormal,&eventData);
        }

        if(goto_homepage)
        {
            if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
            {
                if(!MainWindow::getInstance()->isVisible())
                {
                     emit signal_mainwindow_show(true);
                     CProtocol::getInstance()->call_json_system_passenger_flow(7,2,0);
                }
            }
            else
            {
                emit signal_mainwindow_show(true);
            }
        }



    }while(0);
}

void CMainPage::gui_main_page(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"price_type");
        if(pCJSONFunCode)
        {
            qDebug()<<"gui_main_page pCJSONFunCode->valueint:"<<pCJSONFunCode->valueint;
            if(e_Charge_Mode_One_Ticket == pCJSONFunCode->valueint)
            {
                main_page_notify(kEnumMainPageChargeType, e_Charge_Mode_One_Ticket,0);
            }
            else if(e_Charge_Mode_Block_Charge == pCJSONFunCode->valueint)
            {
               main_page_notify(kEnumMainPageChargeType, e_Charge_Mode_Block_Charge,0);
            }

        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"price_number");
        if(pCJSONFunCode)
        {

        }


    }while(0);
}

void CMainPage::gui_system_io_raw(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {

        pCJSONFunCode = cJSON_GetObjectItem(root,"temp_in");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_car_in_temperature = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"temp_out");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_car_out_temperature = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"temp_dev");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_device_temperature = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"fan_rate");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_fan_speed = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"power_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusDevice->m_input_voltage = pCJSONFunCode->valueint;
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"io_right");//右转
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_right_lamp_status = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_right_lamp_status = 2;

        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_left");//左转
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_left_lamp_status = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_left_lamp_status = 2;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_brake");//刹车
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_stop_car_status = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_stop_car_status = 2;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_wind_screen");//雨刮
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_wiper_status = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_wiper_status = 2;
        }

         pCJSONFunCode = cJSON_GetObjectItem(root,"io_wind_screen");//雨刮
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_wiper_status = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_wiper_status = 2;
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"io_alarm1");//报警1
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_police1_status = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_police1_status = 2;
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"io_alarm2");//报警2
        if(pCJSONFunCode)
        {
            if(2 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_police2_status = 1;
            else
                CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusIO->m_police2_status = 2;
        }

        CCommonData::getInstance()->m_runStatusInfo.m_type = 2;
        emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
        CCommonData::getInstance()->m_runStatusInfo.m_type = 5;
        emit signal_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

    }while(0);
}

#define KEY_BASE  0x00
#define MENU_KEY_PREESED (KEY_BASE)
#define UP_KEY_PREESED (KEY_BASE + 1)
#define DOWN_KEY_PREESED (KEY_BASE + 2)
#define BACK_KEY_PREESED (KEY_BASE + 3)

#define MENU_KEY_RELEASED (MENU_KEY_PREESED|0x80)
#define UP_KEY_RELEASEDD (UP_KEY_PREESED|0x80)
#define DOWN_KEY_RELEASED (DOWN_KEY_PREESED|0x80)
#define BACK_KEY_RELEASED (BACK_KEY_PREESED|0x80)

enum
{
    kEnumEventKeyMenu = 1,
    kEnumEventKeyUp,       //按3下进入客流界面
    kEnumEventKeyDown,     //3下导入刷卡参数  5下导出刷卡参数
    kEnumEventKeyBack      //3下导入报站      5下导出报站
};


void CMainPage::gui_system_key_up_4(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;

    do
    {
        int key_code = 0;
        pCJSONFunCode = cJSON_GetObjectItem(root,"key_code");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        key_code = pCJSONFunCode->valueint;
        if(MENU_KEY_RELEASED == key_code)
        {//菜单按键//
            qDebug() <<"MENU_KEY_RELEASED";
            #ifdef MENU_KEY_SCREEN_SHOT
            ////截图开始，临时////////////////////////////////////
                QString fileName;
                QPixmap pix;
                QPixmap bmp;
                pix = bmp.grabWindow(this->winId(), 0,0, -1, -1);
                fileName = QDateTime::currentDateTime().toString("yyyy-MM-ddHH-mm-ss") + ".bmp";
                if(pix.isNull())
                {
                  qDebug() <<"error";
                }
                else
                {
                   if(!pix.save("/mnt/nfs/" + fileName, "BMP"))
                   {
                        qDebug() <<"save error\n";
                   }
                   else
                   {
                        qDebug() <<"save success\n";
                   }
                }
               ////截图结束，临时////////////////////////////////////
           #endif
           m_key_back_times = 0;
           // 如果是导航画面，key没有动作
           if(MainWindow::kEnumDeclaration == MainWindow::getInstance()->get_current_widget_id()
                || MainWindow::kEnumSetting == MainWindow::getInstance()->get_current_widget_id()
                || MainWindow::kEnumState== MainWindow::getInstance()->get_current_widget_id())
            {
                return;
            }
            if(CMainPage2::getInstance()->get_debug_interface())
            {
                break;
            }

            if(kEnumEventKeyMenu != m_current_key_event_type)
            {
                m_key_times = 0;
                if(m_timer_key_5s->isActive())
                    m_timer_key_5s->stop();

                m_current_key_event_type = kEnumEventKeyMenu;
            }
            qDebug() << "CMainPage::gui_system_key_up MENU_KEY_RELEASED";
            if(MainWindow::getInstance()->isVisible() && MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
                emit signal_pushbutton_menu();
            else
                sendCharacter(Qt::Key_Return);

        }
        else if(UP_KEY_RELEASEDD == key_code)
        {//向上按键
            m_key_back_times = 0;
          // 如果是导航画面，key没有动作
           if(MainWindow::kEnumDeclaration == MainWindow::getInstance()->get_current_widget_id()
                || MainWindow::kEnumSetting == MainWindow::getInstance()->get_current_widget_id()
                || MainWindow::kEnumState== MainWindow::getInstance()->get_current_widget_id())
            {
                return;
            }
            if(CMainPage2::getInstance()->get_debug_interface())
            {
                break;
            }

            qDebug() << "CMainPage::gui_system_key_up UP_KEY_RELEASEDD";
            if(kEnumEventKeyUp != m_current_key_event_type)
            {
                m_key_times = 0;
                m_timer_key_5s->setInterval(5000);
                if(m_timer_key_5s->isActive())
                    m_timer_key_5s->stop();
                m_current_key_event_type = kEnumEventKeyUp;
            }

            if(MainWindow::getInstance()->isVisible() && MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
            {
                m_key_times++;
                if(!m_timer_key_5s->isActive())
                    m_timer_key_5s->start();

                if(m_key_times >= 3)
                {
                    m_key_times = 0;
                    qDebug() << "CMainPage::gui_system_key_up hide mainwindow";
                    emit signal_mainwindow_show(false);
                    CProtocol::getInstance()->call_json_system_passenger_flow(7,1,0);
                }

            }
            else
            {
                int key_value = KEYPAD_KEY7_UP;
                if(keyboard_value(key_value))
                {
                    CCommonInterface::printf_debug("keyboard_value() return :" + QString::number(key_value,16));
                    return;
                }
               sendCharacter(Qt::Key_Up);
            }

        }
        else if(DOWN_KEY_RELEASED == key_code)
        {//向下按键
            m_key_back_times = 0;
            // 如果是导航画面，key没有动作
           if(MainWindow::kEnumDeclaration == MainWindow::getInstance()->get_current_widget_id()
                || MainWindow::kEnumSetting == MainWindow::getInstance()->get_current_widget_id()
                || MainWindow::kEnumState== MainWindow::getInstance()->get_current_widget_id())
            {
                return;
            }
            if(CMainPage2::getInstance()->get_debug_interface())
            {
                break;
            }
            slot_pushbutton_arrow_right();

            break;
            if(kEnumEventKeyDown != m_current_key_event_type)
            {
                m_key_times = 0;
                m_timer_key_5s->setInterval(1000);
                if(m_timer_key_5s->isActive())
                    m_timer_key_5s->stop();
                m_current_key_event_type = kEnumEventKeyDown;
            }

            if(MainWindow::getInstance()->isVisible() && MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
            {
                m_key_times++;
                if(m_timer_key_5s->isActive())
                    m_timer_key_5s->stop();

                m_timer_key_5s->start();
            }
            else
            {
                int key_value = KEYPAD_KEY7_UP;
                if(keyboard_value(key_value))
                {
                    CCommonInterface::printf_debug("keyboard_value() return :" + QString::number(key_value,16));
                    return;
                }
                sendCharacter(Qt::Key_Down);
            }
            qDebug() << "CMainPage::gui_system_key_up DOWN_KEY_RELEASED";

        }
        else if(BACK_KEY_RELEASED == key_code)
        {//返回按键
            qDebug() << "CMainPage::gui_system_key_up BACK_KEY_RELEASED";
            // 画面是导航画面的时候，back连续押５次，进入导航画面；其他key不做动作
             if(MainWindow::kEnumDeclaration == MainWindow::getInstance()->get_current_widget_id()
                || MainWindow::kEnumSetting == MainWindow::getInstance()->get_current_widget_id()
                || MainWindow::kEnumState== MainWindow::getInstance()->get_current_widget_id())
            {
               qDebug()<<"11111111111111111111 m_key_back_times: "<<m_key_back_times;
                m_key_back_times++;
                if(5 == m_key_back_times)
                {
                     m_key_back_times = 0;
                     emit signal_show_main_page(true);
                }
                return;
            }

            if(CMainPage2::getInstance()->get_debug_interface())
            {
                CMainPage2::getInstance()->debug_interface(false);
                break;
            }

            if(kEnumEventKeyBack != m_current_key_event_type)
            {
                m_key_times = 0;
                m_timer_key_5s->setInterval(1000);
                if(m_timer_key_5s->isActive())
                    m_timer_key_5s->stop();
                m_current_key_event_type = kEnumEventKeyBack;
            }

            if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
            {
                if(!MainWindow::getInstance()->isVisible())
                {
                     emit signal_mainwindow_show(true);
                     CProtocol::getInstance()->call_json_system_passenger_flow(7,2,0);
                }
                else
                {
                    m_key_times++;
                    if(m_timer_key_5s->isActive())
                        m_timer_key_5s->stop();
                    m_timer_key_5s->start();
                }
            }
            else
            {
                int point_x = CMyDefineScreen::getInstance()->get_change_factor_x(950) + MainWindow::getInstance()->pos().x();
                int point_y = CMyDefineScreen::getInstance()->get_change_factor_y(40) + MainWindow::getInstance()->pos().y();

                QPoint point(point_x,point_y);
                CCommonInterface::input_panel_control(false);

                if(CMyMouseEvent::getInstance()->is_head_back(point))
                {
                    CCommonInterface::printf_debug("gui_system_key_up value  BACK_KEY_RELEASED 1");
                    CMyMouseEvent::getInstance()->touch_click_event(point.x(),point.y(),false);
                    CMyMouseEvent::getInstance()->touch_click_event(point.x(),point.y(),true);
                }
                else
                {
                    int key_value = KEYPAD_KEY7_EXIT;
                    //qDebug() << "gui_system_key_up_4() 2";
                    if(keyboard_value(key_value))
                    {
                        CCommonInterface::printf_debug("keyboard_value() return :" + QString::number(key_value,16));
                        return;
                    }
                }
            }

        }

     }while(0);
}


void CMainPage::gui_get_qr_pay_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;

    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"driver_id");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CGetQrPayInfo.driver_id = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"driver_name");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CGetQrPayInfo.driver_name = pCJSONFunCode->valuestring;
        }
  //      pCJSONFunCode = cJSON_GetObjectItem(root,"money_qr");
//        if(pCJSONFunCode  && pCJSONFunCode->valuestring)
//        {
//           CCommonData::getInstance()->m_CGetQrPayInfo.qr_tang = pCJSONFunCode->valuestring;
//            CCommonData::getInstance()->m_CGetQrPayInfo.qr_tang =  CCommonData::getInstance()->m_CGetQrPayInfo.qr_tang.remove("|");
//            printf(">>>>>>>>>>>>>>>>>money_qr = %s<<<<<<<\n",CCommonData::getInstance()->m_CGetQrPayInfo.qr_tang.toStdString().c_str());
//        }
        #if 1
          pCJSONFunCode = cJSON_GetObjectItem(root,"total_money");
        if(pCJSONFunCode  && pCJSONFunCode->valuestring)
        {
           // CCommonData::getInstance()->m_CGetQrPayInfo.qr_tang =  QString::number(pCJSONFunCode->valueint,10);//QString::number(pCJSONFunCode->valuestring, 10);
            // CCommonData::getInstance()->m_CGetQrPayInfo.qr_tang =  CCommonData::getInstance()->m_CGetQrPayInfo.qr_tang.remove("|");
           CCommonData::getInstance()->m_CGetQrPayInfo.qr_tang = pCJSONFunCode->valuestring;
        }
         pCJSONFunCode = cJSON_GetObjectItem(root,"one_total_money");
        if(pCJSONFunCode)
        {
           CCommonData::getInstance()->m_CGetQrPayInfo.qr_number =pCJSONFunCode->valueint;
            //CCommonData::getInstance()->m_CGetQrPayInfo.qr_number = CCommonData::getInstance()->m_CGetQrPayInfo.qr_number.remove("|");

        }



        #endif

        pCJSONFunCode = cJSON_GetObjectItem(root,"money_total_qr");
        if(pCJSONFunCode  && pCJSONFunCode->valuestring)
        {
            CCommonData::getInstance()->m_CGetQrPayInfo.qr_ban = pCJSONFunCode->valuestring;
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"ic_card_tang");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            CCommonData::getInstance()->m_CGetQrPayInfo.ic_card_tang = pCJSONFunCode->valuestring;
            CCommonData::getInstance()->m_CGetQrPayInfo.ic_card_tang = CCommonData::getInstance()->m_CGetQrPayInfo.ic_card_tang.remove("|");
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"ic_card_ban");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            CCommonData::getInstance()->m_CGetQrPayInfo.ic_card_ban = pCJSONFunCode->valuestring;
            CCommonData::getInstance()->m_CGetQrPayInfo.ic_card_ban = CCommonData::getInstance()->m_CGetQrPayInfo.ic_card_ban.remove("|");
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"ic_number");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CGetQrPayInfo.ic_card_number = pCJSONFunCode->valueint;
        }
//        pCJSONFunCode = cJSON_GetObjectItem(root,"qr_number");
//        if(pCJSONFunCode)
//        {
//           CCommonData::getInstance()->m_CGetQrPayInfo.qr_number =pCJSONFunCode->valueint;
//            //CCommonData::getInstance()->m_CGetQrPayInfo.qr_number = CCommonData::getInstance()->m_CGetQrPayInfo.qr_number.remove("|");
//            printf(">>>>>>>>>>>>>>>>>qr_number = %d<<<<<<<",CCommonData::getInstance()->m_CGetQrPayInfo.qr_number);
//        }

        emit signal_paycard_record_widget(CCommonData::getInstance()->m_CGetQrPayInfo);

     }while(0);
}

typedef struct picture_manage_s{
    int number;
    char path[512];
    int size;
    char status;
    unsigned char *data;
}picture_manage_t;

void CMainPage::gui_picture_play(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"addr");
        if(pCJSONFunCode)
        {
            picture_manage_t *pData = (picture_manage_t *)pCJSONFunCode->valueint;

            //qDebug() << "gui_picture_play() >>>>>>>>>>>>>>1 size:" << pData->size;
            CCommonData::getInstance()->m_CPlayPicture.m_image.loadFromData((const uchar *)pData->data,pData->size);
            emit signal_main_page2_picture(CCommonData::getInstance()->m_CPlayPicture);
#if 0
            if(!CCommonData::getInstance()->m_play_picture_list.contains(pData->number))
            {
                CPlayPicture picture_data;

                QFile file(pData->path);

                if(!file.open(QIODevice::ReadOnly))
                {
                    break;
                }
                QByteArray pictureContent = file.read(file.size());
                file.close();
                picture_data.m_image.loadFromData(pictureContent);

                CCommonData::getInstance()->m_play_picture_list[pData->number] = picture_data;
                qDebug() << "gui_picture_play()<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<new:" << QString::number(pData->number);
            }
            emit signal_main_page2_picture(CCommonData::getInstance()->m_play_picture_list[pData->number]);
#endif
        }

    }while(0);
}

typedef enum __qrcode_state__
{
    QRCODE_IDENTIFY_SUCCESS=1,
    QRCODE_DEST_STA_NOT_EXIST,
    QRCODE_INVALID,
    QRCODE_EXPIRE,
    QRCODE_HAD_CHECKED
}qrcode_state_e;

void CMainPage::gui_two_dimensional_code_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;

    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"identify_flag");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        if(QRCODE_IDENTIFY_SUCCESS == pCJSONFunCode->valueint)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"money");
            if(pCJSONFunCode)
            {
                //main_page_notify(kEnumMainPageTicketPrice,pCJSONFunCode->valueint,0);   //票价

                CEventData eventData;

                eventData.m_str_data = QString::number(pCJSONFunCode->valuedouble) + tr("元");
                main_page_notify(kEnumMainPageAmount,0,&eventData);   //金额

                eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_last_station;
                main_page_notify(kEnumMainPageUpdateStationName1,0,&eventData);

            }

            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumQrStatusSuccess,0);

        }
        else
        {
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumQrStatusabNormal,0);

        }
        if(MainWindow::kEnumMainWindow == MainWindow::getInstance()->get_current_widget_id())
        {
            if(!MainWindow::getInstance()->isVisible())
            {
                 emit signal_mainwindow_show(true);
                 CProtocol::getInstance()->call_json_system_passenger_flow(7,2,0);
            }
        }
        else
        {
            emit signal_mainwindow_show(true);
        }


     }while(0);
}

void CMainPage::gui_login_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"use_dynamic");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CUserLoginInfo.use_dynamic = pCJSONFunCode->valueint;

        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"use_sd_data");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CUserLoginInfo.use_sd_data = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"admin_password");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            CCommonData::getInstance()->m_CUserLoginInfo.admin_password = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"sd_format_password");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            CCommonData::getInstance()->m_CUserLoginInfo.sd_format_password = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"sd_admin_password");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            CCommonData::getInstance()->m_CUserLoginInfo.sd_admin_password = pCJSONFunCode->valuestring;
        }

    }while(0);
}

void CMainPage::gui_system_bus_interval(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        CEventData eventData;

        pCJSONFunCode = cJSON_GetObjectItem(root,"prev_bus");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            eventData.m_str_data = gb2312ToUtf8(pCJSONFunCode->valuestring);
            main_page_notify(kEnumMainPageVehicle3,kEnumVehicleStationBack,&eventData); //前车
        }

        pCJSONFunCode = cJSON_GetObjectItem(root,"next_bus");
        if(pCJSONFunCode && pCJSONFunCode->valuestring)
        {
            eventData.m_str_data = gb2312ToUtf8(pCJSONFunCode->valuestring);
            main_page_notify(kEnumMainPageVehicle1,kEnumVehicleStationBack,&eventData);  //后车
        }

    }while(0);
}


void CMainPage::gui_sdcard_format_progress(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;

    do
    {
        SystemProgressInfo systemProgressInfo;

        pCJSONFunCode = cJSON_GetObjectItem(root,"total_size");
        if(pCJSONFunCode)
        {
            systemProgressInfo.total_size = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cur_size");
        if(pCJSONFunCode)
        {
            systemProgressInfo.current_size = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {//0 conitue; 1 finish; -1 failed.
            systemProgressInfo.status = pCJSONFunCode->valueint;
        }
        emit signal_storage_manage_widget_event_ex(0,&systemProgressInfo);

     }while(0);
}

void CMainPage::gui_passenger_flow_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;

    do
    {

        pCJSONFunCode = cJSON_GetObjectItem(root,"passenger_sum");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CPassengerInfo.passenger_sum = pCJSONFunCode->valueint;
            main_page_notify(kEnumMainPageWithinVehicleNumber,pCJSONFunCode->valueint,0);   //车内人数
            CCommonData::getInstance()->m_CGetPassengerInfo.within = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"passenger_up");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CPassengerInfo.passenger_up = pCJSONFunCode->valueint;
            main_page_notify(kEnumMainPageUpVehicleNumber,pCJSONFunCode->valueint,0);       //上车人数
            CCommonData::getInstance()->m_CGetPassengerInfo.up = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"passenger_down");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CPassengerInfo.passenger_down = pCJSONFunCode->valueint;
            main_page_notify(kEnumMainPageDownVehicleNumber,pCJSONFunCode->valueint,0);      //下车人数
            CCommonData::getInstance()->m_CGetPassengerInfo.down = pCJSONFunCode->valueint;
        }

        emit signal_passenger_widget(CCommonData::getInstance()->m_CGetPassengerInfo);

        emit signal_passenger_info(CCommonData::getInstance()->m_CPassengerInfo);

        main_page_notify(kEnumMainPageUpVehicleNumber,CCommonData::getInstance()->m_CPassengerInfo.passenger_up,0);       //上车人数
        main_page_notify(kEnumMainPageDownVehicleNumber,CCommonData::getInstance()->m_CPassengerInfo.passenger_down,0);      //下车人数
        main_page_notify(kEnumMainPageWithinVehicleNumber,CCommonData::getInstance()->m_CPassengerInfo.passenger_sum,0);   //车内人数


     }while(0);
}

void CMainPage::gui_system_passenger_flow(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");

        if(NULL == pCJSONFunCode)
        {
            break;
        }

        if(1 == pCJSONFunCode->valueint)
        {
            /*CAM1-1通道设置*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_enable");
            if(pCJSONFunCode)
            {/*使能  0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_id");
            if(pCJSONFunCode)
            {/*ID*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_id  = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_guide");
            if(pCJSONFunCode)
            {/*辅助线 0:不选择 1:选择*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.guide  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_count_show_mode");
            if(pCJSONFunCode)
            {/*计数显示模式  0:标准  1:进站*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_show_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_count_rule");
            if(pCJSONFunCode)
            {/*计数规则  //0:上进下出  1:下进上出*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_rule  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_noise_filter");
            if(pCJSONFunCode)
            {/*噪声过滤*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.noise_filter = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_sensitivity");
            if(pCJSONFunCode)
            {/*灵敏度*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.sensitivity  = pCJSONFunCode->valuedouble;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_count_type");
            if(pCJSONFunCode)
            {/*计数类型*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_install_height");
            if(pCJSONFunCode)
            {/*安装高度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.install_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_filter_height");
            if(pCJSONFunCode)
            {/*过滤高度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.filter_height  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_count_line1");
            if(pCJSONFunCode)
            {/*上计数线1*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line1  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_count_line2");
            if(pCJSONFunCode)
            {/*下计数线2*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_count_line3");
            if(pCJSONFunCode)
            {/*左边界线3*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line3  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_count_line4");
            if(pCJSONFunCode)
            {/*右边界线4*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line4 = pCJSONFunCode->valueint;
            }
            //CAM2-1通道设置
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_enable");
            if(pCJSONFunCode)
            {/*使能  0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_id");
            if(pCJSONFunCode)
            {/*ID*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_id  = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_guide");
            if(pCJSONFunCode)
            {/*辅助线 0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.guide = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_count_show_mode");
            if(pCJSONFunCode)
            {/*计数显示模式  0:标准  1:进站*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_show_mode  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_count_rule");
            if(pCJSONFunCode)
            {/*计数规则  //0:上进下出  1:下进上出*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_rule  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_noise_filter");
            if(pCJSONFunCode)
            {/*噪声过滤*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.noise_filter = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_sensitivity");
            if(pCJSONFunCode)
            {/*灵敏度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.sensitivity = pCJSONFunCode->valuedouble;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_count_type");
            if(pCJSONFunCode)
            {/*计数类型*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_install_height");
            if(pCJSONFunCode)
            {/*安装高度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.install_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_filter_height");
            if(pCJSONFunCode)
            {/*过滤高度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.filter_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_count_line1");
            if(pCJSONFunCode)
            {/*上计数线1*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line1  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_count_line2");
            if(pCJSONFunCode)
            {/*下计数线2*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_count_line3");
            if(pCJSONFunCode)
            {/*左边界线3*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line3  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_count_line4");
            if(pCJSONFunCode)
            {/*右边界线4*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line4 = pCJSONFunCode->valueint;
            }
            //CAM3-1通道设置----------------------------------------------------------
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_enable");
            if(pCJSONFunCode)
            {/*使能  0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_id");
            if(pCJSONFunCode)
            {/*ID*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_id  = pCJSONFunCode->valuedouble;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_guide");
            if(pCJSONFunCode)
            {/*辅助线 0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.guide = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_count_show_mode");
            if(pCJSONFunCode)
            {/*计数显示模式  0:标准  1:进站*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_show_mode  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_count_rule");
            if(pCJSONFunCode)
            {/*计数规则  //0:上进下出  1:下进上出*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_rule  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_noise_filter");
            if(pCJSONFunCode)
            {/*噪声过滤*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.noise_filter = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_sensitivity");
            if(pCJSONFunCode)
            {/*灵敏度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.sensitivity = pCJSONFunCode->valuedouble;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_count_type");
            if(pCJSONFunCode)
            {/*计数类型*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_install_height");
            if(pCJSONFunCode)
            {/*安装高度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.install_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_filter_height");
            if(pCJSONFunCode)
            {/*过滤高度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.filter_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_count_line1");
            if(pCJSONFunCode)
            {/*上计数线1*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line1  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_count_line2");
            if(pCJSONFunCode)
            {/*下计数线2*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_count_line3");
            if(pCJSONFunCode)
            {/*左边界线3*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line3  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_count_line4");
            if(pCJSONFunCode)
            {/*右边界线4*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line4 = pCJSONFunCode->valueint;
            }
            //CAM4-1通道设置----------------------------------------------------------
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_enable");
            if(pCJSONFunCode)
            {/*使能  0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_id");
            if(pCJSONFunCode)
            {/*ID*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_id  = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_guide");
            if(pCJSONFunCode)
            {/*辅助线 0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.guide = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_count_show_mode");
            if(pCJSONFunCode)
            {/*计数显示模式  0:标准  1:进站*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_show_mode  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_count_rule");
            if(pCJSONFunCode)
            {/*计数规则  //0:上进下出  1:下进上出*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_rule  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_noise_filter");
            if(pCJSONFunCode)
            {/*噪声过滤*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.noise_filter = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_sensitivity");
            if(pCJSONFunCode)
            {/*灵敏度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.sensitivity = pCJSONFunCode->valuedouble;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_count_type");
            if(pCJSONFunCode)
            {/*计数类型*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_install_height");
            if(pCJSONFunCode)
            {/*安装高度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.install_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_filter_height");
            if(pCJSONFunCode)
            {/*过滤高度*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.filter_height = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_count_line1");
            if(pCJSONFunCode)
            {/*上计数线1*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line1  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_count_line2");
            if(pCJSONFunCode)
            {/*下计数线2*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_count_line3");
            if(pCJSONFunCode)
            {/*左边界线3*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line3  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_count_line4");
            if(pCJSONFunCode)
            {/*右边界线4*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line4 = pCJSONFunCode->valueint;
            }
            /*高级设置－计数使能*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"real_dynamic_statistics");
            if(pCJSONFunCode)
            {/*实时动态统计  0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.real_dynamic_statistics = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"open_door_statistics");
            if(pCJSONFunCode)
            {/*开门统计  0:不选择 1:选择*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.open_door_statistics  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"in_station_statistics");
            if(pCJSONFunCode)
            {/*进站统计  0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.in_station_statistics = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"out_station_statistics");
            if(pCJSONFunCode)
            {/*站外统计  0:不选择 1:选择*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.out_station_statistics  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"speed_statistics_enable");
            if(pCJSONFunCode)
            {/*速度统计使能  0:不选择 1:选择*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.speed_statistics_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"speed_statistics_value");
            if(pCJSONFunCode)
            {/*速度统计值*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.speed_statistics_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"single_loop_error_calibration");
            if(pCJSONFunCode)
            {/*单圈误差校准*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.single_loop_error_calibration = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"close_delay");
            if(pCJSONFunCode)
            {/*关门延时*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.close_delay  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"time_upload_time");
            if(pCJSONFunCode)
            {/*定时上报时间*/
                CCommonData::getInstance()->m_CSystemPassengerFlow.time_upload_time = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"image_mosaic");
            if(pCJSONFunCode)
            {/*图像拼接　0:不选择 1:选择*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.image_mosaic  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"interval_width");
            if(pCJSONFunCode)
            {/*间隔宽度　CM*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.interval_width  = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_is_connect");
            if(pCJSONFunCode)
            {/*1:连接 2:未连接*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.is_connect = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_param_file_exist");
            if(pCJSONFunCode)
            {/*参数文件 1:存在 2:不存在*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.param_file_exist = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_is_connect");
            if(pCJSONFunCode)
            {/*1:连接 2:未连接*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.is_connect = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_param_file_exist");
            if(pCJSONFunCode)
            {/*参数文件 1:存在 2:不存在*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.param_file_exist = pCJSONFunCode->valueint;
            }
            emit signal_passenger_flow_widget_event(1,&CCommonData::getInstance()->m_CSystemPassengerFlow);
        }
        else if(2 == pCJSONFunCode->valueint)
        {//同步操作结果

            CPassengerResult *pPassengerResult = new CPassengerResult;

            if(NULL == pPassengerResult)
            {
                break;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"return_type");
            if(pCJSONFunCode)
            {
                pPassengerResult->m_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"return_result");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {
                pPassengerResult->m_return_result = gb2312ToUtf8(pCJSONFunCode->valuestring);
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"value");
            if(pCJSONFunCode)
            {
                pPassengerResult->m_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam_select");
            if(pCJSONFunCode)
            {
                pPassengerResult->m_cam_select = pCJSONFunCode->valueint;
            }

            emit signal_passenger_flow_widget_event(2,pPassengerResult);
            delete pPassengerResult;

        }
        else if(3 == pCJSONFunCode->valueint)
        {
            CPassengerResult *pPassengerResult = new CPassengerResult;

            if(NULL == pPassengerResult)
            {
                break;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"return_type");
            if(pCJSONFunCode)
            {
                pPassengerResult->m_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"return_result");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {
                pPassengerResult->m_return_result = gb2312ToUtf8(pCJSONFunCode->valuestring);
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"value");
            if(pCJSONFunCode)
            {
                pPassengerResult->m_value = pCJSONFunCode->valueint;
            }

            emit signal_passenger_flow_widget_event(3,pPassengerResult);
            delete pPassengerResult;
        }
        else if(4 == pCJSONFunCode->valueint)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_id");
            if(pCJSONFunCode)
            {/*ID*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_id  = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_id");
            if(pCJSONFunCode)
            {/*ID*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_id  = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_id");
            if(pCJSONFunCode)
            {/*ID*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_id  = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_id");
            if(pCJSONFunCode)
            {/*ID*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_id  = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_is_connect");
            if(pCJSONFunCode)
            {/*1:连接 2:未连接*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.is_connect = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_is_connect");
            if(pCJSONFunCode)
            {/*1:连接 2:未连接*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.is_connect = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_is_connect");
            if(pCJSONFunCode)
            {/*1:连接 2:未连接*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.is_connect = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_is_connect");
            if(pCJSONFunCode)
            {/*1:连接 2:未连接*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.is_connect = pCJSONFunCode->valueint;
            }
            emit signal_passenger_flow_widget_event(4,&CCommonData::getInstance()->m_CSystemPassengerFlow);
        }
        else if(5 == pCJSONFunCode->valueint)
        {

            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_param_file_exist");
            if(pCJSONFunCode)
            {/*参数文件 1:存在 2:不存在*/

               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.param_file_exist = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_param_file_exist");
            if(pCJSONFunCode)
            {/*参数文件 1:存在 2:不存在*/

               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.param_file_exist = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_param_file_exist");
            if(pCJSONFunCode)
            {/*参数文件 1:存在 2:不存在*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.param_file_exist = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam4_1_param_file_exist");
            if(pCJSONFunCode)
            {/*参数文件 1:存在 2:不存在*/
               CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.param_file_exist = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_param_file_name");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.param_file_name = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_param_file_name");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.param_file_name = pCJSONFunCode->valuestring;
            }

            emit signal_passenger_flow_widget_event(5,&CCommonData::getInstance()->m_CSystemPassengerFlow);

        }
        else if(6 == pCJSONFunCode->valueint)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"return_result");
            if(pCJSONFunCode)
            {



                CMyMessageBox::getInstance()->set_content(tr("温馨提示"),gb2312ToUtf8(pCJSONFunCode->valuestring),CLtyString::getInstance()->m_confirm,"");
                CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
                CMyMessageBox::getInstance()->exec();
            }
        }


    }while(0);
}


void CMainPage::gui_system_camera_pon_cam(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {

        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(1 == pCJSONFunCode->valueint)
        {
            /*PON*/
            cJSON *pJson_array = cJSON_GetObjectItem(root,"pon_rows");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*数组大小*/
                tasklist=pJson_array->child;/*子对象*/
            }
            CSystemPonInfo *pTemp = NULL;
            for(int index = 0;index < array_size;index++)  /*共有10个pon*/
            {
                pTemp = NULL;

                if(0 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon1;
                else if(1 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon2;
                else if(2 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon3;
                else if(3 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon4;
                else if(4 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon5;
                else if(5 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon6;
                else if(6 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon7;
                else if(7 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon8;
                else if(8 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon9;
                else if(9 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon10;

                if(NULL == pTemp)
                    continue;

                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"config_ip");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {/*IP地址*/
                    pTemp->config_ip = pCJSONFunCode->valuestring;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"config_sound_recording");
                if(pCJSONFunCode)
                {/*录音  0:开启　　1:关闭*/
                    pTemp->config_sound_recording = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"resolving_power_main");
                if(pCJSONFunCode)
                {/*分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    pTemp->resolving_power_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"resolving_power_sub");
                if(pCJSONFunCode)
                {/*分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    pTemp->resolving_power_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"frame_rate_main");
                if(pCJSONFunCode)
                {/*帧率 主 0~24 对应值是 1~25*/
                    pTemp->frame_rate_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"frame_rate_sub");
                if(pCJSONFunCode)
                {/*帧率 子 0~24 对应值是 1~25*/
                    pTemp->frame_rate_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"picture_main");
                if(pCJSONFunCode)
                {/*画质　主 0:高 1:中 2:低*/
                    pTemp->picture_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"picture_sub");
                if(pCJSONFunCode)
                {/*画质　子 0:高 1:中 2:低*/
                    pTemp->picture_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"identification");
                if(pCJSONFunCode)
                {/*标识 0:IPC摄像机 1:其他设备*/
                    pTemp->identification = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"image");
                if(pCJSONFunCode)
                {
                    pTemp->m_image = pCJSONFunCode->valueint;
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
                array_size = cJSON_GetArraySize(pJson_array);/*数组大小*/
                tasklist=pJson_array->child;/*子对象*/
            }
            CSystemCamInfo *pCamInfoTemp = NULL;

            for(int index = 0;index < array_size;index++)  /*共有6个cam*/
            {
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


                if(NULL == pCamInfoTemp)
                    continue;

                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"config_sound_recording");
                if(pCJSONFunCode)
                {/*录音  0:开启　　1:关闭*/
                    pCamInfoTemp->config_sound_recording = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"resolving_power_main");
                if(pCJSONFunCode)
                {/*分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    pCamInfoTemp->resolving_power_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"resolving_power_sub");
                if(pCJSONFunCode)
                {/*分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    pCamInfoTemp->resolving_power_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"frame_rate_main");
                if(pCJSONFunCode)
                {/*帧率 主 0~24 对应值是 1~25*/
                    pCamInfoTemp->frame_rate_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"frame_rate_sub");
                if(pCJSONFunCode)
                {/*帧率 子 0~24 对应值是 1~25*/
                    pCamInfoTemp->frame_rate_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"picture_main");
                if(pCJSONFunCode)
                {/*画质　主 0:高 1:中 2:低*/
                    pCamInfoTemp->picture_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"picture_sub");
                if(pCJSONFunCode)
                {/*画质　子 0:高 1:中 2:低*/
                    pCamInfoTemp->picture_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"identification");
                if(pCJSONFunCode)
                {/*标识 0:视频录像 1:客流录像*/
                    pCamInfoTemp->identification = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"image");
                if(pCJSONFunCode)
                {
                    pCamInfoTemp->m_image = pCJSONFunCode->valueint;
                }

                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }
            emit signal_camera_config_widget_notify(1,0,0);
        }
        else if(2 == pCJSONFunCode->valueint)
        {

            pCJSONFunCode = cJSON_GetObjectItem(root,"cam_type");
            if(NULL == pCJSONFunCode)
            {
                break;
            }
            int cam_type = pCJSONFunCode->valueint;
            pCJSONFunCode = cJSON_GetObjectItem(root,"number");
            if(NULL == pCJSONFunCode)
            {
                break;
            }
            int number = pCJSONFunCode->valueint;

            if(1 == pCJSONFunCode->valueint)
            {
                CSystemPonInfo *pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon_default;

                pCJSONFunCode = cJSON_GetObjectItem(root,"config_ip");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {/*IP地址*/
                    pTemp->config_ip = pCJSONFunCode->valuestring;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"config_sound_recording");
                if(pCJSONFunCode)
                {/*录音  0:开启　　1:关闭*/
                    pTemp->config_sound_recording = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"resolving_power_main");
                if(pCJSONFunCode)
                {/*分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    pTemp->resolving_power_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"resolving_power_sub");
                if(pCJSONFunCode)
                {/*分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    pTemp->resolving_power_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"frame_rate_main");
                if(pCJSONFunCode)
                {/*帧率 主 0~24 对应值是 1~25*/
                    pTemp->frame_rate_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"frame_rate_sub");
                if(pCJSONFunCode)
                {/*帧率 子 0~24 对应值是 1~25*/
                    pTemp->frame_rate_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"picture_main");
                if(pCJSONFunCode)
                {/*画质　主 0:高 1:中 2:低*/
                    pTemp->picture_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"picture_sub");
                if(pCJSONFunCode)
                {/*画质　子 0:高 1:中 2:低*/
                    pTemp->picture_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"identification");
                if(pCJSONFunCode)
                {/*标识 0:IPC摄像机 1:其他设备*/
                    pTemp->identification = pCJSONFunCode->valueint;
                }
            }
            else if(2 == pCJSONFunCode->valueint)
            {

                CSystemCamInfo *pCamInfoTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam_default;

                pCJSONFunCode = cJSON_GetObjectItem(root,"config_sound_recording");
                if(pCJSONFunCode)
                {/*录音  0:开启　　1:关闭*/
                    pCamInfoTemp->config_sound_recording = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"resolving_power_main");
                if(pCJSONFunCode)
                {/*分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    pCamInfoTemp->resolving_power_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"resolving_power_sub");
                if(pCJSONFunCode)
                {/*分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF*/
                    pCamInfoTemp->resolving_power_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"frame_rate_main");
                if(pCJSONFunCode)
                {/*帧率 主 0~24 对应值是 1~25*/
                    pCamInfoTemp->frame_rate_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"frame_rate_sub");
                if(pCJSONFunCode)
                {/*帧率 子 0~24 对应值是 1~25*/
                    pCamInfoTemp->frame_rate_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"picture_main");
                if(pCJSONFunCode)
                {/*画质　主 0:高 1:中 2:低*/
                    pCamInfoTemp->picture_main = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"picture_sub");
                if(pCJSONFunCode)
                {/*画质　子 0:高 1:中 2:低*/
                    pCamInfoTemp->picture_sub = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"identification");
                if(pCJSONFunCode)
                {/*标识 0:视频录像 1:客流录像*/
                    pCamInfoTemp->identification = pCJSONFunCode->valueint;
                }
            }
            emit signal_camera_config_widget_notify(2,cam_type,number);

        }
        else if(3 == pCJSONFunCode->valueint)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"cam_type");
            if(NULL == pCJSONFunCode)
            {
                break;
            }
            int cam_type = pCJSONFunCode->valueint;
            pCJSONFunCode = cJSON_GetObjectItem(root,"number");
            if(NULL == pCJSONFunCode)
            {
                break;
            }
            int number = pCJSONFunCode->valueint;
            emit signal_camera_config_widget_notify(1,cam_type,number);
        }



    }while(0);
}


void CMainPage::gui_system_camera_config(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        /*通用设置---------------*/
        pCJSONFunCode = cJSON_GetObjectItem(root,"videotape_standard");/*录像制式  0:PAL 1:NTSC*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.videotape_standard = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"auto_cover");/*自动覆盖 0:是  1:否*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.auto_cover = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"videotape_lock");/*录像锁定　0:1 1:2 2:3 3:4 4:5 5:6 6:7*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.videotape_lock = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"videotape_prev");/*录像预录    min*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.videotape_prev = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"network_transmission_mode");/*网传模式  0:流畅+清晰 1:非常清晰 2:清晰 3:流畅 4:非常流畅*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.network_transmission_mode = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"videotape_storage");/*录像存储器 0:EMMC+外置SD卡 1:EMMC  2:外置SD卡*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.videotape_storage = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"videotape_mode");/*录像模式  0:镜像录像  1:报警备份录像 2:子码流录像*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.videotape_mode = pCJSONFunCode->valueint;
        }
        /*配置－镜像通道*/
        cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
        int array_size = 0;
        cJSON *tasklist = NULL;
        if(NULL != pJson_array)
        {
            array_size = cJSON_GetArraySize(pJson_array);/*数组大小*/
            tasklist=pJson_array->child;/*子对象*/
        }
        CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.clear();
        for(int index = 0;index < array_size;index++)
        {

            pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back(pCJSONFunCode->valueint); /*值*/
            }

            tasklist = tasklist->next;
            if(NULL == tasklist)
            {
                break;
            }
        }
        /*显示设置---------------*/
        pCJSONFunCode = cJSON_GetObjectItem(root,"time_show_enable");/*时间显示使能*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.time_show_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"time_show_value");/*时间显示值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.time_show_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"passageway_name_enable");/*通道名称使能*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.passageway_name_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"passageway_name_value");/*通道名称值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.passageway_name_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"vehicle_number_enable");/*车辆编号使能*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.vehicle_number_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"vehicle_number_value");/*车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.vehicle_number_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"coordinate_info_enable");/*坐标信息使能*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.coordinate_info_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"coordinate_info_value");/*车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.coordinate_info_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"real_speed_enable");/*实时速度使能*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.real_speed_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"real_speed_value");/*实时速度值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.real_speed_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"station_info_enable");/*站点信息使能*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.station_info_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"station_info_value");/*站点信息值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.station_info_value = pCJSONFunCode->valueint;
        }
        /*网络摄像通道设置---------------*/
        pCJSONFunCode = cJSON_GetObjectItem(root,"network_passageway_number");/*网络摄像机使用通道数*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.network_passageway_number = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon1_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon1.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon1_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon1.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon2_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon2.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon2_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon2.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon3_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon3.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon3_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon3.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon4_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon4.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon4_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon4.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon5_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon5.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon5_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon5.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon6_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon6.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon6_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon6.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon7_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon7.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon7_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon7.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon8_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon8.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon8_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon8.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon9_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon9.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon9_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon9.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon10_ip");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon10.ip = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"pon10_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.pon10.select_value = pCJSONFunCode->valueint;
        }
        /*模拟摄像机通道设置---------------*/
        pCJSONFunCode = cJSON_GetObjectItem(root,"monitor_passageway_number");/*模拟摄像机使用通道数*/
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.monitor_passageway_number = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_1_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.cam1.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cam1_2_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.cam2.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_1_select_value");
        if(pCJSONFunCode)
        {if(pCJSONFunCode)
            CCommonData::getInstance()->m_CSystemCameraConfig.cam3.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cam2_2_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.cam4.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_1_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.cam5.select_value = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"cam3_2_select_value");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemCameraConfig.cam6.select_value = pCJSONFunCode->valueint;
        }

        emit signal_camera_config_widget_event(CCommonData::getInstance()->m_CSystemCameraConfig);

    }while(0);
}
#include "class/common/my_export.h"
void CMainPage::gui_system_maintain(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        if(1 == pCJSONFunCode->valueint)
        {/*1:列表操作*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"main_type");
            if(NULL == pCJSONFunCode)
            {
                break;
            }

            char main_type = 0;
            if(1 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumApplicationProgram; //应用程序
            else if(10 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumExportSystemLog;//导出系统日志
            else if(18 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumImportPicture;//导入广告
            else if(19 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumExportPicture;//导出广告
            else if(20 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumCleanPicture;//清除广告

 #if 0
            else if(2 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumMcuProgram; //MCU程序
            else if(3 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumUBootProgram; //UBOOT程序
            else if(4 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumImportStationFile; //导入报站文件
            else if(5 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumImportLogoFile; //导入开机LOGO文件
#endif
            pCJSONFunCode = cJSON_GetObjectItem(root,"operate_type");
            if(NULL == pCJSONFunCode)
            {
                break;
            }
            CCommonData::getInstance()->m_CSystemSystemMaintain.m_operate_type = pCJSONFunCode->valueint;
            if(1 == pCJSONFunCode->valueint)
            {/*获取数据*/
                CSoftwareUpgrade *pSoftwareUpgrade = new CSoftwareUpgrade;
                pCJSONFunCode = cJSON_GetObjectItem(root,"number");
                if(pCJSONFunCode)
                {/*文件编号，从1开始*/
                    pSoftwareUpgrade->m_number = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"path");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {/*文件路径*/
                    pSoftwareUpgrade->m_file_name = pCJSONFunCode->valuestring;
                }
                CCommonData::getInstance()->m_CSystemSystemMaintain.m_param = pSoftwareUpgrade;
            }
            else if(2 == pCJSONFunCode->valueint)
            {/*获取进度*/
                pCJSONFunCode = cJSON_GetObjectItem(root,"percent");
                if(pCJSONFunCode)
                {
                    CCommonData::getInstance()->m_CSystemSystemMaintain.percent = pCJSONFunCode->valueint;
                }

            }
            else if(3 == pCJSONFunCode->valueint)
            {
               CCommonData::getInstance()->m_CSystemSystemMaintain.m_operate_type = CSystemSystemMaintain::kEnumSuccess;
            }
            else if(4 == pCJSONFunCode->valueint)
            {/*操作失败*/
               CCommonData::getInstance()->m_CSystemSystemMaintain.m_operate_type = CSystemSystemMaintain::kEnumFail;
               pCJSONFunCode = cJSON_GetObjectItem(root,"result_str");
               if(pCJSONFunCode && pCJSONFunCode->valuestring)
               {
                    CCommonData::getInstance()->m_CSystemSystemMaintain.m_result = gb2312ToUtf8(pCJSONFunCode->valuestring);
               }
            }
            emit signal_system_maintain_widget_event(main_type,CCommonData::getInstance()->m_CSystemSystemMaintain);
        }
        else if(2 == pCJSONFunCode->valueint)
        {//功能操作
            pCJSONFunCode = cJSON_GetObjectItem(root,"main_type");
            if(NULL == pCJSONFunCode)
            {
                break;
            }

            char main_type = 0;
            //报站文件
            if(4 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumImportStationFile; //导入报站文件
            else if(6 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumExportStationFile;//导出报站文件
            else if(7 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumClearStationFile;//清除报站文件版本
            //日志/记录
            else if(8 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumExportVehicleRecord;//导出行车记录
            else if(9 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumClearVehicleRecord;//清除行车日志
            else if(10 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumExportSystemLog;//导出系统日志
            else if(11 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumClearSystemLog;//清除系统日志
            else if(12 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumExportOperationLog;//导出操作日志
            else if(13 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumClearOperationLog;//清除操作日志
            //高级
            else if(14 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumResumeApplicationProgram;//恢复应用程序
            else if(15 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumResumeMcuProgram;//恢复MCU程序
            else if(16 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumResumeUBootProgram;//恢复UBoot程序
            else if(17 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumResumeFactoryConfig;//恢复出厂设置
            else if(21 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumImportCardParam;//导入刷卡参数
            else if(22 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumExportCardParam;//导出刷卡参数
            else if(23 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumCleanCardParam;//清除刷卡参数
            else if(24 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumCleanCardRecord;//清除刷卡记录
            else if(25 == pCJSONFunCode->valueint)
                main_type = CSystemSystemMaintain::kEnumFormatStationFile;//格式化报站分区

            pCJSONFunCode = cJSON_GetObjectItem(root,"operate_type");
            if(NULL == pCJSONFunCode)
            {
                break;
            }
            qDebug() << "operate_type:" << QString::number(pCJSONFunCode->valueint);
            if(5 == pCJSONFunCode->valueint)
                CCommonData::getInstance()->m_CSystemSystemMaintain.m_operate_type = CSystemSystemMaintain::kEnumOperate;
            else if(3 == pCJSONFunCode->valueint)
            {
                CCommonData::getInstance()->m_CSystemSystemMaintain.m_operate_type = CSystemSystemMaintain::kEnumSuccess;

                if(!CMyExport::getInstance()->isVisible())
                {
                    CEventData eventData;
                    eventData.m_i_data = 5;
                    eventData.m_param1 = 1;
                    eventData.m_str_data = tr("操作成功!");
                    main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusMessage,&eventData);
                }

            }
            else if(4 == pCJSONFunCode->valueint)
            {
                CCommonData::getInstance()->m_CSystemSystemMaintain.m_operate_type = CSystemSystemMaintain::kEnumFail;
               pCJSONFunCode = cJSON_GetObjectItem(root,"result_str");
               if(pCJSONFunCode && pCJSONFunCode->valuestring)
               {
                    CCommonData::getInstance()->m_CSystemSystemMaintain.m_result = gb2312ToUtf8(pCJSONFunCode->valuestring);

               }
               if(!CMyExport::getInstance()->isVisible())
               {
                    CEventData eventData;
                    eventData.m_i_data = 5;
                    eventData.m_param1 = 1;
                    eventData.m_str_data = tr("失败:\n") + CCommonData::getInstance()->m_CSystemSystemMaintain.m_result;
                    main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusMessage,&eventData);
               }

            }
            if(CMyExport::getInstance()->isVisible())
                emit signal_system_maintain_widget_event(main_type,CCommonData::getInstance()->m_CSystemSystemMaintain);

        }

    }while(0);
}

void CMainPage::gui_system_system_setup(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
            pCJSONFunCode = cJSON_GetObjectItem(root,"screen_mode");
            if(pCJSONFunCode)
            {/*屏幕模式 0:高清模式 1:标清模式*/
               CCommonData::getInstance()->m_CSystemSystemSetup.screen_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"drusb_mode");
            if(pCJSONFunCode)
            {/*DRUSB接口模式 0:标准 1:灾备*/
               CCommonData::getInstance()->m_CSystemSystemSetup.drusb_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"sleep_shutdown_enable");
            if(pCJSONFunCode)
            {/*延时关机使能*/
               CCommonData::getInstance()->m_CSystemSystemSetup.sleep_shutdown_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"sleep_shutdown_value");
            if(pCJSONFunCode)
            {/*延时关机值　(min)*/
               CCommonData::getInstance()->m_CSystemSystemSetup.sleep_shutdown_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"device_door_report_enable");
            if(pCJSONFunCode)
            {/*设备门开启上报间隔使能*/
               CCommonData::getInstance()->m_CSystemSystemSetup.device_door_report_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"device_door_report_value");
            if(pCJSONFunCode)
            {/*设备门开启上报间隔值*/
               CCommonData::getInstance()->m_CSystemSystemSetup.device_door_report_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"hard_disk_heating_enable");
            if(pCJSONFunCode)
            {/*硬盘加热使能*/
               CCommonData::getInstance()->m_CSystemSystemSetup.hard_disk_heating_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"hard_disk_heating_value");
            if(pCJSONFunCode)
            {/*硬盘加热值*/
               CCommonData::getInstance()->m_CSystemSystemSetup.hard_disk_heating_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"now_time_enable");
            if(pCJSONFunCode)
            {/*整点报时使能*/
               CCommonData::getInstance()->m_CSystemSystemSetup.now_time_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"now_time_value1");
            if(pCJSONFunCode)
            {/*整点报时值1   h*/
               CCommonData::getInstance()->m_CSystemSystemSetup.now_time_value1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"now_time_value2");
            if(pCJSONFunCode)
            {/*整点报时值2   h*/

               CCommonData::getInstance()->m_CSystemSystemSetup.now_time_value2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"voltage_enable");
            if(pCJSONFunCode)
            {/*超电压反警使能*/
               CCommonData::getInstance()->m_CSystemSystemSetup.voltage_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"voltage_value1");
            if(pCJSONFunCode)
            {/*超电压反警值1   V*/
               CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"voltage_value2");
            if(pCJSONFunCode)
            {/*超电压反警值2   V*/
               CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"voltage_value3");
            if(pCJSONFunCode)
            {/*//超电压反警值3   min*/
               CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value3 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"temperature_enable");
            if(pCJSONFunCode)
            {/*超温度报警使能*/
               CCommonData::getInstance()->m_CSystemSystemSetup.temperature_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"temperature_value1");
            if(pCJSONFunCode)
            {/*超温度报警值1   V*/
               CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"temperature_value2");
            if(pCJSONFunCode)
            {/*超温度报警值2   V*/
               CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"temperature_value3");
            if(pCJSONFunCode)
            {/*超温度报警值3   min*/
               CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value3 = pCJSONFunCode->valueint;
            }
            //add
            pCJSONFunCode = cJSON_GetObjectItem(root,"fan_control_enable");
            if(pCJSONFunCode)
            {//风扇控制使能
               CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"fan_control_value1");
            if(pCJSONFunCode)
            {//风扇控制值1
               CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_value1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"fan_control_value2");
            if(pCJSONFunCode)
            {//风扇控制值2
               CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_value2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"picutre_play_time");
            if(pCJSONFunCode)
            {//图片轮播
               CCommonData::getInstance()->m_CSystemSystemSetup.picutre_play_time = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"volumn_size");
            if(pCJSONFunCode)
            {//声音大小
               CCommonData::getInstance()->m_CSystemSystemSetup.volumn_size = pCJSONFunCode->valueint;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"current_time_zone");
            if(pCJSONFunCode)
            {/*//当前选择的时区*/
               CCommonData::getInstance()->m_CSystemSystemSetup.m_current_time_zone = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"time1");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*时间1*/
               CCommonData::getInstance()->m_CSystemSystemSetup.m_time1 = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"time2");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*时间2*/
               CCommonData::getInstance()->m_CSystemSystemSetup.m_time2 = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"satellite_time_enable");
            if(pCJSONFunCode)
            {/*卫星校时  0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSystemSetup.satellite_time_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_time_enable");
            if(pCJSONFunCode)
            {/*中心校时  0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSystemSetup.center_time_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"ntp_enable");
            if(pCJSONFunCode)
            {/*/NTP校时使能  0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSystemSetup.ntp_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"ntp_value");
            if(pCJSONFunCode)
            {/*NTP校时值 0:time.windows.com 1:time.nist.gov 2:time-nw.nist.gov 3:time-a.nist.gov 4:time-b.nist.gov*/
               CCommonData::getInstance()->m_CSystemSystemSetup.ntp_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"use_password");
            if(pCJSONFunCode)
            {/*使用密码  0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSystemSetup.use_password = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"auto_lock_enable");
            if(pCJSONFunCode)
            {/*自动锁定使能  0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSystemSetup.auto_lock_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"auto_lock_value");
            if(pCJSONFunCode)
            {/*自动锁定值(min)  0:1  1:2  2:3  3:5  4:10*/
               CCommonData::getInstance()->m_CSystemSystemSetup.auto_lock_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"timeout_exit_enable");
            if(pCJSONFunCode)
            {/*//超时退出使能  0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSystemSetup.timeout_exit_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"timeout_exit_value");
            if(pCJSONFunCode)
            {/*超时退出值(min) 0:1  1:2  2:3  3:5  4:10*/
               CCommonData::getInstance()->m_CSystemSystemSetup.timeout_exit_value = pCJSONFunCode->valueint;
            }

             cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*数组大小*/
                tasklist=pJson_array->child;/*子对象*/
            }
            CCommonData::getInstance()->m_CSystemSystemSetup.m_time_zone_list.clear();
            for(int index = 0;index < array_size;index++)
            {
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {
                    CCommonData::getInstance()->m_CSystemSystemSetup.m_time_zone_list.push_back(pCJSONFunCode->valuestring);
                }
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }



            emit signal_system_setup_widget_event(CCommonData::getInstance()->m_CSystemSystemSetup);

    }while(0);
}

void CMainPage::gui_system_senior_setup(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(NULL == pCJSONFunCode)
        {
            break;
        }
        if(1 == pCJSONFunCode->valueint)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"location_mode");
            if(pCJSONFunCode)
            {/*//定位方式 0:混合定位 1:北斗定位 2:GPS定位*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.location_mode = pCJSONFunCode->valueint;
            }
             pCJSONFunCode = cJSON_GetObjectItem(root,"modular_mode");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//模块型号
               CCommonData::getInstance()->m_CSystemSeniorSetup.modular_mode = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"software_version");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//软件版本
               CCommonData::getInstance()->m_CSystemSeniorSetup.software_version = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"sn_number");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {//S/N号
               CCommonData::getInstance()->m_CSystemSeniorSetup.sn_number = pCJSONFunCode->valuestring;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"custom_navigation_enable");
            if(pCJSONFunCode)
            {/*惯性导航使能*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.custom_navigation_enable = pCJSONFunCode->valueint;
            }
            /*惯性导航-配置(start)*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"corner_x");
            if(pCJSONFunCode)
            {/*设备X轴相对车X轴转角*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.corner_x = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"corner_y");
            if(pCJSONFunCode)
            {/*设备Y轴相对车Y轴转角*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.corner_y = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"corner_z");
            if(pCJSONFunCode)
            {/*设备Z轴相对车Z轴转角*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.corner_z = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"corner_gain");
            if(pCJSONFunCode)
            {/*转弯Gain值*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.corner_gain = pCJSONFunCode->valueint;
            }
            /*惯性导航-配置(end)*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"station_mode");
            if(pCJSONFunCode)
            {/*//报站方式 0:自动 1:手动*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.m_station_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"in_station_radius");
            if(pCJSONFunCode)
            {/*进站半径*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.in_station_radius = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"out_station_radius");
            if(pCJSONFunCode)
            {/*出站半径*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.out_station_radius = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"inflection_point_radius");
            if(pCJSONFunCode)
            {/*拐点半径*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.inflection_point_radius = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"over_speed_still_tip");
            if(pCJSONFunCode)
            {/*超速保持提醒*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.over_speed_still_tip = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"over_speed_tip_times");
            if(pCJSONFunCode)
            {/*超速提醒次数*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.over_speed_tip_times = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"non_operating_speed_limit");
            if(pCJSONFunCode)
            {/*非运营限速*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.non_operating_speed_limit = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"operating_mode");
            if(pCJSONFunCode)
            {//0:营运　1:非营运
               CCommonData::getInstance()->m_CSystemSeniorSetup.operating_mode = pCJSONFunCode->valueint;
            }
            /*告警使能*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"urgent_acceleration_enable");
            if(pCJSONFunCode)
            {/*紧急加速使能 // 0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.urgent_acceleration_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"urgent_acceleration_value");
            if(pCJSONFunCode)
            {/*紧急加速值  (km/s)*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.urgent_acceleration_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"emergency_deceleration_enable");
            if(pCJSONFunCode)
            {/*紧急减速使能 // 0:未选择状态 1:选中状态*/
                CCommonData::getInstance()->m_CSystemSeniorSetup.emergency_deceleration_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"emergency_deceleration_value");
            if(pCJSONFunCode)
            {/*紧急减速值  (km/s)*/
                CCommonData::getInstance()->m_CSystemSeniorSetup.emergency_deceleration_value =pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"hysteresis_alarm_enable");
            if(pCJSONFunCode)
            {/*滞站报警使能 // 0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.hysteresis_alarm_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"hysteresis_alarm_value");
            if(pCJSONFunCode)
            {/*滞站报警值*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.hysteresis_alarm_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"skip_alarm_enable");
            if(pCJSONFunCode)
            {/*越站报警使能 // 0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.skip_alarm_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"skip_alarm_value");
            if(pCJSONFunCode)
            {/*越站报警值*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.skip_alarm_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"fatigue_driving_check_enable");
            if(pCJSONFunCode)
            {/*疲劳驾驶检测使能 // 0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.fatigue_driving_check_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"fatigue_driving_check_value");
            if(pCJSONFunCode)
            {/*疲劳驾驶检测值*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.fatigue_driving_check_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"idling_enable");
            if(pCJSONFunCode)
            {/*怠速使能使能 // 0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.idling_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"idling_value");
            if(pCJSONFunCode)
            {/*怠速使能值*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value = pCJSONFunCode->valueint;
            }
            /*CAN设置*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"can1_enable");
            if(pCJSONFunCode)
            {/*CAN1  0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.can1_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"can2_enable");
            if(pCJSONFunCode)
            {/*CAN2  0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.can2_enable = pCJSONFunCode->valueint;
            }
            /*高级设置*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"volumn_adjustment");
            if(pCJSONFunCode)
            {/*音量调整使能 0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.volumn_adjustment = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"idling_enable1");
            if(pCJSONFunCode)
            {/*怠速使能 0:未选择状态 1:选中状态*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.idling_enable1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"idling_value1");
            if(pCJSONFunCode)
            {/*怠速值1*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"idling_value2");
            if(pCJSONFunCode)
            {/*怠速值2*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"temperature_check_enable");
            if(pCJSONFunCode)
            {/*温度检测使能*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.temperature_check_enable = pCJSONFunCode->valueint;
            }
            /*温度检测-配置*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"in_temperature_security1");
            if(pCJSONFunCode)
            {/*内温安全范围1*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.in_temperature_security1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"in_temperature_security2");
            if(pCJSONFunCode)
            {/*内温安全范围2*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.in_temperature_security2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"out_temperature_security1");
            if(pCJSONFunCode)
            {/*外温安全范围1*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.out_temperature_security1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"out_temperature_security2");
            if(pCJSONFunCode)
            {/*外温安全范围2*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.out_temperature_security2 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"police_upload_time");
            if(pCJSONFunCode)
            {/*报警上传时间间隔*/
               CCommonData::getInstance()->m_CSystemSeniorSetup.police_upload_time = pCJSONFunCode->valueint;
            }
            emit signal_senior_setup_widget_event(1,CCommonData::getInstance()->m_CSystemSeniorSetup);
        }
        else if(2 == pCJSONFunCode->valueint)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"result");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CCanInfo1.m_result = pCJSONFunCode->valuestring;
            }
            emit signal_senior_setup_widget_event(2,CCommonData::getInstance()->m_CSystemSeniorSetup);
        }
        else if(3 == pCJSONFunCode->valueint)
        {
            pCJSONFunCode = cJSON_GetObjectItem(root,"result");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CCanInfo2.m_result = pCJSONFunCode->valuestring;
            }
            emit signal_senior_setup_widget_event(3,CCommonData::getInstance()->m_CSystemSeniorSetup);
        }

    }while(0);
}

void CMainPage::gui_system_switch_config(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        /*车门检测*/
        pCJSONFunCode = cJSON_GetObjectItem(root,"enable");
        if(pCJSONFunCode)
        {/* 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"car_door_number");
        if(pCJSONFunCode)
        {/*车门数量 0:1 1:2 2:3*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_car_door_number = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"car_door_type");
        if(pCJSONFunCode)
        {/*车门类型  0:两线气动门　1:传感器门　2:不检测*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_car_door_type = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"door_front");
        if(pCJSONFunCode)
        {/*开前门电平 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_door_front = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"door_middle");
        if(pCJSONFunCode)
        {/*开中门电平 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_door_middle = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"door_back");
        if(pCJSONFunCode)
        {/*开后门电平 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_door_back = pCJSONFunCode->valueint;
        }
        /*车门检测－配置*/
        pCJSONFunCode = cJSON_GetObjectItem(root,"speed_open_door_check");
        if(pCJSONFunCode)
        {/*带速度开门检测*/
            CCommonData::getInstance()->m_CSystemIoConfig.speed_open_door_check = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"station_open_door_check");
        if(pCJSONFunCode)
        {/*站间开门检测*/
            CCommonData::getInstance()->m_CSystemIoConfig.station_open_door_check = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"arrive_open_door_check");
        if(pCJSONFunCode)
        {/*到站未开门检测*/
            CCommonData::getInstance()->m_CSystemIoConfig.arrive_open_door_check = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"arrive_stop_door_check");
        if(pCJSONFunCode)
        {/*到站未停车检测*/
            CCommonData::getInstance()->m_CSystemIoConfig.arrive_stop_door_check = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"speed_enable");
        if(pCJSONFunCode)
        {/*异常开关门最小速度使能 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_speed_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"speed");
        if(pCJSONFunCode)
        {/*异常开关门最小速度*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_speed = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"frequency_enable");
        if(pCJSONFunCode)
        {/*异常开关门提醒使能 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_frequency_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"frequency");
        if(pCJSONFunCode)
        {/*异常开关门提醒次数*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_frequency = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"io_input1");//
        if(pCJSONFunCode)
        {/*I/O输入1 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.m_io_input1 = pCJSONFunCode->valueint;
        }

        /*其他开关量检测*/
        pCJSONFunCode = cJSON_GetObjectItem(root,"back_car_monitor_enable");
        if(pCJSONFunCode)
        {/*倒车监视 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.back_car_monitor_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"back_car_monitor_status");
        if(pCJSONFunCode)
        {/*倒车监视 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.back_car_monitor_status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"no_brake_enable");
        if(pCJSONFunCode)
        {/*未刹车 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.no_brake_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"no_brake_status");
        if(pCJSONFunCode)
        {/*未刹车 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.no_brake_status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"open_front_box_enable");
        if(pCJSONFunCode)
        {/*开前箱 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.open_front_box_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"open_front_box_status");
        if(pCJSONFunCode)
        {/*开前箱 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.open_front_box_status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"wiper_enable");
        if(pCJSONFunCode)
        {/*雨刮 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.wiper_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"wiper_status");
        if(pCJSONFunCode)
        {/*雨刮 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.wiper_status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"left_lamp_enable");
        if(pCJSONFunCode)
        {/*左转灯 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.left_lamp_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"left_lamp_status");
        if(pCJSONFunCode)
        {/*左转灯 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.left_lamp_status = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"right_lamp_enable");
        if(pCJSONFunCode)
        {/*右转灯 0:未选择状态 1:选中状态*/
            CCommonData::getInstance()->m_CSystemIoConfig.right_lamp_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"right_lamp_status");
        if(pCJSONFunCode)
        {/*右转灯 0:低电平有效　1:高电平有效*/
            CCommonData::getInstance()->m_CSystemIoConfig.right_lamp_status = pCJSONFunCode->valueint;
        }
        emit signal_switch_config_widget_event(CCommonData::getInstance()->m_CSystemIoConfig);

    }while(0);
}

void CMainPage::gui_system_wifi(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
          int operate_type = 0;
          CWifiSearch WifiSearch;

          pCJSONFunCode = cJSON_GetObjectItem(root,"type");
          if(NULL == pCJSONFunCode)
          {
            break;
          }
          operate_type =  pCJSONFunCode->valueint;
          pCJSONFunCode = cJSON_GetObjectItem(root,"number");
          if(pCJSONFunCode)
          {
            WifiSearch.m_number = pCJSONFunCode->valueint;
          }
          pCJSONFunCode = cJSON_GetObjectItem(root,"name");
          if(pCJSONFunCode && pCJSONFunCode->valuestring)
          {
            WifiSearch.m_name = pCJSONFunCode->valuestring;
          }
          pCJSONFunCode = cJSON_GetObjectItem(root,"signal");
          if(pCJSONFunCode && pCJSONFunCode->valuestring)
          {
            WifiSearch.m_signal = pCJSONFunCode->valuestring;
          }
          pCJSONFunCode = cJSON_GetObjectItem(root,"lock");
          if(pCJSONFunCode)
          {
            WifiSearch.m_lock = pCJSONFunCode->valueint;
          }
          emit signal_network_select(operate_type,WifiSearch);


    }while(0);
}

void CMainPage::gui_system_network(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        /*1:移动网络-----------------------------------------------------*/
            pCJSONFunCode = cJSON_GetObjectItem(root,"mobile_network_enable");
            if(pCJSONFunCode)
            {/*0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"mobile_network_operator1");
            if(pCJSONFunCode)
            {/*运营商  0:自动 1:手动*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->operator1 = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"mobile_network_type");
            if(pCJSONFunCode)
            {/*网络类型 0:4G 1:3G 2:2G*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->network_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"mobile_network_month_end_date");
            if(pCJSONFunCode)
            {/*月结日*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->month_end_date = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"mobile_network_use_vpn");
            if(pCJSONFunCode)
            {/*是否使用VPN  0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->use_vpn = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"mobile_network_vpn_name");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*VPN名称*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_name = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"mobile_network_vpn_account");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*VPN账号*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_account = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"mobile_network_vpn_password");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*VPN密码*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_password = pCJSONFunCode->valuestring;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"modular_name1");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*模块型号名称1 龙尚*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->modular_name1 = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"modular_name2");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*模块型号名称1 8300C*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->modular_name2 = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"modular_version");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*软件版本*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->modular_version = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"modular_imei");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*IMEI号*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->modular_imei = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"modular_sn");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*S/N号*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->modular_sn = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"modular_meid");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*MEID*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->modular_meid = pCJSONFunCode->valuestring;
            }

            CCommonData::getInstance()->m_CSystemNetwork.m_type = 1;
            emit signal_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            /*2:中心服务器1-----------------------------------------------------*/

            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_enable");
            if(pCJSONFunCode)
            {/*0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_protocol_type");
            if(pCJSONFunCode)
            {/*选中的协议类型*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->protocol_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_access_mode");
            if(pCJSONFunCode)
            {/*接入模式 0:固件IP地址 1:域名*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->access_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*IP地址或域名*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_port");
            if(pCJSONFunCode)
            {/*端口*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_port = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server1_data_send_interval");
            if(pCJSONFunCode)
            {/*数据发送间隔 (秒)*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_data_send_interval = pCJSONFunCode->valueint;
            }
#if 0
             cJSON *pJson_array1 = cJSON_GetObjectItem(root,"rows1");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array1)
            {
                array_size = cJSON_GetArraySize(pJson_array1);/*数组大小*/
                tasklist=pJson_array1->child;/*子对象*/
            }
            CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->protocol_type_list.clear();
            for(int index = 0;index < array_size;index++)
            {
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {
                    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->protocol_type_list.push_back(pCJSONFunCode->valuestring);
                }
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }
#endif

            CCommonData::getInstance()->m_CSystemNetwork.m_type = 2;
            emit signal_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);

            /*3:中心服务器2-----------------------------------------------------*/

            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_enable");
            if(pCJSONFunCode)
            {/*0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_protocol_type");
            if(pCJSONFunCode)
            {/*选中的协议类型*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->protocol_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_access_mode");
            if(pCJSONFunCode)
            {/*接入模式 0:固件IP地址 1:域名*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->access_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*IP地址或域名*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_port");
            if(pCJSONFunCode)
            {/*端口*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_port = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"center_server2_data_send_interval");
            if(pCJSONFunCode)
            {/*数据发送间隔 (秒)*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_data_send_interval = pCJSONFunCode->valueint;
            }
#if 0
             cJSON *pJson_array2 = cJSON_GetObjectItem(root,"rows2");

            if(NULL != pJson_array2)
            {
                array_size = cJSON_GetArraySize(pJson_array2);/*数组大小*/
                tasklist=pJson_array2->child;/*子对象*/
            }
            CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->protocol_type_list.clear();
            for(int index = 0;index < array_size;index++)
            {
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {
                    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->protocol_type_list.push_back(pCJSONFunCode->valuestring);
                }
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }
#endif
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 3;
            emit signal_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            /*4:视频服务器-----------------------------------------------------*/

            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_enable");
            if(pCJSONFunCode)
            {/*0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_protocol_type");
            if(pCJSONFunCode)
            {/*选中的协议类型*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->protocol_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_access_mode");
            if(pCJSONFunCode)
            {/*接入模式 0:静态IP 1:自动获取*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->access_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*IP地址或域名*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"video_server_port");
            if(pCJSONFunCode)
            {/*端口*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_port = pCJSONFunCode->valueint;
            }
#if 0
            cJSON *pJson_array3 = cJSON_GetObjectItem(root,"rows3");

            if(NULL != pJson_array3)
            {
                array_size = cJSON_GetArraySize(pJson_array3);/*数组大小*/
                tasklist=pJson_array3->child;/*子对象*/
            }
            CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->protocol_type_list.clear();
            for(int index = 0;index < array_size;index++)
            {
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {
                    CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->protocol_type_list.push_back(pCJSONFunCode->valuestring);
                }
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }
#endif
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 4;
            emit signal_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            /*5:本地网络-----------------------------------------------------*/

            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_enable");
            if(pCJSONFunCode)
            {/*0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_dhcp_server");
            if(pCJSONFunCode)
            {/*0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_dhcp_server = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_access_mode");
            if(pCJSONFunCode)
            {/*接入模式 0:静态IP 1:自动获取*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->access_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*IP地址*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_subnet_mask");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*子网掩码*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->subnet_mask = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_gateway");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*网关*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->gateway = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_nds1");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*DNS1*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->nds1 = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"local_network_nds2");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*DNS2*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->nds2 = pCJSONFunCode->valuestring;
            }
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 5;
            emit signal_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            /*6:WIFI-----------------------------------------------------*/

            pCJSONFunCode = cJSON_GetObjectItem(root,"wifi_enable");
            if(pCJSONFunCode)
            {/*0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"wifi_access_mode");
            if(pCJSONFunCode)
            {/*接入模式 0:静态IP 1:自动获取*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->access_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"wifi_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*IP地址*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"wifi_subnet_mask");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*子网掩码*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->subnet_mask = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"wifi_gateway");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*网关*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->gateway = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"wifi_nds1");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*DNS1*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->nds1 = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"wifi_nds2");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*DNS2*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->nds2 = pCJSONFunCode->valuestring;
            }
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 6;
            emit signal_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            /*7.FTP-----------------------------------------------------*/

            pCJSONFunCode = cJSON_GetObjectItem(root,"ftp_enable");
            if(pCJSONFunCode)
            {/*0:未选择状态 1:选中状态 2:禁用状态*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_enable = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"ftp_access_mode");
            if(pCJSONFunCode)
            {/*接入模式 0:静态IP 1:域名*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->access_mode = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"ftp_ip");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*IP地址*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_ip = pCJSONFunCode->valuestring;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"ftp_gateway");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*网关*/
                CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->gateway = pCJSONFunCode->valuestring;
            }
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 7;
            emit signal_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);


    }while(0);
}

void CMainPage::gui_system_storage_manage(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        CSystemStorageManage info;

        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(pCJSONFunCode)
        {
            info.m_type = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"enable");
        if(pCJSONFunCode)
        {
            info.m_enable = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"surplus_storage");
        if(pCJSONFunCode)
        {
            info.m_surplus_storage = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"total_storage");
        if(pCJSONFunCode)
        {
            info.m_total_storage = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"status");
        if(pCJSONFunCode)
        {
            info.m_status = pCJSONFunCode->valueint;
        }

        emit signal_storage_manage_widget_event(info);

    }while(0);
}

static bool compare_year(const CMonthInfo &barAmount1, const CMonthInfo &barAmount2)
{
    if (barAmount1.m_year > barAmount2.m_year)
    {
        return true;
    }
    return false;
}

static bool compare_month(const char &barAmount1, const char &barAmount2)
{
    if (barAmount1 > barAmount2)
    {
        return true;
    }
    return false;
}


void CMainPage::gui_system_videotape_query(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        pCJSONFunCode = cJSON_GetObjectItem(root,"type");
        if(NULL == pCJSONFunCode)
        {
            break;
        }

        if(1 == pCJSONFunCode->valueint)
        {/*获取年月数据*/
            CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_type = CSystemVideotapeQueryEx::kEnumGetDate;

            cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*数组大小*/
                tasklist=pJson_array->child;/*子对象*/
            }

            CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery->m_list.clear();

            for(int index = 0;index < array_size;index++)  //年列表
            {
                CMonthInfo month_info;
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"year");
                if(pCJSONFunCode)
                {
                    month_info.m_year = 2000 + pCJSONFunCode->valueint;

                }

                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"month");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {
                    QString month_value = pCJSONFunCode->valuestring;
                    QStringList month_list = month_value.split(",");
                    for(int index = 0;index < month_list.size();index++)
                    {
                        if(!month_list.at(index).isEmpty())
                        {
                            month_info.m_month_list.push_back(month_list.at(index).toInt());
                        }
                    }
                }
                qSort(month_info.m_month_list.begin(),month_info.m_month_list.end(),compare_month);
                CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery->m_list.push_back(month_info);
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }
            }
            qSort(CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery->m_list.begin(),
            CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery->m_list.end(), compare_year);
            if(MainWindow::kEnumMenuSystemManager == MainWindow::getInstance()->get_current_widget_id())
                emit signal_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);
            else
                emit signal_menu_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);

        }
        else if(2 == pCJSONFunCode->valueint)
        {/*获取月信息*/

            CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_type = CSystemVideotapeQueryEx::kEnumGetMonth;

            cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*数组大小*/
                tasklist=pJson_array->child;/*子对象*/
            }

            CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_VideotapeQueryDate->m_day_list.clear();
            for(int index = 0;index < array_size;index++)  //天列表
            {
                VideotapeDay day;
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"day");
                if(pCJSONFunCode)
                {
                    day.m_day =  pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"normal");
                if(pCJSONFunCode)
                {
                    day.normal =  pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"alarm");
                if(pCJSONFunCode)
                {
                    day.alarm =  pCJSONFunCode->valueint;
                }
                CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_VideotapeQueryDate->m_day_list.push_back(day);
                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }

            }

            if(MainWindow::kEnumMenuSystemManager == MainWindow::getInstance()->get_current_widget_id())
                emit signal_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);
            else
                emit signal_menu_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);

        }
        else if(3 == pCJSONFunCode->valueint)
        {/*获取列表信息*/


            CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_type = CSystemVideotapeQueryEx::kEnumGetDayList;

                CQueryResult *pResult = new CQueryResult;

                pCJSONFunCode = cJSON_GetObjectItem(root,"number");
                if(pCJSONFunCode)
                {
                    pResult->m_number = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"channel");
                if(pCJSONFunCode)
                {/* 录像通道 */
                    pResult->m_passageway_value = pCJSONFunCode->valueint;
                    pResult->m_passageway = QString::number(pResult->m_passageway_value);
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"rec_type");
                {/* 录像类型 */

                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"code_type");
                {

                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"idx_amount");
                {/* I帧总数 */

                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"data_size");
                {/* 数据总长度 /字节*/
                    pResult->m_size = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"time_start");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {/* 录像开始时间 */
                    pResult->m_date = pCJSONFunCode->valuestring;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"time_stop");

                QString stop_time = "";
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {/* 录像结束时间 */
                    stop_time = pCJSONFunCode->valuestring;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"frame_rate");
                {/*帧率*/

                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"bbak");
                {/*是否备份*/

                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"disktype");
                {

                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"file_index");
                {

                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"bfirst");
                {

                }

                pCJSONFunCode = cJSON_GetObjectItem(root,"data_end");
                {
                    if(1 == pCJSONFunCode->valueint)
                        pResult->data_end = true;
                    else
                        pResult->data_end = false;
                }

                qint64 start_time = QDateTime::fromString(pResult->m_date,"yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch();
                qint64 end_time = QDateTime::fromString(stop_time,"yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch();
                pResult->m_time = (end_time - start_time)/1000;
                pResult->m_position = 0;
                pResult->m_path = "";


                CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_VideotapeQueryList->param = pResult;

                if(MainWindow::kEnumMenuSystemManager == MainWindow::getInstance()->get_current_widget_id())
                    emit signal_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);
                else
                    emit signal_menu_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);

#if 0
            cJSON *pJson_array = cJSON_GetObjectItem(root,"rows");
            int array_size = 0;
            cJSON *tasklist = NULL;
            if(NULL != pJson_array)
            {
                array_size = cJSON_GetArraySize(pJson_array);/*数组大小*/
                tasklist=pJson_array->child;/*子对象*/
            }

            for(int index = 0;index < array_size;index++)
            {
                CQueryResult *pResult = new CQueryResult;

                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"number");
                if(pCJSONFunCode)
                {
                    pResult->m_number = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"channel");
                if(pCJSONFunCode)
                {/* 录像通道 */
                    pResult->m_passageway_value = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"rec_type");
                {/* 录像类型 */

                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"code_type");
                {

                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"idx_amount");
                {/* I帧总数 */

                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"data_size");
                {/* 数据总长度 /字节*/
                    pResult->m_size = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"time_start");
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {/* 录像开始时间 */
                    pResult->m_date = pCJSONFunCode->valuestring;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"time_stop");

                QString stop_time = "";
                if(pCJSONFunCode && pCJSONFunCode->valuestring)
                {/* 录像结束时间 */
                    stop_time = pCJSONFunCode->valuestring;
                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"frame_rate");
                {/*帧率*/

                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"bbak");
                {/*是否备份*/

                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"disktype");
                {

                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"file_index");
                {

                }
                pCJSONFunCode = cJSON_GetObjectItem(tasklist,"bfirst");
                {

                }

                pResult->m_time = 0;
                pResult->m_position = 0;
                pResult->m_path = "";


                CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_VideotapeQueryList->param = pResult;

                emit signal_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);

                tasklist = tasklist->next;
                if(NULL == tasklist)
                {
                    break;
                }

            }
#endif

        }
        else if(4 == pCJSONFunCode->valueint)
        {/*回放*/

            CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_type = CSystemVideotapeQueryEx::kEnumPlayBack;

            pCJSONFunCode = cJSON_GetObjectItem(root,"result");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {
                CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_result = gb2312ToUtf8(pCJSONFunCode->valuestring);
            }
            quit_interface();

            if(MainWindow::kEnumMenuSystemManager == MainWindow::getInstance()->get_current_widget_id())
                emit signal_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);
            else
                emit signal_menu_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);

        }
        else if(5 == pCJSONFunCode->valueint)
        {/*导出*/

            CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_type = CSystemVideotapeQueryEx::kEnumExport;
            pCJSONFunCode = cJSON_GetObjectItem(root,"percent");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideoExport->m_value = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"status");
            if(pCJSONFunCode)
            {
                CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideoExport->m_type = pCJSONFunCode->valueint;
            }

            pCJSONFunCode = cJSON_GetObjectItem(root,"result");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {/*导出失败原因*/
                CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideoExport->result = gb2312ToUtf8(pCJSONFunCode->valuestring);
            }




                emit signal_videotape_query_widget_event(CCommonData::getInstance()->m_CSystemVideotapeQueryEx);


        }
        else if(6 == pCJSONFunCode->valueint)
        {
            if(MainWindow::kEnumPlayback == MainWindow::getInstance()->get_current_widget_id())
                emit signal_playback_quit();
            //quit_interface();
        }
        else if(7 == pCJSONFunCode->valueint)
        {
            char play_type = 0;
            char value = 0;

            pCJSONFunCode = cJSON_GetObjectItem(root,"play_type");
            if(pCJSONFunCode)
            {
                play_type = pCJSONFunCode->valueint;
            }
            pCJSONFunCode = cJSON_GetObjectItem(root,"value");
            if(pCJSONFunCode)
            {
                value = pCJSONFunCode->valueint;
            }
            emit signal_playback_notify_event(play_type,value);
        }

    }while(0);
}


void CMainPage::gui_system_coordinate_collection(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        char current_type = 0;
        pCJSONFunCode = cJSON_GetObjectItem(root,"get_type");
        if(pCJSONFunCode)
        {
            current_type = pCJSONFunCode->valueint;
        }

        switch(current_type)
        {
            case 1:
            {/*获取所有数据*/
                cJSON *pJson_array1 = cJSON_GetObjectItem(root,"rows1");/*取数组*/
                cJSON *pJson_array2 = cJSON_GetObjectItem(root,"rows2");
                cJSON *pJson_array3 = cJSON_GetObjectItem(root,"rows3");
                cJSON *pJson_array4 = cJSON_GetObjectItem(root,"rows4");
                cJSON *pJson_array5 = cJSON_GetObjectItem(root,"rows5");
                cJSON *pJson_array6 = cJSON_GetObjectItem(root,"rows6");


                int array_size = 0;
                cJSON *tasklist = NULL;
                if(NULL != pJson_array1)
                {
                    array_size = cJSON_GetArraySize(pJson_array1);/*数组大小*/
                    tasklist=pJson_array1->child;/*子对象*/
                }

                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_line_list.clear();
                for(int index = 0;index < array_size;index++)  //线路列表
                {
                    pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                    if(pCJSONFunCode && pCJSONFunCode->valuestring)
                    {
                        CCommonData::getInstance()->m_CSystemCoordinateCollection.m_line_list.push_back(gb2312ToUtf8(pCJSONFunCode->valuestring));
                    }
                    tasklist = tasklist->next;
                    if(NULL == tasklist)
                    {
                        break;
                    }
                }
                array_size = 0;
                tasklist = NULL;
                if(NULL != pJson_array2)
                {
                    array_size=cJSON_GetArraySize(pJson_array2);/*数组大小*/
                    tasklist=pJson_array2->child;/*子对象*/
                }
                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_site_list.clear();
                for(int index = 0;index < array_size;index++)  //站点列表
                {
                    pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                    if(pCJSONFunCode && pCJSONFunCode->valuestring)
                    {
                        CCommonData::getInstance()->m_CSystemCoordinateCollection.m_site_list.push_back(gb2312ToUtf8(pCJSONFunCode->valuestring));
                    }
                    tasklist = tasklist->next;
                    if(NULL == tasklist)
                    {
                        break;
                    }
                }
                array_size = 0;
                tasklist = NULL;
                if(NULL != pJson_array3)
                {
                    array_size=cJSON_GetArraySize(pJson_array3);/*数组大小*/
                    tasklist=pJson_array3->child;/*子对象*/
                }
                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_speed_limiting_section_list.clear();
                for(int index = 0;index < array_size;index++)  //限速段列表
                {
                    pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                    if(pCJSONFunCode && pCJSONFunCode->valuestring)
                    {
                        CCommonData::getInstance()->m_CSystemCoordinateCollection.m_speed_limiting_section_list.push_back(gb2312ToUtf8(pCJSONFunCode->valuestring));
                    }
                    tasklist = tasklist->next;
                    if(NULL == tasklist)
                    {
                        break;
                    }
                }
                array_size = 0;
                tasklist = NULL;
                if(NULL != pJson_array4)
                {
                    array_size=cJSON_GetArraySize(pJson_array4);/*数组大小*/
                    tasklist=pJson_array4->child;/*子对象*/
                }
                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_inflection_point_list.clear();
                for(int index = 0;index < array_size;index++)  //拐点列表
                {
                    pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                    if(pCJSONFunCode && pCJSONFunCode->valuestring)
                    {
                        CCommonData::getInstance()->m_CSystemCoordinateCollection.m_inflection_point_list.push_back(gb2312ToUtf8(pCJSONFunCode->valuestring));
                    }
                    tasklist = tasklist->next;
                    if(NULL == tasklist)
                    {
                        break;
                    }
                }
                array_size = 0;
                tasklist = NULL;
                if(NULL != pJson_array5)
                {
                    array_size=cJSON_GetArraySize(pJson_array5);/*数组大小*/
                    tasklist=pJson_array5->child;/*子对象*/
                }

                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_station_list.clear();
                for(int index = 0;index < array_size;index++)  //场站列表
                {
                    pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                    if(pCJSONFunCode && pCJSONFunCode->valuestring)
                    {
                        CCommonData::getInstance()->m_CSystemCoordinateCollection.m_station_list.push_back(gb2312ToUtf8(pCJSONFunCode->valuestring));
                    }
                    tasklist = tasklist->next;
                    if(NULL == tasklist)
                    {
                        break;
                    }
                }
                array_size = 0;
                tasklist = NULL;
                if(NULL != pJson_array6)
                {
                    array_size=cJSON_GetArraySize(pJson_array6);/*数组大小*/
                    tasklist=pJson_array6->child;/*子对象*/
                }
                CCommonData::getInstance()->m_CSystemCoordinateCollection.m_field_coordinate_list.clear();
                for(int index = 0;index < array_size;index++)  //场坐标列表
                {
                    pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
                    if(pCJSONFunCode && pCJSONFunCode->valuestring)
                    {
                        CCommonData::getInstance()->m_CSystemCoordinateCollection.m_field_coordinate_list.push_back(gb2312ToUtf8(pCJSONFunCode->valuestring));
                    }
                    tasklist = tasklist->next;
                    if(NULL == tasklist)
                    {
                        break;
                    }
                }

                pCJSONFunCode = cJSON_GetObjectItem(root,"current_line");
                if(pCJSONFunCode)
                {//当前线路
                    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_line = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"current_site");
                if(pCJSONFunCode)
                {//当前站点
                    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_site = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"current_speed_limiting_section");
                if(pCJSONFunCode)
                {//当前限速段
                    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_speed_limiting_section = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"current_inflection_point");
                if(pCJSONFunCode)
                {//当前拐点
                    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_inflection_point = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"current_station");
                if(pCJSONFunCode)
                {//当前场站
                   CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_station  = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"current_field_coordinate");
                if(pCJSONFunCode)
                {//当前场坐标
                   CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_field_coordinate  = pCJSONFunCode->valueint;
                }
                pCJSONFunCode = cJSON_GetObjectItem(root,"direction");
                if(pCJSONFunCode)
                {//0:上行 1:下行
                   CCommonData::getInstance()->m_CSystemCoordinateCollection.m_direction  = pCJSONFunCode->valueint;
                }

            }break;
        }



        emit signal_coordinate_collection_widget_event(1,CCommonData::getInstance()->m_CSystemCoordinateCollection);

    }while(0);
}

void CMainPage::gui_system_register_info(cJSON *root)
{
    cJSON *pCJSONFunCode = NULL;
    do
    {
        cJSON *pJson_array=cJSON_GetObjectItem(root,"rows");/*取数组*/

        int array_size=cJSON_GetArraySize(pJson_array);/*数组大小*/
        cJSON *tasklist=pJson_array->child;/*子对象*/

        CCommonData::getInstance()->m_CSystemRegisterInfo.m_line.clear();
        for(int index = 0;index < array_size;index++)
        {
            pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {
                CCommonData::getInstance()->m_CSystemRegisterInfo.m_line.push_back(gb2312ToUtf8(pCJSONFunCode->valuestring));
            }
            tasklist = tasklist->next;
            if(NULL == tasklist)
            {
                break;
            }
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"line");
        if(pCJSONFunCode)
        {/*线路*/
            CCommonData::getInstance()->m_CSystemRegisterInfo.m_select_line = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"line_attribute");
        if(pCJSONFunCode)
        {
            CCommonData::getInstance()->m_CSystemRegisterInfo.m_line_attribute = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"car_number");
        if(pCJSONFunCode)
        {/*车辆编号*/
            CCommonData::getInstance()->m_CSystemRegisterInfo.m_car_number = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"manufacturer_number");
        if(pCJSONFunCode)
        {/*厂商编号*/
            CCommonData::getInstance()->m_CSystemRegisterInfo.m_manufacturer_number = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"select_rovince");
        if(pCJSONFunCode)
        {/*用户选择的省份*/
            CCommonData::getInstance()->m_CSystemRegisterInfo.m_select_rovince = pCJSONFunCode->valueint;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"license_plate");
        if(pCJSONFunCode)
        {/*车牌号码*/
            CCommonData::getInstance()->m_CSystemRegisterInfo.m_license_plate = pCJSONFunCode->valuestring;
        }
        pCJSONFunCode = cJSON_GetObjectItem(root,"device_id");
        if(pCJSONFunCode)
        {/*设备ID号*/
            CCommonData::getInstance()->m_CSystemRegisterInfo.m_device_id = pCJSONFunCode->valuestring;
        }

#if 0
        pJson_array=cJSON_GetObjectItem(root,"rows1");/*取数组*/

        array_size=cJSON_GetArraySize(pJson_array);/*数组大小*/
        tasklist=pJson_array->child;/*子对象*/


        CCommonData::getInstance()->m_CSystemRegisterInfo.m_rovince_list.clear();
        for(int index = 0;index < array_size;index++)
        {
            pCJSONFunCode = cJSON_GetObjectItem(tasklist,"value");
            if(pCJSONFunCode && pCJSONFunCode->valuestring)
            {
                CCommonData::getInstance()->m_CSystemRegisterInfo.m_rovince_list.push_back(pCJSONFunCode->valuestring);
            }
            tasklist = tasklist->next;
            if(NULL == tasklist)
            {
                break;
            }
        }
#endif

        emit signal_register_info_widget_event(CCommonData::getInstance()->m_CSystemRegisterInfo);

    }while(0);
}



void CMainPage::reset_touch_timer()
{
    int time_move = 0;

    time_move = m_time_touch_reset->elapsed();

    //if(time_move >= 1/*20*/)
    {
        emit signal_reset_touch_event();
        m_time_touch_reset->restart();
    }

}
void CMainPage::slot_reset_touch_event()
{
    CMyMouseEvent::getInstance()->reset_timer();
}

bool CMainPage::current_first_press()
{
    return CMyMouseEvent::getInstance()->m_first_press;
}

void CMainPage::slot_pushbutton_test()
{
    static int index = 0;

    if(!m_label_test_picture->isVisible())
    {
        m_label_test_picture->show();
        m_label_test_picture->setPixmap(QPixmap("/opt/test_picture/test1.png"));
        index = 1;
    }
    else if(1 == index)
    {
        m_label_test_picture->setPixmap(QPixmap("/opt/test_picture/test2.jpg"));
        index = 2;
    }
    else
    {
        m_label_test_picture->hide();
        index = 3;
    }
}


void CMainPage::slot_pushbutton_arrow_left()
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        cJSON_AddStringToObject(root, "msg_type", "gui_station_info");

        cJSON_AddNumberToObject(root, "station_prev", 1);

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}
void CMainPage::slot_pushbutton_arrow_right()
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        cJSON_AddStringToObject(root, "msg_type", "gui_station_info");

        cJSON_AddNumberToObject(root, "station_next", 1);

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

void CMainPage::slot_touch_mouse_event(int _x,int _y,bool _flag)
{
    CMyMouseEvent::getInstance()->touch_click_event(_x,_y,_flag);
}

bool CMainPage::handle_mouse_event(int _x,int _y,bool _flag)
{
    //m_CMyMouseEvent.touch_click_event(_x,_y,_flag);
    emit signal_touch_mouse_event(_x,_y,_flag);
    return true;
}

void CMainPage::slot_timer_vehicle()
{
    static bool show_change = true;
    CEventData eventData;

   // CCommonInterface::printf_debug("slot_timer_vehicle()");

    if(show_change)
    {
        show_change = false;
        eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_line_name;
        main_page_notify(kEnumMainPageVehicleModel,1,&eventData);
    }
    else
    {
        show_change = true;
        eventData.m_str_data = CCommonData::getInstance()->m_sd_info.m_bus_number;
        main_page_notify(kEnumMainPageVehicleModel,0,&eventData);
    }





}

void CMainPage::slot_timer_test()
{
     //CCommonData::getInstance()->m_CPassengerInfo.passenger_sum++;
     //emit signal_passenger_info(CCommonData::getInstance()->m_CPassengerInfo);

    //main_page_notify(kEnumMainPageVerificationRrStatus,kEnumQrStatusabNormal,0);
    //m_timer_test->stop();
    slot_pushbutton_arrow_right();
    slot_pushbutton_arrow_right();
}

void CMainPage::slot_timer_station()
{
    m_timer_station->stop();
    emit signal_driver_enter_station(false);
}

void CMainPage::slot_timer_key_5s()
{
    qDebug() << "slot_timer_key_5s() start m_key_times:" + QString::number(m_key_times);
    if(m_timer_key_5s->isActive())
        m_timer_key_5s->stop();

    CEventData eventData;
    eventData.m_i_data = 20 * 60;
    eventData.m_param1 = 1;

    if(kEnumEventKeyDown == m_current_key_event_type)
    {//3下导入刷卡参数  5下导出刷卡参数
        if(3 == m_key_times)
        {//导入刷卡参数
            qDebug() << "slot_timer_key_5s import flash param";

            eventData.m_str_data = tr("正在导入刷卡参数...");
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusMessage,&eventData);
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumImportCardParam);
        }
        else if(5 == m_key_times)
        {//导出刷卡参数
            qDebug() << "slot_timer_key_5s export flash param";

            eventData.m_str_data = tr("正在导出刷卡参数...");
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusMessage,&eventData);
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumExportCardParam);
        }
        else if(1 == m_key_times)
        {
            qDebug() << "slot_timer_key_5s next station!!!!!!!!!";
            slot_pushbutton_arrow_right();
            //slot_pushbutton_arrow_right();
        }
    }
    else if(kEnumEventKeyBack == m_current_key_event_type)
    {//3下导入报站      5下导出报站
        if(3 == m_key_times)
        {//导入报站
            qDebug() << "slot_timer_key_5s import station";

            eventData.m_str_data = tr("正在导入报站...");
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusMessage,&eventData);
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumImportStationFile);
        }
        else if(5 == m_key_times)
        {//导出报站
            qDebug() << "slot_timer_key_5s export station";

            eventData.m_str_data = tr("正在导出报站...");
            main_page_notify(kEnumMainPageVerificationRrStatus,kEnumCardStatusMessage,&eventData);
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumExportStationFile);
        }
    }
    else if(kEnumEventKeyUp == m_current_key_event_type)
    {
        if(1 == m_key_times)
        {
            slot_pushbutton_arrow_left();
           // slot_pushbutton_arrow_left();
        }
    }

    m_key_times = 0;
    qDebug() << "slot_timer_key_5s() end";
}














