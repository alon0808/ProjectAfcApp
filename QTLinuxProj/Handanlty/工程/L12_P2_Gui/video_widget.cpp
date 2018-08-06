

#include "video_widget.h"
#include "class/common_interface.h"
#include "protocol.h"
#include "LtyCommonStyle.h"

#include <QDebug>

#define HEAD_WIDGET_HEIGHT  82

//#define VIDEO_BUTTON_SHEET_1 "QPushButton{border:0px;background:#23afff;}"
//#define VIDEO_BUTTON_SHEET_2 "QPushButton{border:0px;background:#77ceff;}"

#define VIDEO_BUTTON_SHEET_1 "QPushButton{border:0px;background:#000000;}"
#define VIDEO_BUTTON_SHEET_2 "QPushButton{border:0px;background:#000000;}"

#define VIDEO_SPLIT_LINE    "QWidget{border:0px;background-color:#ffffff;}"
#define VIDEO_SPLIT_PIXEL   2




#define VIDEO_WIDTH_ADD_PIXEL   2
#define VIDEO_HEIGHT_ADD_PIXEL   2

#define VIDEO_SHOW_TEXT tr("无视频")
#define VIDEO_TEXT_40   "QLabel{font:40px;color:#ffffff;}"
#define VIDEO_TEXT_20   "QLabel{font:20px;color:#ffffff;}"

//1分频
CVideoOneWidget::CVideoOneWidget(QWidget *parent)
:QWidget(parent)
{
    this->setFixedSize(1024,768);


    QString str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play,0,0,1024,768,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number,1024 - 0 - 40,768 - 0 - 30 - 10,40,30,"",str_sheet);
    m_show_number->setText(tr("01"));

    int str_length = CCommonInterface::get_string_length(VIDEO_SHOW_TEXT,40);
    str_sheet = VIDEO_TEXT_40;
    m_show_text = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text,(1024 - str_length)/2,(768 - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,40);
    m_show_text->setText(VIDEO_SHOW_TEXT);


    set_output_status(1,true);
}

void CVideoOneWidget::modify_number(char _number)
{
    m_show_number->setText(QString::number(_number));
}

void CVideoOneWidget::set_output_status(char _number,bool _output)
{
    QString str_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    switch(_number)
    {
        case 1:
        {
            if(!_output)
            {
                m_show_text->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text->hide();
            }
            m_button_sheet1 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play,str_sheet);
        }break;
     }
}

//4分频
CVideoFourWidget::CVideoFourWidget(QWidget *parent)
:QWidget(parent)
{
    this->setFixedSize(1024,768);
    //border-image:url(:/img/res/main_page/menu/system_manage/nav_left_background.png);
    //this->setStyleSheet("QWidget{border:0px;border-image:url(:/img/res/main_page/16video/4bg.png);}");

    m_widget_line1 = new QWidget(this);
    m_widget_line2 = new QWidget(this);

    m_widget_line1->setFixedSize(1024,VIDEO_SPLIT_PIXEL);
    m_widget_line1->move(0,(768 - VIDEO_SPLIT_PIXEL)/2);
    m_widget_line1->setStyleSheet(VIDEO_SPLIT_LINE);

    m_widget_line2->setFixedSize(VIDEO_SPLIT_PIXEL,768);
    m_widget_line2->move((1024 - VIDEO_SPLIT_PIXEL)/2,0);
    m_widget_line2->setStyleSheet(VIDEO_SPLIT_LINE);

    #define FOUR_SCREEN_MAX_WIDTH    ((1024 - VIDEO_SPLIT_PIXEL)/2)
    #define FOUR_SCREEN_MAX_HEIGHT   ((768 - VIDEO_SPLIT_PIXEL)/2)

    int label_width = FOUR_SCREEN_MAX_WIDTH;
    int label_height = FOUR_SCREEN_MAX_HEIGHT;

    QString str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play1,0,0,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number1 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number1,0 + label_width - 40,0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number1->setText(tr("01"));

    int str_length = CCommonInterface::get_string_length(VIDEO_SHOW_TEXT,20);

    str_sheet = VIDEO_TEXT_20;
    m_show_text1 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text1,(0 + label_width - str_length)/2,(0 + label_height - 30)/2,str_length,30,"",str_sheet);
    m_show_text1->setText(VIDEO_SHOW_TEXT);


    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play2,0 + label_width + VIDEO_SPLIT_PIXEL,0,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number2 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number2,0 + label_width * 2 + VIDEO_SPLIT_PIXEL - 40,0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number2->setText(tr("02"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text2 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text2,0 + label_width + VIDEO_SPLIT_PIXEL + (label_width - str_length)/2,(0 + label_height - 30)/2,str_length,30,"",str_sheet);
    m_show_text2->setText(VIDEO_SHOW_TEXT);


    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play3 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play3,0,0 + label_height + VIDEO_SPLIT_PIXEL,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number3 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number3,0 + label_width - 40,0 + label_height * 2 + VIDEO_SPLIT_PIXEL - 30 - 10,40,30,"",str_sheet);
    m_show_number3->setText(tr("03"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text3 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text3,(0 + label_width - str_length)/2,0 + label_height + VIDEO_SPLIT_PIXEL + (label_height - 30)/2,str_length,30,"",str_sheet);
    m_show_text3->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play4 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play4,0 + label_width + VIDEO_SPLIT_PIXEL,0 + label_height + VIDEO_SPLIT_PIXEL,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number4 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number4,0 + label_width * 2 + VIDEO_SPLIT_PIXEL - 40,0 + label_height * 2 + VIDEO_SPLIT_PIXEL - 30 - 10,40,30,"",str_sheet);
    m_show_number4->setText(tr("04"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text4 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text4,0 + label_width + VIDEO_SPLIT_PIXEL + (label_width - str_length)/2,0 + label_height + VIDEO_SPLIT_PIXEL + (label_height - 30)/2,str_length,30,"",str_sheet);
    m_show_text4->setText(VIDEO_SHOW_TEXT);

///////////////////////////////////////////////////////////////
    str_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    m_pushbutton_amplify = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_amplify,0,0,1024,768,str_sheet);

    connect(m_pushbutton_amplify,SIGNAL(clicked()),this,SLOT(slot_pushbutton_amplify()));

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_amplify = new QLabel(this);
    CCommonInterface::init_label_text(m_show_amplify,1024 - 0 - 40,768 - 0 - 30 - 10,40,30,"",str_sheet);
    m_show_amplify->setText(tr("01"));

    str_length = CCommonInterface::get_string_length(VIDEO_SHOW_TEXT,40);
    str_sheet = VIDEO_TEXT_40;
    m_show_amplify_text = new QLabel(this);
    CCommonInterface::init_label_text(m_show_amplify_text,(1024 - str_length)/2,(768 - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,40);
    m_show_amplify_text->setText(VIDEO_SHOW_TEXT);

    slot_pushbutton_amplify();

    connect(m_pushbutton_play1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play1()));
    connect(m_pushbutton_play2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play2()));
    connect(m_pushbutton_play3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play3()));
    connect(m_pushbutton_play4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play4()));



    set_output_status(1,true);
    set_output_status(2,true);
    set_output_status(3,true);
    set_output_status(4,true);


}

void CVideoFourWidget::slot_pushbutton_amplify()
{
    m_pushbutton_amplify->hide();
    m_show_amplify->hide();
    m_show_amplify_text->hide();
    m_widget_line1->show();
    m_widget_line2->show();

    m_pushbutton_play1->show();
    m_pushbutton_play2->show();
    m_pushbutton_play3->show();
    m_pushbutton_play4->show();
    m_show_number1->show();
    m_show_number2->show();
    m_show_number3->show();
    m_show_number4->show();
    //m_show_text1->show();
    //m_show_text2->show();
    //m_show_text3->show();
    //m_show_text4->show();
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame4;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);

}

