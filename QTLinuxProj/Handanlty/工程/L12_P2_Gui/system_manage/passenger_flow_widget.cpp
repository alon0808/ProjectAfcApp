#include "passenger_flow_widget.h"

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"
#include <QListView>
#include <QTimer>

#include "./class/common_interface.h"
#include "mainwindow.h"
#include "class/ltystring.h"
#include "class/common/my_export.h"
#include "class/common/my_messagebox.h"
#include "LtyCommonStyle.h"

#include "protocol.h"
#include "common_data.h"
#include "class/common/progress_box.h"

#include <QDebug>

#define MESSAGEBOX_BUTTON_WIDTH   160
#define MESSAGEBOX_BUTTON_HEIGHT   65

#define STR_CONNECT         tr("连接")
#define STR_NO_CONNECT     tr("未连接")
#define STR_EXIST   tr("存在")
#define STR_NO_EXIST   tr("不存在")


CSelectSingBox *CSelectSingBox::m_single = NULL;

CSelectSingBox *CSelectSingBox::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CSelectSingBox(MainWindow::getInstance());
    }
    return m_single;
}

CSelectSingBox::CSelectSingBox(QWidget *parent) :
    QDialog(parent)
{

    this->setFixedSize(540,397);
    this->setObjectName("CSelectSingBox");
    this->setStyleSheet("QDialog#CSelectSingBox{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    //标题
    QString str_sheet = "";
    str_sheet = "QLabel{border:0px solid #ffffff;font:46px;background:#275ba7;color:#80e3ff;}";
    m_label_title = new QLabel(this);
    CCommonInterface::init_label_text(m_label_title,1,1,this->width() - 2,77,"",str_sheet);
    m_label_title->setText(tr("选择同步方式"));

    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png");

    m_pushbutton_close = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 10 - 40,(77 - 52)/2,40,52,str_sheet);

    //int show_width = this->width() - 5 - 5;
    //int show_height = this->height() - 77 - MESSAGEBOX_BUTTON_HEIGHT - 5 - 5 - 45;

    str_sheet = "QLabel{border:0px solid #ffffff;font:38px;background:transparent;color:#90A1BF;}";




    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,0,0,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,0,0,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);


    m_button_type = KEnumButtonCancel;

    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));


    int button1_x = (this->width() - m_pushbutton1->width() - m_pushbutton2->width() - 20)/2;
    int button2_x = button1_x + 20 + m_pushbutton1->width();
    int button1_y = this->height() - m_pushbutton1->height() - 45;
    m_pushbutton1->show();
    m_pushbutton2->show();

    m_pushbutton1->move(button1_x,button1_y);
    m_pushbutton1->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton2->move(button2_x,button1_y);
    m_pushbutton2->setText(CLtyString::getInstance()->m_cancel);

    m_current_select = kEnumSelect1;

    m_pushbutton_select1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_select1,100 + 50,100,70,70,"");

    m_pushbutton_select2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_select2,100 + 50,100 + 80,70,70,"");

//QLabel *m_label_select1;
   // QLabel *m_label_select2;
    m_label_select1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_select1,100 + 50 + 70,100 + 15,0,40,tr("U盘"));
    m_label_select2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_select2,100 + 50 + 70,100 + 80 + 15,0,40,tr("本地"));

    connect(m_pushbutton_select1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_select1()));
    connect(m_pushbutton_select2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_select2()));

    CCommonInterface::modify_button_sheet(m_pushbutton_select1,BUTTON_SELECT);
    CCommonInterface::modify_button_sheet(m_pushbutton_select2,BUTTON_SELECT_NO);

    m_define_screen = new CMyDefineScreen();

}

void CSelectSingBox::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSelectSingBox::init()
{
    static bool is_first = true;
    if(is_first)
    {
        is_first = false;
        this->move(242,166);
        m_define_screen->add_parent(this);
    }
    else
        this->move(CMyDefineScreen::getInstance()->get_change_factor_x(242),CMyDefineScreen::getInstance()->get_change_factor_y(166));

}

void CSelectSingBox::slot_pushbutton_close()
{

    slot_pushbutton2();
}

void CSelectSingBox::slot_pushbutton1()
{
     m_button_type = KEnumButtonOk;
     hide();

}
void CSelectSingBox::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();

}

CSelectSingBox::ButtonType CSelectSingBox::get_result()
{
    return m_button_type;
}

void CSelectSingBox::slot_pushbutton_select1()
{
    m_current_select = kEnumSelect1;
    CCommonInterface::modify_button_sheet(m_pushbutton_select1,BUTTON_SELECT);
    CCommonInterface::modify_button_sheet(m_pushbutton_select2,BUTTON_SELECT_NO);
}
void CSelectSingBox::slot_pushbutton_select2()
{
    m_current_select = kEnumSelect2;
    CCommonInterface::modify_button_sheet(m_pushbutton_select1,BUTTON_SELECT_NO);
    CCommonInterface::modify_button_sheet(m_pushbutton_select2,BUTTON_SELECT);
}

CPassengerFlowInfo::CPassengerFlowInfo(char _type,QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("CPassengerFlowInfo");
    this->setStyleSheet("QWidget#CPassengerFlowInfo{border:0px solid #24508F;background-color:#ffffff;}");
    m_type = _type;
    set_type(_type);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this,false);

}

void CPassengerFlowInfo::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CPassengerFlowInfo::set_type(char _type)
{
    switch(m_type)
    {
        case kEnumCam1_1:
        case kEnumCam2_1:
        case kEnumCam3_1:
        case kEnumCam4_1:
        {
            cam1_1_init();
        }break;

        case kEnumSeniorSetup:
        {
            senior_setup_init();
        }break;
    }

}
void CPassengerFlowInfo::cam1_1_init()
{
    this->setFixedSize(620,ROW_HEIGHT * 13);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 13);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 13;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("使能");
    m_label_line1_0 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1_0,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(620 - 70 - 20,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    str_show = tr("RS485");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_show = STR_NO_CONNECT;
    m_label_line1_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1_1,620 - 300 - 30, ROW_HEIGHT * 1,300,ROW_HEIGHT,str_show,"",Qt::AlignRight | Qt::AlignVCenter);



    str_show = tr("ID");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT *2,0,ROW_HEIGHT,str_show);

    str_show = tr("000000149");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,620 - 250 - 200, ROW_HEIGHT * 2,300,ROW_HEIGHT,str_show,"",Qt::AlignRight | Qt::AlignVCenter);

    m_lineedit_line2 = new QLineEdit(this);
    m_lineedit_line2->move(620 - 250 - 50,ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line2->setFixedSize(150,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line2,true);
    m_lineedit_line2->hide();

    QString str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_line2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line2,650 - 250 + 110,ROW_HEIGHT * 2 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line2->setText(tr("更改"));
    connect(m_pushbutton_line2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line2()));

    str_show = tr("参数文件");
    m_label_line3 = new QLabel(this);
    int str_length = CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = "";
    m_label_line3_0 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_0,30 + str_length + 30, ROW_HEIGHT * 3,100,ROW_HEIGHT,str_show,"",Qt::AlignRight | Qt::AlignVCenter);


    str_show = STR_NO_EXIST;
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,620 - 250 - 200, ROW_HEIGHT * 3,300,ROW_HEIGHT,str_show,"",Qt::AlignRight | Qt::AlignVCenter);


    str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_line3 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line3,650 - 250 + 110,ROW_HEIGHT * 3 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line3->setText(tr("同步"));

    connect(m_pushbutton_line3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line3()));

    str_show = tr("辅助线");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_checkbox_line4 = new QCheckBox(this);
    m_checkbox_line4->setFixedSize(70,70);
    m_checkbox_line4->move(620 - 70 - 20,ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4->setFocusPolicy(Qt::NoFocus);

    str_show = tr("计数显示模式");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_combobox_line5 = new QComboBox(this);
    m_combobox_line5->move(650 - 250,ROW_HEIGHT * 5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5->setFixedSize(190,COMBOBOX_HEIGHT);
    m_combobox_line5->addItem(tr("标准"));
    m_combobox_line5->addItem(tr("进站"));
    m_combobox_line5->setView(new QListView());

    str_show = tr("计数规则");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    m_combobox_line6 = new QComboBox(this);
    m_combobox_line6->move(650 - 250,ROW_HEIGHT * 6 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6->setFixedSize(190,COMBOBOX_HEIGHT);
    m_combobox_line6->addItem(tr("上进下出"));
    m_combobox_line6->addItem(tr("下进上出"));
    m_combobox_line6->setView(new QListView());

    str_show = tr("噪声过滤");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);

    m_lineedit_line7 = new QLineEdit(this);
    m_lineedit_line7->move(650 - 250,ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line7->setFixedSize(190,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line7,true);

    str_show = tr("灵敏度");
    m_label_line8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8,30, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    m_lineedit_line8 = new QLineEdit(this);
    m_lineedit_line8->move(650 - 250,ROW_HEIGHT * 8 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line8->setFixedSize(190,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line8);
    CCommonInterface::set_lineedit_double(m_lineedit_line8);

    str_show = tr("计数类型");
    m_label_line9 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line9,30, ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show);

    m_lineedit_line9 = new QLineEdit(this);
    m_lineedit_line9->move(650 - 250,ROW_HEIGHT * 9 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line9->setFixedSize(190,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line9,true);

    str_show = tr("安装高度");
    m_label_line10 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line10,30, ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show);

    m_lineedit_line10 = new QLineEdit(this);
    m_lineedit_line10->move(650 - 250,ROW_HEIGHT * 10 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line10->setFixedSize(190 - 40,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line10,true);

    str_show = tr("CM");
    m_label_line10_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line10_1,650 - 250 + 150 + 5, ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show);

    str_show = tr("过滤高度");
    m_label_line11 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line11,30, ROW_HEIGHT * 11,0,ROW_HEIGHT,str_show);

    m_lineedit_line11 = new QLineEdit(this);
    m_lineedit_line11->move(650 - 250,ROW_HEIGHT * 11 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line11->setFixedSize(190 - 40,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line11,true);

    str_show = tr("CM");
    m_label_line11_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line11_1,650 - 250 + 150 + 5, ROW_HEIGHT * 11,0,ROW_HEIGHT,str_show);

    //str_show = tr("检测区域");
    //m_label_line12 = new QLabel(this);
    //CCommonInterface::init_label_text(m_label_line12,30, ROW_HEIGHT * 11,0,ROW_HEIGHT,str_show);
    m_button_line12 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line12->move(0, ROW_HEIGHT * 12);
    m_button_line12->setDisplayText(tr("检测区域"));
    m_button_line12->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line12->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line12->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    pushbutton_line2_flag = true;
    connect(m_button_line12,SIGNAL(clicked()),this,SIGNAL(signal_button_line12()));

}
void CPassengerFlowInfo::set_progress(QString _text,bool _show)
{
    m_label_line3_0->setText(_text);
    if(_show)
    {
        m_label_line3_0->show();
        m_label_line3_0->update();
    }
    else
        m_label_line3_0->hide();
}
/*
138  246 355
*/
#if defined(APPLICATION_TYPE_D2)
#define TOP_BOTTOM_START    138
#define TOP_BOTTOM_MIDDLE   246
#define TOP_BOTTOM_END      355

#elif defined(APPLICATION_TYPE_P2)
#define TOP_BOTTOM_START    111
#define TOP_BOTTOM_MIDDLE   197
#define TOP_BOTTOM_END      283

#else
#define TOP_BOTTOM_START    177
#define TOP_BOTTOM_MIDDLE   315
#define TOP_BOTTOM_END      455
#endif

CCheckRegionInfo::CCheckRegionInfo(QWidget *parent,char _type) :
    QWidget(parent)
{
    m_type = _type;

    m_define_screen = new CMyDefineScreen();

    QWidget *pRectWidget1 = new QWidget(this);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    //int alignRight = 290 - 250 + 300;

    QString str_show = tr("检测区域");
    m_label_line1 = new QLabel(this);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,400,77,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,33);

/*
    QLineEdit *m_lineedit_line1;
    QLineEdit *m_lineedit_line2;
    QLineEdit *m_lineedit_line3;
    QLineEdit *m_lineedit_line4;
*/
    m_lineedit_line1 = new QLineEdit(this);
    m_lineedit_line1->move(777 - 250,274 - MAIN_HEAD_HEIGHT - 50);
    m_lineedit_line1->setFixedSize(130,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line1,true,0,90);
    m_lineedit_line1->setText("");

    m_lineedit_line2 = new QLineEdit(this);
    m_lineedit_line2->move(777 - 250,350 - MAIN_HEAD_HEIGHT - 50);
    m_lineedit_line2->setFixedSize(130,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line2,true,0,90);
    m_lineedit_line2->setText("");

    m_lineedit_line3 = new QLineEdit(this);
    m_lineedit_line3->move(777 - 250,427 - MAIN_HEAD_HEIGHT - 50);
    m_lineedit_line3->setFixedSize(130,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line3,true,0,90);
    m_lineedit_line3->setText("");

    m_lineedit_line4 = new QLineEdit(this);
    m_lineedit_line4->move(777 - 250,504 - MAIN_HEAD_HEIGHT - 50);
    m_lineedit_line4->setFixedSize(130,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line4,true,0,90);
    m_lineedit_line4->setText("");

    m_label_line1_1 = new QLabel(this);
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:#70F288;}";
    CCommonInterface::init_label_text(m_label_line1_1,646 - 250,294 - MAIN_HEAD_HEIGHT - 50,10,28,"",str_sheet);

    m_label_line1_2 = new QLabel(this);
    str_show = tr("上计数线1");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:23px;color:#8ea2c0;}";
    CCommonInterface::init_label_text(m_label_line1_2,646 - 250 + 20,294 - MAIN_HEAD_HEIGHT - 50,0,28,str_show,str_sheet,Qt::AlignCenter,23);

    m_label_line2_1 = new QLabel(this);
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:#CD76FF;}";
    CCommonInterface::init_label_text(m_label_line2_1,646 - 250,370 - MAIN_HEAD_HEIGHT - 50,10,28,"",str_sheet);

    m_label_line2_2 = new QLabel(this);
    str_show = tr("下计数线2");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:23px;color:#8ea2c0;}";
    CCommonInterface::init_label_text(m_label_line2_2,646 - 250 + 20,370 - MAIN_HEAD_HEIGHT - 50,0,28,str_show,str_sheet,Qt::AlignCenter,23);

    m_label_line3_1 = new QLabel(this);
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:#FF7676;}";
    CCommonInterface::init_label_text(m_label_line3_1,646 - 250,446 - MAIN_HEAD_HEIGHT - 50,10,28,"",str_sheet);

    m_label_line3_2 = new QLabel(this);
    str_show = tr("左边界线3");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:23px;color:#8ea2c0;}";
    CCommonInterface::init_label_text(m_label_line3_2,646 - 250 + 20,446 - MAIN_HEAD_HEIGHT - 50,0,28,str_show,str_sheet,Qt::AlignCenter,23);


    m_label_line4_1 = new QLabel(this);
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:#FAFF76;}";
    CCommonInterface::init_label_text(m_label_line4_1,646 - 250,523 - MAIN_HEAD_HEIGHT - 50,10,28,"",str_sheet);

    m_label_line4_2 = new QLabel(this);
    str_show = tr("右边界线4");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:23px;color:#8ea2c0;}";
    CCommonInterface::init_label_text(m_label_line4_2,646 - 250 + 20,523 - MAIN_HEAD_HEIGHT - 50,0,28,str_show,str_sheet,Qt::AlignCenter,23);


    str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_default = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_default,646 - 250,606 - MAIN_HEAD_HEIGHT - 50,67,65,str_sheet);
    m_pushbutton_default->setText(tr("默认"));

    m_pushbutton_bg_save = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_bg_save,740 - 290 + 25 - 5,606 - MAIN_HEAD_HEIGHT - 50,105 + 10,65,str_sheet);
    m_pushbutton_bg_save->setText(tr("背景保存"));

    m_pushbutton_save = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_save,842 - 250,606 - MAIN_HEAD_HEIGHT - 50,67,65,str_sheet);
    m_pushbutton_save->setText(tr("保存"));

    connect(m_pushbutton_default,SIGNAL(clicked()),this,SIGNAL(signal_pushbutton_default()));
    connect(m_pushbutton_bg_save,SIGNAL(clicked()),this,SIGNAL(signal_pushbutton_bg_save()));
    connect(m_pushbutton_save,SIGNAL(clicked()),this,SIGNAL(signal_pushbutton_save()));


    m_widget_no_video = new QWidget(this);
    m_widget_no_video->move(330 - 250,325 - MAIN_HEAD_HEIGHT - 50);
    m_widget_no_video->setFixedSize(253,278);
    m_widget_no_video->setStyleSheet("QWidget{border:0px solid #24508F;border-image:url(:/img/res/main_page/menu/system_manage/passenger_flow/content_background1.png);}");
    m_widget_no_video->hide();

