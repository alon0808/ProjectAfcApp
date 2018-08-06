#include "about_widget.h"

#include "class/common_interface.h"
#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"
#include <QTimer>
#include <QLine>

CAboutMachineWidget::CAboutMachineWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CAboutMachineWidget");
   this->setStyleSheet("QWidget#CAboutMachineWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
#if 1
    for(int index = 0;index < 11;index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }
    QWidget *pWidgetLine1 = new QWidget(m_widget_main);
    QWidget *pWidgetLine2 = new QWidget(m_widget_main);

    pWidgetLine1->setFixedSize(1,ROW_HEIGHT * 10);
    pWidgetLine1->move(290 - 250,158 - MAIN_HEAD_HEIGHT);
    pWidgetLine1->setStyleSheet("QWidget{background-color:#1B4279;}");

    pWidgetLine2->setFixedSize(1,ROW_HEIGHT * 10);
    pWidgetLine2->move(290 - 250 + 620,158 - MAIN_HEAD_HEIGHT);
    pWidgetLine2->setStyleSheet("QWidget{background-color:#1B4279;}");

    //pLine1->setPoints(QPoint(290 - 250,158 - MAIN_HEAD_HEIGHT),QPoint(290 - 250,158 - MAIN_HEAD_HEIGHT + ROW_HEIGHT * 12));


    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT;
    int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = tr("应用程序版本:");
    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("V4.0 BUILD 675");
    m_label_line1_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_1,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("根文件系统版本:");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("4.0.494");
    m_label_line2_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2_1,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("MCU程序版本:");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("4.0.225");
    m_label_line3_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3_1,alignRight, startY + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("系统资源");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("4.0.675");
    m_label_line4_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4_1,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("系统插件");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0.0.0");
    m_label_line5_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5_1,alignRight, startY + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("内核版本");
    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("4.0.375");
    m_label_line6_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6_1,alignRight,startY + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("引导版本:");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX,startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("4.0.522");
    m_label_line7_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7_1,alignRight, startY + ROW_HEIGHT * 6,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("键盘程序版本:");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("V7.2.0.223");
    m_label_line8_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8_1,alignRight, startY + ROW_HEIGHT * 7,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("设备序列号:");
    m_label_line9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9,startX, startY + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("LTY00100755201365412");
    m_label_line9_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9_1,alignRight, startY + ROW_HEIGHT * 8,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("MCA地址:");
    m_label_line10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line10,startX, startY + ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("64:D9:54:88:B4:98");
    m_label_line10_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line10_1,alignRight, startY + ROW_HEIGHT * 9,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("CAM1-1参数:");
    m_label_line11 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line11,startX, startY + ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("3.6-10000");
    m_label_line11_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line11_1,alignRight, startY + ROW_HEIGHT * 10,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("CAM2-1参数:");
    m_label_line12 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line12,startX, startY + ROW_HEIGHT * 11,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("3.6-11r000");
    m_label_line12_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line12_1,alignRight, startY + ROW_HEIGHT * 11,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
#endif

    m_label_line11->hide();
    m_label_line11_1->hide();
    m_label_line12->hide();
    m_label_line12_1->hide();

    QTimer::singleShot(10,this,SLOT(slot_ablout_machine_init()));



    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);
}

void CAboutMachineWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}


void CAboutMachineWidget::slot_ablout_machine_init()
{
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + 500));
}

