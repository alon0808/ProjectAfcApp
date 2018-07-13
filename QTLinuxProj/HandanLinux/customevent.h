#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H


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

#endif // CUSTOMEVENT_H
