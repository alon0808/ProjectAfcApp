#ifndef MAIN_MESSAGE_DIALOG_H
#define MAIN_MESSAGE_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTimer>

#include "class/common/parent_widget.h"

class CMainMessageDialog : public CParentWidget
{
    Q_OBJECT
public:
    explicit CMainMessageDialog(QWidget *parent = 0);
    static CMainMessageDialog *getInstance();
    enum
    {
        kEnumMessageDialog, //来短信时
        kEnumOperateDialog  //点击运营中
    };

signals:
    
public slots:
    void slot_pushbutton_close();
    void slot_timer_close_window();
public:
    void set_show_content(int _type,const QString &_show1,const QString &_show2);
    bool keyboard_value(int _value);
private:
    QPushButton *m_pushbutton_close;
    QWidget *m_widget_background;

////////////////////////////////////////////来短信时 start
    QLabel *m_label_messae;

////////////////////////////////////////////来短信时 end


////////////////////////////////////////////点击运营中 start
    QLabel *m_label_start_vehicle1;
    QLabel *m_label_start_vehicle2;

    QLabel *m_label_drive_time1;
    QLabel *m_label_drive_time2;

    QLabel *m_label_auto_close1;
    QLabel *m_label_auto_close2;
////////////////////////////////////////////点击运营中 end
    QTimer *m_timer;
    char m_timer_10s;

    static CMainMessageDialog *m_single;

};

#endif // MAIN_MESSAGE_DIALOG_H