void CAboutMachineWidget::scroll_layout()
{

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

#ifdef APPLICATION_TYPE_P2
    m_scroll_widget_width = NAVIGATE_RIGHT_WIDGET_WIDTH - 15;
    m_scroll_widget_height = NAVIGATE_RIGHT_WIDGET_HEIGHT - 50 - 50;
#else
    m_scroll_widget_width = NAVIGATE_RIGHT_WIDGET_WIDTH - 30;
    m_scroll_widget_height = NAVIGATE_RIGHT_WIDGET_HEIGHT - 50 - 50;
#endif
    m_scroll_widget = new QWidget(this);
    m_scroll_widget->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_scroll_widget->move(250 - 250,50);
    m_scroll_widget->setStyleSheet("QWidget{border:0px solid #DD4F43;background: transparent;}");

    m_pMainLayout = new QVBoxLayout();//左 上 右 下
    m_pMainLayout->setContentsMargins(0,0,0,0);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/);
    m_pMainLayout->addStretch(0);
    m_scroll_widget->setLayout(m_pMainLayout);

    m_scrollarea_1=new QScrollArea(m_scroll_widget);  //新建了一条滚动条
    m_scrollarea_1->setFixedHeight(m_define_screen->get_change_factor_y(m_scroll_widget_height));
    m_scrollarea_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollarea_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pMainLayout->addWidget(m_scrollarea_1);

    m_ScrollController = new ScrollController(m_scroll_widget);
    m_ScrollController->addMonitorWidget((QScrollArea *)(m_scrollarea_1));
    m_ScrollController->setVerticalScrollBarStyleSheet(CCommonData::getInstance()->get_scroll_sheet(),0,50,50);
    m_ScrollController->set_scroll_right_length(20);

    QWidget * pwidget=new QWidget(this);

    pwidget->setAttribute(Qt::WA_StyledBackground);
    pwidget->setObjectName("pwidget");
    pwidget->setStyleSheet("QWidget#pwidget { background: transparent;}");

    m_vboxlayout_widget=new QVBoxLayout(pwidget);
    m_vboxlayout_widget->setContentsMargins(0,0,0,0);
    m_vboxlayout_widget->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    m_vboxlayout_widget->setSpacing(0);


    pwidget->setLayout(m_vboxlayout_widget);
    m_scrollarea_1->setWidget(pwidget);
    m_scrollarea_1->setWidgetResizable(true);


    m_widget_main = new QWidget;
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_widget_main->setStyleSheet("QWidget{background: transparent;}");
    m_vboxlayout_widget->addWidget(m_widget_main);

}



CManufactorInfoWidget::CManufactorInfoWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CManufactorInfoWidget");
   this->setStyleSheet("QWidget#CManufactorInfoWidget{background-color:#ffffff;}");


    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");


    QString str_show = tr("网址:lantaiyuan.com");
    m_label_company = new QLabel(this);
    CCommonInterface::init_label_text(m_label_company,380 - 250, 400 - MAIN_HEAD_HEIGHT,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,46);
    CCommonInterface::init_label_text(m_label_company,380 - 250, 400 - MAIN_HEAD_HEIGHT,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,46);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

}

void CManufactorInfoWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

CAboutWidget::CAboutWidget(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(1024,768);
    this->setObjectName("CAboutWidget");
    this->setStyleSheet("QDialog#CAboutWidget{background-color:#000000;}");//444a59
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetAbout,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));

#if 0
    m_widget_bg1 = new QWidget(this);
    m_widget_bg1->setFixedSize(220,76);
    m_widget_bg1->move(0,64);
    m_widget_bg1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/nav_left_bj.png);}");

    m_label_about = new QLabel(this);
    m_label_about->setFixedSize(150,32);
    m_label_about->move(15,64 + (76 - 32)/2);
    m_label_about->setAlignment(Qt::AlignCenter);
    m_label_about->setStyleSheet("QLabel{border:0px;background:transparent;font:30px;color:#1dddff;}");
    m_label_about->setText(tr("关于本机"));
#endif
    int button_height = 78 + 18;;
    int navigate_width = 230;
    int navigate_height = 2 * button_height;

    m_widget_bg2 = new QWidget(this);
    m_widget_bg2->setFixedSize(190,636);
    m_widget_bg2->move(40,107);
    m_widget_bg2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/menu/system_manage/nav_left_background.png);}");


    m_CNavigateLeft = new CNavigateLeft(navigate_width,button_height,navigate_height,this);
    m_CNavigateLeft->move(20,106 + 58 + 10);
    m_CNavigateLeft->setFixedSize(navigate_width,navigate_height);

    m_CNavigateLeft->add_button(CNavigateLeft::kEnumAbountMachine,tr("关于本机"));
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumManufactorInfo,tr("厂家信息"));



