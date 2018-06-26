// HighVoltagePowerSupplyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HighVoltagePowerSupply.h"
#include "HighVoltagePowerSupplyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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


// CHighVoltagePowerSupplyDlg dialog

bool CHighVoltagePowerSupplyDlg::m_bIsComPortListEmpty = { true };
int  CHighVoltagePowerSupplyDlg::m_SpinCtrlBiasPos = 0;
int  CHighVoltagePowerSupplyDlg::m_SpinCtrlFlamentPos = 0;


CHighVoltagePowerSupplyDlg::CHighVoltagePowerSupplyDlg(CWnd* pParent /* nullptr */)	: 
	CDialogEx(IDD_HIGHVOLTAGEPOWERSUPPLY_DIALOG, pParent),
	m_ulSliderVoltageToSetPos{ MinMax::g_AcceleratorMin },
	m_strSliderVoltageToSetPos { std::to_wstring(m_ulSliderVoltageToSetPos) },
	m_dValue{ 0.0 },
	m_dSliderVoltageToSetPosBias{ MinMax::g_BiasMin },
	m_dSliderVoltageToSetPosFlament{ MinMax::g_FlamentMin },
	m_bIsAcceleratorEnabled{ false }, // false - отключен
	m_bIsBIASEnabled{ false },
	m_bIsFlamentEnabled{ false },
	m_nCurComPortIndex{ -1 }, // -1 - ничего не выбрано
	m_ChSend{ {} },
	m_bIsConnectedToCom{ false },
	m_hIcon{ nullptr }
{

	m_ChSend[0] = 85;  // 85 - 0x55 in hex
	m_ChSend[1] = 26;


	FillComPortList();
	m_bIsComPortListEmpty = m_ComPortVec.empty();

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pTips = nullptr;

	m_vecRecvData.resize(33);
	m_vecSendData.resize(33);
}

void CHighVoltagePowerSupplyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_LogToFileButton);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_SPIN, m_VoltageToSetSpin);
	DDX_Control(pDX, IDC_SPIN_VOLTAGE_TO_SET, m_VoltageToSetSpinCtrl);
	DDX_Control(pDX, IDC_SLIDER_VOLTAGE_TO_SET, m_SliderVoltageToSet);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_KEYBORD, m_VoltageToSetKeyboard);
	DDX_Control(pDX, IDC_COMBO_COM_PORT, m_ComPortCmbBox);
	DDX_Control(pDX, IDC_EDIT_INTERNAL_DATA, m_InternalTempEdit);
	DDX_Control(pDX, IDC_EDIT_POWER_TEMP, m_PowerTempEdit);
	DDX_Control(pDX, IDC_EDIT_Humidity, m_HumidityEdit);
	DDX_Control(pDX, IDC_SLIDER_VOLTAGE_TO_SET_BIAS, m_SliderVoltageToSetBias);
	DDX_Control(pDX, IDC_SLIDER_VOLTAGE_TO_SET_BIAS2, m_SliderVoltageToSetFlament);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_KEYBORD_BIAS, m_VoltageToSetKeyboardBias);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_SPIN_BIAS, m_VoltageToSetSpinBias);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_SPIN_BIAS2, m_VoltageToSetSpinFlament);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_KEYBORD_BIAS2, m_VoltageToSetKeyboardFlament);
	DDX_Control(pDX, IDC_SPIN_VOLTAGE_TO_SET_BIAS, m_VoltageToSetSpinCtrlBias);
	DDX_Control(pDX, IDC_SPIN_VOLTAGE_TO_SET_BIAS2, m_VoltageToSetSpinCtrlFlament);
	DDX_Control(pDX, IDC_BUTTON_ENABLE, m_ButtonEnableAccelerator);
	DDX_Control(pDX, IDC_BUTTON_ENABLE_BIAS, m_ButtonEnableBias);
	DDX_Control(pDX, IDC_BUTTON_ENABLE_FLAMENT, m_ButtonEnableFlament);
}

