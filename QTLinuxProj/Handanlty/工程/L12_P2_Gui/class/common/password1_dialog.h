#ifndef PASSWORD1_DIALOG_H
#define PASSWORD1_DIALOG_H


#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QLineEdit>

#include "class/common/define_screen.h"
#include "class/common/parent_widget.h"

class CHeadWidget;

class CPassword1Dialog : public CParentWidget
{
    Q_OBJECT
public:
    static CPassword1Dialog *getInstance();
    explicit CPassword1Dialog(QWidget *parent = 0,char _type = kEnumNormal);
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
    void signal_user_exit_password();
public slots:

    void slot_pushbutton1();
    void slot_pushbutton2();


public:

    void set_content(const QString&_title,const QString&_context,
                                const QString &buttonText1,
                                const QString &buttonText2);
    CPassword1Dialog::ButtonType get_result();
    void password_init();
    bool keyboard_value(int _value);
private:
    void resizeEvent(QResizeEvent *e);


    QPushButton *m_pushbutton1;  //确定
    QPushButton *m_pushbutton2;  //取消
    ButtonType m_button_type;


    QLabel *m_label_password;
    QLineEdit *m_lineedit_password;

    char m_type;
    QString m_button_text1;
    QString m_button_text2;

    static CPassword1Dialog *m_single;

    CMyDefineScreen *m_define_screen;
    char m_password_type;
    CHeadWidget *m_CHeadWidget;

};


#endif // PASSWORD_DIALOG_H