void CVideoFourWidget::slot_pushbutton_play1()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"01");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 1;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoFourWidget::slot_pushbutton_play2()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"02");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 2;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoFourWidget::slot_pushbutton_play3()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"03");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 3;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoFourWidget::slot_pushbutton_play4()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"04");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 4;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}

void CVideoFourWidget::show_one_status(QString _sheet,const QString &_number)
{

    CCommonInterface::modify_button_sheet(m_pushbutton_amplify,VIDEO_BUTTON_SHEET_TRANSPARENT);
    m_show_amplify->setText(_number);
    m_pushbutton_amplify->show();
    m_show_amplify->show();
    //m_show_amplify_text->show();
    m_save_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    m_save_number = _number;
    m_widget_line1->hide();
    m_widget_line2->hide();

    m_pushbutton_play1->hide();
    m_pushbutton_play2->hide();
    m_pushbutton_play3->hide();
    m_pushbutton_play4->hide();
    m_show_number1->hide();
    m_show_number2->hide();
    m_show_number3->hide();
    m_show_number4->hide();

    m_show_text1->hide();
    m_show_text2->hide();
    m_show_text3->hide();
    m_show_text4->hide();


}

void CVideoFourWidget::set_full_screen_status(char _number,bool _output)
{
     if(_output)
     {
         CCommonInterface::modify_button_sheet(m_pushbutton_amplify,VIDEO_BUTTON_SHEET_TRANSPARENT);
         m_show_amplify->setText(QString::number(_number));
     }
     else
     {
         CCommonInterface::modify_button_sheet(m_pushbutton_amplify,m_save_sheet);
         m_show_amplify->setText(QString::number(_number));
     }
}

void CVideoFourWidget::set_show_normal()
{
    slot_pushbutton_amplify();
}


void CVideoFourWidget::set_output_status(char _number,bool _output)
{
    QString str_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    switch(_number)
    {
        case 1:
        {
            if(!_output)
            {
                str_sheet = VIDEO_BUTTON_SHEET_1;
                m_show_text1->show();
            }
            else
            {
                m_show_text1->hide();
            }
            m_button_sheet1 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play1,str_sheet);
        }break;
        case 2:
        {
            if(!_output)
            {
                str_sheet = VIDEO_BUTTON_SHEET_2;
                m_show_text2->show();
            }
            else
            {
                m_show_text2->hide();
            }
            m_button_sheet2 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play2,str_sheet);
        }break;
        case 3:
        {
            if(!_output)
            {
                str_sheet = VIDEO_BUTTON_SHEET_2;
                m_show_text3->show();
            }
            else
            {
                m_show_text3->hide();
            }
            m_button_sheet3 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play3,str_sheet);
        }break;
        case 4:
        {
            if(!_output)
            {
                str_sheet = VIDEO_BUTTON_SHEET_1;
                m_show_text4->show();
            }
            else
            {
                m_show_text4->hide();
            }
            m_button_sheet4 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play4,str_sheet);
        }break;
    }
}

//9分频
CVideoNineWidget::CVideoNineWidget(QWidget *parent)
:QWidget(parent)
{
    m_parent = (CVideoWidget *)parent;
    this->setFixedSize(1024,768);

    int label_width = (1024 - VIDEO_SPLIT_PIXEL * 2)/3;
    int label_height = (768 - VIDEO_SPLIT_PIXEL * 2)/3;

    m_widget_V_line1 = new QWidget(this);
    m_widget_V_line2 = new QWidget(this);
    m_widget_H_line1 = new QWidget(this);
    m_widget_H_line2 = new QWidget(this);

    int height_H = (768 - VIDEO_SPLIT_PIXEL * 2)/3;
    m_widget_V_line1->setFixedSize(1024,VIDEO_SPLIT_PIXEL);
    m_widget_V_line1->move(0,height_H);
    m_widget_V_line1->setStyleSheet(VIDEO_SPLIT_LINE);

    m_widget_V_line2->setFixedSize(1024,VIDEO_SPLIT_PIXEL);
    m_widget_V_line2->move(0,height_H * 2 + VIDEO_SPLIT_PIXEL);
    m_widget_V_line2->setStyleSheet(VIDEO_SPLIT_LINE);


    int height_V = (1024 - VIDEO_SPLIT_PIXEL * 2)/3;
    m_widget_H_line1->setFixedSize(VIDEO_SPLIT_PIXEL,768);
    m_widget_H_line1->move(height_V,0);
    m_widget_H_line1->setStyleSheet(VIDEO_SPLIT_LINE);

    m_widget_H_line2->setFixedSize(VIDEO_SPLIT_PIXEL,768);
    m_widget_H_line2->move(height_V * 2 + VIDEO_SPLIT_PIXEL,0);
    m_widget_H_line2->setStyleSheet(VIDEO_SPLIT_LINE);


    QString str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play1 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play1,0,0,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number1 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number1,0 + label_width - 40,0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number1->setText(tr("01"));

    int str_length = CCommonInterface::get_string_length(VIDEO_SHOW_TEXT,20);
    str_sheet = VIDEO_TEXT_20;
    m_show_text1 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text1,0 + (label_width - str_length)/2,0 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text1->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play2 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play2,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1,0,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number2 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number2,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1 + label_width - 40,0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number2->setText(tr("02"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text2 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text2,(label_width + VIDEO_SPLIT_PIXEL) * 1 + (label_width - str_length)/2,0 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text2->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play3 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play3,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2,0,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number3 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number3,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2 + label_width - 40,0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number3->setText(tr("03"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text3 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text3,(label_width + VIDEO_SPLIT_PIXEL) * 2 + (label_width - str_length)/2,0 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text3->setText(VIDEO_SHOW_TEXT);

    /////
    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play4 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play4,0,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number4 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number4,0 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number4->setText(tr("04"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text4 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text4,(label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 1 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text4->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play5 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play5,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number5 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number5,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number5->setText(tr("05"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text5 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text5,(label_width + VIDEO_SPLIT_PIXEL) * 1 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 1 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text5->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play6 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play6,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number6 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number6,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number6->setText(tr("06"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text6 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text6,(label_width + VIDEO_SPLIT_PIXEL) * 2 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 1 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text6->setText(VIDEO_SHOW_TEXT);

//// 7 8 9
    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play7 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play7,0,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2,label_width,label_height + VIDEO_HEIGHT_ADD_PIXEL,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number7 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number7,0 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number7->setText(tr("07"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text7 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text7,(label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 2 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text7->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play8 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play8,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2,label_width,label_height + VIDEO_HEIGHT_ADD_PIXEL,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number8 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number8,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number8->setText(tr("08"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text8 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text8,(label_width + VIDEO_SPLIT_PIXEL) * 1 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 2 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text8->setText(VIDEO_SHOW_TEXT);


    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play9 = new QPushButton(this);;
    CCommonInterface::init_new_button(m_pushbutton_play9,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2,label_width,label_height + VIDEO_HEIGHT_ADD_PIXEL,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number9 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number9,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number9->setText(tr("09"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text9 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text9,(label_width + VIDEO_SPLIT_PIXEL) * 2 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 2 + (label_height - 30)/2,str_length,30,"",str_sheet,Qt::AlignCenter,20);
    m_show_text9->setText(VIDEO_SHOW_TEXT);


///////////////////////////////////////////////////////////////
    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_amplify = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_amplify,0,0,1024,768,str_sheet);

    connect(m_pushbutton_amplify,SIGNAL(clicked()),this,SLOT(slot_pushbutton_amplify()));

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_amplify = new QLabel(this);
    CCommonInterface::init_label_text(m_show_amplify,1024 - 0 - 40,768 - 0 - 30 - 10,40,30,"",str_sheet);
    m_show_amplify->setText(tr("01"));

    str_length = CCommonInterface::get_string_length(VIDEO_SHOW_TEXT,40);
    str_sheet = VIDEO_TEXT_40;
    m_show_amplify_text = new QLabel(this);
    CCommonInterface::init_label_text(m_show_amplify_text,(1024 - str_length)/2,(768 - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,40);
    m_show_amplify_text->setText(VIDEO_SHOW_TEXT);

    slot_pushbutton_amplify();

    connect(m_pushbutton_play1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play1()));
    connect(m_pushbutton_play2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play2()));
    connect(m_pushbutton_play3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play3()));
    connect(m_pushbutton_play4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play4()));
    connect(m_pushbutton_play5,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play5()));
    connect(m_pushbutton_play6,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play6()));
    connect(m_pushbutton_play7,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play7()));
    connect(m_pushbutton_play8,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play8()));
    connect(m_pushbutton_play9,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play9()));

    set_output_status(1,true);
    set_output_status(2,true);
    set_output_status(3,true);
    set_output_status(4,true);
    set_output_status(5,true);
    set_output_status(6,true);
    set_output_status(7,true);
    set_output_status(8,true);
    set_output_status(9,true);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

}
void CVideoNineWidget::set_show_normal()
{
    slot_pushbutton_amplify();
}
void CVideoNineWidget::set_line_hide()
{
    m_widget_H_line1->hide();
    m_widget_H_line2->hide();
    m_widget_V_line1->hide();
    m_widget_V_line2->hide();
    m_show_number1->hide();
    m_show_number2->hide();
    m_show_number3->hide();
    m_show_number4->hide();
    m_show_number5->hide();
    m_show_number6->hide();
    m_show_number7->hide();
    m_show_number8->hide();
    m_show_number9->hide();
}

void CVideoNineWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CVideoNineWidget::slot_pushbutton_amplify()
{
    m_pushbutton_amplify->hide();
    m_show_amplify->hide();
    m_show_amplify_text->hide();

    m_widget_H_line1->show();
    m_widget_H_line2->show();

    m_widget_V_line1->show();
    m_widget_V_line2->show();

    m_pushbutton_play1->show();
    m_pushbutton_play2->show();
    m_pushbutton_play3->show();
    m_pushbutton_play4->show();
    m_pushbutton_play5->show();
    m_pushbutton_play6->show();
    m_pushbutton_play7->show();
    m_pushbutton_play8->show();
    m_pushbutton_play9->show();
    m_show_number1->show();
    m_show_number2->show();
    m_show_number3->show();
    m_show_number4->show();
    m_show_number5->show();
    m_show_number6->show();
    m_show_number7->show();
    m_show_number8->show();
    m_show_number9->show();

    //m_show_text1->show();
    //m_show_text2->show();
    //m_show_text3->show();
    //m_show_text4->show();
    //m_show_text5->show();
    //m_show_text6->show();
    //m_show_text7->show();
    //m_show_text8->show();
    //m_show_text9->show();

    if(m_parent->get_current_mode() == CVideoWidget::kEnumVideoMode)
    {
        CVideoOutput videoOutput;
        videoOutput.m_frame = CProtocol::kEnumFrame9;

        CProtocol::getInstance()->call_json_camera_set(videoOutput);
    }

}

void CVideoNineWidget::slot_pushbutton_play1()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"01");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 1;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoNineWidget::slot_pushbutton_play2()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"02");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 2;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoNineWidget::slot_pushbutton_play3()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"03");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 3;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoNineWidget::slot_pushbutton_play4()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"04");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 4;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoNineWidget::slot_pushbutton_play5()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"05");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 5;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoNineWidget::slot_pushbutton_play6()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"06");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 6;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoNineWidget::slot_pushbutton_play7()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"07");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 7;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoNineWidget::slot_pushbutton_play8()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"08");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 8;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoNineWidget::slot_pushbutton_play9()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"09");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 9;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}

