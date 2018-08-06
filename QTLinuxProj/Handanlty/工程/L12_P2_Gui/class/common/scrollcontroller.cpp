
#include "scrollcontroller.h"

#include <QScrollBar>
#include <QScrollArea>
#include <QEvent>
#include <QTableView>
#include <QTreeView>
#include <QMouseEvent>
#include <QHeaderView>

#define SCROLL_HANDLE_NORMAL	"#cccccc"
#define SCROLL_HANDLE_HOVER		"#aaaaaa"
#define SCROLL_HANDLE_PRESSED	"#999999"

#define SCROLL_VERTICAL_WIDTH   50

#define SCROLL_VERTICAL	"QScrollBar:vertical\
                    {\
                    border:0px;\
                    width:6px;\
                    background-color:rgba(255,255,255,0%);\
                    margin:%1px,0px,0px,0px;\
                    }\
                    QScrollBar::handle:vertical\
                    {\
                    width:6px;\
                    background-color:%2;\
                    border-radius: 3px;\
                    min-height:100px;\
                    }\
                    QScrollBar::add-line:vertical\
                    {\
                    width: 0px;\
                    background-color:rgba(255,255,255,0%);\
                    }\
                    QScrollBar::sub-line:vertical\
                    {\
                    width: 0px;\
                    background-color:rgba(255,255,255,0%);\
                    }\
                    QScrollBar::add-page:vertical\
                    {\
                    background:transparent;\
                    }\
                    QScrollBar::sub-page:vertical\
                    {\
                    background:transparent;\
                    }"
#define LTY_QSCROLLBAR_VERTICAL \
"QScrollArea{border: 0px solid #275ba7;background:#0E1D30;}"\
"QScrollBar:vertical{width:50px;background:#0E1D30;margin:%1px,1px,0px,0px;padding-top:50px;padding-bottom:50px;}"\
"QScrollBar::handle:vertical{width:50px;border-image:url(:/img/res/main_page/menu/system_manage/scroll_handle.png);border-radius:0px;min-height:100px;}"\
"QScrollBar::handle:vertical:hover{width:50px;border-image:url(:/img/res/main_page/menu/system_manage/scroll_handle.png);border-radius:0px;min-height:100px;}"\
"QScrollBar::add-line:vertical{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/down.png);subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/up.png);subcontrol-position:top;}"\
"QScrollBar::add-line:vertical:hover{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/down.png);subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical:hover{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/up.png);subcontrol-position:top;}"\
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:#0E1D30;}"

#define SCROLL_HORIZONTAL	"QScrollBar:horizontal\
                    {\
                    border:0px;\
                    height:6px;\
                    background-color:rgba(255,255,255,0%);\
                    margin:0px,0px,0px,%1px;\
                    }\
                    QScrollBar::handle:horizontal\
                    {\
                    height:6px;\
                    background-color:%2;\
                    border-radius: 3px;\
                    min-width:35px;\
                    }\
                    QScrollBar::add-line:horizontal\
                    {\
                    height: 0px;\
                    background-color:rgba(255,255,255,0%);\
                    }\
                    QScrollBar::sub-line:horizontal\
                    {\
                    height: 0px;\
                    background-color:rgba(255,255,255,0%);\
                    }\
                    QScrollBar::add-page:horizontal\
                    {\
                    background:transparent;\
                    }\
                    QScrollBar::sub-page:horizontal\
                    {\
                    background:transparent;\
                    }"


ScrollController::ScrollController(QObject *parent): QObject(parent)
{
    m_MarginBorder = 1/*2*/;			//和边框的间距
    m_ScrollBarWidth = SCROLL_VERTICAL_WIDTH;		//滚动条宽度
    m_ScrollHandleWidth = SCROLL_VERTICAL_WIDTH;	//滚动条Handle宽度

    m_VerticalVisible = true;
    m_HorizontalVisible = false;

    m_VerticalScrollBarStyleSheet = LTY_QSCROLLBAR_VERTICAL;//SCROLL_VERTICAL;
    m_HorizontalScrollBarStyleSheet = SCROLL_HORIZONTAL;
    m_scroll_right_length = 0;

}

ScrollController::~ScrollController()
{
    m_WidgetAndVerticalScrollBarMap.clear();
}

