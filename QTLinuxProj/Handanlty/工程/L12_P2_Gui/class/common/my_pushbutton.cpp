#include "my_pushbutton.h"

#include "class/common_interface.h"

CMyPushButton::CMyPushButton(QWidget *parent) :
    QPushButton(parent)
{
    m_enable = true;
    m_sheet_open = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/systemManage/button_open.png");;
    m_sheet_close = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/systemManage/button_close.png");
    set_pushbutton_enable(true);
    connect(this,SIGNAL(clicked()),this,SLOT(slot_pushbutton_clicked()));

}

void CMyPushButton::slot_pushbutton_clicked()
{
    if(m_enable)
    {
        set_pushbutton_enable(false);
        m_enable = false;
    }
    else
    {
        set_pushbutton_enable(true);
        m_enable = true;
    }
}

void CMyPushButton::set_pushbutton_sheet(const QString &_open,const QString &_close)
{
    m_sheet_open = _open;
    m_sheet_close = _close;

    if(m_enable)
       CCommonInterface::modify_button_sheet(this,m_sheet_open);
    else
       CCommonInterface::modify_button_sheet(this,m_sheet_close);


}

void CMyPushButton::set_pushbutton_enable(bool _enable)
{
    if(_enable)
    {
       CCommonInterface::modify_button_sheet(this,m_sheet_open);
       m_enable = true;
    }
    else
    {
       CCommonInterface::modify_button_sheet(this,m_sheet_close);
       m_enable = false;
    }

}
bool CMyPushButton::get_pushbutton_enable()
{
    return m_enable;
}
