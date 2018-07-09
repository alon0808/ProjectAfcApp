#include "stdafx.h"
#include "Macro_Proj.h"
#include "AFcappUIFunc.h"
#include "Application.h"
#include "xCardOpera.h"
//#include "Macro_Proj.h"
#include "UtilityProc.h"

#include <string.h>
#include <Windows.h> 

#pragma comment(lib,"SystemRelation.lib")
#pragma comment(lib,"UtilPro.lib")

extern void ShowFatalMessage(CString msg);

static CAfcappWinUIDlg *s_pHndWin = NULL;

#define MSG_ST_COUNT	20

static stMsg s_msgSt[MSG_ST_COUNT];
static int s_curPos = 0;
static CRITICAL_SECTION s_SemaHandleShowUI;

#pragma region other functions

static CString getCurPath(void) {
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);

	return path;
}

void ShowUsbs() {
	char num = 0;
	unsigned short usbs[255];
	int i;
	char tmpChs[100];
	CComboBox *combBox = &s_pHndWin->m_combReaders;
	int retcode = Ret_OK;

	if (xSCardInit() != Ret_OK) {
		ShowFatalMessage("读卡器初始化失败");
	}

	retcode = xSCardGetReaderList(&num, (int *)usbs);
	if (retcode == Ret_OK) {
		printf("successful to list readers\n");
	}
	else {
		printf("fail to list readers\n");
	}
	// clear all reader list
	combBox->ResetContent();
	for (i = 0; i < num; ++i)
	{
		sprintf_s(tmpChs, "%4X", usbs[i]);
		combBox->AddString(tmpChs);
	}
	if (num > 0) {
		combBox->SetCurSel(0);
		SetDefaultReader(combBox);
	}
}

#pragma endregion other functions

#pragma region user interface show function


static void ShowFatalMessage(CString msg) {
	CString cation = "fatal error";
	MessageBox(s_pHndWin->m_hWnd, msg, cation, MB_OK);
}

static void showInUI_thread(int row, int column, char msg[], int textPosition = TP_INVALID) {
	stMsg *pMsgSt;
	int len;

	EnterCriticalSection(&s_SemaHandleShowUI);
	pMsgSt = s_msgSt + s_curPos;
	++s_curPos;
	if (s_curPos == MSG_ST_COUNT) {
		s_curPos = 0;
	}
	if (pMsgSt->msg != NULL) {
		ShowFatalMessage("the message queue is full");
	}
	len = (msg == NULL) ? 0 : strlen(msg);

	pMsgSt->row = row;
	pMsgSt->column = column;
	/*if (len > SCREEN_COLUMN) {
		len = SCREEN_COLUMN;
	}*/
	if (len > 0) {
		pMsgSt->msg = (char *)malloc(strlen(msg) + 1);
		strcpy(pMsgSt->msg, msg);
	}
	else {
		pMsgSt->msg = NULL;
	}
	//pMsgSt->msg[len] = '\0';
	pMsgSt->textPosition = textPosition;
	::PostMessage(s_pHndWin->m_hWnd, WM_USER_MSG, WPARAM(pMsgSt), LPARAM(0));
	//Sleep(200);
	LeaveCriticalSection(&s_SemaHandleShowUI);
}


#pragma endregion user interface show function

#pragma region event process functions

