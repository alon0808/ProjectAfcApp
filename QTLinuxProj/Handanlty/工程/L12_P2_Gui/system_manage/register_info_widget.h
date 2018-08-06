#ifndef REGISTER_INFO_WIDGET_H
#define REGISTER_INFO_WIDGET_H

#include <QWidget>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

#include <QListWidget>
#include "protocol.h"

#include "class/common/define_screen.h"
#include "class/common/parent_widget.h"

class CRegisterInfoWidget : public CMyWidget
{
    Q_OBJECT
public:
    explicit CRegisterInfoWidget(QWidget *parent = 0);
    
signals:
    void signal_window_close();
public slots:
    void slot_pushbutton_ok();
    void slot_pushbutton_cancel();
    void slot_register_info_widget_event(const CSystemRegisterInfo &_registerInfo);
public:
    void sava_data();
    void set_focus_first();
    bool keyboard_value(int _value);
private:

    void resizeEvent(QResizeEvent *e);
    void register_info_init();
    void change_focus(int key_code);

    void read_data();

    QVBoxLayout *m_pMainLayout;
    QWidget *m_widget_main;
    QLabel *m_label_test;

    QWidget *m_widget_bg;

    QLabel *m_label_line1;
    QComboBox *m_combobox_1;

    QLabel *m_label_line1_0;
    QComboBox *m_combobox_1_0;

    QLabel *m_label_line2;
    QLineEdit *m_lineedit2;

    QLabel *m_label_line3;
    QLineEdit *m_lineedit3;

    QLabel *m_label_line4;
    QComboBox *m_combobox_4;
    QLineEdit *m_lineedit4;
    QLabel *m_label_line5;
    QLabel *m_lineedit5;

    QPushButton *m_pushbutton_ok;
    QPushButton *m_pushbutton_cancel;

    CMyDefineScreen *m_define_screen;

    QList<QWidget *> m_focus_widget_list;
    int m_focus_index;
};

#endif // REGISTER_INFO_WIDGET_H