void CVideoNineWidget::show_one_status(QString _sheet,const QString &_number)
{
    CCommonInterface::modify_button_sheet(m_pushbutton_amplify,VIDEO_BUTTON_SHEET_TRANSPARENT);
    m_show_amplify->setText(_number);
    m_pushbutton_amplify->show();
    //m_show_amplify->show();
    //m_show_amplify_text->show();
    m_save_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    m_save_number = _number;

    m_widget_H_line1->hide();
    m_widget_H_line2->hide();

    m_widget_V_line1->hide();
    m_widget_V_line2->hide();

    m_pushbutton_play1->hide();
    m_pushbutton_play2->hide();
    m_pushbutton_play3->hide();
    m_pushbutton_play4->hide();
    m_pushbutton_play5->hide();
    m_pushbutton_play6->hide();
    m_pushbutton_play7->hide();
    m_pushbutton_play8->hide();
    m_pushbutton_play9->hide();
    m_show_number1->hide();
    m_show_number2->hide();
    m_show_number3->hide();
    m_show_number4->hide();
    m_show_number5->hide();
    m_show_number6->hide();
    m_show_number7->hide();
    m_show_number8->hide();
    m_show_number9->hide();

    m_show_text1->hide();
    m_show_text2->hide();
    m_show_text3->hide();
    m_show_text4->hide();
    m_show_text5->hide();
    m_show_text6->hide();
    m_show_text7->hide();
    m_show_text8->hide();
    m_show_text9->hide();

}
void CVideoNineWidget::set_pushbutton_enable(bool _enable)
{
    m_pushbutton_play1->setEnabled(_enable);
    m_pushbutton_play2->setEnabled(_enable);
    m_pushbutton_play3->setEnabled(_enable);
    m_pushbutton_play4->setEnabled(_enable);
    m_pushbutton_play5->setEnabled(_enable);
    m_pushbutton_play6->setEnabled(_enable);
    m_pushbutton_play7->setEnabled(_enable);
    m_pushbutton_play8->setEnabled(_enable);
    m_pushbutton_play9->setEnabled(_enable);

}



void CVideoNineWidget::set_full_screen_status(char _number,bool _output)
{
     if(_output)
     {
         CCommonInterface::modify_button_sheet(m_pushbutton_amplify,VIDEO_BUTTON_SHEET_TRANSPARENT);
         m_show_amplify->setText(QString::number(_number));
     }
     else
     {
         CCommonInterface::modify_button_sheet(m_pushbutton_amplify,m_save_sheet);
         m_show_amplify->setText(QString::number(_number));
     }
}

void CVideoNineWidget::set_output_status(char _number,bool _output)
{
    QString str_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    switch(_number)
    {
        case 1:
        {
            if(!_output)
            {
                m_show_text1->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }else
            {
                m_show_text1->hide();
            }

            m_button_sheet1 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play1,str_sheet);
        }break;
        case 2:
        {
            if(!_output)
            {
                m_show_text2->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }else
            {
                m_show_text2->hide();
            }

            m_button_sheet2 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play2,str_sheet);
        }break;
        case 3:
        {
            if(!_output)
            {
                m_show_text3->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }else
            {
                m_show_text3->hide();
            }

            m_button_sheet3 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play3,str_sheet);
        }break;
        case 4:
        {
            if(!_output)
            {
                m_show_text4->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }else
            {
                m_show_text4->hide();
            }

            m_button_sheet4 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play4,str_sheet);
        }break;
        case 5:
        {
            if(!_output)
            {
                m_show_text5->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }else
            {
                m_show_text5->hide();
            }

            m_button_sheet5 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play5,str_sheet);
        }break;
        case 6:
        {
            if(!_output)
            {
                m_show_text6->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }else
            {
                m_show_text6->hide();
            }

            m_button_sheet6 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play6,str_sheet);
        }break;
        case 7:
        {
            if(!_output)
            {
                m_show_text7->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }else
            {
                m_show_text7->hide();
            }

            m_button_sheet7 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play7,str_sheet);
        }break;
        case 8:
        {
            if(!_output)
            {
                m_show_text8->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }else
            {
                m_show_text8->hide();
            }

            m_button_sheet8 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play8,str_sheet);
        }break;
        case 9:
        {
            if(!_output)
            {
                m_show_text9->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text9->hide();
            }
            m_button_sheet9 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play9,str_sheet);
        }break;
    }
}


