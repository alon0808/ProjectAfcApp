/**
 * @Description LanTaiYuan lfsm 
 * @Author ZhangLong
 * @Modify ZhangLong
 * @Time	2018.5.10 16
 */
#include "Macro_Proj.h"
#include "xSysApi_LanTai.h"
#include "inputmisc/IcCardCtrlApi.h"
#include "inputmisc/GPIOCtrl.h"

#include "ProcCharacter.h"


/*CIcCardCtrlApi s_iccCardApi;
struct ev_loop *loop;

CGPIOCtrl LIGHT_CGPIOCtrl;

int xSysLty_InitIcc(void)
{
	s_iccCardApi.Init(loop);

	return Ret_OK;
}

int xSysLty_CmdFindCardSe(char *buf,int *len, char *status)
{
	int ret;

	ret = s_iccCardApi.CmdFindCardSet((unsigned char *)buf, (unsigned int *)len, (unsigned char *)status);

	return ret;
}


int xSysLty_CmdReadSet(int block,char *buf,int *len, char *status)
{
	int ret;

	ret = s_iccCardApi.CmdReadSet((unsigned int)block,(unsigned char *)buf, (unsigned int *)len, (unsigned char *)status);

	return ret;
}


int xSysLty_CmdWriteSet(int block,char *buf,int len, char *status)
{
	int ret;

	ret = s_iccCardApi.CmdWriteSet((unsigned int)block,(unsigned char *)buf, (unsigned int)len, (unsigned char *)status);

	return ret;
}

int xSysLty_GPIOCtrlInitQR(void)
{
	LIGHT_CGPIOCtrl.Init_QR(loop);

	return Ret_OK;
}

int xSysLty_GPIOCtrlInitScreen(void)
{
	LIGHT_CGPIOCtrl.Init_SCREEN(loop);

	return Ret_OK;
}

int xSysLty_GPIOCtrlOpen(void)
{
	LIGHT_CGPIOCtrl.Open();

	return Ret_OK;
}

int xSysLty_SetGPIODir(int pin, int enDir)
{
	LIGHT_CGPIOCtrl.SetGPIODir((unsigned int)pin, enDir);

	return Ret_OK;
}

int xSysLty_GPIOSetVal(int pin, int enDir)
{
	LIGHT_CGPIOCtrl.GPIOSetVal((unsigned int)pin, enDir);

	return Ret_OK;
}*/


