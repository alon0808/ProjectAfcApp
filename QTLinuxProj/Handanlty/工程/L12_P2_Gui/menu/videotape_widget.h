#ifndef CALL_WIDGET_H
#define CALL_WIDGET_H

#include <QDialog>
#include <QLabel>

#include "head_widget.h"
#include <QStackedWidget>
#include "navigate_left.h"

#include "system_manage/videotape_query_widget.h"

class CVideotapeWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CVideotapeWidget(QWidget *parent = 0);
    
signals:
     void signal_set_default_id(int);
     void signal_videotape_query_user_return();
     void signal_pushbutton_home();
     void signal_pushbutton_menu();
public slots:
    void slot_pushbutton_home();
    void slot_pushbutton_back();
    void slot_pushbutton_close();
    void slot_videotape_widget_event(int _type,QString _text);

public:
    bool goto_main_page() {return m_goto_main_page;}
    void clean_date_list();
private:
    void goto_videotape_query();//录像查询

    CHeadWidget *m_CHeadWidget;
    QWidget *m_widget_bg2;
    CNavigateLeft *m_CNavigateLeft;
    QStackedWidget *m_stackwidget_navigate_right;

    CVideotapeQueryWidget *m_CVideotapeQueryWidget;             //录像查询

    bool m_goto_main_page;
    char m_current_select_id;
};

#endif // CALL_WIDGET_H