//16分频
CVideoSixteenWidget::CVideoSixteenWidget(QWidget *parent)
:QWidget(parent)
{
    this->setFixedSize(1024,768);
    int label_width = (1024 - VIDEO_SPLIT_PIXEL * 3)/4;
    int label_height = (768 - VIDEO_SPLIT_PIXEL * 3)/4;

    m_widget_V_line1 = new QWidget(this);
    m_widget_V_line2 = new QWidget(this);
    m_widget_V_line3 = new QWidget(this);
    m_widget_H_line1 = new QWidget(this);
    m_widget_H_line2 = new QWidget(this);
    m_widget_H_line3 = new QWidget(this);

    int height_H = (768 - VIDEO_SPLIT_PIXEL * 3)/4;
    m_widget_V_line1->setFixedSize(1024,VIDEO_SPLIT_PIXEL);
    m_widget_V_line1->move(0,height_H);
    m_widget_V_line1->setStyleSheet(VIDEO_SPLIT_LINE);

    m_widget_V_line2->setFixedSize(1024,VIDEO_SPLIT_PIXEL);
    m_widget_V_line2->move(0,height_H * 2 + VIDEO_SPLIT_PIXEL);
    m_widget_V_line2->setStyleSheet(VIDEO_SPLIT_LINE);

    m_widget_V_line3->setFixedSize(1024,VIDEO_SPLIT_PIXEL);
    m_widget_V_line3->move(0,height_H * 3 + VIDEO_SPLIT_PIXEL * 2);
    m_widget_V_line3->setStyleSheet(VIDEO_SPLIT_LINE);

    int height_V = (1024 - VIDEO_SPLIT_PIXEL * 3)/4;
    m_widget_H_line1->setFixedSize(VIDEO_SPLIT_PIXEL,768);
    m_widget_H_line1->move(height_V,0);
    m_widget_H_line1->setStyleSheet(VIDEO_SPLIT_LINE);

    m_widget_H_line2->setFixedSize(VIDEO_SPLIT_PIXEL,768);
    m_widget_H_line2->move(height_V * 2 + VIDEO_SPLIT_PIXEL,0);
    m_widget_H_line2->setStyleSheet(VIDEO_SPLIT_LINE);

    m_widget_H_line3->setFixedSize(VIDEO_SPLIT_PIXEL,768);
    m_widget_H_line3->move(height_V * 3 + VIDEO_SPLIT_PIXEL * 2,0);
    m_widget_H_line3->setStyleSheet(VIDEO_SPLIT_LINE);


    QString str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play1,0 + (label_width + VIDEO_SPLIT_PIXEL) * 0,0 + (label_height + VIDEO_SPLIT_PIXEL) * 0,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number1 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number1,0 + (label_width + VIDEO_SPLIT_PIXEL) * 0 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number1->setText(tr("01"));

    int str_length = CCommonInterface::get_string_length(VIDEO_SHOW_TEXT,20);
    str_sheet = VIDEO_TEXT_20;
    m_show_text1 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text1,(label_width + VIDEO_SPLIT_PIXEL) * 0 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 0 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text1->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play2,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1,0 + (label_height + VIDEO_SPLIT_PIXEL) * 0,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number2 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number2,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number2->setText(tr("02"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text2 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text2,(label_width + VIDEO_SPLIT_PIXEL) * 1 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 0 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text2->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play3 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play3,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2,0 + (label_height + VIDEO_SPLIT_PIXEL) * 0,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number3 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number3,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number3->setText(tr("03"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text3 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text3,(label_width + VIDEO_SPLIT_PIXEL) * 2 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 0 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text3->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play4 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play4,0 + (label_width + VIDEO_SPLIT_PIXEL) * 3,0 + (label_height + VIDEO_SPLIT_PIXEL) * 0,label_width + VIDEO_WIDTH_ADD_PIXEL,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number4 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number4,0 + (label_width + VIDEO_SPLIT_PIXEL) * 3 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 0 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number4->setText(tr("04"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text4 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text4,(label_width + VIDEO_SPLIT_PIXEL) * 3 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 0 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text4->setText(VIDEO_SHOW_TEXT);

    //第二行
    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play5 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play5,0 + (label_width + VIDEO_SPLIT_PIXEL) * 0,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number5 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number5,0 + (label_width + VIDEO_SPLIT_PIXEL) * 0 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number5->setText(tr("05"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text5 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text5,(label_width + VIDEO_SPLIT_PIXEL) * 0 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 1 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text5->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play6 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play6,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number6 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number6,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number6->setText(tr("06"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text6 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text6,(label_width + VIDEO_SPLIT_PIXEL) * 1 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 1 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text6->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play7 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play7,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number7 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number7,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number7->setText(tr("07"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text7 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text7,(label_width + VIDEO_SPLIT_PIXEL) * 2 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 1 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text7->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play8 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play8,0 + (label_width + VIDEO_SPLIT_PIXEL) * 3,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1,label_width + VIDEO_WIDTH_ADD_PIXEL,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number8 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number8,0 + (label_width + VIDEO_SPLIT_PIXEL) * 3 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 1 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number8->setText(tr("08"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text8 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text8,(label_width + VIDEO_SPLIT_PIXEL) * 3 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 1 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text8->setText(VIDEO_SHOW_TEXT);

    //第三行
    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play9 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play9,0 + (label_width + VIDEO_SPLIT_PIXEL) * 0,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number9 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number9,0 + (label_width + VIDEO_SPLIT_PIXEL) * 0 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number9->setText(tr("09"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text9 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text9,(label_width + VIDEO_SPLIT_PIXEL) * 0 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 2 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text9->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play10 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play10,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number10 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number10,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number10->setText(tr("10"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text10 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text10,(label_width + VIDEO_SPLIT_PIXEL) * 1 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 2 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text10->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play11 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play11,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2,label_width,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number11 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number11,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number11->setText(tr("11"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text11 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text11,(label_width + VIDEO_SPLIT_PIXEL) * 2 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 2 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text11->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play12 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play12,0 + (label_width + VIDEO_SPLIT_PIXEL) * 3,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2,label_width + VIDEO_WIDTH_ADD_PIXEL,label_height,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number12 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number12,0 + (label_width + VIDEO_SPLIT_PIXEL) * 3 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 2 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number12->setText(tr("12"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text12 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text12,(label_width + VIDEO_SPLIT_PIXEL) * 3 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 2 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text12->setText(VIDEO_SHOW_TEXT);

     //第四行
    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play13 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play13,0 + (label_width + VIDEO_SPLIT_PIXEL) * 0,0 + (label_height + VIDEO_SPLIT_PIXEL) * 3,label_width,label_height + VIDEO_HEIGHT_ADD_PIXEL,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number13 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number13,0 + (label_width + VIDEO_SPLIT_PIXEL) * 0 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 3 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number13->setText(tr("13"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text13 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text13,(label_width + VIDEO_SPLIT_PIXEL) * 0 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 3 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text13->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play14 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play14,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1,0 + (label_height + VIDEO_SPLIT_PIXEL) * 3,label_width,label_height + VIDEO_HEIGHT_ADD_PIXEL,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number14 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number14,0 + (label_width + VIDEO_SPLIT_PIXEL) * 1 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 3 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number14->setText(tr("14"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text14 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text14,(label_width + VIDEO_SPLIT_PIXEL) * 1 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 3 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text14->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_2;
    m_pushbutton_play15 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play15,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2,0 + (label_height + VIDEO_SPLIT_PIXEL) * 3,label_width,label_height + VIDEO_HEIGHT_ADD_PIXEL,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number15 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number15,0 + (label_width + VIDEO_SPLIT_PIXEL) * 2 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 3 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number15->setText(tr("15"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text15 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text15,(label_width + VIDEO_SPLIT_PIXEL) * 2 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 3 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text15->setText(VIDEO_SHOW_TEXT);

    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_play16 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_play16,0 + (label_width + VIDEO_SPLIT_PIXEL) * 3,0 + (label_height + VIDEO_SPLIT_PIXEL) * 3,label_width + VIDEO_WIDTH_ADD_PIXEL,label_height + VIDEO_HEIGHT_ADD_PIXEL,str_sheet);

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_number16 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_number16,0 + (label_width + VIDEO_SPLIT_PIXEL) * 3 + label_width - 40,0 + (label_height + VIDEO_SPLIT_PIXEL) * 3 + label_height - 30 - 10,40,30,"",str_sheet);
    m_show_number16->setText(tr("16"));

    str_sheet = VIDEO_TEXT_20;
    m_show_text16 = new QLabel(this);
    CCommonInterface::init_label_text(m_show_text16,(label_width + VIDEO_SPLIT_PIXEL) * 3 + (label_width - str_length)/2,(label_height + VIDEO_SPLIT_PIXEL) * 3 + (label_height - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,20);
    m_show_text16->setText(VIDEO_SHOW_TEXT);

///////////////////////////////////////////////////////////////
    str_sheet = VIDEO_BUTTON_SHEET_1;
    m_pushbutton_amplify = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_amplify,0,0,1024,768,str_sheet);

    connect(m_pushbutton_amplify,SIGNAL(clicked()),this,SLOT(slot_pushbutton_amplify()));

    str_sheet = "QLabel{font:20px;color:#ffffff;}";
    m_show_amplify = new QLabel(this);
    CCommonInterface::init_label_text(m_show_amplify,1024 - 0 - 40,768 - 0 - 30 - 10,40,30,"",str_sheet);
    m_show_amplify->setText(tr("01"));

    str_length = CCommonInterface::get_string_length(VIDEO_SHOW_TEXT,40);
    str_sheet = VIDEO_TEXT_40;
    m_show_amplify_text = new QLabel(this);
    CCommonInterface::init_label_text(m_show_amplify_text,(1024 - str_length)/2,(768 - 50)/2,str_length,50,"",str_sheet,Qt::AlignCenter,40);
    m_show_amplify_text->setText(VIDEO_SHOW_TEXT);

    slot_pushbutton_amplify();

    connect(m_pushbutton_play1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play1()));
    connect(m_pushbutton_play2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play2()));
    connect(m_pushbutton_play3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play3()));
    connect(m_pushbutton_play4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play4()));
    connect(m_pushbutton_play5,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play5()));
    connect(m_pushbutton_play6,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play6()));
    connect(m_pushbutton_play7,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play7()));
    connect(m_pushbutton_play8,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play8()));
    connect(m_pushbutton_play9,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play9()));

    connect(m_pushbutton_play10,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play10()));
    connect(m_pushbutton_play11,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play11()));
    connect(m_pushbutton_play12,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play12()));
    connect(m_pushbutton_play13,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play13()));
    connect(m_pushbutton_play14,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play14()));
    connect(m_pushbutton_play15,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play15()));
    connect(m_pushbutton_play16,SIGNAL(clicked()),this,SLOT(slot_pushbutton_play16()));

    set_output_status(1,true);
    set_output_status(2,true);
    set_output_status(3,true);
    set_output_status(4,true);
    set_output_status(5,true);
    set_output_status(6,true);
    set_output_status(7,true);
    set_output_status(8,true);
    set_output_status(9,true);
    set_output_status(10,true);
    set_output_status(11,true);
    set_output_status(12,true);
    set_output_status(13,true);
    set_output_status(14,true);
    set_output_status(15,true);
    set_output_status(16,true);

}

void CVideoSixteenWidget::slot_pushbutton_amplify()
{
    m_pushbutton_amplify->hide();
    m_show_amplify->hide();
    m_show_amplify_text->hide();

    m_widget_V_line1->show();
    m_widget_V_line2->show();
    m_widget_V_line3->show();
    m_widget_H_line1->show();
    m_widget_H_line2->show();
    m_widget_H_line3->show();

    m_pushbutton_play1->show();
    m_pushbutton_play2->show();
    m_pushbutton_play3->show();
    m_pushbutton_play4->show();
    m_pushbutton_play5->show();
    m_pushbutton_play6->show();
    m_pushbutton_play7->show();
    m_pushbutton_play8->show();
    m_pushbutton_play9->show();
    m_pushbutton_play10->show();
    m_pushbutton_play11->show();
    m_pushbutton_play12->show();
    m_pushbutton_play13->show();
    m_pushbutton_play14->show();
    m_pushbutton_play15->show();
    m_pushbutton_play16->show();

    m_show_number1->show();
    m_show_number2->show();
    m_show_number3->show();
    m_show_number4->show();
    m_show_number5->show();
    m_show_number6->show();
    m_show_number7->show();
    m_show_number8->show();
    m_show_number9->show();
    m_show_number10->show();
    m_show_number11->show();
    m_show_number12->show();
    m_show_number13->show();
    m_show_number14->show();
    m_show_number15->show();
    m_show_number16->show();
#if 0
    m_show_text1->show();
    m_show_text2->show();
    m_show_text3->show();
    m_show_text4->show();
    m_show_text5->show();
    m_show_text6->show();
    m_show_text7->show();
    m_show_text8->show();
    m_show_text9->show();
    m_show_text10->show();
    m_show_text11->show();
    m_show_text12->show();
    m_show_text13->show();
    m_show_text14->show();
    m_show_text15->show();
    m_show_text16->show();
#endif
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame16;

    CProtocol::getInstance()->call_json_camera_set(videoOutput);

}

void CVideoSixteenWidget::slot_pushbutton_play1()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"01");
        CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 1;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play2()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"02");
            CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 2;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play3()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"03");
                CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 3;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play4()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"04");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 4;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play5()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"05");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 5;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play6()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"06");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 6;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play7()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"07");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 7;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play8()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"08");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 8;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play9()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"09");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 9;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}

void CVideoSixteenWidget::slot_pushbutton_play10()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"10");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 10;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play11()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"11");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 11;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play12()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"12");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 12;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play13()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"13");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 13;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play14()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"14");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 14;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play15()
{
    show_one_status(VIDEO_BUTTON_SHEET_2,"15");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 15;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}
