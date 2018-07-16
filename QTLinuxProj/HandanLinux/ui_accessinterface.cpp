#include "Macro_Proj.h"

#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"
#include <QDebug>

#include "customevent.h"
#include "UtilityProc.h"


int func_UiAccess(unsigned char* pParam) {


	return Ret_OK;
}

//////////////////////////////////////////////////////////////////////////////////

MyThread::MyThread(QObject *parent) :
	QThread(parent)
{
}


void MyThread::run()
{
	long long int i;
	for (i = 10000000000; i > 0; i--)
	{
		qDebug() << "now i is: " << i;
		QThread::msleep(1000);
		/*can not operate the GUI directly from another thread, I guess signal-slot mechanism should be used.
		//this->tEdit->setText("test");
		*/
		QString str = QString::number(i);
		emit dataChanged("now i is: " + str);
	}
}
