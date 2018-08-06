#include "videotape_playback.h"

#include "LtyCommonDefine.h"
#include "./class/common_interface.h"
#include "LtyCommonStyle.h"

#include "protocol.h"

CVideotapePlayback::CVideotapePlayback(QWidget *parent) :
    QDialog(parent)
{

    this->setObjectName("CVideotapePlayback");
    this->setStyleSheet("QDialog#CVideotapePlayback{background:transparent;}");
    this->setFixedSize(1024,768);
    this->setWindowFlags(Qt::FramelessWindowHint/*|Qt::WindowStaysOnTopHint*/);
    this->move(0,0);

    this->setAttribute(Qt::WA_TranslucentBackground, true);

    QString str_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    m_pushbutton_amplify = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_amplify,0,0,1024,768,str_sheet);

    connect(m_pushbutton_amplify,SIGNAL(clicked()),this,SLOT(slot_pushbutton_amplify()));


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetVideoPlayback,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_button_back()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_button_home()));
    connect(this,SIGNAL(signal_head_home_back_show(bool)),m_CHeadWidget,SLOT(slot_main_head_home_back_show(bool)));

    m_widget_bottom1 = new QWidget(this);
    m_widget_bottom1->setFixedSize(1024,90);
    m_widget_bottom1->move(0,768 - 90);
    m_widget_bottom1->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/bg.png);}");

    /*
        QPushButton *m_pushbutton_1;
    QPushButton *m_pushbutton_2;
    CMyPushButton *m_pushbutton_3;
    QPushButton *m_pushbutton_4;
    QPushButton *m_pushbutton_5;

    CMyPushButton *m_pushbutton_6;
    */
    QString str_show = tr("正常播放");
    m_label_play_status = new QLabel(this);
    str_sheet = "QLabel{border:0px;font:26px;color:#e5e8e9;background:transparent;}";
    CCommonInterface::init_label_text(m_label_play_status,0,768 - 90,200,90,str_show,str_sheet);
    m_label_play_status->setAlignment(Qt::AlignCenter);

    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/fast.png").arg(":/img/res/main_page/menu/system_manage/fast.png").arg(":/img/res/main_page/menu/system_manage/fast.png");

    m_pushbutton_1 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_1,352 - 70 - 8,704,80,50,str_sheet);

    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/ona.png").arg(":/img/res/main_page/menu/system_manage/ona_selected.png").arg(":/img/res/main_page/menu/system_manage/ona_selected.png");

    m_pushbutton_2 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_2,456 - 70 - 8,704,80,50,str_sheet);

    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/suspended.png").arg(":/img/res/main_page/menu/system_manage/suspended.png").arg(":/img/res/main_page/menu/system_manage/suspended.png");

    m_pushbutton_3 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_3,548 - 70 - 8,704,80,50,str_sheet);


    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/next.png").arg(":/img/res/main_page/menu/system_manage/next_selected.png").arg(":/img/res/main_page/menu/system_manage/next_selected.png");

    m_pushbutton_4 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_4,631 - 70 - 8,704,80,50,str_sheet);

    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/back.png").arg(":/img/res/main_page/menu/system_manage/back_selected.png").arg(":/img/res/main_page/menu/system_manage/back_selected.png");

    m_pushbutton_5 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_5,724 - 70 - 8,704,80,50,str_sheet);

    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/closesound.png").arg(":/img/res/main_page/menu/system_manage/closesound.png").arg(":/img/res/main_page/menu/system_manage/closesound.png");

    m_pushbutton_6 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_6,1024 - 100 + (100 - 80)/2,768 - 90 + 20,80,50,str_sheet);


     str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/left.png").arg(":/img/res/main_page/menu/system_manage/left.png").arg(":/img/res/main_page/menu/system_manage/left.png");

    //上一个通道
    m_pushbutton_prev_passageway = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_prev_passageway,17,312,80,50,str_sheet);

    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/right.png").arg(":/img/res/main_page/menu/system_manage/right.png").arg(":/img/res/main_page/menu/system_manage/right.png");

    //下一个通道
    m_pushbutton_next_passageway = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_next_passageway,920,312,80,50,str_sheet);

    m_pushbutton_prev_passageway->hide();
    m_pushbutton_next_passageway->hide();

    connect(m_pushbutton_prev_passageway,SIGNAL(clicked()),this,SLOT(slot_pushbutton_prev_passageway()));
    connect(m_pushbutton_next_passageway,SIGNAL(clicked()),this,SLOT(slot_pushbutton_next_passageway()));

   //QWidget *m_widget_line1;
   //QWidget *m_widget_line2;
    m_widget_line1 = new QWidget(this);
    m_widget_line1->setFixedSize(2,90);
    m_widget_line1->move(200,768 - 90);
    m_widget_line1->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/line.png);}");

    m_widget_line2 = new QWidget(this);
    m_widget_line2->setFixedSize(2,90);
    m_widget_line2->move(1024 - 100,768 - 90);
    m_widget_line2->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/line.png);}");

    connect(m_pushbutton_1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_1()));
    connect(m_pushbutton_2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_2()));
    connect(m_pushbutton_3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_3()));
    connect(m_pushbutton_4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_4()));
    connect(m_pushbutton_5,SIGNAL(clicked()),this,SLOT(slot_pushbutton_5()));
    connect(m_pushbutton_6,SIGNAL(clicked()),this,SLOT(slot_pushbutton_6()));

    m_pushbutton_1->hide();
    m_pushbutton_5->hide();

    m_play_or_stop = false;  //true:暂停　false 播放
    m_sound = false;  //true:静音　false 不静音

      //  QPushButton *m_pushbutton_7;//全屏
   // QPushButton *m_pushbutton_8;//不全屏

    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/expand.png").arg(":/img/res/main_page/menu/system_manage/expand.png").arg(":/img/res/main_page/menu/system_manage/expand.png");

    m_pushbutton_7 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_7,471,768 - 50 - 90,80,50,str_sheet);

    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/shrink.png").arg(":/img/res/main_page/menu/system_manage/shrink.png").arg(":/img/res/main_page/menu/system_manage/shrink.png");

    m_pushbutton_8 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_8,471,722,80,50,str_sheet);
    m_pushbutton_8->hide();

    connect(m_pushbutton_7,SIGNAL(clicked()),this,SLOT(slot_pushbutton_7()));
    connect(m_pushbutton_8,SIGNAL(clicked()),this,SLOT(slot_pushbutton_8()));

    slot_videotape_query_playback(1);

}