void CVideoSixteenWidget::slot_pushbutton_play16()
{
    show_one_status(VIDEO_BUTTON_SHEET_1,"16");
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFrame1;
    videoOutput.m_number = 16;
    CProtocol::getInstance()->call_json_camera_set(videoOutput);
}

void CVideoSixteenWidget::show_one_status(QString _sheet,const QString &_number)
{
    CCommonInterface::modify_button_sheet(m_pushbutton_amplify,VIDEO_BUTTON_SHEET_TRANSPARENT);
    m_show_amplify->setText(_number);
    m_pushbutton_amplify->show();
    m_show_amplify->show();
    //m_show_amplify_text->show();
    m_save_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    m_save_number = _number;

    m_widget_V_line1->hide();
    m_widget_V_line2->hide();
    m_widget_V_line3->hide();
    m_widget_H_line1->hide();
    m_widget_H_line2->hide();
    m_widget_H_line3->hide();

    m_pushbutton_play1->hide();
    m_pushbutton_play2->hide();
    m_pushbutton_play3->hide();
    m_pushbutton_play4->hide();
    m_pushbutton_play5->hide();
    m_pushbutton_play6->hide();
    m_pushbutton_play7->hide();
    m_pushbutton_play8->hide();
    m_pushbutton_play9->hide();
    m_pushbutton_play10->hide();
    m_pushbutton_play11->hide();
    m_pushbutton_play12->hide();
    m_pushbutton_play13->hide();
    m_pushbutton_play14->hide();
    m_pushbutton_play15->hide();
    m_pushbutton_play16->hide();

    m_show_number1->hide();
    m_show_number2->hide();
    m_show_number3->hide();
    m_show_number4->hide();
    m_show_number5->hide();
    m_show_number6->hide();
    m_show_number7->hide();
    m_show_number8->hide();
    m_show_number9->hide();
    m_show_number10->hide();
    m_show_number11->hide();
    m_show_number12->hide();
    m_show_number13->hide();
    m_show_number14->hide();
    m_show_number15->hide();
    m_show_number16->hide();

    m_show_text1->hide();
    m_show_text2->hide();
    m_show_text3->hide();
    m_show_text4->hide();
    m_show_text5->hide();
    m_show_text6->hide();
    m_show_text7->hide();
    m_show_text8->hide();
    m_show_text9->hide();
    m_show_text10->hide();
    m_show_text11->hide();
    m_show_text12->hide();
    m_show_text13->hide();
    m_show_text14->hide();
    m_show_text15->hide();
    m_show_text16->hide();

}