/*
    QPushButton *m_pushbutton_line1;
    QPushButton *m_pushbutton_line2;
    QPushButton *m_pushbutton_line3;
    QPushButton *m_pushbutton_line4;

    QPushButton *m_pushbutton_left;
    QPushButton *m_pushbutton_right;
    QPushButton *m_pushbutton_top;
    QPushButton *m_pushbutton_bottom;
*/
    m_pushbutton_line1 = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/passenger_flow/upper_technicalline.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/upper_technicalline.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/upper_technicalline.png");

    CCommonInterface::init_new_button(m_pushbutton_line1,320 - 250,177,292,33,str_sheet);

    m_pushbutton_line2 = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/passenger_flow/lower_technical_line.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/lower_technical_line.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/lower_technical_line.png");

    CCommonInterface::init_new_button(m_pushbutton_line2,320 - 250,455,292,33,str_sheet);

    m_pushbutton_line3 = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/passenger_flow/left_limit.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/left_limit.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/left_limit.png");

    CCommonInterface::init_new_button(m_pushbutton_line3,68,288 - MAIN_HEAD_HEIGHT - 50,25,328,str_sheet);

    m_pushbutton_line4 = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/passenger_flow/right_boundary.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/right_boundary.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/right_boundary.png");

    CCommonInterface::init_new_button(m_pushbutton_line4,320,288 - MAIN_HEAD_HEIGHT - 50,25,328,str_sheet);

    //左　右 上　下
    m_pushbutton_left = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/passenger_flow/left_arrow.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/left_arrow.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/left_arrow.png");

    CCommonInterface::init_new_button(m_pushbutton_left,m_pushbutton_line3->x() - 50,m_pushbutton_line3->y() - 13,50,50,str_sheet);

    m_pushbutton_right = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/passenger_flow/right_arrow.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/right_arrow.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/right_arrow.png");

    CCommonInterface::init_new_button(m_pushbutton_right,m_pushbutton_line3->x() + m_pushbutton_line3->width(),m_pushbutton_line3->y() - 13,50,50,str_sheet);

    m_pushbutton_top = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/passenger_flow/up_arrow.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/up_arrow.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/up_arrow.png");

    CCommonInterface::init_new_button(m_pushbutton_top,m_pushbutton_line1->x() + m_pushbutton_line1->width() - 50 + 13,m_pushbutton_line1->y() - 50,50,50,str_sheet);

    m_pushbutton_bottom = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/passenger_flow/down_arrow.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/down_arrow.png").arg(":/img/res/main_page/menu/system_manage/passenger_flow/down_arrow.png");

    CCommonInterface::init_new_button(m_pushbutton_bottom,m_pushbutton_line1->x() + m_pushbutton_line1->width() - 50 + 13,m_pushbutton_line1->y() + m_pushbutton_line1->height(),50,50,str_sheet);

    m_pushbutton_top->hide();
    m_pushbutton_bottom->hide();
    m_current_line = kEnumLine3;

    connect(m_pushbutton_line1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line1()));
    connect(m_pushbutton_line2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line2()));
    connect(m_pushbutton_line3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line3()));
    connect(m_pushbutton_line4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line4()));

    connect(m_pushbutton_left,SIGNAL(clicked()),this,SLOT(slot_pushbutton_left()));
    connect(m_pushbutton_right,SIGNAL(clicked()),this,SLOT(slot_pushbutton_right()));
    connect(m_pushbutton_top,SIGNAL(clicked()),this,SLOT(slot_pushbutton_top()));
    connect(m_pushbutton_bottom,SIGNAL(clicked()),this,SLOT(slot_pushbutton_bottom()));

    connect(m_lineedit_line1,SIGNAL(textChanged(QString)),this,SLOT(slot_lineedit1_changed(QString)));
    connect(m_lineedit_line2,SIGNAL(textChanged(QString)),this,SLOT(slot_lineedit2_changed(QString)));
    connect(m_lineedit_line3,SIGNAL(textChanged(QString)),this,SLOT(slot_lineedit3_changed(QString)));
    connect(m_lineedit_line4,SIGNAL(textChanged(QString)),this,SLOT(slot_lineedit4_changed(QString)));

    m_auto_input = false;
    //update_lineedit_line1();
    //update_lineedit_line2();
    //update_lineedit_line3();
    //update_lineedit_line4();
    m_lineedit_line1->setText(QString::number(0));
    m_lineedit_line2->setText(QString::number(0));
    m_lineedit_line3->setText(QString::number(0));
    m_lineedit_line4->setText(QString::number(0));

    m_define_screen->add_parent(this,false);
    #if 0
    int top_start  = m_define_screen->get_change_factor_y(TOP_BOTTOM_START);
    int top_middle  = m_define_screen->get_change_factor_y(TOP_BOTTOM_MIDDLE);
    int top_end  = m_define_screen->get_change_factor_y(TOP_BOTTOM_END);

    top_end = top_end;
    #endif


}


void CCheckRegionInfo::resizeEvent(QResizeEvent *e)
{

    m_define_screen->reset_widget();

    m_lineedit_line1->setText("");//上计数线1
    m_lineedit_line2->setText("");//下计数线2
    m_lineedit_line3->setText("");//左边界线3
    m_lineedit_line4->setText("");//右边界线4

    if(1 == m_type)
    {
        //int test1 = CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line1;
        //int test2 = CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line2;
        //int test3 = CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line3;
        //int test4 = CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line4;

        m_lineedit_line1->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line1));//上计数线1
        m_lineedit_line2->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line2));//下计数线2
        m_lineedit_line3->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line3));//左边界线3
        m_lineedit_line4->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line4));//右边界线4
    }
    else if(2 == m_type)
    {
        m_lineedit_line1->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line1));//上计数线1
        m_lineedit_line2->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line2));//下计数线2
        m_lineedit_line3->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line3));//左边界线3
        m_lineedit_line4->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line4));//右边界线4
    }
    else if(3 == m_type)
    {
        m_lineedit_line1->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line1));//上计数线1
        m_lineedit_line2->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line2));//下计数线2
        m_lineedit_line3->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line3));//左边界线3
        m_lineedit_line4->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line4));//右边界线4
    }
    else if(4 == m_type)
    {
        m_lineedit_line1->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line1));//上计数线1
        m_lineedit_line2->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line2));//下计数线2
        m_lineedit_line3->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line3));//左边界线3
        m_lineedit_line4->setText(QString::number(CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line4));//右边界线4
    }


}

void CCheckRegionInfo::slot_lineedit1_changed(const QString &_text)
{

    //qDebug() << "CCheckRegionInfo::slot_lineedit1_changed() " << QString::number(m_auto_input);

    if(m_auto_input)
    {
        m_auto_input = false;
    }
    else
    {
        //qDebug() << "CCheckRegionInfo::slot_lineedit1_changed() enter!";

        hide_button_all();

        int input_value = _text.toInt();

        QPoint point = m_pushbutton_line1->pos();
        int new_y = TOP_BOTTOM_START + (TOP_BOTTOM_MIDDLE - TOP_BOTTOM_START) * input_value/90;

        point.setY(new_y);
        m_pushbutton_line1->move(point);

        this->repaint();
    }

}


void CCheckRegionInfo::slot_lineedit2_changed(const QString &_text)
{
    if(m_auto_input)
    {
        m_auto_input = false;
    }
    else
    {
        hide_button_all();

        int input_value = _text.toInt();

        input_value = 90 - input_value;
        QPoint point = m_pushbutton_line2->pos();
        int new_y = TOP_BOTTOM_MIDDLE + (TOP_BOTTOM_END - TOP_BOTTOM_MIDDLE) * input_value/90;

        point.setY(new_y);
        m_pushbutton_line2->move(point);

        this->repaint();
    }

}

#if defined(APPLICATION_TYPE_P2)
#define LEFT_RIGHT_START    43
#define LEFT_RIGHT_MIDDLE   122
#define LEFT_RIGHT_END      200
#else
#define LEFT_RIGHT_START    68
#define LEFT_RIGHT_MIDDLE   194
#define LEFT_RIGHT_END      320
#endif

void CCheckRegionInfo::slot_lineedit3_changed(const QString &_text)
{
    if(m_auto_input)
    {
        m_auto_input = false;
    }
    else
    {
        hide_button_all();

        int input_value = _text.toInt();

        QPoint point = m_pushbutton_line3->pos();
        int new_x = LEFT_RIGHT_START + (LEFT_RIGHT_MIDDLE - LEFT_RIGHT_START) * input_value/90;

        point.setX(new_x);
        m_pushbutton_line3->move(point);

        this->repaint();
    }

}
void CCheckRegionInfo::slot_lineedit4_changed(const QString &_text)
{
    if(m_auto_input)
    {
        m_auto_input = false;
    }
    else
    {
        hide_button_all();

        int input_value = _text.toInt();

        input_value = 90 - input_value;
        QPoint point = m_pushbutton_line4->pos();
        int new_x = LEFT_RIGHT_MIDDLE + (LEFT_RIGHT_END - LEFT_RIGHT_MIDDLE) * input_value/90;

        point.setX(new_x);
        m_pushbutton_line4->move(point);

        this->repaint();
    }

}

void CCheckRegionInfo::hide_button_all()
{
    m_pushbutton_left->hide();
    m_pushbutton_right->hide();
    m_pushbutton_top->hide();
    m_pushbutton_bottom->hide();
}

//线1
void CCheckRegionInfo::slot_pushbutton_line1()
{
    m_current_line = kEnumLine1;

    m_pushbutton_left->hide();
    m_pushbutton_right->hide();
    m_pushbutton_top->show();
    m_pushbutton_bottom->show();
#ifdef APPLICATION_TYPE_P2
    m_pushbutton_top->move(m_pushbutton_line1->x() + m_pushbutton_line1->width() - m_pushbutton_top->width() + 7,m_pushbutton_line1->y() - m_define_screen->get_change_factor_y(50));
    m_pushbutton_bottom->move(m_pushbutton_line1->x() + m_pushbutton_line1->width() - m_pushbutton_top->width() + 7,m_pushbutton_line1->y() + m_pushbutton_line1->height());
#else
    m_pushbutton_top->move(m_pushbutton_line1->x() + m_pushbutton_line1->width() - 50 + 13,m_pushbutton_line1->y() - 50);
    m_pushbutton_bottom->move(m_pushbutton_line1->x() + m_pushbutton_line1->width() - 50 + 13,m_pushbutton_line1->y() + m_pushbutton_line1->height());
#endif
    update_lineedit_line1();

    this->repaint();
}
//线2
void CCheckRegionInfo::slot_pushbutton_line2()
{
    m_current_line = kEnumLine2;

    m_pushbutton_left->hide();
    m_pushbutton_right->hide();
    m_pushbutton_top->show();
    m_pushbutton_bottom->show();
#ifdef APPLICATION_TYPE_P2
    m_pushbutton_top->move(m_pushbutton_line2->x() + m_pushbutton_line2->width() - m_pushbutton_top->width() + 7,m_pushbutton_line2->y() - m_define_screen->get_change_factor_y(50));
    m_pushbutton_bottom->move(m_pushbutton_line2->x() + m_pushbutton_line2->width() - m_pushbutton_top->width() + 7,m_pushbutton_line2->y() + m_pushbutton_line2->height());
#else
    m_pushbutton_top->move(m_pushbutton_line2->x() + m_pushbutton_line2->width() - 50 + 13,m_pushbutton_line2->y() - 50);
    m_pushbutton_bottom->move(m_pushbutton_line2->x() + m_pushbutton_line2->width() - 50 + 13,m_pushbutton_line2->y() + m_pushbutton_line2->height());
#endif
    update_lineedit_line2();

    this->repaint();
}



