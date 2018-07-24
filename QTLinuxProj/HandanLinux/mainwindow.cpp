#include "mainwindow.h"
#include "ui_accessinterface.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextCodec>
#include <stdio.h>

#include "Application.h"

static char s_showContent[SCREEN_COLUMN_ALL * SCREEN_ROW];


static char *getRowBegin(char *pContent, int row) {
    char *pRetValue = NULL;
    unsigned int i;

    if (row == 0) {	// 第一行不用加1
        pRetValue = pContent;
    }
    else {
        for (i = 0; i < strlen(pContent); ++i) {
            if (pContent[i] == '\n') {
                --row;
                if (row <= 0) {	// 多少行
                    pRetValue = pContent + i + 1;
                    break;
                }
            }
        }
    }
    return pRetValue;
}


static void showInUIFatalError(char msg[]) {
	//showInUI(4, 1, msg, TP_MIDDLE);
	printf("showInUIFatalError:%s\n", msg);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    char buffer[1024];
	//Ui::s_mainUI = ui;
	// make it support chinese simple
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

	ui->setupUi(this);


	//ui->menuBar->addMenu(QString::fromLocal8Bit("主测试菜单"));

	//ui->menuBar->addMenu(("主测试111菜单"));

	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
	QString strMessage = QString::fromLocal8Bit("我是UTF8编码的文件：");
	//strMessage.
	qDebug() << strMessage;


	qDebug() << "UTF8编";


	//ui->textEdit->set
	ui->textEdit->setText(QString::fromLocal8Bit("san lian zhong rui"));

	ui->textEdit->append(QString::fromLocal8Bit("是UTF8编码的"));
	//ui->textEdit->setSizePolicy();

    ui->textEdit->setAttribute(Qt::WA_TranslucentBackground, true);

    ui->textEdit_2->setAttribute(Qt::WA_TranslucentBackground, true);

	//ui->verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
	//ui->verticalLayoutWidget->maximumSize();
	//connect(&render,SIGNAL(notify(int)),this,SLOT(OnNotify(int)));
	t = new MyThread();
	if (t != NULL) {
		connect(t, SIGNAL(dataChanged(QString)), this, SLOT(processUiAccess(QString)));
        t->start();
	}
    char *p = getcwd(buffer, sizeof(buffer));
    qDebug() << buffer;

    //InitApp(buffer, func_UiAccess);

    //StartApp();
}

MainWindow::~MainWindow()
{
	if (t != NULL) {
		t->terminate();
		delete t;
	}
	delete ui;
}

