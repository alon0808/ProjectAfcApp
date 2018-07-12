#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void QResizeEvent(const QSize &size, const QSize &oldSize);

private:
    Ui::MainWindow *ui;
    //static Ui::MainWindow *s_mainUI;
};

#endif // MAINWINDOW_H
