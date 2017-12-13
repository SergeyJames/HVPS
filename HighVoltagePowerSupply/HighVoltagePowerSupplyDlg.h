
// HighVoltagePowerSupplyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <string>
#include <vector>
#include <minwindef.h>
#include <winperf.h>

const unsigned long Min = 0;
const unsigned long Max = 30000;

/// this defination for GetComPortList() function
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383


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

public: // Voltage To Set Methodes
	afx_msg void OnDeltaposSpinVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult);

private: // Voltage To Set Variables
	CButton m_LogToFileButton;
	CButton m_EnableButton;

	CEdit m_VoltageToSetSpin;
	CEdit m_VoltageToSetKeyboard;

	CSpinButtonCtrl m_VoltageToSetSpinCtrl;
	CSliderCtrl m_SliderVoltageToSet;

	CComboBox m_ComPortCmbBox;


private: // Voltage To Set Variables
	unsigned long m_ulSliderVoltageToSetPos; // The slider position
	std::wstring m_strSliderVoltageToSetPos;	// The slider position (std::string)
	void GetComPortList();
	std::vector<std::wstring> m_ComPortVec;
public:
	afx_msg void OnEnChangeEditVoltageToSetKeybord();
private:
	CEdit m_InternalTempEdit;
	CEdit m_PowerTempEdit;
	CEdit m_HumidityEdit;
};
