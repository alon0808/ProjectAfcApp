
#include "run_status_widget.h"

#include "class/common_interface.h"
#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"

#include "protocol.h"



CConnectDetails::CConnectDetails(QWidget *parent) :
    QWidget(parent)
{
    QWidget *pRectWidget1 = new QWidget(this);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    QWidget *pWidgetRect2 = new QWidget(this);

    pWidgetRect2->setFixedSize(620,ROW_HEIGHT * 5 + 2);
    pWidgetRect2->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + 77);
    pWidgetRect2->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < 6;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }

    int alignRight = 290 - 250 + 300;

    QString str_show = tr("");
    m_label_line1 = new QLabel(this);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,400,77,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,33);

    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("服务器地址:");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_show = tr("");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("端口");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("");
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("网络类型");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = tr("");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("当日连接次数");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    str_show = tr("0");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("最后连接时间");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    str_show = tr("");
    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this,false);
}

void CConnectDetails::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CConnectDetails::set_title(const QString &_title)
{
    m_label_line1->setText(_title);
}

//1.移动网络
CMobileNetworkWidget::CMobileNetworkWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CMobileNetworkWidget");
   this->setStyleSheet("QWidget#CMobileNetworkWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
    widget_init();

    mCConnectDetails1 = new CConnectDetails(m_widget_main1);
    mCConnectDetails1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    mCConnectDetails1->set_title(tr("中心服务器1"));

    mCConnectDetails2 = new CConnectDetails(m_widget_main2);
    mCConnectDetails2->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    mCConnectDetails2->set_title(tr("中心服务器2"));

    mCConnectDetails3 = new CConnectDetails(m_widget_main3);
    mCConnectDetails3->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    mCConnectDetails3->set_title(tr("视频服务器"));

    QTimer::singleShot(10,this,SLOT(slot_ablout_machine_init()));


    m_define_screen->add_parent(this);

    m_define_screen->add_parent(m_widget_main);
    m_define_screen->add_parent(m_widget_main1);
    m_define_screen->add_parent(m_widget_main2);
    m_define_screen->add_parent(m_widget_main3);

}

void CMobileNetworkWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}


void CMobileNetworkWidget::widget_init()
{
    QWidget *pWidgetRect = new QWidget(m_widget_main);

    pWidgetRect->setFixedSize(620,ROW_HEIGHT * 9 + 1);
    pWidgetRect->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetRect->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < 10;index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }



    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;
    int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = tr("模块状态:");
    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_1,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("SIM卡:");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_2,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("信号质量:");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    QString str_sheet_signal = "QLabel{border:0px solid #8ea2c0;background:transparent;border-image:url(:/img/res/main_page/signal_4.png);}";
    m_label_line1_3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_3,620 - 38 - 10, startY + ROW_HEIGHT * 2 + (ROW_HEIGHT - 42)/2,38,42,"",str_sheet_signal);

    QString str_sheet_signal1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:12px;color:#ffffff;}";
    m_label_line3_2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3_2,620 - 38 - 15, startY + ROW_HEIGHT * 2 + 15,20,20,"4G",str_sheet_signal1);


    str_show = tr("网络类型:");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_4,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("复位次数:");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0");
    m_label_line1_5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_5,alignRight, startY + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#if 0
    str_show = tr("中心服务器1:");
    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("连接成功");
    m_label_line1_6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_6,alignRight, startY + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
#endif
    m_pushbutton_line6 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,m_widget_main);
    m_pushbutton_line6->move(startX - 30,startY +  ROW_HEIGHT * 5);
    m_pushbutton_line6->setDisplayText(tr("中心服务器1:"));
    m_pushbutton_line6->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line6->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line6->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line6->setText1Text2(tr(""),tr(""));
#if 0
    str_show = tr("中心服务器2:");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未使用");
    m_label_line1_7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_7,alignRight, startY + ROW_HEIGHT * 6,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
#endif
    m_pushbutton_line7 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,m_widget_main);
    m_pushbutton_line7->move(startX - 30,startY +  ROW_HEIGHT * 6);
    m_pushbutton_line7->setDisplayText(tr("中心服务器2:"));
    m_pushbutton_line7->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line7->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line7->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line7->setText1Text2(tr(""),tr(""));
#if 0
    str_show = tr("视频服务器:");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("拔号中");
    m_label_line1_8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_8,alignRight, startY + ROW_HEIGHT * 7,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
#endif

    m_pushbutton_line8 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,m_widget_main);
    m_pushbutton_line8->move(startX - 30,startY +  ROW_HEIGHT * 7);
    m_pushbutton_line8->setDisplayText(tr("视频服务器:"));
    m_pushbutton_line8->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line8->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line8->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_pushbutton_line8->setText1Text2(tr(""),tr(""));

    str_show = tr("本月已使用流量:");
    m_label_line9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9,startX, startY + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0M");
    m_label_line1_9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_9,alignRight, startY + ROW_HEIGHT * 8,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    connect(m_pushbutton_line6,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line6()));
    connect(m_pushbutton_line7,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line7()));
    connect(m_pushbutton_line8,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line8()));

}

void CMobileNetworkWidget::slot_pushbutton_line6()
{
    m_widget_main->hide();
    m_widget_main1->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main1);
}
void CMobileNetworkWidget::slot_pushbutton_line7()
{
    m_widget_main->hide();
    m_widget_main2->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main2);
}
void CMobileNetworkWidget::slot_pushbutton_line8()
{
    m_widget_main->hide();
    m_widget_main3->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main3);
}

void CMobileNetworkWidget::slot_pushbutton_return()
{
    if(m_widget_main1->isVisible())
    {
        m_widget_main->show();
        m_widget_main1->hide();
        m_vboxlayout_widget->removeWidget(m_widget_main1);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_main2->isVisible())
    {
        m_widget_main->show();
        m_widget_main2->hide();
        m_vboxlayout_widget->removeWidget(m_widget_main2);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_main3->isVisible())
    {
        m_widget_main->show();
        m_widget_main3->hide();
        m_vboxlayout_widget->removeWidget(m_widget_main3);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else
    {
        emit signal_window_close();
    }
}

void CMobileNetworkWidget::slot_ablout_machine_init()
{
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + 300));
}
void CMobileNetworkWidget::scroll_layout()
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

    m_widget_main1 = new QWidget;
    m_widget_main1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_main1->setStyleSheet("QWidget{background: transparent;}");

    m_widget_main2 = new QWidget;
    m_widget_main2->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_main2->setStyleSheet("QWidget{background: transparent;}");

    m_widget_main3 = new QWidget;
    m_widget_main3->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_main3->setStyleSheet("QWidget{background: transparent;}");

    m_widget_main1->hide();
    m_widget_main2->hide();
    m_widget_main3->hide();

    m_vboxlayout_widget->addWidget(m_widget_main);

}

//2.开关量状态
CSwitchStatusWidget::CSwitchStatusWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CMobileNetworkWidget");
   this->setStyleSheet("QWidget#CMobileNetworkWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
    widget_init();


#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
    QTimer::singleShot(10,this,SLOT(slot_ablout_machine_init()));
#endif

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);

}

void CSwitchStatusWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSwitchStatusWidget::widget_init()
{
    QWidget *pWidgetRect = new QWidget(m_widget_main);
    int row_number = 0;


#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    row_number = 4;
#else
    row_number = 12 - 3;
#endif

    pWidgetRect->setFixedSize(620,ROW_HEIGHT * row_number + 1);
    pWidgetRect->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetRect->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < (row_number + 1);index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }


    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;
    int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = tr("前门:");
    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_1,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("中门:");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_2,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("后门:");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_3,alignRight, startY + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("I/O输出1:");
    m_label_line9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_9,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#else
    str_show = tr("倒车:");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_4,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("刹车:");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_5,alignRight, startY + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("雨刮:");
    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_6,alignRight, startY + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("左转灯:");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_7,alignRight, startY + ROW_HEIGHT * 6,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("右转灯:");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_8,alignRight, startY + ROW_HEIGHT * 7,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("I/O输出1:");
    m_label_line9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9,startX, startY + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_9,alignRight, startY + ROW_HEIGHT * 8,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("I/O输出2:");
    m_label_line10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line10,startX, startY + ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_10,alignRight, startY + ROW_HEIGHT * 9,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("报警输入");
    m_label_line11 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line11,startX, startY + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_11 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_11,alignRight, startY + ROW_HEIGHT * 8,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("报警输入2:");
    m_label_line12 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line12,startX, startY + ROW_HEIGHT * 11,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_12 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_12,alignRight, startY + ROW_HEIGHT * 11,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    m_label_line9->hide();
    m_label_line1_9->hide();
    m_label_line10->hide();
    m_label_line1_10->hide();
    m_label_line12->hide();
    m_label_line1_12->hide();

#endif

}
void CSwitchStatusWidget::slot_ablout_machine_init()
{
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 500);
}
void CSwitchStatusWidget::scroll_layout()
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
    m_scrollarea_1->setFixedHeight(m_scroll_widget_height);
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

//3.定位模块
CLocationModularWidget::CLocationModularWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CMobileNetworkWidget");
   this->setStyleSheet("QWidget#CMobileNetworkWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
    widget_init();


    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);

    QTimer::singleShot(10,this,SLOT(slot_ablout_machine_init()));


}

void CLocationModularWidget::slot_ablout_machine_init()
{
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 10));
}


void CLocationModularWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CLocationModularWidget::scroll_layout()
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

void CLocationModularWidget::widget_init()
{
    QWidget *pWidgetRect = new QWidget(m_widget_main);

//#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
    int row_number = 6 + 13 - 5;
//#else
//    int row_number = 6;
//#endif

    pWidgetRect->setFixedSize(620,ROW_HEIGHT * row_number + 1);
    pWidgetRect->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetRect->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < (row_number + 1);index++)
    {
        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }

    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;
    int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = tr("卫星颗数:");
    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0");
    m_label_line1_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_1,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("经度:");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_2,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("纬度:");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_3,alignRight, startY + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("方位角:");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_4,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("海拔高度:");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_5,alignRight, startY + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("实时速度:");
    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0km/h");
    m_label_line1_6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_6,alignRight, startY + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

//#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
    str_show = tr("固件版本:");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("V1.0");
    m_label_line1_7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_7,alignRight, startY + ROW_HEIGHT * 6,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("db值>40");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0");
    m_label_line1_8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_8,alignRight, startY + ROW_HEIGHT * 7,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("卫星1~4:");
    m_label_line9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9,startX, startY + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0db");
    m_label_line1_9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_9,alignRight, startY + ROW_HEIGHT * 8,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("卫星4~8:");
    m_label_line10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line10,startX, startY + ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0db");
    m_label_line1_10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_10,alignRight, startY + ROW_HEIGHT * 9,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("卫星9~12:");
    m_label_line11 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line11,startX, startY + ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0db");
    m_label_line1_11 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_11,alignRight, startY + ROW_HEIGHT * 10,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("卫星13~16:");
    m_label_line12 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line12,startX, startY + ROW_HEIGHT * 11,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0db");
    m_label_line1_12 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_12,alignRight, startY + ROW_HEIGHT * 11,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("卫星17~20:");
    m_label_line13 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line13,startX, startY + ROW_HEIGHT * 12,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0db");
    m_label_line1_13 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_13,alignRight, startY + ROW_HEIGHT * 12,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("卫星21~24:");
    m_label_line14 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line14,startX, startY + ROW_HEIGHT * 13,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("0db");
    m_label_line1_14 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_14,alignRight, startY + ROW_HEIGHT * 13,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("卫星8:");
    m_label_line15 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line15,startX, startY + ROW_HEIGHT * 14,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);
    m_label_line15->hide();

    str_show = tr("0db");
    m_label_line1_15 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_15,alignRight, startY + ROW_HEIGHT * 14,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
    m_label_line1_15->hide();

    str_show = tr("卫星9:");
    m_label_line16 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line16,startX, startY + ROW_HEIGHT * 15,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);
    m_label_line16->hide();

    str_show = tr("0db");
    m_label_line1_16 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_16,alignRight, startY + ROW_HEIGHT * 15,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
    m_label_line1_16->hide();

    str_show = tr("卫星10:");
    m_label_line17 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line17,startX, startY + ROW_HEIGHT * 16,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);
    m_label_line17->hide();

    str_show = tr("0db");
    m_label_line1_17 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_17,alignRight, startY + ROW_HEIGHT * 16,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
    m_label_line1_17->hide();

    str_show = tr("卫星11:");
    m_label_line18 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line18,startX, startY + ROW_HEIGHT * 17,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);
    m_label_line18->hide();


    str_show = tr("0db");
    m_label_line1_18 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_18,alignRight, startY + ROW_HEIGHT * 17,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
    m_label_line1_18->hide();

    str_show = tr("卫星12:");
    m_label_line19 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line19,startX, startY + ROW_HEIGHT * 18,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);
    m_label_line19->hide();

    str_show = tr("0db");
    m_label_line1_19 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_19,alignRight, startY + ROW_HEIGHT * 18,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);
    m_label_line1_19->hide();


//#endif
}

//4.本地网络
CLocalNetworkWidget::CLocalNetworkWidget(QWidget *parent) :
    QWidget(parent)
{

    m_define_screen = new CMyDefineScreen();

   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CMobileNetworkWidget");
   this->setStyleSheet("QWidget#CMobileNetworkWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
    widget_init();

#if(defined(APPLICATION_TYPE_D2))
#elif (defined(APPLICATION_TYPE_P2))
    QTimer::singleShot(10,this,SLOT(slot_ablout_machine_init()));
#else
    QTimer::singleShot(10,this,SLOT(slot_ablout_machine_init()));
#endif


    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);

}

void CLocalNetworkWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}


void CLocalNetworkWidget::widget_init()
{
    QWidget *pWidgetRect = new QWidget(m_widget_main);

    int row_number = 0;

#if(defined(APPLICATION_TYPE_D2))
    row_number = 6;
#elif (defined(APPLICATION_TYPE_P2))
    row_number = 8;
#else
    row_number = 10;
#endif
    pWidgetRect->setFixedSize(620,ROW_HEIGHT * row_number + 1);
    pWidgetRect->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetRect->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < (row_number + 1);index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }


    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;
    int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = "";

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("本地网络");
#else
    str_show = tr("PON1:");
#endif

    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("");
#else
    str_show = tr("");
#endif

    m_label_line1_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_1,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("IP地址");
#else
    str_show = tr("PON2:");
#endif

    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("");
#else
    str_show = tr("");
#endif

    m_label_line1_2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_2,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("子网掩码");
#else
    str_show = tr("PON3:");
#endif

    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("");
#else
    str_show = tr("");
#endif

    m_label_line1_3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_3,alignRight, startY + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("网关");
#else
    str_show = tr("PON4:");
#endif

    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("");
#else
    str_show = tr("");
#endif

    m_label_line1_4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_4,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("DNS1");
#else
    str_show = tr("PON5:");
#endif

    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("");
#else
    str_show = tr("");
#endif

    m_label_line1_5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_5,alignRight, startY + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("DNS2");
#else
    str_show = tr("PON6:");
#endif

    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("");
#else
    str_show = tr("");
