#include "mytableview.h"


#include <QModelIndex>
#include <QDebug>
#include <QPainter>

#include "./class/common/commontablemodel.h"

//#include "../../ListView/AppManageList/AppMgrTableModel.h"
//#include "../../ListView/DownloadManageList/DownloadManageModel.h"

#include <QEvent>
#include <QMouseEvent>
#include <QRubberBand>
#include <QResizeEvent>
#include <QTableWidget>
#include <QScrollArea>
#include <QTreeView>
#include <QKeyEvent>
#include <QHeaderView>

#include "class/common/scrollcontroller.h"

//#define SCROLL_HANDLE_NORMAL	"rgba(123,123,123,50%)"
//#define SCROLL_HANDLE_HOVER	"rgba(123,123,123,100%)"



#if 0

void MyScrollBar::enterEvent(QEvent *e)
{
    int iHead = m_MyTableView->horizontalHeader()->height();
    QString str = QString(SCROLL_VERTICAL).arg(QString::number(iHead),SCROLL_HANDLE_HOVER);

    setStyleSheet(str);
    return QScrollBar::enterEvent(e);
}

void MyScrollBar::leaveEvent(QEvent *e)
{
    int iHead = m_MyTableView->horizontalHeader()->height();
    QString str = QString(SCROLL_VERTICAL).arg(QString::number(iHead),SCROLL_HANDLE_NORMAL);

    setStyleSheet(str);

    return QScrollBar::leaveEvent(e);
}

void MyScrollBar::mousePressEvent(QMouseEvent *e)
{
    int iHead = m_MyTableView->horizontalHeader()->height();
    QString str = QString(SCROLL_VERTICAL).arg(QString::number(iHead),SCROLL_HANDLE_PRESSED);
    setStyleSheet(str);

    return QScrollBar::mousePressEvent(e);
}

void MyScrollBar::mouseReleaseEvent(QMouseEvent *e)
{
    int iHead = m_MyTableView->horizontalHeader()->height();

    bool isHoverTmp = this->rect().contains(e->pos());
    QString strQss;
    if (isHoverTmp)
    {
        strQss = QString(SCROLL_VERTICAL).arg(QString::number(iHead),SCROLL_HANDLE_HOVER);
    }
    else
    {
        strQss = QString(SCROLL_VERTICAL).arg(QString::number(iHead),SCROLL_HANDLE_NORMAL);
    }

    setStyleSheet(strQss);

    return QScrollBar::mouseReleaseEvent(e);
}

MyScrollBar::MyScrollBar(Qt::Orientation _orientation,QWidget *parent)
    : QScrollBar(_orientation,parent)
{
    m_MyTableView = (MyTableView *)parent;
}
MyScrollBar::~MyScrollBar()
{

}

#endif
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
MyTableView::MyTableView(QWidget *parent,bool _useMyScroll)
    : QTableView(parent)
{

    m_maxShake = 10;
    m_minTriggerTimeValue = 20;
    m_horizontalAccumulateShake = 0;
    m_verticalAccumulateShake = 0;
    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    m_originPoint = QPoint(0, 0);
    m_curPoint = QPoint(0, 0);
    m_originScrollPoint = QPoint(0, 0);
    m_curScrollPoint = QPoint(0, 0);
    m_isPressed = false;
    m_isMoved = false;
    m_selectedRect = QRect();
    m_selecteDisplaydRect = QRect();
    m_selectType = TABLEVIEW_SELECT_MOVE_TYPE;
    m_enableUpdate = true;

    m_originTableWidth = 0;
    m_originTableHeight = 0;
    m_originRowHeight = 35;

    m_currHovered = -1;
    m_isEnableInlineLeaveEventAndSetMouseOver = false;

    m_isRubberBandShow = true;


    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //setFrameShape(QFrame::NoFrame); //设置无边框
    setShowGrid(false); //设置不显示格子线
    setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false); //隐藏行表头
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection); //设置为可以选中多个目标

    viewport()->setMouseTracking(true);

    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //setFocusPolicy(Qt::NoFocus);
    horizontalHeader()->setStretchLastSection(true);

    //rgb(255, 255, 255);\\transparent  // #0E1D30
    setStyleSheet("\
                        QTableView{\
                        background-color: #0E1D30;\
                        gridline-color: #24508F;\
                        border:1px solid #24508F;\
                        }\
                        QTableView::item:selected {\
                        background-color: rgb(226,250,192);\
                        }\
                        QTableView::item:hover {\
                        background-color: rgb(241,253,224);\
                        }");
                        //border-image:url(:/button/images/button/down.png)
