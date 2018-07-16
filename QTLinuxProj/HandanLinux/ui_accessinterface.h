#ifndef UI_ACCESSINTERFACE_H
#define UI_ACCESSINTERFACE_H


#include <QThread>
#include <QTextEdit>

typedef enum {
	FUNC_INVALID = 0,           // ,0x0000
	FUNC_SHOWTEXT = 1,          // 显示文本,0x0001
}emCardExeRet;

typedef enum {
	FPARAM_INVALID = 0,         // invalid value,0x0000
	FPARAM_ROW = 1,            // row of screen,0x0001
	FPARAM_COLUMN = 2,          // column of screen,0x0002
}emFuncParam;

#ifdef __cplusplus
extern "C" {
#endif
	//extern int initUIAccessInterface();

	extern int func_UiAccess(unsigned char* pParam);

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

private:


signals:
	void dataChanged(QString);

	public slots:
};

#endif // UI_ACCESSINTERFACE_H

