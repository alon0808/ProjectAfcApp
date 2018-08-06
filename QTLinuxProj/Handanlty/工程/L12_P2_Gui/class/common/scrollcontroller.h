#ifndef SCROLLCONTROLLER_H
#define SCROLLCONTROLLER_H

#include <QObject>
#include <QMap>

class QScrollBar;
class QScrollArea;
class QEvent;

class ScrollController: public QObject
{
    Q_OBJECT
public:
    ScrollController(QObject *parent = 0);
    ~ScrollController();
    void addMonitorWidget(QScrollArea *);
    void removeMonitorWidget(QScrollArea *);
    void setVerticalScrollBarVisible(bool);
    void setHorizontalScrollBarVisible(bool);
    bool verticalScrollBarVisible();
    bool horizontalScrollBarVisible();
    void setVerticalScrollBarStyleSheet(const QString &paramStyleSheet, int _marginBorder, int _scrollBarWidth, int _scrollHandleWidth);
    void setHorizontalScrollBarStyleSheet(const QString &paramStyleSheet, int _marginBorder, int _scrollBarWidth, int _scrollHandleWidth);
    void set_scroll_right_length(int _length);

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    void updateVerticalRange(QScrollArea *, QScrollBar *, int, int);
    void updateHorizontalRange(QScrollArea *, QScrollBar *, int, int);

private slots:
    void verticalScrollValueChanged(int _value);
    void verticalScrollRangeChanged( int _min, int _max);
    void MyVerticalScrollValueChanged(int _value);


    void horizontalScrollValueChanged(int _value);
    void horizontalScrollRangeChanged( int _min, int _max);
    void MyHorizontalScrollValueChanged(int _value);

private:
    QMap<QScrollArea *, QScrollBar *> m_WidgetAndVerticalScrollBarMap;
    QMap<QScrollArea *, QScrollBar *> m_WidgetAndHorizontalScrollBarMap;
    bool m_VerticalVisible;
    bool m_HorizontalVisible;
    QString m_VerticalScrollBarStyleSheet;
    QString m_HorizontalScrollBarStyleSheet;

    int m_MarginBorder;			//和边框的间距
    int m_ScrollBarWidth;		//滚动条宽度
    int m_ScrollHandleWidth;	//滚动条Handle宽度
    int m_scroll_right_length;


};
#endif // SCROLLCONTROLLER_H
