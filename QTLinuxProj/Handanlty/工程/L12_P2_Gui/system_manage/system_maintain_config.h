#ifndef SYSTEM_MAINTAIN_CONFIG_H
#define SYSTEM_MAINTAIN_CONFIG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QTableWidget>

#include "class/common/scrollcontroller.h"

class CConfigWidget :public QWidget
{
    Q_OBJECT
public:
    CConfigWidget(QWidget *parent = 0);

public slots:

private:
    QLabel *m_label_filename;
    QCheckBox *m_checkbox;
    QComboBox *m_combobox;

};


class CSystemMaintainConfig : public QDialog
{
    Q_OBJECT
public:
    explicit CSystemMaintainConfig(QWidget *parent = 0);
    typedef enum
    {
        KEnumButtonOk,
        KEnumButtonCancel
    }ButtonType;

signals:

public slots:
    void slot_pushbutton_close();
    void slot_pushbutton1();
    void slot_pushbutton2();

public:

    CSystemMaintainConfig::ButtonType get_result();

private:
    void tablewidget_init();
    void add_data(QWidget *_widget);

    QPushButton *m_pushbutton_close;

    QPushButton *m_pushbutton1;         //确定
    QPushButton *m_pushbutton2;         //取消
    ButtonType m_button_type;
    QLabel *m_label_title;  //标题

    QTableWidget *m_tablewidget;
    ScrollController *m_ScrollController;


};


#endif // SYSTEM_MAINTAIN_CONFIG_H
