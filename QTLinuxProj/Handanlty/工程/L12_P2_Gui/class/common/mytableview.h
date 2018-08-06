#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H


class QTableView;
class QEvent;
class QMouseEvent;
class QRubberBand;
class QTimer;
class QResizeEvent;
class QTableWidget;
class QScrollArea;

#include <QPoint>
#include <QRect>
#include <QWidget>
#include <QTime>
#include <QScrollBar>
#include <QTableView>
#include <QMap>
#include <QWheelEvent>

enum
{
    TABLEVIEW_SELECT_MOVE_TYPE,	//移动时触发更新框选区域
    TABLEVIEW_SELECT_UP_TYPE	//释放时触发更新框选区域
};

class MyTableView;

class ScrollController;

#if 0

class MyScrollBar: public QScrollBar
{
    Q_OBJECT
public:
    MyScrollBar(Qt::Orientation _orientation,QWidget *parent = 0);
    ~MyScrollBar();

    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    MyTableView *m_MyTableView;
};

#endif

class MyTableView : public QTableView
{
    Q_OBJECT

public:
    MyTableView(QWidget *parent = 0,bool _useMyScroll = false);
    ~MyTableView();

    /*用于框选*/
    void mousePressEvent ( QMouseEvent *event );
    void mouseMoveEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent *event );
    void wheelEvent(QWheelEvent *event);
    void updateRubberBandForMouseMoveOrWheel(const QPoint &_curPoint);
#if 0
    void resizeEvent (QResizeEvent *e);
#endif
    void setOriginPoint(const QPoint &paramPoint);
    void setCurPoint(const QPoint &paramPoint);
    QPoint originPoint() const;
    QPoint curPoint() const;
    void setIsPressed(bool flag);
    bool isPressed() const;
    void setSelectedRect(const QRect &rect);
    QRect selectedRect() const;
    void updateRubberBandStatus();
    void setSelectType(int paramType);
    QPoint originScrollPoint() const;
    QPoint curScrollPoint() const;
    void setOriginScrollPoint(const QPoint &paramPoint);
    void setCurScrollPoint(const QPoint &paramPoint);
    QPoint calculateScrollPoint() const;
    QPoint offsetPointForHeader() const;
    //void getScreenRowInfo(int &paramFirst, int &paramLength);
    void screenRowList(int &paramFrom, int &paramLength);	//返回当前屏幕行列表

    /*用于自适应高度*/
    int originTableWidth() const;
    int originTableHeight() const;
    int originRowHeight() const;
    void setOriginTableWidth(int paramOriginTableWidth);
    void setOriginTableHeight(int paramOriginTableHeight);
    void setOriginRowHeight(int paramOriginRowHeight);
    void adjustHeight();

    int currHovered() const;
    void setCurrHovered(int paramCurrHovered);
    bool isEnableInlineLeaveEventAndSetMouseOver() const;
    void setIsEnableInlineLeaveEventAndSetMouseOver(bool paramFlag);
    void setMouseOver(bool _mouseOver,const QModelIndex &index);
    void clearMouseOverStatus();
    void leaveEvent(QEvent *event);

    void setIsRubberBandShow(bool);
    bool isRubberBandShow() const;

    void setVerticalScrollBarVisible(bool);
    void setHorizontalScrollBarVisible(bool);

    void setVerticalScrollBarStyleSheet(const QString &paramStyleSheet, int _marginBorder, int _scrollBarWidth, int _scrollHandleWidth);
    void setHorizontalScrollBarStyleSheet(const QString &paramStyleSheet, int _marginBorder, int _scrollBarWidth, int _scrollHandleWidth);

    void setVerticalScrollBarValue(int);

    void setTableViewShowGrid(bool _flag);

protected:
    bool eventFilter(QObject *, QEvent *);

public slots:
    void snapshotRect();
#if 0
    void verticalScrollValueChanged(int);
    void verticalScrollRangeChanged(int,int);
    void MyVerticalScrollValueChanged(int);
#endif

signals:
    void signalMouseOverRow(bool _mouseOver,const QModelIndex &index);
    void signalDisplayRubberBand(const QRect &);
    void signalHideRubberBand();
    void signalCheckRows(int, int);
    //void signalsCleanButtonStatus();
    void signalKeyPressDelete();
    void signalRightClickEmptyArea(const QPoint &);
    void signalClickEmptyArea(const QPoint &);
    void signalShowToolTip(int, int, const QPoint &);

private:
    /*用于框选*/
    QPoint m_originPoint;
    QPoint m_curPoint;
    bool m_isPressed;
    bool m_isMoved;
    QRect m_selectedRect;		//实际框选矩形
    QRect m_selecteDisplaydRect;//实际显示的矩形
    QRubberBand *m_rubberBand;
    QList<int> m_rowList;
    int m_selectType;
    bool m_enableUpdate;
    QPoint m_originScrollPoint;
    QPoint m_curScrollPoint;
    int m_maxShake;
    int m_minTriggerTimeValue;
    QTime m_triggerTime;
    int m_horizontalAccumulateShake;
    int m_verticalAccumulateShake;

    /*用于自适应高度*/
    int m_originTableWidth;
    int m_originTableHeight;
    int m_originRowHeight;

    int m_currHovered;
    bool m_isEnableInlineLeaveEventAndSetMouseOver;

    QScrollBar *m_verticalQScrollBar;

    bool m_isRubberBandShow;

    ScrollController *m_ScrollController;
};


#endif // MYTABLEVIEW_H
