#ifndef MY_PUSHBUTTON_H
#define MY_PUSHBUTTON_H

#include <QWidget>
#include <QPushButton>


class CMyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CMyPushButton(QWidget *parent = 0);
    
signals:
    
public slots:
    void slot_pushbutton_clicked();
public:
    void set_pushbutton_enable(bool _enable);
    bool get_pushbutton_enable();

    void set_pushbutton_sheet(const QString &_open,const QString &_close);

private:
    bool m_enable;
    QString m_sheet_open;
    QString m_sheet_close;

};

#endif // MY_PUSHBUTTON_H
