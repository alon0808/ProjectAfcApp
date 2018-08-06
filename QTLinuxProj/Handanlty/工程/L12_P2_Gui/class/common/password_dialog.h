#ifndef PASSWORD_DIALOG_H
#define PASSWORD_DIALOG_H


#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QLineEdit>

#include "class/common/define_screen.h"
#include "class/common/parent_widget.h"

class CPasswordDialog : public CParentWidget
{
    Q_OBJECT
public:
    static CPasswordDialog *getInstance();
    explicit CPasswordDialog(QWidget *parent = 0,char _type = kEnumNormal);
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


public:
    enum
    {
        kEnumTypeFormat,
        kEnumTypeSystem
    };
    void set_content(const QString&_title,const QString&_context,
                                const QString &buttonText1,
                                const QString &buttonText2);
    CPasswordDialog::ButtonType get_result();
    void password_init();
    void set_password_type(char _type);
    bool keyboard_value(int _value);
private:
    void resizeEvent(QResizeEvent *e);
    QPushButton *m_pushbutton_close;

    QPushButton *m_pushbutton1;  //确定
    QPushButton *m_pushbutton2;  //取消
    ButtonType m_button_type;
    QLabel *m_label_title;  //标题

    QLabel *m_label_password;
    QLineEdit *m_lineedit_password;

    char m_type;
    QString m_button_text1;
    QString m_button_text2;

    static CPasswordDialog *m_single;

    CMyDefineScreen *m_define_screen;
    char m_password_type;
};


#endif // PASSWORD_DIALOG_H




