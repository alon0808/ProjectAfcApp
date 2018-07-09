#include "Macro_Proj.h"

#if WHICH_PLATFORM == _WIN32_PLATFORM_V
#include "xCom.h"
#include <windows.h>


TLPVOID xOpenCom(char comN[], int baund) {
	TLPVOID hCom = CreateFile(comN,//COM1口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if (hCom == (HANDLE)-1)
	{
		PRINT_ERROR("Fail to Open %s!\n", comN);
		return hCom;
	}
	else
	{
		PRINT_DEBUG("Success to Open %s！\n", comN);
	}
	SetupComm(hCom, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = 1000;
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.ReadTotalTimeoutConstant = 5000;
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时
	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = baund; //波特率为9600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONE5STOPBITS; //两个停止位
	SetCommState(hCom, &dcb);

	return hCom;
}


#endif
