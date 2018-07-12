// WatchDog.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Macro_Proj.h"
#include "TransmitData.h"
#include "xSys.h"
#include "xStorage.h"

#include <windows.h>
#include <string.h>
#include <Tlhelp32.h>

#pragma comment(lib,"E:\\三联众瑞项目文件夹\\afcapp\\ProjectAfcApp\\vs2015\\Debug\\UtilPro.lib")
#pragma comment(lib,"E:\\三联众瑞项目文件夹\\afcapp\\ProjectAfcApp\\vs2015\\Debug\\SystemRelation.lib")

#define POS_BUF_TYPE	0
#define POS_BUF_PARAM	1

#define PeripheralRAMSize	8

#define SharedMemoryName_Watchdog	"sharedMemoryWatchDog"
#define AppName	"afc_app.x.exe"

TStorageInit s_storInit;
static unsigned char s_appRam[1000];


static TUINT8 stopApp(const char *ExeName) {

	TUINT8   ret = 0;
	LPCSTR   File;
	HANDLE   hProcessSnap;
	PROCESSENTRY32   pe32;

	if (!ExeName || !ExeName[0])
		return   ret;
	File = strrchr(ExeName, '\\');
	if (File != 0) {
		ExeName = File + 1;
	}
	hProcessSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS, 0);
	if ((int)hProcessSnap == -1)
		return   ret;

	memset(&pe32, 0, sizeof(pe32));
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do {
			File = strrchr(pe32.szExeFile, '\\');
			File = File ? File + 1 : pe32.szExeFile;
			if (memcmp(File, ExeName, strlen(ExeName)) == 0) {
				PRINT_DEBUG((char *)File);
				++ret;
			}
		} while (Process32Next(hProcessSnap, &pe32) && ret < 2);
	}
	CloseHandle(hProcessSnap);
	return ret >= 2;
}

int main(int argc, char* argv[])
{
	int hSharedMoery = REF_INVALID;
	char pDataBuf[PeripheralRAMSize];
	int tmpI = 0;
	int counter = 0;
	int retCode = Ret_OK;
	stDataBuffer stData;

	TStorageInit *pStorInit = &s_storInit;

	tmpI = strlen(argv[0]);
	for (; tmpI >= 0; --tmpI)
	{
		if (argv[0][tmpI] == '\\') {
			argv[0][tmpI + 1] = '\0';
			break;
		}
	}

	pStorInit->m_pAppRamBegin = s_appRam;
	pStorInit->m_appRamLen = sizeof(s_appRam);
	retCode = xStor_InstallStorage(pStorInit);
	if (retCode != Ret_OK)
	{
		PRINT_ERROR("fail to install storage, application exit\n");
		goto main_over;
	}

	printf("watch dog is start\n");
	hSharedMoery = initSharedMemory(SharedMemoryName_Watchdog, 0x00, PeripheralRAMSize);
	if (hSharedMoery < 0) {
		retCode = Ret_Error;
		goto main_over;
	}

	while (hSharedMoery >= 0)
	{

		xUsleep(1000);
		++counter;
		//pDataBuf = s_pDataBuffer;
		stData.pBuffer = (unsigned char *)pDataBuf;
		stData.blen = PeripheralRAMSize;
		if (readData(hSharedMoery, &stData) == Ret_OK) {

			tmpI = GET_INT32(pDataBuf);
			if (tmpI != 0) {
				counter = 0;
			}
		}
		if (counter >= 1000) {
			counter = 0;
			printf("watch dog find " AppName" time out, stop it\n");
			system("taskkill /im " AppName);
			printf("watch dog find " AppName" time out, start it\n");
			sprintf(pDataBuf, "start /b %s%s", argv[0], AppName);
			system(pDataBuf);
		}
		else {
			printf("watch dog: %d\n", counter);
		}
	}
main_over:
	getchar();
	return retCode;
}

