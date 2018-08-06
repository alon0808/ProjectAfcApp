
#include "driver_menu_widget.h"
#include "./class/common_interface.h"
#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"
#include <QListView>


CBaseSetInfo::CBaseSetInfo(char _type,QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("CStorageInfo");
    this->setStyleSheet("QWidget#CStorageInfo{border:1px solid #24508F;background-color:#ffffff;}");//#03111E
    m_type = _type;
    set_type(_type);
}

void CBaseSetInfo::volumn_init()
{

    this->setFixedSize(620,ROW_HEIGHT * 3);
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 3);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 5;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("内喇叭:");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_combobox_line1 = new QComboBox(this);
    m_combobox_line1->move(660 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    for(int index = 0;index <= VOICE_MAX;index++)
        m_combobox_line1->addItem(QString::number(index));

    m_combobox_line1->setView(new QListView());

    str_show = tr("外喇叭:");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(660 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    for(int index = 0;index <= VOICE_MAX;index++)
        m_combobox_line2->addItem(QString::number(index));
    m_combobox_line2->setView(new QListView());

    str_show = tr("司机喇叭:");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(660 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    for(int index = 0;index <= VOICE_MAX;index++)
        m_combobox_line3->addItem(QString::number(index));
    m_combobox_line3->setView(new QListView());

}
void CBaseSetInfo::light_init()
{
    this->setFixedSize(620,ROW_HEIGHT * 1);
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 1);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 3;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("屏幕亮度:");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_combobox_line1 = new QComboBox(this);
    m_combobox_line1->move(660 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);

    m_combobox_line1->addItem(tr("自动"));
    m_combobox_line1->addItem(tr("非常亮"));
    m_combobox_line1->addItem(tr("一般"));
    m_combobox_line1->addItem(tr("暗"));
    m_combobox_line1->addItem(tr("非常暗"));

    m_combobox_line1->setView(new QListView());

    connect(m_combobox_line1,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_light_changed(int)));

}

void CBaseSetInfo::slot_light_changed(int index)
{
    CProtocol::getInstance()->call_json_menu_driver(CBaseSetWidget::kEnumLightSet,index,false);
}

void CBaseSetInfo::set_type(char _type)
{
    switch(_type)
    {
        case kEnumVolumn:
        {
            volumn_init();
        }break;
        case kEnumlight:
        {
            light_init();
        }break;
    }
}

CBaseSetWidget::CBaseSetWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CBaseSetWidget");
   this->setStyleSheet("QWidget#CBaseSetWidget{background-color:#ffffff;}");

    scroll_layout();
    base_set_init();

}

void CBaseSetWidget::scroll_layout()
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



void CBaseSetWidget::base_set_init()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("音量设置"));

    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    //m_button_1->move(290 - 250,158 - MAIN_HEAD_HEIGHT);

    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("亮度设置"));

    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_base_set1 = new CBaseSetInfo(CBaseSetInfo::kEnumVolumn,m_widget_main);
    m_base_set2 = new CBaseSetInfo(CBaseSetInfo::kEnumlight,m_widget_main);

    m_label_space1  = new QLabel(m_widget_main);
    m_label_space1->setFixedHeight(10);
    m_label_space1->setStyleSheet("QLabel{border:0px;background:transparent;}");


    m_vboxlayout_storage = new QVBoxLayout(m_widget_main);//左 上 右 下
    m_vboxlayout_storage->setContentsMargins(290 - 250,158 - MAIN_HEAD_HEIGHT,0,0);
    m_vboxlayout_storage->setSpacing(0);
    m_vboxlayout_storage->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_vboxlayout_storage->addWidget(m_button_1);
    m_vboxlayout_storage->addWidget(m_base_set1);
    m_vboxlayout_storage->addWidget(m_label_space1);
    m_vboxlayout_storage->addWidget(m_button_2);
    m_vboxlayout_storage->addWidget(m_base_set2);

    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));


    m_button_press_1 = false;
    m_button_press_2 = false;

    m_base_set1->hide();
    m_base_set2->hide();

}

void CBaseSetWidget::slot_button_1()
{
    if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_base_set1->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_base_set1->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    m_button_2->setSelected(false);

    m_button_press_2 = false;

    m_base_set2->hide();



}
void CBaseSetWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_base_set2->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_base_set2->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height );
    }
    m_button_1->setSelected(false);

    m_button_press_1 = false;

    m_base_set1->hide();

}

