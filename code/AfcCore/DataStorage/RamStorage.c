#include "Macro_Proj.h"
#include "RamStorage.h"

#include <pthread.h>
#include <unistd.h>
//#include <stdio.h>
#include "RecordFile.h"

unsigned char DriveCardNo[4];
unsigned int g24GDisFlash = 0;//24G卡界面刷新
unsigned int endTime = 0;
unsigned int gmissflag = 0;//24G卡界面刷新
unsigned int GPRSLinkProcess = 0;
unsigned char nnAudioflag = 0;
unsigned char gucRestorFlag = 0;


void COM_it_enable(unsigned char com) {
	return 0;
}

void delayxs(unsigned int ms) {
	usleep(ms);
	return 0;
}

void flashread(unsigned int addr, unsigned char *rec_data, unsigned int length) {
	FR_flashread(addr, rec_data, length);
}

unsigned char getkey(unsigned char beepE) {
	return 0;
}

void flasherase(unsigned char mode, unsigned int addr) {

}

void addStatMoney(unsigned char mode, unsigned int moneyv, unsigned char type) {

}

