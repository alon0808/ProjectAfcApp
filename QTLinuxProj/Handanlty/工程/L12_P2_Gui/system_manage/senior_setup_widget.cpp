#include "senior_setup_widget.h"

#include <QListView>
#include <QDateTime>

#include "LtyCommonDefine.h"

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"

#include "./class/common_interface.h"
#include "common_data.h"



#define BUTTON_STYPE_SHEET1 "QPushButton {\
                                      border:0px solid #24508F;\
                                      text-align:left;\
                                      background:transparent;\
                                      font:26px;\
                                      border-radius: 0px;\
                                      color:#8ea2c0;\
                                      padding: 0px 0px 0px 30px;\
                                      } \
                                      QPushButton:pressed {background:#0E2748;color:#6F8DB8;} \
                                      QPushButton:hover:!pressed {background:#0E2748;color:#6F8DB8;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"

CSeniorSetupInfo::CSeniorSetupInfo(int _type,QWidget *parent) :
    QWidget(parent)
{
    m_type = _type;
    this->setObjectName("CSeniorSetupInfo");
    this->setStyleSheet("QWidget#CSeniorSetupInfo{border:1px solid #24508F;background-color:#ffffff;}");//#03111E

    set_type(_type);
}

void CSeniorSetupInfo::common_config_init()      //通用配置
{
    char row_number = 10;
    this->setFixedSize(620,ROW_HEIGHT * row_number);
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * row_number);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < (row_number + 1);index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("定位方式");
    //m_label_line1 = new QLabel(this);
   // CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    //m_pushbutton_line1
    QString str_sheet = BUTTON_STYPE_SHEET1;
    m_pushbutton_line1_click = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line1_click,290 - 290,0,620,ROW_HEIGHT,str_sheet);
    m_pushbutton_line1_click->setText(str_show);

    connect(m_pushbutton_line1_click,SIGNAL(clicked()),this,SIGNAL(signal_location_mode())); //定位方式

    m_combobox_line1 = new QComboBox(this);
    m_combobox_line1->move(649 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line1->addItem(tr("混合定位"));
    m_combobox_line1->addItem(tr("北斗定位"));
    m_combobox_line1->addItem(tr("GPS定位"));
    m_combobox_line1->setView(new QListView());


    str_show = tr("惯性导航");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_checkbox_line2 = new QCheckBox(this);
    m_checkbox_line2->setFixedSize(70,70);
    m_checkbox_line2->move(758 - 290 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    str_sheet = BUTTON_CONFIG_SHEET;

    m_pushbutton_line2_1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line2_1,799 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line2_1->setText(tr("配置"));

    connect(m_pushbutton_line2_1,SIGNAL(clicked()),this,SIGNAL(signal_navigation())); //惯性导航

    str_show = tr("报站方式");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(649 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("自动"));
    m_combobox_line3->addItem(tr("手动"));
    m_combobox_line3->setView(new QListView());


    str_show = tr("进站半径");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_lineedit4 = new QLineEdit(this);
    m_lineedit4->move(649 - 290,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit4->setFixedSize(200,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit4,true);

    str_show = tr("M");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,862 - 290, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = tr("出站半径");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_lineedit5 = new QLineEdit(this);
    m_lineedit5->move(649 - 290,ROW_HEIGHT * 4 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit5->setFixedSize(200,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit5,true);

    str_show = tr("M");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,862 - 290, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);


    str_show = tr("拐点半径");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_lineedit6 = new QLineEdit(this);
    m_lineedit6->move(649 - 290,ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit6->setFixedSize(200,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit6,true);

    str_show = tr("M");
    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,862 - 290, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    str_show = tr("超速保持提醒");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    m_lineedit7 = new QLineEdit(this);
    m_lineedit7->move(649 - 290,ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit7->setFixedSize(200,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit7,true);

    str_show = tr("S");
    m_label_line7_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7_1,862 - 290, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);


    str_show = tr("超速提醒次数");
    m_label_line8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8,30, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);

    m_lineedit8 = new QLineEdit(this);
    m_lineedit8->move(649 - 290,ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit8->setFixedSize(200,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit8,true);

    str_show = tr("T");
    m_label_line8_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8_1,862 - 290, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);

    str_show = tr("非运营限速");
    m_label_line9 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line9,30, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    m_lineedit9 = new QLineEdit(this);
    m_lineedit9->move(649 - 290,ROW_HEIGHT * 8 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit9->setFixedSize(158,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit9,true);

    str_show = tr("km/h");
    m_label_line9_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line9_1,826 - 290, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    str_show = tr("营运模式");
    m_label_line10 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line10,30, ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show);

    m_combobox_line10 = new QComboBox(this);
    m_combobox_line10->move(649 - 290,ROW_HEIGHT * 9 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line10->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line10->addItem(tr("营运"));
    m_combobox_line10->addItem(tr("非营运"));
    m_combobox_line10->setView(new QListView());

}

void CSeniorSetupInfo::warning_enable_init()     //告警使能
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
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("紧急加速");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(649 - 290 - 20,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    str_show = tr("≥");
    m_label_line1_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1_1,713 - 20 - 290, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_lineedit1 = new QLineEdit(this);
    m_lineedit1->move(713 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit1->setFixedSize(110,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit1,true);

    str_show = tr("km/s");
    m_label_line1_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1_2,832 - 290, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);


    str_show = tr("紧急减速");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_checkbox_line2 = new QCheckBox(this);
    m_checkbox_line2->setFixedSize(70,70);
    m_checkbox_line2->move(649 - 290 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    str_show = tr("≥");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,713 - 20 - 290, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_lineedit2 = new QLineEdit(this);
    m_lineedit2->move(713 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit2->setFixedSize(110,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit2,true);

    str_show = tr("km/s");
    m_label_line2_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_2,832 - 290, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);



    str_show = tr("滞站报警");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_checkbox_line3 = new QCheckBox(this);
    m_checkbox_line3->setFixedSize(70,70);
    m_checkbox_line3->move(649 - 290 - 20,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3->setFocusPolicy(Qt::NoFocus);

    str_show = tr("≥");
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,713 - 20 - 290, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_lineedit3 = new QLineEdit(this);
    m_lineedit3->move(713 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit3->setFixedSize(110,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit3,true);

    str_show = tr("min");
    m_label_line3_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_2,832 - 290, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);


    str_show = tr("越站报警");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_checkbox_line4 = new QCheckBox(this);
    m_checkbox_line4->setFixedSize(70,70);
    m_checkbox_line4->move(649 - 290 - 20,ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4->setFocusPolicy(Qt::NoFocus);

    str_show = tr("≥");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,713 - 20 - 290, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_lineedit4 = new QLineEdit(this);
    m_lineedit4->move(713 - 290,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit4->setFixedSize(110,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit4,true);

    str_show = tr("min");
    m_label_line4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_2,832 - 290, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);


    str_show = tr("疲劳驾驶检测");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_checkbox_line5 = new QCheckBox(this);
    m_checkbox_line5->setFixedSize(70,70);
    m_checkbox_line5->move(622 - 290 - 20 - 10,ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5->setFocusPolicy(Qt::NoFocus);


    str_show = tr("时长");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,664 - 290 - 10, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_lineedit5 = new QLineEdit(this);
    m_lineedit5->move(712 - 290,ROW_HEIGHT * 4 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit5->setFixedSize(110,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit5,true);

    str_show = tr("h");
    m_label_line5_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_2,833 - 290, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    str_show = tr("怠速使能");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_checkbox_line6 = new QCheckBox(this);
    m_checkbox_line6->setFixedSize(70,70);
    m_checkbox_line6->move(622 - 290 - 20 - 10,ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6->setFocusPolicy(Qt::NoFocus);


    str_show = tr("时长");
    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,664 - 290 - 10, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_lineedit6 = new QLineEdit(this);
    m_lineedit6->move(712 - 290,ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit6->setFixedSize(110,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit6,true);

    str_show = tr("h");
    m_label_line6_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_2,833 - 290, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

}

void CSeniorSetupInfo::can_setting_init()        //CAN设置
{
    this->setFixedSize(620,ROW_HEIGHT * 2);
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 2);
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

    QString str_show = tr("CAN1");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);
#if 0
    m_pushbutton_line1 = new CMyPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line1,758 - 290 - 20,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2,70,70);
    m_pushbutton_line1->set_pushbutton_enable(true);
    m_pushbutton_line1->set_pushbutton_sheet("QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}",
                                            "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}");
#endif
    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(758 - 290 - 20,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    QString str_sheet = BUTTON_CONFIG_SHEET;

    m_pushbutton_line1_1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line1_1,799 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line1_1->setText(tr("调试"));

    connect(m_pushbutton_line1_1,SIGNAL(clicked()),this,SIGNAL(signal_can1())); //CAN1

    str_show = tr("CAN2");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
#if 0
    m_pushbutton_line2 = new CMyPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line2,758 - 290 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2,70,70);
    m_pushbutton_line2->set_pushbutton_enable(true);
    m_pushbutton_line2->set_pushbutton_sheet("QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}",
                                            "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}");
#endif
    m_checkbox_line2 = new QCheckBox(this);
    m_checkbox_line2->setFixedSize(70,70);
    m_checkbox_line2->move(758 - 290 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    str_sheet = BUTTON_CONFIG_SHEET;

    m_pushbutton_line2_1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line2_1,799 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line2_1->setText(tr("调试"));

    connect(m_pushbutton_line2_1,SIGNAL(clicked()),this,SIGNAL(signal_can2())); //CAN2

}

void CSeniorSetupInfo::senior_setup_init()       //高级设置
{
    this->setFixedSize(620,ROW_HEIGHT * 3);
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 3);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 4;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("音量调整使能");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);
#if 0
    m_pushbutton_line1 = new CMyPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line1,848 - 290 - 20,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2,70,70);
    m_pushbutton_line1->set_pushbutton_enable(true);
    m_pushbutton_line1->set_pushbutton_sheet("QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}",
                                            "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}");
#endif
    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(848 - 290 - 20,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    str_show = tr("怠速使能");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
#if 0
    m_pushbutton_line1 = new CMyPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line1,621 - 290 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2,70,70);
    m_pushbutton_line1->set_pushbutton_enable(true);
    m_pushbutton_line1->set_pushbutton_sheet("QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}",
                                            "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}");
#endif
    m_checkbox_line2 = new QCheckBox(this);
    m_checkbox_line2->setFixedSize(70,70);
    m_checkbox_line2->move(621 - 290 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    m_lineedit2 = new QLineEdit(this);
    m_lineedit2->move(665 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit2->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit2,true);

    str_show = tr("~");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,751 - 290, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_lineedit2_1 = new QLineEdit(this);
    m_lineedit2_1->move(771 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit2_1->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit2_1,true);

    str_show = tr("h");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,861 - 290, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);


    str_show = tr("温度检测");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
#if 0
    m_pushbutton_line3 = new CMyPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line3,758 - 290 - 20,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2,70,70);
    m_pushbutton_line3->set_pushbutton_enable(true);
    m_pushbutton_line3->set_pushbutton_sheet("QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}",
                                            "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}");
#endif
    m_checkbox_line3 = new QCheckBox(this);
    m_checkbox_line3->setFixedSize(70,70);
    m_checkbox_line3->move(758 - 290 - 20,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3->setFocusPolicy(Qt::NoFocus);

    QString str_sheet = BUTTON_CONFIG_SHEET;

    m_pushbutton_line3_1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line3_1,799 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line3_1->setText(tr("配置"));

    connect(m_pushbutton_line3_1,SIGNAL(clicked()),this,SIGNAL(signal_temperature_check())); //温度检测
}

void CSeniorSetupInfo::set_type(char _type)
{
    switch(_type)
    {
        case kEnumCommonConfig:      //通用配置
        {
            common_config_init();
        }break;
        case kEnumWarningEnable:       //告警使能
        {
            warning_enable_init();
        }break;
        case kEnumCanSetting:    //CAN设置
        {
            can_setting_init();
        }break;
        case SeniorSetup:             //高级设置
        {
            senior_setup_init();
        }break;
    }
}

SeniorSetupWidget::SeniorSetupWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("SeniorSetupWidget");
    this->setStyleSheet("QWidget#SeniorSetupWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

        QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    scroll_layout();
    senior_setup_init_new();
    submain_1_init();
    submain_2_init();
    submain_3_init();
    submain_4_init();
    submain_5_init();
    submain_6_init();

    read_data();
    CProtocol::getInstance()->call_json_system_senior_setup_set(CProtocol::kEnumSave);
}

void SeniorSetupWidget::scroll_layout()
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
/*
    QWidget *m_widget_submain_1; //通用配置　－－　定位方式
    QWidget *m_widget_submain_2;//通用配置　－－　惯性导航（配置）
    QWidget *m_widget_submain_3;//通用配置　－－　惯性导航（配置 - 帮助）
    QWidget *m_widget_submain_4;//CAN设置　－－　调试
    QWidget *m_widget_submain_5;//高级设置　－－　温度检测（配置）
*/
    m_widget_submain_1 = new QWidget;//通用配置　－－　定位方式
    m_widget_submain_1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_submain_1->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_1->hide();

    m_widget_submain_2 = new QWidget;//通用配置　－－　惯性导航（配置）
    m_widget_submain_2->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_submain_2->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_2->hide();

    m_widget_submain_3 = new QWidget;//通用配置　－－　惯性导航（配置 - 帮助）
    m_widget_submain_3->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_submain_3->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_3->hide();

    m_widget_submain_4 = new QWidget;//CAN设置　－－　调试1
    m_widget_submain_4->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_submain_4->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_4->hide();

    m_widget_submain_5 = new QWidget;//CAN设置　－－　调试2
    m_widget_submain_5->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_submain_5->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_5->hide();

    m_widget_submain_6 = new QWidget;//高级设置　－－　温度检测（配置）
    m_widget_submain_6->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_submain_6->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_6->hide();


}

void SeniorSetupWidget::submain_1_init() //通用配置　－－　定位方式
{
    QWidget *pRectWidget = new QWidget(m_widget_submain_1);
    pRectWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget->setFixedSize(620,77);
    pRectWidget->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    for(int index = 0;index < 5;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_submain_1);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
    QWidget *pLine1 = new QWidget(m_widget_submain_1);
    QWidget *pLine2 = new QWidget(m_widget_submain_1);
     pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,3 * ROW_HEIGHT);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,3 * ROW_HEIGHT);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    int alignRight = 290 - 250 + 300;

    QString str_show = tr("模块信息");
    m_label_submain1_line1 = new QLabel(m_widget_submain_1);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_submain1_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("模块型号");
    m_label_submain1_line2 = new QLabel(m_widget_submain_1);
    CCommonInterface::init_label_text(m_label_submain1_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_show = tr("UM220-INSN");
    m_label_submain1_line2_1 = new QLabel(m_widget_submain_1);
    CCommonInterface::init_label_text(m_label_submain1_line2_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);


    str_show = tr("软件版本");
    m_label_submain1_line3 = new QLabel(m_widget_submain_1);
    CCommonInterface::init_label_text(m_label_submain1_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("V2.0.102.456");
    m_label_submain1_line3_1 = new QLabel(m_widget_submain_1);
    CCommonInterface::init_label_text(m_label_submain1_line3_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("S/N号");
    m_label_submain1_line4 = new QLabel(m_widget_submain_1);
    CCommonInterface::init_label_text(m_label_submain1_line4,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = tr("SA1023554663285");
    m_label_submain1_line4_1 = new QLabel(m_widget_submain_1);
    CCommonInterface::init_label_text(m_label_submain1_line4_1,alignRight, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,str_sheet1,Qt::AlignVCenter | Qt::AlignRight);

}


void SeniorSetupWidget::submain_2_init()//通用配置　－－　惯性导航（配置）
{
    QWidget *pRectWidget = new QWidget(m_widget_submain_2);
    pRectWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget->setFixedSize(620,77);
    pRectWidget->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    for(int index = 0;index < 6;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_submain_2);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
    QWidget *pLine1 = new QWidget(m_widget_submain_2);
    QWidget *pLine2 = new QWidget(m_widget_submain_2);
     pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,4 * ROW_HEIGHT);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,4 * ROW_HEIGHT);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    //int alignRight = 290 - 250 + 300;

    QString str_show = tr("设备安装方向配置");
    m_label_submain2_line1 = new QLabel(m_widget_submain_2);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_submain2_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    str_show = tr("设备X轴相对车X轴转角");
    m_label_submain2_line2 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_show = tr("<");
    m_label_submain2_line2_1 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line2_1,761 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_lineedit_submain2_line2 = new QLineEdit(m_widget_submain_2);
    m_lineedit_submain2_line2->move(783 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain2_line2->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain2_line2,true);

    str_show = tr("°");
    m_label_submain2_line2_2 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line2_2,873 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);


    str_show = tr("设备Y轴相对车Y轴转角");
    m_label_submain2_line3 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("<");
    m_label_submain2_line3_1 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line3_1,761 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_lineedit_submain2_line3 = new QLineEdit(m_widget_submain_2);
    m_lineedit_submain2_line3->move(783 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain2_line3->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain2_line3,true);

    str_show = tr("°");
    m_label_submain2_line3_2 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line3_2,873 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);


    str_show = tr("设备Z轴相对车Z轴转角");
    m_label_submain2_line4 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line4,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

     str_show = tr("<");
    m_label_submain2_line4_1 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line4_1,761 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_lineedit_submain2_line4 = new QLineEdit(m_widget_submain_2);
    m_lineedit_submain2_line4->move(783 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain2_line4->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain2_line4,true);

    str_show = tr("°");
    m_label_submain2_line4_2 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line4_2,873 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);


    str_show = tr("转弯Gain值");
    m_label_submain2_line5 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line5,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    str_show = tr("<");
    m_label_submain2_line5_1 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line5_1,761 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_lineedit_submain2_line5 = new QLineEdit(m_widget_submain_2);
    m_lineedit_submain2_line5->move(783 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain2_line5->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain2_line5,true);

    str_show = tr("°");
    m_label_submain2_line5_2 = new QLabel(m_widget_submain_2);
    CCommonInterface::init_label_text(m_label_submain2_line5_2,873 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_pushbutton_save = new QPushButton(m_widget_submain_2);
    CCommonInterface::init_new_button(m_pushbutton_save,435 - 250,640 - MAIN_HEAD_HEIGHT - 70,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_save->setText(tr("保存"));

    m_pushbutton_help = new QPushButton(m_widget_submain_2);
    CCommonInterface::init_new_button(m_pushbutton_help,624 - 250,640 - MAIN_HEAD_HEIGHT - 70,160,65,LTY_PUSHBUTTON_HELP);
    m_pushbutton_help->setText(tr("帮助"));

    connect(m_pushbutton_help,SIGNAL(clicked()),this,SLOT(slot_seniorsetupinfo_help()));

}
void SeniorSetupWidget::submain_3_init()//通用配置　－－　惯性导航（配置 - 帮助）
{
    QWidget *pRectWidget = new QWidget(m_widget_submain_3);
    pRectWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget->setFixedSize(620,77);
    pRectWidget->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");
    #if 0
    for(int index = 0;index < 2;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(_widget);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
    #endif
    QWidget *pLine1 = new QWidget(m_widget_submain_3);
    QWidget *pLine2 = new QWidget(m_widget_submain_3);
    QWidget *pLine3 = new QWidget(m_widget_submain_3);

    pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,1 * ROW_HEIGHT + 80 + 296);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,1 * ROW_HEIGHT + 80 + 296);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine3->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT + 1 * ROW_HEIGHT + 80 + 296);
    pLine3->setFixedSize(620,1);
    pLine3->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");


    QString str_show = tr("帮助");

    m_label_submain3_line1 = new QLabel(m_widget_submain_3);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_submain3_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

        //QWidget *m_widget_submain3_1;
    //QWidget *m_widget_submain3_2;
    m_widget_submain3_1 = new QWidget(m_widget_submain_3);
    m_widget_submain3_1->setFixedSize(238,189);
    m_widget_submain3_1->move(320 - 250,340 - MAIN_HEAD_HEIGHT - 50);
    m_widget_submain3_1->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/senior_setup/icon_engine.png);}");

    m_widget_submain3_2 = new QWidget(m_widget_submain_3);
    m_widget_submain3_2->setFixedSize(290,189);
    m_widget_submain3_2->move(588 - 250,340 - MAIN_HEAD_HEIGHT - 50);
    m_widget_submain3_2->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/senior_setup/icon_bus.png);}");

}

