

#include "videotape_export.h"
#include "./class/common_interface.h"
#include "class/ltystring.h"
#include "LtyCommonStyle.h"


#define MESSAGEBOX_BUTTON_WIDTH   140
#define MESSAGEBOX_BUTTON_HEIGHT   65

#define STR_MOUNTED     tr("已挂载")
#define STR_UMOUNT     tr("未挂载")




CVideotapeExport::CVideotapeExport(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(540,488);
    this->setObjectName("CVideotapeExport");
    this->setStyleSheet("QDialog#CVideotapeExport{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    //标题
    QString str_show = "录像导出";
    QString str_sheet = "QLabel{border:0px solid #ffffff;font:46px;background:#275ba7;color:#80e3ff;}";
    m_label_title = new QLabel(this);
    CCommonInterface::init_label_text(m_label_title,1,1,this->width() - 2,77,str_show,str_sheet);

    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png");

    m_pushbutton_close = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 10 - 40,(77 - 52)/2,40,52,str_sheet);
    str_sheet = "QLabel{border:0px;font:18px;color:#8ea2c0;background:transparent;}";

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));

    str_show = tr("导出文件数量:");
    m_label_1 = new QLabel(this);
    int str_length = CCommonInterface::init_label_text(m_label_1,383 - 242,254 - 140,0,40,str_show,str_sheet,Qt::AlignRight);

    str_show = tr("2个");
    m_label_1_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_1_1,526 - 242,254 - 140,0,40,str_show,str_sheet,Qt::AlignLeft);


    str_show = tr("文件大小:");
    m_label_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_2,383 - 242 - 50,305 - 140,str_length,40,str_show,str_sheet,Qt::AlignRight);

    str_show = tr("127.6MB");
    m_label_2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_2_1,526 - 242,306 - 140,0,40,str_show,str_sheet,Qt::AlignLeft);

    str_show = tr("USB设备:");
    m_label_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_3,383 - 242 - 50,356 - 140,str_length,40,str_show,str_sheet,Qt::AlignRight);
    m_label_3->hide();

    str_show = STR_MOUNTED;
    m_label_3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_3_1,526 - 242,357 - 140,0,40,str_show,str_sheet,Qt::AlignLeft);
    m_label_3_1->hide();

    str_show = tr("原始数据");
    m_label_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_4,526 - 242,410 - 140,0,40,str_show,str_sheet,Qt::AlignLeft);


    str_show = tr("AVI数据");
    m_label_5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_5,526 - 242,464 - 140,0,40,str_show,str_sheet,Qt::AlignLeft);

#if 0
    m_checkbox_1 = new QCheckBox(this);
    m_checkbox_1->setFixedSize(70,70);
    m_checkbox_1->move(526 - 242 - 70 - 10,410 - 140 - 30 + 3);
    m_checkbox_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_1->setFocusPolicy(Qt::NoFocus);

    m_checkbox_2 = new QCheckBox(this);
    m_checkbox_2->setFixedSize(70,70);
    m_checkbox_2->move(526 - 242 - 70 - 10,464 - 140 - 30 + 3);
    m_checkbox_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_2->setFocusPolicy(Qt::NoFocus);
#endif
    m_current_select = kEnumTypeSource;

    m_pushbutton_source = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_source,526 - 242 - 70 - 10,410 - 140 - 30 + 3,70,70,"");

    m_pushbutton_avi = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_avi,526 - 242 - 70 - 10,464 - 140 - 30 + 3,70,70,"");

    connect(m_pushbutton_source,SIGNAL(clicked()),this,SLOT(slot_pushbutton_source()));
    connect(m_pushbutton_avi,SIGNAL(clicked()),this,SLOT(slot_pushbutton_avi()));

    CCommonInterface::modify_button_sheet(m_pushbutton_source,BUTTON_SELECT);
    CCommonInterface::modify_button_sheet(m_pushbutton_avi,BUTTON_SELECT_NO);

    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,(this->width() - MESSAGEBOX_BUTTON_WIDTH * 2 - 10)/2,this->height() - 35 - MESSAGEBOX_BUTTON_HEIGHT,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);
    m_pushbutton1->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,(this->width() - MESSAGEBOX_BUTTON_WIDTH * 2 - 10)/2 + MESSAGEBOX_BUTTON_WIDTH + 10,this->height() - 35 - MESSAGEBOX_BUTTON_HEIGHT,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);
    m_pushbutton2->setText(CLtyString::getInstance()->m_cancel);

    m_button_type = KEnumButtonCancel;

    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));


}

void CVideotapeExport::slot_pushbutton_close()
{
    close();
}

void CVideotapeExport::slot_pushbutton1()
{
     m_button_type = KEnumButtonOk;
     hide();
}
void CVideotapeExport::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();
}

CVideotapeExport::ButtonType CVideotapeExport::get_result()
{
    return m_button_type;
}

void CVideotapeExport::slot_pushbutton_source()
{
    m_current_select = kEnumTypeSource;
    CCommonInterface::modify_button_sheet(m_pushbutton_source,BUTTON_SELECT);
    CCommonInterface::modify_button_sheet(m_pushbutton_avi,BUTTON_SELECT_NO);

}
void CVideotapeExport::slot_pushbutton_avi()
{
    m_current_select = kEnumTypeAvi;
    CCommonInterface::modify_button_sheet(m_pushbutton_source,BUTTON_SELECT_NO);
    CCommonInterface::modify_button_sheet(m_pushbutton_avi,BUTTON_SELECT);
}

void CVideotapeExport::set_data(int _number,QString _size,bool _usb_state)
{
    m_label_1_1->setText(QString::number(_number) + tr(" 个"));
    m_label_2_1->setText(_size);
    if(_usb_state)
        m_label_3_1->setText(STR_MOUNTED);
    else
        m_label_3_1->setText(STR_UMOUNT);
}

































