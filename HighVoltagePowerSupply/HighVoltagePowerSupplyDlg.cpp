
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



CHighVoltagePowerSupplyDlg::CHighVoltagePowerSupplyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HIGHVOLTAGEPOWERSUPPLY_DIALOG, pParent),
	m_ulSliderVoltageToSetPos{ Min },
	m_strSliderVoltageToSetPos { std::to_wstring(m_ulSliderVoltageToSetPos) }
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHighVoltagePowerSupplyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_LogToFileButton);
	DDX_Control(pDX, IDC_BUTTON_ENABLE, m_EnableButton);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_SPIN, m_VoltageToSetSpin);
	DDX_Control(pDX, IDC_SPIN_VOLTAGE_TO_SET, m_VoltageToSetSpinCtrl);
	DDX_Control(pDX, IDC_SLIDER_VOLTAGE_TO_SET, m_SliderVoltageToSet);
	DDX_Control(pDX, IDC_EDIT_VOLTAGE_TO_SET_KEYBORD, m_VoltageToSetKeyboard);
}

BEGIN_MESSAGE_MAP(CHighVoltagePowerSupplyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHighVoltagePowerSupplyDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VOLTAGE_TO_SET, &CHighVoltagePowerSupplyDlg::OnDeltaposSpinVoltageToSet)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VOLTAGE_TO_SET, &CHighVoltagePowerSupplyDlg::OnNMCustomdrawSliderVoltageToSet)
	ON_EN_CHANGE(IDC_EDIT_VOLTAGE_TO_SET_KEYBORD, &CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybord)
END_MESSAGE_MAP()


// CHighVoltagePowerSupplyDlg message handlers

BOOL CHighVoltagePowerSupplyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_VoltageToSetSpinCtrl.SetRange(Min, Max);
	m_VoltageToSetSpinCtrl.SetPos(Min);

	m_SliderVoltageToSet.SetRange(Min, Max);
	

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

	// TODO: Add extra initialization here

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

	std::wstring tmp;
	if (pNMUpDown->iDelta < 0) {
		if (pNMUpDown->iPos == 0) {
			tmp = std::to_wstring(0);
			m_VoltageToSetKeyboard.SetWindowTextW(tmp.c_str());
			m_SliderVoltageToSet.SetPos(m_VoltageToSetSpinCtrl.GetPos());
		}
		else {
			tmp = std::to_wstring(m_VoltageToSetSpinCtrl.GetPos() - 1);
			m_VoltageToSetKeyboard.SetWindowTextW(tmp.c_str());
			m_SliderVoltageToSet.SetPos(m_VoltageToSetSpinCtrl.GetPos() - 1);
		}
	}
	else {
		tmp = std::to_wstring(m_VoltageToSetSpinCtrl.GetPos() + 1);
		m_VoltageToSetKeyboard.SetWindowTextW(tmp.c_str());
		m_SliderVoltageToSet.SetPos(m_VoltageToSetSpinCtrl.GetPos() + 1);
	}

	
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
}


void CHighVoltagePowerSupplyDlg::OnEnChangeEditVoltageToSetKeybord()
{
	CString text;
	m_VoltageToSetKeyboard.GetWindowTextW(text);
	std::wstring wstext = text;

	if (!wstext.empty()) {
		m_SliderVoltageToSet.SetPos(std::stol(wstext));
	}

	
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
