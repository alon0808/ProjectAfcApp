#include "commonshadowwidget.h"


#include "commonshadowwidget.h"
#include <QGraphicsDropShadowEffect>

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QApplication>

#include "./class/common/commonuiinit.h"
#include "LtyCommonDefine.h"

class CommonShadowWidgetPrivate
{
public:
    CommonShadowWidgetPrivate();
    ~CommonShadowWidgetPrivate();

    void init();
public:
    QHBoxLayout *m_pMiddleLayout;
    QHBoxLayout *m_pMainLayout;
    QWidget *m_centralWidget;
    QPoint m_ptMove;
    QPoint m_ptPress;
    bool m_bLeftButtonPress;
    int m_mouseActivateHeight;
    int m_margin;
    bool m_bHideWhenPhoneDisconnect;
    bool m_bEnableCloseWidgetByKeyEscape;
    int m_normalWidth;
    int m_normalHeight;
    QWidget *m_pMinitorWidget;
    bool m_bMove;
    int m_curPos;
    bool m_bEnableResize;
};

CommonShadowWidgetPrivate::CommonShadowWidgetPrivate()
{
    init();
}

CommonShadowWidgetPrivate::~CommonShadowWidgetPrivate()
{
    if (m_pMiddleLayout){
        delete m_pMiddleLayout;
        m_pMiddleLayout = NULL;
    }
    if (m_centralWidget){
        delete m_centralWidget;
        m_centralWidget = NULL;
    }
    if (m_pMainLayout){
        delete m_pMainLayout;
        m_pMainLayout = NULL;
    }
}

void CommonShadowWidgetPrivate::init()
{
    m_pMiddleLayout = NULL;
    m_pMainLayout = NULL;
    m_centralWidget = NULL;
    m_pMinitorWidget = NULL;

    m_ptMove = QPoint(0, 0);
    m_ptPress = QPoint(0, 0);
    m_bLeftButtonPress = false;
    m_mouseActivateHeight = 42;
    m_margin = COM_SHADOWWIDGET_MARGIN;

    m_pMiddleLayout = new QHBoxLayout;
    m_pMainLayout = new QHBoxLayout;
    m_centralWidget = new QWidget;

    m_pMiddleLayout->setSpacing(0);
    m_pMiddleLayout->setMargin(0);

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect();
    shadow_effect->setBlurRadius(11.0);
    shadow_effect->setColor(QColor(0, 0, 0, 160));
    shadow_effect->setOffset(0.0);


    m_centralWidget->setGraphicsEffect(shadow_effect);
    m_centralWidget->setLayout(m_pMiddleLayout);
    m_centralWidget->setObjectName("m_centralWidget");
    m_centralWidget->setStyleSheet("QWidget#m_centralWidget{background-color:white;}");


    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setContentsMargins(m_margin,m_margin,m_margin,m_margin);
    m_pMainLayout->addWidget(m_centralWidget, Qt::AlignCenter);

    m_bHideWhenPhoneDisconnect = true;
    m_bEnableCloseWidgetByKeyEscape = true;

    m_normalWidth = 0;
    m_normalHeight = 0;
    m_bMove = false;
    m_curPos = 0;
    m_bEnableResize = false;
}

CommonShadowWidget::CommonShadowWidget(QWidget *parent)
    : QDialog(parent)
{

    this->setMouseTracking(true);


    createWidgets();

    this->installEventFilter(this);

    //connect(ConnectManage::getInstance(), SIGNAL(signalConnectManagePhoneDisconnect()), this, SLOT(slotShadowWidgetPhoneDisconnect()));
    //connect(ConnectManage::getInstance(), SIGNAL(signalConnectManagePhoneDisconnect()), this, SIGNAL(signalShadowWidgetPhoneDisconnect()));
}

CommonShadowWidget::~CommonShadowWidget()
{

}
#include <QLineEdit>
#include <QPushButton>
void CommonShadowWidget::createWidgets()
{
    //setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //this->setAttribute(Qt::WA_TranslucentBackground);
#if 1	//4.8.2的Qt::ToolTip会导致如下问题，其他窗口最小化，我们的软件置于后方
    //this->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::Dialog);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    //this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::Dialog);

#endif


    d = new CommonShadowWidgetPrivate;

    this->setLayout(d->m_pMainLayout);
}

int CommonShadowWidget::countFlag(QPoint p, int row)
{
    if(p.y()< COM_SHADOWWIDGET_MARGIN_DOUBLE)
        return 10+row;
    else if(p.y()>this->height()-COM_SHADOWWIDGET_MARGIN_DOUBLE)
        return 30+row;
    else
        return 20+row;
}

void CommonShadowWidget::setCursorType(int flag)
{
    Qt::CursorShape cursor;
    switch(flag)
    {
    case 11:
    case 33:
        cursor=Qt::SizeFDiagCursor;break;
    case 13:
    case 31:
        cursor=Qt::SizeBDiagCursor;break;
    case 21:
    case 23:
        cursor=Qt::SizeHorCursor;break;
    case 12:
    case 32:
        cursor=Qt::SizeVerCursor;break;
    case 22:
        if (d->m_bMove)
        {
            cursor=Qt::OpenHandCursor;
        }break;
    default:
        break;

    }
    setCursor(cursor);
}

int CommonShadowWidget::countRow(QPoint p)
{
    return (p.x()<COM_SHADOWWIDGET_MARGIN_DOUBLE)?1:(p.x()>(this->width()-COM_SHADOWWIDGET_MARGIN_DOUBLE)?3:2);
}

