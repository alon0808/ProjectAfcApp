
// AfcappWinUIDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAfcappWinUIDlg �Ի���
class CAfcappWinUIDlg : public CDialogEx
{
	// ����
public:
	CAfcappWinUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AFCAPPWINUI_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ȡ����ť
	CButton m_btnCancel;
	// ���ϰ�ť
	CButton m_btnUp;
	// ���°�ť
	CButton m_btnDown;
	// �س���ť
	CButton m_btnEnter;
	// �Ӱ�ť
	CButton m_btnAdd;
	// ����ť
	CButton m_btnMinus;
	// ��Ļ����
	CRichEditCtrl m_richeditScreen;
	afx_msg void OnBnClickedBtn();
	// IP�б�ؼ�
	CComboBox m_combIpAddress;
	CEdit m_editPort;
	// �������б�
	CComboBox m_combReaders;
	afx_msg void OnCbnDropdownComboReaders();
	//	afx_msg void OnBnClickedSetipPort();
		// set ip and port button
	CButton m_btnSetIpPort;
	// value of ip
	CString m_valIp;
	// value of port
	int m_valPort;
	afx_msg void OnCbnSelchangeComboReaders();
	afx_msg LRESULT showInUI(WPARAM wParam, LPARAM lParam);

	void clearInUI(int row = 0);
};
