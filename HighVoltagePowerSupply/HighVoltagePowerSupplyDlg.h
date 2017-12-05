
// HighVoltagePowerSupplyDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CHighVoltagePowerSupplyDlg dialog
class CHighVoltagePowerSupplyDlg : public CDialogEx
{
// Construction
public:
	CHighVoltagePowerSupplyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIGHVOLTAGEPOWERSUPPLY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CButton m_OkButton;
};
