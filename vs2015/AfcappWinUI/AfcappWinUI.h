
// AfcappWinUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAfcappWinUIApp: 
// �йش����ʵ�֣������ AfcappWinUI.cpp
//

class CAfcappWinUIApp : public CWinApp
{
public:
	CAfcappWinUIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAfcappWinUIApp theApp;