#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <QtDebug>
#include <QEvent>
#include <QCoreApplication>
#include <QObject>

class customEvent : public QEvent
{
public:
    static const QEvent::Type MyEventType;
    customEvent():QEvent(customEvent::MyEventType){
        qDebug()<<"construct customEvent MyEventType is "<<MyEventType;
    }
};
class MySender:public QCoreApplication
{
public:
    MySender(int argc, char *argv[]):QCoreApplication(argc, argv) {}
    bool notify(QObject *r, QEvent *e);
    static bool sendEvent(QObject *r, QEvent *e);
    static void postEvent(QObject *receiver, QEvent *event);
};

#if 0
class MyArmy :public QObject
{
public:
    MyArmy() ;
    void MyEventHandler(QEvent *e);
	bool event(QEvent *e);
};
//
class MyWatcher :public QObject
{
public:
    MyWatcher() ;
	bool eventFilter(QObject *r, QEvent *e);
};
#endif

#endif // CUSTOMEVENT_H
