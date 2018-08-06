#ifndef POS_CONFIG_WIDGET_H
#define POS_CONFIG_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>

#include "class/common/scrollcontroller.h"
#include "common_data.h"

#include "class/common/define_screen.h"

class CSuportCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSuportCardWidget(QWidget *parent = 0);

signals:

public slots:
public:
    void resizeEvent(QResizeEvent *e);
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
//////////////////////////
    QList<QCheckBox *> m_checkbox_list;
    QList<QLabel *> m_label_list;

    QCheckBox *m_checkbox_line6_1;
    QLabel *m_label_line6_1;
    QCheckBox *m_checkbox_line6_2;
    QLabel *m_label_line6_2;
    QCheckBox *m_checkbox_line6_3;
    QLabel *m_label_line6_3;
    QCheckBox *m_checkbox_line6_4;
    QLabel *m_label_line6_4;

    QCheckBox *m_checkbox_line7_1;
    QLabel *m_label_line7_1;
    QCheckBox *m_checkbox_line7_2;
    QLabel *m_label_line7_2;
    QCheckBox *m_checkbox_line7_3;
    QLabel *m_label_line7_3;
    QCheckBox *m_checkbox_line7_4;
    QLabel *m_label_line7_4;

    QCheckBox *m_checkbox_line8_1;
    QLabel *m_label_line8_1;
    QCheckBox *m_checkbox_line8_2;
    QLabel *m_label_line8_2;
    QCheckBox *m_checkbox_line8_3;
    QLabel *m_label_line8_3;
    QCheckBox *m_checkbox_line8_4;
    QLabel *m_label_line8_4;

    QCheckBox *m_checkbox_line9_1;
    QLabel *m_label_line9_1;
    QCheckBox *m_checkbox_line9_2;
    QLabel *m_label_line9_2;
    QCheckBox *m_checkbox_line9_3;
    QLabel *m_label_line9_3;
    QCheckBox *m_checkbox_line9_4;
    QLabel *m_label_line9_4;

    CMyDefineScreen *m_define_screen;

};


class CPosConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPosConfigWidget(QWidget *parent = 0);
    
signals:
    void signal_window_close();
public slots:
     void slot_pos_config_widget_init();
     void slot_pos_config_widget_init1();
     void slot_pushbutton_config();
     void slot_pushbutton_return();
     void slot_pos_config_widget_event(const CSystemPosSetup &_info);
     void slot_currentIndex_changed(int index);
public:
    void save_data();
    void read_data();
    void resizeEvent(QResizeEvent *e);
private:
    void scroll_layout();
    void widget_init();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;
    QWidget *m_widget_main1;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg;

    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
    QLabel *m_label_line6_1;
    QLabel *m_label_line7_1;

    QLabel *m_label_line8;

    QComboBox *m_combobox_line1;
    QComboBox *m_combobox_line2;
    QComboBox *m_combobox_line3;
    QPushButton *m_pushbutton_line4;
    QComboBox *m_combobox_line5;
    QLineEdit *m_lineedit6;
    QLineEdit *m_lineedit7;
    QLineEdit *m_lineedit8;

    CSuportCardWidget *m_support_card_widget;

    CMyDefineScreen *m_define_screen;

};

#endif // POS_CONFIG_WIDGET_H