#if 1
bool ScrollController::eventFilter(QObject *obj, QEvent *e)
{
    QScrollBar *senderVerticalScrollBar = (QScrollBar *)(obj);
    QScrollBar *senderHorizontalScrollBar = (QScrollBar *)(obj);
    QScrollArea *senderScrollArea = (QScrollArea *)(obj);

    if ((senderScrollArea != NULL) && (m_WidgetAndVerticalScrollBarMap.contains(senderScrollArea)))
    {
        QScrollBar *verticalScrollBarTmp = m_WidgetAndVerticalScrollBarMap.value(senderScrollArea);
        if (verticalScrollBarTmp)
        {
            switch (e->type())
            {
            case QEvent::Resize:
                {
                    QResizeEvent *resizeEventTmp = static_cast<QResizeEvent *>(e);
                    verticalScrollBarTmp->move(resizeEventTmp->size().width() - m_ScrollBarWidth - m_MarginBorder + m_scroll_right_length,0);
                    verticalScrollBarTmp->setFixedSize(m_ScrollBarWidth, resizeEventTmp->size().height() - 1);
                }
                break;
            default:
                break;
            }
        }
    }
    if ((senderScrollArea != NULL) && (m_WidgetAndHorizontalScrollBarMap.contains(senderScrollArea)))
    {
        QScrollBar *horizontalScrollBarTmp = m_WidgetAndHorizontalScrollBarMap.value(senderScrollArea);
        if (horizontalScrollBarTmp)
        {
            switch (e->type())
            {
            case QEvent::Resize:
                {
                    QResizeEvent *resizeEventTmp = static_cast<QResizeEvent *>(e);
                    horizontalScrollBarTmp->move(0, resizeEventTmp->size().height() - m_ScrollBarWidth - m_MarginBorder);
                    horizontalScrollBarTmp->setFixedSize(resizeEventTmp->size().width(), m_ScrollBarWidth);
                }
                break;
            default:
                break;
            }
        }
    }




    if ((senderVerticalScrollBar != NULL) && (m_WidgetAndVerticalScrollBarMap.values().contains(senderVerticalScrollBar)))
    {
        QScrollArea *scrollAreaTmp = m_WidgetAndVerticalScrollBarMap.key(senderVerticalScrollBar);
        if (scrollAreaTmp)
        {
            int iHead = 0;
            if (scrollAreaTmp->inherits("QTableView"))
            {
                QTableView *tableViewTmp = (QTableView *)(scrollAreaTmp);
                if(tableViewTmp->horizontalHeader()->isVisible())
                    iHead = tableViewTmp->horizontalHeader()->height();
            }
            else if (scrollAreaTmp->inherits("QTreeView"))
            {
                QTreeView *treeViewTmp = (QTreeView *)(scrollAreaTmp);
                iHead = treeViewTmp->header()->height();
            }
            else if (scrollAreaTmp->inherits("QTextEdit"))
            {
                iHead = 0;
            }
            else
            {
            #ifdef APPLICATION_TYPE_P2
                iHead = 37;
            #else
                iHead = 60;
            #endif
            }
            iHead += 1;

            switch (e->type())
            {
            case QEvent::Enter:
                {
                    QString str = QString(m_VerticalScrollBarStyleSheet).arg(QString::number(iHead));
                    //QString str = m_VerticalScrollBarStyleSheet;
                    senderVerticalScrollBar->setStyleSheet(str);
                }
                break;
            case QEvent::Leave:
                {
                    QString str = QString(m_VerticalScrollBarStyleSheet).arg(QString::number(iHead));
                    //QString str = m_VerticalScrollBarStyleSheet;
                    senderVerticalScrollBar->setStyleSheet(str);
                }
                break;
            case QEvent::MouseButtonPress:
                {
                    //QMouseEvent *mosueEventTmp = static_cast<QMouseEvent *>(e);
                    QString str = QString(m_VerticalScrollBarStyleSheet).arg(QString::number(iHead));
                    //QString str = m_VerticalScrollBarStyleSheet;
                    senderVerticalScrollBar->setStyleSheet(str);
                }
                break;
            case QEvent::MouseButtonRelease:
                {
                    QMouseEvent *mouseEventTmp = static_cast<QMouseEvent *>(e);

                    bool isHoverTmp = scrollAreaTmp->rect().contains(mouseEventTmp->pos());
                    QString strQss;
                    if (isHoverTmp)
                    {
                        strQss = QString(m_VerticalScrollBarStyleSheet).arg(QString::number(iHead));
                    }
                    else
                    {
                        strQss = QString(m_VerticalScrollBarStyleSheet).arg(QString::number(iHead));
                    }

                    senderVerticalScrollBar->setStyleSheet(strQss);
                }
                break;
            default:
                break;
            }
        }
    }


    if ((senderHorizontalScrollBar != NULL) && (m_WidgetAndHorizontalScrollBarMap.values().contains(senderHorizontalScrollBar)))
    {
        QScrollArea *scrollAreaTmp = m_WidgetAndHorizontalScrollBarMap.key(senderHorizontalScrollBar);
        if (scrollAreaTmp)
        {
            int iHead = 0;
            if (scrollAreaTmp->inherits("QTableView"))
            {
                QTableView *tableViewTmp = (QTableView *)(scrollAreaTmp);
                iHead = tableViewTmp->verticalHeader()->width();
            }
            else if (scrollAreaTmp->inherits("QTreeView"))
            {
                iHead = 0;
            }
            else
            {
                iHead = 0;
            }


            switch (e->type())
            {
            case QEvent::Enter:
                {
                    QString str = QString(m_HorizontalScrollBarStyleSheet).arg(QString::number(iHead),SCROLL_HANDLE_HOVER);
                    senderHorizontalScrollBar->setStyleSheet(str);
                }
                break;
            case QEvent::Leave:
                {
                    QString str = QString(m_HorizontalScrollBarStyleSheet).arg(QString::number(iHead),SCROLL_HANDLE_NORMAL);
                    senderHorizontalScrollBar->setStyleSheet(str);
                }
                break;
            case QEvent::MouseButtonPress:
                {
                    //QMouseEvent *mosueEventTmp = static_cast<QMouseEvent *>(e);
                    QString str = QString(m_HorizontalScrollBarStyleSheet).arg(QString::number(iHead),SCROLL_HANDLE_PRESSED);
                    senderHorizontalScrollBar->setStyleSheet(str);
                }
                break;
            case QEvent::MouseButtonRelease:
                {
                    QMouseEvent *mouseEventTmp = static_cast<QMouseEvent *>(e);

                    bool isHoverTmp = scrollAreaTmp->rect().contains(mouseEventTmp->pos());
                    QString strQss;
                    if (isHoverTmp)
                    {
                        strQss = QString(m_HorizontalScrollBarStyleSheet).arg(QString::number(iHead),SCROLL_HANDLE_HOVER);
                    }
                    else
                    {
                        strQss = QString(m_HorizontalScrollBarStyleSheet).arg(QString::number(iHead),SCROLL_HANDLE_NORMAL);
                    }

                    senderHorizontalScrollBar->setStyleSheet(strQss);
                }
                break;
            default:
                break;
            }
        }
    }





    return QObject::eventFilter(obj, e);
}
#endif

