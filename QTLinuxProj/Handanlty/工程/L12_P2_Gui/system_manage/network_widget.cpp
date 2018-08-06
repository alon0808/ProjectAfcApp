#include "network_widget.h"

#include "LtyCommonDefine.h"
#include "./class/common_interface.h"
#include "LtyCommonStyle.h"
#include <QListView>
#include "network_select.h"
#include "mainwindow.h"
#include "main_page.h"
//protocol
#define PROTOCOL_TYPE_0   tr("关闭")
#define PROTOCOL_TYPE_1   tr("蓝泰源V1.0")


CNetworkInfo::CNetworkInfo(char _type,QWidget *parent) :
    QWidget(parent)
{
    m_type = _type;

    this->setObjectName("CNetworkInfo");
    this->setStyleSheet("QWidget#CNetworkInfo{border:1px solid #24508F;background-color:#ffffff;}");

    Network_info_init(m_type);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this,false);
}

void CNetworkInfo::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CNetworkInfo::move_network_init()   //移动网络
{

    this->setFixedSize(620,ROW_HEIGHT * 9);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 9);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");


    for(int index = 0;index < 10;index++)
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


    //str_show = tr("模块型号");
    //m_label_line2 = new QLabel(this);
    //CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
    m_button_line2 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line2->move(0, ROW_HEIGHT * 1);
    m_button_line2->setDisplayText(tr("模块型号"));
    m_button_line2->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setText1Text2(tr("龙尚"),tr("8300C"));

    connect(m_button_line2,SIGNAL(clicked()),this,SIGNAL(signal_button_line2()));

    str_show = tr("运营商");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("自动"));
    m_combobox_line3->addItem(tr("手动"));
    m_combobox_line3->setView(new QListView());

    str_show = tr("网络类型");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
    m_combobox_line4 = new QComboBox(this);
    m_combobox_line4->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line4->addItem(tr("4G"));
    m_combobox_line4->addItem(tr("3G"));
    m_combobox_line4->addItem(tr("2G"));
    m_combobox_line4->setView(new QListView());

    str_show = tr("月结日");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
    m_combobox_line5 = new QComboBox(this);
    m_combobox_line5->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    for(int index = 1;index <= 31;index++)
        m_combobox_line5->addItem(QString::number(index));
    m_combobox_line5->setView(new QListView());

    str_show = tr("是否使用VPN");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_checkbox_line6 = new QCheckBox(this);
    m_checkbox_line6->setFixedSize(70,70);
    m_checkbox_line6->move(620 - 70,ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6->setFocusPolicy(Qt::NoFocus);

    connect(m_checkbox_line6,SIGNAL(stateChanged(int)),this,SLOT(slot_checkbox_changed_line6(int)));

    str_show = tr("VPN名称");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
    m_lineedit7 = new QLineEdit(this);
    m_lineedit7->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit7->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit7);


    str_show = tr("VPN账号");
    m_label_line8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8,30, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
    m_lineedit8 = new QLineEdit(this);
    m_lineedit8->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit8->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit8);


    str_show = tr("VPN密码");
    m_label_line9 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line9,30, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);
    m_lineedit9 = new QLineEdit(this);
    m_lineedit9->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 8 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit9->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit9);
    m_lineedit9->setEchoMode(QLineEdit::Password);

}

void CNetworkInfo::slot_checkbox_changed_line6(int _status)
{
    if(Qt::Checked == _status)
    {
        m_lineedit7->setReadOnly(false);
        m_lineedit8->setReadOnly(false);
        m_lineedit9->setReadOnly(false);
    }
    else if(Qt::Unchecked == _status)
    {
        m_lineedit7->setReadOnly(true);
        m_lineedit8->setReadOnly(true);
        m_lineedit9->setReadOnly(true);
    }
}

void CNetworkInfo::center_server1_init() //中心服务器1
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
    QString str_show = tr("使能");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(620 - 70,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    str_show = tr("协议类型");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(PROTOCOL_TYPE_0);
    m_combobox_line2->addItem(PROTOCOL_TYPE_1);
    m_combobox_line2->setView(new QListView());

    QString str_data = "";
    CCommonInterface::get_config_info("center_server1",str_data);

    QStringList str_list = str_data.split(",");

    str_show = tr("接入模式");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("固定IP"));
    m_combobox_line3->addItem(tr("域名"));
    m_combobox_line3->setView(new QListView());


    str_show = tr("IP地址");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
    m_lineedit4 = new QLineEdit(this);
    m_lineedit4->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit4->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit4);

    str_show = tr("端口");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
    m_lineedit5 = new QLineEdit(this);
    m_lineedit5->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 4 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit5->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit5,true);

    str_show = tr("数据发送间隔");
    QString str_miao = tr("秒");

    int str_length = CCommonInterface::get_string_length(str_miao,26);

    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);
    m_lineedit6 = new QLineEdit(this);
    m_lineedit6->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit6->setFixedSize(EDIT_WIDTH - str_length - 2,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit6,true);

    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,620 - 25 - str_length + 2, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_miao);

}

void CNetworkInfo::video_server_init()   //视频服务器
{
    this->setFixedSize(620,ROW_HEIGHT * 5);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 5);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");


    for(int index = 0;index < 6;index++)
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
    str_show = tr("协议类型");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(PROTOCOL_TYPE_0);
    m_combobox_line2->addItem(PROTOCOL_TYPE_1);
    m_combobox_line2->setView(new QListView());

#if 0
    m_pushbutton_line2 = new CMyPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line2,620 - 70,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2,70,70);
    m_pushbutton_line2->set_pushbutton_enable(true);
    m_pushbutton_line2->set_pushbutton_sheet("QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}",
                                            "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}");
