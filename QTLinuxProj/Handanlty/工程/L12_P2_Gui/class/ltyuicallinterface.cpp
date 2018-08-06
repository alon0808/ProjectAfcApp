#include "ltyuicallinterface.h"

#include <stdio.h>

#include <QApplication>
#include <QTextCodec>
#include <QWSServer>

#include "LtyCommonDefine.h"

#include "main_page.h"
#include "mainwindow.h"

#include "msglist.h"

static const char *app = "./ltyapp";
static const char *appqws = "-qws";
static int qtargc = 2;
static QApplication *pApp;

static ltySendMessge2UiCallback_ pltySendMessge2UiCallback;

int lytSendMsgToUiAppCall(cJSON *root, char _type,ltySendMessge2UiCallback_ sendMsgCallBack)
{

    //printf("lytSendMsgToUiAppCall\n");
    if(NULL == *sendMsgCallBack)
    {
        printf("sendMsgCallBack is NULL.\n");
        return -1;
    }

    if(NULL == *pltySendMessge2UiCallback)
    {
        pltySendMessge2UiCallback = sendMsgCallBack;
    }

   if(root)
   {
        GuiMsgListAdd(root, 0, 0,_type);//添加保存指针到队列
   }
    return 0;
}

TOUCH_ATTR_S g_prev_point = {0,0,0};
#include "main_page.h"
int lytTouchEvent(void *_param)
{
    int ret_value = 0;
    TOUCH_ATTR_S *pTouch = (TOUCH_ATTR_S *)_param;
    do
    {
        //1.需要重新计时定时器

        MainWindow *pMainWindow = MainWindow::getInstance();
        if(pMainWindow)
        {
            pMainWindow->reset_touch_timer();
        }
        if(g_prev_point.x == pTouch->x && g_prev_point.y == pTouch->y && g_prev_point.up == pTouch->up)
        {
            //printf("lytTouchEvent() prev.x:%d,prev.y:%d  current.x:%d,current.y:%d\n",
            //g_prev_point.x,g_prev_point.y,pTouch->x,pTouch->y);
            break;
        }
        g_prev_point.x = pTouch->x;
        g_prev_point.y = pTouch->y;
        g_prev_point.up = pTouch->up;
        //pMainPage->handle_mouse_event(pTouch->x,pTouch->y,false);

        ret_value = 1;

    }while(0);

    return ret_value;
}

int gui_send_message2system(cJSON *root)
{
    int ret = -1;

    if(NULL == root)
    {
        printf("root is null.\n");
        return -1;
    }
    if(NULL != *pltySendMessge2UiCallback)
    {
       ret = pltySendMessge2UiCallback( root );
    }

    return ret;
}

#include "./include/myinputpanelcontext.h"
#include "class/common_interface.h"
//#include "LtyCommonStyle.h"


