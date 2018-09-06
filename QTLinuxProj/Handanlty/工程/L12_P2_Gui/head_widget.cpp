#include "head_widget.h"

#include <QDateTime>
#include "./class/common_interface.h"

#include "LtyCommonDefine.h"

#define MENU_BUTTON_GROUP_SELECTED_QSS	"\
QPushButton{\
padding: 0px 0px 0px 40px;\
text-align : left;\
font:24px;\
border: 0px;\
background:transparent;\
color:#ffffff;\
}\
QPushButton:hover:pressed {\
background:transparent;\
color:#ffffff;\
} \
QPushButton:hover:!pressed {\
background:transparent;\
color:#ffffff;\
}"

#define HEAD_WIDGET_HEIGHT      82

CHeadWidget::CHeadWidget(int _type,QWidget *parent) :
    QWidget(parent)
{

    this->setFixedWidth(1024);
    this->setObjectName("CHeadWidget");
    this->setStyleSheet("QWidget#CHeadWidget{background:transparent;}");

    m_widget_head = new QWidget(this);
    m_widget_head->setFixedSize(1024,103);
    m_widget_head->move(0,0);

    m_widget_head1 = new QWidget(this);
    m_pushbutton_sd_status = NULL;
    if(kEnumHeadWidgetMainPage == _type || kEnumHeadWidgetVideo == _type)
    {
        timer_background_init();
    }
    if(kEnumHeadWidgetMainPage == _type)
    {
        main_head_init();
    }
    else if(kEnumHeadWidgetVideo == _type)
    {
       head_widget_button_home_init();
       head_widget_button_back_init();
    }
    else if(kEnumHeadWidgetMenu == _type || kEnumHeadWidgetSystemManage == _type
            || kEnumHeadWidgetMessage == _type
            || kEnumHeadWidgetCall == _type
            || kEnumHeadWidgetAbout == _type
            || kEnumHeadWidgetDriverMenu == _type
            || kEnumHeadWidgetRunStatus == _type
            || kEnumHeadWidget16Video == _type
            || kEnumHeadWidgetVideotape == _type
            || kEnumHeadWidgetPassageway == _type
            || kEnumHeadWidgetPayInfo == _type
            || kEnumHeadWidgetPassword == _type
            || kEnumHeadWidgetPassenger == _type)
    {
        this->setFixedHeight(HEAD_WIDGET_HEIGHT);

        m_widget_head->setStyleSheet("QWidget {border:0px;background-color:#1A427C;}");

        m_widget_head1->setFixedSize(392,76);
        m_widget_head1->move((1024 - 392)/2,6);
        m_widget_head1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/time_bg.png);}");

        if(kEnumHeadWidget16Video == _type)
            head_widget_button_home_init();
        else if(kEnumHeadWidgetPassword != _type)
            head_widget_button_back_init();


        m_munu_title = new QLabel(this);
        m_munu_title->setFixedSize(150,70);
        m_munu_title->move((1024 - 150)/2,(82-70)/2);
        m_munu_title->setStyleSheet("QLabel{border:0px;background:transparent;font:40px;color:#ffffff; }");
        m_munu_title->setAlignment(Qt::AlignCenter);
        if(kEnumHeadWidgetMenu == _type)
        {
            head_widget_button_home_init();
            m_munu_title->setText(tr("菜单"));
        }
        else if(kEnumHeadWidgetSystemManage == _type)
        {
            QString show_content = tr("系统管理");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);

            head_widget_button_home_init();

        }
        else if(kEnumHeadWidgetMessage == _type)
        {
            head_widget_button_home_init();
            m_munu_title->setText(tr("消息"));
        }
        else if(kEnumHeadWidgetCall == _type)
            m_munu_title->setText(tr("通话"));
        else if(kEnumHeadWidgetAbout == _type)
        {
            QString show_content = tr("关于本机");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);
            head_widget_button_home_init();
        }
        else if(kEnumHeadWidgetDriverMenu == _type)
        {
            QString show_content = tr("司机菜单");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);
            head_widget_button_home_init();
        }
        else if(kEnumHeadWidgetRunStatus == _type)
        {
            head_widget_button_home_init();
            QString show_content = tr("运行状态");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);
        }
        else if(kEnumHeadWidgetVideotape == _type)
        {
            head_widget_button_home_init();
            QString show_content = tr("录像查询");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);
        }
        else if(kEnumHeadWidget16Video == _type)
        {
            QString show_content = tr("视频预览");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);

            QString str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                .arg(":/img/res//main_page/16video/button.png").arg(":/img/res/main_page/16video/button_hover.png").arg(":/img/res/main_page/16video/button_hover.png");

            m_pushbutton_1 = new QPushButton(this);
            CCommonInterface ::init_new_button(m_pushbutton_1,766,7,54,70,str_sheet);

            connect(m_pushbutton_1,SIGNAL(clicked()),SLOT(slot_pushbutton_1()));
            m_pushbutton_1->setText(tr("1"));

            m_pushbutton_2 = new QPushButton(this);
            CCommonInterface ::init_new_button(m_pushbutton_2,828,7,54,70,str_sheet);
            m_pushbutton_2->setText(tr("4"));

            connect(m_pushbutton_2,SIGNAL(clicked()),SLOT(slot_pushbutton_2()));

            m_pushbutton_3 = new QPushButton(this);
            CCommonInterface ::init_new_button(m_pushbutton_3,890,7,54,70,str_sheet);
            m_pushbutton_3->setText(tr("9"));

            connect(m_pushbutton_3,SIGNAL(clicked()),SLOT(slot_pushbutton_3()));

            m_pushbutton_4 = new QPushButton(this);
            CCommonInterface ::init_new_button(m_pushbutton_4,952,7,54,70,str_sheet);
            m_pushbutton_4->setText(tr("16"));

            connect(m_pushbutton_4,SIGNAL(clicked()),SLOT(slot_pushbutton_4()));

            str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                .arg(":/img/res//main_page/menu/passenger_flow.png").arg(":/img/res/main_page/menu/passenger_flow_hover.png").arg(":/img/res/main_page/menu/passenger_flow_hover.png");

            m_pushbutton_passenger_flow = new QPushButton(this);
            CCommonInterface ::init_new_button(m_pushbutton_passenger_flow,152,27,35,28,str_sheet);

            str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                .arg(":/img/res/main_page/menu/loop_playback_hover.png").arg(":/img/res/main_page/menu/loop_playback_hover.png").arg(":/img/res/main_page/menu/loop_playback_hover.png");

            m_movie_loop = new QMovie(this);
            m_movie_loop->setFileName(":/img/res/main_page/menu/loop_playback.gif");
            m_label_loop = new QLabel(this);
            m_label_loop->setFixedSize(35, 36);
            m_label_loop->move(223,23);
            m_label_loop->setMovie(m_movie_loop);


            m_pushbutton_loop_playback = new QPushButton(this);
            CCommonInterface ::init_new_button(m_pushbutton_loop_playback,223,23,35,36,str_sheet);

            connect(m_pushbutton_passenger_flow,SIGNAL(clicked()),SLOT(slot_pushbutton_passenger_flow()));
            connect(m_pushbutton_loop_playback,SIGNAL(clicked()),SLOT(slot_pushbutton_loop_playback()));
            set_movie(false);
        }
        else if(kEnumHeadWidgetPassageway == _type)
        {
            head_widget_button_home_init();
        }
        else if(kEnumHeadWidgetPayInfo == _type)
        {
            head_widget_button_home_init();
            QString show_content = tr("交易信息");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);
        }
        else if(kEnumHeadWidgetPassword == _type)
        {
            QString show_content = tr("用户登录");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);
        }
        else if(kEnumHeadWidgetPassenger == _type)
        {
            head_widget_button_home_init();
            QString show_content = tr("客流统计");
            int str_length = CCommonInterface::get_string_length(show_content,61);
            m_munu_title->setFixedWidth(str_length);
            m_munu_title->move((1024 - str_length)/2,(82-70)/2);
            m_munu_title->setText(show_content);
        }
    }
    else if(kEnumHeadWidgetVideoPlayback == _type)
    {
        this->setFixedHeight(HEAD_WIDGET_HEIGHT);

        head_widget_button_home_init();
        head_widget_button_back_init();
    }
    else if(kEnumHeadWidgetPassengerFlow == _type)
    {
        this->setFixedHeight(HEAD_WIDGET_HEIGHT);
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        timer_background_init();
        main_head_init_d2();

        m_pushbutton_quit = new QPushButton(this);
        m_pushbutton_quit->move((1024 - 392)/2 + 80,6);
        m_pushbutton_quit->setFixedSize(392 - 80,76);
        m_pushbutton_quit->setFocusPolicy(Qt::NoFocus);
        m_pushbutton_quit->setFlat(true);
        m_pushbutton_quit->setStyleSheet("QPushButton{border:0px;background:transparent;}");
        connect(m_pushbutton_quit,SIGNAL(clicked()),this,SIGNAL(signal_button_menu()));


        QString str_sheet = "QLabel{font:28px;color:#94c6ff;}";
        QString str_show = tr("车内:");
        m_label_total1 = new QLabel(this);
        int str_length = CCommonInterface::init_label_text(m_label_total1,715 - 10,(this->height() - 30)/2,0,30,str_show,str_sheet,Qt::AlignCenter,28);

        str_sheet = "QLabel{border: 1px solid #2865bd;font:20px;color:#dddddd;background:#13315d;border-radius: 4px;}";
        str_show = tr("0");
        m_label_total2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_total2,754/*200 + str_length + 10*/,(this->height() - 44)/2,50,44,str_show,str_sheet,Qt::AlignCenter,20);

        int start_x = m_label_total2->x() + 100 + 31;
        str_sheet = "QLabel{font:28px;color:#94c6ff;}";
        str_show = tr("上车:");
        m_label_up1 = new QLabel(this);
        str_length = CCommonInterface::init_label_text(m_label_up1,814 - 10/*start_x*/,(this->height() - 30)/2,0,30,str_show,str_sheet,Qt::AlignCenter,28);

        str_sheet = "QLabel{border: 1px solid #2865bd;font:20px;color:#dddddd;background:#13315d;border-radius: 4px;}";
        str_show = tr("0");
        m_label_up2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_up2,853/*start_x + str_length + 10*/,(this->height() - 44)/2,50,44,str_show,str_sheet,Qt::AlignCenter,20);

        start_x = m_label_up2->x() + 100 + 31;
        str_sheet = "QLabel{font:28px;color:#94c6ff;}";
        str_show = tr("下车:");
        m_label_down1 = new QLabel(this);
        str_length = CCommonInterface::init_label_text(m_label_down1,913 - 10/*start_x*/,(this->height() - 30)/2,0,30,str_show,str_sheet,Qt::AlignCenter,28);

        str_sheet = "QLabel{border: 1px solid #2865bd;font:20px;color:#dddddd;background:#13315d;border-radius: 4px;}";
        str_show = tr("0");
        m_label_down2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_down2,952/*start_x + str_length + 10*/,(this->height() - 44)/2,50,44,str_show,str_sheet,Qt::AlignCenter,20);