/**
* @Description - init the user interface
*
* @Param pHndWin(CAfcappWinUIDlg *) - handle of main window
*
* @Return
*/
void InitUI(CAfcappWinUIDlg *pHndWin) {

	CRichEditCtrl *pRichEditCtrl;


	if (pHndWin == NULL) {

		return;
	}
	// clear msg ui data units
	memset(s_msgSt, 0x00, sizeof(stMsg)*MSG_ST_COUNT);
	s_pHndWin = pHndWin;
	pRichEditCtrl = &pHndWin->m_richeditScreen;

	InitializeCriticalSection(&s_SemaHandleShowUI);
	// start application firstly
	CString curPath = getCurPath();
	InitApp(curPath.GetBuffer(), showInUI_thread);

	pRichEditCtrl->SetOptions(ECOOP_OR, ECO_SAVESEL);
	pRichEditCtrl->ShowScrollBar(SB_VERT, FALSE);

	//pHndWin->m_combIpAddress.SetDroppedWidth(1000);

	pHndWin->clearInUI();
#if SWITCH_DEBUG_UI
	//showInUI(1, 5, "请刷卡吧M1");
#endif
	//pHndWin->SetFocus();
#if WHICH_DEV == _ACR322_DEV_V

#else
	// set the postion of buttons
	RECT rect, rect1;
	memset(&rect, 0x00, sizeof(RECT));
	memset(&rect1, 0x00, sizeof(RECT));
	pHndWin->m_btnDown.GetWindowRect(&rect);
	pRichEditCtrl->GetWindowRect(&rect1);;
	rect.top -= rect.bottom - (rect1.bottom + 7);
	//pHndWin->m_btnDown.SetWindowPos(pHndWin, 500, 500, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	pHndWin->m_btnEnter.SetWindowPos(pHndWin, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	pHndWin->m_btnCancel.GetWindowRect(&rect);
	rect.top -= rect.bottom - (rect1.bottom + 7);
	//pHndWin->m_btnDown.SetWindowPos(pHndWin, 500, 500, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	pHndWin->m_btnCancel.SetWindowPos(pHndWin, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	pHndWin->m_btnUp.GetWindowRect(&rect);
	rect.top -= rect.bottom - (rect1.bottom + 7);
	//pHndWin->m_btnDown.SetWindowPos(pHndWin, 500, 500, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	pHndWin->m_btnUp.SetWindowPos(pHndWin, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	//pHndWin->m_btnUp.GetWindowRect(&rect);
		//pHndWin->m_btnDown.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOSIZE);
		//pHndWin->m_btnUp.ShowWindow(SW_HIDE);
	pHndWin->m_btnDown.ShowWindow(SW_HIDE);
	pHndWin->m_btnAdd.ShowWindow(SW_HIDE);
	pHndWin->m_btnMinus.ShowWindow(SW_HIDE);
#endif
	// reader list
	ShowUsbs();
	// ip and port
	pHndWin->m_valIp = "58.20.213.70";
	pHndWin->m_combIpAddress.AddString(pHndWin->m_valIp);
	pHndWin->m_valPort = 8830;
	pHndWin->UpdateData(FALSE);

	StartApp();
}
/**
 * @Description button event
 *
 * @Param btnId(CWnd *) - handle of button
 *
 * @Return void
 */
void PressButton(CWnd *pBtn) {
	CAfcappWinUIDlg *pHndWin = s_pHndWin;
	HWND hwnd = pBtn->m_hWnd;

	// which button 
	if (hwnd == pHndWin->m_btnCancel) {	// cancel button
		ShowFatalMessage("djfsldjfsak");
	}
	else if (hwnd == pHndWin->m_btnUp) {	// up button
		showInUI_thread(0, 0, "up向上down向下一");
	}
	else if (hwnd == pHndWin->m_btnDown) {	// up button
		pHndWin->clearInUI(1);
	}
	else if (hwnd == pHndWin->m_btnEnter) {	// up button
		pHndWin->clearInUI();
	}
	else if (hwnd == pHndWin->m_btnAdd) {	// up button
		pHndWin->clearInUI(2);
	}
	else if (hwnd == pHndWin->m_btnMinus) {	// up button
		pHndWin->clearInUI(4);
	}
	else if (hwnd == pHndWin->m_btnSetIpPort) {	// set ip and port
		pHndWin->UpdateData();
		hwnd = hwnd;
		//clearInUI(4);
	}
	else {

	}

}

void SetDefaultReader(CComboBox *comb) {
	unsigned char defaultReader[2];
	int retcode = Ret_OK;
	CString tmpStr;

	comb->GetLBText(comb->GetCurSel(), tmpStr);

	retcode = CharsToBytes((unsigned char *)tmpStr.GetBuffer(), tmpStr.GetLength(), defaultReader, 2);

	// little endian
	//RevertTurn(defaultReader, 2);

	retcode = xSCardSetReaderName((char *)defaultReader);
	if (retcode == Ret_OK) {
		printf("successful to set reader name:%s\n", tmpStr.GetBuffer());
	}
	else {
		printf("fail to set reader name:%s\n", tmpStr.GetBuffer());
	}
}

#pragma endregion event process functions

