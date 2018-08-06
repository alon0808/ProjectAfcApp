

#include "shundown_dialog.h"
#include "class/common_interface.h"

CShundownDialog::CShundownDialog(QWidget *parent) :
    QDialog(parent)
{
        this->setFixedSize(1024,768);
    this->setObjectName("CShundownDialog");
    this->setStyleSheet("QDialog#CShundownDialog{border-image:url(:/img/res/main_page/shundown.png);}");//1f2024
    this->setWindowFlags(Qt::FramelessWindowHint/*|Qt::WindowStaysOnTopHint*/);


    QString str_show = tr("系统重启");
    m_label_show = new QLabel(this);
    QString str_sheet = "QLabel{border:0px solid #80e3ff;background:transparent;font:77px;color:#80E3FF;}";
    CCommonInterface::init_label_text(m_label_show,391 - 40,349 - 20,0,90,str_show,str_sheet,Qt::AlignCenter,77);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

}

void CShundownDialog::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}