#define LEFT_RIGHT_PACE 5  //左右移动间隔
#define TOP_BOTTOM_PACE 7  //上下移动间隔



//线3
void CCheckRegionInfo::slot_pushbutton_line3()
{
    m_current_line = kEnumLine3;

    m_pushbutton_left->show();
    m_pushbutton_right->show();
    m_pushbutton_top->hide();
    m_pushbutton_bottom->hide();
#ifdef APPLICATION_TYPE_P2
    m_pushbutton_left->move(m_pushbutton_line3->x() - m_pushbutton_left->width(),m_pushbutton_line3->y() - 7);
    m_pushbutton_right->move(m_pushbutton_line3->x() + m_pushbutton_line3->width(),m_pushbutton_line3->y() - 7);
#else
    m_pushbutton_left->move(m_pushbutton_line3->x() - 50,m_pushbutton_line3->y() - 13);
    m_pushbutton_right->move(m_pushbutton_line3->x() + m_pushbutton_line3->width(),m_pushbutton_line3->y() - 13);
#endif
    update_lineedit_line3();

    this->repaint();
}
//线4
void CCheckRegionInfo::slot_pushbutton_line4()
{
    m_current_line = kEnumLine4;

    m_pushbutton_left->show();
    m_pushbutton_right->show();
    m_pushbutton_top->hide();
    m_pushbutton_bottom->hide();
#ifdef APPLICATION_TYPE_P2
    m_pushbutton_left->move(m_pushbutton_line4->x() - m_pushbutton_left->width(),m_pushbutton_line4->y() - 7);
    m_pushbutton_right->move(m_pushbutton_line4->x() + m_pushbutton_line4->width(),m_pushbutton_line4->y() - 7);
#else
    m_pushbutton_left->move(m_pushbutton_line4->x() - 50,m_pushbutton_line4->y() - 13);
    m_pushbutton_right->move(m_pushbutton_line4->x() + m_pushbutton_line4->width(),m_pushbutton_line4->y() - 13);
#endif
    update_lineedit_line4();

        this->repaint();
}

void CCheckRegionInfo::update_lineedit_line1()
{
    m_auto_input = true;
    int real_width = 90 * (m_pushbutton_line1->y() - TOP_BOTTOM_START)/(TOP_BOTTOM_MIDDLE - TOP_BOTTOM_START);
    m_lineedit_line1->setText(QString::number(real_width));
}
void CCheckRegionInfo::update_lineedit_line2()
{

    m_auto_input = true;
    int real_width = 90 - 90 * (m_pushbutton_line2->y() - TOP_BOTTOM_MIDDLE)/(TOP_BOTTOM_END - TOP_BOTTOM_MIDDLE);
    m_lineedit_line2->setText(QString::number(real_width));

}
void CCheckRegionInfo::update_lineedit_line3()
{
    m_auto_input = true;
    int real_width = 90 * (m_pushbutton_line3->x() - LEFT_RIGHT_START)/(LEFT_RIGHT_MIDDLE - LEFT_RIGHT_START);
    m_lineedit_line3->setText(QString::number(real_width));
}
void CCheckRegionInfo::update_lineedit_line4()
{
    m_auto_input = true;
    int real_width = 90 - 90 * (m_pushbutton_line4->x() - LEFT_RIGHT_MIDDLE)/(LEFT_RIGHT_END - LEFT_RIGHT_MIDDLE);
    m_lineedit_line4->setText(QString::number(real_width));
}

#include <QDebug>

//左
void CCheckRegionInfo::slot_pushbutton_left()
{
    if(kEnumLine3 == m_current_line)
    {
        QPoint point = m_pushbutton_line3->pos();

        if(point.x() > LEFT_RIGHT_START/* && point.x() <= 194*/)
        {
            int new_x = point.x() - LEFT_RIGHT_PACE;
            if(new_x < LEFT_RIGHT_START)
                new_x = LEFT_RIGHT_START;
            point.setX(new_x);
            m_pushbutton_line3->move(point);
        }

        slot_pushbutton_line3();
        qDebug() << m_pushbutton_line3->x() << ","<< m_pushbutton_line3->y();
    }
    else if(kEnumLine4 == m_current_line)
    {
        QPoint point = m_pushbutton_line4->pos();
        if(point.x() > LEFT_RIGHT_MIDDLE/* && point.x() <= 320*/)
        {
            int new_x = point.x() - LEFT_RIGHT_PACE;
            if(new_x < LEFT_RIGHT_MIDDLE)
                new_x = LEFT_RIGHT_MIDDLE;
            point.setX(new_x);
            m_pushbutton_line4->move(point);
        }


        slot_pushbutton_line4();
    }
    this->repaint();
}
//右
void CCheckRegionInfo::slot_pushbutton_right()
{
    if(kEnumLine3 == m_current_line)
    {
        QPoint point = m_pushbutton_line3->pos();
        if(/*point.x() >= 68 &&*/ point.x() < LEFT_RIGHT_MIDDLE)
        {
            int new_x = point.x() + LEFT_RIGHT_PACE;
            if(new_x > LEFT_RIGHT_MIDDLE)
                new_x = LEFT_RIGHT_MIDDLE;
            point.setX(new_x);
            m_pushbutton_line3->move(point);
        }

        slot_pushbutton_line3();
        qDebug() << m_pushbutton_line3->x() << ","<< m_pushbutton_line3->y();
    }
    else if(kEnumLine4 == m_current_line)
    {
        QPoint point = m_pushbutton_line4->pos();
        if(/*point.x() >= 194 &&*/ point.x() < LEFT_RIGHT_END)
        {
            int new_x = point.x() + LEFT_RIGHT_PACE;
            if(new_x > LEFT_RIGHT_END)
                new_x = LEFT_RIGHT_END;
            point.setX(new_x);
            m_pushbutton_line4->move(point);
        }

        slot_pushbutton_line4();
        qDebug() << m_pushbutton_line4->x() << ","<< m_pushbutton_line4->y();
    }
    this->repaint();
}



//上
void CCheckRegionInfo::slot_pushbutton_top()
{

    if(kEnumLine1 == m_current_line)
    {
        QPoint point = m_pushbutton_line1->pos();

        if(point.y() > TOP_BOTTOM_START /*&& point.y() <= 316*/)//177  316
        {
            int new_y = point.y() - TOP_BOTTOM_PACE;
            if(new_y < TOP_BOTTOM_START)
                new_y = TOP_BOTTOM_START;

            point.setY(new_y);
            m_pushbutton_line1->move(point);
        }


        slot_pushbutton_line1();

        qDebug() << m_pushbutton_line1->x() << ","<< m_pushbutton_line1->y();
    }
    else if(kEnumLine2 == m_current_line)
    {
        QPoint point = m_pushbutton_line2->pos();

        if(point.y() > TOP_BOTTOM_MIDDLE /*&& point.y() <= 455*/)//177  316
        {
            int new_y = point.y() - TOP_BOTTOM_PACE;
            if(new_y < TOP_BOTTOM_MIDDLE)
                new_y = TOP_BOTTOM_MIDDLE;

            point.setY(new_y);
            m_pushbutton_line2->move(point);
        }

        slot_pushbutton_line2();
        qDebug() << m_pushbutton_line2->x() << ","<< m_pushbutton_line2->y();
    }
    this->repaint();
}
//下
void CCheckRegionInfo::slot_pushbutton_bottom()
{
    if(kEnumLine1 == m_current_line)
    {
        QPoint point = m_pushbutton_line1->pos();

        if(/*point.y() >= 177 &&*/ point.y() < TOP_BOTTOM_MIDDLE)
        {
            int new_y = point.y() + LEFT_RIGHT_PACE;
            if(new_y > TOP_BOTTOM_MIDDLE)
                new_y = TOP_BOTTOM_MIDDLE;

            point.setY(new_y);
            m_pushbutton_line1->move(point);
        }


        slot_pushbutton_line1();

        qDebug() << m_pushbutton_line1->x() << ","<< m_pushbutton_line1->y();
    }
    else if(kEnumLine2 == m_current_line)
    {
        QPoint point = m_pushbutton_line2->pos();

         if(/*point.y() >= 316 &&*/ point.y() < TOP_BOTTOM_END)
        {
            int new_y = point.y() + LEFT_RIGHT_PACE;
            if(new_y > TOP_BOTTOM_END)
                new_y = TOP_BOTTOM_END;

            point.setY(new_y);
            m_pushbutton_line2->move(point);
        }

        slot_pushbutton_line2();
        qDebug() << m_pushbutton_line2->x() << ","<< m_pushbutton_line2->y();
    }
    this->repaint();
}





void CPassengerFlowInfo::slot_pushbutton_line2()
{
    if(pushbutton_line2_flag)
    {
        pushbutton_line2_flag = false;
        //修改状态
        m_label_line2_1->hide();
        m_lineedit_line2->show();
        m_lineedit_line2->setText(m_label_line2_1->text());
        m_pushbutton_line2->setText(tr("保存"));
    }
    else
    {
        pushbutton_line2_flag = true;
        //保存状态
        m_label_line2_1->show();
        m_lineedit_line2->hide();
        m_label_line2_1->setText(m_lineedit_line2->text());
        m_pushbutton_line2->setText(tr("修改"));
    }

}