void ScrollController::addMonitorWidget(QScrollArea *paramScrollArea)
{
    if ((paramScrollArea != NULL) && (!m_WidgetAndVerticalScrollBarMap.contains(paramScrollArea)) && (!m_WidgetAndHorizontalScrollBarMap.contains(paramScrollArea)))
    {

        paramScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        paramScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QString strV = QString(m_VerticalScrollBarStyleSheet).arg(QString::number(1));
        QScrollBar *verticalScrollBarTmp = new QScrollBar(Qt::Vertical, paramScrollArea);
        verticalScrollBarTmp->setContextMenuPolicy(Qt::NoContextMenu);
        verticalScrollBarTmp->setValue(0);
        verticalScrollBarTmp->setStyleSheet(strV);
        m_WidgetAndVerticalScrollBarMap.insert(paramScrollArea, verticalScrollBarTmp);
        //setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);


        QString strH = QString(m_HorizontalScrollBarStyleSheet).arg(QString::number(0),SCROLL_HANDLE_NORMAL);
        QScrollBar *horizontalScrollBarTmp = new QScrollBar(Qt::Horizontal, paramScrollArea);
        horizontalScrollBarTmp->setContextMenuPolicy(Qt::NoContextMenu);
        horizontalScrollBarTmp->setValue(0);
        horizontalScrollBarTmp->setStyleSheet(strH);
        m_WidgetAndHorizontalScrollBarMap.insert(paramScrollArea, horizontalScrollBarTmp);

        connect(verticalScrollBarTmp,SIGNAL(valueChanged(int)),this,SLOT(MyVerticalScrollValueChanged(int)));
        connect(paramScrollArea->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(verticalScrollValueChanged(int)));
        connect(paramScrollArea->verticalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(verticalScrollRangeChanged(int,int)));


        connect(horizontalScrollBarTmp,SIGNAL(valueChanged(int)),this,SLOT(MyHorizontalScrollValueChanged(int)));
        connect(paramScrollArea->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(horizontalScrollValueChanged(int)));
        connect(paramScrollArea->horizontalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(horizontalScrollRangeChanged(int,int)));


        verticalScrollBarTmp->installEventFilter(this);
        horizontalScrollBarTmp->installEventFilter(this);
        paramScrollArea->installEventFilter(this);

        updateVerticalRange(paramScrollArea, verticalScrollBarTmp, paramScrollArea->verticalScrollBar()->minimum(), paramScrollArea->verticalScrollBar()->maximum());
        updateHorizontalRange(paramScrollArea, horizontalScrollBarTmp, paramScrollArea->horizontalScrollBar()->minimum(), paramScrollArea->horizontalScrollBar()->maximum());
    }
}

