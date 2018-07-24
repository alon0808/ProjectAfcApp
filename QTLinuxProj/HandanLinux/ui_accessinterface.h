#ifndef UI_ACCESSINTERFACE_H
#define UI_ACCESSINTERFACE_H


#include <QThread>
#include <QTextEdit>

typedef enum {
	FUNC_INVALID = 0,           // ,0x0000
	FUNC_SHOWTEXT = 1,          // æ˜¾ç¤ºæ–‡æœ¬,0x0001
}emCardExeRet;

typedef enum {
	FPARAM_INVALID = 0,         // invalid value,0x0000
	FPARAM_ROW = 1,            // row of screen,0x0001
	FPARAM_COLUMN = 2,          // column of screen,0x0002
}emFuncParam;

#define SCREEN_ROW			8
#define SCREEN_COLUMN		50
#define SCREEN_COLUMN_ALL	(SCREEN_COLUMN + 2)

#ifndef TYPEDEF_TEXT_POSITION
#define TYPEDEF_TEXT_POSITION
// ÎÄ±¾Î»ÖÃ
typedef enum
{
    TP_INVALID = 0,
    TP_LEFT,
    TP_MIDDLE,
    TP_RIGHT
}emTextPostion;
#endif


typedef struct {
    int row;
    int column;
    char *msg;	// for printf
    int textPosition;
}stMsg;

#ifdef __cplusplus
extern "C" {
#endif
	//extern int initUIAccessInterface();

    extern void func_UiAccess(int row, int column, char msg[], int textPosition = TP_INVALID);

#ifdef __cplusplus
}
#endif


///////////////////////////////////////////////////////
class MyThread : public QThread
{
	Q_OBJECT
public:
	explicit MyThread(QObject *parent = 0);
	virtual void run();
    //void showUI(stMsg *pMsgSt);

private:


signals:
    void dataChanged(stMsg *pMsgSt);

	public slots:
};

#endif // UI_ACCESSINTERFACE_H

