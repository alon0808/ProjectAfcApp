
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once


#include "AfcappWinUIDlg.h"
#include "uiAccess.h"

#define WM_USER_MSG WM_USER + 1001  

typedef struct {
	int row;
	int column;
	char *msg;	// for printf
	int textPosition;
}stMsg;

extern void InitUI(CAfcappWinUIDlg *pHndWin);
extern void PressButton(CWnd *pBtn);
extern void ShowUsbs();
extern void SetDefaultReader(CComboBox *selReader);
//LRESULT (showInUI)(WPARAM wparam, LPARAM lparam);