//鼠标按下事件
void CommonShadowWidget::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        d->m_ptPress = event->globalPos();
        d->m_bLeftButtonPress = true;

        if(event->y()<d->m_mouseActivateHeight)
        {
            d->m_bMove = true;
        }
        d->m_curPos=countFlag(event->pos(),countRow(event->pos()));

    }

    event->ignore();
}

//鼠标移动事件
void CommonShadowWidget::mouseMoveEvent(QMouseEvent *event)
{
    int poss=countFlag(event->pos(),countRow(event->pos()));

    if (d->m_bEnableResize)
    {
        setCursorType(poss);
    }


    if(d->m_bLeftButtonPress)
    {
        d->m_ptMove = event->globalPos();
        QPoint ptOffset = d->m_ptMove - d->m_ptPress;

        if (d->m_curPos == 22)
        {
            //移动主窗口

            if (d->m_bMove)
            {
                this->move(this->pos()+d->m_ptMove-d->m_ptPress);

                if((d->m_ptMove - d->m_ptPress).manhattanLength() > QApplication::startDragDistance())
                {
                    emit signalNeedShowNormal();
                }
            }
        }
        else
        {
            if (d->m_bEnableResize)
            {
                QRect widRect=geometry();

                switch(d->m_curPos)
                {

                case 11:widRect.setTopLeft(widRect.topLeft()+ptOffset);break;
                case 13:widRect.setTopRight(widRect.topRight()+ptOffset);break;
                case 31:widRect.setBottomLeft(widRect.bottomLeft()+ptOffset);break;
                case 33:widRect.setBottomRight(widRect.bottomRight()+ptOffset);break;
                case 12:widRect.setTop(widRect.top()+ptOffset.y());break;
                case 21:widRect.setLeft(widRect.left()+ptOffset.x());break;
                case 23:widRect.setRight(widRect.right()+ptOffset.x());break;
                case 32:widRect.setBottom(widRect.bottom()+ptOffset.y());break;
                }
                setGeometry(widRect);
            }
        }

        //重新设置m_ptPress;
        d->m_ptPress = d->m_ptMove;
    }

    event->ignore();
}
//鼠标释放事件
void CommonShadowWidget::mouseReleaseEvent(QMouseEvent *event)
{
#if 0
    if (event->button() == Qt::LeftButton)
    {
        //        d->m_ptMove = event->globalPos();
        //        this->move(this->pos()+d->m_ptMove-md->_ptPress);
        d->m_bLeftButtonPress = false;
    }
#endif
    d->m_bLeftButtonPress = false;
    d->m_bMove = false;

    setCursor(Qt::ArrowCursor);

    event->ignore();
}

bool CommonShadowWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if (keyEvent->key() == Qt::Key_Escape)
        {
            emit signalCloseWidgetByKeyEscape();
            if (!d->m_bEnableCloseWidgetByKeyEscape)
            {
                return true;
            }
            else
            {
                hide();
                return true;
            }
        }
    }
    else if (d->m_pMinitorWidget && d->m_pMinitorWidget == obj)
    {
        if (!d->m_bMove)
        {
            setCursor(Qt::ArrowCursor);
        }
    }

    return QDialog::eventFilter(obj, e);
}

void CommonShadowWidget::setMouseActivateHeight(int paramHeight)
{
    d->m_mouseActivateHeight = paramHeight;
}

int CommonShadowWidget::mouseActivateHeight() const
{
    return d->m_mouseActivateHeight;
}

void CommonShadowWidget::setMiddleWidget(QWidget *paramWidget, int _width, int _height)
{
    if (paramWidget)
    {
        if ((_width != 0) && (_height != 0))
        {
            d->m_normalWidth = _width;
            d->m_normalHeight = _height;
            this->resize(_width + d->m_margin * 2, _height + d->m_margin * 2);
            d->m_bEnableResize = true;
        }
        else
        {
            this->resize(paramWidget->width() + d->m_margin * 2, paramWidget->height() + d->m_margin * 2);
        }

        d->m_pMinitorWidget = paramWidget;
        d->m_pMiddleLayout->addWidget(paramWidget);
        d->m_pMinitorWidget->installEventFilter(this);
    }
}

void CommonShadowWidget::updateSize(int _width, int _height)
{
    d->m_normalWidth = _width;
    d->m_normalHeight = _height;
    this->resize(_width + d->m_margin * 2, _height + d->m_margin * 2);
}

void CommonShadowWidget::removeMiddleWidget(QWidget *paramWidget)
{
    if (paramWidget)
    {
        d->m_pMiddleLayout->removeWidget(paramWidget);
    }
}

void CommonShadowWidget::setHideWhenPhoneDisconnectFlag(bool _flag)
{
    if (d->m_bHideWhenPhoneDisconnect != _flag)
    {
        d->m_bHideWhenPhoneDisconnect = _flag;
    }
}

void CommonShadowWidget::setEnableCloseWidgetByKeyEscape(bool _flag)
{
    if (d->m_bEnableCloseWidgetByKeyEscape != _flag)
    {
        d->m_bEnableCloseWidgetByKeyEscape = _flag;
    }
}

void CommonShadowWidget::slotShadowWidgetPhoneDisconnect()
{
    if (d->m_bHideWhenPhoneDisconnect)
    {
        hide();
    }
}
