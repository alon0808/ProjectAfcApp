#include "Macro_Proj.h"
#include "xAppConfig.h"
#include "xStorage.h"
#include "UtilityProc.h"
#include "xSys.h"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct
{
	TUINT16 diDisprice[2];        //投币显示价格
	TUINT16 diPrice[2][2];            //分段票价
}stDivInfo;

#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#include <windows.h>
#include <io.h>
static CRITICAL_SECTION s_SemaHandleConfig;
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#include <pthread.h>
static pthread_mutex_t s_SemaHandleTempRam = NULL;
#endif

#define FILENAME_CONFIG	"flashparam"
//#define FILENAME_PERME_CONFIG	"flashparamPermenant"


static int s_fdParam = -1;
static stDivInfo DivInfo;

int initAfcParam(char curPath[]) {
	int len = 0;
	int bufRef = 0;
	char *pBuf = NULL;
	int retcode = 0;
	int tmpI;
	char *pTmpCh = NULL, *pTmpCh1;
	char bakCh = 0;	// backup

	len = strlen(curPath);
	bufRef = xStor_MallocTempRam(len + 100);
	pBuf = xStor_GetTempRamPoint(bufRef);
	pTmpCh = pBuf + 30;
	if (s_fdParam >= 0) {
		close(s_fdParam);
	}
	// copy the path, use its up one level, then with '\0',
	memcpyE(pTmpCh, curPath, len);
	tmpI = strlen("/../configs/");
	memcpyE(pTmpCh + len, "/../configs/", tmpI);
	len += tmpI;
	tmpI = strlen(FILENAME_CONFIG);
	memcpyE(pTmpCh + len, FILENAME_CONFIG, tmpI);
	pTmpCh[len + tmpI] = '\0';
#if WHICH_PLATFORM == _WIN32_PLATFORM_V
	replaceE(pTmpCh, '/', '\\', len + tmpI);
#else
	replaceE(pTmpCh, '\\', '/', len + tmpI);
#endif

	s_fdParam = open(pTmpCh, O_RDWR);
	if (s_fdParam == -1)
	{	// 4M
		bakCh = pTmpCh[len];
		pTmpCh[len] = '\0';	// only left the directory, not filepath
		// 'cd ' font of pTmpCh
		tmpI = strlen("cd ");
		memcpyE(pTmpCh - tmpI, "cd ", tmpI);
		retcode = system(pTmpCh - tmpI);
		if (retcode != 0) {
			// 'mkdir ' font of pTmpCh
			tmpI = strlen("mkdir ");
			memcpyE(pTmpCh - tmpI, "mkdir ", tmpI);
			retcode = system(pTmpCh - tmpI);	
		}
		// create a file with 4M
		
#if WHICH_PLATFORM == _WIN32_PLATFORM_V
		tmpI = strlen("fsutil file createnew ");
		pTmpCh1 = pTmpCh - tmpI;
		memcpyE(pTmpCh1, "fsutil file createnew ", tmpI);
		tmpI = strlen(FILENAME_CONFIG" 4194304");
		memcpyE(pTmpCh + len, FILENAME_CONFIG" 4194304", tmpI);
#else
		tmpI = strlen("cd ");
		pTmpCh1 = pTmpCh - tmpI;
		memcpyE(pTmpCh1, "cd ", tmpI);
		tmpI = strlen(";dd if=/dev/zero of="FILENAME_CONFIG" bs=4096k count=1");
		memcpyE(pTmpCh + len, ";dd if=/dev/zero of="FILENAME_CONFIG" bs=4096k count=1", tmpI);
#endif
		len += tmpI;

		system(pTmpCh1);	// run only one command at windows

		pTmpCh[len] = bakCh;
		s_fdParam = open(pTmpCh, O_RDWR);
	}
	xStor_FreeTempRam(bufRef);

	xSysCreateMutext(s_SemaHandleConfig);

	return Ret_OK;
}

