#include "customevent.h"


const QEvent::Type customEvent::MyEventType = (QEvent::Type)QEvent::registerEventType(QEvent::User+100);

TBOOL MySender::notify(QObject *r, QEvent *e)
{
    qDebug()<<"notify got event MyEventType !\n";
    if(e->type() == customEvent::MyEventType){
        qDebug()<<"MyEventType is coming!\n";
        //return false;
    }
    return QCoreApplication::notify(r, e);
}

TBOOL MySender::sendEvent(QObject *r, QEvent *e)
{
    qDebug()<<"sendEvent got event MyEventType !";
    qDebug()<<"e->type() == "<<e->type();
    qDebug()<<"MyEventType is "<<customEvent::MyEventType;
    if(e->type() == customEvent::MyEventType){
        qDebug()<<"MyEventType is coming!\n";
        //return false;
    }
    return QCoreApplication::sendEvent(r, e);
}

void MySender::postEvent(QObject *r, QEvent *e)
{
    qDebug()<<"postEvent got event MyEventType!\n";
    if(e->type() == customEvent::MyEventType){
        qDebug()<<"MyEventType is coming!\n";
        //return false;
    }
    QCoreApplication::postEvent(r, e);
}

