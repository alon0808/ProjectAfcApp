
// AfcappWinUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AfcappWinUI.h"
#include "AfcappWinUIDlg.h"
#include "afxdialogex.h"

#include "AFcappUIFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAfcappWinUIDlg �Ի���



CAfcappWinUIDlg::CAfcappWinUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AFCAPPWINUI_DIALOG, pParent)
	, m_valIp(_T(""))
	, m_valPort(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAfcappWinUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDBTNCANCEL, m_btnCancel);
	DDX_Control(pDX, IDBTNUP, m_btnUp);
	DDX_Control(pDX, IDBTNDOWN, m_btnDown);
	DDX_Control(pDX, IDBTNENTER, m_btnEnter);
	DDX_Control(pDX, IDBTNADD, m_btnAdd);
	DDX_Control(pDX, IDBTNMINUS, m_btnMinus);
	DDX_Control(pDX, IDC_RICHEDITUI, m_richeditScreen);
	DDX_Control(pDX, IDC_COMBO2, m_combIpAddress);
	DDX_Control(pDX, IDC_EDIT1, m_editPort);
	DDX_Control(pDX, IDC_COMBO1, m_combReaders);
	DDX_Control(pDX, IDBTN_SETIP_PORT, m_btnSetIpPort);
	DDX_CBString(pDX, IDC_COMBO2, m_valIp);
	DDX_Text(pDX, IDC_EDIT1, m_valPort);
}

BEGIN_MESSAGE_MAP(CAfcappWinUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_MSG, showInUI)
	ON_BN_CLICKED(IDBTNCANCEL, &CAfcappWinUIDlg::OnBnClickedBtn)
	ON_BN_CLICKED(IDBTNUP, &CAfcappWinUIDlg::OnBnClickedBtn)
	ON_BN_CLICKED(IDBTNDOWN, &CAfcappWinUIDlg::OnBnClickedBtn)
	ON_BN_CLICKED(IDBTNENTER, &CAfcappWinUIDlg::OnBnClickedBtn)
	ON_BN_CLICKED(IDBTNADD, &CAfcappWinUIDlg::OnBnClickedBtn)
	ON_BN_CLICKED(IDBTNMINUS, &CAfcappWinUIDlg::OnBnClickedBtn)
	ON_BN_CLICKED(IDBTN_SETIP_PORT, &CAfcappWinUIDlg::OnBnClickedBtn)
	ON_CBN_DROPDOWN(IDC_COMBO_READERS, &CAfcappWinUIDlg::OnCbnDropdownComboReaders)
	//	ON_BN_CLICKED(IDBTN_SETIP_PORT, &CAfcappWinUIDlg::OnBnClickedSetipPort)
	ON_CBN_SELCHANGE(IDC_COMBO_READERS, &CAfcappWinUIDlg::OnCbnSelchangeComboReaders)
END_MESSAGE_MAP()


// CAfcappWinUIDlg ��Ϣ�������

BOOL CAfcappWinUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	::AllocConsole();//�򿪿ؼ�̨��Դ  
	FILE *fp;
	freopen_s(&fp, "CONOUT$", "w+t", stdout);//����д����������VS2013�汾�Ĵ��룬��VS��Ϊ���ڵİ汾����VS2008�У��ɽ�freopen_s��Ϊfreopen������������Ϊ��Ӧ��ʽ����  

	InitUI(this);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAfcappWinUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAfcappWinUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAfcappWinUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAfcappWinUIDlg::OnBnClickedBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd *pCtrl = ((CWnd*)(this->GetFocus()));

	PressButton(pCtrl);
}

// drop the reader list
void CAfcappWinUIDlg::OnCbnDropdownComboReaders()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowUsbs();
}



void CAfcappWinUIDlg::OnCbnSelchangeComboReaders()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDefaultReader(&this->m_combReaders);
}

#pragma region user interface show function


static char s_showContent[SCREEN_COLUMN_ALL * SCREEN_ROW];

static char *getRowBegin(char *pContent, int row) {
	char *pRetValue = NULL;
	unsigned int i;

	if (row == 0) {	// ��һ�в��ü�1
		pRetValue = pContent;
	}
	else {
		for (i = 0; i < strlen(pContent); ++i) {
			if (pContent[i] == '\n') {
				--row;
				if (row <= 0) {	// ������
					pRetValue = pContent + i + 1;
					break;
				}
			}
		}
	}
	return pRetValue;
}

static void showInUIFatalError(char msg[]) {
	//showInUI(4, 1, msg, TP_MIDDLE);
	printf("showInUIFatalError:%s\n", msg);
}