BEGIN_MESSAGE_MAP(CHighVoltagePowerSupplyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHighVoltagePowerSupplyDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VOLTAGE_TO_SET, &CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSet)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VOLTAGE_TO_SET, &CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSet)
	ON_EN_CHANGE(IDC_EDIT_VOLTAGE_TO_SET_KEYBORD, &CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybord)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_COM, &CHighVoltagePowerSupplyDlg::OnBnClickedButtonUpdateCom)
	ON_EN_CHANGE(IDC_EDIT_VOLTAGE_TO_SET_KEYBORD_BIAS, &CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybordBias)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VOLTAGE_TO_SET_BIAS, &CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSetBias)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VOLTAGE_TO_SET_BIAS, &CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSetBias)
	ON_EN_CHANGE(IDC_EDIT_VOLTAGE_TO_SET_KEYBORD_BIAS2, &CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybordBias2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VOLTAGE_TO_SET_BIAS2, &CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSetBias2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VOLTAGE_TO_SET_FLAMENT, &CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSetFlament)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE, &CHighVoltagePowerSupplyDlg::OnBnClickedButtonEnable)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE_BIAS, &CHighVoltagePowerSupplyDlg::OnBnClickedButtonEnableBias)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE_FLAMENT, &CHighVoltagePowerSupplyDlg::OnBnClickedButtonEnableFlament)
	ON_CBN_SELENDOK(IDC_COMBO_COM_PORT, &CHighVoltagePowerSupplyDlg::OnCbnSelendokComboComPort)
END_MESSAGE_MAP()


// CHighVoltagePowerSupplyDlg message handlers

BOOL CHighVoltagePowerSupplyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	

	m_pTips = new CToolTipCtrl;
	m_pTips->Create(this);
	m_pTips->AddTool(&m_VoltageToSetKeyboard, L"Button 1");


	SetAllSpinCtrlRanges();
	SetAllSliderRanges();
	FillComPortComboBox();
	m_VoltageToSetSpinBias.SetWindowTextW(L"0.0");


	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	SetUpAcceleratorSection(std::wstring(L"Enable"), RGB(47, 128, 55), FALSE);
	SetUpBIASSection(std::wstring(L"Enable"), RGB(47, 128, 55), FALSE);
	SetUpFilamentSection(std::wstring(L"Enable"), RGB(47, 128, 55), FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHighVoltagePowerSupplyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHighVoltagePowerSupplyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHighVoltagePowerSupplyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHighVoltagePowerSupplyDlg::OnBnClickedOk()
{
	std::string strFilter ="Text Files (*.txt)|*.txt|";
	CString FileName;
	CString FilePath;
	CString FullPath;

	CFileDialog FileDlg(FALSE, CString(".txt"), NULL, 0, CString(strFilter.c_str()));

	if (FileDlg.DoModal() == IDOK) {
		FileName = FileDlg.GetFileName(); //filename
		FilePath = FileDlg.GetFolderPath(); //filepath (folders)
		FullPath = FilePath + "\\" + FileName;
		/*
		TODO:
		.
		.
		.
		.
		*/
	}
}

void CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	SetUpSpinCtrl(pNMUpDown, m_SliderVoltageToSet, m_VoltageToSetKeyboard, m_VoltageToSetSpinCtrl);
}

void CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_ulSliderVoltageToSetPos = m_SliderVoltageToSet.GetPos();
	m_strSliderVoltageToSetPos = std::to_wstring(m_ulSliderVoltageToSetPos);

	m_VoltageToSetKeyboard.SetWindowTextW(m_strSliderVoltageToSetPos.c_str());
	m_VoltageToSetSpin.SetWindowTextW(m_strSliderVoltageToSetPos.c_str());

	InsertVoltageValueToSendArray(m_ulSliderVoltageToSetPos);
}

void CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybord()
{
	SetUpSliderPosInEditBox(m_SliderVoltageToSet, m_VoltageToSetKeyboard);
}

void CHighVoltagePowerSupplyDlg::SetAllSpinCtrlRanges()
{
	// Accelerator
	m_VoltageToSetSpinCtrl.SetRange(MinMax::g_AcceleratorMin, MinMax::g_AcceleratorMax);
	m_VoltageToSetSpinCtrl.SetPos(MinMax::g_AcceleratorMin);

	// BIAS 
	m_VoltageToSetSpinCtrlBias.SetRange(MinMax::g_BiasMin, MinMax::g_BiasMax);
	m_VoltageToSetSpinCtrlBias.SetPos(MinMax::g_BiasMin);

	// FLAMENT
	m_VoltageToSetSpinCtrlFlament.SetRange(MinMax::g_FlamentMin, MinMax::g_FlamentMax);
	m_VoltageToSetSpinCtrlFlament.SetPos(MinMax::g_FlamentMin);
}

