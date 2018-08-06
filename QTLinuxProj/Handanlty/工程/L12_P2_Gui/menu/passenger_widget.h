
#ifndef PASSENGER_WIDGET_H
#define PASSENGER_WIDGET_H

#include <QDialog>
#include <QLabel>

#include "head_widget.h"
#include "class/common/define_screen.h"

class CPassengerWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CPassengerWidget(QWidget *parent = 0);
    
signals:

    void signal_pushbutton_home();
    void signal_pushbutton_menu();

public slots:
    void slot_passenger_widget(const CGetPassengerInfo &_info);

public:

private:
    void set_show_money(QLabel *_label1,QLabel *_label2,int _start_x);
    void resizeEvent(QResizeEvent *e);
    CHeadWidget *m_CHeadWidget;

    CMyDefineScreen *m_define_screen;

    QWidget *m_widget_info1;
    QWidget *m_widget_info2;
    QWidget *m_widget_info3;
    QWidget *m_widget_info4;
    QWidget *m_widget_info5;


    QLabel *m_label_info1;
    QLabel *m_label_info2;
    QLabel *m_label_info3;
    QLabel *m_label_info4;
    QLabel *m_label_info5;

    QLabel *m_label_info1_2;
    QLabel *m_label_info2_2;
    QLabel *m_label_info3_2;
    QLabel *m_label_info4_2;
    QLabel *m_label_info5_2;

    QLabel *m_label_info1_1;
    QLabel *m_label_info2_1;
    QLabel *m_label_info3_1;
    QLabel *m_label_info4_1;
    QLabel *m_label_info5_1;


};

#endif // SYSTEM_MANAGE_H






