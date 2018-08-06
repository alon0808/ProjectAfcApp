#ifndef VIDEOTAPE_EXPORT_H
#define VIDEOTAPE_EXPORT_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

class CVideotapeExport : public QDialog
{
    Q_OBJECT
public:
    explicit CVideotapeExport(QWidget *parent = 0);
    typedef enum
    {
        KEnumButtonOk,
        KEnumButtonCancel
    }ButtonType;
    enum
    {
        kEnumTypeSource = 1,
        kEnumTypeAvi = 2
    };
signals:

public slots:
    void slot_pushbutton_close();
    void slot_pushbutton1();
    void slot_pushbutton2();

    void slot_pushbutton_source();
    void slot_pushbutton_avi();

public:
    CVideotapeExport::ButtonType get_result();
    void set_data(int _number,QString _size,bool _usb_state);
    char get_current_type() {return m_current_select;}
private:

    QWidget *m_widget_head;
    QLabel *m_label_title;
    QPushButton *m_pushbutton_close;

    QLabel *m_label_1;
    QLabel *m_label_1_1;
    QLabel *m_label_2;
    QLabel *m_label_2_1;
    QLabel *m_label_3;
    QLabel *m_label_3_1;

    QLabel *m_label_4;
    QLabel *m_label_5;


    QPushButton *m_pushbutton1;         //确定
    QPushButton *m_pushbutton2;         //取消
    ButtonType m_button_type;

   // QCheckBox *m_checkbox_1;
   // QCheckBox *m_checkbox_2;
   QPushButton *m_pushbutton_source;
   QPushButton *m_pushbutton_avi;
   char m_current_select;
};


#endif // VIDEOTAPE_EXPORT_H