void SeniorSetupWidget::submain_45_create(char _type,QWidget *_widget)
{
    QWidget *pRectWidget = new QWidget(_widget);
    pRectWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget->setFixedSize(620,77);
    pRectWidget->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    for(int index = 0;index < 3;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(_widget);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
    QWidget *pLine1 = new QWidget(_widget);
    QWidget *pLine2 = new QWidget(_widget);
    QWidget *pLine3 = new QWidget(_widget);

     pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,1 * ROW_HEIGHT + 80 + 296);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,1 * ROW_HEIGHT + 80 + 296);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine3->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT + 1 * ROW_HEIGHT + 80 + 296);
    pLine3->setFixedSize(620,1);
    pLine3->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");


    //int alignRight = 290 - 250 + 300;

    QString str_show = "";
    if(1 == _type)
        str_show = tr("CAN1调试");
    else
        str_show = tr("CAN2调试");
    m_label_submain4_line1 = new QLabel(_widget);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_submain4_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    str_show = tr("测试命令");
    m_label_submain4_line2 = new QLabel(_widget);
    CCommonInterface::init_label_text(m_label_submain4_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_sheet = BUTTON_CONFIG_SHEET;

    m_pushbutton_submain4_line2 = new QPushButton(_widget);
    CCommonInterface::init_new_button(m_pushbutton_submain4_line2,800 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_submain4_line2->setText(tr("发送"));



    str_sheet = "QTextEdit{\
        background:#0F1D31;\
        border:1px solid #24508F;\
        font: 20px;\
        color:#8ea2c0;\
        }";
    if(1 == _type)
    {
        m_textedit1 = new QTextEdit(_widget);
        m_textedit1->setStyleSheet(str_sheet);
        m_textedit1->move(290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + 40);
        m_textedit1->setFixedSize(558,296);

        connect(m_pushbutton_submain4_line2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_send_can1_command()));
    }
    else
    {
        m_textedit2 = new QTextEdit(_widget);
        m_textedit2->setStyleSheet(str_sheet);
        m_textedit2->move(290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + 40);
        m_textedit2->setFixedSize(558,296);

        connect(m_pushbutton_submain4_line2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_send_can2_command()));
    }


}