#endif



    str_show = tr("接入模式");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("固定IP"));
    m_combobox_line3->addItem(tr("域名"));
    m_combobox_line3->setView(new QListView());


    str_show = tr("IP地址");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
    m_lineedit4 = new QLineEdit(this);
    m_lineedit4->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit4->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit4);

    str_show = tr("端口");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
    m_lineedit5 = new QLineEdit(this);
    m_lineedit5->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 4 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit5->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit5,true);


}
void CNetworkInfo::network_wifi_init(char _type)//网络和WIFI共用一份数据
{
    this->setFixedSize(620,ROW_HEIGHT * 8);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 8);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");


    for(int index = 0;index < 9;index++)
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

    if(kEnumLocalNetwork == _type)
        str_show = tr("DHCP服务");
    else
        str_show = tr("接入点");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    if(kEnumLocalNetwork == _type)
    {
#if 0
        m_pushbutton_line2 = new CMyPushButton(this);
        CCommonInterface::init_new_button(m_pushbutton_line2,620 - 70,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2,70,70);
        m_pushbutton_line2->set_pushbutton_enable(true);
        m_pushbutton_line2->set_pushbutton_sheet("QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}",
                                                "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}");


#endif
        m_checkbox_line2 = new QCheckBox(this);
        m_checkbox_line2->setFixedSize(70,70);
        m_checkbox_line2->move(620 - 70,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
        m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    }
    else
    {
        QString str_sheet = BUTTON_CONFIG_SHEET;

        m_pushbutton_line2_1 = new QPushButton(this);
        CCommonInterface::init_new_button(m_pushbutton_line2_1,799 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
        m_pushbutton_line2_1->setText(tr("搜索"));
        connect(m_pushbutton_line2_1,SIGNAL(clicked()),this,SIGNAL(signal_button_wifi_search()));
    }

    str_show = tr("接入模式");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("静态IP"));
    m_combobox_line3->addItem(tr("自动获取"));
    m_combobox_line3->setView(new QListView());


    connect(m_combobox_line3,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_m_combobox_changed_line3(int)));

    str_show = tr("IP地址");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
    m_lineedit4 = new QLineEdit(this);
    m_lineedit4->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit4->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit4);

    str_show = tr("子网掩码");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
    m_lineedit5 = new QLineEdit(this);
    m_lineedit5->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 4 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit5->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit5);

    str_show = tr("网关");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);
    m_lineedit6 = new QLineEdit(this);
    m_lineedit6->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit6->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit6);

    str_show = tr("DNS1");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
    m_lineedit7 = new QLineEdit(this);
    m_lineedit7->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit7->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit7);

    str_show = tr("DNS2");
    m_label_line8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8,30, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
    m_lineedit8 = new QLineEdit(this);
    m_lineedit8->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit8->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit8);
}

void CNetworkInfo::slot_m_combobox_changed_line3(int _index)
{
    if(0 == _index)
    {
        m_lineedit4->setReadOnly(false);
        m_lineedit5->setReadOnly(false);
        m_lineedit6->setReadOnly(false);
        m_lineedit7->setReadOnly(false);
        m_lineedit8->setReadOnly(false);

    }
    else if(1 == _index)
    {
        m_lineedit4->setReadOnly(true);
        m_lineedit5->setReadOnly(true);
        m_lineedit6->setReadOnly(true);
        m_lineedit7->setReadOnly(true);
        m_lineedit8->setReadOnly(true);
    }
}

void CNetworkInfo::local_network_init(char _type)  //本地网络
{
    network_wifi_init(_type);

}
void CNetworkInfo::wifi_init(char _type)          //WIFI
{
    network_wifi_init(_type);
}
void CNetworkInfo::ftp_server_init()      //FTP服务器
{
    this->setFixedSize(620,ROW_HEIGHT * 4);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 4);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");


    for(int index = 0;index < 5;index++)
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

    str_show = tr("连接方式");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(620 - 25 - COMBOBOX_WIDTH,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(tr("静态IP"));
    m_combobox_line2->addItem(tr("域名"));
    m_combobox_line2->setView(new QListView());


    str_show = tr("IP地址");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
    m_lineedit3 = new QLineEdit(this);
    m_lineedit3->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit3->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit3);

    str_show = tr("网关");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
    m_lineedit4 = new QLineEdit(this);
    m_lineedit4->move(620 - 25 - EDIT_WIDTH,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit4->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit4);
}

void CNetworkInfo::Network_info_init(char _type)
{
    switch(_type)
    {

        case kEnumMoveNetwork:   //移动网络
        {
            move_network_init();
        }break;
        case kEnumCenterServer1: //中心服务器1
        {
            center_server1_init();
        }break;
        case kEnumCenterServer2: //中心服务器2
        {
            center_server1_init();
        }break;
        case kEnumVideoServer:   //视频服务器
        {
            video_server_init();
        }break;
        case kEnumLocalNetwork:  //本地网络
        {
            local_network_init(_type);
        }break;
        case kEnumWifi:          //WIFI
        {
            wifi_init(_type);
        }break;
        case kEnumFtpServer:      //FTP服务器
        {
            ftp_server_init();
        }break;
    }
}


CNetworkWidget::CNetworkWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

    this->setObjectName("CNetworkWidget");
    this->setStyleSheet("QWidget#CNetworkWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

        QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    scroll_layout();
    network_init_new();
    m_CNetworkSelect = NULL;


    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);
    m_define_screen->add_parent(m_widget_main1);
}

void CNetworkWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CNetworkWidget::scroll_layout()
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

    m_widget_main1 = new QWidget;
    m_widget_main1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_widget_main1->setStyleSheet("QWidget{background: transparent;}");
    m_widget_main1->hide();

    model_type_init();

    QTimer::singleShot(10,this,SLOT(slot_timer_init()));
}

void CNetworkWidget::slot_timer_init()
{
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
#endif

    read_data();

}

void CNetworkWidget::model_type_init()
{
    QWidget *pRectWidget = new QWidget(m_widget_main1);
    pRectWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget->setFixedSize(620,77);
    pRectWidget->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    for(int index = 0;index < 6;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_main1);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
    QWidget *pLine1 = new QWidget(m_widget_main1);
    QWidget *pLine2 = new QWidget(m_widget_main1);
     pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,4 * ROW_HEIGHT);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,4 * ROW_HEIGHT);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    int alignRight = 290 - 250 + 300;
   // m_label_main1_line1
    QString str_show = tr("模块信息");
    m_label_main1_line1 = new QLabel(m_widget_main1);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_main1_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("软件版本");
    m_label_main1_line2 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_show = tr("V2.0.102.456");
    m_label_main1_line2_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line2_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);



    str_show = tr("IMEI号");
    m_label_main1_line3 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("1121015252245323");
    m_label_main1_line3_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line3_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("S/N号");
    m_label_main1_line4 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line4,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = tr("SA1023554663285");
    m_label_main1_line4_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line4_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("MEID");
    m_label_main1_line5 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line5,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    str_show = tr("A1000038E79DF49");
    m_label_main1_line5_1 = new QLabel(m_widget_main1);
    CCommonInterface::init_label_text(m_label_main1_line5_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


}