void CPassengerFlowInfo::slot_pushbutton_line3()//guoyang
{

    //CSelectSingBox::getInstance()->move(242,166);
    CSelectSingBox::getInstance()->init();
    CSelectSingBox::getInstance()->exec();

    if(CSelectSingBox::KEnumButtonOk == CSelectSingBox::getInstance()->get_result())
    {
        CSelectSingBox::getInstance()->get_select();
        CProtocol::getInstance()->call_json_system_passenger_flow(4,CSelectSingBox::getInstance()->get_select(),m_type);//开始同步
#if 0


        CMyExport::getInstance()->set_content(tr("参数文件"),tr("正在同步..."),"",CLtyString::getInstance()->m_cancel);

        CMyExport::getInstance()->exec();
 #if 0
        if(CMyExport::KEnumButtonNotify == CMyExport::getInstance()->get_result())
        {
            QString str_tip = "";
            if(1 == CSelectSingBox::getInstance()->m_return_type)
            {
                str_tip = tr("同步成功!");
            }
            else if(2 == CSelectSingBox::getInstance()->m_return_type)
            {
                str_tip = tr("同步失败:") + CSelectSingBox::getInstance()->m_return_result;
            }


     //       CMyMessageBox::getInstance()->set_content(tr("温馨提示"),str_tip,CLtyString::getInstance()->m_confirm,"");
    //        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
     //       CMyMessageBox::getInstance()->exec();
        }
        else
        {//用户取消
            //通知取消
   //         CProtocol::getInstance()->call_json_system_passenger_flow(4,2,m_type);//取消同步
        }
#endif
#endif

        QString str_bg_save = tr("同步参数");


        //CProgressBox::getInstance()->test_init();
        CProgressBox::getInstance()->data_init();
        CProgressBox::getInstance()->set_title(str_bg_save);

        CProgressBox::getInstance()->exec();

        if(CProgressBox::KEnumStatusSuccess == CProgressBox::getInstance()->get_status())
        {
            CMyMessageBox::getInstance()->set_content(str_bg_save,tr("同步参数成功!"),
            CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
        }
 #if 1
       else if(CProgressBox::KEnumStatusUmount == CProgressBox::getInstance()->get_status())
        {
            CMyMessageBox::getInstance()->set_content(str_bg_save,tr("U盘挂载不上!"),
            CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
        }
  #endif

       else if(CProgressBox::KEnumStatusNull == CProgressBox::getInstance()->get_status())
        {


            CMyMessageBox::getInstance()->set_content(str_bg_save,tr("没有匹配参数!"),
            CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
        }

        else
        {
            if(CProgressBox::KEnumStatusFail == CProgressBox::getInstance()->get_status())
            {
                 //通知取消操作
                 CProtocol::getInstance()->call_json_system_passenger_flow(4,2,m_type);//取消同步
                 return;

            }
                 CMyMessageBox::getInstance()->set_content(str_bg_save,tr("同步参数失败!"),
                 CLtyString::getInstance()->m_confirm,"");
                 CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
                 CMyMessageBox::getInstance()->exec();


        }

    }

}




void CPassengerFlowInfo::cam2_1_init()
{

}
void CPassengerFlowInfo::senior_setup_init()
{
    this->setFixedSize(620,ROW_HEIGHT * 3);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 3);
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

    m_button_line1 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line1->move(0, ROW_HEIGHT * 0);
    m_button_line1->setDisplayText(tr("计数使能"));
    m_button_line1->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    pushbutton_line2_flag = true;
    connect(m_button_line1,SIGNAL(clicked()),this,SIGNAL(signal_button_line1()));

    QString str_show = tr("图像拼接");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_checkbox_line2 = new QCheckBox(this);
    m_checkbox_line2->setFixedSize(70,70);
    m_checkbox_line2->move(620 - 70 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    str_show = tr("间隔宽度");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_lineedit_line3 = new QLineEdit(this);
    m_lineedit_line3->move(650 - 250,ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line3->setFixedSize(190 - 40,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line3,true);

    str_show = tr("CM");
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,650 - 250 + 150 + 5, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);


}


CPassengerFlowWidget::CPassengerFlowWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

    this->setObjectName("CPassengerFlowWidget");
    this->setStyleSheet("QWidget#CPassengerFlowWidget{background:transparent;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

    m_widget_bg_color = new QWidget(this);
    m_widget_bg_color->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_widget_bg_color->move(0,0);
    m_widget_bg_color->setStyleSheet("QWidget {border:0px;background:#000000;}");

    m_widget_bg_color_1 = new QWidget(this);
    m_widget_bg_color_1->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_widget_bg_color_1->move(0,0);
      //  m_widget_bg_color_1->setFixedSize(744,636);
    //m_widget_bg_color_1->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg_color_1->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background_1.png);}");
    m_widget_bg_color_1->hide();

    scroll_layout();
    init();
    count_enable_init();

    read_data();


    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);
    m_define_screen->add_parent(m_widget_count_enable);


}

void CPassengerFlowWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CPassengerFlowWidget::scroll_layout()
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
    //int test_value = m_define_screen->get_change_factor_y(m_scroll_widget_height);
    m_scrollarea_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollarea_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pMainLayout->addWidget(m_scrollarea_1);

    m_ScrollController = new ScrollController(m_scroll_widget);
    m_ScrollController->addMonitorWidget((QScrollArea *)(m_scrollarea_1));
    m_ScrollController->setVerticalScrollBarStyleSheet(CCommonData::getInstance()->get_scroll_sheet(),0,50,50);
    m_ScrollController->set_scroll_right_length(20);

    //m_define_screen->add_parent(m_scroll_widget,false);

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

    //m_define_screen->add_parent(pwidget);

    m_widget_main = new QWidget;
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_widget_main->setStyleSheet("QWidget{background: transparent;}");
    m_vboxlayout_widget->addWidget(m_widget_main);

    m_widget_check_region1 = new QWidget;
    m_widget_check_region1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_check_region1->setStyleSheet("QWidget{background: transparent;}");

    m_widget_check_region2 = new QWidget;
    m_widget_check_region2->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_check_region2->setStyleSheet("QWidget{background: transparent;}");


    m_widget_check_region3 = new QWidget;
    m_widget_check_region3->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_check_region3->setStyleSheet("QWidget{background: transparent;}");

    m_widget_check_region4 = new QWidget;
    m_widget_check_region4->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_check_region4->setStyleSheet("QWidget{background: transparent;}");


    m_widget_count_enable = new QWidget;
    m_widget_count_enable->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_count_enable->setStyleSheet("QWidget{background: transparent;}");


    m_widget_count_enable->hide();
    m_widget_check_region1->hide();
    m_widget_check_region2->hide();
    m_widget_check_region3->hide();
    m_widget_check_region4->hide();

    m_CCheckRegionInfo1 = new CCheckRegionInfo(m_widget_check_region1,1);
    m_CCheckRegionInfo1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_CCheckRegionInfo2 = new CCheckRegionInfo(m_widget_check_region2,2);
    m_CCheckRegionInfo2->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);


    m_CCheckRegionInfo3 = new CCheckRegionInfo(m_widget_check_region3,3);
    m_CCheckRegionInfo3->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_CCheckRegionInfo4 = new CCheckRegionInfo(m_widget_check_region4,4);
    m_CCheckRegionInfo4->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    //m_define_screen->add_parent(m_widget_main);

    m_define_screen->add_parent(m_widget_check_region1);
    m_define_screen->add_parent(m_widget_check_region2);
    m_define_screen->add_parent(m_widget_check_region3);
    m_define_screen->add_parent(m_widget_check_region4);


    connect(m_CCheckRegionInfo1,SIGNAL(signal_pushbutton_default()),this,SLOT(slot_check_region_cam1_1_button_default()));
    connect(m_CCheckRegionInfo1,SIGNAL(signal_pushbutton_bg_save()),this,SLOT(slot_check_region_cam1_1_button_bg_save()));
    connect(m_CCheckRegionInfo1,SIGNAL(signal_pushbutton_save()),this,SLOT(slot_check_region_cam1_1_button_save()));
    connect(m_CCheckRegionInfo2,SIGNAL(signal_pushbutton_default()),this,SLOT(slot_check_region_cam2_1_button_default()));
    connect(m_CCheckRegionInfo2,SIGNAL(signal_pushbutton_bg_save()),this,SLOT(slot_check_region_cam2_1_button_bg_save()));
    connect(m_CCheckRegionInfo2,SIGNAL(signal_pushbutton_save()),this,SLOT(slot_check_region_cam2_1_button_save()));


    connect(m_CCheckRegionInfo3,SIGNAL(signal_pushbutton_default()),this,SLOT(slot_check_region_cam3_1_button_default()));
    connect(m_CCheckRegionInfo3,SIGNAL(signal_pushbutton_bg_save()),this,SLOT(slot_check_region_cam3_1_button_bg_save()));
    connect(m_CCheckRegionInfo3,SIGNAL(signal_pushbutton_save()),this,SLOT(slot_check_region_cam3_1_button_save()));

    connect(m_CCheckRegionInfo4,SIGNAL(signal_pushbutton_default()),this,SLOT(slot_check_region_cam4_1_button_default()));
    connect(m_CCheckRegionInfo4,SIGNAL(signal_pushbutton_bg_save()),this,SLOT(slot_check_region_cam4_1_button_bg_save()));
    connect(m_CCheckRegionInfo4,SIGNAL(signal_pushbutton_save()),this,SLOT(slot_check_region_cam4_1_button_save()));



}

void CPassengerFlowWidget::slot_timer_init()
{
    m_widget_count_enable->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + 300));
}


void CPassengerFlowWidget::init()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("CAM1-1通道设置"));

    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    //m_button_1->move(290 - 250,158 - MAIN_HEAD_HEIGHT);

    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("CAM2-1通道设置"));

    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_3 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_3->setDisplayText(tr("CAM3-1通道设置"));

    m_button_3->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_3->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_3->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_4 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_4->setDisplayText(tr("CAM4-1通道设置"));

    m_button_4->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_4->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_4->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_5 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_5->setDisplayText(tr("高级设置"));

    m_button_5->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_5->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_5->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_passenger_flow_info1 = new CPassengerFlowInfo(CPassengerFlowInfo::kEnumCam1_1,m_widget_main);
    m_passenger_flow_info2 = new CPassengerFlowInfo(CPassengerFlowInfo::kEnumCam2_1,m_widget_main);
    m_passenger_flow_info3 = new CPassengerFlowInfo(CPassengerFlowInfo::kEnumCam3_1,m_widget_main);
    m_passenger_flow_info4 = new CPassengerFlowInfo(CPassengerFlowInfo::kEnumCam4_1,m_widget_main);
    m_passenger_flow_info5 = new CPassengerFlowInfo(CPassengerFlowInfo::kEnumSeniorSetup,m_widget_main);

    connect(m_passenger_flow_info1,SIGNAL(signal_button_line12()),this,SLOT(slot_button_check_region1()));
    connect(m_passenger_flow_info2,SIGNAL(signal_button_line12()),this,SLOT(slot_button_check_region2()));
    connect(m_passenger_flow_info3,SIGNAL(signal_button_line12()),this,SLOT(slot_button_check_region3()));
    connect(m_passenger_flow_info4,SIGNAL(signal_button_line12()),this,SLOT(slot_button_check_region4()));
    connect(m_passenger_flow_info5,SIGNAL(signal_button_line1()),this,SLOT(slot_button_count_enable()));

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


    m_vboxlayout_storage = new QVBoxLayout(m_widget_main);//左 上 右 下
    m_vboxlayout_storage->setContentsMargins(m_define_screen->get_change_factor_x(290 - 250),m_define_screen->get_change_factor_y(158 - MAIN_HEAD_HEIGHT - 50),0,0);
    m_vboxlayout_storage->setSpacing(0);
    m_vboxlayout_storage->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_vboxlayout_storage->addWidget(m_button_1);
    m_vboxlayout_storage->addWidget(m_passenger_flow_info1);
    m_vboxlayout_storage->addWidget(m_label_space1);

    m_vboxlayout_storage->addWidget(m_button_2);
    m_vboxlayout_storage->addWidget(m_passenger_flow_info2);
    m_vboxlayout_storage->addWidget(m_label_space2);

    m_vboxlayout_storage->addWidget(m_button_3);
    m_vboxlayout_storage->addWidget(m_passenger_flow_info3);
    m_vboxlayout_storage->addWidget(m_label_space3);

    m_vboxlayout_storage->addWidget(m_button_4);
    m_vboxlayout_storage->addWidget(m_passenger_flow_info4);
    m_vboxlayout_storage->addWidget(m_label_space4);

    m_vboxlayout_storage->addWidget(m_button_5);
    m_vboxlayout_storage->addWidget(m_passenger_flow_info5);

#if !defined(APPLICATION_TYPE_D2)
    m_button_3->hide();
    m_passenger_flow_info3->hide();
    m_label_space3->hide();

    m_button_4->hide();
    m_passenger_flow_info4->hide();
    m_label_space4->hide();

#endif

    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));
    connect(m_button_3,SIGNAL(clicked()),this,SLOT(slot_button_3()));
    connect(m_button_4,SIGNAL(clicked()),this,SLOT(slot_button_4()));
    connect(m_button_5,SIGNAL(clicked()),this,SLOT(slot_button_5()));

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;

    m_passenger_flow_info1->hide();
    m_passenger_flow_info2->hide();
    m_passenger_flow_info3->hide();
    m_passenger_flow_info4->hide();
    m_passenger_flow_info5->hide();

}

void CPassengerFlowWidget::count_enable_init()
{
    QWidget *pRectWidget = new QWidget(m_widget_count_enable);
    pRectWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget->setFixedSize(620,77);
    pRectWidget->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    for(int index = 0;index < 10;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_count_enable);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }
    QWidget *pLine1 = new QWidget(m_widget_count_enable);
    QWidget *pLine2 = new QWidget(m_widget_count_enable);
     pLine1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine1->setFixedSize(1,8 * ROW_HEIGHT);
    pLine1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    pLine2->move(290 - 250 + 620,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT);
    pLine2->setFixedSize(1,8 * ROW_HEIGHT);
    pLine2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    //int alignRight = 290 - 250 + 300;

    QString str_show = tr("计数使能");
    m_label_count_enable_line1 = new QLabel(m_widget_count_enable);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_count_enable_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,0,77,str_show,str_sheet,Qt::AlignCenter,33);

    str_show = tr("实时动态统计");
    m_label_count_enable_line2 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_checkbox_count_enable_line2 = new QCheckBox(m_widget_count_enable);
    m_checkbox_count_enable_line2->setFixedSize(70,70);
    m_checkbox_count_enable_line2->move(856 - 250 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_count_enable_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_count_enable_line2->setFocusPolicy(Qt::NoFocus);

    str_show = tr("开门统计");
    m_label_count_enable_line3 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line3,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_checkbox_count_enable_line3 = new QCheckBox(m_widget_count_enable);
    m_checkbox_count_enable_line3->setFixedSize(70,70);
    m_checkbox_count_enable_line3->move(856 - 250 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_count_enable_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_count_enable_line3->setFocusPolicy(Qt::NoFocus);

    str_show = tr("进站统计");
    m_label_count_enable_line4 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line4,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_checkbox_count_enable_line4 = new QCheckBox(m_widget_count_enable);
    m_checkbox_count_enable_line4->setFixedSize(70,70);
    m_checkbox_count_enable_line4->move(856 - 250 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_count_enable_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_count_enable_line4->setFocusPolicy(Qt::NoFocus);

    str_show = tr("站外统计");
    m_label_count_enable_line5 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line5,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_checkbox_count_enable_line5 = new QCheckBox(m_widget_count_enable);
    m_checkbox_count_enable_line5->setFixedSize(70,70);
    m_checkbox_count_enable_line5->move(856 - 250 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_count_enable_line5->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_count_enable_line5->setFocusPolicy(Qt::NoFocus);

    str_show = tr("速度统计");
    m_label_count_enable_line6 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line6,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_checkbox_count_enable_line6 = new QCheckBox(m_widget_count_enable);
    m_checkbox_count_enable_line6->setFixedSize(70,70);
    m_checkbox_count_enable_line6->move(620 - 75 - 50 - 20 - 70,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_count_enable_line6->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_count_enable_line6->setFocusPolicy(Qt::NoFocus);

    str_show = tr("≤");
    m_label_count_enable_line6_1 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line6_1,620 - 75 - 50 - 25,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);


    m_lineedit_count_enable_line6 = new QLineEdit(m_widget_count_enable);
    m_lineedit_count_enable_line6->move(620 - 75 - 50,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_count_enable_line6->setFixedSize(75,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_count_enable_line6,true);

    str_show = tr("km/h");
    m_label_count_enable_line6_2 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line6_2,620 - 40,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    str_show = tr("单圈误差校准");
    m_label_count_enable_line7 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line7,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    m_lineedit_count_enable_line7 = new QLineEdit(m_widget_count_enable);
    m_lineedit_count_enable_line7->move(620 - 75 - 50,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_count_enable_line7->setFixedSize(75,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_count_enable_line7,true);

    str_show = tr("Min");
    m_label_count_enable_line7_1 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line7_1,620 - 40,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    str_show = tr("关门延时");
    m_label_count_enable_line8 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line8,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);

    m_lineedit_count_enable_line8 = new QLineEdit(m_widget_count_enable);
    m_lineedit_count_enable_line8->move(620 - 75 - 50,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_count_enable_line8->setFixedSize(75,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_count_enable_line8,true);

    str_show = tr("S");
    m_label_count_enable_line8_1 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line8_1,620 - 40,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);

    str_show = tr("定时上报时间");
    m_label_count_enable_line9 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line9,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    m_lineedit_count_enable_line9 = new QLineEdit(m_widget_count_enable);
    m_lineedit_count_enable_line9->move(620 - 75 - 50,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 8 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_count_enable_line9->setFixedSize(75,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_count_enable_line9,true);

    str_show = tr("S");
    m_label_count_enable_line9_1 = new QLabel(m_widget_count_enable);
    CCommonInterface::init_label_text(m_label_count_enable_line9_1,620 - 40,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);



}


void CPassengerFlowWidget::slot_button_1()
{
    if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_passenger_flow_info1->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_passenger_flow_info1->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 12));
    }
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_5->setSelected(false);
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_passenger_flow_info2->hide();
    m_passenger_flow_info3->hide();
    m_passenger_flow_info4->hide();
    m_passenger_flow_info5->hide();


}
void CPassengerFlowWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_passenger_flow_info2->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_passenger_flow_info2->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 12));
    }
    m_button_1->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_5->setSelected(false);
    m_button_press_1 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_passenger_flow_info1->hide();
    m_passenger_flow_info3->hide();
    m_passenger_flow_info4->hide();
    m_passenger_flow_info5->hide();
}
void CPassengerFlowWidget::slot_button_3()
{
    if(m_button_press_3)
    {
        m_button_press_3 = false;
        m_button_3->setSelected(false);
        m_passenger_flow_info3->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_3 = true;
        m_button_3->setSelected(true);
        m_passenger_flow_info3->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 12));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_4->setSelected(false);
    m_button_5->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_4 = false;
    m_button_press_5 = false;
    m_passenger_flow_info1->hide();
    m_passenger_flow_info2->hide();
    m_passenger_flow_info4->hide();
    m_passenger_flow_info5->hide();
}