void CVideoSixteenWidget::set_show_normal()
{
    slot_pushbutton_amplify();
}

void CVideoSixteenWidget::set_full_screen_status(char _number,bool _output)
{
     if(_output)
     {
         CCommonInterface::modify_button_sheet(m_pushbutton_amplify,VIDEO_BUTTON_SHEET_TRANSPARENT);
         m_show_amplify->setText(QString::number(_number));
     }
     else
     {
         CCommonInterface::modify_button_sheet(m_pushbutton_amplify,m_save_sheet);
         m_show_amplify->setText(QString::number(_number));
     }
}

void CVideoSixteenWidget::set_output_status(char _number,bool _output)
{
    QString str_sheet = VIDEO_BUTTON_SHEET_TRANSPARENT;
    switch(_number)
    {
        case 1:
        {
            if(!_output)
            {
                m_show_text1->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text1->hide();
            }

            m_button_sheet1 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play1,str_sheet);
        }break;
        case 2:
        {
            if(!_output)
            {
                m_show_text2->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }
            else
            {
                m_show_text2->hide();
            }

            m_button_sheet2 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play2,str_sheet);
        }break;
        case 3:
        {
            if(!_output)
            {
                m_show_text3->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }
            else
            {
                m_show_text3->hide();
            }

            m_button_sheet3 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play3,str_sheet);
        }break;
        case 4:
        {
            if(!_output)
            {
                m_show_text4->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text4->hide();
            }

            m_button_sheet4 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play4,str_sheet);
        }break;
        case 5:
        {
            if(!_output)
            {
                m_show_text5->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }
            else
            {
                m_show_text5->hide();
            }

            m_button_sheet5 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play5,str_sheet);
        }break;
        case 6:
        {
            if(!_output)
            {
                m_show_text6->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text6->hide();
            }

            m_button_sheet6 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play6,str_sheet);
        }break;
        case 7:
        {
            if(!_output)
            {
                m_show_text7->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }
            else
            {
                m_show_text7->hide();
            }

            m_button_sheet7 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play7,str_sheet);
        }break;
        case 8:
        {
            if(!_output)
            {
                m_show_text8->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text8->hide();
            }

            m_button_sheet8 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play8,str_sheet);
        }break;
        case 9:
        {
            if(!_output)
            {
                m_show_text9->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text9->hide();
            }

            m_button_sheet9 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play9,str_sheet);
        }break;
        case 10:
        {
            if(!_output)
            {
                m_show_text10->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }
            else
            {
                m_show_text10->hide();
            }

            m_button_sheet10 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play10,str_sheet);
        }break;
        case 11:
        {
            if(!_output)
            {
                m_show_text11->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text11->hide();
            }

            m_button_sheet11 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play11,str_sheet);
        }break;
        case 12:
        {
            if(!_output)
            {
                m_show_text12->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }
            else
            {
                m_show_text12->hide();
            }

            m_button_sheet12 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play12,str_sheet);
        }break;
        case 13:
        {
            if(!_output)
            {
                m_show_text13->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }
            else
            {
                m_show_text13->hide();
            }
            m_button_sheet13 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play13,str_sheet);
        }break;
        case 14:
        {
            if(!_output)
            {
                m_show_text14->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text14->hide();
            }

            m_button_sheet14 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play14,str_sheet);
        }break;
        case 15:
        {
            if(!_output)
            {
                m_show_text15->show();
                str_sheet = VIDEO_BUTTON_SHEET_2;
            }
            else
            {
                m_show_text15->hide();
            }

            m_button_sheet15 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play15,str_sheet);
        }break;
        case 16:
        {
            if(!_output)
            {
                m_show_text16->show();
                str_sheet = VIDEO_BUTTON_SHEET_1;
            }
            else
            {
                m_show_text16->hide();
            }

            m_button_sheet16 = str_sheet;
            CCommonInterface::modify_button_sheet(m_pushbutton_play16,str_sheet);
        }break;
    }
}


#define SIXTEEN_SCREEN_MAX_WIDTH    ((1024 - 5 * 3)/4)
#define SIXTEEN_SCREEN_MAX_HEIGHT   ((768 - 5 * 3)/4)

#define SIXTEEN_SCREEN_NORMAL_WIDTH     ((1024 - 5 * 3 - 20 * 2)/4)
#define SIXTEEN_SCREEN_NORMAL_HEIGHT    ((768 - HEAD_WIDGET_HEIGHT - 5 * 3 - 25 * 2)/4)

CVideoWidget::CVideoWidget(QWidget *parent) :
    CParentWidget(parent)
{
    this->setFixedSize(1024,768);
    this->setObjectName("CVideoWidget");
    //this->setStyleSheet("QDialog#CVideoWidget{background-color:#03111E;}");
    this->setStyleSheet("QDialog#CVideoWidget{background:transparent;}");

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    int video_show_width = 1024;
    int video_show_height = 768;

    m_stackwidget_video = new QStackedWidget(this);
    m_stackwidget_video->move(0,0);
    m_stackwidget_video->setFixedSize(video_show_width,video_show_height);
    m_stackwidget_video->setObjectName(QString::fromUtf8("m_stackwidget_video"));
    m_stackwidget_video->setContentsMargins(0, 0, 0, 0);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidget16Video,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));
    connect(m_CHeadWidget,SIGNAL(signal_button_video(int)),this,SLOT(slot_button_video(int)));
    connect(m_CHeadWidget,SIGNAL(signal_pushbutton_passenger_flow()),this,SLOT(slot_pushbutton_passenger_flow()));
    connect(m_CHeadWidget,SIGNAL(signal_pushbutton_loop_playback(bool)),this,SLOT(slot_pushbutton_loop_playback(bool)));


    m_CHeadWidget_flow = new CHeadWidget(kEnumHeadWidgetPassengerFlow,this);
    connect(m_CHeadWidget_flow,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home_flow()));
    connect(m_CHeadWidget_flow,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back_flow()));
    connect(m_CHeadWidget_flow,SIGNAL(signal_pushbutton_quit()),this,SIGNAL(signal_pushbutton_quit()));

    connect(m_CHeadWidget_flow,SIGNAL(signal_button_menu()),this,SIGNAL(signal_button_menu()));


    connect(this,SIGNAL(signal_passenger_info(CPassengerInfo)),m_CHeadWidget_flow,SLOT(slot_passenger_info(CPassengerInfo)));

    m_CHeadWidget_flow->hide();



    m_CVideoOneWidget = NULL;
    m_CVideoFourWidget = NULL;
    m_CVideoNineWidget = NULL;
    m_CVideoSixteenWidget = NULL;
    m_menu = NULL;

 /*       QPushButton *m_pushbutton_shrink;   //收缩
    QPushButton *m_pushbutton_open;     //展开
*/
    QString str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res//main_page/16video/shrink.png").arg(":/img/res/main_page/16video/shrink.png").arg(":/img/res/main_page/16video/shrink.png");

    m_pushbutton_shrink = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_shrink,(1024 - 80)/2,HEAD_WIDGET_HEIGHT,80,50,str_sheet);

    connect(m_pushbutton_shrink,SIGNAL(clicked()),this,SLOT(slot_pushbutton_shrink()));

    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res//main_page/16video/expand.png").arg(":/img/res/main_page/16video/expand.png").arg(":/img/res/main_page/16video/expand.png");

    m_pushbutton_open = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_open,(1024 - 80)/2,0,80,50,str_sheet);

    connect(m_pushbutton_open,SIGNAL(clicked()),this,SLOT(slot_pushbutton_open()));
    m_pushbutton_open->hide();


    m_current_mode = kEnumVideoMode;

    slot_pushbutton_shrink();

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

    m_timer = new QTimer(this);
    m_timer->setInterval(3000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timer_play_video()));

    m_current_play_video = 0;

}

void CVideoWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CVideoWidget::slot_pushbutton_shrink()   //收缩
{
    m_pushbutton_open->show();
    m_pushbutton_shrink->hide();
    if(kEnumPassengerFlowMode == m_current_mode)
        m_CHeadWidget_flow->hide();
    else
        m_CHeadWidget->hide();

}
void CVideoWidget::slot_pushbutton_open()     //展开
{
    m_pushbutton_open->hide();
    m_pushbutton_shrink->show();
    if(kEnumPassengerFlowMode == m_current_mode)
        m_CHeadWidget_flow->show();
    else
        m_CHeadWidget->show();
}

void CVideoWidget::slot_button_video(int _type)
{
    show_video_event(_type,m_current_default_one);
}

void CVideoWidget::show_video_event(int _type,int _number)
{
    m_prev_video = m_current_video;
    m_current_video = _type;
    switch(_type)
    {
        case kEnumVideoOne:      //1分频
        {
            show_video_one(_number);
            CVideoOutput videoOutput;
            videoOutput.m_number = _number;
            videoOutput.m_frame = CProtocol::kEnumFrame1;
            CProtocol::getInstance()->call_json_camera_set(videoOutput);
        }break;
        case kEnumVideoFour:     //4分频
        {
            show_video_four();
         #if 0
            CVideoOutput videoOutput;
            videoOutput.m_frame = CProtocol::kEnumFrame4;
            CProtocol::getInstance()->call_json_camera_set(videoOutput);
         #endif
        }break;
        case kEnumVideoNine:     //9分频
        {
            show_video_nine();
          #if 0
            CVideoOutput videoOutput;
            videoOutput.m_frame = CProtocol::kEnumFrame9;
            CProtocol::getInstance()->call_json_camera_set(videoOutput);
          #endif
        }break;
        case kEnumVideoSixteen:   //16分频
        {

            show_video_sixteen();
        #if 0
            CVideoOutput videoOutput;
            videoOutput.m_frame = CProtocol::kEnumFrame16;
            CProtocol::getInstance()->call_json_camera_set(videoOutput);
        #endif
        }break;
    }

}




void CVideoWidget::slot_pushbutton_home()
{
    start_play_timer(false);
    m_CHeadWidget->set_movie(false);
    CCommonInterface::input_panel_control(false);
    //close();
    emit signal_pushbutton_home();
}

#if 0
#include <QDebug>
#include <QApplication>
void CVideoWidget::slot_pushbutton_menu()
{
    if(NULL == m_menu)
    {
        m_menu = new QMenu;
        m_menu->setStyleSheet(MENU_STYLE_SHEET);
        m_menu->addAction(tr("1路"), this, SLOT(slot_menu_one()));
        m_menu->addAction(tr("4路"), this, SLOT(slot_menu_four()));
        m_menu->addAction(tr("9路"), this, SLOT(slot_menu_nine()));
        m_menu->addAction(tr("16路"), this, SLOT(slot_menu_sixteen()));
    }
    //qDebug() << "slot_pushbutton_menu " << m_menu;
    //m_menu->setFocus();
     //QApplication::setActiveWindow(m_menu);
    m_menu->exec(QPoint(1024 - 120 - 120 - 10,HEAD_WIDGET_HEIGHT));

}
#endif

void CVideoWidget::slot_menu_one()
{
    show_video_event(kEnumVideoOne);
}
void CVideoWidget::slot_menu_four()
{
    show_video_event(kEnumVideoFour);
}
void CVideoWidget::slot_menu_nine()
{
    show_video_event(kEnumVideoNine);
}
void CVideoWidget::slot_menu_sixteen()
{
    show_video_event(kEnumVideoSixteen);
}

void CVideoWidget::show_video_one(int _number)      //1分频
{
    if(NULL == m_CVideoOneWidget)
    {
        m_CVideoOneWidget = new CVideoOneWidget(this);
        m_stackwidget_video->addWidget(m_CVideoOneWidget);
    }
    m_CVideoOneWidget->modify_number(_number);
    m_stackwidget_video->setCurrentWidget(m_CVideoOneWidget);
}
void CVideoWidget::show_video_four()     //4分频
{
    if(NULL == m_CVideoFourWidget)
    {
        m_CVideoFourWidget = new CVideoFourWidget(this);
        m_stackwidget_video->addWidget(m_CVideoFourWidget);
    }
    m_CVideoFourWidget->set_show_normal();
    m_stackwidget_video->setCurrentWidget(m_CVideoFourWidget);
}
void CVideoWidget::show_video_nine()     //9分频
{
    if(NULL == m_CVideoNineWidget)
    {
        m_CVideoNineWidget = new CVideoNineWidget(this);
        m_stackwidget_video->addWidget(m_CVideoNineWidget);
    }
    m_CVideoNineWidget->set_show_normal();
    m_stackwidget_video->setCurrentWidget(m_CVideoNineWidget);
}
void CVideoWidget::show_video_sixteen()   //16分频
{
    if(NULL == m_CVideoSixteenWidget)
    {
        m_CVideoSixteenWidget = new CVideoSixteenWidget(this);
        m_stackwidget_video->addWidget(m_CVideoSixteenWidget);
    }
    m_CVideoSixteenWidget->set_show_normal();
    m_stackwidget_video->setCurrentWidget(m_CVideoSixteenWidget);
}

void CVideoWidget::slot_pushbutton_passenger_flow()
{
    start_play_timer(false);

    m_current_mode = kEnumPassengerFlowMode;
    //m_CVideoNineWidget->set_current_type(CVideoNineWidget::kEnumVideoNinePassengerFlowMode);
    slot_menu_nine();
    m_CHeadWidget_flow->show();
    m_CHeadWidget->hide();
    m_CVideoNineWidget->slot_pushbutton_amplify();



    m_CVideoNineWidget->set_output_status(1,true);
    m_CVideoNineWidget->set_output_status(2,true);
    m_CVideoNineWidget->set_output_status(3,true);
    m_CVideoNineWidget->set_output_status(4,true);
    m_CVideoNineWidget->set_output_status(5,true);
    m_CVideoNineWidget->set_output_status(6,true);
    m_CVideoNineWidget->set_output_status(7,true);
    m_CVideoNineWidget->set_output_status(8,true);
    m_CVideoNineWidget->set_output_status(9,true);
    m_CVideoNineWidget->set_pushbutton_enable(false);
    m_CVideoNineWidget->set_line_hide();

#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
    //CProtocol::getInstance()->call_json_system_passenger_flow(7,1,0);
    CVideoOutput videoOutput;
    videoOutput.m_frame = CProtocol::kEnumFramePassengerFlow;

    CProtocol::getInstance()->call_json_camera_set(videoOutput);
#endif
}

void CVideoWidget::slot_pushbutton_loop_playback(bool _flag)
{
    if(_flag)
        start_play_timer(true);
    else
        start_play_timer(false);
}

//////////////////////////////////////////////客流
void CVideoWidget::slot_pushbutton_home_flow()
{
    m_CHeadWidget_flow->hide();
    m_CHeadWidget->show();
    m_current_mode = kEnumVideoMode;
    //close();

    m_CVideoNineWidget->set_output_status(1,true);
    m_CVideoNineWidget->set_output_status(2,true);
    m_CVideoNineWidget->set_output_status(3,true);
    m_CVideoNineWidget->set_output_status(4,true);
    m_CVideoNineWidget->set_output_status(5,true);
    m_CVideoNineWidget->set_output_status(6,true);
    m_CVideoNineWidget->set_output_status(7,true);
    m_CVideoNineWidget->set_output_status(8,true);
    m_CVideoNineWidget->set_output_status(9,true);
    m_CVideoNineWidget->set_pushbutton_enable(true);



    emit signal_pushbutton_home();



}