void CNetworkWidget::network_init_new()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("移动网络"));
    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("中心服务器1"));
    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_3 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_3->setDisplayText(tr("中心服务器2"));
    m_button_3->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_3->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_3->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_4 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_4->setDisplayText(tr("视频服务器"));
    m_button_4->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_4->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_4->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_5 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_5->setDisplayText(tr("本地网络"));
    m_button_5->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_5->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_5->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_6 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_6->setDisplayText(tr("WIFI"));
    m_button_6->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_6->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_6->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_6->hide();

    m_button_7 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_7->setDisplayText(tr("FTP服务器"));
    m_button_7->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_7->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_7->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_7->hide();

    m_NetworkInfo1 = new CNetworkInfo(CNetworkInfo::kEnumMoveNetwork,m_widget_main);
    m_NetworkInfo2 = new CNetworkInfo(CNetworkInfo::kEnumCenterServer1,m_widget_main);
    m_NetworkInfo3 = new CNetworkInfo(CNetworkInfo::kEnumCenterServer2,m_widget_main);
    m_NetworkInfo4 = new CNetworkInfo(CNetworkInfo::kEnumVideoServer,m_widget_main);
    m_NetworkInfo5 = new CNetworkInfo(CNetworkInfo::kEnumLocalNetwork,m_widget_main);
    m_NetworkInfo6 = new CNetworkInfo(CNetworkInfo::kEnumWifi,m_widget_main);
    m_NetworkInfo7 = new CNetworkInfo(CNetworkInfo::kEnumFtpServer,m_widget_main);

    connect(m_NetworkInfo1,SIGNAL(signal_button_line2()),this,SLOT(slot_button_model_type()));
    connect(m_NetworkInfo6,SIGNAL(signal_button_wifi_search()),this,SLOT(slot_button_wifi_search()));


    m_label_space1  = new QLabel(m_widget_main);
    m_label_space1->setFixedHeight(10);
    m_label_space1->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space2  = new QLabel(m_widget_main);
    m_label_space2->setFixedHeight(10);
    m_label_space2->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space3  = new QLabel(m_widget_main);
    m_label_space3->setFixedHeight(10);
    m_label_space3->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space4  = new QLabel(m_widget_main);
    m_label_space4->setFixedHeight(10);
    m_label_space4->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space5  = new QLabel(m_widget_main);
    m_label_space5->setFixedHeight(10);
    m_label_space5->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space6  = new QLabel(m_widget_main);
    m_label_space6->setFixedHeight(10);
    m_label_space6->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_vboxlayout_network = new QVBoxLayout(m_widget_main);//左 上 右 下
    m_vboxlayout_network->setContentsMargins(m_define_screen->get_change_factor_x(290 - 250),m_define_screen->get_change_factor_y(158 - MAIN_HEAD_HEIGHT - 50),0,0);
    m_vboxlayout_network->setSpacing(0);
    m_vboxlayout_network->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_vboxlayout_network->addWidget(m_button_1);
    m_vboxlayout_network->addWidget(m_NetworkInfo1);
    m_vboxlayout_network->addWidget(m_label_space1);

    m_vboxlayout_network->addWidget(m_button_2);
    m_vboxlayout_network->addWidget(m_NetworkInfo2);
    m_vboxlayout_network->addWidget(m_label_space2);

    m_vboxlayout_network->addWidget(m_button_3);
    m_vboxlayout_network->addWidget(m_NetworkInfo3);
    m_vboxlayout_network->addWidget(m_label_space3);

    m_vboxlayout_network->addWidget(m_button_4);
    m_vboxlayout_network->addWidget(m_NetworkInfo4);
    m_vboxlayout_network->addWidget(m_label_space4);


    m_vboxlayout_network->addWidget(m_button_5);
    m_vboxlayout_network->addWidget(m_NetworkInfo5);
    m_vboxlayout_network->addWidget(m_label_space5);

    m_vboxlayout_network->addWidget(m_button_6);
    m_vboxlayout_network->addWidget(m_NetworkInfo6);
    m_vboxlayout_network->addWidget(m_label_space6);

    m_vboxlayout_network->addWidget(m_button_7);
    m_vboxlayout_network->addWidget(m_NetworkInfo7);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_button_6->hide();
    m_NetworkInfo6->hide();
    m_label_space6->hide();
#endif

    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));
    connect(m_button_3,SIGNAL(clicked()),this,SLOT(slot_button_3()));
    connect(m_button_4,SIGNAL(clicked()),this,SLOT(slot_button_4()));
    connect(m_button_5,SIGNAL(clicked()),this,SLOT(slot_button_5()));
    connect(m_button_6,SIGNAL(clicked()),this,SLOT(slot_button_6()));
    connect(m_button_7,SIGNAL(clicked()),this,SLOT(slot_button_7()));

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_button_press_6 = false;
    m_button_press_7 = false;

    m_NetworkInfo1->hide();
    m_NetworkInfo2->hide();
    m_NetworkInfo3->hide();
    m_NetworkInfo4->hide();
    m_NetworkInfo5->hide();
    m_NetworkInfo6->hide();
    m_NetworkInfo7->hide();


}
void CNetworkWidget::slot_button_1()
{
     if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_NetworkInfo1->hide();
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#else
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
#endif

    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_NetworkInfo1->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 9 + 50));
    }
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_5->setSelected(false);
    m_button_6->setSelected(false);
    m_button_7->setSelected(false);

    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_button_press_6 = false;
    m_button_press_7 = false;

    m_NetworkInfo2->hide();
    m_NetworkInfo3->hide();
    m_NetworkInfo4->hide();
    m_NetworkInfo5->hide();
    m_NetworkInfo6->hide();
    m_NetworkInfo7->hide();

}
void CNetworkWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_NetworkInfo2->hide();

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#else
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
#endif

    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_NetworkInfo2->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 6 + 50));
    }
    m_button_1->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_5->setSelected(false);
    m_button_6->setSelected(false);
    m_button_7->setSelected(false);

    m_button_press_1 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_button_press_6 = false;
    m_button_press_7 = false;

    m_NetworkInfo1->hide();
    m_NetworkInfo3->hide();
    m_NetworkInfo4->hide();
    m_NetworkInfo5->hide();
    m_NetworkInfo6->hide();
    m_NetworkInfo7->hide();
}
void CNetworkWidget::slot_button_3()
{
    if(m_button_press_3)
    {
        m_button_press_3 = false;
        m_button_3->setSelected(false);
        m_NetworkInfo3->hide();

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#else
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
#endif

    }
    else
    {
        m_button_press_3 = true;
        m_button_3->setSelected(true);
        m_NetworkInfo3->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 6 + 50));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_4->setSelected(false);
    m_button_5->setSelected(false);
    m_button_6->setSelected(false);
    m_button_7->setSelected(false);

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_button_press_6 = false;
    m_button_press_7 = false;

    m_NetworkInfo1->hide();
    m_NetworkInfo2->hide();
    m_NetworkInfo4->hide();
    m_NetworkInfo5->hide();
    m_NetworkInfo6->hide();
    m_NetworkInfo7->hide();
}
void CNetworkWidget::slot_button_4()
{
    if(m_button_press_4)
    {
        m_button_press_4 = false;
        m_button_4->setSelected(false);
        m_NetworkInfo4->hide();

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#else
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
#endif


    }
    else
    {
        m_button_press_4 = true;
        m_button_4->setSelected(true);
        m_NetworkInfo4->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 5 + 50));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_5->setSelected(false);
    m_button_6->setSelected(false);
    m_button_7->setSelected(false);

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_5 = false;
    m_button_press_6 = false;
    m_button_press_7 = false;

    m_NetworkInfo1->hide();
    m_NetworkInfo2->hide();
    m_NetworkInfo3->hide();
    m_NetworkInfo5->hide();
    m_NetworkInfo6->hide();
    m_NetworkInfo7->hide();
}
void CNetworkWidget::slot_button_5()
{
    if(m_button_press_5)
    {
        m_button_press_5 = false;
        m_button_5->setSelected(false);
        m_NetworkInfo5->hide();
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#else
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
#endif


    }
    else
    {
        m_button_press_5 = true;
        m_button_5->setSelected(true);
        m_NetworkInfo5->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 8 + 50));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_6->setSelected(false);
    m_button_7->setSelected(false);

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_6 = false;
    m_button_press_7 = false;

    m_NetworkInfo1->hide();
    m_NetworkInfo2->hide();
    m_NetworkInfo3->hide();
    m_NetworkInfo4->hide();
    m_NetworkInfo6->hide();
    m_NetworkInfo7->hide();
}
void CNetworkWidget::slot_button_6()
{
    if(m_button_press_6)
    {
        m_button_press_6 = false;
        m_button_6->setSelected(false);
        m_NetworkInfo6->hide();
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#else
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 100);
#endif

    }
    else
    {
        m_button_press_6 = true;
        m_button_6->setSelected(true);
        m_NetworkInfo6->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 9 + 50));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_5->setSelected(false);
    m_button_7->setSelected(false);

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_button_press_7 = false;

    m_NetworkInfo1->hide();
    m_NetworkInfo2->hide();
    m_NetworkInfo3->hide();
    m_NetworkInfo4->hide();
    m_NetworkInfo5->hide();
    m_NetworkInfo7->hide();
}
void CNetworkWidget::slot_button_7()
{
    if(m_button_press_7)
    {
        m_button_press_7 = false;
        m_button_7->setSelected(false);
        m_NetworkInfo7->hide();

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#else
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 100);
#endif

    }
    else
    {
        m_button_press_7 = true;
        m_button_7->setSelected(true);
        m_NetworkInfo7->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 5));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_5->setSelected(false);
    m_button_6->setSelected(false);

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_button_press_6 = false;

    m_NetworkInfo1->hide();
    m_NetworkInfo2->hide();
    m_NetworkInfo3->hide();
    m_NetworkInfo4->hide();
    m_NetworkInfo5->hide();
    m_NetworkInfo6->hide();
}