void CPassengerFlowWidget::slot_button_4()
{
    if(m_button_press_4)
    {
        m_button_press_4 = false;
        m_button_4->setSelected(false);
        m_passenger_flow_info4->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_4 = true;
        m_button_4->setSelected(true);
        m_passenger_flow_info4->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 12));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_5->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_5 = false;
    m_passenger_flow_info1->hide();
    m_passenger_flow_info2->hide();
    m_passenger_flow_info3->hide();
    m_passenger_flow_info5->hide();
}

void CPassengerFlowWidget::slot_button_5()
{
    if(m_button_press_5)
    {
        m_button_press_5 = false;
        m_button_5->setSelected(false);
        m_passenger_flow_info5->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_5 = true;
        m_button_5->setSelected(true);
        m_passenger_flow_info5->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + 200));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_passenger_flow_info1->hide();
    m_passenger_flow_info2->hide();
    m_passenger_flow_info3->hide();
    m_passenger_flow_info4->hide();
}

void CPassengerFlowWidget::slot_button_check_region1()
{
     CVideoOutput _videoOutput;
    _videoOutput.m_frame = CProtocol::kEnumFramePassengerDefine;
    _videoOutput.m_number = 1;
    _videoOutput.m_x = 330;
    _videoOutput.m_y = 325;
    _videoOutput.m_width = 253;
    _videoOutput.m_height = 278;
    _videoOutput.m_close = 1;
    CProtocol::getInstance()->call_json_camera_set(_videoOutput);

    m_widget_bg->hide();
    m_widget_bg_color->hide();
    m_widget_bg_color_1->show();
    m_widget_main->hide();
    m_widget_check_region1->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_check_region1);
}
void CPassengerFlowWidget::slot_button_check_region2()
{
    CVideoOutput _videoOutput;
    _videoOutput.m_frame = CProtocol::kEnumFramePassengerDefine;
    _videoOutput.m_number = 2;
    _videoOutput.m_x = 330;
    _videoOutput.m_y = 325;
    _videoOutput.m_width = 253;
    _videoOutput.m_height = 278;
    _videoOutput.m_close = 1;
    CProtocol::getInstance()->call_json_camera_set(_videoOutput);

    m_widget_bg->hide();
    m_widget_bg_color->hide();
    m_widget_bg_color_1->show();
    m_widget_main->hide();
    m_widget_check_region2->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_check_region2);
}
void CPassengerFlowWidget::slot_button_check_region3()
{
    m_widget_bg->hide();
    m_widget_bg_color->hide();
    m_widget_bg_color_1->show();
    m_widget_main->hide();
    m_widget_check_region3->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_check_region3);
}
void CPassengerFlowWidget::slot_button_check_region4()
{
    m_widget_bg->hide();
    m_widget_bg_color->hide();
    m_widget_bg_color_1->show();
    m_widget_main->hide();
    m_widget_check_region4->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_check_region4);
}


void CPassengerFlowWidget::slot_button_count_enable()
{
    static bool first_init = false;

    m_widget_main->hide();
    m_widget_count_enable->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_count_enable);

    if(!first_init)
    {
        first_init = true;
        QTimer::singleShot(10,this,SLOT(slot_timer_init()));
    }

}

