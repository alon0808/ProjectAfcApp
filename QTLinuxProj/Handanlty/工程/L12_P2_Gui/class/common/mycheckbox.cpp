#include "mycheckbox.h"

#include "iconLocation.h"

MyCheckBox::MyCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
    m_enableUpdate = true;

    QString qssStr = "\
                      QCheckBox::indicator {\
                      width: %1px;\
                      height: %2px;\
                      }\
                      QCheckBox{\
                      background:transparent;\
                      spacing: 0px;\
                      font:12px;\
                      color:#494949;\
                      }\
                      QCheckBox:hover{color:#22b567;}\
                      QCheckBox::indicator:unchecked {\
                      image: url(%3);\
                      }\
                      QCheckBox::indicator:checked {\
                      image: url(%4);\
                      }\
                      QCheckBox::indicator:unchecked:hover{\
                      image: url(%5);\
                      }\
                      QCheckBox:!enabled{\
                      color:#999999; \
                      }\
                      QCheckBox::indicator!enabled{\
                      image: url(%6);\
                      }";

    setStyleSheet(qssStr.arg(QString::number(NAVIGATE_MIDDLE_CHECKBOX_ICON_WIDTH),QString::number(NAVIGATE_MIDDLE_CHECKBOX_ICON_HEIGHT),COM_CIRCLE_UNSELECT, COM_CIRCLE_SELECT, COM_CIRCLE_UNSELECT, COM_CIRCLE_UNSELECT));


    connect(this, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
}

MyCheckBox::~MyCheckBox()
{

}

void MyCheckBox::setEnableUpdate(bool flag)
{
    m_enableUpdate = flag;
}

void MyCheckBox::slotStateChanged(int state)
{
    if (m_enableUpdate)
    {
        //qDebug()<<"signalStateChanged:"<<state;
        emit signalStateChanged(state);
    }
    else
    {
        m_enableUpdate = true;
    }
}