#else
        m_widget_head->setStyleSheet("QWidget {border:0px;background-color:#1A427C;}");
        head_widget_button_home_init();
        head_widget_button_back_init();

        /*
            QLabel *m_label_total1;
    QLabel *m_label_total2;
    QLabel *m_label_up1;
    QLabel *m_label_up2;
    QLabel *m_label_down1;
    QLabel *m_label_down2;
        */
        QString str_sheet = "QLabel{font:23px;color:#77ceff;}";
        QString str_show = tr("车内:");
        m_label_total1 = new QLabel(this);
        int str_length = CCommonInterface::init_label_text(m_label_total1,200,(this->height() - 30)/2,0,30,str_show,str_sheet);

        str_sheet = "QLabel{border: 1px solid #77ceff;font:30px;color:#77ceff;background:#000000;}";
        str_show = tr("0");
        m_label_total2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_total2,200 + str_length + 10,(this->height() - 52)/2,100,52,str_show,str_sheet);

        int start_x = m_label_total2->x() + 100 + 31;
        str_sheet = "QLabel{font:23px;color:#77ceff;}";
        str_show = tr("上车:");
        m_label_up1 = new QLabel(this);
        str_length = CCommonInterface::init_label_text(m_label_up1,start_x,(this->height() - 30)/2,0,30,str_show,str_sheet);

        str_sheet = "QLabel{border: 1px solid #77ceff;font:30px;color:#77ceff;background:#000000;}";
        str_show = tr("0");
        m_label_up2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_up2,start_x + str_length + 10,(this->height() - 52)/2,100,52,str_show,str_sheet);

        start_x = m_label_up2->x() + 100 + 31;
        str_sheet = "QLabel{font:23px;color:#77ceff;}";
        str_show = tr("下车:");
        m_label_down1 = new QLabel(this);
        str_length = CCommonInterface::init_label_text(m_label_down1,start_x,(this->height() - 30)/2,0,30,str_show,str_sheet);

        str_sheet = "QLabel{border: 1px solid #77ceff;font:30px;color:#77ceff;background:#000000;}";
        str_show = tr("0");
        m_label_down2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_down2,start_x + str_length + 10,(this->height() - 52)/2,100,52,str_show,str_sheet);
