#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_accessinterface.h"

namespace Ui {
class MainWindow;
//static MainWindow *s_mainUI;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //static Ui::MainWindow *s_mainUI;
    MyThread *t;

private slots:
    void processUiAccess(QString msg);
};

#endif // MAINWINDOW_H
