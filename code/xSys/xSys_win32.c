#include "Macro_Proj.h"

#if WHICH_PLATFORM == _WIN32_PLATFORM_V
#include "xSys.h"
#include <windows.h>
#include <Tlhelp32.h>


TLPVOID xCreateThread(TUINT32 *pThreadId, ThreadStartAddr startAddr, TLPVOID params) {
	//LPTHREAD_START_ROUTINE addr = startAddr;
	TLPVOID pHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startAddr, params, 0, pThreadId);

	xUsleep(3);

	return pHandle;
}

void xUsleep(TUINT32 milliseconds) {
	Sleep(milliseconds);
}

TUINT8 xIsAppRunning(const char *ExeName) {

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

//////////////////////////////////////////////////////////
// DLL library
TLPVOID xSysLoadLib(const TINT8* pLibName, TINT32 OptionParam)
{
	UNUSED_VAR(OptionParam);

	return (TLPVOID)(LoadLibrary(pLibName));
}


TINT32 xSysGetLibName(const TLPVOID handle, TINT8* pLibPath, TINT32 pathMaxLen)
{
	TINT32 retCode = (TINT32)GetModuleFileName(handle, pLibPath, pathMaxLen);
	//GetFileNameByHandle(handle, pLibPath, pathMaxLen);

	return retCode;
}


void *xSysGetProcEntryAddr(TLPVOID libHandle, const TINT8 *pEntryName)
{
	return (void *)(GetProcAddress(libHandle, pEntryName));
}

//////////////////////////////////////////////////////////////////////////
// message queue


TINT32 inline xSysPostThreadMsg(int threadId, TUINT32 msgId, stDataBuffer *pStData)
{
	//UINT8 count = 3;
	//TUINT32 retCode=0;
	while (PostThreadMessage(threadId, msgId + WM_USER, (WPARAM)pStData->pBuffer, (LPARAM)pStData->blen) == FALSE)
	{
		return Ret_Error;
	}
	return Ret_OK;
}

TINT32 xSysRecvThreadMsg(int hQueue, TUINT32 msgId, stDataBuffer *pRecvInfo)
{
	MSG l_msg;
	TINT32 ret = 0;


	//limit ????
	memset(pRecvInfo, 0x00, sizeof(stDataBuffer));

	ret = GetMessage(&l_msg, NULL, WM_USER + msgId, WM_USER + msgId);
	if (ret > 0)
	{
		UNUSED_VAR(hQueue);
		//*pMsgId = l_msg.message;

		pRecvInfo->pBuffer = (TUINT8 *)l_msg.wParam;
		pRecvInfo->blen = (TINT32)l_msg.lParam;
	}
	else
	{//0,-1
		return Ret_Error;
	}

	return Ret_OK;
}


void xSyscreateQueueIfNotExisted()
{

}

void xSysdestroyQueueIfNotExisted()
{

}

#endif
