#ifndef VIDEOTAPE_QUERY_WIDGET_H
#define VIDEOTAPE_QUERY_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>

#include "./list_view/videotape_query/videotape_query_list.h"
#include "./list_view/common_view/common_query_list.h"

#include "protocol.h"
#include <QTimer>

class CVideotapeQueryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CVideotapeQueryWidget(char _type,QWidget *parent = 0);
    enum
    {
        kEnumVideotapeQuery,
        kEnumSystemManage
    };
signals:
    void signal_window_close();
    void signal_window_home();
    void signal_pushbutton_playback();
public slots:
    void slot_pushbutton_1();//搜  索
    void slot_pushbutton_2();//全选
    void slot_pushbutton_3();//回放
    void slot_pushbutton_4();//导出

    void slot_pushbutton_ok();
    void slot_pushbutton_cancel();
    void slot_pushbutton_return();
    void slot_combobox1_index_changed(int _index);
    void slot_combobox2_index_changed(int _index);
    void slot_combobox3_index_changed(int _index);

    void slot_videotape_query_widget_event(const CSystemVideotapeQueryEx &_info);
    void slot_timer_test();

public:
    void clean_date_list();

private:
    enum
    {
        kEnumShowQueryDate,
        kEnumShowQueryResult
    };

    void videotape_query_init();
    void videotape_query_date();
    void set_current_show(char _type);
    void get_month_info(bool _year_change,int &_month);
////////////////////////////////////////////////////new start  日期查询界面
    QLabel *m_label_1;
    QComboBox *m_combobox_1;
    QList<int> m_year_list;
    QLabel *m_label_2;
    QComboBox *m_combobox_2;
    QComboBox *m_combobox_3;

    QLabel *m_label_3;
    QLabel *m_label_4;
    QLabel *m_label_5;
    QLabel *m_label_6;

    QPushButton *m_pushbutton_ok;
    QPushButton *m_pushbutton_cancel;

    CCommonQueryList *m_date_select_list;

////////////////////////////////////////////////////new end

////////////////////////////////////////////////////new start  查询结果界面
    QLabel *m_label_7;
    QLabel *m_label_8;
    QLabel *m_label_9;
    QComboBox *m_combobox_4;
    QLineEdit *m_lineedit_1;
    QLineEdit *m_lineedit_2;
    CCommonQueryList *m_query_result_list;

////////////////////////////////////////////////////new end

    QVBoxLayout *m_pMainLayout;
    QWidget *m_widget_query_date;
    QWidget *m_widget_query_result;
    QLabel *m_label_test;

    QWidget *m_widget_bg;
    QWidget *m_widget_bg1;

    CVideotapeQueryList *m_CVideotapeQueryList;

    QLabel *m_label_line2;
    QComboBox *m_combobox_line2;

    QLabel *m_label_line3;
    QComboBox *m_combobox_line3;

    QLabel *m_label_line4;
    QComboBox *m_combobox_line4;

    QLabel *m_label_line5;
    QLineEdit *m_lineedit5;

    QLabel *m_label_line6;
    QLineEdit *m_lineedit6;

    QLabel *m_label_line7;
    QLineEdit *m_lineedit7;

    QPushButton *m_pushbutton_1;//搜  索
    QPushButton *m_pushbutton_2;//回  放
    QPushButton *m_pushbutton_3;//备  份
    QPushButton *m_pushbutton_4;//退  出

    char m_type;

    QTimer *m_test_timer;

    char m_export_value;
    QString m_export_result;

    int m_year;
    int m_month;
    int m_day;



};

#endif // VIDEOTAPE_QUERY_WIDGET_H