#if 1

    m_CAboutMachineWidget = NULL;
    m_CManufactorInfoWidget = NULL;

    m_stackwidget_navigate_right = new QStackedWidget(this);
    m_stackwidget_navigate_right->move(MAIN_NAVIGATE_LEFT,MAIN_HEAD_HEIGHT);
    m_stackwidget_navigate_right->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_stackwidget_navigate_right->setObjectName(QString::fromUtf8("m_stackwidget_navigate_right"));
    m_stackwidget_navigate_right->setContentsMargins(0, 0, 0, 0);

    connect(m_CNavigateLeft,SIGNAL(signalCurrentClassificationChanged(int,QString)),this,SLOT(slot_about_widget_event(int,QString)));

    connect(this,SIGNAL(signal_set_default_id(int)),m_CNavigateLeft,SLOT(slotSetDefaultSelected(int)));

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

    emit signal_set_default_id(CNavigateLeft::kEnumAbountMachine);
#endif

}

void CAboutWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CAboutWidget::slot_pushbutton_home()
{
    m_goto_main_page = true;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_home();
}

void CAboutWidget::slot_pushbutton_back()
{
    m_goto_main_page = false;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_menu();
}

void CAboutWidget::slot_about_widget_event(int _type,QString _text)
{
    m_CHeadWidget->set_head_widget_title(_text);
    switch(_type)
    {
        case CNavigateLeft::kEnumAbountMachine://关于本机
        {
            show_about_machine();
        }break;
        case CNavigateLeft::kEnumManufactorInfo://厂家信息
        {
            show_manufactor_info();
        }break;
    }
}

void CAboutWidget::show_about_machine()
{
    if(NULL == m_CAboutMachineWidget)
    {
        m_CAboutMachineWidget = new CAboutMachineWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CAboutMachineWidget);

    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CAboutMachineWidget);
}

void CAboutWidget::show_manufactor_info()
{
    if(NULL == m_CManufactorInfoWidget)
    {
        m_CManufactorInfoWidget = new CManufactorInfoWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CManufactorInfoWidget);
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CManufactorInfoWidget);
}

void CAboutWidget::slot_about_machine_widget_event(const CAboutMachineInfo &_info)
{
    m_CAboutMachineWidget->m_label_line1_1->setText(_info.m_application_program_version);//应用程序版本
    m_CAboutMachineWidget->m_label_line2_1->setText(_info.m_file_system_version);//根文件系统版本
    m_CAboutMachineWidget->m_label_line3_1->setText(_info.m_mcu_program_version);//MCU程序版本
    m_CAboutMachineWidget->m_label_line4_1->setText(_info.m_system_resouce);//系统资源
    m_CAboutMachineWidget->m_label_line5_1->setText(_info.m_system_plug_in);//系统插件
    m_CAboutMachineWidget->m_label_line6_1->setText(_info.m_kernel_version);//内核版本
    m_CAboutMachineWidget->m_label_line7_1->setText(_info.m_guide_version);//引导版本
    m_CAboutMachineWidget->m_label_line8_1->setText(_info.m_keyboard_program_version);//键盘程序版本
    m_CAboutMachineWidget->m_label_line9_1->setText(_info.m_device_serial_number);//设备序列号
    m_CAboutMachineWidget->m_label_line10_1->setText(_info.m_mac_addr); //MAC地址
    m_CAboutMachineWidget->m_label_line11_1->setText(_info.m_cam1_1_parameter);//CAM1-1参数
    m_CAboutMachineWidget->m_label_line12_1->setText(_info.m_cam2_3_parameter); //CAM2-1参数

}
