void SeniorSetupWidget::submain_45_printf_can(char _type,const QString &_data)
{
    QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss ");
    if(1 == _type)
        m_textedit1->append(strTime + _data);
    else
        m_textedit2->append(strTime + _data);

}

void SeniorSetupWidget::slot_pushbutton_send_can1_command()//CAN设置　－－　调试1-发送
{
    CCommonData::getInstance()->m_CCanInfo1.m_command = "send test command1";
    CProtocol::getInstance()->call_json_system_senior_setup_set(3);
    submain_45_printf_can(1,CCommonData::getInstance()->m_CCanInfo1.m_command );

}
void SeniorSetupWidget::slot_pushbutton_send_can2_command()//CAN设置　－－　调试2-发送
{
    CCommonData::getInstance()->m_CCanInfo2.m_command = "send test command2";
    CProtocol::getInstance()->call_json_system_senior_setup_set(4);
    submain_45_printf_can(2,CCommonData::getInstance()->m_CCanInfo2.m_command );
}

void SeniorSetupWidget::submain_4_init()//CAN设置　－－　调试1
{
    submain_45_create(1,m_widget_submain_4);

}
void SeniorSetupWidget::submain_5_init()//CAN设置　－－　调试2
{
    submain_45_create(2,m_widget_submain_5);
}

