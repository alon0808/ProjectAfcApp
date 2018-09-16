#ifndef DIALOGINPUT_H
#define DIALOGINPUT_H

#include <QDialog>
#include <QTimer>

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
	int Init(QString title, QString msg, char *pVal);
	QString GetValue();
	void ChangeValue(int addormunus);
	char * NextNumber();
	void stopTimer();

};

#endif // DIALOGINPUT_H
