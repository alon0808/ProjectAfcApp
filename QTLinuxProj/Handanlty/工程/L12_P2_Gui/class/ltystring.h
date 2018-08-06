#ifndef LTYSTRING_H
#define LTYSTRING_H

#include <QWidget>

class CLtyString :public QWidget
{
public:
    static CLtyString *getInstance();

    CLtyString();

public:
    QString m_confirm; //确认
    QString m_cancel;  //取消
    QString m_save;    //保存
    QString m_station_in;
    QString m_station_out;

private:
    static CLtyString *m_single;

};

#endif // LTYSTRING_H