#endif
    }

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this,false);

    //m_define_screen->reset_widget();
    m_status_4g = false;
    m_status_gps = false;
    m_status_harddisk = false;
    m_status_it = false;
    m_status_video = false;
}

void CHeadWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CHeadWidget::head_widget_button_home_init()
{
    m_pushbutton_home = new QPushButton(this);

    QString str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res//main_page/menu/home.png").arg(":/img/res/main_page/menu/home_hover.png").arg(":/img/res/main_page/menu/home_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_home,30,(this->height() - 56)/2,96,56,str_sheet);

    connect(m_pushbutton_home,SIGNAL(clicked()),SIGNAL(signal_button_home()));
}

void CHeadWidget::head_widget_button_back_init()
{
    //返回按钮
    m_pushbutton_back = new QPushButton(this);
    m_pushbutton_back->setObjectName("CHeadWidget_back");

    QString str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
     .arg(":/img/res//main_page/menu/return.png").arg(":/img/res/main_page/menu/return_hover.png").arg(":/img/res/main_page/menu/return_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_back,1024 - 30 - 96,(this->height() - 56)/2,96,56,str_sheet);

    connect(m_pushbutton_back,SIGNAL(clicked()),SIGNAL(signal_button_back()));
}

void CHeadWidget::main_head_init_d2()
{
    m_label_network = new QLabel(this);
    m_label_network->setFixedSize(22,19);
    m_label_network->move(33 - 20,13);
    m_label_network->setStyleSheet("QLabel{border:0px;background:transparent;font:13px;color:#ffffff;}");


    //信号
    m_pushbutton_signal = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_signal,40 - 20,(82 - 42)/2,38,42);


    //GPS
    m_label_gps_number = new QLabel(this);
    m_label_gps_number->setFixedSize(22,19);
    m_label_gps_number->move(138 - 30,13);
    m_label_gps_number->setStyleSheet("QLabel{border:0px;background:transparent;font:13px;color:#ffffff;}");


    m_pushbutton_gps = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_gps,40 + 38 + 30 - 30,(82 - 42)/2,31,42);

    //平台连接状态
    m_pushbutton_platform_status = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_platform_status,192,(82 - 42)/2,36,42);

    m_pushbutton_sd_status = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_sd_status,135,(82 - 42)/2,36,42);


    //设备异常
    m_pushbutton_device_abnormal = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_device_abnormal,238 + 5,18,36,42);
    m_pushbutton_device_abnormal->hide();

