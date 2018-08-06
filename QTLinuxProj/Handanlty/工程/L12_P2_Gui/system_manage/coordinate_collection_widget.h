#ifndef COORDINATE_COLLECTION_WIDGET_H
#define COORDINATE_COLLECTION_WIDGET_H

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>

#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>

#include "protocol.h"
class CCoordinateCollectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCoordinateCollectionWidget(QWidget *parent = 0);

signals:

public slots:
    void slot_pushbutton_help();
    void slot_pushbutton_ok();
    void slot_pushbutton_cancel();
    void slot_pushbutton_close();

    void slot_pushbutton_1(); //进站点
    void slot_pushbutton_2();//出站点
    void slot_pushbutton_3();//开始点
    void slot_pushbutton_4();//结束点
    void slot_pushbutton_5();//采集
    void slot_pushbutton_6();//采集

    void slot_coordinate_collection_widget_event(char _type,const CSystemCoordinateCollection &_info);

    void slot_combobox_1_change_index(int _index);
    void slot_combobox_1_1_change_index(int _index);
public:
    void sava_data();

private:
    void coordinate_collection_init();
    void coordinate_collection_help();
    void read_data();


    QVBoxLayout *m_pMainLayout;
    QWidget *m_widget_main;
    QLabel *m_label_test;
    QWidget *m_widget_bg;
    QWidget *m_widget_bg1;

    QWidget *m_widget_main_1;
    QWidget *m_widget_main_2;

//////////////////////////////////////////////左
    QLabel *m_label_line1;
    QComboBox *m_combobox_1;

    QLabel *m_label_line1_1;
    QComboBox *m_combobox_1_1;

    QLabel *m_label_line2;
    QComboBox *m_combobox_2;

    QLabel *m_label_line3;
    QComboBox *m_combobox_3;


    QLabel *m_label_line4;
    QComboBox *m_combobox_4;

    QLabel *m_label_line5;
    QComboBox *m_combobox_5;

    QLabel *m_label_line6;
    QComboBox *m_combobox_6;
//////////////////////////////////////////////右
    QPushButton *m_pushbutton_1;
    QPushButton *m_pushbutton_2;
    QPushButton *m_pushbutton_3;
    QPushButton *m_pushbutton_4;
    QPushButton *m_pushbutton_5;
    QPushButton *m_pushbutton_6;

    QLabel *m_label_line10;
    QLabel *m_label_line11;
    QLabel *m_label_line12;
    QLabel *m_label_line13;


    QPushButton *m_pushbutton_help;
    QPushButton *m_pushbutton_ok;
    QPushButton *m_pushbutton_cancel;

/////////////////////////////////////////////////帮助页面
    QLabel *m_label_help;
    QPushButton *m_pushbutton_close;
    QTextEdit *m_textedit;

    bool m_auto_update_data;

};

#endif // COORDINATE_COLLECTION_WIDGET_H
