#ifndef DIALOGINPUT_H
#define DIALOGINPUT_H

#include <QDialog>
#include <QTimer>


typedef enum {
	ms_idle = 0x00000000,
	ms_getDevInfo = 0x00000100,
	ms_setDevId = 0x00000001,
	ms_setUnpayDevId = 0x00000002,
	ms_setUnpayDownKey = 0x00000003,
	ms_maskSet = 0x000000FF,
	ms_showMenu = 0x80000000,
	ms_setMenu = 0x40000000,
}emMenuStatus;

namespace Ui {
	class DialogInput;
}

class DialogInput : public QDialog
{
	Q_OBJECT

public:
	explicit DialogInput(QWidget *parent = 0);
	~DialogInput();

private:
	Ui::DialogInput *ui;
	int m_curPos;
	int m_maxlen;
	char textVal[100];
	QTimer *m_timeValChange;
	int m_addormunus;

private slots:
	void onTimeout();

public:
	int Init(int type, QString title, int len, char *pVal);
	QString GetValue();
	void ChangeValue(int addormunus);
	char * NextNumber();
	void stopTimer();

};

#endif // DIALOGINPUT_H
