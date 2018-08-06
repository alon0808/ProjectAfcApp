

#include "steering_wheel.h"
#include "./class/common_interface.h"
#include "LtyCommonDefine.h"

#define DX  760
#define DY  64

CSteeringWheel::CSteeringWheel(QWidget *parent) :
    QWidget(parent)
{
    ////////////////////////////////////////////////////////中间右边显示
    this->setFixedSize(266,587);
    //this->setStyleSheet("QWidget {border-image:url(:/img/res/zj_ycbg.png);}");
    this->setStyleSheet("QWidget {background:transparent;}");

    m_widget_head = new QWidget(this);
    m_widget_head->setFixedSize(266,587);
    m_widget_head->move(0,0);
    m_widget_head->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/zj_ycbg.png);}");


    m_label_speed1 = new QLabel(this); //限速
    m_label_speed1->setFixedSize(55,28);
    m_label_speed1->move(826 - DX,123 + 5 - DY);
    m_label_speed1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff; }");
    m_label_speed1->setText(tr("限速:"));

    m_label_speed2 = new QLabel(this); //速度
    m_label_speed2->setFixedSize(43,39);
    m_label_speed2->move(826 +55 - DX,118 - DY);
    m_label_speed2->setStyleSheet("QLabel{border:0px;background:transparent;font:38px;color:#ffffff; }");



    m_label_speed3 = new QLabel(this); //km/h
    m_label_speed3->setFixedSize(60,50);
    m_label_speed3->move(826 +55 + 43 - DX,118 + 4 - DY);
    m_label_speed3->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#ffffff; }");
    m_label_speed3->setText(tr("km/h"));

    m_label_speed4 = new QLabel(this); //速度
    m_label_speed4->setFixedSize(79,58);
    m_label_speed4->move(853 - DX ,245 + 3 - DY);
    m_label_speed4->setStyleSheet("QLabel{border:0px;background:transparent;font:69px;color:#ffffff; }");


    m_label_speed5 = new QLabel(this); //km/h
    m_label_speed5->setFixedSize(75,30);
    m_label_speed5->move(859 - DX ,245 + 58 + 14 - DY);
    m_label_speed5->setStyleSheet("QLabel{border:0px;background:transparent;font:28px;color:#ffffff; }");
    m_label_speed5->setText(tr("km/h"));

    m_label_speed6 = new QLabel(this); //超速图标
    m_label_speed6->setFixedSize(67,78);
    m_label_speed6->move(857 - DX ,401 - DY);
    m_label_speed6->setStyleSheet("QLabel{border:0px;border-image:url(:/img/res/zj_csbj1.png);font:28px;color:#1dddff; }");

    m_label_speed7 = new QLabel(this); //超速
    m_label_speed7->setFixedSize(95,28);
    m_label_speed7->move(857 - DX ,491 - DY);
    m_label_speed7->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#203360; }");


    //前门
    m_pushbutton_door_1 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_door_1,790 - DX,557 - DY,32,56);
    connect(m_pushbutton_door_1,SIGNAL(clicked()),this,SIGNAL(signal_pushbutton_door_1()));

    //后门
    m_pushbutton_door_2 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_door_2,960 - DX,557 - DY,32,56);
    connect(m_pushbutton_door_2,SIGNAL(clicked()),this,SIGNAL(signal_pushbutton_door_2()));
}

void CSteeringWheel::steering_wheel_notify(int _msg,int _event,void *_param)
{
    QString str_sheet = "";
    switch(_msg)
    {
        ////////////////////////////////////////////////////////////////////中间右边显示　start
        case kEnumMainPageLimitSpeed:    //限速
        {
            if(_event > 0)
                m_label_speed2->setText(QString::number(_event));

        }break;
        case kEnumMainPageCurrentSpeed:  //当前速度
        {
            m_label_speed4->setText(QString::number(_event));
        }break;
        case kEnumMainPageSpeedAlert:    //超速警报
        {
            if(kEnumSpeedAlertStart == _event)
            {
                m_label_speed6->setStyleSheet("QLabel{border:0px;border-image:url(:/img/res/zj_csbj2.png);font:28px;color:#1dddff; }");
                m_label_speed7->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#f8fa35; }");
                CEventData *pData = (CEventData *)_param;
                if(pData)
                     m_label_speed7->setText(pData->m_str_data);
            }
            else //if(kEnumSpeedAlertStop == _event)
            {
                m_label_speed6->setStyleSheet("QLabel{border:0px;border-image:url(:/img/res/zj_csbj1.png);font:28px;color:#1dddff; }");
                m_label_speed7->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#203360; }");
                CEventData *pData = (CEventData *)_param;
                if(pData)
                     m_label_speed7->setText(pData->m_str_data);
            }
        }break;
        case kEnumMainPageFrontDoor:     //前门
        {
            if(kEnumDoorOpen == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                        .arg(":/img/res//zj_qm1.png").arg(":/img/res/zj_qm2.png").arg(":/img/res/zj_qm2.png");

            else //if(kEnumDoorClose == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                        .arg(":/img/res//zj_hm1.png").arg(":/img/res/zj_hm2.png").arg(":/img/res/zj_hm2.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_door_1,str_sheet);

        }break;
        case kEnumMainPageBackDoor:       //后门
        {
            if(kEnumDoorOpen == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                        .arg(":/img/res//zj_qm1.png").arg(":/img/res/zj_qm2.png").arg(":/img/res/zj_qm2.png");

            else //if(kEnumDoorClose == _event)
                str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
                        .arg(":/img/res//zj_hm1.png").arg(":/img/res/zj_hm2.png").arg(":/img/res/zj_hm2.png");

            CCommonInterface::modify_button_sheet(m_pushbutton_door_2,str_sheet);

        }break;
        ////////////////////////////////////////////////////////////////////中间右边显示　end
    }
}