/**
* @Description - show text in user interface
*
* @Param row(int) - row of user interface, start from 1
* @Param column(int) - column of user interface, start from 1
* @Param msg(char[]) - clear one line when NULL, set string when not NULL
* @Param textPosition(int) - position of font (emFontPostion)
* @Param fontSize(int) - size of font, default is 250..invalid now
* @Param fontColor(int) - color of font
*
* @Return
*/
afx_msg LRESULT CAfcappWinUIDlg::showInUI(WPARAM wparam, LPARAM lparam) {
	CAfcappWinUIDlg *pHndWin = this;
	CRichEditCtrl *pRichEditCtrl = &pHndWin->m_richeditScreen;
	//	CFont *pFont;
	//	LOGFONT lfNew;
	//	COLORREF rgb;
	int textLen = 0;
	char *pContent = s_showContent;
	char content[1000];
	//	CHARFORMAT cfm;
	//	int tmpI;// , rowStart, rowLen;
	int rowPix = 0;
	char *pTmpCh = NULL;
	stMsg *pStMsg = (stMsg *)wparam;
	int row = pStMsg->row;
	int column = pStMsg->column;
	char *msg = pStMsg->msg;
	int textPosition = pStMsg->textPosition;
	CStringW msgW = NULL;

	msgW = CStringW(msg);
	textLen = (int)(msg == NULL ? SCREEN_COLUMN : strlen(msg));
	if (row == -1 && column == -1) {	// printf special
		printf(msg);
		goto showInUI_OVER;
	}
	if (row == 0 && column == 0 && msg == NULL) {
		clearInUI();
		goto showInUI_OVER;
	}
	if (row <= 0 || row > SCREEN_ROW || column <= 0 || column > SCREEN_COLUMN) {
		sprintf_s(content, "illegal show row,column:%d, %d", row, column);
		showInUIFatalError(content);
		goto showInUI_OVER;
	}
	--row;
	--column;

	// �ı���λ��
	switch (textPosition)
	{
	case TP_INVALID:
		break;
	case TP_LEFT:
		column = 0;
		break;
	case TP_RIGHT:
		column = SCREEN_COLUMN - textLen;
		if (column < 0) {
			column = 0;
		}
		break;
	case TP_MIDDLE:
		column = (SCREEN_COLUMN - textLen) >> 1;
		if (column < 0) {
			column = 0;
		}
		break;
	default:
		break;
	}

	pTmpCh = getRowBegin(pContent, row);
	if (pTmpCh == NULL) {
		sprintf_s(content, "1illegal show row,column:%d, %d", row, column);
		showInUIFatalError(content);
		goto showInUI_OVER;
	}
	if (msg == NULL) {
		memset(pTmpCh, ' ', SCREEN_COLUMN);
	}
	else {
		// in case of 
		if (textLen + column > SCREEN_COLUMN) {
			column = SCREEN_COLUMN - textLen;
			if (column < 0) {
				column = 0;
				textLen = SCREEN_COLUMN;
			}
		}
		memcpy(pTmpCh + column, msg, textLen);
	}
	pRichEditCtrl->SetWindowTextA(pContent);

#if 0
	// ��������
	pRichEditCtrl->SetSel(rowStart, rowLen);

	pRichEditCtrl->GetSelectionCharFormat(cfm);
	// �����С
	//cfm.yHeight = fontSize;
	// ������ɫ
	switch (fontColor)
	{
	case FC_INVALID:
		rgb = RGB(0, 0, 0);
		break;
	case FC_RED:
		rgb = RGB(255, 0, 0);
		break;
	case FC_WHITE:
		rgb = RGB(0, 0, 0);
		break;
	default:
		rgb = RGB(0, 0, 0);
		break;
	}
	cfm.dwEffects = CFM_BOLD;
	cfm.crTextColor = rgb;
	cfm.dwMask = CFM_COLOR | CFM_SIZE | CFM_FACE;
	pRichEditCtrl->SetSelectionCharFormat(cfm);

	pRichEditCtrl->GetWindowTextA(content, 1000);

	pRichEditCtrl = pRichEditCtrl;
	//pRichEditCtrl->Invalidate(FALSE);
#endif
showInUI_OVER:
	if (msg != NULL) {
		free(msg);
	}
	memset(pStMsg, 0x00, sizeof(stMsg));
	return 0;
}


void CAfcappWinUIDlg::clearInUI(int line) {
	CAfcappWinUIDlg *pHndWin = this;
	CRichEditCtrl *pRichEditCtrl = &pHndWin->m_richeditScreen;
	char content[100];// = "";
					  //	int index, length;
	int i, tmpI;
	char *pContent = s_showContent;
	char *pTmpCh = NULL;
	CFont font1;

	//pRichEditCtrl->gette(&content);

	if (line == 0) {	// clear all
		pRichEditCtrl->SetBackgroundColor(TRUE, RGB(255, 255, 255));
		// clear buffer
		tmpI = SCREEN_ROW*SCREEN_COLUMN_ALL;
		memset(pContent, ' ', tmpI);
		memset(pContent + tmpI - 2, '\0', 2);
		font1.CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, 0, 1,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			_T("����"));
		pRichEditCtrl->SetFont(&font1);
		// set line split character
		for (i = 0; i < SCREEN_ROW - 1; ++i) {
			tmpI = (i + 1) * SCREEN_COLUMN_ALL - 2;
			pContent[tmpI] = '\r';
			pContent[tmpI + 1] = '\n';
		}
		pRichEditCtrl->SetWindowTextA(pContent);


		tmpI = pRichEditCtrl->GetWindowTextA(pContent, 1000);
	}
	else {	// clear one line
		if (line > SCREEN_ROW || line <= 0) {
			sprintf_s(content, "illegal show line:%d", line);
			showInUIFatalError(content);
			return;
		}
		--line;
		pTmpCh = getRowBegin(pContent, line);
		if (pTmpCh == NULL) {
			sprintf_s(content, "1illegal show row,column:%d, %d", line, 0);
			showInUIFatalError(content);
			return;
		}

		memset(pTmpCh, ' ', SCREEN_COLUMN);
		pRichEditCtrl->SetWindowTextA(pContent);
	}
}



#pragma endregion user interface show function