void MainWindow::processUiAccess(stMsg *pStMsg)
{
	int textLen = 0;
	char *pContent = s_showContent;
	char content[1000];
	//	CHARFORMAT cfm;
	//	int tmpI;// , rowStart, rowLen;
	int rowPix = 0;
	char *pTmpCh = NULL;
    //stMsg *pStMsg = (stMsg *)wparam;
	int row = pStMsg->row;
	int column = pStMsg->column;
	char *msg = pStMsg->msg;
	int textPosition = pStMsg->textPosition;
    QString msgW = NULL;

    msgW = QString(msg);
	textLen = (int)(msg == NULL ? SCREEN_COLUMN : strlen(msg));
	if (row == -1 && column == -1) {	// printf special
		printf(msg);
		goto showInUI_OVER;
	}
	if (row == 0 && column == 0 && msg == NULL) {
		clearInUI();
		goto showInUI_OVER;
	}
	if (row <= 0 || row > SCREEN_ROW || column <= 0 || column > SCREEN_COLUMN) {
        sprintf(content, "illegal show row,column:%d, %d", row, column);
		showInUIFatalError(content);
		goto showInUI_OVER;
	}
	--row;
	--column;

	// 文本及位置
	switch (textPosition)
	{
	case TP_INVALID:
		break;
	case TP_LEFT:
		column = 0;
		break;
	case TP_RIGHT:
		column = SCREEN_COLUMN - textLen;
		if (column < 0) {
			column = 0;
		}
		break;
	case TP_MIDDLE:
		column = (SCREEN_COLUMN - textLen) >> 1;
		if (column < 0) {
			column = 0;
		}
		break;
	default:
		break;
	}

	pTmpCh = getRowBegin(pContent, row);
	if (pTmpCh == NULL) {
        sprintf(content, "1illegal show row,column:%d, %d", row, column);
		showInUIFatalError(content);
		goto showInUI_OVER;
	}
	if (msg == NULL) {
		memset(pTmpCh, ' ', SCREEN_COLUMN);
	}
	else {
		// in case of 
		if (textLen + column > SCREEN_COLUMN) {
			column = SCREEN_COLUMN - textLen;
			if (column < 0) {
				column = 0;
				textLen = SCREEN_COLUMN;
			}
		}
		memcpy(pTmpCh + column, msg, textLen);
	}
    ui->textEdit->setText(msg);
	//pRichEditCtrl->SetWindowTextA(pContent);

#if 0
	// 设置字体
	pRichEditCtrl->SetSel(rowStart, rowLen);

	pRichEditCtrl->GetSelectionCharFormat(cfm);
	// 字体大小
	//cfm.yHeight = fontSize;
	// 字体颜色
	switch (fontColor)
	{
	case FC_INVALID:
		rgb = RGB(0, 0, 0);
		break;
	case FC_RED:
		rgb = RGB(255, 0, 0);
		break;
	case FC_WHITE:
		rgb = RGB(0, 0, 0);
		break;
	default:
		rgb = RGB(0, 0, 0);
		break;
	}
	cfm.dwEffects = CFM_BOLD;
	cfm.crTextColor = rgb;
	cfm.dwMask = CFM_COLOR | CFM_SIZE | CFM_FACE;
	pRichEditCtrl->SetSelectionCharFormat(cfm);

	pRichEditCtrl->GetWindowTextA(content, 1000);

	pRichEditCtrl = pRichEditCtrl;
	//pRichEditCtrl->Invalidate(FALSE);
#endif
showInUI_OVER:
	if (msg != NULL) {
		free(msg);
	}
	memset(pStMsg, 0x00, sizeof(stMsg));
//	return 0;
	//ui->textEdit->append(msg);
}


void MainWindow::clearInUI(int line) {
    //CAfcappWinUIDlg *pHndWin = this;
    QTextEdit *pRichEditCtrl = ui->textEdit;
    char content[100];// = "";
                      //	int index, length;
    int i, tmpI;
    char *pContent = s_showContent;
    char *pTmpCh = NULL;
    //QFont font1;

    //pRichEditCtrl->gette(&content);

    if (line == 0) {	// clear all
        //pRichEditCtrl->SetBackgroundColor(TRUE, RGB(255, 255, 255));
        // clear buffer
        tmpI = SCREEN_ROW*SCREEN_COLUMN_ALL;
        memset(pContent, ' ', tmpI);
        memset(pContent + tmpI - 2, '\0', 2);
       // font1.CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, 0, 1,\
            OUT_DEFAULT_PRECIS,\
            CLIP_DEFAULT_PRECIS,\
            DEFAULT_QUALITY,\
            DEFAULT_PITCH | FF_SWISS,\
            _T("宋体"));
        //pRichEditCtrl->SetFont(&font1);
        // set line split character
        for (i = 0; i < SCREEN_ROW - 1; ++i) {
            tmpI = (i + 1) * SCREEN_COLUMN_ALL - 2;
            pContent[tmpI] = '\r';
            pContent[tmpI + 1] = '\n';
        }
        //pRichEditCtrl->SetWindowTextA(pContent);


        //tmpI = pRichEditCtrl->GetWindowTextA(pContent, 1000);
    }
    else {	// clear one line
        if (line > SCREEN_ROW || line <= 0) {
            sprintf(content, "illegal show line:%d", line);
            showInUIFatalError(content);
            return;
        }
        --line;
        pTmpCh = getRowBegin(pContent, line);
        if (pTmpCh == NULL) {
            sprintf(content, "1illegal show row,column:%d, %d", line, 0);
            showInUIFatalError(content);
            return;
        }

        memset(pTmpCh, ' ', SCREEN_COLUMN);
        ui->textEdit->setText(pContent);
        //pRichEditCtrl->SetWindowTextA(pContent);
    }
}