#endif

    m_label_line1_6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_6,alignRight, startY + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#ifdef APPLICATION_TYPE_P2
    str_show = tr("域socket状态");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("正常");
    m_label_line1_7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_7,alignRight, startY + ROW_HEIGHT * 6,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("主从机状态");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_8,alignRight, startY + ROW_HEIGHT * 7,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


#endif

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
    str_show = tr("PON7:");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_7,alignRight, startY + ROW_HEIGHT * 6,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("PON8:");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_8,alignRight, startY + ROW_HEIGHT * 7,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("PON9:");
    m_label_line9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9,startX, startY + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_9,alignRight, startY + ROW_HEIGHT * 8,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("PON10:");
    m_label_line10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line10,startX, startY + ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line1_10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_10,alignRight, startY + ROW_HEIGHT * 9,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#endif


}
void CLocalNetworkWidget::slot_ablout_machine_init()
{
#ifdef APPLICATION_TYPE_P2
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_x(m_scroll_widget_height + 200));
#else
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 400);
#endif

}




void CLocalNetworkWidget::scroll_layout()
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

//5.设备状态
CDeviceStatusWidget::CDeviceStatusWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CMobileNetworkWidget");
   this->setStyleSheet("QWidget#CMobileNetworkWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
    widget_init();

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
    //QTimer::singleShot(10,this,SLOT(slot_ablout_machine_init()));
#endif

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(m_widget_main);
    m_define_screen->add_parent(m_widget_main2);
    m_define_screen->add_parent(this);

}

void CDeviceStatusWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CDeviceStatusWidget::widget_init()
{
    QWidget *pWidgetRect = new QWidget(m_widget_main);
    int row_number = 0;

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    row_number = 3;
#else
    row_number = 8 - 2;
#endif

    pWidgetRect->setFixedSize(620,ROW_HEIGHT * row_number + 1);
    pWidgetRect->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetRect->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < (row_number + 1);index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }


    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;
    int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = tr("设备门:");


#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("输入电压:");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("12V");
    m_label_line1_3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_3,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("设备温度:");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("60℃");
    m_label_line1_4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_4,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#else
    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("开启");
    m_label_line1_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_1,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("风扇转速:");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("1800r/min");
    m_label_line1_2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_2,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("输入电压:");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("12V");
    m_label_line1_3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_3,alignRight, startY + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("设备温度:");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("60℃");
    m_label_line1_4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_4,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("车外温度:");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("３0℃");
    m_label_line1_5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_5,alignRight, startY + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("车内温度:");
    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("26℃");
    m_label_line1_6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_6,alignRight, startY + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#endif

    m_button_line7 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,m_widget_main);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_button_line7->move(startX - 30,startY +  ROW_HEIGHT * 2);
    m_button_line7->hide();
#else
    m_button_line7->move(startX - 30,startY +  ROW_HEIGHT * 6);
#endif
    m_button_line7->setDisplayText(tr("串口状态:"));
    m_button_line7->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line7->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line7->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line8 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,m_widget_main);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_button_line8->move(startX - 30, startY + ROW_HEIGHT * 2);
#else
    m_button_line8->move(startX - 30, startY + ROW_HEIGHT * 7);

#endif

    m_button_line8->setDisplayText(tr("CAN状态:"));
    m_button_line8->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line8->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line8->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    connect(m_button_line7,SIGNAL(clicked()),this,SLOT(slot_button_serial_port_status()));
    connect(m_button_line8,SIGNAL(clicked()),this,SLOT(slot_button_can_status()));
    m_button_line7->hide();
    m_button_line8->hide();
}

void CDeviceStatusWidget::slot_button_serial_port_status()
{
    m_widget_main->hide();
    m_widget_main1->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main1);

}
void CDeviceStatusWidget::slot_button_can_status()
{
    m_widget_main->hide();
    m_widget_main2->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main2);
}

void CDeviceStatusWidget::slot_pushbutton_return()
{
    if(m_widget_main1->isVisible())
    {
        m_widget_main->show();
        m_widget_main1->hide();
        m_vboxlayout_widget->removeWidget(m_widget_main1);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_main2->isVisible())
    {
        m_widget_main->show();
        m_widget_main2->hide();
        m_vboxlayout_widget->removeWidget(m_widget_main2);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else
    {
        emit signal_window_close();
    }
}


void CDeviceStatusWidget::slot_ablout_machine_init()
{
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 200);
}
void CDeviceStatusWidget::scroll_layout()
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
    m_scrollarea_1->setFixedHeight(m_scroll_widget_height);
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

    m_widget_main1 = new QWidget;
    m_widget_main1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_widget_main1->setStyleSheet("QWidget{background: transparent;}");
    m_widget_main1->hide();

    m_widget_main2 = new QWidget;
    m_widget_main2->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_widget_main2->setStyleSheet("QWidget{background: transparent;}");
    m_widget_main2->hide();

    serial_port_init();
    can_init();
}

void CDeviceStatusWidget::serial_port_init()
{
    QWidget *pRectWidget1 = new QWidget(m_widget_main1);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    QWidget *pWidgetRect2 = new QWidget(m_widget_main1);

    pWidgetRect2->setFixedSize(620,ROW_HEIGHT * 5 + 2);
    pWidgetRect2->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + 77);
    pWidgetRect2->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < 7;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_main1);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }

    int alignRight = 290 - 250 + 300;
   // m_label_main1_line1
    QString str_show = tr("串口状态");
    m_label_main1_line1 = new QLabel(m_widget_main1);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_main1_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("串口1");
    m_label_main1_line2 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_show = tr("连接");
    m_label_main1_line2_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line2_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);



    str_show = tr("串口2");
    m_label_main1_line3 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("连接");
    m_label_main1_line3_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line3_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("串口3");
    m_label_main1_line4 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line4,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = tr("连接");
    m_label_main1_line4_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line4_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("串口4");
    m_label_main1_line5 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line5,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    str_show = tr("未连接");
    m_label_main1_line5_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line5_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("串口5");
    m_label_main1_line6 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line6,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    str_show = tr("未连接");
    m_label_main1_line6_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line6_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


}

void CDeviceStatusWidget::can_init()
{
    QWidget *pRectWidget1 = new QWidget(m_widget_main2);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");
    int line_number = 4;
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    line_number = 3;
#endif

    QWidget *pWidgetRect2 = new QWidget(m_widget_main2);

    pWidgetRect2->setFixedSize(620,ROW_HEIGHT * (line_number - 2) + 2);
    pWidgetRect2->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + 77);
    pWidgetRect2->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < line_number;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_main2);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
#if 0
    QWidget *pLine1 = new QWidget(m_widget_main2);
    QWidget *pLine2 = new QWidget(m_widget_main2);
     pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,2 * ROW_HEIGHT);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,2 * ROW_HEIGHT);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
#endif
    int alignRight = 290 - 250 + 300;
   // m_label_main1_line1
    QString str_show = tr("CAN状态");
    m_label_main2_line1 = new QLabel(m_widget_main2);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_main2_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("CAN1");
    m_label_main2_line2 = new QLabel(m_widget_main2);
    CCommonInterface::init_label_text(m_label_main2_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_show = tr("连接");
    m_label_main2_line2_1 = new QLabel(m_widget_main2);
    CCommonInterface::init_label_text(m_label_main2_line2_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);



    str_show = tr("CAN2");
    m_label_main2_line3 = new QLabel(m_widget_main2);
    CCommonInterface::init_label_text(m_label_main2_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("断开");
    m_label_main2_line3_1 = new QLabel(m_widget_main2);
    CCommonInterface::init_label_text(m_label_main2_line3_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_label_main2_line3->hide();
    m_label_main2_line3_1->hide();
#endif


}
////////////////////////////////////////////////////////////////////////////////////////
//外设(L12)
CPeripheralWidget::CPeripheralWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CPeripheralWidget");
   this->setStyleSheet("QWidget#CPeripheralWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
    widget_init();

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(m_widget_main);
    m_define_screen->add_parent(this);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
    QTimer::singleShot(10,this,SLOT(slot_peripheral_init()));
#endif


}

void CPeripheralWidget::slot_peripheral_init()
{
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + 400));
}


void CPeripheralWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CPeripheralWidget::widget_init()
{
    QWidget *pWidgetRect = new QWidget(m_widget_main);
    int row_number = 10;

    pWidgetRect->setFixedSize(620,ROW_HEIGHT * row_number + 1);
    pWidgetRect->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetRect->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < (row_number + 1);index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }


    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;
    int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = tr("探头1");

    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line1_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_1,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("探头2");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line2_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2_1,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("探头3");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line3_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3_1,alignRight, startY + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("探头4");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line4_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4_1,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("探头5");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line5_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5_1,alignRight, startY + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("探头6");
    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line6_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6_1,alignRight, startY + ROW_HEIGHT * 5,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("探头7");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line7_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7_1,alignRight, startY + ROW_HEIGHT * 6,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("探头8");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line8_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8_1,alignRight, startY + ROW_HEIGHT * 7,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("探头9");
    m_label_line9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9,startX, startY + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("未知");
    m_label_line9_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9_1,alignRight, startY + ROW_HEIGHT * 8,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("手动报警");
    m_label_line10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line10,startX, startY + ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("断开");
    m_label_line10_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line10_1,alignRight, startY + ROW_HEIGHT * 9,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


}


void CPeripheralWidget::slot_pushbutton_return()
{
     emit signal_window_close();
}



void CPeripheralWidget::scroll_layout()
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
    m_scrollarea_1->setFixedHeight(m_scroll_widget_height);
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

////////////////////////////////////////////////////////////////////////////////////////
#if 1
//6.站点信息
CStationInfoWidget::CStationInfoWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CStationInfoWidget");
   this->setStyleSheet("QWidget#CStationInfoWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
    widget_init();

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(m_widget_main);
    m_define_screen->add_parent(this);

}

void CStationInfoWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CStationInfoWidget::widget_init()
{
    QWidget *pWidgetRect = new QWidget(m_widget_main);
    int row_number = 5;

    pWidgetRect->setFixedSize(620,ROW_HEIGHT * row_number + 1);
    pWidgetRect->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetRect->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < (row_number + 1);index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }


    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;
    int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = tr("起始站:");

    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("测试1");
    m_label_line1_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1_1,alignRight, startY + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("终点站:");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("测试2");
    m_label_line2_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2_1,alignRight, startY + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("当前站:");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("测试2");
    m_label_line3_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3_1,alignRight, startY + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("状态:");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("上行");
    m_label_line4_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4_1,alignRight, startY + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("站号:");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("");
    m_label_line5_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5_1,alignRight, startY + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


}


void CStationInfoWidget::slot_pushbutton_return()
{
     emit signal_window_close();
}



void CStationInfoWidget::scroll_layout()
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
    m_scrollarea_1->setFixedHeight(m_scroll_widget_height);
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
#endif


////////////////////////////////////////////////////////////////////////////////////////


CRunStatusWidget::CRunStatusWidget(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(1024,768);
    this->setObjectName("CAboutWidget");
    this->setStyleSheet("QDialog#CAboutWidget{background-color:#000000;}");//444a59
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetRunStatus,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));

    int button_height = 78 + 18;;
    int navigate_width = 230;
    int navigate_height = 5 * button_height;

    m_widget_bg2 = new QWidget(this);
    m_widget_bg2->setFixedSize(190,636);
    m_widget_bg2->move(40,107);
    m_widget_bg2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/menu/system_manage/nav_left_background.png);}");


    m_CNavigateLeft = new CNavigateLeft(navigate_width,button_height,navigate_height,this);
    m_CNavigateLeft->move(20,106 + 58 + 10);
    m_CNavigateLeft->setFixedSize(navigate_width,navigate_height);

    m_CNavigateLeft->add_button(CNavigateLeft::kEnumMobileNetwork,tr("移动网络"));
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumSwitchStatus,tr("I/O"));
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumLocationModular,tr("定位模块"));
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumLocalNetwork,tr("本地网络"));
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumDeviceStatus,tr("设备"));
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumPeripheral,tr("外设"));
#endif

    m_CNavigateLeft->add_button(CNavigateLeft::kEnumStationInfo,tr("站点信息"));


    m_pushbutton_up = new QPushButton(this);
    QString str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/nav_left_button_up_default.png").arg(":/img/res/main_page/menu/system_manage/nav_left_button_up_selected.png").arg(":/img/res/main_page/menu/system_manage/nav_left_button_up_selected.png");

    CCommonInterface::init_new_button(m_pushbutton_up,106,107,58,58,str_sheet);


    m_pushbutton_down = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/nav_left_button_down_default.png").arg(":/img/res/main_page/menu/system_manage/nav_left_button_down_selected.png").arg(":/img/res/main_page/menu/system_manage/nav_left_button_down_selected.png");

    CCommonInterface::init_new_button(m_pushbutton_down,106,666,58,58,str_sheet);


    m_CMobileNetworkWidget = NULL;       //1.移动网络
    m_CSwitchStatusWidget = NULL;         //2.开关量
    m_CLocationModularWidget = NULL;   //3.定位模块
    m_CLocalNetworkWidget = NULL;         //4.本地网络
    m_CDeviceStatusWidget = NULL;         //5.设备
    m_CStationInfoWidget = NULL;
    m_CPeripheralWidget = NULL;

    m_stackwidget_navigate_right = new QStackedWidget(this);
    m_stackwidget_navigate_right->move(MAIN_NAVIGATE_LEFT,MAIN_HEAD_HEIGHT);
    m_stackwidget_navigate_right->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_stackwidget_navigate_right->setObjectName(QString::fromUtf8("m_stackwidget_navigate_right"));
    m_stackwidget_navigate_right->setContentsMargins(0, 0, 0, 0);

    connect(m_CNavigateLeft,SIGNAL(signalCurrentClassificationChanged(int,QString)),this,SLOT(slot_run_status_widget_event(int,QString)));

    connect(this,SIGNAL(signal_set_default_id(int)),m_CNavigateLeft,SLOT(slotSetDefaultSelected(int)));

    connect(m_pushbutton_up,SIGNAL(clicked()),this,SLOT(slot_pushbutton_up()));
    connect(m_pushbutton_down,SIGNAL(clicked()),this,SLOT(slot_pushbutton_down()));

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

    emit signal_set_default_id(CNavigateLeft::kEnumMobileNetwork);

}

void CRunStatusWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CRunStatusWidget::slot_pushbutton_home()
{
    m_goto_main_page = true;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_home();

}

void CRunStatusWidget::slot_pushbutton_close()
{
    m_goto_main_page = false;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_menu();
}

void CRunStatusWidget::slot_pushbutton_up()
{
    m_CNavigateLeft->set_scroll_value(false);
}
void CRunStatusWidget::slot_pushbutton_down()
{
    m_CNavigateLeft->set_scroll_value(true);
}

void CRunStatusWidget::slot_pushbutton_back()
{
    if(CNavigateLeft::kEnumDeviceStatus == m_current_index)
    {
        emit signal_device_status_user_return();
    }
    else if(CNavigateLeft::kEnumMobileNetwork == m_current_index)
    {
        emit signal_mobile_network_user_return();
    }

    else
    {
        m_goto_main_page = false;
        CCommonInterface::input_panel_control(false);
        emit signal_pushbutton_menu();
    }
}