//#ifndef CURRENT_ARM_RUN
    QString str_sheet = "QPushButton{border:0px;background:transparent;font:30px;color:#ffffff;}";

#ifndef CURRENT_USE_LIB
    //菜单和退出调换位置
    m_pushbutton_menu = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_menu,238 + 5 + 36 + 10,18,50,42,str_sheet);
    m_pushbutton_menu->setText("退出");
    connect(m_pushbutton_menu,SIGNAL(clicked()),this,SIGNAL(signal_pushbutton_quit()));
#endif

    m_pushbutton_font_door = NULL;//前门
    m_pushbutton_back_door = NULL;//后门
    //m_pushbutton_device_abnormal = NULL;//设备异常
    m_pushbutton_hard_disk_status = NULL;//硬盘状态
    //m_pushbutton_platform_status = NULL;//平台连接状态 (调度平台)
    m_pushbutton_platform_status1 = NULL;//平台连接状态 (视频服务器)


    slot_1s_timer();
}

void CHeadWidget::main_head_init()
{
    QString str_sheet = "";

    //QLabel *m_label_network;            //2G 3G 4G
    m_label_network = new QLabel(this);
    m_label_network->setFixedSize(22,19);
    m_label_network->move(33,13);
    m_label_network->setStyleSheet("QLabel{border:0px;background:transparent;font:13px;color:#ffffff;}");


    //信号
    m_pushbutton_signal = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_signal,40,(82 - 42)/2,38,42);


    //GPS
    m_label_gps_number = new QLabel(this);
    m_label_gps_number->setFixedSize(22,19);
    m_label_gps_number->move(138,13);
    m_label_gps_number->setStyleSheet("QLabel{border:0px;background:transparent;font:13px;color:#ffffff;}");


    m_pushbutton_gps = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_gps,40 + 38 + 30,(82 - 42)/2,31,42);

    //前门
    m_pushbutton_font_door = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_font_door,40 + 38 + 30 + 31 + 30,(82 - 42)/2,20,42);

    //后门
    m_pushbutton_back_door = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_back_door,40 + 38 + 30 + 31 + 30 + 20 + 30,(82 - 42)/2,20,42);

