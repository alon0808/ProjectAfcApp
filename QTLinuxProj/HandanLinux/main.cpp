#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;



    int fontId = QFontDatabase::addApplicationFont("/usr/fonts/msyh.ttf");
        qDebug() << "fontId = "<< fontId;
        if (fontId > -1){
        QString msyh = QFontDatabase::applicationFontFamilies(fontId).at(0);
        qDebug() <<"msyh = " <<msyh;
        QFont font(msyh);
        QApplication::setFont(font);
        }

        w.setWindowState(Qt::WindowMaximized);

    w.show();


    return a.exec();
}