void CVideotapePlayback::slot_pushbutton_1()
{
    CSystemPlayBack play_back;
    play_back.m_operate_type = CSystemPlayBack::kEnumPrevVideo;//上一个视频
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
}
void CVideotapePlayback::slot_pushbutton_2()
{
    QString str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/play.png").arg(":/img/res/main_page/menu/system_manage/play.png").arg(":/img/res/main_page/menu/system_manage/play.png");

    CCommonInterface ::modify_button_sheet(m_pushbutton_3,str_sheet);
    m_play_or_stop = true;
    //m_label_play_status->setText(tr("正常播放"));


    CSystemPlayBack play_back;
    play_back.m_operate_type = CSystemPlayBack::kEnumPrevPlay;//后退
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
}

void CVideotapePlayback::playback_init()
{
    //初始化按钮为暂停状态
    QString str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/play.png").arg(":/img/res/main_page/menu/system_manage/play.png").arg(":/img/res/main_page/menu/system_manage/play.png");

    CCommonInterface ::modify_button_sheet(m_pushbutton_3,str_sheet);
    m_play_or_stop = true;
    m_label_play_status->setText(tr("正常播放"));

    m_sound = false;
    str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/closesound.png").arg(":/img/res/main_page/menu/system_manage/closesound.png").arg(":/img/res/main_page/menu/system_manage/closesound.png");

    CCommonInterface ::modify_button_sheet(m_pushbutton_6,str_sheet);
}

void CVideotapePlayback::slot_pushbutton_3() //暂停　播放
{
    QString str_sheet = "";
    if(m_play_or_stop)
    {
        m_play_or_stop = false;
            str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/suspended.png").arg(":/img/res/main_page/menu/system_manage/suspended.png").arg(":/img/res/main_page/menu/system_manage/suspended.png");

        CCommonInterface ::modify_button_sheet(m_pushbutton_3,str_sheet);
        m_label_play_status->setText(tr("暂停播放"));

        CSystemPlayBack play_back;
        play_back.m_operate_type = CSystemPlayBack::kEnumStop;//停止
        CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
    }
    else
    {
        str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/play.png").arg(":/img/res/main_page/menu/system_manage/play.png").arg(":/img/res/main_page/menu/system_manage/play.png");

        CCommonInterface ::modify_button_sheet(m_pushbutton_3,str_sheet);
        m_play_or_stop = true;
        m_label_play_status->setText(tr("正常播放"));

        CSystemPlayBack play_back;
        play_back.m_operate_type = CSystemPlayBack::kEnumStart;//播放
        CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
    }
}
void CVideotapePlayback::slot_pushbutton_4()
{

    QString str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
    .arg(":/img/res/main_page/menu/system_manage/play.png").arg(":/img/res/main_page/menu/system_manage/play.png").arg(":/img/res/main_page/menu/system_manage/play.png");

    CCommonInterface ::modify_button_sheet(m_pushbutton_3,str_sheet);
    m_play_or_stop = true;
    //m_label_play_status->setText(tr("正常播放"));


    CSystemPlayBack play_back;
    play_back.m_operate_type = CSystemPlayBack::kEnumBackPlay;//前进
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
}
void CVideotapePlayback::slot_pushbutton_5()
{
    CSystemPlayBack play_back;
    play_back.m_operate_type = CSystemPlayBack::kEnumBackVideo; //下一个视频
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
}
void CVideotapePlayback::slot_pushbutton_6()//静音　不静音
{
    QString str_sheet = "";
    if(m_sound)
    {
        m_sound = false;
            str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/closesound.png").arg(":/img/res/main_page/menu/system_manage/closesound.png").arg(":/img/res/main_page/menu/system_manage/closesound.png");

        CCommonInterface ::modify_button_sheet(m_pushbutton_6,str_sheet);

        CSystemPlayBack play_back;
        play_back.m_operate_type = CSystemPlayBack::kEnumVoice;//不静音
        CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
    }
    else
    {
        str_sheet = QString("QPushButton {border:0px;font:30px;color:#74FFE3;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/volume.png").arg(":/img/res/main_page/menu/system_manage/volume.png").arg(":/img/res/main_page/menu/system_manage/volume.png");

        CCommonInterface ::modify_button_sheet(m_pushbutton_6,str_sheet);
        m_sound = true;

        CSystemPlayBack play_back;
        play_back.m_operate_type = CSystemPlayBack::kEnumNoVoice;//静音
        CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
    }
}

