

#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>

#include "mainwindow.h"
#include "menu_widget.h"
#include "main_page.h"

#include "class/ltyuicallinterface.h"

int main(int argc, char *argv[])
{
    lytUiappStart();
#if 1
    QApplication a(argc, argv);

    //font.setPointSize(160);
    //font.setFamily(("wenquanyi"));
    QFont font;

    font.setFamily(("msyh"));
    font.setBold(false);

    a.setFont(font);

    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);

#if 0
    QTranslator translator;
    bool b = false;
    b = translator.load("app_cn.qm");

    a.installTranslator(&translator);
#endif


    CMainPage main_page;

    main_page.show();
   // MainWindow w;
    //a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    //w.setAttribute(Qt::WA_QuitOnClose,true);
   // w.show();
    //CMenuWidget menu_widget;

    //menu_widget.show();
    return a.exec();
#endif
}
