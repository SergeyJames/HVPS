
// HighVoltagePowerSupplyDlg.h : header file
//

#pragma once

namespace MinMax {
	// ACCELERATOR 
	const unsigned long g_AcceleratorMin = 0;
	const unsigned long g_AcceleratorMax = 30000;

	// BIAS 
	const double g_BiasMin = 0.0;
	const double g_BiasMax = 6000.0;

	// FLAMENT
	const double g_FlamentMin = 0.0;
	const double g_FlamentMax = 10.0;

	const short g_iInternalTempMin = 0;
	const short g_iInternalTempMax = 255;

	const short g_iHumidityMin = 0;
	const short g_iHumidityMax = 100;
}

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
	std::vector<std::bitset<8>> m_vecRecvData;
	std::vector<std::bitset<8>> m_vecSendData;


	CComboBox m_ComPortCmbBox;
	CEdit m_InternalTempEdit;
	CEdit m_PowerTempEdit;
	CEdit m_HumidityEdit;

	CButton m_LogToFileButton;

	static bool m_bIsComPortListEmpty;

public: // Accelerator Methodes
	afx_msg void OnDeltaposSpinVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult);

private: // Accelerator Variables

	CMFCButton m_ButtonEnableAccelerator;
	
	CEdit m_VoltageToSetSpin;
	CEdit m_VoltageToSetKeyboard;
	CSliderCtrl m_SliderVoltageToSet;
	CSpinButtonCtrl m_VoltageToSetSpinCtrl;


	unsigned long m_ulSliderVoltageToSetPos; // The slider position
	std::wstring m_strSliderVoltageToSetPos;	// The slider position (std::string)
	std::vector<std::wstring> m_ComPortVec;

private: // BIAS Variables

	CMFCButton m_ButtonEnableBias;

	CEdit m_VoltageToSetSpinBias;
	CEdit m_VoltageToSetKeyboardBias;
	CSliderCtrl m_SliderVoltageToSetBias;
	CSpinButtonCtrl m_VoltageToSetSpinCtrlBias;
	static int m_SpinCtrlBiasPos;

	double m_dValue;

	double m_dSliderVoltageToSetPosBias; // The slider position
	std::wstring m_strSliderVoltageToSetPosBias;	// The slider position (std::string)
	
private: // FLAMENT Variables

	CMFCButton m_ButtonEnableFlament;

	CEdit m_VoltageToSetSpinFlament;
	CEdit m_VoltageToSetKeyboardFlament;
	CSliderCtrl m_SliderVoltageToSetFlament;
	CSpinButtonCtrl m_VoltageToSetSpinCtrlFlament;
	static int m_SpinCtrlFlamentPos;


	CToolTipCtrl* m_pTips;

	double m_dSliderVoltageToSetPosFlament; // The slider position
	std::wstring m_strSliderVoltageToSetPosFlament;	// The slider position (std::string)

private:
	void SetAllSpinCtrlRanges();
	void SetAllSliderRanges();
	void FillComPortList();
	void FillComPortComboBox();
	void RemoveZeros(std::wstring& a_wstr);
	void SetUpSlider(double a_dPos, CSliderCtrl& a_SliderCtrl, std::wstring& a_wstrSliderPos, CEdit& a_Edit, CEdit& a_EditSpin);
	void SetUpSliderPosInEditBox(CSliderCtrl& a_SliderCtrl, CEdit& a_Edit);
	void SetUpSpinCtrl(LPNMUPDOWN pNMUpDown, CSliderCtrl& a_SliderCtrl, CEdit& a_Edit, CSpinButtonCtrl& a_SpinButtonCtrl);

public:
	afx_msg void OnBnClickedButtonUpdateCom();
	afx_msg void OnEnChangeEditVoltageToSetKeybordBias();
	afx_msg void OnDeltaposSpinVoltageToSetBias(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderVoltageToSetBias(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditVoltageToSetKeybord();
	afx_msg void OnEnChangeEditVoltageToSetKeybordBias2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMCustomdrawSliderVoltageToSetBias2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinVoltageToSetFlament(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonEnable();
	afx_msg void OnBnClickedButtonEnableBias();
	afx_msg void OnBnClickedButtonEnableFlament();
	afx_msg void OnCbnSelendokComboComPort();
};
