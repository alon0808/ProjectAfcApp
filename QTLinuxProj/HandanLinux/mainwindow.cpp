#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextCodec>

#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Ui::s_mainUI = ui;
    // make it support chinese simple
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    ui->setupUi(this);


    //ui->menuBar->addMenu(QString::fromLocal8Bit("�����Բ˵�"));

    //ui->menuBar->addMenu(("������111�˵�"));

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QString strMessage = QString::fromLocal8Bit("����UTF8������ļ���");
    //strMessage.
    qDebug() << strMessage;


    qDebug() << "UTF8��";


    //ui->textEdit->set
    ui->textEdit->setText(QString::fromLocal8Bit("san lian zhong rui"));

    ui->textEdit->append(QString::fromLocal8Bit("��UTF8�����"));
    //ui->textEdit->setSizePolicy();

    //ui->verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
    //ui->verticalLayoutWidget->maximumSize();
    //connect(&render,SIGNAL(notify(int)),this,SLOT(OnNotify(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