void ScrollController::removeMonitorWidget(QScrollArea *paramScrollArea)
{
    if (paramScrollArea)
    {
        m_WidgetAndVerticalScrollBarMap.take(paramScrollArea);
        m_WidgetAndHorizontalScrollBarMap.take(paramScrollArea);
    }
}

void ScrollController::setVerticalScrollBarVisible(bool _visible)
{
    m_VerticalVisible = _visible;

    QMapIterator<QScrollArea *, QScrollBar *> i(m_WidgetAndVerticalScrollBarMap);
    while (i.hasNext()) {
        i.next();
        QScrollArea *scrollAreaTmp = i.key();
        QScrollBar *scrollBarTmp = i.value();
        if ((scrollAreaTmp != NULL) && (scrollBarTmp != NULL))
        {
            updateVerticalRange(scrollAreaTmp, scrollBarTmp, scrollBarTmp->minimum(), scrollBarTmp->maximum());
        }
    }
}

void ScrollController::setHorizontalScrollBarVisible(bool _visible)
{
    m_HorizontalVisible = _visible;

    QMapIterator<QScrollArea *, QScrollBar *> i(m_WidgetAndHorizontalScrollBarMap);
    while (i.hasNext()) {
        i.next();
        QScrollArea *scrollAreaTmp = i.key();
        QScrollBar *scrollBarTmp = i.value();
        if ((scrollAreaTmp != NULL) && (scrollBarTmp != NULL))
        {
            updateHorizontalRange(scrollAreaTmp, scrollBarTmp, scrollBarTmp->minimum(), scrollBarTmp->maximum());
        }
    }
}

bool ScrollController::verticalScrollBarVisible()
{
    return m_VerticalVisible;
}

bool ScrollController::horizontalScrollBarVisible()
{
    return m_HorizontalVisible;
}



void ScrollController::setVerticalScrollBarStyleSheet(const QString &paramStyleSheet, int _marginBorder, int _scrollBarWidth, int _scrollHandleWidth)
{
    m_MarginBorder = _marginBorder;
    m_ScrollBarWidth = _scrollBarWidth;
    m_ScrollHandleWidth = _scrollHandleWidth;

    m_VerticalScrollBarStyleSheet = paramStyleSheet;

    QMapIterator<QScrollArea *, QScrollBar *> i(m_WidgetAndVerticalScrollBarMap);
    while (i.hasNext()) {
        i.next();
        QScrollArea *scrollAreaTmp = i.key();
        QScrollBar *scrollBarTmp = i.value();
        if ((scrollAreaTmp != NULL) && (scrollBarTmp != NULL))
        {
            updateVerticalRange(scrollAreaTmp, scrollBarTmp, scrollBarTmp->minimum(), scrollBarTmp->maximum());
        }
    }
}

