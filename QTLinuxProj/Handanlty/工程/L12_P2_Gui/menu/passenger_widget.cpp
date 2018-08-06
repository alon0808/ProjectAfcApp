

#include "passenger_widget.h"

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"

#include "class/common_interface.h"


CPassengerWidget::CPassengerWidget(QWidget *parent) :
    QDialog(parent)
{

    this->setFixedSize(1024,768);
    this->setObjectName("CPassengerWidget");
    this->setStyleSheet("QDialog#CPassengerWidget{background-color:#000000;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetPassenger,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SIGNAL(signal_pushbutton_menu()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SIGNAL(signal_pushbutton_home()));
#if 0
    QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(250,768 - 82);
    pWidget->move(0,82);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");
#endif

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

    m_widget_info1 = new QWidget(this);
    m_widget_info1->setFixedSize(144,156);
    m_widget_info1->move(83,96);
    m_widget_info1->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/passenger/icon_get_on_number.png);}");

    m_widget_info2 = new QWidget(this);
    m_widget_info2->setFixedSize(144,156);
    m_widget_info2->move(249,96);
    m_widget_info2->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/passenger/icon_debus_number.png);}");

    m_widget_info3 = new QWidget(this);
    m_widget_info3->setFixedSize(144,156);
    m_widget_info3->move(415,96);
    m_widget_info3->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/passenger/icon_headcount.png);}");

    m_widget_info4 = new QWidget(this);
    m_widget_info4->setFixedSize(144,156);
    m_widget_info4->move(83,272);
    m_widget_info4->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/passenger/icon_passenger_flow1.png);}");

    m_widget_info5 = new QWidget(this);
    m_widget_info5->setFixedSize(144,156);
    m_widget_info5->move(249,272);
    m_widget_info5->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/passenger/icon_passenger_flow2.png);}");

    QString str_show = "";
    QString str_sheet = "";




    QString str_money = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#ffffff;}";
    str_show = tr("0");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:22px;color:#ffffff;}";
    m_label_info1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info1,83,216,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("人");
    m_label_info1_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info1_2,159,219,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info1,m_label_info1_2,83); //上车人数

    str_show = tr("0");
    m_label_info2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2,249,216,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("人");
    m_label_info2_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2_2,325,219,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info2,m_label_info2_2,249); //下车人数

    str_show = tr("0");
    m_label_info3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3,415,216,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("人");
    m_label_info3_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3_2,490,219,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info3,m_label_info3_2,415); //车内人数

    str_show = tr("0");
    m_label_info4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4,83,388,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("人");
    m_label_info4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4_2,178,394,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info4,m_label_info4_2,83); //当趟客流
    //5~8
    str_show = tr("0");
    m_label_info5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info5,249,388,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("人");
    m_label_info5_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info5_2,344,394,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info5,m_label_info5_2,249); //当班客流


    /////////////--------------------------
    str_show = tr("上车人数");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:14px;color:#8ea2c0;}";
    m_label_info1_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info1_1,126,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("下车人数");
    m_label_info2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2_1,292,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("车内人数");
    m_label_info3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3_1,458,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("当趟客流");
    m_label_info4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4_1,126,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("当班客流");
    m_label_info5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info5_1,292,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);




}

void CPassengerWidget::set_show_money(QLabel *_label1,QLabel *_label2,int _start_x)
{
    QRect label1_rect = _label1->rect();
    QRect label2_rect = _label2->rect();
    QPoint label1_point = _label1->pos();
    QPoint label2_point = _label2->pos();

    int label1_width = CCommonInterface::get_string_length(_label1->text(),22);
    int label2_width = CCommonInterface::get_string_length(_label2->text(),16);

    int new_x = _start_x + (128 - label1_width - label2_width)/2;

    _label1->setFixedWidth(label1_width);
    _label1->move(new_x,label1_point.y());
    _label2->move(new_x + label1_width,label2_point.y());

}

void CPassengerWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CPassengerWidget::slot_passenger_widget(const CGetPassengerInfo &_info)
{

      m_label_info1->setText(QString::number(_info.up));
      m_label_info2->setText(QString::number(_info.down));
      m_label_info3->setText(QString::number(_info.within));

      m_label_info4->setText(QString::number(_info.tang));
      m_label_info4->setText(QString::number(_info.ban));

    set_show_money(m_label_info1,m_label_info1_2,83); //上车人数
    set_show_money(m_label_info2,m_label_info2_2,249); //下车人数
    set_show_money(m_label_info3,m_label_info3_2,415); //车内人数
    set_show_money(m_label_info4,m_label_info4_2,83); //当趟客流
    set_show_money(m_label_info5,m_label_info5_2,249); //当班客流

}










