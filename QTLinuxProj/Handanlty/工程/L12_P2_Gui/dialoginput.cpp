#include "dialoginput.h"
#include "ui_dialoginput.h"

#include <string.h>
#include <stdio.h>
#include "libHandanCore.h"
#include "UtilityProc.h"

#define TIMER_TIMEOUT	400

DialogInput::DialogInput(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogInput)
{
	ui->setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);

	m_curPos = 0;
	m_maxlen = 0;
	textVal[0] = '\0';
	m_addormunus = 1;

	m_timeValChange = new QTimer(this);
	connect(m_timeValChange, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

DialogInput::~DialogInput()
{
	delete ui;
}

int DialogInput::Init(int type, QString title, int len, char *pVal) {
	char tmpBuffer[1000];

	ui->label_title->setText(title);
	//ui->label_title->adjustSize();

	if (type == ms_setUnpayDownKey) {
		ui->lineEdit_value->hide();
		ui->buttonBox->setStandardButtons(QDialogButtonBox::NoButton);

		SetDevParam(dpt_unionpayDownKey, NULL, 0);

		this->hide();
	}
	else {
		if (type == ms_getDevInfo) {
			ui->lineEdit_value->hide();
			ui->buttonBox->setStandardButtons(QDialogButtonBox::NoButton);

			BytesToChars(pVal, len, tmpBuffer, 1000);
			//SetDevParam(dpt_unionpayDownKey, NULL, 0);
			ui->label_Infor->setText(tmpBuffer);
		}
		else {
			ui->lineEdit_value->show();
			ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
			if (pVal != NULL) {
				ui->lineEdit_value->setText(pVal);
				m_maxlen = strlen(pVal);
				if (m_maxlen > sizeof(textVal) - 1) {
					return -1;
				}
				memcpy(textVal, pVal, m_maxlen);
			}
			else {
				m_maxlen = 0;
				textVal[0] = '\0';
			}
			m_curPos = 0;
			ui->lineEdit_value->setSelection(m_curPos, 1);

		}

		this->show();
	}


	if (type != ms_getDevInfo) {
		ui->label_Infor->hide();
	}
	else {
		ui->label_Infor->show();
	}
	return 0;
}


QString DialogInput::GetValue() {

	this->hide();
	return ui->lineEdit_value->text();
}


void DialogInput::ChangeValue(int addormunus) {
	if (isVisible()) {
		unsigned char item = textVal[m_curPos];

		if (addormunus == 1) {	// increase
			if (item >= '9') {
				item = '0';
			}
			else if (item >= '0') {
				++item;
			}
			else {
				printf("ChangeValue:%d, %02X\n", addormunus, item);
				item = '9';
			}
		}
		else {	// decrease
			if (item <= '0') {
				item = '9';
			}
			else if (item <= '9') {
				--item;
			}
			else {
				printf("ChangeValue:%d, %02X\n", addormunus, item);
				item = '0';
			}
		}
		textVal[m_curPos] = item;
		ui->lineEdit_value->setText(textVal);
		ui->lineEdit_value->setSelection(m_curPos, 1);

		m_addormunus = addormunus;

		m_timeValChange->start(TIMER_TIMEOUT);
	}
}


char *DialogInput::NextNumber() {
	if (isVisible()) {
		if (m_curPos >= m_maxlen - 1) {
			//this->hide();
			textVal[m_maxlen] = '\0';
			return textVal;
		}
		++m_curPos;
		ui->lineEdit_value->setSelection(m_curPos, 1);
	}
	return NULL;
}

void DialogInput::onTimeout() {
	ChangeValue(m_addormunus);
}

void DialogInput::stopTimer() {
	if (m_timeValChange->isActive()) {
		m_timeValChange->stop();
	}
}
