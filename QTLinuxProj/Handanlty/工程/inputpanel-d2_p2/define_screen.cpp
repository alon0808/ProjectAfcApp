

#include "define_screen.h"
//#include "class/common_interface.h"
#include <QDebug>

#define SCREEN_DEBUG   0

CDefineScreen::CDefineScreen(QWidget *parent) :
    QWidget(parent)
{
#if defined(APPLICATION_TYPE_D2)
    m_factor_x = 1024/1024.0;
    m_factor_y = 600/768.0;
#elif defined(APPLICATION_TYPE_P2)
    m_factor_x = 640/1024.0;
    m_factor_y = 480/768.0;
#else
    m_factor_x = 1;
    m_factor_y = 1;
#endif

#if SCREEN_DEBUG
    qDebug() << "m_factor_x: " << m_factor_x << "m_factor_y: " << m_factor_y;
#endif
}

#include <QLabel>

void CDefineScreen::add_parent(QWidget *_parent,bool _add)
{
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))

    QObjectList child_list = _parent->children();
#if SCREEN_DEBUG
    CCommonInterface::printf_debug("CDefineScreen::add_parent() " + QString::number(child_list.size()));
#endif
    if(_add)
        add_widget(_parent);

    for(int index = 0;index < child_list.size();index++)
    {

        if(is_ignore((QWidget *)child_list.at(index)))
        {
            continue;
        }

        if(child_list.at(index)->inherits("QLabel"))
        {
            QLabel *pWidget = (QLabel *)child_list.at(index);

            pWidget->setScaledContents(true);
        }

        if(!child_list.at(index)->inherits("QTimer") &&
        !child_list.at(index)->inherits("QMovie") &&
        !child_list.at(index)->inherits("QHBoxLayout")&&
        !child_list.at(index)->inherits("QVBoxLayout") &&
        !child_list.at(index)->inherits("ScrollController") &&
        !child_list.at(index)->inherits("QIntValidator")&&
        !child_list.at(index)->inherits("QRegExpValidator")&&
        !child_list.at(index)->inherits("QStyledItemDelegate"))
        {
#if SCREEN_DEBUG
            qDebug() << child_list.at(index);
#endif
            add_widget((QWidget *)child_list.at(index));
        }

    }
#endif
}

void CDefineScreen::add_widget(QWidget *widget)
{
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    AutoResizeOriginalData resizeData;
    QRect tmp;

    tmp=widget->geometry();
    tmp.setX(widget->x());
    tmp.setY(widget->y());
    tmp.setWidth(abs(tmp.width()));
    tmp.setHeight(abs(tmp.height()));

    resizeData.data_rect=tmp;
    resizeData.data_font=widget->font();

    if(widget->inherits("QLabel") || widget->inherits("QPushButton") || widget->inherits("QLineEdit"))
        resizeData.data_sheet = widget->styleSheet();
    else
        resizeData.data_sheet = "";


    m_resize_map[widget]=resizeData;
#endif
}

void CDefineScreen::reset_widget()
{
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    QMapIterator<QWidget*, AutoResizeOriginalData> _itarator(m_resize_map);
#if SCREEN_DEBUG
    CCommonInterface::printf_debug("CDefineScreen::reset_widget() " + QString::number(m_resize_map.size()));
#endif
    while(_itarator.hasNext())
    {
        _itarator.next();

        QWidget* _item=_itarator.key();
        QRect tmp=_itarator.value().data_rect;
        #if SCREEN_DEBUG
        qDebug() << _item << tmp;
        #endif
        reset_size(_item,tmp,_itarator.value().data_font,_itarator.value().data_sheet);
    }
#endif
}

void CDefineScreen::reset_size(QWidget *widget,const QRect &_rect,const QFont &_font,const QString &_sheet)
{
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    int widgetX = _rect.x();
    int widgetY = _rect.y();
    int widgetWid = _rect.width();
    int widgetHei = _rect.height();
    int nWidgetX = (int)(widgetX * m_factor_x);
    int nWidgetY = (int)(widgetY * m_factor_y);
    int nWidgetWid = (int)(widgetWid * m_factor_x);
    int nWidgetHei = (int)(widgetHei * m_factor_y);
#if SCREEN_DEBUG
    qDebug() << widget << "nWidgetX: " << nWidgetX << "nWidgetY: " << nWidgetY
    << "nWidgetWid: " << nWidgetWid << "nWidgetHei: " << nWidgetHei;
#endif
    //widget->setGeometry(nWidgetX,nWidgetY,nWidgetWid,nWidgetHei);
    widget->move(nWidgetX,nWidgetY);

    if(0 == nWidgetHei)
        nWidgetHei = 1;

    if(0 == nWidgetWid)
        nWidgetWid = 1;

    widget->setFixedSize(nWidgetWid,nWidgetHei);
    //widget->repaint();

    if(widget->inherits("QLabel") || widget->inherits("QPushButton") || widget->inherits("QLineEdit"))
    {

        QString str_test = "QLabel{border:0px solid #8ea2c0;background:transparent;font: 20 px;color:#8ea2c0;}";
        int current_font_size = 5;
        QString str_sheet = _sheet;

        QString str_new = "";

        QRegExp rx("(.*)font:(.*)px;(.*)");
        int pos = str_sheet.indexOf(rx);
        bool is_ok = false;
        if(pos >= 0)
        {
            qDebug() << rx.capturedTexts().size();

            if(rx.capturedTexts().size() > 2)
            {
                QString str_font_size = rx.capturedTexts().at(2);

                str_font_size = str_font_size.trimmed();

                int font_size = 0;
                font_size = str_font_size.toInt(&is_ok);
                if(is_ok)
                    current_font_size = font_size;
            }
        }
        if(is_ok)
        {
            str_new = QString("font:%1px;").arg((int)(current_font_size * m_factor_y));
            str_sheet = str_sheet.replace(QRegExp("font:(.*)px;"),str_new);

            widget->setStyleSheet(str_sheet);
        }

#if 0
        QFont changedFont;
        changedFont = _font;
        changedFont.setPixelSize(_font.pixelSize() - 4);
        widget->setFont(changedFont);
        #if SCREEN_DEBUG
        qDebug() << widget << "_font.pixelSize() " << _font.pixelSize() << "," << _font.pixelSize() - 4;
        #endif
#endif

    }


#if SCREEN_DEBUG
    qDebug() << widget <<  widget->rect();
#endif

#endif
}

double CDefineScreen::get_factor_x()
{
    return m_factor_x;
}
double CDefineScreen::get_factor_y()
{
    return m_factor_y;
}

int CDefineScreen::get_change_factor_x(int _x)
{
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    return (int)(m_factor_x * _x);
#else
    return (int)(1 * _x);
#endif
}

int CDefineScreen::get_change_factor_y(int _y)
{
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    return (int)(m_factor_y * _y);
#else
    return (int)(1 * _y);
#endif
}

void CDefineScreen::ignore_widget(QWidget *_widget)
{
    m_ignore_list.push_back(_widget);
}

bool CDefineScreen::is_ignore(QWidget *_widget)
{
    bool ret_value = false;
    for(int index = 0;index < m_ignore_list.size();index++)
    {
        if(_widget == m_ignore_list.at(index))
        {
            ret_value = true;
            break;
        }
    }
    return ret_value;
}












