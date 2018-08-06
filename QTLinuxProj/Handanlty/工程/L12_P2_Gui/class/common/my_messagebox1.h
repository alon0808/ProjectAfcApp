#ifndef MY_MESSAGEBOX1_H
#define MY_MESSAGEBOX1_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>


#include "class/common/parent_widget.h"

class CMyMessageBox1 : public CParentWidget
{
    Q_OBJECT
public:
    static CMyMessageBox1 *getInstance();
    explicit CMyMessageBox1(QWidget *parent = 0);

    enum
    {
        kEnumSuccess,
        kEnumFail,
        kEnumAbnormal
    };


signals:
    
public slots:
    void slot_pushbutton_close();
    void slot_timer_close_window();

public:
    void set_type(char _type);
    void set_timer();
    bool keyboard_value(int _value);
private:

    QPushButton *m_pushbutton_close;


    QLabel *m_label_content;
    QWidget *m_widget_status;

    QTimer *m_timer;
    int m_timer_10s;


    static CMyMessageBox1 *m_single;



};

#endif // MY_MESSAGEBOX_H