void SeniorSetupWidget::submain_6_init()//高级设置　－－　温度检测（配置）
{
    QWidget *pRectWidget = new QWidget(m_widget_submain_6);
    pRectWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget->setFixedSize(620,77);
    pRectWidget->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    for(int index = 0;index < 6;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_submain_6);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
    QWidget *pLine1 = new QWidget(m_widget_submain_6);
    QWidget *pLine2 = new QWidget(m_widget_submain_6);
     pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,4 * ROW_HEIGHT);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,4 * ROW_HEIGHT);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    //int alignRight = 290 - 250 + 300;

    QString str_show = tr("温度检测");
    m_label_submain6_line1 = new QLabel(m_widget_submain_6);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_submain6_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    str_show = tr("内温安全范围");
    m_label_submain6_line2 = new QLabel(m_widget_submain_6);
    CCommonInterface::init_label_text(m_label_submain6_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_lineedit_submain6_line2_1 = new QLineEdit(m_widget_submain_6);
    m_lineedit_submain6_line2_1->move(665 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain6_line2_1->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain6_line2_1,true);

    str_show = tr("~");
    m_label_submain6_line2_1 = new QLabel(m_widget_submain_6);
    CCommonInterface::init_label_text(m_label_submain6_line2_1,751 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_lineedit_submain6_line2_2 = new QLineEdit(m_widget_submain_6);
    m_lineedit_submain6_line2_2->move(771 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain6_line2_2->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain6_line2_2,true);

    str_show = tr("℃");
    m_label_submain6_line2_2 = new QLabel(m_widget_submain_6);
    CCommonInterface::init_label_text(m_label_submain6_line2_2,861 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);


    str_show = tr("外温安全范围");
    m_label_submain6_line3 = new QLabel(m_widget_submain_6);
    CCommonInterface::init_label_text(m_label_submain6_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_lineedit_submain6_line3_1 = new QLineEdit(m_widget_submain_6);
    m_lineedit_submain6_line3_1->move(665 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain6_line3_1->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain6_line3_1,true);

    str_show = tr("~");
    m_label_submain6_line3_1 = new QLabel(m_widget_submain_6);
    CCommonInterface::init_label_text(m_label_submain6_line3_1,751 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_lineedit_submain6_line3_2 = new QLineEdit(m_widget_submain_6);
    m_lineedit_submain6_line3_2->move(771 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain6_line3_2->setFixedSize(80,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain6_line3_2,true);

    str_show = tr("℃");
    m_label_submain6_line3_2 = new QLabel(m_widget_submain_6);
    CCommonInterface::init_label_text(m_label_submain6_line3_2,861 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);


    str_show = tr("报警上传时间间隔");
    m_label_submain6_line4 = new QLabel(m_widget_submain_6);
    CCommonInterface::init_label_text(m_label_submain6_line4,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_lineedit_submain6_line4_1 = new QLineEdit(m_widget_submain_6);
    m_lineedit_submain6_line4_1->move(665 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_submain6_line4_1->setFixedSize(186,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_submain6_line4_1,true);

    str_show = tr("S");
    m_label_submain6_line4_1 = new QLabel(m_widget_submain_6);
    CCommonInterface::init_label_text(m_label_submain6_line4_1,861 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);


}

void SeniorSetupWidget::senior_setup_init_new()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("通用配置"));

    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");


    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("告警使能"));

    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_3 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_3->setDisplayText(tr("CAN设置"));

    m_button_3->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_3->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_3->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_4 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_4->setDisplayText(tr("高级设置"));

    m_button_4->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_4->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_4->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");


    //CSeniorSetupInfo *m_senior_info1;
    m_senior_info1 = new CSeniorSetupInfo(CSeniorSetupInfo::kEnumCommonConfig,m_widget_main);
    m_senior_info2 = new CSeniorSetupInfo(CSeniorSetupInfo::kEnumWarningEnable,m_widget_main);
    m_senior_info3 = new CSeniorSetupInfo(CSeniorSetupInfo::kEnumCanSetting,m_widget_main);
    m_senior_info4 = new CSeniorSetupInfo(CSeniorSetupInfo::SeniorSetup,m_widget_main);

    connect(m_senior_info1,SIGNAL(signal_location_mode()),this,SLOT(slot_seniorsetupinfo_location_mode()));
    connect(m_senior_info1,SIGNAL(signal_navigation()),this,SLOT(slot_seniorsetupinfo__navigation()));
    connect(m_senior_info3,SIGNAL(signal_can1()),this,SLOT(slot_seniorsetupinfo__can1()));
    connect(m_senior_info3,SIGNAL(signal_can2()),this,SLOT(slot_seniorsetupinfo__can2()));
    connect(m_senior_info4,SIGNAL(signal_temperature_check()),this,SLOT(slot_seniorsetupinfo__temperature_check()));

    m_label_space1  = new QLabel(m_widget_main);
    m_label_space1->setFixedHeight(10);
    m_label_space1->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space2  = new QLabel(m_widget_main);
    m_label_space2->setFixedHeight(10);
    m_label_space2->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space3  = new QLabel(m_widget_main);
    m_label_space3->setFixedHeight(10);
    m_label_space3->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_vboxlayout_storage = new QVBoxLayout(m_widget_main);//左 上 右 下
    m_vboxlayout_storage->setContentsMargins(290 - 250,158 - MAIN_HEAD_HEIGHT - 50,0,0);
    m_vboxlayout_storage->setSpacing(0);
    m_vboxlayout_storage->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_vboxlayout_storage->addWidget(m_button_1);
    m_vboxlayout_storage->addWidget(m_senior_info1);
    m_vboxlayout_storage->addWidget(m_label_space1);
    m_vboxlayout_storage->addWidget(m_button_2);
    m_vboxlayout_storage->addWidget(m_senior_info2);
    m_vboxlayout_storage->addWidget(m_label_space2);
    m_vboxlayout_storage->addWidget(m_button_3);
    m_vboxlayout_storage->addWidget(m_senior_info3);
    m_vboxlayout_storage->addWidget(m_label_space3);
    m_vboxlayout_storage->addWidget(m_button_4);
    m_vboxlayout_storage->addWidget(m_senior_info4);

    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));
    connect(m_button_3,SIGNAL(clicked()),this,SLOT(slot_button_3()));
    connect(m_button_4,SIGNAL(clicked()),this,SLOT(slot_button_4()));

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_senior_info1->hide();
    m_senior_info2->hide();
    m_senior_info3->hide();
    m_senior_info4->hide();

}

//通用配置
void SeniorSetupWidget::slot_seniorsetupinfo_location_mode()    //定位方式
{
    m_widget_main->hide();
    m_widget_submain_1->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_submain_1);
}
void SeniorSetupWidget::slot_seniorsetupinfo__navigation()       //惯性导航
{
    m_widget_main->hide();
    m_widget_submain_2->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_submain_2);
}
void SeniorSetupWidget::slot_seniorsetupinfo_help()
{
    m_widget_submain_2->hide();
    m_widget_submain_3->show();
    m_vboxlayout_widget->removeWidget(m_widget_submain_2);
    m_vboxlayout_widget->addWidget(m_widget_submain_3);
}