void CVideotapePlayback::slot_pushbutton_prev_passageway() //上一个通道
{
    CSystemPlayBack play_back;
    play_back.m_operate_type = CSystemPlayBack::kEnumPrevPassageway;//上一个通道
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
}
void CVideotapePlayback::slot_pushbutton_next_passageway() //下一个通道
{
    CSystemPlayBack play_back;
    play_back.m_operate_type = CSystemPlayBack::kEnumNextPassageway; //下一个通道
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
}

void CVideotapePlayback::slot_pushbutton_7()
{//全屏
    m_pushbutton_7->hide();
    m_pushbutton_8->show();

    m_widget_bottom1->hide();
    m_label_play_status->hide();
    m_widget_line1->hide();
    m_widget_line2->hide();
    m_pushbutton_1->hide();
    m_pushbutton_2->hide();
    m_pushbutton_3->hide();
    m_pushbutton_4->hide();
    m_pushbutton_5->hide();
    m_pushbutton_6->hide();
    emit signal_head_home_back_show(false);
}
void CVideotapePlayback::slot_pushbutton_8()
{//缩小
    m_pushbutton_7->show();
    m_pushbutton_8->hide();

    m_widget_bottom1->show();
    m_label_play_status->show();
    m_widget_line1->show();
    m_widget_line2->show();
    m_pushbutton_1->hide();
    m_pushbutton_2->show();
    m_pushbutton_3->show();
    m_pushbutton_4->show();
    m_pushbutton_5->hide();
    m_pushbutton_6->show();
    emit signal_head_home_back_show(true);
}

char CVideotapePlayback::get_click_button_type()
{
    return m_type;
}

void CVideotapePlayback::slot_button_back()
{
    CSystemPlayBack play_back;
    play_back.m_operate_type = CSystemPlayBack::kEnumExit;
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
    emit signal_button_back();
}
void CVideotapePlayback::slot_button_home()
{
    CSystemPlayBack play_back;
    play_back.m_operate_type = CSystemPlayBack::kEnumExit;
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
    emit signal_button_home();

}

void CVideotapePlayback::slot_pushbutton_amplify()
{
}

#define VIDEO_BUTTON_SHEET_1 "QPushButton{border:0px;background:#000000;}"
void CVideotapePlayback::slot_videotape_query_playback(int _value)
{
    if(1 == _value)
    {
        CCommonInterface::modify_button_sheet(m_pushbutton_amplify,VIDEO_BUTTON_SHEET_TRANSPARENT);
    }
    else
    {
        CCommonInterface::modify_button_sheet(m_pushbutton_amplify,VIDEO_BUTTON_SHEET_1);
    }
}
#include <QDebug>
void CVideotapePlayback::slot_playback_quit()
{
    qDebug() << "CVideotapePlayback::slot_playback_quit()...........................";
    emit signal_button_back();
}

void CVideotapePlayback::slot_playback_notify_event(char _play_type,char _value)
{
    switch(_play_type)
    {
        /*1.启动 2.停止 3.快进 4.后退*/
        case 1:
        {
            m_label_play_status->setText(tr("正常播放"));
        }break;
        case 2:
        {
            m_label_play_status->setText(tr("停止播放"));
        }break;
        case 3:
        {
            m_label_play_status->setText(tr("快进:") + QString::number(_value) + tr("X"));
        }break;
        case 4:
        {
            m_label_play_status->setText(tr("快退:") + QString::number(_value) + tr("X"));
        }break;
    }
}