#if 0
    //ACC状态
    m_pushbutton_acc_status = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_acc_status,35 + 22 + 30 + 20 + 30,18,14,26);


    //设备门（机器上）
    m_pushbutton_device_door = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_device_door,35 + 22 + 30 + 20 + 30 + 14 + 30,18,14,27);
#endif

    //设备异常
    m_pushbutton_device_abnormal = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_device_abnormal,1024 - (36 + 30) * 2 - 38 - 30 - 36 - 30,18,36,42);


    //硬盘状态
    m_pushbutton_hard_disk_status = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_hard_disk_status,1024 - (36 + 30) * 2 - 38 - 30,(82 - 42)/2,38,42);


    //平台连接状态
    m_pushbutton_platform_status = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_platform_status,1024 - (36 + 30) * 2,(82 - 42)/2,36,42);

#if 0
    //消息
    m_pushbutton_message = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_message,1024 - 36 - 30,(82 - 42)/2,36,42,str_sheet);
#endif

        //平台连接状态
    m_pushbutton_platform_status1 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_platform_status1,1024 - 36 - 30,(82 - 42)/2,36,42);


    slot_1s_timer();


}

void CHeadWidget::slot_1s_timer()
{
    QString strCurrentTime = QDateTime::currentDateTimeUtc().toString("hh:mm:ss");

    m_label_time->setText(strCurrentTime);

}

