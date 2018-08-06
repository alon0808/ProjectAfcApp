#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QWidget>

#include <QCheckBox>

class MyCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    MyCheckBox(QWidget *parent =0);
    ~MyCheckBox();
    void setEnableUpdate(bool flag);

signals:
    void signalStateChanged(int);

public slots:
    void slotStateChanged(int state);

private:
    bool m_enableUpdate;
};

#endif // MYCHECKBOX_H
