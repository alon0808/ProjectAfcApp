#ifndef MY_MOUSE_EVENT_H
#define MY_MOUSE_EVENT_H

#include <QWidget>
#include <QTimer>
#include <QTime>

class CMyMouseEvent :QWidget
{
    Q_OBJECT
public:
    CMyMouseEvent(QWidget *parent = 0);
    static CMyMouseEvent *getInstance();

    typedef enum
    {
        kEnumEmpty,
        kEnumInputPanel,
        kEnumMenuList,
        kEnumComboList
    }WidgetType;

public slots:
    void slot_timer_outtime();

private:
    QWidget* get_current_widget(QPoint &_point,WidgetType &_widgetType);
    QWidget* get_point_widget(QWidget *_current,QPoint _point);
    void set_widget_focus(QWidget *_widget,WidgetType _widgetType,bool _flag = true);
public:
    void touch_click_event(int _x,int _y,bool _release = false);
    void stop_timer();
    void reset_timer();
    bool m_first_press; //第一次按下
    QPoint get_first_point();
    bool is_head_back(QPoint &_point);
    void hide_keyboard(int _x,int _y);
private:
    void touch_click_handle(QWidget *_widget,const QPoint &_p1,const QPoint &_p2,bool _release);


    QPoint m_first_point;  //第一次按下的点
    QPoint m_current_point;//记录实时的点
    QWidget *m_press_widget;
    QTimer *m_timer_mouse;
    QTime *m_time_move;
    QTime *m_timeout_touch;

    static CMyMouseEvent *m_single;
};

#endif // MY_MOUSE_EVENT_H
