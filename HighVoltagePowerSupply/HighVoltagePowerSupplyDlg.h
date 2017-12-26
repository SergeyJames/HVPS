
// HighVoltagePowerSupplyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <string>
#include <vector>
#include <minwindef.h>
#include <winperf.h>

const unsigned long AcceleratorMin = 0;
const unsigned long AcceleratorMax = 30000;

/// this defination for GetComPortList() function
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383


// CHighVoltagePowerSupplyDlg dialog
class CHighVoltagePowerSupplyDlg : public CDialogEx
{
// Construction
public:
	CHighVoltagePowerSupplyDlg(CWnd* pParent = nullptr);	// standard constructor

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

private: // Other Variables

	CComboBox m_ComPortCmbBox;
	CEdit m_InternalTempEdit;
	CEdit m_PowerTempEdit;
	CEdit m_HumidityEdit;

	CButton m_LogToFileButton;
	CButton m_EnableButton;

	static bool m_bIsComPortListEmpty;

public: // Accelerator Methodes
	afx_msg void OnDeltaposSpinVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult);

private: // Accelerator Variables

	CEdit m_VoltageToSetSpin;
	CEdit m_VoltageToSetKeyboard;
	CSliderCtrl m_SliderVoltageToSet;
	CSpinButtonCtrl m_VoltageToSetSpinCtrl;


	unsigned long m_ulSliderVoltageToSetPos; // The slider position
	std::wstring m_strSliderVoltageToSetPos;	// The slider position (std::string)
	std::vector<std::wstring> m_ComPortVec;

private: // BIAS Variables

	CEdit m_VoltageToSetSpinBias;
	CEdit m_VoltageToSetKeyboardBias;
	CSliderCtrl m_SliderVoltageToSetBias;
	CSpinButtonCtrl m_VoltageToSetSpinCtrlBias;
	static int m_SpinCtrlBiasPos;

	double m_dValue;

	double m_ulSliderVoltageToSetPosBias; // The slider position
	std::wstring m_strSliderVoltageToSetPosBias;	// The slider position (std::string)
	
private: // FLAMENT Variables

	CEdit m_VoltageToSetSpinFlament;
	CEdit m_VoltageToSetKeyboardFlament;
	CSliderCtrl m_SliderVoltageToSetFlament;
	CSpinButtonCtrl m_VoltageToSetSpinCtrlFlament;


private:
	void SetAllSpinCtrlRanges();
	void SetAllSliderRanges();
	void FillComPortList();
	void FillComPortComboBox();
	void RemoveZeros(std::wstring& a_wstr);
public:
	afx_msg void OnBnClickedButtonUpdateCom();
	afx_msg void OnEnChangeEditVoltageToSetKeybordBias();
	afx_msg void OnDeltaposSpinVoltageToSetBias(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderVoltageToSetBias(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditVoltageToSetKeybord();
};