void CRunStatusWidget::slot_run_status_widget_event(int _type,QString _text)
{
    m_CHeadWidget->set_head_widget_title(_text);
    m_current_index = _type;
    switch(_type)
    {
        case CNavigateLeft::kEnumMobileNetwork:          //移动网络
        {
            show_mobile_network();
        }break;
        case CNavigateLeft::kEnumSwitchStatus:          //开关量
        {
            show_switch_status();
        }break;
        case CNavigateLeft::kEnumLocationModular:         //网络状态
        {
            show_location_modular();
        }break;
        case CNavigateLeft::kEnumLocalNetwork:         //存储状态
        {
            show_local_network();
        }break;
        case CNavigateLeft::kEnumDeviceStatus:          //设备
        {
            show_device_status();
        }break;
        case CNavigateLeft::kEnumStationInfo:
        {
            show_station_info();
        }break;
        case CNavigateLeft::kEnumPeripheral:
        {
            show_peripheral();
        }break;
    }

}

//1.移动网络
void CRunStatusWidget::show_mobile_network()
{
    if(NULL == m_CMobileNetworkWidget)
    {
        m_CMobileNetworkWidget = new CMobileNetworkWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CMobileNetworkWidget);
        connect(m_CMobileNetworkWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_mobile_network_user_return()),m_CMobileNetworkWidget,SLOT(slot_pushbutton_return()));

        CProtocol::getInstance()->call_json_run_status_set(CProtocol::kEnumRunStatusMobileNetwork);
        CCommonData::getInstance()->m_runStatusInfo.m_type = 1;
        slot_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);

    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CMobileNetworkWidget);
}



//2.开关量
void CRunStatusWidget::show_switch_status()
{
    if(NULL == m_CSwitchStatusWidget)
    {
        m_CSwitchStatusWidget = new CSwitchStatusWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CSwitchStatusWidget);

        //CProtocol::getInstance()->call_json_run_status_set(CProtocol::kEnumRunStatusIO);
        CCommonData::getInstance()->m_runStatusInfo.m_type = CProtocol::kEnumRunStatusIO;

        slot_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
    }

    m_stackwidget_navigate_right->setCurrentWidget(m_CSwitchStatusWidget);
}
//3.定位模块
void CRunStatusWidget::show_location_modular()
{
     if(NULL == m_CLocationModularWidget)
    {
        m_CLocationModularWidget = new CLocationModularWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CLocationModularWidget);

        //CProtocol::getInstance()->call_json_run_status_set(CProtocol::kEnumRunStatusGPS);
        CCommonData::getInstance()->m_runStatusInfo.m_type = 3;
        slot_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CLocationModularWidget);
}
//4.本地网络
void CRunStatusWidget::show_local_network()
{
     if(NULL == m_CLocalNetworkWidget)
    {
        m_CLocalNetworkWidget = new CLocalNetworkWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CLocalNetworkWidget);


        CCommonData::getInstance()->m_runStatusInfo.m_type = 4;
        slot_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
    }
    CProtocol::getInstance()->call_json_run_status_set(CProtocol::kEnumRunStatusLocalNetwork);
    m_stackwidget_navigate_right->setCurrentWidget(m_CLocalNetworkWidget);
}
//5.设备
void CRunStatusWidget::show_device_status()
{
    if(NULL == m_CDeviceStatusWidget)
    {
        m_CDeviceStatusWidget = new CDeviceStatusWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CDeviceStatusWidget);
        connect(m_CDeviceStatusWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_device_status_user_return()),m_CDeviceStatusWidget,SLOT(slot_pushbutton_return()));

        CProtocol::getInstance()->call_json_run_status_set(CProtocol::kEnumRunStatusDevice);
        CCommonData::getInstance()->m_runStatusInfo.m_type = 5;
        slot_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CDeviceStatusWidget);
}

void CRunStatusWidget::show_peripheral()
{
    if(NULL == m_CPeripheralWidget)
    {
        m_CPeripheralWidget = new CPeripheralWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CPeripheralWidget);
        connect(m_CPeripheralWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        //connect(this,SIGNAL(signal_device_status_user_return()),m_CPeripheralWidget,SLOT(slot_pushbutton_return()));
        //CProtocol::getInstance()->call_json_run_status_set(CProtocol::kEnumRunStatusPeripheral);
        CCommonData::getInstance()->m_runStatusInfo.m_type = 7;
        slot_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CPeripheralWidget);
}


