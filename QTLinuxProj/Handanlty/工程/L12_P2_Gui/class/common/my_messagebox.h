#ifndef MY_MESSAGEBOX_H
#define MY_MESSAGEBOX_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include "class/common/define_screen.h"
#include "class/common/parent_widget.h"

class CMyMessageBox : public CParentWidget
{
    Q_OBJECT
public:
    static CMyMessageBox *getInstance();
    explicit CMyMessageBox(QWidget *parent = 0,char _type = kEnumNormal);
    typedef enum
    {
        KEnumButtonOk,
        KEnumButtonCancel
    }ButtonType;

    enum
    {
        kEnumNormal,
        kEnumFail
    };
    enum
    {
        KEnumTimerNull,
        KEnumTimerOk,
        KEnumTimerCancel
    };


signals:
    
public slots:
    void slot_pushbutton_close();
    void slot_pushbutton1();
    void slot_pushbutton2();
    void slot_timer_close_window();

public:
    void set_content(const QString&_title,const QString&_context,
                                const QString &buttonText1,
                                const QString &buttonText2);
    CMyMessageBox::ButtonType get_result();
    void set_timer(char _timerType);
    bool keyboard_value(int _value);
private:
    void resizeEvent(QResizeEvent *e);
    QPushButton *m_pushbutton_close;

    QPushButton *m_pushbutton1;  //确定
    QPushButton *m_pushbutton2;  //取消
    ButtonType m_button_type;
    QLabel *m_label_title;  //标题
    QLabel *m_label_content;

    char m_type;
    QString m_button_text1;
    QString m_button_text2;

    QTimer *m_timer;
    int m_timer_10s;
    char m_timer_type;

    static CMyMessageBox *m_single;

    CMyDefineScreen *m_define_screen;

};

#endif // MY_MESSAGEBOX_H













