#include "glappeventfilter.h"

#include <QPoint>
#include <QKeyEvent>
#include <QDebug>

#include "mainwindow.h"

GlAppEventFilter *GlAppEventFilter::m_single = NULL;

GlAppEventFilter *GlAppEventFilter::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new GlAppEventFilter;
    }
    return m_single;
}


GlAppEventFilter::GlAppEventFilter(QObject *parent) :
    QObject(parent)
{
}

#ifdef CURRENT_ARM_RUN
#include "./include/myinputpanelcontext.h"
#include <QWSServer>
#endif
#include "class/common_interface.h"
bool GlAppEventFilter::eventFilter(QObject *pObj, QEvent *pEvent)
{
    bool filter_event = false;

    QMouseEvent *mouse_event = static_cast<QMouseEvent*>(pEvent);
    switch (pEvent->type()) {
    case QEvent::MouseButtonRelease:
    {
#ifdef APPLICATION_TYPE_P2
#ifdef CURRENT_ARM_RUN
    if(!QWSServer::isCursorVisible())
        QWSServer::setCursorVisible(true);
#endif
#endif
    }break;
    case QEvent::MouseButtonPress:
    {
#ifdef APPLICATION_TYPE_P2
#ifdef CURRENT_ARM_RUN
    if(!QWSServer::isCursorVisible())
        QWSServer::setCursorVisible(true);
#endif
#endif
    }break;
    case QEvent::MouseMove:
    {
#ifdef APPLICATION_TYPE_P2
#ifdef CURRENT_ARM_RUN
    if(!QWSServer::isCursorVisible())
        QWSServer::setCursorVisible(true);
#endif
#endif
    }break;
    default:
        break;
    }

#if 0
    if(QEvent::KeyRelease == pEvent->type())
    {

        //if("QScrollArea" == pObj->objectName() || "CNavigateLeft" == pObj->objectName())
        //    return true;

        if(pObj->inherits("QComboBox"))
        {
            int i;
            i = 0;
            qDebug() << "this is QComboBox 1";
            filter_event = true;
        }

        if("QComboBox"  == pObj->objectName())
        {
            int i;
            i = 0;
            qDebug() << "this is QComboBox 2";
        }
        if(pObj->inherits("QScrollArea"))
        {
            int i;
            i = 0;
            qDebug() << "this is QScrollArea";
            filter_event = true;
            //return true;
        }

    }
    bool bHandle = false;
    if("MainWindow" == pObj->objectName() ||
       "CVoiceDialog" == pObj->objectName() ||
       "CMyMessageBox" == pObj->objectName() ||
       "CCardDialog" == pObj->objectName() ||
       "CMainMessageDialog" == pObj->objectName() ||
       "CMyExport" == pObj->objectName() ||
       "CPasswordDialog" == pObj->objectName() ||
       "CPassword1Dialog" == pObj->objectName() ||
       "CProgressBox" == pObj->objectName() ||
       "CNetworkWidget" == pObj->objectName() ||
       "CServiceDialog" == pObj->objectName() ||
       pObj->inherits("QComboBox") ||
       pObj->inherits("QListView"))
    {
        bHandle = true;
    }

    if (QEvent::KeyRelease == pEvent->type() && bHandle)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(pEvent);

        qDebug() << pObj;
        qDebug() << keyEvent->key();
        //MainWindow::handleKeyEvent
        switch(keyEvent->key())
        {

            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Left:
            case Qt::Key_Right:
            case Qt::Key_Return:
            case Qt::Key_F1:
            case Qt::Key_F2:
            //case Qt::Key_Enter:
            {
                MainWindow::getInstance()->handleKeyEvent(pObj,keyEvent);
                return true;
            }break;


#if 0
            case Qt::Key_Up:
            {
                //MainWindow::getInstance()->handleKeyEvent(keyEvent);
                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Up");
            }break;
            case Qt::Key_Down:
            {
                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Down");
                //MainWindow::getInstance()->handleKeyEvent(keyEvent);
            }break;
            case Qt::Key_Left:
            {
                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Left");
                //MainWindow::getInstance()->handleKeyEvent(keyEvent);
            }break;
            case Qt::Key_Right:
            {
                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Right");
                //MainWindow::getInstance()->handleKeyEvent(keyEvent);
            }break;
#endif
        }

    }


#ifdef CURRENT_ARM_RUN
    if (NULL != pEvent)
    {
        //CCommonInterface::printf_debug("GlAppEventFilter::eventFilter 0");

        if (QEvent::MouseButtonRelease == pEvent->type())
        {
            do
            {
                //CCommonInterface::printf_debug("GlAppEventFilter::eventFilter 1");

                QPoint glPos = static_cast<QMouseEvent*>(pEvent)->globalPos();
                MyInputPanelContext *pInputPanel = (MyInputPanelContext *)CCommonInterface::get_method_widget();

                if(NULL == pInputPanel)
                {
                    break;
                }
                //CCommonInterface::printf_debug("get_current_widget 1");
                QWidget *pMethodWidget = NULL;
                pMethodWidget  = pInputPanel->current_window();
                //qDebug() << pMethodWidget;
                if(NULL == pMethodWidget)
                {
                    break;
                }
                //CCommonInterface::printf_debug("get_current_widget 2");
                if(!pMethodWidget->isVisible())
                {
                    break;
                }
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


                if(!method_rect.contains(glPos))
                {
                    CCommonInterface::input_panel_control(false);
                   // CCommonInterface::printf_debug("GlAppEventFilter::eventFilter method close");
                }
                else
                {
                  //  CCommonInterface::printf_debug("GlAppEventFilter::eventFilter method");
                }

            }while(0);


        }
    }
#endif
    if(filter_event)
    {
        qDebug() << "no handle event...";
        return false;
    }
#endif

#ifdef APPLICATION_TYPE_P2
#ifdef CURRENT_ARM_RUN
        do
        {
            if (QEvent::MouseButtonRelease != pEvent->type())
            {
                break;
            }

            QPoint glPos = static_cast<QMouseEvent*>(pEvent)->globalPos();
            MyInputPanelContext *pInputPanel = (MyInputPanelContext *)CCommonInterface::get_method_widget();

            if(NULL == pInputPanel)
            {
                break;
            }

            QWidget *pMethodWidget = NULL;
            pMethodWidget  = pInputPanel->current_window();

            if(NULL == pMethodWidget)
            {
                break;
            }

            if(!pMethodWidget->isVisible())
            {
                break;
            }

            CMyMouseEvent::getInstance()->hide_keyboard(glPos.x(),glPos.y());


        }while(0);



#endif
#endif
    return QObject::eventFilter(pObj,pEvent);
}