void CHighVoltagePowerSupplyDlg::SetAllSliderRanges()
{
	// Accelerator
	m_SliderVoltageToSet.SetRange(MinMax::g_AcceleratorMin, MinMax::g_AcceleratorMax);

	// BIAS 
	m_SliderVoltageToSetBias.SetRange(MinMax::g_BiasMin, MinMax::g_BiasMax);

	// FLAMENT
	m_SliderVoltageToSetFlament.SetRange(MinMax::g_FlamentMin, MinMax::g_FlamentMax);

}

void CHighVoltagePowerSupplyDlg::FillComPortComboBox()
{
	if (!m_ComPortVec.empty()) {
		std::vector<std::wstring>::const_iterator it = m_ComPortVec.begin();
		for (; it != m_ComPortVec.end(); ++it) {
			m_ComPortCmbBox.AddString((*it).c_str());
		}
	}
	else {
		m_ComPortCmbBox.AddString(LR"(empty)");
	}
}

void CHighVoltagePowerSupplyDlg::RemoveZeros(std::wstring& a_wstr)
{
	for (size_t i = 0; i < 5; ++i) {
		a_wstr.pop_back();
	}
}

void CHighVoltagePowerSupplyDlg::SetUpSlider(double a_dPos, CSliderCtrl & a_SliderCtrl, std::wstring& a_wstrSliderPos, CEdit & a_Edit, CEdit& a_EditSpin)
{
	a_dPos = a_SliderCtrl.GetPos();
	std::wstring tmp = std::to_wstring(a_dPos);
	RemoveZeros(tmp);
	a_wstrSliderPos = tmp;

	a_Edit.SetWindowTextW(a_wstrSliderPos.c_str());
	a_EditSpin.SetWindowTextW(a_wstrSliderPos.c_str());
}

void CHighVoltagePowerSupplyDlg::SetUpSliderPosInEditBox(CSliderCtrl & a_SliderCtrl, CEdit& a_Edit)
{
	CString text;
	a_Edit.GetWindowTextW(text);
	std::wstring wstext = text;
	long lPos = 0;

	if (!wstext.empty()) {
		try {
			lPos = std::stol(wstext);
		}
		catch (...) {}

		a_SliderCtrl.SetPos(lPos);
		InsertVoltageValueToSendArray(lPos);
	}
}

void CHighVoltagePowerSupplyDlg::SetUpSpinCtrl(LPNMUPDOWN pNMUpDown, CSliderCtrl& a_SliderCtrl, CEdit& a_Edit, CSpinButtonCtrl& a_SpinButtonCtrl)
{
	std::wstring tmp;
	if (pNMUpDown->iDelta < 0) {
		if (pNMUpDown->iPos == 0) {
			tmp = std::to_wstring(0);
			a_Edit.SetWindowTextW(tmp.c_str());
			a_SliderCtrl.SetPos(a_SpinButtonCtrl.GetPos());
		}
		else {
			tmp = std::to_wstring(a_SpinButtonCtrl.GetPos() - 1);
			a_Edit.SetWindowTextW(tmp.c_str());
			a_SliderCtrl.SetPos(a_SpinButtonCtrl.GetPos() - 1);
		}
	}
	else {
		tmp = std::to_wstring(a_SpinButtonCtrl.GetPos() + 1);
		a_Edit.SetWindowTextW(tmp.c_str());
		a_SliderCtrl.SetPos(a_SpinButtonCtrl.GetPos() + 1);
	}

	pNMUpDown->iPos = std::stol(tmp);
	InsertVoltageValueToSendArray(pNMUpDown->iPos);
}

void CHighVoltagePowerSupplyDlg::SetUpAcceleratorSection(std::wstring&& a_wsTooltip, const COLORREF& a_color, BOOL a_bState)
{
	m_ButtonEnableAccelerator.SetTooltip(a_wsTooltip.c_str());
	m_ButtonEnableAccelerator.SetTextColor(a_color);
	m_ButtonEnableAccelerator.EnableWindow(a_bState);
	m_VoltageToSetKeyboard.EnableWindow(a_bState);
	m_VoltageToSetSpinCtrl.EnableWindow(a_bState);
	m_SliderVoltageToSet.EnableWindow(a_bState);
}