void CRunStatusWidget::show_station_info()
{
    if(NULL == m_CStationInfoWidget)
    {
        m_CStationInfoWidget = new CStationInfoWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CStationInfoWidget);
        connect(m_CStationInfoWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_device_status_user_return()),m_CStationInfoWidget,SLOT(slot_pushbutton_return()));

        CCommonData::getInstance()->m_runStatusInfo.m_type = 6;
        slot_run_status_widget_event(CCommonData::getInstance()->m_runStatusInfo);
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CStationInfoWidget);
}
#include <QDebug>
void CRunStatusWidget::slot_run_status_widget_event(const CRunStatusInfo &_runStatusInfo)
{
    //qDebug() << "slot_run_status_widget_event() type:" << QString::number(_runStatusInfo.m_type);

    switch(_runStatusInfo.m_type)
    {
        case CProtocol::kEnumRunStatusMobileNetwork://移动网络
        {
            if(NULL == _runStatusInfo.m_CRunStatusMobileNetwork)
            {
                break;
            }
            if(NULL == m_CMobileNetworkWidget)
            {
                break;
            }
            if(1 == _runStatusInfo.m_CRunStatusMobileNetwork->m_model_status)
                m_CMobileNetworkWidget->m_label_line1_1->setText(tr("正常"));
            else
                m_CMobileNetworkWidget->m_label_line1_1->setText(tr("不正常"));

            if(1 == _runStatusInfo.m_CRunStatusMobileNetwork->m_sim_status)
                m_CMobileNetworkWidget->m_label_line1_2->setText(tr("存在"));
            else
                m_CMobileNetworkWidget->m_label_line1_2->setText(tr("不存在"));


            QString str_sheet = "";
            bool show_4G = true;

             if(1 == _runStatusInfo.m_CRunStatusMobileNetwork->m_signal_status)
                str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;border-image:url(:/img/res/main_page/signal_1.png);}";
            else if(2 == _runStatusInfo.m_CRunStatusMobileNetwork->m_signal_status)
                str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;border-image:url(:/img/res/main_page/signal_2.png);}";
            else if(3 == _runStatusInfo.m_CRunStatusMobileNetwork->m_signal_status)
                str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;border-image:url(:/img/res/main_page/signal_3.png);}";
            else if(4 == _runStatusInfo.m_CRunStatusMobileNetwork->m_signal_status)
                str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;border-image:url(:/img/res/main_page/signal_4.png);}";
            else if(5 == _runStatusInfo.m_CRunStatusMobileNetwork->m_signal_status)
            {
                show_4G = false;
                str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;border-image:url(:/img/res/main_page/signal_5.png);}";
            }
            else
            {
                show_4G = false;
                str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;border-image:url(:/img/res/main_page/signal.png);}";
            }


            m_CMobileNetworkWidget->m_label_line1_3->setStyleSheet(str_sheet);

            if(show_4G)
                m_CMobileNetworkWidget->m_label_line3_2->show();
            else
                m_CMobileNetworkWidget->m_label_line3_2->hide();

            m_CMobileNetworkWidget->m_label_line1_4->setText(_runStatusInfo.m_CRunStatusMobileNetwork->m_network_type);

            m_CMobileNetworkWidget->m_label_line1_5->setText(QString::number(_runStatusInfo.m_CRunStatusMobileNetwork->m_reset_times));

            //中心服务器1   //1:未使用 2:拔号中 3:连接成功
            if(3 == _runStatusInfo.m_CRunStatusMobileNetwork->center_server1_status)
                m_CMobileNetworkWidget->m_pushbutton_line6->setText1Text2(tr("连接成功"),tr(""));
            else if(2 == _runStatusInfo.m_CRunStatusMobileNetwork->center_server1_status)
                m_CMobileNetworkWidget->m_pushbutton_line6->setText1Text2(tr("拔号中"),tr(""));
            else
                m_CMobileNetworkWidget->m_pushbutton_line6->setText1Text2(tr("未使用"),tr(""));

            m_CMobileNetworkWidget->mCConnectDetails1->m_label_line2_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_addr);
            m_CMobileNetworkWidget->mCConnectDetails1->m_label_line3_1->setText(QString::number(_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_port));
            m_CMobileNetworkWidget->mCConnectDetails1->m_label_line4_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_type);
            m_CMobileNetworkWidget->mCConnectDetails1->m_label_line5_1->setText(QString::number(_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_connect_times));
            m_CMobileNetworkWidget->mCConnectDetails1->m_label_line6_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->center_server1_last_connect);

            //中心服务器2   //1:未使用 2:拔号中 3:连接成功

            qDebug() << "center_server2_status:" << QString::number(_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status);

            if(3 == _runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status)
                m_CMobileNetworkWidget->m_pushbutton_line7->setText1Text2(tr("连接成功"),tr(""));
            else if(2 == _runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status)
                m_CMobileNetworkWidget->m_pushbutton_line7->setText1Text2(tr("拔号中"),tr(""));
            else
                m_CMobileNetworkWidget->m_pushbutton_line7->setText1Text2(tr("未使用"),tr(""));

            m_CMobileNetworkWidget->mCConnectDetails2->m_label_line2_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_addr);
            m_CMobileNetworkWidget->mCConnectDetails2->m_label_line3_1->setText(QString::number(_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_port));
            m_CMobileNetworkWidget->mCConnectDetails2->m_label_line4_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_type);
            m_CMobileNetworkWidget->mCConnectDetails2->m_label_line5_1->setText(QString::number(_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_connect_times));
            m_CMobileNetworkWidget->mCConnectDetails2->m_label_line6_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_last_connect);

            //视频服务器   //1:未使用 2:拔号中 3:连接成功
            if(3 == _runStatusInfo.m_CRunStatusMobileNetwork->video_server_status)
                m_CMobileNetworkWidget->m_pushbutton_line8->setText1Text2(tr("连接成功"),tr(""));
            else if(2 == _runStatusInfo.m_CRunStatusMobileNetwork->video_server_status)
                m_CMobileNetworkWidget->m_pushbutton_line8->setText1Text2(tr("拔号中"),tr(""));
            else
                m_CMobileNetworkWidget->m_pushbutton_line8->setText1Text2(tr("未使用"),tr(""));

            m_CMobileNetworkWidget->mCConnectDetails3->m_label_line2_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->video_server_addr);
            m_CMobileNetworkWidget->mCConnectDetails3->m_label_line3_1->setText(QString::number(_runStatusInfo.m_CRunStatusMobileNetwork->video_server_port));
            m_CMobileNetworkWidget->mCConnectDetails3->m_label_line4_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->video_server_type);
            m_CMobileNetworkWidget->mCConnectDetails3->m_label_line5_1->setText(QString::number(_runStatusInfo.m_CRunStatusMobileNetwork->video_server_connect_times));
            m_CMobileNetworkWidget->mCConnectDetails3->m_label_line6_1->setText(_runStatusInfo.m_CRunStatusMobileNetwork->video_server_last_connect);

            //本月已使用流量
            m_CMobileNetworkWidget->m_label_line1_9->setText(_runStatusInfo.m_CRunStatusMobileNetwork->m_use_flow);


        }break;
        case CProtocol::kEnumRunStatusIO:           //I/O
        {
            if(NULL == _runStatusInfo.m_CRunStatusIO)
            {
                break;
            }
            if(NULL == m_CSwitchStatusWidget)
            {
                break;
            }

            QString str_start = tr("开启");
            QString str_end = tr("关闭");

            if(1 == _runStatusInfo.m_CRunStatusIO->front_door_status)//前门
                m_CSwitchStatusWidget->m_label_line1_1->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_1->setText(str_end);

            if(1 == _runStatusInfo.m_CRunStatusIO->middle_door_status)//中门
                m_CSwitchStatusWidget->m_label_line1_2->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_2->setText(str_end);


            if(1 == _runStatusInfo.m_CRunStatusIO->back_door_status)//后门
                m_CSwitchStatusWidget->m_label_line1_3->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_3->setText(str_end);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
            if(1 == _runStatusInfo.m_CRunStatusIO->back_car_status)//倒车
                m_CSwitchStatusWidget->m_label_line1_4->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_4->setText(str_end);


            if(1 == _runStatusInfo.m_CRunStatusIO->m_stop_car_status)//刹车
                m_CSwitchStatusWidget->m_label_line1_5->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_5->setText(str_end);


            if(1 == _runStatusInfo.m_CRunStatusIO->m_wiper_status)//雨刮
                m_CSwitchStatusWidget->m_label_line1_6->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_6->setText(str_end);

            if(1 == _runStatusInfo.m_CRunStatusIO->m_left_lamp_status)//左转灯
                m_CSwitchStatusWidget->m_label_line1_7->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_7->setText(str_end);

            if(1 == _runStatusInfo.m_CRunStatusIO->m_right_lamp_status)//右转灯
                m_CSwitchStatusWidget->m_label_line1_8->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_8->setText(str_end);
#endif

            if(1 == _runStatusInfo.m_CRunStatusIO->m_io1_status)//I/O输出1
                m_CSwitchStatusWidget->m_label_line1_9->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_9->setText(str_end);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
            if(1 == _runStatusInfo.m_CRunStatusIO->m_io2_status)//I/O输出2
                m_CSwitchStatusWidget->m_label_line1_10->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_10->setText(str_end);

            if(1 == _runStatusInfo.m_CRunStatusIO->m_police1_status)//报警输入1
                m_CSwitchStatusWidget->m_label_line1_11->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_11->setText(str_end);

            if(1 == _runStatusInfo.m_CRunStatusIO->m_police2_status)//报警输入2
                m_CSwitchStatusWidget->m_label_line1_12->setText(str_start);
            else
                m_CSwitchStatusWidget->m_label_line1_12->setText(str_end);
#endif


        }break;
        case CProtocol::kEnumRunStatusGPS:          //定位模块
        {
            if(NULL == _runStatusInfo.m_CRunStatusGPS)
            {
                break;
            }
            if(NULL == m_CLocationModularWidget)
            {
                break;
            }

            m_CLocationModularWidget->m_label_line1_1->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_gps_number));
            m_CLocationModularWidget->m_label_line1_2->setText(_runStatusInfo.m_CRunStatusGPS->m_longitude);
            m_CLocationModularWidget->m_label_line1_3->setText(_runStatusInfo.m_CRunStatusGPS->m_latitude);
            m_CLocationModularWidget->m_label_line1_4->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_Azimuth) + tr("°"));
            m_CLocationModularWidget->m_label_line1_5->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_height) + tr("M"));
            m_CLocationModularWidget->m_label_line1_6->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_real_speed) + tr("km/h"));
            m_CLocationModularWidget->m_label_line1_7->setText(_runStatusInfo.m_CRunStatusGPS->m_firmware_version);

            int db_number = 0;
            for(int index = 0;index < 24;index++)
            {
               // qDebug() << _runStatusInfo.m_CRunStatusGPS->m_db_value[index];
                if(_runStatusInfo.m_CRunStatusGPS->m_db_value[index] >= 40)
                    db_number++;
            }

            m_CLocationModularWidget->m_label_line1_8->setText(QString::number(db_number));

            m_CLocationModularWidget->m_label_line1_9->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[0]) + tr("db,") +
                                                               QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[1]) + tr("db,") +
                                                               QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[2]) + tr("db,") +
                                                               QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[3]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_10->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[4]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[5]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[6]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[7]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_11->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[8]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[9]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[10]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[11]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_12->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[12]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[13]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[14]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[15]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_13->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[16]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[17]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[18]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[19]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_14->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[20]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[21]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[22]) + tr("db,") +
                                                                QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[23]) + tr("db"));
