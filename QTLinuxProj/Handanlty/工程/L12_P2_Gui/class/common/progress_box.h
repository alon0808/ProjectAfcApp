#ifndef PROGRESS_BOX_H
#define PROGRESS_BOX_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>

#include "class/common/define_screen.h"
#include "class/common/parent_widget.h"

class CProgressBox : public CParentWidget
{
    Q_OBJECT
public:
    static CProgressBox *getInstance();
    explicit CProgressBox(QWidget *parent = 0);
    typedef enum
    {
        KEnumButtonOk,
        KEnumButtonCancel
    }ButtonType;

    typedef enum
    {
        KEnumStatusNull,
        KEnumStatusSuccess,
        KEnumStatusFail,
        KEnumStatusStop,
        KEnumStatusUmount
    }StatusType;

    enum
    {
        kEnumProgreessNormal = 1,
        kEnumProgreessFail = 2,
        kEnumProgreessStop = 3,
         kEnumProgreessNull = 4,
        kEnumProgreessUmount=5

    };
    bool keyboard_value(int _value);

signals:

public slots:

    void slot_pushbutton_close();
    void slot_pushbutton1();
    void slot_pushbutton2();
    void slot_test_timer();
    void slot_progress_box_event(char _type,int _percent);

public:

    CProgressBox::ButtonType get_result();
    CProgressBox::StatusType get_status();
    void test_init();
    void data_init();
    void set_title(QString _title);
private:
    void resizeEvent(QResizeEvent *e);

    QPushButton *m_pushbutton_close;
    QProgressBar *m_progressbar_1;

    QPushButton *m_pushbutton1;         //确定
    QPushButton *m_pushbutton2;         //取消
    ButtonType m_button_type;
    QLabel *m_label_title;  //标题

    CProgressBox::StatusType m_status;
    QTimer *m_timer;
    int m_timer_10s;

private:
    static CProgressBox *m_single;

    CMyDefineScreen *m_define_screen;

};


#endif // PROGRESS_BOX_H