void CHighVoltagePowerSupplyDlg::SetUpBIASSection(std::wstring&& a_wsTooltip, const COLORREF& a_color, BOOL a_bState)
{
	m_ButtonEnableBias.SetTooltip(a_wsTooltip.c_str());
	m_ButtonEnableBias.SetTextColor(a_color);
	m_ButtonEnableBias.EnableWindow(a_bState);
	m_VoltageToSetKeyboardBias.EnableWindow(a_bState);
	m_VoltageToSetSpinCtrlBias.EnableWindow(a_bState);
	m_SliderVoltageToSetBias.EnableWindow(a_bState);

}

void CHighVoltagePowerSupplyDlg::SetUpFilamentSection(std::wstring&& a_wsTooltip, const COLORREF& a_color, BOOL a_bState)
{
	m_ButtonEnableFlament.SetTooltip(a_wsTooltip.c_str());
	m_ButtonEnableFlament.SetTextColor(a_color);
	m_ButtonEnableFlament.EnableWindow(a_bState);
	m_VoltageToSetKeyboardFlament.EnableWindow(a_bState);
	m_VoltageToSetSpinCtrlFlament.EnableWindow(a_bState);
	m_SliderVoltageToSetFlament.EnableWindow(a_bState);
}

void CHighVoltagePowerSupplyDlg::SetButtonTextAndColor(CMFCButton& a_button, bool& a_bCurState)
{
	if (a_bCurState) {
		a_bCurState = false;
		a_button.SetWindowTextW(L"Enable");
		a_button.SetTooltip(_T("Enable!"));
		a_button.SetTextColor(RGB(47, 128, 55));
	}
	else {
		a_bCurState = true;
		a_button.SetWindowTextW(L"Diasble");
		a_button.SetTooltip(_T("Diasble!"));
		a_button.SetTextColor(RGB(255, 0, 0));
	}
}

void CHighVoltagePowerSupplyDlg::InsertVoltageValueToSendArray(unsigned long a_ul)
{
	std::bitset<16> bsVoltageValue(a_ul);
	std::bitset<8> bs0;
	std::bitset<8> bs1;

	for (size_t i = 0, j = 8; i < 8; ++i, ++j) {
		bs0.operator[](i) = bsVoltageValue.operator[](i);
		bs1.operator[](i) = bsVoltageValue.operator[](j);
	}

	// Внимание, элементы в bitset хранятся в обратном порядке, по этому пишем так 
	m_ChSend[4] = bs0.to_ulong();
	m_ChSend[3] = bs1.to_ulong();

	if (m_bIsConnectedToCom) {
		m_ComPort.WriteDate(m_ChSend, sizeof(m_ChSend));
	}
}

void CHighVoltagePowerSupplyDlg::ResetSections()
{
	SetUpAcceleratorSection(std::wstring(L"Enable"), RGB(47, 128, 55), FALSE);
	SetUpBIASSection(std::wstring(L"Enable"), RGB(47, 128, 55), FALSE);
	SetUpFilamentSection(std::wstring(L"Enable"), RGB(47, 128, 55), FALSE);
}

void CHighVoltagePowerSupplyDlg::FillComPortList()
{
	m_ComPortVec = m_ComPort.GetComPortList();
}


void CHighVoltagePowerSupplyDlg::OnBnClickedButtonUpdateCom()
{
	m_ComPortCmbBox.ResetContent();
	ResetSections();
	// Обязательно 
	m_ComPortVec.clear();
	m_ComPortVec.shrink_to_fit();

	FillComPortList();
	FillComPortComboBox();
}


void CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybordBias()
{
	SetUpSliderPosInEditBox(m_SliderVoltageToSetBias, m_VoltageToSetKeyboardBias);

	/*CString text;
	m_VoltageToSetKeyboardBias.GetWindowTextW(text);
	std::wstring wstext = text;
	double dValue = 0.0;


	try	{
		if (!wstext.empty()) {
			dValue = boost::lexical_cast<double>(wstext);
		}
	}
	catch (...)
	{
		MessageBoxA(this->m_hWnd, "Please type Integer or Float values ( 0.0 <= value <= 10.0), charachters not allowed", "", 48);
		SendMessageA(m_VoltageToSetKeyboardBias, WM_SETTEXT, WPARAM(0), LPARAM(""));
	}

	// temp min max ( 0.0 and 10.0 )
	if (dValue <= 0.0) {
		m_SliderVoltageToSetBias.SetPos(0);
		m_VoltageToSetSpinBias.SetWindowTextW(L"0");;
		m_dValue = 0.0;
	}
	else if (dValue > 10.0) {
		m_SliderVoltageToSetBias.SetPos(100);
		m_VoltageToSetSpinBias.SetWindowTextW(L"10.0");;
		m_dValue = 10.0;
		SendMessageA(m_VoltageToSetKeyboardBias, WM_SETTEXT, WPARAM(0), LPARAM("10.0"));
	}
	else if (dValue >= 0.0 && dValue <= 10.0) {
		m_SliderVoltageToSetBias.SetPos(dValue * 10);
		m_dValue = dValue;
		std::wstring tmp = std::to_wstring(dValue);
		RemoveZeros(tmp);
		m_VoltageToSetSpinBias.SetWindowTextW(tmp.c_str());
	}*/
}

void CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSetBias(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	static float value = 0.0;
	float d = 0.0;
	std::wstring tmp;

	if (pNMUpDown->iDelta < 0) {
		value -= 0.1;
	}
	else {
		value += 0.1;
	}

	if (value <= MinMax::g_BiasMin) {
		d = 0.0;
	}
	else if (value >= MinMax::g_BiasMax) {
		d = MinMax::g_BiasMax;
	}
	else {
		d = value;
	}

	tmp = std::to_wstring(d);
	RemoveZeros(tmp);
	m_VoltageToSetSpinBias.SetWindowTextW(tmp.c_str());
	pNMUpDown->iPos;
	//m_SliderVoltageToSetBias.SetPos(pNMUpDown->iPos);
	m_VoltageToSetKeyboardBias.SetWindowTextW(tmp.c_str());





	/*if (pNMUpDown->iDelta < 0) {
		if (pNMUpDown->iPos == 0) {
			tmp = std::to_wstring(0.0);
			m_VoltageToSetSpinBias.SetWindowTextW(L"0.0");
			m_SliderVoltageToSetBias.SetPos(0);
		}
		else {
			m_SpinCtrlBiasPos = pNMUpDown->iPos - 1;
			d = m_SpinCtrlBiasPos;
			tmp = std::to_wstring(d);
			RemoveZeros(tmp);
			m_VoltageToSetSpinBias.SetWindowTextW(tmp.c_str());
			m_SliderVoltageToSetBias.SetPos(m_SpinCtrlBiasPos - 1);
			m_VoltageToSetKeyboardBias.SetWindowTextW(tmp.c_str());
		}
	}
	else {
		if (pNMUpDown->iPos == 100) {
			m_SpinCtrlBiasPos = pNMUpDown->iPos;
		}
		else if (pNMUpDown->iPos < 100) {
			m_SpinCtrlBiasPos = pNMUpDown->iPos + 1;
		}

		d = m_SpinCtrlBiasPos;
		tmp = std::to_wstring(d);
		RemoveZeros(tmp);
		m_VoltageToSetSpinBias.SetWindowTextW(tmp.c_str());
		m_SliderVoltageToSetBias.SetPos(m_SpinCtrlBiasPos + 1);
		m_VoltageToSetKeyboardBias.SetWindowTextW(tmp.c_str());
	}*/
}


void CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSetBias(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	SetUpSlider(m_dSliderVoltageToSetPosBias,
		m_SliderVoltageToSetBias,
		m_strSliderVoltageToSetPosBias,
		m_VoltageToSetKeyboardBias, m_VoltageToSetSpinBias);
}


void CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybordBias2()
{
	SetUpSliderPosInEditBox(m_SliderVoltageToSetFlament, m_VoltageToSetKeyboardFlament);
}


BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CHighVoltagePowerSupplyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_pTips)
		m_pTips->RelayEvent(pMsg);


	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			return TRUE;                // Do not process further
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSetBias2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	SetUpSlider(m_dSliderVoltageToSetPosFlament, m_SliderVoltageToSetFlament, m_strSliderVoltageToSetPosFlament, m_VoltageToSetKeyboardFlament, m_VoltageToSetSpinFlament);
}


void CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSetFlament(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	float d = 0.0;
	std::wstring tmp;

	if (pNMUpDown->iDelta < 0) {
		if (pNMUpDown->iPos == 0) {
			tmp = std::to_wstring(0.0);
			m_VoltageToSetSpinFlament.SetWindowTextW(L"0.0");
			m_SliderVoltageToSetFlament.SetPos(0);
		}
		else {
			m_SpinCtrlFlamentPos = pNMUpDown->iPos - 1;
			d = m_SpinCtrlFlamentPos;
			tmp = std::to_wstring(d);
			RemoveZeros(tmp);
			m_VoltageToSetSpinFlament.SetWindowTextW(tmp.c_str());
			m_SliderVoltageToSetFlament.SetPos(m_SpinCtrlFlamentPos - 1);
			m_VoltageToSetKeyboardFlament.SetWindowTextW(tmp.c_str());
		}
	}
	else {
		if (pNMUpDown->iPos == 10) {
			m_SpinCtrlFlamentPos = pNMUpDown->iPos;
		}
		else if (pNMUpDown->iPos < 10) {
			m_SpinCtrlFlamentPos = pNMUpDown->iPos + 1;
		}

		d = m_SpinCtrlFlamentPos;
		tmp = std::to_wstring(d);
		RemoveZeros(tmp);
		m_VoltageToSetSpinFlament.SetWindowTextW(tmp.c_str());
		m_SliderVoltageToSetFlament.SetPos(m_SpinCtrlFlamentPos + 1);
		m_VoltageToSetKeyboardFlament.SetWindowTextW(tmp.c_str());
	}
}


void CHighVoltagePowerSupplyDlg::OnBnClickedButtonEnable()
{
	SetButtonTextAndColor(m_ButtonEnableAccelerator, m_bIsAcceleratorEnabled);


	

	std::bitset<8> bs(m_ChSend[2]);
	if (m_bIsAcceleratorEnabled) {
		bs.operator[](7) = 1;
	}
	else {
		bs.operator[](7) = 0;
	}

	m_ChSend[2] = bs.to_ulong();


	m_ComPort.WriteDate(m_ChSend, sizeof(m_ChSend));
}


void CHighVoltagePowerSupplyDlg::OnBnClickedButtonEnableBias()
{
	SetButtonTextAndColor(m_ButtonEnableBias, m_bIsBIASEnabled);

	std::bitset<8> bs(m_ChSend[2]);
	if (m_bIsBIASEnabled) {
		bs.operator[](6) = 1;
	}
	else {
		bs.operator[](6) = 0;
	}

	m_ChSend[2] = bs.to_ulong();

	m_ComPort.WriteDate(m_ChSend, sizeof(m_ChSend));
}


void CHighVoltagePowerSupplyDlg::OnBnClickedButtonEnableFlament()
{
	SetButtonTextAndColor(m_ButtonEnableFlament, m_bIsFlamentEnabled);

	std::bitset<8> bs(m_ChSend[2]);
	if (m_bIsBIASEnabled) {
		bs.operator[](5) = 1;
	}
	else {
		bs.operator[](5) = 0;
	}

	m_ChSend[2] = bs.to_ulong();

	m_ComPort.WriteDate(m_ChSend, sizeof(m_ChSend));
}


void CHighVoltagePowerSupplyDlg::OnCbnSelendokComboComPort()
{
	m_nCurComPortIndex = m_ComPortCmbBox.GetCurSel();
	if (m_nCurComPortIndex < m_ComPortVec.size() && m_nCurComPortIndex > -1) {
		std::wstring data = m_ComPortVec.at(m_nCurComPortIndex);
		m_bIsConnectedToCom = m_ComPort.Connect(data.c_str()); //После того, как имя COM порта получено, соединяемся
		
		SetUpAcceleratorSection(std::wstring(L"Enable"), RGB(47, 128, 55), TRUE);	
		SetUpBIASSection(std::wstring(L"Enable"), RGB(47, 128, 55), TRUE);
		SetUpFilamentSection(std::wstring(L"Enable"), RGB(47, 128, 55), TRUE);
	}	
}