#if 0
            m_CLocationModularWidget->m_label_line1_15->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[7]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_16->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[8]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_17->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[9]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_18->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[10]) + tr("db"));
            m_CLocationModularWidget->m_label_line1_19->setText(QString::number(_runStatusInfo.m_CRunStatusGPS->m_db_value[11]) + tr("db"));
#endif
            m_CLocationModularWidget->update();
        }break;
        case CProtocol::kEnumRunStatusLocalNetwork: //本地网络
        {
            if(NULL == _runStatusInfo.m_CRunStatusLocalNetwork)
            {
                break;
            }
            if(NULL == m_CLocalNetworkWidget)
            {
                break;
            }
            QString str_connect = tr("已连接");
            QString str_noconnect = tr("未连接");
            QString str_connect_bug = tr("连接异常");
            QString str_connecting = tr("连接中");

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
            //状态    1:已连接 2:未连接  3:连接异常  4:连接中
            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->local_network_status)
                m_CLocalNetworkWidget->m_label_line1_1->setText(str_connect);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->local_network_status)
                m_CLocalNetworkWidget->m_label_line1_1->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->local_network_status)
                m_CLocalNetworkWidget->m_label_line1_1->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->local_network_status)
                m_CLocalNetworkWidget->m_label_line1_1->setText(str_connecting);
            else
                m_CLocalNetworkWidget->m_label_line1_1->setText(str_noconnect);

            m_CLocalNetworkWidget->m_label_line1_2->setText(_runStatusInfo.m_CRunStatusLocalNetwork->local_network_ip);
            m_CLocalNetworkWidget->m_label_line1_3->setText(_runStatusInfo.m_CRunStatusLocalNetwork->local_network_subnet_mask);
            m_CLocalNetworkWidget->m_label_line1_4->setText(_runStatusInfo.m_CRunStatusLocalNetwork->local_network_gateway);
            m_CLocalNetworkWidget->m_label_line1_5->setText(_runStatusInfo.m_CRunStatusLocalNetwork->local_network_dns1);
            m_CLocalNetworkWidget->m_label_line1_6->setText(_runStatusInfo.m_CRunStatusLocalNetwork->local_network_dns2);
#ifdef APPLICATION_TYPE_P2
            if(0 == _runStatusInfo.m_CRunStatusLocalNetwork->m_socket_status)
                m_CLocalNetworkWidget->m_label_line1_7->setText(tr("正常"));
            else
                m_CLocalNetworkWidget->m_label_line1_7->setText(tr("异常"));

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_host_from_status)
                m_CLocalNetworkWidget->m_label_line1_8->setText(tr("已连接"));
            else
                m_CLocalNetworkWidget->m_label_line1_8->setText(tr("未连接"));
#endif
#else
            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon1_status)
                m_CLocalNetworkWidget->m_label_line1_1->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon1_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon1_status)
                m_CLocalNetworkWidget->m_label_line1_1->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon1_status)
                m_CLocalNetworkWidget->m_label_line1_1->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon1_status)
                m_CLocalNetworkWidget->m_label_line1_1->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon2_status)
                m_CLocalNetworkWidget->m_label_line1_2->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon2_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon2_status)
                m_CLocalNetworkWidget->m_label_line1_2->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon2_status)
                m_CLocalNetworkWidget->m_label_line1_2->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon2_status)
                m_CLocalNetworkWidget->m_label_line1_2->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon3_status)
                m_CLocalNetworkWidget->m_label_line1_3->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon3_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon3_status)
                m_CLocalNetworkWidget->m_label_line1_3->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon3_status)
                m_CLocalNetworkWidget->m_label_line1_3->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon3_status)
                m_CLocalNetworkWidget->m_label_line1_3->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon4_status)
                m_CLocalNetworkWidget->m_label_line1_4->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon4_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon4_status)
                m_CLocalNetworkWidget->m_label_line1_4->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon4_status)
                m_CLocalNetworkWidget->m_label_line1_4->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon4_status)
                m_CLocalNetworkWidget->m_label_line1_4->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon5_status)
                m_CLocalNetworkWidget->m_label_line1_5->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon5_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon5_status)
                m_CLocalNetworkWidget->m_label_line1_5->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon5_status)
                m_CLocalNetworkWidget->m_label_line1_5->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon5_status)
                m_CLocalNetworkWidget->m_label_line1_5->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon6_status)
                m_CLocalNetworkWidget->m_label_line1_6->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon6_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon6_status)
                m_CLocalNetworkWidget->m_label_line1_6->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon6_status)
                m_CLocalNetworkWidget->m_label_line1_6->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon6_status)
                m_CLocalNetworkWidget->m_label_line1_6->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon7_status)
                m_CLocalNetworkWidget->m_label_line1_7->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon7_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon7_status)
                m_CLocalNetworkWidget->m_label_line1_7->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon7_status)
                m_CLocalNetworkWidget->m_label_line1_7->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon7_status)
                m_CLocalNetworkWidget->m_label_line1_7->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon8_status)
                m_CLocalNetworkWidget->m_label_line1_8->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon8_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon8_status)
                m_CLocalNetworkWidget->m_label_line1_8->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon8_status)
                m_CLocalNetworkWidget->m_label_line1_8->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon8_status)
                m_CLocalNetworkWidget->m_label_line1_8->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon9_status)
                m_CLocalNetworkWidget->m_label_line1_9->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon9_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon9_status)
                m_CLocalNetworkWidget->m_label_line1_9->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon9_status)
                m_CLocalNetworkWidget->m_label_line1_9->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon9_status)
                m_CLocalNetworkWidget->m_label_line1_9->setText(str_connecting);

            if(1 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon10_status)
                m_CLocalNetworkWidget->m_label_line1_10->setText(_runStatusInfo.m_CRunStatusLocalNetwork->m_pon10_ip);
            else if(2 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon10_status)
                m_CLocalNetworkWidget->m_label_line1_10->setText(str_noconnect);
            else if(3 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon10_status)
                m_CLocalNetworkWidget->m_label_line1_10->setText(str_connect_bug);
            else if(4 == _runStatusInfo.m_CRunStatusLocalNetwork->m_pon10_status)
                m_CLocalNetworkWidget->m_label_line1_10->setText(str_connecting);