void ScrollController::setHorizontalScrollBarStyleSheet(const QString &paramStyleSheet, int _marginBorder, int _scrollBarWidth, int _scrollHandleWidth)
{
    m_MarginBorder = _marginBorder;
    m_ScrollBarWidth = _scrollBarWidth;
    m_ScrollHandleWidth = _scrollHandleWidth;

    m_HorizontalScrollBarStyleSheet = paramStyleSheet;

    QMapIterator<QScrollArea *, QScrollBar *> i(m_WidgetAndHorizontalScrollBarMap);
    while (i.hasNext()) {
        i.next();
        QScrollArea *scrollAreaTmp = i.key();
        QScrollBar *scrollBarTmp = i.value();
        if ((scrollAreaTmp != NULL) && (scrollBarTmp != NULL))
        {
            updateHorizontalRange(scrollAreaTmp, scrollBarTmp, scrollBarTmp->minimum(), scrollBarTmp->maximum());
        }
    }
}

void ScrollController::updateVerticalRange(QScrollArea *paramScrollArea, QScrollBar *paramBar, int paramMin, int paramMax)
{
    if ((paramBar != NULL) && (paramScrollArea != NULL))
    {
        paramBar->setRange(paramMin,paramMax);
        paramBar->setSliderPosition(paramScrollArea->verticalScrollBar()->sliderPosition());
        paramBar->setSingleStep(paramScrollArea->verticalScrollBar()->singleStep());
        paramBar->setPageStep(paramScrollArea->verticalScrollBar()->pageStep());

        paramBar->setFixedSize(m_ScrollBarWidth,paramScrollArea->height());
        paramBar->move(paramScrollArea->width() - m_ScrollBarWidth - m_MarginBorder,0);

        int iHead = 0;
        if (paramScrollArea->inherits("QTableView"))
        {
            QTableView *tableViewTmp = (QTableView *)(paramScrollArea);
            paramBar->setContentsMargins(0,tableViewTmp->horizontalHeader()->height(),0,0);
            if(tableViewTmp->horizontalHeader()->isVisible())
                iHead = tableViewTmp->horizontalHeader()->height();
        }
        else if (paramScrollArea->inherits("QTreeView"))
        {
            QTreeView *treeViewTmp = (QTreeView *)(paramScrollArea);
            paramBar->setContentsMargins(0,treeViewTmp->header()->height(),0,0);
            iHead = treeViewTmp->header()->height();
        }
        else if (paramScrollArea->inherits("QTextEdit"))
        {
            iHead = 0;
        }
        else
        {
#ifdef APPLICATION_TYPE_P2
            iHead = 37;
#else
            iHead = 60;
#endif
        }
        iHead += 1;

        QString str = QString(m_VerticalScrollBarStyleSheet).arg(QString::number(iHead));


        paramBar->setStyleSheet(str);


        if(0 == paramMax - paramMin)
        {
            paramBar->hide();
        }
        else
        {
            if (m_VerticalVisible)
            {
                paramBar->show();
            }
        }



    }
}


void ScrollController::MyVerticalScrollValueChanged(int _value)
{
    QScrollBar *pObj = (QScrollBar *)(this->sender());
    if (pObj != NULL && m_WidgetAndVerticalScrollBarMap.values().contains(pObj))
    {
        QScrollArea *scrollAreaTmp = m_WidgetAndVerticalScrollBarMap.key(pObj);
        if ((scrollAreaTmp != NULL) && (scrollAreaTmp->verticalScrollBar()->value() != _value))
        {
            scrollAreaTmp->verticalScrollBar()->setValue(_value);
        }
    }
}

void ScrollController::verticalScrollValueChanged(int _value)
{
    QScrollBar *scrollBarSender = (QScrollBar *)(this->sender());
    if (scrollBarSender)
    {
        QMapIterator<QScrollArea *, QScrollBar *> i(m_WidgetAndVerticalScrollBarMap);
        while (i.hasNext()) {
            i.next();
            if (i.key()->verticalScrollBar() == scrollBarSender)
            {
                QScrollBar *scrollBarTmp = i.value();
                if (scrollBarTmp && (scrollBarTmp->value() != _value))
                {
                    scrollBarTmp->setValue(_value);
                }
                break;
            }
        }
    }
}

