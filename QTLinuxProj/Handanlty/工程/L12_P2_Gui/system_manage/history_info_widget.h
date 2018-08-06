#ifndef HISTORY_INFO_WIDGET_H
#define HISTORY_INFO_WIDGET_H

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>

class CHistoryInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CHistoryInfoWidget(QWidget *parent = 0);

signals:

public slots:

    void slot_pushbutton_reset_line1_1();
    void slot_pushbutton_reset_line2();

    void slot_pushbutton_reset_line4();
    void slot_pushbutton_reset_line5();
    void slot_pushbutton_reset_line6();
    void slot_pushbutton_reset_line7();
    void slot_pushbutton_reset_line8();

    void slot_pushbutton_save();
    void slot_pushbutton_reset();



private:
    void history_info_init();

        QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_vboxlayout_widget;
    QScrollArea *m_scrollarea_1;
    QWidget *m_widget_main;
    QLabel *m_label_test;

    QLabel *m_label_line1;
    QLabel *m_label_line1_1;
    QLabel *m_label_line1_2;
    QPushButton *m_pushbutton_reset_line1_1;

    QLabel *m_label_line2;
    QLabel *m_label_line2_1;
    QLabel *m_label_line2_2;
    QPushButton *m_pushbutton_reset_line2;


    QLabel *m_label_line3;
    QLabel *m_label_line3_1;
    QLabel *m_label_line3_2;

    QLabel *m_label_line4;
    QLabel *m_label_line4_1;
    QLabel *m_label_line4_2;
    QPushButton *m_pushbutton_reset_line4;

    QLabel *m_label_line5;
    QLabel *m_label_line5_1;
    QLabel *m_label_line5_2;
    QPushButton *m_pushbutton_reset_line5;

    QLabel *m_label_line6;
    QLabel *m_label_line6_1;
    QLabel *m_label_line6_2;
    QPushButton *m_pushbutton_reset_line6;

    QLabel *m_label_line7;
    QLabel *m_label_line7_1;
    QLabel *m_label_line7_2;
    QPushButton *m_pushbutton_reset_line7;

    QLabel *m_label_line8;
    QLabel *m_label_line8_1;
    QLabel *m_label_line8_2;
    QPushButton *m_pushbutton_reset_line8;


    QLabel *m_label_line9;
    QLabel *m_label_line9_1;
    QLabel *m_label_line9_2;

    QLabel *m_label_line10;
    QLabel *m_label_line10_1;
    QLabel *m_label_line10_2;

    QLabel *m_label_line11;
    QLabel *m_label_line11_1;
    QLabel *m_label_line11_2;

    QPushButton *m_pushbutton_save;
    QPushButton *m_pushbutton_reset;

};
#endif // HISTORY_INFO_WIDGET_H
