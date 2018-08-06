#ifndef PARENT_WIDGET_H
#define PARENT_WIDGET_H

#include <QDialog>
#include <QWidget>

class CParentWidget:public QDialog
{
    Q_OBJECT
public:
    explicit CParentWidget(QWidget *parent);
    
    virtual bool keyboard_value(int) = 0;

signals:
    
public slots:
    
};


class CMyWidget:public QWidget
{
    Q_OBJECT
public:
    explicit CMyWidget(QWidget *parent);

    virtual bool keyboard_value(int) = 0;

signals:

public slots:

};


#endif // PARENT_WIDGET_H