void CHeadWidget::head_widget_notify(int _msg,int _event,void *_param)
{
    QString str_sheet = "";

    switch(_msg)
    {
        ////////////////////////////////////////////////////////////////////头部显示 start
        case kEnumMainPageSignal1://SIM卡　2G 3G 4G
        {
            if(NULL == m_label_network)
                break;
            if(kEnumNetwork2G == _event)
                m_label_network->setText(tr("2G"));
            else if(kEnumNetwork3G == _event)
                m_label_network->setText(tr("3G"));
            else if(kEnumNetwork4G == _event)
                m_label_network->setText(tr("4G"));
            else
                m_label_network->setText(tr(""));
        }break;
        case kEnumMainPageSignal2: //SIM卡信号强弱
        {
            if(NULL == m_pushbutton_signal)
                break;
            m_status_4g = true;
            if(kEnumNetworkSignal1 == _event)
            {
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
                        .arg(":/img/res/main_page/signal_1.png");
                CCommonInterface::modify_button_sheet(m_pushbutton_signal,str_sheet);
            }
            else if(kEnumNetworkSignal2 == _event)
            {
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
                        .arg(":/img/res/main_page/signal_2.png");
                CCommonInterface::modify_button_sheet(m_pushbutton_signal,str_sheet);
            }
            else if(kEnumNetworkSignal3 == _event)
            {
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
                        .arg(":/img/res/main_page/signal_3.png");
                CCommonInterface::modify_button_sheet(m_pushbutton_signal,str_sheet);
            }
            else if(kEnumNetworkSignal4 == _event)
            {
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
                        .arg(":/img/res/main_page/signal_4.png");
                CCommonInterface::modify_button_sheet(m_pushbutton_signal,str_sheet);
            }
            else if(kEnumNetworkSignalError == _event)
            {
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
                        .arg(":/img/res/main_page/signal_5.png");
                CCommonInterface::modify_button_sheet(m_pushbutton_signal,str_sheet);
                m_status_4g = false;
            }
            else
            {
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
                        .arg(":/img/res/main_page/signal.png");

                CCommonInterface::modify_button_sheet(m_pushbutton_signal,str_sheet);
                m_status_4g = false;
            }
#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
            check_all_status();
#endif
        }break;
        case kEnumMainPageGpsNumber://GPS个数
        {
            if(NULL == m_label_gps_number)
                break;
            m_label_gps_number->hide();
            if(_event >= 0)
                m_label_gps_number->setText(QString::number(_event));
            else
                m_label_gps_number->setText("");

        }break;
        case kEnumMainPageGpsStatus://GPS状态
        {
            if(NULL == m_pushbutton_gps)
                break;
            m_status_gps = true;
            if(kEnumGpsNormal == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/positioning.png");
            else //if(kEnumGpsAbnormal == _event)
            {
                m_status_gps = false;
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/positioning_2.png");
            }
            CCommonInterface::modify_button_sheet(m_pushbutton_gps,str_sheet);
#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
            check_all_status();
#endif
        }break;
        case kEnumMainPageFrontDoor:     //前门
        {
            if(NULL == m_pushbutton_font_door)
                break;

            if(kEnumDoorOpen == _event)
            {
                CCommonInterface ::init_new_button(m_pushbutton_font_door,40 + 38 + 30 + 31 + 30,(82 - 42)/2,20,42);
                m_pushbutton_back_door->move(40 + 38 + 30 + 31 + 30 + 20 + 30,(82 - 42)/2);
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                        .arg(":/img/res/main_page/open.png").arg(":/img/res/main_page/open.png").arg(":/img/res/main_page/open.png");
            }
            else //if(kEnumDoorClose == _event)
            {
                CCommonInterface ::init_new_button(m_pushbutton_font_door,40 + 38 + 30 + 31 + 30,(82 - 42)/2,25,42);
                m_pushbutton_back_door->move(40 + 38 + 30 + 31 + 30 + 25 + 30,(82 - 42)/2);
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                        .arg(":/img/res//main_page/close.png").arg(":/img/res/main_page/close.png").arg(":/img/res/main_page/close.png");
            }
            CCommonInterface::modify_button_sheet(m_pushbutton_font_door,str_sheet);

        }break;
        case kEnumMainPageBackDoor:       //后门
        {
            if(NULL == m_pushbutton_back_door)
                break;

            if(kEnumDoorOpen == _event)
            {
                m_pushbutton_back_door->setFixedWidth(20);
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                        .arg(":/img/res/main_page/open.png").arg(":/img/res/main_page/open.png").arg(":/img/res/main_page/open.png");
            }
            else //if(kEnumDoorClose == _event)
            {
               m_pushbutton_back_door->setFixedWidth(25);
               str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                        .arg(":/img/res/main_page/close.png").arg(":/img/res/main_page/close.png").arg(":/img/res/main_page/close.png");
            }
            CCommonInterface::modify_button_sheet(m_pushbutton_back_door,str_sheet);

        }break;
#if 0
        case kEnumMainPageAccStatus://ACC状态
        {
            if(kEnumAcc1 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/db_suo1.png");
             else //if(kEnumAcc2l == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/db_suo2.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_acc_status,str_sheet);

        }break;
        case kEnumMainPageDeviceDoorStatus: //设备门（机器上）状态
        {
            if(kEnumDeviceDoorOpen == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }") .arg(":/img/res//db_sbm1.png");
            else //if(kEnumDeviceDoorClose == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }") .arg(":/img/res//db_sbm2.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_device_door,str_sheet);
        }break;
#endif
        case kEnumMainPageDeviceAbnorma://设备异常（机器上）
        {
            if(NULL == m_pushbutton_device_abnormal)
                break;

            if(kEnumDeviceNormal == _event)
                str_sheet = QString("QPushButton {border:0px;background:transparent; }");
            else //if(kEnumDeviceAbnormal == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/alarm.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_device_abnormal,str_sheet);

        }break;
        case kEnumMainPageHardDiskStatus://硬盘状态
        {
            if(NULL == m_pushbutton_hard_disk_status)
                break;
            m_status_harddisk = false;
            if(kEnumHardDiskNormal == _event)
            {
                m_status_harddisk = true;
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/hard_disk_1.png");
            }
            else if(kEnumHardDiskAbnormal1 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/hard_disk_2.png");
            else if(kEnumHardDiskAbnormal2 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/hard_disk_3.png");
            else
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/hard_disk_1.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_hard_disk_status,str_sheet);
#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
            check_all_status();
#endif
        }break;
        case kEnumMainPagePlatformStatus://平台连接状态
        {
            if(NULL == m_pushbutton_platform_status)
                break;
            m_status_it = false;
            if(kEnumPlatformNormal == _event)
            {
                m_status_it = true;
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_1.png");
            }
            else if(kEnumPlatformAbnormal1 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_2.png");
            else if(kEnumPlatformAbnormal2 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_3.png");
            else if(kEnumPlatformConnect == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_4.png");
            else
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_1.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_platform_status,str_sheet);
#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
            check_all_status();
#endif
        }break;
        case kEnumMainPagePlatformStatus1://平台连接状态
        {
            if(NULL == m_pushbutton_platform_status1)
                break;
            m_status_video = false;
            if(kEnumPlatformNormal == _event)
            {
                m_status_video = true;
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_1.png");
            }
            else if(kEnumPlatformAbnormal1 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_2.png");
            else if(kEnumPlatformAbnormal2 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_3.png");
            else if(kEnumPlatformConnect == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_4.png");
            else
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/screen_status_1.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_platform_status1,str_sheet);
#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
            check_all_status();
#endif
        }break;
#if 0
        case kEnumMainPageMessageStatus: //消息状态
        {
            if(kEnumMessageStatus1 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res//db_xx1.png");
            else //if(kEnumMessageStatus2 == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res//db_xx2.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_message,str_sheet);

        }break;
#endif
        case kEnumMainPageSdStatus:
        {
            if(NULL == m_pushbutton_sd_status)
                break;

            if(kEnumSdNormalNormal == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/SD_card.png");
            else if(kEnumError == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/SD_card_3.png");
            else
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/SD_card_2.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_sd_status,str_sheet);

        }break;

        ////////////////////////////////////////////////////////////////////头部显示 end
    }
}


void CHeadWidget::slot_pushbutton_1()
{
    emit signal_button_video(0);
}
void CHeadWidget::slot_pushbutton_2()
{
    emit signal_button_video(1);
}
void CHeadWidget::slot_pushbutton_3()
{
    emit signal_button_video(2);
}
void CHeadWidget::slot_pushbutton_4()
{
    emit signal_button_video(3);
}

void CHeadWidget::set_head_widget_title(const QString &_text)
{
    m_munu_title->setText(_text);
}

void CHeadWidget::slot_pushbutton_passenger_flow()
{
    emit signal_pushbutton_passenger_flow();
}
void CHeadWidget::slot_pushbutton_loop_playback()
{
    if(m_movie_run)
    {
        m_movie_run = false;
    }
    else
    {
        m_movie_run = true;
    }
    set_movie(m_movie_run);
    emit signal_pushbutton_loop_playback(m_movie_run);
}


void CHeadWidget::set_movie(bool _flag)
{
    m_movie_run = _flag;
    if(_flag)
    {
        m_label_loop->show();
        m_movie_loop->start();
    }
    else
    {
        m_label_loop->hide();
        m_movie_loop->stop();
    }
}
bool CHeadWidget::get_movie()
{
    return m_movie_run;
}

void CHeadWidget::timer_background_init()
{
    //时间定时器
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_1s_timer()));
    m_timer->start();

    this->setFixedHeight(HEAD_WIDGET_HEIGHT);
    m_widget_head->setStyleSheet("QWidget {border:0px;background-color:#1A427C;}");

    QString strCurrentTime = tr("55:55:55");//QDateTime::currentDateTime().toString("hh:mm:ss");
    int str_length = CCommonInterface::get_string_length(strCurrentTime,48);
    m_label_time = new QLabel(this);
    m_label_time->setFixedSize(str_length,76);
    m_label_time->move((1024 - str_length)/2,6);
    m_label_time->setStyleSheet("QLabel{border:0px;background:transparent;font:48px;color:#ffffff; }");
    m_label_time->setAlignment(Qt::AlignCenter);

    m_widget_head1->setFixedSize(392,76);
    m_widget_head1->move((1024 - 392)/2,6);
    m_widget_head1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/time_bg.png);}");
}

void CHeadWidget::slot_passenger_info(const CPassengerInfo & _info)
{

    m_label_total2->setText(QString::number(_info.passenger_sum));
    m_label_up2->setText(QString::number(_info.passenger_up));
    m_label_down2->setText(QString::number(_info.passenger_down));
    m_label_total2->update();
    m_label_up2->update();
    m_label_down2->update();


}

void CHeadWidget::check_all_status()
{
    if(m_status_4g &&
       m_status_gps &&
       m_status_harddisk &&
       m_status_it &&
       m_status_video)
    {
        m_pushbutton_device_abnormal->hide();
    }
    else
    {
        m_pushbutton_device_abnormal->show();
    }

}

void CHeadWidget::slot_main_head_home_back_show(bool _flag)
{
    if(_flag)
    {
        m_pushbutton_home->show();
        m_pushbutton_back->show();
    }
    else
    {
        m_pushbutton_home->hide();
        m_pushbutton_back->hide();
    }

}