void CNetworkWidget::slot_button_model_type()
{
    m_widget_main->hide();
    m_widget_main1->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main1);
}
void CNetworkWidget::slot_button_wifi_search()
{
    //查找网络
    CProtocol::getInstance()->call_json_system_wifi_set(CProtocol::kEnumWifiInfo,NULL);

    if(NULL == m_CNetworkSelect)
    {
        m_CNetworkSelect = new CNetworkSelect(MainWindow::getInstance());

        qRegisterMetaType<CWifiSearch>("CWifiSearch");
        connect(CMainPage::getInstance(),SIGNAL(signal_network_select(char,CWifiSearch)),m_CNetworkSelect,SLOT(slot_network_select(char,CWifiSearch)));
    }
    m_CNetworkSelect->set_interface_type(CNetworkSelect::kEnumSelectNetwork);
    m_CNetworkSelect->move(242,166);
    m_CNetworkSelect->exec();

    //if(CNetworkSelect::KEnumButtonOk == select_dialog->get_result())
    {
    }
}

void CNetworkWidget::slot_pushbutton_return()
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

void CNetworkWidget::slot_pushbutton_save()
{

}
void CNetworkWidget::slot_pushbutton_reset()
{

}
//1:移动网络
bool CNetworkWidget::networkinfo1_change()
{
    bool ret_value = false;
    CMobileNetworkInfo MobileNetworkInfo;

    if(m_NetworkInfo1->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo1->m_checkbox_line1->isChecked())
            MobileNetworkInfo.m_enable = 1;
        else
            MobileNetworkInfo.m_enable = 0;
    }
    else
        MobileNetworkInfo.m_enable = 2;

    MobileNetworkInfo.operator1 = m_NetworkInfo1->m_combobox_line3->currentIndex();
    MobileNetworkInfo.network_type = m_NetworkInfo1->m_combobox_line4->currentIndex();
    MobileNetworkInfo.month_end_date = m_NetworkInfo1->m_combobox_line5->currentIndex();
    MobileNetworkInfo.use_vpn = m_NetworkInfo1->m_checkbox_line6->isChecked();

    MobileNetworkInfo.vpn_name = m_NetworkInfo1->m_lineedit7->text();
    MobileNetworkInfo.vpn_account = m_NetworkInfo1->m_lineedit8->text();
    MobileNetworkInfo.vpn_password = m_NetworkInfo1->m_lineedit9->text();

    if(MobileNetworkInfo.m_enable != CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->m_enable ||
        MobileNetworkInfo.operator1 != CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->operator1 ||
        MobileNetworkInfo.network_type != CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->network_type ||
        MobileNetworkInfo.month_end_date != CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->month_end_date ||
        MobileNetworkInfo.use_vpn != CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->use_vpn ||
        MobileNetworkInfo.vpn_name != CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_name ||
        MobileNetworkInfo.vpn_account != CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_account ||
        MobileNetworkInfo.vpn_password != CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_password
    )
    {
        ret_value = true;
    }
    return ret_value;
}
//2:中心服务器1
bool CNetworkWidget::networkinfo2_change()
{
   bool ret_value = false;
    CCenterServerInfo CenterServerInfo1;
    if(m_NetworkInfo2->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo2->m_checkbox_line1->isChecked())
            CenterServerInfo1.m_enable = 1;
        else
            CenterServerInfo1.m_enable = 0;
    }
    else
        CenterServerInfo1.m_enable = 2;

    CenterServerInfo1.protocol_type = m_NetworkInfo2->m_combobox_line2->currentIndex();
    CenterServerInfo1.access_mode = m_NetworkInfo2->m_combobox_line3->currentIndex();//接入模式 0:固件IP地址 1:域名
    CenterServerInfo1.m_ip = m_NetworkInfo2->m_lineedit4->text();
    CenterServerInfo1.m_port = m_NetworkInfo2->m_lineedit5->text().toInt();
    CenterServerInfo1.m_data_send_interval = m_NetworkInfo2->m_lineedit6->text().toInt();

    if(CenterServerInfo1.m_enable != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_enable ||
        CenterServerInfo1.protocol_type != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->protocol_type ||
        CenterServerInfo1.access_mode != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->access_mode ||
        CenterServerInfo1.m_ip != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_ip ||
        CenterServerInfo1.m_port != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_port ||
        CenterServerInfo1.m_data_send_interval != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_data_send_interval
    )
    {
        ret_value = true;
    }
    return ret_value;
}
//3:中心服务器2
bool CNetworkWidget::networkinfo3_change()
{
    bool ret_value = false;
    CCenterServerInfo CenterServerInfo2;

    if(m_NetworkInfo3->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo3->m_checkbox_line1->isChecked())
            CenterServerInfo2.m_enable = 1;
        else
            CenterServerInfo2.m_enable = 0;
    }
    else
        CenterServerInfo2.m_enable = 2;

    CenterServerInfo2.protocol_type = m_NetworkInfo3->m_combobox_line2->currentIndex();
    CenterServerInfo2.access_mode = m_NetworkInfo3->m_combobox_line3->currentIndex();//接入模式 0:固件IP地址 1:域名
    CenterServerInfo2.m_ip = m_NetworkInfo3->m_lineedit4->text();
    CenterServerInfo2.m_port = m_NetworkInfo3->m_lineedit5->text().toInt();
    CenterServerInfo2.m_data_send_interval = m_NetworkInfo3->m_lineedit6->text().toInt();

    if(CenterServerInfo2.m_enable != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_enable ||
        CenterServerInfo2.protocol_type != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->protocol_type ||
        CenterServerInfo2.access_mode != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->access_mode ||
        CenterServerInfo2.m_ip != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_ip ||
        CenterServerInfo2.m_port != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_port ||
        CenterServerInfo2.m_data_send_interval != CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_data_send_interval
    )
    {
        ret_value = true;
    }
    return ret_value;
}
//4:视频服务器
bool CNetworkWidget::networkinfo4_change()
{
    bool ret_value = false;
    CVideoServerInfo VideoServerInfo;
    if(m_NetworkInfo4->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo4->m_checkbox_line1->isChecked())
            VideoServerInfo.m_enable = 1;
        else
            VideoServerInfo.m_enable = 0;
    }
    else
        VideoServerInfo.m_enable = 2;

    VideoServerInfo.protocol_type = m_NetworkInfo4->m_combobox_line2->currentIndex();
    VideoServerInfo.access_mode = m_NetworkInfo4->m_combobox_line3->currentIndex();//接入模式 0:静态IP 1:自动获取
    VideoServerInfo.m_ip = m_NetworkInfo4->m_lineedit4->text();
    VideoServerInfo.m_port = m_NetworkInfo4->m_lineedit5->text().toInt();

    if(VideoServerInfo.m_enable != CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_enable ||
        VideoServerInfo.protocol_type != CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->protocol_type ||
        VideoServerInfo.access_mode != CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->access_mode ||
        VideoServerInfo.m_ip != CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_ip ||
        VideoServerInfo.m_port != CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_port
    )
    {
        ret_value = true;
    }
    return ret_value;
}
//5:本地网络
bool CNetworkWidget::networkinfo5_change()
{
    bool ret_value = false;
    CLocalNetworkInfo LocalNetworkInfo;

    if(m_NetworkInfo5->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo5->m_checkbox_line1->isChecked())
            LocalNetworkInfo.m_enable = 1;
        else
            LocalNetworkInfo.m_enable = 0;
    }
    else
        LocalNetworkInfo.m_enable = 2;

    if(m_NetworkInfo5->m_checkbox_line2->isEnabled())
    {
        if(m_NetworkInfo5->m_checkbox_line2->isChecked())
            LocalNetworkInfo.m_dhcp_server = 1;
        else
            LocalNetworkInfo.m_dhcp_server = 0;
    }
    else
        LocalNetworkInfo.m_dhcp_server = 2;

    LocalNetworkInfo.access_mode = m_NetworkInfo5->m_combobox_line3->currentIndex();
    LocalNetworkInfo.m_ip = m_NetworkInfo5->m_lineedit4->text();
    LocalNetworkInfo.subnet_mask = m_NetworkInfo5->m_lineedit5->text();
    LocalNetworkInfo.gateway = m_NetworkInfo5->m_lineedit6->text();
    LocalNetworkInfo.nds1 = m_NetworkInfo5->m_lineedit7->text();
    LocalNetworkInfo.nds2 = m_NetworkInfo5->m_lineedit8->text();

    if(LocalNetworkInfo.access_mode != CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->access_mode ||
        LocalNetworkInfo.m_ip != CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_ip ||
        LocalNetworkInfo.subnet_mask != CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->subnet_mask ||
        LocalNetworkInfo.gateway != CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->gateway ||
        LocalNetworkInfo.nds1 != CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->nds1 ||
        LocalNetworkInfo.nds2 != CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->nds2
    )
    {
        ret_value = true;
    }
    return ret_value;
}
//6:WIFI
bool CNetworkWidget::networkinfo6_change()
{
    CWifiInfo WifiInfo;
    bool ret_value = false;

    if(m_NetworkInfo6->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo6->m_checkbox_line1->isChecked())
            WifiInfo.m_enable = 1;
        else
            WifiInfo.m_enable = 0;
    }
    else
        WifiInfo.m_enable = 2;

    WifiInfo.access_mode = m_NetworkInfo6->m_combobox_line3->currentIndex();
    WifiInfo.m_ip = m_NetworkInfo6->m_lineedit4->text();
    WifiInfo.subnet_mask = m_NetworkInfo6->m_lineedit5->text();
    WifiInfo.gateway = m_NetworkInfo6->m_lineedit6->text();
    WifiInfo.nds1 = m_NetworkInfo6->m_lineedit7->text();
    WifiInfo.nds2 = m_NetworkInfo6->m_lineedit8->text();

    if(WifiInfo.access_mode != CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->access_mode ||
        WifiInfo.m_ip != CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_ip ||
        WifiInfo.subnet_mask != CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->subnet_mask ||
        WifiInfo.gateway != CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->gateway ||
        WifiInfo.nds1 != CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->nds1 ||
        WifiInfo.nds2 != CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->nds2
    )
    {
        ret_value = true;
    }
    return ret_value;
}
//7.FTP
bool CNetworkWidget::networkinfo7_change()
{
    bool ret_value = false;
    CFtpInfo FtpInfo;
    if(m_NetworkInfo7->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo7->m_checkbox_line1->isChecked())
            FtpInfo.m_enable = 1;
        else
            FtpInfo.m_enable = 0;
    }
    else
        FtpInfo.m_enable = 2;

    FtpInfo.access_mode = m_NetworkInfo7->m_combobox_line2->currentIndex();
    FtpInfo.m_ip = m_NetworkInfo7->m_lineedit3->text();
    FtpInfo.gateway = m_NetworkInfo7->m_lineedit4->text();

    if(FtpInfo.m_enable != CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_enable ||
        FtpInfo.access_mode != CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->access_mode ||
        FtpInfo.m_ip != CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_ip ||
        FtpInfo.gateway != CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->gateway
    )
    {
        ret_value = true;
    }
    return ret_value;
}