//border-image:url(:/button/images/button/up.png)
//border-image:url(:/button/images/button/down_mouseDown.png)
//border-image:url(:/button/images/button/up_mouseDown.png)
     #if 0
     this->verticalScrollBar()->setStyleSheet(

"QScrollBar:vertical{width:40px;background:transparent;background-color:rgb(248, 248, 248);margin:0px,0px,0px,0px;padding-top:36px;padding-bottom:36px;}"
"QScrollBar::handle:vertical{width:40px;background:lightgray ;border-radius:5px;min-height:20px;}"
"QScrollBar::handle:vertical:hover{width:40px;background:gray;border-radius:5px;min-height:20px;}"
"QScrollBar::add-line:vertical{height:36px;width:40px;background-color:#FF83FA;subcontrol-position:bottom;}"
"QScrollBar::sub-line:vertical{height:36px;width:40px;background-color:#FF83FA;subcontrol-position:top;}"
"QScrollBar::add-line:vertical:hover{height:36px;width:40px;background-color:#FF83FA;subcontrol-position:bottom;}"
"QScrollBar::sub-line:vertical:hover{height:36px;width:40px;background-color:#FF83FA;subcontrol-position:top;}"
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;border-radius:5px;}"
);
#endif

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //设置默认行列高宽
    verticalHeader()->setDefaultSectionSize(50);

    if(_useMyScroll)
    {
        m_ScrollController = new ScrollController(this);
        m_ScrollController->addMonitorWidget((QScrollArea *)(this));
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else
    {
     //   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }


#if 0
    m_verticalQScrollBar = NULL;

    m_verticalQScrollBar = new QScrollBar(Qt::Vertical,this);
    m_verticalQScrollBar->setValue(0);
    m_verticalQScrollBar->hide();
    QString str = QString(SCROLL_VERTICAL).arg(QString::number(0),SCROLL_HANDLE_NORMAL);
    m_verticalQScrollBar->setStyleSheet(str);//hsva
    connect(m_verticalQScrollBar,SIGNAL(valueChanged(int)),this,SLOT(MyVerticalScrollValueChanged(int)));

    connect(verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(verticalScrollValueChanged(int)));
    connect(verticalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(verticalScrollRangeChanged(int,int)));
#endif

    this->installEventFilter(this);
    this->viewport()->installEventFilter(this);
}

#if 0
void MyTableView::resizeEvent (QResizeEvent *e)
{
    if (NULL != m_verticalQScrollBar)
    {
        m_verticalQScrollBar->move(e->size().width() - 6 - 2,0);
        m_verticalQScrollBar->setFixedSize(6,e->size().height());
    }
    return QTableView::resizeEvent(e);
}
#endif

MyTableView::~MyTableView()
{

}

void MyTableView::setTableViewShowGrid(bool _flag)
{
    setShowGrid(_flag);
}

void MyTableView::mousePressEvent ( QMouseEvent *event )
{
#if 0
    if (event->button() == Qt::LeftButton)
    {
        m_horizontalAccumulateShake = 0;
        m_verticalAccumulateShake = 0;

        m_rowList.clear();

        m_isMoved = false;
        m_isPressed = true;

        m_originScrollPoint = calculateScrollPoint();
        m_curScrollPoint = calculateScrollPoint();

        QPoint offsetPointForHeaderTmp = offsetPointForHeader();

        setOriginPoint(event->pos() + offsetPointForHeaderTmp);
        setCurPoint(event->pos() + offsetPointForHeaderTmp);
        setSelectedRect(QRect(m_originPoint, m_curPoint).normalized());

        QModelIndex index = this->indexAt(event->pos());
        if (index.isValid() && (!m_rowList.contains(index.row())))
        {
            m_rowList.append(index.row());
        }

        m_triggerTime.restart();
    }
#endif
    return QTableView::mousePressEvent(event);
}

void MyTableView::mouseMoveEvent ( QMouseEvent * event )
{
    updateRubberBandForMouseMoveOrWheel(event->pos());

#if 0
    //if (event->button() == Qt::LeftButton)
    {
        m_isMoved = true;
        QModelIndex index = indexAt(event->pos());
        bool _mouseOver = true;

        if (m_isPressed == true)
        {



            m_curScrollPoint = calculateScrollPoint();

            QPoint offsetPointForScrollTmp = m_curScrollPoint - m_originScrollPoint;

            QPoint offsetPointForHeaderTmp = offsetPointForHeader();


            QPoint lastPointTmp = m_curPoint;
            setCurPoint(event->pos() + offsetPointForHeaderTmp);

            m_horizontalAccumulateShake += qAbs((m_curPoint - lastPointTmp).x());
            m_verticalAccumulateShake += qAbs((m_curPoint - lastPointTmp).y());


            //m_selectedRect = QRect(m_originPoint, m_curPoint).normalized();
            //m_selectedRect = QRect(m_originPoint - scrollOffsetTmp, m_curPoint).normalized();
            setSelectedRect(QRect(m_originPoint - offsetPointForScrollTmp, m_curPoint).normalized());

            updateRubberBandStatus();
            if (m_selectType == TABLEVIEW_SELECT_MOVE_TYPE) {
                //防抖处理
                if ((m_horizontalAccumulateShake > m_maxShake) || (m_verticalAccumulateShake > m_maxShake)) {
                    m_enableUpdate = true;
                }
                else {
                    m_enableUpdate = false;
                }
            }
            else if (m_selectType == TABLEVIEW_SELECT_UP_TYPE) {
                m_enableUpdate = false;
            }
            else {
                //qDebug()<<"unknow m_selectType";
            }

            snapshotRect();
        }
        else
        {
            //qDebug()<<"m_isPressed == false";
        }

        if (index.isValid())
        {
            //emit signalMouseOverRow(_mouseOver,index);
        }


        QPoint offsetPointForHeaderTmp = offsetPointForHeader();
        int offsetYForHeaderTmp = offsetPointForHeaderTmp.y();
        QAbstractItemModel *modelTmp =  this->model();
        int rowCountTmp = modelTmp->rowCount();

        if (rowCountTmp * m_originRowHeight >= (m_originTableHeight - offsetYForHeaderTmp))	//行超过屏幕
        {
            // do nothing
        }
        else	//行未超出屏幕
        {
            if (event->pos().y() >= rowCountTmp * m_originRowHeight)	//滑出了最后一行
            {
#if 1	//
                clearMouseOverStatus();
#endif
            }
        }
    }

#endif
    return QTableView::mouseMoveEvent(event);
}


void MyTableView::mouseReleaseEvent ( QMouseEvent *event )
{
    if (event->button() == Qt::LeftButton)
    {

        int elapsedTriggerTimeTmp = m_triggerTime.elapsed();
        bool enableUpdateTmp = false;
        if (elapsedTriggerTimeTmp > m_minTriggerTimeValue)
        {
            enableUpdateTmp = true;
        }


        if (m_selectType == TABLEVIEW_SELECT_MOVE_TYPE) {
            m_enableUpdate = false;
        }
        else if (m_selectType == TABLEVIEW_SELECT_UP_TYPE) {
            //防抖处理
            if (((m_horizontalAccumulateShake > m_maxShake) || (m_verticalAccumulateShake > m_maxShake)) && (enableUpdateTmp)) {
            //if (enableUpdateTmp) {
                m_enableUpdate = true;
            }
            else {
                m_enableUpdate = false;
            }
        }
        else {
            //qDebug()<<"unknow m_selectType";
        }
        snapshotRect();
        m_isMoved = false;
        m_isPressed = false;
        m_horizontalAccumulateShake = 0;
        m_verticalAccumulateShake = 0;
        //qDebug()<<"MyTableView::mouseReleaseEvent";
        updateRubberBandStatus();

        //qDebug()<<"m_rowList:"<<m_rowList;
    }

    return QTableView::mouseReleaseEvent(event);
}

void MyTableView::wheelEvent(QWheelEvent *event)
{
    QTableView::wheelEvent(event);
    updateRubberBandForMouseMoveOrWheel(event->pos());
}

void MyTableView::updateRubberBandForMouseMoveOrWheel(const QPoint &_curPoint)
{
#if 1
    //if (event->button() == Qt::LeftButton)
    {
        m_isMoved = true;
        QModelIndex index = indexAt(_curPoint);
        //bool _mouseOver = true;

        if (m_isPressed == true)
        {



            m_curScrollPoint = calculateScrollPoint();

            QPoint offsetPointForScrollTmp = m_curScrollPoint - m_originScrollPoint;

            QPoint offsetPointForHeaderTmp = offsetPointForHeader();


            QPoint lastPointTmp = m_curPoint;
            setCurPoint(_curPoint + offsetPointForHeaderTmp);

            m_horizontalAccumulateShake += qAbs((m_curPoint - lastPointTmp).x());
            m_verticalAccumulateShake += qAbs((m_curPoint - lastPointTmp).y());


            //m_selectedRect = QRect(m_originPoint, m_curPoint).normalized();
            //m_selectedRect = QRect(m_originPoint - scrollOffsetTmp, m_curPoint).normalized();
            setSelectedRect(QRect(m_originPoint - offsetPointForScrollTmp, m_curPoint).normalized());

            updateRubberBandStatus();
            if (m_selectType == TABLEVIEW_SELECT_MOVE_TYPE) {
                //防抖处理
                if ((m_horizontalAccumulateShake > m_maxShake) || (m_verticalAccumulateShake > m_maxShake)) {
                    m_enableUpdate = true;
                }
                else {
                    m_enableUpdate = false;
                }
            }
            else if (m_selectType == TABLEVIEW_SELECT_UP_TYPE) {
                m_enableUpdate = false;
            }
            else {
                //qDebug()<<"unknow m_selectType";
            }

            snapshotRect();
        }
        else
        {
            //qDebug()<<"m_isPressed == false";
        }

        if (index.isValid())
        {
            //emit signalMouseOverRow(_mouseOver,index);
        }


        QPoint offsetPointForHeaderTmp = offsetPointForHeader();
        int offsetYForHeaderTmp = offsetPointForHeaderTmp.y();
        QAbstractItemModel *modelTmp =  this->model();
        int rowCountTmp = modelTmp->rowCount();

        if (rowCountTmp * m_originRowHeight >= (m_originTableHeight - offsetYForHeaderTmp))	//行超过屏幕
        {
            // do nothing
        }
        else	//行未超出屏幕
        {
            if (_curPoint.y() >= rowCountTmp * m_originRowHeight)	//滑出了最后一行
            {
#if 1	//
                clearMouseOverStatus();
#endif
            }
        }
    }

#endif
}

void MyTableView::setOriginPoint(const QPoint &paramPoint)
{
    m_originPoint = paramPoint;
}

void MyTableView::setCurPoint(const QPoint &paramPoint)
{
    m_curPoint = paramPoint;
}

QPoint MyTableView::originPoint() const
{
    return m_originPoint;
}

QPoint MyTableView::curPoint() const
{
    return m_curPoint;
}

void MyTableView::setIsPressed(bool flag)
{
    m_isPressed = flag;
}

bool MyTableView::isPressed() const
{
    return m_isPressed;
}

void MyTableView::setSelectedRect(const QRect &rect)
{
    m_selectedRect = rect;
    m_selecteDisplaydRect = rect;


    int leftOffsetTmp = verticalHeader()->isVisible() ? verticalHeader()->width(): 0;
    int rightOffsetTmp	 = verticalScrollBar()->isVisible() ? verticalScrollBar()->width(): 0;
    int topOffsetTmp = horizontalHeader()->isVisible() ? horizontalHeader()->height() : 0;
    int bottomOffsetTmp = horizontalScrollBar()->isVisible() ? horizontalScrollBar()->height() : 0;
    int extraOffsetTmp = 1;

    leftOffsetTmp -= extraOffsetTmp;
    rightOffsetTmp -= extraOffsetTmp;
    topOffsetTmp -= extraOffsetTmp;
    bottomOffsetTmp -= extraOffsetTmp;

    if (m_selecteDisplaydRect.left() < leftOffsetTmp)
    {
        m_selecteDisplaydRect.setLeft(leftOffsetTmp);
    }
    if (m_selecteDisplaydRect.right() > width() - rightOffsetTmp)
    {
        m_selecteDisplaydRect.setRight(width() - rightOffsetTmp);
    }
    if (m_selecteDisplaydRect.top() < topOffsetTmp)
    {
        m_selecteDisplaydRect.setTop(topOffsetTmp);
    }
    if (m_selecteDisplaydRect.bottom() > height() - bottomOffsetTmp)
    {
        m_selecteDisplaydRect.setBottom(height() - bottomOffsetTmp);
    }
}

QRect MyTableView::selectedRect() const
{
    return m_selectedRect;
}

void MyTableView::updateRubberBandStatus()
{
    if (m_isPressed)
    {
        if ((m_horizontalAccumulateShake > m_maxShake) || (m_verticalAccumulateShake > m_maxShake))	//防抖处理
        {
            m_rubberBand->setGeometry(m_selecteDisplaydRect);
            if (m_isRubberBandShow)
            {
                m_rubberBand->show();
            }
            //emit signalDisplayRubberBand(m_selectedRect);
        }
    }
    else
    {
        m_rubberBand->hide();
        //emit signalHideRubberBand();
    }
}

void MyTableView::setSelectType(int paramType)
{
    m_selectType = paramType;
}

QPoint MyTableView::originScrollPoint() const
{
    return m_originScrollPoint;
}

QPoint MyTableView::curScrollPoint() const
{
    return m_curScrollPoint;
}

void MyTableView::setOriginScrollPoint(const QPoint &paramPoint)
{
    m_originScrollPoint = paramPoint;
}

void MyTableView::setCurScrollPoint(const QPoint &paramPoint)
{
    m_curScrollPoint = paramPoint;
}

QPoint MyTableView::calculateScrollPoint() const
{
    int horizontalScrollValueTmp = horizontalScrollBar()->value();
    int verticalScrollValueTmp = verticalScrollBar()->value();
    return QPoint(horizontalScrollValueTmp, verticalScrollValueTmp);
}
QPoint MyTableView::offsetPointForHeader() const
{
    int offsetXTmp = 0;
    int offsetYTmp = 0;
    if (horizontalHeader()->isVisible())
    {
        offsetYTmp+= horizontalHeader()->height();
    }
    if (this->verticalHeader()->isVisible())
    {
        offsetXTmp+= verticalHeader()->width();
    }
    QPoint offsetPointTmp(offsetXTmp, offsetYTmp);
    return offsetPointTmp;
}

void MyTableView::screenRowList(int &paramFrom, int &paramLength)	//返回当前屏幕行列表
{
    int fromTmp = -1;
    int lengthTmp = 0;
    QList <int>rowListTmp;
    int curValueTmp = -1;
    const int marginXTmp = 20;
    QPoint offsetPointForHeaderTmp = offsetPointForHeader();
    int topTmp = 0;
    int bottomTmp = height() - offsetPointForHeaderTmp.y();
    QPoint originPointTmp = QPoint(marginXTmp, topTmp);
    QPoint curPointTmp = QPoint(marginXTmp, bottomTmp);
    QRect selectRectTmp = QRect(originPointTmp, curPointTmp).normalized();


    QModelIndex index;

    bool retTmp = false;

    if (verticalScrollBar()->isVisible())	//有垂直滚动条
    {
        for (int i = 0; i < selectRectTmp.height(); i++)
        {
            curValueTmp = topTmp + i;
            index = this->indexAt(QPoint(marginXTmp, curValueTmp));
            if (index.isValid() && (index.row() != -1))
            {
                if (!rowListTmp.contains(index.row()))
                {
                    rowListTmp.append(index.row());
                    retTmp = true;
                    //qDebug()<<"first:"<<index.row();
                    break;
                }
            }
        }

        if (retTmp)
        {
            for (int i = selectRectTmp.height(); i >= 0; i--)
            {
                curValueTmp = topTmp + i;
                index = this->indexAt(QPoint(marginXTmp, curValueTmp));
                if (index.isValid() && (index.row() != -1))
                {
                    if (!rowListTmp.contains(index.row()))
                    {
                        rowListTmp.append(index.row());
                        //qDebug()<<"last:"<<index.row();
                        break;
                    }
                }
            }
        }
    }
    else	//行数较少未填满屏幕
    {
        if (model()->rowCount() != 0)
        {
            rowListTmp.append(0);
            rowListTmp.append(model()->rowCount() - 1);
        }
    }

    //qDebug()<<"rowListTmp    :"<<rowListTmp;

    if (rowListTmp.count() != 0)
    {
        fromTmp = rowListTmp.first();
        lengthTmp = rowListTmp.last() - rowListTmp.first() + 1;
    }

    paramFrom = fromTmp;
    paramLength = lengthTmp;

    //return rowListTmp;
}

int MyTableView::originTableWidth() const
{
    return m_originTableWidth;
}

int MyTableView::originTableHeight() const
{
    return m_originTableHeight;
}

int MyTableView::originRowHeight() const
{
    return m_originRowHeight;
}

void MyTableView::setOriginTableWidth(int paramOriginTableWidth)
{
    m_originTableWidth = paramOriginTableWidth;
}

void MyTableView::setOriginTableHeight(int paramOriginTableHeight)
{
    m_originTableHeight = paramOriginTableHeight;
}

void MyTableView::setOriginRowHeight(int paramOriginRowHeight)
{
    m_originRowHeight = paramOriginRowHeight;
}

void MyTableView::adjustHeight()
{
    QPoint offsetPointForHeaderTmp = offsetPointForHeader();
    int offsetYForHeaderTmp = offsetPointForHeaderTmp.y();
    QAbstractItemModel *modelTmp =  this->model();
    int rowCountTmp = modelTmp->rowCount();

    if (rowCountTmp * m_originRowHeight >= (m_originTableHeight - offsetYForHeaderTmp))
    {
        setFixedSize(width(), m_originTableHeight);
    }
    else
    {
        setFixedSize(width(), (rowCountTmp * m_originRowHeight + offsetYForHeaderTmp));
    }
}

int MyTableView::currHovered() const
{
    return m_currHovered;
}

void MyTableView::setCurrHovered(int paramCurrHovered)
{
    m_currHovered = paramCurrHovered;
}

bool MyTableView::isEnableInlineLeaveEventAndSetMouseOver() const
{
    return m_isEnableInlineLeaveEventAndSetMouseOver;
}

void MyTableView::setIsEnableInlineLeaveEventAndSetMouseOver(bool paramFlag)
{
    m_isEnableInlineLeaveEventAndSetMouseOver = paramFlag;
}

void MyTableView::setMouseOver(bool _mouseOver,const QModelIndex &index)
{
    if (m_isEnableInlineLeaveEventAndSetMouseOver)
    {
        int row = index.row();
        CommonTableModel *commonModelTmp = (CommonTableModel *)model();

        if ( m_currHovered != -1 && row != m_currHovered)
        {
            commonModelTmp->setMouseOver(m_currHovered,false);
        }

        commonModelTmp->setMouseOver(row,_mouseOver);

        m_currHovered = row;
    }

    /*
    int row = index.row();

    if ( m_currHovered != -1 && row != m_currHovered)
    {
    m_ModelData->setMouseOver(m_currHovered,false);
    }

    m_ModelData->setMouseOver(row,_mouseOver);

    m_currHovered = row;
    */
}

void MyTableView::clearMouseOverStatus()
{
    if (m_isEnableInlineLeaveEventAndSetMouseOver)
    {
        CommonTableModel *commonModelTmp = (CommonTableModel *)model();

        do
        {
            if (-1 == m_currHovered)
            {
                break;
            }

            bool isCheckedTmp = false;
            if (commonModelTmp->getCheckInfo(m_currHovered, isCheckedTmp))
            {
                if (!isCheckedTmp)
                {
                    commonModelTmp->setMouseOver(m_currHovered,false);
                }
            }
            else
            {
                m_currHovered = -1;
                break;
            }
            m_currHovered = -1;
#if 0
            if (commonModelTmp->tableMode() == CommonTableModel::AppManage)
            {
                AppMgrModelData *appMgrModelDataTmp = (AppMgrModelData *)model();
                appMgrModelDataTmp->cleanButtonStatus();
            }
            else if (commonModelTmp->tableMode() == CommonTableModel::DownloadManage)
            {
                DownloadModelData *downloadModelDataTmp = (DownloadModelData *)model();
                downloadModelDataTmp->cleanButtonStatus();
            }
#endif

            //emit signalsCleanButtonStatus();
        } while (0);

    }
}

void MyTableView::leaveEvent(QEvent *event)
{
    //getCheckInfo
    //qDebug()<<"MyTableView::leaveEvent";

    clearMouseOverStatus();
    return QTableView::leaveEvent(event);

    /*
    do
    {
    if (-1 == m_currHovered)
    {
    break;
    }
    if (m_currHovered >= m_ModelData->GetCount())
    {
    break;
    }
    if (m_ModelData->GetCount() <= 0)
    {
    break;
    }


    MessageMgrTableModel data;

    if (!m_ModelData->GetData(m_currHovered,data))
    {
    break;
    }

    if (!data.ischecked)
    {
    m_ModelData->setMouseOver(m_currHovered,false);
    }

    m_currHovered = -1;
    } while (0);
    */
}

void MyTableView::setIsRubberBandShow(bool paramFlag)
{
    m_isRubberBandShow = paramFlag;
}

bool MyTableView::isRubberBandShow() const
{
    return m_isRubberBandShow;
}

void MyTableView::setHorizontalScrollBarVisible(bool _visible)
{
    m_ScrollController->setHorizontalScrollBarVisible(_visible);
}

void MyTableView::setVerticalScrollBarVisible(bool _visible)
{
    m_ScrollController->setVerticalScrollBarVisible(_visible);
}

void MyTableView::setVerticalScrollBarStyleSheet(const QString &paramStyleSheet, int _marginBorder, int _scrollBarWidth, int _scrollHandleWidth)
{
    m_ScrollController->setVerticalScrollBarStyleSheet(paramStyleSheet, _marginBorder, _scrollBarWidth, _scrollHandleWidth);
}

void MyTableView::setHorizontalScrollBarStyleSheet(const QString &paramStyleSheet, int _marginBorder, int _scrollBarWidth, int _scrollHandleWidth)
{
    m_ScrollController->setHorizontalScrollBarStyleSheet(paramStyleSheet, _marginBorder, _scrollBarWidth, _scrollHandleWidth);
}

void MyTableView::setVerticalScrollBarValue(int _value)
{
    this->verticalScrollBar()->setValue(_value);
}

#include <QWheelEvent>
bool MyTableView::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent * pKeyEventTmp = static_cast<QKeyEvent *>(e);
        if (pKeyEventTmp->key() == Qt::Key_Delete)
        {
            emit signalKeyPressDelete();
            return true;
        }
    }
    else if (e->type() == QEvent::MouseButtonPress)
    {
        //qDebug()<<"table press";
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);


        QModelIndex index = this->indexAt(mouseEvent->pos());
        if (!index.isValid())
        {
            emit signalClickEmptyArea(mouseEvent->globalPos());
            if (mouseEvent->button() == Qt::RightButton)
            {
                emit signalRightClickEmptyArea(mouseEvent->globalPos());
            }
        }


    }


    else if (e->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
        QPoint offsetPointForHeaderTmp = offsetPointForHeader();
        int offsetXTmp = offsetPointForHeaderTmp.x();
        int offsetYTmp = offsetPointForHeaderTmp.y();


        QModelIndex index = this->indexAt(helpEvent->pos() - QPoint(offsetXTmp, offsetYTmp));
        if (index.isValid())
        {
            emit signalShowToolTip(index.row(), index.column(), helpEvent->globalPos());
        }

    }


