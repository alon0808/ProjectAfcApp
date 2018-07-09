#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuBar->addMenu("skdf;skdfs");

    //ui->textEdit->set
    ui->textEdit->setText("san lian zhong rui");

    ui->verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
    //ui->verticalLayoutWidget->maximumSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent * event)
{
   // ui->verticalLayout->setstr
}
