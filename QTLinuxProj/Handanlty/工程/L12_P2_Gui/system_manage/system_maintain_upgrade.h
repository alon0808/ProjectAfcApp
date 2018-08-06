#ifndef SYSTEM_MAINTAIN_UPGRADE_H
#define SYSTEM_MAINTAIN_UPGRADE_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QThread>

#include "./list_view/common_view/common_query_list.h"

#include "class/common/define_screen.h"

class CFindUpgradeFile;

class SystemMaintainUpgrade : public QDialog
{
    Q_OBJECT
public:
    explicit SystemMaintainUpgrade(char _type,QWidget *parent = 0);
    typedef enum
    {
        KEnumButtonOk,
        KEnumButtonCancel
    }ButtonType;

    enum
    {
        kEnumMountCreateFail,
        kEnumMountSuccess,//挂载U盘成功
        kEnumMountFail,   //挂载U盘失败
        kEnumNoFile,      //U盘没有升级文件
        kEnumGetFile,      //获取升级文件
        kEnumClean        //清除列表

    };

signals:

public slots:
    void slot_pushbutton_search();
    void slot_pushbutton_close();
    void slot_pushbutton1();
    void slot_pushbutton2();

    void slot_upgrade_notify_event(char _type,void *_param);

public:
    void add_data(CSoftwareUpgrade *_data);
    void set_title(const QString &_title);
    SystemMaintainUpgrade::ButtonType get_result();
    void *get_row_info();

    void start_get_upgrade();

private:
    void resizeEvent(QResizeEvent *e);
    void test_init();

    QPushButton *m_pushbutton_close;

    QPushButton *m_pushbutton_search;   //搜索
    QPushButton *m_pushbutton1;         //确定
    QPushButton *m_pushbutton2;         //取消
    ButtonType m_button_type;
    QLabel *m_label_title;  //标题

    QLabel *m_label_udisk_status;

    CCommonQueryList *m_upgrade_list;

    char m_type;

    CMyDefineScreen *m_define_screen;

    CFindUpgradeFile *m_CFindUpgradeFile;

};

class CFindUpgradeFile :public QThread
{
    Q_OBJECT

signals:
    void signal_upgrade_notify_event(char _type,void *_param);

public:
    CFindUpgradeFile();

private:
    void run();

private:


private:

};



#endif // SYSTEM_MAINTAIN_UPGRADE_H