void ScrollController::verticalScrollRangeChanged( int _min, int _max)
{
    QScrollBar *scrollBarSender = (QScrollBar *)(this->sender());
    if (scrollBarSender)
    {
        QMapIterator<QScrollArea *, QScrollBar *> i(m_WidgetAndVerticalScrollBarMap);
        while (i.hasNext()) {
            i.next();
            if (i.key()->verticalScrollBar() == scrollBarSender)
            {
                QScrollArea *scrollAreaTmp = i.key();
                QScrollBar *scrollBarTmp = i.value();
                if ((scrollAreaTmp != NULL) && (scrollBarTmp != NULL))
                {
                    updateVerticalRange(scrollAreaTmp, scrollBarTmp, _min, _max);
                }
                break;
            }
        }
    }
}








#if 1
#include <QDebug>
void ScrollController::updateHorizontalRange(QScrollArea *paramScrollArea, QScrollBar *paramBar, int paramMin, int paramMax)
{
    if ((paramBar != NULL) && (paramScrollArea != NULL))
    {
        paramBar->setRange(paramMin,paramMax);
        paramBar->setSliderPosition(paramScrollArea->horizontalScrollBar()->sliderPosition());
        paramBar->setSingleStep(paramScrollArea->horizontalScrollBar()->singleStep());
        paramBar->setPageStep(paramScrollArea->horizontalScrollBar()->pageStep());

        paramBar->setFixedSize(paramScrollArea->width(), m_ScrollBarWidth);
        paramBar->move(0, paramScrollArea->height() - m_ScrollBarWidth - m_MarginBorder);

        int iHead = 0;
        if (paramScrollArea->inherits("QTableView"))
        {
            QTableView *tableViewTmp = (QTableView *)(paramScrollArea);
            paramBar->setContentsMargins(tableViewTmp->verticalHeader()->width(), 0, 0, 0);
            iHead = tableViewTmp->verticalHeader()->width();
        }
        else if (paramScrollArea->inherits("QTreeView"))
        {
            iHead = 0;
        }
        else
        {
            iHead = 0;
        }


        QString str = QString(m_HorizontalScrollBarStyleSheet).arg(QString::number(iHead),SCROLL_HANDLE_NORMAL);


        paramBar->setStyleSheet(str);


        if (m_HorizontalVisible)
        {
            if(0 == paramMax - paramMin)
            {
                paramBar->hide();
            }
            else
            {
                paramBar->show();
            }
        }
        else
        {
            paramBar->hide();
        }

    }
}
#endif





#if 1
void ScrollController::MyHorizontalScrollValueChanged(int _value)
{
    QScrollBar *pObj = (QScrollBar *)(this->sender());
    if (pObj != NULL && m_WidgetAndHorizontalScrollBarMap.values().contains(pObj))
    {
        QScrollArea *scrollAreaTmp = m_WidgetAndHorizontalScrollBarMap.key(pObj);
        if ((scrollAreaTmp != NULL) && (scrollAreaTmp->horizontalScrollBar()->value() != _value))
        {
            scrollAreaTmp->horizontalScrollBar()->setValue(_value);
        }
    }
}

void ScrollController::horizontalScrollValueChanged(int _value)
{
    QScrollBar *scrollBarSender = (QScrollBar *)(this->sender());
    if (scrollBarSender)
    {
        QMapIterator<QScrollArea *, QScrollBar *> i(m_WidgetAndHorizontalScrollBarMap);
        while (i.hasNext()) {
            i.next();
            if (i.key()->horizontalScrollBar() == scrollBarSender)
            {
                QScrollBar *scrollBarTmp = i.value();
                if (scrollBarTmp && (scrollBarTmp->value() != _value))
                {
                    scrollBarTmp->setValue(_value);
                }
                break;
            }
        }
    }
}

void ScrollController::horizontalScrollRangeChanged( int _min, int _max)
{
    QScrollBar *scrollBarSender = (QScrollBar *)(this->sender());
    if (scrollBarSender)
    {
        QMapIterator<QScrollArea *, QScrollBar *> i(m_WidgetAndHorizontalScrollBarMap);
        while (i.hasNext()) {
            i.next();
            if (i.key()->horizontalScrollBar() == scrollBarSender)
            {
                QScrollArea *scrollAreaTmp = i.key();
                QScrollBar *scrollBarTmp = i.value();
                if ((scrollAreaTmp != NULL) && (scrollBarTmp != NULL))
                {
                    updateHorizontalRange(scrollAreaTmp, scrollBarTmp, _min, _max);
                }
                break;
            }
        }
    }
}
#endif

void ScrollController::set_scroll_right_length(int _length)
{
    m_scroll_right_length = _length;
}