#if 0
    else if (e->type() == QEvent::Wheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(e);
        updateRubberBandForMouseMoveOrWheel(wheelEvent->pos());
    }
    else if (e->type() == QEvent::GraphicsSceneWheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(e);
        updateRubberBandForMouseMoveOrWheel(wheelEvent->pos());
    }
#endif
    return QTableView::eventFilter(obj, e);
}

void MyTableView::snapshotRect()
{
    QPoint offsetPointForHeaderTmp = offsetPointForHeader();
    //int offsetXTmp = offsetPointForHeaderTmp.x();
    int offsetYTmp = offsetPointForHeaderTmp.y();

    //int stepTmp = 5;
    int curValueTmp = -1;
    int marginXTmp = 20;

    if (m_isPressed)
    {
        m_rowList.clear();
        int topTmp = m_selectedRect.top() - offsetYTmp;
        //int bottomTmp = m_selectedRect.bottom() - offsetYTmp;

        /*
        qDebug()<<"m_selectedRect.top():"<<m_selectedRect.top();
        qDebug()<<"m_selectedRect.bottom():"<<m_selectedRect.bottom();
        qDebug()<<"topTmp:"<<topTmp;
        qDebug()<<"bottomTmp:"<<bottomTmp;
        */

        QModelIndex index;

        int retTmp = false;

        for (int i = 0; i < m_selectedRect.height(); i++)
        {
            curValueTmp = topTmp + i;
            index = this->indexAt(QPoint(marginXTmp, curValueTmp));
            if (index.isValid() && (index.row() != -1))
            {
                if (!m_rowList.contains(index.row()))
                {
                    m_rowList.append(index.row());
                    //qDebug()<<"first:"<<index.row();
                    retTmp = true;
                    break;
                }
            }
        }
        if (retTmp)
        {
            for (int i = m_selectedRect.height(); i >= 0; i--)
            {
                curValueTmp = topTmp + i;
                index = this->indexAt(QPoint(marginXTmp, curValueTmp));
                if (index.isValid() && (index.row() != -1))
                {
                    if (!m_rowList.contains(index.row()))
                    {
                        m_rowList.append(index.row());
                        //qDebug()<<"last:"<<index.row();
                        break;
                    }
                }
            }
        }



        //qDebug()<<"m_rowList    :"<<m_rowList;
        int from = -1;
        int length = 0;
        if (m_rowList.count() != 0)
        {
            from = m_rowList.first();
            length = m_rowList.last() - m_rowList.first() + 1;
        }

        if (m_selectType == TABLEVIEW_SELECT_MOVE_TYPE) {
            if (m_isPressed && m_isMoved) {
                if (m_enableUpdate) {
                    emit signalCheckRows(from, length);
                }
            }
        }
        else if (m_selectType == TABLEVIEW_SELECT_UP_TYPE) {
            if (m_isPressed && m_isMoved) {
                if (m_enableUpdate) {
                    emit signalCheckRows(from, length);
                }
            }
        }
        else {
            //qDebug()<<"unknown m_selectType";
        }
    }
}

