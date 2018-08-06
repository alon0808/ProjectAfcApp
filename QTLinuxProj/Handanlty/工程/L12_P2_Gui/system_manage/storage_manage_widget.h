#ifndef STORAGE_MANAGE_WIDGET_H
#define STORAGE_MANAGE_WIDGET_H

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>

#include "class/common/navigateleftbutton.h"
#include "class/common/my_pushbutton.h"
#include "class/common/scrollcontroller.h"

class CStorageInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CStorageInfo(QWidget *parent = 0);

signals:
    void signal_pushbutton_format();
public:
    void resizeEvent(QResizeEvent *e);

    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line2_1;
    QLabel *m_label_line2_2;
    QLabel *m_label_line3;
    QLabel *m_label_line3_1;
    QWidget *m_widget_bg;

    QCheckBox *m_checkbox;
    //CMyPushButton *m_pushbutton_enable;
    QPushButton *m_pushbutton_ok;

    CMyDefineScreen *m_define_screen;

};
class CSystemStorageManage;
class CStorageManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CStorageManageWidget(QWidget *parent = 0);

signals:

public slots:


    void slot_button_1();
    void slot_button_2();
    void slot_button_3();
    void slot_timer_init();

    void slot_pushbutton_format1();
    void slot_pushbutton_format2();
    void slot_pushbutton_format3();

    void slot_storage_manage_widget_event(const CSystemStorageManage &_info);
    void slot_storage_manage_widget_event_ex(char _type,void *_param);

private:
private:
    void resizeEvent(QResizeEvent *e);
    void storage_manage_init_new();
    void scroll_layout();
    bool password_pass(char _type);


////////////////////////////////////////////////////new start
    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;
    NavigateLeftButton *m_button_3;

    bool m_button_press_1;
    bool m_button_press_2;
    bool m_button_press_3;

    CStorageInfo *m_sotrage_info1;
    CStorageInfo *m_sotrage_info2;
    CStorageInfo *m_sotrage_info3;

    QLabel *m_label_space1;
    QLabel *m_label_space2;

    QScrollArea *m_scrollarea_1;

    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

////////////////////////////////////////////////////new end
    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_vboxlayout_storage;
    QWidget *m_widget_main;
    QWidget *m_widget_bg;
    QWidget *m_widget_scroll;

    QLabel *m_label_line1;
    QComboBox *m_combobox_1;

    QLabel *m_label_line2;
    QLabel *m_label_line2_1;

    QLabel *m_label_line3;
    QLabel *m_label_line3_1;

    QLabel *m_label_line4;
    QLabel *m_label_line4_1;
    QLabel *m_label_line5;
    QLabel *m_label_line5_1;

    QPushButton *m_pushbutton_format;

    CMyDefineScreen *m_define_screen;

};
#endif // STORAGE_MANAGE_WIDGET_H