void CNetworkWidget::save_data()
{
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        if(networkinfo1_change() ||
        networkinfo2_change() ||
        networkinfo3_change() ||
        networkinfo4_change() ||
        networkinfo5_change() ||
        networkinfo7_change())
#else
        if(networkinfo1_change() ||
        networkinfo2_change() ||
        networkinfo3_change() ||
        networkinfo4_change() ||
        networkinfo5_change() ||
        networkinfo6_change() ||
        networkinfo7_change())
#endif
     {
        if(CCommonData::getInstance()->messagebox_data_change())
        {
            read_data();
            CProtocol::getInstance()->call_json_system_network_set(CProtocol::kEnumSave);
        }
        else
        {
            //1:移动网络 2:中心服务器1 3:中心服务器2 4:视频服务器 5:本地网络 6:WIFI 7:FTP
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 1;
            slot_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 2;
            slot_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 3;
            slot_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 4;
            slot_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 5;
            slot_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 6;
            slot_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);
            CCommonData::getInstance()->m_CSystemNetwork.m_type = 7;
            slot_network_widget_event(CCommonData::getInstance()->m_CSystemNetwork);


        }
     }
}
void CNetworkWidget::read_data()
{
    //1:移动网络
    if(m_NetworkInfo1->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo1->m_checkbox_line1->isChecked())
            CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->m_enable = 1;
        else
            CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->m_enable = 0;
    }
    else
        CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->m_enable = 2;

    CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->operator1 = m_NetworkInfo1->m_combobox_line3->currentIndex();
    CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->network_type = m_NetworkInfo1->m_combobox_line4->currentIndex();
    CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->month_end_date = m_NetworkInfo1->m_combobox_line5->currentIndex();
    CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->use_vpn = m_NetworkInfo1->m_checkbox_line6->isChecked();

    CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_name = m_NetworkInfo1->m_lineedit7->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_account = m_NetworkInfo1->m_lineedit8->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CMobileNetworkInfo->vpn_password = m_NetworkInfo1->m_lineedit9->text();

    //2:中心服务器1
    if(m_NetworkInfo2->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo2->m_checkbox_line1->isChecked())
            CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_enable = 1;
        else
            CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_enable = 0;
    }
    else
        CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_enable = 2;

    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->protocol_type = m_NetworkInfo2->m_combobox_line2->currentIndex();

    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->access_mode = m_NetworkInfo2->m_combobox_line3->currentIndex();//接入模式 0:固件IP地址 1:域名
    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_ip = m_NetworkInfo2->m_lineedit4->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_port = m_NetworkInfo2->m_lineedit5->text().toInt();
    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo1->m_data_send_interval = m_NetworkInfo2->m_lineedit6->text().toInt();

    //3:中心服务器2
    if(m_NetworkInfo3->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo3->m_checkbox_line1->isChecked())
            CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_enable = 1;
        else
            CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_enable = 0;
    }
    else
        CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_enable = 2;

    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->protocol_type = m_NetworkInfo3->m_combobox_line2->currentIndex();
    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->access_mode = m_NetworkInfo3->m_combobox_line3->currentIndex();//接入模式 0:固件IP地址 1:域名
    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_ip = m_NetworkInfo3->m_lineedit4->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_port = m_NetworkInfo3->m_lineedit5->text().toInt();
    CCommonData::getInstance()->m_CSystemNetwork.m_CCenterServerInfo2->m_data_send_interval = m_NetworkInfo3->m_lineedit6->text().toInt();

    //4:视频服务器
    if(m_NetworkInfo4->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo4->m_checkbox_line1->isChecked())
            CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_enable = 1;
        else
            CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_enable = 0;
    }
    else
        CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_enable = 2;

    CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->protocol_type = m_NetworkInfo4->m_combobox_line2->currentIndex();
    CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->access_mode = m_NetworkInfo4->m_combobox_line3->currentIndex();//接入模式 0:静态IP 1:自动获取
    CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_ip = m_NetworkInfo4->m_lineedit4->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CVideoServerInfo->m_port = m_NetworkInfo4->m_lineedit5->text().toInt();

    //5:本地网络
    if(m_NetworkInfo5->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo5->m_checkbox_line1->isChecked())
            CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_enable = 1;
        else
            CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_enable = 0;
    }
    else
        CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_enable = 2;

    if(m_NetworkInfo5->m_checkbox_line2->isEnabled())
    {
        if(m_NetworkInfo5->m_checkbox_line2->isChecked())
            CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_dhcp_server = 1;
        else
            CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_dhcp_server = 0;
    }
    else
        CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_dhcp_server = 2;

    CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->access_mode = m_NetworkInfo5->m_combobox_line3->currentIndex();
    CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->m_ip = m_NetworkInfo5->m_lineedit4->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->subnet_mask = m_NetworkInfo5->m_lineedit5->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->gateway = m_NetworkInfo5->m_lineedit6->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->nds1 = m_NetworkInfo5->m_lineedit7->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CLocalNetworkInfo->nds2 = m_NetworkInfo5->m_lineedit8->text();

    //6:WIFI
    if(m_NetworkInfo6->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo6->m_checkbox_line1->isChecked())
            CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_enable = 1;
        else
            CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_enable = 0;
    }
    else
        CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_enable = 2;

    CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->access_mode = m_NetworkInfo6->m_combobox_line3->currentIndex();
    CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->m_ip = m_NetworkInfo6->m_lineedit4->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->subnet_mask = m_NetworkInfo6->m_lineedit5->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->gateway = m_NetworkInfo6->m_lineedit6->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->nds1 = m_NetworkInfo6->m_lineedit7->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CWifiInfo->nds2 = m_NetworkInfo6->m_lineedit8->text();

    //7:FTP
    if(m_NetworkInfo7->m_checkbox_line1->isEnabled())
    {
        if(m_NetworkInfo7->m_checkbox_line1->isChecked())
            CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_enable = 1;
        else
            CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_enable = 0;
    }
    else
        CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_enable = 2;

    CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->access_mode = m_NetworkInfo7->m_combobox_line2->currentIndex();
    CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->m_ip = m_NetworkInfo7->m_lineedit3->text();
    CCommonData::getInstance()->m_CSystemNetwork.m_CFtpInfo->gateway = m_NetworkInfo7->m_lineedit4->text();

}
void CNetworkWidget::slot_network_widget_event(const CSystemNetwork & _info)
{
    //1:移动网络 2:中心服务器1 3:中心服务器2 4:视频服务器 5:本地网络 6:WIFI 7:FTP
    switch(_info.m_type)
    {
        case 1://移动网络
        {
            if(NULL == _info.m_CMobileNetworkInfo)
            {
                break;
            }
            // 0:未选择状态 1:选中状态 2:禁用状态
            if(0 == _info.m_CMobileNetworkInfo->m_enable)
            {
                m_NetworkInfo1->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo1->m_checkbox_line1->setChecked(false);
            }
            else if(1 == _info.m_CMobileNetworkInfo->m_enable)
            {
                m_NetworkInfo1->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo1->m_checkbox_line1->setChecked(true);
            }
            else if(2 == _info.m_CMobileNetworkInfo->m_enable)
            {
                m_NetworkInfo1->m_checkbox_line1->setEnabled(false);
            }
            m_NetworkInfo1->m_button_line2->setText1Text2(_info.m_CMobileNetworkInfo->modular_name1,_info.m_CMobileNetworkInfo->modular_name2);

            m_label_main1_line2_1->setText(_info.m_CMobileNetworkInfo->modular_version);
            m_label_main1_line3_1->setText(_info.m_CMobileNetworkInfo->modular_imei);
            m_label_main1_line4_1->setText(_info.m_CMobileNetworkInfo->modular_sn);
            m_label_main1_line5_1->setText(_info.m_CMobileNetworkInfo->modular_meid);

            m_NetworkInfo1->m_combobox_line3->setCurrentIndex(_info.m_CMobileNetworkInfo->operator1);
            m_NetworkInfo1->m_combobox_line4->setCurrentIndex(_info.m_CMobileNetworkInfo->network_type);
            m_NetworkInfo1->m_combobox_line5->setCurrentIndex(_info.m_CMobileNetworkInfo->month_end_date);

            if(0 == _info.m_CMobileNetworkInfo->use_vpn)
                m_NetworkInfo1->m_checkbox_line6->setChecked(false);
            else if(1 == _info.m_CMobileNetworkInfo->use_vpn)
                m_NetworkInfo1->m_checkbox_line6->setChecked(true);

            m_NetworkInfo1->m_lineedit7->setText(_info.m_CMobileNetworkInfo->vpn_name);
            m_NetworkInfo1->m_lineedit8->setText(_info.m_CMobileNetworkInfo->vpn_account);
            m_NetworkInfo1->m_lineedit9->setText(_info.m_CMobileNetworkInfo->vpn_password);

        }break;
        case 2://中心服务器1
        {
            if(NULL == _info.m_CCenterServerInfo1)
            {
                break;
            }
            // 0:未选择状态 1:选中状态 2:禁用状态
            if(0 == _info.m_CCenterServerInfo1->m_enable)
            {
                m_NetworkInfo2->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo2->m_checkbox_line1->setChecked(false);
            }
            else if(1 == _info.m_CCenterServerInfo1->m_enable)
            {
                m_NetworkInfo2->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo2->m_checkbox_line1->setChecked(true);
            }
            else if(2 == _info.m_CCenterServerInfo1->m_enable)
            {
                m_NetworkInfo2->m_checkbox_line1->setEnabled(false);
            }
            #if 0
            if(_info.m_CCenterServerInfo1->protocol_type_list.size() > 0)
            {
                m_NetworkInfo2->m_combobox_line2->clear();
                char current_index = 0;
                for(int index = 0;index < _info.m_CCenterServerInfo1->protocol_type_list.size();index++)
                {
                    m_NetworkInfo2->m_combobox_line2->addItem(_info.m_CCenterServerInfo1->protocol_type_list.at(index));

                    if(_info.m_CCenterServerInfo1->protocol_type == _info.m_CCenterServerInfo1->protocol_type_list.at(index))
                    {
                        current_index = index;
                    }

                }
                m_NetworkInfo2->m_combobox_line2->setCurrentIndex(current_index);
            }
            #endif
            m_NetworkInfo2->m_combobox_line2->setCurrentIndex(_info.m_CCenterServerInfo1->protocol_type);

            m_NetworkInfo2->m_combobox_line3->setCurrentIndex(_info.m_CCenterServerInfo1->access_mode);//接入模式 0:固件IP地址 1:域名
            m_NetworkInfo2->m_lineedit4->setText(_info.m_CCenterServerInfo1->m_ip);
            m_NetworkInfo2->m_lineedit5->setText(QString::number(_info.m_CCenterServerInfo1->m_port));
            m_NetworkInfo2->m_lineedit6->setText(QString::number(_info.m_CCenterServerInfo1->m_data_send_interval));



        }break;
        case 3://中心服务器2
        {
            if(NULL == _info.m_CCenterServerInfo2)
            {
                break;
            }
                        // 0:未选择状态 1:选中状态 2:禁用状态
            if(0 == _info.m_CCenterServerInfo2->m_enable)
            {
                m_NetworkInfo3->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo3->m_checkbox_line1->setChecked(false);
            }
            else if(1 == _info.m_CCenterServerInfo2->m_enable)
            {
                m_NetworkInfo3->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo3->m_checkbox_line1->setChecked(true);
            }
            else if(2 == _info.m_CCenterServerInfo2->m_enable)
            {
                m_NetworkInfo3->m_checkbox_line1->setEnabled(false);
            }
#if 0
            if(_info.m_CCenterServerInfo2->protocol_type_list.size() > 0)
            {
                m_NetworkInfo3->m_combobox_line2->clear();
                int current_index = 0;
                for(int index = 0;index < _info.m_CCenterServerInfo2->protocol_type_list.size();index++)
                {
                    m_NetworkInfo3->m_combobox_line2->addItem(_info.m_CCenterServerInfo2->protocol_type_list.at(index));

                    if(_info.m_CCenterServerInfo2->protocol_type == _info.m_CCenterServerInfo2->protocol_type_list.at(index))
                    {
                        current_index = index;
                    }
                }
                m_NetworkInfo3->m_combobox_line2->setCurrentIndex(current_index);
            }
#endif
            m_NetworkInfo3->m_combobox_line2->setCurrentIndex(_info.m_CCenterServerInfo2->protocol_type);

            m_NetworkInfo3->m_combobox_line3->setCurrentIndex(_info.m_CCenterServerInfo2->access_mode);//接入模式 0:固件IP地址 1:域名
            m_NetworkInfo3->m_lineedit4->setText(_info.m_CCenterServerInfo2->m_ip);
            m_NetworkInfo3->m_lineedit5->setText(QString::number(_info.m_CCenterServerInfo2->m_port));
            m_NetworkInfo3->m_lineedit6->setText(QString::number(_info.m_CCenterServerInfo2->m_data_send_interval));

        }break;
        case 4://视频服务器
        {
            if(NULL == _info.m_CVideoServerInfo)
            {
                break;
            }
            if(0 == _info.m_CVideoServerInfo->m_enable)
            {
                m_NetworkInfo4->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo4->m_checkbox_line1->setChecked(false);
            }
            else if(1 == _info.m_CVideoServerInfo->m_enable)
            {
                m_NetworkInfo4->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo4->m_checkbox_line1->setChecked(true);
            }
            else if(2 == _info.m_CVideoServerInfo->m_enable)
            {
                m_NetworkInfo4->m_checkbox_line1->setEnabled(false);
            }
#if 0
            if(_info.m_CVideoServerInfo->protocol_type_list.size() > 0)
            {
                m_NetworkInfo4->m_combobox_line2->clear();
                int current_index = 0;
                for(int index = 0;index < _info.m_CVideoServerInfo->protocol_type_list.size();index++)
                {
                    m_NetworkInfo4->m_combobox_line2->addItem(_info.m_CVideoServerInfo->protocol_type_list.at(index));

                     if(_info.m_CVideoServerInfo->protocol_type == _info.m_CVideoServerInfo->protocol_type_list.at(index))
                    {
                        current_index = index;
                    }
                }
                m_NetworkInfo4->m_combobox_line2->setCurrentIndex(current_index);
            }
#endif
            m_NetworkInfo4->m_combobox_line2->setCurrentIndex(_info.m_CVideoServerInfo->protocol_type);

            m_NetworkInfo4->m_combobox_line3->setCurrentIndex(_info.m_CVideoServerInfo->access_mode);//接入模式 0:静态IP 1:自动获取
            m_NetworkInfo4->m_lineedit4->setText(_info.m_CVideoServerInfo->m_ip);
            m_NetworkInfo4->m_lineedit5->setText(QString::number(_info.m_CVideoServerInfo->m_port));


        }break;
        case 5://本地网络
        {
            if(NULL == _info.m_CLocalNetworkInfo)
            {
                break;
            }
            if(0 == _info.m_CLocalNetworkInfo->m_enable)
            {
                m_NetworkInfo5->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo5->m_checkbox_line1->setChecked(false);
            }
            else if(1 == _info.m_CLocalNetworkInfo->m_enable)
            {
                m_NetworkInfo5->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo5->m_checkbox_line1->setChecked(true);
            }
            else if(2 == _info.m_CLocalNetworkInfo->m_enable)
            {
                m_NetworkInfo5->m_checkbox_line1->setEnabled(false);
            }
            if(0 == _info.m_CLocalNetworkInfo->m_dhcp_server)
            {
                m_NetworkInfo5->m_checkbox_line2->setEnabled(true);
                m_NetworkInfo5->m_checkbox_line2->setChecked(false);
            }
            else if(1 == _info.m_CLocalNetworkInfo->m_dhcp_server)
            {
                m_NetworkInfo5->m_checkbox_line2->setEnabled(true);
                m_NetworkInfo5->m_checkbox_line2->setChecked(true);
            }
            else if(2 == _info.m_CLocalNetworkInfo->m_dhcp_server)
            {
                m_NetworkInfo5->m_checkbox_line2->setEnabled(false);
            }
            m_NetworkInfo5->m_combobox_line3->setCurrentIndex(_info.m_CLocalNetworkInfo->access_mode);
            m_NetworkInfo5->m_lineedit4->setText(_info.m_CLocalNetworkInfo->m_ip);
            m_NetworkInfo5->m_lineedit5->setText(_info.m_CLocalNetworkInfo->subnet_mask);
            m_NetworkInfo5->m_lineedit6->setText(_info.m_CLocalNetworkInfo->gateway);
            m_NetworkInfo5->m_lineedit7->setText(_info.m_CLocalNetworkInfo->nds1);
            m_NetworkInfo5->m_lineedit8->setText(_info.m_CLocalNetworkInfo->nds2);


        }break;
        case 6://WIFI
        {
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
            break;
#else
#endif

            if(NULL == _info.m_CWifiInfo)
            {
                break;
            }
            if(0 == _info.m_CWifiInfo->m_enable)
            {
                m_NetworkInfo6->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo6->m_checkbox_line1->setChecked(false);
            }
            else if(1 == _info.m_CWifiInfo->m_enable)
            {
                m_NetworkInfo6->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo6->m_checkbox_line1->setChecked(true);
            }
            else if(2 == _info.m_CWifiInfo->m_enable)
            {
                m_NetworkInfo6->m_checkbox_line1->setEnabled(false);
            }
            m_NetworkInfo6->m_combobox_line3->setCurrentIndex(_info.m_CWifiInfo->access_mode);
            m_NetworkInfo6->m_lineedit4->setText(_info.m_CWifiInfo->m_ip);
            m_NetworkInfo6->m_lineedit5->setText(_info.m_CWifiInfo->subnet_mask);
            m_NetworkInfo6->m_lineedit6->setText(_info.m_CWifiInfo->gateway);
            m_NetworkInfo6->m_lineedit7->setText(_info.m_CWifiInfo->nds1);
            m_NetworkInfo6->m_lineedit8->setText(_info.m_CWifiInfo->nds2);

        }break;
        case 7://FTP
        {
            if(NULL == _info.m_CFtpInfo)
            {
                break;
            }
            if(0 == _info.m_CFtpInfo->m_enable)
            {
                m_NetworkInfo7->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo7->m_checkbox_line1->setChecked(false);
            }
            else if(1 == _info.m_CFtpInfo->m_enable)
            {
                m_NetworkInfo7->m_checkbox_line1->setEnabled(true);
                m_NetworkInfo7->m_checkbox_line1->setChecked(true);
            }
            else if(2 == _info.m_CFtpInfo->m_enable)
            {
                m_NetworkInfo7->m_checkbox_line1->setEnabled(false);
            }
            m_NetworkInfo7->m_combobox_line2->setCurrentIndex(_info.m_CFtpInfo->access_mode);
            m_NetworkInfo7->m_lineedit3->setText(_info.m_CFtpInfo->m_ip);
            m_NetworkInfo7->m_lineedit4->setText(_info.m_CFtpInfo->gateway);


        }break;
#if 0
        case 8://选择WIFI列表
        {
            if(NULL == _info.m_select_wifi)
            {
                break;
            }
            emit signal_network_select(_info.m_select_wifi);
            _info.m_select_wifi = NULL;

        }break;
#endif
    }

}

void CNetworkWidget::set_focus_first()
{
    m_button_1->setFocus();
    m_button_1->click();
}
















