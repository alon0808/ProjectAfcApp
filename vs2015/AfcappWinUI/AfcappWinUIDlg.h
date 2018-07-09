
// AfcappWinUIDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAfcappWinUIDlg 对话框
class CAfcappWinUIDlg : public CDialogEx
{
	// 构造
public:
	CAfcappWinUIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AFCAPPWINUI_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 取消按钮
	CButton m_btnCancel;
	// 向上按钮
	CButton m_btnUp;
	// 向下按钮
	CButton m_btnDown;
	// 回车按钮
	CButton m_btnEnter;
	// 加按钮
	CButton m_btnAdd;
	// 减按钮
	CButton m_btnMinus;
	// 屏幕对象
	CRichEditCtrl m_richeditScreen;
	afx_msg void OnBnClickedBtn();
	// IP列表控件
	CComboBox m_combIpAddress;
	CEdit m_editPort;
	// 读卡器列表
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