//CAN设置
void SeniorSetupWidget::slot_seniorsetupinfo__can1()
{
    m_widget_main->hide();
    m_widget_submain_4->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_submain_4);
}
void SeniorSetupWidget::slot_seniorsetupinfo__can2()
{
    m_widget_main->hide();
    m_widget_submain_5->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_submain_5);
}
//高级设置
void SeniorSetupWidget::slot_seniorsetupinfo__temperature_check() //温度检测
{
    m_widget_main->hide();
    m_widget_submain_6->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_submain_6);
}

void SeniorSetupWidget::slot_pushbutton_return()
{

    if(m_widget_submain_1->isVisible())
    {
        m_widget_main->show();
        m_widget_submain_1->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_1);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_submain_2->isVisible())
    {
        m_widget_main->show();
        m_widget_submain_2->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_2);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_submain_3->isVisible())
    {
        m_widget_submain_2->show();
        m_widget_submain_3->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_3);
        m_vboxlayout_widget->addWidget(m_widget_submain_2);
    }
    else if(m_widget_submain_4->isVisible())
    {
        m_widget_main->show();
        m_widget_submain_4->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_4);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
     else if(m_widget_submain_5->isVisible())
    {
        m_widget_main->show();
        m_widget_submain_5->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_5);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
     else if(m_widget_submain_6->isVisible())
    {
        m_widget_main->show();
        m_widget_submain_6->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_6);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else
    {
        emit signal_window_close();
    }
}

