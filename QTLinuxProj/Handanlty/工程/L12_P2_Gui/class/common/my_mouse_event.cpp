#include "my_mouse_event.h"

#include "../common_interface.h"

#include "LtyCommonDefine.h"

#include <QEvent>
#include <QMouseEvent>
#include <QApplication>

#include "../ltyuicallinterface.h"

extern TOUCH_ATTR_S g_prev_point;

#define SIMULATION_MOUSE_INTRVAL  (200)

CMyMouseEvent *CMyMouseEvent::m_single = NULL;

CMyMouseEvent *CMyMouseEvent::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CMyMouseEvent;
    }
    return m_single;
}


CMyMouseEvent::CMyMouseEvent(QWidget *parent) :
    QWidget(parent)
{
    m_first_press = false;
    m_timer_mouse = new QTimer(this);
    m_timeout_touch = new QTime;
    m_time_move = new QTime;
    m_timer_mouse->setInterval(SIMULATION_MOUSE_INTRVAL);
    connect(m_timer_mouse, SIGNAL(timeout()), this, SLOT(slot_timer_outtime()));

}
extern bool EmptyQueue();
bool g_touch_release = false;
void CMyMouseEvent::slot_timer_outtime()
{
//#if USB_QT_TIMER
    m_timer_mouse->stop();

    if(!m_first_press)
        return;


    //CCommonInterface::printf_debug("slot_timer_outtime() x:" + QString::number(m_current_point.x()) + ",y:" + QString::number(m_current_point.y()));
    CCommonInterface::printf_debug("CMyMouseEvent::slot_timer_outtime()  " + QString::number(SIMULATION_MOUSE_INTRVAL) + "  " + QString::number(EmptyQueue()) + " " + QString::number(m_timeout_touch->elapsed()));
    g_touch_release = true;

    touch_click_event(m_first_point.x(),m_first_point.y(),true);


//#endif
}

bool CMyMouseEvent::is_head_back(QPoint &_point)
{
    bool ret_value = false;
    do
    {
        QWidget *current_widget = NULL;
        QWidget *pWidget = NULL;
        QPoint pos;
        WidgetType widget_type = kEnumEmpty;

        pos.setX(_point.x());
        pos.setY(_point.y());


        current_widget = get_current_widget(pos,widget_type);
        if(NULL == current_widget)
        {
            //CCommonInterface::printf_debug("is_head_back() if(NULL == current_widget)");
            break;
        }
        //CCommonInterface::printf_debug("current_widget:" + current_widget->objectName());
        pos = current_widget->mapFromGlobal(pos);

        pWidget = current_widget->childAt(pos);

        if(NULL == pWidget)
        {
           // CCommonInterface::printf_debug("is_head_back() if(NULL == pWidget)");
            break;
        }
        CCommonInterface::printf_debug(pWidget->objectName());
        if(pWidget->objectName() == "CHeadWidget_back")
        {
            ret_value = true;
        }
        else
        {
           //CCommonInterface::printf_debug("is_head_back() if(pWidget->objectName() != CHeadWidget_back)");
        }

    }while(0);

    return ret_value;
}
#include <QDebug>
void CMyMouseEvent::hide_keyboard(int _x,int _y)
{
    QWidget *current_widget = NULL;
    QPoint pos;
    WidgetType widget_type = kEnumEmpty;
    QWidget *pWidget = NULL;

    do
    {
        pos.setX(_x);
        pos.setY(_y);


        current_widget = get_current_widget(pos,widget_type);

        if(NULL == current_widget)
        {
            CCommonInterface::printf_debug("current_widget NULL");
            break;
        }
        //qDebug() << "current pos 1: " << pos;
        QPoint new_pos = current_widget->mapFromGlobal(pos);
        //qDebug() << "current pos 2: " << pos;


        pWidget = current_widget->childAt(new_pos);
        if(NULL == pWidget)
        {
            //CCommonInterface::printf_debug("pWidget NULL");
            if(new_pos.x() == pos.x() && new_pos.y() == pos.y())
            {
                set_widget_focus(current_widget,widget_type,false);
            }
            break;
        }
        set_widget_focus(pWidget,widget_type,false);

    }while(0);
}