void CVideoWidget::slot_pushbutton_back_flow()
{
    m_CVideoNineWidget->set_output_status(1,true);
    m_CVideoNineWidget->set_output_status(2,true);
    m_CVideoNineWidget->set_output_status(3,true);
    m_CVideoNineWidget->set_output_status(4,true);
    m_CVideoNineWidget->set_output_status(5,true);
    m_CVideoNineWidget->set_output_status(6,true);
    m_CVideoNineWidget->set_output_status(7,true);
    m_CVideoNineWidget->set_output_status(8,true);
    m_CVideoNineWidget->set_output_status(9,true);
    m_CVideoNineWidget->set_pushbutton_enable(true);


    m_CHeadWidget_flow->hide();
    m_CHeadWidget->show();
    m_current_mode = kEnumVideoMode;
    //m_CVideoNineWidget->set_current_type(CVideoNineWidget::kEnumVideoNineVideoMode);
    show_video_event(m_prev_video,m_current_default_one);

#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
    //CProtocol::getInstance()->call_json_system_passenger_flow(7,2,0);
#endif

}
///////////////////////////////////////////////////////////////////////






void CVideoWidget::slot_vidoe_widget_event(char _frame,char _number,bool _output)
{


    switch(m_current_video)
    {
        case kEnumVideoOne:      //1分频
        {
            if(NULL == m_CVideoOneWidget)
            {
                m_CVideoOneWidget->set_output_status(_number,_output);
            }
        }break;
        case kEnumVideoFour:     //4分频
        {
            if(NULL == m_CVideoFourWidget)
            {
                if(CProtocol::kEnumFrame1 == _frame)
                    m_CVideoFourWidget->set_full_screen_status(_number,_output);
                else
                    m_CVideoFourWidget->set_output_status(_number,_output);
            }
        }break;
        case kEnumVideoNine:     //9分频
        {
            if(NULL == m_CVideoNineWidget)
            {
                if(CProtocol::kEnumFrame1 == _frame)
                    m_CVideoNineWidget->set_full_screen_status(_number,_output);
                else
                    m_CVideoNineWidget->set_output_status(_number,_output);
            }
        }break;
        case kEnumVideoSixteen:   //16分频
        {
            if(NULL == m_CVideoSixteenWidget)
            {
                if(CProtocol::kEnumFrame1 == _frame)
                    m_CVideoSixteenWidget->set_full_screen_status(_number,_output);
                else
                    m_CVideoSixteenWidget->set_output_status(_number,_output);
            }
        }break;
    }


}

void CVideoWidget::set_start_status()
{
    m_pushbutton_shrink->hide();   //收缩
    m_pushbutton_open->hide();     //展开

}

void CVideoWidget::set_main_page()
{
    slot_pushbutton_passenger_flow();


}

void CVideoWidget::slot_msg_notify(int _msg,int _event,void *_param)
{
    m_CHeadWidget_flow->head_widget_notify(_msg,_event,_param);
}


void CVideoWidget::video_init()
{

    CCommonInterface::printf_debug("video_init() " + QString::number(CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame));

    /*视频默认画面 0:16分格 1:9分格 2:4分格 3:1 4:2 5:3 6:4 7:5 8:6 9:7 10:8
      11:9 12:10 13:11 14:12 15:13 16:14 17:15 18:16*/
    m_current_default_one = 1;
    if(0 == CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame)
    {
        show_video_event(kEnumVideoSixteen);
    }
    else if(1 == CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame)
    {
        show_video_event(kEnumVideoNine);
    }
    else if(2 == CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame)
    {
        show_video_event(kEnumVideoFour);
    }
    else
    {
        int current_chanel = CCommonData::getInstance()->m_CDriverMenuInfo.video_default_frame;
        bool error_flag = true;
        current_chanel = current_chanel - 2;

        if(current_chanel > 0 && current_chanel< 17)
            error_flag = false;

        if(error_flag)
            current_chanel = 1;
        m_current_default_one = current_chanel;
        show_video_event(kEnumVideoOne,current_chanel);
    }
}

bool CVideoWidget::keyboard_value(int _value)
{
    bool ret_value = false;
    switch(_value)
    {
        case KEYPAD_KEY7_F0:
        {

        }break;
        case KEYPAD_KEY7_F1:
        {

        }break;
        case KEYPAD_KEY7_F2:
        {

        }break;
        case KEYPAD_KEY7_F3:
        {

        }break;
        case KEYPAD_KEY7_F4:
        {

        }break;
        case KEYPAD_KEY7_F5:
        {

        }break;
        case KEYPAD_KEY7_F6:
        {

        }break;
        case KEYPAD_KEY7_F7:
        {

        }break;
        case KEYPAD_KEY7_F8:
        {

        }break;
        case KEYPAD_KEY7_F9:
        {

        }break;
        case KEYPAD_KEY7_PLAY:
        {

        }break;
        case KEYPAD_KEY7_REPLAY:
        {

        }break;
        case KEYPAD_KEY7_MENU:
        {

        }break;
        case KEYPAD_KEY7_EXIT:
        {
            if(kEnumPassengerFlowMode == m_current_mode)
                slot_pushbutton_back_flow();
            else
            {
                start_play_timer(false);
                m_CHeadWidget->set_movie(false);
                emit signal_pushbutton_home();
            }
            ret_value = true;

        }break;
        case KEYPAD_KEY7_FUN:
        {

        }break;
        case KEYPAD_KEY7_UP:
        {

        }break;
        case KEYPAD_KEY7_LEFT:
        {

        }break;
        case KEYPAD_KEY7_RIGHT:
        {

        }break;
        case KEYPAD_KEY7_DOWN:
        {

        }break;
    }
    return ret_value;
}

void CVideoWidget::start_play_timer(bool _start)
{
    if(_start)
    {
        char play_time = 0;
        //0:3S 1:5S 2:10S 3:20S 4:30S 5:50S
        if(0 == CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval)
            play_time = 3;
        else if(1 == CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval)
            play_time = 5;
        else if(2 == CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval)
            play_time = 10;
        else if(3 == CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval)
            play_time = 20;
        else if(4 == CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval)
            play_time = 30;
        else if(5 == CCommonData::getInstance()->m_CDriverMenuInfo.round_robin_interval)
            play_time = 50;
        else
            play_time = 3;

        m_timer->setInterval(play_time * 1000);

        if(m_timer->isActive())
            m_timer->stop();
        m_current_play_video = 0;
        m_timer->start();
    }
    else
    {
        if(m_timer->isActive())
            m_timer->stop();
    }
}

void CVideoWidget::slot_timer_play_video()
{
    char get_index = 0;

    if(get_current_play(get_index))
    {
        if(get_index >= 0 && get_index <= 15)
            show_video_event(kEnumVideoOne,get_index + 1);
        else if(16 == get_index)
            slot_menu_four();
        else if(17 == get_index)
            slot_menu_nine();
        else if(18 == get_index)
            slot_menu_sixteen();
        //qDebug() << "slot_timer_play_video() " << QString::number(get_index);
    }
    else
    {
       // qDebug() << "slot_timer_play_video() " << QString::number(m_current_play_video);
        m_current_play_video = 0;
    }
}

bool CVideoWidget::get_current_play(char &_value)
{
    bool ret_value = false;

    for(int index = m_current_play_video;index <= 18;index++)
    {
        if(1 == CCommonData::getInstance()->m_CDriverMenuInfo.passageway[index])
        {
            _value = index;
            m_current_play_video = _value + 1;
            ret_value = true;
            break;
        }
    }

    return ret_value;
}













