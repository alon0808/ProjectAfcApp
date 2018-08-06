#ifndef PASSAGEWAY_WIDGET_H
#define PASSAGEWAY_WIDGET_H

#include <QDialog>
#include <QLabel>

#include "head_widget.h"
#include <QStackedWidget>
#include "navigate_left.h"

#include "system_manage/passenger_flow_widget.h"

#include "class/common/define_screen.h"

class CPassagewayWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CPassagewayWidget(QWidget *parent = 0);
    
signals:
     void signal_set_default_id(int);
     void signal_passenger_flow_user_return();
     void signal_pushbutton_home();
     void signal_pushbutton_menu();
public slots:
    void slot_pushbutton_home();
    void slot_pushbutton_back();
    void slot_pushbutton_close();
    void slot_passageway_widget_event(int _type,QString _text);

public:
    bool goto_main_page() {return m_goto_main_page;}
private:
    void goto_passenger_flow();         //客流统计
    void resizeEvent(QResizeEvent *e);

    CHeadWidget *m_CHeadWidget;
    QWidget *m_widget_bg2;
    CNavigateLeft *m_CNavigateLeft;
    QStackedWidget *m_stackwidget_navigate_right;

    CPassengerFlowWidget *m_CPassengerFlowWidget;               //客流统计

    bool m_goto_main_page;
    char m_current_select_id;

    CMyDefineScreen *m_define_screen;

};

#endif // CALL_WIDGET_H