CImageSetWidget1::CImageSetWidget1(QWidget *parent) :
    QWidget(parent)
{
    QWidget *pRectWidget1 = new QWidget(this);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    QWidget *pWidgetRect2 = new QWidget(this);

    pWidgetRect2->setFixedSize(620,430 + 100);
    pWidgetRect2->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + 77);
    pWidgetRect2->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    QString str_show = tr("通道选择");
    m_label_line1 = new QLabel(this);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,300,77,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,33);


    //1~4
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:22px;color:#8ea2c0;}";
    m_checkbox_line2_1 = new QCheckBox(this);
    m_checkbox_line2_1->setFixedSize(70,70);
    m_checkbox_line2_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("1");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_2 = new QCheckBox(this);
    m_checkbox_line2_2->setFixedSize(70,70);
    m_checkbox_line2_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("2");
    m_label_line2_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_3 = new QCheckBox(this);
    m_checkbox_line2_3->setFixedSize(70,70);
    m_checkbox_line2_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("3");
    m_label_line2_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_4 = new QCheckBox(this);
    m_checkbox_line2_4->setFixedSize(70,70);
    m_checkbox_line2_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("4");
    m_label_line2_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //5~8
    m_checkbox_line3_1 = new QCheckBox(this);
    m_checkbox_line3_1->setFixedSize(70,70);
    m_checkbox_line3_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("5");
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_2 = new QCheckBox(this);
    m_checkbox_line3_2->setFixedSize(70,70);
    m_checkbox_line3_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("6");
    m_label_line3_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_3 = new QCheckBox(this);
    m_checkbox_line3_3->setFixedSize(70,70);
    m_checkbox_line3_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("7");
    m_label_line3_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_4 = new QCheckBox(this);
    m_checkbox_line3_4->setFixedSize(70,70);
    m_checkbox_line3_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("8");
    m_label_line3_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //9~12  (pon1 ~ pon10)
    m_checkbox_line4_1 = new QCheckBox(this);
    m_checkbox_line4_1->setFixedSize(70,70);
    m_checkbox_line4_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("9");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_2 = new QCheckBox(this);
    m_checkbox_line4_2->setFixedSize(70,70);
    m_checkbox_line4_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("10");
    m_label_line4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_3 = new QCheckBox(this);
    m_checkbox_line4_3->setFixedSize(70,70);
    m_checkbox_line4_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("11");
    m_label_line4_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_4 = new QCheckBox(this);
    m_checkbox_line4_4->setFixedSize(70,70);
    m_checkbox_line4_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("12");
    m_label_line4_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //13~16
    m_checkbox_line5_1 = new QCheckBox(this);
    m_checkbox_line5_1->setFixedSize(70,70);
    m_checkbox_line5_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("13");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_2 = new QCheckBox(this);
    m_checkbox_line5_2->setFixedSize(70,70);
    m_checkbox_line5_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("14");
    m_label_line5_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_3 = new QCheckBox(this);
    m_checkbox_line5_3->setFixedSize(70,70);
    m_checkbox_line5_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("15");
    m_label_line5_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_4 = new QCheckBox(this);
    m_checkbox_line5_4->setFixedSize(70,70);
    m_checkbox_line5_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("16");
    m_label_line5_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line6_1 = new QCheckBox(this);
    m_checkbox_line6_1->setFixedSize(70,70);
    m_checkbox_line6_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("4通道");
    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line6_2 = new QCheckBox(this);
    m_checkbox_line6_2->setFixedSize(70,70);
    m_checkbox_line6_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("9通道");
    m_label_line6_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line6_3 = new QCheckBox(this);
    m_checkbox_line6_3->setFixedSize(70,70);
    m_checkbox_line6_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("16通道");
    m_label_line6_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line6_4 = new QCheckBox(this);
    m_checkbox_line6_4->setFixedSize(70,70);
    m_checkbox_line6_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("9通道");
    m_label_line6_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line7_1 = new QCheckBox(this);
    m_checkbox_line7_1->setFixedSize(70,70);
    m_checkbox_line7_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line7_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line7_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("16通道");
    m_label_line7_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line6_4->hide();
    m_label_line6_4->hide();
    m_checkbox_line7_1->hide();
    m_label_line7_1->hide();

    str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_default = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_default,757 - 290 + 50,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6,100,60,str_sheet);
    m_pushbutton_default->setText(tr("默认"));

}