int installAfcParam(char curPath[]) {
	int i = 0;
	int retcode = Ret_OK;
	TUINT8 buffer[512] = { 0 };

	initAfcParam(curPath);

	//bzero(&M1Deal, sizeof(M1Deal));
	//bzero(&LNT_Para, sizeof(LNT_Para));
	memset(&DivInfo, 0x00, sizeof(DivInfo));

	ReadParam(BIT_IS_FIRST_FLAG, Param.IsFirstFlag, 4);//读初始化标志

	if (memcmp(Param.IsFirstFlag, "ZZ02", 4) != 0)//初始化标志判断
	{
		LCD_Clr();
		ShowLine(0x00, 0x00, 0, "提示:               ");
		ShowLine(0x01, 0x00, 0, "第一次启动程序      ");
		ShowLine(0x02, 0x00, 0, "正在初始化          ");

		ClearConfigData();
		WriteParam(BIT_IS_FIRST_FLAG, "ZZ02", 4);//置初始化标志
	}

	ReadParam(BIT_POINTER, &Param.pointer, BIT_POINTER_LEN);//取记录指针
	if (!verifyPointer(&Param.pointer))
		return false;

	ReadParam(BIT_FLAG, &Param.flag, BIT_FLAG_LEN);//初始化车载机状态
	Param.flag.set_line_status = 0;   //每次开机都需要刷设置卡才能刷线路卡
#ifdef CUS_DEMO
	Param.flag.stop_flag = 0;
#else
	Param.flag.stop_flag = 1;
#endif

	ReadParam(BIT_KONGTIAOFALG, &Param.kongTiaoFlag, BIT_KONGTIAOFALG_LEN);//空调flag
	ReadParam(BIT_POSSEQ, &Param.PosSeq, BIT_POSSEQ_LEN);//取系统流水号
	ReadParam(BIT_DEVICENO, Param.DeviceNo, BIT_DEVICENO_LEN);//取终端号
	ReadParam(BIT_DRIVERID, Param.DriverID, BIT_DRIVERID_LEN);//取司机卡号

	ReadParam(BIT_GPRS_IC_IP, (U8*)&Param.ServerGPRSIP, BIT_GPRS_IC_IP_LEN);
	ReadParam(BIT_GPRS_IC_PORT, (U8*)&Param.ServerGPRSPort, BIT_GPRS_IC_PORT_LEN);
	ReadParam(BIT_WIFI_IC_IP, (U8*)&Param.ServerWIFIIP, BIT_WIFI_IC_IP_LEN);
	ReadParam(BIT_WIFI_IC_PORT, (U8*)&Param.ServerWIFIPort, BIT_WIFI_IC_PORT_LEN);

	WordData tempct;
	tempct.Data[0] = tempct.Data[1] = tempct.Data[2] = tempct.Data[3] = 255;
	if (Param.ServerGPRSIP > tempct.IntVal)
		Param.ServerGPRSIP = 0;

	if (Param.ServerWIFIIP > tempct.IntVal)
		Param.ServerWIFIIP = 0;

	if (Param.ServerGPRSPort >= 65535)
		Param.ServerGPRSPort = 0;

	if (Param.ServerWIFIPort >= 65535)
		Param.ServerWIFIPort = 0;

	//Set volume
	U8 vol = 100;
	ReadParam(BIT_VOLUME, &vol, BIT_VOLUME_LEN);
	if (vol > 100) vol = 100;
	Sound_SetVolume(vol);

	ReadParam(BIT_CTLESS_LIMIT, &Param.CTLessLimit, 4);
	ReadParam(BIT_LOWEST_LIMIT, &Param.LowestLimit, 4);

	if (Param.CTLessLimit >= 10000 || Param.CTLessLimit == 0)
	{
		Param.CTLessLimit = 1000;
		WriteParam(BIT_CTLESS_LIMIT, &Param.CTLessLimit, 4);
	}

	if (Param.LowestLimit >= 10000 || Param.LowestLimit == 0)
	{
		Param.LowestLimit = 500;
		WriteParam(BIT_LOWEST_LIMIT, &Param.LowestLimit, 4);
	}

	//ChangePriceEnable = 0;
	//ReadParam(BIT_CHANGEPRICE, &ChangePriceEnable, BIT_CHANGEPRICE_LEN);
	ChangePrice = 0;

	memset(buffer, 0, sizeof(buffer));
	ReadParam(BIT_DIVINFOPRICE, buffer, BIT_DIVINFOPRICE_LEN);

	for (i = 0; i < 2; i++)
	{
		DivInfo.Disprice[i] = buffer[i * 2 + 1] * 256 + buffer[i * 2];
	}
	for (i = 0; i < 2; i++)
	{
		DivInfo.Price[i][0] = buffer[i * 4 + 1] * 256 + buffer[i * 4];
		DivInfo.Price[i][1] = buffer[i * 4 + 3] * 256 + buffer[i * 4 + 2];
	}

	//ReadParam(BIT_SENDSN, &Sn, BIT_SENDSN_LEN);

	ReadParam(BIT_CONTROL, (U8*)&LineControl, BIT_CONTROL_LEN);

	// 为了解决设备使用旧的这两个配置
	LineControl.CardControl[CARD_STUDENT].CanTapNoSplit = 0x00;
	LineControl.CardControl[CARD_STUDENT].CanJudgeDate = 0x01;
	/*
	memset(buffer, 0, sizeof(buffer));
	ReadParam(BIT_HISDATA_GATHER, buffer, BIT_HISDATA_GATHER_LEN);

	for(i = 0; i < 10; i++)
	{
	memcpy(hisfile[i].hisName, buffer + 21 * i, 7);
	memcpy(hisfile[i].fileSno, buffer + 21 * i + 7, 2);
	memcpy((U8*)&hisfile[i].strAddr, buffer + 21 * i + 9, 4);
	memcpy((U8*)&hisfile[i].endAddr, buffer + 21 * i + 13, 4);
	memcpy((U8*)&hisfile[i].fileLength, buffer + 21 * i + 17, 4);
	}
	*/
	CFG_LoadFile(CFG_BLK, false);
	CFG_LoadFile(CFG_PKI, false);
	CFG_LoadFile(CFG_WHT, false);
	CFG_LoadFile(CFG_WPB, false);
	CFG_LoadFile(CFG_PRI, false);

	return true;
}

#if SPLIT_BAR	// other buffer
#endif

