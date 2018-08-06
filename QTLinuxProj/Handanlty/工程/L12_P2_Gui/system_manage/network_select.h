#ifndef NETWORK_SELECT_H
#define NETWORK_SELECT_H

#include <QDialog>
#include <QComboBox>

#include "./list_view/common_view/common_query_list.h"

class CNetworkSelect : public QDialog
{
     Q_OBJECT
public:
    explicit CNetworkSelect(QWidget *parent = 0);
    typedef enum
    {
        KEnumButtonOk,
        KEnumButtonCancel
    }ButtonType;

    enum
    {
        kEnumSelectNetwork,
        kEnumSelectConnect
    };

    enum
    {
        kEnumStatusNoConnect,
        kEnumStatusConnecting,
        kEnumStatusConnected,
        kEnumStatusFail
    };
    enum
    {
        kEnumGetData = 1,
        kEnumConnectSuccess = 2,
        kEnumConnectFail = 3
    };
signals:

public slots:

    void slot_pushbutton_close();
    void slot_pushbutton1();
    void slot_pushbutton2();

    void slot_pushbutton_connect();
    void slot_timer_connect();
    void slot_timer_connect_test();

    void slot_network_select(char _type,const CWifiSearch &_param);

public:

    CNetworkSelect::ButtonType get_result();

    void set_interface_type(char _type);
private:
    void show_select_network();
    void show_select_connect();
    void test_init();
    void select_init();


    QPushButton *m_pushbutton_close;


    QPushButton *m_pushbutton1;         //确定
    QPushButton *m_pushbutton2;         //取消
    ButtonType m_button_type;
    QLabel *m_label_title;  //标题

    CCommonQueryList *m_select_list;


    QWidget *m_widget_line1;
    QWidget *m_widget_line2;
    QWidget *m_widget_line3;

    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;

    QLabel *m_label_line1_1;
    QComboBox *m_combobox_line2;
    QLineEdit *m_lineedit3;

    QPushButton *m_pushbutton_connect;

    QTimer *m_timer;
    QTimer *m_timer_test;
    char m_status;
    char m_connect_index;

    CWifiConnectInfo m_CWifiConnectInfo;


};

#endif // NETWORK_SELECT_H







