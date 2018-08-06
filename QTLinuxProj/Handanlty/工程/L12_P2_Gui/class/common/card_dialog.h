#ifndef CARD_DIALOG_H
#define CARD_DIALOG_H


#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QLineEdit>

#include "class/common/define_screen.h"
#include "class/common/parent_widget.h"

class CCardDialog : public CParentWidget
{
    Q_OBJECT
public:
    static CCardDialog *getInstance();
    explicit CCardDialog(QWidget *parent = 0,char _type = kEnumNormal);
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
    void set_content(const QString&_title,const QString&_context,
                                const QString &buttonText1,
                                const QString &buttonText2);
    CCardDialog::ButtonType get_result();
    void card_init();
    QString get_card_number();

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

    static CCardDialog *m_single;

    CMyDefineScreen *m_define_screen;
    QString m_card_number;

};


#endif // PASSWORD_DIALOG_H