#define LTY_COMBOBOX_STYLE           "QComboBox {color:#94c6ff;font:%5px;border-image:url(:/img/res/main_page/menu/system_manage/combox_1.png);border 1px solid #275ba7;min-height: 40px; }\
    QComboBox:disabled{color:#536579;border-image:url(:/img/res/main_page/menu/system_manage/combox_2.png);border 1px solid #275ba7;min-height: 60px; }\
    QComboBox QAbstractItemView::item {\
    min-height: %1px; \
    min-width:  50px;\
}\
\
QListView{\
border:1px solid #275ba7;\
font:%6px;\
}\
QListView::item {\
    background-color: #0F1D31;\
    color:#94c6ff;\
}\
QListView::item:selected {\
     background:#80E3FF;\
     color:#0f1d31;\
}\
\
QComboBox::down-arrow{\
image:url(:/img/res/main_page/menu/system_manage/button_drop_down.png);\
width:  %2px;\
height:  %3px;\
}\
QComboBox::down-arrow:disabled{\
image:url(:/img/res/main_page/menu/system_manage/button_drop_down_disable.png);\
width:  %7px;\
height:  %8px;\
}\
QComboBox::drop-down{\
border:none;\
                background:transparent;\
                min-width:  %4px;\
}"

#define SCROLL_SIZE 50

#if 1
#define LTY_QSCROLLBAR_VERTICAL \
"QScrollArea{border: 0px solid #275ba7;background:#0E1D30;}"\
"QScrollBar:vertical{width:%1px;background:#0E1D30;margin:0px,0px,0px,0px;padding-top:%2px;padding-bottom:%3px;}"\
"QScrollBar::handle:vertical{width:%4px;border-image:url(:/img/res/main_page/menu/system_manage/scroll_handle.png);border-radius:0px;min-height:100px;}"\
"QScrollBar::handle:vertical:hover{width:%5px;border-image:url(:/img/res/main_page/menu/system_manage/scroll_handle.png);border-radius:0px;min-height:100px;}"\
"QScrollBar::add-line:vertical{height:%6px;width:%7px;border-image:url(:/img/res/main_page/menu/system_manage/down.png);subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical{height:%8px;width:%9px;border-image:url(:/img/res/main_page/menu/system_manage/up.png);subcontrol-position:top;}"\
"QScrollBar::add-line:vertical:hover{height:%10px;width:%11px;border-image:url(:/img/res/main_page/menu/system_manage/down.png);subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical:hover{height:%12px;width:%13px;border-image:url(:/img/res/main_page/menu/system_manage/up.png);subcontrol-position:top;}"\
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:#0E1D30;}"

#else
#define LTY_QSCROLLBAR_VERTICAL \
"QScrollArea{border: 0px solid #275ba7;background:#0E1D30;}"\
"QScrollBar:vertical{width:50px;background:#0E1D30;margin:0px,0px,0px,0px;padding-top:50px;padding-bottom:50px;}"\
"QScrollBar::handle:vertical{width:30px;background-color:#275ba7;border-radius:0px;min-height:20px;}"\
"QScrollBar::handle:vertical:hover{width:30px;background-color:#275ba7;border-radius:0px;min-height:20px;}"\
"QScrollBar::add-line:vertical{height:50px;width:50px;background:#BF0000;subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/up.png);subcontrol-position:top;}"\
"QScrollBar::add-line:vertical:hover{height:50px;width:50px;background:#BF0000;subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical:hover{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/up.png);subcontrol-position:top;}"\
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:#0E1D30;}"
#endif

#define SCROLL_VERTICAL	"QScrollBar:vertical\
                    {\
                    border:0px;\
                    width:40px;\
                    background-color:rgba(255,255,255,0%);\
                    margin:0px,0px,0px,0px;\
                    }\
                    QScrollBar::handle:vertical\
                    {\
                    width:40px;\
                    background-color:%2;\
                    border-radius: 3px;\
                    min-height:100px;\
                    }\
                    QScrollBar::add-line:vertical\
                    {\
                    width: 0px;\
                    background-color:rgba(255,255,255,0%);\
                    }\
                    QScrollBar::sub-line:vertical\
                    {\
                    width: 0px;\
                    background-color:rgba(255,255,255,0%);\
                    }\
                    QScrollBar::add-page:vertical\
                    {\
                    background:transparent;\
                    }\
                    QScrollBar::sub-page:vertical\
                    {\
                    background:transparent;\
                    }"


//QComboBox::down-arrow {\
//    border-image:url(:/img/res/systemManage/xljt.png);\
//    min-width:26;

#include "class/common/glappeventfilter.h"
#include "class/common/define_screen.h"
int lytUiappStart()
{

    char *qtargv[2];
    qtargv[0] = (char*)app;
    qtargv[1] = (char*)appqws;

    init_touch_event();

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    printf("qtargc=%d argv[0]=%s argv[1]=%s \n", qtargc, qtargv[0],qtargv[1]);
    pApp = new QApplication(qtargc, qtargv);

    CMyDefineScreen *m_define_screen = new CMyDefineScreen();

    QFont font;
    font.setFamily(("msyh"));
    font.setBold(false);//font-weight:bold;
 //"QComboBox QAbstractItemView{border: 2px solid #32435E;selection-background-color:qlineargradient(x1:0,y1:0,x2:0, y2:1, stop:0 rgba(21,69,160), stop:0.25 rgb(33,51,82,60),stop:0.751 rgb(24,58,119,60), stop:1.0 rgba(0,55,170));background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #1B2534, stop: 0.4 #010101,stop: 0.5 #000101, stop: 1.0 #1F2B3C);}");
    pApp->setStyleSheet("*{font-size:26px;color:#000000;}"+
                        QString(LTY_COMBOBOX_STYLE).arg(m_define_screen->get_change_factor_y(80)).arg(m_define_screen->get_change_factor_y(40)).arg(m_define_screen->get_change_factor_y(54))
                        .arg(m_define_screen->get_change_factor_y(55))
                        /*font*/
                        .arg(m_define_screen->get_change_factor_y(26)).arg(m_define_screen->get_change_factor_y(26))
                        .arg(m_define_screen->get_change_factor_y(40)).arg(m_define_screen->get_change_factor_y(54)) +
                        QString(LTY_QSCROLLBAR_VERTICAL).arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        .arg(m_define_screen->get_change_factor_y(SCROLL_SIZE))
                        );
    pApp->setInputContext((MyInputPanelContext *)CCommonInterface::get_method_widget());
#ifdef CURRENT_ARM_RUN
    pApp->installEventFilter(GlAppEventFilter::getInstance());
#endif
    pApp->setFont(font);
#ifdef CURRENT_ARM_RUN

#ifdef APPLICATION_TYPE_P2
    QWSServer::setCursorVisible(false);
#endif

#if( !defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
    QWSServer::setCursorVisible(false);
#endif
    QWSServer::setBackground(QColor(0,0,0,0));
#endif
    //QWSServer::setBackground(QColor(0,0,0,0));

    //CSystemManage dialog;
    //dialog.show();

    //CMainPage main_page;
    //main_page.show();
    MainWindow main_window;

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    main_window.move((screenRect.width() - main_window.width())/2,(screenRect.height() - main_window.height())/2);
    main_window.show();


    return pApp->exec();

}
