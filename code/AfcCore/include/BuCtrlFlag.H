
//---------后台控制车载机功能函数 头文件------------------
//	2013-10-8  KSLI
//-------------------------------------------------

#ifdef Center_Ctrl_BU_ENABEL

#ifndef BUCtrl_Head_File_
#define BUCtrl_Head_File_

#define BUCTRL_FLAG_qPBOC	0x80

extern unsigned char gBuCtrlFlag;

extern void BUCtrl_Save_Flag(unsigned char cflag);
extern void BUCtrl_Get_Flag(void);
//mode = 0 显示大字体，否则显示小字体
extern void BUCtrl_Dis_onScreen(unsigned char x, unsigned char y, unsigned char mode);

#endif
#endif




