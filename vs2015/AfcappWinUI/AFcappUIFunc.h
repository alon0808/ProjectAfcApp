
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

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