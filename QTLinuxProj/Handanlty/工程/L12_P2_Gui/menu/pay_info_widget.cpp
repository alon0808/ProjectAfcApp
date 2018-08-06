

#include "pay_info_widget.h"

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"

#include "class/common_interface.h"

#define NEW_VERSION 1

CPayInfoWidget::CPayInfoWidget(QWidget *parent) :
    QDialog(parent)
{

    this->setFixedSize(1024,768);
    this->setObjectName("CPayInfoWidget");
    this->setStyleSheet("QDialog#CPayInfoWidget{background-color:#000000;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetPayInfo,this);
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
#if NEW_VERSION
    m_widget_info1 = new QWidget(this);
    m_widget_info1->setFixedSize(144,156);
    m_widget_info1->move(83,96);
    m_widget_info1->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_name.png);}");

    m_widget_info2 = new QWidget(this);
    m_widget_info2->setFixedSize(144,156);
    m_widget_info2->move(249,96);
    m_widget_info2->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_ic_number_of _transactions.png);}");

    m_widget_info3 = new QWidget(this);
    m_widget_info3->setFixedSize(144,156);
    m_widget_info3->move(415,96);
    m_widget_info3->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_ic_transaction_amount.png);}");

    m_widget_info4 = new QWidget(this);
    m_widget_info4->setFixedSize(144,156);
    m_widget_info4->move(83,272);
    m_widget_info4->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_qr_icon_number_transactions.png);}");

    m_widget_info5 = new QWidget(this);
    m_widget_info5->setFixedSize(144,156);
    m_widget_info5->move(249,272);
    m_widget_info5->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_qr_transaction_amount.png);}");

//    m_widget_info5 = new QWidget(this);
//    m_widget_info5->setFixedSize(144,156);
//    m_widget_info5->move(415,272);
//    m_widget_info5->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_qr_transaction_amount.png);}");

    QString str_show = "";
    QString str_sheet = "";
#if  1


    QString str_money = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#ffffff;}";
    str_show = tr("张三");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:22px;color:#ffffff;}";
    m_label_info1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info1,83,215,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("21");
    m_label_info2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2,249,216,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("次");
    m_label_info2_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2_2,324,219,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info2,m_label_info2_2,249); //IC卡-交易次数

    str_show = tr("236.3");
    m_label_info3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3,415,216,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("元");
    m_label_info3_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3_2,507,219,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info3,m_label_info3_2,415); //IC卡-交易金额

    str_show = tr("16");
    m_label_info4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4,83,391,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("次");
    m_label_info4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4_2,160,395,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info4,m_label_info4_2,83); //二维码-交易次数
    //5~8
    //str_show = tr("EW85426");
    //m_label_info5 = new QLabel(this);
    //CCommonInterface::init_label_text(m_label_info5,34,391,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("154.6");
    m_label_info6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info6,249,392,144,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("元");
    m_label_info6_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info6_2,342,395,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info6,m_label_info6_2,249); //二维码-交易金额

//      str_show = tr("154.6");
//    m_label_info6 = new QLabel(this);
//    CCommonInterface::init_label_text(m_label_info6,415,392,144,25,str_show,str_sheet,Qt::AlignCenter,22);

//    str_show = tr("元");
//    m_label_info6_2 = new QLabel(this);
//    CCommonInterface::init_label_text(m_label_info6_2,437,395,20,18,str_show,str_money,Qt::AlignCenter,16);

//    set_show_money(m_label_info6,m_label_info6_2,415); //二维码-交易金额

#endif

    /////////////--------------------------
    str_show = tr("司机姓名");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:14px;color:#8ea2c0;}";
    m_label_info1_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info1_1,126,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("IC卡-交易次数");
    m_label_info2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2_1,275,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("IC卡-交易金额");
    m_label_info3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3_1,442,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("二维码-交易次数");
    m_label_info4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4_1,102,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("二维码-交易金额");
    m_label_info5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info5_1,267,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);

//    str_show = tr("二维码-交易金额");
//    m_label_info5_1 = new QLabel(this);
//    CCommonInterface::init_label_text(m_label_info5_1,432,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);

