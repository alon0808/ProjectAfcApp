#include "switch_config_widget.h"

#include "LtyCommonDefine.h"
#include "./class/common_interface.h"
#include <QListView>
#include "LtyCommonStyle.h"
#include <QTimer>
#include "common_data.h"

CSwitchConfigInfo::CSwitchConfigInfo(char _type,QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("CSwitchConfigInfo");
    this->setStyleSheet("QWidget#CSwitchConfigInfo{border:1px solid #24508F;background-color:#ffffff;}");

    m_type = _type;
    set_type(_type);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);
}

void CSwitchConfigInfo::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSwitchConfigInfo::vehicle_door_check_init()  //车门检测
{
    this->setFixedSize(620,ROW_HEIGHT * 7);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 7);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");


    for(int index = 0;index < 8;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }

    QString str_show = tr("使能");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(620 - 70,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    str_show = tr("车门数量");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(788 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(100,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(tr("1"));
    m_combobox_line2->addItem(tr("2"));
    m_combobox_line2->addItem(tr("3"));
    m_combobox_line2->setView(new QListView());


    str_show = tr("车门类型");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(718 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("两线气动门"));
    m_combobox_line3->addItem(tr("传感器门"));
    m_combobox_line3->addItem(tr("不检测"));
    m_combobox_line3->setView(new QListView());


    str_show = tr("开前门电平");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
    m_combobox_line4 = new QComboBox(this);
    m_combobox_line4->move(718 - 290,ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line4->addItem(tr("低电平有效"));
    m_combobox_line4->addItem(tr("高电平有效"));
    m_combobox_line4->addItem(tr("不检测"));
    m_combobox_line4->setView(new QListView());


    str_show = tr("开中门电平");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
    m_combobox_line5 = new QComboBox(this);
    m_combobox_line5->move(718 - 290,ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line5->addItem(tr("低电平有效"));
    m_combobox_line5->addItem(tr("高电平有效"));
    m_combobox_line5->addItem(tr("不检测"));
    m_combobox_line5->setView(new QListView());


    str_show = tr("开后门电平");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);
    m_combobox_line6 = new QComboBox(this);
    m_combobox_line6->move(718 - 290,ROW_HEIGHT * 5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line6->addItem(tr("低电平有效"));
    m_combobox_line6->addItem(tr("高电平有效"));
    m_combobox_line6->addItem(tr("不检测"));
    m_combobox_line6->setView(new QListView());

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    str_show = tr("I/O输入1");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    m_combobox_line7 = new QComboBox(this);
    m_combobox_line7->move(718 - 290,ROW_HEIGHT * 6 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line7->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line7->addItem(tr("低电平有效"));
    m_combobox_line7->addItem(tr("高电平有效"));
    m_combobox_line7->addItem(tr("不检测"));
    m_combobox_line7->setView(new QListView());
#else
    str_show = tr("其他使能");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    QString str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_line7 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line7,787 - 290,ROW_HEIGHT * 6 + (ROW_HEIGHT - 60)/2,100,60,str_sheet);
    m_pushbutton_line7->setText(tr("配置"));

    connect(m_pushbutton_line7,SIGNAL(clicked()),this,SIGNAL(signal_other_enable())); //其他使能

#endif

}

void CSwitchConfigInfo::other_switch_check_init()   //其他开关量检测
{
    this->setFixedSize(620,ROW_HEIGHT * 6);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 6);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");


    for(int index = 0;index < 7;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }

    QString str_show = tr("倒车监视");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(678 - 290 - 20,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    m_combobox_line1 = new QComboBox(this);
    m_combobox_line1->move(718 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line1->addItem(tr("低电平有效"));
    m_combobox_line1->addItem(tr("高电平有效"));
    m_combobox_line1->addItem(tr("不检测"));
    m_combobox_line1->setView(new QListView());


    str_show = tr("未刹车");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_checkbox_line2 = new QCheckBox(this);
    m_checkbox_line2->setFixedSize(70,70);
    m_checkbox_line2->move(678 - 290 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(718 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(tr("低电平有效"));
    m_combobox_line2->addItem(tr("高电平有效"));
    m_combobox_line2->addItem(tr("不检测"));
    m_combobox_line2->setView(new QListView());

    str_show = tr("开前箱");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_checkbox_line3 = new QCheckBox(this);
    m_checkbox_line3->setFixedSize(70,70);
    m_checkbox_line3->move(678 - 290 - 20,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3->setFocusPolicy(Qt::NoFocus);

    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(718 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("低电平有效"));
    m_combobox_line3->addItem(tr("高电平有效"));
    m_combobox_line3->addItem(tr("不检测"));
    m_combobox_line3->setView(new QListView());

    str_show = tr("雨刮");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);


    m_checkbox_line4 = new QCheckBox(this);
    m_checkbox_line4->setFixedSize(70,70);
    m_checkbox_line4->move(678 - 290 - 20,ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4->setFocusPolicy(Qt::NoFocus);

    m_combobox_line4 = new QComboBox(this);
    m_combobox_line4->move(718 - 290,ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line4->addItem(tr("低电平有效"));
    m_combobox_line4->addItem(tr("高电平有效"));
    m_combobox_line4->addItem(tr("不检测"));
    m_combobox_line4->setView(new QListView());


    str_show = tr("左转灯");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_checkbox_line5 = new QCheckBox(this);
    m_checkbox_line5->setFixedSize(70,70);
    m_checkbox_line5->move(678 - 290 - 20,ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5->setFocusPolicy(Qt::NoFocus);

    m_combobox_line5 = new QComboBox(this);
    m_combobox_line5->move(718 - 290,ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line5->addItem(tr("低电平有效"));
    m_combobox_line5->addItem(tr("高电平有效"));
    m_combobox_line5->addItem(tr("不检测"));
    m_combobox_line5->setView(new QListView());

    str_show = tr("右转灯");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_checkbox_line6 = new QCheckBox(this);
    m_checkbox_line6->setFixedSize(70,70);
    m_checkbox_line6->move(678 - 290 - 20,ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6->setFocusPolicy(Qt::NoFocus);

    m_combobox_line6 = new QComboBox(this);
    m_combobox_line6->move(718 - 290,ROW_HEIGHT * 5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6->setFixedSize(170,COMBOBOX_HEIGHT);
    m_combobox_line6->addItem(tr("低电平有效"));
    m_combobox_line6->addItem(tr("高电平有效"));
    m_combobox_line6->addItem(tr("不检测"));
    m_combobox_line6->setView(new QListView());

}

void CSwitchConfigInfo::set_type(char _type)
{
    switch(_type)
    {
        case kEnumVehicleDoorCheck:  //车门检测
        {
            vehicle_door_check_init();
        }break;
        case kEnumOtherSwitchCheck:   //其他开关量检测
        {
            other_switch_check_init();
        }break;
     }
}


CSwitchConfigWidget::CSwitchConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

    this->setObjectName("CSwitchConfigWidget");
    this->setStyleSheet("QWidget#CSwitchConfigWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

        QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    scroll_layout();
    switch_config_init_new();
    other_enable_init();

    read_data();



    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);

}

void CSwitchConfigWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSwitchConfigWidget::scroll_layout()
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


    m_widget_other_enable = new QWidget;
    m_widget_other_enable->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + ROW_HEIGHT);

    m_widget_other_enable->setStyleSheet("QWidget{background: transparent;}");
    m_widget_other_enable->hide();

}

void CSwitchConfigWidget::other_enable_init()
{
    QWidget *pRectWidget = new QWidget(m_widget_other_enable);
    pRectWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget->setFixedSize(620,77);
    pRectWidget->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    for(int index = 0;index < 8;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_other_enable);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
    QWidget *pLine1 = new QWidget(m_widget_other_enable);
    QWidget *pLine2 = new QWidget(m_widget_other_enable);
     pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,6 * ROW_HEIGHT);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,6 * ROW_HEIGHT);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    //int alignRight = 290 - 250 + 300;

    QString str_show = tr("其他使能");
    m_label_other_enable_line1 = new QLabel(m_widget_other_enable);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_other_enable_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    str_show = tr("带速度开门检测");
    m_label_other_enable_line2 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_checkbox_other_enable_line2 = new QCheckBox(m_widget_other_enable);
    m_checkbox_other_enable_line2->setFixedSize(70,70);
    m_checkbox_other_enable_line2->move(856 - 250 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_other_enable_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_other_enable_line2->setFocusPolicy(Qt::NoFocus);

    str_show = tr("站间开门检测");
    m_label_other_enable_line3 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_checkbox_other_enable_line3 = new QCheckBox(m_widget_other_enable);
    m_checkbox_other_enable_line3->setFixedSize(70,70);
    m_checkbox_other_enable_line3->move(856 - 250 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_other_enable_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_other_enable_line3->setFocusPolicy(Qt::NoFocus);

    str_show = tr("到站未开门检测");
    m_label_other_enable_line4 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line4,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_checkbox_other_enable_line4 = new QCheckBox(m_widget_other_enable);
    m_checkbox_other_enable_line4->setFixedSize(70,70);
    m_checkbox_other_enable_line4->move(856 - 250 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_other_enable_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_other_enable_line4->setFocusPolicy(Qt::NoFocus);

    str_show = tr("到站未停车检测");
    m_label_other_enable_line5 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line5,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_checkbox_other_enable_line5 = new QCheckBox(m_widget_other_enable);
    m_checkbox_other_enable_line5->setFixedSize(70,70);
    m_checkbox_other_enable_line5->move(856 - 250 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_other_enable_line5->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_other_enable_line5->setFocusPolicy(Qt::NoFocus);

    str_show = tr("异常开关门最小速度");
    m_label_other_enable_line6 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line6,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_checkbox_other_enable_line6 = new QCheckBox(m_widget_other_enable);
    m_checkbox_other_enable_line6->setFixedSize(70,70);
    m_checkbox_other_enable_line6->move(856 - 250 - 20 - 180,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_other_enable_line6->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_other_enable_line6->setFocusPolicy(Qt::NoFocus);

    str_show = tr("≤");
    m_label_other_enable_line6_1 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line6_1,707 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_label_other_enablelineedit6 = new QLineEdit(m_widget_other_enable);
    m_label_other_enablelineedit6->move(728 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 +ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_label_other_enablelineedit6->setFixedSize(100,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_label_other_enablelineedit6,true);

    str_show = tr("km/h");
    m_label_other_enable_line6_2 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line6_2,838 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);



    str_show = tr("异常开关门提醒次数");
    m_label_other_enable_line7 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line7,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    m_checkbox_other_enable_line7 = new QCheckBox(m_widget_other_enable);
    m_checkbox_other_enable_line7->setFixedSize(70,70);
    m_checkbox_other_enable_line7->move(856 - 250 - 20 - 180,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6 + (ROW_HEIGHT - 70)/2);
    m_checkbox_other_enable_line7->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_other_enable_line7->setFocusPolicy(Qt::NoFocus);

    m_label_other_enablelineedit7 = new QLineEdit(m_widget_other_enable);
    m_label_other_enablelineedit7->move(728 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 +ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_label_other_enablelineedit7->setFixedSize(100,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_label_other_enablelineedit7,true);

    str_show = tr("T");
    m_label_other_enable_line7_2 = new QLabel(m_widget_other_enable);
    CCommonInterface::init_label_text(m_label_other_enable_line7_2,838 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);


}

void CSwitchConfigWidget::switch_config_init_new()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("车门检测"));

    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    //m_button_1->move(290 - 250,158 - MAIN_HEAD_HEIGHT);

    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("其他开关量检测"));

    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
#if 0
    m_button_3 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_3->setDisplayText(tr("EMMC"));

    m_button_3->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_3->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_3->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
#endif
    m_SwitchConfigInfo1 = new CSwitchConfigInfo(CSwitchConfigInfo::kEnumVehicleDoorCheck,m_widget_main);
    m_SwitchConfigInfo2 = new CSwitchConfigInfo(CSwitchConfigInfo::kEnumOtherSwitchCheck,m_widget_main);
   // m_SwitchConfigInfo3 = new CSwitchConfigInfo(CSwitchConfigInfo::kEnumOtherSwitchCheck,);

    connect(m_SwitchConfigInfo1,SIGNAL(signal_other_enable()),this,SLOT(slot_switchconfiginfo_other_enable()));

    m_label_space1  = new QLabel(m_widget_main);
    m_label_space1->setFixedHeight(10);
    m_label_space1->setStyleSheet("QLabel{border:0px;background:transparent;}");

   // m_label_space2  = new QLabel(m_widget_main);
  //  m_label_space2->setFixedHeight(10);
 //   m_label_space2->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_vboxlayout_storage = new QVBoxLayout(m_widget_main);//左 上 右 下
    m_vboxlayout_storage->setContentsMargins(m_define_screen->get_change_factor_x(290 - 250),m_define_screen->get_change_factor_y(158 - MAIN_HEAD_HEIGHT - 50),0,0);
    m_vboxlayout_storage->setSpacing(0);
    m_vboxlayout_storage->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_vboxlayout_storage->addWidget(m_button_1);
    m_vboxlayout_storage->addWidget(m_SwitchConfigInfo1);
    m_vboxlayout_storage->addWidget(m_label_space1);
    m_vboxlayout_storage->addWidget(m_button_2);
    m_vboxlayout_storage->addWidget(m_SwitchConfigInfo2);
 //   m_vboxlayout_storage->addWidget(m_label_space2);
    //m_vboxlayout_storage->addWidget(m_button_3);
   // m_vboxlayout_storage->addWidget(m_SwitchConfigInfo3);

    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));
   // connect(m_button_3,SIGNAL(clicked()),this,SLOT(slot_button_3()));

    m_button_press_1 = false;
    m_button_press_2 = false;
  //  m_button_press_3 = false;
    m_SwitchConfigInfo1->hide();
    m_SwitchConfigInfo2->hide();
   // m_SwitchConfigInfo3->hide();

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_label_space1->hide();
    m_button_2->hide();
    m_SwitchConfigInfo2->hide();
    QTimer::singleShot(10,this,SLOT(slot_timer_init_button1()));
#endif

}

void CSwitchConfigWidget::slot_timer_init_button1()
{
    slot_button_1();
}

void CSwitchConfigWidget::slot_switchconfiginfo_other_enable()
{
    m_widget_main->hide();
    m_widget_other_enable->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_other_enable);
}

void CSwitchConfigWidget::slot_pushbutton_return()
{
    if(m_widget_other_enable->isVisible())
    {
        m_widget_main->show();
        m_widget_other_enable->hide();
        m_vboxlayout_widget->removeWidget(m_widget_other_enable);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else
    {
        emit signal_window_close();
    }
}


void CSwitchConfigWidget::slot_button_1()
{
    if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_SwitchConfigInfo1->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_SwitchConfigInfo1->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 4));
    }
    m_button_2->setSelected(false);

    m_button_press_2 = false;

    m_SwitchConfigInfo2->hide();



}
void CSwitchConfigWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_SwitchConfigInfo2->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_SwitchConfigInfo2->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + ROW_HEIGHT * 3);
    }
    m_button_1->setSelected(false);

    m_button_press_1 = false;

    m_SwitchConfigInfo1->hide();

}
#include "common_data.h"
void CSwitchConfigWidget::read_data()
{
      //车门检测

    CCommonData::getInstance()->m_CSystemIoConfig.m_enable = (m_SwitchConfigInfo1->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemIoConfig.m_car_door_number = m_SwitchConfigInfo1->m_combobox_line2->currentIndex();//车门数量 0:1 1:2 2:3
    CCommonData::getInstance()->m_CSystemIoConfig.m_car_door_type = m_SwitchConfigInfo1->m_combobox_line3->currentIndex();//车门类型  0:两线气动门　1:传感器门　2:不检测
    CCommonData::getInstance()->m_CSystemIoConfig.m_door_front = m_SwitchConfigInfo1->m_combobox_line4->currentIndex();//开前门电平 0:低电平有效　1:高电平有效
    CCommonData::getInstance()->m_CSystemIoConfig.m_door_middle = m_SwitchConfigInfo1->m_combobox_line5->currentIndex();//开中门电平 0:低电平有效　1:高电平有效
    CCommonData::getInstance()->m_CSystemIoConfig.m_door_back = m_SwitchConfigInfo1->m_combobox_line6->currentIndex();//开后门电平 0:低电平有效　1:高电平有效
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    CCommonData::getInstance()->m_CSystemIoConfig.m_io_input1 = m_SwitchConfigInfo1->m_combobox_line7->currentIndex();
#endif

    //车门检测－配置

    CCommonData::getInstance()->m_CSystemIoConfig.speed_open_door_check = (m_checkbox_other_enable_line2->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.station_open_door_check = (m_checkbox_other_enable_line3->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.arrive_open_door_check = (m_checkbox_other_enable_line4->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.arrive_stop_door_check = (m_checkbox_other_enable_line5->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.m_speed_enable = (m_checkbox_other_enable_line6->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.m_speed = m_label_other_enablelineedit6->text().toInt();
    CCommonData::getInstance()->m_CSystemIoConfig.m_frequency_enable = (m_checkbox_other_enable_line7->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.m_frequency = m_label_other_enablelineedit7->text().toInt();


    //其他开关量检测

    CCommonData::getInstance()->m_CSystemIoConfig.back_car_monitor_enable = (m_SwitchConfigInfo2->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.back_car_monitor_status = m_SwitchConfigInfo2->m_combobox_line1->currentIndex();

    CCommonData::getInstance()->m_CSystemIoConfig.no_brake_enable = (m_SwitchConfigInfo2->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.no_brake_status = m_SwitchConfigInfo2->m_combobox_line2->currentIndex();

    CCommonData::getInstance()->m_CSystemIoConfig.open_front_box_enable = (m_SwitchConfigInfo2->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.open_front_box_status = m_SwitchConfigInfo2->m_combobox_line3->currentIndex();

    CCommonData::getInstance()->m_CSystemIoConfig.wiper_enable = (m_SwitchConfigInfo2->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.wiper_status = m_SwitchConfigInfo2->m_combobox_line4->currentIndex();

    CCommonData::getInstance()->m_CSystemIoConfig.left_lamp_enable = (m_SwitchConfigInfo2->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.left_lamp_status = m_SwitchConfigInfo2->m_combobox_line5->currentIndex();

    CCommonData::getInstance()->m_CSystemIoConfig.right_lamp_enable = (m_SwitchConfigInfo2->m_checkbox_line6->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemIoConfig.right_lamp_status = m_SwitchConfigInfo2->m_combobox_line6->currentIndex();


}
void CSwitchConfigWidget::save_data()
{
    CSystemIoConfig info;
    //车门检测
    info.m_enable = (m_SwitchConfigInfo1->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;

    info.m_car_door_number = m_SwitchConfigInfo1->m_combobox_line2->currentIndex();//车门数量 0:1 1:2 2:3
    info.m_car_door_type = m_SwitchConfigInfo1->m_combobox_line3->currentIndex();//车门类型  0:两线气动门　1:传感器门　2:不检测
    info.m_door_front = m_SwitchConfigInfo1->m_combobox_line4->currentIndex();//开前门电平 0:低电平有效　1:高电平有效
    info.m_door_middle = m_SwitchConfigInfo1->m_combobox_line5->currentIndex();//开中门电平 0:低电平有效　1:高电平有效
    info.m_door_back = m_SwitchConfigInfo1->m_combobox_line6->currentIndex();//开后门电平 0:低电平有效　1:高电平有效
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    info.m_io_input1 = m_SwitchConfigInfo1->m_combobox_line7->currentIndex();
#endif

    //车门检测－配置

    info.speed_open_door_check = (m_checkbox_other_enable_line2->checkState() == Qt::Checked)? 1:0;
    info.station_open_door_check = (m_checkbox_other_enable_line3->checkState() == Qt::Checked)? 1:0;
    info.arrive_open_door_check = (m_checkbox_other_enable_line4->checkState() == Qt::Checked)? 1:0;
    info.arrive_stop_door_check = (m_checkbox_other_enable_line5->checkState() == Qt::Checked)? 1:0;
    info.m_speed_enable = (m_checkbox_other_enable_line6->checkState() == Qt::Checked)? 1:0;
    info.m_speed = m_label_other_enablelineedit6->text().toInt();
    info.m_frequency_enable = (m_checkbox_other_enable_line7->checkState() == Qt::Checked)? 1:0;
    info.m_frequency = m_label_other_enablelineedit7->text().toInt();


    //其他开关量检测

    info.back_car_monitor_enable = (m_SwitchConfigInfo2->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;
    info.back_car_monitor_status = m_SwitchConfigInfo2->m_combobox_line1->currentIndex();

    info.no_brake_enable = (m_SwitchConfigInfo2->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;
    info.no_brake_status = m_SwitchConfigInfo2->m_combobox_line2->currentIndex();

    info.open_front_box_enable = (m_SwitchConfigInfo2->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;
    info.open_front_box_status = m_SwitchConfigInfo2->m_combobox_line3->currentIndex();

    info.wiper_enable = (m_SwitchConfigInfo2->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;
    info.wiper_status = m_SwitchConfigInfo2->m_combobox_line4->currentIndex();

    info.left_lamp_enable = (m_SwitchConfigInfo2->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;
    info.left_lamp_status = m_SwitchConfigInfo2->m_combobox_line5->currentIndex();

    info.right_lamp_enable = (m_SwitchConfigInfo2->m_checkbox_line6->checkState() == Qt::Checked)? 1:0;
    info.right_lamp_status = m_SwitchConfigInfo2->m_combobox_line6->currentIndex();


    if(info.m_enable != CCommonData::getInstance()->m_CSystemIoConfig.m_enable ||
        info.m_car_door_number != CCommonData::getInstance()->m_CSystemIoConfig.m_car_door_number ||
        info.m_car_door_type != CCommonData::getInstance()->m_CSystemIoConfig.m_car_door_type ||
        info.m_door_front != CCommonData::getInstance()->m_CSystemIoConfig.m_door_front ||
        info.m_door_middle != CCommonData::getInstance()->m_CSystemIoConfig.m_door_middle ||
        info.m_door_back != CCommonData::getInstance()->m_CSystemIoConfig.m_door_back ||
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        info.m_io_input1 != CCommonData::getInstance()->m_CSystemIoConfig.m_io_input1 ||
#endif
        info.speed_open_door_check != CCommonData::getInstance()->m_CSystemIoConfig.speed_open_door_check ||
        info.station_open_door_check != CCommonData::getInstance()->m_CSystemIoConfig.station_open_door_check ||
        info.arrive_open_door_check != CCommonData::getInstance()->m_CSystemIoConfig.arrive_open_door_check ||
        info.arrive_stop_door_check != CCommonData::getInstance()->m_CSystemIoConfig.arrive_stop_door_check ||
        info.m_speed_enable != CCommonData::getInstance()->m_CSystemIoConfig.m_speed_enable ||
        info.m_speed != CCommonData::getInstance()->m_CSystemIoConfig.m_speed ||
        info.m_frequency_enable != CCommonData::getInstance()->m_CSystemIoConfig.m_frequency_enable ||
        info.m_frequency != CCommonData::getInstance()->m_CSystemIoConfig.m_frequency ||
        info.back_car_monitor_enable != CCommonData::getInstance()->m_CSystemIoConfig.back_car_monitor_enable ||
        info.back_car_monitor_status != CCommonData::getInstance()->m_CSystemIoConfig.back_car_monitor_status ||
        info.no_brake_enable != CCommonData::getInstance()->m_CSystemIoConfig.no_brake_enable ||
        info.no_brake_status != CCommonData::getInstance()->m_CSystemIoConfig.no_brake_status ||
        info.open_front_box_enable != CCommonData::getInstance()->m_CSystemIoConfig.open_front_box_enable ||
        info.open_front_box_status != CCommonData::getInstance()->m_CSystemIoConfig.open_front_box_status ||
        info.wiper_enable != CCommonData::getInstance()->m_CSystemIoConfig.wiper_enable ||
        info.wiper_status != CCommonData::getInstance()->m_CSystemIoConfig.wiper_status ||
        info.left_lamp_enable != CCommonData::getInstance()->m_CSystemIoConfig.left_lamp_enable ||
        info.left_lamp_status != CCommonData::getInstance()->m_CSystemIoConfig.left_lamp_status ||
        info.right_lamp_enable != CCommonData::getInstance()->m_CSystemIoConfig.right_lamp_enable ||
        info.right_lamp_status != CCommonData::getInstance()->m_CSystemIoConfig.right_lamp_status
    )
    {
         if(CCommonData::getInstance()->messagebox_data_change())
        {
            read_data();
            CProtocol::getInstance()->call_json_system_switch_config_set(CProtocol::kEnumSave);
        }
        else
        {
            slot_switch_config_widget_event(CCommonData::getInstance()->m_CSystemIoConfig);
        }

    }


}

//m_SwitchConfigInfo1
void CSwitchConfigWidget::slot_switch_config_widget_event(const CSystemIoConfig &_info)
{
        //车门检测
     if(0 == _info.m_enable)// 0:未选择状态 1:选中状态
        m_SwitchConfigInfo1->m_checkbox_line1->setChecked(false);
     else
        m_SwitchConfigInfo1->m_checkbox_line1->setChecked(true);

    m_SwitchConfigInfo1->m_combobox_line2->setCurrentIndex(_info.m_car_door_number);//车门数量 0:1 1:2 2:3
    m_SwitchConfigInfo1->m_combobox_line3->setCurrentIndex(_info.m_car_door_type);//车门类型  0:两线气动门　1:传感器门　2:不检测
    m_SwitchConfigInfo1->m_combobox_line4->setCurrentIndex(_info.m_door_front);//开前门电平 0:低电平有效　1:高电平有效
    m_SwitchConfigInfo1->m_combobox_line5->setCurrentIndex(_info.m_door_middle);//开中门电平 0:低电平有效　1:高电平有效
    m_SwitchConfigInfo1->m_combobox_line6->setCurrentIndex(_info.m_door_back);//开后门电平 0:低电平有效　1:高电平有效

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_SwitchConfigInfo1->m_combobox_line7->setCurrentIndex(_info.m_io_input1);//I/O输入1 0:低电平有效　1:高电平有效
#else
    //车门检测－配置
    if(0 == _info.speed_open_door_check)//带速度开门检测
        m_checkbox_other_enable_line2->setChecked(false);
    else
        m_checkbox_other_enable_line2->setChecked(true);
    if(0 == _info.station_open_door_check)//站间开门检测
         m_checkbox_other_enable_line3->setChecked(false);
    else
        m_checkbox_other_enable_line3->setChecked(true);
    if(0 == _info.arrive_open_door_check)//到站未开门检测
         m_checkbox_other_enable_line4->setChecked(false);
    else
        m_checkbox_other_enable_line4->setChecked(true);
    if(0 == _info.arrive_stop_door_check)//到站未停车检测
         m_checkbox_other_enable_line5->setChecked(false);
    else
        m_checkbox_other_enable_line5->setChecked(true);
    if(0 == _info.m_speed_enable)//异常开关门最小速度使能 0:未选择状态 1:选中状态
         m_checkbox_other_enable_line6->setChecked(false);
    else
        m_checkbox_other_enable_line6->setChecked(true);
    m_label_other_enablelineedit6->setText(QString::number(_info.m_speed));//异常开关门最小速度

    if(0 == _info.m_frequency_enable)//异常开关门提醒使能 0:未选择状态 1:选中状态
         m_checkbox_other_enable_line7->setChecked(false);
    else
        m_checkbox_other_enable_line7->setChecked(true);

    m_label_other_enablelineedit7->setText(QString::number(_info.m_frequency));//异常开关门提醒次数
#endif


    //其他开关量检测
    if(0 == _info.back_car_monitor_enable)//倒车监视 0:未选择状态 1:选中状态
        m_SwitchConfigInfo2->m_checkbox_line1->setChecked(false);
    else
        m_SwitchConfigInfo2->m_checkbox_line1->setChecked(true);

    m_SwitchConfigInfo2->m_combobox_line1->setCurrentIndex(_info.back_car_monitor_status);//倒车监视 0:低电平有效　1:高电平有效

    if(0 == _info.no_brake_enable)//未刹车 0:未选择状态 1:选中状态
        m_SwitchConfigInfo2->m_checkbox_line2->setChecked(false);
    else
        m_SwitchConfigInfo2->m_checkbox_line2->setChecked(true);

    m_SwitchConfigInfo2->m_combobox_line2->setCurrentIndex(_info.no_brake_status); //未刹车 0:低电平有效　1:高电平有效

    if(0 == _info.open_front_box_enable)//开前箱 0:未选择状态 1:选中状态
        m_SwitchConfigInfo2->m_checkbox_line3->setChecked(false);
    else
        m_SwitchConfigInfo2->m_checkbox_line3->setChecked(true);

    m_SwitchConfigInfo2->m_combobox_line3->setCurrentIndex(_info.open_front_box_status);//开前箱 0:低电平有效　1:高电平有效

    if(0 == _info.wiper_enable)//雨刮 0:未选择状态 1:选中状态
        m_SwitchConfigInfo2->m_checkbox_line4->setChecked(false);
    else
        m_SwitchConfigInfo2->m_checkbox_line4->setChecked(true);
    m_SwitchConfigInfo2->m_combobox_line4->setCurrentIndex(_info.wiper_status);//雨刮 0:低电平有效　1:高电平有效

    if(0 == _info.left_lamp_enable)//左转灯 0:未选择状态 1:选中状态
        m_SwitchConfigInfo2->m_checkbox_line5->setChecked(false);
    else
        m_SwitchConfigInfo2->m_checkbox_line5->setChecked(true);

    m_SwitchConfigInfo2->m_combobox_line5->setCurrentIndex(_info.left_lamp_status);//左转灯 0:低电平有效　1:高电平有效

    if(0 == _info.right_lamp_enable)//右转灯 0:未选择状态 1:选中状态
        m_SwitchConfigInfo2->m_checkbox_line6->setChecked(false);
    else
        m_SwitchConfigInfo2->m_checkbox_line6->setChecked(true);

    m_SwitchConfigInfo2->m_combobox_line6->setCurrentIndex(_info.right_lamp_status);//右转灯 0:低电平有效　1:高电平有效
}