#if 0
void MyTableView::verticalScrollValueChanged(int _value)
{

    if (m_verticalQScrollBar && (m_verticalQScrollBar->value() != _value))
    {
        m_verticalQScrollBar->setValue(_value);
    }
}

void MyTableView::verticalScrollRangeChanged( int _min, int _max)
{

    if (m_verticalQScrollBar)
    {
        m_verticalQScrollBar->setRange(_min,_max);
        m_verticalQScrollBar->setSliderPosition(verticalScrollBar()->sliderPosition());
        m_verticalQScrollBar->setSingleStep(verticalScrollBar()->singleStep());
        m_verticalQScrollBar->setPageStep(verticalScrollBar()->pageStep());

        //qDebug() << "verticalScrollRangeChanged() horizontalHeader()->height():" << horizontalHeader()->height();
        m_verticalQScrollBar->setContentsMargins(0,horizontalHeader()->height(),0,0);
        m_verticalQScrollBar->setFixedSize(6,this->height());
        int iHead = horizontalHeader()->height();
        QString str = QString(SCROLL_VERTICAL).arg(QString::number(iHead),SCROLL_HANDLE_NORMAL);


        m_verticalQScrollBar->setStyleSheet(str);


        if(0 == _max - _min)
        {
            m_verticalQScrollBar->hide();
        }
        else
        {
            m_verticalQScrollBar->show();
        }

    }

}

void MyTableView::MyVerticalScrollValueChanged(int _value)
{
    if (verticalScrollBar()->value() != _value)
    {
        verticalScrollBar()->setValue(_value);
    }
}

#endif