void CPassengerFlowWidget::slot_pushbutton_home()
{
    if(m_widget_check_region1->isVisible())
    {
        CVideoOutput _videoOutput;
        _videoOutput.m_frame = CProtocol::kEnumFramePassengerDefine;
        _videoOutput.m_number = 1;
        _videoOutput.m_close = 2;
        CProtocol::getInstance()->call_json_camera_set(_videoOutput);

        m_widget_bg->show();
        m_widget_bg_color->show();
        m_widget_bg_color_1->hide();
        m_widget_main->show();
        m_widget_check_region1->hide();
        m_vboxlayout_widget->removeWidget(m_widget_check_region1);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_check_region2->isVisible())
    {
        CVideoOutput _videoOutput;
        _videoOutput.m_frame = CProtocol::kEnumFramePassengerDefine;
        _videoOutput.m_number = 2;
        _videoOutput.m_close = 2;
        CProtocol::getInstance()->call_json_camera_set(_videoOutput);

        m_widget_bg->show();
        m_widget_bg_color->show();
        m_widget_bg_color_1->hide();
        m_widget_main->show();
        m_widget_check_region2->hide();
        m_vboxlayout_widget->removeWidget(m_widget_check_region2);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
}

void CPassengerFlowWidget::slot_pushbutton_return()
{
    if(m_widget_count_enable->isVisible())
    {
        m_widget_main->show();
        m_widget_count_enable->hide();
        m_vboxlayout_widget->removeWidget(m_widget_count_enable);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_check_region1->isVisible())
    {
        CVideoOutput _videoOutput;
        _videoOutput.m_frame = CProtocol::kEnumFramePassengerDefine;
        _videoOutput.m_number = 1;
        _videoOutput.m_close = 2;
        CProtocol::getInstance()->call_json_camera_set(_videoOutput);

        m_widget_bg->show();
        m_widget_bg_color->show();
        m_widget_bg_color_1->hide();
        m_widget_main->show();
        m_widget_check_region1->hide();
        m_vboxlayout_widget->removeWidget(m_widget_check_region1);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_check_region2->isVisible())
    {
        CVideoOutput _videoOutput;
        _videoOutput.m_frame = CProtocol::kEnumFramePassengerDefine;
        _videoOutput.m_number = 2;
        _videoOutput.m_close = 2;
        CProtocol::getInstance()->call_json_camera_set(_videoOutput);

        m_widget_bg->show();
        m_widget_bg_color->show();
        m_widget_bg_color_1->hide();
        m_widget_main->show();
        m_widget_check_region2->hide();
        m_vboxlayout_widget->removeWidget(m_widget_check_region2);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    else if(m_widget_check_region3->isVisible())
    {
        m_widget_bg->show();
        m_widget_bg_color->show();
        m_widget_bg_color_1->hide();
        m_widget_main->show();
        m_widget_check_region3->hide();
        m_vboxlayout_widget->removeWidget(m_widget_check_region3);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else if(m_widget_check_region4->isVisible())
    {
        m_widget_bg->show();
        m_widget_bg_color->show();
        m_widget_bg_color_1->hide();
        m_widget_main->show();
        m_widget_check_region4->hide();
        m_vboxlayout_widget->removeWidget(m_widget_check_region4);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
#endif
    else
    {
        emit signal_window_close();
    }
}



/*
    m_label_line1_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1_1,620 - 250 - 200, ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,"",Qt::AlignRight | Qt::AlignVCenter);

    m_checkbox_line1
*/

void CPassengerFlowWidget::read_data()
{

    //CAM1-1通道设置

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_enable = (m_passenger_flow_info1->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使能  0:不选择 1:选择

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.is_connect;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_id = m_passenger_flow_info1->m_label_line2_1->text(); //ID

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.param_file_exist;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.guide = (m_passenger_flow_info1->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_show_mode = m_passenger_flow_info1->m_combobox_line5->currentIndex();//计数显示模式  0:标准  1:进站
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_rule = m_passenger_flow_info1->m_combobox_line6->currentIndex();//计数规则  //0:上进下出  1:下进上出
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.noise_filter = m_passenger_flow_info1->m_lineedit_line7->text().toInt();//噪声过滤
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.sensitivity = m_passenger_flow_info1->m_lineedit_line8->text().toDouble();//灵敏度
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_type = m_passenger_flow_info1->m_lineedit_line9->text().toInt();//计数类型
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.install_height = m_passenger_flow_info1->m_lineedit_line10->text().toInt();//安装高度
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.filter_height = m_passenger_flow_info1->m_lineedit_line11->text().toInt();//过滤高度
    //CAM1-1通道设置-检测区域
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line1 = m_CCheckRegionInfo1->m_lineedit_line1->text().toInt();//上计数线1
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line2 = m_CCheckRegionInfo1->m_lineedit_line2->text().toInt();//下计数线2
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line3 = m_CCheckRegionInfo1->m_lineedit_line3->text().toInt();//左边界线3
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line4 = m_CCheckRegionInfo1->m_lineedit_line4->text().toInt();//右边界线4

    //int test1 = CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line1;
    //int test2 = CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line2;
    //int test3 = CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line3;
    //int test4 = CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line4;

    //CAM2-1通道设置
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_enable = (m_passenger_flow_info2->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使能  0:不选择 1:选择

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.is_connect;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_id = m_passenger_flow_info2->m_label_line2_1->text(); //ID

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.param_file_exist;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.guide = (m_passenger_flow_info2->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//辅助线 0:不选择 1:选择

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_show_mode = m_passenger_flow_info2->m_combobox_line5->currentIndex();//计数显示模式  0:标准  1:进站
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_rule = m_passenger_flow_info2->m_combobox_line6->currentIndex();//计数规则  //0:上进下出  1:下进上出
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.noise_filter = m_passenger_flow_info2->m_lineedit_line7->text().toInt();//噪声过滤
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.sensitivity = m_passenger_flow_info2->m_lineedit_line8->text().toDouble();//灵敏度
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_type = m_passenger_flow_info2->m_lineedit_line9->text().toInt();//计数类型
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.install_height = m_passenger_flow_info2->m_lineedit_line10->text().toInt();//安装高度
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.filter_height = m_passenger_flow_info2->m_lineedit_line11->text().toInt();//过滤高度
    //CAM2-1通道设置-检测区域
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line1 = m_CCheckRegionInfo2->m_lineedit_line1->text().toInt();//上计数线1
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line2 = m_CCheckRegionInfo2->m_lineedit_line2->text().toInt();//下计数线2
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line3 = m_CCheckRegionInfo2->m_lineedit_line3->text().toInt();//左边界线3
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line4 = m_CCheckRegionInfo2->m_lineedit_line4->text().toInt();//右边界线4

    //CAM3-1通道设置
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_enable = (m_passenger_flow_info3->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使能  0:不选择 1:选择

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.is_connect;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_id = m_passenger_flow_info3->m_label_line2_1->text(); //ID

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.param_file_exist;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.guide = (m_passenger_flow_info3->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//辅助线 0:不选择 1:选择

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_show_mode = m_passenger_flow_info3->m_combobox_line5->currentIndex();//计数显示模式  0:标准  1:进站
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_rule = m_passenger_flow_info3->m_combobox_line6->currentIndex();//计数规则  //0:上进下出  1:下进上出
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.noise_filter = m_passenger_flow_info3->m_lineedit_line7->text().toInt();//噪声过滤
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.sensitivity = m_passenger_flow_info3->m_lineedit_line8->text().toDouble();//灵敏度
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_type = m_passenger_flow_info3->m_lineedit_line9->text().toInt();//计数类型
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.install_height = m_passenger_flow_info3->m_lineedit_line10->text().toInt();//安装高度
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.filter_height = m_passenger_flow_info3->m_lineedit_line11->text().toInt();//过滤高度
    //CAM3-1通道设置-检测区域
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line1 = m_CCheckRegionInfo3->m_lineedit_line1->text().toInt();//上计数线1
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line2 = m_CCheckRegionInfo3->m_lineedit_line2->text().toInt();//下计数线2
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line3 = m_CCheckRegionInfo3->m_lineedit_line3->text().toInt();//左边界线3
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line4 = m_CCheckRegionInfo3->m_lineedit_line4->text().toInt();//右边界线4

    //CAM4-1通道设置
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_enable = (m_passenger_flow_info4->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使能  0:不选择 1:选择

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.is_connect;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_id = m_passenger_flow_info4->m_label_line2_1->text(); //ID

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.param_file_exist;

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.guide = (m_passenger_flow_info4->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//辅助线 0:不选择 1:选择

    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_show_mode = m_passenger_flow_info4->m_combobox_line5->currentIndex();//计数显示模式  0:标准  1:进站
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_rule = m_passenger_flow_info4->m_combobox_line6->currentIndex();//计数规则  //0:上进下出  1:下进上出
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.noise_filter = m_passenger_flow_info4->m_lineedit_line7->text().toInt();//噪声过滤
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.sensitivity = m_passenger_flow_info4->m_lineedit_line8->text().toDouble();//灵敏度
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_type = m_passenger_flow_info4->m_lineedit_line9->text().toInt();//计数类型
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.install_height = m_passenger_flow_info4->m_lineedit_line10->text().toInt();//安装高度
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.filter_height = m_passenger_flow_info4->m_lineedit_line11->text().toInt();//过滤高度
    //CAM3-1通道设置-检测区域
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line1 = m_CCheckRegionInfo4->m_lineedit_line1->text().toInt();//上计数线1
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line2 = m_CCheckRegionInfo4->m_lineedit_line2->text().toInt();//下计数线2
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line3 = m_CCheckRegionInfo4->m_lineedit_line3->text().toInt();//左边界线3
    CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line4 = m_CCheckRegionInfo4->m_lineedit_line4->text().toInt();//右边界线4


    //高级设置
    //高级设置－计数使能

    CCommonData::getInstance()->m_CSystemPassengerFlow.real_dynamic_statistics = (m_checkbox_count_enable_line2->checkState() == Qt::Checked)? 1:0;//实时动态统计  0:不选择 1:选择
    CCommonData::getInstance()->m_CSystemPassengerFlow.open_door_statistics = (m_checkbox_count_enable_line3->checkState() == Qt::Checked)? 1:0;//开门统计  0:不选择 1:选择
    CCommonData::getInstance()->m_CSystemPassengerFlow.in_station_statistics = (m_checkbox_count_enable_line4->checkState() == Qt::Checked)? 1:0;//进站统计  0:不选择 1:选择
    CCommonData::getInstance()->m_CSystemPassengerFlow.out_station_statistics = (m_checkbox_count_enable_line5->checkState() == Qt::Checked)? 1:0;//站外统计  0:不选择 1:选择
    CCommonData::getInstance()->m_CSystemPassengerFlow.speed_statistics_enable = (m_checkbox_count_enable_line6->checkState() == Qt::Checked)? 1:0;//速度统计使能  0:不选择 1:选择
    CCommonData::getInstance()->m_CSystemPassengerFlow.speed_statistics_value = m_lineedit_count_enable_line6->text().toInt();//速度统计值
    CCommonData::getInstance()->m_CSystemPassengerFlow.single_loop_error_calibration = m_lineedit_count_enable_line7->text().toInt();//单圈误差校准
    CCommonData::getInstance()->m_CSystemPassengerFlow.close_delay = m_lineedit_count_enable_line8->text().toInt();//关门延时
    CCommonData::getInstance()->m_CSystemPassengerFlow.time_upload_time = m_lineedit_count_enable_line9->text().toInt();//定时上报时间

    CCommonData::getInstance()->m_CSystemPassengerFlow.image_mosaic = (m_passenger_flow_info5->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//图像拼接　0:不选择 1:选择

    CCommonData::getInstance()->m_CSystemPassengerFlow.interval_width = m_passenger_flow_info5->m_lineedit_line3->text().toInt();//间隔宽度　CM



}
void CPassengerFlowWidget::save_data()
{
    CSystemPassengerFlow info;

    //CAM1-1通道设置
    info.m_cam1_1_set.m_enable = (m_passenger_flow_info1->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使能  0:不选择 1:选择
    //info.m_cam1_1_set.is_connect;
    info.m_cam1_1_set.m_id = m_passenger_flow_info1->m_label_line2_1->text(); //ID
    //info.m_cam1_1_set.param_file_exist;
    info.m_cam1_1_set.guide = (m_passenger_flow_info1->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;

    info.m_cam1_1_set.count_show_mode = m_passenger_flow_info1->m_combobox_line5->currentIndex();//计数显示模式  0:标准  1:进站
    info.m_cam1_1_set.count_rule = m_passenger_flow_info1->m_combobox_line6->currentIndex();//计数规则  //0:上进下出  1:下进上出
    info.m_cam1_1_set.noise_filter = m_passenger_flow_info1->m_lineedit_line7->text().toInt();//噪声过滤
    info.m_cam1_1_set.sensitivity = m_passenger_flow_info1->m_lineedit_line8->text().toDouble();//灵敏度
    info.m_cam1_1_set.count_type = m_passenger_flow_info1->m_lineedit_line9->text().toInt();//计数类型
    info.m_cam1_1_set.install_height = m_passenger_flow_info1->m_lineedit_line10->text().toInt();//安装高度
    info.m_cam1_1_set.filter_height = m_passenger_flow_info1->m_lineedit_line11->text().toInt();//过滤高度
    //CAM1-1通道设置-检测区域
    info.m_cam1_1_set.count_line1 = m_CCheckRegionInfo1->m_lineedit_line1->text().toInt();//上计数线1
    info.m_cam1_1_set.count_line2 = m_CCheckRegionInfo1->m_lineedit_line2->text().toInt();//下计数线2
    info.m_cam1_1_set.count_line3 = m_CCheckRegionInfo1->m_lineedit_line3->text().toInt();//左边界线3
    info.m_cam1_1_set.count_line4 = m_CCheckRegionInfo1->m_lineedit_line4->text().toInt();//右边界线4

    //CAM2-1通道设置
    info.m_cam2_1_set.m_enable = (m_passenger_flow_info2->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使能  0:不选择 1:选择
    //info.m_cam2_1_set.is_connect;
    info.m_cam2_1_set.m_id = m_passenger_flow_info2->m_label_line2_1->text(); //ID
    //info.m_cam2_1_set.param_file_exist;
    info.m_cam2_1_set.guide = (m_passenger_flow_info2->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//辅助线 0:不选择 1:选择
    info.m_cam2_1_set.count_show_mode = m_passenger_flow_info2->m_combobox_line5->currentIndex();//计数显示模式  0:标准  1:进站
    info.m_cam2_1_set.count_rule = m_passenger_flow_info2->m_combobox_line6->currentIndex();//计数规则  //0:上进下出  1:下进上出
    info.m_cam2_1_set.noise_filter = m_passenger_flow_info2->m_lineedit_line7->text().toInt();//噪声过滤
    info.m_cam2_1_set.sensitivity = m_passenger_flow_info2->m_lineedit_line8->text().toDouble();//灵敏度
    info.m_cam2_1_set.count_type = m_passenger_flow_info2->m_lineedit_line9->text().toInt();//计数类型
    info.m_cam2_1_set.install_height = m_passenger_flow_info2->m_lineedit_line10->text().toInt();//安装高度
    info.m_cam2_1_set.filter_height = m_passenger_flow_info2->m_lineedit_line11->text().toInt();//过滤高度
    //CAM2-1通道设置-检测区域
    info.m_cam2_1_set.count_line1 = m_CCheckRegionInfo2->m_lineedit_line1->text().toInt();//上计数线1
    info.m_cam2_1_set.count_line2 = m_CCheckRegionInfo2->m_lineedit_line2->text().toInt();//下计数线2
    info.m_cam2_1_set.count_line3 = m_CCheckRegionInfo2->m_lineedit_line3->text().toInt();//左边界线3
    info.m_cam2_1_set.count_line4 = m_CCheckRegionInfo2->m_lineedit_line4->text().toInt();//右边界线4

    //CAM3-1通道设置
    info.m_cam3_1_set.m_enable = (m_passenger_flow_info3->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使能  0:不选择 1:选择
    //info.m_cam3_1_set.is_connect;
    info.m_cam3_1_set.m_id = m_passenger_flow_info3->m_label_line2_1->text(); //ID
    //info.m_cam3_1_set.param_file_exist;
    info.m_cam3_1_set.guide = (m_passenger_flow_info3->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//辅助线 0:不选择 1:选择
    info.m_cam3_1_set.count_show_mode = m_passenger_flow_info3->m_combobox_line5->currentIndex();//计数显示模式  0:标准  1:进站
    info.m_cam3_1_set.count_rule = m_passenger_flow_info3->m_combobox_line6->currentIndex();//计数规则  //0:上进下出  1:下进上出
    info.m_cam3_1_set.noise_filter = m_passenger_flow_info3->m_lineedit_line7->text().toInt();//噪声过滤
    info.m_cam3_1_set.sensitivity = m_passenger_flow_info3->m_lineedit_line8->text().toDouble();//灵敏度
    info.m_cam3_1_set.count_type = m_passenger_flow_info3->m_lineedit_line9->text().toInt();//计数类型
    info.m_cam3_1_set.install_height = m_passenger_flow_info3->m_lineedit_line10->text().toInt();//安装高度
    info.m_cam3_1_set.filter_height = m_passenger_flow_info3->m_lineedit_line11->text().toInt();//过滤高度
    //CAM3-1通道设置-检测区域
    info.m_cam3_1_set.count_line1 = m_CCheckRegionInfo3->m_lineedit_line1->text().toInt();//上计数线1
    info.m_cam3_1_set.count_line2 = m_CCheckRegionInfo3->m_lineedit_line2->text().toInt();//下计数线2
    info.m_cam3_1_set.count_line3 = m_CCheckRegionInfo3->m_lineedit_line3->text().toInt();//左边界线3
    info.m_cam3_1_set.count_line4 = m_CCheckRegionInfo3->m_lineedit_line4->text().toInt();//右边界线4

    //CAM4-1通道设置
    info.m_cam4_1_set.m_enable = (m_passenger_flow_info4->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使能  0:不选择 1:选择
    //info.m_cam4_1_set.is_connect;
    info.m_cam4_1_set.m_id = m_passenger_flow_info4->m_label_line2_1->text(); //ID
    //info.m_cam4_1_set.param_file_exist;
    info.m_cam4_1_set.guide = (m_passenger_flow_info4->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//辅助线 0:不选择 1:选择
    info.m_cam4_1_set.count_show_mode = m_passenger_flow_info4->m_combobox_line5->currentIndex();//计数显示模式  0:标准  1:进站
    info.m_cam4_1_set.count_rule = m_passenger_flow_info4->m_combobox_line6->currentIndex();//计数规则  //0:上进下出  1:下进上出
    info.m_cam4_1_set.noise_filter = m_passenger_flow_info4->m_lineedit_line7->text().toInt();//噪声过滤
    info.m_cam4_1_set.sensitivity = m_passenger_flow_info4->m_lineedit_line8->text().toDouble();//灵敏度
    info.m_cam4_1_set.count_type = m_passenger_flow_info4->m_lineedit_line9->text().toInt();//计数类型
    info.m_cam4_1_set.install_height = m_passenger_flow_info4->m_lineedit_line10->text().toInt();//安装高度
    info.m_cam4_1_set.filter_height = m_passenger_flow_info4->m_lineedit_line11->text().toInt();//过滤高度
    //CAM4-1通道设置-检测区域
    info.m_cam4_1_set.count_line1 = m_CCheckRegionInfo4->m_lineedit_line1->text().toInt();//上计数线1
    info.m_cam4_1_set.count_line2 = m_CCheckRegionInfo4->m_lineedit_line2->text().toInt();//下计数线2
    info.m_cam4_1_set.count_line3 = m_CCheckRegionInfo4->m_lineedit_line3->text().toInt();//左边界线3
    info.m_cam4_1_set.count_line4 = m_CCheckRegionInfo4->m_lineedit_line4->text().toInt();//右边界线4




    //高级设置
    //高级设置－计数使能
    info.real_dynamic_statistics = (m_checkbox_count_enable_line2->checkState() == Qt::Checked)? 1:0;//实时动态统计  0:不选择 1:选择
    info.open_door_statistics = (m_checkbox_count_enable_line3->checkState() == Qt::Checked)? 1:0;//开门统计  0:不选择 1:选择
    info.in_station_statistics = (m_checkbox_count_enable_line4->checkState() == Qt::Checked)? 1:0;//进站统计  0:不选择 1:选择
    info.out_station_statistics = (m_checkbox_count_enable_line5->checkState() == Qt::Checked)? 1:0;//站外统计  0:不选择 1:选择
    info.speed_statistics_enable = (m_checkbox_count_enable_line6->checkState() == Qt::Checked)? 1:0;//速度统计使能  0:不选择 1:选择
    info.speed_statistics_value = m_lineedit_count_enable_line6->text().toInt();//速度统计值
    info.single_loop_error_calibration = m_lineedit_count_enable_line7->text().toInt();//单圈误差校准
    info.close_delay = m_lineedit_count_enable_line8->text().toInt();//关门延时
    info.time_upload_time = m_lineedit_count_enable_line9->text().toInt();//定时上报时间
    info.image_mosaic = (m_passenger_flow_info5->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//图像拼接　0:不选择 1:选择
    info.interval_width = m_passenger_flow_info5->m_lineedit_line3->text().toInt();//间隔宽度　CM



    if(info.m_cam1_1_set.m_enable != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_enable ||
        info.m_cam1_1_set.m_id != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.m_id ||
        info.m_cam1_1_set.guide != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.guide ||
        info.m_cam1_1_set.count_show_mode != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_show_mode ||
        info.m_cam1_1_set.count_rule != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_rule ||
        info.m_cam1_1_set.noise_filter != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.noise_filter ||
        info.m_cam1_1_set.sensitivity != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.sensitivity ||
        info.m_cam1_1_set.count_type != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_type ||
        info.m_cam1_1_set.install_height != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.install_height ||
        info.m_cam1_1_set.filter_height != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.filter_height ||
        info.m_cam1_1_set.count_line1 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line1 ||
        info.m_cam1_1_set.count_line2 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line2 ||
        info.m_cam1_1_set.count_line3 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line3 ||
        info.m_cam1_1_set.count_line4 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam1_1_set.count_line4 ||
        info.m_cam2_1_set.m_enable != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_enable ||
        info.m_cam2_1_set.m_id != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.m_id ||
        info.m_cam2_1_set.guide != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.guide ||
        info.m_cam2_1_set.count_show_mode != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_show_mode ||
        info.m_cam2_1_set.count_rule != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_rule ||
        info.m_cam2_1_set.noise_filter != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.noise_filter ||
        info.m_cam2_1_set.sensitivity != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.sensitivity ||
        info.m_cam2_1_set.count_type != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_type ||
        info.m_cam2_1_set.install_height != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.install_height ||
        info.m_cam2_1_set.filter_height != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.filter_height ||
        info.m_cam2_1_set.count_line1 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line1 ||
        info.m_cam2_1_set.count_line2 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line2 ||
        info.m_cam2_1_set.count_line3 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line3 ||
        info.m_cam2_1_set.count_line4 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam2_1_set.count_line4 ||
        #if 1
        //CAM3-1通道设置
        info.m_cam3_1_set.m_enable != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_enable ||//使能  0:不选择 1:选择
        info.m_cam3_1_set.m_id !=  CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.m_id ||//ID
        info.m_cam3_1_set.guide != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.guide ||//辅助线 0:不选择 1:选择
        info.m_cam3_1_set.count_show_mode != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_show_mode ||//计数显示模式  0:标准  1:进站
        info.m_cam3_1_set.count_rule != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_rule ||//计数规则  //0:上进下出  1:下进上出
        info.m_cam3_1_set.noise_filter != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.noise_filter ||//噪声过滤
        info.m_cam3_1_set.sensitivity != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.sensitivity ||//灵敏度
        info.m_cam3_1_set.count_type != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_type ||//计数类型
        info.m_cam3_1_set.install_height != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.install_height ||//安装高度
        info.m_cam3_1_set.filter_height != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.filter_height ||//过滤高度
        //CAM3-1通道设置-检测区域
        info.m_cam3_1_set.count_line1 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line1 ||//上计数线1
        info.m_cam3_1_set.count_line2 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line2 ||//下计数线2
        info.m_cam3_1_set.count_line3 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line3 ||//左边界线3
        info.m_cam3_1_set.count_line4 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam3_1_set.count_line4 ||//右边界线4
        //CAM4-1通道设置
        info.m_cam4_1_set.m_enable != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_enable ||//使能  0:不选择 1:选择
        info.m_cam4_1_set.m_id !=  CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.m_id ||//ID
        info.m_cam4_1_set.guide != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.guide ||//辅助线 0:不选择 1:选择
        info.m_cam4_1_set.count_show_mode != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_show_mode ||//计数显示模式  0:标准  1:进站
        info.m_cam4_1_set.count_rule != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_rule ||//计数规则  //0:上进下出  1:下进上出
        info.m_cam4_1_set.noise_filter != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.noise_filter ||//噪声过滤
        info.m_cam4_1_set.sensitivity != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.sensitivity ||//灵敏度
        info.m_cam4_1_set.count_type != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_type ||//计数类型
        info.m_cam4_1_set.install_height != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.install_height ||//安装高度
        info.m_cam4_1_set.filter_height != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.filter_height ||//过滤高度
        //CAM4-1通道设置-检测区域
        info.m_cam4_1_set.count_line1 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line1 ||//上计数线1
        info.m_cam4_1_set.count_line2 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line2 ||//下计数线2
        info.m_cam4_1_set.count_line3 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line3 ||//左边界线3
        info.m_cam4_1_set.count_line4 != CCommonData::getInstance()->m_CSystemPassengerFlow.m_cam4_1_set.count_line4 ||//右边界线4
        #endif
        info.real_dynamic_statistics != CCommonData::getInstance()->m_CSystemPassengerFlow.real_dynamic_statistics ||
        info.open_door_statistics != CCommonData::getInstance()->m_CSystemPassengerFlow.open_door_statistics ||
        info.in_station_statistics != CCommonData::getInstance()->m_CSystemPassengerFlow.in_station_statistics ||
        info.out_station_statistics != CCommonData::getInstance()->m_CSystemPassengerFlow.out_station_statistics ||
        info.speed_statistics_enable != CCommonData::getInstance()->m_CSystemPassengerFlow.speed_statistics_enable ||
        info.speed_statistics_value != CCommonData::getInstance()->m_CSystemPassengerFlow.speed_statistics_value ||
        info.single_loop_error_calibration != CCommonData::getInstance()->m_CSystemPassengerFlow.single_loop_error_calibration ||
        info.close_delay != CCommonData::getInstance()->m_CSystemPassengerFlow.close_delay ||
        info.time_upload_time != CCommonData::getInstance()->m_CSystemPassengerFlow.time_upload_time ||
        info.image_mosaic != CCommonData::getInstance()->m_CSystemPassengerFlow.image_mosaic ||
        info.interval_width != CCommonData::getInstance()->m_CSystemPassengerFlow.interval_width
    )
    {
       //  slot_passenger_flow_widget_event(1,&CCommonData::getInstance()->m_CSystemPassengerFlow);
        if(CCommonData::getInstance()->messagebox_data_change())
        {
            read_data();
            CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumSave);
        }
        else
        {

            slot_passenger_flow_widget_event(1,&CCommonData::getInstance()->m_CSystemPassengerFlow);
        }

    }


}

void CPassengerFlowWidget::slot_passenger_flow_widget_event(char _type,void *_param)
{


    switch(_type)
    {
        case 1:
        {
            CSystemPassengerFlow *passenger_flow = (CSystemPassengerFlow *)_param;

            //CAM1-1通道设置
            if(0 == passenger_flow->m_cam1_1_set.m_enable) //使能  0:不选择 1:选择
                m_passenger_flow_info1->m_checkbox_line1->setChecked(false);
            else
                m_passenger_flow_info1->m_checkbox_line1->setChecked(true);

            if(1 == passenger_flow->m_cam1_1_set.is_connect) // 1:连接  2:未连接
                m_passenger_flow_info1->m_label_line1_1->setText(STR_CONNECT);
            else
                m_passenger_flow_info1->m_label_line1_1->setText(STR_NO_CONNECT);

            m_passenger_flow_info1->m_label_line2_1->setText(passenger_flow->m_cam1_1_set.m_id); //ID

            if(1 == passenger_flow->m_cam1_1_set.param_file_exist) //1:存在　　2:不存在
                m_passenger_flow_info1->m_label_line3_1->setText(STR_EXIST);
            else
                m_passenger_flow_info1->m_label_line3_1->setText(STR_NO_EXIST);

            if(0 == passenger_flow->m_cam1_1_set.guide)//辅助线 0:不选择 1:选择
                m_passenger_flow_info1->m_checkbox_line4->setChecked(false);
            else
                m_passenger_flow_info1->m_checkbox_line4->setChecked(true);

            m_passenger_flow_info1->m_combobox_line5->setCurrentIndex(passenger_flow->m_cam1_1_set.count_show_mode);//计数显示模式  0:标准  1:进站
            m_passenger_flow_info1->m_combobox_line6->setCurrentIndex(passenger_flow->m_cam1_1_set.count_rule);//计数规则  //0:上进下出  1:下进上出
            m_passenger_flow_info1->m_lineedit_line7->setText(QString::number(passenger_flow->m_cam1_1_set.noise_filter));//噪声过滤
            m_passenger_flow_info1->m_lineedit_line8->setText(QString::number(passenger_flow->m_cam1_1_set.sensitivity));//灵敏度
            m_passenger_flow_info1->m_lineedit_line9->setText(QString::number(passenger_flow->m_cam1_1_set.count_type));//计数类型
            m_passenger_flow_info1->m_lineedit_line10->setText(QString::number(passenger_flow->m_cam1_1_set.install_height));//安装高度
            m_passenger_flow_info1->m_lineedit_line11->setText(QString::number(passenger_flow->m_cam1_1_set.filter_height));//过滤高度
            //CAM1-1通道设置-检测区域

            m_CCheckRegionInfo1->m_lineedit_line1->setText(QString::number(passenger_flow->m_cam1_1_set.count_line1));//上计数线1
            m_CCheckRegionInfo1->m_lineedit_line2->setText(QString::number(passenger_flow->m_cam1_1_set.count_line2));//下计数线2
            m_CCheckRegionInfo1->m_lineedit_line3->setText(QString::number(passenger_flow->m_cam1_1_set.count_line3));//左边界线3
            m_CCheckRegionInfo1->m_lineedit_line4->setText(QString::number(passenger_flow->m_cam1_1_set.count_line4));//右边界线4

            //CAM2-1通道设置
           if(0 == passenger_flow->m_cam2_1_set.m_enable) //使能  0:不选择 1:选择
                m_passenger_flow_info2->m_checkbox_line1->setChecked(false);
            else
                m_passenger_flow_info2->m_checkbox_line1->setChecked(true);

            if(1 == passenger_flow->m_cam2_1_set.is_connect) // 1:连接  2:未连接
                m_passenger_flow_info2->m_label_line1_1->setText(STR_CONNECT);
            else
                m_passenger_flow_info2->m_label_line1_1->setText(STR_NO_CONNECT);

            m_passenger_flow_info2->m_label_line2_1->setText(passenger_flow->m_cam2_1_set.m_id); //ID

            if(1 == passenger_flow->m_cam2_1_set.param_file_exist) //1:存在　　2:不存在
                m_passenger_flow_info2->m_label_line3_1->setText(STR_EXIST);
            else
                m_passenger_flow_info2->m_label_line3_1->setText(STR_NO_EXIST);

            if(0 == passenger_flow->m_cam2_1_set.guide)//辅助线 0:不选择 1:选择
                m_passenger_flow_info2->m_checkbox_line4->setChecked(false);
            else
                m_passenger_flow_info2->m_checkbox_line4->setChecked(true);

            m_passenger_flow_info2->m_combobox_line5->setCurrentIndex(passenger_flow->m_cam2_1_set.count_show_mode);//计数显示模式  0:标准  1:进站
            m_passenger_flow_info2->m_combobox_line6->setCurrentIndex(passenger_flow->m_cam2_1_set.count_rule);//计数规则  //0:上进下出  1:下进上出
            m_passenger_flow_info2->m_lineedit_line7->setText(QString::number(passenger_flow->m_cam2_1_set.noise_filter));//噪声过滤
            m_passenger_flow_info2->m_lineedit_line8->setText(QString::number(passenger_flow->m_cam2_1_set.sensitivity));//灵敏度
            m_passenger_flow_info2->m_lineedit_line9->setText(QString::number(passenger_flow->m_cam2_1_set.count_type));//计数类型
            m_passenger_flow_info2->m_lineedit_line10->setText(QString::number(passenger_flow->m_cam2_1_set.install_height));//安装高度
            m_passenger_flow_info2->m_lineedit_line11->setText(QString::number(passenger_flow->m_cam2_1_set.filter_height));//过滤高度
            //CAM2-1通道设置-检测区域
            m_CCheckRegionInfo2->m_lineedit_line1->setText(QString::number(passenger_flow->m_cam2_1_set.count_line1));//上计数线1
            m_CCheckRegionInfo2->m_lineedit_line2->setText(QString::number(passenger_flow->m_cam2_1_set.count_line2));//下计数线2
            m_CCheckRegionInfo2->m_lineedit_line3->setText(QString::number(passenger_flow->m_cam2_1_set.count_line3));//左边界线3
            m_CCheckRegionInfo2->m_lineedit_line4->setText(QString::number(passenger_flow->m_cam2_1_set.count_line4));//右边界线4

           //CAM3-1通道设置
           if(0 == passenger_flow->m_cam3_1_set.m_enable) //使能  0:不选择 1:选择
                m_passenger_flow_info3->m_checkbox_line1->setChecked(false);
            else
                m_passenger_flow_info3->m_checkbox_line1->setChecked(true);

            if(1 == passenger_flow->m_cam3_1_set.is_connect) // 1:连接  2:未连接
                m_passenger_flow_info3->m_label_line1_1->setText(STR_CONNECT);
            else
                m_passenger_flow_info3->m_label_line1_1->setText(STR_NO_CONNECT);

            m_passenger_flow_info3->m_label_line2_1->setText(passenger_flow->m_cam3_1_set.m_id); //ID

            if(1 == passenger_flow->m_cam3_1_set.param_file_exist) //1:存在　　2:不存在
                m_passenger_flow_info3->m_label_line3_1->setText(STR_EXIST);
            else
                m_passenger_flow_info3->m_label_line3_1->setText(STR_NO_EXIST);

            if(0 == passenger_flow->m_cam3_1_set.guide)//辅助线 0:不选择 1:选择
                m_passenger_flow_info3->m_checkbox_line4->setChecked(false);
            else
                m_passenger_flow_info3->m_checkbox_line4->setChecked(true);

            m_passenger_flow_info3->m_combobox_line5->setCurrentIndex(passenger_flow->m_cam3_1_set.count_show_mode);//计数显示模式  0:标准  1:进站
            m_passenger_flow_info3->m_combobox_line6->setCurrentIndex(passenger_flow->m_cam3_1_set.count_rule);//计数规则  //0:上进下出  1:下进上出
            m_passenger_flow_info3->m_lineedit_line7->setText(QString::number(passenger_flow->m_cam3_1_set.noise_filter));//噪声过滤
            m_passenger_flow_info3->m_lineedit_line8->setText(QString::number(passenger_flow->m_cam3_1_set.sensitivity));//灵敏度
            m_passenger_flow_info3->m_lineedit_line9->setText(QString::number(passenger_flow->m_cam3_1_set.count_type));//计数类型
            m_passenger_flow_info3->m_lineedit_line10->setText(QString::number(passenger_flow->m_cam3_1_set.install_height));//安装高度
            m_passenger_flow_info3->m_lineedit_line11->setText(QString::number(passenger_flow->m_cam3_1_set.filter_height));//过滤高度
            //CAM3-1通道设置-检测区域
            m_CCheckRegionInfo3->m_lineedit_line1->setText(QString::number(passenger_flow->m_cam3_1_set.count_line1));//上计数线1
            m_CCheckRegionInfo3->m_lineedit_line2->setText(QString::number(passenger_flow->m_cam3_1_set.count_line2));//下计数线2
            m_CCheckRegionInfo3->m_lineedit_line3->setText(QString::number(passenger_flow->m_cam3_1_set.count_line3));//左边界线3
            m_CCheckRegionInfo3->m_lineedit_line4->setText(QString::number(passenger_flow->m_cam3_1_set.count_line4));//右边界线4

           //CAM4-1通道设置
           if(0 == passenger_flow->m_cam4_1_set.m_enable) //使能  0:不选择 1:选择
                m_passenger_flow_info4->m_checkbox_line1->setChecked(false);
            else
                m_passenger_flow_info4->m_checkbox_line1->setChecked(true);

            if(1 == passenger_flow->m_cam4_1_set.is_connect) // 1:连接  2:未连接
                m_passenger_flow_info4->m_label_line1_1->setText(STR_CONNECT);
            else
                m_passenger_flow_info4->m_label_line1_1->setText(STR_NO_CONNECT);

            m_passenger_flow_info4->m_label_line2_1->setText(passenger_flow->m_cam4_1_set.m_id); //ID

            if(1 == passenger_flow->m_cam4_1_set.param_file_exist) //1:存在　　2:不存在
                m_passenger_flow_info4->m_label_line3_1->setText(STR_EXIST);
            else
                m_passenger_flow_info4->m_label_line3_1->setText(STR_NO_EXIST);

            if(0 == passenger_flow->m_cam4_1_set.guide)//辅助线 0:不选择 1:选择
                m_passenger_flow_info4->m_checkbox_line4->setChecked(false);
            else
                m_passenger_flow_info4->m_checkbox_line4->setChecked(true);

            m_passenger_flow_info4->m_combobox_line5->setCurrentIndex(passenger_flow->m_cam4_1_set.count_show_mode);//计数显示模式  0:标准  1:进站
            m_passenger_flow_info4->m_combobox_line6->setCurrentIndex(passenger_flow->m_cam4_1_set.count_rule);//计数规则  //0:上进下出  1:下进上出
            m_passenger_flow_info4->m_lineedit_line7->setText(QString::number(passenger_flow->m_cam4_1_set.noise_filter));//噪声过滤
            m_passenger_flow_info4->m_lineedit_line8->setText(QString::number(passenger_flow->m_cam4_1_set.sensitivity));//灵敏度
            m_passenger_flow_info4->m_lineedit_line9->setText(QString::number(passenger_flow->m_cam4_1_set.count_type));//计数类型
            m_passenger_flow_info4->m_lineedit_line10->setText(QString::number(passenger_flow->m_cam4_1_set.install_height));//安装高度
            m_passenger_flow_info4->m_lineedit_line11->setText(QString::number(passenger_flow->m_cam4_1_set.filter_height));//过滤高度
            //CAM4-1通道设置-检测区域
            m_CCheckRegionInfo4->m_lineedit_line1->setText(QString::number(passenger_flow->m_cam4_1_set.count_line1));//上计数线1
            m_CCheckRegionInfo4->m_lineedit_line2->setText(QString::number(passenger_flow->m_cam4_1_set.count_line2));//下计数线2
            m_CCheckRegionInfo4->m_lineedit_line3->setText(QString::number(passenger_flow->m_cam4_1_set.count_line3));//左边界线3
            m_CCheckRegionInfo4->m_lineedit_line4->setText(QString::number(passenger_flow->m_cam4_1_set.count_line4));//右边界线4



            //高级设置
            //高级设置－计数使能
            if(0 == passenger_flow->real_dynamic_statistics)//实时动态统计  0:不选择 1:选择
                m_checkbox_count_enable_line2->setChecked(false);
            else
                m_checkbox_count_enable_line2->setChecked(true);

            if(0 == passenger_flow->open_door_statistics)//开门统计  0:不选择 1:选择
                m_checkbox_count_enable_line3->setChecked(false);
            else
                m_checkbox_count_enable_line3->setChecked(true);

            if(0 == passenger_flow->in_station_statistics)//进站统计  0:不选择 1:选择
                m_checkbox_count_enable_line4->setChecked(false);
            else
                m_checkbox_count_enable_line4->setChecked(true);

            if(0 == passenger_flow->out_station_statistics)//站外统计  0:不选择 1:选择
                m_checkbox_count_enable_line5->setChecked(false);
            else
                m_checkbox_count_enable_line5->setChecked(true);

            if(0 == passenger_flow->speed_statistics_enable) //速度统计使能  0:不选择 1:选择
                m_checkbox_count_enable_line6->setChecked(false);
            else
                m_checkbox_count_enable_line6->setChecked(true);

            m_lineedit_count_enable_line6->setText(QString::number(passenger_flow->speed_statistics_value));//速度统计值

            m_lineedit_count_enable_line7->setText(QString::number(passenger_flow->single_loop_error_calibration));//单圈误差校准
            m_lineedit_count_enable_line8->setText(QString::number(passenger_flow->close_delay));//关门延时
            m_lineedit_count_enable_line9->setText(QString::number(passenger_flow->time_upload_time));//定时上报时间

            if(0 == passenger_flow->image_mosaic)//图像拼接　0:不选择 1:选择
                m_passenger_flow_info5->m_checkbox_line2->setChecked(false);
            else
                m_passenger_flow_info5->m_checkbox_line2->setChecked(true);

            m_passenger_flow_info5->m_lineedit_line3->setText(QString::number(passenger_flow->interval_width));//间隔宽度　CM

        }break;
        case 2://同步结果
        {

            CPassengerResult *pPassengerResult = (CPassengerResult *)_param;
            bool progress_dialog_show = false;

            if(CProgressBox::getInstance()->isVisible())
                progress_dialog_show = true;

            if(progress_dialog_show)
            {
                m_passenger_flow_info1->set_progress("",false);
                if(1 == pPassengerResult->m_type)
                {
                    CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,100);
                    //pPassengerResult->m_return_result;
                }
                else if(2 == pPassengerResult->m_type)
                {
                    CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessFail,0);//fail
                }
                else if(3 == pPassengerResult->m_type)
                {
                    CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,pPassengerResult->m_value);
                }
                  else if(1)
                {

                    CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNull,0);
                }
                else if(5 == pPassengerResult->m_type)
                {
                    CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessUmount,0);
                }
            }
            else
            {
                if(1 == pPassengerResult->m_type)
                {
                    if(1 == pPassengerResult->m_cam_select)
                        m_passenger_flow_info1->set_progress("",false);
                    else if(2 == pPassengerResult->m_cam_select)
                        m_passenger_flow_info2->set_progress("",false);
                }
                else if(2 == pPassengerResult->m_type)
                {
                    if(1 == pPassengerResult->m_cam_select)
                        m_passenger_flow_info1->set_progress(tr("同步失败"),true);
                    else if(2 == pPassengerResult->m_cam_select)
                        m_passenger_flow_info2->set_progress(tr("同步失败"),true);
                }
                else if(3 == pPassengerResult->m_type)
                {
                    if(1 == pPassengerResult->m_cam_select)
                        m_passenger_flow_info1->set_progress(QString("(%1%)").arg(pPassengerResult->m_value),true);
                    else if(2 == pPassengerResult->m_cam_select)
                        m_passenger_flow_info2->set_progress(QString("(%1%)").arg(pPassengerResult->m_value),true);
                }
                 else if(4 == pPassengerResult->m_type)
                {
                    if(1 == pPassengerResult->m_cam_select)
                        m_passenger_flow_info1->set_progress(tr("没有匹配参数"),true);
                    else if(2 == pPassengerResult->m_cam_select)
                        m_passenger_flow_info2->set_progress(tr("没有匹配参数"),true);
                }

            }


        }break;
        case 3://保存背景结果
        {
            CPassengerResult *pPassengerResult = (CPassengerResult *)_param;

            if(1 == pPassengerResult->m_type)
            {

                CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,100);
                //pPassengerResult->m_return_result;
            }
            else if(2 == pPassengerResult->m_type)
            {
                CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessFail,0);
            }
            else if(3 == pPassengerResult->m_type)
            {

                CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,pPassengerResult->m_value);
            }
            else if(4 == pPassengerResult->m_type)
            {
                CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNull,0);
            }
            else if(5 == pPassengerResult->m_type)
            {
                CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessUmount,0);
            }

        }break;
        case 4:
        {
            CSystemPassengerFlow *passenger_flow = (CSystemPassengerFlow *)_param;

            if(1 == passenger_flow->m_cam1_1_set.is_connect) // 1:连接  2:未连接
                m_passenger_flow_info1->m_label_line1_1->setText(STR_CONNECT);
            else
                m_passenger_flow_info1->m_label_line1_1->setText(STR_NO_CONNECT);

            m_passenger_flow_info1->m_label_line2_1->setText(passenger_flow->m_cam1_1_set.m_id); //ID

            if(1 == passenger_flow->m_cam2_1_set.is_connect) // 1:连接  2:未连接
                m_passenger_flow_info2->m_label_line1_1->setText(STR_CONNECT);
            else
                m_passenger_flow_info2->m_label_line1_1->setText(STR_NO_CONNECT);

            m_passenger_flow_info2->m_label_line2_1->setText(passenger_flow->m_cam2_1_set.m_id); //ID

            if(1 == passenger_flow->m_cam3_1_set.is_connect) // 1:连接  2:未连接
                m_passenger_flow_info3->m_label_line1_1->setText(STR_CONNECT);
            else
                m_passenger_flow_info3->m_label_line1_1->setText(STR_NO_CONNECT);

            m_passenger_flow_info3->m_label_line2_1->setText(passenger_flow->m_cam3_1_set.m_id); //ID

             if(1 == passenger_flow->m_cam4_1_set.is_connect) // 1:连接  2:未连接
                m_passenger_flow_info4->m_label_line1_1->setText(STR_CONNECT);
            else
                m_passenger_flow_info4->m_label_line1_1->setText(STR_NO_CONNECT);

            m_passenger_flow_info4->m_label_line2_1->setText(passenger_flow->m_cam4_1_set.m_id); //ID


        }break;
        case 5:
        {
            CSystemPassengerFlow *passenger_flow = (CSystemPassengerFlow *)_param;

            if(1 == passenger_flow->m_cam1_1_set.param_file_exist) //1:存在　　2:不存在
            {
                if(!passenger_flow->m_cam1_1_set.param_file_name.isEmpty())
                    m_passenger_flow_info1->m_label_line3_1->setText(passenger_flow->m_cam1_1_set.param_file_name);
                else
                    m_passenger_flow_info1->m_label_line3_1->setText(STR_EXIST);
            }
            else
                m_passenger_flow_info1->m_label_line3_1->setText(STR_NO_EXIST);


            if(1 == passenger_flow->m_cam2_1_set.param_file_exist) //1:存在　　2:不存在
            {
                if(!passenger_flow->m_cam2_1_set.param_file_name.isEmpty())
                    m_passenger_flow_info2->m_label_line3_1->setText(passenger_flow->m_cam2_1_set.param_file_name);
                else
                    m_passenger_flow_info2->m_label_line3_1->setText(STR_EXIST);
            }
            else
                m_passenger_flow_info2->m_label_line3_1->setText(STR_NO_EXIST);

            if(1 == passenger_flow->m_cam3_1_set.param_file_exist) //1:存在　　2:不存在
                m_passenger_flow_info3->m_label_line3_1->setText(STR_EXIST);
            else
                m_passenger_flow_info3->m_label_line3_1->setText(STR_NO_EXIST);

            if(1 == passenger_flow->m_cam4_1_set.param_file_exist) //1:存在　　2:不存在
                m_passenger_flow_info4->m_label_line3_1->setText(STR_EXIST);
            else
                m_passenger_flow_info4->m_label_line3_1->setText(STR_NO_EXIST);

        }break;

    }

}

void CPassengerFlowWidget::slot_check_region_cam1_1_button_default()
{
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumDefault,1);
}
void CPassengerFlowWidget::slot_check_region_cam1_1_button_bg_save()
{
    save_bg_progress(1);
}
void CPassengerFlowWidget::slot_check_region_cam1_1_button_save()
{
    read_data();
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumSave);
}
void CPassengerFlowWidget::slot_check_region_cam2_1_button_default()
{
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumDefault,2);
}
void CPassengerFlowWidget::slot_check_region_cam2_1_button_bg_save()
{
    save_bg_progress(2);
}
void CPassengerFlowWidget::slot_check_region_cam2_1_button_save()
{
    read_data();
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumSave);
}
//cam3-1
void CPassengerFlowWidget::slot_check_region_cam3_1_button_default()
{
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumDefault,3);
}
void CPassengerFlowWidget::slot_check_region_cam3_1_button_bg_save()
{
    save_bg_progress(3);
}
void CPassengerFlowWidget::slot_check_region_cam3_1_button_save()
{
    read_data();
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumSave);
}
//cam4-1
void CPassengerFlowWidget::slot_check_region_cam4_1_button_default()
{
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumDefault,4);
}
void CPassengerFlowWidget::slot_check_region_cam4_1_button_bg_save()
{
    save_bg_progress(4);
}
void CPassengerFlowWidget::slot_check_region_cam4_1_button_save()
{
    read_data();
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumSave);
}
void CPassengerFlowWidget::save_bg_progress(int _type)
{
    QString str_bg_save = tr("背景保存");


    //CProgressBox::getInstance()->test_init();
    CProgressBox::getInstance()->data_init();
    CProgressBox::getInstance()->set_title(str_bg_save);

    CProtocol::getInstance()->call_json_system_passenger_flow(5,1,_type);

    CProgressBox::getInstance()->exec();



    if(CProgressBox::KEnumStatusSuccess == CProgressBox::getInstance()->get_status())
    {



        CMyMessageBox::getInstance()->set_content(str_bg_save,tr("背景保存成功!"),
        CLtyString::getInstance()->m_confirm,"");
        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
        CMyMessageBox::getInstance()->exec();

    }
    else
    {
        if(CProgressBox::KEnumStatusStop == CProgressBox::getInstance()->get_status())
        {
             //通知取消操作
             CProtocol::getInstance()->call_json_system_passenger_flow(5,2);
             return;
        }

        CMyMessageBox::getInstance()->set_content(str_bg_save,tr("背景保存失败!"),
        CLtyString::getInstance()->m_confirm,"");
        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
        CMyMessageBox::getInstance()->exec();

    }
}



