#include <QDebug>
void CMyMouseEvent::touch_click_event(int _x,int _y,bool _release)
{
    QPoint pos;

    //return;
    if(CCommonInterface::notify_ui_printf())
        CCommonInterface::printf_debug("touch_click_event fun....................start");

    pos.setX(_x);
    pos.setY(_y);
    m_current_point = pos;

    QWidget *current_widget = NULL;
    QWidget *pWidget = NULL;
    QPoint control_pos;
    QWidget *tempWidget = NULL;//pWidget;
    //bool is_input_pannel = false;
    WidgetType widget_type = kEnumEmpty;
    do
    {

        if(g_touch_release)
        {
            if(!_release)
            {
                CCommonInterface::printf_debug("void CMyMouseEvent::touch_click_event timeout");
                break;
            }
        }

        current_widget = get_current_widget(pos,widget_type);

        if(NULL == current_widget)
        {
            CCommonInterface::printf_debug("current_widget NULL");
            break;
        }

        //if(!is_input_pannel)
        pos = current_widget->mapFromGlobal(pos);

#if 0
        if(!_release && m_first_press)
            pWidget = m_press_widget;
        else
#endif
        pWidget = current_widget->childAt(pos);//get_point_widget(current_widget,pos);//current_widget->childAt(pos);

        if(NULL == pWidget)
        {
            if (kEnumMenuList == widget_type || kEnumComboList == widget_type)
            {
                touch_click_handle(current_widget,m_current_point,current_widget->pos(),_release);
                //CCommonInterface::printf_debug(" if (kEnumMenuList == widget_type || kEnumComboList == widget_type)");
            }
            else
            {

            }
            break;
        }

        //CCommonInterface::printf_debug("touch_click_event()  2");
        set_widget_focus(pWidget,widget_type);

        control_pos = pWidget->pos();
        tempWidget = pWidget;
        //CCommonInterface::printf_debug("touch_click_event()  3");
#if 0
        if(is_input_pannel)
        {
           pos = pos + control_pos;
        }
#endif
        while(1)
        {

            if(tempWidget->parentWidget() == current_widget)
            {
                //CCommonInterface::printf_debug("touch_click_event()  while(1)  1");
                break;
            }

            if(!tempWidget->parentWidget())
            {
                //CCommonInterface::printf_debug("touch_click_event()  while(1)  2");
                break;
            }
            //CCommonInterface::printf_debug("touch_click_event()  while(1)  3");
            control_pos = control_pos + tempWidget->parentWidget()->pos();
            tempWidget = tempWidget->parentWidget();

        }
        //CCommonInterface::printf_debug("touch_click_event()  4");
        if(pWidget)
        {
            //QPoint clicked_point(pos.x() - control_pos.x(),pos.y() - control_pos.y());
            touch_click_handle(pWidget,pos,control_pos,_release);
        }

    }while(0);
    if(CCommonInterface::notify_ui_printf())
        CCommonInterface::printf_debug("touch_click_event fun....................end");
}