#endif


        }break;
        case CProtocol::kEnumRunStatusDevice:        //设备
        {
            if(NULL == _runStatusInfo.m_CRunStatusDevice)
            {
                break;
            }
            if(NULL == m_CDeviceStatusWidget)
            {
                break;
            }
/*
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
*/
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
            m_CDeviceStatusWidget->m_label_line1_3->setText(QString::number(_runStatusInfo.m_CRunStatusDevice->m_input_voltage) + tr("V"));
            m_CDeviceStatusWidget->m_label_line1_4->setText(QString::number(_runStatusInfo.m_CRunStatusDevice->m_device_temperature) + tr("℃"));
#else
            if(1 == _runStatusInfo.m_CRunStatusDevice->m_device_door)
                m_CDeviceStatusWidget->m_label_line1_1->setText(tr("开启"));
            else
                m_CDeviceStatusWidget->m_label_line1_1->setText(tr("关闭"));

            m_CDeviceStatusWidget->m_label_line1_2->setText(QString::number(_runStatusInfo.m_CRunStatusDevice->m_fan_speed) + tr("r/min"));
            m_CDeviceStatusWidget->m_label_line1_3->setText(QString::number(_runStatusInfo.m_CRunStatusDevice->m_input_voltage) + tr("V"));
            m_CDeviceStatusWidget->m_label_line1_4->setText(QString::number(_runStatusInfo.m_CRunStatusDevice->m_device_temperature) + tr("℃"));
            m_CDeviceStatusWidget->m_label_line1_5->setText(QString::number(_runStatusInfo.m_CRunStatusDevice->m_car_out_temperature) + tr("℃"));
            m_CDeviceStatusWidget->m_label_line1_6->setText(QString::number(_runStatusInfo.m_CRunStatusDevice->m_car_in_temperature) + tr("℃"));

#endif


            QString str_connect = tr("连接");
            QString str_noconnect = tr("未连接");
            QString str_disconnect = tr("断开");

            if(1 == _runStatusInfo.m_CRunStatusDevice->m_serial1_port_status)
                m_CDeviceStatusWidget->m_label_main1_line2_1->setText(str_connect);
            else
                m_CDeviceStatusWidget->m_label_main1_line2_1->setText(str_noconnect);

            if(1 == _runStatusInfo.m_CRunStatusDevice->m_serial2_port_status)
                m_CDeviceStatusWidget->m_label_main1_line3_1->setText(str_connect);
            else
                m_CDeviceStatusWidget->m_label_main1_line3_1->setText(str_noconnect);

            if(1 == _runStatusInfo.m_CRunStatusDevice->m_serial3_port_status)
                m_CDeviceStatusWidget->m_label_main1_line4_1->setText(str_connect);
            else
                m_CDeviceStatusWidget->m_label_main1_line4_1->setText(str_noconnect);

            if(1 == _runStatusInfo.m_CRunStatusDevice->m_serial4_port_status)
                m_CDeviceStatusWidget->m_label_main1_line5_1->setText(str_connect);
            else
                m_CDeviceStatusWidget->m_label_main1_line5_1->setText(str_noconnect);

            if(1 == _runStatusInfo.m_CRunStatusDevice->m_serial5_port_status)
                m_CDeviceStatusWidget->m_label_main1_line6_1->setText(str_connect);
            else
                m_CDeviceStatusWidget->m_label_main1_line6_1->setText(str_noconnect);

            if(1 == _runStatusInfo.m_CRunStatusDevice->m_can1_status)
                m_CDeviceStatusWidget->m_label_main2_line2_1->setText(str_connect);
            else
                m_CDeviceStatusWidget->m_label_main2_line2_1->setText(str_disconnect);

            if(1 == _runStatusInfo.m_CRunStatusDevice->m_can2_status)
                m_CDeviceStatusWidget->m_label_main2_line3_1->setText(str_connect);
            else
                m_CDeviceStatusWidget->m_label_main2_line3_1->setText(str_disconnect);

        }break;
        case CProtocol::kEnumRunStatusStation:  //站点信息
        {
            if(NULL == m_CStationInfoWidget)
                break;

            m_CStationInfoWidget->m_label_line1_1->setText(_runStatusInfo.m_CStationInfo->start_station);       //起始站
            m_CStationInfoWidget->m_label_line2_1->setText(_runStatusInfo.m_CStationInfo->end_station);         //终点站
            m_CStationInfoWidget->m_label_line3_1->setText(_runStatusInfo.m_CStationInfo->current_station);     //当前站

            if(1 == _runStatusInfo.m_CStationInfo->status)//1:上行 2:下行
                m_CStationInfoWidget->m_label_line4_1->setText(tr("上行"));
            else
                m_CStationInfoWidget->m_label_line4_1->setText(tr("下行"));

            m_CStationInfoWidget->m_label_line5_1->setText(tr("第") + QString::number(_runStatusInfo.m_CStationInfo->current_id + 1) + tr("站"));

        }break;
        case CProtocol::kEnumRunStatusPeripheral:
        {

            if(NULL == m_CPeripheralWidget)
                break;
            QString str_normal = "正常";
            QString str_police = "报警";
            QString str_unknown = "未知";
            QString str_offline = "离线";
            //1

            //qDebug() << "CRunStatusWidget::slot_run_status_widget_event 3";
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe1)
                m_CPeripheralWidget->m_label_line1_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe1)
                m_CPeripheralWidget->m_label_line1_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe1)
                m_CPeripheralWidget->m_label_line1_1->setText(str_offline);
            else// if(0x03 == _runStatusInfo.m_CPeripheralStatus->probe1)
                m_CPeripheralWidget->m_label_line1_1->setText(str_unknown);
            //2
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe2)
                m_CPeripheralWidget->m_label_line2_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe2)
                m_CPeripheralWidget->m_label_line2_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe2)
                m_CPeripheralWidget->m_label_line2_1->setText(str_offline);
            else
                m_CPeripheralWidget->m_label_line2_1->setText(str_unknown);

            //3
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe3)
                m_CPeripheralWidget->m_label_line3_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe3)
                m_CPeripheralWidget->m_label_line3_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe3)
                m_CPeripheralWidget->m_label_line3_1->setText(str_offline);
            else
                m_CPeripheralWidget->m_label_line3_1->setText(str_unknown);

            //4
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe4)
                m_CPeripheralWidget->m_label_line4_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe4)
                m_CPeripheralWidget->m_label_line4_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe4)
                m_CPeripheralWidget->m_label_line4_1->setText(str_offline);
            else
                m_CPeripheralWidget->m_label_line4_1->setText(str_unknown);

            //5
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe5)
                m_CPeripheralWidget->m_label_line5_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe5)
                m_CPeripheralWidget->m_label_line5_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe5)
                m_CPeripheralWidget->m_label_line5_1->setText(str_offline);
            else
                m_CPeripheralWidget->m_label_line5_1->setText(str_unknown);

            //6
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe6)
                m_CPeripheralWidget->m_label_line6_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe6)
                m_CPeripheralWidget->m_label_line6_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe6)
                m_CPeripheralWidget->m_label_line6_1->setText(str_offline);
            else
                m_CPeripheralWidget->m_label_line6_1->setText(str_unknown);

            //7
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe7)
                m_CPeripheralWidget->m_label_line7_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe7)
                m_CPeripheralWidget->m_label_line7_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe7)
                m_CPeripheralWidget->m_label_line7_1->setText(str_offline);
            else
                m_CPeripheralWidget->m_label_line7_1->setText(str_unknown);

            //8
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe8)
                m_CPeripheralWidget->m_label_line8_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe8)
                m_CPeripheralWidget->m_label_line8_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe8)
                m_CPeripheralWidget->m_label_line8_1->setText(str_offline);
            else
                m_CPeripheralWidget->m_label_line8_1->setText(str_unknown);

            //9
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe9)
                m_CPeripheralWidget->m_label_line9_1->setText(str_normal);
            else if(0x02 == _runStatusInfo.m_CPeripheralStatus->probe9)
                m_CPeripheralWidget->m_label_line9_1->setText(str_police);
            else if(0xFF == _runStatusInfo.m_CPeripheralStatus->probe9)
                m_CPeripheralWidget->m_label_line9_1->setText(str_offline);
            else
                m_CPeripheralWidget->m_label_line9_1->setText(str_unknown);

            //10
            if(0x01 == _runStatusInfo.m_CPeripheralStatus->probe10)
                m_CPeripheralWidget->m_label_line10_1->setText(tr("断开"));
            else
                m_CPeripheralWidget->m_label_line10_1->setText(tr("闭合"));


        }break;
    }
    
}



