void SeniorSetupWidget::slot_button_1()
{
    if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_senior_info1->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_senior_info1->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height +  ROW_HEIGHT * 8);
    }
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_senior_info2->hide();
    m_senior_info3->hide();
    m_senior_info4->hide();


}
void SeniorSetupWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_senior_info2->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_senior_info2->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height +  ROW_HEIGHT * 4);
    }
    m_button_1->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_press_1 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_senior_info1->hide();
    m_senior_info3->hide();
    m_senior_info4->hide();
}
void SeniorSetupWidget::slot_button_3()
{
    if(m_button_press_3)
    {
        m_button_press_3 = false;
        m_button_3->setSelected(false);
        m_senior_info3->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_3 = true;
        m_button_3->setSelected(true);
        m_senior_info3->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_4->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_4 = false;
    m_senior_info1->hide();
    m_senior_info2->hide();
    m_senior_info4->hide();
}
void SeniorSetupWidget::slot_button_4()
{
    if(m_button_press_4)
    {
        m_button_press_4 = false;
        m_button_4->setSelected(false);
        m_senior_info4->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_4 = true;
        m_button_4->setSelected(true);
        m_senior_info4->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + ROW_HEIGHT * 1);
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_senior_info1->hide();
    m_senior_info2->hide();
    m_senior_info3->hide();
}

void SeniorSetupWidget::read_data()
{
    //通用配置
    CCommonData::getInstance()->m_CSystemSeniorSetup.location_mode = m_senior_info1->m_combobox_line1->currentIndex(); //定位方式 0:混合定位 1:北斗定位 2:GPS定位
    CCommonData::getInstance()->m_CSystemSeniorSetup.custom_navigation_enable = (m_senior_info1->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//惯性导航使能
    //惯性导航-配置(start)
    CCommonData::getInstance()->m_CSystemSeniorSetup.corner_x = m_lineedit_submain2_line2->text().toInt(); //设备X轴相对车X轴转角
    CCommonData::getInstance()->m_CSystemSeniorSetup.corner_y = m_lineedit_submain2_line3->text().toInt();//设备Y轴相对车Y轴转角
    CCommonData::getInstance()->m_CSystemSeniorSetup.corner_z = m_lineedit_submain2_line4->text().toInt(); //设备Z轴相对车Z轴转角
    CCommonData::getInstance()->m_CSystemSeniorSetup.corner_gain = m_lineedit_submain2_line5->text().toInt(); //转弯Gain值
    //惯性导航-配置(end)
    CCommonData::getInstance()->m_CSystemSeniorSetup.m_station_mode = m_senior_info1->m_combobox_line3->currentIndex(); //报站方式 0:自动 1:手动
    CCommonData::getInstance()->m_CSystemSeniorSetup.in_station_radius = m_senior_info1->m_lineedit4->text().toInt();//进站半径
    CCommonData::getInstance()->m_CSystemSeniorSetup.out_station_radius = m_senior_info1->m_lineedit5->text().toInt();//出站半径
    CCommonData::getInstance()->m_CSystemSeniorSetup.inflection_point_radius = m_senior_info1->m_lineedit6->text().toInt();//拐点半径
    CCommonData::getInstance()->m_CSystemSeniorSetup.over_speed_still_tip = m_senior_info1->m_lineedit7->text().toInt(); //超速保持提醒
    CCommonData::getInstance()->m_CSystemSeniorSetup.over_speed_tip_times = m_senior_info1->m_lineedit8->text().toInt();//超速提醒次数
    CCommonData::getInstance()->m_CSystemSeniorSetup.non_operating_speed_limit = m_senior_info1->m_lineedit9->text().toInt();//非运营限速
    CCommonData::getInstance()->m_CSystemSeniorSetup.operating_mode = m_senior_info1->m_combobox_line10->currentIndex();//0:营运　1:非营运

    //告警使能
    CCommonData::getInstance()->m_CSystemSeniorSetup.urgent_acceleration_enable = (m_senior_info2->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//紧急加速使能 // 0:未选择状态 1:选中状态
    CCommonData::getInstance()->m_CSystemSeniorSetup.urgent_acceleration_value = m_senior_info2->m_lineedit1->text().toInt();//紧急加速值  (km/s)

    CCommonData::getInstance()->m_CSystemSeniorSetup.emergency_deceleration_enable = (m_senior_info2->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//紧急减速使能 // 0:未选择状态 1:选中状态
    CCommonData::getInstance()->m_CSystemSeniorSetup.emergency_deceleration_value = m_senior_info2->m_lineedit2->text().toInt();//紧急减速值  (km/s)

    CCommonData::getInstance()->m_CSystemSeniorSetup.hysteresis_alarm_enable = (m_senior_info2->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;//滞站报警使能 // 0:未选择状态 1:选中状态
    CCommonData::getInstance()->m_CSystemSeniorSetup.hysteresis_alarm_value = m_senior_info2->m_lineedit3->text().toInt();//滞站报警值

    CCommonData::getInstance()->m_CSystemSeniorSetup.skip_alarm_enable = (m_senior_info2->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//越站报警使能 // 0:未选择状态 1:选中状态
    CCommonData::getInstance()->m_CSystemSeniorSetup.skip_alarm_value = m_senior_info2->m_lineedit4->text().toInt();//越站报警值

    CCommonData::getInstance()->m_CSystemSeniorSetup.fatigue_driving_check_enable = (m_senior_info2->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//疲劳驾驶检测使能 // 0:未选择状态 1:选中状态
    CCommonData::getInstance()->m_CSystemSeniorSetup.fatigue_driving_check_value = m_senior_info2->m_lineedit5->text().toInt();//疲劳驾驶检测值

    CCommonData::getInstance()->m_CSystemSeniorSetup.idling_enable = (m_senior_info2->m_checkbox_line6->checkState() == Qt::Checked)? 1:0;//怠速使能使能 // 0:未选择状态 1:选中状态
    CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value = m_senior_info2->m_lineedit6->text().toInt();//怠速使能值


    //CAN设置
    CCommonData::getInstance()->m_CSystemSeniorSetup.can1_enable = (m_senior_info3->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemSeniorSetup.can2_enable = (m_senior_info3->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;

    //高级设置
    CCommonData::getInstance()->m_CSystemSeniorSetup.volumn_adjustment = (m_senior_info4->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemSeniorSetup.idling_enable1 =(m_senior_info4->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value1 = m_senior_info4->m_lineedit2->text().toInt();//怠速值1
    CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value2 = m_senior_info4->m_lineedit2_1->text().toInt();//怠速值2

    CCommonData::getInstance()->m_CSystemSeniorSetup.temperature_check_enable =(m_senior_info4->m_checkbox_line3->checkState() == Qt::Checked)? 1:0; //温度检测使能

    //温度检测-配置
    CCommonData::getInstance()->m_CSystemSeniorSetup.in_temperature_security1 = m_lineedit_submain6_line2_1->text().toInt(); //内温安全范围1
    CCommonData::getInstance()->m_CSystemSeniorSetup.in_temperature_security2 = m_lineedit_submain6_line2_2->text().toInt();//内温安全范围2
    CCommonData::getInstance()->m_CSystemSeniorSetup.out_temperature_security1 = m_lineedit_submain6_line3_1->text().toInt();//外温安全范围1
    CCommonData::getInstance()->m_CSystemSeniorSetup.out_temperature_security2 = m_lineedit_submain6_line3_2->text().toInt();//外温安全范围2
    CCommonData::getInstance()->m_CSystemSeniorSetup.police_upload_time = m_lineedit_submain6_line4_1->text().toInt(); //报警上传时间间隔

}
void SeniorSetupWidget::save_data()
{
    CSystemSeniorSetup info;
    //通用配置
    info.location_mode = m_senior_info1->m_combobox_line1->currentIndex(); //定位方式 0:混合定位 1:北斗定位 2:GPS定位
    info.custom_navigation_enable = (m_senior_info1->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//惯性导航使能
    //惯性导航-配置(start)
    info.corner_x = m_lineedit_submain2_line2->text().toInt(); //设备X轴相对车X轴转角
    info.corner_y = m_lineedit_submain2_line3->text().toInt();//设备Y轴相对车Y轴转角
    info.corner_z = m_lineedit_submain2_line4->text().toInt(); //设备Z轴相对车Z轴转角
    info.corner_gain = m_lineedit_submain2_line5->text().toInt(); //转弯Gain值
    //惯性导航-配置(end)
    info.m_station_mode = m_senior_info1->m_combobox_line3->currentIndex(); //报站方式 0:自动 1:手动
    info.in_station_radius = m_senior_info1->m_lineedit4->text().toInt();//进站半径
    info.out_station_radius = m_senior_info1->m_lineedit5->text().toInt();//出站半径
    info.inflection_point_radius = m_senior_info1->m_lineedit6->text().toInt();//拐点半径
    info.over_speed_still_tip = m_senior_info1->m_lineedit7->text().toInt(); //超速保持提醒
    info.over_speed_tip_times = m_senior_info1->m_lineedit8->text().toInt();//超速提醒次数
    info.non_operating_speed_limit = m_senior_info1->m_lineedit9->text().toInt();//非运营限速
    info.operating_mode = m_senior_info1->m_combobox_line10->currentIndex();   //0:营运　1:非营运

    //告警使能
    info.urgent_acceleration_enable = (m_senior_info2->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//紧急加速使能 // 0:未选择状态 1:选中状态
    info.urgent_acceleration_value = m_senior_info2->m_lineedit1->text().toInt();//紧急加速值  (km/s)

    info.emergency_deceleration_enable = (m_senior_info2->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//紧急减速使能 // 0:未选择状态 1:选中状态
    info.emergency_deceleration_value = m_senior_info2->m_lineedit2->text().toInt();//紧急减速值  (km/s)

    info.hysteresis_alarm_enable = (m_senior_info2->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;//滞站报警使能 // 0:未选择状态 1:选中状态
    info.hysteresis_alarm_value = m_senior_info2->m_lineedit3->text().toInt();//滞站报警值

    info.skip_alarm_enable = (m_senior_info2->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//越站报警使能 // 0:未选择状态 1:选中状态
    info.skip_alarm_value = m_senior_info2->m_lineedit4->text().toInt();//越站报警值

    info.fatigue_driving_check_enable = (m_senior_info2->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//疲劳驾驶检测使能 // 0:未选择状态 1:选中状态
    info.fatigue_driving_check_value = m_senior_info2->m_lineedit5->text().toInt();//疲劳驾驶检测值

    info.idling_enable = (m_senior_info2->m_checkbox_line6->checkState() == Qt::Checked)? 1:0;//怠速使能使能 // 0:未选择状态 1:选中状态
    info.idling_value = m_senior_info2->m_lineedit6->text().toInt();//怠速使能值


    //CAN设置
    info.can1_enable = (m_senior_info3->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;
    info.can2_enable = (m_senior_info3->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;

    //高级设置
    info.volumn_adjustment = (m_senior_info4->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;
    info.idling_enable1 =(m_senior_info4->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;

    info.idling_value1 = m_senior_info4->m_lineedit2->text().toInt();//怠速值1
    info.idling_value2 = m_senior_info4->m_lineedit2_1->text().toInt();//怠速值2

    info.temperature_check_enable =(m_senior_info4->m_checkbox_line3->checkState() == Qt::Checked)? 1:0; //温度检测使能

    //温度检测-配置
    info.in_temperature_security1 = m_lineedit_submain6_line2_1->text().toInt(); //内温安全范围1
    info.in_temperature_security2 = m_lineedit_submain6_line2_2->text().toInt();//内温安全范围2
    info.out_temperature_security1 = m_lineedit_submain6_line3_1->text().toInt();//外温安全范围1
    info.out_temperature_security2 = m_lineedit_submain6_line3_2->text().toInt();//外温安全范围2
    info.police_upload_time = m_lineedit_submain6_line4_1->text().toInt(); //报警上传时间间隔

    if(info.location_mode != CCommonData::getInstance()->m_CSystemSeniorSetup.location_mode ||
        info.custom_navigation_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.custom_navigation_enable ||
        info.corner_x != CCommonData::getInstance()->m_CSystemSeniorSetup.corner_x ||
        info.corner_y != CCommonData::getInstance()->m_CSystemSeniorSetup.corner_y ||
        info.corner_z != CCommonData::getInstance()->m_CSystemSeniorSetup.corner_z ||
        info.m_station_mode != CCommonData::getInstance()->m_CSystemSeniorSetup.m_station_mode ||
        info.in_station_radius != CCommonData::getInstance()->m_CSystemSeniorSetup.in_station_radius ||
        info.out_station_radius != CCommonData::getInstance()->m_CSystemSeniorSetup.out_station_radius ||
        info.inflection_point_radius != CCommonData::getInstance()->m_CSystemSeniorSetup.inflection_point_radius ||
        info.over_speed_still_tip != CCommonData::getInstance()->m_CSystemSeniorSetup.over_speed_still_tip ||
        info.over_speed_tip_times != CCommonData::getInstance()->m_CSystemSeniorSetup.over_speed_tip_times ||
        info.non_operating_speed_limit != CCommonData::getInstance()->m_CSystemSeniorSetup.non_operating_speed_limit ||
        info.operating_mode != CCommonData::getInstance()->m_CSystemSeniorSetup.operating_mode ||
        info.urgent_acceleration_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.urgent_acceleration_enable ||
        info.urgent_acceleration_value != CCommonData::getInstance()->m_CSystemSeniorSetup.urgent_acceleration_value ||
        info.emergency_deceleration_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.emergency_deceleration_enable ||
        info.emergency_deceleration_value != CCommonData::getInstance()->m_CSystemSeniorSetup.emergency_deceleration_value ||
        info.hysteresis_alarm_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.hysteresis_alarm_enable ||
        info.hysteresis_alarm_value !=  CCommonData::getInstance()->m_CSystemSeniorSetup.hysteresis_alarm_value ||
        info.skip_alarm_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.skip_alarm_enable ||
        info.skip_alarm_value != CCommonData::getInstance()->m_CSystemSeniorSetup.skip_alarm_value ||
        info.fatigue_driving_check_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.fatigue_driving_check_enable ||
        info.fatigue_driving_check_value != CCommonData::getInstance()->m_CSystemSeniorSetup.fatigue_driving_check_value ||
        info.idling_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.idling_enable ||
        info.idling_value != CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value ||
        info.can1_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.can1_enable ||
        info.can2_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.can2_enable ||
        info.volumn_adjustment != CCommonData::getInstance()->m_CSystemSeniorSetup.volumn_adjustment ||
        info.idling_enable1 != CCommonData::getInstance()->m_CSystemSeniorSetup.idling_enable1 ||
        info.idling_value1 != CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value1 ||
        info.idling_value2 != CCommonData::getInstance()->m_CSystemSeniorSetup.idling_value2 ||
        info.temperature_check_enable != CCommonData::getInstance()->m_CSystemSeniorSetup.temperature_check_enable ||
        info.in_temperature_security1 != CCommonData::getInstance()->m_CSystemSeniorSetup.in_temperature_security1 ||
        info.in_temperature_security2 != CCommonData::getInstance()->m_CSystemSeniorSetup.in_temperature_security2 ||
        info.out_temperature_security1 != CCommonData::getInstance()->m_CSystemSeniorSetup.out_temperature_security1 ||
        info.out_temperature_security2 != CCommonData::getInstance()->m_CSystemSeniorSetup.out_temperature_security2 ||
        info.police_upload_time != CCommonData::getInstance()->m_CSystemSeniorSetup.police_upload_time
    )
    {
        if(CCommonData::getInstance()->messagebox_data_change())
        {
            read_data();
            CProtocol::getInstance()->call_json_system_senior_setup_set(CProtocol::kEnumSave);
        }
        else
        {
            slot_senior_setup_widget_event(1,CCommonData::getInstance()->m_CSystemSeniorSetup);
        }
    }




}

void SeniorSetupWidget::slot_senior_setup_widget_event(char _type,const CSystemSeniorSetup &_info)
{
    switch(_type)
    {
        case 1:
        {
            //通用配置
            m_senior_info1->m_combobox_line1->setCurrentIndex(_info.location_mode); //定位方式 0:混合定位 1:北斗定位 2:GPS定位

            m_label_submain1_line2_1->setText(_info.modular_mode);//模块型号
            m_label_submain1_line3_1->setText(_info.software_version);//软件版本
            m_label_submain1_line4_1->setText(_info.sn_number);//S/N号

            if(0 == _info.custom_navigation_enable)//惯性导航使能
                m_senior_info1->m_checkbox_line2->setChecked(false);
            else
                m_senior_info1->m_checkbox_line2->setChecked(true);

            //惯性导航-配置(start)
            m_lineedit_submain2_line2->setText(QString::number(_info.corner_x)); //设备X轴相对车X轴转角
            m_lineedit_submain2_line3->setText(QString::number(_info.corner_y));//设备Y轴相对车Y轴转角
            m_lineedit_submain2_line4->setText(QString::number(_info.corner_z)); //设备Z轴相对车Z轴转角
            m_lineedit_submain2_line5->setText(QString::number(_info.corner_gain)); //转弯Gain值
            //惯性导航-配置(end)

            m_senior_info1->m_combobox_line3->setCurrentIndex(_info.m_station_mode);                //报站方式 0:自动 1:手动
            m_senior_info1->m_lineedit4->setText(QString::number(_info.in_station_radius));         //进站半径
            m_senior_info1->m_lineedit5->setText(QString::number(_info.out_station_radius));        //出站半径
            m_senior_info1->m_lineedit6->setText(QString::number(_info.inflection_point_radius));   //拐点半径
            m_senior_info1->m_lineedit7->setText(QString::number(_info.over_speed_still_tip));      //超速保持提醒
            m_senior_info1->m_lineedit8->setText(QString::number(_info.over_speed_tip_times));      //超速提醒次数
            m_senior_info1->m_lineedit9->setText(QString::number(_info.non_operating_speed_limit)); //非运营限速
            m_senior_info1->m_combobox_line10->setCurrentIndex(_info.operating_mode);               //0:营运　1:非营运

            //告警使能

            if(0 == _info.urgent_acceleration_enable)//紧急加速使能 // 0:未选择状态 1:选中状态
                m_senior_info2->m_checkbox_line1->setChecked(false);
            else
                m_senior_info2->m_checkbox_line1->setChecked(true);
            m_senior_info2->m_lineedit1->setText(QString::number(_info.urgent_acceleration_value));//紧急加速值  (km/s)

            if(0 == _info.emergency_deceleration_enable)//紧急减速使能 // 0:未选择状态 1:选中状态
                m_senior_info2->m_checkbox_line2->setChecked(false);
            else
                m_senior_info2->m_checkbox_line2->setChecked(true);
            m_senior_info2->m_lineedit2->setText(QString::number(_info.emergency_deceleration_value));//紧急减速值  (km/s)

            if(0 == _info.hysteresis_alarm_enable)//滞站报警使能 // 0:未选择状态 1:选中状态
                m_senior_info2->m_checkbox_line3->setChecked(false);
            else
                m_senior_info2->m_checkbox_line3->setChecked(true);
            m_senior_info2->m_lineedit3->setText(QString::number(_info.hysteresis_alarm_value));//滞站报警值

           if(0 == _info.skip_alarm_enable)//越站报警使能
                m_senior_info2->m_checkbox_line4->setChecked(false);
            else
                m_senior_info2->m_checkbox_line4->setChecked(true);
            m_senior_info2->m_lineedit4->setText(QString::number(_info.skip_alarm_value));//越站报警值

            if(0 == _info.fatigue_driving_check_enable)//疲劳驾驶检测使能
                m_senior_info2->m_checkbox_line5->setChecked(false);
            else
                m_senior_info2->m_checkbox_line5->setChecked(true);
            m_senior_info2->m_lineedit5->setText(QString::number(_info.fatigue_driving_check_value));//疲劳驾驶检测值

            if(0 == _info.idling_enable)//怠速使能使能
                m_senior_info2->m_checkbox_line6->setChecked(false);
            else
                m_senior_info2->m_checkbox_line6->setChecked(true);
            m_senior_info2->m_lineedit6->setText(QString::number(_info.idling_value));//怠速使能值

            //CAN设置
            if(0 == _info.can1_enable)//CAN1  0:未选择状态 1:选中状态
                m_senior_info3->m_checkbox_line1->setChecked(false);
            else
                m_senior_info3->m_checkbox_line1->setChecked(true);

            if(0 == _info.can2_enable)//CAN2  0:未选择状态 1:选中状态
                m_senior_info3->m_checkbox_line2->setChecked(false);
            else
                m_senior_info3->m_checkbox_line2->setChecked(true);

            //高级设置  m_lineedit2_1

            if(0 == _info.volumn_adjustment)//音量调整使能  0:未选择状态 1:选中状态
                m_senior_info4->m_checkbox_line1->setChecked(false);
            else
                m_senior_info4->m_checkbox_line1->setChecked(true);

            if(0 == _info.idling_enable1)//怠速使能  0:未选择状态 1:选中状态
                m_senior_info4->m_checkbox_line2->setChecked(false);
            else
                m_senior_info4->m_checkbox_line2->setChecked(true);

            m_senior_info4->m_lineedit2->setText(QString::number(_info.idling_value1));//怠速值1
            m_senior_info4->m_lineedit2_1->setText(QString::number(_info.idling_value2));//怠速值2

            if(0 == _info.temperature_check_enable)//温度检测使能  0:未选择状态 1:选中状态
                m_senior_info4->m_checkbox_line3->setChecked(false);
            else
                m_senior_info4->m_checkbox_line3->setChecked(true);


            //温度检测-配置
            m_lineedit_submain6_line2_1->setText(QString::number(_info.in_temperature_security1)); //内温安全范围1
            m_lineedit_submain6_line2_2->setText(QString::number(_info.in_temperature_security2));//内温安全范围2
            m_lineedit_submain6_line3_1->setText(QString::number(_info.out_temperature_security1));//外温安全范围1
            m_lineedit_submain6_line3_2->setText(QString::number(_info.out_temperature_security2));//外温安全范围2
            m_lineedit_submain6_line4_1->setText(QString::number(_info.police_upload_time)); //报警上传时间间隔
        }break;
        case 2:
        {
            submain_45_printf_can(1,CCommonData::getInstance()->m_CCanInfo1.m_result);
        }break;
        case 3:
        {
            submain_45_printf_can(2,CCommonData::getInstance()->m_CCanInfo2.m_result);
        }break;
    }


}




