#else
    m_widget_info1 = new QWidget(this);
    m_widget_info1->setFixedSize(128,156);
    m_widget_info1->move(34,96);
    m_widget_info1->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_name.png);}");

    m_widget_info2 = new QWidget(this);
    m_widget_info2->setFixedSize(128,156);
    m_widget_info2->move(182,96);
    m_widget_info2->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_IC_money1.png);}");

    m_widget_info3 = new QWidget(this);
    m_widget_info3->setFixedSize(128,156);
    m_widget_info3->move(330,96);
    m_widget_info3->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_Qr_Code_money1.png);}");

    m_widget_info4 = new QWidget(this);
    m_widget_info4->setFixedSize(128,156);
    m_widget_info4->move(478,96);
    m_widget_info4->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_total_money1.png);}");

    m_widget_info5 = new QWidget(this);
    m_widget_info5->setFixedSize(128,156);
    m_widget_info5->move(34,272);
    m_widget_info5->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_driver_number.png);}");

    m_widget_info6 = new QWidget(this);
    m_widget_info6->setFixedSize(128,156);
    m_widget_info6->move(182,272);
    m_widget_info6->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_IC_money2.png);}");

    m_widget_info7 = new QWidget(this);
    m_widget_info7->setFixedSize(128,156);
    m_widget_info7->move(330,272);
    m_widget_info7->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_Qr_Code_money2.png);}");

    m_widget_info8 = new QWidget(this);
    m_widget_info8->setFixedSize(128,156);
    m_widget_info8->move(478,272);
    m_widget_info8->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/pay_info/icon_total_money2.png);}");

    QString str_show = "";
    QString str_sheet = "";

    QString str_money = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#ffffff;}";
    str_show = tr("张三");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:22px;color:#ffffff;}";
    m_label_info1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info1,34,215,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("154.4");
    m_label_info2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2,182,216,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("元");
    m_label_info2_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2_2,267,219,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info2,m_label_info2_2,182); //IC卡-当趟金额

    str_show = tr("236.3");
    m_label_info3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3,330,216,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("元");
    m_label_info3_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3_2,414,219,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info3,m_label_info3_2,330); //二维码-当趟金额

    str_show = tr("731.1");
    m_label_info4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4,478,216,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("元");
    m_label_info4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4_2,564,219,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info4,m_label_info4_2,478); //合计-当趟金额
    //5~8
    str_show = tr("EW85426");
    m_label_info5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info5,34,391,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("2398.4");
    m_label_info6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info6,182,391,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("元");
    m_label_info6_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info6_2,274,394,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info6,m_label_info6_2,182); //IC卡-当班金额

    str_show = tr("2426.0");
    m_label_info7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info7,351,391,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("元");
    m_label_info7_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info7_2,422,394,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info7,m_label_info7_2,330); //二维码-当班金额

    str_show = tr("8398.4");
    m_label_info8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info8,499,391,125,25,str_show,str_sheet,Qt::AlignCenter,22);

    str_show = tr("元");
    m_label_info8_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info8_2,570,394,20,18,str_show,str_money,Qt::AlignCenter,16);

    set_show_money(m_label_info8,m_label_info8_2,478); //合计-当班金额


    /////////////--------------------------
    str_show = tr("司机姓名");
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:14px;color:#8ea2c0;}";
    m_label_info1_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info1_1,70,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("IC卡-当趟金额");
    m_label_info2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info2_1,201,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("二维码-当趟金额");
    m_label_info3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info3_1,342,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("合计-当趟金额");
    m_label_info4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info4_1,497,166,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("司机编号");
    m_label_info5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info5_1,70,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("IC卡-当班金额");
    m_label_info6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info6_1,201,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("二维码-当班金额");
    m_label_info7_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info7_1,342,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);

    str_show = tr("合计-当班金额");
    m_label_info8_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_info8_1,497,342,140,20,str_show,str_sheet,Qt::AlignLeft,14);
#endif

}

void CPayInfoWidget::set_show_money(QLabel *_label1,QLabel *_label2,int _start_x)
{
    QRect label1_rect = _label1->rect();
    QRect label2_rect = _label2->rect();
    QPoint label1_point = _label1->pos();
    QPoint label2_point = _label2->pos();

    int label1_width = CCommonInterface::get_string_length(_label1->text(),22);
    int label2_width = CCommonInterface::get_string_length(_label2->text(),16);

    int new_x = _start_x + (144 - label1_width - label2_width)/2;

    _label1->setFixedWidth(label1_width);
    _label1->move(new_x,label1_point.y());
    _label2->move(new_x + label1_width,label2_point.y());

}

void CPayInfoWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}
/*
    QString driver_name;
    QString ic_card_tang;
    int qr_tang;
    QString driver_id;
    QString ic_card_ban;
    int qr_ban;
*/

#include <QDebug>


void CPayInfoWidget::slot_paycard_record_widget(const CGetQrPayInfo & _info)
{
    //qDebug() << "slot_paycard_record_widget()";
    qDebug()<<"slot_paycard_record_widget ic num:"<<_info.ic_card_number<<"ic money："<<_info.ic_card_tang;
    m_label_info1->setText(_info.driver_name);
    m_label_info2->setText(QString::number(_info.ic_card_number));//ic 交易次数
    m_label_info3->setText(_info.ic_card_tang); //ic 交易金额
    m_label_info4->setText(QString::number(_info.qr_number));// 二维码交易次数
    // float qr_money = _info.qr_tang.toFloat();// + _info.qr_ban.toFloat();
    m_label_info6->setText(_info.qr_tang); //二维码交易金额

      //ic_card_number = 0;
        //qr_number = 0;
    set_show_money(m_label_info2,m_label_info2_2,249); //IC卡-交易次数
    set_show_money(m_label_info3,m_label_info3_2,415); //IC卡-交易金额
    set_show_money(m_label_info4,m_label_info4_2,83); //二维码-交易次数
    set_show_money(m_label_info6,m_label_info6_2,249); //二维码-交易金额


#if 0
      m_label_info1->setText(_info.driver_name);
      m_label_info2->setText(_info.ic_card_tang);
      m_label_info3->setText(_info.qr_tang);
      float total_tang = _info.ic_card_tang.toFloat() + _info.qr_tang.toFloat();
      m_label_info4->setText(QString::number(total_tang,'f',1));

      m_label_info5->setText(_info.driver_id);
      m_label_info6->setText(_info.ic_card_ban);
      m_label_info7->setText(_info.qr_ban);
      float total_ban = _info.ic_card_ban.toFloat() + _info.qr_ban.toFloat();
      m_label_info8->setText(QString::number(total_ban,'f',1));

    set_show_money(m_label_info2,m_label_info2_2,182); //IC卡-当趟金额
    set_show_money(m_label_info3,m_label_info3_2,330); //二维码-当趟金额
    set_show_money(m_label_info4,m_label_info4_2,478); //合计-当趟金额
    set_show_money(m_label_info6,m_label_info6_2,182); //IC卡-当班金额
    set_show_money(m_label_info7,m_label_info7_2,330); //二维码-当班金额
    set_show_money(m_label_info8,m_label_info8_2,478); //合计-当班金额
#endif

}