void CMyMouseEvent::touch_click_handle(QWidget *_widget,const QPoint &_p1,const QPoint &_p2,bool _release)
{
    //CCommonInterface::printf_debug("touch_click_handle ....................start");

    QPoint clicked_point(_p1.x() - _p2.x(),_p1.y() - _p2.y());
    QMouseEvent *mEvnPress;
    QMouseEvent *mEvnRelease;
    QMouseEvent *mEvnMove;

    if(!_release)
    {//当前是按下
        if(!m_first_press)
        {//第一次按下
            m_first_point = m_current_point;
            m_time_move->restart();
            m_first_press = true;
            m_press_widget = _widget;

            mEvnPress = new QMouseEvent(QEvent::MouseButtonPress, clicked_point, Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
            bool send_ret = QApplication::sendEvent(_widget,mEvnPress);
            if(CCommonInterface::notify_ui_printf())
                qDebug() << "CMyMouseEvent::touch_click_event  MouseButtonPress" << QString::number(send_ret);

        }
        else
        {//可能是移动

            if (!_widget->inherits("QScrollBar"))
            {
                //break;
            }
            //CCommonInterface::printf_debug("CMyMouseEvent::touch_click_event current is QScrollBar");
#if 0
            if(is_input_pannel)
            {
                CCommonInterface::printf_debug("CMyMouseEvent::touch_click_event  MouseMove  is_input_pannel");
                clicked_point = QPoint(pos.x(),pos.y());
            }
#endif
            //int time_move = m_time_move->elapsed();
            //if(time_move >= 5)
            {

                //QPoint first_point = current_widget->mapFromGlobal(m_first_point);
                //clicked_point = QPoint(first_point.x() - control_pos.x(),(first_point.y() - control_pos.y()) + (pos.y() - first_point.y()));
                //CCommonInterface::printf_debug("touch_click_event()  7");
                mEvnMove = new QMouseEvent(QEvent::MouseMove, clicked_point, Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
                //CCommonInterface::printf_debug("touch_click_event()  8");
                QApplication::sendEvent(_widget,mEvnMove);

                m_time_move->restart();
                //CCommonInterface::printf_debug("touch_click_event()  9");

            }
        }

    }
    else
    {
        m_first_press = false;
        g_touch_release = false;
        mEvnRelease = new QMouseEvent(QEvent::MouseButtonRelease, clicked_point, Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
        bool send_ret = QApplication::sendEvent(_widget,mEvnRelease);
        if(CCommonInterface::notify_ui_printf())
            qDebug() << "CMyMouseEvent::touch_click_event  MouseButtonRelease" << QString::number(send_ret);
        g_prev_point.x = 0;
        g_prev_point.y = 0;

    }

   // CCommonInterface::printf_debug("touch_click_handle ....................end");
}

#include <QComboBox>
#include <QAbstractItemView>

void CMyMouseEvent::set_widget_focus(QWidget *_widget,WidgetType _widgetType,bool _flag)
{
    if (_widget->inherits("QLineEdit"))
    {
        if(_flag)
        {
            //CCommonInterface::printf_debug("CMyMouseEvent::set_widget_focus()  QLineEdit");
            _widget->setFocus();
        }

    }
    else
    {

        if(kEnumInputPanel != _widgetType && kEnumComboList != _widgetType)
        {
             QWidget *pFocusWidget = QApplication::focusWidget();

            if(pFocusWidget && pFocusWidget->inherits("QLineEdit"))
            {
                //CCommonInterface::printf_debug("CMyMouseEvent::set_widget_focus clearFocus");

                pFocusWidget->clearFocus();
                CCommonInterface::input_panel_control(false);
            }
        }
    }


}

QWidget* CMyMouseEvent::get_point_widget(QWidget *_current,QPoint _point)
{
    QWidget *pWidget = _current->childAt(_point);
    CCommonInterface::printf_debug(QString().sprintf("get_point_widget %d,%d,%d,%d",_current->pos().x(),_current->pos().y(),_current->width(),_current->height()));
    CCommonInterface::printf_debug(QString().sprintf("get_point_widget %d,%d",_point.x(),_point.y()));
    if(NULL == pWidget)
    {
        pWidget = _current;
    }

    return pWidget;
}


#ifdef CURRENT_ARM_RUN
//#include "include/syszuxim.h"
#include "./include/myinputpanelcontext.h"
#endif
QWidget *CMyMouseEvent::get_current_widget(QPoint &_point,WidgetType &_widgetType)
{
#ifdef CURRENT_ARM_RUN
    //SyszuxIM *pSyszux = (SyszuxIM *)CCommonInterface::get_method_widget();
    MyInputPanelContext *pInputPanel = (MyInputPanelContext *)CCommonInterface::get_method_widget();
    QWidget *pMethodWidget = NULL;



    QWidget *pPopupWidget = NULL;
    pPopupWidget = QApplication::activePopupWidget();

    if(pPopupWidget)
    {
        if (pPopupWidget->inherits("QMenu"))
        {
            CCommonInterface::printf_debug("get_current_widget QMenu");
            _widgetType = kEnumMenuList;
            return pPopupWidget;
        }
    }

        //_inputpannel = false;
    QWidget *pFocusWidget = QApplication::focusWidget();
    QWidget *parentWidget = NULL;
    if(pFocusWidget)
    {
        parentWidget = pFocusWidget->parentWidget();

        if (pFocusWidget->inherits("QListView"))
        {
            QPoint focusPoint = pFocusWidget->mapToGlobal(pFocusWidget->pos());


            if (parentWidget && parentWidget->inherits("QComboBoxPrivateContainer"))
            {
#if 0
                QRect listview_rect;
                listview_rect.setX(focusPoint.x());
                listview_rect.setY(focusPoint.y());
                listview_rect.setWidth(pFocusWidget->width());
                listview_rect.setHeight(pFocusWidget->height());

                if(listview_rect.contains(_point))
#endif
                if(pFocusWidget->isVisible())
                {
                    _widgetType = kEnumComboList;
                    //qDebug() << "CMyMouseEvent::get_current_widget focus parent is QComboBox";
                    return pFocusWidget;
                }

            }
        }


    }

    if(pInputPanel/*pSyszux*/ != NULL)
    {
        //CCommonInterface::printf_debug("get_current_widget 1");
         pMethodWidget  = pInputPanel->current_window(); //pSyszux->current_window();
         //qDebug() << pMethodWidget;
         if(NULL != pMethodWidget)
         {
            //CCommonInterface::printf_debug("get_current_widget 2");
            if(pMethodWidget->isVisible())
            {
                QRect method_rect;
                QPoint method_widget_pos;

                method_widget_pos = pMethodWidget->mapToGlobal(pMethodWidget->pos());


                //method_rect.setX(method_widget_pos.x());
                //method_rect.setY(method_widget_pos.y());
                method_rect.setX(pMethodWidget->pos().x());
                method_rect.setY(pMethodWidget->pos().y());
                method_rect.setWidth(pMethodWidget->width());
                method_rect.setHeight(pMethodWidget->height());
               // CCommonInterface::printf_debug("get_current_widget pMethodWidget");


                if(method_rect.contains(_point))
                {
                   // CCommonInterface::printf_debug("get_current_widget pMethodWidget");
                    _widgetType = kEnumInputPanel;
                    return pMethodWidget;
                }
            }
         }


    }



    //qDebug() << "CMyMouseEvent::get_current_widget focus parent Widget " << parentWidget;
    //qDebug() << "CMyMouseEvent::get_current_widget focusWidget " << pFocusWidget;
#endif
    return QApplication::activeWindow();
}


void CMyMouseEvent::reset_timer()
{
    m_timer_mouse->disconnect();
    if(m_timer_mouse->isActive())
    {
        m_timer_mouse->stop();
    }
    connect(m_timer_mouse, SIGNAL(timeout()), this, SLOT(slot_timer_outtime()));
    m_timer_mouse->start();
    m_timeout_touch->restart();
}

void CMyMouseEvent::stop_timer()
{
    m_timer_mouse->disconnect();
    if(m_timer_mouse->isActive())
    {
        m_timer_mouse->stop();
    }
}

QPoint CMyMouseEvent::get_first_point()
{
    return m_first_point;
}












