#include "Macro_Proj.h"

#if WHICH_PLATFORM == _WIN32_PLATFORM_V
#include "xCom.h"
#include <windows.h>


TLPVOID xOpenCom(char comN[], int baund) {
	TLPVOID hCom = CreateFile(comN,//COM1��
		GENERIC_READ | GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if (hCom == (HANDLE)-1)
	{
		PRINT_ERROR("Fail to Open %s!\n", comN);
		return hCom;
	}
	else
	{
		PRINT_DEBUG("Success to Open %s��\n", comN);
	}
	SetupComm(hCom, 1024, 1024); //���뻺����������������Ĵ�С����1024
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = 1000;
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.ReadTotalTimeoutConstant = 5000;
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ
	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = baund; //������Ϊ9600
	dcb.ByteSize = 8; //ÿ���ֽ���8λ
	dcb.Parity = NOPARITY; //����żУ��λ
	dcb.StopBits = ONE5STOPBITS; //����ֹͣλ
	SetCommState(hCom, &dcb);

	return hCom;
}


#endif
