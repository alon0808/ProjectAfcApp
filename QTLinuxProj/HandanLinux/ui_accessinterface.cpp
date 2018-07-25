#include "Macro_Proj.h"

#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"
#include <QDebug>

#include "customevent.h"
#include "UtilityProc.h"

#define MSG_ST_COUNT	20
static stMsg s_msgSt;//[MSG_ST_COUNT];
static int s_curPos = 0;

static pthread_mutex_t s_myMutex = PTHREAD_MUTEX_INITIALIZER;


static MyThread *myThread = NULL;



void func_UiAccess(int row, int column, char msg[], int textPosition ) {
    stMsg *pMsgSt;
    int len;
    int timeOut = 100;

    pthread_mutex_lock(&s_myMutex);
    s_curPos = 1;
    pMsgSt = &s_msgSt;// + s_curPos;
        if (pMsgSt->msg != NULL) {
            printf("the message queue is full");
        }
        len = (msg == NULL) ? 0 : strlen(msg);

        pMsgSt->row = row;
        pMsgSt->column = column;
        /*if (len > SCREEN_COLUMN) {
            len = SCREEN_COLUMN;
        }*/
        if (len > 0) {
            pMsgSt->msg = (char *)malloc(strlen(msg) + 1);
            strcpy(pMsgSt->msg, msg);
        }
        else {
            pMsgSt->msg = NULL;
        }
        //pMsgSt->msg[len] = '\0';
        pMsgSt->textPosition = textPosition;
s_curPos = 2;
    while(s_curPos == 2){
        if (timeOut <= 0){
        sleep(10);
        --timeOut;
        }
    }
    s_curPos = 0;
//myThread->showUI(pMsgSt);

    pthread_mutex_unlock(&s_myMutex);
    //return Ret_OK;
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief MyThread::MyThread
/// \param parent
///

MyThread::MyThread(QObject *parent) :
	QThread(parent)
{
myThread = this;
}


void MyThread::run()
{
    long long int i;

    while(true)
    {
        qDebug() << "now i is: " << i;
        QThread::msleep(10000);
        if (s_curPos == 2){
            /*can not operate the GUI directly from another thread, I guess signal-slot mechanism should be used.
            //this->tEdit->setText("test");
            */
            //QString str = QString::number(i);

            emit dataChanged(&s_msgSt);

            s_curPos = 3;
        }
    }
}
