#ifndef CAMERA_CONFIG_WIDGET_H
#define CAMERA_CONFIG_WIDGET_H

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QWidget>
#include <QStackedWidget>
#include <QComboBox>
#include <QGridLayout>
#include <QCheckBox>

#include "class/common/my_pushbutton.h"

#include "./class/common/definitionbutton.h"
#include "class/common/navigateleftbutton.h"
#include "class/common/scrollcontroller.h"

#include "protocol.h"

//1.基本设置
class CBaseConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CBaseConfigWidget(QWidget *parent = 0);

signals:
    void signal_pushbutton_config();
public slots:

private:
    QWidget *m_widget_bg;
    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
public:
    QComboBox *m_combobox_line1;
    QComboBox *m_combobox_line2;
    QComboBox *m_combobox_line3;
    QComboBox *m_combobox_line5;
    QComboBox *m_combobox_line6;
    QComboBox *m_combobox_line7;
    QLabel *m_label_line3_1;
    QLabel *m_label_line4_1;
    //CMyPushButton *m_pushbutton_line4;
    QCheckBox *m_checkbox_line4;

    QLineEdit *m_lineedit_line4;
    CMyPushButton *m_pushbutton_line6;
    QCheckBox *m_checkbox_line6;
    QPushButton *m_pushbutton_line7;

};

//2.显示设置
class CShowConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CShowConfigWidget(QWidget *parent = 0);

signals:

public slots:

private:
    void combobox_init(QComboBox *_combobox);
    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;

    QWidget *m_widget_bg;
public:
    //CMyPushButton *m_pushbutton_line1;
    QCheckBox *m_checkbox_line1;
    QComboBox *m_combobox_line1;
    //CMyPushButton *m_pushbutton_line2;
    QCheckBox *m_checkbox_line2;
    QComboBox *m_combobox_line2;
    //CMyPushButton *m_pushbutton_line3;
    QCheckBox *m_checkbox_line3;
    QComboBox *m_combobox_line3;
    //CMyPushButton *m_pushbutton_line4;
    QCheckBox *m_checkbox_line4;
    QComboBox *m_combobox_line4;
    //CMyPushButton *m_pushbutton_line5;
    QCheckBox *m_checkbox_line5;
    QComboBox *m_combobox_line5;
    //CMyPushButton *m_pushbutton_line6;
    QCheckBox *m_checkbox_line6;
    QComboBox *m_combobox_line6;

};

class CCamLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCamLineWidget(int _type,QWidget *parent = 0);
    enum
    {
        kEnumIpc,
        kEnumSimulation
    };

signals:
    void signal_button_data(int _type,int _number);
public slots:
    void slot_pushbutton_line();
public:
    void set_text(QString _text,char _number);
private:
    QLabel *m_label_line;
    QPushButton *m_pushbutton_line;
    char m_type;
    char m_number;

};

//3.通道设置
class CPassagewayConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPassagewayConfigWidget(QWidget *parent = 0);

signals:
    void signal_button_data(int _type,int _number,int _cam_number,int _ipc_number);

    void signal_current_passageway(int _total,int _cam_number,int _pon_number);

public slots:
    void combobox_line0_index_changed(int index);
    void combobox_line1_index_changed(int index);

    void slot_button_clicked(int,int);

private:
    void combobox_data_handle();

    QWidget *m_widget_bg;
public:
    QLabel *m_label_line0;
    QComboBox *m_combobox_line0;
    QLabel *m_label_line0_1;
    QComboBox *m_combobox_line0_1;

    QVBoxLayout *m_vboxlayout_widget;

    CCamLineWidget *m_SimulationWidget1;
    CCamLineWidget *m_SimulationWidget2;
    CCamLineWidget *m_SimulationWidget3;
    CCamLineWidget *m_SimulationWidget4;
    CCamLineWidget *m_SimulationWidget5;
    CCamLineWidget *m_SimulationWidget6;

    CCamLineWidget *m_IPCWidget1;
    CCamLineWidget *m_IPCWidget2;
    CCamLineWidget *m_IPCWidget3;
    CCamLineWidget *m_IPCWidget4;
    CCamLineWidget *m_IPCWidget5;
    CCamLineWidget *m_IPCWidget6;
    CCamLineWidget *m_IPCWidget7;
    CCamLineWidget *m_IPCWidget8;
    CCamLineWidget *m_IPCWidget9;
    CCamLineWidget *m_IPCWidget10;

    QList<CCamLineWidget *> m_simulation_list;
    QList<CCamLineWidget *> m_ipc_list;



};


//镜像设置
class CImageSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CImageSetWidget(QWidget *parent = 0);

signals:

public slots:


public:
    QLabel *m_label_line1;
    QCheckBox *m_checkbox_line2_1;
    QLabel *m_label_line2_1;
    QCheckBox *m_checkbox_line2_2;
    QLabel *m_label_line2_2;
    QCheckBox *m_checkbox_line2_3;
    QLabel *m_label_line2_3;
    QCheckBox *m_checkbox_line2_4;
    QLabel *m_label_line2_4;

    QCheckBox *m_checkbox_line3_1;
    QLabel *m_label_line3_1;
    QCheckBox *m_checkbox_line3_2;
    QLabel *m_label_line3_2;
    QCheckBox *m_checkbox_line3_3;
    QLabel *m_label_line3_3;
    QCheckBox *m_checkbox_line3_4;
    QLabel *m_label_line3_4;

    QCheckBox *m_checkbox_line4_1;
    QLabel *m_label_line4_1;
    QCheckBox *m_checkbox_line4_2;
    QLabel *m_label_line4_2;
    QCheckBox *m_checkbox_line4_3;
    QLabel *m_label_line4_3;
    QCheckBox *m_checkbox_line4_4;
    QLabel *m_label_line4_4;

    QCheckBox *m_checkbox_line5_1;
    QLabel *m_label_line5_1;
    QCheckBox *m_checkbox_line5_2;
    QLabel *m_label_line5_2;
    QCheckBox *m_checkbox_line5_3;
    QLabel *m_label_line5_3;
    QCheckBox *m_checkbox_line5_4;
    QLabel *m_label_line5_4;

    QCheckBox *m_checkbox_line6_1;
    QLabel *m_label_line6_1;
    QCheckBox *m_checkbox_line6_2;
    QLabel *m_label_line6_2;


    QPushButton *m_pushbutton_default;

};

class CPonCamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPonCamWidget(char _type,QWidget *parent = 0);
    enum
    {
        kEnumPon,
        kEnumCam
    };
signals:
    void signal_notify_save_data();
public slots:
    void slot_pon_widet_combobox_change(int);
    void slot_cam_widet_combobox_change(int);

    void slot_pushbutton_default();
    void slot_pushbutton_search();

public:
    void modify_title(const QString &_title);
    void update_data(const CSystemPonInfo &_info,char _number);
    void update_data(const CSystemCamInfo &_info,char _number);
    void copy_data(CSystemPonInfo *_src,CSystemPonInfo *_dst);
    void copy_data(CSystemCamInfo *_src,CSystemCamInfo *_dst);

    void clean_copy_sheet_list();
    void add_copy_sheet_list(const QString &_text);

private:
    void pon_init();
    void cam_init();
    void set_type(char _type);

    void *read_data();
public:
    void save_data();
    QWidget *m_widget_bg;

    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
    QLabel *m_label_line8;
    QLabel *m_label_line9;

    QLineEdit *m_lineedit_line2;
    QPushButton *m_pushbutton_line2;
    QComboBox *m_combobox_line3;
    QLabel *m_label_line4_1;
    QComboBox *m_combobox_line4_1;
    QLabel *m_label_line4_2;
    QComboBox *m_combobox_line4_2;
    QLabel *m_label_line5_1;
    QComboBox *m_combobox_line5_1;
    QLabel *m_label_line5_2;
    QComboBox *m_combobox_line5_2;
    QLabel *m_label_line6_1;
    QComboBox *m_combobox_line6_1;
    QLabel *m_label_line6_2;
    QComboBox *m_combobox_line6_2;
    QComboBox *m_combobox_line7;
    QComboBox *m_combobox_line8;

    QCheckBox *m_checkbox_line9;

    QPushButton *m_pushbutton_default;
    char m_type;
    char m_number;

public:
    bool m_auto_change;

};

class CCameraConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCameraConfigWidget(QWidget *parent = 0);

signals:
    void signal_window_close();
public slots:

    void slot_button_1();
    void slot_button_2();
    void slot_button_3();
    void slot_button_4();

    void slot_pushbutton_image_set();
    void slot_save_data();
/*按钮编号:
int _type
int _number 按钮编号 1开始

*/
    void slot_button_config_data(int _type,int _number,int _cam_number,int _ipc_number);

    void slot_pushbutton_return();

    void slot_timer_init();

    void slot_camera_config_widget_event(const CSystemCameraConfig &_info);
    void slot_camera_config_widget_notify(char _type,char _cam_type,char _number);

    void slot_current_passageway(int _total,int _cam_number,int _pon_number);

private:
    void widget_init();
    void scroll_layout();
    void show_pon(const CSystemPonInfo &_info, const QString &_title,char _number);
    void show_cam(const CSystemCamInfo &_info, const QString &_title,char _number);

    void read_data();
public:
    void save_data(bool _dialog_tip = true);

    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;
    NavigateLeftButton *m_button_3;
    NavigateLeftButton *m_button_4;

    bool m_button_press_1;
    bool m_button_press_2;
    bool m_button_press_3;
    bool m_button_press_4;

    QLabel *m_label_space1;
    QLabel *m_label_space2;
    QLabel *m_label_space3;

    QScrollArea *m_scrollarea_1;

    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_vboxlayout_storage;
    QWidget *m_widget_main;
    QWidget *m_widget_submain_1;
    QWidget *m_widget_submain_2;
    QWidget *m_widget_submain_3; //镜像设置

    QWidget *m_widget_bg;
    QWidget *m_widget_scroll;

    CPonCamWidget *m_pon_widget;
    CPonCamWidget *m_cam_widget;
    CImageSetWidget *m_image_set_widget;

    CBaseConfigWidget *m_CBaseConfigWidget;
    CShowConfigWidget *m_CShowConfigWidget;
    CPassagewayConfigWidget *m_CPassagewayConfigWidget;

    int m_type;
    int m_number;
    int m_cam_number;
    int m_ipc_number;

};


#endif // CAMERA_CONFIG_WIDGET_H
