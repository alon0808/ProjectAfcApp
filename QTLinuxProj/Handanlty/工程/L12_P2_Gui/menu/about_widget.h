#ifndef ABOUT_WIDGET_H
#define ABOUT_WIDGET_H


#include <QDialog>
#include <QLabel>

#include "head_widget.h"
#include "navigate_left.h"

#include <QStackedWidget>
#include "class/common/scrollcontroller.h"

#include "protocol.h"

#include "class/common/define_screen.h"

//关于本机
class CAboutMachineWidget :public QWidget
{
    Q_OBJECT
public:
    CAboutMachineWidget(QWidget *parent = 0);

public slots:
    void slot_ablout_machine_init();

private:
    void resizeEvent(QResizeEvent *e);
    void scroll_layout();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg;
public:
    QLabel *m_label_line1;
    QLabel *m_label_line1_1;

    QLabel *m_label_line2;
    QLabel *m_label_line2_1;

    QLabel *m_label_line3;
    QLabel *m_label_line3_1;

    QLabel *m_label_line4;
    QLabel *m_label_line4_1;

    QLabel *m_label_line5;
    QLabel *m_label_line5_1;

    QLabel *m_label_line6;
    QLabel *m_label_line6_1;

    QLabel *m_label_line7;
    QLabel *m_label_line7_1;

    QLabel *m_label_line8;
    QLabel *m_label_line8_1;


    QLabel *m_label_line9;
    QLabel *m_label_line9_1;

    QLabel *m_label_line10;
    QLabel *m_label_line10_1;

    QLabel *m_label_line11;
    QLabel *m_label_line11_1;

    QLabel *m_label_line12;
    QLabel *m_label_line12_1;

    CMyDefineScreen *m_define_screen;

};

//厂家信息
class CManufactorInfoWidget :public QWidget
{
    Q_OBJECT
public:
    CManufactorInfoWidget(QWidget *parent = 0);

public slots:


private:
    void resizeEvent(QResizeEvent *e);

    QLabel *m_label_company;
    QWidget *m_widget_bg;
    CMyDefineScreen *m_define_screen;

};

class CAboutWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CAboutWidget(QWidget *parent = 0);
    
signals:
    void signal_set_default_id(int);
    void signal_pushbutton_home();
    void signal_pushbutton_menu();
public slots:
    void slot_pushbutton_home();
    void slot_pushbutton_back();
    void slot_about_widget_event(int _type,QString _text);

    void slot_about_machine_widget_event(const CAboutMachineInfo &_info);

public:
    bool goto_main_page() {return m_goto_main_page;}
private:
    void resizeEvent(QResizeEvent *e);
    void show_about_machine();
    void show_manufactor_info();


    CHeadWidget *m_CHeadWidget;
    QWidget *m_widget_bg1;
    QLabel *m_label_about;
    QWidget *m_widget_bg2;

    CNavigateLeft *m_CNavigateLeft;


    QStackedWidget *m_stackwidget_navigate_right;

    CAboutMachineWidget *m_CAboutMachineWidget;
    CManufactorInfoWidget *m_CManufactorInfoWidget;

    bool m_goto_main_page;

    CMyDefineScreen *m_define_screen;

};

#endif // ABOUT_WIDGET_H
