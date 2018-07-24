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
    MyThread *t;

private slots:
    void clearInUI(int line=0);
    void processUiAccess(stMsg *pStMsg);
};

#endif // MAINWINDOW_H