CSeniorSetWidget::CSeniorSetWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CSeniorSetWidget");
   this->setStyleSheet("QWidget#CSeniorSetWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();

    for(int index = 0;index < 14;index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }
    QWidget *pWidgetLine1 = new QWidget(m_widget_main);
    QWidget *pWidgetLine2 = new QWidget(m_widget_main);

    pWidgetLine1->setFixedSize(1,ROW_HEIGHT * 13);
    pWidgetLine1->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetLine1->setStyleSheet("QWidget{background-color:#1B4279;}");

    pWidgetLine2->setFixedSize(1,ROW_HEIGHT * 13);
    pWidgetLine2->move(290 - 250 + 620,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetLine2->setStyleSheet("QWidget{background-color:#1B4279;}");

    //pLine1->setPoints(QPoint(290 - 250,158 - MAIN_HEAD_HEIGHT),QPoint(290 - 250,158 - MAIN_HEAD_HEIGHT + ROW_HEIGHT * 12));


    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;
    //int alignRight = startX + 250;
    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    QString str_show = tr("行驶方向");
    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line1 = new QComboBox(m_widget_main);
    m_combobox_line1->move(650 - 250,startY + ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line1->addItem(tr("上行"));
    m_combobox_line1->addItem(tr("下行"));
    m_combobox_line1->setView(new QListView());

    //调整为第4行
    str_show = tr("副主页显示模式");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line2 = new QComboBox(m_widget_main);
    m_combobox_line2->move(650 - 250,startY + ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(tr("标准"));
    m_combobox_line2->addItem(tr("全屏"));
    m_combobox_line2->setView(new QListView());

    //调整为第3行
    str_show = tr("下客门视频切换方式");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line3 = new QComboBox(m_widget_main);
    m_combobox_line3->move(650 - 250,startY + ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("开下客门"));
    m_combobox_line3->addItem(tr("进站"));
    m_combobox_line3->addItem(tr("进站或开下客门"));
    m_combobox_line3->setView(new QListView());


    //调整为第2行
    str_show = tr("下客门视频通道");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_checkbox_line4 = new QCheckBox(m_widget_main);
    m_checkbox_line4->setFixedSize(70,70);
    m_checkbox_line4->move(650 - 250,startY + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4->setFocusPolicy(Qt::NoFocus);


    m_combobox_line4 = new QComboBox(m_widget_main);
    m_combobox_line4->move(650 - 250 + 100,startY + ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4->setFixedSize(130/*COMBOBOX_WIDTH*/,COMBOBOX_HEIGHT);
    m_combobox_line4->addItem(tr("1"));
    m_combobox_line4->addItem(tr("2"));
    m_combobox_line4->addItem(tr("3"));
    m_combobox_line4->addItem(tr("4"));
    m_combobox_line4->addItem(tr("5"));
    m_combobox_line4->addItem(tr("6"));
    m_combobox_line4->addItem(tr("7"));
    m_combobox_line4->addItem(tr("8"));
    m_combobox_line4->addItem(tr("9"));
    m_combobox_line4->addItem(tr("10"));
    m_combobox_line4->addItem(tr("11"));
    m_combobox_line4->addItem(tr("12"));
    m_combobox_line4->addItem(tr("13"));
    m_combobox_line4->addItem(tr("14"));
    m_combobox_line4->addItem(tr("15"));
    m_combobox_line4->addItem(tr("16"));
    m_combobox_line4->setView(new QListView());

    //下客门延时退出时长
    str_show = tr("下客门延时退出时长");
    m_label_line4_0 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4_0,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line4_0 = new QComboBox(m_widget_main);
    m_combobox_line4_0->move(650 - 250,startY + ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4_0->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line4_0->addItem(tr("5"));
    m_combobox_line4_0->addItem(tr("10"));
    m_combobox_line4_0->addItem(tr("15"));
    m_combobox_line4_0->addItem(tr("20"));
    m_combobox_line4_0->setView(new QListView());


    str_show = tr("倒车视频通道");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_checkbox_line5 = new QCheckBox(m_widget_main);
    m_checkbox_line5->setFixedSize(70,70);
    m_checkbox_line5->move(650 - 250,startY + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5->setFocusPolicy(Qt::NoFocus);

    m_combobox_line5 = new QComboBox(m_widget_main);
    m_combobox_line5->move(650 - 250 + 100,startY + ROW_HEIGHT * 5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5->setFixedSize(130/*COMBOBOX_WIDTH*/,COMBOBOX_HEIGHT);
    m_combobox_line5->addItem(tr("1"));
    m_combobox_line5->addItem(tr("2"));
    m_combobox_line5->addItem(tr("3"));
    m_combobox_line5->addItem(tr("4"));
    m_combobox_line5->addItem(tr("5"));
    m_combobox_line5->addItem(tr("6"));
    m_combobox_line5->addItem(tr("7"));
    m_combobox_line5->addItem(tr("8"));
    m_combobox_line5->addItem(tr("9"));
    m_combobox_line5->addItem(tr("10"));
    m_combobox_line5->addItem(tr("11"));
    m_combobox_line5->addItem(tr("12"));
    m_combobox_line5->addItem(tr("13"));
    m_combobox_line5->addItem(tr("14"));
    m_combobox_line5->addItem(tr("15"));
    m_combobox_line5->addItem(tr("16"));
    m_combobox_line5->setView(new QListView());

    str_show = tr("发车计划声道");
    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line6 = new QComboBox(m_widget_main);
    m_combobox_line6->move(650 - 250,startY + ROW_HEIGHT * 6 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line6->addItem(tr("内嗽叭"));
    m_combobox_line6->addItem(tr("司机喇叭"));
    m_combobox_line6->addItem(tr("外嗽叭"));
    m_combobox_line6->setView(new QListView());

    str_show = tr("超速警示声道");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line7 = new QComboBox(m_widget_main);
    m_combobox_line7->move(650 - 250,startY + ROW_HEIGHT * 7 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line7->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line7->addItem(tr("内嗽叭"));
    m_combobox_line7->addItem(tr("司机喇叭"));
    m_combobox_line7->addItem(tr("外嗽叭"));
    m_combobox_line7->setView(new QListView());

    str_show = tr("消息超时退出");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX,startY + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line8 = new QComboBox(m_widget_main);
    m_combobox_line8->move(650 - 250,startY + ROW_HEIGHT * 8 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line8->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line8->addItem(tr("5S"));
    m_combobox_line8->addItem(tr("10S"));
    m_combobox_line8->addItem(tr("20S"));
    m_combobox_line8->addItem(tr("30S"));
    m_combobox_line8->addItem(tr("60S"));
    m_combobox_line8->addItem(tr("永不"));
    m_combobox_line8->setView(new QListView());

    str_show = tr("视频默认画面");
    m_label_line9 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line9,startX,startY + ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line9 = new QComboBox(m_widget_main);
    m_combobox_line9->move(650 - 250,startY + ROW_HEIGHT * 9 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line9->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line9->addItem(tr("16分格"));
    m_combobox_line9->addItem(tr("9分格"));
    m_combobox_line9->addItem(tr("4分格"));
    m_combobox_line9->addItem(tr("1"));
    m_combobox_line9->addItem(tr("2"));
    m_combobox_line9->addItem(tr("3"));
    m_combobox_line9->addItem(tr("4"));
    m_combobox_line9->addItem(tr("5"));
    m_combobox_line9->addItem(tr("6"));
    m_combobox_line9->addItem(tr("7"));
    m_combobox_line9->addItem(tr("8"));
    m_combobox_line9->addItem(tr("9"));
    m_combobox_line9->addItem(tr("10"));
    m_combobox_line9->addItem(tr("11"));
    m_combobox_line9->addItem(tr("12"));
    m_combobox_line9->addItem(tr("13"));
    m_combobox_line9->addItem(tr("14"));
    m_combobox_line9->addItem(tr("15"));
    m_combobox_line9->addItem(tr("16"));
    m_combobox_line9->setView(new QListView());

    str_show = tr("视频轮循");
    m_label_line10 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line10,startX,startY + ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_checkbox_line10 = new QCheckBox(m_widget_main);
    m_checkbox_line10->setFixedSize(70,70);
    m_checkbox_line10->move(650 - 250 + 180,startY + ROW_HEIGHT * 10 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line10->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line10->setFocusPolicy(Qt::NoFocus);
    m_checkbox_line10->setEnabled(true);

    str_show = tr("轮循通道");
    m_label_line11 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line11,startX,startY + ROW_HEIGHT * 11,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line11 = new QComboBox(m_widget_main);
    m_combobox_line11->move(650 - 250,startY + ROW_HEIGHT * 11 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line11->setFixedSize(COMBOBOX_WIDTH - 100,COMBOBOX_HEIGHT);
    m_combobox_line11->addItem(tr("顺序"));
    m_combobox_line11->addItem(tr("随机"));
    m_combobox_line11->setView(new QListView());

    QString str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_line11 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_line11,650 - 250 + 150,startY + ROW_HEIGHT * 11 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line11->setText(tr("配置"));
    connect(m_pushbutton_line11,SIGNAL(clicked()),this,SLOT(slot_pushbutton_config()));

    str_show = tr("轮循间隔");
    m_label_line12 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line12,startX,startY + ROW_HEIGHT * 12,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line12 = new QComboBox(m_widget_main);
    m_combobox_line12->move(650 - 250,startY + ROW_HEIGHT * 12 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line12->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line12->addItem(tr("3S"));
    m_combobox_line12->addItem(tr("5S"));
    m_combobox_line12->addItem(tr("10S"));
    m_combobox_line12->addItem(tr("20S"));
    m_combobox_line12->addItem(tr("30S"));
    m_combobox_line12->addItem(tr("50S"));
    m_combobox_line12->setView(new QListView());

    m_pushbutton_line13 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_line13,750 - 250,startY + ROW_HEIGHT * 13 + 38,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_line13->setText(tr("默认"));

    connect(m_pushbutton_line13,SIGNAL(clicked()),this,SLOT(slot_pushbutton_default()));

    QTimer::singleShot(10,this,SLOT(slot_ablout_machine_init()));
}

void CSeniorSetWidget::slot_ablout_machine_init()
{
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 700);
}

void CSeniorSetWidget::scroll_layout()
{

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    m_scroll_widget_width = NAVIGATE_RIGHT_WIDGET_WIDTH - 30;
    m_scroll_widget_height = NAVIGATE_RIGHT_WIDGET_HEIGHT - 50 - 50;
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

    m_image_set_widget = new CImageSetWidget1(m_widget_main1);
    m_image_set_widget->move(0,0);

    m_widget_main1->hide();

}

void CSeniorSetWidget::slot_timer_init()
{
    m_widget_main1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 100);
}

void CSeniorSetWidget::slot_pushbutton_config()
{
    static bool first_init = false;
    m_widget_main1->show();
    m_widget_main->hide();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main1);

    if(!first_init)
    {
        first_init = true;
        QTimer::singleShot(10,this,SLOT(slot_timer_init()));
    }

}

void CSeniorSetWidget::slot_pushbutton_return()
{
    if(m_widget_main1->isVisible())
    {
        m_widget_main->show();
        m_widget_main1->hide();
        m_vboxlayout_widget->removeWidget(m_widget_main1);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else
    {
        emit signal_window_close();
    }
}

void CSeniorSetWidget::slot_pushbutton_default()
{
    CProtocol::getInstance()->call_json_menu_driver(CBaseSetWidget::kEnumSeniorDefault,0);
}

CDriverMenuWidget::CDriverMenuWidget(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(1024,768);
    this->setObjectName("CDriverMenuWidget");
    this->setStyleSheet("QDialog#CDriverMenuWidget{background-color:#000000;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetDriverMenu,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));

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

    m_CNavigateLeft->add_button(CNavigateLeft::kEnumBaseSet,tr("基本设置"));
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumSeniorSet,tr("高级设置"));


    m_CBaseSetWidget = NULL;
    m_CSeniorSetWidget = NULL;

    m_stackwidget_navigate_right = new QStackedWidget(this);
    m_stackwidget_navigate_right->move(MAIN_NAVIGATE_LEFT,MAIN_HEAD_HEIGHT);
    m_stackwidget_navigate_right->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_stackwidget_navigate_right->setObjectName(QString::fromUtf8("m_stackwidget_navigate_right"));
    m_stackwidget_navigate_right->setContentsMargins(0, 0, 0, 0);

    connect(m_CNavigateLeft,SIGNAL(signalCurrentClassificationChanged(int,QString)),this,SLOT(slot_driver_menu_widget_event(int,QString)));

    connect(this,SIGNAL(signal_set_default_id(int)),m_CNavigateLeft,SLOT(slotSetDefaultSelected(int)));

    emit signal_set_default_id(CNavigateLeft::kEnumBaseSet);
}

void CDriverMenuWidget::slot_pushbutton_home()
{
    save_data();
    m_goto_main_page = true;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_home();
}

void CDriverMenuWidget::slot_pushbutton_close()
{
    save_data();
    m_goto_main_page = false;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_menu();
}

void CDriverMenuWidget::slot_pushbutton_back()
{

    if(CNavigateLeft::kEnumSeniorSet == m_current_index)
    {
        emit signal_driver_menu_user_return();
    }
    else
    {
        save_data();
        m_goto_main_page = false;
        CCommonInterface::input_panel_control(false);
        emit signal_pushbutton_menu();
    }

}

void CDriverMenuWidget::slot_driver_menu_widget_event(int _type,QString _text)
{
    m_CHeadWidget->set_head_widget_title(_text);

    switch(_type)
    {
        case CNavigateLeft::kEnumBaseSet://基本设置
        {
            show_base_set();
        }break;
        case CNavigateLeft::kEnumSeniorSet://高级设置
        {
            show_senior_set();
        }break;
    }
    m_current_index = _type;
}

void CDriverMenuWidget::show_base_set()     //基本设置
{
    if(NULL == m_CBaseSetWidget)
    {
        m_CBaseSetWidget = new CBaseSetWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CBaseSetWidget);
        m_CBaseSetWidget->data_init();
    }

    save_data();
    m_stackwidget_navigate_right->setCurrentWidget(m_CBaseSetWidget);
}
void CDriverMenuWidget::show_senior_set()  //高级设置
{
    if(NULL == m_CSeniorSetWidget)
    {
        m_CSeniorSetWidget = new CSeniorSetWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CSeniorSetWidget);
        connect(m_CSeniorSetWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_driver_menu_user_return()),m_CSeniorSetWidget,SLOT(slot_pushbutton_return()));

        read_senior_set_info();
        CProtocol::getInstance()->call_json_menu_driver(CBaseSetWidget::kEnumSeniorGet,0);
    }
    save_data();
    m_stackwidget_navigate_right->setCurrentWidget(m_CSeniorSetWidget);
}

/////////////////////////////////////////////////////////////////////////////////数据交互区

#include "class/cJSON.h"
#include "common_data.h"
#include "class/ltyuicallinterface.h"

void CBaseSetWidget::data_init()
{
    CProtocol::getInstance()->call_json_menu_driver(kEnumVolumeGet,0);
    CProtocol::getInstance()->call_json_menu_driver(kEnumLightGet,0);


    if(CCommonData::getInstance()->m_CGuiVoice.m_volume_within != 0)
    {
        m_base_set1->m_combobox_line1->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_within - 1);
    }

    if(CCommonData::getInstance()->m_CGuiVoice.m_volume_out != 0)
    {
        m_base_set1->m_combobox_line2->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_out - 1);
    }

    if(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS != 0)
    {
        m_base_set1->m_combobox_line3->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS - 1);
    }
    m_base_set2->m_combobox_line1->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_light);

}

void call_json_test()
{
        do
    {
        cJSON *root = NULL, *rows = NULL, *row = NULL, *getObject = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        //////////////////////////////////////////////////存储数据 start
        cJSON_AddNumberToObject(root, "size", 20);

        // 在object中加入array
        rows = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "rows",rows);

        for(int index = 0; index < 20;index++) {
        // 在array中加入object
            row = cJSON_CreateObject();
            cJSON_AddItemToArray(rows,row);
            cJSON_AddNumberToObject(row, "value", index);

        }
        //////////////////////////////////////////////////存储数据 end

        //////////////////////////////////////////////////获取数据 start

        int array_size = 0;
        getObject = cJSON_GetObjectItem(root,"size");
        if(getObject)
        {
            array_size = getObject->valueint;
        }

        cJSON *taskArry=cJSON_GetObjectItem(root,"rows");//取数组
        int arrySize=cJSON_GetArraySize(taskArry);//数组大小
        cJSON *tasklist=taskArry->child;//子对象
        int get_value = 0;
        for(int index = 0;index < arrySize;index++)
        {
            getObject = cJSON_GetObjectItem(tasklist,"value");
            if(getObject)
            {
                get_value = getObject->valueint;
            }
            tasklist = tasklist->next;
            if(NULL == tasklist)
            {
                break;
            }
        }
#if 0
        while(tasklist!=NULL)
        {
                    printf("%s\n",cJSON_GetObjectItem(tasklist,"value")->valuestring);
            tasklist=tasklist->next;
        }
#endif
        //////////////////////////////////////////////////获取数据 end
        char *out;
        out = cJSON_Print(root);
        printf("%s\n",out);
        free(out);
        //gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}


/*
    kEnumPopDialogNull,
    kEnumPopDialogClickOk,
    kEnumPopDialogClickCancel
*/
void CDriverMenuWidget::save_data()
{
   char dialog_status = kEnumPopDialogNull;
   switch(m_current_index)
   {
        case CNavigateLeft::kEnumBaseSet://基本设置
        {
            int volume_within = m_CBaseSetWidget->m_base_set1->m_combobox_line1->currentIndex();
            int volume_out = m_CBaseSetWidget->m_base_set1->m_combobox_line2->currentIndex();
            int volume_TTS = m_CBaseSetWidget->m_base_set1->m_combobox_line3->currentIndex();
            m_CBaseSetWidget->m_base_set1->m_combobox_line3->currentIndex();

            if(CCommonData::getInstance()->m_CGuiVoice.m_volume_within != volume_within
            || CCommonData::getInstance()->m_CGuiVoice.m_volume_out != volume_out
            || CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS != volume_TTS)
            {
                if(CCommonData::getInstance()->messagebox_data_change())
                {
                    CCommonData::getInstance()->m_CGuiVoice.m_volume_within = volume_within;
                    CCommonData::getInstance()->m_CGuiVoice.m_volume_out = volume_out;
                    CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS = volume_TTS;
                    CProtocol::getInstance()->call_json_menu_driver(CBaseSetWidget::kEnumVolumeSet,0);
                    dialog_status = kEnumPopDialogClickOk;
                }
                else
                {
                    m_CBaseSetWidget->m_base_set1->m_combobox_line1->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_within);
                    m_CBaseSetWidget->m_base_set1->m_combobox_line2->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_out);
                    m_CBaseSetWidget->m_base_set1->m_combobox_line3->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS);
                    dialog_status = kEnumPopDialogClickCancel;
                }
            }
            //m_base_set2->m_combobox_line1->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_light);
            if(m_CBaseSetWidget->m_base_set2->m_combobox_line1->currentIndex() != CCommonData::getInstance()->m_CGuiVoice.m_light)
            {
                if(kEnumPopDialogClickOk == dialog_status)
                {
                    CCommonData::getInstance()->m_CGuiVoice.m_light = m_CBaseSetWidget->m_base_set2->m_combobox_line1->currentIndex();
                }
                else if(kEnumPopDialogNull == dialog_status)
                {
                    if(CCommonData::getInstance()->messagebox_data_change())
                    {
                        CCommonData::getInstance()->m_CGuiVoice.m_light = m_CBaseSetWidget->m_base_set2->m_combobox_line1->currentIndex();
                        CProtocol::getInstance()->call_json_menu_driver(CBaseSetWidget::kEnumLightSet,CCommonData::getInstance()->m_CGuiVoice.m_light);
                    }
                    else
                    {
                        m_CBaseSetWidget->m_base_set2->m_combobox_line1->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_light);
                    }
                }
                else
                {
                    m_CBaseSetWidget->m_base_set2->m_combobox_line1->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_light);
                }

            }


        }break;
        case CNavigateLeft::kEnumSeniorSet://高级设置
        {
            //Qt::CheckState state = m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_1->checkState();

            CDriverMenuInfo driverMenuInfo;
            driverMenuInfo.travel_direction = m_CSeniorSetWidget->m_combobox_line1->currentIndex();//行驶方向
            driverMenuInfo.deputy_page_show_mode = m_CSeniorSetWidget->m_combobox_line2->currentIndex();//副主页显示模式
            driverMenuInfo.out_door_video_change = m_CSeniorSetWidget->m_combobox_line3->currentIndex();//下客门视频切换方式
            driverMenuInfo.out_door_video_passageway = m_CSeniorSetWidget->m_combobox_line4->currentIndex();/*下客门视频通道*/
            driverMenuInfo.back_car_video_passageway = m_CSeniorSetWidget->m_combobox_line5->currentIndex();//倒车视频通道
            driverMenuInfo.start_plan_passageway = m_CSeniorSetWidget->m_combobox_line6->currentIndex();//发车计划声道
            driverMenuInfo.over_speed_warning_passageway = m_CSeniorSetWidget->m_combobox_line7->currentIndex();//超速警示声道
            driverMenuInfo.message_outtime = m_CSeniorSetWidget->m_combobox_line8->currentIndex();//消息超时退出
            driverMenuInfo.video_default_frame = m_CSeniorSetWidget->m_combobox_line9->currentIndex();/*视频默认画面*/
            driverMenuInfo.video_round_robin = (m_CSeniorSetWidget->m_checkbox_line10->checkState() == Qt::Checked)? 1:0;;//视频轮循
            driverMenuInfo.round_robin_passageway = m_CSeniorSetWidget->m_combobox_line11->currentIndex();//轮循通道
            driverMenuInfo.round_robin_interval = m_CSeniorSetWidget->m_combobox_line12->currentIndex();//轮循间隔

            //add
            driverMenuInfo.out_door_video_passageway_enable = (m_CSeniorSetWidget->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//下客门视频通道使能
            driverMenuInfo.back_car_video_passageway_enable = (m_CSeniorSetWidget->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//倒车视频通道使能
            driverMenuInfo.out_door_delay_quit = m_CSeniorSetWidget->m_combobox_line4_0->currentIndex(); //下客门延时退出时长

            driverMenuInfo.passageway[0] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_1->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[1] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_2->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[2] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_3->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[3] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_4->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[4] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_1->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[5] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_2->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[6] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_3->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[7] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_4->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[8] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_1->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[9] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_2->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[10] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_3->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[11] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_4->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[12] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_1->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[13] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_2->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[14] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_3->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[15] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_4->checkState() == Qt::Checked)? 1:0;

            driverMenuInfo.passageway[16] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_1->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[17] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_2->checkState() == Qt::Checked)? 1:0;
            driverMenuInfo.passageway[18] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_3->checkState() == Qt::Checked)? 1:0;
            //driverMenuInfo.passageway[19] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_4->checkState() == Qt::Checked)? 1:0;
            //driverMenuInfo.passageway[20] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line7_1->checkState() == Qt::Checked)? 1:0;



            if(driverMenuInfo.travel_direction != CCommonData::getInstance()->m_CDriverMenuInfo.travel_direction ||
            driverMenuInfo.deputy_page_show_mode != CCommonData::getInstance()->m_CDriverMenuInfo.deputy_page_show_mode ||
            driverMenuInfo.out_door_video_change != CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_change ||
            driverMenuInfo.out_door_video_passageway != CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway ||
            driverMenuInfo.back_car_video_passageway != CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway ||
            driverMenuInfo.start_plan_passageway != CCommonData::getInstance()->m_CDriverMenuInfo.start_plan_passageway ||
            driverMenuInfo.over_speed_warning_passageway != CCommonData::getInstance()->m_CDriverMenuInfo.over_speed_warning_passageway ||
            driverMenuInfo.message_outtime != CCommonData::getInstance()->m_CDriverMenuInfo.message_outtime ||
            driverMenuInfo.video_default_frame != CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame ||
            driverMenuInfo.video_round_robin != CCommonData::getInstance()->m_CDriverMenuInfo.video_round_robin ||
            driverMenuInfo.round_robin_passageway != CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_passageway ||
            driverMenuInfo.round_robin_interval != CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval ||
            //add
            driverMenuInfo.out_door_video_passageway_enable != CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway_enable ||
            driverMenuInfo.back_car_video_passageway_enable != CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway_enable ||
            driverMenuInfo.out_door_delay_quit != CCommonData::getInstance()->m_CDriverMenuInfo.out_door_delay_quit ||
            driverMenuInfo.passageway[0] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[0] ||
            driverMenuInfo.passageway[1] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[1] ||
            driverMenuInfo.passageway[2] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[2] ||
            driverMenuInfo.passageway[3] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[3] ||
            driverMenuInfo.passageway[4] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[4] ||
            driverMenuInfo.passageway[5] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[5] ||
            driverMenuInfo.passageway[6] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[6] ||
            driverMenuInfo.passageway[7] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[7] ||
            driverMenuInfo.passageway[8] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[8] ||
            driverMenuInfo.passageway[9] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[9] ||
            driverMenuInfo.passageway[10] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[10] ||
            driverMenuInfo.passageway[11] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[11] ||
            driverMenuInfo.passageway[12] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[12] ||
            driverMenuInfo.passageway[13] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[13] ||
            driverMenuInfo.passageway[14] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[14] ||
            driverMenuInfo.passageway[15] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[15] ||
            driverMenuInfo.passageway[16] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[16] ||
            driverMenuInfo.passageway[17] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[17] ||
            driverMenuInfo.passageway[18] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[18] ||
            driverMenuInfo.passageway[19] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[19] ||
            driverMenuInfo.passageway[20] != CCommonData::getInstance()->m_CDriverMenuInfo.passageway[20])
            {
                if(CCommonData::getInstance()->messagebox_data_change())
                {
                    //1.保存到缓存
                    read_senior_set_info();
                    //2.通知保存到本地文件
                    CProtocol::getInstance()->call_json_menu_driver(CBaseSetWidget::kEnumSeniorSet,0);
                }
                else
                {
                    //恢复到原来数据
                    slot_driver_menu_widget_event(CCommonData::getInstance()->m_CDriverMenuInfo);
                }
            }

        }break;
   }
}

void CDriverMenuWidget::slot_driver_menu_voice_event(int _value1,int _value2,int _value3)
{
    CCommonData::getInstance()->m_CGuiVoice.m_volume_within = _value1;
    CCommonData::getInstance()->m_CGuiVoice.m_volume_out = _value2;
    CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS = _value3;

    m_CBaseSetWidget->m_base_set1->m_combobox_line1->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_within);
    m_CBaseSetWidget->m_base_set1->m_combobox_line2->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_out);
    m_CBaseSetWidget->m_base_set1->m_combobox_line3->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS);

}

void CDriverMenuWidget::slot_driver_menu_light_event(int _value)
{
    CCommonData::getInstance()->m_CGuiVoice.m_light = _value;
    m_CBaseSetWidget->m_base_set2->m_combobox_line1->setCurrentIndex(CCommonData::getInstance()->m_CGuiVoice.m_light);
}



void CDriverMenuWidget::slot_driver_menu_widget_event(const CDriverMenuInfo &_driverMenuInfo)
{
    do
    {
        if(NULL == m_CSeniorSetWidget)
        {
            break;
        }

        m_CSeniorSetWidget->m_combobox_line1->setCurrentIndex(_driverMenuInfo.travel_direction);//行驶方向
        m_CSeniorSetWidget->m_combobox_line2->setCurrentIndex(_driverMenuInfo.deputy_page_show_mode);//副主页显示模式
        m_CSeniorSetWidget->m_combobox_line3->setCurrentIndex(_driverMenuInfo.out_door_video_change);//下客门视频切换方式
        m_CSeniorSetWidget->m_combobox_line4->setCurrentIndex(_driverMenuInfo.out_door_video_passageway);//下客门视频通道
        m_CSeniorSetWidget->m_combobox_line5->setCurrentIndex(_driverMenuInfo.back_car_video_passageway);//倒车视频通道
        m_CSeniorSetWidget->m_combobox_line6->setCurrentIndex(_driverMenuInfo.start_plan_passageway);//发车计划声道
        m_CSeniorSetWidget->m_combobox_line7->setCurrentIndex(_driverMenuInfo.over_speed_warning_passageway);//超速警示声道
        m_CSeniorSetWidget->m_combobox_line8->setCurrentIndex(_driverMenuInfo.message_outtime);//消息超时退出
        m_CSeniorSetWidget->m_combobox_line9->setCurrentIndex(_driverMenuInfo.video_default_frame);//视频默认画面

        if(1 == _driverMenuInfo.video_round_robin)//视频轮循
            m_CSeniorSetWidget->m_checkbox_line10->setChecked(true);
        else
            m_CSeniorSetWidget->m_checkbox_line10->setChecked(false);

        m_CSeniorSetWidget->m_combobox_line11->setCurrentIndex(_driverMenuInfo.round_robin_passageway);//轮循通道
        m_CSeniorSetWidget->m_combobox_line12->setCurrentIndex(_driverMenuInfo.round_robin_passageway);//轮循间隔

        if(1 == _driverMenuInfo.out_door_video_passageway_enable)//下客门视频通道使能
            m_CSeniorSetWidget->m_checkbox_line4->setChecked(true);
        else
            m_CSeniorSetWidget->m_checkbox_line4->setChecked(false);

        if(1 == _driverMenuInfo.back_car_video_passageway_enable) //倒车视频通道使能
            m_CSeniorSetWidget->m_checkbox_line5->setChecked(true);
        else
            m_CSeniorSetWidget->m_checkbox_line5->setChecked(false);

        m_CSeniorSetWidget->m_combobox_line4_0->setCurrentIndex(_driverMenuInfo.out_door_delay_quit); //下客门延时退出时长

        bool get_value = false;
        //通道选择
        for(int index = 0;index < 21;index++)
        {
            get_value = (_driverMenuInfo.passageway[index] == 0)? false:true;

            if(0 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_1->setChecked(get_value);
            else if(1 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_2->setChecked(get_value);
            else if(2 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_3->setChecked(get_value);
            else if(3 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_4->setChecked(get_value);
            else if(4 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_1->setChecked(get_value);
            else if(5 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_2->setChecked(get_value);
            else if(6 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_3->setChecked(get_value);
            else if(7 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_4->setChecked(get_value);
            else if(8 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_1->setChecked(get_value);
            else if(9 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_2->setChecked(get_value);
            else if(10 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_3->setChecked(get_value);
            else if(11 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_4->setChecked(get_value);
            else if(12 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_1->setChecked(get_value);
            else if(13 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_2->setChecked(get_value);
            else if(14 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_3->setChecked(get_value);
            else if(15 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_4->setChecked(get_value);
            else if(16 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_1->setChecked(get_value);
            else if(17 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_2->setChecked(get_value);
            else if(18 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_3->setChecked(get_value);
#if 0
            else if(19 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_4->setEnabled(_driverMenuInfo.passageway[index]);
            else if(20 == index)
                m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line7_1->setEnabled(_driverMenuInfo.passageway[index]);
#endif

        }


    }while(0);
}

void CDriverMenuWidget::read_senior_set_info()
{
    do
    {
        if(NULL == m_CSeniorSetWidget)
        {
            break;
        }

    CCommonData::getInstance()->m_CDriverMenuInfo.travel_direction = m_CSeniorSetWidget->m_combobox_line1->currentIndex();//行驶方向
    CCommonData::getInstance()->m_CDriverMenuInfo.deputy_page_show_mode = m_CSeniorSetWidget->m_combobox_line2->currentIndex();//副主页显示模式
    CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_change = m_CSeniorSetWidget->m_combobox_line3->currentIndex();//下客门视频切换方式
    CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway = m_CSeniorSetWidget->m_combobox_line4->currentIndex();/*下客门视频通道*/
    CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway = m_CSeniorSetWidget->m_combobox_line5->currentIndex();//倒车视频通道
    CCommonData::getInstance()->m_CDriverMenuInfo.start_plan_passageway = m_CSeniorSetWidget->m_combobox_line6->currentIndex();//发车计划声道
    CCommonData::getInstance()->m_CDriverMenuInfo.over_speed_warning_passageway = m_CSeniorSetWidget->m_combobox_line7->currentIndex();//超速警示声道
    CCommonData::getInstance()->m_CDriverMenuInfo.message_outtime = m_CSeniorSetWidget->m_combobox_line8->currentIndex();//消息超时退出
    CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame = m_CSeniorSetWidget->m_combobox_line9->currentIndex();/*视频默认画面*/
    CCommonData::getInstance()->m_CDriverMenuInfo.video_round_robin = (m_CSeniorSetWidget->m_checkbox_line10->checkState() == Qt::Checked)? 1:0;;//视频轮循
    CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_passageway = m_CSeniorSetWidget->m_combobox_line11->currentIndex();//轮循通道
    CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval = m_CSeniorSetWidget->m_combobox_line12->currentIndex();//轮循间隔

    //add
    CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway_enable = (m_CSeniorSetWidget->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//下客门视频通道使能
    CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway_enable = (m_CSeniorSetWidget->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//倒车视频通道使能
    CCommonData::getInstance()->m_CDriverMenuInfo.out_door_delay_quit = m_CSeniorSetWidget->m_combobox_line4_0->currentIndex(); //下客门延时退出时长

    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[0] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_1->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[1] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_2->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[2] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_3->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[3] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line2_4->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[4] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_1->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[5] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_2->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[6] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_3->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[7] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line3_4->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[8] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_1->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[9] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_2->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[10] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_3->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[11] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line4_4->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[12] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_1->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[13] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_2->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[14] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_3->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[15] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line5_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[16] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_1->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[17] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_2->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CDriverMenuInfo.passageway[18] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_3->checkState() == Qt::Checked)? 1:0;
    //CCommonData::getInstance()->m_CDriverMenuInfo.passageway[19] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line6_4->checkState() == Qt::Checked)? 1:0;
    //CCommonData::getInstance()->m_CDriverMenuInfo.passageway[20] = (m_CSeniorSetWidget->m_image_set_widget->m_checkbox_line7_1->checkState() == Qt::Checked)? 1:0;



    }while(0);
}































